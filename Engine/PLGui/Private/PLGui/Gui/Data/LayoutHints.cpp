/*********************************************************\
 *  File: LayoutHints.cpp                                *
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
#include "PLGui/Gui/Data/LayoutHints.h"


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
LayoutHints::LayoutHints() :
	m_cSizeX	    (SizeHint::Preferred),
	m_cSizeY	    (SizeHint::Preferred),
	m_cMinSizeX	    (SizeHint::Floating),
	m_cMinSizeY	    (SizeHint::Floating),
	m_cMaxSizeX	    (SizeHint::Floating),
	m_cMaxSizeY	    (SizeHint::Floating),
	m_cMarginLeft   (SizeHint::Pixel, 0),
	m_cMarginRight  (SizeHint::Pixel, 0),
	m_cMarginTop    (SizeHint::Pixel, 0),
	m_cMarginBottom (SizeHint::Pixel, 0),
	m_cPaddingLeft  (SizeHint::Pixel, 0),
	m_cPaddingRight (SizeHint::Pixel, 0),
	m_cPaddingTop   (SizeHint::Pixel, 0),
	m_cPaddingBottom(SizeHint::Pixel, 0),
	m_nAlign		(AlignCenter),
	m_nVAlign		(AlignTop),
	m_nSpacing		(10)
{
}

/**
*  @brief
*    Destructor
*/
LayoutHints::~LayoutHints()
{
}

/**
*  @brief
*    Get desired size (X)
*/
const SizeHint &LayoutHints::GetSizeX() const
{
	// Return size
	return m_cSizeX;
}

/**
*  @brief
*    Set desired size (X)
*/
void LayoutHints::SetSizeX(const SizeHint &cSize)
{
	// Set size
	m_cSizeX = cSize;

	// Update layout hints
	Update();
}

/**
*  @brief
*    Get desired size (Y)
*/
const SizeHint &LayoutHints::GetSizeY() const
{
	// Return size
	return m_cSizeY;
}

/**
*  @brief
*    Set desired size (Y)
*/
void LayoutHints::SetSizeY(const SizeHint &cSize)
{
	// Set size
	m_cSizeY = cSize;

	// Update layout hints
	Update();
}

/**
*  @brief
*    Get minimum size (X)
*/
const SizeHint &LayoutHints::GetMinSizeX() const
{
	// Return size
	return m_cMinSizeX;
}

/**
*  @brief
*    Set minimum size (X)
*/
void LayoutHints::SetMinSizeX(const SizeHint &cSize)
{
	// Set size
	m_cMinSizeX = cSize;

	// Update layout hints
	Update();
}

/**
*  @brief
*    Get minimum size (Y)
*/
const SizeHint &LayoutHints::GetMinSizeY() const
{
	// Return size
	return m_cMinSizeY;
}

/**
*  @brief
*    Set minimum size (Y)
*/
void LayoutHints::SetMinSizeY(const SizeHint &cSize)
{
	// Set size
	m_cMinSizeY = cSize;

	// Update layout hints
	Update();
}

/**
*  @brief
*    Get maximum size (X)
*/
const SizeHint &LayoutHints::GetMaxSizeX() const
{
	// Return size
	return m_cMaxSizeX;
}

/**
*  @brief
*    Set maximum size (X)
*/
void LayoutHints::SetMaxSizeX(const SizeHint &cSize)
{
	// Set size
	m_cMaxSizeX = cSize;

	// Update layout hints
	Update();
}

/**
*  @brief
*    Get maximum size (Y)
*/
const SizeHint &LayoutHints::GetMaxSizeY() const
{
	// Return size
	return m_cMaxSizeY;
}

/**
*  @brief
*    Set maximum size (Y)
*/
void LayoutHints::SetMaxSizeY(const SizeHint &cSize)
{
	// Set size
	m_cMaxSizeY = cSize;

	// Update layout hints
	Update();
}

/**
*  @brief
*    Get margin (left)
*/
const SizeHint &LayoutHints::GetMarginLeft() const
{
	// Return margin
	return m_cMarginLeft;
}

/**
*  @brief
*    Set margin (left)
*/
void LayoutHints::SetMarginLeft(const SizeHint &cSize)
{
	// Set margin
	m_cMarginLeft = cSize;

	// Update layout hints
	Update();
}

/**
*  @brief
*    Get margin (right)
*/
const SizeHint &LayoutHints::GetMarginRight() const
{
	// Return margin
	return m_cMarginRight;
}

/**
*  @brief
*    Set margin (right)
*/
void LayoutHints::SetMarginRight(const SizeHint &cSize)
{
	// Set margin
	m_cMarginRight = cSize;

	// Update layout hints
	Update();
}

/**
*  @brief
*    Get margin (top)
*/
const SizeHint &LayoutHints::GetMarginTop() const
{
	// Return margin
	return m_cMarginTop;
}

/**
*  @brief
*    Set margin (top)
*/
void LayoutHints::SetMarginTop(const SizeHint &cSize)
{
	// Set margin
	m_cMarginTop = cSize;

	// Update layout hints
	Update();
}

/**
*  @brief
*    Get margin (bottom)
*/
const SizeHint &LayoutHints::GetMarginBottom() const
{
	// Return margin
	return m_cMarginBottom;
}

/**
*  @brief
*    Set margin (bottom)
*/
void LayoutHints::SetMarginBottom(const SizeHint &cSize)
{
	// Set margin
	m_cMarginBottom = cSize;

	// Update layout hints
	Update();
}

/**
*  @brief
*    Set margin (all)
*/
void LayoutHints::SetMargin(const SizeHint &cSize)
{
	// Set margin
	m_cMarginLeft	= cSize;
	m_cMarginRight	= cSize;
	m_cMarginTop	= cSize;
	m_cMarginBottom = cSize;

	// Update layout hints
	Update();
}

/**
*  @brief
*    Get padding (left)
*/
const SizeHint &LayoutHints::GetPaddingLeft() const
{
	// Return padding
	return m_cPaddingLeft;
}

/**
*  @brief
*    Set padding (left)
*/
void LayoutHints::SetPaddingLeft(const SizeHint &cSize)
{
	// Set padding
	m_cPaddingLeft = cSize;

	// Update layout hints
	Update();
}

/**
*  @brief
*    Get padding (right)
*/
const SizeHint &LayoutHints::GetPaddingRight() const
{
	// Return padding
	return m_cPaddingRight;
}

/**
*  @brief
*    Set padding (right)
*/
void LayoutHints::SetPaddingRight(const SizeHint &cSize)
{
	// Set padding
	m_cPaddingRight = cSize;

	// Update layout hints
	Update();
}

/**
*  @brief
*    Get padding (top)
*/
const SizeHint &LayoutHints::GetPaddingTop() const
{
	// Return padding
	return m_cPaddingTop;
}

/**
*  @brief
*    Set padding (top)
*/
void LayoutHints::SetPaddingTop(const SizeHint &cSize)
{
	// Set padding
	m_cPaddingTop = cSize;

	// Update layout hints
	Update();
}

/**
*  @brief
*    Get padding (bottom)
*/
const SizeHint &LayoutHints::GetPaddingBottom() const
{
	// Return padding
	return m_cPaddingBottom;
}

/**
*  @brief
*    Set padding (bottom)
*/
void LayoutHints::SetPaddingBottom(const SizeHint &cSize)
{
	// Set padding
	m_cPaddingBottom = cSize;

	// Update layout hints
	Update();
}

/**
*  @brief
*    Set padding (all)
*/
void LayoutHints::SetPadding(const SizeHint &cSize)
{
	// Set padding
	m_cPaddingLeft	 = cSize;
	m_cPaddingRight	 = cSize;
	m_cPaddingTop	 = cSize;
	m_cPaddingBottom = cSize;

	// Update layout hints
	Update();
}

/**
*  @brief
*    Get alignment of child elements (horizontal)
*/
EAlign LayoutHints::GetAlign() const
{
	// Return alignment
	return m_nAlign;
}

/**
*  @brief
*    Set alignment of child elements (horizontal)
*/
void LayoutHints::SetAlign(EAlign nAlign)
{
	// Set alignment
	m_nAlign = nAlign;

	// Update layout hints
	Update();
}

/**
*  @brief
*    Get alignment of child elements (vertical)
*/
EVAlign LayoutHints::GetVAlign() const
{
	// Return alignment
	return m_nVAlign;
}

/**
*  @brief
*    Set alignment of child elements (vertical)
*/
void LayoutHints::SetVAlign(EVAlign nAlign)
{
	// Set alignment
	m_nVAlign = nAlign;

	// Update layout hints
	Update();
}

/**
*  @brief
*    Get spacing between child elements
*/
uint32 LayoutHints::GetSpacing() const
{
	// Return spacing
	return m_nSpacing;
}

/**
*  @brief
*    Set spacing between child elements
*/
void LayoutHints::SetSpacing(uint32 nSpacing)
{
	// Set spacing
	m_nSpacing = nSpacing;

	// Update layout hints
	Update();
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Update layout hints
*/
void LayoutHints::Update()
{
	// Call event
	EventUpdate();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
