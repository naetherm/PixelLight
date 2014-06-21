/*********************************************************\
 *  File: FrameBufferObject.h                            *
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


#ifndef __PLRENDEREROPENGL_FRAMEBUFFEROBJECT_H__
#define __PLRENDEREROPENGL_FRAMEBUFFEROBJECT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2i.h>
#include <PLRenderer/Renderer/TextureBuffer.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class TextureBufferBuffer;
class Renderer;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Frame buffer object
*/
class FrameBufferObject {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Frame buffer flags
		*/
		enum EFlags {
			Color   = 1<<0,	/**< Color buffer */
			Depth16 = 1<<1,	/**< 16 bit depth buffer */
			Depth24 = 1<<2,	/**< 24 bit depth buffer */
			Depth32 = 1<<3,	/**< 32 bit depth buffer */
			Stencil = 1<<4	/**< Stencil buffer */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		FrameBufferObject();

		/**
		*  @brief
		*    Destructor
		*/
		~FrameBufferObject();

		/**
		*  @brief
		*    Initializes the frame buffer object
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] vSize
		*    Frame buffer size
		*  @param[in] nFormat
		*    Frame buffer format
		*  @param[in] bNoMultisampleAntialiasing
		*    Do not use multisample antialiasing
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool Initialize(Renderer &cRenderer, const PLMath::Vector2i &vSize, PLCore::uint32 nFormat, PLRenderer::TextureBuffer::EPixelFormat nTextureFormat, bool bNoMultisampleAntialiasing);

		/**
		*  @brief
		*    Switches the texture buffer target
		*
		*  @param[in] cTextureBuffer
		*    Target texture buffer
		*  @param[in] nAttachIndex
		*    Color attachment index
		*  @param[in] nFace
		*    Cube map face to render in
		*/
		void SwitchTarget(PLRenderer::TextureBuffer &cTextureBuffer, PLCore::uint32 nAttachIndex = 0, PLCore::uint8 nFace = 0);

		/**
		*  @brief
		*    Binds the frame buffer
		*/
		void Bind();

		/**
		*  @brief
		*    Finishes the process
		*/
		void Finish();

		/**
		*  @brief
		*    Unbinds the frame buffer
		*/
		void Unbind();

		/**
		*  @brief
		*    Unbinds the currently used depth texture from the frame buffer
		*/
		void UnbindDephTexture();

		/**
		*  @brief
		*    Returns the size of the frame buffer
		*
		*  @return
		*    Size of the frame buffer
		*/
		const PLMath::Vector2i &GetSize() const;

		/**
		*  @brief
		*    Takes over the control of the depth buffer of the given FBO
		*
		*  @param[in] cFBO
		*    FBO we take the depth buffer away
		*/
		void TakeDepthBufferFromFBO(FrameBufferObject &cFBO);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Checks the frame buffer
		*
		*  @return
		*    'true' if all is ok, else 'false'
		*/
		bool CheckFrameBufferStatus() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLMath::Vector2i m_vSize;
		GLuint			 m_nFrameBufferIndex;
		GLuint			 m_nMultisampleFrameBufferIndex;
		GLuint			 m_nRenderToFrameBufferIndex;	/**< Points to "m_nFrameBufferIndex" or "m_nMultisampleFrameBufferIndex" */
		GLuint			 m_nColorBufferIndex;
		GLuint			 m_nDepthBufferIndex;
		GLuint			 m_nStencilBufferIndex;
		GLuint			 m_nDepthBufferAttachment;		/**< Depth buffer attachment (GL_DEPTH_ATTACHMENT_EXT or GL_DEPTH_STENCIL_ATTACHMENT) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL


#endif // __PLRENDEREROPENGL_FRAMEBUFFEROBJECT_H__
