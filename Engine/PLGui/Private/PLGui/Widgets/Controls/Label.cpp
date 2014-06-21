/*********************************************************\
 *  File: Label.cpp                                      *
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
#include "PLGui/Gui/Resources/Graphics.h"
#include "PLGui/Gui/Gui.h"
#include "PLGui/Themes/Theme.h"
#include "PLGui/Widgets/Controls/Label.h"


//[-------------------------------------------------------]
//[ Public definitions                                    ]
//[-------------------------------------------------------]
static const int GAP = 8;	// Gap between two words


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLGui {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(Label)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Label::Label(Widget *pParent) : Widget(pParent),
	Color(this),
	Align(this),
	VAlign(this),
	Wrap(this),
	Style(this),
	Text(this),
	m_cColor(Color4::Black),
	m_nAlign(AlignLeft),
	m_nVAlign(AlignMiddle),
	m_nWrap(TextWrap),
	m_nStyle(0),
	m_cFont(*GetGui())
{
	// Get default font
	m_cFont = GetGui()->GetTheme()->GetDefaultFont();

	// Don't accept keyboard focus
	SetFocusStyle(NoFocus);
}

/**
*  @brief
*    Destructor
*/
Label::~Label()
{
}

/**
*  @brief
*    Get font
*/
const Font &Label::GetFont() const
{
	// Return font
	return m_cFont;
}

/**
*  @brief
*    Set font
*/
void Label::SetFont(const Font &cFont)
{
	// Set font
	m_cFont = cFont;

	// Update widget
	UpdateContent();

	// Redraw
	Redraw();
}

/**
*  @brief
*    Get color
*/
Color4 Label::GetColor() const
{
	// Return color
	return m_cColor;
}

/**
*  @brief
*    Set color
*/
void Label::SetColor(const Color4 &cColor)
{
	// Set color
	m_cColor = cColor;

	// Update widget
	UpdateContent();

	// Redraw
	Redraw();
}

/**
*  @brief
*    Get horizontal alignment
*/
EAlign Label::GetAlign() const
{
	// Return alignment
	return m_nAlign;
}

/**
*  @brief
*    Set horizontal alignment
*/
void Label::SetAlign(EAlign nAlign)
{
	// Set alignment
	m_nAlign = nAlign;

	// Update widget
	UpdateContent();

	// Redraw
	Redraw();
}

/**
*  @brief
*    Get vertical alignment
*/
EVAlign Label::GetVAlign() const
{
	// Return alignment
	return m_nVAlign;
}

/**
*  @brief
*    Set vertical alignment
*/
void Label::SetVAlign(EVAlign nAlign)
{
	// Set alignment
	m_nVAlign = nAlign;

	// Update widget
	UpdateContent();

	// Redraw
	Redraw();
}

/**
*  @brief
*    Get text wrapping
*/
ETextWrap Label::GetWrap() const
{
	// Return wrapping
	return m_nWrap;
}

/**
*  @brief
*    Set text wrapping
*/
void Label::SetWrap(ETextWrap nWrap)
{
	// Set wrapping
	m_nWrap = nWrap;

	// Update widget
	UpdateContent();

	// Redraw
	Redraw();
}

/**
*  @brief
*    Get text style
*/
uint32 Label::GetStyle() const
{
	// Return style
	return m_nStyle;
}

/**
*  @brief
*    Set text style
*/
void Label::SetStyle(uint32 nStyle)
{
	// Set style
	m_nStyle = nStyle;

	// Update widget
	UpdateContent();

	// Redraw
	Redraw();
}

/**
*  @brief
*    Get text
*/
String Label::GetText() const
{
	// Return text
	return m_sText;
}

/**
*  @brief
*    Set text
*/
void Label::SetText(const String &sText)
{
	// Set text
	m_sText = sText;

	// Update widget
	UpdateContent();

	// Redraw
	Redraw();
}


//[-------------------------------------------------------]
//[ Protected virtual WidgetFunctions functions           ]
//[-------------------------------------------------------]
PLMath::Vector2i Label::OnPreferredSize(const Vector2i &vRefSize) const
{
	// Call base function
	Widget::OnPreferredSize(vRefSize);

	// Try to calculate the size in Y direction. If X is already known, we can use the actual line width
	// to calculate the height, otherwise we use a virtually endless line and therefore calculate only
	// hard-coded line feeds.

	// Create an offscreen graphics object
	Graphics cGraphics(*GetGui());

	// Get font
	Font cFont = GetFont();

	// Get underline mode
	int nUnderline = 0;
	if (GetStyle() & UnderlineText) {
		nUnderline = 2;
	}

	// First of all: Is there any text?
	String sText = GetText();
	if (sText.GetLength()) {
		// Get text width and height
		uint32 nTextWidth  = cGraphics.GetTextWidth (cFont, sText);
		uint32 nTextHeight = cGraphics.GetTextHeight(cFont, sText);

		// Get line with
		int nLineWidth = vRefSize.x;

		// Check if text has to be wrapped
		bool bHasNewLines = (sText.IndexOf('\r') > -1) || (sText.IndexOf('\n') > -1);
		if (GetWrap() == NoTextWrap || (nLineWidth > -1 && static_cast<int>(nTextWidth) < nLineWidth && !bHasNewLines) ) {
			// No wrapping
			Vector2i vPreferredSize;
			vPreferredSize.x = (vRefSize.x > -1 ? vRefSize.x : nTextWidth);
			vPreferredSize.y = nTextHeight + nUnderline;
			return vPreferredSize;
		} else {
			// With text wrapping
			int nGap = GAP;		// Gap between two words

			// Get words and mark newlines
			RegEx cRegEx("([\\r\\n])|([\\s]*([\\S]+))");
			Array<String> lstWords;
			uint32 nParsePos = 0;
			uint32 nLine = 0;
			uint32 nNumLines = 0;
			while (cRegEx.Match(sText, nParsePos)) {
				// Get next word
				nParsePos	 = cRegEx.GetPosition();
				String sWord = cRegEx.GetResult(0);
				if (sWord.GetLength() > 0)	sWord = '\n';
				else						sWord = cRegEx.GetResult(2);
				int nWidth	 = cGraphics.GetTextWidth(cFont, sWord);

				// Add word to line
				if ((sWord != '\n') && (static_cast<int>(nLine+nWidth) <= nLineWidth || nLineWidth <= -1 || nLine == 0)) {
					// Add word
					lstWords.Add(sWord);
					nLine += nWidth + nGap;
				} else {
					// Add line wrap
					lstWords.Add('\n');
					nLine = 0;
					nNumLines++;

					// Add first word of next line
					if (sWord != '\n') {
						lstWords.Add(sWord);
						nLine = nWidth + nGap;
					}
				}
			}

			// Add '\n' after last line
			String sLastWord = lstWords.Get(lstWords.GetNumOfElements()-1);
			if (sLastWord.GetLength() && sLastWord != '\n') {
				lstWords.Add('\n');
				nNumLines++;
			}

			// Set start position (Y)
			Vector2i vPos(0, 0);

			// Calculate lines
			int nMaxLineWidth = -1;
			uint32 nFirst = 0;
			while (nFirst < lstWords.GetNumOfElements()) {
				// Get end of line ('\n')
				uint32 nLast;
				for (nLast=nFirst; nLast<lstWords.GetNumOfElements(); nLast++) {
					if (lstWords[nLast] == '\n') break;
				}

				// Get number of words in this line
				uint32 nWords = nLast - nFirst;
				if (nWords > 0) {
					// Get width of line
					uint32 nWidth = 0;
					for (uint32 i=nFirst; i<nLast; i++) {
						if (nWidth > 0 && GetWrap() != TextWrapBlock) nWidth += nGap;
						nWidth += cGraphics.GetTextWidth(cFont, lstWords[i]);
					}

					// Set start position (X)
					vPos.x = 0;
					if (GetWrap() == TextWrapBlock) {
						// Calculate size of gaps between words for block-text
						if (nLineWidth > -1) nGap = (nLineWidth - nWidth) / (nWords>1 ? nWords-1 : 1);
						else				 nGap = GAP;
						if (nWords > 1) nWidth += (nWords-1) * nGap;
					} else {
						// No block text, use static gaps and align text by alignment options
						nGap = GAP;
						if (nLineWidth > -1) {
								 if (GetAlign() == AlignCenter)	vPos.x = nLineWidth/2 - nWidth/2;	// Center
							else if (GetAlign() == AlignRight)	vPos.x = nLineWidth   - nWidth;		// Right
						}
					}
					Vector2i vStartPos = vPos;

					// Calculate line
					for (uint32 i=nFirst; i<nLast; i++) {
						// Get word
						String sWord = lstWords[i];

						// Next word
						vPos.x += cGraphics.GetTextWidth(cFont, sWord) + nGap;
					}

					// Save width of widest line
					if (static_cast<int>(nWidth) > nMaxLineWidth)
						nMaxLineWidth = nWidth;
				}

				// Next line
				nFirst = nLast + 1;
				vPos.y += nTextHeight;
			}

			// Return vertical size
			Vector2i vPreferredSize;
			vPreferredSize.x = (vRefSize.x > -1 ? vRefSize.x : nMaxLineWidth);
			vPreferredSize.y = vPos.y + nUnderline;
			return vPreferredSize;
		}
	}

	// Use something like a standard text height
	uint32 nTextHeight = cGraphics.GetTextHeight(cFont, "Text");
	return Vector2i(-1, nTextHeight + nUnderline);
}

void Label::OnDraw(Graphics &cGraphics)
{
	// Call base implementation
	Widget::OnDraw(cGraphics);

	// First of all: Is there any text?
	String sText = GetText();
	if (sText.GetLength()) {
		// Get font
		Font cFont = GetFont();

		// Get text color
		Color4 cColor = IsEnabled() ? GetColor() : Color4::Gray;

		// Get text width and height
		uint32 nTextWidth  = cGraphics.GetTextWidth (cFont, sText);
		uint32 nTextHeight = cGraphics.GetTextHeight(cFont, sText);

		// Get line with
		int nLineWidth = GetSize().x;

		// Check if text has to be wrapped
		bool bHasNewLines = (sText.IndexOf('\r') > -1) || (sText.IndexOf('\n') > -1);
		if (GetWrap() == NoTextWrap || (static_cast<int>(nTextWidth) < nLineWidth && !bHasNewLines) ) {
			// No wrapping

			// Align text (horizontally)
			uint32 nX = 0;
				 if (GetAlign() == AlignCenter)	nX = GetSize().x/2 - nTextWidth/2;	// Center
			else if (GetAlign() == AlignRight)	nX = GetSize().x   - nTextWidth;	// Right

			// Align text (vertically)
			uint32 nY = 0;
				 if (GetVAlign() == AlignBottom) nY = GetSize().y   - nTextHeight;		// Bottom
			else if (GetVAlign() == AlignMiddle) nY = GetSize().y/2 - nTextHeight/2;	// Middle

			// Draw text
			cGraphics.DrawText(cFont, cColor, Color4::Transparent, Vector2i(nX, nY), sText);

			// Draw underline
			if (GetStyle() & UnderlineText)
				cGraphics.DrawLine(cColor, Vector2i(nX, nY + nTextHeight), Vector2i(nX + nTextWidth, nY + nTextHeight));

			// Draw cross-out
			if (GetStyle() & CrossoutText)
				cGraphics.DrawLine(cColor, Vector2i(nX, nY + nTextHeight/2), Vector2i(nX + nTextWidth, nY + nTextHeight/2));
		} else {
			// Draw with text wrapping
			int nGap = GAP;		// Gap between two words

			// Get words and mark newlines
			RegEx cRegEx("([\\r\\n])|([\\s]*([\\S]+))");
			Array<String> lstWords;
			uint32 nParsePos = 0;
			uint32 nLine = 0;
			uint32 nNumLines = 0;
			while (cRegEx.Match(sText, nParsePos)) {
				// Get next word
				nParsePos	 = cRegEx.GetPosition();
				String sWord = cRegEx.GetResult(0);
				if (sWord.GetLength() > 0)	sWord = '\n';
				else						sWord = cRegEx.GetResult(2);
				int nWidth	 = cGraphics.GetTextWidth(cFont, sWord);

				// Add word to line
				if ((sWord != '\n') && (static_cast<int>(nLine+nWidth) <= nLineWidth || nLine == 0)) {
					// Add word
					lstWords.Add(sWord);
					nLine += nWidth + nGap;
				} else {
					// Add line wrap
					lstWords.Add('\n');
					nLine = 0;
					nNumLines++;

					// Add first word of next line
					if (sWord != '\n') {
						lstWords.Add(sWord);
						nLine = nWidth + nGap;
					}
				}
			}

			// Add '\n' after last line
			String sLastWord = lstWords.Get(lstWords.GetNumOfElements()-1);
			if (sLastWord.GetLength() && sLastWord != '\n') {
				lstWords.Add('\n');
				nNumLines++;
			}

			// Set start position (Y)
			Vector2i vPos(0, 0);
				 if (GetVAlign() == AlignBottom) vPos.y = GetSize().y   - (nTextHeight*nNumLines);		// Bottom
			else if (GetVAlign() == AlignMiddle) vPos.y = GetSize().y/2 - (nTextHeight*nNumLines)/2;	// Middle

			// Print lines
			uint32 nFirst = 0;
			while (nFirst < lstWords.GetNumOfElements()) {
				// Get end of line ('\n')
				uint32 nLast;
				for (nLast=nFirst; nLast<lstWords.GetNumOfElements(); nLast++) {
					if (lstWords[nLast] == '\n') break;
				}

				// Get number of words in this line
				uint32 nWords = nLast - nFirst;
				if (nWords > 0) {
					// Get width of line
					uint32 nWidth = 0;
					for (uint32 i=nFirst; i<nLast; i++) {
						if (nWidth > 0 && GetWrap() != TextWrapBlock) nWidth += nGap;
						nWidth += cGraphics.GetTextWidth(cFont, lstWords[i]);
					}

					// Set start position (X)
					vPos.x = 0;
					if (GetWrap() == TextWrapBlock) {
						// Calculate size of gaps between words for block-text
						nGap = (GetSize().x - nWidth) / (nWords>1 ? nWords-1 : 1);
						if (nWords > 1) nWidth += (nWords-1) * nGap;
					} else {
						// No block text, use static gaps and align text by alignment options
						nGap = GAP;
							 if (GetAlign() == AlignCenter)	vPos.x = nLineWidth/2 - nWidth/2;	// Center
						else if (GetAlign() == AlignRight)	vPos.x = nLineWidth   - nWidth;		// Right
					}
					Vector2i vStartPos = vPos;

					// Print line
					for (uint32 i=nFirst; i<nLast; i++) {
						// Get word
						String sWord = lstWords[i];

						// Draw word
						cGraphics.DrawText(cFont, cColor, Color4::Transparent, vPos, sWord);

						// Next word
						vPos.x += cGraphics.GetTextWidth(cFont, sWord) + nGap;
					}

					// Draw underline
					if (GetStyle() & UnderlineText)
						cGraphics.DrawLine(cColor, Vector2i(vStartPos.x, vStartPos.y+nTextHeight), Vector2i(vStartPos.x+nWidth, vStartPos.y+nTextHeight));

					// Draw cross-out
					if (GetStyle() & CrossoutText)
						cGraphics.DrawLine(cColor, Vector2i(vStartPos.x, vStartPos.y+nTextHeight/2), Vector2i(vStartPos.x+nWidth, vStartPos.y+nTextHeight/2));
				}

				// Next line
				nFirst = nLast + 1;
				vPos.y += nTextHeight;
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
