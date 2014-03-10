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
project(Tools)

##################################################
## Target
##################################################
add_custom_target(Tools)

##################################################
## Prerequisites
##################################################

# Create "Tools"-directory
file(MAKE_DIRECTORY ${PL_BIN_DIR}/Tools)
set(PL_TOOLS_BIN_DIR "${PL_BIN_DIR}/Tools/${CMAKETOOLS_TARGET_ARCHBITSIZE}")
file(MAKE_DIRECTORY ${PL_TOOLS_BIN_DIR})

##################################################
## Projects
##################################################
if(PL_NATIVE_PLPROJECT)
	add_subdirectory(PLProject)
endif()
if(PL_TOOL_PLUPGRADE)
	add_subdirectory(PLUpgrade)
endif()
if(PL_TOOL_PLINSTALL)
	add_subdirectory(PLInstall)
endif()
add_subdirectory(PLViewer)
if(PL_EXPORTER_3DSMAX_2008)
	add_subdirectory(PL3dsMaxSceneExport_2008)
endif()
if(PL_EXPORTER_3DSMAX_2009)
	add_subdirectory(PL3dsMaxSceneExport_2009)
endif()
if(PL_EXPORTER_3DSMAX_2010)
	add_subdirectory(PL3dsMaxSceneExport_2010)
endif()
if(PL_EXPORTER_3DSMAX_2011)
	add_subdirectory(PL3dsMaxSceneExport_2011)
endif()
if(PL_EXPORTER_3DSMAX_2012)
	add_subdirectory(PL3dsMaxSceneExport_2012)
endif()
if(PL_EXPORTER_3DSMAX_2013)
	add_subdirectory(PL3dsMaxSceneExport_2013)
endif()
if(PL_PLUGIN_FRONTEND_QT)
	add_subdirectory(PLViewerQt)
endif()

##################################################
## Dependencies
##################################################
add_dependencies(PixelLight Tools)

##################################################
## Install
##################################################

install(FILES	${CMAKE_SOURCE_DIR}/Tools/FindPixelLight.cmake
	DESTINATION ${PL_INSTALL_ROOT}/Tools		COMPONENT SDK
)

# "WhereIsTheLog.txt"-file
install(FILES ${CMAKE_SOURCE_DIR}/Bin/Tools/${CMAKETOOLS_TARGET_ARCHBITSIZE}/WhereIsTheLog.txt
	DESTINATION ${PL_INSTALL_TOOLS_BIN}	COMPONENT SDK
)
