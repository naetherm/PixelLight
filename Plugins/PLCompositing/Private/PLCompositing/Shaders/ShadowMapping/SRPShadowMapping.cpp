/*********************************************************\
 *  File: SRPShadowMapping.cpp                           *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Tools/Tools.h>
#include <PLMath/EulerAngles.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Shader.h>
#include <PLRenderer/Renderer/Program.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ProgramAttribute.h>
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Material/Material.h>
#include <PLRenderer/Material/Parameter.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLMesh/Mesh.h>
#include <PLMesh/MeshHandler.h>
#include <PLMesh/MeshLODLevel.h>
#include <PLScene/Scene/SNLight.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLScene/Scene/SNSpotLight.h>
#include <PLScene/Scene/SNPointLight.h>
#include <PLScene/Scene/SceneQueryHandler.h>
#include <PLScene/Visibility/SQCull.h>
#include <PLScene/Visibility/VisPortal.h>
#include <PLScene/Visibility/VisContainer.h>
#include "PLCompositing/Shaders/ShadowMapping/ShadowMappingUSM.h"
#include "PLCompositing/Shaders/ShadowMapping/SRPShadowMapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
using namespace PLMesh;
using namespace PLScene;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SRPShadowMapping)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SRPShadowMapping::SRPShadowMapping() :
	ShaderLanguage(this),
	SlopeScaleDepthBias(this),
	DepthBias(this),
	m_pShadowMapping(new ShadowMappingUSM()),
	m_pLightCullQuery(new SceneQueryHandler()),
	m_pCurrentCubeShadowRenderTarget(nullptr),
	m_pCurrentSpotShadowRenderTarget(nullptr),
	m_pProgramGenerator(nullptr)
{
	// Init shadow maps
	for (int i=0; i<CubeShadowRenderTargets; i++)
		m_pCubeShadowRenderTarget[i] = nullptr;
	for (int i=0; i<SpotShadowRenderTargets; i++)
		m_pSpotShadowRenderTarget[i] = nullptr;
}

/**
*  @brief
*    Destructor
*/
SRPShadowMapping::~SRPShadowMapping()
{
	// Cube shadow maps
	for (int i=0; i<CubeShadowRenderTargets; i++) {
		if (m_pCubeShadowRenderTarget[i]) {
			delete m_pCubeShadowRenderTarget[i];
			m_pCubeShadowRenderTarget[i] = nullptr;
		}
	}
	m_pCurrentCubeShadowRenderTarget = nullptr;

	// Spot shadow maps
	for (int i=0; i<SpotShadowRenderTargets; i++) {
		if (m_pSpotShadowRenderTarget[i]) {
			delete m_pSpotShadowRenderTarget[i];
			m_pSpotShadowRenderTarget[i] = nullptr;
		}
	}
	m_pCurrentSpotShadowRenderTarget = nullptr;

	// Destroy the cull query
	SceneQuery *pSceneQuery = m_pLightCullQuery->GetElement();
	if (pSceneQuery)
		pSceneQuery->GetSceneContainer().DestroyQuery(*pSceneQuery);
	delete m_pLightCullQuery;

	// Destroy the used shadow mapping algorithm
	if (m_pShadowMapping)
		delete m_pShadowMapping;

	// Destroy the program generator
	if (m_pProgramGenerator)
		delete m_pProgramGenerator;

	// Destroy all mesh batches
	{ // Currently free ones
		Iterator<MeshBatch*> cIterator = m_lstFreeMeshBatches.GetIterator();
		while (cIterator.HasNext())
			delete cIterator.Next();
		m_lstFreeMeshBatches.Clear();
	}
	{ // Currently used ones
		Iterator<MeshBatch*> cIterator = m_lstMeshBatches.GetIterator();
		while (cIterator.HasNext())
			delete cIterator.Next();
		m_lstMeshBatches.Clear();
	}
}

/**
*  @brief
*    Updates the shadow map
*/
void SRPShadowMapping::UpdateShadowMap(Renderer &cRenderer, SNLight &cLight, const SQCull &cCullQuery, float fSquaredDistanceToCamera)
{
	if ((cLight.GetFlags() & SNLight::CastShadow) && cLight.IsPointLight()) {
		// Squared distance to camera must be >= 0
		fSquaredDistanceToCamera = Math::Abs(fSquaredDistanceToCamera);

		// Get/create render targets depending on the light type
		if (cLight.IsSpotLight()) {
			// Get texture size
			uint16 nTextureSize = 1024;
			m_pCurrentSpotShadowRenderTarget = &m_pSpotShadowRenderTarget[0];
			const float fRange = static_cast<SNSpotLight&>(cLight).GetRange();
			float fMinimumDistanceToCamera = fSquaredDistanceToCamera-(fRange*fRange);
			if (fMinimumDistanceToCamera > 20.0f*20.0f) {
				nTextureSize = 64;
				m_pCurrentSpotShadowRenderTarget = &m_pSpotShadowRenderTarget[4];
			} else if (fMinimumDistanceToCamera > 15.0f*15.0f) {
				nTextureSize = 128;
				m_pCurrentSpotShadowRenderTarget = &m_pSpotShadowRenderTarget[3];
			} else if (fMinimumDistanceToCamera > 10.0f*10.0f) {
				nTextureSize = 256;
				m_pCurrentSpotShadowRenderTarget = &m_pSpotShadowRenderTarget[2];
			} else if (fMinimumDistanceToCamera > 5.0f*5.0f) {
				nTextureSize = 512;
				m_pCurrentSpotShadowRenderTarget = &m_pSpotShadowRenderTarget[1];
			}

			// Get/create the texture
			if (!*m_pCurrentSpotShadowRenderTarget)
				*m_pCurrentSpotShadowRenderTarget = cRenderer.CreateSurfaceTextureBuffer2D(Vector2i(nTextureSize, nTextureSize), TextureBuffer::D24, SurfaceTextureBuffer::Depth|SurfaceTextureBuffer::NoMultisampleAntialiasing);
		} else if (cLight.IsPointLight()) {
			// Get texture size
			uint16 nTextureSize = 512;
			m_pCurrentCubeShadowRenderTarget = &m_pCubeShadowRenderTarget[0];
			const float fRange = static_cast<SNPointLight&>(cLight).GetRange();
			float fMinimumDistanceToCamera = fSquaredDistanceToCamera-(fRange*fRange);
			if (fMinimumDistanceToCamera > 20.0f*20.0f) {
				nTextureSize = 32;
				m_pCurrentCubeShadowRenderTarget = &m_pCubeShadowRenderTarget[4];
			} else if (fMinimumDistanceToCamera > 15.0f*15.0f) {
				nTextureSize = 64;
				m_pCurrentCubeShadowRenderTarget = &m_pCubeShadowRenderTarget[3];
			} else if (fMinimumDistanceToCamera > 10.0f*10.0f) {
				nTextureSize = 128;
				m_pCurrentCubeShadowRenderTarget = &m_pCubeShadowRenderTarget[2];
			} else if (fMinimumDistanceToCamera > 5.0f*5.0f) {
				nTextureSize = 256;
				m_pCurrentCubeShadowRenderTarget = &m_pCubeShadowRenderTarget[1];
			}

			// Get/create the texture
			if (!*m_pCurrentCubeShadowRenderTarget)
				*m_pCurrentCubeShadowRenderTarget = cRenderer.CreateSurfaceTextureBufferCube(nTextureSize, TextureBuffer::R8G8B8A8, SurfaceTextureBuffer::Depth|SurfaceTextureBuffer::NoMultisampleAntialiasing);
		}

		// Render
		if ((m_pCurrentCubeShadowRenderTarget && *m_pCurrentCubeShadowRenderTarget) ||
			(m_pCurrentSpotShadowRenderTarget && *m_pCurrentSpotShadowRenderTarget)) {
			// Setup render states
			cRenderer.GetRendererContext().GetEffectManager().Use();

			// Create cull query if required
			const SQCull *pRootQuery = cCullQuery.GetVisRootContainer() ? cCullQuery.GetVisRootContainer()->GetCullQuery() : nullptr;
			if (pRootQuery) {
				SQCull *pLightCullQuery = static_cast<SQCull*>(m_pLightCullQuery->GetElement());
				if (!pLightCullQuery) {
					pLightCullQuery = static_cast<SQCull*>(pRootQuery->GetSceneContainer().CreateQuery("PLScene::SQCull"));
					if (pLightCullQuery) {
						m_pLightCullQuery->SetElement(pLightCullQuery);

						// Set the cull mode
						pLightCullQuery->SetMode(SQCull::Frustum);

						// We do not need to inform any scene query listeners
						pLightCullQuery->SetFlags(0);
					}
				}
				if (pLightCullQuery) {
					// Get the shader language to use
					String sShaderLanguage = ShaderLanguage;
					if (!sShaderLanguage.GetLength())
						sShaderLanguage = cRenderer.GetDefaultShaderLanguage();

					// Create the program generator if there's currently no instance of it
					if (!m_pProgramGenerator || m_pProgramGenerator->GetShaderLanguage() != sShaderLanguage) {
						// If there's an previous instance of the program generator, destroy it first
						if (m_pProgramGenerator) {
							delete m_pProgramGenerator;
							m_pProgramGenerator = nullptr;
						}

						// Choose the shader source codes depending on the requested shader language
						if (sShaderLanguage == "GLSL") {
							#include "SRPShadowMapping_GLSL.h"
							if (cRenderer.GetAPI() == "OpenGL ES 2.0") {
								// Get shader source codes
								m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sSRPShadowMapping_GLSL_VS, "110", sSRPShadowMapping_GLSL_FS, "110");	// OpenGL 2.0 ("#version 110")
							} else {
								// Remove precision qualifiers because they usually create some nasty driver issues!
								m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, Shader::RemovePrecisionQualifiersFromGLSL(sSRPShadowMapping_GLSL_VS), "110", Shader::RemovePrecisionQualifiersFromGLSL(sSRPShadowMapping_GLSL_FS), "110");	// OpenGL 2.0 ("#version 110")
							}
						} else if (sShaderLanguage == "Cg") {
							#include "SRPShadowMapping_Cg.h"
							m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sSRPShadowMapping_Cg_VS, "arbvp1", sSRPShadowMapping_Cg_FS, "arbfp1");
						}
					}

					// If there's no program generator, we don't need to continue
					if (m_pProgramGenerator) {
						// Backup the color mask
						bool bRed, bGreen, bBlue, bAlpha;
						cRenderer.GetColorMask(bRed, bGreen, bBlue, bAlpha);

						// Get current render target
						Surface *pSurfaceBackup = cRenderer.GetRenderTarget();
						cRenderer.SetRenderState(RenderState::ScissorTestEnable, true);

						// Set polygon offset to avoid nasty shadow artifacts
						cRenderer.SetRenderState(RenderState::SlopeScaleDepthBias,	Tools::FloatToUInt32(SlopeScaleDepthBias.Get()));
						cRenderer.SetRenderState(RenderState::DepthBias,			Tools::FloatToUInt32(DepthBias.Get()));

						// Get world space inverse light radius
						const float fInvRadius = 1.0f/(cLight.IsPointLight() ? static_cast<SNPointLight&>(cLight).GetRange() : 0.0f);

						// Spot or point light?
						if (cLight.IsSpotLight()) {
							// Get the shadow mapping algorithm to use
							if (m_pShadowMapping) {
								// Calculate the view matrix and projection matrix for the given light
								m_pShadowMapping->CalculateLightMatrices(cLight, m_mLightView, m_mLightProjection);

								// Concatenate (multiply) the view matrix and the projection matrix
								m_mLightViewProjection = m_mLightProjection;
								m_mLightViewProjection *= m_mLightView;

								// Calculate the light frustum
								Frustum cLightFrustum;
								cLightFrustum.CreateViewPlanes(m_mLightViewProjection, true);

								// Setup render query
								pLightCullQuery->SetCameraContainer(cLight.GetContainer()->IsCell() ? cLight.GetContainer() : nullptr);
								pLightCullQuery->SetCameraPosition(cLight.GetTransform().GetPosition());
								pLightCullQuery->SetViewFrustum(cLightFrustum);
								pLightCullQuery->SetProjectionMatrix(m_mLightProjection);
								pLightCullQuery->SetViewMatrix(m_mLightView);
								pLightCullQuery->SetViewProjectionMatrix(m_mLightViewProjection);

								// Set the new render target
								if (cRenderer.SetRenderTarget(*m_pCurrentSpotShadowRenderTarget)) {
									// Perform the visibility determination
									pLightCullQuery->PerformQuery();

									// Clear the frame buffer
									cRenderer.Clear(Clear::ZBuffer);

									// Disable color writes
									cRenderer.SetColorMask(false, false, false, false);

									// Collect recursive
									CollectMeshBatchesRec(*pLightCullQuery);

									{ // Loop through all currently used materials
										Iterator<const Material*> cMaterialIterator = m_lstMaterials.GetIterator();
										while (cMaterialIterator.HasNext()) {
											// Get the current material
											const Material *pMaterial = cMaterialIterator.Next();

											// Make the material to the currently used one
											GeneratedProgramUserData *pGeneratedProgramUserData = MakeMaterialCurrent(cRenderer, *pMaterial, fInvRadius);
											if (pGeneratedProgramUserData) {
												// Draw all mesh batches using this material
												Iterator<MeshBatch*> cMeshBatchIterator = m_lstMeshBatches.GetIterator();
												while (cMeshBatchIterator.HasNext()) {
													// Get the current mesh batch
													MeshBatch *pMeshBatch = cMeshBatchIterator.Next();

													// Same material?
													if (pMeshBatch->pMaterial == pMaterial) {
														// Draw the mesh batch
														DrawMeshBatch(cRenderer, *pGeneratedProgramUserData, *pMeshBatch);
													}
												}
											}
										}
										{ // Free the mesh batches
											Iterator<MeshBatch*> cMeshBatchIterator = m_lstMeshBatches.GetIterator();
											while (cMeshBatchIterator.HasNext())
												m_lstFreeMeshBatches.Add(cMeshBatchIterator.Next());
											m_lstMeshBatches.Clear();
										}
										m_lstMaterials.Clear();
									}
								}
							}
						} else {
							SNPointLight &cPointLight = static_cast<SNPointLight&>(cLight);

							// Enable color writes - this shadow map technique requires it!
							cRenderer.SetColorMask();

							// For each of the 6 cube faces...
							Vector3 vRot;
							for (uint8 nFace=0; nFace<6; nFace++) {
								// Get rotation
								switch (nFace) {
									// x-positive
									case 0:
										vRot.SetXYZ(  0.0f,  90.0f, 180.0f);
										break;

									// x-negative
									case 1:
										vRot.SetXYZ(  0.0f, -90.0f, 180.0f);
										break;

									// y-positive
									case 2:
										vRot.SetXYZ(-90.0f, 180.0f, 180.0f);
										break;

									// y-negative
									case 3:
										vRot.SetXYZ( 90.0f, 180.0f, 180.0f);
										break;

									// z-positive
									case 4:
										vRot.SetXYZ(  0.0f, 180.0f, 180.0f);
										break;

									// z-negative
									case 5:
										vRot.SetXYZ(  0.0f,   0.0f, 180.0f);
										break;
								}
								Quaternion qRot;
								EulerAngles::ToQuaternion(static_cast<float>(vRot.x*Math::DegToRad), static_cast<float>(vRot.y*Math::DegToRad), static_cast<float>(vRot.z*Math::DegToRad), qRot);

								// Calculate and set projection matrix
								m_mLightProjection.PerspectiveFov(static_cast<float>(90.0f*Math::DegToRad), 1.0f, 0.01f, cPointLight.GetRange());

								// Calculate and set view matrix
								m_mLightView.View(qRot, cLight.GetTransform().GetPosition());

								// Concatenate (multiply) the view matrix and the projection matrix
								m_mLightViewProjection = m_mLightProjection;
								m_mLightViewProjection *= m_mLightView;

								// Calculate the light frustum
								Frustum cLightFrustum;
								cLightFrustum.CreateViewPlanes(m_mLightViewProjection);

								// Setup render query
								pLightCullQuery->SetCameraContainer(cLight.GetContainer()->IsCell() ? cLight.GetContainer() : nullptr);
								pLightCullQuery->SetCameraPosition(cLight.GetTransform().GetPosition());
								pLightCullQuery->SetViewFrustum(cLightFrustum);
								pLightCullQuery->SetProjectionMatrix(m_mLightProjection);
								pLightCullQuery->SetViewMatrix(m_mLightView);
								pLightCullQuery->SetViewProjectionMatrix(m_mLightViewProjection);

								// Set the new render target
								if (m_pCurrentCubeShadowRenderTarget && cRenderer.SetRenderTarget(*m_pCurrentCubeShadowRenderTarget, nFace)) {
									// Perform the visibility determination
									pLightCullQuery->PerformQuery();

									// Clear the frame buffer
									cRenderer.Clear(Clear::Color|Clear::ZBuffer, PLGraphics::Color4::White);

									// Collect recursive
									CollectMeshBatchesRec(*pLightCullQuery);

									{ // Loop through all currently used materials
										Iterator<const Material*> cMaterialIterator = m_lstMaterials.GetIterator();
										while (cMaterialIterator.HasNext()) {
											// Get the current material
											const Material *pMaterial = cMaterialIterator.Next();

											// Make the material to the currently used one
											GeneratedProgramUserData *pGeneratedProgramUserData = MakeMaterialCurrent(cRenderer, *pMaterial, fInvRadius);
											if (pGeneratedProgramUserData) {
												// Draw all mesh batches using this material
												Iterator<MeshBatch*> cMeshBatchIterator = m_lstMeshBatches.GetIterator();
												while (cMeshBatchIterator.HasNext()) {
													// Get the current mesh batch
													MeshBatch *pMeshBatch = cMeshBatchIterator.Next();

													// Same material?
													if (pMeshBatch->pMaterial == pMaterial) {
														// Draw the mesh batch
														DrawMeshBatch(cRenderer, *pGeneratedProgramUserData, *pMeshBatch);
													}
												}
											}
										}
										{ // Free the mesh batches
											Iterator<MeshBatch*> cMeshBatchIterator = m_lstMeshBatches.GetIterator();
											while (cMeshBatchIterator.HasNext())
												m_lstFreeMeshBatches.Add(cMeshBatchIterator.Next());
											m_lstMeshBatches.Clear();
										}
										m_lstMaterials.Clear();
									}
								}
							}
						}

						// Reset renderer target
						cRenderer.SetRenderTarget(pSurfaceBackup);

						// Restore the color mask
						cRenderer.SetColorMask(bRed, bGreen, bBlue, bAlpha);
					}
				}
			}
		}
	}
}

/**
*  @brief
*    Returns the render target for the cube shadow map
*/
SurfaceTextureBuffer *SRPShadowMapping::GetCubeShadowRenderTarget() const
{
	return m_pCurrentCubeShadowRenderTarget ? *m_pCurrentCubeShadowRenderTarget : nullptr;
}

/**
*  @brief
*    Returns the render target for the spot shadow map
*/
SurfaceTextureBuffer *SRPShadowMapping::GetSpotShadowRenderTarget() const
{
	return m_pCurrentSpotShadowRenderTarget ? *m_pCurrentSpotShadowRenderTarget : nullptr;
}

/**
*  @brief
*    Returns the used combined light view projection matrix
*/
const Matrix4x4 &SRPShadowMapping::GetLightViewProjectionMatrix() const
{
	return m_mLightViewProjection;
}

/**
*  @brief
*    Returns the used light projection matrix
*/
const Matrix4x4 &SRPShadowMapping::GetLightProjectionMatrix() const
{
	return m_mLightProjection;
}

/**
*  @brief
*    Returns the used light view matrix
*/
const Matrix4x4 &SRPShadowMapping::GetLightViewMatrix() const
{
	return m_mLightView;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Collect mesh batches recursive
*/
void SRPShadowMapping::CollectMeshBatchesRec(const SQCull &cCullQuery)
{
	// Get scene container
	const VisContainer &cVisContainer = cCullQuery.GetVisContainer();

	// Loop through all visible scene nodes of this scene container
	Iterator<VisNode*> cIterator = cVisContainer.GetVisNodes().GetIterator();
	while (cIterator.HasNext()) {
		// Get visibility node and scene node
		const VisNode   *pVisNode   = cIterator.Next();
			  SceneNode *pSceneNode = pVisNode->GetSceneNode();

		// Can this scene node product shadows? LOOK OUT: This may result in that objects behind portals don't
		// cast shadows if the light source is on the other side of the portal! (may or may not be desired!)
		if (pSceneNode && (pSceneNode->GetFlags() & SceneNode::CastShadow)) {
			// Is this scene node a portal?
			if (pVisNode->IsPortal()) {
				// Get the target cell visibility container
				const VisContainer *pVisCell = static_cast<const VisPortal*>(pVisNode)->GetTargetVisContainer();
				if (pVisCell && pVisCell->GetCullQuery())
					CollectMeshBatchesRec(*pVisCell->GetCullQuery());

			// Is this scene node a container? We do not need to check for cells because we will
			// NEVER receive cells from SQCull directly, they are ONLY visible through portals! (see above)
			} else if (pVisNode->IsContainer()) {
				// Collect this container without special processing
				if (static_cast<const VisContainer*>(pVisNode)->GetCullQuery())
					CollectMeshBatchesRec(*static_cast<const VisContainer*>(pVisNode)->GetCullQuery());

			// This must just be a quite boring scene node :)
			} else {
				// Here we draw the stuff by hand in order to minimize state changes and other overhead
				const MeshHandler *pMeshHandler = pSceneNode->GetMeshHandler();
				if (pMeshHandler && pMeshHandler->GetVertexBuffer() && pMeshHandler->GetNumOfMaterials()) {
					// Get the used mesh
					const Mesh *pMesh = pMeshHandler->GetResource();
					if (pMesh) {
						// Get the mesh LOD level to use
						const MeshLODLevel *pLODLevel = pMesh->GetLODLevel(0);
						if (pLODLevel && pLODLevel->GetIndexBuffer() && pMeshHandler->GetVertexBuffer()) {
							// Loop through all mesh geometries
							const Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
							for (uint32 nGeo=0; nGeo<lstGeometries.GetNumOfElements(); nGeo++) {
								// Is this geometry active and is it using the current used mesh material?
								const Geometry &cGeometry = lstGeometries[nGeo];
								if (cGeometry.IsActive()) {
									// Get the material the mesh geometry is using
									const Material *pMaterial = pMeshHandler->GetMaterial(cGeometry.GetMaterial());
									if (pMaterial) {
										// Transparent material? Only solid materials can cast a shadow.
										static const String sOpacity = "Opacity";
										const Parameter *pParameter = pMaterial->GetParameter(sOpacity);
										if (!pParameter || pParameter->GetValue1f() >= 1.0f) {
											// Get a free mesh batch
											MeshBatch &cMeshBatch = GetFreeMeshBatch();
											m_lstMeshBatches.Add(&cMeshBatch);

											// Fill the mesh batch
											cMeshBatch.pMaterial			= pMaterial;
											cMeshBatch.pVertexBuffer		= pMeshHandler->GetVertexBuffer();
											cMeshBatch.pIndexBuffer			= pLODLevel->GetIndexBuffer();
											cMeshBatch.pCullQuery			= &cCullQuery;
											cMeshBatch.pGeometry			= &cGeometry;
											cMeshBatch.pVisNode				= pVisNode;
											cMeshBatch.sScissorRectangle	= cVisContainer.GetProjection().cRectangle;

											// Add material
											if (!m_lstMaterials.IsElement(pMaterial))
												m_lstMaterials.Add(pMaterial);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

/**
*  @brief
*    Makes a material to the currently used one
*/
SRPShadowMapping::GeneratedProgramUserData *SRPShadowMapping::MakeMaterialCurrent(Renderer &cRenderer, const Material &cMaterial, float fInvRadius)
{
	const Parameter *pParameter = nullptr;

	{ // Two sided? Setup cull mode...
		static const String sTwoSided = "TwoSided";
		pParameter = cMaterial.GetParameter(sTwoSided);
		cRenderer.SetRenderState(RenderState::CullMode, (pParameter && pParameter->GetValue1f() == 1.0f) ? Cull::None : Cull::CCW);
	}

	// Get diffuse map and alpha reference
	float fAlphaReference = 0.0f;
	TextureBuffer *pDiffuseMap = cMaterial.GetParameterTextureBuffer(Material::DiffuseMap);

	// Enable/disable alpha test (fragments are thrown away inside the fragment shader using the 'discard' keyword)
	if (pDiffuseMap && pDiffuseMap->GetComponentsPerPixel() == 4) {
		// Get alpha reference
		static const String sAlphaReference = "AlphaReference";
		pParameter = cMaterial.GetParameter(sAlphaReference);
		fAlphaReference = pParameter ? pParameter->GetValue1f() : 0.5f;
	}

	// Reset the program flags
	m_cProgramFlags.Reset();

	// Set fragment program flags - we only need the diffuse map for alpha test
	if (fAlphaReference) {
		PL_ADD_VS_FLAG(m_cProgramFlags, VS_TEXCOORD0)
		PL_ADD_FS_FLAG(m_cProgramFlags, FS_ALPHATEST)
	}

	// Get a program instance from the program generator using the given program flags
	ProgramGenerator::GeneratedProgram *pGeneratedProgram = m_pProgramGenerator->GetProgram(m_cProgramFlags);

	// Make our program to the current one
	GeneratedProgramUserData *pGeneratedProgramUserData = nullptr;
	if (pGeneratedProgram && cRenderer.SetProgram(pGeneratedProgram->pProgram)) {
		// Set pointers to uniforms & attributes of a generated program if they are not set yet
		pGeneratedProgramUserData = static_cast<GeneratedProgramUserData*>(pGeneratedProgram->pUserData);
		if (!pGeneratedProgramUserData) {
			pGeneratedProgram->pUserData = pGeneratedProgramUserData = new GeneratedProgramUserData;
			Program *pProgram = pGeneratedProgram->pProgram;
			// Vertex shader attributes
			static const String sVertexPosition = "VertexPosition";
			pGeneratedProgramUserData->pVertexPosition	= pProgram->GetAttribute(sVertexPosition);
			static const String sVertexTexCoord0 = "VertexTexCoord0";
			pGeneratedProgramUserData->pVertexTexCoord0	= pProgram->GetAttribute(sVertexTexCoord0);
			// Vertex shader uniforms
			static const String sWorldVP = "WorldVP";
			pGeneratedProgramUserData->pWorldVP			= pProgram->GetUniform(sWorldVP);
			static const String sWorldV = "WorldV";
			pGeneratedProgramUserData->pWorldV			= pProgram->GetUniform(sWorldV);
			static const String sInvRadius = "InvRadius";
			pGeneratedProgramUserData->pInvRadius		= pProgram->GetUniform(sInvRadius);
			// Fragment shader uniforms
			static const String sDiffuseMap = "DiffuseMap";
			pGeneratedProgramUserData->pDiffuseMap		= pProgram->GetUniform(sDiffuseMap);
			static const String sAlphaReference = "AlphaReference";
			pGeneratedProgramUserData->pAlphaReference	= pProgram->GetUniform(sAlphaReference);
		}

		// Set world space inverse light radius
		if (pGeneratedProgramUserData->pInvRadius)
			pGeneratedProgramUserData->pInvRadius->Set(fInvRadius);

		// Diffuse
		if (pGeneratedProgramUserData->pDiffuseMap) {
			// Set the "DiffuseMap" fragment shader parameter
			const int nTextureUnit = pGeneratedProgramUserData->pDiffuseMap->Set(pDiffuseMap);
			if (nTextureUnit >= 0) {
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::Linear);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::Linear);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::Linear);
			}

			// Set the "AlphaReference" fragment shader parameter
			if (pGeneratedProgramUserData->pAlphaReference)
				pGeneratedProgramUserData->pAlphaReference->Set(fAlphaReference);
		}
	}

	// Done
	return pGeneratedProgramUserData;
}

/**
*  @brief
*    Draws a mesh batch
*/
void SRPShadowMapping::DrawMeshBatch(Renderer &cRenderer, GeneratedProgramUserData &cGeneratedProgramUserData, MeshBatch &cMeshBatch) const
{
	// Get the mesh batch vertex buffer
	VertexBuffer *pVertexBuffer = cMeshBatch.pVertexBuffer;

	{ // Set per mesh batch program parameters
		// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
		if (cGeneratedProgramUserData.pVertexPosition)
			cGeneratedProgramUserData.pVertexPosition->Set(pVertexBuffer, PLRenderer::VertexBuffer::Position);
		if (cGeneratedProgramUserData.pVertexTexCoord0)
			cGeneratedProgramUserData.pVertexTexCoord0->Set(pVertexBuffer, PLRenderer::VertexBuffer::TexCoord);

		// Get the mesh batch visibility node
		const VisNode *pVisNode = cMeshBatch.pVisNode;

		// Set world view projection matrix
		if (cGeneratedProgramUserData.pWorldVP)
			cGeneratedProgramUserData.pWorldVP->Set(pVisNode->GetWorldViewProjectionMatrix());

		// Set world view matrix
		if (cGeneratedProgramUserData.pWorldV)
			cGeneratedProgramUserData.pWorldV->Set(pVisNode->GetWorldViewMatrix());
	}

	// Bind index buffer
	cRenderer.SetIndexBuffer(cMeshBatch.pIndexBuffer);

	// Set the new scissor rectangle
	cRenderer.SetScissorRect(&cMeshBatch.sScissorRectangle);

	// Get the mesh patch geometry
	const Geometry *pGeometry = cMeshBatch.pGeometry;

	// Draw the geometry
	cRenderer.DrawIndexedPrimitives(
		pGeometry->GetPrimitiveType(),
		0,
		pVertexBuffer->GetNumOfElements()-1,
		pGeometry->GetStartIndex(),
		pGeometry->GetIndexSize()
	);
}

/**
*  @brief
*    Returns a free mesh batch
*/
SRPShadowMapping::MeshBatch &SRPShadowMapping::GetFreeMeshBatch()
{
	if (m_lstFreeMeshBatches.IsEmpty())
		return *(new MeshBatch);
	else {
		MeshBatch *pMeshBatch = m_lstFreeMeshBatches.Get(0);
		m_lstFreeMeshBatches.RemoveAtIndex(0);
		return *pMeshBatch;
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRPShadowMapping::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Not used
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
