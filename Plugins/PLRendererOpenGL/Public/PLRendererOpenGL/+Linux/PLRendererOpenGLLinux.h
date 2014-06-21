/*********************************************************\
 *  File: PLRendererOpenGLLinux.h                        *
 *      Linux definitions for PLRendererOpenGL
 *
 *  Copyright (C) 2002-2013 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 *  and associated documentation files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all copies or
 *  substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 *  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


#ifndef __PLRENDEREROPENGL_LINUX_H__
#define __PLRENDEREROPENGL_LINUX_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/PLCoreLinuxIncludes.h>
#include "../../../../../External/Recommended/OpenGL/gl.h"
#include "../../../../../External/Recommended/OpenGL/glu.h"
#include "../../../../../External/Recommended/OpenGL/glext.h"


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
// We need to undefine some OS defines to avoid name conflicts... :(
#undef None
#undef Always
#undef Success

// Missing OpenGL definitions
#ifndef GL_ATI_texture_compression_3dc
	#define GL_ATI_texture_compression_3dc			1
	#define GL_COMPRESSED_LUMINANCE_ALPHA_3DC_ATI	0x8837
#endif


#endif // __PLRENDEREROPENGL_LINUX_H__
