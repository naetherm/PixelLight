#*********************************************************#
#*  File: PixelLight.cmake                               *
#*
#*  Copyright (C) 2002-2013 The PixelLight Team (http://www.pixellight.org/)
#*
#*  This file is part of PixelLight.
#*
#*  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
#*  and associated documentation files (the "Software"), to deal in the Software without
#*  restriction, including without limitation the rights to use, copy, modify, merge, publish,
#*  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
#*  Software is furnished to do so, subject to the following conditions:
#*
#*  The above copyright notice and this permission notice shall be included in all copies or
#*  substantial portions of the Software.
#*
#*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
#*  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
#*  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
#*  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#*  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#*********************************************************#


##################################################
## PixelLight cmake scripts
##
## Contains scripts and macros that are specific for
## the PixelLight project.
##################################################


##################################################
## Project information
##################################################

# Project name
set(CMAKETOOLS_PROJECT_NAME "PixelLight")

# Ensure the CMake variable CMAKE_BUILD_TYPE exists and has a value, in case it's an empty
# string assign "Release" to make it easier to find this variable within the CMake GUI (looks like a CMake cache problem?)
if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
	set (CMAKE_BUILD_TYPE Release CACHE STRING "One of: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

# Check which architecture has the host system
# There is currently a bug in CMake that makes it report 32 bit architecture even on 64 bit system. This is why we don't use the
# CMAKE_SYSTEM_PROCESSOR value. We prefer the native bitsize for the system, however, for MS Visual Studio, we always pick the
# bitsize specified by the generator (as it won't compile otherwise!)
if(CMAKE_GENERATOR MATCHES "Visual Studio .. Win64")
	set(X86_64 1)
elseif(CMAKE_GENERATOR MATCHES "Visual Studio*")
	set(X86 1)
elseif(CMAKE_SIZEOF_VOID_P MATCHES 8)
	set(X86_64 1)
else()
	set(X86 1)
endif()

# Use native PLProject? (internal option set by toolchains)
if(NOT PL_NATIVE_PLPROJECT)
	set(PL_NATIVE_PLPROJECT 1)
endif()

# Build for mobile platforms? (internal option set by toolchains)
if(NOT PL_MOBILE)
	set(PL_MOBILE 0)
endif()

# Nightly build
set(CMAKETOOLS_CONFIG_NIGHTLY "0" CACHE BOOL "Create a nightly build?")

# Build docs
set(PL_DOCS "0" CACHE BOOL "Build the user documentation")

# Target architecture (x86, arm...)
if(CMAKETOOLS_TARGET_ARCH)
	set(CMAKETOOLS_TARGET_ARCH ${CMAKETOOLS_TARGET_ARCH} CACHE STRING "Target architecture")
else()
	set(CMAKETOOLS_TARGET_ARCH "x86" CACHE STRING "Target architecture")
endif()

# Set default bit size depending on the host system architecture (currently set 64bit only for x86_64 hosts)
if(X86_64 AND CMAKETOOLS_TARGET_ARCH MATCHES "x86")
	# Target bitsize (32/64)
	set(CMAKETOOLS_TARGET_BITSIZE "64" CACHE STRING "Target bitsize")

	# Target architecture & bitsize (x86, x64 - yes, there are many names for the 64 bit version, but e.g. "x64" is just more handy as the technically "x86_64")
	set(CMAKETOOLS_TARGET_ARCHBITSIZE "x64" CACHE STRING "Target architecture & bitsize")
else()
	# Target bitsize (32/64)
	set(CMAKETOOLS_TARGET_BITSIZE "32" CACHE STRING "Target bitsize")

	# Target architecture & bitsize (x86, x64 - yes, there are many names for the 64 bit version, but e.g. "x64" is just more handy as the technically "x86_32")
	set(CMAKETOOLS_TARGET_ARCHBITSIZE ${CMAKETOOLS_TARGET_ARCH} CACHE STRING "Target architecture & bitsize")
endif()

# Use system libraries or build externals?
set(CMAKETOOLS_USE_SYSTEM_LIBS "0" CACHE BOOL "Use system libraries or build own external libraries?")

# The following is Linux only
if(LINUX)
	if(X86_64 AND CMAKETOOLS_TARGET_ARCHBITSIZE MATCHES "x86")
		message(STATUS "Setup additional paths to find 32bit system libraries instead of the 64bit ones")

		# We are doing a "cross compilation" (building an 32bit application on a 64Bit host)
		set(CMAKE_PREFIX_PATH
			/usr/lib32
			/lib32
			/opt/nvidia-cg-toolkit/lib32 # Special case under Gentoo
		)
	endif()

	# Linux standalone: Set Linux RPATH to "$ORIGIN" so shared libraries are first searched
	# within the directory of the executable (see http://www.cmake.org/Wiki/CMake_RPATH_handling)
	# Type e.g. "objdump -x libPLCore.so" and have a look at "Dynamic Section"->"RPATH",
	# should be "$ORIGIN" when this option is enabled
	if(CMAKETOOLS_SET_RPATH_TO_ORIGIN)
		set(CMAKETOOLS_SET_RPATH_TO_ORIGIN ${CMAKETOOLS_SET_RPATH_TO_ORIGIN} CACHE BOOL "Set \"RPATH\" to \"$ORIGIN\" so shared libraries are first searched within the directory of the executable?")
	else()
		set(CMAKETOOLS_SET_RPATH_TO_ORIGIN "0" CACHE BOOL "Set \"RPATH\" to \"$ORIGIN\" so shared libraries are first searched within the directory of the executable?")
	endif()

	# Detailed version information?
	set(CMAKETOOLS_DETAILED_VERSION_INFORMATION "0" CACHE BOOL "Create detailed version information? If active, symlinks like \"libPLCore.so.1.0.1\" and \"libPLCore.so.0\" are created and also used directly within the created libraries and executables.")
endif()

# Repository URL (where to download the external pre-built packages from)
set(PL_EXTERNAL_REPOSITORY "pixellight.sourceforge.net/externals" CACHE STRING "External repository")

# User name for access to the repository
set(PL_EXTERNAL_USER "" CACHE STRING "External repository user name. Required in order to automatically download protected, non public external packages. Do also set PL_EXTERNAL_PASS (test your accout by e.g. opening \"pixellight.sourceforge.net/externals/Windows_x86_32/nonpublic\" within your webbrowser).")

# User password for access to the repository
set(PL_EXTERNAL_PASS "" CACHE STRING "External repository user password. Required in order to automatically download protected, non public external packages. Do also set PL_EXTERNAL_USER (test your accout by e.g. opening \"pixellight.sourceforge.net/externals/Windows_x86_32/nonpublic\" within your webbrowser).")

# Build minimal projects?
if(CMAKETOOLS_MINIMAL)
	set(CMAKETOOLS_MINIMAL ${CMAKETOOLS_MINIMAL} CACHE BOOL "Build minimal projects (or all)?")
else()
	set(CMAKETOOLS_MINIMAL "0" CACHE BOOL "Build minimal projects (or all)?")
endif()

# Build SDK projects?
if(CMAKETOOLS_BUILD_SDK)
	set(CMAKETOOLS_BUILD_SDK ${CMAKETOOLS_BUILD_SDK} CACHE BOOL "Build SDK projects (or all)?")
else()
	set(CMAKETOOLS_BUILD_SDK "0" CACHE BOOL "Build SDK projects (or all)?")
endif()

# Project suffix
#   The suffix is appended to every library (e.g. [lib]MyLibrary-<suffix>.dll/lib/so/a)
#   and can be used to install several versions of PixelLight at the same time
set(CMAKETOOLS_CONFIG_SUFFIX "" CACHE STRING "Library suffix")

# Version
set(CMAKETOOLS_PROJECT_VERSION_MAJOR    1)
set(CMAKETOOLS_PROJECT_VERSION_MINOR    0)
set(CMAKETOOLS_PROJECT_VERSION_PATCH    1)
set(CMAKETOOLS_PROJECT_VERSION_RELEASE "R1")
if(CMAKETOOLS_CONFIG_NIGHTLY)
	# [TODO] Add date to string (e.g. "nightly-20100101")
	set(CMAKETOOLS_PROJECT_VERSION_RELEASE "nightly")
endif()

# Upload destination
if(CMAKETOOLS_CONFIG_NIGHTLY)
	set(CMAKETOOLS_UPLOAD_LOCATION "pixellight.org:/srv/www/pl-dev/files/nightly")
else()
	set(CMAKETOOLS_UPLOAD_LOCATION "pixellight.org:/srv/www/pl-dev/files/v${CMAKETOOLS_PROJECT_VERSION_MAJOR}.${CMAKETOOLS_PROJECT_VERSION_MINOR}")
endif()
message(STATUS "Upload destination is '${CMAKETOOLS_UPLOAD_LOCATION}'")

# Install locations
if(WIN32)
	set(PL_INSTALL_ROOT				".")															# C:\Programme\PixelLight
	set(PL_INSTALL_BIN				".")															# C:\Programme\PixelLight
	set(PL_INSTALL_LIB				"Lib/${CMAKETOOLS_TARGET_ARCHBITSIZE}")							# C:\Programme\PixelLight\Lib\x86
	set(PL_INSTALL_INCLUDE			"Include")														# C:\Programme\PixelLight\Include
	set(PL_INSTALL_DOCS				"Docs")															# C:\Programme\PixelLight\Docs
	set(PL_INSTALL_RUNTIME			"Runtime")														# C:\Programme\PixelLight\Runtime
	set(PL_INSTALL_RUNTIME_BIN		"Runtime/${CMAKETOOLS_TARGET_ARCHBITSIZE}")						# C:\Programme\PixelLight\Runtime\x86
	set(PL_INSTALL_SAMPLES			"Samples")														# C:\Programme\PixelLight\Samples
	set(PL_INSTALL_SAMPLES_BIN		"Samples/Bin/${CMAKETOOLS_TARGET_ARCHBITSIZE}")					# C:\Programme\PixelLight\Samples\Bin\x86
	set(PL_INSTALL_SAMPLES_DATA		"Samples/Bin")													# C:\Programme\PixelLight\Samples\Bin
	set(PL_INSTALL_TOOLS			"Tools")														# C:\Programme\PixelLight\Tools
	set(PL_INSTALL_TOOLS_BIN		"Tools/${CMAKETOOLS_TARGET_ARCHBITSIZE}")						# C:\Programme\PixelLight\Tools\x86
	set(PL_INSTALL_BROWSER			"BrowserPlugins")												# C:\Programme\PixelLight\BrowserPlugins
elseif(LINUX)
	set(PL_INSTALL_ROOT				"share/pixellight")												# /usr/share/pixellight
	set(PL_INSTALL_BIN				"bin")															# /usr/bin
	set(PL_INSTALL_LIB				"lib")															# /usr/lib
	set(PL_INSTALL_INCLUDE			"include/pixellight")											# /usr/include/pixellight
	set(PL_INSTALL_DOCS				"share/docs/pixellight")										# /usr/share/docs/pixellight
	set(PL_INSTALL_RUNTIME			"share/pixellight/Runtime")										# /usr/share/pixellight/Runtime
	set(PL_INSTALL_RUNTIME_BIN		"${PL_INSTALL_RUNTIME}/${CMAKETOOLS_TARGET_ARCHBITSIZE}")		# /usr/share/pixellight/Runtime/x86
	set(PL_INSTALL_SAMPLES			"share/pixellight/Samples")										# /usr/share/pixellight/Samples
	set(PL_INSTALL_SAMPLES_BIN		"${PL_INSTALL_SAMPLES}/Bin/${CMAKETOOLS_TARGET_ARCHBITSIZE}")	# /usr/share/pixellight/Samples/Bin/x86
	set(PL_INSTALL_SAMPLES_DATA		"share/pixellight/Samples/Bin")									# /usr/share/pixellight/Samples/Bin
	set(PL_INSTALL_TOOLS			"share/pixellight/Tools")										# /usr/share/pixellight/Tools
	set(PL_INSTALL_TOOLS_BIN		"${PL_INSTALL_TOOLS}/${CMAKETOOLS_TARGET_ARCHBITSIZE}")			# /usr/share/pixellight/Tools/x86
	set(PL_INSTALL_BROWSER			"share/pixellight/BrowserPlugins")								# /usr/share/pixellight/BrowserPlugins
endif()


##################################################
## All optional projects
##################################################
# By default, disable plugins using stuff were we can't provide a public downloadable package
# in order to avoid compiler errors due to not found includes (at least when using the default settings)
set(PL_USE_NONPUBLIC 0)

# Build or exclude plugins. For the best overview, all optional projects are listed in here. Options may be removed if not available for a target.
if(CMAKETOOLS_MINIMAL)
	# Minimal build (no scripting, no database, no own or external GUI system library, no sound, no physics etc., just renderer to be able to see anything)
	set (PL_CORE_ZIP								"0"					CACHE BOOL "Build in ZIP support within 'PLCore'? (it's highly recommended to enable ZIP support, requires 'zlib' external dependency)")	# Some stuff may not work, but this is a minimal build, really minimal to have something to start with
	set (PL_PLUGIN_SCRIPT_NULL						"0"					CACHE BOOL "Build plugin 'PLScriptNull'?")
	set (PL_PLUGIN_SCRIPT_LUA						"0"					CACHE BOOL "Build plugin 'PLScriptLua'? (you may also want to use 'PLScriptBindings')")
	set (PL_PLUGIN_SCRIPT_V8						"0"					CACHE BOOL "Build plugin 'PLScriptV8'? (you may also want to use 'PLScriptBindings')")
	set (PL_PLUGIN_SCRIPT_PYTHON					"0"					CACHE BOOL "Build plugin 'PLScriptPython'? (you may also want to use 'PLScriptBindings')")
	set (PL_PLUGIN_SCRIPT_ANGELSCRIPT				"0"					CACHE BOOL "Build plugin 'PLScriptAngelScript'? (you may also want to use 'PLScriptBindings')")
	set (PL_PLUGIN_DATABASE_NULL					"0"					CACHE BOOL "Build plugin 'PLDatabaseNull'?")
	set (PL_PLUGIN_DATABASE_MYSQL					"0"					CACHE BOOL "Build plugin 'PLDatabaseMySQL'?")
	set (PL_PLUGIN_DATABASE_POSTGRESQL				"0"					CACHE BOOL "Build plugin 'PLDatabasePostgreSQL'?")
	set (PL_PLUGIN_DATABASE_SQLITE					"0"					CACHE BOOL "Build plugin 'PLDatabaseSQLite'?")
	set (PL_PLUGIN_PHYSICS_NULL						"0"					CACHE BOOL "Build plugin 'PLPhysicsNull'?")
	set (PL_PLUGIN_PHYSICS_NEWTON					"0"					CACHE BOOL "Build plugin 'PLPhysicsNewton'?")
	set (PL_PLUGIN_PHYSICS_ODE						"0"					CACHE BOOL "Build plugin 'PLPhysicsODE'?")
	set (PL_PLUGIN_PHYSICS_PHYSX					"0"					CACHE BOOL "Build plugin 'PLPhysicsPhysX'? (due to legal issues, we can't provide a public downloadable package)")
	set (PL_PLUGIN_PHYSICS_BULLET					"0"					CACHE BOOL "Build plugin 'PLPhysicsBullet'? (heavily under construction)")
	set (PL_PLUGIN_RENDERER_NULL					"0"					CACHE BOOL "Build plugin 'PLRendererNull'?")
	if(PL_MOBILE)
		set (PL_PLUGIN_FRONTEND_OS					"1"					CACHE BOOL "Build plugin 'PLFrontendOS'?")
		set (PL_PLUGIN_RENDERER_OPENGL				"0"					CACHE BOOL "Build plugin 'PLRendererOpenGL'? (for font support, do also enable 'PL_PLUGIN_RENDERER_OPENGL_FONT')")
		set (PL_PLUGIN_RENDERER_OPENGL_FONT			"0"					CACHE BOOL "Build plugin 'PLRendererOpenGL' with font support? (requires 'freetype' external dependency)")
		set (PL_PLUGIN_RENDERER_OPENGLCG			"0"					CACHE BOOL "Build plugin 'PLRendererOpenGLCg'? (requires 'PLRendererOpenGL') (due to legal issues, we can't provide a public downloadable package)")
		set (PL_PLUGIN_RENDERER_OPENGLES2			"1"					CACHE BOOL "Build plugin 'PLRendererOpenGLES2'? (for font support, do also enable 'PL_PLUGIN_RENDERER_OPENGLES2_FONT')")
		set (PL_PLUGIN_RENDERER_OPENGLES2_EMULATOR	"0"					CACHE BOOL "Build plugin 'PLRendererOpenGLES2' and add proprietary emulator? (do also enable 'PL_PLUGIN_RENDERER_OPENGLES2_FONT') (due to legal issues, we can't provide a public downloadable package)")
		set (PL_PLUGIN_RENDERER_OPENGLES2_FONT		"0"					CACHE BOOL "Build plugin 'PLRendererOpenGLES2' with font support? (requires 'freetype' external dependency)?")
		# Tools
		set (PL_TOOL_PLUPGRADE						"0"					CACHE BOOL "Build plugin 'PLUpgrade'?")
		set (PL_TOOL_PLINSTALL						"0"					CACHE BOOL "Build plugin 'PLInstall'?")
	else()
		set (PL_PLUGIN_FRONTEND_OS					"1"					CACHE BOOL "Build plugin 'PLFrontendOS'?")
		set (PL_PLUGIN_RENDERER_OPENGL				"1"					CACHE BOOL "Build plugin 'PLRendererOpenGL'? (for font support, do also enable 'PL_PLUGIN_RENDERER_OPENGL_FONT')")
		set (PL_PLUGIN_RENDERER_OPENGL_FONT			"0"					CACHE BOOL "Build plugin 'PLRendererOpenGL' with font support? (requires 'freetype' external dependency)")
		set (PL_PLUGIN_RENDERER_OPENGLCG			"0"					CACHE BOOL "Build plugin 'PLRendererOpenGLCg'? (requires 'PLRendererOpenGL') (due to legal issues, we can't provide a public downloadable package)")
		set (PL_PLUGIN_RENDERER_OPENGLES2			"0"					CACHE BOOL "Build plugin 'PLRendererOpenGLES2'? (for font support, do also enable 'PL_PLUGIN_RENDERER_OPENGLES2_FONT')")
		set (PL_PLUGIN_RENDERER_OPENGLES2_EMULATOR	"0"					CACHE BOOL "Build plugin 'PLRendererOpenGLES2' and add proprietary emulator? (do also enable 'PL_PLUGIN_RENDERER_OPENGLES2_FONT') (due to legal issues, we can't provide a public downloadable package)")
		set (PL_PLUGIN_RENDERER_OPENGLES2_FONT		"0"					CACHE BOOL "Build plugin 'PLRendererOpenGLES2' with font support? (requires 'freetype' external dependency)?")
		# Tools
		set (PL_TOOL_PLUPGRADE						"1"					CACHE BOOL "Build plugin 'PLUpgrade'?")
		set (PL_TOOL_PLINSTALL						"1"					CACHE BOOL "Build plugin 'PLInstall'?")
	endif()
	set (PL_PLUGIN_RENDERER_D3D9					"0"					CACHE BOOL "Build plugin 'PLRendererD3D9'? (due to legal issues, we can't provide a public downloadable package)")
	set (PL_PLUGIN_RENDERER_D3D11					"0"					CACHE BOOL "Build plugin 'PLRendererD3D11'? (due to legal issues, we can't provide a public downloadable package)")
	set (PL_PLUGIN_SOUND_NULL						"0"					CACHE BOOL "Build plugin 'PLSoundNull'?")
	set (PL_PLUGIN_SOUND_OPENAL						"0"					CACHE BOOL "Build plugin 'PLSoundOpenAL'?")
	set (PL_PLUGIN_SOUND_OPENSLES					"0"					CACHE BOOL "Build plugin 'PLSoundOpenSLES'? (heavily under construction)")
	set (PL_PLUGIN_SOUND_FMOD						"0"					CACHE BOOL "Build plugin 'PLSoundFMOD'? (due to legal issues, we can't provide a public downloadable package)")
	set (PL_PLUGIN_SOUND_FMODEX						"0"					CACHE BOOL "Build plugin 'PLSoundFMODEx'? (due to legal issues, we can't provide a public downloadable package)")
	set (PL_PLUGIN_ENGINE_COMPOSITING				"0"					CACHE BOOL "Build plugin 'PLCompositing'?")
	set (PL_PLUGIN_ENGINE_POSTPROCESS				"0"					CACHE BOOL "Build plugin 'PLPostProcessEffects'? (requires 'PLCompositing')")
	set (PL_PLUGIN_ENGINE_NATURE					"0"					CACHE BOOL "Build plugin 'PLNature'?")
	set (PL_PLUGIN_ENGINE_SCRIPTBINDINGS			"0"					CACHE BOOL "Build plugin 'PLScriptBindings'?")
	set (PL_PLUGIN_ENGINE_ASSIMP					"0"					CACHE BOOL "Build plugin 'PLAssimp'?")
	set (PL_PLUGIN_ENGINE_IMAGEEXR					"0"					CACHE BOOL "Build plugin 'PLImageLoaderEXR'?")
	set (PL_PLUGIN_ENGINE_PARTICLEGROUPS			"0"					CACHE BOOL "Build plugin 'PLParticleGroups'?")
	set (PL_PLUGIN_ENGINE_SPARK						"0"					CACHE BOOL "Build plugin 'SPARK_PL'?")
	set (PL_PLUGIN_ENGINE_LIBROCKET					"0"					CACHE BOOL "Build plugin 'PLlibRocket'?")
	# Tools
	set (PL_EXPORTER_3DSMAX_2008					"0"					CACHE BOOL "Build exporter plugin 'PL3dsMaxSceneExport_2008'? (due to legal issues, we can't provide a public downloadable package)")
	set (PL_EXPORTER_3DSMAX_2009					"0"					CACHE BOOL "Build exporter plugin 'PL3dsMaxSceneExport_2009'? (due to legal issues, we can't provide a public downloadable package)")
	set (PL_EXPORTER_3DSMAX_2010					"0"					CACHE BOOL "Build exporter plugin 'PL3dsMaxSceneExport_2010'? (due to legal issues, we can't provide a public downloadable package)")
	set (PL_EXPORTER_3DSMAX_2011					"0"					CACHE BOOL "Build exporter plugin 'PL3dsMaxSceneExport_2011'? (due to legal issues, we can't provide a public downloadable package)")
	set (PL_EXPORTER_3DSMAX_2012					"0"					CACHE BOOL "Build exporter plugin 'PL3dsMaxSceneExport_2012'? (due to legal issues, we can't provide a public downloadable package)")
	set (PL_EXPORTER_3DSMAX_2013					"0"					CACHE BOOL "Build exporter plugin 'PL3dsMaxSceneExport_2013'? (due to legal issues, we can't provide a public downloadable package)")
	# PLGraphics image loaders
	set (PL_PLUGIN_GRAPHICS_PNG						"0"					CACHE BOOL "Build in 'png' support within 'PLGraphics'?")
	set (PL_PLUGIN_GRAPHICS_JPG						"0"					CACHE BOOL "Build in 'jpg' support within 'PLGraphics'?")
	# Network
	set (PL_PLUGIN_NETWORK_IRC						"0"					CACHE BOOL "Build plugin 'PLIRC'?")
	set (PL_PLUGIN_NETWORK_JABBER					"0"					CACHE BOOL "Build plugin 'PLJabber'?")
	# Frontend
	set (PL_PLUGIN_FRONTEND_NULL					"0"					CACHE BOOL "Build plugin 'PLFrontendNull'?")
	set (PL_PLUGIN_FRONTEND_ACTIVEX					"0"					CACHE BOOL "Build plugin 'PLFrontendActiveX'?")
	set (PL_PLUGIN_FRONTEND_MOZILLA					"0"					CACHE BOOL "Build plugin 'PLFrontendMozilla'?")
	# PLGui
	set (PL_PLGUI									"0"					CACHE BOOL "Build 'PLGui'?")
	set (PL_PLUGIN_GUI_XMLTEXT						"0"					CACHE BOOL "Build plugin 'PLGuiXmlText'? (requires 'PLGui')")
	set (PL_PLUGIN_FRONTEND_PLGUI					"0"					CACHE BOOL "Build plugin 'PLFrontendPLGui'? (requires 'PLGui')")
	# Qt
	set (PL_PLUGIN_FRONTEND_QT						"0"					CACHE BOOL "Build plugin 'PLFrontendQt'?")
	# PLVolume
	set (PL_PLUGIN_VOLUME							"0"					CACHE BOOL "Build plugin 'PLVolume'?")
	set (PL_PLUGIN_VOLUME_RENDERER					"0"					CACHE BOOL "Build plugin 'PLVolumeRenderer'? (requires 'PLVolume')")
	set (PL_PLUGIN_VOLUME_GUI						"0"					CACHE BOOL "Build plugin 'PLVolumeGui'? (requires 'PLVolume')")
	set (PL_PLUGIN_VOLUME_LOADER_PVM				"0"					CACHE BOOL "Build plugin 'PLVolumeLoaderPVM'? (requires 'PLVolume')")
	set (PL_PLUGIN_VOLUME_LOADER_DICOM_GDCM			"0"					CACHE BOOL "Build plugin 'PLVolumeLoaderDICOM_GDCM'? (requires 'PLVolume')")
	# Tests and samples
	set (PL_SAMPLES									"0"					CACHE BOOL "Build the samples?")
	set (PL_TESTS									"0"					CACHE BOOL "Build the tests?")
elseif(CMAKETOOLS_BUILD_SDK)
	# SDK build
	# -> We don't use "${PL_USE_NONPUBLIC}" in here because this would result in required additional configurations
	set (PL_CORE_ZIP								"1"					CACHE BOOL "Build in ZIP support within 'PLCore'? (it's highly recommended to enable ZIP support, requires 'zlib' external dependency)")
	set (PL_PLUGIN_SCRIPT_NULL						"1"					CACHE BOOL "Build plugin 'PLScriptNull'?")
	set (PL_PLUGIN_SCRIPT_LUA						"1"					CACHE BOOL "Build plugin 'PLScriptLua'?")
	set (PL_PLUGIN_SCRIPT_V8						"0"					CACHE BOOL "Build plugin 'PLScriptV8'?")
	set (PL_PLUGIN_SCRIPT_PYTHON					"0"					CACHE BOOL "Build plugin 'PLScriptPython'?")
	set (PL_PLUGIN_SCRIPT_ANGELSCRIPT				"0"					CACHE BOOL "Build plugin 'PLScriptAngelScript'?")
	set (PL_PLUGIN_DATABASE_NULL					"1"					CACHE BOOL "Build plugin 'PLDatabaseNull'?")
	set (PL_PLUGIN_DATABASE_MYSQL					"0"					CACHE BOOL "Build plugin 'PLDatabaseMySQL'?")
	set (PL_PLUGIN_DATABASE_POSTGRESQL				"0"					CACHE BOOL "Build plugin 'PLDatabasePostgreSQL'?")
	set (PL_PLUGIN_DATABASE_SQLITE					"1"					CACHE BOOL "Build plugin 'PLDatabaseSQLite'?")
	set (PL_PLUGIN_PHYSICS_NULL						"1"					CACHE BOOL "Build plugin 'PLPhysicsNull'?")
	set (PL_PLUGIN_PHYSICS_NEWTON					"1"					CACHE BOOL "Build plugin 'PLPhysicsNewton'?")
	set (PL_PLUGIN_PHYSICS_ODE						"0"					CACHE BOOL "Build plugin 'PLPhysicsODE'?")
	set (PL_PLUGIN_PHYSICS_BULLET					"0"					CACHE BOOL "Build plugin 'PLPhysicsBullet'? (heavily under construction)")	# Disabled until the implementation is finished and useable
	set (PL_PLUGIN_PHYSICS_PHYSX					"0"					CACHE BOOL "Build plugin 'PLPhysicsPhysX'? (due to legal issues, we can't provide a public downloadable package)")
	set (PL_PLUGIN_RENDERER_D3D9					"0"					CACHE BOOL "Build plugin 'PLRendererD3D9'? (due to legal issues, we can't provide a public downloadable package)")
	set (PL_PLUGIN_RENDERER_D3D11					"0"					CACHE BOOL "Build plugin 'PLRendererD3D11'? (due to legal issues, we can't provide a public downloadable package)")
	set (PL_PLUGIN_FRONTEND_ACTIVEX					"0"					CACHE BOOL "Build plugin 'PLFrontendActiveX'?")
	set (PL_PLUGIN_FRONTEND_MOZILLA					"0"					CACHE BOOL "Build plugin 'PLFrontendMozilla'?")
	set (PL_EXPORTER_3DSMAX_2008					"1"					CACHE BOOL "Build exporter plugin 'PL3dsMaxSceneExport_2008'? (due to legal issues, we can't provide a public downloadable package)")	# ${PL_USE_NONPUBLIC}
	set (PL_EXPORTER_3DSMAX_2009					"1"					CACHE BOOL "Build exporter plugin 'PL3dsMaxSceneExport_2009'? (due to legal issues, we can't provide a public downloadable package)")	# ${PL_USE_NONPUBLIC}
	set (PL_EXPORTER_3DSMAX_2010					"1"					CACHE BOOL "Build exporter plugin 'PL3dsMaxSceneExport_2010'? (due to legal issues, we can't provide a public downloadable package)")	# ${PL_USE_NONPUBLIC}
	set (PL_EXPORTER_3DSMAX_2011					"1"					CACHE BOOL "Build exporter plugin 'PL3dsMaxSceneExport_2011'? (due to legal issues, we can't provide a public downloadable package)")	# ${PL_USE_NONPUBLIC}
	set (PL_EXPORTER_3DSMAX_2012					"1"					CACHE BOOL "Build exporter plugin 'PL3dsMaxSceneExport_2012'? (due to legal issues, we can't provide a public downloadable package)")	# ${PL_USE_NONPUBLIC}
	set (PL_EXPORTER_3DSMAX_2013					"1"					CACHE BOOL "Build exporter plugin 'PL3dsMaxSceneExport_2013'? (due to legal issues, we can't provide a public downloadable package)")	# ${PL_USE_NONPUBLIC}
	set (PL_PLUGIN_ENGINE_IMAGEEXR					"1"					CACHE BOOL "Build plugin 'PLImageLoaderEXR'?")
	set (PL_TOOL_PLINSTALL							"1"					CACHE BOOL "Build plugin 'PLInstall'?")
	set (PL_PLUGIN_RENDERER_NULL					"1"					CACHE BOOL "Build plugin 'PLRendererNull'?")
	set (PL_PLUGIN_RENDERER_OPENGL					"1"					CACHE BOOL "Build plugin 'PLRendererOpenGL'? (for font support, do also enable 'PL_PLUGIN_RENDERER_OPENGL_FONT')")
	set (PL_PLUGIN_RENDERER_OPENGL_FONT				"1"					CACHE BOOL "Build plugin 'PLRendererOpenGL' with font support? (requires 'freetype' external dependency)")
	set (PL_PLUGIN_RENDERER_OPENGLCG				"1"					CACHE BOOL "Build plugin 'PLRendererOpenGLCg'? (requires 'PLRendererOpenGL') (due to legal issues, we can't provide a public downloadable package)")	# ${PL_USE_NONPUBLIC}
	set (PL_PLUGIN_RENDERER_OPENGLES2				"1"					CACHE BOOL "Build plugin 'PLRendererOpenGLES2'? (for font support, do also enable 'PL_PLUGIN_RENDERER_OPENGLES2_FONT')")
	set (PL_PLUGIN_RENDERER_OPENGLES2_EMULATOR		"0"					CACHE BOOL "Build plugin 'PLRendererOpenGLES2' and add proprietary emulator? (do also enable 'PL_PLUGIN_RENDERER_OPENGLES2_FONT') (due to legal issues, we can't provide a public downloadable package)")
	set (PL_PLUGIN_RENDERER_OPENGLES2_FONT			"1"					CACHE BOOL "Build plugin 'PLRendererOpenGLES2' with font support? (requires 'freetype' external dependency)?")
	set (PL_PLUGIN_SOUND_NULL						"1"					CACHE BOOL "Build plugin 'PLSoundNull'?")
	set (PL_PLUGIN_SOUND_OPENAL						"1"					CACHE BOOL "Build plugin 'PLSoundOpenAL'?")
	set (PL_PLUGIN_SOUND_OPENSLES					"0"					CACHE BOOL "Build plugin 'PLSoundOpenSLES'? (heavily under construction)")
	set (PL_PLUGIN_SOUND_FMOD						"0"					CACHE BOOL "Build plugin 'PLSoundFMOD'? (due to legal issues, we can't provide a public downloadable package)")	# ${PL_USE_NONPUBLIC}
	set (PL_PLUGIN_SOUND_FMODEX						"0"					CACHE BOOL "Build plugin 'PLSoundFMODEx'? (due to legal issues, we can't provide a public downloadable package)")	# ${PL_USE_NONPUBLIC}
	set (PL_PLUGIN_ENGINE_COMPOSITING				"1"					CACHE BOOL "Build plugin 'PLCompositing'?")
	set (PL_PLUGIN_ENGINE_POSTPROCESS				"1"					CACHE BOOL "Build plugin 'PLPostProcessEffects'? (requires 'PLCompositing')")
	set (PL_PLUGIN_ENGINE_NATURE					"1"					CACHE BOOL "Build plugin 'PLNature'?")
	set (PL_PLUGIN_ENGINE_SCRIPTBINDINGS			"1"					CACHE BOOL "Build plugin 'PLScriptBindings'?")
	set (PL_PLUGIN_ENGINE_ASSIMP					"1"					CACHE BOOL "Build plugin 'PLAssimp'?")
	set (PL_PLUGIN_ENGINE_PARTICLEGROUPS			"1"					CACHE BOOL "Build plugin 'PLParticleGroups'?")
	set (PL_PLUGIN_ENGINE_SPARK						"1"					CACHE BOOL "Build plugin 'SPARK_PL'?")
	set (PL_PLUGIN_ENGINE_LIBROCKET					"1"					CACHE BOOL "Build plugin 'PLlibRocket'?")
	# PLGraphics image loaders
	set (PL_PLUGIN_GRAPHICS_PNG						"1"					CACHE BOOL "Build in 'png' support within 'PLGraphics'?")
	set (PL_PLUGIN_GRAPHICS_JPG						"1"					CACHE BOOL "Build in 'jpg' support within 'PLGraphics'?")
	# Network
	set (PL_PLUGIN_NETWORK_IRC						"1"					CACHE BOOL "Build plugin 'PLIRC'?")
	set (PL_PLUGIN_NETWORK_JABBER					"1"					CACHE BOOL "Build plugin 'PLJabber'?")
	# Frontend	
	set (PL_PLUGIN_FRONTEND_NULL					"1"					CACHE BOOL "Build plugin 'PLFrontendNull'?")
	set (PL_PLUGIN_FRONTEND_OS						"1"					CACHE BOOL "Build plugin 'PLFrontendOS'?")
	# PLGui
	set (PL_PLGUI									"1"					CACHE BOOL "Build 'PLGui'?")
	set (PL_PLUGIN_GUI_XMLTEXT						"1"					CACHE BOOL "Build plugin 'PLGuiXmlText'? (requires 'PLGui')")
	set (PL_PLUGIN_FRONTEND_PLGUI					"1"					CACHE BOOL "Build plugin 'PLFrontendPLGui'? (requires 'PLGui')")
	# Qt
	set (PL_PLUGIN_FRONTEND_QT						"1"					CACHE BOOL "Build plugin 'PLFrontendQt'?")
	# PLVolume
	set (PL_PLUGIN_VOLUME							"1"					CACHE BOOL "Build plugin 'PLVolume'?")
	set (PL_PLUGIN_VOLUME_RENDERER					"1"					CACHE BOOL "Build plugin 'PLVolumeRenderer'? (requires 'PLVolume')")
	set (PL_PLUGIN_VOLUME_GUI						"1"					CACHE BOOL "Build plugin 'PLVolumeGui'? (requires 'PLVolume' and 'PLFrontendQt')")
	set (PL_PLUGIN_VOLUME_LOADER_PVM				"1"					CACHE BOOL "Build plugin 'PLVolumeLoaderPVM'? (requires 'PLVolume')")
	set (PL_PLUGIN_VOLUME_LOADER_DICOM_GDCM			"0"					CACHE BOOL "Build plugin 'PLVolumeLoaderDICOM_GDCM'? (requires 'PLVolume')")
	# Tools
	set (PL_TOOL_PLUPGRADE							"1"					CACHE BOOL "Build plugin 'PLUpgrade'?")
	# Tests and samples
	set (PL_SAMPLES									"1"					CACHE BOOL "Build the samples?")
	set (PL_TESTS									"1"					CACHE BOOL "Build the tests?")
	
	# We want to include docs in the SDK
	set	(PL_DOCS									"1"					CACHE BOOL "Build the user documentation")
else()
	# Include everything
	set (PL_CORE_ZIP								"1"					CACHE BOOL "Build in ZIP support within 'PLCore'? (it's highly recommended to enable ZIP support, requires 'zlib' external dependency)")
	set (PL_PLUGIN_SCRIPT_NULL						"1"					CACHE BOOL "Build plugin 'PLScriptNull'?")
	set (PL_PLUGIN_SCRIPT_LUA						"1"					CACHE BOOL "Build plugin 'PLScriptLua'?")
	set (PL_PLUGIN_SCRIPT_V8						"1"					CACHE BOOL "Build plugin 'PLScriptV8'?")
	set (PL_PLUGIN_SCRIPT_PYTHON					"1"					CACHE BOOL "Build plugin 'PLScriptPython'?")
	set (PL_PLUGIN_SCRIPT_ANGELSCRIPT				"1"					CACHE BOOL "Build plugin 'PLScriptAngelScript'?")
	set (PL_PLUGIN_DATABASE_NULL					"1"					CACHE BOOL "Build plugin 'PLDatabaseNull'?")
	set (PL_PLUGIN_DATABASE_MYSQL					"1"					CACHE BOOL "Build plugin 'PLDatabaseMySQL'?")
	set (PL_PLUGIN_DATABASE_POSTGRESQL				"1"					CACHE BOOL "Build plugin 'PLDatabasePostgreSQL'?")
	set (PL_PLUGIN_DATABASE_SQLITE					"1"					CACHE BOOL "Build plugin 'PLDatabaseSQLite'?")
	set (PL_PLUGIN_PHYSICS_NULL						"1"					CACHE BOOL "Build plugin 'PLPhysicsNull'?")
	set (PL_PLUGIN_PHYSICS_NEWTON					"1"					CACHE BOOL "Build plugin 'PLPhysicsNewton'?")
	set (PL_PLUGIN_PHYSICS_ODE						"1"					CACHE BOOL "Build plugin 'PLPhysicsODE'?")
	set (PL_PLUGIN_PHYSICS_BULLET					"0"					CACHE BOOL "Build plugin 'PLPhysicsBullet'? (heavily under construction)")	# Disabled until the implementation is finished and useable
	set (PL_PLUGIN_PHYSICS_PHYSX					${PL_USE_NONPUBLIC}	CACHE BOOL "Build plugin 'PLPhysicsPhysX'? (due to legal issues, we can't provide a public downloadable package)")
	set (PL_PLUGIN_RENDERER_D3D9					${PL_USE_NONPUBLIC}	CACHE BOOL "Build plugin 'PLRendererD3D9'? (due to legal issues, we can't provide a public downloadable package)")
	set (PL_PLUGIN_RENDERER_D3D11					${PL_USE_NONPUBLIC}	CACHE BOOL "Build plugin 'PLRendererD3D11'? (due to legal issues, we can't provide a public downloadable package)")
	set (PL_PLUGIN_FRONTEND_ACTIVEX					"1"					CACHE BOOL "Build plugin 'PLFrontendActiveX'?")
	set (PL_PLUGIN_FRONTEND_MOZILLA					"1"					CACHE BOOL "Build plugin 'PLFrontendMozilla'?")
	set (PL_EXPORTER_3DSMAX_2008					${PL_USE_NONPUBLIC}	CACHE BOOL "Build exporter plugin 'PL3dsMaxSceneExport_2008'? (due to legal issues, we can't provide a public downloadable package)")
	set (PL_EXPORTER_3DSMAX_2009					${PL_USE_NONPUBLIC}	CACHE BOOL "Build exporter plugin 'PL3dsMaxSceneExport_2009'? (due to legal issues, we can't provide a public downloadable package)")
	set (PL_EXPORTER_3DSMAX_2010					${PL_USE_NONPUBLIC}	CACHE BOOL "Build exporter plugin 'PL3dsMaxSceneExport_2010'? (due to legal issues, we can't provide a public downloadable package)")
	set (PL_EXPORTER_3DSMAX_2011					${PL_USE_NONPUBLIC}	CACHE BOOL "Build exporter plugin 'PL3dsMaxSceneExport_2011'? (due to legal issues, we can't provide a public downloadable package)")
	set (PL_EXPORTER_3DSMAX_2012					${PL_USE_NONPUBLIC}	CACHE BOOL "Build exporter plugin 'PL3dsMaxSceneExport_2012'? (due to legal issues, we can't provide a public downloadable package)")
	set (PL_EXPORTER_3DSMAX_2013					${PL_USE_NONPUBLIC}	CACHE BOOL "Build exporter plugin 'PL3dsMaxSceneExport_2013'? (due to legal issues, we can't provide a public downloadable package)")
	set (PL_PLUGIN_ENGINE_IMAGEEXR					"1"					CACHE BOOL "Build plugin 'PLImageLoaderEXR'?")
	set (PL_TOOL_PLINSTALL							"1"					CACHE BOOL "Build plugin 'PLInstall'?")
	set (PL_PLUGIN_RENDERER_NULL					"1"					CACHE BOOL "Build plugin 'PLRendererNull'?")
	set (PL_PLUGIN_RENDERER_OPENGL					"1"					CACHE BOOL "Build plugin 'PLRendererOpenGL'? (for font support, do also enable 'PL_PLUGIN_RENDERER_OPENGL_FONT')")
	set (PL_PLUGIN_RENDERER_OPENGL_FONT				"1"					CACHE BOOL "Build plugin 'PLRendererOpenGL' with font support? (requires 'freetype' external dependency)")
	set (PL_PLUGIN_RENDERER_OPENGLCG				"1"					CACHE BOOL "Build plugin 'PLRendererOpenGLCg'? (requires 'PLRendererOpenGL') (due to legal issues, we can't provide a public downloadable package)")
	set (PL_PLUGIN_RENDERER_OPENGLES2				"1"					CACHE BOOL "Build plugin 'PLRendererOpenGLES2'? (for font support, do also enable 'PL_PLUGIN_RENDERER_OPENGLES2_FONT')")
	set (PL_PLUGIN_RENDERER_OPENGLES2_EMULATOR		${PL_USE_NONPUBLIC}	CACHE BOOL "Build plugin 'PLRendererOpenGLES2' and add proprietary emulator? (do also enable 'PL_PLUGIN_RENDERER_OPENGLES2_FONT') (due to legal issues, we can't provide a public downloadable package)")
	set (PL_PLUGIN_RENDERER_OPENGLES2_FONT			"1"					CACHE BOOL "Build plugin 'PLRendererOpenGLES2' with font support? (requires 'freetype' external dependency)?")
	set (PL_PLUGIN_SOUND_NULL						"1"					CACHE BOOL "Build plugin 'PLSoundNull'?")
	set (PL_PLUGIN_SOUND_OPENAL						"1"					CACHE BOOL "Build plugin 'PLSoundOpenAL'?")
	set (PL_PLUGIN_SOUND_OPENSLES					"1"					CACHE BOOL "Build plugin 'PLSoundOpenSLES'? (heavily under construction)")
	set (PL_PLUGIN_SOUND_FMOD						${PL_USE_NONPUBLIC}	CACHE BOOL "Build plugin 'PLSoundFMOD'? (due to legal issues, we can't provide a public downloadable package)")
	set (PL_PLUGIN_SOUND_FMODEX						${PL_USE_NONPUBLIC}	CACHE BOOL "Build plugin 'PLSoundFMODEx'? (due to legal issues, we can't provide a public downloadable package)")
	set (PL_PLUGIN_ENGINE_COMPOSITING				"1"					CACHE BOOL "Build plugin 'PLCompositing'?")
	set (PL_PLUGIN_ENGINE_POSTPROCESS				"1"					CACHE BOOL "Build plugin 'PLPostProcessEffects'? (requires 'PLCompositing' and Cg support)")
	set (PL_PLUGIN_ENGINE_NATURE					"1"					CACHE BOOL "Build plugin 'PLNature'?")
	set (PL_PLUGIN_ENGINE_SCRIPTBINDINGS			"1"					CACHE BOOL "Build plugin 'PLScriptBindings'?")
	set (PL_PLUGIN_ENGINE_ASSIMP					"1"					CACHE BOOL "Build plugin 'PLAssimp'?")
	set (PL_PLUGIN_ENGINE_PARTICLEGROUPS			"1"					CACHE BOOL "Build plugin 'PLParticleGroups'?")
	set (PL_PLUGIN_ENGINE_SPARK						"1"					CACHE BOOL "Build plugin 'SPARK_PL'?")
	set (PL_PLUGIN_ENGINE_LIBROCKET					"1"					CACHE BOOL "Build plugin 'PLlibRocket'?")
	# PLGraphics image loaders
	set (PL_PLUGIN_GRAPHICS_PNG						"1"					CACHE BOOL "Build in 'png' support within 'PLGraphics'?")
	set (PL_PLUGIN_GRAPHICS_JPG						"1"					CACHE BOOL "Build in 'jpg' support within 'PLGraphics'?")
	# Network
	set (PL_PLUGIN_NETWORK_IRC						"1"					CACHE BOOL "Build plugin 'PLIRC'?")
	set (PL_PLUGIN_NETWORK_JABBER					"1"					CACHE BOOL "Build plugin 'PLJabber'?")
	# Frontend	
	set (PL_PLUGIN_FRONTEND_NULL					"1"					CACHE BOOL "Build plugin 'PLFrontendNull'?")
	set (PL_PLUGIN_FRONTEND_OS						"1"					CACHE BOOL "Build plugin 'PLFrontendOS'?")
	# PLGui
	set (PL_PLGUI									"1"					CACHE BOOL "Build 'PLGui'?")
	set (PL_PLUGIN_GUI_XMLTEXT						"1"					CACHE BOOL "Build plugin 'PLGuiXmlText'? (requires 'PLGui')")
	set (PL_PLUGIN_FRONTEND_PLGUI					"1"					CACHE BOOL "Build plugin 'PLFrontendPLGui'? (requires 'PLGui')")
	# Qt
	set (PL_PLUGIN_FRONTEND_QT						"1"					CACHE BOOL "Build plugin 'PLFrontendQt'?")
	# PLVolume
	set (PL_PLUGIN_VOLUME							"1"					CACHE BOOL "Build plugin 'PLVolume'?")
	set (PL_PLUGIN_VOLUME_RENDERER					"1"					CACHE BOOL "Build plugin 'PLVolumeRenderer'? (requires 'PLVolume')")
	set (PL_PLUGIN_VOLUME_GUI						"1"					CACHE BOOL "Build plugin 'PLVolumeGui'? (requires 'PLVolume' and 'PLFrontendQt')")
	set (PL_PLUGIN_VOLUME_LOADER_PVM				"1"					CACHE BOOL "Build plugin 'PLVolumeLoaderPVM'? (requires 'PLVolume')")
	set (PL_PLUGIN_VOLUME_LOADER_DICOM_GDCM			"1"					CACHE BOOL "Build plugin 'PLVolumeLoaderDICOM_GDCM'? (requires 'PLVolume')")
	# Tools
	set (PL_TOOL_PLUPGRADE							"1"					CACHE BOOL "Build plugin 'PLUpgrade'?")
	# Tests and samples
	set (PL_SAMPLES									"1"					CACHE BOOL "Build the samples?")
	set (PL_TESTS									"1"					CACHE BOOL "Build the tests?")
	
	# Documentation
	set	(PL_DOCS									"1"					CACHE BOOL "Build the user documentation")
endif()

##################################################
## Disable unsupported plugins
##################################################
unset (PL_EXPORTER_3DSMAX_2008 CACHE)
unset (PL_EXPORTER_3DSMAX_2009 CACHE)
unset (PL_EXPORTER_3DSMAX_2010 CACHE)
unset (PL_EXPORTER_3DSMAX_2011 CACHE)
unset (PL_EXPORTER_3DSMAX_2012 CACHE)
unset (PL_EXPORTER_3DSMAX_2013 CACHE)
unset (PL_PLUGIN_FRONTEND_ACTIVEX CACHE)
unset (PL_PLUGIN_RENDERER_D3D9 CACHE)
unset (PL_PLUGIN_RENDERER_D3D11 CACHE)
unset (PL_PLUGIN_PHYSICS_PHYSX CACHE)

##################################################
## Disable plugins that will be enabled in later version
##################################################
unset (PL_PLUGIN_SCRIPT_V8 CACHE)
unset (PL_PLUGIN_SCRIPT_PYTHON CACHE)
unset (PL_PLUGIN_SCRIPT_ANGELSCRIPT	CACHE)
unset (PL_PLUGIN_DATABASE_POSTGRESQL CACHE)
unset (PL_PLUGIN_DATABASE_SQLITE CACHE)
unset (PL_PLUGIN_PHYSICS_ODE CACHE)
unset (PL_PLUGIN_FRONTEND_MOZILLA CACHE)
unset (PL_PLUGIN_ENGINE_IMAGEEXR CACHE)
unset (PL_PLUGIN_RENDERER_OPENGLES2_EMULATOR CACHE)
unset (PL_PLUGIN_SOUND_OPENSLES CACHE)
unset (PL_PLUGIN_SOUND_FMOD CACHE)
unset (PL_PLUGIN_SOUND_FMODEX CACHE)
unset (PL_PLUGIN_ENGINE_SPARK CACHE)
unset (PL_PLUGIN_ENGINE_LIBROCKET CACHE)
unset (PL_PLUGIN_NETWORK_IRC CACHE)
unset (PL_PLUGIN_NETWORK_JABBER CACHE)
unset (PL_PLUGIN_VOLUME_LOADER_PVM CACHE)
unset (PL_PLUGIN_VOLUME_LOADER_DICOM_GDCM CACHE)

##################################################
## Temporarily disable projects that don't build yet
##################################################
unset (PL_PLUGIN_DATABASE_MYSQL CACHE)
#unset (PL_PLUGIN_PHYSICS_BULLET CACHE)

unset (PL_TESTS CACHE)
unset (PL_DOCS CACHE)

##################################################
## Remove optional projects not available on a certain platform
##################################################
# X86_64 aka x64
if(X86_64)
	# We don't support the legacy FMOD for x64, use the new version FMODEx instead
	unset (PL_PLUGIN_SOUND_FMOD						CACHE)

	# (MS Windows) We don't support the legacy 3ds Max 2008 for x64, use a newer version instead
	unset (PL_EXPORTER_3DSMAX_2008					CACHE)

	# There's no 64 bit version available of the OpenGL ES 2.0 Emulator from ARM (http://www.malideveloper.com/tools/software-development/opengl-es-20-emulator.php)
	unset (PL_PLUGIN_RENDERER_OPENGLES2_EMULATOR	CACHE)

	# [TODO] The following plugins are not yet supported (create prebuild external packages and test it)
	# Disable only when not building an 32bit target
	if(LINUX AND NOT APPLE AND NOT ANDROID AND NOT CMAKETOOLS_TARGET_ARCHBITSIZE MATCHES "x86")
		# Linux x64
		unset (PL_PLUGIN_SCRIPT_V8					CACHE)
		unset (PL_PLUGIN_SCRIPT_PYTHON				CACHE)
		unset (PL_PLUGIN_SCRIPT_ANGELSCRIPT			CACHE)
		unset (PL_PLUGIN_DATABASE_MYSQL				CACHE)
		unset (PL_PLUGIN_DATABASE_POSTGRESQL		CACHE)
		unset (PL_PLUGIN_PHYSICS_ODE				CACHE)
	endif()
endif()

# Not Windows
if(NOT WIN32)
	# Windows only features, for other targets, don't show this options
	unset (PL_PLUGIN_PHYSICS_PHYSX					CACHE)
	unset (PL_PLUGIN_RENDERER_D3D9					CACHE)
	unset (PL_PLUGIN_RENDERER_D3D11					CACHE)
	unset (PL_PLUGIN_FRONTEND_ACTIVEX				CACHE)
	unset (PL_PLUGIN_FRONTEND_MOZILLA				CACHE)
	unset (PL_EXPORTER_3DSMAX_2008					CACHE)
	unset (PL_EXPORTER_3DSMAX_2009					CACHE)
	unset (PL_EXPORTER_3DSMAX_2010					CACHE)
	unset (PL_EXPORTER_3DSMAX_2011					CACHE)
	unset (PL_EXPORTER_3DSMAX_2012					CACHE)
	unset (PL_EXPORTER_3DSMAX_2013					CACHE)
	unset (PL_PLUGIN_ENGINE_IMAGEEXR				CACHE)
	unset (PL_TOOL_PLINSTALL						CACHE)

	# [TODO] The following plugins are not yet supported (create prebuild external packages and test it)
	unset (PL_PLUGIN_VOLUME_LOADER_DICOM_GDCM		CACHE)
elseif(X86_64)
	# Windows x64

	# [TODO] The following plugins are not yet supported (create prebuild external packages and test it)
	unset (PL_PLUGIN_DATABASE_POSTGRESQL			CACHE)
	unset (PL_PLUGIN_PHYSICS_PHYSX					CACHE)
endif()

# Mac OS X
if(APPLE)
	# Remove features not supported or required for Mac OS X

	# [TODO] The following plugins are not yet supported (create prebuild external packages and test it)
	unset (PL_PLUGIN_SCRIPT_LUA						CACHE)
	unset (PL_PLUGIN_SCRIPT_V8						CACHE)
	unset (PL_PLUGIN_SCRIPT_PYTHON					CACHE)
	unset (PL_PLUGIN_SCRIPT_ANGELSCRIPT				CACHE)
	unset (PL_PLUGIN_DATABASE_MYSQL					CACHE)
	unset (PL_PLUGIN_DATABASE_POSTGRESQL			CACHE)
	unset (PL_PLUGIN_DATABASE_SQLITE				CACHE)
	unset (PL_PLUGIN_PHYSICS_NEWTON					CACHE)
	unset (PL_PLUGIN_PHYSICS_ODE					CACHE)
	unset (PL_PLUGIN_PHYSICS_BULLET					CACHE)
	unset (PL_PLUGIN_PHYSICS_ODE					CACHE)
	unset (PL_PLUGIN_ENGINE_IMAGEEXR				CACHE)
	unset (PL_PLUGIN_RENDERER_OPENGL_FONT			CACHE)
	unset (PL_PLUGIN_RENDERER_OPENGLCG				CACHE)
	unset (PL_PLUGIN_RENDERER_OPENGLES2_EMULATOR	CACHE)
	unset (PL_PLUGIN_RENDERER_OPENGLES2_FONT		CACHE)
	unset (PL_PLUGIN_SOUND_OPENAL					CACHE)
	unset (PL_PLUGIN_SOUND_FMOD						CACHE)
	unset (PL_PLUGIN_SOUND_FMODEX					CACHE)
	unset (PL_PLUGIN_ENGINE_ASSIMP					CACHE)
	unset (PL_PLUGIN_ENGINE_LIBROCKET				CACHE)
	unset (PL_PLUGIN_VOLUME_LOADER_DICOM_GDCM		CACHE)
	unset (PL_PLUGIN_GRAPHICS_PNG					CACHE)
	unset (PL_PLUGIN_GRAPHICS_JPG					CACHE)
	unset (PL_PLUGIN_FRONTEND_QT					CACHE)
endif()

# Android
if(ANDROID)
	# Remove features not supported or required for Android
	unset (PL_PLUGIN_RENDERER_OPENGL				CACHE)
	unset (PL_PLUGIN_RENDERER_OPENGL_FONT			CACHE)
	unset (PL_PLUGIN_RENDERER_OPENGLCG				CACHE)
	unset (PL_PLUGIN_RENDERER_OPENGLES2_EMULATOR	CACHE)
	unset (PL_PLUGIN_SOUND_OPENAL					CACHE)
	unset (PL_PLUGIN_SOUND_FMOD						CACHE)
	unset (PL_PLUGIN_FRONTEND_QT					CACHE)
	unset (PL_TOOL_PLUPGRADE						CACHE)

	# [TODO] PLVolume: There's currently no GLSL ES 2.0 support within the volume renderer
	unset (PL_PLUGIN_VOLUME							CACHE)
	unset (PL_PLUGIN_VOLUME_RENDERER				CACHE)
	unset (PL_PLUGIN_VOLUME_GUI						CACHE)
	unset (PL_PLUGIN_VOLUME_LOADER_PVM				CACHE)
	unset (PL_PLUGIN_VOLUME_LOADER_DICOM_GDCM		CACHE)

	# [TODO] The following plugins are not yet supported (create prebuild external packages and test it)
	unset (PL_PLUGIN_SCRIPT_V8						CACHE)
	unset (PL_PLUGIN_SCRIPT_PYTHON					CACHE)
	unset (PL_PLUGIN_SCRIPT_ANGELSCRIPT				CACHE)	# Experimental package is already there, but there are still linker issues to be solved
	unset (PL_PLUGIN_DATABASE_MYSQL					CACHE)
	unset (PL_PLUGIN_DATABASE_POSTGRESQL			CACHE)
	unset (PL_PLUGIN_DATABASE_SQLITE				CACHE)
	unset (PL_PLUGIN_PHYSICS_ODE					CACHE)
	unset (PL_PLUGIN_ENGINE_IMAGEEXR				CACHE)
	unset (PL_PLUGIN_VOLUME_LOADER_DICOM_GDCM		CACHE)
else()
	# Remove Android only features
	unset (PL_PLUGIN_SOUND_OPENSLES					CACHE)
endif()


##################################################
## Validation
##################################################
# PLPostProcessEffects requires Cg support
if(NOT ANDROID AND PL_PLUGIN_ENGINE_POSTPROCESS AND NOT PL_PLUGIN_RENDERER_OPENGLCG)
	message(WARNING "PL_PLUGIN_ENGINE_POSTPROCESS needs PL_PLUGIN_RENDERER_OPENGLCG -> Postprocess effects might not work")
endif()


##################################################
## Extensions of files to exclude when e.g. copying a directory
##################################################
# sdf	= VisualStudio 2010 file
# ncb	= VisualStudio 2010 file
# suo	= VisualStudio 2010 file
# user	= VisualStudio 2010 file
# *.*~	= Backup file ending used by some editors under Linux
set(PL_EXCLUDE_FILE_EXTENSIONS ".*\\.(sdf|ncb|suo|user|.*~")							# Set opening bracket as well as some standard unwanted file extensions
if(NOT WIN32)
	set(PL_EXCLUDE_FILE_EXTENSIONS "${PL_EXCLUDE_FILE_EXTENSIONS}|sln|vcxproj|filters")	# VisualStudio 2010 files only for MS Windows, please
endif()
if(NOT PL_PLUGIN_SCRIPT_LUA)
	set(PL_EXCLUDE_FILE_EXTENSIONS "${PL_EXCLUDE_FILE_EXTENSIONS}|lua")					# Lua
endif()
if(NOT PL_PLUGIN_SCRIPT_V8)
	set(PL_EXCLUDE_FILE_EXTENSIONS "${PL_EXCLUDE_FILE_EXTENSIONS}|js")					# JavaScript
endif()
if(NOT PL_PLUGIN_SCRIPT_PYTHON)
	set(PL_EXCLUDE_FILE_EXTENSIONS "${PL_EXCLUDE_FILE_EXTENSIONS}|py")					# Python
endif()
if(NOT PL_PLUGIN_SCRIPT_ANGELSCRIPT)
	set(PL_EXCLUDE_FILE_EXTENSIONS "${PL_EXCLUDE_FILE_EXTENSIONS}|as")					# AngelScript
endif()
set(PL_EXCLUDE_FILE_EXTENSIONS "${PL_EXCLUDE_FILE_EXTENSIONS})")						# Set closing bracket
message(STATUS "Extensions of files to exclude when e.g. copying a directory: ${PL_EXCLUDE_FILE_EXTENSIONS}")


##################################################
## Macros
##################################################

##################################################
## MACRO: call plproject
##
## Call plproject tool
##################################################
macro(pl_plproject dir outputpath writeplugin)
	add_dependencies(${CMAKETOOLS_CURRENT_TARGET} PLProject)
	add_custom_command(TARGET ${CMAKETOOLS_CURRENT_TARGET}
		COMMAND ${PL_PLPROJECT_COMMAND} . --output-path ${outputpath} --write-plugin ${writeplugin} ${ARGN}
		WORKING_DIRECTORY ${dir}
	)
endmacro(pl_plproject dir outputpath writeplugin)

##################################################
## MACRO: pl_create_plugin_description
##
## Create PixelLight plugin description
##################################################
macro(pl_create_plugin_description src dest)
	if(PL_PLPROJECT_COMMAND)
		if(CMAKETOOLS_CONFIG_SUFFIX)
			set(suffix --suffix ${CMAKETOOLS_CONFIG_SUFFIX})
		else()
			set(suffix "")
		endif()

		if("${ARGV2}" STREQUAL "")
			set(entryFile "")
		else()
			set(entryFile --entry-file ${ARGV2})
		endif()

		# Call PLProject
		pl_plproject(${src} ${dest} "${suffix}" "${entryFile}")
	endif()
endmacro(pl_create_plugin_description src dest)


##################################################
## Initialization
##################################################

# Set path to PLProject
# -> No plugin files required because for Android, we need to load in all shared libraries right at the beginning
if(NOT ANDROID)
	if(PL_NATIVE_PLPROJECT)
		if(CMAKE_BUILD_TYPE MATCHES "Debug")
			set(PL_PLPROJECT_COMMAND "${CMAKE_SOURCE_DIR}/Bin-${CMAKETOOLS_HOST_SYSTEM}/Tools/${CMAKETOOLS_TARGET_ARCHBITSIZE}/PLProjectD${CMAKE_EXECUTABLE_SUFFIX}")
		else()
			set(PL_PLPROJECT_COMMAND "${CMAKE_SOURCE_DIR}/Bin-${CMAKETOOLS_HOST_SYSTEM}/Tools/${CMAKETOOLS_TARGET_ARCHBITSIZE}/PLProject${CMAKE_EXECUTABLE_SUFFIX}")
		endif()
	else()
		set(PL_PLPROJECT_COMMAND "${CMAKE_SOURCE_DIR}/Tools/PLProject/src/PLProject.py")
	endif()
endif()

# Configure use of inline assembly
if(NOT CMAKETOOLS_TARGET_ARCH STREQUAL "x86")
	set(CMAKETOOLS_CONFIG_NO_INLINE_ASM "1")
endif()
if(NOT CMAKETOOLS_TARGET_BITSIZE STREQUAL "32")
	set(CMAKETOOLS_CONFIG_NO_INLINE_ASM "1")
endif()

##################################################
## MSVC hack - late search for VC tools
##################################################

#TODO: how to solve this???

# Try to find nmake
if(NOT NMAKE_FOUND)
	# Search for nmake
	find_program(NMAKE_EXECUTABLE NAMES nmake.exe)
	if(NMAKE_EXECUTABLE)
		set(NMAKE_FOUND ON)
		message(STATUS "NMAKE found")
	else()
		set(NMAKE_FOUND OFF)
		message(STATUS "NMAKE NOT found")
	endif()
endif()

# Try to find vcbuild
if(NOT VCBUILD_FOUND)
	# Get path hint based on location of nmake
	set(path_hint)
	if(NMAKE_FOUND)
		get_filename_component(path1 ${NMAKE_EXECUTABLE} PATH)
		get_filename_component(path2 ${path1} PATH)
		set(path_hint "${path2}/vcpackages")
	endif()

	# Search for vcbuild
	find_program(VCBUILD_EXECUTABLE NAMES vcbuild.exe PATHS ${path_hint})
	if(VCBUILD_EXECUTABLE)
		set(VCBUILD_FOUND ON)
		message(STATUS "VCBUILD found")
	else()
		set(VCBUILD_FOUND OFF)
		message(STATUS "VCBUILD NOT found")
	endif()
endif()