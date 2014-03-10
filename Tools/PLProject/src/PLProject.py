#!/usr/bin/python


# [TODO] Add verbose command line option
# [TODO] Write the current PixelLight version into the XML plugin file
# [TODO] Change "PLProject V1.0 (Python)" into "PLProject <PixelLight version> (Python)" (e.g. "PLProject PixelLight 1.0.1-R1 (Python)")
# [TODO] Add "Delayed" option within the resulting XML plugin file (currently always set to "0")
# [TODO] Add RTTI class metadata within the resulting XML plugin file


import sys
import os
#import io
import getopt
import re
import glob

# Setup regular expressions
reModule				= re.compile(r"^\s*pl_module\(\s*\"(?P<name>\w*)\"\s*\)\s*$")
reModulePlugin			= re.compile(r"^\s*pl_module_plugin\(\s*\"(?P<name>\w*)\"\s*\)\s*$")
reVendor				= re.compile(r"^\s*pl_module_vendor\s*\(\s*(?P<text>\".*\")\s*\)\s*$", re.IGNORECASE)
reLicense				= re.compile(r"^\s*pl_module_license\s*\(\s*(?P<text>\".*\")\s*\)\s*$", re.IGNORECASE)
reDescription			= re.compile(r"^\s*pl_module_description\s*\(\s*(?P<text>\".*\")\s*\)\s*$", re.IGNORECASE)
reVersion				= re.compile(r"^\s*pl_module_version\s*\(\s*(?P<text>\".*\")\s*\)\s*$", re.IGNORECASE)
reActive				= re.compile(r"^\s*pl_module_active\s*\((?P<num>\d)\)\s*$")
# Windows
reDepsWindows32Release	= re.compile(r"^\s*pl_module_dependencies_windows_32_release\s*\(\s*(?P<text>\".*\")\s*\)\s*$", re.IGNORECASE)
reDepsWindows32Debug	= re.compile(r"^\s*pl_module_dependencies_windows_32_debug\s*\(\s*(?P<text>\".*\")\s*\)\s*$", re.IGNORECASE)
reDepsWindows64Release	= re.compile(r"^\s*pl_module_dependencies_windows_64_release\s*\(\s*(?P<text>\".*\")\s*\)\s*$", re.IGNORECASE)
reDepsWindows64Debug	= re.compile(r"^\s*pl_module_dependencies_windows_64_debug\s*\(\s*(?P<text>\".*\")\s*\)\s*$", re.IGNORECASE)
# Linux
reDepsLinux32Release	= re.compile(r"^\s*pl_module_dependencies_linux_32_release\s*\(\s*(?P<text>\".*\")\s*\)\s*$", re.IGNORECASE)
reDepsLinux32Debug		= re.compile(r"^\s*pl_module_dependencies_linux_32_debug\s*\(\s*(?P<text>\".*\")\s*\)\s*$", re.IGNORECASE)
reDepsLinux64Release	= re.compile(r"^\s*pl_module_dependencies_linux_64_release\s*\(\s*(?P<text>\".*\")\s*\)\s*$", re.IGNORECASE)
reDepsLinux64Debug		= re.compile(r"^\s*pl_module_dependencies_linux_64_debug\s*\(\s*(?P<text>\".*\")\s*\)\s*$", re.IGNORECASE)
# Android
reDepsAndroid32Release	= re.compile(r"^\s*pl_module_dependencies_android_32_release\s*\(\s*(?P<text>\".*\")\s*\)\s*$", re.IGNORECASE)
reDepsAndroid32Debug	= re.compile(r"^\s*pl_module_dependencies_android_32_debug\s*\(\s*(?P<text>\".*\")\s*\)\s*$", re.IGNORECASE)
reDepsAndroid64Release	= re.compile(r"^\s*pl_module_dependencies_android_64_release\s*\(\s*(?P<text>\".*\")\s*\)\s*$", re.IGNORECASE)
reDepsAndroid64Debug	= re.compile(r"^\s*pl_module_dependencies_android_64_debug\s*\(\s*(?P<text>\".*\")\s*\)\s*$", re.IGNORECASE)
# Mac OS X
reDepsMacOSX32Release	= re.compile(r"^\s*pl_module_dependencies_macosx_32_release\s*\(\s*(?P<text>\".*\")\s*\)\s*$", re.IGNORECASE)
reDepsMacOSX32Debug		= re.compile(r"^\s*pl_module_dependencies_macosx_32_debug\s*\(\s*(?P<text>\".*\")\s*\)\s*$", re.IGNORECASE)
reDepsMacOSX64Release	= re.compile(r"^\s*pl_module_dependencies_macosx_64_release\s*\(\s*(?P<text>\".*\")\s*\)\s*$", re.IGNORECASE)
reDepsMacOSX64Debug		= re.compile(r"^\s*pl_module_dependencies_macosx_64_debug\s*\(\s*(?P<text>\".*\")\s*\)\s*$", re.IGNORECASE)

# Project data type
class Project(object):
	def __init__(self):
		self.path					= ""
		self.name					= ""
		self.suffix					= ""
		self.moduleName				= ""
		self.moduleVersion			= ""
		self.moduleVendor			= ""
		self.moduleLicense			= ""
		self.moduleDescription		= ""
		self.modulePlugin			= False
		self.moduleActive			= False
		self.libWindowsRelease		= ""
		self.libWindowsDebug		= ""
		self.libLinuxRelease		= ""
		self.libLinuxDebug			= ""

		# Windows
		self.dependWindows32Release = ""
		self.dependWindows32Debug	= ""
		self.dependWindows64Release = ""
		self.dependWindows64Debug	= ""
		# Linux
		self.dependLinux32Release	= ""
		self.dependLinux32Debug		= ""
		self.dependLinux64Release	= ""
		self.dependLinux64Debug		= ""
		# Android
		self.dependAndroid32Release = ""
		self.dependAndroid32Debug	= ""
		self.dependAndroid64Release = ""
		self.dependAndroid64Debug	= ""
		# Mac OS X
		self.dependMacOSX32Release	= ""
		self.dependMacOSX32Debug	= ""
		self.dependMacOSX64Release	= ""
		self.dependMacOSX64Debug	= ""

		self.mainSrc			= ""
		self.sourcePath			= ""
		self.includePath		= ""
		self.outputPath			= ""
		self.outputPlugin		= ""

# Print debug message (debug mode)
def PrintDebug(message):
	global debug
	if debug:
		print(message)
	return
	
# Print a list (debug mode)
def PrintList(list, prefix):
	global debug
	if debug:
		for item in list:
			print(prefix + "'" + item + "'")
	return

# Find files
def Find(path, pattern, recursive):
	# Find files
	if recursive:
		filenames = glob.glob(path + '/*/' + pattern)
	else:
		filenames = glob.glob(path + '/' + pattern)

	# Return files
	return filenames

# Get quoted string
def GetQuotedString(text):
	result = ""
	if text[0] == '"' and text[-1] == '"':
		result = text[1:-1]
	return result

# Get boolean is string
def BoolToString(bool):
	if bool:
		return "1"
	else:
		return "0"

# Parse all files of a project
def ParseFile(project, files):
	return True

# Parse module definition
def ParseModule(project):
	# Parse main source file
	mainSrc = project.mainSrc
	print("Parsing main source file at '" + mainSrc + "'")

	# Open file
	try:                                
		file = open(mainSrc)
	except getopt.GetoptError:
		print("ERROR: Could not open file '" + mainSrc + "'!")
		return False

	# Parse file
	for line in file:
		# [DEBUG]
		PrintDebug(line)

		# Check for pl_module
		match = reModule.match(line)
		if match:
			project.moduleName   = match.group("name")
			project.modulePlugin = False
			print("Module name = '" + project.moduleName + "'")

		# Check for pl_module_plugin
		match = reModulePlugin.match(line)
		if match:
			project.moduleName   = match.group("name")
			project.modulePlugin = True
			print("Module name = '" + project.moduleName + "'")
			print("This module is a plugin")

		# Check for pl_module_vendor
		match = reVendor.match(line)
		if match:
			project.moduleVendor = GetQuotedString(match.group("text"))
			print("Vendor name = '" + project.moduleVendor + "'")

		# Check for pl_module_license
		match = reLicense.match(line)
		if match:
			project.moduleLicense = GetQuotedString(match.group("text"))
			print("License = '" + project.moduleLicense + "'")

		# Check for pl_module_description
		match = reDescription.match(line)
		if match:
			project.moduleDescription = GetQuotedString(match.group("text"))
			print("Description = '" + project.moduleDescription + "'")

		# Check for pl_module_version
		match = reVersion.match(line)
		if match:
			project.moduleVersion = GetQuotedString(match.group("text"))
			print("Version = '" + project.moduleVersion + "'")

		# Check for pl_module_active
		match = reActive.match(line)
		if match:
			active = match.group("num")
			project.moduleActive = (active == "1")
			if project.moduleActive:
				print("Active = 'yes'")
			else:
				print("Active = 'no'")


		# Windows
		# Check for pl_module_dependencies_windows_32_release
		match = reDepsWindows32Release.match(line)
		if match:
			project.dependWindows32Release = GetQuotedString(match.group("text"))
			print("Windows 32 bit release dependencies = '" + project.dependWindows32Release + "'")

		# Check for pl_module_dependencies_windows_32_debug
		match = reDepsWindows32Debug.match(line)
		if match:
			project.dependWindows32Debug = GetQuotedString(match.group("text"))
			print("Windows 32 debug dependencies = '" + project.dependWindows32Debug + "'")

		# Check for pl_module_dependencies_windows_64_release
		match = reDepsWindows64Release.match(line)
		if match:
			project.dependWindows64Release = GetQuotedString(match.group("text"))
			print("Windows 64 bit release dependencies = '" + project.dependWindows64Release + "'")

		# Check for pl_module_dependencies_windows_64_debug
		match = reDepsWindows64Debug.match(line)
		if match:
			project.dependWindows64Debug = GetQuotedString(match.group("text"))
			print("Windows 64 bit debug dependencies = '" + project.dependWindows64Debug + "'")


		# Linux
		# Check for pl_module_dependencies_linux_32_release
		match = reDepsLinux32Release.match(line)
		if match:
			project.dependLinux32Release = GetQuotedString(match.group("text"))
			print("Linux 32 bit release dependencies = '" + project.dependLinux32Release + "'")

		# Check for pl_module_dependencies_linux_32_debug
		match = reDepsLinux32Debug.match(line)
		if match:
			project.dependLinux32Debug = GetQuotedString(match.group("text"))
			print("Linux 32 debug dependencies = '" + project.dependLinux32Debug + "'")

		# Check for pl_module_dependencies_linux_64_release
		match = reDepsLinux64Release.match(line)
		if match:
			project.dependLinux64Release = GetQuotedString(match.group("text"))
			print("Linux 64 bit release dependencies = '" + project.dependLinux64Release + "'")

		# Check for pl_module_dependencies_linux_64_debug
		match = reDepsLinux64Debug.match(line)
		if match:
			project.dependLinux64Debug = GetQuotedString(match.group("text"))
			print("Linux 64 bit debug dependencies = '" + project.dependLinux64Debug + "'")


		# Android
		# Check for pl_module_dependencies_android_32_release
		match = reDepsAndroid32Release.match(line)
		if match:
			project.dependAndroid32Release = GetQuotedString(match.group("text"))
			print("Android 32 bit release dependencies = '" + project.dependAndroid32Release + "'")

		# Check for pl_module_dependencies_android_32_debug
		match = reDepsAndroid32Debug.match(line)
		if match:
			project.dependAndroid32Debug = GetQuotedString(match.group("text"))
			print("Android 32 debug dependencies = '" + project.dependAndroid32Debug + "'")

		# Check for pl_module_dependencies_android_64_release
		match = reDepsAndroid64Release.match(line)
		if match:
			project.dependAndroid64Release = GetQuotedString(match.group("text"))
			print("Android 64 bit release dependencies = '" + project.dependAndroid64Release + "'")

		# Check for pl_module_dependencies_android_64_debug
		match = reDepsWindows64Debug.match(line)
		if match:
			project.dependWindows64Debug = GetQuotedString(match.group("text"))
			print("Windows 64 bit debug dependencies = '" + project.dependWindows64Debug + "'")


		# Mac OS X
		# Check for pl_module_dependencies_macosx_32_release
		match = reDepsMacOSX32Release.match(line)
		if match:
			project.dependMacOSX32Release = GetQuotedString(match.group("text"))
			print("MacOSX 32 bit release dependencies = '" + project.dependMacOSX32Release + "'")

		# Check for pl_module_dependencies_macosx_32_debug
		match = reDepsMacOSX32Debug.match(line)
		if match:
			project.dependMacOSX32Debug = GetQuotedString(match.group("text"))
			print("MacOSX 32 debug dependencies = '" + project.dependMacOSX32Debug + "'")

		# Check for pl_module_dependencies_macosx_64_release
		match = reDepsMacOSX64Release.match(line)
		if match:
			project.dependMacOSX64Release = GetQuotedString(match.group("text"))
			print("MacOSX 64 bit release dependencies = '" + project.dependMacOSX64Release + "'")

		# Check for pl_module_dependencies_macosx_64_debug
		match = reDepsMacOSX64Debug.match(line)
		if match:
			project.dependMacOSX64Debug = GetQuotedString(match.group("text"))
			print("MacOSX 64 bit debug dependencies = '" + project.dependMacOSX64Debug + "'")


	# Close file
	file.close()

	# Compose library names
	if project.modulePlugin:
		# Compose file names
		project.libWindowsRelease	= project.name + project.suffix + ".dll"
		project.libWindowsDebug		= project.name + project.suffix + "D.dll"
		project.libLinuxRelease		= "lib" + project.name + project.suffix + ".so"
		project.libLinuxDebug		= "lib" + project.name + project.suffix + "D.so"
		project.libAndroidRelease	= "lib" + project.name + project.suffix + ".so"
		project.libAndroidDebug		= "lib" + project.name + project.suffix + "D.so"
		project.libMacOSXRelease	= "lib" + project.name + project.suffix + ".dylib"
		project.libMacOSXDebug		= "lib" + project.name + project.suffix + "D.dylib"

		# Output file names
		print("Windows release  library = '" + project.libWindowsRelease)
		print("Windows debug    library = '" + project.libWindowsDebug)
		print("Linux release    library = '" + project.libLinuxRelease)
		print("Linux debug      library = '" + project.libLinuxDebug)
		print("Android release  library = '" + project.libAndroidRelease)
		print("Android debug    library = '" + project.libAndroidDebug)
		print("Mac OS X release library = '" + project.libMacOSXRelease)
		print("Mac OS X debug   library = '" + project.libMacOSXDebug)

	# Done
	return True

# Parse a project source tree
def ParseProject(project):
	# Parse project
	path = project.path
	print("Parsing project at '" + path + "'")

	# Find VC projects
	print("Looking for project files")
	projects = Find(path, "*.vcxproj", False)
	PrintList(projects, "found ")

	# Find out name of project
	name = ""
	for filename in projects:
		# [DEBUG]
		PrintDebug("Project '" + filename + "'")

		# Check file
		if filename.find("Stat") > -1:
			# Ignore static libs
			PrintDebug("This is a static lib version, ignoring ...")
		elif filename.find("Internal") > -1:
			# Ignore internal projects
			PrintDebug("This is an internal project file, ignoring ...")
		else:
			# Cut extension (.vcxproj)
			fpath, fname = os.path.split(filename)
			name, fext = os.path.splitext(fname)
			PrintDebug("Taking '" + name + "' as project name")

	# Recognize suffix
	if project.suffix != "":
		print("Using project suffix '" + project.suffix + "'")

	# Check if a project name has been found
	if name != "":
		print("Found project '" + name + "'")
		project.name = name
	else:
		print("ERROR: Could not determine project name!")
		return False

	# Find main source file
	mainSrc = path + "/src/" + name + ".cpp"
	if os.path.exists(mainSrc):
		print("Found main source file '" + mainSrc + "'")
		project.mainSrc = mainSrc
	else:
		print("ERROR: Could not determine main source file! " + mainSrc)
		return False

	# Parse module file
	if not ParseModule(project):
		# Error parsing module file
		return False

	# Find source files
	sourcePath = path + "/src"
	if os.path.exists(sourcePath) and os.path.isdir(sourcePath):
		print("Found source files at '" + sourcePath + "'")
		project.sourcePath = sourcePath
	else:
		print("ERROR: Could not find source files in '" + sourcePath + "' - ")
		if not os.path.exists(sourcePath):
			print("ERROR: " "File does not exist!")
		else:
			print("ERROR: " "File is not a directory!")
		return False

	# Find include files
	includePath = path + "/include"
	if os.path.exists(includePath) and os.path.isdir(includePath):
		print("Found include files at '" + includePath + "'")
	else:
		print("Could not find include files, taking '" + sourcePath + "'")
		project.includePath = sourcePath

	# Add all source, header and inline files
	files = Find(sourcePath, "*.cpp", True)
	files = files + Find(includePath, "*.h", True)
	files = files + Find(includePath, "*.inl", True)
	PrintList(files, "- Found file ")

	# And now, parse all these files :-)
	if not ParseFile(project, files):
		# Error parsing files
		return False

	# Done
	return True

# Create plugin file for a project
def CreatePluginFile(project):
	# Compose output filename
	project.outputPlugin = project.outputPath + "/" + project.name + project.suffix + ".plugin"
	print("Writing plugin file '" + project.outputPlugin + "'")

	# Open plugin file
#	if (cFile.Open(File::FileCreate | File::FileWrite | File::FileText)) {
	try:                                
		file = open(project.outputPlugin, 'w')
	except getopt.GetoptError:
		# Could not open file
		print("ERROR: Could not write to file '" + project.outputPlugin + "'!")
		return False

	# Write plugin file
	file.write(u"<?xml version=\"1.0\" ?>\n")
	file.write(u"<Plugin Version=\"1\">\n")
	file.write(u"\t<Active>" + BoolToString(project.moduleActive) + "</Active>\n")
	file.write(u"\t<Delayed>0</Delayed>\n")
	file.write(u"\t<Name>" + project.moduleName + "</Name>\n")
	file.write(u"\t<Version>" + project.moduleVersion + "</Version>\n")

	# Vendor
	if project.moduleVendor != "":
		file.write(u"\t<Vendor>" + project.moduleVendor + "</Vendor>\n")

	# License
	if project.moduleLicense != "":
		file.write(u"\t<License>" + project.moduleLicense + "</License>\n")

	# Description
	if project.moduleDescription != "":
		file.write(u"\t<Description>" + project.moduleDescription + "</Description>\n")


	# Windows
	# Platform: Windows 32 bit
	file.write(u"\t<Platform Name=\"Windows32\">\n")

	# Library: Release 32 bit
	if project.dependWindows32Release != "":
		file.write(u"\t\t<Library Type=\"Release\" Dependency=\"" + project.dependWindows32Release + "\">" + project.libWindows32Release + "</Library>\n")
	else:
		file.write(u"\t\t<Library Type=\"Release\">" + project.libWindows32Release + "</Library>\n")

	# Library: Debug 32 bit
	if project.dependWindows32Debug != "":
		file.write(u"\t\t<Library Type=\"Debug\" Dependency=\"" + project.dependWindows32Debug + "\">" + project.libWindows32Debug + "</Library>\n")
	else:
		file.write(u"\t\t<Library Type=\"Debug\">" + project.libWindows32Debug + "</Library>\n")

	# Platform end tag
	file.write(u"\t</Platform>\n")

	# Library: Release 64 bit
	if project.dependWindows64Release != "":
		file.write(u"\t\t<Library Type=\"Release\" Dependency=\"" + project.dependWindows64Release + "\">" + project.libWindows64Release + "</Library>\n")
	else:
		file.write(u"\t\t<Library Type=\"Release\">" + project.libWindows64Release + "</Library>\n")

	# Library: Debug 64 bit
	if project.dependWindows64Debug != "":
		file.write(u"\t\t<Library Type=\"Debug\" Dependency=\"" + project.dependWindows64Debug + "\">" + project.libWindows64Debug + "</Library>\n")
	else:
		file.write(u"\t\t<Library Type=\"Debug\">" + project.libWindows64Debug + "</Library>\n")

	# Platform end tag
	file.write(u"\t</Platform>\n")


	# Linux
	# Platform: Linux 32 bit
	file.write(u"\t<Platform Name=\"Linux32\">\n")

	# Library: Release 32 bit
	if project.dependLinux32Release != "":
		file.write(u"\t\t<Library Type=\"Release\" Dependency=\"" + project.dependLinux32Release + "\">" + project.libLinux32Release + "</Library>\n")
	else:
		file.write(u"\t\t<Library Type=\"Release\">" + project.libLinux32Release + "</Library>\n")

	# Library: Debug 32 bit
	if project.dependLinux32Debug != "":
		file.write(u"\t\t<Library Type=\"Debug\" Dependency=\"" + project.dependLinux32Debug + "\">" + project.libLinux32Debug + "</Library>\n")
	else:
		file.write(u"\t\t<Library Type=\"Debug\">" + project.libLinux32Debug + "</Library>\n")

	# Platform end tag
	file.write(u"\t</Platform>\n")

	# Library: Release 64 bit
	if project.dependLinux64Release != "":
		file.write(u"\t\t<Library Type=\"Release\" Dependency=\"" + project.dependLinux64Release + "\">" + project.libLinux64Release + "</Library>\n")
	else:
		file.write(u"\t\t<Library Type=\"Release\">" + project.libLinux64Release + "</Library>\n")

	# Library: Debug 64 bit
	if project.dependLinux64Debug != "":
		file.write(u"\t\t<Library Type=\"Debug\" Dependency=\"" + project.dependLinux64Debug + "\">" + project.libLinux64Debug + "</Library>\n")
	else:
		file.write(u"\t\t<Library Type=\"Debug\">" + project.libLinux64Debug + "</Library>\n")

	# Platform end tag
	file.write(u"\t</Platform>\n")


	# Android
	# Platform: Android 32 bit
	file.write(u"\t<Platform Name=\"Android32\">\n")

	# Library: Release 32 bit
	if project.dependAndroid32Release != "":
		file.write(u"\t\t<Library Type=\"Release\" Dependency=\"" + project.dependAndroid32Release + "\">" + project.libAndroid32Release + "</Library>\n")
	else:
		file.write(u"\t\t<Library Type=\"Release\">" + project.libAndroid32Release + "</Library>\n")

	# Library: Debug 32 bit
	if project.dependAndroid32Debug != "":
		file.write(u"\t\t<Library Type=\"Debug\" Dependency=\"" + project.dependAndroid32Debug + "\">" + project.libAndroid32Debug + "</Library>\n")
	else:
		file.write(u"\t\t<Library Type=\"Debug\">" + project.libAndroid32Debug + "</Library>\n")

	# Platform end tag
	file.write(u"\t</Platform>\n")

	# Library: Release 64 bit
	if project.dependAndroid64Release != "":
		file.write(u"\t\t<Library Type=\"Release\" Dependency=\"" + project.dependAndroid64Release + "\">" + project.libAndroid64Release + "</Library>\n")
	else:
		file.write(u"\t\t<Library Type=\"Release\">" + project.libAndroid64Release + "</Library>\n")

	# Library: Debug 64 bit
	if project.dependAndroid64Debug != "":
		file.write(u"\t\t<Library Type=\"Debug\" Dependency=\"" + project.dependAndroid64Debug + "\">" + project.libAndroid64Debug + "</Library>\n")
	else:
		file.write(u"\t\t<Library Type=\"Debug\">" + project.libAndroid64Debug + "</Library>\n")

	# Platform end tag
	file.write(u"\t</Platform>\n")


	# Mac OS X
	# Platform: Mac OS X 32 bit
	file.write(u"\t<Platform Name=\"MacOSX32\">\n")

	# Library: Release 32 bit
	if project.dependMacOSX32Release != "":
		file.write(u"\t\t<Library Type=\"Release\" Dependency=\"" + project.dependMacOSX32Release + "\">" + project.libMacOSX32Release + "</Library>\n")
	else:
		file.write(u"\t\t<Library Type=\"Release\">" + project.libMacOSX32Release + "</Library>\n")

	# Library: Debug 32 bit
	if project.dependMacOSX32Debug != "":
		file.write(u"\t\t<Library Type=\"Debug\" Dependency=\"" + project.dependMacOSX32Debug + "\">" + project.libMacOSX32Debug + "</Library>\n")
	else:
		file.write(u"\t\t<Library Type=\"Debug\">" + project.libMacOSX32Debug + "</Library>\n")

	# Platform end tag
	file.write(u"\t</Platform>\n")

	# Library: Release 64 bit
	if project.dependMacOSX64Release != "":
		file.write(u"\t\t<Library Type=\"Release\" Dependency=\"" + project.dependMacOSX64Release + "\">" + project.libMacOSX64Release + "</Library>\n")
	else:
		file.write(u"\t\t<Library Type=\"Release\">" + project.libMacOSX64Release + "</Library>\n")

	# Library: Debug 64 bit
	if project.dependMacOSX64Debug != "":
		file.write(u"\t\t<Library Type=\"Debug\" Dependency=\"" + project.dependMacOSX64Debug + "\">" + project.libMacOSX64Debug + "</Library>\n")
	else:
		file.write(u"\t\t<Library Type=\"Debug\">" + project.libMacOSX64Debug + "</Library>\n")

	# Platform end tag
	file.write(u"\t</Platform>\n")


	# Platform end tag
	file.write(u"\t</Platform>\n")

	# Plugin end tag
	file.write(u"</Plugin>\n")

	# Done
	file.close()
	return True


# Print help
def help():
	print("Usage:")
	print("  PLProject <path> [--write-plugin] [--output-path <filename>] [--suffix <suffix>] [--debug]")
	return

# Main function
def main(argv):
	# Init
	global debug
	debug = False

	# Output startup info
	print("PLProject V1.0 (Python)")
	print("Current dir is '" + os.getcwd() + "'")

	# Parse command line
	path		 = argv[0]
	output_path	 = "."
	suffix		 = ""
	write_plugin = False
	try:
		opts, args = getopt.getopt(argv[1:], "hds:o:w", ["help", "debug", "suffix=", "output-path=", "write-plugin"])
		for opt, arg in opts:
			if opt in ("-h", "--help"):
				help()
				sys.exit()
			elif opt in ("-d", "--debug"):
				debug = True
			elif opt in ("-w", "--write-plugin"):
				write_plugin = True
			elif opt in ("-s", "--suffix"):
				suffix = arg
			elif opt in ("-o", "--output-path"):
				if arg[-1] == '\\' or arg[-1] == '/':
					output_path = arg[:-1]
				else:
					output_path = arg

	except getopt.GetoptError:
		help()
		sys.exit(2)

	# Parse project
	project = Project()
	project.path		 = path
	project.suffix		 = suffix
	project.modulePlugin = False
	project.moduleActive = True
	if ParseProject(project):
		# Write plugin file
		if write_plugin:
			project.outputPath = output_path
			if not CreatePluginFile(project):
				print("Could not write out plugin file!")
				sys.exit(1)
	else:
		print("Could not parse project!")
		sys.exit(1)
	return

# Call main
main(sys.argv[1:])
