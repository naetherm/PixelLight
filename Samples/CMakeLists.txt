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
project(Samples)

##################################################
## Prerequisites
##################################################

# Create "Samples"-directory
file(MAKE_DIRECTORY ${PL_BIN_DIR}/Samples)
set(PL_SAMPLES_BIN_DIR "${PL_BIN_DIR}/Samples/${CMAKETOOLS_TARGET_ARCHBITSIZE}")
file(MAKE_DIRECTORY ${PL_SAMPLES_BIN_DIR})
file(MAKE_DIRECTORY ${PL_BIN_DIR}/Samples/Data)

# Copy samples data
file(MAKE_DIRECTORY "${PL_BIN_DIR}/Samples/Data")
copy_files("${CMAKE_SOURCE_DIR}/Bin/Samples/Data/*.*" "${PL_BIN_DIR}/Samples/Data")

if(LINUX AND NOT ANDROID)
	# This is needed so that the helper runner script works
	set(CMAKETOOLS_BASEPATH_GO_UPS "/../../")
endif()

##################################################
## Target
##################################################
add_custom_target(Samples)

##################################################
## Projects
##################################################
add_subdirectory(01Application)
add_subdirectory(02CommandLine)
add_subdirectory(03Signals)
add_subdirectory(04Dirs)
add_subdirectory(05NetworkChat)
add_subdirectory(30Input)
add_subdirectory(40ScriptHelloWorld)
add_subdirectory(41ScriptGlobalVariables)
add_subdirectory(42ScriptFunctors)
add_subdirectory(43ScriptBindings)
add_subdirectory(44ScriptOOP)
add_subdirectory(45ScriptApplication)
add_subdirectory(50RendererTriangle)
add_subdirectory(51RendererTexturing)
add_subdirectory(52RendererMesh)
add_subdirectory(54RendererRTT)
add_subdirectory(55RendererTessellation)
add_subdirectory(56RendererInstancing)
add_subdirectory(60Scene)
add_subdirectory(61Sound)
add_subdirectory(62Physics)
add_subdirectory(63PhysicsRagdoll)
add_subdirectory(64Picking)
add_subdirectory(65PostProcess)
add_subdirectory(67Game2D)
# PLGui
if(PL_PLGUI)
	add_subdirectory(20Gui)
#	add_subdirectory(21GuiWidgets) # [TODO]
	# PLFrontendPLGui
	if(PL_PLUGIN_FRONTEND_PLGUI)
		add_subdirectory(53RendererMultiView)
		add_subdirectory(66IngameGUI)
	endif()
	# PLlibRocket
	if(PL_PLUGIN_ENGINE_LIBROCKET)
		add_subdirectory(71libRocket)
	endif()
endif()
# PLFrontendQt
if(PL_PLUGIN_FRONTEND_QT)
	add_subdirectory(80QtHelloWorld)
	add_subdirectory(81QtScene)
	add_subdirectory(82QtInputHandling)
endif()
# SPARK_PL
if(PL_PLUGIN_ENGINE_SPARK)
	add_subdirectory(70SPARK)
endif()

##################################################
## Dependencies
##################################################
add_dependencies(PixelLight Samples)

##################################################
## Install
##################################################

# "WhereIsTheLog.txt"-file
install(FILES ${CMAKE_SOURCE_DIR}/Bin/Samples/${CMAKETOOLS_TARGET_ARCHBITSIZE}/WhereIsTheLog.txt
	DESTINATION ${PL_INSTALL_SAMPLES}/Bin/${CMAKETOOLS_TARGET_ARCHBITSIZE}	COMPONENT SDK
)

# Data
install(DIRECTORY ${PL_BIN_DIR}/Samples/Data
	DESTINATION ${PL_INSTALL_SAMPLES_DATA}	COMPONENT SDK	REGEX ${PL_EXCLUDE_FILE_EXTENSIONS} EXCLUDE
)

# Microsoft Visual Studio Solution
if(WIN32)
	install(FILES ${CMAKE_SOURCE_DIR}/Samples/Samples.sln
		DESTINATION ${PL_INSTALL_SAMPLES}	COMPONENT SDK
	)
endif()
