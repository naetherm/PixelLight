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
define_external_project(Qt)

##################################################
## Configuration
##################################################
set(QT_VERSION "4.8.5")
set(QT_URL "http://download.qt-project.org/official_releases/qt/4.8/${QT_VERSION}/qt-everywhere-opensource-src-${QT_VERSION}.tar.gz")
#set(QT_MD5 "edd26ef2bf561786b2185f2b8afda78b")

##################################################
## Need to download?
##################################################
#TODO: what about debug configuration?
message(STATUS "Looking for external dependency 'Qt' with minimum version ${QT_VERSION}...")
#find_package(Qt4 ${QT_VERSION})

if(Qt4_FOUND)
	#[TODO]
else()
	# Download and build Qt from source
	if(WIN32)
		# Try to use jom if available
		find_program(JOM jom)
		mark_as_advanced(JOM)
		if(JOM)
			set(QT_BUILD_PROGRAM "${JOM}")
		else()
			set(QT_BUILD_PROGRAM nmake)
		endif()
		
		# Create our custom CMake config
		configure_file(${CMAKETOOLS_CURRENT_DIR}/CMake/QtWinConfig.cmake.in ${CMAKETOOLS_CURRENT_BUILD_DIR}/QtWinConfig.cmake)
		
		# Set build variables
		set(QT_CONFIGURE ${CMAKE_COMMAND} -P ${CMAKETOOLS_CURRENT_BUILD_DIR}/QtWinConfig.cmake)
		set(QT_BUILD ${QT_BUILD_PROGRAM})
		set(QT_INSTALL "")
	else()
		#[TODO]
		
	endif()
	
	# Add the external project
	ExternalProject_Add(${CMAKETOOLS_CURRENT_TARGET}
		DOWNLOAD_DIR ${CMAKETOOLS_CURRENT_DOWNLOAD_DIR}
		SOURCE_DIR ${CMAKETOOLS_CURRENT_SRC_DIR}
#		BINARY_DIR ${CMAKETOOLS_CURRENT_BUILD_DIR}
		INSTALL_DIR ${CMAKETOOLS_CURRENT_INSTALL_DIR}
		TMP_DIR ${CMAKETOOLS_CURRENT_TMP_DIR}
		STAMP_DIR ${CMAKETOOLS_CURRENT_STAMP_DIR}
		URL ${QT_URL}
#		URL_MD5 ${QT_MD5}
#		TIMEOUT 30
		BUILD_IN_SOURCE 1
		
		# Apply our code patch
		# Unfortunately, CMake's ExternalProject seems to apply .patch files multiple times when using
		# multi-configuration builds (MSVC) so we have our patched file stored locally
		PATCH_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKETOOLS_CURRENT_DIR}/Patch/MathExtras.h <SOURCE_DIR>/src/3rdparty/javascriptcore/JavaScriptCore/wtf
		
		CONFIGURE_COMMAND ${QT_CONFIGURE}
		BUILD_COMMAND ${QT_BUILD}
#		INSTALL_COMMAND ${QT_INSTALL}
		CMAKE_ARGS
			-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
	)

	add_dependencies(External-Optional ${CMAKETOOLS_CURRENT_TARGET})
endif()
