/*********************************************************\
 *  File: FontInfo.cpp                                   *
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
#include "PLGui/Gui/Resources/FontInfo.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLGui {


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FontInfo::FontInfo() :
	m_sFilename(),
	m_sFamily(),
	m_nHeight(0),
	m_nWeight(WeightNormal),
	m_nStyle(StyleNormal)
{
}

/**
*  @brief
*    Constructor
*/
FontInfo::FontInfo(const String &sFilename, const String &sFamily, uint32 nHeight, EFontWeight nWeight, EFontStyle nStyle) :
	m_sFilename(sFilename),
	m_sFamily(sFamily),
	m_nHeight(nHeight),
	m_nWeight(nWeight),
	m_nStyle(nStyle)
{
}

/**
*  @brief
*    Copy constructor
*/
FontInfo::FontInfo(const FontInfo &cFontInfo) :
	m_sFilename(cFontInfo.m_sFilename),
	m_sFamily(cFontInfo.m_sFamily),
	m_nHeight(cFontInfo.m_nHeight),
	m_nWeight(cFontInfo.m_nWeight),
	m_nStyle(cFontInfo.m_nStyle)
{
}

/**
*  @brief
*    Destructor
*/
FontInfo::~FontInfo()
{
}

/**
*  @brief
*    Assignment operator
*/
FontInfo &FontInfo::operator =(const FontInfo &cFontInfo)
{
	// Copy font info
	m_sFamily	= cFontInfo.m_sFamily;
	m_nHeight	= cFontInfo.m_nHeight;
	m_nWeight	= cFontInfo.m_nWeight;
	m_nStyle	= cFontInfo.m_nStyle;
	m_sFilename	= cFontInfo.m_sFilename;

	// Return reference to this font
	return *this;
}

/**
*  @brief
*    Comparison operator
*/
bool FontInfo::operator ==(const FontInfo &cFontInfo) const
{
	// Compare fonts
	return (m_sFamily	== cFontInfo.m_sFamily &&
			m_nHeight	== cFontInfo.m_nHeight &&
			m_nWeight	== cFontInfo.m_nWeight &&
			m_nStyle	== cFontInfo.m_nStyle  &&
			m_sFilename	== cFontInfo.m_sFilename );
}

/**
*  @brief
*    Get font file name
*/
String FontInfo::GetFilename() const
{
	// Return filename
	return m_sFilename;
}

/**
*  @brief
*    Get font family
*/
String FontInfo::GetFamily() const
{
	// Return family name
	return m_sFamily;
}

/**
*  @brief
*    Get font height
*/
uint32 FontInfo::GetHeight() const
{
	// Return height
	return m_nHeight;
}

/**
*  @brief
*    Get font weight
*/
EFontWeight FontInfo::GetWeight() const
{
	// Return weight
	return m_nWeight;
}

/**
*  @brief
*    Get font style
*/
EFontStyle FontInfo::GetStyle() const
{
	// Return style
	return m_nStyle;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
