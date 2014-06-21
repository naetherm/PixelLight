/*********************************************************\
 *  File: SRPDeferredLighting.h                          *
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


#ifndef __PLCOMPOSITING_DEFERRED_LIGHTING_H__
#define __PLCOMPOSITING_DEFERRED_LIGHTING_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Texture/TextureHandler.h>
#include <PLRenderer/Renderer/ProgramGenerator.h>
#include "PLCompositing/Shaders/Deferred/SRPDeferred.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class ProgramUniform;
	class ProgramAttribute;
}
namespace PLScene {
	class SNLight;
	class VisNode;
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
*    Scene renderer pass for deferred rendering lighting
*
*  @note
*    - SceneNode::ReceiveShadow is not supported
*/
class SRPDeferredLighting : public SRPDeferred {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Texture filtering modes
		*/
		enum ETextureFiltering {
			NoFiltering   = 0,	/**< No filtering */
			Bilinear      = 1,	/**< Bilinear filtering */
			Anisotropic2  = 2,	/**< Anisotropic x2 */
			Anisotropic4  = 4,	/**< Anisotropic x4 */
			Anisotropic8  = 8,	/**< Anisotropic x8 */
			Anisotropic16 = 16	/**< Anisotropic x16 */
		};
		pl_enum(ETextureFiltering)
			pl_enum_value(NoFiltering,		"No filtering")
			pl_enum_value(Bilinear,			"Bilinear")
			pl_enum_value(Anisotropic2,		"Anisotropic x2")
			pl_enum_value(Anisotropic4,		"Anisotropic x4")
			pl_enum_value(Anisotropic8,		"Anisotropic x8")
			pl_enum_value(Anisotropic16,	"Anisotropic x16")
		pl_enum_end

		/**
		*  @brief
		*    Scene renderer pass flags (SceneRendererPass flags extension)
		*/
		enum EFlags {
			NoShadow                = 1<<1,		/**< Do not render shadows */
			NoSoftShadow            = 1<<2,		/**< No soft shadows */
			NoShadowLOD             = 1<<3,		/**< Do not use level of detail for shadow maps, with LOD on, shadows far away will have a lower resolution */
			NoAlbedo                = 1<<4,		/**< Ignore albedo data */
			NoAmbientOcclusion      = 1<<5,		/**< Ignore ambient occlusion data */
			NoSpecular              = 1<<6,		/**< No specular */
			NoSpecularColor         = 1<<7,		/**< Ignore specular color data */
			NoSpecularExponent      = 1<<8,		/**< Ignore specular exponent data */
			NoProjectivePointLights = 1<<9,		/**< No projective point lights */
			NoProjectiveSpotLights  = 1<<10,	/**< No projective spot lights */
			NoDiscard               = 1<<11,	/**< Disable discard (may result in better or worse performance) */
			NoGammaCorrection       = 1<<12		/**< Do not perform gamma correction (for projective lights) - if gamma correction is enabled, there should be a gamma correction at the end of the render pipeline */
		};
		pl_enum(EFlags)
			pl_enum_base(SRPDeferred::EFlags)
			pl_enum_value(NoShadow,					"Do not render shadows")
			pl_enum_value(NoSoftShadow,				"No soft shadows")
			pl_enum_value(NoShadowLOD,				"Do not use level of detail for shadow maps, with LOD on, shadows far away will have a lower resolution")
			pl_enum_value(NoAlbedo,					"Ignore albedo data")
			pl_enum_value(NoAmbientOcclusion,		"Ignore ambient occlusion data")
			pl_enum_value(NoSpecular,				"No specular")
			pl_enum_value(NoSpecularColor,			"Ignore specular color data")
			pl_enum_value(NoSpecularExponent,		"Ignore specular exponent data")
			pl_enum_value(NoProjectivePointLights,	"No projective point lights")
			pl_enum_value(NoProjectiveSpotLights,	"No projective spot lights")
			pl_enum_value(NoDiscard,				"Disable discard (may result in better or worse performance)")
			pl_enum_value(NoGammaCorrection,		"Do not perform gamma correction (for projective lights) - if gamma correction is enabled, there should be a gamma correction at the end of the render pipeline")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SRPDeferredLighting, "PLCompositing", PLCompositing::SRPDeferred, "Scene renderer pass for deferred rendering lighting")
		// Attributes
		pl_attribute(ShaderLanguage,	PLCore::String,						"",								ReadWrite,	DirectValue,	"Shader language to use (for example \"GLSL\" or \"Cg\"), if empty string, the default shader language of the renderer will be used",	"")
		pl_attribute(TextureFiltering,	pl_enum_type(ETextureFiltering),	Anisotropic8,					ReadWrite,	DirectValue,	"Texture filtering, used for projective lights",																						"")
			// Overwritten PLScene::SceneRendererPass attributes
		pl_attribute(Flags,				pl_flag_type(EFlags),				NoShadowLOD|NoAmbientOcclusion,	ReadWrite,	GetSet,			"Flags",																																"")
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
		PLCOM_API SRPDeferredLighting();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SRPDeferredLighting();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Sets correct texture filtering modes
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] nStage
		*    Texture stage
		*  @param[in] nTextureFiltering
		*    Texture filtering mode to use
		*/
		void SetupTextureFiltering(PLRenderer::Renderer &cRenderer, PLCore::uint32 nStage, PLCore::uint32 nTextureFiltering) const;

		/**
		*  @brief
		*    Draws recursive
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cCullQuery
		*    Cull query to use
		*  @param[in] cSRPDeferredGBuffer
		*    "SRPDeferredGBuffer" scene renderer pass to use
		*/
		void DrawRec(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery, SRPDeferredGBuffer &cSRPDeferredGBuffer);

		/**
		*  @brief
		*    Renders a light
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cCullQuery
		*    Cull query to use
		*  @param[in] cSRPDeferredGBuffer
		*    "SRPDeferredGBuffer" scene renderer pass to use
		*  @param[in] cLight
		*    Used light node
		*  @param[in] cLightVisNode
		*    The light visibility node
		*/
		void RenderLight(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery, SRPDeferredGBuffer &cSRPDeferredGBuffer, PLScene::SNLight &cLight, const PLScene::VisNode &cLightVisNode);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Fragment shader flags, flag names become to source code definitions
		*/
		enum EFragmentShaderFlags {
			FS_DIRECTIONAL				= 1<<0,		/**< Directional light */
			FS_PROJECTIVE_POINT			= 1<<1,		/**< Projective point light */
			FS_SPOT						= 1<<2,		/**< Spot light */
				FS_PROJECTIVE_SPOT		= 1<<3,		/**< Projective spot light (FS_SPOT must be set, too) */
				FS_SPOT_CONE			= 1<<4,		/**< Spot light with a cone (FS_SPOT must be set, too) */
					FS_SPOT_SMOOTHCONE	= 1<<5,		/**< Spot light with a smooth cone (FS_SPOT & FS_SPOT_CONE must be set, too) */
			FS_SHADOWMAPPING			= 1<<6,		/**< Perform shadow mapping */
				FS_SOFTSHADOWMAPPING	= 1<<7,		/**< Perform soft shadow mapping (FS_SHADOWMAPPING must be set, too) */
			FS_NO_ALBEDO				= 1<<8,		/**< Ignore albedo data */
			FS_NO_AMBIENTOCCLUSION		= 1<<9,		/**< Ignore ambient occlusion data */
			FS_NO_SPECULAR				= 1<<10,	/**< No specular */
			FS_NO_SPECULARCOLOR			= 1<<11,	/**< Ignore specular color data */
			FS_NO_SPECULAREXPONENT		= 1<<12,	/**< Ignore specular exponent data */
			FS_DISCARD					= 1<<13,	/**< Use discard */
			FS_GAMMACORRECTION			= 1<<14		/**< Use gamma correction (sRGB to linear space) */
		};

		/**
		*  @brief
		*    Direct pointers to uniforms & attributes of a generated program
		*/
		struct GeneratedProgramUserData {
			// Vertex shader attributes
			PLRenderer::ProgramAttribute *pVertexPosition;
			PLRenderer::ProgramAttribute *pVertexTexCoord0;
			// Vertex shader uniforms
			PLRenderer::ProgramUniform *pTextureSize;
			// Fragment shader uniforms
			PLRenderer::ProgramUniform *pLightDirection;
			PLRenderer::ProgramUniform *pLightPosition;
			PLRenderer::ProgramUniform *pLightRadius;
			PLRenderer::ProgramUniform *pProjectivePointCubeMap;
			PLRenderer::ProgramUniform *pViewSpaceToCubeMapSpace;
			PLRenderer::ProgramUniform *pProjectiveSpotMap;
			PLRenderer::ProgramUniform *pViewSpaceToSpotMapSpace;
			PLRenderer::ProgramUniform *pSpotConeCos;
			PLRenderer::ProgramUniform *pShadowMap;
			PLRenderer::ProgramUniform *pViewSpaceToShadowMapSpace;
			PLRenderer::ProgramUniform *pViewSpaceToShadowCubeMapSpace;
			PLRenderer::ProgramUniform *pInvLightRadius;
			PLRenderer::ProgramUniform *pTexelSize;
			PLRenderer::ProgramUniform *pLightColor;
			PLRenderer::ProgramUniform *pInvFocalLen;
			PLRenderer::ProgramUniform *pRenderTargetTexture0;
			PLRenderer::ProgramUniform *pRenderTargetTexture1;
			PLRenderer::ProgramUniform *pRenderTargetTexture2;
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLRenderer::ProgramGenerator		*m_pProgramGenerator;	/**< Program generator, can be a null pointer */
		PLRenderer::ProgramGenerator::Flags	 m_cProgramFlags;		/**< Program flags as class member to reduce dynamic memory allocations */
		PLRenderer::TextureHandler			 m_cSpotMapHandler;		/**< Texture handler for the default spot map */
		PLRenderer::TextureHandler			 m_cCubeMapHandler;		/**< Texture handler for the default cube map */


	//[-------------------------------------------------------]
	//[ Private virtual PLScene::SceneRendererPass functions  ]
	//[-------------------------------------------------------]
	private:
		virtual void Draw(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLCOMPOSITING_DEFERRED_LIGHTING_H__
