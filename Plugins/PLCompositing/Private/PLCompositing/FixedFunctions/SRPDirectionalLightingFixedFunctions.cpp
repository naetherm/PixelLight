/*********************************************************\
 *  File: SRPDirectionalLightingFixedFunctions.cpp       *
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
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/FixedFunctions.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Renderer/RenderStates.h>
#include <PLRenderer/Material/Parameter.h>
#include <PLRenderer/Material/Material.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLRenderer/Texture/TextureManager.h>
#include <PLMesh/MeshHandler.h>
#include <PLMesh/MeshLODLevel.h>
#include <PLScene/Scene/SNLight.h>
#include <PLScene/Visibility/VisNode.h>
#include "PLCompositing/FixedFunctions/SRPDirectionalLightingFixedFunctions.h"


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
pl_implement_class(SRPDirectionalLightingFixedFunctions)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SRPDirectionalLightingFixedFunctions::SRPDirectionalLightingFixedFunctions() :
	Flags(this),
	m_pRenderStates(new RenderStates()),
	m_nMaterialChanges(0),
	m_pCurrentMaterial(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
SRPDirectionalLightingFixedFunctions::~SRPDirectionalLightingFixedFunctions()
{
	// Destroy render states 'translator'
	delete m_pRenderStates;
}


//[-------------------------------------------------------]
//[ Private virtual SRPDirectionalLighting functions      ]
//[-------------------------------------------------------]
void SRPDirectionalLightingFixedFunctions::DrawMesh(Renderer &cRenderer, const SQCull &cCullQuery, const VisNode &cVisNode, SceneNode &cSceneNode, const MeshHandler &cMeshHandler, const Mesh &cMesh, const MeshLODLevel &cMeshLODLevel, VertexBuffer &cVertexBuffer)
{
	// Get the fixed functions interface (when we're in here, we know that it must exist!)
	FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();

	// Set the current world matrix
	pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, cVisNode.GetWorldMatrix());

	// Get buffers
		  IndexBuffer     *pIndexBuffer  = cMeshLODLevel.GetIndexBuffer();
	const Array<Geometry> &lstGeometries = *cMeshLODLevel.GetGeometries();

	// Bind buffers
	cRenderer.SetIndexBuffer(pIndexBuffer);
	pFixedFunctions->SetVertexBuffer(&cVertexBuffer);

	// Draw mesh
	for (uint32 nMat=0; nMat<cMeshHandler.GetNumOfMaterials(); nMat++) {
		// Get mesh material
		const Material *pMaterial = cMeshHandler.GetMaterial(nMat);
		if (pMaterial && !pMaterial->GetEffect()) {
			// Draw geometries
			for (uint32 nGeo=0; nGeo<lstGeometries.GetNumOfElements(); nGeo++) {
				// Is this geometry active and is it using the current used mesh material?
				const Geometry &cGeometry = lstGeometries[nGeo];
				if (cGeometry.IsActive() && nMat == cGeometry.GetMaterial()) {
					// Transparent material?
					static const String sOpacity = "Opacity";
					const Parameter *pParameter = pMaterial->GetParameter(sOpacity);
					if ((GetFlags() & TransparentPass) ? (pParameter && pParameter->GetValue1f() < 1.0f) : (!pParameter || pParameter->GetValue1f() >= 1.0f)) {
						// Material change?
						if (m_pCurrentMaterial != pMaterial) {
							// Update current material
							m_nMaterialChanges++;
							m_pCurrentMaterial = pMaterial;

							// Get opacity
							const float fOpacity = pParameter ? pParameter->GetValue1f() : 1.0f;
							if (fOpacity < 1) {
								// Get and set source blend function
								uint32 nValue = BlendFunc::SrcAlpha;
								static const String sSrcBlendFunc = "SrcBlendFunc";
								pParameter = pMaterial->GetParameter(sSrcBlendFunc);
								if (pParameter) {
									m_pRenderStates->SetAttribute("SrcBlendFunc", pParameter->GetParameterString());
									nValue = m_pRenderStates->Get(RenderState::SrcBlendFunc);
								}
								cRenderer.SetRenderState(RenderState::SrcBlendFunc, nValue);

								// Get and set destination blend function
								nValue = BlendFunc::InvSrcAlpha;
								static const String sDstBlendFunc = "DstBlendFunc";
								pParameter = pMaterial->GetParameter(sDstBlendFunc);
								if (pParameter) {
									m_pRenderStates->SetAttribute("DstBlendFunc", pParameter->GetParameterString());
									nValue = m_pRenderStates->Get(RenderState::DstBlendFunc);
								}
								cRenderer.SetRenderState(RenderState::DstBlendFunc, nValue);
							}

							// Setup cull mode
							static const String sTwoSided = "TwoSided";
							pParameter = pMaterial->GetParameter(sTwoSided);
							cRenderer.SetRenderState(RenderState::CullMode, (pParameter && pParameter->GetValue1f()) == 1.0f ? Cull::None : Cull::CCW);

							// Setup transparency and diffuse color
							static const String sDiffuseColor = "DiffuseColor";
							pParameter = pMaterial->GetParameter(sDiffuseColor);
							if (pParameter) {
								float fDiffuseColor[3] = { 1.0f, 1.0f, 1.0f };
								pParameter->GetValue3f(fDiffuseColor[0], fDiffuseColor[1], fDiffuseColor[2]);
								pFixedFunctions->SetColor(Color4(fDiffuseColor[0], fDiffuseColor[1], fDiffuseColor[2], fOpacity));
							} else {
								pFixedFunctions->SetColor(Color4(1.0f, 1.0f, 1.0f, fOpacity));
							}

							// Specular highlight
							if (GetFlags() & NoSpecular) {
								pFixedFunctions->SetMaterialState(FixedFunctions::MaterialState::Specular,  Color4::Black.ToUInt32());
								pFixedFunctions->SetMaterialState(FixedFunctions::MaterialState::Shininess, 0);
							} else {
								static const String sSpecularColor    = "SpecularColor";
								static const String sSpecularExponent = "SpecularExponent";
								float  fSpecularExponent = 45.0f;
								Color4 cSpecularColor    = Color4::White;

								// First, get specular color - if it's 0, we don't have any specular at all
								pParameter = pMaterial->GetParameter(sSpecularColor);
								if (pParameter)
									cSpecularColor = pParameter->GetValue3fv();
								if (cSpecularColor != 0.0f) {
									// Get specular exponent
									pParameter = pMaterial->GetParameter(sSpecularExponent);
									if (pParameter)
										pParameter->GetValue1f(fSpecularExponent);
								}

								// Set material states
								pFixedFunctions->SetMaterialState(FixedFunctions::MaterialState::Specular,  cSpecularColor.ToUInt32());
								pFixedFunctions->SetMaterialState(FixedFunctions::MaterialState::Shininess, Tools::FloatToUInt32(fSpecularExponent));
							}

							// Bind textures
							// Diffuse map (stage 0)
							const Texture *pTexture = nullptr;
							if (GetFlags() & NoDiffuseMap)
								cRenderer.SetTextureBuffer(0, nullptr);
							else {
								pParameter = pMaterial->GetParameter(Material::DiffuseMap);
								if (pParameter)
									pTexture = pParameter->GetValueTexture();
								if (pTexture && pTexture->GetTextureBuffer()) {
									pTexture->Bind(0);
									SetupTextureFiltering(cRenderer, 0);

									// Enable/disable alpha test - but only if this material is not transparent, else the result may look odd
									if (pTexture->GetTextureBuffer()->GetComponentsPerPixel() == 4 && fOpacity >= 1) {
										// Get alpha reference
										static const String sAlphaReference = "AlphaReference";
										pParameter = pMaterial->GetParameter(sAlphaReference);
										float fAlphaReference = pParameter ? pParameter->GetValue1f() : 0.5f;

										// Set alpha render states
										if (fAlphaReference) {
											pFixedFunctions->SetRenderState(FixedFunctions::RenderState::AlphaTestEnable,    true);
											pFixedFunctions->SetRenderState(FixedFunctions::RenderState::AlphaTestReference, Tools::FloatToUInt32(fAlphaReference));
										} else {
											pFixedFunctions->SetRenderState(FixedFunctions::RenderState::AlphaTestEnable, false);
										}
									} else {
										pFixedFunctions->SetRenderState(FixedFunctions::RenderState::AlphaTestEnable, false);
									}
								} else {
									cRenderer.SetTextureBuffer(0, nullptr);

									// Disable alpha test
									pFixedFunctions->SetRenderState(FixedFunctions::RenderState::AlphaTestEnable, false);
								}
							}

							// Light map (stage 1)
							bool bLightMapUsed = false;
							if (GetFlags() & NoLightMap)
								cRenderer.SetTextureBuffer(1, nullptr);
							else {
								pTexture = nullptr;
								pParameter = pMaterial->GetParameter(Material::LightMap);
								if (pParameter)
									pTexture = pParameter->GetValueTexture();
								if (pTexture) {
									pTexture->Bind(1);
									SetupTextureFiltering(cRenderer, 1);
									bLightMapUsed = true;
								} else {
									cRenderer.SetTextureBuffer(1, nullptr);
								}
							}

							// Ambient occlusion map (stage 1)
							if (!bLightMapUsed) {
								if (GetFlags() & NoAmbientOcclusionMap)
									cRenderer.SetTextureBuffer(1, nullptr);
								else {
									pTexture = nullptr;
									pParameter = pMaterial->GetParameter(Material::AmbientOcclusionMap);
									if (pParameter)
										pTexture = pParameter->GetValueTexture();
									if (pTexture) {
										pTexture->Bind(1);
										SetupTextureFiltering(cRenderer, 1);
									} else {
										cRenderer.SetTextureBuffer(1, nullptr);
									}
								}
							}

							// Reflection map (stage 2)
							if (GetFlags() & NoReflectionMap)
								cRenderer.SetTextureBuffer(2, nullptr);
							else {
								pTexture = nullptr;
								pParameter = pMaterial->GetParameter(Material::ReflectionMap);
								if (pParameter)
									pTexture = pParameter->GetValueTexture();
								if (pTexture) {
									pTexture->Bind(2);
									SetupTextureFiltering(cRenderer, 2);

									// 2D or cube ?
									const TextureBuffer *pTextureBuffer = pTexture->GetTextureBuffer();
									if (pTextureBuffer) {
										if (pTextureBuffer->GetType() == TextureBuffer::TypeTextureBufferCube) {
											pFixedFunctions->SetTextureStageState(2, FixedFunctions::TextureStage::TexGen, FixedFunctions::TexCoordGen::ReflectionMap);
											cRenderer.SetSamplerState(2, Sampler::AddressU, TextureAddressing::Clamp);
											cRenderer.SetSamplerState(2, Sampler::AddressV, TextureAddressing::Clamp);
											cRenderer.SetSamplerState(2, Sampler::AddressW, TextureAddressing::Clamp);
										} else if (pTextureBuffer->GetType() == TextureBuffer::TypeTextureBuffer2D) {
											pFixedFunctions->SetTextureStageState(2, FixedFunctions::TextureStage::TexGen, FixedFunctions::TexCoordGen::SphereMap);
											cRenderer.SetSamplerState(2, Sampler::AddressU, TextureAddressing::Wrap);
											cRenderer.SetSamplerState(2, Sampler::AddressV, TextureAddressing::Wrap);
											cRenderer.SetSamplerState(2, Sampler::AddressW, TextureAddressing::Wrap);
										}
									}
								} else {
									cRenderer.SetTextureBuffer(2, nullptr);
								}
							}
						}

						// Draw geometry
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


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRPDirectionalLightingFixedFunctions::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Fixed functions support required
	FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
	if (pFixedFunctions) {
		// The user may have changed light states by hand...
		pFixedFunctions->ResetLights();

		// Reset all render states to default
		cRenderer.GetRendererContext().GetEffectManager().Use();
		cRenderer.SetRenderState(RenderState::ScissorTestEnable, true);
		const Color3 &cAmbientColor = AmbientColor.Get();
		pFixedFunctions->SetRenderState(FixedFunctions::RenderState::Ambient,  Color4::ToUInt32(cAmbientColor.r, cAmbientColor.g, cAmbientColor.b, 1.0f));

		// Is this a transparent renderer pass?
		if (GetFlags() & TransparentPass) {
			cRenderer.SetRenderState(RenderState::BlendEnable,  true);
			cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
		} else {
			// Maybe the z-buffer was already filled by another scene renderer pass? Let the user decide...
			cRenderer.SetRenderState(RenderState::ZWriteEnable, !(GetFlags() & NoZWrite));
		}

		// Search for the first (= nearest) visible directional light scene node - but only if lighting is enabled
		const VisNode *pVisNode = (GetFlags() & NoLighting) ? nullptr : GetFirstDirectionalLight(cCullQuery);
		if (pVisNode && pVisNode->GetSceneNode() && pVisNode->GetSceneNode()->IsLight()) {
			pFixedFunctions->SetRenderState(FixedFunctions::RenderState::Lighting, true);

			// Setup light
			FixedFunctions::Light sLight;
			pFixedFunctions->GetDefaultLightSettings(sLight);
			sLight.cDiffuse = static_cast<SNLight*>(pVisNode->GetSceneNode())->Color.Get()*LightingIntensity;
			sLight.nType = FixedFunctions::LightType::Directional;
			sLight.vDirection = -pVisNode->GetWorldMatrix().GetZAxis().GetNormalized();
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, Matrix4x4::Identity);
			pFixedFunctions->SetLightEnabled(0, false);
			pFixedFunctions->SetLight		(0, sLight);
			pFixedFunctions->SetLightEnabled(0, true);
		} else {
			// Sorry, there's no visible directional light scene node
			pFixedFunctions->SetRenderState(FixedFunctions::RenderState::Lighting, false);
		}

		// Reset current material
		m_nMaterialChanges = 0;
		m_pCurrentMaterial = nullptr;

		// Draw recursive from back to front
		DrawRec(cRenderer, cCullQuery);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
