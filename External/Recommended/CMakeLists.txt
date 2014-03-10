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
project(External-Recommended)

##################################################
## Target
##################################################
add_custom_target(External-Recommended)

##################################################
## Projects
##################################################
# PLGraphics image formats
if(PL_PLUGIN_GRAPHICS_PNG)
	add_subdirectory(png)
endif()
if(PL_PLUGIN_GRAPHICS_JPG)
	add_subdirectory(jpg)
endif()
# Script backends
if(PL_PLUGIN_SCRIPT_LUA)
	add_subdirectory(Lua)
endif()
# PLRenderer backends
if(PL_PLUGIN_RENDERER_OPENGL)
	add_subdirectory(OpenGL)
endif()
if((PL_PLUGIN_RENDERER_OPENGL AND PL_PLUGIN_RENDERER_OPENGL_FONT) OR (PL_PLUGIN_RENDERER_OPENGLES2 AND PL_PLUGIN_RENDERER_OPENGLES2_FONT))
	add_subdirectory(freetype)
endif()
# PLPhysics backends
if(PL_PLUGIN_PHYSICS_NEWTON)
	add_subdirectory(Newton)
endif()
# PLSound backends
if(PL_PLUGIN_SOUND_OPENAL)
	add_subdirectory(OpenAL)
	add_subdirectory(libogg)
	add_subdirectory(libvorbis)
endif()

##################################################
## Dependencies
##################################################
add_dependencies(External External-Recommended)
