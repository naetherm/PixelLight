/*********************************************************\
 *  File: SRPDiffuseOnly.h                               *
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


#ifndef __PLCOMPOSITING_DIFFUSEONLY_H__
#define __PLCOMPOSITING_DIFFUSEONLY_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLScene/Compositing/SceneRendererPass.h>
#include "PLCompositing/PLCompositing.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class VertexBuffer;
}
namespace PLMesh {
	class Mesh;
	class MeshHandler;
	class MeshLODLevel;
}
namespace PLScene {
	class VisNode;
	class SceneNode;
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
*    Abstract diffuse only scene renderer pass, can also be used as depth only renderer pass
*
*  @remarks
*    Implementations of this scene renderer pass should only draw the scene using diffuse maps and diffuse color. Therefore,
*    this is intended to be one of the most primitive scene renderer implementations. It can be used for ultra low performance
*    machines or for testing purposes.
*    Because a depth only renderer pass implementation would be nearly identical to this renderer pass, this renderer pass can
*    also be used as depth only renderer pass so that there's just one implementation which is able to do both jobs.
*/
class SRPDiffuseOnly : public PLScene::SceneRendererPass {


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
		*    Scene renderer pass flags (SceneRendererPass flags extension)
		*/
		enum EFlags {
			TransparentPass = 1<<1,	/**< This is a transparent render pass */
			NoZWrite        = 1<<2,	/**< Do not write z-values */
			ZWriteOnly      = 1<<3,	/**< Do only write z-values, but no color values (depth only renderer pass) */
			NoDiffuseMap    = 1<<4	/**< Ignore diffuse map */
		};
		pl_enum(EFlags)
			pl_enum_base(SceneRendererPass::EFlags)
			pl_enum_value(TransparentPass,	"This is a transparent render pass")
			pl_enum_value(NoZWrite,			"Do not write z-values")
			pl_enum_value(ZWriteOnly,		"Do only write z-values, but no color values (depth only renderer pass)")
			pl_enum_value(NoDiffuseMap,		"Ignore diffuse map")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SRPDiffuseOnly, "PLCompositing", PLScene::SceneRendererPass, "Abstract diffuse only scene renderer pass, can also be used as depth only renderer pass")
		// Attributes
		pl_attribute(TextureFiltering,	pl_enum_type(ETextureFiltering),	Anisotropic8,	ReadWrite,	DirectValue,	"Texture filtering",	"")
			// Overwritten PLScene::SceneRendererPass attributes
		pl_attribute(Flags,				pl_flag_type(EFlags),				0,				ReadWrite,	GetSet,			"Flags",				"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCOM_API SRPDiffuseOnly();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SRPDiffuseOnly();

		/**
		*  @brief
		*    Sets correct texture filtering modes
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] nStage
		*    Texture stage
		*/
		PLCOM_API void SetupTextureFiltering(PLRenderer::Renderer &cRenderer, PLCore::uint32 nStage) const;

		/**
		*  @brief
		*    Draws recursive
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cCullQuery
		*    Cull query to use
		*/
		PLCOM_API void DrawRec(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery);


	//[-------------------------------------------------------]
	//[ Protected virtual SRPDiffuseOnly functions            ]
	//[-------------------------------------------------------]
	protected:
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
		*    Mesh LOD level to draw
		*  @param[in] cVertexBuffer
		*    Vertex buffer to use
		*/
		virtual void DrawMesh(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode, PLScene::SceneNode &cSceneNode, const PLMesh::MeshHandler &cMeshHandler, const PLMesh::Mesh &cMesh, const PLMesh::MeshLODLevel &cMeshLODLevel, PLRenderer::VertexBuffer &cVertexBuffer) = 0;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLCOMPOSITING_DIFFUSEONLY_H__
