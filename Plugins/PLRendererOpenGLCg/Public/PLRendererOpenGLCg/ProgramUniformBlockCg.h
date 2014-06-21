/*********************************************************\
 *  File: ProgramUniformBlockCg.h                        *
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


#ifndef __PLRENDEREROPENGLCG_PROGRAMUNIFORMBLOCKCG_H__
#define __PLRENDEREROPENGLCG_PROGRAMUNIFORMBLOCKCG_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <Cg/cgGL.h>
#include <PLCore/String/String.h>
#include <PLRenderer/Renderer/ProgramUniformBlock.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRendererOpenGLCg {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    OpenGL Cg renderer program uniform block
*/
class ProgramUniformBlockCg : public PLRenderer::ProgramUniformBlock {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class ProgramCg;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pCgParameter
		*    Cg program parameter, must be valid!
		*  @param[in] sShaderLanguage
		*    The name of the shader language the uniform block is using (for example "GLSL" or "Cg")
		*/
		ProgramUniformBlockCg(CGparameter pCgParameter, const PLCore::String &sShaderLanguage);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ProgramUniformBlockCg();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		PLCore::String m_sShaderLanguage;	/**< The name of the shader language the uniform block is using (for example "GLSL" or "Cg") */
		CGparameter	   m_pCgParameter;		/**< Cg program parameter, always valid! */


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
} // PLRendererOpenGLCg


#endif // __PLRENDEREROPENGLCG_PROGRAMUNIFORMBLOCKCG_H__
