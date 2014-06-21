/*********************************************************\
 *  File: SRPShadowMapping.h                             *
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


#ifndef __PLCOMPOSITING_SRPSHADOWMAPPING_H__
#define __PLCOMPOSITING_SRPSHADOWMAPPING_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Pool.h>
#include <PLMath/Rectangle.h>
#include <PLRenderer/Renderer/ProgramGenerator.h>
#include <PLScene/Compositing/SceneRendererPass.h>
#include "PLCompositing/PLCompositing.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLMath {
	class Matrix4x4;
}
namespace PLRenderer {
	class Renderer;
	class Material;
	class IndexBuffer;
	class VertexBuffer;
	class ProgramUniform;
	class ProgramAttribute;
	class SurfaceTextureBuffer;
}
namespace PLMesh {
	class Mesh;
	class Geometry;
	class MeshHandler;
	class MeshLODLevel;
}
namespace PLScene {
	class SQCull;
	class SNLight;
	class VisNode;
	class SceneQueryHandler;
}
namespace PLCompositing {
	class ShadowMapping;
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
*    Shadow mapping scene renderer pass implementation
*/
class SRPShadowMapping : public PLScene::SceneRendererPass {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SRPShadowMapping, "PLCompositing", PLScene::SceneRendererPass, "Shadow map manager scene renderer pass implementation")
		// Attributes
		pl_attribute(ShaderLanguage,		PLCore::String,	"",		ReadWrite,	DirectValue,	"Shader language to use (for example \"GLSL\" or \"Cg\"), if empty string, the default shader language of the renderer will be used",	"")
		pl_attribute(SlopeScaleDepthBias,	float,			2.0f,	ReadWrite,	DirectValue,	"Slope scale depth bias (polygon offset to avoid nasty shadow artifacts)",																"")
		pl_attribute(DepthBias,				float,			10.0f,	ReadWrite,	DirectValue,	"Depth bias (polygon offset to avoid nasty shadow artifacts)",																			"")
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
		PLCOM_API SRPShadowMapping();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SRPShadowMapping();

		/**
		*  @brief
		*    Updates the shadow map
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cLight
		*    Light scene node to render the shadow map for
		*  @param[in] cCullQuery
		*    Current cull query
		*  @param[in] fSquaredDistanceToCamera
		*    Squared distance to camera, if <=0 means no shadow LOD 
		*
		*  @note
		*    - Changes the current render states!
		*/
		PLCOM_API void UpdateShadowMap(PLRenderer::Renderer &cRenderer, PLScene::SNLight &cLight, const PLScene::SQCull &cCullQuery, float fSquaredDistanceToCamera);

		/**
		*  @brief
		*    Returns the render target for the cube shadow map
		*
		*  @return
		*    The render target for the cube shadow map, a null pointer on error
		*
		*  @note
		*    - Data of the previous "UpdateShadowMap()"-call
		*/
		PLCOM_API PLRenderer::SurfaceTextureBuffer *GetCubeShadowRenderTarget() const;

		/**
		*  @brief
		*    Returns the render target for the spot shadow map
		*
		*  @return
		*    The render target for the spot shadow map, a null pointer on error
		*
		*  @note
		*    - Data of the previous "UpdateShadowMap()"-call
		*/
		PLCOM_API PLRenderer::SurfaceTextureBuffer *GetSpotShadowRenderTarget() const;

		/**
		*  @brief
		*    Returns the used combined light view projection matrix
		*
		*  @return
		*    The used combined light view projection matrix
		*
		*  @note
		*    - Data of the previous "UpdateShadowMap()"-call
		*    - GetLightViewProjectionMatrix() = GetLightProjectionMatrix() * GetLightViewMatrix()
		*    - Matrix purpose: Scene node space to view space and then view space to clip space [-1...1] combined within one matrix
		*/
		PLCOM_API const PLMath::Matrix4x4 &GetLightViewProjectionMatrix() const;

		/**
		*  @brief
		*    Returns the used light projection matrix
		*
		*  @return
		*    The used light projection matrix
		*
		*  @note
		*    - Data of the previous "UpdateShadowMap()"-call
		*    - Matrix purpose: View space to clip space [-1...1]
		*/
		PLCOM_API const PLMath::Matrix4x4 &GetLightProjectionMatrix() const;

		/**
		*  @brief
		*    Returns the used light view matrix
		*
		*  @return
		*    The used light view matrix
		*
		*  @note
		*    - Data of the previous "UpdateShadowMap()"-call
		*    - Matrix purpose: Scene node space to view space
		*/
		PLCOM_API const PLMath::Matrix4x4 &GetLightViewMatrix() const;


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
			FS_ALPHATEST = 1<<0		/**< Use alpha test to discard fragments */
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
			PLRenderer::ProgramUniform *pWorldVP;
			PLRenderer::ProgramUniform *pWorldV;
			PLRenderer::ProgramUniform *pInvRadius;
			// Fragment shader uniforms
			PLRenderer::ProgramUniform *pDiffuseMap;
			PLRenderer::ProgramUniform *pAlphaReference;
		};

		/**
		*  @brief
		*    Mesh batch
		*/
		struct MeshBatch {
			const PLRenderer::Material		*pMaterial;			/**< Used material, always valid! */
				  PLRenderer::VertexBuffer	*pVertexBuffer;		/**< Used vertex buffer, always valid! */
				  PLRenderer::IndexBuffer	*pIndexBuffer;		/**< Used index buffer, always valid! */
			const PLMesh::Geometry			*pGeometry;			/**< Used geometry, always valid! */
			const PLScene::SQCull			*pCullQuery;		/**< Used cull query, always valid! */
			const PLScene::VisNode			*pVisNode;			/**< Used visibility node, always valid! */
				  PLMath::Rectangle			 sScissorRectangle;	/**< Scissor rectangle */
		};


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Collect mesh batches recursive
		*
		*  @param[in] cCullQuery
		*    Cull query to use
		*/
		void CollectMeshBatchesRec(const PLScene::SQCull &cCullQuery);

		/**
		*  @brief
		*    Makes a material to the currently used one
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cMaterial
		*    Material to use
		*  @param[in] fInvRadius
		*    Set world space inverse light radius
		*
		*  @return
		*    Generated program user data, do NOT delete the memory the pointer points to
		*/
		GeneratedProgramUserData *MakeMaterialCurrent(PLRenderer::Renderer &cRenderer, const PLRenderer::Material &cMaterial, float fInvRadius);

		/**
		*  @brief
		*    Draws a mesh batch
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cGeneratedProgramUserData
		*    Generated program user data for the mesh batch material
		*  @param[in] cMeshBatch
		*    Mesh batch to use
		*/
		void DrawMeshBatch(PLRenderer::Renderer &cRenderer, GeneratedProgramUserData &cGeneratedProgramUserData, MeshBatch &cMeshBatch) const;

		/**
		*  @brief
		*    Returns a free mesh batch
		*
		*  @return
		*    Free mesh batch
		*
		*  @note
		*    - Use FreeMeshBatch() if you no longer need this mesh batch
		*/
		MeshBatch &GetFreeMeshBatch();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ShadowMapping				*m_pShadowMapping;			/**< The shadow mapping algorithm to use, can be a null pointer */
		PLMath::Matrix4x4			 m_mLightProjection;		/**< View space to clip space [-1...1] */
		PLMath::Matrix4x4			 m_mLightView;				/**< Scene node space to view space */
		PLMath::Matrix4x4			 m_mLightViewProjection;	/**< Scene node space to view space and then view space to clip space [-1...1] combined within one matrix (m_mLightViewProjection = m_mLightProjection * m_mLightView) */
		PLScene::SceneQueryHandler	*m_pLightCullQuery;			/**< Light cull query (always valid!) */

		// Cube shadow map
		static const PLCore::uint32 CubeShadowRenderTargets = 5;
		PLRenderer::SurfaceTextureBuffer *m_pCubeShadowRenderTarget[CubeShadowRenderTargets];	/**< 256, 128, 64, 32, 16, can be a null pointer */
		PLRenderer::SurfaceTextureBuffer **m_pCurrentCubeShadowRenderTarget;

		// Spot shadow map
		static const PLCore::uint32 SpotShadowRenderTargets = 5;
		PLRenderer::SurfaceTextureBuffer *m_pSpotShadowRenderTarget[SpotShadowRenderTargets];	/**< 512, 256, 128, 64, 32, can be a null pointer */
		PLRenderer::SurfaceTextureBuffer **m_pCurrentSpotShadowRenderTarget;

		// Material & shader
		PLCore::Pool<const PLRenderer::Material*>  m_lstMaterials;			/**< List of currently used materials */
		PLCore::Pool<MeshBatch*>				   m_lstFreeMeshBatches;	/**< List of currently free mesh batches */
		PLCore::Pool<MeshBatch*>				   m_lstMeshBatches;		/**< List of currently used mesh batches */
		PLRenderer::ProgramGenerator			  *m_pProgramGenerator;		/**< Program generator, can be a null pointer */
		PLRenderer::ProgramGenerator::Flags		   m_cProgramFlags;			/**< Program flags as class member to reduce dynamic memory allocations */


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


#endif // __PLCOMPOSITING_SRPSHADOWMAPPING_H__
