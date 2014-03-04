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
define_external_project(libpng)

##################################################
## Configuration
##################################################
set(PNG_VERSION "1.6.9")
set(PNG_URL "http://download.sourceforge.net/libpng/libpng-${PNG_VERSION}.tar.gz")
set(PNG_MD5 "edd26ef2bf561786b2185f2b8afda78b")

##################################################
## Need to download?
##################################################
# First look if we have the library installed already. This will be the case for the majority of Linux systems
#TODO: what about debug configuration?
message(STATUS "Looking for external dependency 'libpng' with minimum version ${PNG_VERSION}...")
find_package(PNG ${PNG_VERSION})

# Do we have it?
if(PNG_FOUND)
	# We will use the library present on the user system
	message(STATUS "'libpng' was found on this system")
	
	# The find module from CMake itself doesn't add the XXX_LIBRARIES variable and XXX_INCLUDE_DIR variable to the cache, which is the correct behaviour.
	# These variables should normaly only be visible within the cmake file in which the find module was called.
	# Otherwise the variables would be visible for all other projects if wanted or not
	# We want this, because this variables are used in other CMake files, which doesn't include this file, so we add the variables to the cache
	set(PNG_LIBRARIES ${PNG_LIBRARIES} CACHE INTERNAL "")
	set(PNG_INCLUDE_DIR ${PNG_INCLUDE_DIR} CACHE INTERNAL "")
else()
	# We'll need to download and build the library ourselves
	message(STATUS "'libpng' was not found, will be downloaded and built from [${PNG_URL}]")
	
	# Define the external project
	ExternalProject_Add(${CMAKETOOLS_CURRENT_TARGET}
		DOWNLOAD_DIR ${CMAKETOOLS_CURRENT_DOWNLOAD_DIR}
		SOURCE_DIR ${CMAKETOOLS_CURRENT_SRC_DIR}
		BINARY_DIR ${CMAKETOOLS_CURRENT_BUILD_DIR}
		INSTALL_DIR ${CMAKETOOLS_CURRENT_INSTALL_DIR}
		TMP_DIR ${CMAKETOOLS_CURRENT_TMP_DIR}
		STAMP_DIR ${CMAKETOOLS_CURRENT_STAMP_DIR}
		URL ${PNG_URL}
		URL_MD5 ${PNG_MD5}
		TIMEOUT 30
		CMAKE_ARGS
			-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
			-DZLIB_LIBRARY:STRING=${ZLIB_LIBRARY_LIST}
			-DZLIB_LIBRARIES:STRING=${ZLIB_LIBRARY_LIST}
			-DZLIB_INCLUDE_DIR:STRING=${ZLIB_INCLUDE_DIR}
			-DZLIB_FOUND=ON
			
		# See zlib's CMakeLists.txt
		LIST_SEPARATOR ^^
	)
	
	if(WIN32)
		# On Windows, the output .lib file has a different name for debug configuration
		set(PNG_LIBRARIES optimized ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/libpng16_static.lib debug ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/libpng16_staticd.lib CACHE INTERNAL "")
	else()
		set(PNG_LIBRARIES ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/libpng.a CACHE INTERNAL "")
	endif()
	set(PNG_INCLUDE_DIR ${CMAKETOOLS_CURRENT_INSTALL_DIR}/include CACHE INTERNAL "")
	
	add_dependencies(External-Recommended ${CMAKETOOLS_CURRENT_TARGET})
	
	# We depend on zlib, so set dependency if it will be built using ExternalProject
	#if(ZLIB_HAS_PROJECT)
		add_dependencies(${CMAKETOOLS_CURRENT_TARGET} External-zlib)
	#endif()
endif()
