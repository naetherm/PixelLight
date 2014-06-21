/*********************************************************\
 *  File: TrayIconLinux.cpp                              *
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
#include "PLGui/Gui/Gui.h"
#include "PLGui/Gui/TrayIcon.h"
#include "PLGui/Backend/Linux/GuiLinux.h"
#include "PLGui/Backend/Linux/TrayIconLinux.h"
#include "PLGui/Backend/Linux/TrayIconWidgetLinux.h"


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
namespace PLGui {


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
TrayIconLinux::TrayIconLinux(TrayIcon &cTrayIcon) : TrayIconImpl(cTrayIcon),
	m_pDisplay(static_cast<GuiLinux*>(cTrayIcon.GetGui()->GetImpl())->GetDisplay()),
	m_pWidget(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
TrayIconLinux::~TrayIconLinux()
{
}


//[-------------------------------------------------------]
//[ Private virtual TrayIconImpl functions                ]
//[-------------------------------------------------------]
void TrayIconLinux::CreateTrayIcon()
{
	// Create tray icon widget
	m_pWidget = new TrayIconWidgetLinux();
	m_pWidget->SetTrayIcon(m_pTrayIcon);
	m_pWidget->SetIcon(m_pTrayIcon->GetIcon());
	m_pWidget->EmbedIntoSysTray();
}

void TrayIconLinux::DestroyTrayIcon()
{
	// Destroy window
	if (m_pWidget) {
		m_pWidget->Destroy();
		m_pWidget = nullptr;
	}
}

void TrayIconLinux::SetVisible(bool bVisible)
{
	// Tray icon created?
	if (m_pWidget) {
		// Set visibility
		m_pWidget->SetVisible(bVisible);
	}
}

void TrayIconLinux::Update()
{
}

void TrayIconLinux::ShowNotification(const String &sTitle, const String &sText)
{
	// Tray icon created?
	if (m_pWidget) {
		// Show notification
		m_pWidget->ShowNotification(sTitle, sText);
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
