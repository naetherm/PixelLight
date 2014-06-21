/*********************************************************\
 *  File: SRPBegin.h                                     *
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


#ifndef __PLCOMPOSITING_BEGIN_H__
#define __PLCOMPOSITING_BEGIN_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGraphics/Color/Color4.h>
#include <PLRenderer/Renderer/TextureBuffer.h>
#include <PLScene/Compositing/SceneRendererPass.h>
#include "PLCompositing/PLCompositing.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class Surface;
	class SurfaceTextureBuffer;
	class TextureBufferRectangle;
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
*    First scene renderer pass
*
*  @note
*    - This scene renderer pass should be at the very top of the scene render pipeline
*    - Sets initial render states - sets all render states to known default settings
*    - Manages render to texture with double buffering support
*    - When render to texture is used, a separate depth buffer is used which is independent of the one of the surface we're render into
*    - Doesn't use shaders or fixed functions features, therefore there's usually no need to derive from this class
*/
class SRPBegin : public PLScene::SceneRendererPass {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Clear flags
		*/
		enum EClearFlags {
			ClearColor   = 1,	/**< Clear color buffer */
			ClearDepth   = 2,	/**< Clear depth buffer */
			ClearStencil = 4	/**< Clear stencil buffer */
		};
		pl_enum(EClearFlags)
			pl_enum_value(ClearColor,	"Clear color buffer")
			pl_enum_value(ClearDepth,	"Clear depth buffer")
			pl_enum_value(ClearStencil,	"Clear stencil buffer")
		pl_enum_end

		/**
		*  @brief
		*    Fill mode
		*/
		enum EFillMode {
			SolidMode = 0,	/**< Solid fill mode */
			LineMode  = 1,	/**< Line fill mode (also called wireframes) */
			PointMode = 2	/**< Point fill mode */
		};
		pl_enum(EFillMode)
			pl_enum_value(SolidMode,	"Solid fill mode")
			pl_enum_value(LineMode,		"Line fill mode (also called wireframes)")
			pl_enum_value(PointMode,	"Point fill mode")
		pl_enum_end

		/**
		*  @brief
		*    Scene renderer pass flags (SceneRendererPass flags extension)
		*/
		enum EFlags {
			NoStencil				  = 1<<1,	/**< No stencil buffer */
			NoMultisampleAntialiasing = 1<<2,	/**< Do not use multisample antialiasing */
			NoDepthTexture			  = 1<<4,	/**< Do not provide a depth texture when rendering into a texture */
			CustomTextureSize		  = 1<<5	/**< Use a texture size given by the user instead of using the original window size */
		};
		pl_enum(EFlags)
			pl_enum_base(SceneRendererPass::EFlags)
			pl_enum_value(NoStencil,				 "No stencil buffer")
			pl_enum_value(NoMultisampleAntialiasing, "Do not use multisample antialiasing")
			pl_enum_value(NoDepthTexture,			 "Do not provide a depth texture when rendering into a texture")
			pl_enum_value(CustomTextureSize,		 "Use a texture size given by the user instead of using the original window size")
		pl_enum_end

		/**
		*  @brief
		*    Texture format
		*/
		pl_enum_direct(ETextureFormat, int)
			pl_enum_value_direct(L8,			PLRenderer::TextureBuffer::L8,				"8-bit pixel format, all bits luminance")
			pl_enum_value_direct(L16,			PLRenderer::TextureBuffer::L16,				"16-bit pixel format, all bits luminance")
			pl_enum_value_direct(A8,			PLRenderer::TextureBuffer::A8,				"8-bit pixel format, all bits alpha")
			pl_enum_value_direct(L4A4,			PLRenderer::TextureBuffer::L4A4,			"8-bit pixel format, 4 bits for luminance and alpha")
			pl_enum_value_direct(L8A8,			PLRenderer::TextureBuffer::L8A8,			"16-bit pixel format, 8 bits for luminance and alpha")
			pl_enum_value_direct(R3G3B2,		PLRenderer::TextureBuffer::R3G3B2,			"8-bit pixel format, 3 bits red, 3 bits green and 2 bits blue")
			pl_enum_value_direct(R5G6B5,		PLRenderer::TextureBuffer::R5G6B5,			"16-bit pixel format, 5 bits red, 6 bits green and 5 bits blue")
			pl_enum_value_direct(R5G5B5A1,		PLRenderer::TextureBuffer::R5G5B5A1,		"16-bit pixel format, 5 bits red, 5 bits green, 5 bits blue and 1 bits alpha")
			pl_enum_value_direct(R4G4B4A4,		PLRenderer::TextureBuffer::R4G4B4A4,		"16-bit pixel format, 4 bits for red, green, blue and alpha")
			pl_enum_value_direct(R8G8B8,		PLRenderer::TextureBuffer::R8G8B8,			"24-bit (or 32-bit if 24-bits are not supported by the hardware) pixel format, 8 bits for red, green and blue")
			pl_enum_value_direct(R8G8B8A8,		PLRenderer::TextureBuffer::R8G8B8A8,		"32-bit pixel format, 8 bits for red, green, blue and alpha")
			pl_enum_value_direct(R10G10B10A2,	PLRenderer::TextureBuffer::R10G10B10A2,		"32-bit pixel format, 10 bits for red, green, blue and 2 bits for alpha")
			pl_enum_value_direct(R16G16B16A16,	PLRenderer::TextureBuffer::R16G16B16A16,	"64-bit pixel format, 16 bits for red, green, blue and alpha")
			pl_enum_value_direct(L16F,			PLRenderer::TextureBuffer::L16F,			"16-bit float format using 16 bits for luminance")
			pl_enum_value_direct(L32F,			PLRenderer::TextureBuffer::L32F,			"32-bit float format using 32 bits for luminance")
			pl_enum_value_direct(R16G16B16A16F,	PLRenderer::TextureBuffer::R16G16B16A16F,	"64-bit float format using 16 bits for the each channel (red, green, blue, alpha)")
			pl_enum_value_direct(R32G32B32A32F,	PLRenderer::TextureBuffer::R32G32B32A32F,	"128-bit float format using 32 bits for the each channel (red, green, blue, alpha)")
			pl_enum_value_direct(Unknown,		PLRenderer::TextureBuffer::Unknown,			"Unknown texture pixel format")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SRPBegin, "PLCompositing", PLScene::SceneRendererPass, "First scene renderer pass")
		// Attributes
		pl_attribute(TextureFormat,	pl_enum_type(ETextureFormat),	PLRenderer::TextureBuffer::Unknown,			ReadWrite,	DirectValue,	"Render target texture format, unknown means no render to texture",	"")
		pl_attribute(TextureSize,	PLMath::Vector2i,				PLMath::Vector2i::Zero,						ReadWrite,	DirectValue,	"Render target size, only used in case the ''-flag is set",	"")
		pl_attribute(ClearFlags,	pl_flag_type(EClearFlags),		ClearColor|ClearDepth|ClearStencil,			ReadWrite,	DirectValue,	"Clear flags",														"")
		pl_attribute(ColorClear,	PLGraphics::Color4,				PLGraphics::Color4(0.0f, 0.0f, 0.0f, 0.0f),	ReadWrite,	DirectValue,	"Clear color (r/g/b/a)",											"")
		pl_attribute(FillMode,		pl_enum_type(EFillMode),		SolidMode,									ReadWrite,	DirectValue,	"Fill mode",														"")
			// Overwritten PLScene::SceneRendererPass attributes
		pl_attribute(Flags,			pl_flag_type(EFlags),			0,											ReadWrite,	GetSet,			"Flags",															"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCOM_API SRPBegin();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SRPBegin();

		/**
		*  @brief
		*    Returns the render target that was set before the current render target was changed to the front/back render target
		*
		*  @return
		*    The render target that was set before the current render target was changed to the front/back render target, can be a null pointer
		*/
		PLCOM_API PLRenderer::Surface *GetOriginalRenderTarget() const;

		/**
		*  @brief
		*    Returns the current front render target
		*
		*  @return
		*    The current front render target, can be a null pointer
		*/
		PLCOM_API PLRenderer::SurfaceTextureBuffer *GetFrontRenderTarget() const;

		/**
		*  @brief
		*    Returns the current back render target
		*
		*  @return
		*    The current back render target, can be a null pointer
		*
		*  @note
		*    - The back render target receives the current drawings
		*/
		PLCOM_API PLRenderer::SurfaceTextureBuffer *GetBackRenderTarget() const;

		/**
		*  @brief
		*    Returns the depth texture (used when rendering into a texture)
		*
		*  @return
		*    Depth texture (used when rendering into a texture), can be a null pointer
		*/
		PLCOM_API PLRenderer::TextureBufferRectangle *GetTextureBufferDepth() const;

		/**
		*  @brief
		*    Swaps the render targets
		*/
		PLCOM_API void SwapRenderTargets();


	//[-------------------------------------------------------]
	//[ Protected virtual PLScene::SceneRendererPass functions ]
	//[-------------------------------------------------------]
	protected:
		virtual void Draw(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLRenderer::Surface				 *m_pOriginalRenderTarget;		/**< The render target that was set before the current render target was changed to the front/back render target, can be a null pointer */
		PLRenderer::SurfaceTextureBuffer *m_pRenderTarget[2];			/**< Render targets, can be a null pointer */
		bool							  m_bCurrentFrontRenderTarget;	/**< Index of the current front render target */
		PLRenderer::SurfaceTextureBuffer *m_pTextureBufferDepth;		/**< Depth texture (used when rendering into a texture) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLCOMPOSITING_BEGIN_H__
