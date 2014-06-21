/*********************************************************\
 *  File: FontWindows.cpp                                *
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
#include "PLGui/Backend/+Windows/FontWindows.h"


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
FontWindows::FontWindows(Font &cFont) : FontImpl(cFont),
	m_hFont(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
FontWindows::~FontWindows()
{
	// Release font
	if (m_hFont) DeleteObject(m_hFont);
}

/**
*  @brief
*    Returns the font handle
*/
HFONT FontWindows::GetHandle() const
{
	// Return font handle
	return m_hFont;
}


//[-------------------------------------------------------]
//[ Public virtual FontImpl functions                     ]
//[-------------------------------------------------------]
bool FontWindows::LoadFont(const String &sFamily, uint32 nHeight, uint32 nWeight, EFontStyle nStyle)
{
	// Release old font
	if (m_hFont) DeleteObject(m_hFont);

	// Scale weight
	int nW = nWeight * 10;
	if (nW < 0)		nW = 0;
	if (nW > 1000)	nW = 1000;

	// Create font
	if (sFamily.GetFormat() == String::ASCII) {
		m_hFont = CreateFontA(
			nHeight > 0 ? nHeight : 12,	// nHeight
			0,							// nWidth
			0,							// nEscapement
			0,							// nOrientation
			nW,							// nWeight
			(nStyle == StyleItalics),	// bItalic
			false,						// bUnderline
			0,							// cStrikeOut
			ANSI_CHARSET,				// nCharSet
			OUT_DEFAULT_PRECIS,			// nOutPrecision
			CLIP_DEFAULT_PRECIS,		// nClipPrecision
			DEFAULT_QUALITY,			// nQuality
			DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
			sFamily.GetASCII()			// lpszFacename
		);
	} else {
		m_hFont = CreateFontW(
			nHeight > 0 ? nHeight : 12,	// nHeight
			0,							// nWidth
			0,							// nEscapement
			0,							// nOrientation
			nW,							// nWeight
			(nStyle == StyleItalics),	// bItalic
			false,						// bUnderline
			0,							// cStrikeOut
			ANSI_CHARSET,				// nCharSet
			OUT_DEFAULT_PRECIS,			// nOutPrecision
			CLIP_DEFAULT_PRECIS,		// nClipPrecision
			DEFAULT_QUALITY,			// nQuality
			DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
			sFamily.GetUnicode()		// lpszFacename
		);
	}

	// Return if a font has been successfully created
	return (m_hFont != nullptr);
}

bool FontWindows::LoadFont(const String &sFilename, uint32 nHeight)
{
	// Release old font
	if (m_hFont) DeleteObject(m_hFont);

	// [TODO] Implement
	m_hFont = nullptr;

	// Return if a font has been successfully created
	return (m_hFont != nullptr);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
