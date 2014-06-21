/*********************************************************\
 *  File: SRPVolumetricFog.cpp                           *
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
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Renderer/RenderStates.h>
#include <PLRenderer/Renderer/Shader.h>
#include <PLRenderer/Renderer/Program.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ProgramAttribute.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLRenderer/Material/Material.h>
#include <PLRenderer/Material/Parameter.h>
#include <PLMesh/MeshHandler.h>
#include <PLMesh/MeshLODLevel.h>
#include <PLScene/Scene/SNSphereFog.h>
#include <PLScene/Scene/SNCellPortal.h>
#include <PLScene/Visibility/SQCull.h>
#include <PLScene/Visibility/VisPortal.h>
#include <PLScene/Visibility/VisContainer.h>
#include "PLCompositing/Shaders/General/SRPVolumetricFog.h"


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
pl_implement_class(SRPVolumetricFog)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPVolumetricFog::SRPVolumetricFog() :
	TextureFiltering(this),
	ShaderLanguage(this),
	Flags(this),
	m_pProgramGenerator(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
SRPVolumetricFog::~SRPVolumetricFog()
{
	// Destroy the program generator
	if (m_pProgramGenerator)
		delete m_pProgramGenerator;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets correct texture filtering modes
*/
void SRPVolumetricFog::SetupTextureFiltering(Renderer &cRenderer, uint32 nStage) const
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
void SRPVolumetricFog::DrawRec(Renderer &cRenderer, const SQCull &cCullQuery)
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

			// Is this a fog node?
			} else if (pSceneNode->IsFog()) {
				SNFog *pFog = static_cast<SNFog*>(pSceneNode);
				// Spherical fog?
				if (pFog->IsSphereFog())
					DrawVolumetricSphereFogRec(cRenderer, cCullQuery, *static_cast<SNSphereFog*>(pFog), *pVisNode);

			// This must just be a quite boring scene node, ignore it
			} else {
			}
		}
	}
}

/**
*  @brief
*    Draws a volumetric spherical fog recursive
*/
void SRPVolumetricFog::DrawVolumetricSphereFogRec(Renderer &cRenderer, const SQCull &cCullQuery, SNSphereFog &cSphereFog, const VisNode &cSphereFogVisNode)
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
				const VisPortal    *pVisPortal = static_cast<const VisPortal*>(pVisNode);
				const VisContainer *pVisCell   = pVisPortal->GetTargetVisContainer();
				if (pVisCell && pVisCell->GetCullQuery() && pVisPortal->GetSceneNode()) {
					// [TODO] Find a better solution without changing temporarily fog variables
					// The next thing is a little bit tricky: We need to change fog variables
					// temporarily...

					// Backup the old position, rotation and scale
					Vector3    vOldPosition = cSphereFog.GetTransform().GetPosition();
					Quaternion qOldRotation = cSphereFog.GetTransform().GetRotation();
					Vector3    vOldScale    = cSphereFog.GetTransform().GetScale();

					// Update position, scale and rotation using the given warp matrix of the portal
					const Matrix3x4 &mWarp = static_cast<SNCellPortal*>(pVisPortal->GetSceneNode())->GetWarpMatrix();
					// New position
					Vector3 vPosition = mWarp*vOldPosition;
					cSphereFog.GetTransform().SetPosition(vPosition);
					// New rotation
					Quaternion qRotation = qOldRotation*mWarp;
					cSphereFog.GetTransform().SetRotation(qRotation);
					// New scale
					Vector3 vScale = vOldScale;
					mWarp.RotateVector(vScale);
					cSphereFog.GetTransform().SetScale(vScale);

					// Draw the target cell
					DrawVolumetricSphereFogRec(cRenderer, *pVisCell->GetCullQuery(), cSphereFog, cSphereFogVisNode);

					// [TODO] Use new PLMath::Transform3 features!
					// Restore the old position, rotation and scale
					cSphereFog.GetTransform().SetPosition(vOldPosition);
					cSphereFog.GetTransform().SetRotation(qOldRotation);
					cSphereFog.GetTransform().SetScale(vOldScale);

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

			// Is this a light, if so, ignore it
			} else if (pSceneNode->IsLight()) {

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
						if (pLODLevel && pLODLevel->GetIndexBuffer()) {
							// Get the vertex buffer which needs at least a position attribute
							VertexBuffer *pVertexBuffer = pMeshHandler->GetVertexBuffer();
							if (pVertexBuffer && pVertexBuffer->GetVertexAttribute(VertexBuffer::Position)) {
								// Draw the mesh
								DrawMesh(cRenderer, cCullQuery, *pVisNode, *pSceneNode, *pMeshHandler, *pMesh, *pLODLevel, *pVertexBuffer, cSphereFog, cSphereFogVisNode);
							}
						}
					}
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Private PLScene::SRPDiffuseOnly functions             ]
//[-------------------------------------------------------]
void SRPVolumetricFog::DrawMesh(Renderer &cRenderer, const SQCull &cCullQuery, const VisNode &cVisNode, SceneNode &cSceneNode, const MeshHandler &cMeshHandler, const Mesh &cMesh, const MeshLODLevel &cMeshLODLevel, VertexBuffer &cVertexBuffer, SNSphereFog &cSphereFog, const VisNode &cSphereFogVisNode)
{
	// Get scene container
	const VisContainer &cVisContainer = cCullQuery.GetVisContainer();

	// Get buffers
		  IndexBuffer     *pIndexBuffer  = cMeshLODLevel.GetIndexBuffer();
	const Array<Geometry> &lstGeometries = *cMeshLODLevel.GetGeometries();

	// Bind buffers
	cRenderer.SetIndexBuffer(pIndexBuffer);

	// Get available vertex buffer attributes
	const bool bHasVertexTexCoord = (cVertexBuffer.GetVertexAttribute(VertexBuffer::TexCoord) != nullptr);

	// Draw mesh
	for (uint32 nMat=0; nMat<cMeshHandler.GetNumOfMaterials(); nMat++) {
		// Get mesh material
		const Material *pMaterial = cMeshHandler.GetMaterial(nMat);
		if (pMaterial) {
			// Draw geometries
			for (uint32 nGeo=0; nGeo<lstGeometries.GetNumOfElements(); nGeo++) {
				// Is this geometry active and is it using the current used mesh material?
				const Geometry &cGeometry = lstGeometries[nGeo];
				if (cGeometry.IsActive() && nMat == cGeometry.GetMaterial()) {
					// Transparent material? - Only none transparent objects can be fogged by using this scene renderer pass...
					static const String sOpacity = "Opacity";
					const Parameter *pParameter = pMaterial->GetParameter(sOpacity);
					if (!pParameter || pParameter->GetValue1f() >= 1.0f) {
						// Reset the program flags
						m_cProgramFlags.Reset();

						{ // Two sided
							static const String sTwoSided = "TwoSided";
							pParameter = pMaterial->GetParameter(sTwoSided);
							cRenderer.SetRenderState(RenderState::CullMode, (pParameter && pParameter->GetValue1f()) == 1.0f ? Cull::None : Cull::CCW);
						}

						// Diffuse map and alpha reference
						float fAlphaReference = 0.0f;
						TextureBuffer *pDiffuseMap = (!bHasVertexTexCoord || (GetFlags() & NoDiffuseMap)) ? nullptr : pMaterial->GetParameterTextureBuffer(Material::DiffuseMap);
						if (pDiffuseMap) {
							// Get alpha reference
							if (pDiffuseMap->GetComponentsPerPixel() == 4) {
								static const String sAlphaReference = "AlphaReference";
								pParameter = pMaterial->GetParameter(sAlphaReference);
								fAlphaReference = pParameter ? pParameter->GetValue1f() : 0.5f;
								if (fAlphaReference != 0.0f)
									PL_ADD_FS_FLAG(m_cProgramFlags, FS_ALPHATEST)
							}

							// We only need the diffuse map to perform an alpha test...
							if (m_cProgramFlags.GetFragmentShaderFlags() & FS_ALPHATEST) {
								PL_ADD_VS_FLAG(m_cProgramFlags, VS_TEXCOORD0)
								PL_ADD_FS_FLAG(m_cProgramFlags, FS_DIFFUSEMAP)
							} else {
								// We don't need the diffuse map, just ignore it
								pDiffuseMap = nullptr;
							}
						}

						// Get a program instance from the program generator using the given program flags
						ProgramGenerator::GeneratedProgram *pGeneratedProgram = m_pProgramGenerator->GetProgram(m_cProgramFlags);

						// Make our program to the current one
						if (pGeneratedProgram && cRenderer.SetProgram(pGeneratedProgram->pProgram)) {
							// Set pointers to uniforms & attributes of a generated program if they are not set yet
							GeneratedProgramUserData *pGeneratedProgramUserData = static_cast<GeneratedProgramUserData*>(pGeneratedProgram->pUserData);
							if (!pGeneratedProgramUserData) {
								pGeneratedProgram->pUserData = pGeneratedProgramUserData = new GeneratedProgramUserData;
								Program *pProgram = pGeneratedProgram->pProgram;
								// Vertex shader attributes
								static const String sVertexPosition = "VertexPosition";
								pGeneratedProgramUserData->pVertexPosition					= pProgram->GetAttribute(sVertexPosition);
								static const String sVertexTexCoord0 = "VertexTexCoord0";
								pGeneratedProgramUserData->pVertexTexCoord0					= pProgram->GetAttribute(sVertexTexCoord0);
								// Vertex shader uniforms
								static const String sObjectSpaceToWorldSpaceMatrix = "ObjectSpaceToWorldSpaceMatrix";
								pGeneratedProgramUserData->pObjectSpaceToWorldSpaceMatrix	= pProgram->GetUniform(sObjectSpaceToWorldSpaceMatrix);
								static const String sObjectSpaceToClipSpaceMatrix = "ObjectSpaceToClipSpaceMatrix";
								pGeneratedProgramUserData->pObjectSpaceToClipSpaceMatrix	= pProgram->GetUniform(sObjectSpaceToClipSpaceMatrix);
								static const String sFogPosition = "FogPosition";
								pGeneratedProgramUserData->pFogPosition						= pProgram->GetUniform(sFogPosition);
								static const String sFogInvRadius = "FogInvRadius";
								pGeneratedProgramUserData->pFogInvRadius					= pProgram->GetUniform(sFogInvRadius);
								static const String sEyePos = "EyePos";
								pGeneratedProgramUserData->pEyePos							= pProgram->GetUniform(sEyePos);
								// Fragment shader uniforms
								pGeneratedProgramUserData->pDiffuseMap						= pProgram->GetUniform(Material::DiffuseMap);
								static const String sAlphaReference = "AlphaReference";
								pGeneratedProgramUserData->pAlphaReference					= pProgram->GetUniform(sAlphaReference);
								static const String sFogColor0 = "FogColor0";
								pGeneratedProgramUserData->pFogColor0						= pProgram->GetUniform(sFogColor0);
								static const String sFogColor1 = "FogColor1";
								pGeneratedProgramUserData->pFogColor1						= pProgram->GetUniform(sFogColor1);
							}

							// Set object space to clip space matrix uniform
							if (pGeneratedProgramUserData->pObjectSpaceToClipSpaceMatrix)
								pGeneratedProgramUserData->pObjectSpaceToClipSpaceMatrix->Set(cVisNode.GetWorldViewProjectionMatrix());

							// Set object space to world space matrix uniform
							if (pGeneratedProgramUserData->pObjectSpaceToWorldSpaceMatrix)
								pGeneratedProgramUserData->pObjectSpaceToWorldSpaceMatrix->Set(Matrix4x4(cVisNode.GetWorldMatrix()));

							// Diffuse map
							if (pGeneratedProgramUserData->pDiffuseMap) {
								const int nTextureUnit = pGeneratedProgramUserData->pDiffuseMap->Set(pDiffuseMap);
								if (nTextureUnit >= 0) {
									cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
									SetupTextureFiltering(cRenderer, nTextureUnit);

									// Set the "AlphaReference" fragment shader parameter
									if (pGeneratedProgramUserData->pAlphaReference)
										pGeneratedProgramUserData->pAlphaReference->Set(fAlphaReference);
								}
							}

							// Set fog uniforms
							if (pGeneratedProgramUserData->pFogPosition && pGeneratedProgramUserData->pFogInvRadius && pGeneratedProgramUserData->pEyePos && pGeneratedProgramUserData->pFogColor0 && pGeneratedProgramUserData->pFogColor1) {
								// Adjusted inverse square fall-off function that falls to zero at the light radius
								static const float a0 = 0.2f;
								static const float a1 = a0 + 1.0f;
								static const float c  = Math::Sqrt(a1 / a0 - 1.0f);

								// Set uniforms
								pGeneratedProgramUserData->pFogPosition->Set(cSphereFogVisNode.GetWorldMatrix().GetTranslation());
								pGeneratedProgramUserData->pFogInvRadius->Set(c/cSphereFog.GetRange());
								pGeneratedProgramUserData->pEyePos->Set(cVisContainer.GetWorldMatrix()*cCullQuery.GetCameraPosition());
								pGeneratedProgramUserData->pFogColor0->Set(cSphereFog.Color.Get()*a1*cSphereFog.Volumetricy);
								pGeneratedProgramUserData->pFogColor1->Set(cSphereFog.Color.Get()*a0*cSphereFog.Volumetricy);
							}

							// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
							if (pGeneratedProgramUserData->pVertexPosition)
								pGeneratedProgramUserData->pVertexPosition->Set(&cVertexBuffer, PLRenderer::VertexBuffer::Position);
							if (pGeneratedProgramUserData->pVertexTexCoord0)
								pGeneratedProgramUserData->pVertexTexCoord0->Set(&cVertexBuffer, PLRenderer::VertexBuffer::TexCoord);

							// Draw the geometry
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


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRPVolumetricFog::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
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
			#include "SRPVolumetricFog_GLSL.h"
			if (cRenderer.GetAPI() == "OpenGL ES 2.0") {
				// Get shader source codes
				m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sVolumetricFog_GLSL_VS, "100", sVolumetricFog_GLSL_FS, "100");
			} else {
				// Remove precision qualifiers because they usually create some nasty driver issues!
				m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, Shader::RemovePrecisionQualifiersFromGLSL(sVolumetricFog_GLSL_VS), "110", Shader::RemovePrecisionQualifiersFromGLSL(sVolumetricFog_GLSL_FS), "110");	// OpenGL 2.0 ("#version 110")
			}
		} else if (sShaderLanguage == "Cg") {
			#include "SRPVolumetricFog_Cg.h"
			m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sVolumetricFog_Cg_VS, "arbvp1", sVolumetricFog_Cg_FS, "arbfp1");
		}
	}

	// If there's no program generator, we don't need to continue
	if (m_pProgramGenerator) {
		// Reset all render states to default
		cRenderer.GetRendererContext().GetEffectManager().Use();

		// Setup render states
		cRenderer.SetRenderState(RenderState::ZWriteEnable,		 false);
		cRenderer.SetRenderState(RenderState::BlendEnable,		 true);
		cRenderer.SetRenderState(RenderState::SrcBlendFunc,		 BlendFunc::One);
		cRenderer.SetRenderState(RenderState::DstBlendFunc,		 BlendFunc::One);
		cRenderer.SetRenderState(RenderState::ScissorTestEnable, true);

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
