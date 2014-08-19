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

# Suppress warning about the deprecated target property LOCATION in CMake 3
if(CMAKE_MAJOR_VERSION EQUAL 3)
	cmake_policy(SET CMP0026 OLD)
endif()

##################################################
## Project
##################################################
project(PixelLight)

##################################################
## Debug
##################################################
set(PL_DEBUG_BUILD_SYSTEM ON)

##################################################
## Includes
##################################################
include(CMake/Utils.cmake)
include(CMake/Platform.cmake)

##################################################
## Output directories
##################################################

# Install bin
set(PL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/Bin/${PL_TARGET_ARCHBITSIZE}")
# Developer bin
set(PL_DEV_BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/_Bin/${PL_TARGET_ARCHBITSIZE}")

# Install lib
set(PL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/Lib/${PL_TARGET_ARCHBITSIZE}")
# Developer lib
set(PL_DEV_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/_Lib/${PL_TARGET_ARCHBITSIZE}")

# Install include
set(PL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/Include")

# Install documentation
set(PL_DOC_DIR "${CMAKE_INSTALL_PREFIX}/Docs")
# Developer documentation
set(PL_DEV_DOC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/_Docs")

##################################################
## Includes
##################################################
include(CMake/Config.cmake)
include(CMake/Build.cmake)
include(CMake/Project.cmake)

##################################################
## Include our projects
##################################################

# We don't use our automation macros here to filter out everything more effectively
# (and to keep the right order)

add_subdirectory(External)
add_subdirectory(Engine)

if(PL_BUILD_PLUGINS)
	add_subdirectory(Plugins)
endif()

if(PL_BUILD_SAMPLES)
	add_subdirectory(Tutorial)
endif()

if(PL_BUILD_TESTS)
	add_subdirectory(Tests)
endif()

add_subdirectory(Tools)

if(PL_BUILD_DOCS)
	file(MAKE_DIRECTORY ${PL_DEV_DOC_DIR})
	add_subdirectory(Docs)
endif()

if(PL_BUILD_SDK)
	add_subdirectory(Packages)
endif()

##################################################
## Install
##################################################

# Copyright and notes
install(FILES	${CMAKE_SOURCE_DIR}/AUTHORS
				${CMAKE_SOURCE_DIR}/COPYING
				${CMAKE_SOURCE_DIR}/cc-by-nc-sa-3.txt
				${CMAKE_SOURCE_DIR}/ReleaseNotes.txt
	DESTINATION ${CMAKE_INSTALL_PREFIX}				
	COMPONENT SDK
)

# [TODO] Testing
