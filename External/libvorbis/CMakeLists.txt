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
pl_project(vorbis)

##################################################
## Configuration
##################################################
set(VORBIS_VERSION "1.3.4")
set(VORBIS_URL "http://downloads.xiph.org/releases/vorbis/libvorbis-${VORBIS_VERSION}.tar.gz")
set(VORBIS_MD5 "8851c593a52d1ef9c526d95174873852")

##################################################
## Need to download?
##################################################
if(NOT CMAKETOOLS_DONT_USE_SYSTEM_LIBS)
	find_package(Vorbis ${VORBIS_VERSION})
endif()

# Do we have it?
if(VORBIS_FOUND)
	# We will use the library present on the user system
	# The find module from CMake itself doesn't add the XXX_LIBRARIES variable and XXX_INCLUDE_DIR variable to the cache, which is the correct behaviour.
	# These variables should normaly only be visible within the cmake file in which the find module was called.
	# Otherwise the variables would be visible for all other projects if wanted or not
	# We want this, because this variables are used in other CMake files, which doesn't include this file, so we add the variables to the cache
	set(${PL_CURRENT_TARGET_LIBRARIES} ${Vorbis_LIBRARIES} CACHE INTERNAL "")
	set(${PL_CURRENT_TARGET_INCLUDE_DIRS} ${Vorbis_INCLUDE_DIR} CACHE INTERNAL "")
else()
	# We'll need to download and build the library ourselves
	message(STATUS "-- Using external build system for dependency 'vorbis'...")
	
	# Define the external project
	ExternalProject_Add(${PL_CURRENT_TARGET_NAME}
		DOWNLOAD_DIR ${PL_CURRENT_DOWNLOAD_DIR}
		SOURCE_DIR ${PL_CURRENT_SRC_DIR}
		BINARY_DIR ${PL_CURRENT_BUILD_DIR}
		INSTALL_DIR ${PL_CURRENT_INSTALL_DIR}
		TMP_DIR ${PL_CURRENT_TMP_DIR}
		STAMP_DIR ${PL_CURRENT_STAMP_DIR}
		URL ${VORBIS_URL}
		URL_MD5 ${VORBIS_MD5}
		
		# We have a CMake script for libvorbis
		PATCH_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/CMake/CMakeLists.txt.in ${PL_CURRENT_SRC_DIR}/CMakeLists.txt
		
		CMAKE_ARGS
			-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
			-DOGG_INCLUDE_DIR:PATH=${ogg_INCLUDE_DIRS}
	)
	
	# Setup paths
	if(WIN32)
		set(${PL_CURRENT_TARGET_LIBRARIES} ${PL_CURRENT_INSTALL_DIR}/lib/vorbis.lib ${PL_CURRENT_INSTALL_DIR}/lib/vorbisfile.lib CACHE INTERNAL "")
	else()
		set(${PL_CURRENT_TARGET_LIBRARIES} ${PL_CURRENT_INSTALL_DIR}/lib/libvorbis.so ${PL_CURRENT_INSTALL_DIR}/lib/libvorbisfile.so CACHE INTERNAL "")

		# Copy the SO to Bin (we have to use shared library on Linux)
		ExternalProject_Add_Step(${PL_CURRENT_TARGET_NAME} CopyDLL
			COMMAND ${CMAKE_COMMAND} -E copy ${PL_CURRENT_INSTALL_DIR}/lib/libvorbis.so ${PL_DEV_BIN_DIR}/libvorbis.so
			COMMAND ${CMAKE_COMMAND} -E copy ${PL_CURRENT_INSTALL_DIR}/lib/libvorbisfile.so ${PL_DEV_BIN_DIR}/libvorbisfile.so
			DEPENDEES install
		)
	endif()

	set(${PL_CURRENT_TARGET_INCLUDE_DIRS} ${PL_CURRENT_INSTALL_DIR}/include CACHE INTERNAL "")
	
	# Dependencies
	add_dependencies(${PL_CURRENT_TARGET_NAME} ogg)

	# Finalize
	pl_build_external()
endif()

mark_as_advanced(FORCE LIBVORBISFILE_LIBRARY LIBVORBIS_LIBRARY)
