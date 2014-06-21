/*********************************************************\
 *  File: GraphicsPL.cpp                                 *
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
#include <PLGui/Gui/Resources/Font.h>
#include <PLGui/Gui/Resources/Image.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Renderer/SamplerStates.h>
#include "PLFrontendPLGui/Compositing/FontPL.h"
#include "PLFrontendPLGui/Compositing/ImagePL.h"
#include "PLFrontendPLGui/Compositing/GraphicsPL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLGui;
using namespace PLRenderer;
namespace PLFrontendPLGui {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
GraphicsPL::GraphicsPL(Renderer &cRenderer, const Vector2i &vAbsPos, const Vector2i &vScreenSize, ETransparency nTransparency, const Color4 &cTransparencyColor) : GraphicsImpl(),
	m_pRenderer(&cRenderer),
	m_vAbsPos(vAbsPos),
	m_vScreenSize(vScreenSize),
	m_nTransparency(nTransparency),
	m_cTransparencyColor(cTransparencyColor)
{
}

/**
*  @brief
*    Constructor
*/
GraphicsPL::GraphicsPL(Graphics &cGraphics, Renderer &cRenderer, const Vector2i &vAbsPos, ETransparency nTransparency, const Color4 &cTransparencyColor) : GraphicsImpl(cGraphics),
	m_pRenderer(&cRenderer),
	m_vAbsPos(vAbsPos),
	m_nTransparency(nTransparency),
	m_cTransparencyColor(cTransparencyColor)
{
}

/**
*  @brief
*    Destructor
*/
GraphicsPL::~GraphicsPL()
{
}


//[-------------------------------------------------------]
//[ Public virtual GraphicsImpl functions                 ]
//[-------------------------------------------------------]
void GraphicsPL::DrawLine(const Color4 &cColor, const Vector2i &vPos1, const Vector2i &vPos2, uint32 nWidth)
{
	// Get final color to use
	Color4 cFinalColor = cColor;
	if (m_nTransparency == AlphaTransparency)
		cFinalColor.a *= m_cTransparencyColor.a;

	// Setup renderer
	m_pRenderer->SetTextureBuffer();
	const bool bBlendEnable = (cFinalColor.a < 1.0f);
	m_pRenderer->SetRenderState(RenderState::BlendEnable, bBlendEnable);
	if (bBlendEnable) {
		m_pRenderer->SetRenderState(RenderState::SrcBlendFunc, BlendFunc::SrcAlpha);
		m_pRenderer->SetRenderState(RenderState::DstBlendFunc, BlendFunc::InvSrcAlpha);
	}

	// Draw line
	m_pRenderer->GetDrawHelpers().DrawLine(cFinalColor, Vector2(static_cast<float>(vPos1.x) + m_vAbsPos.x, static_cast<float>(vPos1.y) + m_vAbsPos.y),
														Vector2(static_cast<float>(vPos2.x) + m_vAbsPos.x, static_cast<float>(vPos2.y) + m_vAbsPos.y), static_cast<float>(nWidth));
}

void GraphicsPL::DrawRect(const Color4 &cColor, const Vector2i &vPos1, const Vector2i &vPos2, uint32 nRoundX, uint32 nRoundY, uint32 nWidth)
{
	// Get final color to use
	Color4 cFinalColor = cColor;
	if (m_nTransparency == AlphaTransparency)
		cFinalColor.a *= m_cTransparencyColor.a;

	// nRoundX and nRoundY are not supported

	// Setup renderer
	m_pRenderer->SetTextureBuffer();
	const bool bBlendEnable = (cFinalColor.a < 1.0f);
	m_pRenderer->SetRenderState(RenderState::BlendEnable, bBlendEnable);
	if (bBlendEnable) {
		m_pRenderer->SetRenderState(RenderState::SrcBlendFunc, BlendFunc::SrcAlpha);
		m_pRenderer->SetRenderState(RenderState::DstBlendFunc, BlendFunc::InvSrcAlpha);
	}

	// Draw line
	m_pRenderer->GetDrawHelpers().DrawQuad(cFinalColor, Vector2(static_cast<float>(vPos1.x) + m_vAbsPos.x, static_cast<float>(vPos1.y) + m_vAbsPos.y),
														Vector2(static_cast<float>(vPos2.x - vPos1.x), static_cast<float>(vPos2.y - vPos1.y)), static_cast<float>(nWidth));
}

void GraphicsPL::DrawBox(const Color4 &cColor, const Vector2i &vPos1, const Vector2i &vPos2, uint32 nRoundX, uint32 nRoundY)
{
	// Get final color to use
	Color4 cFinalColor = cColor;
	if (m_nTransparency == AlphaTransparency)
		cFinalColor.a *= m_cTransparencyColor.a;

	// nRoundX and nRoundY are not supported

	// Setup renderer
	m_pRenderer->SetTextureBuffer();
	const bool bBlendEnable = (cFinalColor.a < 1.0f);
	m_pRenderer->SetRenderState(RenderState::BlendEnable, bBlendEnable);
	if (bBlendEnable) {
		m_pRenderer->SetRenderState(RenderState::SrcBlendFunc, BlendFunc::SrcAlpha);
		m_pRenderer->SetRenderState(RenderState::DstBlendFunc, BlendFunc::InvSrcAlpha);
	}

	// Draw line
	m_pRenderer->GetDrawHelpers().DrawQuad(cFinalColor, Vector2(static_cast<float>(vPos1.x) + m_vAbsPos.x, static_cast<float>(vPos1.y) + m_vAbsPos.y),
														Vector2(static_cast<float>(vPos2.x - vPos1.x), static_cast<float>(vPos2.y - vPos1.y)), 0.0f);
}

void GraphicsPL::DrawGradientBox(const Color4 &cColor1, const Color4 &cColor2, float fAngle, const Vector2i &vPos1, const Vector2i &vPos2)
{
	// [TODO] Implement me
}

void GraphicsPL::DrawImage(const PLGui::Image &cImage, const Vector2i &vPos, const Vector2i &vSize)
{
	// cImage must be an instance of PLFrontendPLGui::ImagePL!
	ImagePL *pImagePL = static_cast<ImagePL*>(cImage.GetImpl());
	if (pImagePL) {
		// Get the texture buffer
		TextureBuffer *pTextureBuffer = pImagePL->GetTextureBuffer();
		if (pTextureBuffer) {
			// Get final color to use
			Color4 cFinalColor = Color4::White;
			if (m_nTransparency == AlphaTransparency)
				cFinalColor.a *= m_cTransparencyColor.a;

			// Setup renderer
			const bool bBlendEnable = ((pTextureBuffer->GetComponentsPerPixel() == 4) || (m_nTransparency == AlphaTransparency));
			m_pRenderer->SetRenderState(RenderState::BlendEnable, bBlendEnable);
			if (bBlendEnable) {
				m_pRenderer->SetRenderState(RenderState::SrcBlendFunc, BlendFunc::SrcAlpha);
				m_pRenderer->SetRenderState(RenderState::DstBlendFunc, BlendFunc::InvSrcAlpha);
			}

			// Draw
			SamplerStates cSamplerStates;
			m_pRenderer->GetDrawHelpers().DrawImage(*pTextureBuffer, cSamplerStates, Vector2(static_cast<float>(vPos.x) + m_vAbsPos.x, static_cast<float>(vPos.y) + m_vAbsPos.y),
													 Vector2(static_cast<float>(vSize.x), static_cast<float>(vSize.y)), cFinalColor);
		}
	}
}

void GraphicsPL::DrawText(const PLGui::Font &cFont, const Color4 &cTextColor, const Color4 &cBkColor, const Vector2i &vPos, const String &sText)
{
	// cFont must be an instance of PLFrontendPLGui::FontPL!
	FontPL *pFontPL = static_cast<FontPL*>(cFont.GetImpl());
	if (pFontPL) {
		// Get the renderer font
		PLRenderer::Font *pRendererFont = pFontPL->GetRendererFont();
		if (pRendererFont) {
			// Draw background?
			if (!cBkColor.IsTransparent()) {
				// Get final background color to use
				Color4 cFinalBkColor = cBkColor;
				if (m_nTransparency == AlphaTransparency)
					cFinalBkColor.a *= m_cTransparencyColor.a;

				// Draw
				const uint32 nTextWidth  = GetTextWidth (cFont, sText);
				const uint32 nTextHeight = GetTextHeight(cFont, sText);
				DrawBox(cFinalBkColor, vPos, Vector2i(vPos.x + nTextWidth, vPos.y + nTextHeight));
			}

			// Get final text color to use
			Color4 cFinalTextColor = cTextColor;
			if (m_nTransparency == AlphaTransparency)
				cFinalTextColor.a *= m_cTransparencyColor.a;

			// Draw text
			m_pRenderer->GetDrawHelpers().DrawText(*pRendererFont, sText, cFinalTextColor, Vector2(static_cast<float>(vPos.x) + m_vAbsPos.x, static_cast<float>(vPos.y) + m_vAbsPos.y));
		}
	}
}

uint32 GraphicsPL::GetTextWidth(const PLGui::Font &cFont, const String &sText)
{
	// cFont must be an instance of PLFrontendPLGui::FontPL!
	FontPL *pFontPL = static_cast<FontPL*>(cFont.GetImpl());
	if (pFontPL) {
		// Get the renderer font
		PLRenderer::Font *pRendererFont = pFontPL->GetRendererFont();
		if (pRendererFont)
			return static_cast<uint32>(m_pRenderer->GetDrawHelpers().GetTextWidth(*pRendererFont, sText)*m_vScreenSize.x);
	}

	// Error!
	return 0;
}

uint32 GraphicsPL::GetTextHeight(const PLGui::Font &cFont, const String &sText)
{
	// cFont must be an instance of PLFrontendPLGui::FontPL!
	FontPL *pFontPL = static_cast<FontPL*>(cFont.GetImpl());
	if (pFontPL) {
		// Get the renderer font
		PLRenderer::Font *pRendererFont = pFontPL->GetRendererFont();
		if (pRendererFont)
			return static_cast<uint32>(m_pRenderer->GetDrawHelpers().GetTextHeight(*pRendererFont)*m_vScreenSize.y);	// sText is not required
	}

	// Error!
	return 0;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendPLGui
