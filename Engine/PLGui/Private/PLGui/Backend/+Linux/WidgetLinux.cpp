/*********************************************************\
 *  File: WidgetLinux.cpp                                *
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
#include "PLGui/PLGuiLinuxIncludes.h"
#include "PLGui/Gui/Base/Keys.h"
#include "PLGui/Gui/Base/GuiMessage.h"
#include "PLGui/Gui/Resources/Graphics.h"
#include "PLGui/Gui/Resources/Cursor.h"
#include "PLGui/Gui/Gui.h"
#include "PLGui/Widgets/Widget.h"
#include "PLGui/Backend/Linux/GuiLinux.h"
#include "PLGui/Backend/Linux/WidgetLinux.h"
#include "PLGui/Backend/Linux/GraphicsLinux.h"
#include "PLGui/Backend/Linux/ImageLinux.h"
#include "PLGui/Backend/Linux/CursorLinux.h"
#include "PLGui/Backend/Linux/ToolsLinux.h"
#include "PLGui/PLGuiLinuxIncludes.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLGui {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
WidgetLinux::WidgetLinux(Widget &cWidget) :
	WidgetImpl(cWidget),
	m_pDisplay(static_cast<GuiLinux*>(cWidget.GetGui()->GetImpl())->GetDisplay()),
	m_nScreen(0),
	m_nNativeWindowHandle(0),
	m_bWrapper(false),
	m_bDestroyed(false),
	m_bVisible(false),
	m_bEnabled(true),
	m_bTopmost(false),
	m_bShowInTaskbar(false),
	m_nX(0),
	m_nY(0),
	m_nWidth(0),
	m_nHeight(0),
	m_bFullscreen(false),
	m_bMaximized(false),
	m_bMinimized(false)
{
}

/**
*  @brief
*    Destructor
*/
WidgetLinux::~WidgetLinux()
{
	// If not done already, destroy the widget
	Destroy();
}


//[-------------------------------------------------------]
//[ Public virtual WidgetImpl functions                   ]
//[-------------------------------------------------------]
void WidgetLinux::CreateWidget()
{
	// Setup widget attributes
	XSetWindowAttributes sWindowAttr;
	sWindowAttr.border_pixel = 0;
	sWindowAttr.colormap	 = CopyFromParent;
	sWindowAttr.event_mask	 = PropertyChangeMask | ExposureMask | StructureNotifyMask | FocusChangeMask | KeyPressMask | KeyReleaseMask |
							   PointerMotionMask | EnterWindowMask | LeaveWindowMask | ButtonPressMask | ButtonReleaseMask;

	// Get parent widget
	::Window nParent = (m_pWidget->GetParent() ? m_pWidget->GetParent()->GetNativeWindowHandle() : DefaultRootWindow(m_pDisplay));

	// Initialize position and size
	m_nX	  = 0;
	m_nY	  = 0;
	m_nWidth  = 100;
	m_nHeight = 100;

	// Create widget
	m_nNativeWindowHandle = XCreateWindow(m_pDisplay, nParent, m_nX, m_nY, m_nWidth, m_nHeight, 0, CopyFromParent, InputOutput, CopyFromParent, CWBorderPixel | CWColormap | CWEventMask, &sWindowAttr);
	if (m_nNativeWindowHandle) {
		// Set widget properties
		XSetStandardProperties(
			m_pDisplay,				// Display
			m_nNativeWindowHandle,	// Window
			"",						// Window name
			"",						// Icon name
			XLib::None,				// Icon pixmap
			nullptr,				// argv
			0,						// argc
			nullptr					// Hints
		);

		// Set positioning hints
		XSizeHints *pSize = XAllocSizeHints();
		pSize->flags	  = PPosition | PSize | PMinSize;
		pSize->min_width  = 10;
		pSize->min_height = 10;
		XSetWMNormalHints(m_pDisplay, m_nNativeWindowHandle, pSize);
		XFree(pSize);

		// Set focus hints
		XWMHints sWMHints;
		sWMHints.flags = InputHint;
		sWMHints.input = false;
		XSetWMHints(m_pDisplay, m_nNativeWindowHandle, &sWMHints);

		// Remove window manager decoration
		ToolsLinux::SetNoWindowDecoration(m_pDisplay, m_nNativeWindowHandle, false);

		// Save pointer to virtual widget object
		XSaveContext(m_pDisplay, m_nNativeWindowHandle, 0, reinterpret_cast<char*>(this));

		// Add custom message for 'Close Window'
		XSetWMProtocols(m_pDisplay, m_nNativeWindowHandle, reinterpret_cast<Atom*>(&static_cast<GuiLinux*>(m_pWidget->GetGui()->GetImpl())->m_sClientProtocols), 7);

		// Send OnCreate message
		m_pWidget->GetGui()->SendMessage(GuiMessage::OnCreate(m_pWidget));

		// Send OnSize message
		m_pWidget->GetGui()->SendMessage(GuiMessage::OnSize(m_pWidget, Vector2i(m_nWidth, m_nHeight)));
	} else {
		// Could not create widget
		m_bDestroyed			= true;
		m_nNativeWindowHandle	= 0;
	}
}

void WidgetLinux::CreateWrapperWidget(handle nNativeWindowHandle)
{
	// Mark as wrapper widget
	m_bWrapper = true;

	// Save native window handle
	m_nNativeWindowHandle = static_cast< ::Window>(nNativeWindowHandle);
}

bool WidgetLinux::IsDestroyed() const
{
	// Return destroyed-flag
	return m_bDestroyed;
}

void WidgetLinux::Destroy()
{
	// Check if the widget has already been destroyed
	if (!m_bDestroyed) {
		// Get GUI
		GuiLinux *pGuiLinux = static_cast<GuiLinux*>(m_pWidget->GetGui()->GetImpl());

		// Send destroy event
		XEvent sEvent;
		sEvent.type					= ClientMessage;
		sEvent.xclient.window		= m_nNativeWindowHandle;
		sEvent.xclient.message_type = 0;
		sEvent.xclient.format		= 32;
		sEvent.xclient.data.l[0]	= pGuiLinux->m_sClientProtocols.DestroyWidget;
		sEvent.xclient.data.l[1]	= 0;
		sEvent.xclient.data.l[2]	= 0;
		XSendEvent(m_pDisplay, m_nNativeWindowHandle, False, NoEventMask, &sEvent);

		// Do it!
		XSync(m_pDisplay, False);
	}
}

handle WidgetLinux::GetNativeWindowHandle() const
{
	// Return native window handle
	return static_cast<handle>(m_nNativeWindowHandle);
}

void WidgetLinux::SetParent(WidgetImpl *pParent)
{
	// [TODO] Implement
}

bool WidgetLinux::IsShowInTaskbar() const
{
	// Return taskbar flag
	return m_bShowInTaskbar;
}

void WidgetLinux::SetShowInTaskbar(bool bShowInTaskbar)
{
	// Check if widget has been destroyed
	if (!m_bDestroyed) {
		// Get GUI
		GuiLinux *pGuiLinux = static_cast<GuiLinux*>(m_pWidget->GetGui()->GetImpl());

		// Set or remove taskbar flag
		m_bShowInTaskbar = bShowInTaskbar;
		if (!m_bShowInTaskbar)	AddWMState   (pGuiLinux->m_sAtoms._NET_WM_STATE_SKIP_TASKBAR);
		else					RemoveWMState(pGuiLinux->m_sAtoms._NET_WM_STATE_SKIP_TASKBAR);
	}
}

void WidgetLinux::MoveToScreen(Screen &cScreen)
{
	// [TODO] Implement
}

Vector2i WidgetLinux::GetPos() const
{
	// Return position
	return Vector2i(m_nX, m_nY);
}

void WidgetLinux::SetPos(const Vector2i &vPos)
{
	// Check if widget has been destroyed
	if (!m_bDestroyed) {
		// Set position
		XWindowChanges sChanges;
		int nFlags = CWX | CWY;
		sChanges.x = vPos.x;
		sChanges.y = vPos.y;
		XConfigureWindow(m_pDisplay, m_nNativeWindowHandle, nFlags, &sChanges);

		// Do it!
		XSync(m_pDisplay, False);
	}
}

Vector2i WidgetLinux::GetSize() const
{
	// Return size
	return Vector2i(m_nWidth, m_nHeight);
}

void WidgetLinux::SetSize(const Vector2i &vSize)
{
	// Check if widget has been destroyed
	if (!m_bDestroyed) {
		// Set size
		XWindowChanges sChanges;
		sChanges.width  = vSize.x;
		sChanges.height = vSize.y;
		XConfigureWindow(m_pDisplay, m_nNativeWindowHandle, CWWidth | CWHeight, &sChanges);

		// Do it!
		XSync(m_pDisplay, False);
	}
}

void WidgetLinux::SetZPos(EZPos nZPos, Widget *pWidget)
{
	// [TODO] Implement
	/*
	// Set window stacking order
	XWindowChanges changes;
	changes.stack_mode = bTop ? Above : Below;
	XConfigureWindow(m_pDisplay, m_nNativeWindowHandle, CWStackMode, &changes);
	*/
}

bool WidgetLinux::IsTopmost() const
{
	// Return topmost-state
	return m_bTopmost;
}

void WidgetLinux::SetTopmost(bool bTopmost)
{
	// Check if widget has been destroyed
	if (!m_bDestroyed) {
		// Get GUI
		GuiLinux *pGuiLinux = static_cast<GuiLinux*>(m_pWidget->GetGui()->GetImpl());

		// Set or remove topmost flag
		m_bTopmost = bTopmost;
		if (bTopmost) AddWMState   (pGuiLinux->m_sAtoms._NET_WM_STATE_ABOVE);
		else		  RemoveWMState(pGuiLinux->m_sAtoms._NET_WM_STATE_ABOVE);
	}
}

EWindowState WidgetLinux::GetWindowState() const
{
	// Return window state
		 if (m_bFullscreen)	return StateFullscreen;
	else if (m_bMaximized)	return StateMaximized;
	else if (m_bMinimized)	return StateMinimized;
	else					return StateNormal;
}

void WidgetLinux::SetWindowState(EWindowState nWindowState)
{
	// Check if widget has been destroyed
	if (!m_bDestroyed) {
		// Get GUI
		GuiLinux *pGuiLinux = static_cast<GuiLinux*>(m_pWidget->GetGui()->GetImpl());

		// Set window state
		if (nWindowState == StateFullscreen) {
			// Set fullscreen mode
			AddWMState(pGuiLinux->m_sAtoms._NET_WM_STATE_FULLSCREEN);
		} else if (nWindowState == StateMaximized) {
			// Maximize
			AddWMState(pGuiLinux->m_sAtoms._NET_WM_STATE_MAXIMIZED_HORZ, pGuiLinux->m_sAtoms._NET_WM_STATE_MAXIMIZED_VERT);
		} else if (nWindowState == StateMinimized) {
			// Minimize
			XIconifyWindow(m_pDisplay, m_nNativeWindowHandle, m_nScreen);
		} else {
			// Restore window mode
			RemoveWMState(pGuiLinux->m_sAtoms._NET_WM_STATE_FULLSCREEN);
			RemoveWMState(pGuiLinux->m_sAtoms._NET_WM_STATE_MAXIMIZED_HORZ, pGuiLinux->m_sAtoms._NET_WM_STATE_MAXIMIZED_VERT);
			RemoveWMState(pGuiLinux->m_sAtoms._NET_WM_STATE_HIDDEN);
		}
	}
}

void WidgetLinux::SetTransparency(ETransparency nTransparency, const Color4 &cColor)
{
	// [TODO] Implement
}

bool WidgetLinux::IsVisible() const
{
	// Return visible-flag
	return m_bVisible;
}

void WidgetLinux::SetVisible(bool bVisible)
{
	// Check if widget has been destroyed
	if (!m_bDestroyed) {
		// Show/hide
		if (bVisible) XMapWindow(m_pDisplay, m_nNativeWindowHandle);
		else		  XUnmapWindow(m_pDisplay, m_nNativeWindowHandle);

		// Do it!
		XSync(m_pDisplay, False);

		// Show in taskbar?
		if (!m_bShowInTaskbar)	AddWMState   (static_cast<GuiLinux*>(m_pWidget->GetGui()->GetImpl())->m_sAtoms._NET_WM_STATE_SKIP_TASKBAR);
		else					RemoveWMState(static_cast<GuiLinux*>(m_pWidget->GetGui()->GetImpl())->m_sAtoms._NET_WM_STATE_SKIP_TASKBAR);

		// Save visible-flag
		m_bVisible = bVisible;
	}
}

bool WidgetLinux::IsEnabled() const
{
	// Return enabled-flag
	return m_bEnabled;
}

void WidgetLinux::SetEnabled(bool bEnabled)
{
	// Check if widget has been destroyed
	if (!m_bDestroyed) {
		// Set enabled-flag
		m_bEnabled = bEnabled;
	}
}

void WidgetLinux::Activate()
{
	// Check if widget has been destroyed
	if (!m_bDestroyed) {
		// Make widget visible, bring it to the top and give it the focus
		m_pWidget->SetVisible(true);
		m_pWidget->SetZPos(ZTop);
		m_pWidget->SetFocus();
	}
}

void WidgetLinux::SetFocus()
{
	// Check if widget has been destroyed
	if (!m_bDestroyed) {
		// Set focus to this widget
		XSetInputFocus(m_pDisplay, m_nNativeWindowHandle, RevertToParent, CurrentTime);

		// Do it!
		XSync(m_pDisplay, False);
	}
}

void WidgetLinux::Redraw()
{
	// Check if widget has been destroyed
	if (!m_bDestroyed) {
		// Send expose event
		XEvent sEvent;
		sEvent.type			 = Expose;
		sEvent.xany.window	 = m_nNativeWindowHandle;
		sEvent.xexpose.count = 0;
		XSendEvent(m_pDisplay, m_nNativeWindowHandle, False, 0, &sEvent);

		// Do it!
		XSync(m_pDisplay, False);
	}
}

void WidgetLinux::SetTrapMouse(bool bTrap)
{
	// Check if widget has been destroyed
	if (!m_bDestroyed) {
		// Set or release mouse capture?
		if (bTrap) {
			// Grab mouse
			XGrabPointer(m_pDisplay, m_nNativeWindowHandle, True, 0, GrabModeAsync, GrabModeAsync, m_nNativeWindowHandle, XLib::None, CurrentTime);
		} else {
			// Release mouse grab
			XUngrabPointer(m_pDisplay, CurrentTime);
		}

		// Do it!
		XSync(m_pDisplay, False);
	}
}

void WidgetLinux::SetCaptureMouse(bool bCapture)
{
	// Check if widget has been destroyed
	if (!m_bDestroyed) {
		// Set or release mouse capture?
		if (bCapture) {
			// Grab mouse
			XGrabPointer(m_pDisplay,
						 m_nNativeWindowHandle,
						 False,
						 ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
						 GrabModeAsync,
						 GrabModeAsync,
						 XLib::None,
						 XLib::None,
						 CurrentTime);
		} else {
			// Release mouse grab
			XUngrabPointer(m_pDisplay, CurrentTime);
		}

		// Do it!
		XSync(m_pDisplay, False);
	}
}

uint32 WidgetLinux::RegisterHotkey(uint32 nModKey, uint32 nKey)
{
	// [TODO] Implement
	return 0;
}

void WidgetLinux::UnregisterHotkey(uint32 nID)
{
	// [TODO] Implement
}

void WidgetLinux::SetTitle(const String &sTitle)
{
	// Check if widget has been destroyed
	if (!m_bDestroyed) {
		// Get GUI
		GuiLinux *pGuiLinux = static_cast<GuiLinux*>(m_pWidget->GetGui()->GetImpl());

		// Set window title
		XChangeProperty(m_pDisplay, m_nNativeWindowHandle, pGuiLinux->m_sAtoms.WM_NAME,				 pGuiLinux->m_sAtoms.UTF8_STRING, 8, PropModeReplace, reinterpret_cast<const unsigned char*>(sTitle.GetUTF8()), sTitle.GetLength());
		XChangeProperty(m_pDisplay, m_nNativeWindowHandle, pGuiLinux->m_sAtoms._NET_WM_NAME,		 pGuiLinux->m_sAtoms.UTF8_STRING, 8, PropModeReplace, reinterpret_cast<const unsigned char*>(sTitle.GetUTF8()), sTitle.GetLength());
		XChangeProperty(m_pDisplay, m_nNativeWindowHandle, pGuiLinux->m_sAtoms._NET_WM_VISIBLE_NAME, pGuiLinux->m_sAtoms.UTF8_STRING, 8, PropModeReplace, reinterpret_cast<const unsigned char*>(sTitle.GetUTF8()), sTitle.GetLength());

		// Do it!
		XSync(m_pDisplay, False);
	}
}

void WidgetLinux::SetIcon(const Image &cIcon)
{
	// Check if widget has been destroyed
	if (!m_bDestroyed) {
		// Set icon hint
		XWMHints sWMHints;
		sWMHints.flags = IconPixmapHint | IconMaskHint;
		sWMHints.icon_pixmap = static_cast<ImageLinux*>(cIcon.GetImpl())->GetPixmap();
		sWMHints.icon_mask   = static_cast<ImageLinux*>(cIcon.GetImpl())->GetMaskPixmap();
		XSetWMHints(m_pDisplay, m_nNativeWindowHandle, &sWMHints);

		// Do it!
		XSync(m_pDisplay, False);
	}
}

void WidgetLinux::SetCursor(const Cursor &cCursor)
{
	// Check if widget has been destroyed
	if (!m_bDestroyed) {
		// Set cursor
		XDefineCursor(m_pDisplay, m_nNativeWindowHandle, static_cast<CursorLinux*>(cCursor.GetImpl())->GetXCursor());

		// Do it!
		XSync(m_pDisplay, False);
	}
}

bool WidgetLinux::GetMousePos(Vector2i &vPos)
{
	// Get the absolute mouse cursor position on the screen
	XEvent sXEvent;

	// Get the window directly below the current mouse cursor position - do only continue if this is our window
	if (XQueryPointer(m_pDisplay, m_nNativeWindowHandle, &sXEvent.xbutton.root, &sXEvent.xbutton.window,
		&sXEvent.xbutton.x_root, &sXEvent.xbutton.y_root, &sXEvent.xbutton.x, &sXEvent.xbutton.y, &sXEvent.xbutton.state) == True) {
		// Write out the mouse cursor position inside this window
		vPos.x = sXEvent.xbutton.x;
		vPos.y = sXEvent.xbutton.y;

		// Done, the mouse cursor is currently over this window
		return true;
	} else {
		// Error, the mouse cursor is currently not over this window	
		return false;
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
void WidgetLinux::AddWMState(::Atom sAtom1, ::Atom sAtom2)
{
	// Get GUI
	GuiLinux *pGuiLinux = static_cast<GuiLinux*>(m_pWidget->GetGui()->GetImpl());

	// Send _NET_WM_STATE_ADD event
	XEvent sEvent;
	sEvent.type					= ClientMessage;
	sEvent.xclient.window		= m_nNativeWindowHandle;
	sEvent.xclient.message_type = pGuiLinux->m_sAtoms._NET_WM_STATE;
	sEvent.xclient.format		= 32;
	sEvent.xclient.data.l[0]	= _NET_WM_STATE_ADD;
	sEvent.xclient.data.l[1]	= sAtom1;
	sEvent.xclient.data.l[2]	= sAtom2;
	XSendEvent(m_pDisplay, DefaultRootWindow(m_pDisplay), False, SubstructureNotifyMask, &sEvent);

	// Do it!
	XSync(m_pDisplay, False);
}

void WidgetLinux::RemoveWMState(::Atom sAtom1, ::Atom sAtom2)
{
	// Get GUI
	GuiLinux *pGuiLinux = static_cast<GuiLinux*>(m_pWidget->GetGui()->GetImpl());

	// Send _NET_WM_STATE_REMOVE event
	XEvent sEvent;
	sEvent.type					= ClientMessage;
	sEvent.xclient.window		= m_nNativeWindowHandle;
	sEvent.xclient.message_type = pGuiLinux->m_sAtoms._NET_WM_STATE;
	sEvent.xclient.format		= 32;
	sEvent.xclient.data.l[0]	= _NET_WM_STATE_REMOVE;
	sEvent.xclient.data.l[1]	= sAtom1;
	sEvent.xclient.data.l[2]	= sAtom2;
	XSendEvent(m_pDisplay, DefaultRootWindow(m_pDisplay), False, SubstructureNotifyMask, &sEvent);

	// Do it!
	XSync(m_pDisplay, False);
}

void WidgetLinux::UpdateWMStates()
{
	// Check if widget has been destroyed
	if (!m_bDestroyed) {
		// Reset window states
		m_bFullscreen = false;
		m_bMaximized  = false;
		m_bMinimized  = false;

		// Get GUI
		GuiLinux *pGuiLinux = static_cast<GuiLinux*>(m_pWidget->GetGui()->GetImpl());

		// Get WM_STATE properties
		Atom nType;
		int nFormat;
		unsigned char *pData;
		unsigned long nItemsRead, nItemsLeft;
		int nResult = XGetWindowProperty(m_pDisplay, m_nNativeWindowHandle, pGuiLinux->m_sAtoms._NET_WM_STATE, 0, 8192, False, XA_ATOM, &nType, &nFormat, &nItemsRead, &nItemsLeft, &pData);
		if (nResult == XLib::Success) {
			// Get atoms
			Atom *pAtoms = reinterpret_cast<Atom*>(pData);
			for (int i=0; i<nItemsRead; i++) {
				// Look for state atoms
				if (pAtoms[i] == pGuiLinux->m_sAtoms._NET_WM_STATE_FULLSCREEN)
					m_bFullscreen = true;
				else if (pAtoms[i] == pGuiLinux->m_sAtoms._NET_WM_STATE_MAXIMIZED_HORZ || pAtoms[i] == pGuiLinux->m_sAtoms._NET_WM_STATE_MAXIMIZED_VERT)
					m_bMaximized = true;
				else if (pAtoms[i] == pGuiLinux->m_sAtoms._NET_WM_STATE_HIDDEN)
					m_bMinimized = true;
			}

			// Destroy buffer
			XFree(pData);
		}

		// Update window state
		if (m_bFullscreen)		m_pWidget->GetGui()->SendMessage(GuiMessage::OnWindowState(m_pWidget, StateFullscreen));
		else if (m_bMaximized)	m_pWidget->GetGui()->SendMessage(GuiMessage::OnWindowState(m_pWidget, StateMaximized));
		else if (m_bMinimized)	m_pWidget->GetGui()->SendMessage(GuiMessage::OnWindowState(m_pWidget, StateMinimized));
		else					m_pWidget->GetGui()->SendMessage(GuiMessage::OnWindowState(m_pWidget, StateNormal));
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
