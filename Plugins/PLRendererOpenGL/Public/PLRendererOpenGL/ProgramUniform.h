/*********************************************************\
 *  File: ProgramUniform.h                               *
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


#ifndef __PLRENDEREROPENGL_PROGRAMUNIFORM_H__
#define __PLRENDEREROPENGL_PROGRAMUNIFORM_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/ProgramUniform.h>
#include "PLRendererOpenGL/PLRendererOpenGL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract OpenGL renderer program uniform
*/
class ProgramUniform : public PLRenderer::ProgramUniform {


	//[-------------------------------------------------------]
	//[ Public virtual ProgramUniform functions               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    If this uniform is a texture, this method sets the OpenGL texture assigned to the uniform
		*
		*  @param[in] nOpenGLTextureTarget
		*    OpenGL texture target
		*  @param[in] nOpenGLTexture
		*    OpenGL texture to set
		*
		*  @return
		*    The texture unit assigned to this uniform (same as GetTextureUnit()), negative on error
		*/
		virtual int Set(GLenum nOpenGLTextureTarget, GLuint nOpenGLTexture) = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		PLRENDEREROPENGL_API ProgramUniform();

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDEREROPENGL_API virtual ~ProgramUniform();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL


#endif // __PLRENDEREROPENGL_PROGRAMUNIFORM_H__
