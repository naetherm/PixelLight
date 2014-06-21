/*********************************************************\
 *  File: Graphics.cpp                                   *
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
#include "PLGui/Gui/Gui.h"
#include "PLGui/Gui/Resources/Graphics.h"
#include "PLGui/Gui/Data/GraphicsHint.h"
#include "PLGui/Backend/GraphicsImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLGui {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Graphics::Graphics(Gui &cGui) :
	m_pGui(&cGui),
	m_pGraphicsImpl(nullptr),
	m_bManageImpl(true)					// Delete implementation later
{
	// Create implementation
	m_pGraphicsImpl = m_pGui->CreateGraphicsImpl(*this);
}

/**
*  @brief
*    Constructor
*/
Graphics::Graphics(Gui &cGui, GraphicsImpl &cGraphicsImpl) :
	m_pGui(&cGui),
	m_pGraphicsImpl(&cGraphicsImpl),
	m_bManageImpl(false)				// Do not delete implementation in destructor!
{
	// Set owner of implementation
	m_pGraphicsImpl->m_pGraphics = this;
}

/**
*  @brief
*    Destructor
*/
Graphics::~Graphics()
{
	// Destroy hints
	for (uint32 i=0; i<m_lstHints.GetNumOfElements(); i++) {
		delete m_lstHints[i];
	}

	// Destroy implementation object
	if (m_bManageImpl && m_pGraphicsImpl) {
		delete m_pGraphicsImpl;
	}
}

/**
*  @brief
*    Get owner GUI
*/
Gui *Graphics::GetGui() const
{
	// Return GUI
	return m_pGui;
}

/**
*  @brief
*    Get implementation
*/
GraphicsImpl *Graphics::GetImpl() const
{
	// Return Graphics implementation
	return m_pGraphicsImpl;
}

/**
*  @brief
*    Get hints
*/
const List<GraphicsHint*> &Graphics::GetHints()
{
	// Return list of hints
	return m_lstHints;
}

/**
*  @brief
*    Add a hint
*/
void Graphics::AddHint(GraphicsHint *pHint)
{
	// Check if the hint is valid
	if (pHint) {
		// Add hint
		m_lstHints.Add(pHint);
	}
}

/**
*  @brief
*    Draws a line
*/
void Graphics::DrawLine(const Color4 &cColor, const Vector2i &vPos1, const Vector2i &vPos2, uint32 nWidth)
{
	m_pGraphicsImpl->DrawLine(cColor, vPos1, vPos2, nWidth);
}

/**
*  @brief
*    Draws a rectangle
*/
void Graphics::DrawRect(const Color4 &cColor, const Vector2i &vPos1, const Vector2i &vPos2, uint32 nRoundX, uint32 nRoundY, uint32 nWidth)
{
	m_pGraphicsImpl->DrawRect(cColor, vPos1, vPos2, nRoundX, nRoundY, nWidth);
}

/**
*  @brief
*    Draws a filled box
*/
void Graphics::DrawBox(const Color4 &cColor, const Vector2i &vPos1, const Vector2i &vPos2, uint32 nRoundX, uint32 nRoundY)
{
	m_pGraphicsImpl->DrawBox(cColor, vPos1, vPos2, nRoundX, nRoundY);
}

/**
*  @brief
*    Draws a filled box with a color gradient
*/
void Graphics::DrawGradientBox(const Color4 &cColor1, const Color4 &cColor2, float fAngle, const Vector2i &vPos1, const Vector2i &vPos2)
{
	m_pGraphicsImpl->DrawGradientBox(cColor1, cColor2, fAngle, vPos1, vPos2);
}

/**
*  @brief
*    Draws an image
*/
void Graphics::DrawImage(const Image &cImage, const PLMath::Vector2i &vPos, const PLMath::Vector2i &vSize)
{
	m_pGraphicsImpl->DrawImage(cImage, vPos, vSize);
}

/**
*  @brief
*    Draws an tiled image
*/
void Graphics::DrawTiledImage(const Image &cImage, const Vector2i &vPos, const Vector2i &vSize)
{
	// [TODO] Implement me
	m_pGraphicsImpl->DrawImage(cImage, vPos, vSize);
}

/**
*  @brief
*    Draws a text
*/
void Graphics::DrawText(const Font &cFont, const Color4 &cTextColor, const Color4 &cBkColor, const Vector2i &vPos, const String &sText)
{
	m_pGraphicsImpl->DrawText(cFont, cTextColor, cBkColor, vPos, sText);
}

/**
*  @brief
*    Gets the width of a given text in the current font
*/
uint32 Graphics::GetTextWidth(const Font &cFont, const String &sText)
{
	return m_pGraphicsImpl->GetTextWidth(cFont, sText);
}

/**
*  @brief
*    Gets the height of a given text in the current font
*/
uint32 Graphics::GetTextHeight(const Font &cFont, const String &sText)
{
	return m_pGraphicsImpl->GetTextHeight(cFont, sText);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
