/*********************************************************\
 *  File: LayoutBox.cpp                                  *
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
#include <PLMath/Math.h>
#include "PLGui/Widgets/Widget.h"
#include "PLGui/Layout/LayoutBox.h"


//[-------------------------------------------------------]
//[ Disable warnings                                      ]
//[-------------------------------------------------------]
PL_WARNING_PUSH
PL_WARNING_DISABLE(4355) // "'this' : used in base member initializer list"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLGui {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(LayoutBox)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
EOrientation LayoutBox::GetOrientation() const
{
	// Return orientation
	return m_nOrientation;
}

void LayoutBox::SetOrientation(EOrientation nOrientation)
{
	// Set orientation
	m_nOrientation = nOrientation;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
LayoutBox::LayoutBox(EOrientation nOrientation) : Layout(),
	Orientation(this),
	m_nOrientation(nOrientation),
	m_pSizes(nullptr),
	m_nWidgets(0)
{
}

/**
*  @brief
*    Destructor
*/
LayoutBox::~LayoutBox()
{
	// Destroy data pool
	if (m_pSizes) {
		delete [] m_pSizes;
	}
}


//[-------------------------------------------------------]
//[ Protected virtual Layout functions                    ]
//[-------------------------------------------------------]
void LayoutBox::ApplyLayout()
{
	// Variables
	SizeHint cSizeHint;

	// Create data pool if necessary or if it is too small
	uint32 nWidgets = m_pWidget->GetChildren().GetNumOfElements();
	if (nWidgets > m_nWidgets) {
		// (Re-)Create buffer
		if (m_pSizes) delete [] m_pSizes;
		m_pSizes = new Vector2i[nWidgets*3];
		m_nWidgets = nWidgets;
	}

	// Get container size
	Vector2i vContainerSize = m_pWidget->GetSize();
	int &nContainerSize1 = (m_nOrientation == Horizontal ? vContainerSize.x : vContainerSize.y);
	int &nContainerSize2 = (m_nOrientation == Horizontal ? vContainerSize.y : vContainerSize.x);

	// Apply padding of container element
		// Left padding
	cSizeHint = (m_nOrientation == Horizontal ? m_pWidget->GetLayoutHints().GetPaddingLeft() : m_pWidget->GetLayoutHints().GetPaddingTop());
	int nPaddingLeft = Math::Max(cSizeHint.CalculateSize(nContainerSize1, -1), 0);
	nContainerSize1 -= nPaddingLeft;
		// Right padding
	cSizeHint = (m_nOrientation == Horizontal ? m_pWidget->GetLayoutHints().GetPaddingRight() : m_pWidget->GetLayoutHints().GetPaddingBottom());
	int nPaddingRight = Math::Max(cSizeHint.CalculateSize(nContainerSize1, -1), 0);
	nContainerSize1 -= nPaddingRight;
		// Top padding
	cSizeHint = (m_nOrientation == Horizontal ? m_pWidget->GetLayoutHints().GetPaddingTop() : m_pWidget->GetLayoutHints().GetPaddingLeft());
	int nPaddingTop = Math::Max(cSizeHint.CalculateSize(nContainerSize2, -1), 0);
	nContainerSize2 -= nPaddingTop;
		// Bottom padding
	cSizeHint = (m_nOrientation == Horizontal ? m_pWidget->GetLayoutHints().GetPaddingBottom() : m_pWidget->GetLayoutHints().GetPaddingRight());
	int nPaddingBottom = Math::Max(cSizeHint.CalculateSize(nContainerSize2, -1), 0);
	nContainerSize2 -= nPaddingBottom;

	// Apply spacing of container element
	uint32 nSpacing = m_pWidget->GetLayoutHints().GetSpacing();

	// Get alignment
	EAlign  nAlign  = AlignLeft;
	EVAlign nVAlign = AlignTop;
	if (m_nOrientation == Horizontal) {
		// Get alignment in primary direction
		nAlign  = m_pWidget->GetLayoutHints().GetAlign();

		// Get alignment in secondary direction
		nVAlign = m_pWidget->GetLayoutHints().GetVAlign();
	} else {
		// Get alignment in primary direction
		switch (m_pWidget->GetLayoutHints().GetVAlign()) {
			case AlignTop:		nAlign = AlignLeft;   break;
			case AlignMiddle:	nAlign = AlignCenter; break;
			case AlignBottom:	nAlign = AlignRight;  break;
		}

		// Get alignment in secondary direction
		switch (m_pWidget->GetLayoutHints().GetAlign()) {
			case AlignLeft:		nVAlign = AlignTop;    break;
			case AlignCenter:	nVAlign = AlignMiddle; break;
			case AlignRight:	nVAlign = AlignBottom; break;
		}
	}

	// Assign static sizes and calculate remaining space
	uint32 nSizeUsed = 0;
	uint32 nFloating = 0;
	for (uint32 i=0; i<m_pWidget->GetChildren().GetNumOfElements(); i++) {
		// Get child widget
		Widget *pWidget = m_pWidget->GetChildren().Get(i);

		// Get size references
		int &nMarginBefore1	= (m_nOrientation == Horizontal ? m_pSizes[i*3+0].x : m_pSizes[i*3+0].y);
		int &nSize1			= (m_nOrientation == Horizontal ? m_pSizes[i*3+1].x : m_pSizes[i*3+1].y);
		int &nMarginBehind1	= (m_nOrientation == Horizontal ? m_pSizes[i*3+2].x : m_pSizes[i*3+2].y);
		nMarginBefore1 = nSize1 = nMarginBehind1 = -1;

		// Calculate preferred size, step 1: pass layout size to the function
		Vector2i vRefSize(-1, -1);
		vRefSize.x = (m_nOrientation == Horizontal ? nContainerSize1 : nContainerSize2);
		vRefSize.y = (m_nOrientation == Horizontal ? nContainerSize2 : nContainerSize1);
		pWidget->CalculatePreferredSize(vRefSize);

		// Get preferred size
		Vector2i vPreferredSize = pWidget->GetPreferredSize();
		int &nPreferredSize1 = (m_nOrientation == Horizontal ? vPreferredSize.x : vPreferredSize.y);

		// Get margin (before the element)
		cSizeHint = (m_nOrientation == Horizontal ? pWidget->GetLayoutHints().GetMarginLeft() : pWidget->GetLayoutHints().GetMarginTop());
		nMarginBefore1 = cSizeHint.CalculateSize(nContainerSize1, -1);

		// Apply spacing
		if (i > 0) nMarginBefore1 += nSpacing;

		// Get size of element
		cSizeHint = (m_nOrientation == Horizontal ? pWidget->GetLayoutHints().GetSizeX() : pWidget->GetLayoutHints().GetSizeY());
		nSize1 = cSizeHint.CalculateSize(nContainerSize1, nPreferredSize1);

		// Get margin (after the element)
		cSizeHint = (m_nOrientation == Horizontal ? pWidget->GetLayoutHints().GetMarginRight() : pWidget->GetLayoutHints().GetMarginBottom());
		nMarginBehind1 = cSizeHint.CalculateSize(nContainerSize1, -1);

		// Keep track of how many of the space is used and how many floating elements are there for the remaining space
		if (nMarginBefore1 > -1) nSizeUsed += nMarginBefore1;	else nFloating++;
		if (nSize1		   > -1) nSizeUsed += nSize1;			else nFloating++;
		if (nMarginBehind1 > -1) nSizeUsed += nMarginBehind1;	else nFloating++;
	}

	// Assign sizes of floating elements and apply minimum values
	uint32 nSizeLeft = PLMath::Math::Max(static_cast<int>(nContainerSize1) - static_cast<int>(nSizeUsed), 0);
	nSizeUsed = 0;
	for (uint32 i=0; i<m_pWidget->GetChildren().GetNumOfElements(); i++) {
		// Get child widget
		Widget *pWidget = m_pWidget->GetChildren().Get(i);

		// Get size references
		int &nMarginBefore1	= (m_nOrientation == Horizontal ? m_pSizes[i*3+0].x : m_pSizes[i*3+0].y);
		int &nSize1			= (m_nOrientation == Horizontal ? m_pSizes[i*3+1].x : m_pSizes[i*3+1].y);
		int &nMarginBehind1	= (m_nOrientation == Horizontal ? m_pSizes[i*3+2].x : m_pSizes[i*3+2].y);

		// Get preferred size
		Vector2i vPreferredSize = pWidget->GetPreferredSize();
		int &nPreferredSize1 = (m_nOrientation == Horizontal ? vPreferredSize.x : vPreferredSize.y);

		// Set margin (before the element)
		if (nMarginBefore1 == -1) {
			nMarginBefore1 = (nFloating > 0 ? nSizeLeft / nFloating : 0);
			nSizeLeft -= nMarginBefore1;
			if (nFloating > 0) nFloating--;
		}

		// Set margin (after the element)
		if (nMarginBehind1 == -1) {
			nMarginBehind1 = (nFloating > 0 ? nSizeLeft / nFloating : 0);
			nSizeLeft -= nMarginBehind1;
			if (nFloating > 0) nFloating--;
		}

		// Set element size
		if (nSize1 == -1) {
			nSize1 = (nFloating > 0 ? nSizeLeft / nFloating : 0);
			nSizeLeft -= nSize1;
			if (nFloating > 0) nFloating--;
		}

		// Apply minimum size
		cSizeHint = (m_nOrientation == Horizontal ? pWidget->GetLayoutHints().GetMinSizeX() : pWidget->GetLayoutHints().GetMinSizeY());
		int nMinSize = cSizeHint.CalculateSize(nContainerSize1, nPreferredSize1);
		if (nMinSize > -1 && nSize1 < nMinSize) nSize1 = nMinSize;

		// Apply maximum size: put remaining space into margin
		cSizeHint = (m_nOrientation == Horizontal ? pWidget->GetLayoutHints().GetMaxSizeX() : pWidget->GetLayoutHints().GetMaxSizeY());
		int nMaxSize = cSizeHint.CalculateSize(nContainerSize1, nPreferredSize1);
		if (nMaxSize > -1 && nSize1 > nMaxSize) {
			// Calculate remaining space
			int nRemaining = nSize1 - nMaxSize;

			// Set size to maximum size
			nSize1 = nMaxSize;

			// Give remaining space to margin behind element
			nMarginBehind1 += nRemaining;
		}

		// Calculate totally used space
		nSizeUsed += nMarginBefore1 + nSize1 + nMarginBehind1;
	}

	// Assign sizes in secondary direction
	for (uint32 i=0; i<m_pWidget->GetChildren().GetNumOfElements(); i++) {
		// Get child widget
		Widget *pWidget = m_pWidget->GetChildren().Get(i);

		// Get size references
		int &nMarginBefore2	= (m_nOrientation == Horizontal ? m_pSizes[i*3+0].y : m_pSizes[i*3+0].x);
		int &nSize1			= (m_nOrientation == Horizontal ? m_pSizes[i*3+1].x : m_pSizes[i*3+1].y);
		int &nSize2			= (m_nOrientation == Horizontal ? m_pSizes[i*3+1].y : m_pSizes[i*3+1].x);
		int &nMarginBehind2	= (m_nOrientation == Horizontal ? m_pSizes[i*3+2].y : m_pSizes[i*3+2].x);

		// Calculate preferred size, step 2: pass already assigned space as reference size to the function
		Vector2i vRefSize(-1, -1);
		vRefSize.x = (m_nOrientation == Horizontal ? nSize1 : nContainerSize2 - nMarginBefore2 - nMarginBehind2);
		vRefSize.y = (m_nOrientation == Horizontal ? nContainerSize2 - nMarginBefore2 - nMarginBehind2 : nSize1);
		pWidget->CalculatePreferredSize(vRefSize);

		// Get preferred size
		Vector2i vPreferredSize = pWidget->GetPreferredSize();
		int &nPreferredSize2 = (m_nOrientation == Horizontal ? vPreferredSize.y : vPreferredSize.x);

		// Get margin (before the element)
		cSizeHint = (m_nOrientation == Horizontal ? pWidget->GetLayoutHints().GetMarginTop() : pWidget->GetLayoutHints().GetMarginLeft());
		nMarginBefore2 = cSizeHint.CalculateSize(nContainerSize2, -1);
		if (nMarginBefore2 == -1) nMarginBefore2 = 0;

		// Get margin (after the element)
		cSizeHint = (m_nOrientation == Horizontal ? pWidget->GetLayoutHints().GetMarginBottom() : pWidget->GetLayoutHints().GetMarginRight());
		nMarginBehind2 = cSizeHint.CalculateSize(nContainerSize2, -1);
		if (nMarginBehind2 == -1) nMarginBehind2 = 0;

		// Get size of element
		cSizeHint = (m_nOrientation == Horizontal ? pWidget->GetLayoutHints().GetSizeY() : pWidget->GetLayoutHints().GetSizeX());
		nSize2 = cSizeHint.CalculateSize(nContainerSize2, nPreferredSize2);
		if (nSize2 == -1) {
			nSize2 = nContainerSize2 - nMarginBefore2 - nMarginBehind2;
			if (nSize2 < 0) nSize2 = 0;
		}

		// Apply minimum size
		cSizeHint = (m_nOrientation == Horizontal ? pWidget->GetLayoutHints().GetMinSizeY() : pWidget->GetLayoutHints().GetMinSizeX());
		int nMinSize = cSizeHint.CalculateSize(nContainerSize2, nPreferredSize2);
		if (nMinSize > -1 && nSize2 < nMinSize) nSize2 = nMinSize;

		// Apply maximum size
		cSizeHint = (m_nOrientation == Horizontal ? pWidget->GetLayoutHints().GetMaxSizeY() : pWidget->GetLayoutHints().GetMaxSizeX());
		int nMaxSize = cSizeHint.CalculateSize(nContainerSize2, nPreferredSize2);
		if (nMaxSize > -1 && nSize2 > nMaxSize) nSize2 = nMaxSize;

		// Apply alignment, if the element does not fill out the whole space that it could
		int nWholeSpace = nContainerSize2 - nMarginBefore2 - nMarginBehind2;
		if (nSize2 < nWholeSpace) {
			int nDiff = nWholeSpace - nSize2;
				 if (nVAlign == AlignBottom) nMarginBefore2 += nDiff;
			else if (nVAlign == AlignMiddle) nMarginBefore2 += nDiff / 2;
		}
	}

	// Set start position, keep in mind alignment and padding
	uint32 nCurrentPos = nPaddingLeft;
	if (nSizeLeft > 0) {
			 if (nAlign == AlignRight)  nCurrentPos += nSizeLeft;
		else if (nAlign == AlignCenter) nCurrentPos += nSizeLeft / 2;
	}

	// Apply position and size to elements
	for (uint32 i=0; i<m_pWidget->GetChildren().GetNumOfElements(); i++) {
		// Get child widget
		Widget *pWidget = m_pWidget->GetChildren().Get(i);

		// Get position references
		Vector2i vPos = pWidget->GetPos();
		int &nPos1  = (m_nOrientation == Horizontal ? vPos.x  : vPos.y);
		int &nPos2  = (m_nOrientation == Horizontal ? vPos.y  : vPos.x);

		// Get size references
		int &nMarginBefore1	= (m_nOrientation == Horizontal ? m_pSizes[i*3+0].x : m_pSizes[i*3+0].y);
		int &nMarginBefore2	= (m_nOrientation == Horizontal ? m_pSizes[i*3+0].y : m_pSizes[i*3+0].x);
		int &nSize1			= (m_nOrientation == Horizontal ? m_pSizes[i*3+1].x : m_pSizes[i*3+1].y);
//		int &nSize2			= (m_nOrientation == Horizontal ? m_pSizes[i*3+1].y : m_pSizes[i*3+1].x);
		int &nMarginBehind1	= (m_nOrientation == Horizontal ? m_pSizes[i*3+2].x : m_pSizes[i*3+2].y);
//		int &nMarginBehind2	= (m_nOrientation == Horizontal ? m_pSizes[i*3+2].y : m_pSizes[i*3+2].x);

		// Apply margin (before element)
		nCurrentPos += nMarginBefore1;

		// Set position
		nPos1 = nCurrentPos;
		nPos2 = nPaddingTop + nMarginBefore2;
		pWidget->SetPos(vPos);

		// Set size
		pWidget->SetSize(m_pSizes[i*3+1]);

		// Apply size
		nCurrentPos += nSize1;

		// Apply margin (behind element)
		nCurrentPos += nMarginBehind1;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP
