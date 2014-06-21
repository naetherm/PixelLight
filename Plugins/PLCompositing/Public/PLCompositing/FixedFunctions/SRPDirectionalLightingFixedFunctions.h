/*********************************************************\
 *  File: SRPDirectionalLightingFixedFunctions.h         *
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


#ifndef __PLCOMPOSITING_FIXEDFUNCTIONS_LIGHTINGTRANSPARENT_H__
#define __PLCOMPOSITING_FIXEDFUNCTIONS_LIGHTINGTRANSPARENT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCompositing/SRPDirectionalLighting.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class Material;
	class RenderStates;
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
*    Scene renderer pass for rendering transparent meshes with lighting
*
*  @remarks
*    This scene renderer pass can deal with the following material parameters:
*    - Opacity:             Transparency, 0.0 is full transparent, 1.0 is solid, default: 1.0
*    - SrcBlendFunc:        If 'Opacity' is less '1', source blend function, default: 'SrcAlpha' (see 'PLRenderer::BlendFunc::Enum')
*    - DstBlendFunc:        If 'Opacity' is less '1', destination blend function, default: 'InvSrcAlpha' (see 'PLRenderer::BlendFunc::Enum')
*    - TwoSided:            Backface culling active? If 1, both sides of polygons are visible, default: 0
*    - DiffuseColor:        Use this 3 floating point values to set the diffuse color, default: 1.0 1.0 1.0
*    - DiffuseMap:          Diffuse map (2D, RGB, other usual name: 'decal map')
*    - AlphaReference:      Alpha reference 0.0-1.0, only used if the diffuse map has an alpha channel, if 0 no alpha test is performed at all
*    - SpecularColor:       Multiplied with the final specular term, normally values from 0-1, default: 1 for all components. If all 0, there's no specular lighting at all.
*    - SpecularExponent:    The 'power' (aka 'gloss' aka 'shininess') of the specular lighting, normally values from 1-128, default: 45
*    - LightMap:            'Static lighting/shadowing', not influenced by any light but influences the lighting (2D, RGB)
*    - AmbientOcclusionMap: 'Static global ambient occlusion', not influenced by any light but influences the lighting (2D, grayscale)
*    - ReflectionMap:       'Reflections', not influenced by any light (2D/cube map, RGB, other usual name: 'environment map')
*
*    Example:
*  @verbatim
*    <?xml version="1.0"?>
*    <Material Version="1">
*        <Float Name="Opacity">1.0</Float>
*        <String Name="SrcBlendFunc">SrcAlpha</String>
*        <String Name="DstBlendFunc">InvSrcAlpha</String>
*        <Float Name="TwoSided">0.0</Float>
*        <Float3 Name="DiffuseColor">1.0 1.0 1.0</Float3>
*        <Texture Name="DiffuseMap">Data/Textures/My_DiffuseMap.dds</Texture>
*        <Float Name="AlphaReference">0.5</Float>
*        <Float3 Name="SpecularColor">1.0 1.0 1.0</Float3>
*        <Float Name="SpecularExponent">45.0</Float>
*        <Texture Name="LightMap">Data/Textures/My_LightMap.dds</Texture>
*        <Texture Name="AmbientOcclusionMap">Data/Textures/My_AmbientOcclusionMap.dds</Texture>
*        <Texture Name="ReflectionMap">Data/Textures/My_ReflectionMap.dds</Texture>
*    </Material>
*  @endverbatim
*
*  @note
*    - Do ONLY use power of two textures!
*/
class SRPDirectionalLightingFixedFunctions : public SRPDirectionalLighting {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene renderer pass flags (PLScene::SceneRendererPass flags extension)
		*/
		enum EFlags {
			NoSpecular            = 1<<5,	/**< No specular */
			NoReflectionMap       = 1<<6,	/**< Ignore reflection map */
			NoLightMap            = 1<<7,	/**< Ignore light map */
			NoAmbientOcclusionMap = 1<<8	/**< Ignore ambient occlusion map */
		};
		pl_enum(EFlags)
			pl_enum_base(SRPDirectionalLighting::EFlags)
			pl_enum_value(NoSpecular,				"No specular")
			pl_enum_value(NoReflectionMap,			"Ignore reflection map")
			pl_enum_value(NoLightMap,				"Ignore light map")
			pl_enum_value(NoAmbientOcclusionMap,	"Ignore ambient occlusion map")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SRPDirectionalLightingFixedFunctions, "PLCompositing", PLCompositing::SRPDirectionalLighting, "Scene renderer pass for rendering transparent meshes with lighting")
		// Attributes
			// Overwritten PLScene::SceneRendererPass attributes
		pl_attribute(Flags,	pl_flag_type(EFlags),	0,	ReadWrite,	GetSet,	"Flags",	"")
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
		PLCOM_API SRPDirectionalLightingFixedFunctions();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SRPDirectionalLightingFixedFunctions();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLRenderer::RenderStates   *m_pRenderStates;	/**< Used to 'translate' render state strings, always valid! */
		PLCore::uint32				m_nMaterialChanges;	/**< Number of material changes */
		const PLRenderer::Material *m_pCurrentMaterial;	/**< Current used material, can be a null pointer */


	//[-------------------------------------------------------]
	//[ Private virtual SRPDirectionalLighting functions      ]
	//[-------------------------------------------------------]
	private:
		virtual void DrawMesh(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode, PLScene::SceneNode &cSceneNode, const PLMesh::MeshHandler &cMeshHandler, const PLMesh::Mesh &cMesh, const PLMesh::MeshLODLevel &cMeshLODLevel, PLRenderer::VertexBuffer &cVertexBuffer) override;


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


#endif // __PLCOMPOSITING_FIXEDFUNCTIONS_LIGHTINGTRANSPARENT_H__
