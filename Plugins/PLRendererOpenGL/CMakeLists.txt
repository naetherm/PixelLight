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
define_project(PLRendererOpenGL)

##################################################
## Source files
##################################################
add_sources(
	src/PLRendererOpenGL.cpp
	src/Renderer.cpp
	src/FixedFunctions.cpp
	src/Context.cpp
	src/VertexBuffer.cpp
	src/IndexBuffer.cpp
	src/UniformBuffer.cpp
	src/TextureBuffer1D.cpp
	src/TextureBuffer2D.cpp
	src/TextureBuffer2DArray.cpp
	src/TextureBufferRectangle.cpp
	src/TextureBuffer3D.cpp
	src/TextureBufferCube.cpp
	src/SurfaceWindow.cpp
	src/SurfaceTextureBuffer.cpp
	src/OcclusionQuery.cpp
	src/ShaderLanguage.cpp
	src/ShaderLanguageGLSL.cpp
	src/Program.cpp
	src/ProgramUniform.cpp
	src/VertexShaderGLSL.cpp
	src/TessellationControlShaderGLSL.cpp
	src/TessellationEvaluationShaderGLSL.cpp
	src/GeometryShaderGLSL.cpp
	src/FragmentShaderGLSL.cpp
	src/ProgramGLSL.cpp
	src/ProgramAttributeGLSL.cpp
	src/ProgramUniformGLSL.cpp
	src/ProgramUniformBlockGLSL.cpp
	src/ShaderToolsGLSL.cpp
	src/Extensions.cpp
	src/FrameBufferObject.cpp
)
if(PL_PLUGIN_RENDERER_OPENGL_FONT)
	add_sources(
		src/FontManager.cpp
		src/FontTexture.cpp
		src/FontTextureShaders.cpp
		src/FontTextureFixedFunctions.cpp
		src/FontGlyphTexture.cpp
	)
endif()
if(WIN32)
	##################################################
	## Win32
	##################################################
	add_sources(
		src/Windows/ContextWindows.cpp
		src/Windows/ExtensionsWindows.cpp
		src/Windows/SurfaceWindowWindows.cpp
	)
elseif(APPLE)
	##################################################
	## Apple
	##################################################
	add_sources(
		src/MacOSX/ContextMacOSX.cpp
		src/MacOSX/ExtensionsMacOSX.cpp
		src/MacOSX/SurfaceWindowMacOSX.cpp

		# We can also reuse the PLFrontendOS X11 implementation from the Linux port
		# src/MacOSX/ContextMacOSX_X11.cpp
		# src/MacOSX/SurfaceWindowMacOSX_X11.cpp

		# This is what we really want to use on Mac OS X to stay out of compatibility trouble - Cocoa & NSOpenGL can only be accessed when using Objective-C :/
		src/MacOSX/ContextMacOSX_Cocoa.mm
		src/MacOSX/SurfaceWindowMacOSX_Cocoa.mm
	)
elseif(LINUX)
	##################################################
	## Linux
	##################################################
	add_sources(
		src/Linux/ContextLinux.cpp
		src/Linux/ExtensionsLinux.cpp
		src/Linux/SurfaceWindowLinux.cpp
	)
endif()

##################################################
## Include directories
##################################################
add_include_directories(
	include
	${CMAKE_SOURCE_DIR}/Base/PLCore/include
	${CMAKE_SOURCE_DIR}/Base/PLGraphics/include
	${CMAKE_SOURCE_DIR}/Base/PLMath/include
	${CMAKE_SOURCE_DIR}/Base/PLRenderer/include
)
if(PL_PLUGIN_RENDERER_OPENGL_FONT)
	add_include_directories(
		${FREETYPE_INCLUDE_DIRS}
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
	PLGraphics
	PLMath
	PLRenderer
)
if(PL_PLUGIN_RENDERER_OPENGL_FONT)
	add_libs(
		${FREETYPE_LIBRARIES}
	)
endif()
if(WIN32)
	##################################################
	## Win32
	##################################################
	add_libs(
		${WIN32_USER32_LIB}
		${WIN32_GDI32_LIB}
		${WIN32_OPENGL32_LIB}
		${WIN32_GLU32_LIB}
	)
elseif(LINUX)
	##################################################
	## Linux
	##################################################
	add_libs(
		${LINUX_X11_LIBS}
		GL
		GLU
		Xxf86vm
		Xrandr
	)
endif()

##################################################
## Preprocessor definitions
##################################################
add_compile_defs(
	PLRENDEREROPENGL_EXPORTS
)
if(NOT PL_PLUGIN_RENDERER_OPENGL_FONT)
	add_compile_defs(
		DISABLE_FONT_SUPPORT
	)
endif()
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
		GLX_GLXEXT_PROTOTYPES
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
elseif(APPLE)
	##################################################
	## GCC Compiler
	##################################################
	add_linker_flags(
		${LINUX_LINKER_FLAGS}
		-framework OpenGL
		-framework Cocoa
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
add_dependencies(${CMAKETOOLS_CURRENT_TARGET}		External-OpenGL PLCore PLMath PLGraphics PLRenderer)
if(PL_PLUGIN_RENDERER_OPENGL_FONT)
	add_dependencies(${CMAKETOOLS_CURRENT_TARGET}	External-freetype)
endif()
add_dependencies(Plugins							${CMAKETOOLS_CURRENT_TARGET})

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
