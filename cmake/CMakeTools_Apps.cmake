#*********************************************************#
#*  File: CMakeTools_Apps.cmake                          *
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
## CMakeTools applications and libraries
##
## These macros can be used to define C++ projects,
## such as applications and libraries.
##################################################


##################################################
## MACRO: set_target_name
##
## Set or change target name
##   name		name of the target
##   NOSUFFIX	set if no suffix should be set for this project
##################################################
macro(set_target_name name)
	# Set target name (e.g. "MyLib")
	set(CMAKETOOLS_CURRENT_TARGET ${name})
	cmaketools_debug_variable(CMAKETOOLS_CURRENT_TARGET)

	# Set project suffix
	set(CMAKETOOLS_CURRENT_SUFFIX "")
	if(NOT "${ARGV1}" STREQUAL "NOSUFFIX")
		set(CMAKETOOLS_CURRENT_SUFFIX ${CMAKETOOLS_CONFIG_SUFFIX})
	endif()
	cmaketools_debug_variable(CMAKETOOLS_CURRENT_SUFFIX)

	# Get actual suffix ("-<suffix>")
	set(suffix "")
	if(CMAKETOOLS_CURRENT_SUFFIX)
		set(suffix "-${CMAKETOOLS_CURRENT_SUFFIX}")
	endif()

	# Determine output name and add suffixes
	set(CMAKETOOLS_CURRENT_OUTPUT_NAME ${name})
	set(CMAKETOOLS_CURRENT_OUTPUT_NAME ${CMAKETOOLS_CURRENT_OUTPUT_NAME}${CMAKETOOLS_CONFIG_DEBUG_SUFFIX}${suffix})
	cmaketools_debug_variable(CMAKETOOLS_CURRENT_OUTPUT_NAME)

	# Set current project variables
	set(CMAKETOOLS_CURRENT_OUTPUT_DIR  			 "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}")
	set(CMAKETOOLS_CURRENT_EXECUTABLE  			  ${CMAKETOOLS_CURRENT_OUTPUT_NAME}${CMAKE_EXECUTABLE_SUFFIX})
	set(CMAKETOOLS_CURRENT_SHARED_LIB  			  ${CMAKE_SHARED_LIBRARY_PREFIX}${CMAKETOOLS_CURRENT_OUTPUT_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})
	set(CMAKETOOLS_CURRENT_STATIC_LIB  			  ${CMAKE_STATIC_LIBRARY_PREFIX}${CMAKETOOLS_CURRENT_OUTPUT_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX})
	set(CMAKETOOLS_CURRENT_PLUGIN      			  ${name}.plugin)
	set(CMAKETOOLS_CURRENT_SHARED_LIB_VERSION  	  ${CMAKETOOLS_CURRENT_SHARED_LIB}.${CMAKETOOLS_CURRENT_VERSION_MAJOR})
	set(CMAKETOOLS_CURRENT_SHARED_LIB_SO_VERSION  ${CMAKETOOLS_CURRENT_SHARED_LIB}.${CMAKETOOLS_CURRENT_VERSION_MAJOR}.${CMAKETOOLS_CURRENT_VERSION_MINOR}.${CMAKETOOLS_CURRENT_VERSION_PATCH})
	set(CMAKETOOLS_CURRENT_EXT_DIR               "${CMAKE_SOURCE_DIR}/External/_${CMAKETOOLS_CONFIG_NAME}_${CMAKETOOLS_TARGET_ARCH}_${CMAKETOOLS_TARGET_BITSIZE}")

	# Set current project version
	set(CMAKETOOLS_CURRENT_VERSION_MAJOR	${CMAKETOOLS_PROJECT_VERSION_MAJOR})
	set(CMAKETOOLS_CURRENT_VERSION_MINOR	${CMAKETOOLS_PROJECT_VERSION_MINOR})
	set(CMAKETOOLS_CURRENT_VERSION_PATCH	${CMAKETOOLS_PROJECT_VERSION_PATCH})
endmacro(set_target_name name)

##################################################
## MACRO: define_project
##
## Define a project
##   name		name of the project (e.g. "TestProject")
##   NOSUFFIX	set if no suffix should be set for this project
##################################################
macro(define_project name)
	# Set project name (e.g. "MyLib")
	set(CMAKETOOLS_CURRENT_PROJECT ${name})
	cmaketools_debug_variable(CMAKETOOLS_CURRENT_PROJECT)

	# Set target name
	set_target_name(${name} ${ARGV1} ${ARGV2})

	# Begin project
	project(${CMAKETOOLS_CURRENT_TARGET} C CXX ${USE_RC_COMPILER})

	# Initialize project lists
	set(CMAKETOOLS_CURRENT_SOURCES        "")
	set(CMAKETOOLS_CURRENT_LIBS           "")
	set(CMAKETOOLS_CURRENT_COMPILE_DEFS   "")
	set(CMAKETOOLS_CURRENT_COMPILE_FLAGS " ")
	set(CMAKETOOLS_CURRENT_LINK_FLAGS    " ")

	# Disable inline assembler codes if cross-compiling for another architecture
	if(CMAKETOOLS_CONFIG_NO_INLINE_ASM)
		set(CMAKETOOLS_CURRENT_COMPILE_DEFS ${CMAKETOOLS_CURRENT_COMPILE_DEFS} NO_INLINE_ASM)
	endif()

	# Reset all default compiler and linker flags as we want the FULL control over these
	if(NOT CMAKETOOLS_CONFIG_NO_FULL_CONTROL)
		set(CMAKE_CXX_FLAGS						"")
		set(CMAKE_CXX_FLAGS_RELEASE				"")
		set(CMAKE_CXX_FLAGS_DEBUG				"")
		set(CMAKE_C_FLAGS						"")
		set(CMAKE_C_FLAGS_RELEASE				"")
		set(CMAKE_C_FLAGS_DEBUG					"")
		set(CMAKE_EXE_LINKER_FLAGS				"")
		set(CMAKE_EXE_LINKER_FLAGS_RELEASE		"")
		set(CMAKE_EXE_LINKER_FLAGS_DEBUG		"")
		set(CMAKE_SHARED_LINKER_FLAGS			"")
		set(CMAKE_SHARED_LINKER_FLAGS_RELEASE	"")
		set(CMAKE_SHARED_LINKER_FLAGS_DEBUG		"")
	endif()
endmacro(define_project name)

##################################################
## MACRO: set_project_version
##
## Set current project version
##   major		Major version
##   minor		Minor version
##   patch		Patch number
##################################################
macro(set_project_version major minor patch)
	# Set current project version
	set(CMAKETOOLS_CURRENT_VERSION_MAJOR	${major})
	set(CMAKETOOLS_CURRENT_VERSION_MINOR	${minor})
	set(CMAKETOOLS_CURRENT_VERSION_PATCH	${patch})
endmacro(set_project_version major minor patch)

##################################################
## MACRO: add_sources
##
## Add source files to the project
##################################################
macro(add_sources)
	add_to_list(CMAKETOOLS_CURRENT_SOURCES ${ARGN})
endmacro(add_sources)

##################################################
## MACRO: remove_sources
##
## Remove source files from the project
##################################################
macro(remove_sources)
  	remove_from_list(CMAKETOOLS_CURRENT_SOURCES ${ARGN})
endmacro(remove_sources)

##################################################
## MACRO: add_include_directories
##
## Add include directories to the project
##################################################
macro(add_include_directories)
	include_directories(${ARGN})
endmacro(add_include_directories)

##################################################
## MACRO: add_link_directories
##
## Add link directories to the project
##################################################
macro(add_link_directories)
	link_directories(${ARGN})
endmacro(add_link_directories)

##################################################
## MACRO: add_link_directories
##
## Add lib directories to the project
##################################################
macro(add_link_directories)
	link_directories(${ARGN})
endmacro(add_link_directories)

##################################################
## MACRO: add_libs
##
## Add external libraries to the project
##################################################
macro(add_libs)
	add_to_list(CMAKETOOLS_CURRENT_LIBS ${ARGN})
endmacro(add_libs)

##################################################
## MACRO: remove_libs
##
## Remove external libraries from the project
##################################################
macro(remove_libs)
	remove_from_list(CMAKETOOLS_CURRENT_LIBS ${ARGN})
endmacro(remove_libs)

##################################################
## MACRO: add_compile_defs
##
## Add compiler definitions
##################################################
macro(add_compile_defs)
	add_to_list(CMAKETOOLS_CURRENT_COMPILE_DEFS ${ARGN})
endmacro(add_compile_defs)

##################################################
## MACRO: remove_compile_defs
##
## Remove compiler definitions
##################################################
macro(remove_compile_defs)
	remove_from_list(CMAKETOOLS_CURRENT_COMPILE_DEFS ${ARGN})
endmacro(remove_compile_defs)

##################################################
## MACRO: add_compile_flags
##
## Add compiler flags or options
##################################################
macro(add_compile_flags)
	add_to_string(CMAKETOOLS_CURRENT_COMPILE_FLAGS ${ARGN})
endmacro(add_compile_flags)

##################################################
## MACRO: remove_compile_flags
##
## Remove compiler flags or options
##################################################
macro(remove_compile_flags)
	remove_from_list(CMAKETOOLS_CURRENT_COMPILE_FLAGS ${ARGN})
endmacro(remove_compile_flags)

##################################################
## MACRO: add_linker_flags
##
## Add linker flags
##################################################
macro(add_linker_flags)
	add_to_string(CMAKETOOLS_CURRENT_LINK_FLAGS ${ARGN})
endmacro(add_linker_flags)

##################################################
## MACRO: remove_linker_flags
##
## Remove linker flags
##################################################
macro(remove_linker_flags)
	remove_from_string(CMAKETOOLS_CURRENT_LINK_FLAGS ${ARGN})
endmacro(remove_linker_flags)

##################################################
## MACRO: cmaketools_set_properties
##
## Set the propertiers of the current project
##################################################
macro(cmaketools_set_properties target)
   	set_target_properties(${target} PROPERTIES OUTPUT_NAME ${CMAKETOOLS_CURRENT_OUTPUT_NAME})
   	set_target_properties(${target} PROPERTIES COMPILE_DEFINITIONS "${CMAKETOOLS_CURRENT_COMPILE_DEFS}")
   	set_target_properties(${target} PROPERTIES COMPILE_FLAGS ${CMAKETOOLS_CURRENT_COMPILE_FLAGS})
   	set_target_properties(${target} PROPERTIES LINK_FLAGS ${CMAKETOOLS_CURRENT_LINK_FLAGS})
   	#set_target_properties(${target} PROPERTIES LINK_INTERFACE_LIBRARIES "")
	if(CMAKETOOLS_DETAILED_VERSION_INFORMATION)
		set_target_properties(${target} PROPERTIES VERSION "${CMAKETOOLS_CURRENT_VERSION_MAJOR}.${CMAKETOOLS_CURRENT_VERSION_MINOR}.${CMAKETOOLS_CURRENT_VERSION_PATCH}")
		set_target_properties(${target} PROPERTIES SOVERSION ${CMAKETOOLS_CURRENT_VERSION_MAJOR})
	endif()
endmacro(cmaketools_set_properties target)

##################################################
## MACRO: build_library
##
## Build a library in the current project
##   name   Name of target
##   type	Type of library ('STATIC' or 'SHARED')
##################################################
macro(build_library name type)
	# Overwrite target name
	set_target_name(${name})

	# Set library type
	if("${type}" STREQUAL "")
		set(type "STATIC")
	endif()

	# Set/unset build options for shared libraries (only required for MS Windows, but we
	# set at least the definitions to be able to use those definitions within source
	# codes -> we just use the MS Windows names so that we don't have to invent a
	# new definition for this purpose)
	if("${type}" STREQUAL "SHARED")
		# DLL
		add_compile_defs(_WINDLL _USRDLL)
		if(WIN32)
			add_linker_flags(/DLL)
		endif()
	elseif("${type}" STREQUAL "STATIC")
		# Static library
		remove_compile_defs(_WINDLL _USRDLL)
		if(WIN32)
			remove_linker_flags(/DLL)
		endif()
	endif()

	# Get name
	if(NOT "${ARGV1}" STREQUAL "")
	endif()

	# Build library
	add_library(${CMAKETOOLS_CURRENT_TARGET} ${type} ${CMAKETOOLS_CURRENT_SOURCES})
	target_link_libraries(${CMAKETOOLS_CURRENT_TARGET} ${CMAKETOOLS_CURRENT_LIBS})
	cmaketools_set_properties(${CMAKETOOLS_CURRENT_TARGET})
endmacro(build_library name type)

##################################################
## MACRO: build_executable
##
## Build an executable in the current project
##   name       Name of target
##   subsystem	Name of used subsystem ('WIN32' or 'CONSOLE', ignored on Linux)
##################################################
macro(build_executable name subsystem)
	# Overwrite target name
	set_target_name(${name})

	# Check subsystem type on Windows
	if(WIN32)
		# Set subsystem type
		if("${subsystem}" STREQUAL "WIN32")
			set(subsys "WIN32")
		else()
			set(subsys "")
		endif()

		# Add build options for subsystem
		if("${subsys}" STREQUAL "WIN32")
			# Windows application
			remove_compile_defs(_CONSOLE)
			remove_linker_flags(/SUBSYSTEM:CONSOLE)
			add_compile_defs(_WINDOWS)
			add_linker_flags(/SUBSYSTEM:WINDOWS)
		else()
			# Console application
			remove_compile_defs(_WINDOWS)
			remove_linker_flags(/SUBSYSTEM:WINDOWS)
			add_compile_defs   (_CONSOLE)
			add_linker_flags   (/SUBSYSTEM:CONSOLE)
		endif()
	endif()

	# Build executable
	add_executable(${CMAKETOOLS_CURRENT_TARGET} ${subsys} ${CMAKETOOLS_CURRENT_SOURCES})
	target_link_libraries(${CMAKETOOLS_CURRENT_TARGET} ${CMAKETOOLS_CURRENT_LIBS})
	cmaketools_set_properties(${CMAKETOOLS_CURRENT_TARGET})
endmacro(build_executable name subsystem)

##################################################
## MACRO: add_activex_interface
##
## Add ActiveX interface (process interface files using the microsoft MIDL compiler)
##################################################
macro(add_activex_interface target src)
	# Set MIDL options
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(options ${options} /D "DEBUG")
	else()
		set(options "")
	endif()

	# Add additional options
	if("${ARGV2}" STREQUAL "OPTIONS")
		foreach(param ${ARGN})
			if(NOT ${param} STREQUAL "OPTIONS")
				set(options ${options} ${param})
			endif()
		endforeach(param)
	endif()

	# Call MIDL compiler
	add_custom_command(
    	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${target}.tlb ${CMAKE_CURRENT_BINARY_DIR}/${target}_i.h ${CMAKE_CURRENT_BINARY_DIR}/${target}_i.c ${CMAKE_CURRENT_BINARY_DIR}/${target}_p.c
    	COMMAND ${MIDL_COMPILER}
    	/nologo
		/tlb "${CMAKE_CURRENT_BINARY_DIR}/${target}.tlb"
		/h "${CMAKE_CURRENT_BINARY_DIR}/${target}_i.h"
		/iid "${CMAKE_CURRENT_BINARY_DIR}/${target}_i.c"
		/proxy "${CMAKE_CURRENT_BINARY_DIR}/${target}_p.c"
		${options}
		${src} 
	)
endmacro(add_activex_interface target src)

##################################################
## MACRO: add_runner_shell_script
##
## Creates a runner shell script (works only under Linux)
## The script assumes following directory structure:
## <install_prefix>/share/
##	-> pixellight (basepath)
##		-> Runtime
##			-> <ARCH> (e.g. x86)
##		-> Samples
##			-> Bin
##				->  <ARCH> (e.g. x86)
##		-> Tools
##			->  <ARCH> (e.g. x86)
## The CMake variable CMAKETOOLS_BASEPATH_GO_UPS tells the script how many directories it must go "up" to come to the base directory (<install_prefix>/share/pixellight)
## For the samples the CMAKETOOLS_BASEPATH_GO_UPS is set to go three directories up (/../../)
## For the tools the CMAKETOOLS_BASEPATH_GO_UPS is set to go two directories up (/../)
## The script configures PL_RUNTIME and LD_LIBRARY_PATH so that the PL application can run properly
## A part of the path in both environment variables is generated from the calculated "base directory"
## So, the generated script is only useful when the above listed directory structure exists
##################################################
macro(add_runner_shell_script)
	set(CMAKE_TOOLS_CURRENT_RUNNERSCRIPT "${CMAKETOOLS_CURRENT_OUTPUT_DIR}/helperscript/${CMAKETOOLS_CURRENT_OUTPUT_NAME}")
	# Create runner script
	configure_file("${CMAKE_SOURCE_DIR}/PixelLight/runplapp.sh" "${CMAKE_TOOLS_CURRENT_RUNNERSCRIPT}" @ONLY NEWLINE_STYLE UNIX)
endmacro(add_runner_shell_script target scriptsource install_target)
