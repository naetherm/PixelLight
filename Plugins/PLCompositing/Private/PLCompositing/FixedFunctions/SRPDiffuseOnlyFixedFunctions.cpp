/*********************************************************\
 *  File: SRPDiffuseOnlyFixedFunctions.cpp               *
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
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Renderer/RenderStates.h>
#include <PLRenderer/Renderer/FixedFunctions.h>
#include <PLRenderer/Material/Material.h>
#include <PLRenderer/Material/Parameter.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLRenderer/Texture/TextureManager.h>
#include <PLMesh/MeshHandler.h>
#include <PLMesh/MeshLODLevel.h>
#include <PLScene/Visibility/VisNode.h>
#include "PLCompositing/FixedFunctions/SRPDiffuseOnlyFixedFunctions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLMesh;
using namespace PLScene;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SRPDiffuseOnlyFixedFunctions)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPDiffuseOnlyFixedFunctions::SRPDiffuseOnlyFixedFunctions() :
	m_pRenderStates(new RenderStates()),
	m_nMaterialChanges(0),
	m_pCurrentMaterial(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
SRPDiffuseOnlyFixedFunctions::~SRPDiffuseOnlyFixedFunctions()
{
	// Destroy render states 'translator'
	delete m_pRenderStates;
}



//[-------------------------------------------------------]
//[ Private virtual SRPDiffuseOnly functions              ]
//[-------------------------------------------------------]
void SRPDiffuseOnlyFixedFunctions::DrawMesh(Renderer &cRenderer, const SQCull &cCullQuery, const VisNode &cVisNode, SceneNode &cSceneNode, const MeshHandler &cMeshHandler, const Mesh &cMesh, const MeshLODLevel &cMeshLODLevel, VertexBuffer &cVertexBuffer)
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
					const bool bTransparentPass = (GetFlags() & TransparentPass) != 0;
					if (bTransparentPass ? (pParameter && pParameter->GetValue1f() < 1.0f) : (!pParameter || pParameter->GetValue1f() >= 1.0f)) {
						bool bDraw = true;

						// "OpacityOcclusion" mode?
						if (bTransparentPass && (GetFlags() & ZWriteOnly)) {
							static const String sOpacityOcclusion = "OpacityOcclusion";
							pParameter = pMaterial->GetParameter(sOpacityOcclusion);
							bDraw = (pParameter && pParameter->GetValue1f() >= 1.0f);
						}

						// Draw?
						if (bDraw) {
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

								// Set diffuse color - but only if this is not a z write only render pass - we do this in here by changing the ambient color for this material...
								if (!(GetFlags() & ZWriteOnly)) {
									static const String sDiffuseColor = "DiffuseColor";
									pParameter = pMaterial->GetParameter(sDiffuseColor);
									if (pParameter) {
										float fDiffuseColor[3] = { 1.0f, 1.0f, 1.0f };
										pParameter->GetValue3f(fDiffuseColor[0], fDiffuseColor[1], fDiffuseColor[2]);
										pFixedFunctions->SetColor(Color4(fDiffuseColor[0], fDiffuseColor[1],fDiffuseColor[2], fOpacity));
									} else {
										pFixedFunctions->SetColor(Color4(1.0f, 1.0f, 1.0f, fOpacity));
									}
								}

								// Bind diffuse map at texture unit 0
								const Texture *pTexture = nullptr;
								if (GetFlags() & NoDiffuseMap)
									cRenderer.SetTextureBuffer(0, nullptr);
								else {
									pParameter = pMaterial->GetParameter(Material::DiffuseMap);
									if (pParameter)
										pTexture = pParameter->GetValueTexture();
									if (pTexture && pTexture->GetTextureBuffer()) {
										// Enable/disable alpha test
										if (pTexture->GetTextureBuffer()->GetComponentsPerPixel() == 4) {
											// Get alpha reference
											static const String sAlphaReference = "AlphaReference";
											pParameter = pMaterial->GetParameter(sAlphaReference);
											const float fAlphaReference = pParameter ? pParameter->GetValue1f() : 0.5f;

											// Set alpha render states
											if (fAlphaReference) {
												pFixedFunctions->SetRenderState(FixedFunctions::RenderState::AlphaTestEnable,    true);
												pFixedFunctions->SetRenderState(FixedFunctions::RenderState::AlphaTestReference, Tools::FloatToUInt32(fAlphaReference));
											} else {
												pFixedFunctions->SetRenderState(FixedFunctions::RenderState::AlphaTestEnable, false);

												// If this a z write only render pass, we only need the diffuse map to perform an alpha test...
												if (GetFlags() & ZWriteOnly)
													pTexture = nullptr;
											}
										} else {
											pFixedFunctions->SetRenderState(FixedFunctions::RenderState::AlphaTestEnable, false);

											// If this a z write only render pass, we only need the diffuse map to perform an alpha test...
											if (GetFlags() & ZWriteOnly)
												pTexture = nullptr;
										}

										// Is the diffuse map still there?
										if (pTexture) {
											pTexture->Bind(0);
											SetupTextureFiltering(cRenderer, 0);
										} else {
											cRenderer.SetTextureBuffer(0, nullptr);
										}
									} else {
										cRenderer.SetTextureBuffer(0, nullptr);

										// Disable alpha test
										pFixedFunctions->SetRenderState(FixedFunctions::RenderState::AlphaTestEnable, false);
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
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRPDiffuseOnlyFixedFunctions::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// First, perform a sanity check: If this is a z write only scene renderer pass, but z write is disabled...
	if (!(GetFlags() & ZWriteOnly) || !(GetFlags() & NoZWrite)) {
		// Fixed functions support required
		FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
		if (pFixedFunctions) {
			// Reset all render states to default
			cRenderer.GetRendererContext().GetEffectManager().Use();

			// Enable scissor test
			cRenderer.SetRenderState(RenderState::ScissorTestEnable, true);

			// Backup the color mask
			bool bRed, bGreen, bBlue, bAlpha;
			cRenderer.GetColorMask(bRed, bGreen, bBlue, bAlpha);

			// Setup the color mask and enable scissor test
			const bool bWriteColors = !(GetFlags() & ZWriteOnly);
			cRenderer.SetColorMask(bWriteColors, bWriteColors, bWriteColors, bWriteColors);

			// Is this a transparent renderer pass?
			if (GetFlags() & TransparentPass) {
				cRenderer.SetRenderState(RenderState::BlendEnable,  true);
				cRenderer.SetRenderState(RenderState::ZWriteEnable, GetFlags() & ZWriteOnly);		// "OpacityOcclusion" mode?
			} else {
				// Maybe the z-buffer was already filled by another scene renderer pass? Let the user decide...
				cRenderer.SetRenderState(RenderState::ZWriteEnable, !(GetFlags() & NoZWrite));
			}

			// Reset current material
			m_nMaterialChanges = 0;
			m_pCurrentMaterial = nullptr;

			// Draw recursive from front to back
			DrawRec(cRenderer, cCullQuery);

			// Restore the color mask
			cRenderer.SetColorMask(bRed, bGreen, bBlue, bAlpha);
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
