/*********************************************************\
 *  File: ProgramUniformBlockGLSL.h                      *
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


#ifndef __PLRENDEREROPENGL_PROGRAMUNIFORMBLOCKGLSL_H__
#define __PLRENDEREROPENGL_PROGRAMUNIFORMBLOCKGLSL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/String/String.h>
#include <PLRenderer/Renderer/ProgramUniformBlock.h>
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
*    OpenGL GLSL renderer program uniform block
*/
class ProgramUniformBlockGLSL : public PLRenderer::ProgramUniformBlock {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class ProgramGLSL;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nOpenGLProgram
		*    OpenGL program, must be valid!
		*  @param[in] nUniformBlockIndex
		*    Uniform block index, must be valid!
		*  @param[in] nUniformBlockSize
		*    Uniform block size, must be valid!
		*  @param[in] sShaderLanguage
		*    The name of the shader language the uniform block is using (for example "GLSL" or "Cg")
		*/
		ProgramUniformBlockGLSL(GLuint nOpenGLProgram, GLuint nUniformBlockIndex, GLint nUniformBlockSize, const PLCore::String &sShaderLanguage);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ProgramUniformBlockGLSL();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		PLCore::String m_sShaderLanguage;			/**< The name of the shader language the uniform block is using (for example "GLSL" or "Cg") */
		GLuint		   m_nOpenGLProgram;			/**< OpenGL program, always valid! */
		GLint		   m_nUniformBlockIndex;		/**< OpenGL uniform block index */
		GLint		   m_nOpenGLUniformBlockSize;	/**< OpenGL uniform block size */
		int			   m_nBindingPoint;				/**< Used binding point, <0 means not yet assigned */


	//[-------------------------------------------------------]
	//[ Public virtual PLRenderer::ProgramUniformBlock functions ]
	//[-------------------------------------------------------]
	public:
		virtual bool SetUniformBuffer(PLRenderer::UniformBuffer *pUniformBuffer, PLCore::uint32 nBindingPoint) override;
		virtual PLCore::uint32 GetIndex() const override;
		virtual PLCore::uint32 GetSize() const override;
		virtual PLCore::uint32 GetUniformOffset(const PLCore::String &sUniformName) const override;
		virtual PLCore::uint32 GetUniformSize(const PLCore::String &sUniformName) const override;
		virtual PLCore::uint32 GetUniformNumOfArrayElements(const PLCore::String &sUniformName) const override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL


#endif // __PLRENDEREROPENGL_PROGRAMUNIFORMBLOCKGLSL_H__
