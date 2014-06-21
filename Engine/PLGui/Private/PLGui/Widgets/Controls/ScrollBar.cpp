/*********************************************************\
 *  File: ScrollBar.cpp                                  *
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
#include "PLGui/Themes/Theme.h"
#include "PLGui/Widgets/Controls/ScrollBar.h"


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
pl_implement_class(ScrollBar)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ScrollBar::ScrollBar(Widget *pParent) : AbstractSlider(pParent),
	Orientation(this),
	SlotOnTimer(this),
	m_nOrientation(Horizontal),
	m_nRepeatInterval(50),
	m_cTimer(*GetGui())
{
	// Don't draw a background
	SetBackgroundColor(Color4::Transparent);

	// Connect timer callback
	m_cTimer.EventFire.Connect(SlotOnTimer);

	// Init data for button 'minus'
	m_sButtonMinus.vPos1		= Vector2i::Zero;
	m_sButtonMinus.vPos2		= Vector2i::Zero;
	m_sButtonMinus.bMouseOver	= false;
	m_sButtonMinus.bPressed		= false;

	// Init data for button 'plus'
	m_sButtonPlus.vPos1			= Vector2i::Zero;
	m_sButtonPlus.vPos2			= Vector2i::Zero;
	m_sButtonPlus.bMouseOver	= false;
	m_sButtonPlus.bPressed		= false;

	// Init data for slider
	m_sHandle.vPos1				= Vector2i::Zero;
	m_sHandle.vPos2				= Vector2i::Zero;
	m_sHandle.bMouseOver		= false;
	m_sHandle.bPressed			= false;
	m_sHandle.nMinPos			= 0;
	m_sHandle.nMaxPos			= 0;
}

/**
*  @brief
*    Destructor
*/
ScrollBar::~ScrollBar()
{
}

/**
*  @brief
*    Get orientation
*/
EOrientation ScrollBar::GetOrientation() const
{
	// Return orientation
	return m_nOrientation;
}

/**
*  @brief
*    Set orientation
*/
void ScrollBar::SetOrientation(EOrientation nOrientation)
{
	// Set orientation
	m_nOrientation = nOrientation;
}


//[-------------------------------------------------------]
//[ Protected virtual AbstractSlider functions            ]
//[-------------------------------------------------------]
void ScrollBar::OnChangeValue(int nValue)
{
	// Call base implementation
	AbstractSlider::OnChangeValue(nValue);

	// Redraw widget
	Redraw();
}


//[-------------------------------------------------------]
//[ Public virtual Widget functions                       ]
//[-------------------------------------------------------]
void ScrollBar::OnDraw(Graphics &cGraphics)
{
	// Calculate positions
	CalculateMinusButtonPos();
	CalculatePlusButtonPos ();
	CalculateHandlePos	   ();

	// Draw scrollbar
	m_pTheme->DrawScrollBar(cGraphics, Vector2i::Zero, GetSize() - Vector2i(1, 1), m_nOrientation, GetWidgetState());

	// Draw handle and buttons
	m_pTheme->DrawScrollBarMinusButton(cGraphics, m_sButtonMinus.vPos1, m_sButtonMinus.vPos2, m_nOrientation, GetMinusButtonState());
	m_pTheme->DrawScrollBarPlusButton (cGraphics, m_sButtonPlus	.vPos1, m_sButtonPlus .vPos2, m_nOrientation, GetPlusButtonState());
	m_pTheme->DrawScrollBarHandle	  (cGraphics, m_sHandle		.vPos1, m_sHandle	  .vPos2, m_nOrientation, GetHandleState());

	/*
	// Get theme painter
	if (m_pThemePainter) {
		// Cast to scrollbar painter
		ThemePainter_ScrollBar *pPainter = (ThemePainter_ScrollBar*)m_pThemePainter;

		// Calculate positions
		pPainter->CalculateMinusButtonPos(m_sButtonMinus.vPos1, m_sButtonMinus.vPos2);
		pPainter->CalculatePlusButtonPos (m_sButtonPlus .vPos1, m_sButtonPlus .vPos2);
		pPainter->CalculateSliderPos	 (m_sHandle		.vPos1, m_sHandle	  .vPos2, m_sHandle.nMinPos, m_sHandle.nMaxPos);

		// Draw scrollbar
		pPainter->DrawScrollBar(cGraphics);
		pPainter->DrawMinusButton(cGraphics, m_sButtonMinus.vPos1, m_sButtonMinus.vPos2, m_sButtonMinus.bMouseOver, m_sButtonMinus.bPressed);
		pPainter->DrawPlusButton (cGraphics, m_sButtonPlus .vPos1, m_sButtonPlus .vPos2, m_sButtonPlus .bMouseOver, m_sButtonPlus .bPressed);
		if (IsEnabled()) {
			pPainter->DrawSlider (cGraphics, m_sHandle     .vPos1, m_sHandle     .vPos2, m_sHandle     .bMouseOver, m_sHandle     .bPressed);
		}
	}
	*/
}

Vector2i ScrollBar::OnPreferredSize(const Vector2i &vRefSize) const
{
	// Return size of scrollbar
	if (m_nOrientation == Horizontal)	return Vector2i(-1, 18);
	else								return Vector2i(18, -1);
}

void ScrollBar::OnDisable()
{
	// Deactivate timer
	if (m_cTimer.IsActive()) m_cTimer.Stop();

	// Release mouse
	SetCaptureMouse(false);

	// Reset state
	m_sButtonMinus.bMouseOver = m_sButtonMinus.bPressed = false;
	m_sButtonPlus .bMouseOver = m_sButtonPlus .bPressed = false;
	m_sHandle	  .bMouseOver = m_sHandle	 .bPressed = false;

	// Redraw widget
	Redraw();
}

void ScrollBar::OnMouseLeave()
{
	// Reset all mouse-over flags
	if (m_sButtonMinus.bMouseOver || m_sButtonPlus.bMouseOver || m_sHandle.bMouseOver) {
		// Reset state
		m_sButtonMinus.bMouseOver = m_sButtonMinus.bPressed = false;
		m_sButtonPlus .bMouseOver = m_sButtonPlus .bPressed = false;
		m_sHandle	  .bMouseOver = m_sHandle	  .bPressed = false;

		// Redraw widget
		Redraw();
	}
}

void ScrollBar::OnMouseMove(const PLMath::Vector2i &vPos)
{
	// Keep track is anything has changed
	bool bRedraw = false;

	// Save mouse position
	Vector2i vRel = vPos - m_vMousePos;
	m_vMousePos = vPos;

	// Move slider?
	if (m_sHandle.bMouseOver && m_sHandle.bPressed) {
		// Get range
		int nRange = m_nMaxValue - m_nMinValue + 1;

		// Get movable size
		int nSize = m_sHandle.nMaxPos - m_sHandle.nMinPos + 1;
		if (nSize <= 0) nSize = 1;

		// Get ratio of screen-size to value-size
		float fRatio = static_cast<float>(nRange) / static_cast<float>(nSize);

		// Get screen delta
		int nScreenDelta = 0;
			 if (m_nOrientation == Horizontal)	nScreenDelta = vRel.x;
		else if (m_nOrientation == Vertical)	nScreenDelta = vRel.y;
		if (nScreenDelta != 0) {
			// Change value
			int nDelta = static_cast<int>(nScreenDelta * fRatio);
			if (nDelta == 0) nDelta = (nScreenDelta > 0 ? 1 : -1);
			SetValue(GetValue() + nDelta);
		}
	} else {
		// Check if mouse is inside button 'minus'
		bool bButtonMinus = (vPos >= m_sButtonMinus.vPos1 && vPos <= m_sButtonMinus.vPos2);
		if (m_sButtonMinus.bMouseOver != bButtonMinus) {
			m_sButtonMinus.bMouseOver  = bButtonMinus;
			bRedraw = true;
		}

		// Check if button 'minus' has been released
		if (m_sButtonMinus.bPressed && !m_sButtonMinus.bMouseOver) {
			m_sButtonMinus.bPressed = false;
			bRedraw = true;
		}

		// Check if mouse is inside button 'plus'
		bool bButtonPlus = (vPos >= m_sButtonPlus.vPos1 && vPos <= m_sButtonPlus.vPos2);
		if (m_sButtonPlus.bMouseOver != bButtonPlus) {
			m_sButtonPlus.bMouseOver  = bButtonPlus;
			bRedraw = true;
		}

		// Check if button 'plus' has been released
		if (m_sButtonPlus.bPressed && !m_sButtonPlus.bMouseOver) {
			m_sButtonPlus.bPressed = false;
			bRedraw = true;
		}

		// Check if mouse is over the slider
		bool bSlider = (vPos >= m_sHandle.vPos1 && vPos <= m_sHandle.vPos2);
		if (m_sHandle.bMouseOver != bSlider) {
			m_sHandle.bMouseOver  = bSlider;
			bRedraw = true;
		}

		// Check if slider has been released
		if (m_sHandle.bPressed && !m_sHandle.bMouseOver) {
			m_sHandle.bPressed = false;
			bRedraw = true;
		}
	}

	// Redraw widget if necessary
	if (bRedraw) {
		Redraw();
	}
}

void ScrollBar::OnMouseButtonDown(uint32 nButton, const PLMath::Vector2i &vPos)
{
	// Left mouse button
	if (nButton == LeftButton) {
		// Capture mouse
		SetCaptureMouse(true);

		// Check for pressed buttons
		CheckButtons(vPos, true);

		// Start timer
		m_cTimer.Start(m_nRepeatInterval);
	}
}

void ScrollBar::OnMouseButtonUp(uint32 nButton, const PLMath::Vector2i &vPos)
{
	// Left mouse button
	if (nButton == LeftButton) {
		// Stop timer
		m_cTimer.Stop();

		// Release mouse
		SetCaptureMouse(false);

		// Reset button states
		if (m_sButtonMinus.bPressed || m_sButtonPlus.bPressed || m_sHandle.bPressed) {
			// Reset
			m_sButtonMinus.bPressed = false;
			m_sButtonPlus.bPressed	= false;
			m_sHandle.bPressed		= false;

			// Redraw widget
			Redraw();
		}
	}
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Calculate position of minus button
*/
void ScrollBar::CalculateMinusButtonPos()
{
	// Set position and size of button 'minus'
	m_sButtonMinus.vPos1 = Vector2i( 0,  0);
	m_sButtonMinus.vPos2 = Vector2i(17, 17);
}

/**
*  @brief
*    Calculate position of plus button
*/
void ScrollBar::CalculatePlusButtonPos()
{
	// Set position and size of button 'plus'
	m_sButtonPlus.vPos1 = GetSize() - Vector2i(18, 18);
	m_sButtonPlus.vPos2 = GetSize() - Vector2i( 1,  1);
}

/**
*  @brief
*    Calculate position of scrollbar handle
*/
void ScrollBar::CalculateHandlePos()
{
	// Get scrollbar position and size
	Vector2i vScrollBarPos1 = Vector2i::Zero;
	Vector2i vScrollBarPos2 = GetSize() - Vector2i(1, 1);

	// Get value range
	int nRange = GetMaxValue() - GetMinValue() + 1;
	if (nRange < 1) nRange = 1;

	// Get scrollbar range
	int nPos1, nPos2;
	if (m_nOrientation == Horizontal) {
		nPos1 = vScrollBarPos1.x + 18;
		nPos2 = vScrollBarPos2.x - 18;
	} else {
		nPos1 = vScrollBarPos1.y + 18;
		nPos2 = vScrollBarPos2.y - 18;
	}
	int nSize = nPos2 - nPos1 + 1;
	if (nSize < 0) nSize = 0;

	// Get slider size
	int nSliderSize = nSize - nRange;
	if (nSliderSize < 10) nSliderSize = 10;

	// Subtract slider size from scrollbar size
	nSize -= nSliderSize;
	if (nSize < 0) nSize = 0;

	// Get current position
	int nPos = GetValue() - GetMinValue();

	// Calculate current handle position
	int nHandlePos = static_cast<int>(static_cast<float>(nSize) * static_cast<float>(nPos) / static_cast<float>(nRange));
	if (m_nOrientation == Horizontal) {
		m_sHandle.vPos1 = Vector2i(nPos1 + nHandlePos,					vScrollBarPos1.y);
		m_sHandle.vPos2 = Vector2i(nPos1 + nHandlePos + nSliderSize - 1,	vScrollBarPos2.y);
		m_sHandle.nMinPos = nPos1;
		m_sHandle.nMaxPos = nPos2 - (nSliderSize - 1);
	} else {
		m_sHandle.vPos1 = Vector2i(vScrollBarPos1.x, nPos1 + nHandlePos);
		m_sHandle.vPos2 = Vector2i(vScrollBarPos2.x, nPos1 + nHandlePos + nSliderSize - 1);
		m_sHandle.nMinPos = nPos1;
		m_sHandle.nMaxPos = nPos2 - (nSliderSize - 1);
	}

	/*
	// Get current position
	int nPos = GetValue() - GetMinValue();

	// Calculate current slider position
	if (m_nOrientation == Horizontal) {
		vPos1 = Vector2i(vScrollBarPos1.x + 18 + nPos,					 vScrollBarPos1.y);
		vPos2 = Vector2i(vScrollBarPos1.x + 18 + nPos + nSliderSize - 1, vScrollBarPos2.y);
		nMinPos = vPos1.x;
		nMaxPos = vScrollBarPos2.x - (nSliderSize - 1);
	} else {
		vPos1 = Vector2i(vScrollBarPos1.x, vScrollBarPos1.y + 18 + nPos);
		vPos2 = Vector2i(vScrollBarPos2.x, vScrollBarPos1.y + 18 + nPos + nSliderSize - 1);
		nMinPos = vPos1.y;
		nMaxPos = vScrollBarPos2.y - (nSliderSize - 1);
	}
	*/
}

/**
*  @brief
*    Get widget state for the minus button
*/
uint32 ScrollBar::GetMinusButtonState()
{
	// Get widget state
	uint32 nWidgetState = 0;
	if (IsEnabled())				nWidgetState |= WidgetEnabled;
	if (IsActive())					nWidgetState |= WidgetActive;
	if (HasFocus())					nWidgetState |= WidgetFocus;
	if (m_sButtonMinus.bMouseOver)	nWidgetState |= WidgetMouseOver;
	if (m_sButtonMinus.bPressed)	nWidgetState |= WidgetPressed;

	// Return state
	return nWidgetState;
}

/**
*  @brief
*    Get widget state for the plus button
*/
uint32 ScrollBar::GetPlusButtonState()
{
	// Get widget state
	uint32 nWidgetState = 0;
	if (IsEnabled())				nWidgetState |= WidgetEnabled;
	if (IsActive())					nWidgetState |= WidgetActive;
	if (HasFocus())					nWidgetState |= WidgetFocus;
	if (m_sButtonPlus.bMouseOver)	nWidgetState |= WidgetMouseOver;
	if (m_sButtonPlus.bPressed)		nWidgetState |= WidgetPressed;

	// Return state
	return nWidgetState;
}

/**
*  @brief
*    Get widget state for the handle
*/
uint32 ScrollBar::GetHandleState()
{
	// Get widget state
	uint32 nWidgetState = 0;
	if (IsEnabled())			nWidgetState |= WidgetEnabled;
	if (IsActive())				nWidgetState |= WidgetActive;
	if (HasFocus())				nWidgetState |= WidgetFocus;
	if (m_sHandle.bMouseOver)	nWidgetState |= WidgetMouseOver;
	if (m_sHandle.bPressed)		nWidgetState |= WidgetPressed;

	// Return state
	return nWidgetState;
}

/**
*  @brief
*    Check if a button has been pressed
*/
void ScrollBar::CheckButtons(const Vector2i &vPos, bool bSlider)
{
	// Inside button 'minus'?
	if (vPos >= m_sButtonMinus.vPos1 && vPos <= m_sButtonMinus.vPos2) {
		m_sButtonMinus.bPressed = true;
		Decrease();
	}

	// Inside button 'plus'?
	else if (vPos >= m_sButtonPlus.vPos1 && vPos <= m_sButtonPlus.vPos2) {
		m_sButtonPlus.bPressed = true;
		Increase();
	}

	// Inside slider?
	else if (bSlider && vPos >= m_sHandle.vPos1 && vPos <= m_sHandle.vPos2) {
		m_sHandle.bPressed = true;
		Redraw();
	}

	// Left/top of slider?
	else if ( (m_nOrientation == Horizontal && vPos.x < m_sHandle.vPos1.x && vPos.y >= m_sHandle.vPos1.y && vPos.y <= m_sHandle.vPos2.y) ||
			  (m_nOrientation == Vertical   && vPos.y < m_sHandle.vPos1.y && vPos.x >= m_sHandle.vPos1.x && vPos.x <= m_sHandle.vPos2.x) )
	{
		DecreaseFast();
	}

	// Right/bottom of slider?
	else if ( (m_nOrientation == Horizontal && vPos.x > m_sHandle.vPos2.x && vPos.y >= m_sHandle.vPos1.y && vPos.y <= m_sHandle.vPos2.y) ||
			  (m_nOrientation == Vertical   && vPos.y > m_sHandle.vPos2.y && vPos.x >= m_sHandle.vPos1.x && vPos.x <= m_sHandle.vPos2.x) )
	{
		IncreaseFast();
	}
}

/**
*  @brief
*    Called when the timer has fired
*/
void ScrollBar::OnTimer()
{
	// Check for pressed buttons
	CheckButtons(m_vMousePos, false);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP
