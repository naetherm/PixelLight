/*********************************************************\
 *  File: ProgramAttributeCgGLSL.h                       *
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


#ifndef __PLRENDEREROPENGLCG_PROGRAMATTRIBUTECGGLSL_H__
#define __PLRENDEREROPENGLCG_PROGRAMATTRIBUTECGGLSL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRendererOpenGLCg/ProgramAttributeCg.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRendererOpenGLCg {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    OpenGL Cg GLSL renderer program attribute
*
*  @remarks
*    When using Cg on a modern AMD/ATI graphics card, we only have a choice between the profiles
*    "arbvp1", "arbfp1", "glslv", "glslf", "glslg" - meaning we don't have a choice and have to stick
*    to GLSL because "arbvp1" & "arbfp1" are heavily out-dated.
*    Sadly, there are several issues when using Cg with GLSL profiles. When using no GLSL version hint,
*    old school GLSL is used which is related to fixed function stuff. This is causing heavy issues when
*    using VBO because the Cg compiler is now throwing e.g. glMultiTexCoord0 into the resulting GLSL code
*    as source of vertex data, but not using VBO is not an option. So, we need to provide the Cg compiler with a
*    GLSL version like 150... then the next thing happens: Cg just tells us "The parameter used is invalid." when
*    using "cgGLSetParameterPointer()"/"cgGLEnableClientState()". So, we have to figure out the OpenGL attribute
*    location and then falling back to "glVertexAttribPointerARB()"/"glEnableVertexAttribArrayARB()" instead
*    of using buggy Cg OpenGL functions. Long story, short conclusion: Using the combination
*    Cg & OpenGL & GLSL & AMD-hardware is painful, but sadly NVIDIA appears to be not interested to changing anything
*    on this situation... this issue is not new and nearly as old as Cg itself and I'am already fighting with this topic
*    for years. Still, Cg is worth all this efforts... a shader cross-compiler is a fantastic thing...
*/
class ProgramAttributeCgGLSL : public ProgramAttributeCg {


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
		*/
		ProgramAttributeCgGLSL(CGparameter pCgParameter);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ProgramAttributeCgGLSL();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		int m_nOpenGLAttributeLocation;	/**< OpenGL attribute location, <0 if invalid */


	//[-------------------------------------------------------]
	//[ Public virtual PLRenderer::ProgramAttribute functions ]
	//[-------------------------------------------------------]
	public:
		virtual bool Set(PLRenderer::VertexBuffer *pVertexBuffer, PLCore::uint32 nIndex) override;
		virtual bool Set(PLRenderer::VertexBuffer *pVertexBuffer, PLRenderer::VertexBuffer::ESemantic nSemantic, PLCore::uint32 nChannel = 0) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGLCg


#endif // __PLRENDEREROPENGLCG_PROGRAMATTRIBUTECGGLSL_H__
