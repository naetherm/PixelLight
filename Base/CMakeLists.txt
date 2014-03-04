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
project(Base)

##################################################
## Prerequisites
##################################################

# Copy runtime data
file(MAKE_DIRECTORY "${PL_BIN_DIR}/Runtime/Data")
copy_files("${CMAKE_SOURCE_DIR}/Bin/Runtime/Data/*.zip" "${PL_BIN_DIR}/Runtime/Data")

##################################################
## Target
##################################################
add_custom_target(Base)

##################################################
## Projects
##################################################
add_subdirectory(PLCore)
add_subdirectory(PLGraphics)
add_subdirectory(PLMath)
add_subdirectory(PLDatabase)
add_subdirectory(PLInput)
add_subdirectory(PLRenderer)
add_subdirectory(PLMesh)
add_subdirectory(PLScene)
add_subdirectory(PLPhysics)
add_subdirectory(PLSound)
add_subdirectory(PLEngine)
if(PL_PLGUI)
	add_subdirectory(PLGui)
endif()

##################################################
## Dependencies
##################################################
add_dependencies(PixelLight Base)

##################################################
## Install
##################################################

# Data
install(DIRECTORY ${CMAKE_SOURCE_DIR}/Bin/Runtime/Data
	DESTINATION ${PL_INSTALL_RUNTIME}		COMPONENT Runtime	REGEX ${PL_EXCLUDE_FILE_EXTENSIONS} EXCLUDE
)
