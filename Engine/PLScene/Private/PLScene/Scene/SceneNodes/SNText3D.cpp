/*********************************************************\
 *  File: SNText3D.cpp                                   *
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
#include <PLCore/File/Url.h>
#include <PLCore/Tools/Tools.h>
#include <PLMath/Matrix4x4.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Font.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Renderer/FontManager.h>
#include <PLRenderer/Renderer/FixedFunctions.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLRenderer/Material/MaterialManager.h>
#include "PLScene/Visibility/VisNode.h"
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneNodes/SNText3D.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNText3D)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SNText3D::GetBackgroundMaterial() const
{
	return m_sBackgroundMaterial;
}

void SNText3D::SetBackgroundMaterial(const String &sValue)
{
	if (m_sBackgroundMaterial != sValue) {
		m_sBackgroundMaterial = sValue;
		m_bBackgroundMaterial = false;
	}
}

void SNText3D::SetText(const String &sValue)
{
	// Call base implementation
	SNText::SetText(sValue);

	// We have to recalculate the current axis align bounding box in 'scene node space'
	DirtyAABoundingBox();
}

void SNText3D::SetFont(const String &sValue)
{
	// Call base implementation
	SNText::SetFont(sValue);

	// We have to recalculate the current axis align bounding box in 'scene node space'
	DirtyAABoundingBox();
}

void SNText3D::SetFontSize(uint32 nValue)
{
	// Call base implementation
	SNText::SetFontSize(nValue);

	// We have to recalculate the current axis align bounding box in 'scene node space'
	DirtyAABoundingBox();
}

void SNText3D::SetFlags(uint32 nValue)
{
	// Call base implementation
	SNText::SetFlags(nValue);

	// We have to recalculate the current axis align bounding box in 'scene node space'
	DirtyAABoundingBox();
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNText3D::SNText3D() :
	BackgroundMaterial(this),
	Flags(this),
	m_bBackgroundMaterial(false)
{
	// Set draw function flags
	SetDrawFunctionFlags(static_cast<uint8>(GetDrawFunctionFlags() | UseDrawTransparent));

	// We have to recalculate the current axis align bounding box in 'scene node space'
	DirtyAABoundingBox();
}

/**
*  @brief
*    Destructor
*/
SNText3D::~SNText3D()
{
}

/**
*  @brief
*    Returns the handler of the background material
*/
const MaterialHandler &SNText3D::GetBackgroundMaterialHandler()
{
	if (!m_bBackgroundMaterial) {
		// Load material
		Material *pMaterial = GetSceneContext()->GetRendererContext().GetMaterialManager().LoadResource(m_sBackgroundMaterial);
		m_cBackgroundMaterial.SetResource(pMaterial);

		// Check whether this is a material or an effect, if not, we need to set a nice default billboard effect
		if (pMaterial) {
			const String sExtension = Url(pMaterial->GetName()).GetExtension();
			if (sExtension != "mat" && sExtension != "plfx") {
				// Set the text background default effect
				Material *pDefaultMaterial = GetSceneContext()->GetRendererContext().GetMaterialManager().LoadResource("Data/Effects/DefaultBitmap.plfx");
				if (pDefaultMaterial)
					pMaterial->SetEffect(pDefaultMaterial->GetEffect());
			}
		}
		m_bBackgroundMaterial = true;
	}
	return m_cBackgroundMaterial;
}


//[-------------------------------------------------------]
//[ Public virtual SceneNode functions                    ]
//[-------------------------------------------------------]
void SNText3D::DrawTransparent(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Is there any text to draw?
	if (GetText().GetLength() && pVisNode) {
		const PLRenderer::Font *pDefaultFont = cRenderer.GetFontManager().GetDefaultFontTexture();
		const String			sFont        = (GetFont().GetLength() || !pDefaultFont) ? GetFont() : pDefaultFont->GetFilename();
		const uint32			nFontSize    = (GetFontSize() || !pDefaultFont) ? GetFontSize() : pDefaultFont->GetSize();
			  PLRenderer::Font *pFont        = reinterpret_cast<PLRenderer::Font*>(cRenderer.GetFontManager().GetFontTexture(sFont, nFontSize));
		if (pFont) {
			// Is there a background material?
			Material *pMaterial = GetBackgroundMaterialHandler().GetResource();
			if (pMaterial) {
				// Loop through all material passes
				cRenderer.SetRenderState(RenderState::InvCullMode, 1);
				for (uint32 nPass=0; nPass<pMaterial->GetNumOfPasses(); nPass++) {
					// Setup render states
					pMaterial->SetupPass(nPass);
					cRenderer.SetRenderState(RenderState::CullMode, (GetFlags() & NoBackfaceCulling) ? Cull::None : Cull::CCW);

					// Draw the bitmap
					Color4 cColor;
					FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
					if (pFixedFunctions)
						cColor = pFixedFunctions->GetColor();
					// [TODO] Draw helpers update
					/*
					const float fWidth = pFont->GetTextWidth(GetText())/pFont->GetHeight();
					if (GetFlags () & NoCenter)
						cRenderer.GetDrawHelpers().DrawBitmap(cColor, -0.25f, -0.25f, fWidth + 0.25f, 0.75f);
					else
						cRenderer.GetDrawHelpers().DrawBitmap(cColor, -fWidth*0.5f - 0.25f, -0.25f, fWidth*0.5f + 0.25f, 0.75f);
					*/
				}
				cRenderer.SetRenderState(RenderState::InvCullMode, 0);
			}

			// Setup render states
			cRenderer.GetRendererContext().GetEffectManager().Use();
			cRenderer.SetRenderState(RenderState::CullMode, (GetFlags() & NoBackfaceCulling) ? Cull::None : Cull::CCW);
			if (pMaterial) {
				// We need to add a depth offset to avoid z-fighting if theres a background material
				cRenderer.SetRenderState(RenderState::SlopeScaleDepthBias, Tools::FloatToUInt32(-1.0f));
				cRenderer.SetRenderState(RenderState::DepthBias,		   Tools::FloatToUInt32(-2.0f));
			}

			// The 3D placed text should have a height of 1 to be independent of the actual 'font size'
			const float fScale = 1.0f/pFont->GetHeight();

			// Get font draw flags
			uint32 nFlags = Font::Mipmapping;
			if (!(GetFlags() & NoCenter))
				nFlags |= Font::CenterText;
			// [TODO] Update SNText flags
			// UnderlineText = 1<<1,	/**< Underline the text */
			// CrossoutText  = 1<<2	/**< Cross out the text */

			// Draw the text
			pFont->Draw(GetText(), Color.Get(), pVisNode->GetWorldViewProjectionMatrix(), Vector2(fScale, fScale), Vector2::Zero, nFlags);
		}
	}

	// Call base implementation
	SNText::DrawTransparent(cRenderer, pVisNode);
}


//[-------------------------------------------------------]
//[ Private virtual SceneNode functions                   ]
//[-------------------------------------------------------]
void SNText3D::UpdateAABoundingBox()
{
	// Get the font
	Renderer &cRenderer = GetSceneContext()->GetRendererContext().GetRenderer();
	const PLRenderer::Font *pDefaultFont = cRenderer.GetFontManager().GetDefaultFontTexture();
	const String			sFont        = (GetFont().GetLength() || !pDefaultFont) ? GetFont() : pDefaultFont->GetFilename();
	const uint32			nFontSize    = (GetFontSize() || !pDefaultFont) ? GetFontSize() : pDefaultFont->GetSize();
		  PLRenderer::Font *pFont        = reinterpret_cast<PLRenderer::Font*>(cRenderer.GetFontManager().GetFontTexture(sFont, nFontSize));
	if (pFont) {
		// Get the width and height of the text
		// -> The 3D placed text should have a height of 1 to be independent of the actual 'font size'
		const float fWidth = pFont->GetTextWidth(GetText())/pFont->GetHeight();

		// Centered?
		if (GetFlags () & NoCenter) {
			SetAABoundingBox(AABoundingBox(-0.25f,         -0.25f, -0.1f,
										   fWidth + 0.25f,  0.75f,  0.1f));
		} else {
			SetAABoundingBox(AABoundingBox(-fWidth*0.5f - 0.25f, -0.25f, -0.1f,
											fWidth*0.5f + 0.25f,  0.75f,  0.1f));
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
