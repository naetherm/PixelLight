/*********************************************************\
 *  File: WidgetPL.cpp                                   *
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
#include <PLCore/System/MutexGuard.h>
#include <PLGui/Gui/Gui.h>
#include <PLGui/Gui/Screen.h>
#include <PLGui/Gui/Base/GuiMessage.h>
#include <PLGui/Widgets/Widget.h>
#include "PLFrontendPLGui/Compositing/GuiPL.h"
#include "PLFrontendPLGui/Compositing/WidgetPL.h"


/* [TODO] Send messages
		static PLGUI_API GuiMessage OnClose(Widget *pWidget);		*not required*
		static PLGUI_API GuiMessage OnCreate(Widget *pWidget);		*done*
		static PLGUI_API GuiMessage OnDestroy(Widget *pWidget);		*done*
		static PLGUI_API GuiMessage OnShow(Widget *pWidget);		*done*
		static PLGUI_API GuiMessage OnHide(Widget *pWidget);		*done*
		static PLGUI_API GuiMessage OnEnable(Widget *pWidget);		*done*
		static PLGUI_API GuiMessage OnDisable(Widget *pWidget);		*done*
		static PLGUI_API GuiMessage OnGetFocus(Widget *pWidget);	*done*
		static PLGUI_API GuiMessage OnLooseFocus(Widget *pWidget);
		static PLGUI_API GuiMessage OnActivate(Widget *pWidget, bool bActivate);
		static PLGUI_API GuiMessage OnDrawBackground(Widget *pWidget, Graphics *pGraphics);
		static PLGUI_API GuiMessage OnDraw(Widget *pWidget, Graphics *pGraphics);
		static PLGUI_API GuiMessage OnMove(Widget *pWidget, const PLMath::Vector2i &vPos);		*done*
		static PLGUI_API GuiMessage OnSize(Widget *pWidget, const PLMath::Vector2i &vSize);		*done*
		static PLGUI_API GuiMessage OnWindowState(Widget *pWidget, EWindowState nWindowState);
		static PLGUI_API GuiMessage OnEnterFullscreen(Widget *pWidget);
		static PLGUI_API GuiMessage OnLeaveFullscreen(Widget *pWidget);
		static PLGUI_API GuiMessage OnPreferredSize(Widget *pWidget, const PLMath::Vector2i &vSize);
		static PLGUI_API GuiMessage OnAdjustContent(Widget *pWidget);
		static PLGUI_API GuiMessage OnMouseEnter(Widget *pWidget);
		static PLGUI_API GuiMessage OnMouseLeave(Widget *pWidget);
		static PLGUI_API GuiMessage OnMouseOver(Widget *pWidget, bool bMouseOver);
		static PLGUI_API GuiMessage OnMouseMove(Widget *pWidget, const PLMath::Vector2i &vPos);
		static PLGUI_API GuiMessage OnMouseHover(Widget *pWidget);
		static PLGUI_API GuiMessage OnMousePosUpdate(Widget *pWidget, const PLMath::Vector2i &vPos);
		static PLGUI_API GuiMessage OnMouseButtonDown(Widget *pWidget, EMouseButton nButton, const PLMath::Vector2i &vPos);
		static PLGUI_API GuiMessage OnMouseButtonUp(Widget *pWidget, EMouseButton nButton, const PLMath::Vector2i &vPos);
		static PLGUI_API GuiMessage OnMouseButtonClick(Widget *pWidget, EMouseButton nButton, const PLMath::Vector2i &vPos);
		static PLGUI_API GuiMessage OnMouseButtonDoubleClick(Widget *pWidget, EMouseButton nButton, const PLMath::Vector2i &vPos);
		static PLGUI_API GuiMessage OnMouseWheel(Widget *pWidget, int nDelta);
		static PLGUI_API GuiMessage OnKeyDown(Widget *pWidget, PLCore::uint32 nKey, PLCore::uint32 nModifiers);
		static PLGUI_API GuiMessage OnKeyUp(Widget *pWidget, PLCore::uint32 nKey, PLCore::uint32 nModifiers);
		static PLGUI_API GuiMessage OnHotkey(Widget *pWidget, PLCore::uint32 nHotkey);		*not required*
		static PLGUI_API GuiMessage OnDrop(Widget *pWidget, DataObject *pDataObject);
*/


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLGui;
namespace PLFrontendPLGui {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
WidgetPL::WidgetPL(Widget &cWidget) : WidgetImpl(cWidget),
	m_pWidget(&cWidget),
	m_bWrapper(false),
	m_bDestroyed(false),
	m_nWindowState(StateNormal),
	m_bTopmost(false),
	m_bFullscreen(false),
	m_nTransparency(NoTransparency),
	m_cTransparencyColor(Color4::White),
	m_bVisible(false),
	m_bEnabled(true),
	m_bCaptureMouse(false)
{
	// Add widget to list
	if (m_pWidget->GetParent()) {
		// Add the widget into the list of z position sorted child widgets, by default above all already existing widgets
		static_cast<WidgetPL*>(m_pWidget->GetParent()->GetImpl())->m_lstChildren.Add(m_pWidget);
	} else {
		// Add top-level widgets to z position sorted list in Gui implementation, by default above all already existing widget
		static_cast<GuiPL*>(m_pWidget->GetGui()->GetImpl())->AddTopLevelWidget(m_pWidget);
	}
}

/**
*  @brief
*    Destructor
*/
WidgetPL::~WidgetPL()
{
	// If not done already, destroy the widget
	if (!m_bWrapper) {
		// Get the GUI instance
		Gui *pGui = m_pWidget->GetGui();
		if (pGui) {
			// Get the GUI implementation instance
			GuiPL *pGuiPL = static_cast<GuiPL*>(pGui->GetImpl());
			if (pGuiPL) {
				// We REALLY need to check whether there are still some GUI messages within the internal GUI message queue using this widget!

				// Lock the GUI message queue
				const MutexGuard cMutexGuard(reinterpret_cast<Mutex&>(*pGuiPL->m_pMessageQueueCriticalSection));

				// Loop through the queue
				for (uint32 i=0; i<pGuiPL->m_lstMessageQueue.GetNumOfElements(); i++) {
					// Is this GUI message using this widget?
					if (pGuiPL->m_lstMessageQueue[i].GetWidget() == m_pWidget) {
						// Jap, deal with the GUI message!
						pGuiPL->m_lstMessageQueue.RemoveAtIndex(i);
						i--;
					}
				}
			}
		}

		// Destroy the widget
		Destroy();
	}
}


//[-------------------------------------------------------]
//[ Public virtual WidgetImpl functions                   ]
//[-------------------------------------------------------]
void WidgetPL::CreateWidget()
{
	// Send OnCreate message
	m_pWidget->GetGui()->SendMessage(GuiMessage::OnCreate(m_pWidget));
}

void WidgetPL::CreateWrapperWidget(handle nNativeWindowHandle)
{
	// Mark as wrapper widget
	m_bWrapper = true;

	// [TODO] Implement me
}

bool WidgetPL::IsDestroyed() const
{
	// Return destroyed-flag
	return m_bDestroyed;
}

void WidgetPL::Destroy()
{
	// Check if the widget has already been destroyed
	if (!m_bDestroyed) {
		// Get the GUI instance
		Gui *pGui = m_pWidget->GetGui();
		if (pGui) {
			// Get the GUI implementation instance
			GuiPL *pGuiPL = static_cast<GuiPL*>(pGui->GetImpl());

			// If the mouse was over this widget, set the current mouse over widget to a null pointer
			if (pGuiPL && pGuiPL->m_pMouseOver == m_pWidget)
				pGuiPL->m_pMouseOver = nullptr;

			// Send OnDestroy message
			pGui->SendMessage(GuiMessage::OnDestroy(m_pWidget));

			// Mark widget destroyed
			m_bDestroyed = true;

			// Delete child widgets
			List<Widget*> lstChildren = m_pWidget->GetChildren();
			for (uint32 i=0; i<lstChildren.GetNumOfElements(); i++)
				lstChildren[i]->Destroy();

			// Remove from parent widget or GUI top-level list
			if (m_pWidget->GetParent())
				static_cast<WidgetPL*>(m_pWidget->GetParent()->GetImpl())->m_lstChildren.Remove(m_pWidget);
			else {
				if (pGuiPL)
					pGuiPL->m_lstTopLevelWidgets.Remove(m_pWidget);
			}
		}
	}
}

handle WidgetPL::GetNativeWindowHandle() const
{
	// The ingame GUI doesn't have such a native window handle
	return NULL_HANDLE;
}

void WidgetPL::SetParent(WidgetImpl *pParent)
{
	// [TODO] Implement me
}

bool WidgetPL::IsShowInTaskbar() const
{
	// The ingame GUI doesn't have a taskbar
	return false;
}

void WidgetPL::SetShowInTaskbar(bool bShowInTaskbar)
{
	// The ingame GUI doesn't have a taskbar
}

void WidgetPL::MoveToScreen(Screen &cScreen)
{
	// Set position to upper left corner of screen
	m_pWidget->SetPos(cScreen.GetPos());
}

Vector2i WidgetPL::GetPos() const
{
	// Get position
	return m_vPos;
}

void WidgetPL::SetPos(const Vector2i &vPos)
{
	// Set position
	m_vPos = vPos;

	// Send OnMove message
	m_pWidget->GetGui()->SendMessage(GuiMessage::OnMove(m_pWidget, vPos));
}

Vector2i WidgetPL::GetSize() const
{
	// Return size
	return m_vSize;
}

void WidgetPL::SetSize(const Vector2i &vSize)
{
	// Set size
	m_vSize = vSize;

	// Get the GUI instance
	Gui *pGui = m_pWidget->GetGui();

	// Restore from fullscreen mode
	if (m_nWindowState == StateFullscreen) {
		// Send OnLeaveFullscreen message
		pGui->SendMessage(GuiMessage::OnLeaveFullscreen(m_pWidget));
	}

	// Save window state
		 if (m_bFullscreen)	m_nWindowState = StateFullscreen;
	// [TODO] Implement me
/*	else if (wParam == SIZE_MINIMIZED)		m_nWindowState = StateMinimized;
	else if (wParam == SIZE_MAXIMIZED)		m_nWindowState = StateMaximized;
	else if (wParam == SIZE_RESTORED)		m_nWindowState = StateNormal;
*/
	// Send OnWindowState message
	pGui->SendMessage(GuiMessage::OnWindowState(m_pWidget, m_nWindowState));

	// Send OnSize message
	pGui->SendMessage(GuiMessage::OnSize(m_pWidget, m_vSize));
}

void WidgetPL::SetZPos(EZPos nZPos, Widget *pWidget)
{
	// Is this a top-level widget?
	if (m_pWidget->GetParent()) {
		// Set z position
		SetZPos(static_cast<WidgetPL*>(m_pWidget->GetParent()->GetImpl())->m_lstChildren, nZPos, pWidget);

		// Set z position for topmost widgets
		if (m_bTopmost && pWidget && pWidget->GetTopmost())
			SetZPos(static_cast<WidgetPL*>(m_pWidget->GetParent()->GetImpl())->m_lstTopmostChildren, nZPos, pWidget);
	} else {
		// Set z position
		SetZPos(static_cast<GuiPL*>(m_pWidget->GetGui()->GetImpl())->m_lstTopLevelWidgets, nZPos, pWidget);
	}
}

bool WidgetPL::IsTopmost() const
{
	// Return the topmost state
	return m_bTopmost;
}

void WidgetPL::SetTopmost(bool bTopmost)
{
	// State change?
	if (m_bTopmost != bTopmost) {
		// Get the topmost children list of the parent widget
		Array<Widget*> &lstTopmostChildren = static_cast<WidgetPL*>(m_pWidget->GetParent()->GetImpl())->m_lstTopmostChildren;

		// Change to topmost?
		if (m_bTopmost) {
			// Remove the widget from the list
			lstTopmostChildren.Remove(m_pWidget);
		} else {
			// Add the widget from the list, by default above all already existing widgets
			lstTopmostChildren.Add(m_pWidget);
		}

		// Set the topmost state
		m_bTopmost = bTopmost;
	}
}

EWindowState WidgetPL::GetWindowState() const
{
	// Return window state
	return m_nWindowState;
}

void WidgetPL::SetWindowState(EWindowState nWindowState)
{
	// Has something changed?
	if (m_nWindowState != nWindowState) {
		// Save fullscreen-mode
		m_bFullscreen = (nWindowState == StateFullscreen);

		// Change window state
		// [TODO] Implement me

		// Save new window state
		m_nWindowState = nWindowState;
	}
}

void WidgetPL::SetTransparency(ETransparency nTransparency, const Color4 &cColor)
{
	// Set transparency mode and color
	m_nTransparency		 = nTransparency;
	m_cTransparencyColor = cColor;
}

bool WidgetPL::IsVisible() const
{
	// Return widget's visibility
	return m_bVisible;
}

void WidgetPL::SetVisible(bool bVisible)
{
	// Set widget's visibility
	m_bVisible = bVisible;

	// Send OnShow/OnHide message
	if (m_bVisible)	m_pWidget->GetGui()->SendMessage(GuiMessage::OnShow(m_pWidget));
	else			m_pWidget->GetGui()->SendMessage(GuiMessage::OnHide(m_pWidget));
}

bool WidgetPL::IsEnabled() const
{
	// Return widget's state
	return m_bEnabled;
}

void WidgetPL::SetEnabled(bool bEnabled)
{
	// Sets widget's state
	m_bEnabled = bEnabled;

	// Send OnEnable/OnDisable message
	if (m_bEnabled)	m_pWidget->GetGui()->SendMessage(GuiMessage::OnEnable(m_pWidget));
	else			m_pWidget->GetGui()->SendMessage(GuiMessage::OnDisable(m_pWidget));
}

void WidgetPL::Activate()
{
	// Does this widget or any child widget accept the focus?
	if (m_pWidget->CanAcceptFocus()) {
		// Try to set focus to the widget
		m_pWidget->SetFocus();
	}
}

void WidgetPL::SetFocus()
{
	// Do not trust window messages and set focus only, if the widget really want's it
	// [TODO] Can we trust ourself?

	if ((m_pWidget->GetFocusStyle() == AcceptFocus || m_pWidget->GetFocusStyle() == ChildFocusOrSelf) && m_pWidget->IsEnabled()) {
		// Send OnGetFocus message
		m_pWidget->GetGui()->SendMessage(GuiMessage::OnGetFocus(m_pWidget));
	}
}

void WidgetPL::Redraw()
{
	// Not required within the ingame GUI because everything will be drawn when rendering a frame
}

void WidgetPL::SetCaptureMouse(bool bCapture)
{
	m_bCaptureMouse = bCapture;
}

void WidgetPL::SetTrapMouse(bool bTrap)
{
	// [TODO] Implement me
}

uint32 WidgetPL::RegisterHotkey(uint32 nModKey, uint32 nKey)
{
	// Not required within the ingame GUI
	return 0;
}

void WidgetPL::UnregisterHotkey(uint32 nID)
{
	// Not required within the ingame GUI
}

void WidgetPL::SetTitle(const String &sTitle)
{
	// Not required within the ingame GUI, the widget using this implementation is storing this state
}

void WidgetPL::SetIcon(const Image &cIcon)
{
	// Not required within the ingame GUI, the widget using this implementation is storing this state
}

void WidgetPL::SetCursor(const Cursor &cCursor)
{
	// Not required within the ingame GUI, the widget using this implementation is storing this state
}

bool WidgetPL::GetMousePos(PLMath::Vector2i &vPos)
{
	// [TODO]
	return false;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Set Z position
*/
void WidgetPL::SetZPos(Array<Widget*> &lstChildren, EZPos nZPos, Widget *pWidget)
{
	// Set new z position
	switch (nZPos) {
		// Bring a window on top of another window
		case ZAbove:
			// Remove the widget from the parent
			if (lstChildren.Remove(m_pWidget)) {
				// Get the index of the reference widget
				const int nReferenceIndex = lstChildren.GetIndex(pWidget);

				// Add the widget at the correct child list position
				if (nReferenceIndex < 0)
					lstChildren.Add(m_pWidget);	// Error, the given reference widget is not within the list!
				else
					lstChildren.AddAtIndex(m_pWidget, nReferenceIndex);
			}
			break;

		// Bring a window behind another window
		case ZBehind:
			// Remove the widget from the parent
			if (lstChildren.Remove(m_pWidget)) {
				// Get the index of the reference widget
				const int nReferenceIndex = lstChildren.GetIndex(pWidget);

				// Add the widget at the correct child list position
				if (nReferenceIndex < 0)
					lstChildren.Add(m_pWidget);	// Error, the given reference widget is not within the list!
				else
					lstChildren.AddAtIndex(m_pWidget, nReferenceIndex + 1);
			}
			break;

		// Bring a window on top of all other windows
		case ZTop:
			if (lstChildren.Remove(m_pWidget))
				lstChildren.AddAtIndex(m_pWidget, 0);
			break;

		// Bring a window behind all other windows
		case ZBottom:
			if (lstChildren.Remove(m_pWidget))
				lstChildren.Add(m_pWidget);
			break;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendPLGui
