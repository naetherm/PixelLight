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
define_external_project(OpenAL)

##################################################
## Configuration
##################################################
set(OPENAL_VERSION "1.15.1")
set(OPENAL_URL "http://kcat.strangesoft.net/openal-releases/openal-soft-${OPENAL_VERSION}.tar.bz2")
#set(OPENAL_MD5 "edd26ef2bf561786b2185f2b8afda78b")

##################################################
## Need to download?
##################################################
# First look if we have the library installed already. This will be the case for the majority of Linux systems
#TODO: what about debug configuration?
message(STATUS "Looking for external dependency 'OpenAL' with minimum version ${OPENAL_VERSION}...")
find_package(OPENAL ${OPENAL_VERSION})

# Do we have it?
if(OPENAL_FOUND)
	# We will use the library present on the user system
	message(STATUS "'OpenAL' was found on this system")
	
	# The find module from CMake itself doesn't add the XXX_LIBRARIES variable and XXX_INCLUDE_DIR variable to the cache, which is the correct behaviour.
	# These variables should normaly only be visible within the cmake file in which the find module was called.
	# Otherwise the variables would be visible for all other projects if wanted or not
	# We want this, because this variables are used in other CMake files, which doesn't include this file, so we add the variables to the cache
	set(OPENAL_LIBRARIES ${OPENAL_LIBRARIES} CACHE INTERNAL "")
	set(OPENAL_INCLUDE_DIRS ${OPENAL_INCLUDE_DIR} CACHE INTERNAL "")
else()
	# We'll need to download and build the library ourselves
	message(STATUS "'OpenAL' was not found, will be downloaded and built from [${OPENAL_URL}]")
	
	# Define the external project
	ExternalProject_Add(${CMAKETOOLS_CURRENT_TARGET}
		DOWNLOAD_DIR ${CMAKETOOLS_CURRENT_DOWNLOAD_DIR}
		SOURCE_DIR ${CMAKETOOLS_CURRENT_SRC_DIR}
		BINARY_DIR ${CMAKETOOLS_CURRENT_BUILD_DIR}
		INSTALL_DIR ${CMAKETOOLS_CURRENT_INSTALL_DIR}
		TMP_DIR ${CMAKETOOLS_CURRENT_TMP_DIR}
		STAMP_DIR ${CMAKETOOLS_CURRENT_STAMP_DIR}
		URL ${OPENAL_URL}
		TIMEOUT 30
#		URL_MD5 ${OPENAL_MD5}
		CMAKE_ARGS
			-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
		CMAKE_CACHE_ARGS
			-DUTILS:BOOL=OFF
	)
	
	if(WIN32)
		set(OPENAL_LIBRARIES ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/OpenAL32.lib CACHE INTERNAL "")
		
		# Copy the DLL to Bin
		ExternalProject_Add_Step(${CMAKETOOLS_CURRENT_TARGET} CopyDLL
			COMMAND ${CMAKE_COMMAND} -E copy ${CMAKETOOLS_CURRENT_INSTALL_DIR}/bin/OpenAL32.dll ${PL_RUNTIME_BIN_GITDIR}/OpenAL32.dll
			DEPENDEES install
		)
	else()
		set(OPENAL_LIBRARIES ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/libopenal.a CACHE INTERNAL "")
		
		# Copy the SO to Bin
		ExternalProject_Add_Step(${CMAKETOOLS_CURRENT_TARGET} CopyDLL
			COMMAND ${CMAKE_COMMAND} -E copy ${CMAKETOOLS_CURRENT_INSTALL_DIR}/bin/OpenAL32.so ${PL_RUNTIME_BIN_GITDIR}/OpenAL32.so
			DEPENDEES install
		)
	endif()
	set(OPENAL_INCLUDE_DIRS ${CMAKETOOLS_CURRENT_INSTALL_DIR}/include/AL CACHE INTERNAL "")
	
	add_dependencies(External-Recommended ${CMAKETOOLS_CURRENT_TARGET})
endif()
