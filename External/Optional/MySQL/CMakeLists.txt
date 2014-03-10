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
define_external_project(MySQL)

##################################################
## Configuration
##################################################
set(MYSQL_VERSION "5.6.16")
set(MYSQL_URL "http://dev.mysql.com/get/Downloads/MySQL-5.6/mysql-${MYSQL_VERSION}.tar.gz")
set(MYSQL_MD5 "1d3d91e8459c719bbef7c97bb499634d")

##################################################
## Need to download?
##################################################
# [TODO] Find package possible for MySQL?
#find_package(MYSQL ${MYSQL_VERSION})

# Do we have it?
#if(MYSQL_FOUND)
#	# We will use the library present on the user system
#	# The find module from CMake itself doesn't add the XXX_LIBRARIES variable and XXX_INCLUDE_DIR variable to the cache, which is the correct behaviour.
#	# These variables should normaly only be visible within the cmake file in which the find module was called.
#	# Otherwise the variables would be visible for all other projects if wanted or not
#	# We want this, because this variables are used in other CMake files, which doesn't include this file, so we add the variables to the cache
#	set(MYSQL_LIBRARIES ${MYSQL_LIBRARIES} CACHE INTERNAL "")
#	set(MYSQL_INCLUDE_DIRS ${MYSQL_INCLUDE_DIR} CACHE INTERNAL "")
#else()
	# We'll need to download and build the library ourselves
	message(STATUS "-- Using external build system for dependency 'MySQL'...")

	# [TODO] On Linux, the MySQL build without server fails, we should add a custom patch for their CMakeLists.txt file
	# so that it builds. Leads here: http://bugs.mysql.com/bug.php?id=66000
	if(NOT LINUX)
		set(BUILD_WITHOUT_SERVER ON)
	else()
		set(BUILD_WITHOUT_SERVER OFF)
	endif()	

	# Define the external project
	ExternalProject_Add(${CMAKETOOLS_CURRENT_TARGET}
		DOWNLOAD_DIR ${CMAKETOOLS_CURRENT_DOWNLOAD_DIR}
		SOURCE_DIR ${CMAKETOOLS_CURRENT_SRC_DIR}
		BINARY_DIR ${CMAKETOOLS_CURRENT_BUILD_DIR}
		INSTALL_DIR ${CMAKETOOLS_CURRENT_INSTALL_DIR}
		TMP_DIR ${CMAKETOOLS_CURRENT_TMP_DIR}
		STAMP_DIR ${CMAKETOOLS_CURRENT_STAMP_DIR}
		URL ${MYSQL_URL}
		URL_MD5 ${MYSQL_MD5}

		# "Patch" the CMake script - see the file being replaced for more info
		PATCH_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKETOOLS_CURRENT_DIR}/Patch/install_macros.cmake <SOURCE_DIR>/cmake
		
		CMAKE_ARGS
			-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
			
		CMAKE_CACHE_ARGS
			-DBUILD_CONFIG:BOOL=OFF
			-DWITHOUT_SERVER:BOOL=${BUILD_WITHOUT_SERVER}
			-DWITH_UNIT_TESTS:BOOL=OFF
	)
	
	# Setup paths
	if(WIN32)
		set(MYSQL_LIBRARIES
			${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/libmysql.lib
			CACHE INTERNAL "")
			
		# Copy the DLL to Bin
		ExternalProject_Add_Step(${CMAKETOOLS_CURRENT_TARGET} CopyDLL
			COMMAND ${CMAKE_COMMAND} -E copy ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/libmysql.dll ${PL_RUNTIME_BIN_GITDIR}
			DEPENDEES install
		)
	else()
		set(MYSQL_LIBRARIES
			${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/libmysqlclient.so
			CACHE INTERNAL "")
			
		# Copy the SO to Bin
		ExternalProject_Add_Step(${CMAKETOOLS_CURRENT_TARGET} CopyDLL
			COMMAND ${CMAKE_COMMAND} -E copy ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/libmysqlclient.so ${PL_RUNTIME_BIN_GITDIR}
			DEPENDEES install
		)
	endif()
	set(MYSQL_INCLUDE_DIRS ${CMAKETOOLS_CURRENT_INSTALL_DIR}/include CACHE INTERNAL "")
	
	# Dependencies
	add_dependencies(External-Optional ${CMAKETOOLS_CURRENT_TARGET})
#endif()
