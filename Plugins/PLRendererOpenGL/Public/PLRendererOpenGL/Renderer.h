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


#ifndef __PLRENDEREROPENGL_RENDERER_H__
#define __PLRENDEREROPENGL_RENDERER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/Backend/RendererBackend.h>
#include "PLRendererOpenGL/PLRendererOpenGL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Context;
class FixedFunctions;
class ShaderLanguage;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    OpenGL 1.1 + extensions renderer backend
*/
class Renderer : public PLRenderer::RendererBackend {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class FixedFunctions;
	friend class OpenGLRenderContext;


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLRENDEREROPENGL_RTTI_EXPORT, Renderer, "PLRendererOpenGL", PLRenderer::RendererBackend, "OpenGL 1.1 + extensions renderer backend")
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
		PLRENDEREROPENGL_API Renderer(PLCore::handle nNativeWindowHandle, EMode nMode, PLCore::uint32 nZBufferBits, PLCore::uint32 nStencilBits, PLCore::uint32 nMultisampleAntialiasingSamples, PLCore::String sDefaultShaderLanguage);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Renderer();

		/**
		*  @brief
		*    Returns the OpenGL render context
		*
		*  @return
		*    The OpenGL render context
		*/
		Context &GetContext() const;

		/**
		*  @brief
		*    Returns the used multisample antialiasing samples per pixel
		*
		*  @return
		*    The used multisample antialiasing samples per pixel
		*/
		PLCore::uint32 GetMultisampleAntialiasingSamples() const;

		/**
		*  @brief
		*    Returns a PL format as OpenGL format
		*
		*  @param[in] nFormat
		*    PL format to convert
		*
		*  @return
		*    The converted OpenGL format (like GL_LUMINANCE, GL_RGB etc.)
		*/
		PLCore::uint32 GetOpenGLPixelFormat(PLRenderer::TextureBuffer::EPixelFormat nFormat) const;

		/**
		*  @brief
		*    Returns a PL data format as OpenGL data format
		*
		*  @param[in] nFormat
		*    PL data format to convert
		*
		*  @return
		*    The converted data format (like GL_UNSIGNED_UINT, GL_UNSIGNED_SHORT etc.)
		*/
		PLCore::uint32 GetOpenGLDataFormat(PLRenderer::TextureBuffer::EPixelFormat nFormat) const;

		/**
		*  @brief
		*    Chooses the image source and internal texture buffer formats depending on various input parameters
		*
		*  @param[in]  cImage
		*    Image to use
		*  @param[in]  nInternalFormat
		*    Desired internal texture buffer pixel format, if 'PLRenderer::TextureBuffer::Unknown' use the format of the given image
		*  @param[in]  nFlags
		*    Texture buffer flags (see PLRenderer::TextureBuffer::EFlags)
		*  @param[out] nImageFormat
		*    The chosen image format
		*  @param[out] bUsePreCompressedData
		*    'true' if the pre compressed image data should be used, else 'false'
		*
		*  @return
		*    The chosen internal format
		*/
		PLRenderer::TextureBuffer::EPixelFormat ChooseFormats(PLGraphics::Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat, PLCore::uint32 nFlags,
															  PLRenderer::TextureBuffer::EPixelFormat &nImageFormat, bool &bUsePreCompressedData) const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Creates a OpenGL render context instance
		*
		*  @return
		*    The created a OpenGL render context instance, a null pointer on error
		*
		*  @note
		*    - The new render context will be automatically set as the current active one
		*/
		Context *CreateContext();

		/**
		*  @brief
		*    Initializes all wrappers
		*/
		void InitWrappers();

		/**
		*  @brief
		*    Setup the renderer capabilities
		*/
		void SetupCapabilities();

		/**
		*  @brief
		*    General OpenGL settings
		*/
		void GeneralSettings();

		/**
		*  @brief
		*    Sampler state filter mode helper function
		*
		*  @param[in] nStage
		*    Sampler stage
		*
		*  @return
		*    OpenGL sampler filter mode
		*/
		GLuint GetCombinedMinMipFilter(PLCore::uint32 nStage);

		/**
		*  @brief
		*    Restores the device states
		*/
		void RestoreDeviceStates();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool												   m_bInitialized;						/**< 'true' if the renderer instance was initialized successfully within it's constructor, else 'false' */
		Context												  *m_pContext;							/**< OpenGL context, always valid! (except when initialization failed, but in this case the renderer instance has to be destroyed without using it) */
		FixedFunctions										  *m_pFixedFunctions;					/**< Fixed functions interface implementation, can be a null pointer */
		PLRenderer::FontManager								  *m_pFontManager;						/**< OpenGL renderer font manager, always valid! */
		bool												   m_bSceneRendering;					/**< Is the scene rendering currently active? (see BeginScene/EndScene) */
		PLCore::uint32										   m_nMultisampleAntialiasingSamples;	/**< Multisample antialiasing samples per pixel */
		PLCore::String										   m_sDefaultShaderLanguage;			/**< Name of the default shader language to use */
		PLCore::List<const PLCore::Class*>					   m_lstShaderLanguages;				/**< List of available shader language classes */
		PLCore::HashMap<PLCore::String, const PLCore::Class*>  m_mapShaderLanguages;				/**< Map of available shader language classes "<Name> => <Class>" */
		PLCore::Array<ShaderLanguage*>						   m_lstShaderLanguageInstances;		/**< List of available shader language instances */
		PLCore::HashMap<PLCore::String, ShaderLanguage*>	   m_mapShaderLanguageInstances;		/**< Map of available shader language instances "<Name> => <Instance>" */

		/** OpenGL texture type at the certain texture stages */
		PLCore::uint32 *m_nTextureBufferTypes;

		PLRenderer::TextureBuffer **m_ppPrevTextureBuffer;	/**< The previous non a null pointer texture buffer */


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
		virtual bool SetViewport(const PLMath::Rectangle *pRectangle = nullptr, float fMinZ = 0.0f, float fMaxZ = 1.0f) override;
		virtual bool SetScissorRect(const PLMath::Rectangle *pRectangle = nullptr) override;
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

		// [TODO] Clean this up!
		PLRENDEREROPENGL_API bool SetShaderProgramTextureBuffer(int nStage = -1, PLRenderer::TextureBuffer *pTextureBuffer = nullptr);

		virtual bool SetIndexBuffer(PLRenderer::IndexBuffer *pIndexBuffer = nullptr) override;
		virtual bool SetProgram(PLRenderer::Program *pProgram = nullptr) override;

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
} // PLRendererOpenGL


#endif // __PLRENDEREROPENGL_RENDERER_H__
