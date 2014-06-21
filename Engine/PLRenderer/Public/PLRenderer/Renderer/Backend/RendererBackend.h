/*********************************************************\
 *  File: RendererBackend.h                              *
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


#ifndef __PLRENDERERBACKEND_RENDERERBACKEND_H__
#define __PLRENDERERBACKEND_RENDERERBACKEND_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Frustum.h>
#include <PLMath/Rectangle.h>
#include "PLRenderer/Renderer/Renderer.h"
#include "PLRenderer/Renderer/ResourceHandler.h"
#include "PLRenderer/Renderer/SurfaceHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class DrawHelpersBackend;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Renderer backend with general stuff which all backends will need
*
*  @note
*    - All renderer backends must be derived from this class in order to have a common ground
*/
class RendererBackend : public Renderer {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLRENDERER_RTTI_EXPORT, RendererBackend, "PLRenderer", PLRenderer::Renderer, "Renderer backend with general stuff which all backends will need")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the writable renderer statistics for internal usage
		*
		*  @return
		*    The writable renderer statistics
		*/
		inline Statistics &GetWritableStatistics();

		/**
		*  @brief
		*    Makes a surface to the renderers current render target
		*
		*  @param[in] cSurface
		*    Surface to use
		*  @param[in] nFace
		*    Cube map face to render in (0-5) - only used if this is a cube texture buffer render target
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLRENDERER_API bool MakeSurfaceCurrent(Surface &cSurface, PLCore::uint8 nFace = 0);

		/**
		*  @brief
		*    Unmakes a surface from the renderers current render target
		*
		*  @param[in] cSurface
		*    Surface to use
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLRENDERER_API bool UnmakeSurfaceCurrent(Surface &cSurface);

		/**
		*  @brief
		*    Makes a texture buffer to the renderers current texture buffer
		*
		*  @param[in] cTextureBuffer
		*    Texture buffer to use
		*  @param[in] nStage
		*    Texture stage
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLRENDERER_API bool MakeTextureBufferCurrent(TextureBuffer &cTextureBuffer, PLCore::uint32 nStage);

		/**
		*  @brief
		*    Returns the API pixel format
		*
		*  @param[in] nPixelFormat
		*    PixelLight pixel format to return the API pixel format from
		*
		*  @return
		*    The API pixel format of the given PixelLight pixel format, a null pointer on error
		*/
		inline PLCore::uint32 *GetAPIPixelFormat(TextureBuffer::EPixelFormat nPixelFormat) const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nMode
		*    Mode hint the renderer should run in, the renderer is not enforced to use this requested mode
		*/
		PLRENDERER_API RendererBackend(EMode nMode);

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~RendererBackend();

		/**
		*  @brief
		*    Writes the renderer capabilities into the log
		*/
		PLRENDERER_API void ShowRendererCapabilities() const;

		/**
		*  @brief
		*    Checks a 1D texture buffer
		*
		*  @param[in] cImage
		*    Texture buffer image
		*  @param[in] nInternalFormat
		*    Desired internal texture buffer pixel format, if TextureBuffer::Unknown use the format of the given image
		*
		*  @return
		*    'true' if the texture buffer is valid, else 'false'
		*/
		PLRENDERER_API bool CheckTextureBuffer1D(PLGraphics::Image &cImage, TextureBuffer::EPixelFormat nInternalFormat = TextureBuffer::Unknown) const;

		/**
		*  @brief
		*    Checks a 2D texture buffer
		*
		*  @param[in] cImage
		*    Texture buffer image
		*  @param[in] nInternalFormat
		*    Desired internal texture buffer pixel format, if TextureBuffer::Unknown use the format of the given image
		*
		*  @return
		*    'true' if the texture buffer is valid, else 'false'
		*/
		PLRENDERER_API bool CheckTextureBuffer2D(PLGraphics::Image &cImage, TextureBuffer::EPixelFormat nInternalFormat = TextureBuffer::Unknown) const;

		/**
		*  @brief
		*    Checks a 2D array texture buffer
		*
		*  @param[in] cImage
		*    Texture buffer image
		*  @param[in] nInternalFormat
		*    Desired internal texture buffer pixel format, if TextureBuffer::Unknown use the format of the given image
		*
		*  @return
		*    'true' if the texture buffer is valid, else 'false'
		*/
		PLRENDERER_API bool CheckTextureBuffer2DArray(PLGraphics::Image &cImage, TextureBuffer::EPixelFormat nInternalFormat = TextureBuffer::Unknown) const;

		/**
		*  @brief
		*    Checks a rectangle texture buffer
		*
		*  @param[in] cImage
		*    Texture buffer image
		*  @param[in] nInternalFormat
		*    Desired internal texture buffer pixel format, if TextureBuffer::Unknown use the format of the given image
		*
		*  @return
		*    'true' if the texture buffer is valid, else 'false'
		*/
		PLRENDERER_API bool CheckTextureBufferRectangle(PLGraphics::Image &cImage, TextureBuffer::EPixelFormat nInternalFormat = TextureBuffer::Unknown) const;

		/**
		*  @brief
		*    Checks a 3D texture buffer
		*
		*  @param[in] cImage
		*    Texture buffer image
		*  @param[in] nInternalFormat
		*    Desired internal texture buffer pixel format, if TextureBuffer::Unknown use the format of the given image
		*
		*  @return
		*    'true' if the texture buffer is valid, else 'false'
		*/
		PLRENDERER_API bool CheckTextureBuffer3D(PLGraphics::Image &cImage, TextureBuffer::EPixelFormat nInternalFormat = TextureBuffer::Unknown) const;

		/**
		*  @brief
		*    Checks a cube texture buffer
		*
		*  @param[in] cImage
		*    Cube map image (image with 6 image parts)
		*  @param[in] nInternalFormat
		*    Desired internal texture buffer pixel format, if TextureBuffer::Unknown use the format of the given image
		*
		*  @return
		*    'true' if the texture buffer is valid, else 'false'
		*/
		PLRENDERER_API bool CheckTextureBufferCube(PLGraphics::Image &cImage, TextureBuffer::EPixelFormat nInternalFormat = TextureBuffer::Unknown) const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		DrawHelpersBackend  *m_pDrawHelpers;	/**< Draw helpers instance, always valid! */
		Capabilities		 m_sCapabilities;	/**< The renderer capabilities */
		Statistics			 m_sStatistics;		/**< Renderer statistics */

		// Display modes
		PLCore::Array<const DisplayMode*> m_lstDisplayModeList;	/**< A list of all found display modes */
		DisplayMode						  m_sDisplayModeBackup;	/**< The previous display mode */
		DisplayMode						  m_sDisplayMode;		/**< The current display mode */

		// Renderer surfaces of this renderer
		PLCore::Array<Surface*> m_lstSurfaces;

		// Renderer resources of this renderer
		PLCore::Array<Resource*> m_lstResources;

		// All this wrappers will map PL renderer values to API dependent values.
		// E.e. Compare::Less will return GL_LESS
		PLCore::Array<PLCore::uint32> m_cPLE_FILLWrapper;	/**< Fill */
		PLCore::Array<PLCore::uint32> m_cPLE_CULLWrapper;	/**< Cull */
		PLCore::Array<PLCore::uint32> m_cPLE_BLENDWrapper;	/**< BlendFunc */
		PLCore::Array<PLCore::uint32> m_cPLE_CMPWrapper;	/**< Compare */
		PLCore::Array<PLCore::uint32> m_cPLE_SOPWrapper;	/**< StencilOp */
		PLCore::Array<PLCore::uint32> m_cPLE_PTWrapper;		/**< Primitive */
		PLCore::Array<PLCore::uint32> m_cPLE_TAWrapper;		/**< Texture address mode */
		PLCore::Array<PLCore::uint32> m_cPLE_TPFWrapper;	/**< TextureBuffer::EPixelFormat */

		// Misc
		PLCore::uint32		m_nSwapInterval;	/**< Swap interval */
		PLMath::Rectangle	m_cViewportRect;	/**< The current viewport */
		float				m_fViewPortMinZ;	/**< Viewport min z */
		float				m_fViewPortMaxZ;	/**< Viewport max z */
		PLMath::Rectangle	m_cScissorRect;		/**< The current scissor rectangle */
		PLMath::Frustum		m_cFrustum;			/**< The current frustum */
		bool				m_bColorMask[4];	/**< Color mask (RGBA) */
		PLCore::uint8	  **m_ppDataBackup;		/**< Data backup, used inside BackupDeviceObjects()/RestoreDeviceObjects() */

		// States
		PLCore::uint32   m_nRenderState[RenderState::Number];			/**< List of render states (see RenderState) */
		PLCore::uint32 **m_ppnSamplerState;								/**< List of sampler states for each stage (see Sampler) */
		PLCore::uint32 **m_ppnInternalSamplerState;						/**< List of internal sampler states for each stage (see Sampler) */
		PLCore::uint32   m_nDefaultRenderState[RenderState::Number];	/**< Default renderer states */
		PLCore::uint32   m_nDefaultSamplerState[Sampler::Number];		/**< Default sampler states */

		// Current stuff
		SurfaceHandler		m_cCurrentSurface;			/**< The current renderer target surface */
		PLCore::uint8		m_nCurrentSurfaceFace;		/**< The current renderer target surface face */
		TextureBuffer	  **m_ppCurrentTextureBuffer;	/**< The current texture buffer */
		IndexBuffer		   *m_pCurrentIndexBuffer;		/**< Current index buffer */
		ResourceHandler     m_cProgramHandler;			/**< Currently used program */


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		EMode				 m_nMode;				/**< The mode the renderer is running in */
		RendererContext     *m_pRendererContext;	/**< Owner renderer context of this renderer, always valid! */


	//[-------------------------------------------------------]
	//[ Public virtual Renderer functions                     ]
	//[-------------------------------------------------------]
	public:
		PLRENDERER_API virtual RendererContext &GetRendererContext() const override;
		PLRENDERER_API virtual EMode GetMode() const override;
		PLRENDERER_API virtual DrawHelpers &GetDrawHelpers() const override;
		PLRENDERER_API virtual void BackupDeviceObjects() override;
		PLRENDERER_API virtual void RestoreDeviceObjects() override;
		PLRENDERER_API virtual PLCore::uint32 GetNumOfDisplayModes() const override;
		PLRENDERER_API virtual const DisplayMode *GetDisplayMode(PLCore::uint32 nIndex) const override;
		PLRENDERER_API virtual const Capabilities &GetCapabilities() const override;
		PLRENDERER_API virtual bool IsValidTextureBuffer1DSize(int nSize) const override;
		PLRENDERER_API virtual bool IsValidTextureBuffer2DSize(int nSize) const override;
		PLRENDERER_API virtual bool IsValidTextureBufferRectangleSize(int nSize) const override;
		PLRENDERER_API virtual bool IsValidTextureBuffer3DSize(int nSize) const override;
		PLRENDERER_API virtual bool IsValidTextureBufferCubeSize(int nSize) const override;
		PLRENDERER_API virtual const Statistics &GetStatistics() const override;
		PLRENDERER_API virtual const PLMath::Vector2 &GetTexelToPixelOffset() const override;
		PLRENDERER_API virtual void Update() override;
		PLRENDERER_API virtual void Reset() override;

		//[-------------------------------------------------------]
		//[ Surfaces                                              ]
		//[-------------------------------------------------------]
		PLRENDERER_API virtual PLCore::uint32 GetNumOfSurfaces() const override;
		PLRENDERER_API virtual Surface *GetSurface(PLCore::uint32 nIndex = 0) const override;
		PLRENDERER_API virtual bool AddSurface(Surface &cSurface) override;
		PLRENDERER_API virtual bool RemoveSurface(Surface &cSurface) override;
		PLRENDERER_API virtual SurfacePainter *CreateSurfacePainter(const PLCore::String &sClass) override;

		//[-------------------------------------------------------]
		//[ Resources                                             ]
		//[-------------------------------------------------------]
		PLRENDERER_API virtual PLCore::uint32 GetNumOfResources() const override;
		PLRENDERER_API virtual Resource *GetResource(PLCore::uint32 nIndex = 0) const override;
		PLRENDERER_API virtual bool AddResource(Resource &cResource) override;
		PLRENDERER_API virtual bool RemoveResource(Resource &cResource) override;

		//[-------------------------------------------------------]
		//[ States                                                ]
		//[-------------------------------------------------------]
		// Render states
		PLRENDERER_API virtual PLCore::uint32 GetDefaultRenderState(RenderState::Enum nState) const override;
		PLRENDERER_API virtual void ResetRenderStates() override;
		PLRENDERER_API virtual int GetRenderState(RenderState::Enum nState) const override;
		// Sampler states
		PLRENDERER_API virtual PLCore::uint32 GetDefaultSamplerState(Sampler::Enum nState) const override;
		PLRENDERER_API virtual void ResetSamplerStates() override;
		PLRENDERER_API virtual int GetSamplerState(PLCore::uint32 nStage, Sampler::Enum nState) const override;

		//[-------------------------------------------------------]
		//[ Misc                                                  ]
		//[-------------------------------------------------------]
		PLRENDERER_API virtual PLCore::uint32 GetSwapInterval() const override;
		PLRENDERER_API virtual void SetSwapInterval(PLCore::uint32 nSwapInterval = 1) override;
		PLRENDERER_API virtual const PLMath::Rectangle &GetViewport(float *pfMinZ = nullptr, float *pfMaxZ = nullptr) const override;
		PLRENDERER_API virtual bool SetViewport(const PLMath::Rectangle *pRectangle = nullptr, float fMinZ = 0.0f, float fMaxZ = 1.0f) override;
		PLRENDERER_API virtual const PLMath::Rectangle &GetScissorRect() const override;
		PLRENDERER_API virtual bool SetScissorRect(const PLMath::Rectangle *pRectangle = nullptr) override;
	
		//[-------------------------------------------------------]
		//[ Get/set current resources                             ]
		//[-------------------------------------------------------]
		PLRENDERER_API virtual Surface *GetRenderTarget(PLCore::uint8 *pnFace = nullptr) const override;
		PLRENDERER_API virtual TextureBuffer *GetColorRenderTarget(PLCore::uint8 nColorIndex = 0) const override;
		PLRENDERER_API virtual TextureBuffer *GetTextureBuffer(PLCore::uint32 nStage) const override;
		PLRENDERER_API virtual IndexBuffer *GetIndexBuffer() const override;
		PLRENDERER_API virtual Program *GetProgram() const override;
		PLRENDERER_API virtual bool SetProgram(Program *pProgram = nullptr) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/Backend/RendererBackend.inl"


#endif // __PLRENDERERBACKEND_RENDERERBACKEND_H__
