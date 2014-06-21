/*********************************************************\
 *  File: SCRenderToTexture.h                            *
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


#ifndef __PLSCENE_SCENENODE_RENDERTOTEXTURE_H__
#define __PLSCENE_SCENENODE_RENDERTOTEXTURE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLScene/Scene/SceneContainer.h"
#include "PLScene/Scene/SceneNodeHandler.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class Texture;
	class TextureHandler;
	class SurfaceTextureBuffer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Render to texture scene container
*
*  @remarks
*    This scene container can render it's sub-scene or another linked scene using a given
*    camera into a texture. If you want to render the scene into a cube texture, set the
*    variable 'Cube' to 'true'. In this case, only the position of the given camera is used.
*    This for instance can be used to render correct dynamic reflections.
*
*  @note
*    - For 2D textures, non-power-of-two dimensions are supported
*    - If a camera scene node is used and this node is inactive, the texture is not updated
*/
class SCRenderToTexture : public SceneContainer {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node debug flags (SceneNode debug flags extension)
		*/
		enum EDebugFlags {
			DebugNoTexturePreview = 1<<8	/**< Do not draw the texture preview */
		};
		pl_enum(EDebugFlags)
			pl_enum_base(SceneContainer::EDebugFlags)
			pl_enum_value(DebugNoTexturePreview, "Do not draw the texture preview")
		pl_enum_end

		/**
		*  @brief
		*    Surface formats
		*/
		enum EFormat {
			R8G8B8   = 0,	/**< 8 bits per red, green and blue component */
			R8G8B8A8 = 1	/**< 8 bits per red, green, blue and alpha component */
		};
		pl_enum(EFormat)
			pl_enum_value(R8G8B8,	"8 bits per red, green and blue component")
			pl_enum_value(R8G8B8A8,	"8 bits per red, green, blue and alpha component")
		pl_enum_end

		/**
		*  @brief
		*    Surface flags
		*/
		enum ESurfaceFlags {
			Depth   = 1<<0,	/**< Surface has a depth buffer */
			Stencil = 1<<1,	/**< Surface has a stencil buffer */
			Mipmaps = 1<<2	/**< Surface has mipmaps */
		};
		pl_enum(ESurfaceFlags)
			pl_enum_value(Depth,	"Surface has a depth buffer")
			pl_enum_value(Stencil,	"Surface has a stencil buffer")
			pl_enum_value(Mipmaps,	"Surface has mipmaps")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SCRenderToTexture, "PLScene", PLScene::SceneContainer, "Render to texture scene container")
		// Attributes
		pl_attribute(Cube,			bool,							false,					ReadWrite,	GetSet,			"Render to cube texture? If 'true', only 'Width' is used which must be a power of two",	"")
		pl_attribute(Width,			PLCore::uint16,					512,					ReadWrite,	GetSet,			"Texture width",																		"Min='16'")
		pl_attribute(Height,		PLCore::uint16,					512,					ReadWrite,	GetSet,			"Texture height",																		"Min='16'")
		pl_attribute(Format,		pl_enum_type(EFormat),			R8G8B8A8,				ReadWrite,	GetSet,			"Texture format",																		"")
		pl_attribute(SurfaceFlags,	pl_flag_type(ESurfaceFlags),	Depth|Mipmaps,			ReadWrite,	GetSet,			"Texture surface flags",																"")
		pl_attribute(TextureName,	PLCore::String,					"",						ReadWrite,	GetSet,			"Name of the resulting texture",														"")
		pl_attribute(FPSLimit,		float,							30.0f,					ReadWrite,	DirectValue,	"Frames per second limitation, if 0, there's no limitation",							"Min='0.0'")
		pl_attribute(Painter,		PLCore::String,					"PLScene::SPScene",	ReadWrite,	GetSet,			"Name of the used surface painter",														"")
		pl_attribute(SceneRenderer,	PLCore::String,					"Forward.sr",			ReadWrite,	GetSet,			"Name of the used scene renderer, only used if the painter is derived of 'SPScene'",	"")
		pl_attribute(SceneName,		PLCore::String,					"Parent",				ReadWrite,	GetSet,			"Name of the scene to renderer, only used if the painter is derived of 'SPScene'",		"")
		pl_attribute(CameraName,	PLCore::String,					"",						ReadWrite,	DirectValue,	"Name of the camera to use, only used if the painter is derived of 'SPScene'",			"")
			// Overwritten SceneNode attributes
		pl_attribute(Flags,			pl_flag_type(EFlags),			NoCulling|NoRecursion,	ReadWrite,	GetSet,			"Flags",																				"")
		pl_attribute(DebugFlags,	pl_flag_type(EDebugFlags),		0,						ReadWrite,	GetSet,			"Debug flags",																			"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLS_API bool GetCube() const;
		PLS_API void SetCube(bool bValue);
		PLS_API PLCore::uint16 GetWidth() const;
		PLS_API void SetWidth(PLCore::uint16 nValue);
		PLS_API PLCore::uint16 GetHeight() const;
		PLS_API void SetHeight(PLCore::uint16 nValue);
		PLS_API EFormat GetFormat() const;
		PLS_API void SetFormat(EFormat nValue);
		PLS_API PLCore::uint32 GetSurfaceFlags() const;
		PLS_API void SetSurfaceFlags(PLCore::uint32 nValue);
		PLS_API PLCore::String GetTextureName() const;
		PLS_API void SetTextureName(const PLCore::String &sValue);
		PLS_API PLCore::String GetPainter() const;
		PLS_API void SetPainter(const PLCore::String &sValue);
		PLS_API PLCore::String GetSceneRenderer() const;
		PLS_API void SetSceneRenderer(const PLCore::String &sValue);
		PLS_API PLCore::String GetSceneName() const;
		PLS_API void SetSceneName(const PLCore::String &sValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SCRenderToTexture();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SCRenderToTexture();

		/**
		*  @brief
		*    Returns the texture buffer surface we render in
		*
		*  @return
		*   The texture buffer surface we render in, a null pointer on error
		*/
		PLS_API PLRenderer::SurfaceTextureBuffer *GetSurfaceTextureBuffer() const;

		/**
		*  @brief
		*    Returns the resulting texture
		*
		*  @return
		*   The resulting texture, a null pointer on error
		*/
		PLS_API PLRenderer::Texture *GetTexture() const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		bool							  m_bCube;					/**< Render to cube texture? If 'true', only 'Width' is used which must be a power of two */
		PLCore::uint16					  m_nWidth;					/**< Texture width */
		PLCore::uint16					  m_nHeight;				/**< Texture height */
		EFormat							  m_nFormat;				/**< Texture format */
		PLCore::uint32					  m_nSurfaceFlags;			/**< Texture surface flags */
		PLCore::String					  m_sTextureName;			/**< Name of the resulting texture */
		PLCore::String					  m_sPainter;				/**< Name of the used surface painter */
		PLCore::String					  m_sSceneRenderer;			/**< Name of the used scene renderer, only used if the painter is derived of 'SPScene' */
		PLCore::String					  m_sSceneName;				/**< Name of the scene to renderer, only used if the painter is derived of 'SPScene' */
		PLCore::uint64					  m_nFPSLimitLastTime;		/**< Time when the last update was done */
		PLRenderer::SurfaceTextureBuffer *m_pSurfaceTextureBuffer;	/**< Render to texture buffer, can be a null pointer */
		PLRenderer::TextureHandler		 *m_pTextureHandler;		/**< The resulting texture (ALWAYS valid!) */


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Creates/recreates the texture surface
		*/
		void CreateSurfaceTexture();


	//[-------------------------------------------------------]
	//[ Public virtual SceneNode functions                    ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual void DrawPre(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode = nullptr) override;
		PLS_API virtual void DrawDebug(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode = nullptr) override;


	//[-------------------------------------------------------]
	//[ Protected virtual SceneNode functions                 ]
	//[-------------------------------------------------------]
	protected:
		PLS_API virtual void InitFunction() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENENODE_RENDERTOTEXTURE_H__
