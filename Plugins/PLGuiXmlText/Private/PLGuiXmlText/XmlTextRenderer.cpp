/*********************************************************\
 *  File: XmlTextRenderer.cpp                            *
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
#include <PLGui/Gui/Gui.h>
#include <PLGui/Gui/Resources/Graphics.h>
#include <PLGui/Gui/Resources/Font.h>
#include <PLGui/Gui/Resources/Image.h>
#include "PLGuiXmlText/XmlTextRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLGui;
namespace PLGuiXmlText {


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
static const float LINE_HEIGHT_FACTOR = 1.1f;


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ General                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
XmlTextRenderer::XmlTextRenderer(Gui &cGui) :
	m_pGui(&cGui),
	m_pGraphics(nullptr),
	m_nIndent(0),
	m_pFont(nullptr),
	m_nHAlign(HAlignLeft),
	m_nVAlign(VAlignBottom),
	m_nLineWidth(0),
	m_nLineWidthRaw(0),
	m_nLineHeight(0)
{
}

/**
*  @brief
*    Destructor
*/
XmlTextRenderer::~XmlTextRenderer()
{
	// Destroy the font
	if (m_pFont)
		delete m_pFont;
}

/**
*  @brief
*    Returns the GUI to which the XmlTextRenderer element belongs
*/
Gui &XmlTextRenderer::GetGui() const
{
	return *m_pGui;
}

/**
*  @brief
*    Start rendering
*/
void XmlTextRenderer::Init(Graphics &cGraphics, const Vector2i &vSize)
{
	// Save pointer to graphics object
	m_pGraphics = &cGraphics;

	// Destroy the previous font
	if (m_pFont) {
		delete m_pFont;
		m_pFont = nullptr;
	}

	// Initialize renderer state
	m_vSectionPos	= Vector2i::Zero;
	m_vSectionSize	= vSize;
	m_nIndent		= 0;
	m_pFont			= new Font(*m_pGui);
	m_pFont->LoadFromFile("Data/Fonts/LinLibertine_Re-2.7.9.9.otf", 14);
	m_cTextColor	= Color4::Black;
	m_cBgColor		= Color4::Transparent;
	m_nHAlign		= HAlignLeft;
	m_nVAlign		= VAlignBottom;
	m_vPos			= Vector2i::Zero;

	// Initialize current line
	m_lstLine.Clear();
	m_nLineWidth	= 0;
	m_nLineWidthRaw	= 0;
	m_nLineHeight	= 0;
}

/**
*  @brief
*    Finish rendering
*/
void XmlTextRenderer::Finish()
{
	// Reset pointer to graphics object
	m_pGraphics = nullptr;
}

/**
*  @brief
*    Get graphics object
*/
Graphics *XmlTextRenderer::GetGraphics()
{
	// Return pointer to graphics object
	return m_pGraphics;
}


//[-------------------------------------------------------]
//[ Renderer state                                        ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get position of current section
*/
Vector2i XmlTextRenderer::GetSectionPos() const
{
	// Return beginning of section
	return m_vSectionPos;
}

/**
*  @brief
*    Set position of current section
*/
void XmlTextRenderer::SetSectionPos(const Vector2i &vSectionPos)
{
	Flush(false);

	// Set beginning of section
	m_vSectionPos = vSectionPos;

	// Set current position to new section
	m_vPos = vSectionPos;
}

/**
*  @brief
*    Get size of current section
*/
Vector2i XmlTextRenderer::GetSectionSize() const
{
	// Return size of section
	return m_vSectionSize;
}

/**
*  @brief
*    Set size of current section
*/
void XmlTextRenderer::SetSectionSize(const Vector2i &vSectionSize)
{
	// Set size of section
	m_vSectionSize = vSectionSize;
}

/**
*  @brief
*    Get current indentation
*/
int XmlTextRenderer::GetIndent() const
{
	// Return indentation
	return m_nIndent;
}

/**
*  @brief
*    Set current indentation
*/
void XmlTextRenderer::SetIndent(int nIndent)
{
	// Set indentation
	m_nIndent = nIndent;
}

/**
*  @brief
*    Get current font
*/
Font *XmlTextRenderer::GetFont() const
{
	// Return font
	return m_pFont;
}

/**
*  @brief
*    Set current font
*/
void XmlTextRenderer::SetFont(Font *pFont)
{
	// Destroy the previous font
	if (m_pFont) {
		delete m_pFont;
		m_pFont = nullptr;
	}

	// Set font
	m_pFont = pFont;
}

/**
*  @brief
*    Set current font
*/
void XmlTextRenderer::SetFont(const String &sFont, int nFontSize)
{
	// Get font file name and size, use current values if empty
	String sFilename = (sFont.GetLength() || !m_pFont	? sFont		: m_pFont->GetFilename());
	int	   nSize     = (nFontSize > -1    || !m_pFont	? nFontSize : m_pFont->GetHeight());

	// Destroy the previous font
	if (m_pFont) {
		delete m_pFont;
		m_pFont = nullptr;
	}

	// Load font
	m_pFont = new Font(*m_pGui);
	m_pFont->LoadFromFile(sFilename, nSize);
}

/**
*  @brief
*    Get current text color
*/
Color4 XmlTextRenderer::GetTextColor() const
{
	// Return text color
	return m_cTextColor;
}

/**
*  @brief
*    Set current text color
*/
void XmlTextRenderer::SetTextColor(const Color4 &cTextColor)
{
	// Set text color
	m_cTextColor = cTextColor;
}

/**
*  @brief
*    Get current background color
*/
Color4 XmlTextRenderer::GetBgColor() const
{
	// Return background color
	return m_cBgColor;
}

/**
*  @brief
*    Set current background color
*/
void XmlTextRenderer::SetBgColor(const Color4 &cBgColor)
{
	// Set background color
	m_cBgColor = cBgColor;
}

/**
*  @brief
*    Get current horizontal alignment
*/
XmlTextRenderer::HAlign XmlTextRenderer::GetHAlign() const
{
	// Return horizontal alignment
	return m_nHAlign;
}

/**
*  @brief
*    Set current horizontal alignment
*/
void XmlTextRenderer::SetHAlign(XmlTextRenderer::HAlign nHAlign)
{
	// Set horizontal alignment
	m_nHAlign = nHAlign;
}

/**
*  @brief
*    Get current vertical alignment
*/
XmlTextRenderer::VAlign XmlTextRenderer::GetVAlign() const
{
	// Return vertical alignment
	return m_nVAlign;
}

/**
*  @brief
*    Set current vertical alignment
*/
void XmlTextRenderer::SetVAlign(XmlTextRenderer::VAlign nVAlign)
{
	// Set vertical alignment
	m_nVAlign = nVAlign;
}



//[-------------------------------------------------------]
//[ Rendering functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Draw text to screen
*/
void XmlTextRenderer::DrawText(const String &sText)
{
	if (m_pFont) {
		// Add text primitive
		XmlTextPrimitive cPrimitive;
		cPrimitive.m_nType = XmlTextPrimitive::PrimitiveText;
		cPrimitive.m_cTextColor = m_cTextColor;
		cPrimitive.m_cBgColor = m_cBgColor;
		cPrimitive.m_vSize = Vector2i(m_pGraphics->GetTextWidth(*m_pFont, sText), m_pGraphics->GetTextHeight(*m_pFont, sText));
		cPrimitive.m_sText = sText;
		cPrimitive.SetFont(m_pFont);
		AddPrimitive(cPrimitive);
	}
}

/**
*  @brief
*    Draw image to screen
*/
void XmlTextRenderer::DrawImage(const Image *pImage, const Vector2i &vSize)
{
	// Add image primitive if image is valid
	if (pImage) {
		XmlTextPrimitive cPrimitive;
		cPrimitive.m_nType	= XmlTextPrimitive::PrimitiveImage;
		cPrimitive.m_vSize	= (vSize != Vector2i::Zero ? vSize : pImage->GetSize());
		cPrimitive.m_pImage	= pImage;
		AddPrimitive(cPrimitive);
	}
}

/**
*  @brief
*    Draw a bullet at the beginning of the current line
*/
void XmlTextRenderer::DrawBullet()
{
	// Add bullet primitive
	XmlTextPrimitive cPrimitive;
	cPrimitive.m_nType = XmlTextPrimitive::PrimitiveBullet;
	cPrimitive.m_nIndent = m_nIndent;
	AddPrimitive(cPrimitive);
}

/**
*  @brief
*    End current line and insert a line break
*/
void XmlTextRenderer::AddLineBreak(bool bForce, bool bJustify)
{
	// Insert line break if forced or if position is not at the beginning of a line
	if (bForce || m_nLineWidth > 0) {
		// Save line height
		int nLineHeight = (m_nLineHeight > 0 || !m_pFont ? m_nLineHeight : m_pFont->GetHeight());

		// Print current line
		Flush(bJustify);

		// Jump to next line
		m_vPos.x  = m_vSectionPos.x;
		m_vPos.y += static_cast<int>(nLineHeight * LINE_HEIGHT_FACTOR);
	}
}

/**
*  @brief
*    Insert vertical space
*/
void XmlTextRenderer::AddVerticalSpace(int nHeight)
{
	// Add some vertical space here
	m_vPos.y += nHeight;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Add primitive to render list
*/
void XmlTextRenderer::AddPrimitive(const XmlTextPrimitive &cPrimitive)
{
	// Does this fit onto the screen?
	if (m_nLineWidth + cPrimitive.m_vSize.x >= m_vSectionSize.x - m_nIndent) {
		// No, wrap line
		AddLineBreak(false, true);
	}

	// Add primitive
	XmlTextPrimitive cPrim = cPrimitive;
	cPrim.m_nHGap = m_pFont ? static_cast<int>(m_pFont->GetHeight() * 0.3f) : 0;
	m_lstLine.Add(cPrim);

	// Adjust width and height
	m_nLineWidth += cPrim.m_vSize.x;
	if (m_lstLine.GetNumOfElements() > 1) m_nLineWidth += cPrim.m_nHGap;
	m_nLineWidthRaw += cPrim.m_vSize.x;
	if (cPrim.m_vSize.y > m_nLineHeight) m_nLineHeight = cPrim.m_vSize.y;
}

/**
*  @brief
*    Flush primitive list and render current line
*/
void XmlTextRenderer::Flush(bool bJustify)
{
	// If the first element is a bullet, draw it first
	if (m_lstLine.GetNumOfElements() >= 1) {
		if (m_lstLine[0].m_nType == XmlTextPrimitive::PrimitiveBullet) {
			// Draw bullet
			int nSize = 4;
			Vector2i vPos = m_vPos + Vector2i(m_nIndent - nSize - 4, static_cast<int>(m_nLineHeight/2.5f) - nSize/2);
			m_pGraphics->DrawBox(m_cTextColor, vPos, Vector2i(vPos.x + nSize, vPos.y + nSize));
		}
	}

	// Remove bullets
	while (m_lstLine.GetNumOfElements() > 0 && m_lstLine[0].m_nType == XmlTextPrimitive::PrimitiveBullet) {
		m_lstLine.RemoveAtIndex(0);
	}

	// Calculate horizontal gap for justified alignment
	int nCount = (m_lstLine.GetNumOfElements() > 1 ? m_lstLine.GetNumOfElements() - 1 : 1);
	float fRealGap = static_cast<float>(m_vSectionSize.x - m_nIndent - m_nLineWidthRaw) / nCount;
	float fGap = 0.0f;

	// Draw background
	if (!m_cBgColor.IsTransparent()) {
		int nY = m_nLineHeight / 10 + 1;
		m_pGraphics->DrawBox(m_cBgColor, Vector2i(m_vPos.x, m_vPos.y - nY), Vector2i(m_vPos.x + m_vSectionSize.x - 1, m_vPos.y + static_cast<int>(m_nLineHeight * 1.2f) - nY));
	}

	// Calculate start position
	Vector2i vPos = m_vPos;
		 if (m_nHAlign == HAlignLeft)	 vPos.x = m_vSectionPos.x + m_nIndent;
	else if (m_nHAlign == HAlignRight)	 vPos.x = m_vSectionPos.x + m_vSectionSize.x - 1 - m_nLineWidth;
	else if (m_nHAlign == HAlignCenter)	 vPos.x = m_vSectionPos.x + m_nIndent + (m_vSectionSize.x - m_nLineWidth) / 2;
	else if (m_nHAlign == HAlignJustify) vPos.x = m_vSectionPos.x + m_nIndent;

	// Draw
	for (uint32 i=0; i<m_lstLine.GetNumOfElements(); i++) {
		// Get primitive
		XmlTextPrimitive &cPrimitive = m_lstLine[i];

		// Calculate position
		Vector2i vPrimPos = vPos;
			 if (m_nVAlign == VAlignTop)	vPrimPos.y = vPos.y;
		else if (m_nVAlign == VAlignMiddle)	vPrimPos.y = vPos.y + (m_nLineHeight - cPrimitive.m_vSize.y) / 2;
		else if (m_nVAlign == VAlignBottom)	vPrimPos.y = vPos.y + (m_nLineHeight - cPrimitive.m_vSize.y);

		// Text
		if (cPrimitive.m_nType == XmlTextPrimitive::PrimitiveText) {
			if (cPrimitive.GetFont()) {
				m_pGraphics->DrawText(
					*cPrimitive.GetFont(),
					cPrimitive.m_cTextColor,
					cPrimitive.m_cBgColor,
					vPrimPos,
					cPrimitive.m_sText);
			}
		}

		// Image
		else if (cPrimitive.m_nType == XmlTextPrimitive::PrimitiveImage) {
			if (cPrimitive.m_pImage) {
				m_pGraphics->DrawImage(
					*cPrimitive.m_pImage,
					vPrimPos,
					cPrimitive.m_vSize);
			}
		}

		// Calculate position of next primitive
		int nGap = cPrimitive.m_nHGap;
		if (m_nHAlign == HAlignJustify && bJustify) {
			fGap += fRealGap;
			nGap = static_cast<int>(fGap);
			fGap -= nGap;
		}
		vPos.x += cPrimitive.m_vSize.x + nGap;
	}

	// Clear current line
	m_lstLine.Clear();
	m_nLineWidth = 0;
	m_nLineWidthRaw = 0;
	m_nLineHeight = 0;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGuiXmlText
