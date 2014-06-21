/*********************************************************\
 *  File: Renderer.h                                     *
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


#ifndef __PLRENDERERNULL_RENDERER_H__
#define __PLRENDERERNULL_RENDERER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/Backend/RendererBackend.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRendererNull {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class FixedFunctions;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Null renderer backend
*/
class Renderer : public PLRenderer::RendererBackend {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SurfaceWindow;


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(pl_rtti_export, Renderer, "PLRendererNull", PLRenderer::RendererBackend, "Null renderer backend")
		// Constructors
		pl_constructor_6(DefaultConstructor,	PLCore::handle,	pl_enum_type(EMode),	PLCore::uint32,	PLCore::uint32,	PLCore::uint32,	PLCore::String,	"Constructor with renderer mode, Z buffer bits, stencil buffer bits, the number of multisample antialiasing samples per pixel and the default shader language as parameter",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nNativeWindowHandle
		*    Handle of a native OS window which is valid as long as the renderer instance exists, "NULL_HANDLE" if there's no such window
		*  @param[in] nMode
		*    Mode hint the renderer should run in, the renderer is not enforced to use this requested mode
		*  @param[in] nZBufferBits
		*     Z buffer bits (for example 24)
		*  @param[in] nStencilBits
		*    Stencil buffer bits (for example 8)
		*  @param[in] nMultisampleAntialiasingSamples
		*    Multisample antialiasing samples per pixel, <=1 means no antialiasing
		*  @param[in] sDefaultShaderLanguage
		*    The name of the default shader language of the renderer (for example "GLSL" or "Cg"), if the string
		*    is empty, the default is chosen by the renderer implementation, this information is just a hint
		*/
		Renderer(PLCore::handle nNativeWindowHandle, EMode nMode, PLCore::uint32 nZBufferBits, PLCore::uint32 nStencilBits, PLCore::uint32 nMultisampleAntialiasingSamples, PLCore::String sDefaultShaderLanguage);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Renderer();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Setup the renderer capabilities
		*/
		void SetupCapabilities();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		FixedFunctions			*m_pFixedFunctions;	/**< Fixed functions interface implementation, can be a null pointer */
		PLRenderer::FontManager *m_pFontManager;	/**< Renderer font manager, always valid! */
		PLCore::uint32			 m_nViewPortX;
		PLCore::uint32			 m_nViewPortY;
		PLCore::uint32			 m_nViewPortWidth;
		PLCore::uint32			 m_nViewPortHeight;
		float				 	m_fViewPortMinZ;
		float					 m_fViewPortMaxZ;
		PLCore::uint32			 m_nScissorRectX;
		PLCore::uint32			 m_nScissorRectY;
		PLCore::uint32			 m_nScissorRectWidth;
		PLCore::uint32			 m_nScissorRectHeight;
		float					 m_fDepthBoundsZMin;
		float					 m_fDepthBoundsZMax;


	//[-------------------------------------------------------]
	//[ Public virtual PLRenderer::Renderer functions         ]
	//[-------------------------------------------------------]
	public:
		virtual bool IsInitialized() const override;
		virtual PLCore::String GetAPI(PLCore::uint32 *pnVersion = nullptr) const override;
		virtual PLCore::String GetVendor() const override;
		virtual PLCore::String GetDefaultShaderLanguage() const override;
		virtual PLRenderer::ShaderLanguage *GetShaderLanguage(const PLCore::String &sShaderLanguage = "") override;
		virtual PLRenderer::FixedFunctions *GetFixedFunctions() const override;
		virtual PLRenderer::FontManager &GetFontManager() const override;
		virtual void BackupDeviceObjects() override;
		virtual void RestoreDeviceObjects() override;

		//[-------------------------------------------------------]
		//[ Create surface/resource                               ]
		//[-------------------------------------------------------]
		virtual PLRenderer::SurfaceWindow *CreateSurfaceWindow(PLRenderer::SurfaceWindowHandler &cHandler, PLCore::handle nNativeWindowHandle, const PLRenderer::DisplayMode &sDisplayMode, bool bFullscreen = false) override;
		virtual PLRenderer::SurfaceTextureBuffer *CreateSurfaceTextureBuffer2D(const PLMath::Vector2i &vSize, PLRenderer::TextureBuffer::EPixelFormat nFormat, PLCore::uint32 nFlags = PLRenderer::SurfaceTextureBuffer::Depth | PLRenderer::SurfaceTextureBuffer::Stencil, PLCore::uint8 nMaxColorTargets = 1) override;
		virtual PLRenderer::SurfaceTextureBuffer *CreateSurfaceTextureBufferRectangle(const PLMath::Vector2i &vSize, PLRenderer::TextureBuffer::EPixelFormat nFormat, PLCore::uint32 nFlags = PLRenderer::SurfaceTextureBuffer::Depth | PLRenderer::SurfaceTextureBuffer::Stencil, PLCore::uint8 nMaxColorTargets = 1) override;
		virtual PLRenderer::SurfaceTextureBuffer *CreateSurfaceTextureBufferCube(PLCore::uint16 nSize, PLRenderer::TextureBuffer::EPixelFormat nFormat, PLCore::uint32 nFlags = PLRenderer::SurfaceTextureBuffer::Depth | PLRenderer::SurfaceTextureBuffer::Stencil) override;
		virtual PLRenderer::TextureBuffer1D *CreateTextureBuffer1D(PLGraphics::Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat = PLRenderer::TextureBuffer::Unknown, PLCore::uint32 nFlags = PLRenderer::TextureBuffer::Mipmaps) override;
		virtual PLRenderer::TextureBuffer2D *CreateTextureBuffer2D(PLGraphics::Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat = PLRenderer::TextureBuffer::Unknown, PLCore::uint32 nFlags = PLRenderer::TextureBuffer::Mipmaps) override;
		virtual PLRenderer::TextureBuffer2DArray *CreateTextureBuffer2DArray(PLGraphics::Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat = PLRenderer::TextureBuffer::Unknown, PLCore::uint32 nFlags = PLRenderer::TextureBuffer::Mipmaps) override;
		virtual PLRenderer::TextureBuffer *CreateTextureBufferRectangle(PLGraphics::Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat = PLRenderer::TextureBuffer::Unknown, PLCore::uint32 nFlags = 0) override;
		virtual PLRenderer::TextureBuffer3D *CreateTextureBuffer3D(PLGraphics::Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat = PLRenderer::TextureBuffer::Unknown, PLCore::uint32 nFlags = PLRenderer::TextureBuffer::Mipmaps) override;
		virtual PLRenderer::TextureBufferCube *CreateTextureBufferCube(PLGraphics::Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat = PLRenderer::TextureBuffer::Unknown, PLCore::uint32 nFlags = PLRenderer::TextureBuffer::Mipmaps) override;
		virtual PLRenderer::IndexBuffer *CreateIndexBuffer() override;
		virtual PLRenderer::VertexBuffer *CreateVertexBuffer() override;
		virtual PLRenderer::OcclusionQuery *CreateOcclusionQuery() override;

		//[-------------------------------------------------------]
		//[ States                                                ]
		//[-------------------------------------------------------]
		virtual bool SetRenderState(PLRenderer::RenderState::Enum nState, PLCore::uint32 nValue) override;
		virtual bool SetSamplerState(PLCore::uint32 nStage, PLRenderer::Sampler::Enum nState, PLCore::uint32 nValue) override;

		//[-------------------------------------------------------]
		//[ Misc                                                  ]
		//[-------------------------------------------------------]
		virtual bool BeginScene() override;
		virtual bool EndScene() override;
		virtual bool GetDepthBounds(float &fZMin, float &fZMax) const override;
		virtual bool SetDepthBounds(float fZMin = 0.0f, float fZMax = 1.0f) override;
		virtual void GetColorMask(bool &bRed, bool &bGreen, bool &bBlue, bool &bAlpha) const override;
		virtual bool SetColorMask(bool bRed = true, bool bGreen = true, bool bBlue = true, bool bAlpha = true) override;
		virtual bool Clear(PLCore::uint32 nFlags = PLRenderer::Clear::Color | PLRenderer::Clear::ZBuffer,
						   const PLGraphics::Color4 &cColor = PLGraphics::Color4::Black, float fZ = 1.0f, PLCore::uint32 nStencil = 0) override;

		//[-------------------------------------------------------]
		//[ Get/set current resources                             ]
		//[-------------------------------------------------------]
		virtual bool SetRenderTarget(PLRenderer::Surface *pSurface, PLCore::uint8 nFace = 0) override;
		virtual bool SetColorRenderTarget(PLRenderer::TextureBuffer *pTextureBuffer, PLCore::uint8 nColorIndex = 0, PLCore::uint8 nFace = 0) override;
		virtual bool SetDepthRenderTarget(PLRenderer::TextureBuffer *pTextureBuffer, PLCore::uint8 nFace = 0) override;
		virtual bool MakeScreenshot(PLGraphics::Image &cImage) override;
		virtual bool SetTextureBuffer(int nStage = -1, PLRenderer::TextureBuffer *pTextureBuffer = nullptr) override;
		virtual bool SetIndexBuffer(PLRenderer::IndexBuffer *pIndexBuffer = nullptr) override;

		//[-------------------------------------------------------]
		//[ Draw call                                             ]
		//[-------------------------------------------------------]
		virtual bool DrawPrimitives(PLRenderer::Primitive::Enum nType, PLCore::uint32 nStartIndex, PLCore::uint32 nNumVertices) override;
		virtual bool DrawIndexedPrimitives(PLRenderer::Primitive::Enum nType, PLCore::uint32 nMinIndex, PLCore::uint32 nMaxIndex, PLCore::uint32 nStartIndex, PLCore::uint32 nNumVertices) override;
		virtual bool DrawPatches(PLCore::uint32 nVerticesPerPatch, PLCore::uint32 nStartIndex, PLCore::uint32 nNumVertices) override;
		virtual bool DrawIndexedPatches(PLCore::uint32 nVerticesPerPatch, PLCore::uint32 nMinIndex, PLCore::uint32 nMaxIndex, PLCore::uint32 nStartIndex, PLCore::uint32 nNumVertices) override;

		//[-------------------------------------------------------]
		//[ Draw call with multiple primitive instances           ]
		//[-------------------------------------------------------]
		virtual bool DrawPrimitivesInstanced(PLRenderer::Primitive::Enum nType, PLCore::uint32 nStartIndex, PLCore::uint32 nNumVertices, PLCore::uint32 nNumOfInstances) override;
		virtual bool DrawIndexedPrimitivesInstanced(PLRenderer::Primitive::Enum nType, PLCore::uint32 nMinIndex, PLCore::uint32 nMaxIndex, PLCore::uint32 nStartIndex, PLCore::uint32 nNumVertices, PLCore::uint32 nNumOfInstances) override;
		virtual bool DrawPatchesInstanced(PLCore::uint32 nVerticesPerPatch, PLCore::uint32 nStartIndex, PLCore::uint32 nNumVertices, PLCore::uint32 nNumOfInstances) override;
		virtual bool DrawIndexedPatchesInstanced(PLCore::uint32 nVerticesPerPatch, PLCore::uint32 nMinIndex, PLCore::uint32 nMaxIndex, PLCore::uint32 nStartIndex, PLCore::uint32 nNumVertices, PLCore::uint32 nNumOfInstances) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererNull


#endif // __PLRENDERERNULL_RENDERER_H__
