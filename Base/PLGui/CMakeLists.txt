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
define_project(PLGui)

##################################################
## Source files
##################################################
add_sources(
	src/Gui/Base/GuiMessage.cpp
	src/Gui/Base/MessageFilter.cpp
	src/Gui/Base/MessageFilterRule.cpp
	src/Gui/Base/MessageFilterDebug.cpp
	src/Gui/Base/WidgetFunctions.cpp
	src/Gui/Data/ButtonGroup.cpp
	src/Gui/Data/DataObject.cpp
	src/Gui/Data/LayoutHints.cpp
	src/Gui/Data/GraphicsHint.cpp
	src/Gui/Data/SizeHint.cpp
	src/Gui/Resources/Cursor.cpp
	src/Gui/Resources/Font.cpp
	src/Gui/Resources/FontInfo.cpp
	src/Gui/Resources/Graphics.cpp
	src/Gui/Resources/Image.cpp
	src/Gui/Resources/Menu.cpp
	src/Gui/Resources/MenuItem.cpp
	src/Gui/Resources/MenuSeparator.cpp
	src/Gui/Resources/Timer.cpp
	src/Gui/Resources/TimerThread.cpp
	src/Gui/Gui.cpp
	src/Gui/Screen.cpp
	src/Gui/TrayIcon.cpp
	src/Gui/WidgetHandler.cpp
	src/Gui/ClipBoard.cpp
	src/Gui/FontManager.cpp
	src/Gui/CursorManager.cpp
	src/Backend/GraphicsImpl.cpp
	src/Backend/GuiImpl.cpp
	src/Backend/WidgetImpl.cpp
	src/Backend/ImageImpl.cpp
	src/Backend/FontImpl.cpp
	src/Backend/CursorImpl.cpp
	src/Backend/TrayIconImpl.cpp
	src/Backend/ClipBoardImpl.cpp
	src/Widgets/Layout/Box.cpp
	src/Widgets/Base/AbstractButton.cpp
	src/Widgets/Base/AbstractMenu.cpp
	src/Widgets/Base/AbstractSlider.cpp
	src/Widgets/Base/AbstractToggleButton.cpp
	src/Widgets/Base/AbstractTooltip.cpp
	src/Widgets/Containers/ContainerWidget.cpp
	src/Widgets/Containers/ScrollWidget.cpp
#	src/Widgets/Containers/WindowContainer.cpp
#	src/Widgets/Containers/TabContainer.cpp
	src/Widgets/Containers/Border.cpp
	src/Widgets/Containers/Panel.cpp
	src/Widgets/Containers/Splitter.cpp
	src/Widgets/Controls/Label.cpp
	src/Widgets/Controls/Bitmap.cpp
	src/Widgets/Controls/Button.cpp
	src/Widgets/Controls/BitmapButton.cpp
	src/Widgets/Controls/ToggleButton.cpp
	src/Widgets/Controls/BitmapToggleButton.cpp
	src/Widgets/Controls/SystemButton.cpp
	src/Widgets/Controls/CheckBox.cpp
	src/Widgets/Controls/RadioButton.cpp
#	src/Widgets/Controls/TabBar.cpp
#	src/Widgets/Controls/TabBarEntry.cpp
#	src/Widgets/Controls/TabBarButton.cpp
	src/Widgets/Controls/MenuBar.cpp
	src/Widgets/Controls/Slider.cpp
	src/Widgets/Controls/ScrollBar.cpp
	src/Widgets/Windows/Window.cpp
#	src/Widgets/Windows/AppWindow.cpp
#	src/Widgets/Windows/Dialog.cpp
#	src/Widgets/Windows/MessageBox.cpp
	src/Widgets/Popups/PopupMenu.cpp
	src/Widgets/Popups/Tooltip.cpp
	src/Widgets/Slim/SlimEntry.cpp
	src/Widgets/Widget.cpp
	src/Modifiers/Modifier.cpp
	src/Modifiers/ModClose.cpp
	src/Modifiers/ModFullscreen.cpp
	src/Modifiers/ModBlend.cpp
	src/Modifiers/ModTimeout.cpp
	src/Modifiers/ModTooltip.cpp
	src/Modifiers/ModSnap.cpp
	src/Layout/Layout.cpp
	src/Layout/LayoutBox.cpp
	src/Themes/Theme.cpp
	src/Themes/ThemeDesktop.cpp
	src/Themes/ThemeIngame.cpp
	src/Application/GuiApplication.cpp
	src/PLGui.cpp
)
if(WIN32)
	##################################################
	## Win32
	##################################################
	add_sources(
		src/Backend/Windows/GraphicsWindows.cpp
		src/Backend/Windows/GuiWindows.cpp
		src/Backend/Windows/ImageWindows.cpp
		src/Backend/Windows/FontWindows.cpp
		src/Backend/Windows/ToolsWindows.cpp
		src/Backend/Windows/WidgetWindows.cpp
		src/Backend/Windows/CursorWindows.cpp
		src/Backend/Windows/TrayIconWindows.cpp
		src/Backend/Windows/ClipBoardWindows.cpp
	)
elseif(ANDROID)
	##################################################
	## Android
	##################################################
	add_sources(
		src/Backend/Null/GraphicsNull.cpp
		src/Backend/Null/GuiNull.cpp
		src/Backend/Null/ImageNull.cpp
		src/Backend/Null/FontNull.cpp
		src/Backend/Null/WidgetNull.cpp
		src/Backend/Null/CursorNull.cpp
		src/Backend/Null/TrayIconNull.cpp
		src/Backend/Null/ClipBoardNull.cpp
		src/Backend/Android/GuiAndroid.cpp
	)
elseif(LINUX)
	##################################################
	## Linux
	##################################################
	add_sources(
		src/Backend/Linux/GraphicsLinux.cpp
		src/Backend/Linux/GuiLinux.cpp
		src/Backend/Linux/ImageLinux.cpp
		src/Backend/Linux/FontLinux.cpp
		src/Backend/Linux/ToolsLinux.cpp
		src/Backend/Linux/WidgetLinux.cpp
		src/Backend/Linux/CursorLinux.cpp
		src/Backend/Linux/TrayIconLinux.cpp
		src/Backend/Linux/TrayIconWidgetLinux.cpp
		src/Backend/Linux/ClipBoardLinux.cpp
	)
endif()

##################################################
## Include directories
##################################################
add_include_directories(
	include
	${CMAKE_SOURCE_DIR}/Base/PLCore/include
	${CMAKE_SOURCE_DIR}/Base/PLMath/include
	${CMAKE_SOURCE_DIR}/Base/PLGraphics/include
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
)
if(WIN32)
	##################################################
	## Win32
	##################################################
	add_libs(
		${WIN32_STANDARD_LIBS}
		${WIN32_GDI_LIBS}
	)
elseif(ANDROID)
	##################################################
	## Android
	##################################################
	add_libs(
		android
	)
elseif(LINUX)
	##################################################
	## Linux
	##################################################
	add_libs(
		${LINUX_X11_LIBS}
	)
endif()

##################################################
## Preprocessor definitions
##################################################
add_compile_defs(
	PLGUI_EXPORTS
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
add_dependencies(${CMAKETOOLS_CURRENT_TARGET}	PLMath PLGraphics)
add_dependencies(Base	                       	${CMAKETOOLS_CURRENT_TARGET})

##################################################
## Post-Build
##################################################

# Dynamic library
add_custom_command(TARGET ${CMAKETOOLS_CURRENT_TARGET}
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKETOOLS_CURRENT_OUTPUT_DIR}/${CMAKETOOLS_CURRENT_SHARED_LIB} ${PL_RUNTIME_BIN_DIR}
)

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
