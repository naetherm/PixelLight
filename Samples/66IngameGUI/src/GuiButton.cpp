/*********************************************************\
 *  File: GuiButton.cpp                                  *
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
#include <PLCore/String/RegEx.h>
#include <PLMath/Vector2i.h>
#include <PLGui/Gui/Gui.h>
#include <PLGui/Gui/Resources/Graphics.h>
#include <PLGui/Gui/Resources/Font.h>
#include "Application66.h"
#include "GuiButton.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLGui;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(GuiButton, "", GuiBase, "Ingame Gui - Button")
	// Signals
	pl_signal_1_metadata(SignalPressed,	PLCore::uint32,	"Button has been pressed",	"")
pl_class_metadata_end(GuiButton)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
GuiButton::GuiButton(Widget *pParent) : GuiBase(pParent),
	m_pFont(Application66::GuiFont),
	m_cColor(Color4::White),
	m_cMouseOverColor(Color4::White),
	m_nAlign(AlignCenter),
	m_bMouseOver(false),
	m_bMouseDown(false)
{
	// Set transparent background
	SetBackgroundColor(Color4::Transparent);
}

/**
*  @brief
*    Get text
*/
String GuiButton::GetText() const
{
	// Return text
	return m_sText;
}

/**
*  @brief
*    Set text
*/
void GuiButton::SetText(const String &sText)
{
	// Set text
	m_sText = sText;
}

/**
*  @brief
*    Get font
*/
Font *GuiButton::GetFont() const
{
	// Return font
	return m_pFont;
}

/**
*  @brief
*    Set font
*/
void GuiButton::SetFont(Font *pFont)
{
	// Set font
	m_pFont = pFont;
}

/**
*  @brief
*    Get text color
*/
Color4 GuiButton::GetColor() const
{
	// Return color
	return m_cColor;
}

/**
*  @brief
*    Set text color
*/
void GuiButton::SetColor(const Color4 &cColor)
{
	// Set color
	m_cColor = cColor;
}

/**
*  @brief
*    Get mouse over text color
*/
Color4 GuiButton::GetMouseOverColor() const
{
	// Return color
	return m_cMouseOverColor;
}

/**
*  @brief
*    Set mouse over text color
*/
void GuiButton::SetMouseOverColor(const Color4 &cColor)
{
	// Set color
	m_cMouseOverColor = cColor;
}

/**
*  @brief
*    Get text alignment
*/
GuiButton::ETextAlign GuiButton::GetAlign() const
{
	// Return alignment
	return m_nAlign;
}

/**
*  @brief
*    Set text alignment
*/
void GuiButton::SetAlign(ETextAlign nAlign)
{
	// Set alignment
	m_nAlign = nAlign;
}

/**
*  @brief
*    Get background image filename
*/
String GuiButton::GetBackgroundImageFilename() const
{
	return m_sBackgroundImageFilename;
}

/**
*  @brief
*    Set background image filename
*/
void GuiButton::SetBackgroundImageFilename(const String &sFilename)
{
	// Backup the filename
	m_sBackgroundImageFilename = sFilename;

	// Load the new image
	m_cBackground = new PLGui::Image(*GetGui(), sFilename);
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
GuiButton::~GuiButton()
{
}


//[-------------------------------------------------------]
//[ Protected virtual PLGui::Widget functions             ]
//[-------------------------------------------------------]
void GuiButton::OnDraw(Graphics &cGraphics)
{
	// Call base implementation
	Widget::OnDraw(cGraphics);

	// Draw background
	if (m_cBackground.GetPointer())
		cGraphics.DrawImage(*m_cBackground.GetPointer(), Vector2i::Zero, GetSize());

	// Is the font valid?
	if (m_pFont) {
		// Get the text color to use
		const Color4 &cColor = m_bMouseOver ? m_cMouseOverColor : m_cColor;

		// Get text alignment
		if (m_nAlign == AlignLeft) {
			// Left aligned
			uint32 nHeight = cGraphics.GetTextHeight(*m_pFont, m_sText);
			cGraphics.DrawText(*m_pFont, cColor, Color4::Transparent, Vector2i(0, GetSize().y/2 - nHeight/2), m_sText);
		} else if (m_nAlign == AlignRight) {
			// Right aligned
			uint32 nWidth = cGraphics.GetTextWidth(*m_pFont, m_sText);
			uint32 nHeight = cGraphics.GetTextHeight(*m_pFont, m_sText);
			cGraphics.DrawText(*m_pFont, cColor, Color4::Transparent, Vector2i(GetSize().x - nWidth, GetSize().y/2 - nHeight/2), m_sText);
		} else if (m_nAlign == AlignCenter) {
			// Centered
			uint32 nWidth = cGraphics.GetTextWidth(*m_pFont, m_sText);
			uint32 nHeight = cGraphics.GetTextHeight(*m_pFont, m_sText);
			cGraphics.DrawText(*m_pFont, cColor, Color4::Transparent, Vector2i((GetSize().x - nWidth)/2, GetSize().y/2 - nHeight/2), m_sText);
		} else if (m_nAlign == AlignBlock) {
			// Draw block text
			Vector2i vPos (0, 0);
			Vector2i vSize(GetSize());

			// Draw text word for word
			RegEx cRegEx("\\s*([^\\s]+)");
			uint32 nParsePos = 0;
			while (cRegEx.Match(m_sText, nParsePos)) {
				// Get word
				nParsePos = cRegEx.GetPosition();
				String sWord = cRegEx.GetResult(0);

				// Get text width
				int nWidth = cGraphics.GetTextWidth(*m_pFont, sWord);
				if (vPos.x + nWidth >= vSize.x) {
					// Line break
					vPos.x = 0;
					vPos.y = vPos.y + cGraphics.GetTextHeight(*m_pFont, sWord);
				}

				// Draw word
				cGraphics.DrawText(*m_pFont, cColor, Color4::Transparent, vPos, sWord);

				// Next word
				vPos.x += nWidth + 8;
			}
		}
	}
}

void GuiButton::OnMouseMove(const Vector2i &vPos)
{
	// Call base implementation
	Widget::OnMouseMove(vPos);

	// Set MouseOver
	m_bMouseOver = true;
}

void GuiButton::OnMouseEnter()
{
	// Call base implementation
	Widget::OnMouseEnter();

	// Set MouseOver
	m_bMouseOver = true;
}

void GuiButton::OnMouseLeave()
{
	// Call base implementation
	Widget::OnMouseLeave();

	// Reset MouseOver
	m_bMouseOver = false;
}

void GuiButton::OnMouseButtonDown(uint32 nButton, const Vector2i &vPos)
{
	// Call base implementation
	Widget::OnMouseButtonDown(nButton, vPos);

	m_bMouseDown = true;
}

void GuiButton::OnMouseButtonUp(uint32 nButton, const Vector2i &vPos)
{
	// Call base implementation
	Widget::OnMouseButtonUp(nButton, vPos);

	// Button clicked?
	if (m_bMouseOver && nButton == 0 && m_bMouseDown) {
		// Send click-signal
		SignalPressed(GetID());
	}
	m_bMouseDown = false;
}
