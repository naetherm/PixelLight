/*********************************************************\
 *  File: SRPDirectionalLightingShaders.cpp              *
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
#include <PLMath/Matrix3x3.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Renderer/RenderStates.h>
#include <PLRenderer/Renderer/Shader.h>
#include <PLRenderer/Renderer/Program.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ProgramAttribute.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLRenderer/Material/Parameter.h>
#include <PLRenderer/Material/MaterialManager.h>
#include <PLMesh/MeshHandler.h>
#include <PLMesh/MeshLODLevel.h>
#include <PLScene/Scene/SNLight.h>
#include <PLScene/Scene/SNCamera.h>
#include <PLScene/Scene/SceneNodeModifier.h>
#include <PLScene/Visibility/SQCull.h>
#include <PLScene/Visibility/VisContainer.h>
#include "PLCompositing/Shaders/General/SRPDirectionalLightingShadersMaterial.h"
#include "PLCompositing/Shaders/General/SRPDirectionalLightingShaders.h"


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
pl_implement_class(SRPDirectionalLightingShaders)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPDirectionalLightingShaders::SRPDirectionalLightingShaders() :
	ShaderLanguage(this),
	Flags(this),
	EventHandlerMaterialRemoved(&SRPDirectionalLightingShaders::OnMaterialRemoved, this),
	m_bGlowEnabled(false),
	m_fDOFNearBlurDepth(0.0f),
	m_fDOFFocalPlaneDepth(0.0f),
	m_fDOFFarBlurDepth(0.0f),
	m_fDOFBlurrinessCutoff(0.0f),
	m_pProgramGenerator(nullptr),
	m_pRenderStates(new RenderStates())
{
}

/**
*  @brief
*    Destructor
*/
SRPDirectionalLightingShaders::~SRPDirectionalLightingShaders()
{
	{ // SRPDirectionalLightingShaders-material cache cleanup
		Iterator<SRPDirectionalLightingShadersMaterial*> lstIterator = m_lstMaterialCache.GetIterator();
		while (lstIterator.HasNext())
			delete lstIterator.Next();
		m_lstMaterialCache.Clear();
	}

	// Destroy render states 'translator'
	delete m_pRenderStates;

	// Destroy the program generator
	if (m_pProgramGenerator)
		delete m_pProgramGenerator;
}

/**
*  @brief
*    Returns the directional light scene node used for rendering the scene the last time
*/
SNDirectionalLight *SRPDirectionalLightingShaders::GetUsedLight() const
{
	return reinterpret_cast<SNDirectionalLight*>(m_cLightNodeHandler.GetElement());
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a material is removed
*/
void SRPDirectionalLightingShaders::OnMaterialRemoved(Material &cMaterial)
{
	// Is this material cached?
	const uint64 nMaterialID = reinterpret_cast<uint64>(&cMaterial);
	SRPDirectionalLightingShadersMaterial *pSRPDirectionalLightingShadersMaterial = m_lstMaterialCache.Get(nMaterialID);
	if (pSRPDirectionalLightingShadersMaterial) {
		// Remove the material from the cache
		m_lstMaterialCache.Remove(nMaterialID);
		delete pSRPDirectionalLightingShadersMaterial;
	}
}


//[-------------------------------------------------------]
//[ Private virtual SRPDirectionalLighting functions      ]
//[-------------------------------------------------------]
void SRPDirectionalLightingShaders::DrawMesh(Renderer &cRenderer, const SQCull &cCullQuery, const VisNode &cVisNode, SceneNode &cSceneNode, const MeshHandler &cMeshHandler, const Mesh &cMesh, const MeshLODLevel &cMeshLODLevel, VertexBuffer &cVertexBuffer)
{
	// Get scene container
	const VisContainer &cVisContainer = cCullQuery.GetVisContainer();

	// Get buffers
		  IndexBuffer     *pIndexBuffer  = cMeshLODLevel.GetIndexBuffer();
	const Array<Geometry> &lstGeometries = *cMeshLODLevel.GetGeometries();

	// Bind buffers
	cRenderer.SetIndexBuffer(pIndexBuffer);

	// Is lighting enabled for this scene node?
	const bool bLightingEnabled = !(cSceneNode.GetFlags() & SceneNode::NoLighting) && (m_cLightColor != Color3::Black);

	// Get available vertex buffer attributes
	// Binormal and tangent make only sense in this usage when there's also a normal, we need all three vectors!
	const bool bHasVertexTexCoord0 = (cVertexBuffer.GetVertexAttribute(VertexBuffer::TexCoord, 0) != nullptr);	// e.g. for diffuse maps
	const bool bHasVertexTexCoord1 = (cVertexBuffer.GetVertexAttribute(VertexBuffer::TexCoord, 1) != nullptr);	// e.g. for light maps
	const bool bHasVertexNormal    = (cVertexBuffer.GetVertexAttribute(VertexBuffer::Normal) != nullptr);
		  bool bHasVertexTangent   = bHasVertexNormal && (cVertexBuffer.GetVertexAttribute(VertexBuffer::Tangent) != nullptr);
	const bool bHasVertexBinormal  = bHasVertexTangent && (cVertexBuffer.GetVertexAttribute(VertexBuffer::Binormal) != nullptr);

	// For better readability, define whether or not normal mapping is possible with the given vertex data
	const bool bNormalMappingPossible = bHasVertexBinormal;	// We don't need to check for all three vectors in here :D

	// [TODO] Cleanup
	uint32 nEnvironmentFlags = 0;
	if (bHasVertexNormal)
		nEnvironmentFlags |= SRPDirectionalLightingShadersMaterial::EnvironmentVertexNormal;
	if (bHasVertexTexCoord0)
		nEnvironmentFlags |= SRPDirectionalLightingShadersMaterial::EnvironmentVertexTexCoord0;
	if (bHasVertexTexCoord1)
		nEnvironmentFlags |= SRPDirectionalLightingShadersMaterial::EnvironmentVertexTexCoord1;
	if (bNormalMappingPossible)
		nEnvironmentFlags |= SRPDirectionalLightingShadersMaterial::EnvironmentNormalMappingPossible;
	if (bLightingEnabled)
		nEnvironmentFlags |= SRPDirectionalLightingShadersMaterial::EnvironmentLightingEnabled;
	if (m_bGlowEnabled)
		nEnvironmentFlags |= SRPDirectionalLightingShadersMaterial::EnvironmentGlowEnabled;
	if (m_fDOFBlurrinessCutoff)
		nEnvironmentFlags |= SRPDirectionalLightingShadersMaterial::EnvironmentDOFEnabled;

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
					// Transparent material?
					static const String sOpacity = "Opacity";
					const Parameter *pParameter = pMaterial->GetParameter(sOpacity);
					if ((GetFlags() & TransparentPass) ? (pParameter && pParameter->GetValue1f() < 1.0f) : (!pParameter || pParameter->GetValue1f() >= 1.0f)) {
						// SRPDirectionalLightingShaders-material caching
						SRPDirectionalLightingShadersMaterial *pSRPDirectionalLightingShadersMaterial = m_lstMaterialCache.Get(reinterpret_cast<uint64>(pMaterial));
						if (!pSRPDirectionalLightingShadersMaterial) {
							// The material is not yet cached
							pSRPDirectionalLightingShadersMaterial = new SRPDirectionalLightingShadersMaterial(*m_pRenderStates, *pMaterial, *m_pProgramGenerator);
							m_lstMaterialCache.Add(reinterpret_cast<uint64>(pMaterial), pSRPDirectionalLightingShadersMaterial);
						}

						// [TODO] Correct texture filter
						SRPDirectionalLightingShadersMaterial::GeneratedProgramUserData *pGeneratedProgramUserData = pSRPDirectionalLightingShadersMaterial->MakeMaterialCurrent(GetFlags(), nEnvironmentFlags, SRPDirectionalLightingShadersMaterial::Anisotropic2);
						if (pGeneratedProgramUserData) {
							// Ambient color
							if (pGeneratedProgramUserData->pAmbientColor)
								pGeneratedProgramUserData->pAmbientColor->Set(AmbientColor.Get());

							// Set the "ViewSpaceToWorldSpace" fragment shader parameter
							if (pGeneratedProgramUserData->pViewSpaceToWorldSpace) {
								// [TODO] Add *SQCullQuery::GetInvViewMatrix()?
								Matrix3x3 mRot = cCullQuery.GetViewMatrix().GetInverted();
								pGeneratedProgramUserData->pViewSpaceToWorldSpace->Set(mRot);
							}

							if (bLightingEnabled) {
								// Set view space light direction and light color
								if (pGeneratedProgramUserData->pLightDirection)
									pGeneratedProgramUserData->pLightDirection->Set(m_vLightDirection);
								if (pGeneratedProgramUserData->pLightColor)
									pGeneratedProgramUserData->pLightColor->Set(m_cLightColor);
							}

							// DOF
							if (pGeneratedProgramUserData->pDOFParams)
								pGeneratedProgramUserData->pDOFParams->Set(m_fDOFNearBlurDepth, m_fDOFFocalPlaneDepth, m_fDOFFarBlurDepth, m_fDOFBlurrinessCutoff);

							// Set object space to clip space matrix uniform
							if (pGeneratedProgramUserData->pObjectSpaceToClipSpaceMatrix)
								pGeneratedProgramUserData->pObjectSpaceToClipSpaceMatrix->Set(cVisNode.GetWorldViewProjectionMatrix());

							// Set object space to view space matrix uniform
							if (pGeneratedProgramUserData->pObjectSpaceToViewSpaceMatrix)
								pGeneratedProgramUserData->pObjectSpaceToViewSpaceMatrix->Set(cVisNode.GetWorldViewMatrix());

							// Parallax mapping - set object space eye position
							if (pGeneratedProgramUserData->pHeightMap && pGeneratedProgramUserData->pEyePos)
								pGeneratedProgramUserData->pEyePos->Set(cVisNode.GetInverseWorldMatrix()*(cVisContainer.GetWorldMatrix()*cCullQuery.GetCameraPosition()));

							// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
							if (pGeneratedProgramUserData->pVertexPosition)
								pGeneratedProgramUserData->pVertexPosition->Set(&cVertexBuffer, PLRenderer::VertexBuffer::Position);
							if (pGeneratedProgramUserData->pVertexTexCoord0)
								pGeneratedProgramUserData->pVertexTexCoord0->Set(&cVertexBuffer, PLRenderer::VertexBuffer::TexCoord, 0);
							if (pGeneratedProgramUserData->pVertexTexCoord1)
								pGeneratedProgramUserData->pVertexTexCoord1->Set(&cVertexBuffer, PLRenderer::VertexBuffer::TexCoord, 1);
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
								const uint32 nCullModeBackup = cRenderer.GetRenderState(RenderState::CullMode);
								cRenderer.SetRenderState(RenderState::CullMode, Cull::CW);

								// Draw geometry - again
								cRenderer.DrawIndexedPrimitives(
									cGeometry.GetPrimitiveType(),
									0,
									cVertexBuffer.GetNumOfElements()-1,
									cGeometry.GetStartIndex(),
									cGeometry.GetIndexSize()
								);

								// Restore the previous cull mode
								cRenderer.SetRenderState(RenderState::CullMode, nCullModeBackup);
							}
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
void SRPDirectionalLightingShaders::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
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
			#include "SRPDirectionalLightingShaders_GLSL.h"
			if (cRenderer.GetAPI() == "OpenGL ES 2.0") {
				// Get shader source codes
				m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sDirectionalLighting_GLSL_VS, "100", sDirectionalLighting_GLSL_FS, "100");
			} else {
				// Remove precision qualifiers because they usually create some nasty driver issues!
				m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, Shader::RemovePrecisionQualifiersFromGLSL(sDirectionalLighting_GLSL_VS), "120", Shader::RemovePrecisionQualifiersFromGLSL(sDirectionalLighting_GLSL_FS), "120");	// OpenGL 2.1 ("#version 120")
			}
		} else if (sShaderLanguage == "Cg") {
			#include "SRPDirectionalLightingShaders_Cg.h"
			m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sDirectionalLighting_Cg_VS, "arbvp1", sDirectionalLighting_Cg_FS, "arbfp1");
		}
	}

	// If there's no program generator, we don't need to continue
	if (m_pProgramGenerator) {
		// By default, glow is disabled
		m_bGlowEnabled = false;

		// Get depth of field settings from the 'SNMPostProcessDepthOfField' modifier of the used camera
		m_fDOFNearBlurDepth    = 0.0f;
		m_fDOFFocalPlaneDepth  = 0.0f;
		m_fDOFFarBlurDepth     = 0.0f;
		m_fDOFBlurrinessCutoff = 0.0f;
		bool bWriteAlpha = false;
		SNCamera *pCamera = SNCamera::GetCamera();
		if (pCamera) {
			// DOF enabled?
			if (!(GetFlags() & NoDOF)) {
				// Is there an active DOF post process modifier?
				SceneNodeModifier *pModifier = pCamera->GetModifier("PLCompositing::SNMPostProcessDepthOfField");
				if (pModifier && pModifier->IsActive()) {
					if (pModifier->GetAttribute("EffectWeight")->GetFloat() > 0.0f) {
						m_fDOFNearBlurDepth    = pModifier->GetAttribute("NearBlurDepth")   ->GetFloat();
						m_fDOFFocalPlaneDepth  = pModifier->GetAttribute("FocalPlaneDepth") ->GetFloat();
						m_fDOFFarBlurDepth     = pModifier->GetAttribute("FarBlurDepth")    ->GetFloat();
						m_fDOFBlurrinessCutoff = pModifier->GetAttribute("BlurrinessCutoff")->GetFloat();
						bWriteAlpha = (m_fDOFBlurrinessCutoff != 0.0f);
					}
				}
			}

			// Glow enabled?
			if (!m_fDOFBlurrinessCutoff && !(GetFlags() & NoGlow)) {
				// Is there an active glow post process modifier?
				SceneNodeModifier *pModifier = pCamera->GetModifier("PLCompositing::SNMPostProcessGlow");
				if (pModifier && pModifier->IsActive())
					m_bGlowEnabled = bWriteAlpha = true;
			}
		}

		// Reset all render states to default
		cRenderer.GetRendererContext().GetEffectManager().Use();

		// Is this a transparent renderer pass?
		if (GetFlags() & TransparentPass) {
			cRenderer.SetRenderState(RenderState::BlendEnable,  true);
			cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
		} else {
			// Maybe the z-buffer was already filled by another scene renderer pass? Let the user decide...
			cRenderer.SetRenderState(RenderState::ZWriteEnable, !(GetFlags() & NoZWrite));
		}

		// Backup the color mask
		bool bRed, bGreen, bBlue, bAlpha;
		cRenderer.GetColorMask(bRed, bGreen, bBlue, bAlpha);

		// Setup the color mask and enable scissor test
		cRenderer.SetColorMask(true, true, true, true);
		cRenderer.SetRenderState(RenderState::ScissorTestEnable, true);

		// Search for the first (= nearest) visible directional light scene node - but only if lighting is enabled
		const VisNode *pVisNode = (GetFlags() & NoLighting) ? nullptr : GetFirstDirectionalLight(cCullQuery);
		if (pVisNode && pVisNode->GetSceneNode() && pVisNode->GetSceneNode()->IsLight()) {
			// Get the view space light direction vector and the light color
			m_vLightDirection = -pVisNode->GetWorldViewMatrix().GetZAxis().GetNormalized();
			m_cLightColor     = static_cast<SNLight*>(pVisNode->GetSceneNode())->Color.Get()*LightingIntensity;
			m_cLightNodeHandler.SetElement(pVisNode->GetSceneNode());
		} else {
			// Sorry, there's no visible directional light scene node
			m_vLightDirection = Vector3::NegativeUnitY;
			m_cLightColor     = Color3::Black;
			m_cLightNodeHandler.SetElement();
		}

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
