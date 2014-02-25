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
define_external_project(libjpg)

##################################################
## Configuration
##################################################
set(JPEG_VERSION "v9a")
set(JPEG_URL "http://www.ijg.org/files/jpegsrc.${JPEG_VERSION}.tar.gz")
#set(JPEG_MD5 "edd26ef2bf561786b2185f2b8afda78b")

##################################################
## Need to download?
##################################################
# First look if we have the library installed already. This will be the case for the majority of Linux systems
#TODO: what about debug configuration?
message(STATUS "Looking for external dependency 'libjpeg' with minimum version ${JPEG_VERSION}...")
find_package(JPEG)

# Do we have it?
if(JPEG_FOUND)
	# We will use the library present on the user system
	message(STATUS "'libjpeg' was found on this system")
	
	# The find module from CMake itself doesn't add the XXX_LIBRARIES variable and XXX_INCLUDE_DIR variable to the cache, which is the correct behaviour.
	# These variables should normaly only be visible within the cmake file in which the find module was called.
	# Otherwise the variables would be visible for all other projects if wanted or not
	# We want this, because this variables are used in other CMake files, which doesn't include this file, so we add the variables to the cache
	set(JPEG_LIBRARIES ${JPEG_LIBRARIES} CACHE INTERNAL "")
	set(JPEG_INCLUDE_DIR ${JPEG_INCLUDE_DIR} CACHE INTERNAL "")
else()
	# We'll need to download and build the library ourselves
	message(STATUS "'libjpeg' was not found, will be downloaded and built from [${JPEG_URL}]")
	
	# Define the external project
	ExternalProject_Add(${CMAKETOOLS_CURRENT_TARGET}
		DOWNLOAD_DIR ${CMAKETOOLS_CURRENT_DOWNLOAD_DIR}
		SOURCE_DIR ${CMAKETOOLS_CURRENT_SRC_DIR}
		BINARY_DIR ${CMAKETOOLS_CURRENT_BUILD_DIR}
		INSTALL_DIR ${CMAKETOOLS_CURRENT_INSTALL_DIR}
		TMP_DIR ${CMAKETOOLS_CURRENT_TMP_DIR}
		STAMP_DIR ${CMAKETOOLS_CURRENT_STAMP_DIR}
		URL ${JPEG_URL}
#		URL_MD5 ${JPEG_MD5}

		# We have a CMake script for libjpeg, copy it over to the source folder so that
		# ExternalProject finds it and executes it.
		PATCH_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKETOOLS_CURRENT_DIR}/CMake/CMakeLists.txt ${CMAKETOOLS_CURRENT_SRC_DIR}/CMakeLists.txt
		
		CMAKE_ARGS
			-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
	)
	
	if(WIN32)
		set(JPEG_LIBRARIES ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/jpeg.lib CACHE INTERNAL "")
	else()
		set(JPEG_LIBRARIES ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/libjpeg.a CACHE INTERNAL "")
	endif()
	set(JPEG_INCLUDE_DIR ${CMAKETOOLS_CURRENT_INSTALL_DIR}/include CACHE INTERNAL "")
	
	add_dependencies(External-Recommended ${CMAKETOOLS_CURRENT_TARGET})
endif()

