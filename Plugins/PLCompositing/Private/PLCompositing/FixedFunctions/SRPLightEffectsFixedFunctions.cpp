/*********************************************************\
 *  File: SRPLightEffectsFixedFunctions.cpp              *
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
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Renderer/SamplerStates.h>
#include <PLRenderer/Renderer/FixedFunctions.h>
#include <PLRenderer/Renderer/OcclusionQuery.h>
#include <PLScene/Scene/SceneContext.h>
#include <PLScene/Scene/SNEffectLight.h>
#include <PLScene/Visibility/SQCull.h>
#include <PLScene/Visibility/VisPortal.h>
#include <PLScene/Visibility/VisContainer.h>
#include "PLCompositing/FixedFunctions/SRPLightEffectsFixedFunctions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLScene;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SRPLightEffectsFixedFunctions)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPLightEffectsFixedFunctions::SRPLightEffectsFixedFunctions() :
	m_pBillboardVB(nullptr),
	m_pBillboardColorVB(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
SRPLightEffectsFixedFunctions::~SRPLightEffectsFixedFunctions()
{
	// Destroy the billboard buffers
	if (m_pBillboardVB)
		delete m_pBillboardVB;
	if (m_pBillboardColorVB)
		delete m_pBillboardColorVB;

	// Destroy all light effects
	FreeLightEffects();
	Iterator<LightEffect*> cIterator = m_lstFreeLightEffects.GetIterator();
	while (cIterator.HasNext()) {
		LightEffect *pLightEffect = cIterator.Next();
		if (pLightEffect->pOcclusionQuery) {
			delete pLightEffect->pOcclusionQueryAll;
			delete pLightEffect->pOcclusionQuery;
		}
		delete pLightEffect;
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Prepares light effects recursive
*/
void SRPLightEffectsFixedFunctions::PrepareRec(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Get the fixed functions interface (when we're in here, we know that it must exist!)
	FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();

	// Get scene container
	const VisContainer &cVisContainer = cCullQuery.GetVisContainer();

	// Set the new scissor rectangle
	cRenderer.SetScissorRect(&cVisContainer.GetProjection().cRectangle);

	// Render all visible scene nodes of this scene container
	Iterator<VisNode*> cIterator = cVisContainer.GetVisNodes().GetEndIterator();
	while (cIterator.HasPrevious()) {
		// Get visibility node and scene node
		const VisNode   *pVisNode   = cIterator.Previous();
			  SceneNode *pSceneNode = pVisNode->GetSceneNode();
		if (pSceneNode) {
			// Is this scene node a portal?
			if (pVisNode->IsPortal()) {
				// Get the target cell visibility container
				const VisContainer *pVisCell = static_cast<const VisPortal*>(pVisNode)->GetTargetVisContainer();
				if (pVisCell && pVisCell->GetCullQuery()) {
					// Draw the target cell
					PrepareRec(cRenderer, *pVisCell->GetCullQuery());

					// Set the previous scissor rectangle
					cRenderer.SetScissorRect(&cVisContainer.GetProjection().cRectangle);
				}

			// Is this scene node a container? We do not need to check for cells because we will
			// NEVER receive cells from SQCull directly, they are ONLY visible through portals! (see above)
			} else if (pVisNode->IsContainer()) {
				// Draw this container without special processing
				if (static_cast<const VisContainer*>(pVisNode)->GetCullQuery()) {
					PrepareRec(cRenderer, *static_cast<const VisContainer*>(pVisNode)->GetCullQuery());

					// Set the previous scissor rectangle
					cRenderer.SetScissorRect(&cVisContainer.GetProjection().cRectangle);
				}

			// Is this a light?
			} else if (pSceneNode->IsLight()) {
				// Has this light at least one effect?
				if ((pSceneNode->GetFlags() & SNLight::Corona) ||
					(pSceneNode->GetFlags() & SNLight::Flares) ||
					(pSceneNode->GetFlags() & SNLight::Blend)) {
					// First light effect?
					if (!m_lstLightEffects.GetNumOfElements()) {
						// Use corona material for better occlusion by using an alpha test
						Material *pMaterial = GetCoronaMaterialHandler().GetResource();
						if (pMaterial) {
							// Set the initial world matrix
							pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, Matrix4x4::Identity);

							// Setup renderer states
							pMaterial->SetupPass(0);
							// Disable color writes, and use flat shading for speed
							cRenderer.SetColorMask(false, false, false, false);
							cRenderer.SetRenderState(RenderState::ZEnable,      true);
							cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
							cRenderer.SetRenderState(RenderState::BlendEnable,  false);
							pFixedFunctions->SetRenderState(FixedFunctions::RenderState::ShadeMode, FixedFunctions::Shade::Flat);
						}
					}

					// Add this light effect
					LightEffect *pLightEffect;
					if (m_lstFreeLightEffects.IsEmpty()) {
						pLightEffect = new LightEffect;
						pLightEffect->pOcclusionQueryAll = cRenderer.CreateOcclusionQuery();
						pLightEffect->pOcclusionQuery    = cRenderer.CreateOcclusionQuery();
					} else {
						pLightEffect = m_lstFreeLightEffects.Get(0);
						m_lstFreeLightEffects.RemoveAtIndex(0);
					}
					pLightEffect->pVisNode = pVisNode;
					if (pLightEffect->pOcclusionQueryAll && pLightEffect->pOcclusionQuery) {
						// All
						const uint32 nTemp = cRenderer.GetRenderState(RenderState::ZFunc);
						cRenderer.SetRenderState(RenderState::ZFunc, Compare::Always);
						pLightEffect->pOcclusionQueryAll->BeginOcclusionQuery();
						DrawBillboard(pVisNode->GetWorldMatrix().GetTranslation(), static_cast<SNLight*>(pSceneNode)->CoronaSize);
						pLightEffect->pOcclusionQueryAll->EndOcclusionQuery();
						cRenderer.SetRenderState(RenderState::ZFunc, nTemp);
						// Visible
						pLightEffect->pOcclusionQuery->BeginOcclusionQuery();
						// We can do all the work again...
	//					DrawBillboard(pVisNode->GetWorldMatrix().GetTranslation(), ((SNLight*)pSceneNode)->GetCoronaSize()*0.3f);
						// ... or just redraw :)
						cRenderer.DrawPrimitives(Primitive::TriangleStrip, 0, 4);
						pLightEffect->pOcclusionQuery->EndOcclusionQuery();
					}
					m_lstLightEffects.Add(pLightEffect);
				}

			// This must just be a quite boring scene node :)
			} else {
				// Do nothing
			}
		}
	}
}

/**
*  @brief
*    Frees all light effects
*/
void SRPLightEffectsFixedFunctions::FreeLightEffects()
{
	{
		Iterator<LightEffect*> cIterator = m_lstLightEffects.GetIterator();
		while (cIterator.HasNext())
			m_lstFreeLightEffects.Add(cIterator.Next());
	}
	m_lstLightEffects.Clear();
}

/**
*  @brief
*    Draws a light effect flare
*/
void SRPLightEffectsFixedFunctions::DrawFlare(Renderer &cRenderer, float fP, float fS, float fR, float fG, float fB, float fA, const Vector3 &v2DPos) const
{
	TextureBuffer *pTextureBuffer = cRenderer.GetTextureBuffer(0);
	if (pTextureBuffer) {
		SamplerStates cSamplerStates;
		cRenderer.SetRenderState(RenderState::CullMode, Cull::None);
		cRenderer.GetDrawHelpers().DrawImage(*pTextureBuffer, cSamplerStates, Vector2(v2DPos.x*fP-fS, v2DPos.y*fP-fS), Vector2(fS*2, fS*2), Color4(fR, fG, fB, fA));
	}
}

/**
*  @brief
*    Draws a simple billboard
*/
void SRPLightEffectsFixedFunctions::DrawBillboard(const Vector3 &vPos, float fSize)
{
	// Create the billboard vertex buffer?
	if (!m_pBillboardVB) {
		m_pBillboardVB = GetSceneContext()->GetRendererContext().GetRenderer().CreateVertexBuffer();
		m_pBillboardVB->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float3);
		m_pBillboardVB->AddVertexAttribute(VertexBuffer::TexCoord, 0, VertexBuffer::Float2);
		m_pBillboardVB->Allocate(4, Usage::WriteOnly);

		// Set (fixed) texture coordinates
		if (m_pBillboardVB->Lock(Lock::WriteOnly)) {
			// Vertex 0 (right/bottom)
			float *pfVertex = static_cast<float*>(m_pBillboardVB->GetData(0, VertexBuffer::TexCoord));
			pfVertex[0] = 1.0f;
			pfVertex[1] = 1.0f;
			// Vertex 1 (left/bottom)
			pfVertex	= static_cast<float*>(m_pBillboardVB->GetData(1, VertexBuffer::TexCoord));
			pfVertex[0] = 0.0f;
			pfVertex[1] = 1.0f;
			// Vertex 2 (right/top)
			pfVertex	= static_cast<float*>(m_pBillboardVB->GetData(2, VertexBuffer::TexCoord));
			pfVertex[0] = 1.0f;
			pfVertex[1] = 0.0f;
			// Vertex 3 (left/top)
			pfVertex	= static_cast<float*>(m_pBillboardVB->GetData(3, VertexBuffer::TexCoord));
			pfVertex[0] = 0.0f;
			pfVertex[1] = 0.0f;

			// Unlock the vertex buffer
			m_pBillboardVB->Unlock();
		}
	}

	// Is there a billboard vertex buffer?
	if (m_pBillboardVB) {
		Renderer &cRenderer = m_pBillboardVB->GetRenderer();

		// Fixed functions support required
		FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
		if (pFixedFunctions) {
			// Setup the vertex buffer
			if (m_pBillboardVB->Lock(Lock::WriteOnly)) {
				const Matrix4x4 &mView = pFixedFunctions->GetTransformState(FixedFunctions::Transform::View);
				const Vector3 vDX(mView.xx, mView.xy, mView.xz);
				const Vector3 vDY(mView.yx, mView.yy, mView.yz);

				// Vertex 0 (right/bottom)
				float *pfVertex = static_cast<float*>(m_pBillboardVB->GetData(0, VertexBuffer::Position));
				pfVertex[0] = vPos.x + fSize*(vDX.x + vDY.x);
				pfVertex[1] = vPos.y + fSize*(vDX.y + vDY.y);
				pfVertex[2] = vPos.z + fSize*(vDX.z + vDY.z);

				// Vertex 1 (left/bottom)
				pfVertex	= static_cast<float*>(m_pBillboardVB->GetData(1, VertexBuffer::Position));
				pfVertex[0] = vPos.x + fSize*(-vDX.x + vDY.x);
				pfVertex[1] = vPos.y + fSize*(-vDX.y + vDY.y);
				pfVertex[2] = vPos.z + fSize*(-vDX.z + vDY.z);

				// Vertex 2 (right/top)
				pfVertex	= static_cast<float*>(m_pBillboardVB->GetData(2, VertexBuffer::Position));
				pfVertex[0] = vPos.x + fSize*(vDX.x - vDY.x);
				pfVertex[1] = vPos.y + fSize*(vDX.y - vDY.y);
				pfVertex[2] = vPos.z + fSize*(vDX.z - vDY.z);

				// Vertex 3 (left/top)
				pfVertex	= static_cast<float*>(m_pBillboardVB->GetData(3, VertexBuffer::Position));
				pfVertex[0] = vPos.x + fSize*(-vDX.x - vDY.x);
				pfVertex[1] = vPos.y + fSize*(-vDX.y - vDY.y);
				pfVertex[2] = vPos.z + fSize*(-vDX.z - vDY.z);

				// Unlock the vertex buffer
				m_pBillboardVB->Unlock();
			}

			// Set vertex buffers
			pFixedFunctions->SetVertexBuffer(m_pBillboardVB);

			// Draw the billboard
			cRenderer.DrawPrimitives(Primitive::TriangleStrip, 0, 4);
		}
	}
}

/**
*  @brief
*    Draws a simple billboard
*/
void SRPLightEffectsFixedFunctions::DrawBillboard(const Vector3 &vPos, float fSize, const Color4 &cColor)
{
	// Create the billboard vertex buffer?
	if (!m_pBillboardColorVB) {
		m_pBillboardColorVB = GetSceneContext()->GetRendererContext().GetRenderer().CreateVertexBuffer();
		m_pBillboardColorVB->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float3);
		m_pBillboardColorVB->AddVertexAttribute(VertexBuffer::TexCoord, 0, VertexBuffer::Float2);
		m_pBillboardColorVB->AddVertexAttribute(VertexBuffer::Color,    0, VertexBuffer::RGBA);
		m_pBillboardColorVB->Allocate(4, Usage::WriteOnly);

		// Set (fixed) texture coordinates
		if (m_pBillboardColorVB->Lock(Lock::WriteOnly)) {
			// Vertex 0 (right/bottom)
			float *pfVertex = static_cast<float*>(m_pBillboardColorVB->GetData(0, VertexBuffer::TexCoord));
			pfVertex[0] = 1.0f;
			pfVertex[1] = 1.0f;
			// Vertex 1 (left/bottom)
			pfVertex	= static_cast<float*>(m_pBillboardColorVB->GetData(1, VertexBuffer::TexCoord));
			pfVertex[0] = 0.0f;
			pfVertex[1] = 1.0f;
			// Vertex 2 (right/top)
			pfVertex	= static_cast<float*>(m_pBillboardColorVB->GetData(2, VertexBuffer::TexCoord));
			pfVertex[0] = 1.0f;
			pfVertex[1] = 0.0f;
			// Vertex 3 (left/top)
			pfVertex	= static_cast<float*>(m_pBillboardColorVB->GetData(3, VertexBuffer::TexCoord));
			pfVertex[0] = 0.0f;
			pfVertex[1] = 0.0f;

			// Unlock the vertex buffer
			m_pBillboardColorVB->Unlock();
		}
	}

	// Is there a billboard vertex buffer?
	if (m_pBillboardColorVB) {
		Renderer &cRenderer = m_pBillboardColorVB->GetRenderer();

		// Fixed functions support required
		FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
		if (pFixedFunctions) {
			// Setup the vertex buffer
			if (m_pBillboardColorVB->Lock(Lock::WriteOnly)) {
				const Matrix4x4 &mView = pFixedFunctions->GetTransformState(FixedFunctions::Transform::View);
				const Vector3 vDX(mView.xx, mView.xy, mView.xz);
				const Vector3 vDY(mView.yx, mView.yy, mView.yz);

				// Vertex 0 (right/bottom)
				float *pfVertex = static_cast<float*>(m_pBillboardColorVB->GetData(0, VertexBuffer::Position));
				pfVertex[0] = vPos.x + fSize*(vDX.x + vDY.x);
				pfVertex[1] = vPos.y + fSize*(vDX.y + vDY.y);
				pfVertex[2] = vPos.z + fSize*(vDX.z + vDY.z);
				m_pBillboardColorVB->SetColor(0, cColor);

				// Vertex 1 (left/bottom)
				pfVertex	= static_cast<float*>(m_pBillboardColorVB->GetData(1, VertexBuffer::Position));
				pfVertex[0] = vPos.x + fSize*(-vDX.x + vDY.x);
				pfVertex[1] = vPos.y + fSize*(-vDX.y + vDY.y);
				pfVertex[2] = vPos.z + fSize*(-vDX.z + vDY.z);
				m_pBillboardColorVB->SetColor(1, cColor);

				// Vertex 2 (right/top)
				pfVertex	= static_cast<float*>(m_pBillboardColorVB->GetData(2, VertexBuffer::Position));
				pfVertex[0] = vPos.x + fSize*(vDX.x - vDY.x);
				pfVertex[1] = vPos.y + fSize*(vDX.y - vDY.y);
				pfVertex[2] = vPos.z + fSize*(vDX.z - vDY.z);
				m_pBillboardColorVB->SetColor(2, cColor);

				// Vertex 3 (left/top)
				pfVertex	= static_cast<float*>(m_pBillboardColorVB->GetData(3, VertexBuffer::Position));
				pfVertex[0] = vPos.x + fSize*(-vDX.x - vDY.x);
				pfVertex[1] = vPos.y + fSize*(-vDX.y - vDY.y);
				pfVertex[2] = vPos.z + fSize*(-vDX.z - vDY.z);
				m_pBillboardColorVB->SetColor(3, cColor);

				// Unlock the vertex buffer
				m_pBillboardColorVB->Unlock();
			}

			// Set vertex buffers
			pFixedFunctions->SetVertexBuffer(m_pBillboardColorVB);

			// Draw the billboard
			cRenderer.DrawPrimitives(Primitive::TriangleStrip, 0, 4);
		}
	}
}


//[-------------------------------------------------------]
//[ Private virtual SceneRendererPass functions           ]
//[-------------------------------------------------------]
void SRPLightEffectsFixedFunctions::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Fixed functions support required
	FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
	if (pFixedFunctions) {
		// Prepare step?
		if (GetFlags() & PrepareStep) {
			// Draw anything?
			if (!(GetFlags() & NoCorona) || !(GetFlags() & NoFlares) || !(GetFlags() & NoBlend)) {
				// Ensure the correct projection and view matrix is set
				pFixedFunctions->SetTransformState(FixedFunctions::Transform::Projection, cCullQuery.GetProjectionMatrix());
				pFixedFunctions->SetTransformState(FixedFunctions::Transform::View,       cCullQuery.GetViewMatrix());

				// Backup the color mask
				bool bRed, bGreen, bBlue, bAlpha;
				cRenderer.GetColorMask(bRed, bGreen, bBlue, bAlpha);

				// Free all light effects
				FreeLightEffects();

				// Prepare recursive from back to front
				PrepareRec(cRenderer, cCullQuery);

				// Restore the color mask
				cRenderer.SetColorMask(bRed, bGreen, bBlue, bAlpha);
			}
		} else {
			// Get the "PLCompositing::SRPLightEffectsFixedFunctions" instance used for the prepare step
			SRPLightEffectsFixedFunctions *pSRPLightEffectsFixedFunctions = static_cast<SRPLightEffectsFixedFunctions*>(GetFirstInstanceOfSceneRendererPassClass("PLCompositing::SRPLightEffectsFixedFunctions"));
			if (pSRPLightEffectsFixedFunctions) {
				// Are there any light effects to draw?
				if (pSRPLightEffectsFixedFunctions->m_lstLightEffects.GetNumOfElements()) {
					// Ensure the correct projection and view matrix is set
					pFixedFunctions->SetTransformState(FixedFunctions::Transform::Projection, cCullQuery.GetProjectionMatrix());
					pFixedFunctions->SetTransformState(FixedFunctions::Transform::View,       cCullQuery.GetViewMatrix());

					// Set the initial world matrix
					pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, Matrix4x4::Identity);
					cRenderer.SetScissorRect();
					cRenderer.SetRenderState(RenderState::ScissorTestEnable, true);

					// Options
					const bool bCoronaActive = !(GetFlags() & NoCorona);
					const bool bFlaresActive = !(GetFlags() & NoFlares);
					const bool bBlendActive  = !(GetFlags() & NoBlend);

				// Optimized for all 'standard' lights
					bool bEffectLights = false; // Are there any 'effect lights'?
					bool bFlareBlend   = false; // Are there any lights with flares/blend?
					Material *pMaterial = nullptr;
					const float fWidth  = cRenderer.GetViewport().GetWidth();
					const float fHeight = cRenderer.GetViewport().GetHeight();
					const uint32 nTotalFragments = static_cast<uint32>(fWidth*fHeight);
					// Free light effect, get factor and draw coronas
					Iterator<LightEffect*> cIterator = pSRPLightEffectsFixedFunctions->m_lstLightEffects.GetIterator();
					while (cIterator.HasNext()) {
						// Get
						LightEffect *pLightEffect = cIterator.Next();
						pSRPLightEffectsFixedFunctions->m_lstFreeLightEffects.Add(pLightEffect);

						// Check occlusion
						pLightEffect->fFactor = 1.0f;
						if (pLightEffect->pOcclusionQueryAll && pLightEffect->pOcclusionQuery) {
							uint32 nNumberOfVisibleFragmentsAll;
							while (!pLightEffect->pOcclusionQueryAll->PullOcclusionQuery(&nNumberOfVisibleFragmentsAll)) {
								// We have to wait for the result...
							}
							if (nNumberOfVisibleFragmentsAll) {
								uint32 nNumberOfVisibleFragments;
								while (!pLightEffect->pOcclusionQuery->PullOcclusionQuery(&nNumberOfVisibleFragments)) {
									// We have to wait for the result...
								}
								pLightEffect->fFactor = static_cast<float>(nNumberOfVisibleFragments)/static_cast<float>(nNumberOfVisibleFragmentsAll);
								pLightEffect->fFlareBlendFactor = pLightEffect->fFactor*Math::Min(static_cast<float>(nNumberOfVisibleFragmentsAll)/static_cast<float>(nTotalFragments)*1000.0f, 1.0f);
								pLightEffect->fFactor = Math::Pow(pLightEffect->fFactor, 2.0f);
							} else {
								pLightEffect->fFactor = pLightEffect->fFlareBlendFactor = 0.0f;
							}
						}
						SNLight *pLight = static_cast<SNLight*>(pLightEffect->pVisNode->GetSceneNode());
						if (pLightEffect->fFactor) {
							// Calculate some special flare/blend data
							if ((bFlaresActive && (pLight->GetFlags() & SNLight::Flares)) ||
								(bBlendActive  && (pLight->GetFlags() & SNLight::Blend))) {
								// Get 2D position
								Vector3 &v2DPos = pLightEffect->v2DPos;
								v2DPos = Vector3::Zero.To2DCoordinate(pLightEffect->pVisNode->GetWorldViewProjectionMatrix(), cRenderer.GetViewport());
								v2DPos.x = v2DPos.x/fWidth*2.0f - 1.0f;
								v2DPos.y = v2DPos.y/fHeight*2.0f - 1.0f;

								// Adjust the factor so it is brighter at the center of the screen
								pLightEffect->fFlareBlendFactor *= 1.0f - Math::Min(Math::Sqrt(v2DPos.x*v2DPos.x + v2DPos.y*v2DPos.y), 1.0f);
								if (pLightEffect->fFlareBlendFactor < 0.1f)
									pLightEffect->fFlareBlendFactor = 0.0f;
								else
									bFlareBlend = true;
							}

							// Draw
							if (pLight->IsEffectLight())
								bEffectLights = true;
							else {
								if (bCoronaActive && (pLight->GetFlags() & SNLight::Corona)) {
									const Color3 cColor = pLight->Color.Get();
									if (pMaterial != GetCoronaMaterialHandler().GetResource()) {
										pMaterial = GetCoronaMaterialHandler().GetResource();
										pMaterial->SetupPass(0);
									}
									DrawBillboard(pLightEffect->pVisNode->GetWorldMatrix().GetTranslation(), pLight->CoronaSize,
												  Color4(cColor.r, cColor.g, cColor.b, pLightEffect->fFactor));
								}
							}
						} else {
							pLightEffect->fFlareBlendFactor = 0.0f;
						}
					}

					// Give us MOOOORRE light effects :)
					if (bFlareBlend) {
						SamplerStates cSamplerStates;

						// Draw flares
						pMaterial = nullptr;
						while (cIterator.HasPrevious()) {
							// Get
							LightEffect *pLightEffect = cIterator.Previous();
							SNLight *pLight = static_cast<SNLight*>(pLightEffect->pVisNode->GetSceneNode());
							if (!pLight->IsEffectLight()) {
								const float fFactor = pLightEffect->fFlareBlendFactor;
								if (fFactor && bFlaresActive && (pLight->GetFlags() & SNLight::Flares)) {
									// Draw
									const Color3 &cColor = pLight->Color.Get();
									if (pMaterial != GetFlareMaterialHandler().GetResource()) {
										pMaterial = GetFlareMaterialHandler().GetResource();
										pMaterial->SetupPass(0);
									}
									const float fFlareSize = pLight->FlareSize*fFactor;
									const Vector3 &v2DPos = pLightEffect->v2DPos;

									// Begin 2D mode
									DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
									cDrawHelpers.Begin2DMode(-1.0f, -1.0f, 1.0f, 1.0f);

										// Draw the flares
										DrawFlare(cRenderer,  0.7f, 0.2f*fFlareSize,      cColor.r,      cColor.g,      cColor.b, 0.5f*fFactor, v2DPos);
										DrawFlare(cRenderer,  0.6f, 0.3f*fFlareSize,      cColor.r, 0.6f*cColor.g, 0.6f*cColor.b, 0.5f*fFactor, v2DPos);
										DrawFlare(cRenderer,  0.4f, 0.4f*fFlareSize,      cColor.r,      cColor.g,      cColor.b, 0.5f*fFactor, v2DPos);
										DrawFlare(cRenderer,  0.3f, 0.6f*fFlareSize, 0.8f*cColor.r, 0.8f*cColor.g, 0.6f*cColor.b, 0.5f*fFactor, v2DPos);
										DrawFlare(cRenderer,  0.2f, 0.5f*fFlareSize,      cColor.r,      cColor.g,      cColor.b, 0.5f*fFactor, v2DPos);
										DrawFlare(cRenderer, -0.1f, 0.4f*fFlareSize, 0.6f*cColor.r,      cColor.g, 0.6f*cColor.b, 0.5f*fFactor, v2DPos);
										DrawFlare(cRenderer, -0.2f, 0.3f*fFlareSize,      cColor.r,      cColor.g,      cColor.b, 0.5f*fFactor, v2DPos);
										DrawFlare(cRenderer, -0.4f, 0.2f*fFlareSize, 0.6f*cColor.r, 0.8f*cColor.g, 0.8f*cColor.b, 0.5f*fFactor, v2DPos);
										DrawFlare(cRenderer, -0.6f, 0.4f*fFlareSize,      cColor.r,      cColor.g,      cColor.b, 0.5f*fFactor, v2DPos);
										DrawFlare(cRenderer, -0.7f, 0.5f*fFlareSize, 0.6f*cColor.r, 0.6f*cColor.g,      cColor.b, 0.5f*fFactor, v2DPos);
										DrawFlare(cRenderer, -0.8f, 0.6f*fFlareSize,      cColor.r,      cColor.g,      cColor.b, 0.5f*fFactor, v2DPos);
										DrawFlare(cRenderer, -0.9f, 0.5f*fFlareSize, 0.8f*cColor.r, 0.6f*cColor.g, 0.8f*cColor.b, 0.5f*fFactor, v2DPos);
										DrawFlare(cRenderer, -1.2f, 0.3f*fFlareSize,      cColor.r,      cColor.g,      cColor.b, 0.5f*fFactor, v2DPos);

									// End 2D mode
									cDrawHelpers.End2DMode();
								}
							}
						}

						// Draw blend
						pMaterial = nullptr;
						while (cIterator.HasNext()) {
							// Get
							const LightEffect *pLightEffect = cIterator.Next();
							const SNLight *pLight = static_cast<SNLight*>(pLightEffect->pVisNode->GetSceneNode());
							if (!pLight->IsEffectLight() && pLightEffect->fFlareBlendFactor && bBlendActive && (pLight->GetFlags() & SNLight::Blend)) {
								float fFactor = pLightEffect->fFlareBlendFactor*pLight->ScreenBrighten;
								if (fFactor) {
									// Draw
									const Color3 cColor = pLight->Color.Get();
									if (pMaterial != GetBlendMaterialHandler().GetResource()) {
										pMaterial = GetBlendMaterialHandler().GetResource();
										pMaterial->SetupPass(0);
									}

									// Begin 2D mode
									DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
									cDrawHelpers.Begin2DMode();

										// Draw image
										TextureBuffer *pTextureBuffer = cRenderer.GetTextureBuffer(0);
										if (pTextureBuffer) {
											cRenderer.SetRenderState(RenderState::CullMode, Cull::None);
											cDrawHelpers.DrawImage(*pTextureBuffer, cSamplerStates, Vector2::Zero, Vector2::One, Color4(cColor.r*fFactor, cColor.g*fFactor, cColor.b*fFactor, fFactor));
										}

									// End 2D mode
									cDrawHelpers.End2DMode();
								}
							}
						}
					}

				// Draw special 'effect lights'
					if (bEffectLights) {
						while (cIterator.HasPrevious()) {
							// Get
							const LightEffect *pLightEffect = cIterator.Previous();
							const SNLight *pLightT = static_cast<SNLight*>(pLightEffect->pVisNode->GetSceneNode());
							if (pLightT->IsEffectLight()) {
								SNEffectLight *pLight = const_cast<SNEffectLight*>(static_cast<const SNEffectLight*>(pLightT));
								if (pLightEffect->fFactor) {
									const Color3 cColor = pLight->Color.Get();

									// Corona
									if (bCoronaActive && (pLight->GetFlags() & SNLight::Corona)) {
										pMaterial = pLight->GetCoronaMaterialHandler().GetResource();
										if (pMaterial) {
											for (uint32 nPass=0; nPass<pMaterial->GetNumOfPasses(); nPass++) {
												pMaterial->SetupPass(nPass);
												if (nPass)
													cRenderer.DrawPrimitives(Primitive::TriangleStrip, 0, 4);
												else {
													DrawBillboard(pLightEffect->pVisNode->GetWorldMatrix().GetTranslation(), pLight->CoronaSize,
																  Color4(cColor.r, cColor.g, cColor.b, pLightEffect->fFactor));
												}
											}
										}
									}

									// Flares
									if (bFlaresActive && (pLight->GetFlags() & SNLight::Flares) && pLightEffect->fFlareBlendFactor) {
										pMaterial = pLight->GetFlareMaterialHandler().GetResource();
										if (pMaterial) {
											const float fFactor = pLightEffect->fFlareBlendFactor;
											const float fFlareSize = pLight->FlareSize*fFactor;
											if (fFlareSize) {
												const Vector3 &v2DPos = pLightEffect->v2DPos;

												// Begin 2D mode
												DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
												cDrawHelpers.Begin2DMode(-1.0f, -1.0f, 1.0f, 1.0f);

													// Draw the flares
													for (uint32 nPass=0; nPass<pMaterial->GetNumOfPasses(); nPass++) {
														pMaterial->SetupPass(nPass);
														DrawFlare(cRenderer,  0.7f, 0.2f*fFlareSize,      cColor.r,      cColor.g,      cColor.b, 0.5f*fFactor, v2DPos);
														DrawFlare(cRenderer,  0.6f, 0.3f*fFlareSize,      cColor.r, 0.6f*cColor.g, 0.6f*cColor.b, 0.5f*fFactor, v2DPos);
														DrawFlare(cRenderer,  0.4f, 0.4f*fFlareSize,      cColor.r,      cColor.g,      cColor.b, 0.5f*fFactor, v2DPos);
														DrawFlare(cRenderer,  0.3f, 0.6f*fFlareSize, 0.8f*cColor.r, 0.8f*cColor.g, 0.6f*cColor.b, 0.5f*fFactor, v2DPos);
														DrawFlare(cRenderer,  0.2f, 0.5f*fFlareSize,      cColor.r,      cColor.g,      cColor.b, 0.5f*fFactor, v2DPos);
														DrawFlare(cRenderer, -0.1f, 0.4f*fFlareSize, 0.6f*cColor.r,      cColor.g, 0.6f*cColor.b, 0.5f*fFactor, v2DPos);
														DrawFlare(cRenderer, -0.2f, 0.3f*fFlareSize,      cColor.r,      cColor.g,      cColor.b, 0.5f*fFactor, v2DPos);
														DrawFlare(cRenderer, -0.4f, 0.2f*fFlareSize, 0.6f*cColor.r, 0.8f*cColor.g, 0.8f*cColor.b, 0.5f*fFactor, v2DPos);
														DrawFlare(cRenderer, -0.6f, 0.4f*fFlareSize,      cColor.r,      cColor.g,      cColor.b, 0.5f*fFactor, v2DPos);
														DrawFlare(cRenderer, -0.7f, 0.5f*fFlareSize, 0.6f*cColor.r, 0.6f*cColor.g,      cColor.b, 0.5f*fFactor, v2DPos);
														DrawFlare(cRenderer, -0.8f, 0.6f*fFlareSize,      cColor.r,      cColor.g,      cColor.b, 0.5f*fFactor, v2DPos);
														DrawFlare(cRenderer, -0.9f, 0.5f*fFlareSize, 0.8f*cColor.r, 0.6f*cColor.g, 0.8f*cColor.b, 0.5f*fFactor, v2DPos);
														DrawFlare(cRenderer, -1.2f, 0.3f*fFlareSize,      cColor.r,      cColor.g,      cColor.b, 0.5f*fFactor, v2DPos);
													}

												// End 2D mode
												cDrawHelpers.End2DMode();
											}
										}
									}

									// Blend
									if (bBlendActive && (pLight->GetFlags() & SNLight::Blend) && pLightEffect->fFlareBlendFactor) {
										pMaterial = pLight->GetBlendMaterialHandler().GetResource();
										if (pMaterial) {
											const float fFactor = pLightEffect->fFlareBlendFactor*pLight->ScreenBrighten;
											if (fFactor) {
												// Begin 2D mode
												DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
												cDrawHelpers.Begin2DMode();

													// Draw image
													SamplerStates cSamplerStates;
													for (uint32 nPass=0; nPass<pMaterial->GetNumOfPasses(); nPass++) {
														pMaterial->SetupPass(nPass);
														TextureBuffer *pTextureBuffer = cRenderer.GetTextureBuffer(0);
														if (pTextureBuffer) {
															cRenderer.SetRenderState(RenderState::CullMode, Cull::None);
															cDrawHelpers.DrawImage(*pTextureBuffer, cSamplerStates, Vector2::Zero, Vector2::One, Color4(cColor.r*fFactor, cColor.g*fFactor, cColor.b*fFactor, fFactor));
														}
													}

												// End 2D mode
												cDrawHelpers.End2DMode();
											}
										}
									}
								}
							}
						}
					}

					// Done
					pSRPLightEffectsFixedFunctions->m_lstLightEffects.Clear();
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
