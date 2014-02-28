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
define_project(PLFrontendQt)

##################################################
## Include directories
##################################################
add_include_directories(
	include
	include/PLFrontendQt
	${CMAKE_SOURCE_DIR}/Base/PLCore/include
	${CMAKE_SOURCE_DIR}/Base/PLMath/include
	${CMAKE_SOURCE_DIR}/Base/PLGraphics/include
	${CMAKE_SOURCE_DIR}/Base/PLInput/include
	${CMAKE_SOURCE_DIR}/Base/PLRenderer/include
	${CMAKE_SOURCE_DIR}/Base/PLMesh/include
	${CMAKE_SOURCE_DIR}/Base/PLScene/include
	${CMAKE_SOURCE_DIR}/Base/PLEngine/include

	# We need the following to be able to include headers produced by uic in our code
	# (CMAKE_CURRENT_BINARY_DIR holds a path to the build directory of the current project, while INCLUDE_DIRECTORIES() works just like INCLUDEPATH from qmake)
	${CMAKE_CURRENT_BINARY_DIR}
	${QT_INCLUDE_DIRS}
)

##################################################
## Special Qt stuff (see http://www.qtcentre.org/wiki/index.php?title=Compiling_Qt4_apps_with_CMake for more information)
##################################################
# Include files with Qt relevant information
set(INC_FILES
	include/PLFrontendQt/QPLContext.h
	include/PLFrontendQt/Application.h
	include/PLFrontendQt/QPLDebugDialog.h
	include/PLFrontendQt/QPLRenderWindow.h
	include/PLFrontendQt/QPLSceneContext.h
	include/PLFrontendQt/QPLInputHandler.h
	include/PLFrontendQt/DataModels/Helper.h
	include/PLFrontendQt/DataModels/TreeModelBase.h
	include/PLFrontendQt/DataModels/SceneNodeInfoModel.h
	include/PLFrontendQt/DataModels/SceneGraphTreeModel.h
	include/PLFrontendQt/DataModels/PLIntrospectionModel.h
	include/PLFrontendQt/DataModels/PLTreeItemsDelegate.h
	include/PLFrontendQt/DataModels/ModelIndexMimeData.h
	include/PLFrontendQt/DataModels/SceneRendererDataModel/SceneRendererDataModel.h
	include/PLFrontendQt/DataModels/RTTIInfoModels/ClassListModel.h
	include/PLFrontendQt/DataModels/RTTIInfoModels/ClassInfoModel.h
	include/PLFrontendQt/DataModels/TreeSortAndFilterProxyModel.h
	include/PLFrontendQt/External/qtcolorbutton.h
	include/PLFrontendQt/External/filterwidget.h
	include/PLFrontendQt/Widget/ClassInfoWidget.h
	include/PLFrontendQt/Widget/ClassListWidget.h
	include/PLFrontendQt/Widget/RTTIBrowserWidget.h
	include/PLFrontendQt/Widget/FilterWidgetWithFilterTypSelector.h
	include/PLFrontendQt/Widget/SceneGraphMenu.h
	include/PLFrontendQt/DockWidget/DockWidgetSceneGraphQObject.h
	include/PLFrontendQt/DockWidget/DockWidgetSceneRendererQObject.h
)

# Qt .ui files
set(UI_FILES
	include/PLFrontendQt/QPLDebugDialog.ui
)

# Qt .qrc files
set(QRC_FILES
	ressources/ressources.qrc
)

# Run uic on .ui files
QT4_WRAP_UI(UI_SRCS ${UI_FILES})

# Run moc
QT4_WRAP_CPP(MOC_SRCS ${INC_FILES})

# Run rcc
QT4_ADD_RESOURCES(QRC_SRCS ${QRC_FILES})

##################################################
## Source files
##################################################
add_sources(
	src/PLFrontendQt.cpp
	src/Frontend.cpp
	src/FrontendMainWindow.cpp
	src/FrontendRenderWindow.cpp
	src/ConstructFileFilter.cpp
	src/QPLContext.cpp
	src/Application.cpp
	src/QPLSceneContext.cpp
	src/QPLRenderWindow.cpp
	src/QPLInputHandler.cpp
	src/QPLDebugDialog.cpp
	src/DataModels/TreeItemBase.cpp
	src/DataModels/TreeModelBase.cpp
	src/DataModels/HeaderTreeItem.cpp
	src/DataModels/SceneNodeInfoModel.cpp
	src/DataModels/SceneGraphTreeModel.cpp
	src/DataModels/PLIntrospectionModel.cpp
	src/DataModels/PLTreeItemsDelegate.cpp
	src/DataModels/SceneGraphNodeTreeItemBase.cpp
	src/DataModels/PLTreeItems/PLDynVarFlagsTreeItem.cpp
	src/DataModels/PLTreeItems/PLDynVarNativNumericTypeTreeItem.cpp
	src/DataModels/PLTreeItems/PLDynVarColorTreeItem.cpp
	src/DataModels/PLTreeItems/PLDynVarStringTreeItem.cpp
	src/DataModels/PLTreeItems/PLDynVarVector2TreeItem.cpp
	src/DataModels/PLTreeItems/PLDynVarVector3TreeItem.cpp
	src/DataModels/PLTreeItems/PLDynVarVector4TreeItem.cpp
	src/DataModels/PLTreeItems/PLDynVarVector2iTreeItem.cpp
	src/DataModels/PLTreeItems/PLDynVarVector3iTreeItem.cpp
	src/DataModels/PLTreeItems/PLDynVarEnumTreeItem.cpp
	src/DataModels/PLTreeItems/PLDynVarTreeItemsFactory.cpp
	src/DataModels/ModelIndexMimeData.cpp
	src/DataModels/SceneRendererDataModel/SceneRendererDataModel.cpp
	src/DataModels/SceneRendererDataModel/SceneRendererPassTreeItem.cpp
	src/DataModels/RTTIInfoModels/ClassListModel.cpp
	src/DataModels/RTTIInfoModels/ClassInfoModel.cpp
	src/DataModels/TreeSortAndFilterProxyModel.cpp
	src/External/qtcolorbutton.cpp
	src/External/filterwidget.cpp
	src/Widget/ClassInfoWidget.cpp
	src/Widget/ClassListWidget.cpp
	src/Widget/RTTIBrowserWidget.cpp
	src/Widget/FilterWidgetWithFilterTypSelector.cpp
	src/Widget/SceneGraphMenu.cpp
	src/Widget/QTableViewWithCopy.cpp
	src/DockWidget/DockWidgetManager.cpp
	src/DockWidget/DockWidget.cpp
	src/DockWidget/DockWidgetLog.cpp
	src/DockWidget/DockWidgetRTTIBrowser.cpp
	src/DockWidget/DockWidgetObject.cpp
	src/DockWidget/DockWidgetScene.cpp
	src/DockWidget/DockWidgetSceneGraph.cpp
	src/DockWidget/DockWidgetSceneGraphQObject.cpp
	src/DockWidget/DockWidgetSceneRenderer.cpp
	src/DockWidget/DockWidgetSceneRendererQObject.cpp
	src/DockWidget/DockWidgetSceneMeshInformation.cpp
	${UI_SRCS}	# Add source produced by uic
	${MOC_SRCS}	# Add source produced by moc
	${QRC_SRCS}	# Add sources produced by rcc
)

##################################################
## Library directories
##################################################
add_link_directories(
	${PL_LIB_DIR}
)

##################################################
## Additional libraries
##################################################
add_libs(
	PLCore
	PLMath
	PLGraphics
	PLInput
	PLRenderer
	PLMesh
	PLScene
	PLEngine
	${QT_LIBRARIES}
)

##################################################
## Preprocessor definitions
##################################################
add_compile_defs(
	PLFRONTENDQT_EXPORTS
)
if(WIN32)
	##################################################
	## Win32
	##################################################
	add_compile_defs(
		${WIN32_COMPILE_DEFS}
	)
elseif(LINUX)
	##################################################
	## Linux
	##################################################
	add_compile_defs(
		${LINUX_COMPILE_DEFS}
	)
endif()

##################################################
## Compiler flags
##################################################
if(WIN32)
	##################################################
	## MSVC Compiler
	##################################################
	add_compile_flags(
		${WIN32_COMPILE_FLAGS}
	)
elseif(LINUX)
	##################################################
	## GCC Compiler
	##################################################
	add_compile_flags(
		${LINUX_COMPILE_FLAGS}
	)
endif()

##################################################
## Linker flags
##################################################
if(WIN32)
	##################################################
	## MSVC Compiler
	##################################################
	add_linker_flags(
		${WIN32_LINKER_FLAGS}
	)
elseif(LINUX)
	##################################################
	## GCC Compiler
	##################################################
	add_linker_flags(
		${LINUX_LINKER_FLAGS}
	)
endif()

##################################################
## Build
##################################################
build_library(${CMAKETOOLS_CURRENT_TARGET} SHARED)
pl_create_plugin_description(${CMAKE_CURRENT_SOURCE_DIR} ${PL_RUNTIME_BIN_DIR})

##################################################
## Dependencies
##################################################
add_dependencies(${CMAKETOOLS_CURRENT_TARGET}	External-Qt PLCore PLMath PLGraphics PLInput PLRenderer PLMesh PLScene PLEngine)
add_dependencies(Plugins                       	${CMAKETOOLS_CURRENT_TARGET})

##################################################
## Post-Build
##################################################

# Dynamic library
add_custom_command(TARGET ${CMAKETOOLS_CURRENT_TARGET}
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKETOOLS_CURRENT_OUTPUT_DIR}/${CMAKETOOLS_CURRENT_SHARED_LIB} ${PL_RUNTIME_BIN_DIR}
)
if(WIN32)
	# Copy the Qt dlls so that the user can use everything "out of the box"
	add_custom_command(TARGET ${CMAKETOOLS_CURRENT_TARGET}
		COMMAND ${CMAKE_COMMAND} -E copy ${QT_EXTERNAL_DIR}/bin/QtCore4.dll ${PL_RUNTIME_BIN_DIR}
		COMMAND ${CMAKE_COMMAND} -E copy ${QT_EXTERNAL_DIR}/bin/QtGui4.dll ${PL_RUNTIME_BIN_DIR}
	)
endif()

# Create relative symbolic links?
if(LINUX AND CMAKETOOLS_DETAILED_VERSION_INFORMATION)
	add_custom_command(TARGET ${CMAKETOOLS_CURRENT_TARGET}
		COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKETOOLS_CURRENT_SHARED_LIB} ${CMAKETOOLS_CURRENT_SHARED_LIB_VERSION} WORKING_DIRECTORY ${PL_RUNTIME_BIN_DIR}
		COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKETOOLS_CURRENT_SHARED_LIB} ${CMAKETOOLS_CURRENT_SHARED_LIB_SO_VERSION} WORKING_DIRECTORY ${PL_RUNTIME_BIN_DIR}
	)
endif()

# Import library
if(WIN32)
	add_custom_command(TARGET ${CMAKETOOLS_CURRENT_TARGET}
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKETOOLS_CURRENT_OUTPUT_DIR}/${CMAKETOOLS_CURRENT_STATIC_LIB} ${PL_LIB_DIR}
	)
endif()

# External dependencies, but only when not using system-libs
if(NOT CMAKETOOLS_USE_SYSTEM_LIBS)
	if(WIN32)
		add_custom_command(TARGET ${CMAKETOOLS_CURRENT_TARGET}
			COMMAND ${CMAKE_COMMAND} -E copy ${PL_RUNTIME_BIN_GITDIR}/QtCore4.dll ${PL_RUNTIME_BIN_DIR}
			COMMAND ${CMAKE_COMMAND} -E copy ${PL_RUNTIME_BIN_GITDIR}/QtGui4.dll ${PL_RUNTIME_BIN_DIR}
		)
	endif()
	if(LINUX)
		# Set COPY_SOURCE to files which should be copied (this variable is used by the "copy.cmake"-script)
		set(COPY_SOURCE ${QT_EXTERNAL_DIR}/lib/libQtCore.so
						${QT_EXTERNAL_DIR}/lib/libQtCore.so.4
						${QT_EXTERNAL_DIR}/lib/libQtCore.so.4.8
						${QT_EXTERNAL_DIR}/lib/libQtCore.so.4.8.2
						${QT_EXTERNAL_DIR}/lib/libQtGui.so
						${QT_EXTERNAL_DIR}/lib/libQtGui.so.4
						${QT_EXTERNAL_DIR}/lib/libQtGui.so.4.8
						${QT_EXTERNAL_DIR}/lib/libQtGui.so.4.8.2)

		# Set COPY_DEST to a directory to which the files in COPY_SOURCE should be copied into (this variable is used by the "copy.cmake"-script)
		set(COPY_DEST ${PL_RUNTIME_BIN_DIR})

		# Create the "copy_to_runtime.cmake"-file in the build directory of the current target (the configure file replaces the variable references with the value of the variable)
		configure_file("${CMAKETOOLS_DIR}/Scripts/copy.cmake" ${CMAKETOOLS_CURRENT_OUTPUT_DIR}/copy_to_runtime.cmake)

		# Add custom command to call the created "copy_to_runtime.cmake"-file after the target was built
		add_custom_command(TARGET ${CMAKETOOLS_CURRENT_TARGET} COMMAND ${CMAKE_COMMAND} -P "copy_to_runtime.cmake")
	endif()
endif()

# Copy the Qt libraries so that the user can use everything "out of the box"
if(WIN32)
	add_custom_command(TARGET ${CMAKETOOLS_CURRENT_TARGET}
		COMMAND ${CMAKE_COMMAND} -E copy ${QT_EXTERNAL_DIR}/lib/QtCore4.lib ${PL_LIB_DIR}
		COMMAND ${CMAKE_COMMAND} -E copy ${QT_EXTERNAL_DIR}/lib/QtGui4.lib ${PL_LIB_DIR}
		COMMAND ${CMAKE_COMMAND} -E copy ${QT_EXTERNAL_DIR}/lib/qtmain.lib ${PL_LIB_DIR}
#		COMMAND ${CMAKE_COMMAND} -E copy ${QT_EXTERNAL_DIR}/lib/qtmaind.lib ${PL_LIB_DIR}
	)
endif()
if(LINUX)
	# Set COPY_DEST to a directory to which the files in COPY_SOURCE should be copied into (this variable is used by the "copy.cmake"-script)
	set(COPY_DEST ${PL_LIB_DIR})

	# Create the "copy_to_lib.cmake"-file in the build directory of the current target (the configure file replaces the variable references with the value of the variable)
	configure_file("${CMAKETOOLS_DIR}/Scripts/copy.cmake" ${CMAKETOOLS_CURRENT_OUTPUT_DIR}/copy_to_lib.cmake)

	# Add custom command to call the created "copy_to_runtime.cmake"-file after target was built
	add_custom_command(TARGET ${CMAKETOOLS_CURRENT_TARGET} COMMAND ${CMAKE_COMMAND} -P "copy_to_lib.cmake")
endif()

##################################################
## Install
##################################################

# Library
install(TARGETS ${CMAKETOOLS_CURRENT_TARGET}
	RUNTIME DESTINATION ${PL_INSTALL_RUNTIME_BIN}	COMPONENT Runtime
	LIBRARY DESTINATION ${PL_INSTALL_RUNTIME_BIN}	COMPONENT Runtime
	ARCHIVE DESTINATION ${PL_INSTALL_LIB}			COMPONENT SDK
)

# Include files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${CMAKETOOLS_CURRENT_PROJECT}
	DESTINATION ${PL_INSTALL_INCLUDE} 				COMPONENT SDK		REGEX ${PL_EXCLUDE_FILE_EXTENSIONS} EXCLUDE
)

# Plugin file
install(FILES ${PL_RUNTIME_BIN_DIR}/${CMAKETOOLS_CURRENT_PLUGIN}
	DESTINATION ${PL_INSTALL_RUNTIME_BIN}			COMPONENT Runtime
)

# Copy the Qt stuff into the SDK so that the user can use everything "out of the box", but only when not using system-libs
if(NOT CMAKETOOLS_USE_SYSTEM_LIBS)
	install(DIRECTORY "${QT_EXTERNAL_DIR}/include/Qt"
		DESTINATION ${PL_INSTALL_INCLUDE}		COMPONENT SDK		REGEX ${PL_EXCLUDE_FILE_EXTENSIONS} EXCLUDE
	)

	# Copy the copyright information into the include directory
	install(FILES	"${QT_EXTERNAL_DIR}/Readme.txt"
					"${QT_EXTERNAL_DIR}/LICENSE.LGPL"
		DESTINATION "${PL_INSTALL_INCLUDE}/Qt" 		COMPONENT SDK
	)
endif(NOT CMAKETOOLS_USE_SYSTEM_LIBS)

if(WIN32)
	# Copy shared libraries
	install(FILES	${PL_RUNTIME_BIN_DIR}/QtCore4.dll
					${PL_RUNTIME_BIN_DIR}/QtGui4.dll
		DESTINATION ${PL_INSTALL_RUNTIME_BIN}		COMPONENT SDK
	)

	# Copy libraries
	install(FILES	${PL_LIB_DIR}/QtCore4.lib
					${PL_LIB_DIR}/QtGui4.lib
					${PL_LIB_DIR}/qtmain.lib
					${PL_LIB_DIR}/qtmaind.lib
		DESTINATION ${PL_INSTALL_LIB}				COMPONENT SDK
	)

	# Copy tools
	install(FILES	"${QT_EXTERNAL_DIR}/bin/moc.exe"
					"${QT_EXTERNAL_DIR}/bin/uic.exe"
					"${QT_EXTERNAL_DIR}/Readme.txt"
					"${QT_EXTERNAL_DIR}/LICENSE.LGPL"
		DESTINATION ${PL_INSTALL_TOOLS_BIN}/../Qt	COMPONENT SDK	# Those x86 Qt tools can also be used for x64 as well
	)
endif()
if(LINUX AND NOT CMAKETOOLS_USE_SYSTEM_LIBS)
	# Copy shared libraries
	install(FILES	${QT_EXTERNAL_DIR}/lib/libQtCore.so.4
					${QT_EXTERNAL_DIR}/lib/libQtGui.so.4
					${QT_EXTERNAL_DIR}/lib/libQtCore.so.4.8.2
					${QT_EXTERNAL_DIR}/lib/libQtGui.so.4.8.2
		DESTINATION ${PL_INSTALL_RUNTIME_BIN}		COMPONENT SDK
	)

	# Copy tools
	install(FILES	"${QT_EXTERNAL_DIR}/bin/moc"
					"${QT_EXTERNAL_DIR}/bin/uic"
					"${QT_EXTERNAL_DIR}/Readme.txt"
					"${QT_EXTERNAL_DIR}/LICENSE.LGPL"
		DESTINATION ${PL_INSTALL_TOOLS_BIN}/../Qt	COMPONENT SDK	# Those x86 Qt tools can also be used for x64 as well
	)
endif()
