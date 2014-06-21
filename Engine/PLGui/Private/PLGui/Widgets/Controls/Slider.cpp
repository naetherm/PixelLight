/*********************************************************\
 *  File: Slider.cpp                                     *
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
#include "PLGui/Widgets/Controls/Slider.h"


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
pl_implement_class(Slider)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Slider::Slider(Widget *pParent) : AbstractSlider(pParent),
	Orientation(this),
	m_nOrientation(Horizontal)
{
	// Don't draw a background
	SetBackgroundColor(Color4::Transparent);

	// Init data for handle
	m_sHandle.vPos1			 = Vector2i::Zero;
	m_sHandle.vPos2			 = Vector2i::Zero;
	m_sHandle.bMouseOver	 = false;
	m_sHandle.bPressed		 = false;
	m_sHandle.nMinPos		 = 0;
	m_sHandle.nMaxPos		 = 0;
}

/**
*  @brief
*    Destructor
*/
Slider::~Slider()
{
}

/**
*  @brief
*    Get orientation
*/
EOrientation Slider::GetOrientation() const
{
	// Return orientation
	return m_nOrientation;
}

/**
*  @brief
*    Set orientation
*/
void Slider::SetOrientation(EOrientation nOrientation)
{
	// Set orientation
	m_nOrientation = nOrientation;
}


//[-------------------------------------------------------]
//[ Protected virtual AbstractSlider functions            ]
//[-------------------------------------------------------]
void Slider::OnChangeValue(int nValue)
{
	// Call base implementation
	AbstractSlider::OnChangeValue(nValue);

	// Redraw widget
	Redraw();
}


//[-------------------------------------------------------]
//[ Public virtual Widget functions                       ]
//[-------------------------------------------------------]
void Slider::OnDraw(Graphics &cGraphics)
{
	// Calculate handle position
	CalculateHandlePos();

	// Draw slider
	m_pTheme->DrawSlider(cGraphics, Vector2i::Zero, GetSize() - Vector2i(1, 1), m_nOrientation, GetWidgetState());
	m_pTheme->DrawSliderHandle(cGraphics, m_sHandle.vPos1, m_sHandle.vPos2, m_nOrientation, GetHandleState());
}

Vector2i Slider::OnPreferredSize(const Vector2i &vRefSize) const
{
	// Return size of slider
	if (m_nOrientation == Horizontal)	return Vector2i(-1, 18);
	else								return Vector2i(18, -1);
}

void Slider::OnDisable()
{
	// Release mouse
	SetCaptureMouse(false);

	// Reset state
	m_sHandle.bMouseOver = m_sHandle.bPressed = false;

	// Redraw widget
	Redraw();
}

void Slider::OnMouseLeave()
{
	// Reset mouse-over flags
	if (m_sHandle.bMouseOver) {
		// Reset state
		m_sHandle.bMouseOver = m_sHandle.bPressed = false;

		// Redraw widget
		Redraw();
	}
}

void Slider::OnMouseMove(const PLMath::Vector2i &vPos)
{
	// Keep track is anything has changed
	bool bRedraw = false;

	// Save mouse position
	m_vMousePos = vPos;

	// Move handle?
	if (m_sHandle.bMouseOver && m_sHandle.bPressed) {
		// Get range
		int nRange = m_nMaxValue - m_nMinValue;

		// Get movable size
		int nSize = m_sHandle.nMaxPos - m_sHandle.nMinPos + 1;
		if (nSize <= 0) nSize = 1;

		// Get ratio of screen-size to value-size
		float fRatio = static_cast<float>(nRange) / static_cast<float>(nSize);

		// Get screen position
		int nScreenPos = 0;
			 if (m_nOrientation == Horizontal)	nScreenPos = m_vMousePos.x - m_sHandle.nMinPos;
		else if (m_nOrientation == Vertical)	nScreenPos = m_vMousePos.y - m_sHandle.nMinPos;

		// Calculate new value
		int nNewValue = static_cast<int>(m_nMinValue + static_cast<float>(nScreenPos) * fRatio);
		if (m_nValue != nNewValue) {
			// Set Value
			SetValue(nNewValue);
		}
	} else {
		// Check if mouse is over the handle
		bool bSlider = (vPos >= m_sHandle.vPos1 && vPos <= m_sHandle.vPos2);
		if (m_sHandle.bMouseOver != bSlider) {
			m_sHandle.bMouseOver  = bSlider;
			bRedraw = true;
		}

		// Check if slider has been handle
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

void Slider::OnMouseButtonDown(uint32 nButton, const PLMath::Vector2i &vPos)
{
	// Left mouse button
	if (nButton == LeftButton) {
		// Capture mouse
		SetCaptureMouse(true);

		// Check for pressed buttons
		CheckButtons(vPos);
	}
}

void Slider::OnMouseButtonUp(uint32 nButton, const PLMath::Vector2i &vPos)
{
	// Left mouse button
	if (nButton == LeftButton) {
		// Release mouse
		SetCaptureMouse(false);

		// Reset button states
		if (m_sHandle.bPressed) {
			// Reset
			m_sHandle.bPressed = false;

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
*    Calculate position of slider handle
*/
void Slider::CalculateHandlePos()
{
	// Get slider position and size
	Vector2i vSliderPos1 = Vector2i::Zero;
	Vector2i vSliderPos2 = GetSize() - Vector2i(1, 1);
	if (m_nOrientation == Horizontal)	vSliderPos2.x -= 10;
	else								vSliderPos2.y -= 10;

	// Get value range
	int nRange = GetMaxValue() - GetMinValue();
	if (nRange < 1) nRange = 1;

	// Get slider range
	int nPos1, nPos2;
	if (m_nOrientation == Horizontal) {
		nPos1 = vSliderPos1.x;
		nPos2 = vSliderPos2.x;
	} else {
		nPos1 = vSliderPos1.y;
		nPos2 = vSliderPos2.y;
	}
	int nSize = nPos2 - nPos1 + 1;
	if (nSize < 0) nSize = 0;

	// Get current position
	int nPos = GetValue() - GetMinValue();

	// Calculate current handle position
	int nHandlePos = static_cast<int>(static_cast<float>(nSize) * static_cast<float>(nPos) / static_cast<float>(nRange));
	if (m_nOrientation == Horizontal) {
		m_sHandle.vPos1		= Vector2i(vSliderPos1.x + nHandlePos,			vSliderPos1.y);
		m_sHandle.vPos2		= Vector2i(vSliderPos1.x + nHandlePos + 10 - 1,	vSliderPos2.y);
		m_sHandle.nMinPos	= vSliderPos1.x;
		m_sHandle.nMaxPos	= vSliderPos2.x - (10 - 1);
	} else {
		m_sHandle.vPos1		= Vector2i(vSliderPos1.x, vSliderPos1.y + nHandlePos);
		m_sHandle.vPos2		= Vector2i(vSliderPos2.x, vSliderPos1.y + nHandlePos + 10 - 1);
		m_sHandle.nMinPos	= vSliderPos1.y;
		m_sHandle.nMaxPos	= vSliderPos2.y - (10 - 1);
	}
}

/**
*  @brief
*    Get widget state for the handle
*/
uint32 Slider::GetHandleState()
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
void Slider::CheckButtons(const Vector2i &vPos)
{
	// Inside handle?
	if (vPos >= m_sHandle.vPos1 && vPos <= m_sHandle.vPos2) {
		m_sHandle.bPressed = true;
		Redraw();
	}

	// Left/top of handle?
	else if ( (m_nOrientation == Horizontal && vPos.x < m_sHandle.vPos1.x && vPos.y >= m_sHandle.vPos1.y && vPos.y <= m_sHandle.vPos2.y) ||
			  (m_nOrientation == Vertical   && vPos.y < m_sHandle.vPos1.y && vPos.x >= m_sHandle.vPos1.x && vPos.x <= m_sHandle.vPos2.x) )
	{
		Decrease();
	}

	// Right/bottom of handle?
	else if ( (m_nOrientation == Horizontal && vPos.x > m_sHandle.vPos2.x && vPos.y >= m_sHandle.vPos1.y && vPos.y <= m_sHandle.vPos2.y) ||
			  (m_nOrientation == Vertical   && vPos.y > m_sHandle.vPos2.y && vPos.x >= m_sHandle.vPos1.x && vPos.x <= m_sHandle.vPos2.x) )
	{
		Increase();
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
