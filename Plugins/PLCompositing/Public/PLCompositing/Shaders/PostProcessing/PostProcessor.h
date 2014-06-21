/*********************************************************\
 *  File: PostProcessor.h                                *
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


#ifndef __PLCOMPOSITING_POSTPROCESSING_POSTPROCESSOR_H__
#define __PLCOMPOSITING_POSTPROCESSING_POSTPROCESSOR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/TextureBuffer.h>
#include <PLRenderer/Texture/TextureHandler.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class VertexBuffer;
	class SurfaceTextureBuffer;
}
namespace PLCompositing {
	class PostProcess;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCompositing {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Post processor class
*
*  @remarks
*    Using post processing you can create all the amazing post process
*    effects like blur, bloom, color manipulation - and even some fun effects like
*    ASCII character rendering. :)
*    The process takes some input textures like color, position, normal etc.
*    and applies different post processes on it. Then the final result is
*    returned as texture you can draw on the screen using a quadrangle.
*
*  @note
*    - Each post process material can have annotation for extent info.
*      (Material parameters fScaleX & fScaleY, default value for both 1.0f)
*    - Each post process material can have different kernels which are converted
*      from pixel space to texel space automatically.\n
*      texelKernel[n].x = pixelKernel[n].x/width\n
*      texelKernel[n].y = pixelKernel[n].y/height\n
*      whereby width and height are the dimension of the render target.
*    - Material Parameter 'targetDimension' will receive the render target width and height
*    - Some special post process effects have special parameters - in this
*      case you have to derive an own class from PostProcess and implement
*      the update of this special material parameters by yourself.
*    - 2D and rectangle texture targets are supported
*/
class PostProcessor {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLCOM_API PostProcessor();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~PostProcessor();

		/**
		*  @brief
		*    Returns the texture format
		*
		*  @return
		*    The texture format
		*
		*  @note
		*    - The default texture format is PLRenderer::TextureBuffer::R8G8B8A8, but some effects like
		*      tone map need PLRenderer::TextureBuffer::R16G16B16A16F for high dynamic range rendering
		*    - Supported texture formats: PLRenderer::TextureBuffer::R8G8B8, PLRenderer::TextureBuffer::R8G8B8A8,
		*      PLRenderer::TextureBuffer::R16G16B16A16F, PLRenderer::TextureBuffer::R32G32B32A32F
		*/
		PLCOM_API PLRenderer::TextureBuffer::EPixelFormat GetTextureFormat() const;

		/**
		*  @brief
		*    Sets the texture format
		*
		*  @param[in] nFormat
		*    The texture format
		*
		*  @see
		*    - GetTextureFormat
		*/
		PLCOM_API void SetTextureFormat(PLRenderer::TextureBuffer::EPixelFormat nFormat = PLRenderer::TextureBuffer::R8G8B8A8);

		/**
		*  @brief
		*    Returns whether or not processing is currently active
		*
		*  @return
		*    'true' if processing is currently active, else 'false'
		*/
		PLCOM_API bool IsProcessing() const;

		/**
		*  @brief
		*    Begins the processing
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cColorSurface
		*    Color surface to use
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCOM_API bool BeginProcessing(PLRenderer::Renderer &cRenderer, PLRenderer::SurfaceTextureBuffer &cColorSurface);

		/**
		*  @brief
		*    Performs a single post process
		*
		*  @param[in] cPostProcess
		*    Current post process
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @remarks
		*    Perform post-process by setting the previous render target as a
		*    source texture and rendering a quad with the post-process technique
		*    set.\n
		*    Extents are used to control how much of the render target is rendered
		*    during post process. For example, with the extent of 0.5 and 0.5, only
		*    the upper left quarter of the render target will be rendered during
		*    post process.
		*
		*  @note
		*    - This method changes render target without saving any. The caller
		*      should ensure that the default render target is saved before calling
		*      this.
		*/
		bool Process(const PostProcess &cPostProcess);

		/**
		*  @brief
		*    Ends the processing
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCOM_API bool EndProcessing();

		/**
		*  @brief
		*    Draws the result into the current render target
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] fX1
		*    Rectangle for the quadrangle on the screen x start position
		*  @param[in] fY1
		*    Rectangle for the quadrangle on the screen y start position
		*  @param[in] fX2
		*    Rectangle for the quadrangle on the screen x end position
		*  @param[in] fY2
		*    Rectangle for the quadrangle on the screen y end position
		*
		*  @remarks
		*    The final image you can get using GetCurrentSurface() is drawn into the
		*    current render target using a quadrangle.
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCOM_API bool DrawResult(PLRenderer::Renderer &cRenderer, float fX1 = 0.0f, float fY1 = 0.0f,
							   float fX2 = 1.0f, float fY2 = 1.0f) const;

		/**
		*  @brief
		*    Returns the current texture surface
		*
		*  @return
		*    The current texture buffer surface with the result of the post process. This can be the
		*    'original' texture buffer surface or one of the two internal texture buffer surfaces. Can be a null pointer.
		*/
		PLCOM_API PLRenderer::SurfaceTextureBuffer *GetCurrentSurface() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLRenderer::TextureBuffer::EPixelFormat  m_nTextureFormat;
		PLRenderer::SurfaceTextureBuffer		*m_pColorSurface;		/**< Can be a null pointer */
		PLRenderer::VertexBuffer				*m_pVertexBuffer;		/**< Can be a null pointer */
		bool									 m_bProcessing;			/**< Processing active? */
		bool									 m_bFirstPostProcess;	/**< Is this the first active post process? */
		float									 m_fExtentX;			/**< Current x extension of the quad */
		float									 m_fExtentY;			/**< Current y extension of the quad */
		PLCore::uint32						 	 m_nFixedFillModeBackup;

		PLRenderer::SurfaceTextureBuffer *m_pRenderTarget[2];			/**< Can be a null pointer */
		bool							  m_bClearRenderTarget[2];
		bool							  m_bCurrentRenderTarget;
		PLRenderer::TextureHandler		  m_cSourceColorTexture;		/**< The source texture for the given render pass */
		PLRenderer::TextureHandler		  m_cSceneColorTexture;			/**< The original given texture */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLCOMPOSITING_POSTPROCESSING_POSTPROCESSOR_H__
