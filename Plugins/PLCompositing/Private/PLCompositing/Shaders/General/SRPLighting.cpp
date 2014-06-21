/*********************************************************\
 *  File: SRPLighting.cpp                                *
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
#include <float.h>
#include <PLMath/Matrix3x3.h>
#include <PLMath/Intersect.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Renderer/RenderStates.h>
#include <PLRenderer/Renderer/TextureBuffer2D.h>
#include <PLRenderer/Renderer/TextureBufferCube.h>
#include <PLRenderer/Renderer/Shader.h>
#include <PLRenderer/Renderer/Program.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ProgramAttribute.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLRenderer/Material/Parameter.h>
#include <PLRenderer/Material/MaterialManager.h>
#include <PLRenderer/Texture/TextureManager.h>
#include <PLMesh/MeshHandler.h>
#include <PLMesh/MeshLODLevel.h>
#include <PLScene/Scene/SNProjectiveSpotLight.h>
#include <PLScene/Scene/SNProjectivePointLight.h>
#include <PLScene/Scene/SNCellPortal.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLScene/Scene/SceneNodeModifier.h>
#include <PLScene/Visibility/SQCull.h>
#include <PLScene/Visibility/VisPortal.h>
#include <PLScene/Visibility/VisContainer.h>
#include "PLCompositing/Shaders/ShadowMapping/SRPShadowMapping.h"
#include "PLCompositing/Shaders/General/SRPDirectionalLightingShaders.h"
#include "PLCompositing/Shaders/General/SRPLightingMaterial.h"
#include "PLCompositing/Shaders/General/SRPLighting.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLMesh;
using namespace PLScene;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SRPLighting)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPLighting::SRPLighting() :
	TextureFiltering(this),
	ShaderLanguage(this),
	LightingIntensity(this),
	Flags(this),
	EventHandlerMaterialRemoved(&SRPLighting::OnMaterialRemoved, this),
	m_pIgnoredLight(nullptr),
	m_pProgramGenerator(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
SRPLighting::~SRPLighting()
{
	{ // SRPLighting-material cache cleanup
		Iterator<SRPLightingMaterial*> lstIterator = m_lstMaterialCache.GetIterator();
		while (lstIterator.HasNext())
			delete lstIterator.Next();
		m_lstMaterialCache.Clear();
	}

	// Destroy the program generator
	if (m_pProgramGenerator)
		delete m_pProgramGenerator;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets the initial render states
*/
void SRPLighting::SetInitialRenderStates(Renderer &cRenderer)
{
	// Setup render states
	cRenderer.GetRendererContext().GetEffectManager().Use();
	cRenderer.SetRenderState(RenderState::ZWriteEnable,		 false);
	cRenderer.SetRenderState(RenderState::BlendEnable,		 true);
	cRenderer.SetRenderState(RenderState::SrcBlendFunc,		 BlendFunc::One);
	cRenderer.SetRenderState(RenderState::DstBlendFunc,		 BlendFunc::One);
	cRenderer.SetRenderState(RenderState::ScissorTestEnable, true);
}

/**
*  @brief
*    Sets correct texture filtering modes
*/
void SRPLighting::SetupTextureFiltering(Renderer &cRenderer, uint32 nStage) const
{
	// Anisotropic filtering
	if (TextureFiltering > 1) {
		cRenderer.SetSamplerState(nStage, Sampler::MagFilter,     TextureFiltering::Anisotropic);
		cRenderer.SetSamplerState(nStage, Sampler::MinFilter,     TextureFiltering::Anisotropic);
		cRenderer.SetSamplerState(nStage, Sampler::MipFilter,     TextureFiltering::Anisotropic);
		cRenderer.SetSamplerState(nStage, Sampler::MaxAnisotropy, TextureFiltering);

	// Bilinear filtering
	} else if (TextureFiltering > 0) {
		cRenderer.SetSamplerState(nStage, Sampler::MagFilter, TextureFiltering::Linear);
		cRenderer.SetSamplerState(nStage, Sampler::MinFilter, TextureFiltering::Linear);
		cRenderer.SetSamplerState(nStage, Sampler::MipFilter, TextureFiltering::Linear);

	// No filtering
	} else {
		cRenderer.SetSamplerState(nStage, Sampler::MagFilter, TextureFiltering::None);
		cRenderer.SetSamplerState(nStage, Sampler::MinFilter, TextureFiltering::None);
		cRenderer.SetSamplerState(nStage, Sampler::MipFilter, TextureFiltering::None);
	}
}

/**
*  @brief
*    Draws recursive
*/
void SRPLighting::DrawRec(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Get scene container
	const VisContainer &cVisContainer = cCullQuery.GetVisContainer();

	// Set the new scissor rectangle
	cRenderer.SetScissorRect(&cVisContainer.GetProjection().cRectangle);

	// Render all visible scene nodes of this scene container
	Iterator<VisNode*> cIterator = cVisContainer.GetVisNodes().GetIterator();
	while (cIterator.HasNext()) {
		// Get visibility node and scene node
		const VisNode   *pVisNode   = cIterator.Next();
			  SceneNode *pSceneNode = pVisNode->GetSceneNode();
		if (pSceneNode) {
			// Is this scene node a portal?
			if (pVisNode->IsPortal()) {
				// Get the target cell visibility container
				const VisContainer *pVisCell = static_cast<const VisPortal*>(pVisNode)->GetTargetVisContainer();
				if (pVisCell && pVisCell->GetCullQuery()) {
					// Draw the target cell
					DrawRec(cRenderer, *pVisCell->GetCullQuery());

					// Set the previous scissor rectangle
					cRenderer.SetScissorRect(&cVisContainer.GetProjection().cRectangle);
				}

			// Is this scene node a container? We do not need to check for cells because we will
			// NEVER receive cells from SQCull directly, they are ONLY visible through portals! (see above)
			} else if (pVisNode->IsContainer()) {
				// Draw this container without special processing
				if (static_cast<const VisContainer*>(pVisNode)->GetCullQuery()) {
					DrawRec(cRenderer, *static_cast<const VisContainer*>(pVisNode)->GetCullQuery());

					// Set the previous scissor rectangle
					cRenderer.SetScissorRect(&cVisContainer.GetProjection().cRectangle);
				}

			// Is this a light?
			} else if (pSceneNode->IsLight()) {
				// Use this light?
				if (pSceneNode != m_pIgnoredLight && static_cast<SNLight*>(pSceneNode)->IsRenderLight()) {
					SNLight &cLight = *static_cast<SNLight*>(pSceneNode);

					// Skip black lights, they have no visible influence!
					if (cLight.Color.Get() != Color3::Black)
						RenderLight(cRenderer, cCullQuery, cLight, *pVisNode);
				}

			// This must just be a quite boring scene node, ignore it
			} else {
			}
		}
	}
}

/**
*  @brief
*    Renders a light
*/
void SRPLighting::RenderLight(Renderer &cRenderer, const SQCull &cCullQuery, SNLight &cLight, const VisNode &cLightVisNode)
{
	// Get the shadow mapping scene renderer pass
	SRPShadowMapping *pSRPShadowMapping = nullptr;
	// [TODO] Currently, shadow mapping is deactivated on the Android platform
	#ifndef ANDROID
		static const String sClassName = "PLCompositing::SRPShadowMapping";
		pSRPShadowMapping = static_cast<SRPShadowMapping*>(GetFirstInstanceOfSceneRendererPassClass(sClassName));

		// Is the shadow mapping scene renderer pass active?
		if (pSRPShadowMapping && !pSRPShadowMapping->IsActive())
			pSRPShadowMapping = nullptr;	// Just do like there's no shadow mapping scene renderer pass at all

		// Updates shadow maps
		if (!(GetFlags() & NoShadow) && (cLight.GetFlags() & SNLight::CastShadow) && pSRPShadowMapping) {
			pSRPShadowMapping->UpdateShadowMap(cRenderer, cLight, cCullQuery, cLightVisNode.GetSquaredDistanceToCamera());

			// Sets the initial render states
			SetInitialRenderStates(cRenderer);
		}
	#endif

	// Is lighting allowed for this scene container?
	if (!(cCullQuery.GetSceneContainer().GetFlags() & SceneNode::NoLighting))
		RenderLightRec(cRenderer, cCullQuery, cLight, cLightVisNode, pSRPShadowMapping);
}

/**
*  @brief
*    Renders a light recursive
*/
void SRPLighting::RenderLightRec(Renderer &cRenderer, const SQCull &cCullQuery, SNLight &cLight, const VisNode &cLightVisNode, SRPShadowMapping *pSRPShadowMapping)
{
	// Set cull mode
	cRenderer.SetRenderState(RenderState::CullMode, Cull::CCW);

	// Get scene container
	const VisContainer &cVisContainer = cCullQuery.GetVisContainer();

	// Set the new scissor rectangle
	cRenderer.SetScissorRect(&cVisContainer.GetProjection().cRectangle);

	// Render all visible scene nodes of this scene container
	Iterator<VisNode*> cIterator = cVisContainer.GetVisNodes().GetIterator();
	while (cIterator.HasNext()) {
		// Get visibility node and scene node
		const VisNode   *pVisNode   = cIterator.Next();
			  SceneNode *pSceneNode = pVisNode->GetSceneNode();

		// Is lighting allowed for this scene node?
		if (pSceneNode && !(pSceneNode->GetFlags() & SceneNode::NoLighting)) {
			// Is this scene node a portal?
			if (pVisNode->IsPortal()) {
				// Get the target cell visibility container
				const VisPortal    *pVisPortal = static_cast<const VisPortal*>(pVisNode);
				const VisContainer *pVisCell   = pVisPortal->GetTargetVisContainer();
				if (pVisCell && pVisCell->GetCullQuery() && pVisPortal->GetSceneNode()) {
					// [TODO] Find a better solution without changing temporarily the light variables
					// The next thing is a little bit tricky: We need to change light variables
					// temporarily...

					// Backup the old position, rotation and scale
					Vector3    vOldPosition = cLight.GetTransform().GetPosition();
					Quaternion qOldRotation = cLight.GetTransform().GetRotation();
					Vector3    vOldScale    = cLight.GetTransform().GetScale();

					// Update position, scale and rotation using the given warp matrix of the portal
					const Matrix3x4 &mWarp = static_cast<SNCellPortal*>(pVisPortal->GetSceneNode())->GetWarpMatrix();
					// New position
					Vector3 vPosition = mWarp*vOldPosition;
					cLight.GetTransform().SetPosition(vPosition);
					// New rotation
					Quaternion qRotation = qOldRotation*mWarp;
					cLight.GetTransform().SetRotation(qRotation);
					// New scale
					Vector3 vScale = vOldScale;
					mWarp.RotateVector(vScale);
					cLight.GetTransform().SetScale(vScale);

					// Draw the target cell
					RenderLightRec(cRenderer, *pVisCell->GetCullQuery(), cLight, cLightVisNode, pSRPShadowMapping);

					// [TODO] Use new PLMath::Transform3 features!
					// Restore the old position, rotation and scale
					cLight.GetTransform().SetPosition(vOldPosition);
					cLight.GetTransform().SetRotation(qOldRotation);
					cLight.GetTransform().SetScale(vOldScale);

					// Set the previous scissor rectangle
					cRenderer.SetScissorRect(&cVisContainer.GetProjection().cRectangle);
				}

			// Is this scene node a container? We do not need to check for cells because we will
			// NEVER receive cells from SQCull directly, they are ONLY visible through portals! (see above)
			} else if (pVisNode->IsContainer()) {
				// Draw this container without special processing
				if (static_cast<const VisContainer*>(pVisNode)->GetCullQuery()) {
					RenderLightRec(cRenderer, *static_cast<const VisContainer*>(pVisNode)->GetCullQuery(), cLight, cLightVisNode, pSRPShadowMapping);

					// Set the previous scissor rectangle
					cRenderer.SetScissorRect(&cVisContainer.GetProjection().cRectangle);
				}

			// Is this a light, if so, ignore it
			} else if (pSceneNode->IsLight()) {

			// This must just be a quite boring scene node :)
			} else {
				// First at all, check sphere/axis aligned bounding box intersection of the used light and the scene node
				if (Intersect::SphereSphere(cLight.GetContainerBoundingSphere(), pSceneNode->GetContainerBoundingSphere()) &&
					Intersect::SphereAABox(cLight.GetContainerBoundingSphere(), pSceneNode->GetContainerAABoundingBox())) {
					// If this is a spot light, we can do another intersection test
					if (!cLight.IsSpotLight() ||
						Intersect::PlaneSetAABox(static_cast<SNSpotLight&>(cLight).GetFrustum(), pSceneNode->GetContainerAABoundingBox().vMin, pSceneNode->GetContainerAABoundingBox().vMax)) {

						// Here we draw the stuff by hand in order to minimize state changes and other overhead
						const MeshHandler *pMeshHandler = pSceneNode->GetMeshHandler();
						if (pMeshHandler && pMeshHandler->GetVertexBuffer() && pMeshHandler->GetNumOfMaterials()) {
							// Get the used mesh
							const Mesh *pMesh = pMeshHandler->GetResource();
							if (pMesh) {
								// Get the mesh LOD level to use
								const MeshLODLevel *pLODLevel = pMesh->GetLODLevel(0);
								if (pLODLevel && pLODLevel->GetIndexBuffer()) {
									// Get the vertex buffer which needs at least a position attribute
									VertexBuffer *pVertexBuffer = pMeshHandler->GetVertexBuffer();
									if (pVertexBuffer && pVertexBuffer->GetVertexAttribute(VertexBuffer::Position)) {
										// Draw the mesh
										DrawMesh(cRenderer, cCullQuery, *pVisNode, *pSceneNode, *pMeshHandler, *pMesh, *pLODLevel, *pVertexBuffer, cLight, cLightVisNode, pSRPShadowMapping);
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
*    Draws a mesh
*/
void SRPLighting::DrawMesh(Renderer &cRenderer, const SQCull &cCullQuery, const VisNode &cVisNode, SceneNode &cSceneNode, const MeshHandler &cMeshHandler, const Mesh &cMesh, const MeshLODLevel &cMeshLODLevel,
						   VertexBuffer &cVertexBuffer, SNLight &cLight, const VisNode &cLightVisNode, SRPShadowMapping *pSRPShadowMapping)
{
	// Figure out the type of the given light
	const bool bPoint			= cLight.IsPointLight();
	const bool bProjectivePoint	= (GetFlags() & NoProjectivePointLights) ? false : (cLight.IsProjectivePointLight() && !(cLight.GetFlags() & SNProjectivePointLight::NoProjection));
	const bool bSpot			= cLight.IsSpotLight();
	const bool bProjectiveSpot	= (GetFlags() & NoProjectiveSpotLights) ? false : (cLight.IsProjectiveSpotLight() && !(cLight.GetFlags() & SNProjectiveSpotLight::NoProjection));
	const bool bDirectional		= cLight.IsDirectionalLight();

	// Get the light range
	const float fRange = bPoint ? static_cast<SNPointLight&>(cLight).GetRange() : 0.0f;

	// Skip none directional lights with a radius of <= 0, they have no visible influence!
	if (!bDirectional && fRange <= 0.0f)
		return;	// Get us out of here right now

	// Use spot cone or even smooth spot cone for spot lights?
	bool bSpotCone       = false;
	bool bSpotSmoothCone = false;
	if (bSpot && !(cLight.GetFlags() & SNSpotLight::NoCone)) {
		// A spot cone is used
		bSpotCone = true;

		// The inner angle MUST be smaller than the outer one, else we do not use a smooth cone
		bSpotSmoothCone = static_cast<SNSpotLight&>(cLight).GetOuterAngle() > static_cast<SNSpotLight&>(cLight).GetInnerAngle();
	}

	// Get the light color
	const Color3 cLightColor = cLight.Color.Get()*LightingIntensity;

	// Shadow mapping?
	const bool bShadow = (!(GetFlags() & NoShadow) && (cLight.GetFlags() & SNLight::CastShadow) && pSRPShadowMapping && (pSRPShadowMapping->GetCubeShadowRenderTarget() || pSRPShadowMapping->GetSpotShadowRenderTarget()));
	TextureBuffer *pShadowMap = nullptr;
	if (bShadow) {
		// Get the shadow map
		if (bSpot) {
			if (pSRPShadowMapping->GetSpotShadowRenderTarget())
				pShadowMap = pSRPShadowMapping->GetSpotShadowRenderTarget()->GetTextureBuffer();
		} else {
			if (pSRPShadowMapping->GetCubeShadowRenderTarget())
				pShadowMap = pSRPShadowMapping->GetCubeShadowRenderTarget()->GetTextureBuffer();
		}
	}

	// Get scene container
	const VisContainer &cVisContainer = cCullQuery.GetVisContainer();

	// Get buffers
		  IndexBuffer     *pIndexBuffer  = cMeshLODLevel.GetIndexBuffer();
	const Array<Geometry> &lstGeometries = *cMeshLODLevel.GetGeometries();

	// Bind buffers
	cRenderer.SetIndexBuffer(pIndexBuffer);

	// Get available vertex buffer attributes
	// Binormal and tangent make only sense in this usage when there's also a normal, we need all three vectors!
	const bool bHasVertexTexCoord0 = (cVertexBuffer.GetVertexAttribute(VertexBuffer::TexCoord, 0) != nullptr);	// e.g. for diffuse maps
	const bool bHasVertexNormal    = (cVertexBuffer.GetVertexAttribute(VertexBuffer::Normal) != nullptr);
		  bool bHasVertexTangent   = bHasVertexNormal && (cVertexBuffer.GetVertexAttribute(VertexBuffer::Tangent) != nullptr);
	const bool bHasVertexBinormal  = bHasVertexTangent && (cVertexBuffer.GetVertexAttribute(VertexBuffer::Binormal) != nullptr);

	// For better readability, define whether or not normal mapping is possible with the given vertex data
	const bool bNormalMappingPossible = bHasVertexBinormal;	// We don't need to check for all three vectors in here :D

	// [TODO] Cleanup
	uint32 nEnvironmentFlags = 0;
	if (bHasVertexNormal)
		nEnvironmentFlags |= SRPLightingMaterial::EnvironmentVertexNormal;
	if (bHasVertexTexCoord0)
		nEnvironmentFlags |= SRPLightingMaterial::EnvironmentVertexTexCoord0;
	if (bNormalMappingPossible)
		nEnvironmentFlags |= SRPLightingMaterial::EnvironmentNormalMappingPossible;
	if (bDirectional)
		nEnvironmentFlags |= SRPLightingMaterial::EnvironmentDirectionalLight;
	if (bProjectivePoint)
		nEnvironmentFlags |= SRPLightingMaterial::EnvironmentProjectivePointLight;
	if (bSpot)
		nEnvironmentFlags |= SRPLightingMaterial::EnvironmentSpotLight;
	if (bProjectiveSpot)
		nEnvironmentFlags |= SRPLightingMaterial::EnvironmentProjectiveSpotLight;
	if (bSpotCone)
		nEnvironmentFlags |= SRPLightingMaterial::EnvironmentSpotLightCone;
	if (bSpotSmoothCone)
		nEnvironmentFlags |= SRPLightingMaterial::EnvironmentSpotLightSmoothCone;
	if (pShadowMap)
		nEnvironmentFlags |= SRPLightingMaterial::EnvironmentShadow;

	// Draw mesh
	for (uint32 nMat=0; nMat<cMeshHandler.GetNumOfMaterials(); nMat++) {
		// Get mesh material
		Material *pMaterial = cMeshHandler.GetMaterial(nMat);
		if (pMaterial) {
			// Draw geometries
			for (uint32 nGeo=0; nGeo<lstGeometries.GetNumOfElements(); nGeo++) {
				// Is this geometry active and is it using the current used mesh material?
				const Geometry &cGeometry = lstGeometries[nGeo];
				if (cGeometry.IsActive() && nMat == cGeometry.GetMaterial()) {
					// Transparent material? - Only none transparent objects can be lit by using this scene renderer pass...
					static const String sOpacity = "Opacity";
					const Parameter *pParameter = pMaterial->GetParameter(sOpacity);
					if (!pParameter || pParameter->GetValue1f() >= 1.0f) {
						// SRPLighting-material caching
						SRPLightingMaterial *pSRPLightingMaterial = m_lstMaterialCache.Get(reinterpret_cast<uint64>(pMaterial));
						if (!pSRPLightingMaterial) {
							// The material is not yet cached
							pSRPLightingMaterial = new SRPLightingMaterial(*pMaterial, *m_pProgramGenerator);
							m_lstMaterialCache.Add(reinterpret_cast<uint64>(pMaterial), pSRPLightingMaterial);
						}

						// [TODO] Correct texture filter
						SRPLightingMaterial::GeneratedProgramUserData *pGeneratedProgramUserData = pSRPLightingMaterial->MakeMaterialCurrent(GetFlags(), nEnvironmentFlags, SRPLightingMaterial::Anisotropic2, LightingIntensity);
						if (pGeneratedProgramUserData) {
							// Set the "ViewSpaceToWorldSpace" fragment shader parameter
							if (pGeneratedProgramUserData->pViewSpaceToWorldSpace) {
								// [TODO] Add *SQCullQuery::GetInvViewMatrix()?
								Matrix3x3 mRot = cCullQuery.GetViewMatrix().GetInverted();
								pGeneratedProgramUserData->pViewSpaceToWorldSpace->Set(mRot);
							}

							// Directional light?
							if (bDirectional) {
								// Set normalized view space light direction vector
								if (pGeneratedProgramUserData->pLightDirection) {
									Vector3 vLightDirection = cLightVisNode.GetWorldViewMatrix().GetZAxis();
									vLightDirection.Invert();
									vLightDirection.Normalize();
									pGeneratedProgramUserData->pLightDirection->Set(vLightDirection);
								}
							} else {
								// Set object space to clip space matrix uniform
								if (pGeneratedProgramUserData->pObjectSpaceToClipSpaceMatrix)
									pGeneratedProgramUserData->pObjectSpaceToClipSpaceMatrix->Set(cVisNode.GetWorldViewProjectionMatrix());

								// Set object space to view space matrix uniform
								if (pGeneratedProgramUserData->pObjectSpaceToViewSpaceMatrix)
									pGeneratedProgramUserData->pObjectSpaceToViewSpaceMatrix->Set(cVisNode.GetWorldViewMatrix());

								// Set object space eye position
								if (pGeneratedProgramUserData->pEyePos)
									pGeneratedProgramUserData->pEyePos->Set(cVisNode.GetInverseWorldMatrix()*(cVisContainer.GetWorldMatrix()*cCullQuery.GetCameraPosition()));

								// Set the "LightPosition" fragment shader parameter
								if (pGeneratedProgramUserData->pLightPosition)
									pGeneratedProgramUserData->pLightPosition->Set(cLightVisNode.GetWorldViewMatrix().GetTranslation());

								// Set the "LightRadius" fragment shader parameter
								if (pGeneratedProgramUserData->pLightRadius)
									pGeneratedProgramUserData->pLightRadius->Set(fRange);

								// Set cube map
								if (bProjectivePoint) {
									if (pGeneratedProgramUserData->pProjectivePointCubeMap) {
										// The texture buffer we're going to project
										TextureBuffer *pCubeMap = nullptr;

										// Get the projective material to use
										const Material *pProjectiveMaterial = static_cast<SNProjectivePointLight&>(cLight).GetProjectedMaterialHandler().GetResource();
										if (pProjectiveMaterial) {
											// Get the DiffuseMap-parameter of the projective material
											pParameter = pProjectiveMaterial->GetParameter(Material::DiffuseMap);
											if (pParameter) {
												// Get the texture we're going to project
												const Texture *pTexture = pParameter->GetValueTexture();
												if (pTexture)
													pCubeMap = pTexture->GetTextureBuffer();
											}
										}

										// Use the default cube map?
										if (!pCubeMap) {
											if (!m_cCubeMapHandler.GetResource())
												m_cCubeMapHandler.Load(cRenderer.GetRendererContext().GetTextureManager(), "Data/Textures/DefaultCubeMap.dds");
											if (m_cCubeMapHandler.GetResource())
												pCubeMap = m_cCubeMapHandler.GetResource()->GetTextureBuffer();
										}

										// Set the "ProjectivePointCubeMap" fragment shader parameter
										const int nTextureUnit = pGeneratedProgramUserData->pProjectivePointCubeMap->Set(pCubeMap);
										if (nTextureUnit >= 0) {
											cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
											cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
											cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressW, TextureAddressing::Wrap);
											SetupTextureFiltering(cRenderer, nTextureUnit);
										}

										// Set the "ViewSpaceToCubeMapSpace" fragment shader parameter
										if (pGeneratedProgramUserData->pViewSpaceToCubeMapSpace) {
											Matrix3x3 mRot = cLightVisNode.GetWorldViewMatrix();
											mRot.Invert();
											pGeneratedProgramUserData->pViewSpaceToCubeMapSpace->Set(mRot);
										}
									}
								} else {
									// Set spot light parameters
									if (bSpot) {
										// Set light direction in view space
										if (pGeneratedProgramUserData->pLightDirection) {
											Vector3 vLightDirection = cLightVisNode.GetWorldViewMatrix().GetZAxis();
											vLightDirection.Normalize();
											pGeneratedProgramUserData->pLightDirection->Set(vLightDirection);
										}

										// Projective spot light
										if (pGeneratedProgramUserData->pProjectiveSpotMap) {
											// The texture buffer we're going to project
											TextureBuffer *pSpotMap = nullptr;

											// Get the projective material to use
											const Material *pProjectiveMaterial = static_cast<SNProjectiveSpotLight&>(cLight).GetProjectedMaterialHandler().GetResource();
											if (pProjectiveMaterial) {
												// Get the DiffuseMap-parameter of the projective material
												pParameter = pProjectiveMaterial->GetParameter(Material::DiffuseMap);
												if (pParameter) {
													// Get the texture we're going to project
													const Texture *pTexture = pParameter->GetValueTexture();
													if (pTexture)
														pSpotMap = pTexture->GetTextureBuffer();
												}
											}

											// Use the default spot map?
											if (!pSpotMap) {
												if (!m_cSpotMapHandler.GetResource())
													m_cSpotMapHandler.Load(cRenderer.GetRendererContext().GetTextureManager(), "Data/Textures/PLLogo.dds");
												if (m_cSpotMapHandler.GetResource())
													pSpotMap = m_cSpotMapHandler.GetResource()->GetTextureBuffer();
											}

											// Set the "ProjectiveSpotMap" fragment shader parameter
											const int nTextureUnit = pGeneratedProgramUserData->pProjectiveSpotMap->Set(pSpotMap);
											if (nTextureUnit >= 0) {
												// Do NOT use wrapping for projective textures! :)
												cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Border);
												cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Border);
												SetupTextureFiltering(cRenderer, nTextureUnit);
											}

											// Set the "ProjectiveSpotMap" fragment shader parameter
											if (pGeneratedProgramUserData->pViewSpaceToSpotMapSpace) {
												// This matrix brings us from clip space [-1...1] to texture space [0...1] (scale & bias)
												static const Matrix4x4 mClipSpaceToTextureSpace(0.5f, 0.0f, 0.0f, 0.5f,
																								0.0f, 0.5f, 0.0f, 0.5f,
																								0.0f, 0.0f, 0.5f, 0.5f,
																								0.0f, 0.0f, 0.0f, 1.0f);

												// Clip space [-1...1] to texture space [0...1]
												Matrix4x4 mTransform = mClipSpaceToTextureSpace;

												// View space to clip space [-1...1]
												mTransform *= static_cast<SNProjectiveSpotLight&>(cLight).GetProjectionMatrix();

												// Set the fragment shader parameter
												pGeneratedProgramUserData->pViewSpaceToSpotMapSpace->Set(mTransform*cLightVisNode.GetWorldViewMatrix().GetInverted());
											}
										}

										// Set spot light cone parameters
										if (pGeneratedProgramUserData->pSpotConeCos) {
											if (bSpotSmoothCone) {
												// Set spot light cone outer and inner angle in view space
												pGeneratedProgramUserData->pSpotConeCos->Set(static_cast<float>(Math::Cos(static_cast<SNSpotLight&>(cLight).GetOuterAngle()*Math::DegToRad*0.5f)),
																								static_cast<float>(Math::Cos(static_cast<SNSpotLight&>(cLight).GetInnerAngle()*Math::DegToRad*0.5f)));

											} else {
												// Set spot light cone outer angle in view space
												pGeneratedProgramUserData->pSpotConeCos->Set(static_cast<float>(Math::Cos(static_cast<SNSpotLight&>(cLight).GetOuterAngle()*Math::DegToRad*0.5f)));
											}
										}
									}
								}

								// Shadow map
								if (pGeneratedProgramUserData->pShadowMap) {
									const int nTextureUnit = pGeneratedProgramUserData->pShadowMap->Set(pShadowMap);
									if (nTextureUnit >= 0) {
										// Clamp, if this is not done, we may see some artifacts
										cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU,  TextureAddressing::Clamp);
										cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV,  TextureAddressing::Clamp);
										cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressW,  TextureAddressing::Clamp);

										// No mipmapping, please
										cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::Linear);
										cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::Linear);
										cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);

										// Set the "ViewSpaceToShadowMapSpace" fragment shader parameter
										if (bSpot) {
											if (pGeneratedProgramUserData->pViewSpaceToShadowMapSpace) {
												// This matrix brings us from clip space [-1...1] to texture space [0...1] (scale & bias)
												static const Matrix4x4 mClipSpaceToTextureSpace(0.5f, 0.0f, 0.0f, 0.5f,
																								0.0f, 0.5f, 0.0f, 0.5f,
																								0.0f, 0.0f, 0.5f, 0.5f,
																								0.0f, 0.0f, 0.0f, 1.0f);

												// Clip space [-1...1] to texture space [0...1]
												Matrix4x4 mTransform = mClipSpaceToTextureSpace;

												// Scene node space to view space and then view space to clip space [-1...1] combined within one matrix
												mTransform *= pSRPShadowMapping->GetLightViewProjectionMatrix();

												// Set the fragment shader parameter
												pGeneratedProgramUserData->pViewSpaceToShadowMapSpace->Set(mTransform*cLight.GetTransform().GetMatrix()*cLightVisNode.GetWorldViewMatrix().GetInverted());
											}
										} else {
											// Set the "ViewSpaceToShadowCubeMapSpace" fragment shader parameter
											if (pGeneratedProgramUserData->pViewSpaceToShadowCubeMapSpace) {
												const Matrix3x3 mRot = cLight.GetTransform().GetMatrix()*cLightVisNode.GetWorldViewMatrix().GetInverted();
												pGeneratedProgramUserData->pViewSpaceToShadowCubeMapSpace->Set(mRot);
											}

											// Set the "InvLightRadius" fragment shader parameter
											if (pGeneratedProgramUserData->pInvLightRadius)
												pGeneratedProgramUserData->pInvLightRadius->Set(1.0f/fRange);
										}

										// Set texel size
										if (pGeneratedProgramUserData->pTexelSize) {
											float fSize = 0.0f;
											if (pShadowMap->GetType() == TextureBuffer::TypeTextureBuffer2D)
												fSize = static_cast<float>(static_cast<TextureBuffer2D*>(pShadowMap)->GetSize().x);
											else if (pShadowMap->GetType() == TextureBuffer::TypeTextureBufferCube)
												fSize = static_cast<float>(static_cast<TextureBufferCube*>(pShadowMap)->GetSize());
											pGeneratedProgramUserData->pTexelSize->Set(fSize ? 0.5f/fSize : 1.0f);
										}
									}
								}
							}

							// Set the "LightColor" fragment shader parameter
							if (pGeneratedProgramUserData->pLightColor)
								pGeneratedProgramUserData->pLightColor->Set(cLightColor);

							// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
							if (pGeneratedProgramUserData->pVertexPosition)
								pGeneratedProgramUserData->pVertexPosition->Set(&cVertexBuffer, PLRenderer::VertexBuffer::Position);
							if (pGeneratedProgramUserData->pVertexTexCoord0)
								pGeneratedProgramUserData->pVertexTexCoord0->Set(&cVertexBuffer, PLRenderer::VertexBuffer::TexCoord, 0);
							if (pGeneratedProgramUserData->pVertexNormal)
								pGeneratedProgramUserData->pVertexNormal->Set(&cVertexBuffer, PLRenderer::VertexBuffer::Normal);
							if (pGeneratedProgramUserData->pVertexTangent)
								pGeneratedProgramUserData->pVertexTangent->Set(&cVertexBuffer, PLRenderer::VertexBuffer::Tangent);
							if (pGeneratedProgramUserData->pVertexBinormal)
								pGeneratedProgramUserData->pVertexBinormal->Set(&cVertexBuffer, PLRenderer::VertexBuffer::Binormal);

							// Two sided lighting?
							if (pGeneratedProgramUserData->pNormalScale)
								pGeneratedProgramUserData->pNormalScale->Set(1.0f);

							// Draw the geometry
							cRenderer.DrawIndexedPrimitives(
								cGeometry.GetPrimitiveType(),
								0,
								cVertexBuffer.GetNumOfElements()-1,
								cGeometry.GetStartIndex(),
								cGeometry.GetIndexSize()
							);

							// If this is a two sided material, draw the primitives again - but with
							// flipped culling mode and vertex normals
							if (pGeneratedProgramUserData->pNormalScale) {
								// Flip normals
								pGeneratedProgramUserData->pNormalScale->Set(-1.0f);

								// Flip the backface culling
								cRenderer.SetRenderState(RenderState::CullMode, Cull::CW);

								// Draw geometry - again
								cRenderer.DrawIndexedPrimitives(
									cGeometry.GetPrimitiveType(),
									0,
									cVertexBuffer.GetNumOfElements()-1,
									cGeometry.GetStartIndex(),
									cGeometry.GetIndexSize()
								);
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
*    Called when a material is removed
*/
void SRPLighting::OnMaterialRemoved(Material &cMaterial)
{
	// Is this material cached?
	const uint64 nMaterialID = reinterpret_cast<uint64>(&cMaterial);
	SRPLightingMaterial *pSRPLightingMaterial = m_lstMaterialCache.Get(nMaterialID);
	if (pSRPLightingMaterial) {
		// Remove the material from the cache
		m_lstMaterialCache.Remove(nMaterialID);
		delete pSRPLightingMaterial;
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRPLighting::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Get the shader language to use
	String sShaderLanguage = ShaderLanguage;
	if (!sShaderLanguage.GetLength())
		sShaderLanguage = cRenderer.GetDefaultShaderLanguage();

	// Create the program generator if there's currently no instance of it
	if (!m_pProgramGenerator || m_pProgramGenerator->GetShaderLanguage() != sShaderLanguage) {
		// If not already done: Connect event handler - if it's already connected to this event, nothing happens
		cRenderer.GetRendererContext().GetMaterialManager().EventResourceRemoved.Connect(EventHandlerMaterialRemoved);

		// If there's an previous instance of the program generator, destroy it first
		if (m_pProgramGenerator) {
			delete m_pProgramGenerator;
			m_pProgramGenerator = nullptr;
		}

		// Choose the shader source codes depending on the requested shader language
		if (sShaderLanguage == "GLSL") {
			#include "SRPLighting_GLSL.h"
			if (cRenderer.GetAPI() == "OpenGL ES 2.0") {
				// Get shader source codes
				m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sLighting_GLSL_VS, "100", sLighting_GLSL_FS, "100");
			} else {
				// Remove precision qualifiers because they usually create some nasty driver issues!
				m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, Shader::RemovePrecisionQualifiersFromGLSL(sLighting_GLSL_VS), "120", Shader::RemovePrecisionQualifiersFromGLSL(sLighting_GLSL_FS), "120");	// OpenGL 2.1 ("#version 120")
			}
		} else if (sShaderLanguage == "Cg") {
			#include "SRPLighting_Cg.h"
			m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sLighting_Cg_VS, "arbvp1", sLighting_Cg_FS, "arbfp1");
		}
	}

	// If there's no program generator, we don't need to continue
	if (m_pProgramGenerator) {
		{ // Get the instance of the "PLCompositing::SRPDirectionalLightingShaders" scene renderer pass to ask whether or not there's a light that
		  // has already been rendered *the primary directional light source* -> we don't need to render this light again!
			static const String sClassName = "PLCompositing::SRPDirectionalLightingShaders";
			SRPDirectionalLightingShaders *pSRPDirectionalLightingShaders = static_cast<SRPDirectionalLightingShaders*>(GetFirstInstanceOfSceneRendererPassClass(sClassName));
			if (pSRPDirectionalLightingShaders)
				m_pIgnoredLight = reinterpret_cast<SNLight*>(pSRPDirectionalLightingShaders->GetUsedLight());
		}

		// Sets the initial render states
		SetInitialRenderStates(cRenderer);

		// Backup the color mask
		bool bRed, bGreen, bBlue, bAlpha;
		cRenderer.GetColorMask(bRed, bGreen, bBlue, bAlpha);

		// Don't touch the alpha channel - there may be special information for
		// e.g. post processing effects light glow or DOF, and we don't want to overwrite it!
		cRenderer.SetColorMask(true, true, true, false);

		// Draw recursive from front to back
		DrawRec(cRenderer, cCullQuery);

		// Restore the color mask
		cRenderer.SetColorMask(bRed, bGreen, bBlue, bAlpha);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
