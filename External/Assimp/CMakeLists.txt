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
pl_project(Assimp)

##################################################
## Configuration
##################################################
set(ASSIMP_VERSION "3.0.1270")
set(ASSIMP_URL "http://sourceforge.net/projects/assimp/files/assimp-3.0/assimp--${ASSIMP_VERSION}-source-only.zip")
set(ASSIMP_MD5 "52aa4cf4e34e6b2a9c5f6c0b3c319af1")

##################################################
## Need to download?
##################################################
# [TODO] Find package possible for Assimp?
#find_package(ASSIMP ${ASSIMP_VERSION})

# Do we have it?
#if(ASSIMP_FOUND)
#	# We will use the library present on the user system
#	message(STATUS "'Assimp' was found on this system")
#	
#	# The find module from CMake itself doesn't add the XXX_LIBRARIES variable and XXX_INCLUDE_DIR variable to the cache, which is the correct behaviour.
#	# These variables should normaly only be visible within the cmake file in which the find module was called.
#	# Otherwise the variables would be visible for all other projects if wanted or not
#	# We want this, because this variables are used in other CMake files, which doesn't include this file, so we add the variables to the cache
#	set(ASSIMP_LIBRARIES ${ASSIMP_LIBRARIES} CACHE INTERNAL "")
#	set(ASSIMP_INCLUDE_DIRS ${ASSIMP_INCLUDE_DIR} CACHE INTERNAL "")
#else()
	# We'll need to download and build the library ourselves
	message(STATUS "-- Using external build system for dependency 'Assimp'...")

	# We are building static library out of Assimp - we need to enable PIC support!
    if(CMAKE_SYSTEM_NAME STREQUAL Linux)
            set(COMPILER_FLAGS -DCMAKE_C_FLAGS:STRING=-fPIC -DCMAKE_CXX_FLAGS:STRING=-fPIC)
    endif()

	# Define the external project
	ExternalProject_Add(${PL_CURRENT_TARGET_NAME}
		DOWNLOAD_DIR ${PL_CURRENT_DOWNLOAD_DIR}
		SOURCE_DIR ${PL_CURRENT_SRC_DIR}
		BINARY_DIR ${PL_CURRENT_BUILD_DIR}
		INSTALL_DIR ${PL_CURRENT_INSTALL_DIR}
		TMP_DIR ${PL_CURRENT_TMP_DIR}
		STAMP_DIR ${PL_CURRENT_STAMP_DIR}
		URL ${ASSIMP_URL}
		URL_MD5 ${ASSIMP_MD5}
		
		# Include our custom code patch
		PATCH_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Patch/LWOAnimation.cpp <SOURCE_DIR>/code/LWOAnimation.cpp
		
		CMAKE_ARGS
			-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
			-DENABLE_BOOST_WORKAROUND=ON
			-DZLIB_LIBRARY:STRING=${ZLIB_LIBRARY_LIST}
			-DZLIB_LIBRARIES:STRING=${ZLIB_LIBRARY_LIST}
			-DZLIB_INCLUDE_DIR:STRING=${ZLIB_INCLUDE_DIR}
			-DZLIB_FOUND=ON
			
		CMAKE_CACHE_ARGS
			-DBUILD_ASSIMP_TOOLS:BOOL=OFF
			-DBUILD_ASSIMP_SAMPLES:BOOL=OFF
			-DBUILD_TESTS:BOOL=OFF
			-DBUILD_STATIC_LIB:BOOL=ON
			${COMPILER_FLAGS}
			
		# See zlib's CMakeLists.txt
		LIST_SEPARATOR ^^
	)
	
	# Setup paths
	if(WIN32)
		# On Windows, the output .lib file has a different name for debug configuration
		set(${PL_CURRENT_TARGET_LIBRARIES} optimized ${PL_CURRENT_INSTALL_DIR}/lib/assimp.lib debug ${PL_CURRENT_INSTALL_DIR}/lib/assimpD.lib CACHE INTERNAL "")
	else()
		set(${PL_CURRENT_TARGET_LIBRARIES} ${PL_CURRENT_INSTALL_DIR}/lib/libassimp.a CACHE INTERNAL "")
	endif()

	set(${PL_CURRENT_TARGET_INCLUDE_DIRS} ${PL_CURRENT_INSTALL_DIR}/include CACHE INTERNAL "")
	
	# We depend on zlib
	pl_add_dependency(zlib)

	# Finalize
	pl_build_external()
#endif()
