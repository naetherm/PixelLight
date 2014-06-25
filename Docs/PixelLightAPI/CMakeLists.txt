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
## Configure input files
##################################################

# Dot graphing option
set(PL_DOXYGEN_DOT_SWITCH NO)
if(DOXYGEN_DOT_FOUND)
	set(PL_DOXYGEN_DOT_SWITCH YES)
endif()

# List of libraries to include in the build
foreach(lib ${PL_DOXYGEN_PATHS})
	set(PL_DOXYGEN_INCLUDE_PATHS "${PL_DOXYGEN_INCLUDE_PATHS} ${lib}")
endforeach()

# Configure doxyfile
set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/doxyfile)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doxyfile.in ${doxyfile})

# Configure custom footer
#configure_file(${CMAKE_CURRENT_SOURCE_DIR}/footer.html.in )


##################################################
## Run doxygen
##################################################

set(PL_CURRENT_TARGET_NAME Doxygen)

project(${PL_CURRENT_TARGET_NAME})

add_custom_target(${PL_CURRENT_TARGET_NAME}
	COMMAND ${DOXYGEN} \"${doxyfile}\"
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/doxyfile.in
)

set_target_properties(${PL_CURRENT_TARGET_NAME}
	PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD 1)

# Solution folders
set_property(
	TARGET ${PL_CURRENT_TARGET_NAME}
	PROPERTY FOLDER "Docs"
)