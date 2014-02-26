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
## Project
##################################################
define_external_project(zlib)

##################################################
## Configuration
##################################################
set(ZLIB_VERSION "1.2.8")
#set(ZLIB_URL "http://zlib.net/zlib-${ZLIB_VERSION}.tar.gz")
set(ZLIB_URL "http://prdownloads.sourceforge.net/libpng/zlib-${ZLIB_VERSION}.tar.gz")
set(ZLIB_MD5 "44d667c142d7cda120332623eab69f40")

# This is required because these variables are cached. If another library performs the same check (as e.g. libpng
# does), it will result in the external project not being built after repeated CMake runs. We want to make this
# the only place where zlib's (non)existence is decided
set(ZLIB_FOUND "0" CACHE INTERNAL "")
set(ZLIB_LIBRARY "" CACHE INTERNAL "")
set(ZLIB_LIBRARY_LIST "" CACHE INTERNAL "")
set(ZLIB_LIBRARIES "" CACHE INTERNAL "")
set(ZLIB_INCLUDE_DIR "" CACHE INTERNAL "")
# Also mark if there was a project created for zlib for dependency checking
set(ZLIB_HAS_PROJECT OFF CACHE INTERNAL "")

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
	set(ZLIB_LIBRARY ${ZLIB_LIBRARIES} CACHE INTERNAL "")
	set(ZLIB_INCLUDE_DIR ${ZLIB_INCLUDE_DIR} CACHE INTERNAL "")
else()
	# We'll need to download and build the library ourselves
	message(STATUS "'zlib' was not found, will be downloaded and built from [${ZLIB_URL}]")
	
	set(ZLIB_HAS_PROJECT ON CACHE INTERNAL "")
	
	# Define the external project
	ExternalProject_Add(${CMAKETOOLS_CURRENT_TARGET}
		DOWNLOAD_DIR ${CMAKETOOLS_CURRENT_DOWNLOAD_DIR}
		SOURCE_DIR ${CMAKETOOLS_CURRENT_SRC_DIR}
		BINARY_DIR ${CMAKETOOLS_CURRENT_BUILD_DIR}
		INSTALL_DIR ${CMAKETOOLS_CURRENT_INSTALL_DIR}
		TMP_DIR ${CMAKETOOLS_CURRENT_TMP_DIR}
		STAMP_DIR ${CMAKETOOLS_CURRENT_STAMP_DIR}
		URL ${ZLIB_URL}
		URL_MD5 ${ZLIB_MD5}
		TIMEOUT 30
		PATCH_COMMAND ${CMAKE_COMMAND} -E remove <SOURCE_DIR>/zconf.h
		CMAKE_ARGS
			-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
	)
	
	if(WIN32)
		# On Windows, the output .lib file has a different name for debug configuration
		set(ZLIB_LIBRARIES optimized ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/zlibstatic.lib debug ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/zlibstaticd.lib CACHE INTERNAL "")
		set(ZLIB_LIBRARY ${ZLIB_LIBRARIES} CACHE INTERNAL "")
		
		# This is a small hack that allows us to pass this as a -D parameter into other external projects
		set(ZLIB_LIBRARY_LIST optimized^^${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/zlibstatic.lib^^debug^^${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/zlibstaticd.lib CACHE INTERNAL "")
	else()
		set(ZLIB_LIBRARIES ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/libz.a CACHE INTERNAL "")
		set(ZLIB_LIBRARY ${ZLIB_LIBRARIES} CACHE INTERNAL "")
		set(ZLIB_LIBRARY_LIST ${ZLIB_LIBRARIES} CACHE INTERNAL "")
	endif()
	set(ZLIB_INCLUDE_DIR ${CMAKETOOLS_CURRENT_INSTALL_DIR}/include CACHE INTERNAL "")
	
	add_dependencies(External-Base ${CMAKETOOLS_CURRENT_TARGET})
endif()
