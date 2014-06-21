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
#include "PLGui/Gui/Resources/Font.h"
#include "PLGui/Gui/Resources/FontInfo.h"
#include "PLGui/Gui/Gui.h"
#include "PLGui/Backend/FontImpl.h"


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
Font::Font(Gui &cGui) :
	m_pGui(&cGui),
	m_pFontImpl(nullptr),
	m_sFamily(),
	m_nHeight(0),
	m_nWeight(WeightNormal),
	m_nStyle(StyleNormal),
	m_sFilename()
{
	// Load empty font
	Load("");
}

/**
*  @brief
*    Constructor
*/
Font::Font(Gui &cGui, const String &sFamily, uint32 nHeight, EFontWeight nWeight, EFontStyle nStyle) :
	m_pGui(&cGui),
	m_pFontImpl(nullptr),
	m_sFamily(sFamily),
	m_nHeight(nHeight),
	m_nWeight(nWeight),
	m_nStyle(nStyle),
	m_sFilename()
{
	// Load font
	Load(sFamily, nHeight, nWeight, nStyle);
}

/**
*  @brief
*    Constructor
*/
Font::Font(Gui &cGui, const FontInfo &cFontInfo) :
	m_pGui(&cGui),
	m_pFontImpl(nullptr),
	m_sFamily(cFontInfo.GetFamily()),
	m_nHeight(cFontInfo.GetHeight()),
	m_nWeight(cFontInfo.GetWeight()),
	m_nStyle(cFontInfo.GetStyle()),
	m_sFilename(cFontInfo.GetFilename())
{
	// Load font
	if (m_sFilename.GetLength() > 0) LoadFromFile(m_sFilename, m_nHeight);
	else							 Load(m_sFamily, m_nHeight, m_nWeight, m_nStyle);
}

/**
*  @brief
*    Copy constructor
*/
Font::Font(const Font &cFont) :
	m_pGui(cFont.m_pGui),
	m_pFontImpl(cFont.m_pFontImpl),
	m_sFamily(cFont.m_sFamily),
	m_nHeight(cFont.m_nHeight),
	m_nWeight(cFont.m_nWeight),
	m_nStyle(cFont.m_nStyle),
	m_sFilename(cFont.m_sFilename)
{
	// Increase reference count for font
	if (m_pFontImpl)
		m_pFontImpl->AddReference();
}

/**
*  @brief
*    Destructor
*/
Font::~Font()
{
	// Release font
	if (m_pFontImpl)
		m_pFontImpl->Release();
}

/**
*  @brief
*    Assignment operator
*/
Font &Font::operator =(const Font &cFont)
{
	// Release font
	if (m_pFontImpl) {
		m_pFontImpl->Release();
		m_pFontImpl = nullptr;
	}

	// Copy font
	m_pFontImpl	= cFont.m_pFontImpl;
	m_sFamily	= cFont.m_sFamily;
	m_nHeight	= cFont.m_nHeight;
	m_nWeight	= cFont.m_nWeight;
	m_nStyle	= cFont.m_nStyle;
	m_sFilename	= cFont.m_sFilename;
	if (m_pFontImpl) {
		m_pFontImpl->AddReference();
	}

	// Return reference to this font
	return *this;
}

/**
*  @brief
*    Get owner GUI
*/
Gui *Font::GetGui() const
{
	// Return GUI
	return m_pGui;
}

/**
*  @brief
*    Get implementation
*/
FontImpl *Font::GetImpl() const
{
	// Return font implementation
	return m_pFontImpl;
}

/**
*  @brief
*    Load font
*/
bool Font::Load(const String &sFamily, uint32 nHeight, EFontWeight nWeight, EFontStyle nStyle)
{
	// Release font
	if (m_pFontImpl) {
		m_pFontImpl->Release();
		m_pFontImpl = nullptr;
	}

	// Save font data
	m_sFilename	= "";
	m_sFamily	= sFamily;
	m_nHeight	= nHeight;
	m_nWeight	= nWeight;
	m_nStyle	= nStyle;

	// Create font
	m_pFontImpl = m_pGui->CreateFontImpl(*this);
	m_pFontImpl->AddReference();

	// Load font
	return m_pFontImpl->LoadFont(sFamily, nHeight, nWeight, nStyle);
}

/**
*  @brief
*    Load font from file
*/
bool Font::LoadFromFile(const String &sFilename, uint32 nHeight)
{
	// Release font
	if (m_pFontImpl) {
		m_pFontImpl->Release();
		m_pFontImpl = nullptr;
	}

	// Save font data
	m_sFilename	= sFilename;
	m_sFamily	= "";
	m_nHeight	= nHeight;
	m_nWeight	= WeightNormal;
	m_nStyle	= StyleNormal;

	// Create font
	m_pFontImpl = m_pGui->CreateFontImpl(*this);
	m_pFontImpl->AddReference();

	// Load font
	return m_pFontImpl->LoadFont(sFilename, nHeight);
}

/**
*  @brief
*    Get font information
*/
FontInfo Font::GetFontInfo() const
{
	// Return font info
	return FontInfo(m_sFilename, m_sFamily, m_nHeight, m_nWeight, m_nStyle);
}

/**
*  @brief
*    Get font file name
*/
String Font::GetFilename() const
{
	// Return filename
	return m_sFilename;
}

/**
*  @brief
*    Get font family
*/
String Font::GetFamily() const
{
	// Return family name
	return m_sFamily;
}

/**
*  @brief
*    Set font family
*/
void Font::SetFamily(const String &sFamily)
{
	// Load changed font
	Load(sFamily, m_nHeight, m_nWeight, m_nStyle);
}

/**
*  @brief
*    Get font height
*/
uint32 Font::GetHeight() const
{
	// Return height
	return m_nHeight;
}

/**
*  @brief
*    Set font height
*/
void Font::SetHeight(uint32 nHeight)
{
	// Load changed font
	Load(m_sFamily, nHeight, m_nWeight, m_nStyle);
}

/**
*  @brief
*    Get font weight
*/
EFontWeight Font::GetWeight() const
{
	// Return weight
	return m_nWeight;
}

/**
*  @brief
*    Set font weight
*/
void Font::SetWeight(EFontWeight nWeight)
{
	// Load changed font
	Load(m_sFamily, m_nHeight, nWeight, m_nStyle);
}

/**
*  @brief
*    Get font style
*/
EFontStyle Font::GetStyle() const
{
	// Return style
	return m_nStyle;
}

/**
*  @brief
*    Set font style
*/
void Font::SetStyle(EFontStyle nStyle)
{
	// Load changed font
	Load(m_sFamily, m_nHeight, m_nWeight, nStyle);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
