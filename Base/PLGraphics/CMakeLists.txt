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
define_project(PLGraphics)

##################################################
## Source files
##################################################
add_sources(
	src/PLGraphics.cpp
	src/Color/Color3.cpp
	src/Color/Color4.cpp
	src/Image/Image.cpp
	src/Image/ImageBuffer.cpp
	src/Image/ImageData.cpp
	src/Image/ImageEffect.cpp
	src/Image/ImageEffectWrapper.cpp
	src/Image/ImageLoader.cpp
	src/Image/ImagePalette.cpp
	src/Image/ImagePart.cpp
	src/Image/Effects/IEColorKey.cpp
	src/Image/Effects/IEConvert.cpp
	src/Image/Effects/IEFilter.cpp
	src/Image/Effects/IEScale.cpp
	src/Image/Effects/IEMonochrome.cpp
	src/Image/Effects/IEFlipXAxis.cpp
	src/Image/Effects/IEFlipYAxis.cpp
	src/Image/Effects/IERemovePalette.cpp
	src/Image/Loader/ImageLoaderBMP.cpp
	src/Image/Loader/ImageLoaderTGA.cpp
	src/Image/Loader/ImageLoaderPPM.cpp
	src/Image/Loader/ImageLoaderDDS.cpp
)
if(PL_PLUGIN_GRAPHICS_PNG)
	add_sources(
		src/Image/Loader/ImageLoaderPNG.cpp
	)
endif()
if(PL_PLUGIN_GRAPHICS_JPG)
	add_sources(
		src/Image/Loader/ImageLoaderJPG.cpp
	)
endif()
if(WIN32)
	##################################################
	## Win32
	##################################################
	add_sources(
		src/Image/ImageToolsWindows.cpp
	)
endif()

##################################################
## Include directories
##################################################
add_include_directories(
	include
	${CMAKE_SOURCE_DIR}/Base/PLCore/include
	${CMAKE_SOURCE_DIR}/Base/PLMath/include
)
if(PL_PLUGIN_GRAPHICS_PNG)
	add_include_directories(
		${PNG_INCLUDE_DIR}
		${ZLIB_INCLUDE_DIR}
	)
endif()
if(PL_PLUGIN_GRAPHICS_JPG)
	add_include_directories(
		${JPEG_INCLUDE_DIR}
	)
endif()

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
)
if(PL_PLUGIN_GRAPHICS_PNG)
	add_libs(
		${PNG_LIBRARIES}
		${ZLIB_LIBRARIES}
	)
endif()
if(PL_PLUGIN_GRAPHICS_JPG)
	add_libs(
		${JPEG_LIBRARIES}
	)
endif()
if(WIN32)
	##################################################
	## Win32
	##################################################
	add_libs(
		user32.lib
		gdi32.lib
		comdlg32.lib
	)
endif()

##################################################
## Preprocessor definitions
##################################################
add_compile_defs(
	PLGRAPHICS_EXPORTS
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
add_dependencies(${CMAKETOOLS_CURRENT_TARGET}		PLCore PLMath)
if(PL_PLUGIN_GRAPHICS_PNG)
	add_dependencies(${CMAKETOOLS_CURRENT_TARGET}	External-libpng External-zlib)
endif()
if(PL_PLUGIN_GRAPHICS_JPG)
	add_dependencies(${CMAKETOOLS_CURRENT_TARGET}	External-libjpg)
endif()
add_dependencies(Base								${CMAKETOOLS_CURRENT_TARGET})

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
