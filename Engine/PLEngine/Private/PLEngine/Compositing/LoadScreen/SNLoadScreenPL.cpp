/*********************************************************\
 *  File: SNLoadScreenPL.cpp                             *
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
#include <PLCore/Tools/Localization.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Font.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Renderer/FontManager.h>
#include <PLRenderer/Renderer/SamplerStates.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLRenderer/Material/MaterialManager.h>
#include <PLScene/Scene/SceneContext.h>
#include "PLEngine/Compositing/LoadScreen/SNLoadScreenPL.h"


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
pl_implement_class(SNLoadScreenPL)


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNLoadScreenPL::SNLoadScreenPL() :
	ClearFlags(this),
	ClearColor(this),
	Flags(this)
{
	// Set draw function flags
	SetDrawFunctionFlags(static_cast<uint8>(GetDrawFunctionFlags() | UseDrawPost));
}

/**
*  @brief
*    Destructor
*/
SNLoadScreenPL::~SNLoadScreenPL()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Free's the used materials
*/
void SNLoadScreenPL::FreeMaterials()
{
	m_cLogoMaterial.SetResource();
}


//[-------------------------------------------------------]
//[ Public virtual PLScene::SceneNode functions           ]
//[-------------------------------------------------------]
void SNLoadScreenPL::DrawPost(Renderer &cRenderer, const VisNode *pVisNode)
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

		// Setup render states
		cRenderer.GetRendererContext().GetEffectManager().Use();
		cRenderer.SetRenderState(RenderState::ZEnable,      false);
		cRenderer.SetRenderState(RenderState::ZWriteEnable, false);

		// Begin 2D mode
		SamplerStates cSamplerStates;
		DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
		cDrawHelpers.Begin2DMode();

			// PixelLight logo
			Material *pMaterial = m_cLogoMaterial.GetResource();
			if (!pMaterial) {
				pMaterial = cRenderer.GetRendererContext().GetMaterialManager().LoadResource("Data/Textures/PLLogo.dds");
				if (pMaterial)
					m_cLogoMaterial.SetResource(pMaterial);
			}
			if (pMaterial) {
				TextureBuffer *pDiffuseMap = pMaterial->GetParameterTextureBuffer(Material::DiffuseMap);
				if (pDiffuseMap) {
					cRenderer.SetRenderState(RenderState::CullMode, Cull::None);
					cDrawHelpers.DrawImage(*pDiffuseMap, cSamplerStates, Vector2(0.4f, 0.4f), Vector2(0.2f, 0.2f), Color4::White);
				}
			}

			// Draw text
			Font *pFont = cRenderer.GetFontManager().GetDefaultFontTexture();
			if (pFont)
				cDrawHelpers.DrawText(*pFont, PLT("Loading") + String::Format(" - %.0f %%", fProgress*100.0f), Color4::White, Vector2(0.5f, 0.9f), Font::CenterText);

		// End 2D mode
		cDrawHelpers.End2DMode();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
