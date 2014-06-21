/*********************************************************\
 *  File: FontLinux.cpp                                  *
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
#include "PLGui/Gui/Resources/Font.h"
#include "PLGui/Backend/Linux/GuiLinux.h"
#include "PLGui/Backend/Linux/FontLinux.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLGui {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FontLinux::FontLinux(Font &cFont) : FontImpl(cFont),
	m_pDisplay(static_cast<GuiLinux*>(cFont.GetGui()->GetImpl())->GetDisplay()),
	m_pXFont(nullptr),
	m_nHeight(0)
{
}

/**
*  @brief
*    Destructor
*/
FontLinux::~FontLinux()
{
	// Release font
	if (m_pXFont) XFreeFont(m_pDisplay, m_pXFont);
}

/**
*  @brief
*    Get X font
*/
XFontStruct *FontLinux::GetXFont() const
{
	// Return font
	return m_pXFont;
}

/**
*  @brief
*    Get font height
*/
uint32 FontLinux::GetHeight() const
{
	// Return font height
	return m_nHeight;
}


//[-------------------------------------------------------]
//[ Public virtual FontImpl functions                     ]
//[-------------------------------------------------------]
bool FontLinux::LoadFont(const String &sFamily, uint32 nHeight, uint32 nWeight, EFontStyle nStyle)
{
	// Release font
	if (m_pXFont) {
		XFreeFont(m_pDisplay, m_pXFont);
		m_pXFont = nullptr;
	}

	// Load font
	if (sFamily.GetLength()) {
		// Font options
		String sFoundry		= '*';
		String sWeight		= (nWeight >= WeightBold ? "bold" : "medium");
		String sSlant		= (nStyle == StyleItalics ? 'i' : 'r');
		String sWidth		= "normal";
		String sStyle		= '*';
		String sPixelSize	= String() + nHeight;
		String sPointSize	= '*';

		// Compose font name
		String sFontName =	'-' + sFoundry + 
							'-' + sFamily +
							'-' + sWeight +
							'-' + sSlant +
							'-' + sWidth +
							'-' + sStyle +
							'-' + sPixelSize +
							'-' + sPointSize +
							"-*-*-*-*-*-*";

		// Load font
		m_pXFont = XLoadQueryFont(m_pDisplay, sFontName.GetASCII());

		// [DEBUG] Error
		if (!m_pXFont) {
//			printf("Could not load font '%s'\n", sFontName.GetASCII());
		}
	}

	// Check returned font
	bool bSuccess = true;
	if (!m_pXFont) {

		// Load default font
		m_pXFont = XLoadQueryFont(m_pDisplay, "fixed");
		bSuccess = false;
	}

	// Save height
//	m_nHeight = m_pXFont->ascent + m_pXFont->descent - 1;
	m_nHeight = m_pXFont->ascent - 1;

	// Done
	return bSuccess;
}

bool FontLinux::LoadFont(const String &sFilename, uint32 nHeight)
{
	// [TODO]
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
