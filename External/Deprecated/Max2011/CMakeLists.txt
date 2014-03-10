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
define_external_project(Max2011)

##################################################
## INCLUDES AND LIBRARIES
##################################################
# [TODO] System libs possible for Max2011?
#if(CMAKETOOLS_USE_SYSTEM_LIBS)
	##################################################
	## Use system libraries
	##################################################
#	find_package(Max2011)
#	set(MAX2011_INCLUDE_DIRS ${MAX2011_INCLUDE_DIR_MAX2011} CACHE INTERNAL "")
#	set(MAX2011_LIBRARIES ${MAX2011_LIBRARY} CACHE INTERNAL "")
#else()
	# Build external
	message(STATUS "Using build script for external '${CMAKETOOLS_CURRENT_NAME}'")

	##################################################
	## Build external
	##################################################
	add_dependencies(External-Optional ${CMAKETOOLS_CURRENT_TARGET})
	add_dependencies(External-Clean ${CMAKETOOLS_CURRENT_TARGET}-Clean)
	set(MAX2011_INCLUDE_DIRS ${CMAKETOOLS_CURRENT_INCLUDE_DIR} CACHE INTERNAL "")
	if(WIN32)
		set(MAX2011_LIBRARIES ${CMAKETOOLS_CURRENT_LIB_DIR}/core.lib ${CMAKETOOLS_CURRENT_LIB_DIR}/maxutil.lib ${CMAKETOOLS_CURRENT_LIB_DIR}/bmm.lib ${CMAKETOOLS_CURRENT_LIB_DIR}/geom.lib ${CMAKETOOLS_CURRENT_LIB_DIR}/IGame.lib CACHE INTERNAL "")
	endif()

	##################################################
	## Check
	##################################################
	external_check()

	##################################################
	## Fetch
	##################################################
	external_fetch()

	##################################################
	## Configure
	##################################################
	external_configure()

	##################################################
	## Build
	##################################################
	external_build()

	##################################################
	## Install
	##################################################
	external_install()

	##################################################
	## Clean
	##################################################
	external_clean()

# [TODO] System libs possible for Max2011?
#endif()
