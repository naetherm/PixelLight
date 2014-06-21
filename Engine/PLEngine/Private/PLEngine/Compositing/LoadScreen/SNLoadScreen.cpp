/*********************************************************\
 *  File: SNLoadScreen.cpp                               *
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
#include <PLMath/Matrix4x4.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Renderer/SamplerStates.h>
#include <PLRenderer/Material/MaterialManager.h>
#include <PLScene/Scene/SceneContext.h>
#include "PLEngine/Compositing/LoadScreen/SNLoadScreen.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLScene;
namespace PLEngine {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNLoadScreen)


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNLoadScreen::SNLoadScreen() :
	ClearFlags(this),
	ClearColor(this),
	Images(this),
	ImageX(this),
	ImageY(this),
	ImageWidth(this),
	ImageHeight(this),
	ImageColor(this),
	BarX(this),
	BarY(this),
	BarWidth(this),
	BarHeight(this),
	BarColor(this),
	Flags(this),
	m_sBarBackMaterial("Data/Materials/LoadScreen/BarBack.mat"),
	m_fBarBackAlpha(0.0f)
{
	// Init data
	m_sScreenMaterial[0] = "Data/Materials/LoadScreen/0.mat";
	m_sScreenMaterial[1] = "Data/Materials/LoadScreen/1.mat";
	m_sScreenMaterial[2] = "Data/Materials/LoadScreen/2.mat";
	m_sScreenMaterial[3] = "Data/Materials/LoadScreen/3.mat";
	m_sBarMaterial[0]    = "Data/Materials/LoadScreen/Bar0.mat";
	m_sBarMaterial[1]    = "Data/Materials/LoadScreen/Bar1.mat";
	m_fAlpha[0] = m_fAlpha[1] = m_fAlpha[2] = m_fAlpha[3] = 0.0f;
	m_fBarAlpha[0] = m_fBarAlpha[1] = 0.0f;

	// Set draw function flags
	SetDrawFunctionFlags(static_cast<uint8>(GetDrawFunctionFlags() | UseDrawPost));
}

/**
*  @brief
*    Destructor
*/
SNLoadScreen::~SNLoadScreen()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Free's the used materials
*/
void SNLoadScreen::FreeMaterials()
{
	for (uint8 i=0; i<Images; i++)
		m_cScreenMaterial[i].SetResource();
	m_cBarBackMaterial.SetResource();
	m_cBarMaterial[0].SetResource();
	m_cBarMaterial[1].SetResource();
}


//[-------------------------------------------------------]
//[ Public virtual PLScene::SceneNode functions           ]
//[-------------------------------------------------------]
void SNLoadScreen::DrawPost(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Call base implementation
	SNLoadScreenBase::DrawPost(cRenderer, pVisNode);

	// Hide the load screen?
	const float fProgress = GetLoadProgress();
	if (fProgress > 1.0f || fProgress < 0.0f) {
		// We do not longer need the used materials :)
		FreeMaterials();
	} else {
		// Clear the screen
		if (ClearFlags) {
			uint32 nFlags = 0;
			if (ClearFlags & Color)
				nFlags |= Clear::Color;
			if (ClearFlags & Depth)
				nFlags |= Clear::ZBuffer;
			if (ClearFlags & Stencil)
				nFlags |= Clear::Stencil;
			cRenderer.Clear(nFlags, ClearColor.Get());
		}

		// Screen alpha
		const float fPerImage = (Images > 1) ? 1.0f/Images : 0.1f;
		if (GetFlags() & NoImageBlend) {
			// No image blend, please
			for (uint8 i=0; i<Images; i++) {
				m_fAlpha[i] = (fProgress-(static_cast<float>(i)*fPerImage))/fPerImage;
				m_fAlpha[i] = (m_fAlpha[i] > 1.0f || m_fAlpha[i] < 0.0f) ? 0.0f : 1.0f;
			}
		} else {
			if (Images > 1) {
				// Image blend, please
				for (uint8 i=0; i<Images; i++) {
					m_fAlpha[i] = (fProgress-(static_cast<float>(i)*fPerImage))/fPerImage;
					if (m_fAlpha[i] > 1.0f)
						m_fAlpha[i] = 1.0f-(fProgress-(fPerImage*static_cast<float>(i+1)))/fPerImage;
				}
			} else {
				m_fAlpha[0] = fProgress*10;
				if (m_fAlpha[0] > 1.0f)
					m_fAlpha[0] = 1.0f;
			}
		}

		// Load bar alpha
		m_fBarBackAlpha = fProgress/0.25f;
		if (m_fBarBackAlpha > 1.0f)
			m_fBarBackAlpha = 1.0f;
		m_fBarAlpha[0] = (Math::Sin(fProgress)/4+0.75f)*m_fBarBackAlpha;
		m_fBarAlpha[1] = (Math::Cos(fProgress)/4+0.75f)*m_fBarBackAlpha;
		const float fBarRight = BarX+BarWidth*fProgress;

		// Begin 2D mode
		SamplerStates cSamplerStates;
		DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
		cDrawHelpers.Begin2DMode();

			// Screen
			const Color4 cImageColor = ImageColor.Get();
			MaterialManager &cMaterialManager = cRenderer.GetRendererContext().GetMaterialManager();
			for (uint8 i=0; i<Images; i++) {
				if (m_fAlpha[i] > 0.0f) {
					Material *pMaterial = m_cScreenMaterial[i].GetResource();
					if (!pMaterial) {
						pMaterial = cMaterialManager.LoadResource(m_sScreenMaterial[i]);
						if (pMaterial)
							m_cScreenMaterial[i].SetResource(pMaterial);
					}
					if (pMaterial) {
						for (uint32 nPass=0; nPass<pMaterial->GetNumOfPasses(); nPass++) {
							pMaterial->SetupPass(nPass);
							TextureBuffer *pTextureBuffer = cRenderer.GetTextureBuffer(0);
							if (pTextureBuffer) {
								cRenderer.SetRenderState(RenderState::CullMode, Cull::None);
								cDrawHelpers.DrawImage(*pTextureBuffer, cSamplerStates, Vector2(ImageX, ImageY), Vector2(ImageWidth, ImageHeight),
													   Color4(cImageColor.r, cImageColor.g, cImageColor.b, cImageColor.a*m_fAlpha[i]));
							}
						}
					}

				// Else unload (if it's loaded :) the material because the don't need it!
				} else {
					m_cScreenMaterial[i].SetResource();
				}
			}

			// Load bar background
			const Color4 cBarColor = BarColor.Get();
			Material *pMaterial = m_cBarBackMaterial.GetResource();
			if (pMaterial) {
				for (uint32 nPass=0; nPass<pMaterial->GetNumOfPasses(); nPass++) {
					pMaterial->SetupPass(nPass);
					TextureBuffer *pTextureBuffer = cRenderer.GetTextureBuffer(0);
					if (pTextureBuffer) {
						cRenderer.SetRenderState(RenderState::CullMode, Cull::None);
						cDrawHelpers.DrawImage(*pTextureBuffer, cSamplerStates, Vector2(BarX, BarY), Vector2(BarWidth, BarHeight),
											   Color4(cBarColor.r, cBarColor.g, cBarColor.b, (GetFlags() & NoImageBlend) ? cBarColor.a : cBarColor.a*m_fBarBackAlpha));
					}
				}

			// Try to load it
			} else {
				m_cBarBackMaterial.SetResource(cMaterialManager.LoadResource(m_sBarBackMaterial));
			}

			// Load bar
			for (int i=0; i<2; i++) {
				pMaterial = m_cBarMaterial[i].GetResource();
				if (pMaterial) {
					for (uint32 nPass=0; nPass<pMaterial->GetNumOfPasses(); nPass++) {
						pMaterial->SetupPass(nPass);
						TextureBuffer *pTextureBuffer = cRenderer.GetTextureBuffer(0);
						if (pTextureBuffer) {
							cRenderer.SetRenderState(RenderState::CullMode, Cull::None);
							cDrawHelpers.DrawImage(*pTextureBuffer, cSamplerStates, Vector2(BarX, BarY), Vector2(fBarRight - BarX, BarHeight),
												   Color4(cBarColor.r, cBarColor.g, cBarColor.b, cBarColor.a*m_fBarAlpha[i]), 1.0f, Vector2::Zero, Vector2(fProgress, 1.0f));
						}
					}

				// Try to load it
				} else {
					m_cBarMaterial[i].SetResource(cMaterialManager.LoadResource(m_sBarMaterial[i]));
				}
			}

		// End 2D mode
		cDrawHelpers.End2DMode();
	}
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Base functions                 ]
//[-------------------------------------------------------]
// [TODO] New RTTI usage
/*
void SNLoadScreen::VarChanged(const String &sVar)
{
	// Is the scene node initialized?
	if (IsInitialized()) {
		// Get material manager
		MaterialManager &cMaterialManager = GetSceneContext()->GetRendererContext().GetMaterialManager();

		// ScreenMaterial[0]
		if (sVar  == "ScreenMaterial[0]") {
			m_cScreenMaterial[0].SetResource(cMaterialManager.LoadResource(m_sScreenMaterial[0]));

		// ScreenMaterial[1]
		} else if (sVar == "ScreenMaterial[1]") {
			m_cScreenMaterial[1].SetResource(cMaterialManager.LoadResource(m_sScreenMaterial[1]));

		// ScreenMaterial[2]
		} else if (sVar == "ScreenMaterial[2]") {
			m_cScreenMaterial[2].SetResource(cMaterialManager.LoadResource(m_sScreenMaterial[2]));

		// ScreenMaterial[3]
		} else if (sVar == "ScreenMaterial[3]") {
			m_cScreenMaterial[3].SetResource(cMaterialManager.LoadResource(m_sScreenMaterial[3]));

		// BarBackMaterial
		} else if (sVar == "BarBackMaterial") {
			m_cBarBackMaterial.SetResource(cMaterialManager.LoadResource(m_sBarBackMaterial));

		// BarMaterial[0]
		} else if (sVar == "BarMaterial[0]") {
			m_cBarMaterial[0].SetResource(cMaterialManager.LoadResource(m_sBarMaterial[0]));

		// BarMaterial[1]
		} else if (sVar == "BarMaterial[1]") {
			m_cBarMaterial[1].SetResource(cMaterialManager.LoadResource(m_sBarMaterial[1]));

		// For all other...
		} else {
			// Call base implementation
			SNLoadScreenBase::VarChanged(sVar);
		}
	}
}*/


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
