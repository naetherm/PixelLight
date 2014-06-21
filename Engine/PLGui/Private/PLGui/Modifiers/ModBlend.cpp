/*********************************************************\
 *  File: ModBlend.cpp                                   *
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
#include <PLGraphics/Color/Color4.h>
#include "PLGui/Gui/Gui.h"
#include "PLGui/Gui/Resources/Timer.h"
#include "PLGui/Widgets/Widget.h"
#include "PLGui/Modifiers/ModBlend.h"


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_PUSH
PL_WARNING_DISABLE(4355) // "'this' : used in base member initializer list"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLGraphics;
namespace PLGui {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(ModBlend)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ModBlend::ModBlend() :
	SlotOnTimer(this),
	m_nTimeout(20),
	m_fTrans(1.0f),
	m_fTransDest(1.0f),
	m_pTimer(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
ModBlend::~ModBlend()
{
	// Delete timer
	if (m_pTimer) delete m_pTimer;
}

/**
*  @brief
*    Get blend animation time
*/
uint64 ModBlend::GetBlendTime() const
{
	// Return timeout
	return m_nTimeout;
}

/**
*  @brief
*    Set blend animation time
*/
void ModBlend::SetBlendTime(uint64 nBlendTime)
{
	// Set timeout
	if (nBlendTime > 0) {
		m_nTimeout = nBlendTime;
	}
}


//[-------------------------------------------------------]
//[ Protected virtual Modifier functions                  ]
//[-------------------------------------------------------]
void ModBlend::OnAttach(Widget &cWidget)
{
	// Create timer
	m_pTimer = new Timer(*cWidget.GetGui());
	m_pTimer->EventFire.Connect(SlotOnTimer);
}

void ModBlend::OnDetach(Widget &cWidget)
{
}


//[-------------------------------------------------------]
//[ Protected virtual WidgetFunctions functions           ]
//[-------------------------------------------------------]
void ModBlend::OnMouseOver(bool bMouseOver)
{
	// Mouse in over widget or any child-widget
	if (bMouseOver) {
		// Blend to full opacity (1.0)
		m_fTransDest = 1.0f;

		// Start blending
		if (!m_pTimer->IsActive())
			m_pTimer->Start(m_nTimeout);
	}

	// Mouse is not over the widget or any child-widget
	else {
		// Blend to half transparent (0.5)
		m_fTransDest = 0.5f;

		// Start blending
		if (!m_pTimer->IsActive())
			m_pTimer->Start(m_nTimeout);
	}
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the timer has fired
*/
void ModBlend::OnTimer()
{
	// Blend
	if (m_fTransDest > m_fTrans) {
		m_fTrans += 0.04f;
		if (m_fTrans >= m_fTransDest) m_fTrans = m_fTransDest;
	} else if (m_fTransDest < m_fTrans) {
		m_fTrans -= 0.04f;
		if (m_fTrans <= m_fTransDest) m_fTrans = m_fTransDest;
	}

	// Target reached?
	if (m_fTrans == m_fTransDest)
		m_pTimer->Stop();

	// Set transparency
	m_pWidget->SetTransparency(AlphaTransparency, Color4(0.0f, 0.0f, 0.0f, m_fTrans));
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP
