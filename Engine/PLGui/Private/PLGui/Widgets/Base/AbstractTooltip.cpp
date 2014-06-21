/*********************************************************\
 *  File: AbstractTooltip.cpp                            *
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
#include "PLGui/Widgets/Base/AbstractTooltip.h"


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
pl_implement_class(AbstractTooltip)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
AbstractTooltip::AbstractTooltip(Widget *pParent) : Widget(pParent),
	Text(this),
	Timeout(this),
	SlotOnTimer(this),
	m_nTimeout(5000),
	m_cTimer(*GetGui())
{
	// Connect to timer
	m_cTimer.EventFire.Connect(SlotOnTimer);
}

/**
*  @brief
*    Destructor
*/
AbstractTooltip::~AbstractTooltip()
{
}

/**
*  @brief
*    Get tooltip text
*/
String AbstractTooltip::GetText() const
{
	// Return tooltip
	return m_sTooltip;
}

/**
*  @brief
*    Set tooltip text
*/
void AbstractTooltip::SetText(const String &sTooltip)
{
	// Set tooltip
	m_sTooltip = sTooltip;
}

/**
*  @brief
*    Get tooltip timeout
*/
uint64 AbstractTooltip::GetTimeout() const
{
	// Return timeout
	return m_nTimeout;
}

/**
*  @brief
*    Set tooltip timeout
*/
void AbstractTooltip::SetTimeout(uint64 nTimeout)
{
	// Set timeout
	m_nTimeout = nTimeout;
}

/**
*  @brief
*    Show tooltip
*/
void AbstractTooltip::ShowTooltip(const Vector2i &vPos)
{
	// Set position
	SetPos(vPos);

	// Call virtual function
	OnShowTooltip();
}

/**
*  @brief
*    Blend out tooltip
*/
void AbstractTooltip::BlendOut()
{
	// Start timeout timer
	if (!m_cTimer.IsActive() && m_nTimeout > 0) {
		m_cTimer.StartOnce(m_nTimeout);
	}
}


//[-------------------------------------------------------]
//[ Protected virtual AbstractTooltip functions           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called on display of the tooltip
*/
void AbstractTooltip::OnShowTooltip()
{
}

/**
*  @brief
*    Called on timeout
*/
void AbstractTooltip::OnTimeout()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the timer has fired
*/
void AbstractTooltip::OnTimer()
{
	// Emit signal
	SignalTimeout();

	// Call virtual function
	OnTimeout();
}


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
