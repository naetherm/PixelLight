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
#if(NOT CMAKETOOLS_DONT_USE_SYSTEM_LIBS)
#	find_package(Qt4 ${QT_VERSION})
#endif()

if(Qt4_FOUND)
	#[TODO]
else()
	# Download and build Qt from source
	message(STATUS "-- Using external build system for dependency 'Qt'...")
	
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
		set(QT_INSTALL ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/bin <INSTALL_DIR>/bin
			COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/lib <INSTALL_DIR>/lib
			COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/include <INSTALL_DIR>/include
			COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/src <INSTALL_DIR>/src)
	else()
		# Set build variables
		set(QT_CONFIGURE echo yes | sh ./configure --prefix=${CMAKETOOLS_CURRENT_INSTALL_DIR} -opensource -release -no-accessibility -no-qt3support -no-openvg -no-gif -no-libpng -no-libmng -no-libtiff -no-libjpeg -no-openssl -no-phonon -no-phonon-backend -no-multimedia -no-audio-backend -no-webkit -no-script -no-scripttools -no-declarative -no-declarative-debug -no-xmlpatterns -nomake examples -nomake demos -nomake tests -nomake tools -nomake docs)
		set(QT_BUILD make -j4)
		set(QT_INSTALL make install)
	endif()
	
	# Add the external project
	ExternalProject_Add(${CMAKETOOLS_CURRENT_TARGET}
		DOWNLOAD_DIR ${CMAKETOOLS_CURRENT_DOWNLOAD_DIR}
		SOURCE_DIR ${CMAKETOOLS_CURRENT_SRC_DIR}
		INSTALL_DIR ${CMAKETOOLS_CURRENT_INSTALL_DIR}
		TMP_DIR ${CMAKETOOLS_CURRENT_TMP_DIR}
		STAMP_DIR ${CMAKETOOLS_CURRENT_STAMP_DIR}
		URL ${QT_URL}
#		URL_MD5 ${QT_MD5}
		BUILD_IN_SOURCE 1
		
		# Apply our code patch
		# Unfortunately, CMake's ExternalProject seems to apply .patch files multiple times when using
		# multi-configuration builds (MSVC) so we have our patched file stored locally
		PATCH_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKETOOLS_CURRENT_DIR}/Patch/MathExtras.h <SOURCE_DIR>/src/3rdparty/javascriptcore/JavaScriptCore/wtf
		
		CONFIGURE_COMMAND ${QT_CONFIGURE}
		
		BUILD_COMMAND ${QT_BUILD}
		
		INSTALL_COMMAND ${QT_INSTALL}
			
		CMAKE_ARGS
			-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
	)
	
	# Setup paths
	set(QT_INCLUDE_DIRS
		${CMAKETOOLS_CURRENT_INSTALL_DIR}/include
		${CMAKETOOLS_CURRENT_INSTALL_DIR}/include/Qt
		${CMAKETOOLS_CURRENT_INSTALL_DIR}/include/QtGui
		${CMAKETOOLS_CURRENT_INSTALL_DIR}/include/QtCore CACHE INTERNAL "")
		
	if(WIN32)
		set(QT_LIBRARIES ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/QtCore4.lib ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/QtGui4.lib ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/qtmain.lib CACHE INTERNAL "")

		# Set "FindQt4.cmake"-variables
		set(QT_TOOLS_DIR ${CMAKETOOLS_CURRENT_DIR}/Tools/Win)
		set(QT_QMAKE_EXECUTABLE ${QT_TOOLS_DIR}/qmake.exe)	# Tell Qt were to find qmake
		
		# Copy the DLL to Bin
		ExternalProject_Add_Step(${CMAKETOOLS_CURRENT_TARGET} CopyDLL
			COMMAND ${CMAKE_COMMAND} -E copy ${CMAKETOOLS_CURRENT_INSTALL_DIR}/bin/QtCore4.dll ${PL_RUNTIME_BIN_GITDIR}
			COMMAND ${CMAKE_COMMAND} -E copy ${CMAKETOOLS_CURRENT_INSTALL_DIR}/bin/QtGui4.dll ${PL_RUNTIME_BIN_GITDIR}
			DEPENDEES install
		)
	elseif(LINUX)
		set(QT_LIBRARIES ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/libQtCore.so ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/libQtGui.so CACHE INTERNAL "")

		# Set "FindQt4.cmake"-variables
		set(QT_TOOLS_DIR ${CMAKETOOLS_CURRENT_DIR}/Tools/Linux)
		set(QT_QMAKE_EXECUTABLE ${QT_TOOLS_DIR}/qmake)	# Tell Qt were to find qmake
	endif()
	
	set(QT_EXTERNAL_DIR ${CMAKETOOLS_CURRENT_INSTALL_DIR} CACHE INTERNAL "")

	# Setup paths to prebuilt Qt build tools and future libraries, so that find_package will locate them
	
	# Unset QT_QMAKE_EXECUTABLE_LAST otherwise FindQt4.cmake triggers a query to qmake for the Qt library path.
	# The Qt library path is hardcoded into qmake, but we can't relay on the reported path of qmake because the harcoded path is absolute
	# and will not always match with the location of the Qt libraries which are bundled with PixelLight as an external package.
	unset(QT_QMAKE_EXECUTABLE_LAST CACHE)

	# We have to set following variables otherwise the FindQt4.cmake won't find either the executables uic, rcc, moc and the qt library path
	# The QT_INCLUDE_DIR path has also be set, because this variable is an additional condition that FindQt4.cmake sets QT_FOUND to true
	set(QT_INCLUDE_DIR	${CMAKETOOLS_CURRENT_INSTALL_DIR}/include/)	# Path to "include" of Qt4
	set(QT_BINARY_DIR 	${QT_TOOLS_DIR}/)							# Path to "bin" of Qt4
	set(QT_LIBRARY_DIR	${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/)		# Path to "lib" of Qt4

	# We have to set both, QT_QTCORE_LIBRARY and QT_QTCORE_LIBRARY_RELEASE (we could also set QT_QTCORE_LIBRARY_DEBUG instead of QT_QTCORE_LIBRARY_RELEASE).
	# QT_QTCORE_LIBRARY is used to determine if qmake should be queried for the library path to the Qt libraries (which has the problems described above why unsetting QT_QMAKE_EXECUTABLE_LAST).
	# QT_QTCORE_LIBRARY_RELEASE must be set because this variable is used in FindQt4.cmake in order to determine whether or not the path to the Qt libraries has been found.
	if(WIN32)
		set(QT_QTCORE_LIBRARY			${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/QtCore4.lib)	# Path to QtCore library
		set(QT_QTCORE_LIBRARY_RELEASE	${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/QtCore4.lib)	# Path to QtCore library (release version)
	elseif(LINUX)
		set(QT_QTCORE_LIBRARY			${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/libQtCore.so)	# Path to QtCore library
		set(QT_QTCORE_LIBRARY_RELEASE	${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/libQtCore.so)	# Path to QtCore library (release version)
	endif()

	# Find Qt4 libraries and set all required variables
	find_package(Qt4 REQUIRED)

	# Dependencies
	add_dependencies(External-Optional ${CMAKETOOLS_CURRENT_TARGET})
endif()
