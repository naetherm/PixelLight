/*********************************************************\
 *  File: SurfaceTextureBuffer.h                         *
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


#ifndef __PLRENDEREROPENGL_SURFACETEXTUREBUFFER_H__
#define __PLRENDEREROPENGL_SURFACETEXTUREBUFFER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/SurfaceTextureBuffer.h>
#include <PLRenderer/Renderer/ResourceHandler.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class FrameBufferObject;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    A OpenGL texture buffer renderer surface where we can render in
*/
class SurfaceTextureBuffer : public PLRenderer::SurfaceTextureBuffer {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Renderer;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SurfaceTextureBuffer();

		/**
		*  @brief
		*    Returns whether FBO is used or not
		*
		*  @return
		*    'true' if a FBO is used, else 'false'
		*/
		bool IsFBOUsed() const;

		/**
		*  @brief
		*    Sets a color render target
		*
		*  @param[in] nColorIndex
		*    Index of the color render target to set
		*  @param[in] pTextureBuffer
		*    Texture buffer to render in, can be a null pointer
		*/
		void SetColorRenderTarget(PLCore::uint8 nColorIndex, PLRenderer::TextureBuffer *pTextureBuffer);

		/**
		*  @brief
		*    Sets a depth render target
		*
		*  @param[in] pTextureBuffer
		*    Texture buffer to render in, can be a null pointer
		*/
		void SetDepthRenderTarget(PLRenderer::TextureBuffer *pTextureBuffer);

		/**
		*  @brief
		*    Finishes the process
		*/
		void Finish();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    Owner renderer
		*  @param[in] cTextureBuffer
		*    Texture buffer bound to the this surface
		*  @param[in] nFlags
		*    Texture buffer surface flags (see EFlags)
		*  @param[in] nMaxColorTargets
		*    Maximum number of color render targets. This must be at least 1 - main renderer target color.
		*/
		SurfaceTextureBuffer(PLRenderer::Renderer &cRenderer, PLRenderer::TextureBuffer &cTexture,
							 PLCore::uint32 nFlags = Depth | Stencil, PLCore::uint8 nMaxColorTargets = 1);

		/*
		*  @brief
		*    Create a FBO
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool CreateFBO();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		FrameBufferObject			*m_pFrameBufferObject;		/**< Nice frame buffer object, can be a null pointer */
		PLRenderer::ResourceHandler	 m_cTextureBufferHandler;	/**< Texture buffer to render in */
		PLCore::uint8			 	 m_nFace;					/**< Texture buffer face currently rendered in (cube map) */

		PLCore::List<PLRenderer::ResourceHandler*> m_lstTextureBufferHandler;


	//[-------------------------------------------------------]
	//[ Public virtual PLRenderer::SurfaceTexture functions   ]
	//[-------------------------------------------------------]
	public:
		virtual PLRenderer::TextureBuffer *GetTextureBuffer() const override;
		virtual PLCore::uint8 GetTextureBufferFace() const override;
		virtual void TakeDepthBufferFromSurfaceTextureBuffer(PLRenderer::SurfaceTextureBuffer &cSurfaceTextureBuffer) override;


	//[-------------------------------------------------------]
	//[ Public virtual PLRenderer::Surface functions          ]
	//[-------------------------------------------------------]
	public:
		virtual PLMath::Vector2i GetSize() const override;


	//[-------------------------------------------------------]
	//[ Private virtual PLRenderer::Surface functions         ]
	//[-------------------------------------------------------]
	private:
		virtual bool Init() override;
		virtual void DeInit() override;
		virtual bool MakeCurrent(PLCore::uint8 nFace = 0) override;
		virtual bool UnmakeCurrent() override;
		virtual bool Present() override;
		virtual void BackupDeviceData() override;
		virtual void RestoreDeviceData() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL


#endif // __PLRENDEREROPENGL_SURFACETEXTUREBUFFER_H__
