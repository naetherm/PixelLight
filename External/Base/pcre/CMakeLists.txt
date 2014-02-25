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
define_external_project(libpcre)

##################################################
## Configuration
##################################################
set(PCRE_VERSION "8.34")
set(PCRE_URL "http://sourceforge.net/projects/pcre/files/pcre/${PCRE_VERSION}/pcre-${PCRE_VERSION}.tar.gz")
set(PCRE_MD5 "eb34b2c9c727fd64940d6fd9a00995eb")

##################################################
## Need to download?
##################################################
# First look if we have the library installed already. This will be the case for the majority of Linux systems
#TODO: what about debug configuration?
message(STATUS "Looking for external dependency 'pcre' with minimum version ${PCRE_VERSION}...")
find_package(PCRE ${PCRE_VERSION})

# Do we have it?
if(PCRE_FOUND)
	# We will use the library present on the user system
	message(STATUS "'pcre' was found on this system")
	
	# The find module from CMake itself doesn't add the XXX_LIBRARIES variable and XXX_INCLUDE_DIR variable to the cache, which is the correct behaviour.
	# These variables should normaly only be visible within the cmake file in which the find module was called.
	# Otherwise the variables would be visible for all other projects if wanted or not
	# We want this, because this variables are used in other CMake files, which doesn't include this file, so we add the variables to the cache
	set(PCRE_LIBRARIES ${PCRE_LIBRARIES} CACHE INTERNAL "")
	set(PCRE_INCLUDE_DIR ${PCRE_INCLUDE_DIR} CACHE INTERNAL "")
else()
	# We'll need to download and build the library ourselves
	message(STATUS "'pcre' was not found, will be downloaded and built from [${PCRE_URL}]")
	
	# Define the external project
	ExternalProject_Add(${CMAKETOOLS_CURRENT_TARGET}
		DOWNLOAD_DIR ${CMAKETOOLS_CURRENT_DOWNLOAD_DIR}
		SOURCE_DIR ${CMAKETOOLS_CURRENT_SRC_DIR}
		BINARY_DIR ${CMAKETOOLS_CURRENT_BUILD_DIR}
		INSTALL_DIR ${CMAKETOOLS_CURRENT_INSTALL_DIR}
		TMP_DIR ${CMAKETOOLS_CURRENT_TMP_DIR}
		STAMP_DIR ${CMAKETOOLS_CURRENT_STAMP_DIR}
		URL ${PCRE_URL}
		URL_MD5 ${PCRE_MD5}
#		PATCH_COMMAND ${CMAKE_COMMAND} -E remove <SOURCE_DIR>/zconf.h
		CMAKE_ARGS
			-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
	)
	
	# Setup paths
	if(WIN32)
		# On Windows, the output .lib file has a different name for debug configuration, rename it
		ExternalProject_Add_Step(${CMAKETOOLS_CURRENT_TARGET} RenameDebugLib
			COMMAND ${CMAKE_COMMAND} -DCMAKETOOLS_CURRENT_INSTALL_DIR:STRING=${CMAKETOOLS_CURRENT_INSTALL_DIR} -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/PostInstall.cmake
			DEPENDEES install
		)
		
		set(PCRE_LIBRARIES ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/pcre.lib CACHE INTERNAL "")
	else()
		set(PCRE_LIBRARIES ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/libpcre.a CACHE INTERNAL "")
	endif()
	set(PCRE_INCLUDE_DIR ${CMAKETOOLS_CURRENT_INSTALL_DIR}/include CACHE INTERNAL "")
	
	# Dependencies
	add_dependencies(External-Base ${CMAKETOOLS_CURRENT_TARGET})
endif()
