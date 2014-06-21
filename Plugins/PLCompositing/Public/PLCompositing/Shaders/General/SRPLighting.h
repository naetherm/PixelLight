/*********************************************************\
 *  File: SRPLighting.h                                  *
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


#ifndef __PLCOMPOSITING_GENERAL_LIGHTING_H__
#define __PLCOMPOSITING_GENERAL_LIGHTING_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/EventHandler.h>
#include <PLRenderer/Texture/TextureHandler.h>
#include <PLRenderer/Renderer/ProgramGenerator.h>
#include <PLScene/Compositing/SceneRendererPass.h>
#include "PLCompositing/PLCompositing.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class VertexBuffer;
	class ProgramUniform;
	class ProgramAttribute;
}
namespace PLMesh {
	class Mesh;
	class MeshHandler;
	class MeshLODLevel;
}
namespace PLScene {
	class VisNode;
	class SNLight;
	class SceneNode;
}
namespace PLCompositing {
	class SRPShadowMapping;
	class SRPLightingMaterial;
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
*    Shaders based lighting scene renderer pass implementation
*
*  @remarks
*    This scene renderer pass can deal with the following material parameters:
*    - TwoSided:                   Backface culling active? If 1, both sides of polygons are visible, default: 0
*    - DiffuseColor:               Use this 3 floating point values to set the diffuse color, default: 1.0 1.0 1.0
*    - DiffuseMap:                 Diffuse map (2D, RGB, other usual name: 'decal map')
*      - AlphaReference:           Alpha reference 0.0-1.0, only used if the diffuse map has an alpha channel, if 0 no alpha test is performed at all
*    - DiffuseRampMap:             Can be used for 'cell shading' (1D, grayscale)
*    - IndexOfRefraction:          Index of refraction (IOR), used for Fresnel reflection, <= 0.0 means no Fresnel reflection, default: 0.0 (List of refractive indices: http://en.wikipedia.org/wiki/List_of_refractive_indices, the index of refraction of the "from" material is set to air=1.0)
*      - FresnelReflectionPower:   If 'IndexOfRefraction' is > 0.0, this is the Fresnel reflection power, default: 5.0
*    - ReflectionColor:            If 'IndexOfRefraction' is > 0.0 or if there's a 'ReflectionMap', use this 3 floating point values to set the reflection color, default: 1.0 1.0 1.0
*    - Reflectivity:               If 'IndexOfRefraction' is > 0.0 or if there's a 'ReflectionMap', reflectivity factor can be used, 0.0...1.0=no reflection...full reflection, default: 1.0
*    - ReflectivityMap:            Per fragment 'reflectivity' control (2D, grayscale)
*    - ReflectionMap:             'Reflections', not influenced by any light, 2D (spherical environment mapping) or cube map (cubic environment mapping), RGB
*    - NormalMap:                  Normal map (sometimes also called 'Dot3 bump map') required for 'per pixel lighting' (2D, RGB, tangent space)
*      - NormalMapBumpiness:       The 'bumpiness' of the normal map, normally values from 0-2, default: 1
*    - DetailNormalMap:            For more detailed 'per pixel lighting' (2D, RGB, tangent space)
*      - DetailNormalMapBumpiness: The 'bumpiness' of the detail normal map, normally values from 0-2, default: 1
*      - DetailNormalMapUVScale:   Texture coordinate scale factor for the detail normal map, default: 4.0 4.0
*    - HeightMap:                  Height map required for 'per pixel lighting' with "Parallax Mapping" or also called "Offset Normal Mapping" or "Virtual Displacement Mapping" (2D, grayscale, other usual name: 'bump map')
*      - Parallax:                 Controls the 'depth' of the material if parallax mapping is used, normally values from 0.01-0.04, default: 0.04. Parallax mapping can ONLY be used if a height map is given!
*    - SpecularColor:              Multiplied with the final specular term, normally values from 0-1, default: 1 for all components. If all 0, there's no specular lighting at all.
*      - SpecularExponent:         The 'power' (aka 'gloss' aka 'shininess') of the specular lighting, normally values from 1-128, default: 45
*      - SpecularMap:              Specular control map (2D, RGB, if there's an alpha channel it's used for specular power control)
*      - SpecularRampMap:          Can be used for 'cell shading' (1D, grayscale)
*    - EdgeRampMap:                Can be used for cartoon silhouettes (1D, grayscale)
*
*    Example:
*  @verbatim
*    <?xml version="1.0"?>
*    <Material Version="1">
*        <Float Name="TwoSided">0.0</Float>
*        <Float3 Name="DiffuseColor">1.0 1.0 1.0</Float3>
*        <Texture Name="DiffuseMap">Data/Textures/My_DiffuseMap.dds</Texture>
*        <Texture Name="DiffuseRampMap">Data/Textures/My_DiffuseRampMap.dds</Texture>
*        <Float Name="AlphaReference">0.5</Float>
*        <Float Name="IndexOfRefraction">0.0</Float>
*        <Float Name="FresnelReflectionPower">5.0</Float>
*        <Float3 Name="ReflectionColor">1.0 1.0 1.0</Float3>
*        <Float Name="Reflectivity">1.0</Float>
*        <Texture Name="ReflectivityMap">Data/Textures/My_ReflectivityMap.dds</Texture>
*        <Texture Name="ReflectionMap">Data/Textures/My_ReflectionMap.dds</Texture>
*        <Texture Name="NormalMap">Data/Textures/My_NormalMap.dds</Texture>
*        <Float Name="NormalMapBumpiness">1.0</Float>
*        <Texture Name="DetailNormalMap">Data/Textures/My_DetailNormalMap.dds</Texture>
*        <Float Name="DetailNormalMapBumpiness">1.0</Float>
*        <Float2 Name="DetailNormalMapUVScale">4.0 4.0</Float2>
*        <Texture Name="HeightMap">Data/Textures/My_HeightMap.dds</Texture>
*        <Float Name="Parallax">0.04</Float>
*        <Float3 Name="SpecularColor">1.0 1.0 1.0</Float3>
*        <Float Name="SpecularExponent">45.0</Float>
*        <Texture Name="SpecularRampMap">Data/Textures/My_SpecularRampMap.dds</Texture>
*        <Texture Name="SpecularMap">Data/Textures/My_SpecularMap.dds</Texture>
*        <Texture Name="SpecularRampMap">Data/Textures/My_SpecularRampMap.dds</Texture>
*        <Texture Name="EdgeRampMap">Data/Textures/My_EdgeRampMap.dds</Texture>
*    </Material>
*  @endverbatim
*
*  @note
*    - Normal map compression using swizzled DXT5 (xGxR), LATC2 and alternate XY swizzle LATC2 is supported
*    - If there's an SRPDirectionalLightingShaders instance, "SRPDirectionalLightingShaders::GetUsedLight()" will be called to ask for
*      an already drawn directional light source (*the primary light source*)... if such a light exists, this scene renderer
*      pass automatically skips the light rendering so that this light source is not rendered twice!
*/
class SRPLighting : public PLScene::SceneRendererPass {


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
			Bilinear      = 1,	/**< Bilinear */
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
		*    Scene renderer pass flags (PLScene::SceneRendererPass flags extension)
		*/
		enum EFlags {
			NoGammaCorrection       = 1<<1,		/**< Do not perform gamma correction (in general gamma correction done for: DiffuseMap, ReflectionMap) - if gamma correction is enabled, there should be a gamma correction at the end of the render pipeline */
			NoDiffuseMap            = 1<<2,		/**< Ignore diffuse map */
			NoFresnelReflection     = 1<<3,		/**< Ignore fresnel reflection */
			NoReflectivityMap       = 1<<4,		/**< Ignore reflectivity map */
			NoReflectionMap         = 1<<5,		/**< Ignore reflection map */
			NoNormalMap             = 1<<6,		/**< Ignore normal maps */
			NoDetailNormalMap       = 1<<7,		/**< Ignore detail normal maps */
			NoParallaxMapping       = 1<<8,		/**< No parallax mapping (also called 'Photonic Mapping', 'Offset Mapping' or 'Virtual Displacement Mapping') */
			NoSpecular              = 1<<9,		/**< No specular */
			NoSpecularMap           = 1<<10,	/**< Ignore specular maps */
			NoDiffuseRampMap        = 1<<11,	/**< Ignore diffuse ramp maps */
			NoSpecularRampMap       = 1<<12,	/**< Ignore specular ramp maps */
			NoEdgeRampMap           = 1<<13,	/**< Ignore edge ramp maps */
			NoShadow                = 1<<14,	/**< Do not render shadows */
			NoSoftShadow            = 1<<15,	/**< No soft shadows */
			NoProjectivePointLights = 1<<16,	/**< No projective point lights */
			NoProjectiveSpotLights  = 1<<17,	/**< No projective spot lights */
			NoDiscard               = 1<<18		/**< Disable discard (may result in better or worse performance) */
		};
		pl_enum(EFlags)
			pl_enum_base(PLScene::SceneRendererPass::EFlags)
			pl_enum_value(NoGammaCorrection,		"Do not perform gamma correction (in general gamma correction done for: DiffuseMap, ReflectionMap) - if gamma correction is enabled, there should be a gamma correction at the end of the render pipeline")
			pl_enum_value(NoDiffuseMap,				"Ignore diffuse map")
			pl_enum_value(NoFresnelReflection,		"Ignore fresnel reflection")
			pl_enum_value(NoReflectivityMap,		"Ignore reflectivity map")
			pl_enum_value(NoReflectionMap,			"Ignore reflection map")
			pl_enum_value(NoNormalMap,				"Ignore normal maps")
			pl_enum_value(NoDetailNormalMap,		"Ignore detail normal maps")
			pl_enum_value(NoParallaxMapping,		"No parallax mapping (also called 'Photonic Mapping', 'Offset Mapping' or 'Virtual Displacement Mapping')")
			pl_enum_value(NoSpecular,				"No specular")
			pl_enum_value(NoSpecularMap,			"Ignore specular maps")
			pl_enum_value(NoDiffuseRampMap,			"Ignore diffuse ramp maps")
			pl_enum_value(NoSpecularRampMap,		"Ignore specular ramp maps")
			pl_enum_value(NoEdgeRampMap,			"Ignore edge ramp maps")
			pl_enum_value(NoShadow,					"Do not render shadows")
			pl_enum_value(NoSoftShadow,				"No soft shadows")
			pl_enum_value(NoProjectivePointLights,	"No projective point lights")
			pl_enum_value(NoProjectiveSpotLights,	"No projective spot lights")
			pl_enum_value(NoDiscard,				"Disable discard (may result in better or worse performance)")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SRPLighting, "PLCompositing", PLScene::SceneRendererPass, "Shaders based lighting scene renderer pass implementation")
		// Attributes
		pl_attribute(TextureFiltering,	pl_enum_type(ETextureFiltering),	Anisotropic8,	ReadWrite,	DirectValue,	"Texture filtering",																													"")
		pl_attribute(ShaderLanguage,	PLCore::String,						"",				ReadWrite,	DirectValue,	"Shader language to use (for example \"GLSL\" or \"Cg\"), if empty string, the default shader language of the renderer will be used",	"")
		pl_attribute(LightingIntensity,	float,								1.0f,			ReadWrite,	DirectValue,	"General lighting intensity",																											"")
			// Overwritten PLScene::SceneRendererPass attributes
		pl_attribute(Flags,				pl_flag_type(EFlags),				0,				ReadWrite,	GetSet,			"Flags",																																"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLCOM_API SRPLighting();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SRPLighting();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Sets the initial render states
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*/
		void SetInitialRenderStates(PLRenderer::Renderer &cRenderer);

		/**
		*  @brief
		*    Sets correct texture filtering modes
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] nStage
		*    Texture stage
		*/
		void SetupTextureFiltering(PLRenderer::Renderer &cRenderer, PLCore::uint32 nStage) const;

		/**
		*  @brief
		*    Draws recursive
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cCullQuery
		*    Cull query to use
		*/
		void DrawRec(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery);

		/**
		*  @brief
		*    Renders a light
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cCullQuery
		*    Cull query to use
		*  @param[in] cLight
		*    Used light node
		*  @param[in] cLightVisNode
		*    The light visibility node
		*/
		void RenderLight(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery, PLScene::SNLight &cLight, const PLScene::VisNode &cLightVisNode);

		/**
		*  @brief
		*    Renders a light recursive
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cCullQuery
		*    Cull query to use
		*  @param[in] cLight
		*    Used light node
		*  @param[in] cLightVisNode
		*    The light visibility node
		*  @param[in] pSRPShadowMapping
		*    Shadow mapping scene renderer pass to use, can be a null pointer
		*/
		void RenderLightRec(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery, PLScene::SNLight &cLight, const PLScene::VisNode &cLightVisNode, SRPShadowMapping *pSRPShadowMapping);

		/**
		*  @brief
		*    Draws a mesh
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cCullQuery
		*    Cull query to use
		*  @param[in] cVisNode
		*    Visibility node to use
		*  @param[in] cSceneNode
		*    Mesh owner scene node
		*  @param[in] cMeshHandler
		*    Mesh handler to use
		*  @param[in] cMesh
		*    Mesh to draw
		*  @param[in] cMeshLODLevel
		*    LOD level of the mesh to draw
		*  @param[in] cVertexBuffer
		*    Vertex buffer to use
		*  @param[in] cLight
		*    Used light node
		*  @param[in] cLightVisNode
		*    The light visibility node
		*  @param[in] pSRPShadowMapping
		*    Shadow mapping scene renderer pass to use, can be a null pointer
		*/
		void DrawMesh(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode, PLScene::SceneNode &cSceneNode, const PLMesh::MeshHandler &cMeshHandler,
					  const PLMesh::Mesh &cMesh, const PLMesh::MeshLODLevel &cMeshLODLevel, PLRenderer::VertexBuffer &cVertexBuffer, PLScene::SNLight &cLight, const PLScene::VisNode &cLightVisNode, SRPShadowMapping *pSRPShadowMapping);

		/**
		*  @brief
		*    Called when a material is removed
		*
		*  @param[in] cMaterial
		*    Removed material
		*/
		void OnMaterialRemoved(PLRenderer::Material &cMaterial);


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<PLRenderer::Material&> EventHandlerMaterialRemoved;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLScene::SNLight					*m_pIgnoredLight;		/**< Do not render this light, it may have already been rendered by for instance "PLCompositing::SRPDirectionalLightingShaders", can be a null pointer */
		PLRenderer::TextureHandler			 m_cSpotMapHandler;		/**< Texture handler for the default spot map */
		PLRenderer::TextureHandler			 m_cCubeMapHandler;		/**< Texture handler for the default cube map */

		// Material cache
		PLRenderer::ProgramGenerator						  *m_pProgramGenerator;	/**< Program generator, can be a null pointer */
		PLCore::HashMap<PLCore::uint64, SRPLightingMaterial*>  m_lstMaterialCache;	/**< List of cached materials */


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


#endif // __PLCOMPOSITING_GENERAL_LIGHTING_H__
