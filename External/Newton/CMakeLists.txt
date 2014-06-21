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
pl_project(Newton)

##################################################
## Configuration
##################################################
set(NEWTON_VERSION "2.32")
set(NEWTON_URL "http://pixellight.org/external/repack/newton-dynamics-${NEWTON_VERSION}.tgz")
set(NEWTON_MD5 "a922b216785e10a691119f1b5fa299ab")

##################################################
## Need to download?
##################################################
# [TODO] Find package possible for Newton?
#find_package(NEWTON ${NEWTON_VERSION})

# Do we have it?
#if(NEWTON_FOUND)
#	# We will use the library present on the user system
#	message(STATUS "'NEWTON' was found on this system")
#	
#	# The find module from CMake itself doesn't add the XXX_LIBRARIES variable and XXX_INCLUDE_DIR variable to the cache, which is the correct behaviour.
#	# These variables should normaly only be visible within the cmake file in which the find module was called.
#	# Otherwise the variables would be visible for all other projects if wanted or not
#	# We want this, because this variables are used in other CMake files, which doesn't include this file, so we add the variables to the cache
#	set(NEWTON_LIBRARIES ${NEWTON_LIBRARIES} CACHE INTERNAL "")
#	set(NEWTON_INCLUDE_DIRS ${NEWTON_INCLUDE_DIR} CACHE INTERNAL "")
#else()
	# We'll need to download and build the library ourselves
	message(STATUS "-- Using external build system for dependency 'Newton'...")

	# [HACK] We need the output directory, but it is not easily reachable
	get_property(lib_file TARGET LocationDummy PROPERTY LOCATION)
	get_filename_component(lib_path ${lib_file} PATH)
	
	# Define the external project
	ExternalProject_Add(${PL_CURRENT_TARGET_NAME}
		DOWNLOAD_DIR ${PL_CURRENT_DOWNLOAD_DIR}
		SOURCE_DIR ${PL_CURRENT_SRC_DIR}
		BINARY_DIR ${PL_CURRENT_BUILD_DIR}
		INSTALL_DIR ${PL_CURRENT_INSTALL_DIR}
		TMP_DIR ${PL_CURRENT_TMP_DIR}
		STAMP_DIR ${PL_CURRENT_STAMP_DIR}
		URL ${NEWTON_URL}
		URL_MD5 ${NEWTON_MD5}
		
		# We have a custom CMake script to build Newton
		PATCH_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/CMake/CMakeLists.txt.in <SOURCE_DIR>/CMakeLists.txt
		
		CMAKE_ARGS
			-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
	)
	
	# Setup paths
	if(WIN32)
		set(${PL_CURRENT_TARGET_LIBRARIES} ${PL_CURRENT_INSTALL_DIR}/lib/newton.lib CACHE INTERNAL "")
		
		# Copy the DLL to Bin
		ExternalProject_Add_Step(${PL_CURRENT_TARGET_NAME} CopyDLL
			COMMAND ${CMAKE_COMMAND} -E copy ${PL_CURRENT_INSTALL_DIR}/bin/Newton.dll ${lib_path}
			DEPENDEES install
		)
	else()
		set(${PL_CURRENT_TARGET_LIBRARIES} ${PL_CURRENT_INSTALL_DIR}/lib/libNewton.so CACHE INTERNAL "")
		
		# Copy the SO to Bin
		ExternalProject_Add_Step(${PL_CURRENT_TARGET_NAME} CopyDLL
			COMMAND ${CMAKE_COMMAND} -E copy ${PL_CURRENT_INSTALL_DIR}/lib/libNewton.so ${lib_path}
			DEPENDEES install
		)
	endif()

	set(${PL_CURRENT_TARGET_INCLUDE_DIRS} ${PL_CURRENT_INSTALL_DIR}/include CACHE INTERNAL "")
	
	# Finalize
	pl_build_external()
#endif()
