#*********************************************************#
#*  File: CMakeLists.txt                                 *
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
## CMake
##################################################
cmake_minimum_required(VERSION 2.8.3)

##################################################
## Project
##################################################
project(PixelLight)

##################################################
## Includes
##################################################
include(cmake/CMakeTools.cmake)
include(cmake/CMakeTools_Apps.cmake)
include(cmake/CMakeTools_Docs.cmake)
include(cmake/CMakeTools_Externals.cmake)
include(PixelLight/PixelLight.cmake)

##################################################
## Prerequisites
##################################################

# Create PixelLight header file
configure_file("PixelLight/PixelLight.h.in" "${CMAKE_BINARY_DIR}/PixelLight.h")

# Create 'Bin'-directory
set(PL_BIN_DIR "${CMAKE_SOURCE_DIR}/Bin-${CMAKETOOLS_CONFIG_NAME}")
file(MAKE_DIRECTORY ${PL_BIN_DIR})

# Create 'Runtime'-directory
file(MAKE_DIRECTORY ${PL_BIN_DIR}/Runtime)
set(PL_RUNTIME_BIN_DIR "${PL_BIN_DIR}/Runtime/${CMAKETOOLS_TARGET_ARCHBITSIZE}")
file(MAKE_DIRECTORY ${PL_RUNTIME_BIN_DIR})
set(PL_RUNTIME_BIN_GITDIR "${CMAKE_SOURCE_DIR}/Bin/Runtime/${CMAKETOOLS_TARGET_ARCHBITSIZE}")	# Points to the directory directly within the Git repository

# Create 'Lib'-directory
file(MAKE_DIRECTORY ${PL_BIN_DIR}/Lib)
set(PL_LIB_DIR "${PL_BIN_DIR}/Lib/${CMAKETOOLS_TARGET_ARCHBITSIZE}")
file(MAKE_DIRECTORY ${PL_LIB_DIR})

# Copy copyright and notes
copy_file(${CMAKE_SOURCE_DIR}/AUTHORS			${PL_BIN_DIR})
copy_file(${CMAKE_SOURCE_DIR}/COPYING			${PL_BIN_DIR})
copy_file(${CMAKE_SOURCE_DIR}/cc-by-nc-sa-3.txt	${PL_BIN_DIR})
copy_file(${CMAKE_SOURCE_DIR}/ReleaseNotes.txt	${PL_BIN_DIR})

# Copy Microsoft Visual C++ Redistributables
#TODO: for now this does not work as expected. The redistributable should be included in the installable SDK
# but nowhere else
#if(WIN32)
#	file(MAKE_DIRECTORY ${PL_RUNTIME_BIN_DIR}/VC2010_Redistributable)
#	copy_files("${PL_RUNTIME_BIN_GITDIR}/VC2010_Redistributable/*.*" "${PL_RUNTIME_BIN_DIR}/VC2010_Redistributable")
#endif()

##################################################
## Target
##################################################
add_custom_target(PixelLight)

##################################################
## Projects
##################################################
add_subdirectory(External)
add_subdirectory(Base)
add_subdirectory(Plugins)
if(PL_SAMPLES)
	add_subdirectory(Samples)
endif()
if(PL_TESTS)
	add_subdirectory(Tests)
endif()
add_subdirectory(Tools)
if(PL_DOCS)	
	add_subdirectory(Docs EXCLUDE_FROM_ALL)
endif()

##################################################
## Install
##################################################

# Copyright and notes
install(FILES	${CMAKE_SOURCE_DIR}/AUTHORS
				${CMAKE_SOURCE_DIR}/COPYING
				${CMAKE_SOURCE_DIR}/cc-by-nc-sa-3.txt
				${CMAKE_SOURCE_DIR}/ReleaseNotes.txt
	DESTINATION ${PL_INSTALL_ROOT}				COMPONENT SDK
)

# Microsoft Visual C++ Redistributables
#TODO: see above
#if(WIN32)
#	install(DIRECTORY ${PL_RUNTIME_BIN_GITDIR}/VC2010_Redistributable
#		DESTINATION ${PL_INSTALL_RUNTIME_BIN}	COMPONENT SDK
#	)
#endif()

##################################################
## Packages
##################################################
if(CMAKETOOLS_BUILD_SDK)
	add_subdirectory(Packages)
endif()

##################################################
## Testing
##################################################
if(CMAKETOOLS_CONFIG_NIGHTLY)
	include(CTest)
endif()
