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
## Project configuration
##################################################

set(PL_PROJECT_NAME "PixelLight")

set(PL_PROJECT_VERSION_MAJOR    1)
set(PL_PROJECT_VERSION_MINOR    5)
set(PL_PROJECT_VERSION_PATCH    0)
set(PL_PROJECT_VERSION_RELEASE "Sunray")

# Create PixelLight header file
configure_file("CMake/Config/PixelLight.h.in" "${CMAKE_BINARY_DIR}/PixelLight.h")
set(PL_CONFIG_FILE_LOCATION "${CMAKE_BINARY_DIR}" CACHE INTERNAL "Location of the PixelLight.h file")


##################################################
## Developer build
##################################################

# [TODO]: Do we need the developer build?
#set(PL_DEVELOPER_BUILD ON CACHE BOOL "Build a version suitable for developing PixelLight itself")

#if(${PL_DEVELOPER_BUILD})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PL_DEV_LIB_DIR})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PL_DEV_LIB_DIR})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PL_DEV_BIN_DIR})
#	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PL_DEV_LIB_DIR})
#	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PL_DEV_LIB_DIR})
#	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PL_DEV_BIN_DIR})
#	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PL_DEV_LIB_DIR})
#	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PL_DEV_LIB_DIR})
#	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PL_DEV_BIN_DIR})
#else()
#	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
#	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY)
#	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY)
#	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG)
#	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG)
#	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG)
#	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE)
#	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE)
#	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE)
#endif()


##################################################
## Build configuration
##################################################

# Enable IDE folder support
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(PL_BUILD_SAMPLES ON CACHE BOOL "Build sample projects?")
set(PL_BUILD_PLUGINS OFF CACHE BOOL "Build PixelLight plugins")
set(PL_BUILD_DOCS OFF CACHE BOOL "Build development documentation?")
set(PL_BUILD_SDK OFF CACHE BOOL "Build SDK installer?")
set(PL_BUILD_TESTS OFF CACHE BOOL "Build automated unit tests?")

# Build without zip support
set(PL_NO_ZIP_SUPPORT OFF CACHE BOOL "Disable zip filesysytem support (not recommended)")

# Build without jpeg support
set(PL_NO_JPG_SUPPORT OFF CACHE BOOL "Disable JPEG image loading support")

# Build without png support
set(PL_NO_PNG_SUPPORT OFF CACHE BOOL "Disable PNG image loading support")

# Add our tools to CMake module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules)