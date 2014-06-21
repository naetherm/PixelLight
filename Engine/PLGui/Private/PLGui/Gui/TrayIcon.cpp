/*********************************************************\
 *  File: TrayIcon.cpp                                   *
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
#include "PLGui/Backend/TrayIconImpl.h"
#include "PLGui/Gui/Gui.h"
#include "PLGui/Gui/TrayIcon.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLGui {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
TrayIcon::TrayIcon(Gui &cGui) :
	m_pGui(&cGui),
	m_pImpl(nullptr),
	m_bVisible(false),
	m_cIcon(*m_pGui)
{
}

/**
*  @brief
*    Destructor
*/
TrayIcon::~TrayIcon()
{
	// Delete tray icon implementation
	if (m_pImpl) {
		// Destroy tray icon
		m_pImpl->DestroyTrayIcon();

		// Delete implementation
		delete m_pImpl;
	}
}

/**
*  @brief
*    Get owner GUI
*/
Gui *TrayIcon::GetGui() const
{
	// Return GUI
	return m_pGui;
}

/**
*  @brief
*    Get implementation
*/
TrayIconImpl *TrayIcon::GetImpl() const
{
	// Return implementation
	return m_pImpl;
}

/**
*  @brief
*    Get visibility
*/
bool TrayIcon::IsVisible() const
{
	// Return visibility-state
	return m_bVisible;
}

/**
*  @brief
*    Set visibility
*/
void TrayIcon::SetVisible(bool bVisible)
{
	// Set visibility-state
	m_bVisible = bVisible;

	// Create implementation?
	if (m_pImpl == nullptr) {
		// We must have a valid GUI
		if (m_pGui) {
			// Create tray icon implementation
			m_pImpl = m_pGui->CreateTrayIconImpl(*this);

			// Create tray icon
			m_pImpl->CreateTrayIcon();
		}
	}

	// Call implementation
	if (m_pImpl) {
		m_pImpl->SetVisible(m_bVisible);
	}
}

/**
*  @brief
*    Get icon
*/
const Image &TrayIcon::GetIcon() const
{
	// Return icon
	return m_cIcon;
}

/**
*  @brief
*    Set icon
*/
void TrayIcon::SetIcon(const Image &cIcon)
{
	// Set icon
	m_cIcon = cIcon;

	// Call implementation
	if (m_pImpl) {
		m_pImpl->Update();
	}
}

/**
*  @brief
*    Get tooltip
*/
String TrayIcon::GetTooltip() const
{
	// Return tooltip
	return m_sTooltip;
}

/**
*  @brief
*    Set tooltip
*/
void TrayIcon::SetTooltip(const String &sTooltip)
{
	// Set tooltip
	m_sTooltip = sTooltip;

	// Call implementation
	if (m_pImpl) {
		m_pImpl->Update();
	}
}

/**
*  @brief
*    Show a notification text
*/
void TrayIcon::ShowNotification(const String &sTitle, const String &sText)
{
	// Call implementation
	if (m_pImpl) {
		m_pImpl->ShowNotification(sTitle, sText);
	}
}


//[-------------------------------------------------------]
//[ Public virtual TrayIcon functions                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the mouse is moved within the tray icon
*/
void TrayIcon::OnMouseMove(const Vector2i &vPos)
{
	// Call event
	EventMouseMove(vPos);
}

/**
*  @brief
*    Called when a mouse button is pressed
*/
void TrayIcon::OnMouseButtonDown(uint32 nButton, const Vector2i &vPos)
{
	// Call event
	EventMouseButtonDown(nButton, vPos);
}

/**
*  @brief
*    Called when a mouse button is released
*/
void TrayIcon::OnMouseButtonUp(uint32 nButton, const Vector2i &vPos)
{
	// Call event
	EventMouseButtonUp(nButton, vPos);
}

/**
*  @brief
*    Called when a mouse button is clicked
*/
void TrayIcon::OnMouseButtonClick(uint32 nButton, const Vector2i &vPos)
{
	// Call event
	EventMouseButtonClick(nButton, vPos);
}

/**
*  @brief
*    Called when a mouse button is double-clicked
*/
void TrayIcon::OnMouseButtonDoubleClick(uint32 nButton, const Vector2i &vPos)
{
	// Call event
	EventMouseButtonDoubleClick(nButton, vPos);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
