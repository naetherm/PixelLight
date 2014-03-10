#*********************************************************#
#*  File: CMakeTools_Externals.cmake                     *
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
## CMakeTools externals
##
## These macros can be used to build external
## dependencies automatically for your project
##################################################

##################################################
## MACRO: define_external_project
##
## Define an externals project
##   name		name of the project (e.g. "TestProject")
##################################################
macro(define_external_project name)
	# We are now using ExternalProject so there is no "real" project defined here.
	# Instead, we just define a bunch of variables we will be using in our build scripts.
	
	# Set project variables
	set(CMAKETOOLS_CURRENT_NAME ${name})
	set(CMAKETOOLS_CURRENT_TARGET External-${name})
	
	# Where is the current CMakeLists.txt located
	set(CMAKETOOLS_CURRENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
	
	# Root for external projects' directories
	set(CMAKETOOLS_CURRENT_ROOT_DIR "${CMAKE_CURRENT_BINARY_DIR}")
	# Where to download the library
	set(CMAKETOOLS_CURRENT_DOWNLOAD_DIR "${CMAKETOOLS_CURRENT_ROOT_DIR}/Download")
	# Where to extract the library sources
	set(CMAKETOOLS_CURRENT_SRC_DIR "${CMAKETOOLS_CURRENT_ROOT_DIR}/Source")
	# Where to build the library
	set(CMAKETOOLS_CURRENT_BUILD_DIR "${CMAKETOOLS_CURRENT_ROOT_DIR}/Build")
	# Where to build the library
	set(CMAKETOOLS_CURRENT_BUILD_DIR "${CMAKETOOLS_CURRENT_ROOT_DIR}/Build")
	# Where to install the library
	set(CMAKETOOLS_CURRENT_INSTALL_DIR "${CMAKETOOLS_CURRENT_ROOT_DIR}/Install")
	# Where to put temporary files
	set(CMAKETOOLS_CURRENT_TMP_DIR "${CMAKETOOLS_CURRENT_ROOT_DIR}/Temp")
	# Where to put stamp files
	set(CMAKETOOLS_CURRENT_STAMP_DIR "${CMAKETOOLS_CURRENT_ROOT_DIR}/Stamp")
	
	# The default location of installed include files (this may be different for some libraries)
	set(CMAKETOOLS_CURRENT_INCLUDE_DIR "${CMAKETOOLS_CURRENT_INSTALL_DIR}/include")
	# The default location of installed lib files (this may be different for some libraries)
	set(CMAKETOOLS_CURRENT_LIB_DIR "${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib")
endmacro(define_external_project name)
