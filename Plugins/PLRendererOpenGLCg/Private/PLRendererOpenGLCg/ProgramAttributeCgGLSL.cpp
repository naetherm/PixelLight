/*********************************************************\
 *  File: ProgramAttributeCgGLSL.cpp                     *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRendererOpenGL/VertexBuffer.h>
#include "PLRendererOpenGLCg/ProgramAttributeCgGLSL.h"
#include "../../../../External/OpenGL/gl.h"
#ifdef LINUX
	#include "../../../../External/OpenGL/glx.h"	// For "glXGetProcAddressARB"
#endif
#include "../../../../External/OpenGL/glext.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRendererOpenGLCg {


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ProgramAttributeCgGLSL::ProgramAttributeCgGLSL(CGparameter pCgParameter) : ProgramAttributeCg(pCgParameter),
	m_nOpenGLAttributeLocation(cgGetParameterOrdinalNumber(pCgParameter))
{
}

/**
*  @brief
*    Destructor
*/
ProgramAttributeCgGLSL::~ProgramAttributeCgGLSL()
{
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::ProgramAttribute functions ]
//[-------------------------------------------------------]
bool ProgramAttributeCgGLSL::Set(PLRenderer::VertexBuffer *pVertexBuffer, uint32 nIndex)
{
	// We're going to need "glEnableVertexAttribArrayARB", "glDisableVertexAttribArrayARB" and "glVertexAttribPointerARB" from "GL_ARB_vertex_program"
	#ifdef WIN32
		static PFNGLENABLEVERTEXATTRIBARRAYARBPROC  glEnableVertexAttribArrayARB  = reinterpret_cast<PFNGLENABLEVERTEXATTRIBARRAYARBPROC> (wglGetProcAddress("glEnableVertexAttribArrayARB"));
		static PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArrayARB = reinterpret_cast<PFNGLDISABLEVERTEXATTRIBARRAYARBPROC>(wglGetProcAddress("glDisableVertexAttribArrayARB"));
		static PFNGLVERTEXATTRIBPOINTERARBPROC      glVertexAttribPointerARB      = reinterpret_cast<PFNGLVERTEXATTRIBPOINTERARBPROC>     (wglGetProcAddress("glVertexAttribPointerARB"));
	#elif LINUX
		static PFNGLENABLEVERTEXATTRIBARRAYARBPROC  glEnableVertexAttribArrayARB  = reinterpret_cast<PFNGLENABLEVERTEXATTRIBARRAYARBPROC> (glXGetProcAddressARB(reinterpret_cast<const GLubyte*>("glEnableVertexAttribArrayARB")));
		static PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArrayARB = reinterpret_cast<PFNGLDISABLEVERTEXATTRIBARRAYARBPROC>(glXGetProcAddressARB(reinterpret_cast<const GLubyte*>("glDisableVertexAttribArrayARB")));
		static PFNGLVERTEXATTRIBPOINTERARBPROC      glVertexAttribPointerARB      = reinterpret_cast<PFNGLVERTEXATTRIBPOINTERARBPROC>     (glXGetProcAddressARB(reinterpret_cast<const GLubyte*>("glVertexAttribPointerARB")));
	#endif
	if (glEnableVertexAttribArrayARB && glDisableVertexAttribArrayARB && glVertexAttribPointerARB) {
		// Enable or disable the vertex attribute array?
		if (pVertexBuffer) {
			// Get the vertex buffer attribute
			const PLRenderer::VertexBuffer::Attribute *pVertexAttribute = pVertexBuffer->GetVertexAttribute(nIndex);
			if (pVertexAttribute) {
				// Bind and update the vertex buffer if required
				static_cast<PLRendererOpenGL::VertexBuffer*>(pVertexBuffer)->BindAndUpdate();

				// Create the connection between "vertex program attribute" and "vertex buffer attribute"
				glVertexAttribPointerARB(m_nOpenGLAttributeLocation, pVertexAttribute->nComponentsAPI, pVertexAttribute->nTypeAPI, GL_FALSE, pVertexBuffer->GetVertexSize(), reinterpret_cast<const void*>(pVertexAttribute->nOffset));

				// Enable vertex attribute array (OpenGL program independent, so we don't need to call glUseProgram first!)
				glEnableVertexAttribArrayARB(m_nOpenGLAttributeLocation);
			} else {
				// Disable vertex attribute array (OpenGL program independent, so we don't need to call glUseProgram first!)
				glDisableVertexAttribArrayARB(m_nOpenGLAttributeLocation);
			}
		} else {
			// Disable vertex attribute array (OpenGL program independent, so we don't need to call glUseProgram first!)
			glDisableVertexAttribArrayARB(m_nOpenGLAttributeLocation);
		}
	}

	// Done
	return true;
}

bool ProgramAttributeCgGLSL::Set(PLRenderer::VertexBuffer *pVertexBuffer, PLRenderer::VertexBuffer::ESemantic nSemantic, uint32 nChannel)
{
	// We're going to need "glEnableVertexAttribArrayARB", "glDisableVertexAttribArrayARB" and "glVertexAttribPointerARB" from "GL_ARB_vertex_program"
	#ifdef WIN32
		static PFNGLENABLEVERTEXATTRIBARRAYARBPROC  glEnableVertexAttribArrayARB  = reinterpret_cast<PFNGLENABLEVERTEXATTRIBARRAYARBPROC> (wglGetProcAddress("glEnableVertexAttribArrayARB"));
		static PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArrayARB = reinterpret_cast<PFNGLDISABLEVERTEXATTRIBARRAYARBPROC>(wglGetProcAddress("glDisableVertexAttribArrayARB"));
		static PFNGLVERTEXATTRIBPOINTERARBPROC      glVertexAttribPointerARB      = reinterpret_cast<PFNGLVERTEXATTRIBPOINTERARBPROC>     (wglGetProcAddress("glVertexAttribPointerARB"));
	#elif LINUX
		static PFNGLENABLEVERTEXATTRIBARRAYARBPROC  glEnableVertexAttribArrayARB  = reinterpret_cast<PFNGLENABLEVERTEXATTRIBARRAYARBPROC> (glXGetProcAddressARB(reinterpret_cast<const GLubyte*>("glEnableVertexAttribArrayARB")));
		static PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArrayARB = reinterpret_cast<PFNGLDISABLEVERTEXATTRIBARRAYARBPROC>(glXGetProcAddressARB(reinterpret_cast<const GLubyte*>("glDisableVertexAttribArrayARB")));
		static PFNGLVERTEXATTRIBPOINTERARBPROC      glVertexAttribPointerARB      = reinterpret_cast<PFNGLVERTEXATTRIBPOINTERARBPROC>     (glXGetProcAddressARB(reinterpret_cast<const GLubyte*>("glVertexAttribPointerARB")));
	#endif
	if (glEnableVertexAttribArrayARB && glDisableVertexAttribArrayARB && glVertexAttribPointerARB) {
		// Enable or disable the vertex attribute array?
		if (pVertexBuffer) {
			// Get the vertex buffer attribute
			const PLRenderer::VertexBuffer::Attribute *pVertexAttribute = pVertexBuffer->GetVertexAttribute(nSemantic, nChannel);
			if (pVertexAttribute) {
				// Bind and update the vertex buffer if required
				static_cast<PLRendererOpenGL::VertexBuffer*>(pVertexBuffer)->BindAndUpdate();

				// Create the connection between "vertex program attribute" and "vertex buffer attribute"
				glVertexAttribPointerARB(m_nOpenGLAttributeLocation, pVertexAttribute->nComponentsAPI, pVertexAttribute->nTypeAPI, GL_FALSE, pVertexBuffer->GetVertexSize(), reinterpret_cast<const void*>(pVertexAttribute->nOffset));

				// Enable vertex attribute array (OpenGL program independent, so we don't need to call glUseProgram first!)
				glEnableVertexAttribArrayARB(m_nOpenGLAttributeLocation);
			} else {
				// Disable vertex attribute array (OpenGL program independent, so we don't need to call glUseProgram first!)
				glDisableVertexAttribArrayARB(m_nOpenGLAttributeLocation);
			}
		} else {
			// Disable vertex attribute array (OpenGL program independent, so we don't need to call glUseProgram first!)
			glDisableVertexAttribArrayARB(m_nOpenGLAttributeLocation);
		}
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGLCg
