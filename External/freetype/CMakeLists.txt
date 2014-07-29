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
pl_project(freetype)

##################################################
## Configuration
##################################################
set(FREETYPE_VERSION "2.5.2")
set(FREETYPE_URL "http://sourceforge.net/projects/freetype/files/freetype2/${FREETYPE_VERSION}/freetype-${FREETYPE_VERSION}.tar.gz")
set(FREETYPE_MD5 "004320381043d275c4e28bbacf05a1b7")

##################################################
## Need to download?
##################################################
#if(NOT CMAKETOOLS_DONT_USE_SYSTEM_LIBS)
#	find_package(FREETYPE ${FREETYPE_VERSION})
#endif()

# Do we have it?
if(FREETYPE_FOUND)
	# We will use the library present on the user system
	# The find module from CMake itself doesn't add the XXX_LIBRARIES variable and XXX_INCLUDE_DIR variable to the cache, which is the correct behaviour.
	# These variables should normaly only be visible within the cmake file in which the find module was called.
	# Otherwise the variables would be visible for all other projects if wanted or not
	# We want this, because this variables are used in other CMake files, which doesn't include this file, so we add the variables to the cache
	set(FREETYPE_LIBRARIES ${FREETYPE_LIBRARY} CACHE INTERNAL "")
	set(FREETYPE_INCLUDE_DIRS ${FREETYPE_INCLUDE_DIR_freetype2} ${FREETYPE_INCLUDE_DIR_ft2build} CACHE INTERNAL "")
else()
	# We'll need to download and build the library ourselves
	message(STATUS "-- Using external build system for dependency 'freetype2'...")

	# We are building static library out of freetype - we need to enable PIC support!
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
		URL ${FREETYPE_URL}
		URL_MD5 ${FREETYPE_MD5}
		
		CMAKE_ARGS
			-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>

		CMAKE_CACHE_ARGS ${COMPILER_FLAGS}
	)
	
	# Setup paths
	if(WIN32)
		set(${PL_CURRENT_TARGET_LIBRARIES} ${PL_CURRENT_INSTALL_DIR}/lib/freetype.lib CACHE INTERNAL "")
	else()
		set(${PL_CURRENT_TARGET_LIBRARIES} ${PL_CURRENT_INSTALL_DIR}/lib/libfreetype.a CACHE INTERNAL "")
	endif()

	set(${PL_CURRENT_TARGET_INCLUDE_DIRS} ${PL_CURRENT_INSTALL_DIR}/include ${PL_CURRENT_INSTALL_DIR}/include/freetype2 CACHE INTERNAL "")
	
	# Finalize
	pl_build_external()
endif()
