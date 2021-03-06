/*********************************************************\
 *  File: GuiLabel.cpp                                   *
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
#include "GuiLabel.h"


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
pl_class_metadata(GuiLabel, "", GuiBase, "Ingame Gui - Label (text item)")
pl_class_metadata_end(GuiLabel)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
GuiLabel::GuiLabel(Widget *pParent) : GuiBase(pParent),
	m_pFont(Application66::GuiFont),
	m_cColor(Color4::White),
	m_nAlign(AlignCenter)
{
	// Set transparent background
	SetBackgroundColor(Color4::Transparent);
}

/**
*  @brief
*    Get text
*/
String GuiLabel::GetText() const
{
	// Return text
	return m_sText;
}

/**
*  @brief
*    Set text
*/
void GuiLabel::SetText(const String &sText)
{
	// Set text
	m_sText = sText;
}

/**
*  @brief
*    Get font
*/
Font *GuiLabel::GetFont() const
{
	// Return font
	return m_pFont;
}

/**
*  @brief
*    Set font
*/
void GuiLabel::SetFont(Font *pFont)
{
	// Set font
	m_pFont = pFont;
}

/**
*  @brief
*    Get text color
*/
Color4 GuiLabel::GetColor() const
{
	// Return color
	return m_cColor;
}

/**
*  @brief
*    Set text color
*/
void GuiLabel::SetColor(const Color4 &cColor)
{
	// Set color
	m_cColor = cColor;
}

/**
*  @brief
*    Get text alignment
*/
GuiLabel::ETextAlign GuiLabel::GetAlign() const
{
	// Return alignment
	return m_nAlign;
}

/**
*  @brief
*    Set text alignment
*/
void GuiLabel::SetAlign(ETextAlign nAlign)
{
	// Set alignment
	m_nAlign = nAlign;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
GuiLabel::~GuiLabel()
{
}


//[-------------------------------------------------------]
//[ Public virtual PLGui::Widget functions                ]
//[-------------------------------------------------------]
void GuiLabel::OnDraw(Graphics &cGraphics)
{
	// Is the font valid?
	if (m_pFont) {
		// Get text alignment
		if (m_nAlign == AlignLeft) {
			// Left aligned
			cGraphics.DrawText(*m_pFont, m_cColor, Color4::Transparent, Vector2i::Zero, m_sText);
		} else if (m_nAlign == AlignRight) {
			// Right aligned
			uint32 nWidth = cGraphics.GetTextWidth(*m_pFont, m_sText);
			cGraphics.DrawText(*m_pFont, m_cColor, Color4::Transparent, Vector2i(GetSize().x - nWidth, 0), m_sText);
		} else if (m_nAlign == AlignCenter) {
			// Centered
			uint32 nWidth = cGraphics.GetTextWidth(*m_pFont, m_sText);
			cGraphics.DrawText(*m_pFont, m_cColor, Color4::Transparent, Vector2i((GetSize().x - nWidth) / 2, 0), m_sText);
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
				cGraphics.DrawText(*m_pFont, m_cColor, Color4::Transparent, vPos, sWord);

				// Next word
				vPos.x += nWidth + 8;
			}
		}
	}
}
