/*********************************************************\
 *  File: Splitter.cpp                                   *
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
#include "PLGui/Gui/Resources/Graphics.h"
#include "PLGui/Themes/Theme.h"
#include "PLGui/Widgets/Containers/Splitter.h"


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_PUSH
PL_WARNING_DISABLE(4355) // "'this' : used in base member initializer list"


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
pl_implement_class(Splitter)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Splitter::Splitter(Widget *pParent) : Widget(pParent),
	Orientation(this),
	Sizable(this),
	SizeLeft(this),
	SizeRight(this),
	m_nOrientation(Horizontal),
	m_bSizable(true),
	m_pContentLeft(nullptr),
	m_pContentRight(nullptr),
	m_bSizing(false)
{
	// Create left content widget
	m_pContentLeft = new Widget(this);
	m_pContentLeft->SetLayout("LayoutBox", "Orientation=Vertical");
	m_pContentLeft->SetVisible(true);

	// Create right content widget
	m_pContentRight = new Widget(this);
	m_pContentRight->SetLayout("LayoutBox", "Orientation=Vertical");
	m_pContentRight->SetVisible(true);

	// Give focus to child windows
	SetFocusStyle(ChildFocus);
	m_pContentLeft ->SetFocusStyle(ChildFocus);
	m_pContentRight->SetFocusStyle(ChildFocus);
}

/**
*  @brief
*    Destructor
*/
Splitter::~Splitter()
{
}

/**
*  @brief
*    Get orientation
*/
EOrientation Splitter::GetOrientation() const
{
	// Return orientation
	return m_nOrientation;
}

/**
*  @brief
*    Set orientation
*/
void Splitter::SetOrientation(EOrientation nOrientation)
{
	// Set orientation
	m_nOrientation = nOrientation;

	// Size might have changed
	UpdateContent();

	// Redraw widget
	Redraw();
}

/**
*  @brief
*    Check if size can be altered by the user
*/
bool Splitter::GetSizable() const
{
	// Return sizable-flag
	return m_bSizable;
}

/**
*  @brief
*    Set if size can be altered by the user
*/
void Splitter::SetSizable(bool bSizable)
{
	// Set sizable-flag
	m_bSizable = bSizable;
}

/**
*  @brief
*    Get size of left (or top) side
*/
const SizeHint &Splitter::GetSizeLeft() const
{
	// Return size
	return m_cSizeLeft;
}

/**
*  @brief
*    Set size of left (or top) side
*/
void Splitter::SetSizeLeft(const SizeHint &cSize)
{
	// Set size
	m_cSizeLeft  = cSize;
}

/**
*  @brief
*    Get size of right (or bottom) side
*/
const SizeHint &Splitter::GetSizeRight() const
{
	// Return size
	return m_cSizeRight;
}

/**
*  @brief
*    Set size of right (or bottom) side
*/
void Splitter::SetSizeRight(const SizeHint &cSize)
{
	// Set size
	m_cSizeRight = cSize;
}

/**
*  @brief
*    Get content widget of a given side
*/
Widget *Splitter::GetSide(ESide nSide) const
{
	// Return content widget
	if (nSide == SideLeft || nSide == SideTop)	return m_pContentLeft;
	else										return m_pContentRight;
}

/**
*  @brief
*    Get content widget
*/
Widget *Splitter::GetContentWidget() const
{
	// Return left content widget
	return m_pContentLeft;
}


//[-------------------------------------------------------]
//[ Public virtual Widget functions                       ]
//[-------------------------------------------------------]
void Splitter::OnDraw(Graphics &cGraphics)
{
	// Get size of content widgets
	Vector2i vCont1 = m_pContentLeft->GetSize() - Vector2i(1, 1);
	Vector2i vCont2 = m_pContentRight->GetPos();

	// Set splitter position between the two sides
	Vector2i vPos1, vPos2;
	if (m_nOrientation == Horizontal) {
		// Horizontal gap
		vPos1 = Vector2i(vCont1.x+1, 0);
		vPos2 = Vector2i(vCont2.x-1, vCont1.y);
	} else {
		// Vertical gap
		vPos1 = Vector2i(       0, vCont1.y+1);
		vPos2 = Vector2i(vCont1.x, vCont2.y-1);
	}

	// Draw splitter
	m_pTheme->DrawSplitter(cGraphics, vPos1, vPos2, m_nOrientation);
}

Vector2i Splitter::OnPreferredSize(const Vector2i &vRefSize) const
{
	// Give me everything
	return Vector2i(-1, -1);
}

void Splitter::OnAdjustContent()
{
	// Get splitter size
	const int nGap = 8;

	// Get complete size
	Vector2i vSize = GetSize();
	int nSize = (m_nOrientation == Horizontal ? vSize.x : vSize.y);
	nSize -= nGap;
	if (nSize < 0) nSize = 0;

	// Calculate size for left side
	int nSize1 = m_cSizeLeft. CalculateSize(nSize, -1);
	if (nSize1 > -1) {
		// Size assigned to left side, get rest
		nSize -= nSize1;
		if (nSize < 0) nSize = 0;
	}

	// Calculate size for right side
	int nSize2 = m_cSizeRight.CalculateSize(nSize, -1);
	if (nSize2 > -1) {
		// Size assigned to right side, get rest
		nSize -= nSize2;
		if (nSize < 0) nSize = 0;
	}

	// Give rest size to widgets that have not been assigned a size yet
	if (nSize1 == -1 && nSize2 == -1) {
		nSize1 = nSize/2;
		nSize2 = nSize - nSize1;
	} else if (nSize1 == -1) {
		nSize1 = nSize;
	} else if (nSize2 == -1) {
		nSize2 = nSize;
	}

	// Set position and size of content widgets
	Vector2i vPos1  = Vector2i::Zero;
	Vector2i vPos2  = (m_nOrientation == Horizontal ? Vector2i(nSize1+nGap,  0) : Vector2i(0, nSize1+nGap ));
	Vector2i vSize1 = (m_nOrientation == Horizontal ? Vector2i(nSize1, vSize.y) : Vector2i(vSize.x, nSize1));
	Vector2i vSize2 = (m_nOrientation == Horizontal ? Vector2i(nSize2, vSize.y) : Vector2i(vSize.x, nSize2));
	if (m_pContentLeft) {
		m_pContentLeft->SetPos (vPos1);
		m_pContentLeft->SetSize(vSize1);
	}
	if (m_pContentRight) {
		m_pContentRight->SetPos (vPos2);
		m_pContentRight->SetSize(vSize2);
	}
}

void Splitter::OnMouseLeave()
{
	// Change mouse cursor
	Cursor cCursor(*GetGui(), CursorArrow);
	SetCursor(cCursor);
}

void Splitter::OnMouseMove(const Vector2i &vPos)
{
	// Change mouse cursor
	Cursor cCursor(*GetGui(), m_nOrientation == Horizontal ? CursorSizeLeftRight : CursorSizeTopBottom);
	SetCursor(cCursor);

	// Currently sizing?
	if (m_bSizing) {
		// Get relative movement
		Vector2i vRel = vPos - m_vMousePos;

		// Change size
		if (m_nOrientation == Horizontal) ChangeSize(vRel.x);
		else							  ChangeSize(vRel.y);
	}

	// Save mouse position
	m_vMousePos = vPos;
}

void Splitter::OnMouseButtonDown(uint32 nButton, const Vector2i &vPos)
{
	// Left mouse button
	if (nButton == LeftButton) {
		// Is the widget sizable?
		if (m_bSizable) {
			// Start sizing
			SetCaptureMouse(true);
			m_bSizing = true;
		}
	}
}

void Splitter::OnMouseButtonUp(uint32 nButton, const Vector2i &vPos)
{
	// Left mouse button
	if (nButton == LeftButton) {
		// Stop sizing
		if (m_bSizing) {
			// Stop sizing
			SetCaptureMouse(false);
			m_bSizing = false;
		}
	}
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Change size of content widgets
*/
void Splitter::ChangeSize(int nDelta)
{
	// Get splitter size
	const int nGap = 8;

	// Validate size hints
	bool bStaticLeft  = (m_cSizeLeft .GetPolicy() == SizeHint::Pixel || m_cSizeLeft. GetPolicy() == SizeHint::Percent);
	bool bStaticRight = (m_cSizeRight.GetPolicy() == SizeHint::Pixel || m_cSizeRight.GetPolicy() == SizeHint::Percent);
	if (bStaticLeft && bStaticRight) {
		// If both sides are set to a static size, we prefer the left/top side and make the other floating
		m_cSizeRight.Set(SizeHint::Floating, 0);
		bStaticRight = false;
	} else if (!bStaticLeft && !bStaticRight) {
		// If both sides are floating, we set the left/top side to 50% and make the other floating
		m_cSizeLeft .Set(SizeHint::Percent, 50);
		m_cSizeRight.Set(SizeHint::Floating, 0);
		bStaticLeft = true;
	}

	// Get size which we are going to change
	SizeHint &cSize = (bStaticLeft ? m_cSizeLeft : m_cSizeRight);

	// Get overall size
	int nOverallSize = (m_nOrientation == Horizontal ? GetSize().x : GetSize().y) - nGap;
	if (nOverallSize < 0) nOverallSize = 0;

	// Get size delta
	int nSizeDelta = (bStaticLeft ? nDelta : -nDelta);
	if (cSize.GetPolicy() == SizeHint::Percent) {
		// Convert from percent to pixels
		float fSizeDelta = 100.0f * static_cast<float>(nSizeDelta) / static_cast<float>(nOverallSize);

		// Change size
		float fNewSize = cSize.GetSizeFloat() + fSizeDelta;
		if (fNewSize <   0.0f) fNewSize =   0.0f;
		if (fNewSize > 100.0f) fNewSize = 100.0f;
		cSize.SetSizeFloat(fNewSize);
	} else {
		// Change size
		int nNewSize = cSize.GetSize() + nSizeDelta;
		if (nNewSize < 0)			 nNewSize = 0;
		if (nNewSize > nOverallSize) nNewSize = nOverallSize;
		cSize.SetSize(nNewSize);
	}

	// Apply changes
	AdjustContent();
	Redraw();
}


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
