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
project(Docs)

##################################################
## Prerequisites
##################################################

# Create 'Docs'-directory
set(PL_DOCS_DIR "${PL_BIN_DIR}/Docs")
file(MAKE_DIRECTORY ${PL_DOCS_DIR})

# Create temp dir
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Common)

# Copy file format samples
file(MAKE_DIRECTORY "${PL_DOCS_DIR}/FileFormats")
copy_files("${CMAKE_SOURCE_DIR}/Docs/FileFormats/*.*" "${PL_DOCS_DIR}/FileFormats")

# Create title file used within the PixelLight documentations
configure_file("${CMAKE_SOURCE_DIR}/Docs/Common/Title.tex.in" "${CMAKE_SOURCE_DIR}/Docs/Common/Title.tex")

##################################################
## Target
##################################################
add_custom_target(Docs)
set_target_properties(Docs PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD 1)

add_custom_target(Docs-install)
set_target_properties(Docs-install PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD 1)

##################################################
## Projects
##################################################
# The following documents require LaTex and dvipdf,
# if CMake was unable to find it, ignore the following documents in order to avoid confusing confusing errors when building the project
if(LATEX_COMPILER)
	if(DVIPDF_CONVERTER)
		add_subdirectory(PixelLightFeatures)
		add_subdirectory(PixelLightBase)
		add_subdirectory(PixelLightCompositing)
		add_subdirectory(PixelLightConventions)
		add_subdirectory(PixelLightBuild)
		add_subdirectory(PixelLightScript)
		add_subdirectory(PixelLight3dsMaxSceneExport)
	else()
		message(STATUS "Ignoring PixelLightFeatures, PixelLightBase, PixelLightCompositing, PixelLightConventions, PixelLightBuild, PixelLightScript and PixelLight3dsMaxSceneExport documentation because dvipdf was not found")
	endif()
else()
	message(STATUS "Ignoring PixelLightFeatures, PixelLightBase, PixelLightCompositing, PixelLightConventions, PixelLightBuild, PixelLightScript and PixelLight3dsMaxSceneExport documentation because LaTeX was not found")
endif()

# The PixelLight API documentation requires Doxygen (http://www.doxygen.org),
# if CMake was unable to find it, ignore the following document in order to avoid confusing confusing errors when building the project
if(DOXYGEN_FOUND)
	add_subdirectory(PixelLightAPI)
else()
	message(STATUS "Ignoring PixelLight API documentation because Doxygen (http://www.doxygen.org) was not found")
endif()

# The SDK browser can only be build when the Microsoft Html Help Compiler (http://msdn.microsoft.com/en-us/library/ms669985) has been found by CMake,
# if CMake was unable to find it, ignore the following document in order to avoid confusing confusing errors when building the project
if(HTML_HELP_COMPILER)
	add_subdirectory(SDKBrowser)
else()
	message(STATUS "Ignoring PixelLight SDK browser because Microsoft Html Help Compiler (http://msdn.microsoft.com/en-us/library/ms669985) was not found")
endif()

##################################################
## Dependencies
##################################################
add_dependencies(PixelLight Docs)

##################################################
## Install
##################################################

install(DIRECTORY ${CMAKE_SOURCE_DIR}/Docs/FileFormats
	DESTINATION ${PL_INSTALL_DOCS}		COMPONENT Docs		REGEX ${PL_EXCLUDE_FILE_EXTENSIONS} EXCLUDE
)

add_custom_command(TARGET Docs-install
 	COMMAND ${CMAKE_COMMAND} -P cmake_install.cmake
)
