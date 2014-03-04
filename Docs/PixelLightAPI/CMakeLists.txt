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
cmaketools_doc_project(PixelLightAPI)

##################################################
## Prerequisites
##################################################

# Create footer file used within the PixelLight API documentation
configure_file("${CMAKE_SOURCE_DIR}/Docs/PixelLightAPI/footer.html.in" "${CMAKE_SOURCE_DIR}/Docs/PixelLightAPI/footer.html")

##################################################
## Doxygen documentation
##################################################
cmaketools_add_doxygen_docs(${CMAKETOOLS_CURRENT_TARGET} doxyfile.in)

##################################################
## Html help compiler
##################################################
# The compressed PixelLight API document can only be build when the Microsoft Html Help Compiler (http://msdn.microsoft.com/en-us/library/ms669985) has been found by CMake,
# if CMake was unable to find it, ignore the following document in order to avoid confusing confusing errors when building the project.
# The following macro is internally checking for the "HTML_HELP_COMPILER"-variable and set's the "CMAKETOOLS_CURRENT_TARGET_CHM"-variable on success.
cmaketools_add_chm_document(${CMAKETOOLS_CURRENT_TARGET} ${CMAKETOOLS_CURRENT_TARGET_HHP})

##################################################
## Dependencies
##################################################
if(CMAKETOOLS_CURRENT_TARGET_CHM)
	add_dependencies(Docs-PixelLightAPI-CHM	Docs-PixelLightAPI-Doxygen)
endif()
add_dependencies(Docs						Docs-PixelLightAPI)

##################################################
## Post-Build
##################################################

# CHM
if(CMAKETOOLS_CURRENT_TARGET_CHM)
	add_custom_command(TARGET Docs-${CMAKETOOLS_CURRENT_TARGET}
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKETOOLS_CURRENT_TARGET_CHM} ${PL_DOCS_DIR}
	)
endif()

##################################################
## Install
##################################################

# CHM
if(CMAKETOOLS_CURRENT_TARGET_CHM)
	install(FILES ${CMAKETOOLS_CURRENT_TARGET_CHM}
		DESTINATION ${PL_INSTALL_DOCS}		COMPONENT Docs
	)
endif()
