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
cmaketools_doc_project(SDKBrowser)

##################################################
## Prerequisites
##################################################

# Create title file used within the PixelLight SDK browser
configure_file("${CMAKE_SOURCE_DIR}/Docs/SDKBrowser/index.html.in" "${CMAKE_SOURCE_DIR}/Docs/SDKBrowser/index.html")

##################################################
## Html help compiler
##################################################
cmaketools_add_chm_document(${CMAKETOOLS_CURRENT_TARGET} index.hhp)

##################################################
## Dependencies
##################################################
add_dependencies(Docs Docs-SDKBrowser)

##################################################
## Post-Build
##################################################

# CHM
add_custom_command(TARGET Docs-${CMAKETOOLS_CURRENT_TARGET}
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKETOOLS_CURRENT_TARGET_CHM} ${PL_BIN_DIR}
)

##################################################
## Install
##################################################

# CHM
install(FILES ${CMAKETOOLS_CURRENT_TARGET_CHM}
	DESTINATION ${PL_INSTALL_ROOT}		COMPONENT Docs
)
