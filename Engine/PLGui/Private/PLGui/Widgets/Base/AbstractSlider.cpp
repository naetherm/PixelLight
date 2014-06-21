/*********************************************************\
 *  File: AbstractSlider.cpp                             *
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
#include "PLGui/Gui/Base/Keys.h"
#include "PLGui/Widgets/Base/AbstractSlider.h"


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_PUSH
PL_WARNING_DISABLE(4355) // "'this' : used in base member initializer list"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLGui {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(AbstractSlider)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
AbstractSlider::AbstractSlider(Widget *pParent) : Widget(pParent),
	MinValue(this),
	MaxValue(this),
	Value(this),
	StepSize(this),
	StepSizeFast(this),
	m_nMinValue(1),
	m_nMaxValue(100),
	m_nValue(1),
	m_nStepSize(10),
	m_nStepSizeFast(50)
{
}

/**
*  @brief
*    Destructor
*/
AbstractSlider::~AbstractSlider()
{
}

/**
*  @brief
*    Get minimum value
*/
int AbstractSlider::GetMinValue() const
{
	// Return minimum value
	return m_nMinValue;
}

/**
*  @brief
*    Set minimum value
*/
void AbstractSlider::SetMinValue(int nMinValue)
{
	// Set minimum value
	m_nMinValue = nMinValue;

	// Check if value is still valid
	if (m_nValue < m_nMinValue)
		SetValue(m_nMinValue);
}

/**
*  @brief
*    Get maximum value
*/
int AbstractSlider::GetMaxValue() const
{
	// Return maximum value
	return m_nMaxValue;
}

/**
*  @brief
*    Set maximum value
*/
void AbstractSlider::SetMaxValue(int nMaxValue)
{
	// Set maximum value
	m_nMaxValue = nMaxValue;

	// Check if value is still valid
	if (m_nValue > m_nMaxValue)
		SetValue(m_nMaxValue);
}

/**
*  @brief
*    Get current value
*/
int AbstractSlider::GetValue() const
{
	// Return value
	return m_nValue;
}

/**
*  @brief
*    Set current value
*/
void AbstractSlider::SetValue(int nValue)
{
	// Clip value
	int nValueClipped = nValue;
	if (nValueClipped < m_nMinValue) nValueClipped = m_nMinValue;
	if (nValueClipped > m_nMaxValue) nValueClipped = m_nMaxValue;

	// Check if value has changed
	if (m_nValue != nValueClipped) {
		// Set value
		m_nValue = nValueClipped;

		// Call virtual function
		OnChangeValue(m_nValue);
	}
}

/**
*  @brief
*    Get step size (single step)
*/
int AbstractSlider::GetStepSize() const
{
	// Return step-size
	return m_nStepSize;
}

/**
*  @brief
*    Set step size (single step)
*/
void AbstractSlider::SetStepSize(int nStepSize)
{
	// Set step-size
	m_nStepSize = nStepSize;
}

/**
*  @brief
*    Get step size (fast step)
*/
int AbstractSlider::GetStepSizeFast() const
{
	// Return step-size
	return m_nStepSizeFast;
}

/**
*  @brief
*    Set step size (fast step)
*/
void AbstractSlider::SetStepSizeFast(int nStepSize)
{
	// Set step-size
	m_nStepSizeFast = nStepSize;
}

/**
*  @brief
*    Increase value (single step)
*/
void AbstractSlider::Increase()
{
	// Increase value
	SetValue(m_nValue + m_nStepSize);
}

/**
*  @brief
*    Decrease value (single step)
*/
void AbstractSlider::Decrease()
{
	// Decrease value
	SetValue(m_nValue - m_nStepSize);
}

/**
*  @brief
*    Increase value (fast step)
*/
void AbstractSlider::IncreaseFast()
{
	// Increase value
	SetValue(m_nValue + m_nStepSizeFast);
}

/**
*  @brief
*    Decrease value (fast step)
*/
void AbstractSlider::DecreaseFast()
{
	// Decrease value
	SetValue(m_nValue - m_nStepSizeFast);
}


//[-------------------------------------------------------]
//[ Protected virtual AbstractSlider functions            ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a new value has been set
*/
void AbstractSlider::OnChangeValue(int nValue)
{
	// Emit signal
	SignalChangeValue(nValue);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP
