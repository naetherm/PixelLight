/*********************************************************\
 *  File: FrameBufferObject.cpp                          *
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
#include <PLCore/Log/Log.h>
#include "PLRendererOpenGL/Context.h"
#include "PLRendererOpenGL/Renderer.h"
#include "PLRendererOpenGL/Extensions.h"
#include "PLRendererOpenGL/TextureBuffer1D.h"
#include "PLRendererOpenGL/TextureBuffer2D.h"
#include "PLRendererOpenGL/TextureBuffer2DArray.h"
#include "PLRendererOpenGL/TextureBufferRectangle.h"
#include "PLRendererOpenGL/TextureBuffer3D.h"
#include "PLRendererOpenGL/TextureBufferCube.h"
#include "PLRendererOpenGL/FrameBufferObject.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FrameBufferObject::FrameBufferObject() :
	m_nFrameBufferIndex(0),
	m_nMultisampleFrameBufferIndex(0),
	m_nRenderToFrameBufferIndex(0),
	m_nColorBufferIndex(0),
	m_nDepthBufferIndex(0),
	m_nStencilBufferIndex(0),
	m_nDepthBufferAttachment(GL_DEPTH_ATTACHMENT_EXT)
{
}

/**
*  @brief
*    Destructor
*/
FrameBufferObject::~FrameBufferObject()
{
	// Destroy buffers
	if (m_nColorBufferIndex)
		glDeleteRenderbuffersEXT(1, &m_nColorBufferIndex);
	if (m_nDepthBufferIndex)
		glDeleteRenderbuffersEXT(1, &m_nDepthBufferIndex);
	if (m_nStencilBufferIndex)
		glDeleteRenderbuffersEXT(1, &m_nStencilBufferIndex);
	if (m_nMultisampleFrameBufferIndex)
		glDeleteFramebuffersEXT(1, &m_nMultisampleFrameBufferIndex);
	if (m_nFrameBufferIndex)
		glDeleteFramebuffersEXT(1, &m_nFrameBufferIndex);
}

/**
*  @brief
*    Initializes the frame buffer object
*/
bool FrameBufferObject::Initialize(Renderer &cRenderer, const Vector2i &vSize, uint32 nFormat, PLRenderer::TextureBuffer::EPixelFormat nTextureFormat, bool bNoMultisampleAntialiasing)
{
	const Extensions &cExtensions = cRenderer.GetContext().GetExtensions();

	// Is the extension EXT_framebuffer_object available?
	if (cExtensions.IsGL_EXT_framebuffer_object()) {
		// Check parameters
		if (vSize.x != 0 && vSize.y != 0) {
			// Save size
			m_vSize = vSize;

			// Get multisample antialiasing samples
			const uint32 nMultisampleAntialiasingSamples = (!bNoMultisampleAntialiasing && cExtensions.IsGL_EXT_framebuffer_multisample() && cExtensions.IsGL_EXT_framebuffer_blit()) ? cRenderer.GetMultisampleAntialiasingSamples() : 0;

			// Get current bound FBO
			GLint nFrameBufferT;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &nFrameBufferT);

			// Create color buffer?
			if (!PLRenderer::TextureBuffer::IsDepthFormat(nTextureFormat)) {
				// Get API pixel format
				const uint32 *pAPIPixelFormat = cRenderer.GetAPIPixelFormat(nTextureFormat);

				// Create color buffer
				glGenRenderbuffersEXT(1, &m_nColorBufferIndex);
				glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_nColorBufferIndex);
				if (nMultisampleAntialiasingSamples)
					glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, nMultisampleAntialiasingSamples, *pAPIPixelFormat, m_vSize.x, m_vSize.y);
				else
					glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, *pAPIPixelFormat, m_vSize.x, m_vSize.y);
			}

			// Create depth&stencil buffer
			GLuint nDepth = 0;
			bool bDepthStencil = false;
			if ((nFormat & Depth24) && (nFormat & Stencil) && cExtensions.IsGL_EXT_packed_depth_stencil()) {
				nDepth        = GL_DEPTH24_STENCIL8_EXT;
				bDepthStencil = true;
			} else {
				if (nFormat & Depth16)
					nDepth = GL_DEPTH_COMPONENT16;
				else if (nFormat & Depth24)
					nDepth = GL_DEPTH_COMPONENT24;
				else if (nFormat & Depth32)
					nDepth = GL_DEPTH_COMPONENT32;
			}
			if (nDepth) {
				glGenRenderbuffersEXT(1, &m_nDepthBufferIndex);
				glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_nDepthBufferIndex);
				// [TODO] We might check this in more detail:
				// - While e.g. "GL_DEPTH_COMPONENT24" works on AMD GPUs, on NVIDIA GPUs only "GL_DEPTH_COMPONENT" works
				// - Check the framebuffer specification and so on to verify this behaviour
				if (nMultisampleAntialiasingSamples) {
					glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, nMultisampleAntialiasingSamples, nDepth,			  m_vSize.x, m_vSize.y);
				//	glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, nMultisampleAntialiasingSamples, GL_DEPTH_COMPONENT, m_vSize.x, m_vSize.y);
				} else {
					glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, nDepth,			  m_vSize.x, m_vSize.y);
			//		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, m_vSize.x, m_vSize.y);
				}
			}

			// Create stencil buffer
			if (!bDepthStencil && (nFormat & Stencil)) {
				// [TODO] Stencil buffer support currently not working?! (but at least GL_EXT_packed_depth_stencil works)
//				glGenRenderbuffersEXT(1, &m_nStencilBufferIndex);
//				glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_nStencilBufferIndex);
//				glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_STENCIL_INDEX8_EXT, m_vSize.x, m_vSize.y);
			}

			// Create final FBO we will attach textures to
			glGenFramebuffersEXT(1, &m_nFrameBufferIndex);

			// Create multisample FBO used for rendering
			if (nMultisampleAntialiasingSamples) {
				glGenFramebuffersEXT(1, &m_nMultisampleFrameBufferIndex);
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_nMultisampleFrameBufferIndex);
			} else {
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_nFrameBufferIndex);
			}

			// Bind frame buffer
			if (m_nColorBufferIndex)
				glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, m_nColorBufferIndex);
			if (m_nDepthBufferIndex) {
				// Get depth buffer attachment
				m_nDepthBufferAttachment = ((nFormat & Stencil) && !m_nStencilBufferIndex) ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT_EXT;

				// Attach depth buffer
				glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, m_nDepthBufferAttachment, GL_RENDERBUFFER_EXT, m_nDepthBufferIndex);
			}
			if (m_nStencilBufferIndex)
				glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_nStencilBufferIndex);

			// Check frame buffer
			const bool bResult = CheckFrameBufferStatus();
			if (bResult)
				m_nRenderToFrameBufferIndex = m_nMultisampleFrameBufferIndex ? m_nMultisampleFrameBufferIndex : m_nFrameBufferIndex;
			else
				m_nRenderToFrameBufferIndex = 0;

			// Reset current bound FBO
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, nFrameBufferT);

			// Done
			return bResult;
		} else {
			PL_LOG(Error, "Width and height of FBO must be none zero!")
		}
	} else {
		PL_LOG(Error, "GL_EXT_framebuffer_object not supported!")
	}

	// Error!
	return false;
}

/**
*  @brief
*    Switches the texture buffer target
*/
void FrameBufferObject::SwitchTarget(PLRenderer::TextureBuffer &cTextureBuffer, uint32 nAttachIndex, uint8 nFace)
{
	// Valid frame buffer object?
	if (m_nRenderToFrameBufferIndex) {
		// Get target format and OpenGL texture ID
		int nTarget = -1;
		GLuint nOpenGLID = 0;
		switch (cTextureBuffer.GetType()) {
			case PLRenderer::Resource::TypeTextureBuffer1D:
				nTarget = GL_TEXTURE_1D;
				nOpenGLID = static_cast<TextureBuffer1D&>(cTextureBuffer).GetOpenGLTexture();
				break;

			case PLRenderer::Resource::TypeTextureBuffer2D:
				nTarget = GL_TEXTURE_2D;
				nOpenGLID = static_cast<TextureBuffer2D&>(cTextureBuffer).GetOpenGLTexture();
				break;

			case PLRenderer::Resource::TypeTextureBuffer2DArray:
				nTarget = GL_TEXTURE_2D_ARRAY_EXT;
				nOpenGLID = static_cast<TextureBuffer2DArray&>(cTextureBuffer).GetOpenGLTexture();
				break;

			case PLRenderer::Resource::TypeTextureBufferRectangle:
				nTarget = GL_TEXTURE_RECTANGLE_ARB;
				nOpenGLID = static_cast<TextureBufferRectangle&>(cTextureBuffer).GetOpenGLTexture();
				break;

			case PLRenderer::Resource::TypeTextureBuffer3D:
				nTarget = GL_TEXTURE_3D_EXT;
				nOpenGLID = static_cast<TextureBuffer3D&>(cTextureBuffer).GetOpenGLTexture();
				break;

			case PLRenderer::Resource::TypeTextureBufferCube:
				nTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB+nFace;
				nOpenGLID = static_cast<TextureBufferCube&>(cTextureBuffer).GetOpenGLTexture();
				break;

			case PLRenderer::Resource::TypeIndexBuffer:
			case PLRenderer::Resource::TypeVertexBuffer:
			case PLRenderer::Resource::TypeUniformBuffer:
			case PLRenderer::Resource::TypeOcclusionQuery:
			case PLRenderer::Resource::TypeVertexShader:
			case PLRenderer::Resource::TypeTessellationControlShader:
			case PLRenderer::Resource::TypeTessellationEvaluationShader:
			case PLRenderer::Resource::TypeGeometryShader:
			case PLRenderer::Resource::TypeFragmentShader:
			case PLRenderer::Resource::TypeProgram:
			case PLRenderer::Resource::TypeFont:
			default:
				// Only texture types in this switch
				break;
		}
		if (nTarget >= 0 && nOpenGLID) {
			// Check/clamp attach index
			if (nAttachIndex > 15)
				nAttachIndex = 15;

			// Bind
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_nRenderToFrameBufferIndex);
			PLRenderer::TextureBuffer::EPixelFormat nFormat = cTextureBuffer.GetFormat();
			if (nFormat == PLRenderer::TextureBuffer::D16 || nFormat == PLRenderer::TextureBuffer::D24 || nFormat == PLRenderer::TextureBuffer::D32) {
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, m_nDepthBufferAttachment, nTarget, nOpenGLID, 0);
			} else {
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+nAttachIndex, nTarget, nOpenGLID, 0);
			}
		}
	}
}

/**
*  @brief
*    Binds the frame buffer
*/
void FrameBufferObject::Bind()
{
	if (m_nRenderToFrameBufferIndex) 
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_nRenderToFrameBufferIndex);
}

/**
*  @brief
*    Finishes the process
*/
void FrameBufferObject::Finish()
{
	// Use multisample?
	if (m_nMultisampleFrameBufferIndex && m_nFrameBufferIndex) {
		glDrawBuffer(GL_NONE);
		glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, m_nMultisampleFrameBufferIndex);
		glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, m_nFrameBufferIndex);
		glBlitFramebufferEXT(0, 0, m_vSize.x, m_vSize.y, 0, 0, m_vSize.x, m_vSize.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_nFrameBufferIndex);
	}
}

/**
*  @brief
*    Unbinds the frame buffer
*/
void FrameBufferObject::Unbind()
{
	// Use multisample?
	if (m_nMultisampleFrameBufferIndex) {
		if (m_nFrameBufferIndex) {
			// Finish the process
			Finish();

			// Unbind FBO
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		}
	} else {
		// Unbind FBO
		if (m_nFrameBufferIndex)
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}
}

/**
*  @brief
*    Unbinds the currently used depth texture from the frame buffer
*/
void FrameBufferObject::UnbindDephTexture()
{
	// Valid frame buffer object?
	if (m_nRenderToFrameBufferIndex) {
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_nRenderToFrameBufferIndex);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, m_nDepthBufferAttachment, GL_TEXTURE_2D, 0, 0);
	}
}

/**
*  @brief
*    Returns the size of the frame buffer
*/
const Vector2i &FrameBufferObject::GetSize() const
{
	return m_vSize;
}

/**
*  @brief
*    Takes over the control of the depth buffer of the given FBO
*/
void FrameBufferObject::TakeDepthBufferFromFBO(FrameBufferObject &cFBO)
{
	// Has the other FBO a depth buffer?
	if (cFBO.m_nDepthBufferIndex) {
		// Get current bound FBO
		GLint nFrameBufferT;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &nFrameBufferT);

		// [TODO] Test this/clean this up: May flicker on NVIDIA systems if we just attach to GL_DEPTH_ATTACHMENT
		GLuint nDepthBufferAttachment = GL_DEPTH_STENCIL_ATTACHMENT;

		// If we have a depth buffer, we destroy it because the depth buffer of the other FBO is much better *g*
		if (m_nDepthBufferIndex) {
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_nRenderToFrameBufferIndex);
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, nDepthBufferAttachment, GL_RENDERBUFFER_EXT, 0);
			glDeleteRenderbuffersEXT(1, &m_nDepthBufferIndex);
		}

		// Take away the ownership of the depth buffer
		m_nDepthBufferIndex = cFBO.m_nDepthBufferIndex;
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, cFBO.m_nRenderToFrameBufferIndex);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, nDepthBufferAttachment, GL_RENDERBUFFER_EXT, 0);
		cFBO.m_nDepthBufferIndex = 0;
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_nRenderToFrameBufferIndex);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, nDepthBufferAttachment, GL_RENDERBUFFER_EXT, m_nDepthBufferIndex);

		// Reset current bound FBO
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, nFrameBufferT);
	}
}

/**
*  @brief
*    Checks the frame buffer
*/
bool FrameBufferObject::CheckFrameBufferStatus() const
{
	bool bResult = true; // No error by default

	// Backup current draw and read buffer, then set them to none - else
	// 'glCheckFramebufferStatusEXT' will always return an error :(
	GLint nDrawBufferT;
	glGetIntegerv(GL_DRAW_BUFFER, &nDrawBufferT);
	glDrawBuffer(GL_NONE);
	GLint nReadBufferT;
	glGetIntegerv(GL_READ_BUFFER, &nReadBufferT);
	glReadBuffer(GL_NONE);

	// Check FBO status
	GLenum nStatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch (nStatus) {
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			PL_LOG(Debug, String::Format("Successfully created the frame buffer object (%dx%d)", m_vSize.x, m_vSize.y))
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			PL_LOG(Error, "Incomplete attachment frame buffer object")
			bResult = false;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			PL_LOG(Error, "Incomplete missing attachment frame buffer object")
			bResult = false;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			PL_LOG(Error, "Incomplete dimensions frame buffer object")
			bResult = false;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			PL_LOG(Error, "Incomplete formats frame buffer object")
			bResult = false;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			PL_LOG(Error, "Incomplete draw buffer frame buffer object")
			bResult = false;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			PL_LOG(Error, "Incomplete read buffer frame buffer object")
			bResult = false;
			break;

		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			PL_LOG(Error, "Failed to create the frame buffer object, please verify your format")
			bResult = false;
			break;
	}

	// Restore current draw and read buffer
	glDrawBuffer(nDrawBufferT);
	glReadBuffer(nReadBufferT);

	// Done
	return bResult;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL
