/*********************************************************\
 *  File: SRPVolumetricFog.h                             *
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


#ifndef __PLCOMPOSITING_GENERAL_DIFFUSEONLY_H__
#define __PLCOMPOSITING_GENERAL_DIFFUSEONLY_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
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
	class SceneNode;
	class SNSphereFog;
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
*    Shaders based volumetric fog scene renderer pass implementation
*
*  @remarks
*    This scene renderer pass can deal with the following material parameters:
*    - TwoSided:         Backface culling active? If 1, both sides of polygons are visible, default: 0
*    - DiffuseMap:       Diffuse map (2D, RGB, other usual name: 'decal map')
*      - AlphaReference: Alpha reference 0.0-1.0, only used if the diffuse map has an alpha channel, if 0 no alpha test is performed at all
*
*    Example:
*  @verbatim
*    <?xml version="1.0"?>
*    <Material Version="1">
*        <Float Name="TwoSided">0.0</Float>
*        <Texture Name="DiffuseMap">Data/Textures/My_DiffuseMap.dds</Texture>
*        <Float Name="AlphaReference">0.5</Float>
*    </Material>
*  @endverbatim
*/
class SRPVolumetricFog : public PLScene::SceneRendererPass {


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
			NoDiffuseMap = 1<<1	/**< Ignore diffuse map */
		};
		pl_enum(EFlags)
			pl_enum_base(PLScene::SceneRendererPass::EFlags)
			pl_enum_value(NoDiffuseMap, "Ignore diffuse map")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SRPVolumetricFog, "PLCompositing", PLScene::SceneRendererPass, "Shaders based volumetric fog scene renderer pass implementation")
		// Attributes
		pl_attribute(TextureFiltering,	pl_enum_type(ETextureFiltering),	Anisotropic8,	ReadWrite,	DirectValue,	"Texture filtering",																													"")
		pl_attribute(ShaderLanguage,	PLCore::String,						"",				ReadWrite,	DirectValue,	"Shader language to use (for example \"GLSL\" or \"Cg\"), if empty string, the default shader language of the renderer will be used",	"")
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
		PLCOM_API SRPVolumetricFog();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SRPVolumetricFog();


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
		*    Draws a volumetric spherical fog recursive
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cCullQuery
		*    Cull query to use
		*  @param[in] cSphereFog
		*    Used spherical fog node
		*  @param[in] cSphereFogVisNode
		*    The spherical fog visibility node
		*/
		void DrawVolumetricSphereFogRec(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery, PLScene::SNSphereFog &cSphereFog, const PLScene::VisNode &cSphereFogVisNode);

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
		*  @param[in] cSphereFog
		*    Used spherical fog node
		*  @param[in] cSphereFogVisNode
		*    The spherical fog visibility node
		*/
		void DrawMesh(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode, PLScene::SceneNode &cSceneNode, const PLMesh::MeshHandler &cMeshHandler, const PLMesh::Mesh &cMesh, const PLMesh::MeshLODLevel &cMeshLODLevel, PLRenderer::VertexBuffer &cVertexBuffer, PLScene::SNSphereFog &cSphereFog, const PLScene::VisNode &cSphereFogVisNode);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Vertex shader flags, flag names become to source code definitions
		*/
		enum EVertexShaderFlags {
			VS_TEXCOORD0 = 1<<0	/**< Use texture coordinate 0 */
		};

		/**
		*  @brief
		*    Fragment shader flags, flag names become to source code definitions
		*/
		enum EFragmentShaderFlags {
			FS_DIFFUSEMAP		= 1<<0,	/**< Take diffuse map into account */
				FS_ALPHATEST	= 1<<1	/**< Use alpha test to discard fragments (FS_DIFFUSEMAP should be defined!) */
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
			PLRenderer::ProgramUniform *pObjectSpaceToWorldSpaceMatrix;
			PLRenderer::ProgramUniform *pObjectSpaceToClipSpaceMatrix;
			PLRenderer::ProgramUniform *pFogPosition;
			PLRenderer::ProgramUniform *pFogInvRadius;
			PLRenderer::ProgramUniform *pEyePos;
			// Fragment shader uniforms
			PLRenderer::ProgramUniform *pDiffuseMap;
			PLRenderer::ProgramUniform *pAlphaReference;
			PLRenderer::ProgramUniform *pFogColor0;
			PLRenderer::ProgramUniform *pFogColor1;
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLRenderer::ProgramGenerator		*m_pProgramGenerator;	/**< Program generator, can be a null pointer */
		PLRenderer::ProgramGenerator::Flags	 m_cProgramFlags;		/**< Program flags as class member to reduce dynamic memory allocations */


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


#endif // __PLCOMPOSITING_GENERAL_DIFFUSEONLY_H__
