/*********************************************************\
 *  File: ModTimeout.cpp                                 *
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
#include "PLGui/Modifiers/ModTimeout.h"


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
pl_implement_class(ModTimeout)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ModTimeout::ModTimeout() :
	Timeout(this),
	SlotOnTimer(this),
	m_pTimer(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
ModTimeout::~ModTimeout()
{
	// Delete timer
	if (m_pTimer) delete m_pTimer;
}


//[-------------------------------------------------------]
//[ Protected virtual Modifier functions                  ]
//[-------------------------------------------------------]
void ModTimeout::OnAttach(Widget &cWidget)
{
	// Create timer
	m_pTimer = new Timer(*cWidget.GetGui());
	m_pTimer->EventFire.Connect(SlotOnTimer);

	// Start timer, if the window is already visible
	if (cWidget.IsVisible())
		OnShow();
}

void ModTimeout::OnDetach(Widget &cWidget)
{
}


//[-------------------------------------------------------]
//[ Protected virtual WidgetFunctions functions           ]
//[-------------------------------------------------------]
void ModTimeout::OnShow()
{
	// Get and validate timeout (in milliseconds)
	uint64 nTimeout = Timeout.Get();
	if (nTimeout == 0) nTimeout = 100;

	// Start timer
	m_pTimer->StartOnce(nTimeout);
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the timer has fired
*/
void ModTimeout::OnTimer()
{
	// Close window
	m_pWidget->Close();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP
