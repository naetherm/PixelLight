#*********************************************************#
#*  File: CMakeTools_Externals.cmake                     *
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
## CMakeTools externals
##
## These macros can be used to build external
## dependencies automatically for your project
##################################################

##################################################
## MACRO: define_external_project
##
## Define an externals project
##   name		name of the project (e.g. "TestProject")
##################################################
macro(define_external_project name)
	# We are now using ExternalProject so there is no "real" project defined here.
	# Instead, we just define a bunch of variables we will be using in our build scripts.
	
	# Set project variables
	set(CMAKETOOLS_CURRENT_NAME ${name})
	set(CMAKETOOLS_CURRENT_TARGET External-${name})
	
	# Where is the current CMakeLists.txt located
	set(CMAKETOOLS_CURRENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
	
	# Root for external projects' directories
	set(CMAKETOOLS_CURRENT_ROOT_DIR "${CMAKE_CURRENT_BINARY_DIR}")
	# Where to download the library
	set(CMAKETOOLS_CURRENT_DOWNLOAD_DIR "${CMAKETOOLS_CURRENT_ROOT_DIR}/Download")
	# Where to extract the library sources
	set(CMAKETOOLS_CURRENT_SRC_DIR "${CMAKETOOLS_CURRENT_ROOT_DIR}/Source")
	# Where to build the library
	set(CMAKETOOLS_CURRENT_BUILD_DIR "${CMAKETOOLS_CURRENT_ROOT_DIR}/Build")
	# Where to build the library
	set(CMAKETOOLS_CURRENT_BUILD_DIR "${CMAKETOOLS_CURRENT_ROOT_DIR}/Build")
	# Where to install the library
	set(CMAKETOOLS_CURRENT_INSTALL_DIR "${CMAKETOOLS_CURRENT_ROOT_DIR}/Install")
	# Where to put temporary files
	set(CMAKETOOLS_CURRENT_TMP_DIR "${CMAKETOOLS_CURRENT_ROOT_DIR}/Temp")
	# Where to put stamp files
	set(CMAKETOOLS_CURRENT_STAMP_DIR "${CMAKETOOLS_CURRENT_ROOT_DIR}/Stamp")
	
	set(CMAKETOOLS_CURRENT_EXT_DIR "${CMAKE_SOURCE_DIR}/External/_${CMAKETOOLS_CONFIG_NAME}_${CMAKETOOLS_TARGET_ARCH}_${CMAKETOOLS_TARGET_BITSIZE}")
	set(CMAKETOOLS_CURRENT_DEST_DIR "${CMAKETOOLS_CURRENT_EXT_DIR}/${CMAKETOOLS_CURRENT_NAME}")
	set(CMAKETOOLS_CURRENT_INCLUDE_DIR "${CMAKETOOLS_CURRENT_DEST_DIR}/include")
	set(CMAKETOOLS_CURRENT_LIB_DIR "${CMAKETOOLS_CURRENT_DEST_DIR}/lib")
endmacro(define_external_project name)

##################################################
## MACRO: external_command_add_definition
##
## Add definition that is passed to an external command
##   var  Name of destination variable
##   def  Definition that is added to the variable
##################################################
macro(external_command_add_definition var def)
	set(${var} ${${var}} "-D${def}:STRING=${${def}}")
endmacro(external_command_add_definition var def)

##################################################
## MACRO: external_command
##
## Define an external command that is executed for the target
##   target   Target name
##   comment  Comment
##   comment  Name of script that is to be executed
##   depends  Dependencies
##################################################
macro(external_command target comment script depends)
	# Compose command
	set(command ${CMAKE_COMMAND})
	external_command_add_definition(command CMAKE_GENERATOR)
	external_command_add_definition(command CMAKE_TOOLCHAIN_FILE)
	external_command_add_definition(command CMAKE_C_COMPILER)
	external_command_add_definition(command CMAKE_CXX_COMPILER)
	external_command_add_definition(command CMAKE_FIND_ROOT_PATH)
	external_command_add_definition(command HOST_SYSTEM)
	external_command_add_definition(command CMAKETOOLS_DIR)
	external_command_add_definition(command CMAKETOOLS_HOST_SYSTEM)
	external_command_add_definition(command CMAKETOOLS_HOST_SYSTEM_WIN32)
	external_command_add_definition(command CMAKETOOLS_HOST_SYSTEM_LINUX)
	external_command_add_definition(command CMAKETOOLS_HOST_SYSTEM_APPLE)
	external_command_add_definition(command CMAKETOOLS_TARGET_SYSTEM)
	external_command_add_definition(command CMAKETOOLS_TARGET_SYSTEM_WIN32)
	external_command_add_definition(command CMAKETOOLS_TARGET_SYSTEM_LINUX)
	external_command_add_definition(command CMAKETOOLS_TARGET_SYSTEM_APPLE)
	external_command_add_definition(command CMAKETOOLS_CURRENT_NAME)
	external_command_add_definition(command CMAKETOOLS_TARGET_ARCH)
	external_command_add_definition(command CMAKETOOLS_TARGET_BITSIZE)
	external_command_add_definition(command CMAKETOOLS_CURRENT_TARGET)
	external_command_add_definition(command CMAKETOOLS_CONFIG_NAME)
	external_command_add_definition(command CMAKETOOLS_CURRENT_SOURCE_DIR)
	external_command_add_definition(command CMAKETOOLS_CURRENT_DOWNLOAD_DIR)
	external_command_add_definition(command CMAKETOOLS_CURRENT_BUILD_DIR)
	external_command_add_definition(command CMAKETOOLS_CURRENT_EXT_DIR)
	external_command_add_definition(command CMAKETOOLS_CURRENT_DEST_DIR)
	external_command_add_definition(command CMAKETOOLS_CURRENT_INCLUDE_DIR)
	external_command_add_definition(command CMAKETOOLS_CURRENT_LIB_DIR)
	external_command_add_definition(command CMAKETOOLS_CURRENT_BUILT_FILE)
	external_command_add_definition(command DIFF_EXECUTABLE)
	external_command_add_definition(command PATCH_EXECUTABLE)
	external_command_add_definition(command SFK_EXECUTABLE)
	external_command_add_definition(command NMAKE_EXECUTABLE)
	external_command_add_definition(command VCBUILD_EXECUTABLE)
	external_command_add_definition(command PL_EXTERNAL_REPOSITORY)
	external_command_add_definition(command PL_EXTERNAL_USER)
	external_command_add_definition(command PL_EXTERNAL_PASS)
	external_command_add_definition(command PL_BIN_DIR)
	external_command_add_definition(command PL_RUNTIME_BIN_DIR)
	external_command_add_definition(command PL_RUNTIME_BIN_GITDIR)
	external_command_add_definition(command LINUX)
	external_command_add_definition(command ANDROID)
	external_command_add_definition(command NACL)
	external_command_add_definition(command ARMEABI_V7A)
	set(command ${command} "-P" ${CMAKE_CURRENT_SOURCE_DIR}/${script})

	# Get dependency
	set(file_depends DEPENDS ${CMAKETOOLS_CURRENT_SOURCE_DIR}/${target}.cmake)
	if(NOT ${depends} STREQUAL "NO_DEPENDS")
		set(file_depends ${file_depends} ${CMAKETOOLS_CURRENT_BUILD_DIR}/${depends}.done)
	endif()

	# Create target
	add_custom_command(
		OUTPUT ${CMAKETOOLS_CURRENT_BUILD_DIR}/${target}.done
		${file_depends}
		COMMAND   ${command}
		COMMENT   ${comment}
	)

	# Create named target
	add_custom_target(
		${CMAKETOOLS_CURRENT_TARGET}-${target}
		DEPENDS ${CMAKETOOLS_CURRENT_BUILD_DIR}/${target}.done
	)
endmacro(external_command target comment script depends)

##################################################
## MACRO: external_check
##
## Execute 'check' for the external project
##################################################
macro(external_check)
	# Create 'check' target
	external_command(
		Check
		"Checking package files for '${CMAKETOOLS_CURRENT_NAME}'"
		Check.cmake
		NO_DEPENDS
	)
endmacro(external_check)

##################################################
## MACRO: external_fetch
##
## Execute 'fetch' for the external project
##################################################
macro(external_fetch)
	# Create 'fetch' target
	external_command(
		Fetch
		"Downloading files for '${CMAKETOOLS_CURRENT_NAME}'"
		Fetch.cmake
		Check
	)
endmacro(external_fetch)

##################################################
## MACRO: external_configure
##
## Execute 'configure' for the external project
##################################################
macro(external_configure)
	# Create 'configure' target
	external_command(
		Configure
		"Configuring source files for '${CMAKETOOLS_CURRENT_NAME}'"
		Configure.cmake
		Fetch
	)
	add_dependencies(${CMAKETOOLS_CURRENT_TARGET}-Configure ${CMAKETOOLS_CURRENT_TARGET}-Fetch)
endmacro(external_configure)

##################################################
## MACRO: external_build
##
## Execute 'build' for the external project
##################################################
macro(external_build)
	# Create 'build' target
	external_command(
		Build
		"Building binaries for '${CMAKETOOLS_CURRENT_NAME}'"
		Build.cmake
		Configure
	)
	add_dependencies(${CMAKETOOLS_CURRENT_TARGET}-Build ${CMAKETOOLS_CURRENT_TARGET}-Configure)
endmacro(external_build)

##################################################
## MACRO: external_install
##
## Execute 'install' for the external project
##################################################
macro(external_install)
	# Create 'install' target
	external_command(
		Install
		"Installing files for '${CMAKETOOLS_CURRENT_NAME}'"
		Install.cmake
		Build
	)
	add_dependencies(${CMAKETOOLS_CURRENT_TARGET}-Install ${CMAKETOOLS_CURRENT_TARGET}-Build)
	add_dependencies(${CMAKETOOLS_CURRENT_TARGET} ${CMAKETOOLS_CURRENT_TARGET}-Install)
endmacro(external_install)

##################################################
## MACRO: external_clean
##
## Execute 'clean' for the external project
##################################################
macro(external_clean)
	# Create 'clean' target
	external_command(
		Clean
		"Cleaning files for '${CMAKETOOLS_CURRENT_NAME}'"
		Clean.cmake
		NO_DEPENDS
	)

	# The used 'external_command'-macro above added the target '${CMAKETOOLS_CURRENT_TARGET}-Clean'
	# -> Exclude this target from the default build
	# -> If we don't do this, extracted external packages might get removed while
	#    building everything resulting in compiler errors like 'file <x> not found'
	#    (without exclusion there might be no issues on one system, but another system might constantly fail)
	set_target_properties(${CMAKETOOLS_CURRENT_TARGET}-Clean PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD 1)
endmacro(external_clean)
