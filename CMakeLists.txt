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

##################################################
## Includes
##################################################
include(CMake/Config.cmake)
include(CMake/Build.cmake)
include(CMake/Project.cmake)

##################################################
## Target
##################################################
add_custom_target(PixelLight)

##################################################
## Include our projects
##################################################

# We don't use our automation macros here to filter out everything more effectively
# (and to keep the right order)

add_subdirectory(External)
add_subdirectory(Engine)
add_subdirectory(Plugins)

if(PL_BUILD_SAMPLES)
	add_subdirectory(Tutorial)
endif()

if(PL_BUILD_TESTS)
	add_subdirectory(Tests)
endif()

add_subdirectory(Tools)

if(PL_BUILD_DOCS)
	add_subdirectory(Docs EXCLUDE_FROM_ALL)
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