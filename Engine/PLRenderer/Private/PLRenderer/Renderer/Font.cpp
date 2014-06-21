/*********************************************************\
 *  File: Font.cpp                                       *
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
#include "PLRenderer/Renderer/FontGlyph.h"
#include "PLRenderer/Renderer/FontManager.h"
#include "PLRenderer/Renderer/Font.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
Font::~Font()
{
	// Destroy the currently active glyphs
	DestroyGlyphs();
}


//[-------------------------------------------------------]
//[ Public virtual Font functions                         ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets the font size
*/
bool Font::SetSize(uint32 nSize, uint32 nResolution)
{
	// Destroy the currently active glyphs
	DestroyGlyphs();

	// Done
	return true;
}

/**
*  @brief
*    Returns the width of a text (in font units) drawn with this font
*/
float Font::GetTextWidth(const String &sText)
{
	float fWidth = 0.0f;

	// Are there any glyphs at all?
	if (m_lstGlyphs.GetNumOfElements()) {
		// Get the text to 'draw' - we only support ASCII
		const char *pszText = sText.GetASCII();

		// Iterate through all characters of the text to 'draw'
		for (uint32 i=0; i<sText.GetLength(); i++, pszText++) {
			// Get the character code
			const unsigned char nCharacterCode = static_cast<unsigned char>(*pszText);

			// Get the glyph instance of the character to 'draw'
			FontGlyph *pFontGlyph = m_lstGlyphs[nCharacterCode];
			if (pFontGlyph) {
				// 'Draw' the glyph and let the pen advance to the object space position of the next character
				fWidth += pFontGlyph->GetPenAdvance().x;
			}
		}
	}

	// Return the width
	return fWidth;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Font::Font(FontManager &cFontManager, const String &sFilename) : Resource(cFontManager.GetRenderer(), TypeFont),
	m_pFontManager(&cFontManager),
	m_sFilename(sFilename),
	m_nSize(0),
	m_nResolution(0)
{
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Font::Font(const Font &cSource) : Resource(cSource.GetRenderer(), TypeFont)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
Font &Font::operator =(const Font &cSource)
{
	// No implementation because the copy constructor is never used
	return *this;
}

/**
*  @brief
*    Destroys the currently active glyphs
*/
void Font::DestroyGlyphs()
{
	for (uint32 i=0; i<m_lstGlyphs.GetNumOfElements(); i++)
		delete m_lstGlyphs[i];
	m_lstGlyphs.Clear();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
