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
pl_project(PLRendererOpenGL)


##################################################
## Source files
##################################################

# [TODO] optional +Font support
pl_collect_source_files(Font)



##################################################
## Dependencies
##################################################
pl_add_dependency(PLCore)
pl_add_dependency(PLMath)
pl_add_dependency(PLGraphics)
pl_add_dependency(PLRenderer)
pl_add_dependency(freetype) # [TODO] optional


##################################################
## Additional libraries
##################################################
if(WIN32)
	pl_add_link_libs(
		${WIN32_USER32_LIB}
		${WIN32_GDI32_LIB}
		${WIN32_OPENGL32_LIB}
		${WIN32_GLU32_LIB}
	)
elseif(LINUX)
	pl_add_link_libs(
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
pl_add_compile_defs(PLRENDEREROPENGL_EXPORTS)

# [TODO] +Font optional
#if(NOT PL_PLUGIN_RENDERER_OPENGL_FONT)
#	pl_add_compile_defs(DISABLE_FONT_SUPPORT)
#endif()

if(LINUX)
	pl_add_compile_defs(GLX_GLXEXT_PROTOTYPES)
endif()

# [TODO]
#if(APPLE)
#	add_linker_flags(
#		-framework OpenGL
#		-framework Cocoa
#	)
#endif()


##################################################
## Finalize
##################################################
pl_build_library(PLUGIN)