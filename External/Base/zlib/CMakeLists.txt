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

include(ExternalProject)

##################################################
## Configuration
##################################################
set(ZLIB_VERSION "1.2.8")
set(ZLIB_DOWNLOAD "${CMAKE_CURRENT_BINARY_DIR}/Download")
set(ZLIB_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/Source")
set(ZLIB_BUILD "${CMAKE_CURRENT_BINARY_DIR}/Build")
set(ZLIB_INSTALL "${CMAKE_CURRENT_BINARY_DIR}/Install")
set(ZLIB_URL "http://zlib.net/zlib-${ZLIB_VERSION}.tar.gz")
set(ZLIB_MD5 "44d667c142d7cda120332623eab69f40")

##################################################
## Need to download?
##################################################
# First look if we have the library installed already. This will be the case for the majority of Linux systems
#TODO: what about debug configuration?
message(STATUS "Looking for external dependency 'zlib' with minimum version ${ZLIB_VERSION}...")
find_package(ZLIB ${ZLIB_VERSION})

# Do we have it?
if(ZLIB_FOUND)
	# We will use the library present on the user system
	message(STATUS "'zlib' was found on this system")
	
	# The find module from CMake itself doesn't add the XXX_LIBRARIES variable and XXX_INCLUDE_DIR variable to the cache, which is the correct behaviour.
	# These variables should normaly only be visible within the cmake file in which the find module was called.
	# Otherwise the variables would be visible for all other projects if wanted or not
	# We want this, because this variables are used in other CMake files, which doesn't include this file, so we add the variables to the cache
	set(ZLIB_LIBRARIES ${ZLIB_LIBRARIES} CACHE INTERNAL "")
	set(ZLIB_INCLUDE_DIR ${ZLIB_INCLUDE_DIR} CACHE INTERNAL "")
else()
	# We'll need to download and build the library ourselves
	message(STATUS "'zlib' was not found, will be downloaded and built from [${ZLIB_URL}]")
	
	# Define the external project
	ExternalProject_Add(External-zlib
		DOWNLOAD_DIR ${ZLIB_DOWNLOAD}
		SOURCE_DIR ${ZLIB_SOURCE}
		BINARY_DIR ${ZLIB_BUILD}
		INSTALL_DIR ${ZLIB_INSTALL}
		URL ${ZLIB_URL}
		URL_MD5 ${ZLIB_MD5}
		PATCH_COMMAND ${CMAKE_COMMAND} -E remove <SOURCE_DIR>/zconf.h
		CMAKE_ARGS
			-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
	)
	
	if(WIN32)
		set(ZLIB_LIBRARIES ${ZLIB_INSTALL}/lib/zlibstatic.lib CACHE INTERNAL "")
		#set(ZLIB_LIBRARIES ${ZLIB_INSTALL}/lib/zlib.lib CACHE INTERNAL "")
	else()
		set(ZLIB_LIBRARIES ${ZLIB_INSTALL}/lib/libz.a CACHE INTERNAL "")
	endif()
	set(ZLIB_INCLUDE_DIR ${ZLIB_INSTALL}/include CACHE INTERNAL "")
	
	#add_dependencies(External-Base ${CMAKETOOLS_CURRENT_TARGET})
	#add_dependencies(External-Clean ${CMAKETOOLS_CURRENT_TARGET}-Clean)
endif()
