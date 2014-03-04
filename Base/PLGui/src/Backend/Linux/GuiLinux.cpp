/*********************************************************\
 *  File: GuiLinux.cpp                                   *
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
#include <stdio.h>
#include <PLCore/String/RegEx.h>
#include "PLGui/Gui/Base/GuiMessage.h"
#include "PLGui/Gui/Base/Keys.h"
#include "PLGui/Gui/Resources/Graphics.h"
#include "PLGui/Gui/Gui.h"
#include "PLGui/Gui/Screen.h"
#include "PLGui/Widgets/Widget.h"
#include "PLGui/Backend/Linux/GuiLinux.h"
#include "PLGui/Backend/Linux/WidgetLinux.h"
#include "PLGui/Backend/Linux/GraphicsLinux.h"
#include "PLGui/Backend/Linux/ImageLinux.h"
#include "PLGui/Backend/Linux/FontLinux.h"
#include "PLGui/Backend/Linux/CursorLinux.h"
#include "PLGui/Backend/Linux/TrayIconLinux.h"
#include "PLGui/Backend/Linux/TrayIconWidgetLinux.h"
#include "PLGui/Backend/Linux/ClipBoardLinux.h"


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
GuiLinux::GuiLinux(Gui *pGui) : GuiImpl(pGui),
	m_bModifierShift(false),
	m_bModifierCtrl(false),
	m_bModifierAlt(false)
{
	// Yes, we want to access Xlib from multiple threads, so we have to call
	XInitThreads();

	// Set X error handler
	XSetErrorHandler(&GuiLinux::ErrorHandler);

	// Get display
	m_pDisplay = XOpenDisplay(nullptr);

	// Create custom protocol extensions
	m_sClientProtocols.Close				= XInternAtom(m_pDisplay, "WM_DELETE_WINDOW",				False);
	m_sClientProtocols.Focus				= XInternAtom(m_pDisplay, "WM_TAKE_FOCUS",					False);
	m_sClientProtocols.Timer				= XInternAtom(m_pDisplay, "WM_TIMER",						False);
	m_sClientProtocols.Exit					= XInternAtom(m_pDisplay, "WM_EXIT",						False);
	m_sClientProtocols.Wakeup				= XInternAtom(m_pDisplay, "PL_WAKEUP",						False);
	m_sClientProtocols.DestroyWidget		= XInternAtom(m_pDisplay, "PL_DESTROY_WIDGET",				False);
	m_sClientProtocols.ThemeChanged			= XInternAtom(m_pDisplay, "PL_THEME_CHANGED",				False);

	// Create other atoms
	m_sAtoms.UTF8_STRING					= XInternAtom(m_pDisplay, "UTF8_STRING",					False);
	m_sAtoms.WM_NAME						= XInternAtom(m_pDisplay, "WM_NAME",						False);
	m_sAtoms._NET_WM_NAME					= XInternAtom(m_pDisplay, "_NET_WM_NAME",					False);
	m_sAtoms._NET_WM_VISIBLE_NAME			= XInternAtom(m_pDisplay, "_NET_WM_VISIBLE_NAME",			False);
	m_sAtoms._NET_WM_STATE					= XInternAtom(m_pDisplay, "_NET_WM_STATE",					False);
	m_sAtoms._NET_WM_STATE_FULLSCREEN		= XInternAtom(m_pDisplay, "_NET_WM_STATE_FULLSCREEN",		False);
	m_sAtoms._NET_WM_STATE_MAXIMIZED_HORZ	= XInternAtom(m_pDisplay, "_NET_WM_STATE_MAXIMIZED_HORZ",	False);
	m_sAtoms._NET_WM_STATE_MAXIMIZED_VERT	= XInternAtom(m_pDisplay, "_NET_WM_STATE_MAXIMIZED_VERT",	False);
	m_sAtoms._NET_WM_STATE_HIDDEN			= XInternAtom(m_pDisplay, "_NET_WM_STATE_HIDDEN",			False);
	m_sAtoms._NET_WM_STATE_SKIP_TASKBAR		= XInternAtom(m_pDisplay, "_NET_WM_STATE_SKIP_TASKBAR",		False);
	m_sAtoms._NET_WM_STATE_ABOVE			= XInternAtom(m_pDisplay, "_NET_WM_STATE_ABOVE",			False);
	m_sAtoms._XEMBED						= XInternAtom(m_pDisplay, "_XEMBED",						False);
	m_sAtoms._XEMBED_INFO					= XInternAtom(m_pDisplay, "_XEMBED_INFO",					False);
	m_sAtoms._NET_SYSTEM_TRAY_OPCODE		= XInternAtom(m_pDisplay, "_NET_SYSTEM_TRAY_OPCODE",		False);
	m_sAtoms._NET_SYSTEM_TRAY_MESSAGE_DATA	= XInternAtom(m_pDisplay, "_NET_SYSTEM_TRAY_MESSAGE_DATA",	False);

	// Create dummy window for internal messages
	XSetWindowAttributes sAttribs;
	m_nHiddenWindow = XCreateWindow(
						m_pDisplay,
						DefaultRootWindow(m_pDisplay),
						-100, -100,
						1, 1,
						0, CopyFromParent, InputOutput, CopyFromParent,
						0, &sAttribs);
	XSetWMProtocols(m_pDisplay, m_nHiddenWindow, reinterpret_cast<Atom*>(&m_sClientProtocols), 7);
}

/**
*  @brief
*    Destructor
*/
GuiLinux::~GuiLinux()
{
	// Destroy hidden window
	XDestroyWindow(m_pDisplay, m_nHiddenWindow);

	// Close display
	XCloseDisplay(m_pDisplay);
}

/**
*  @brief
*    Get display
*/
Display *GuiLinux::GetDisplay() const
{
	// Return display
	return m_pDisplay;
}

/**
*  @brief
*    Keep track of modifier keys (shift, control and alt)
*/
void GuiLinux::TrackModifierKeys(int nKey, bool bPressed)
{
	// Save state of modifier key
	if (nKey == XK_Shift_L   || nKey == XK_Shift_R)   m_bModifierShift = bPressed;
	if (nKey == XK_Control_L || nKey == XK_Control_R) m_bModifierCtrl  = bPressed;
	if (nKey == XK_Alt_L     || nKey == XK_Alt_R)     m_bModifierAlt   = bPressed;
}

/**
*  @brief
*    Get current state of modifier keys (shift, control and alt)
*/
uint32 GuiLinux::GetModifierKeys()
{
	// Return bitmask of what modifiers are pressed
	uint32 nModifiers = 0;
	if (m_bModifierShift) nModifiers |= PLGUIMOD_SHIFT;
	if (m_bModifierCtrl)  nModifiers |= PLGUIMOD_CONTROL;
	if (m_bModifierAlt)   nModifiers |= PLGUIMOD_ALT;
	return nModifiers;
}


//[-------------------------------------------------------]
//[ Public virtual GuiImpl functions                      ]
//[-------------------------------------------------------]
bool GuiLinux::HasPendingMessages()
{
	// Check for messages
	return (XPending(m_pDisplay) > 0);
}

void GuiLinux::ProcessMessage()
{
	// Wait and get message
	XEvent sEvent;
	XNextEvent(m_pDisplay, &sEvent);

	// Process event
	ProcessXEvent(&sEvent);
}

void GuiLinux::PostMessage(const GuiMessage &cMessage)
{
	// [sbusch] ToDo

	// Get widget
//	Widget *pWidget = cMessage.GetWidget();
//	::Window nNativeWindowHandle = (pWidget ? static_cast< ::Window>(pWidget->GetNativeWindowHandle()) : NULL_HANDLE);

	// Post message
	switch (cMessage.GetType()) {
		case MessageOnExit:					/**< Exit message */
		{
			// Create client message
			XEvent xEvent;
			xEvent.type	= ClientMessage;
			xEvent.xclient.type			= ClientMessage;
			xEvent.xclient.serial		= 0;
			xEvent.xclient.send_event	= True;
			xEvent.xclient.display		= m_pDisplay;
			xEvent.xclient.window		= XLib::None;
			xEvent.xclient.message_type	= 0;
			xEvent.xclient.format		= 32;
			xEvent.xclient.data.l[0]	= m_sClientProtocols.Exit;
			xEvent.xclient.data.l[1]	= 0;
			xEvent.xclient.data.l[2]	= 0;

			// Send message
			XLockDisplay(m_pDisplay);
			XSendEvent(m_pDisplay, m_nHiddenWindow, False, NoEventMask, &xEvent);
			XSync(m_pDisplay, False);
			XUnlockDisplay(m_pDisplay);
			break;
		}

		case MessageOnTimer:					/**< Timer message */
		{
			// Create client message
			XEvent xEvent;
			xEvent.type	= ClientMessage;
			xEvent.xclient.type			= ClientMessage;
			xEvent.xclient.serial		= 0;
			xEvent.xclient.send_event	= True;
			xEvent.xclient.display		= m_pDisplay;
			xEvent.xclient.window		= XLib::None;
			xEvent.xclient.message_type	= 0;
			xEvent.xclient.format		= 32;
			xEvent.xclient.data.l[0]	= m_sClientProtocols.Timer;
			xEvent.xclient.data.l[1]	= reinterpret_cast<long int>(static_cast<void*>(cMessage.GetTimer()));
			xEvent.xclient.data.l[2]	= 0;

			// Send message
			XLockDisplay(m_pDisplay);
			XSendEvent(m_pDisplay, m_nHiddenWindow, False, NoEventMask, &xEvent);
			XSync(m_pDisplay, False);
			XUnlockDisplay(m_pDisplay);
			break;
		}

		case MessageOnWakeup:					/**< Wakeup message loop */
		{
			// Create client message
			XEvent xEvent;
			xEvent.type	= ClientMessage;
			xEvent.xclient.type			= ClientMessage;
			xEvent.xclient.serial		= 0;
			xEvent.xclient.send_event	= True;
			xEvent.xclient.display		= m_pDisplay;
			xEvent.xclient.window		= XLib::None;
			xEvent.xclient.message_type	= 0;
			xEvent.xclient.format		= 32;
			xEvent.xclient.data.l[0]	= m_sClientProtocols.Wakeup;
			xEvent.xclient.data.l[1]	= 0;
			xEvent.xclient.data.l[2]	= 0;

			// Send message
			XLockDisplay(m_pDisplay);
			XSendEvent(m_pDisplay, m_nHiddenWindow, False, NoEventMask, &xEvent);
			XSync(m_pDisplay, False);
			XUnlockDisplay(m_pDisplay);
			break;
		}

		case MessageOnUserMessage:				/**< User message */
			// [TODO]
			break;

		case MessageOnInternalMessage:			/**< Internal message */
			// [TODO]
			break;

		case MessageOnThemeChanged:				/**< Theme has been changed */
			// [TODO]
			break;

		case MessageOnUpdateContent:			/**< Widget content has been changed */
			// [TODO]
			break;

		case MessageOnUpdateChildWidget:		/**< Child widget has been changed */
			// [TODO]
			break;

		case MessageOnAddChildWidget:			/**< Child widget has been added */
			// [TODO]
			break;

		case MessageOnRemoveChildWidget:		/**< Child widget has been removed */
			// [TODO]
			break;

		case MessageOnClose:					/**< Widget shall be closed (ALT+F4 or X-Button pressed) */
			// [TODO]
			break;

		case MessageOnCreate:					/**< Widget has just been created */
			// [TODO]
			break;

		case MessageOnDestroy:					/**< Widget is going to be destroyed */
			// [TODO]
			break;

		case MessageOnShow:						/**< Widget gets shown */
			// [TODO]
			break;

		case MessageOnHide:						/**< Widget gets hidden */
			// [TODO]
			break;

		case MessageOnEnable:					/**< Widget gets enabled */
			// [TODO]
			break;

		case MessageOnDisable:					/**< Widget gets disabled */
			// [TODO]
			break;

		case MessageOnGetFocus:					/**< Widget gets focus */
			// [TODO]
			break;

		case MessageOnLooseFocus:				/**< Widget looses focus */
			// [TODO]
			break;

		case MessageOnDrawBackground:			/**< Widget background gets drawn, graphics object used for painting as parameter */
			// [TODO]
			break;

		case MessageOnDraw:						/**< Widget gets drawn, graphics object used for painting as parameter */
			// [TODO]
			break;

		case MessageOnMove:						/**< Widget gets moved, new widget position as parameter */
			// [TODO]
			break;

		case MessageOnSize:						/**< Widget gets resized, new widget size as parameter */
			// [TODO]
			break;

		case MessageOnWindowState	:			/**< Window state has changed, new window state as parameter */
			// [TODO]
			break;

		case MessageOnEnterFullscreen:			/**< Widget enters fullscreen mode */
			// [TODO]
			break;

		case MessageOnLeaveFullscreen:			/**< Widget leaves fullscreen mode */
			// [TODO]
			break;

		case MessageOnPreferredSize:			/**< Widget has calculated it's preferred size */
			// [TODO]
			break;

		case MessageOnAdjustContent:			/**< Widget content has to be adjusted */
			// [TODO]
			break;

		case MessageOnMouseEnter:				/**< Mouse enters widget */
			// [TODO]
			break;

		case MessageOnMouseLeave:				/**< Mouse leaves widget */
			// [TODO]
			break;

		case MessageOnMouseOver:				/**< Mouse-over widget has changed */
			// [TODO]
			break;

		case MessageOnMouseMove:				/**< Mouse moves inside the widget, mouse position within the widget as parameter */
			// [TODO]
			break;

		case MessageOnMouseHover:				/**< Mouse hovers over the widget */
			// [TODO]
			break;

		case MessageOnMousePosUpdate:			/**< Mouse position inside the widget has changed due to the movement of widget */
			// [TODO]
			break;

		case MessageOnMouseButtonDown:			/**< Mouse button is pressed, mouse button and mouse position within the widget as parameters */
			// [TODO]
			break;

		case MessageOnMouseButtonUp:			/**< Mouse button is released, mouse button and mouse position within the widget as parameters */
			// [TODO]
			break;

		case MessageOnMouseButtonClick:			/**< Mouse button has been clicked, mouse button and mouse position within the widget as parameters */
			// [TODO]
			break;

		case MessageOnMouseButtonDoubleClick:	/**< Mouse button has been double-clicked, mouse button and mouse position within the widget as parameters */
			// [TODO]
			break;

		case MessageOnMouseWheel:				/**< Mouse wheel moved, mouse wheel movement as parameter */
			// [TODO]
			break;

		case MessageOnKeyDown:					/**< Key gets pressed, pressed key and modifier keys pressed as parameters */
			// [TODO]
			break;

		case MessageOnKeyUp:					/**< Key gets released, released key and modifier keys pressed as parameters */
			// [TODO]
			break;

		case MessageOnHotkey:					/**< Hotkey pressed, hotkey ID as parameter */
			// [TODO]
			break;

		case MessageOnDrop:						/**< Data has been dropped onto the widget */
			// [TODO]
			break;

		case MessageOnUnknown:
		case MessageOnActivate:
			// [TODO] Check me: Do we need to handle those cases?
			break;
	}
}

void GuiLinux::EnumerateScreens(List<Screen*> &lstScreens)
{
	// Get display name
	String sDisplay = DisplayString(m_pDisplay);

	// Initialize display components
	String sServer = "";
	int    nNumber = 0;
//	int    nScreen = 0;	// Currently not used

	// Get display components (e.g.: ":0.0")
	RegEx cDisplayRegEx("^(\\w*):(\\d+)\\.(\\d+)$");
	if (cDisplayRegEx.Match(sDisplay)) {
		sServer = cDisplayRegEx.GetResult(0);
		nNumber = cDisplayRegEx.GetResult(1).GetInt();
//		nScreen = cDisplayRegEx.GetResult(2).GetInt();	// Currently not used
	}

	// Get screens of display
	uint32 nScreens = ScreenCount(m_pDisplay);
	for (uint32 i=0; i<nScreens; i++) {
		// Get screen info
		::Screen *pScreenInfo = XScreenOfDisplay(m_pDisplay, i);

		// Add screen
		Screen *pScreen = new Screen(m_pGui);
		pScreen->SetName   (sServer + ':' + nNumber + "." + i);
		pScreen->SetPos	   (Vector2i(0, 0));
		pScreen->SetSize   (Vector2i(XWidthOfScreen(pScreenInfo), XHeightOfScreen(pScreenInfo)));
		pScreen->SetDefault(i == XDefaultScreen(m_pDisplay));
		lstScreens.Add(pScreen);
	}
}

Vector2i GuiLinux::GetScreenSize() const
{
	// Return screen size of default screen
	return Vector2i(
		::XDisplayWidth (m_pDisplay, XDefaultScreen(m_pDisplay)),
		::XDisplayHeight(m_pDisplay, XDefaultScreen(m_pDisplay))
	);
}

bool GuiLinux::HasTaskbar()
{
	// Let's assume that Linux usually has a task bar ...
	return true;
}

void GuiLinux::SetMouseVisible(bool bVisible)
{
	// [TODO] implement
}

void GuiLinux::ListFonts(List<FontInfo> &lstFonts) const
{
	// Get fonts
	char **ppFonts;
	int    nFonts;
	ppFonts = XListFonts(m_pDisplay, "*", 1000, &nFonts);

	// Add fonts to list
	for (int i=0; i<nFonts; i++) {
		// Get font
		String sFont = ppFonts[i];
		RegEx cFontRegEx("\\-[^\\-]*\\-([^\\-]*)\\-([^\\-]*)\\-([^\\-]*)\\-[^\\-]*\\-[^\\-]*\\-([^\\-]*)\\-[^\\-]*\\-[^\\-]*\\-[^\\-]*\\-[^\\-]*\\-[^\\-]*\\-[^\\-]*\\-[^\\-]*");
		if (cFontRegEx.Match(sFont)) {
			// Get font data
			String sFamily	  = cFontRegEx.GetResult(0);
			String sWeight	  = cFontRegEx.GetResult(1);
			String sSlant	  = cFontRegEx.GetResult(2);
			String sPixelSize = cFontRegEx.GetResult(3);

			// Create font info
			FontInfo cFontInfo(
				"",
				sFamily,
				sPixelSize.GetInt(),
				(sWeight.GetInt() >= WeightBold ? WeightBold : (sWeight.GetInt() >= WeightNormal ? WeightNormal : WeightLight)),
				(sSlant == "i" ? StyleItalics : StyleNormal) );

			// Add font (avoid duplicates)
			if (!lstFonts.IsElement(cFontInfo))
				lstFonts.Add(cFontInfo);
		}
	}


	// Destroy font array
	XFreeFontNames(ppFonts);
}

WidgetImpl *GuiLinux::CreateWidgetImpl(Widget &cWidget) const
{
	// Create a Linux widget
	return new WidgetLinux(cWidget);
}

GraphicsImpl *GuiLinux::CreateGraphicsImpl(Graphics &cGraphics) const
{
	// Create a Linux graphics object
	return new GraphicsLinux(cGraphics);
}

ImageImpl *GuiLinux::CreateImageImpl(Image &cImage) const
{
	// Create a Linux image object
	return new ImageLinux(cImage);
}

FontImpl *GuiLinux::CreateFontImpl(Font &cFont) const
{
	// Create a Linux font object
	return new FontLinux(cFont);
}

CursorImpl *GuiLinux::CreateCursorImpl(Cursor &cCursor) const
{
	// Create a Linux cursor object
	return new CursorLinux(cCursor);
}

TrayIconImpl *GuiLinux::CreateTrayIconImpl(TrayIcon &cTrayIcon) const
{
	// Create a Linux tray-icon object
	return new TrayIconLinux(cTrayIcon);
}

ClipBoardImpl *GuiLinux::CreateClipBoardImpl(ClipBoard &cClipBoard) const
{
	// Create a Linux clipboard object
	return new ClipBoardLinux(cClipBoard);
}


//[-------------------------------------------------------]
//[ Private static functions                              ]
//[-------------------------------------------------------]
/**
*  @brief
*    X error handler
*/
int GuiLinux::ErrorHandler(Display *pDisplay, XErrorEvent *pError)
{
	// Get error description
	char szError[256];
	XGetErrorText(pDisplay, pError->error_code, szError, 255);
	printf("X Error: %s\n", szError);
	printf("  Serial number: %lu\n", pError->serial);
	printf("  Major op-code: %d\n", pError->request_code);
	printf("  Minor op-code: %d\n", pError->minor_code);
	printf("\n");
	return 0;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    X event procedure
*/
void GuiLinux::ProcessXEvent(XEvent *pEvent)
{
	// Get pointers to widget and GUI
	WidgetLinux *pWidgetLinux = nullptr;
	XPointer pData = nullptr;
	if (XFindContext(m_pDisplay, pEvent->xany.window, 0, &pData) == 0 && pData) {
		pWidgetLinux = reinterpret_cast<WidgetLinux*>(pData);
	}
	Widget	 *pWidget	= (pWidgetLinux	? pWidgetLinux->m_pWidget : nullptr);
	Gui		 *pGui		= m_pGui;
	GuiLinux *pGuiLinux	= this;

	// Process non-widget events
	switch (pEvent->type)
	{
		// Custom messages
		case ClientMessage:
		{
			// Exit event
			if (static_cast<Atom>(pEvent->xclient.data.l[0]) == m_sClientProtocols.Exit) {
				// Send OnExit message
				m_pGui->SendMessage(GuiMessage::OnExit());
			}

			// Timer event
			if (static_cast<Atom>(pEvent->xclient.data.l[0]) == m_sClientProtocols.Timer) {
				// Get timer
				Timer *pTimer = reinterpret_cast<Timer*>(pEvent->xclient.data.l[1]);
				if (pTimer) {
					// Send OnTimer message
					m_pGui->SendMessage(GuiMessage::OnTimer(pTimer));
				}
			}

			// XEMBED protocol (for embedding windows, like e.g. tray icons)
			if (pEvent->xclient.message_type == m_sAtoms._XEMBED) {
				// Get tray icon window
				TrayIconWidgetLinux *pTrayIcon = static_cast<TrayIconWidgetLinux*>(pWidget);

				// Inform about XEMBED message
				if (pEvent->xclient.data.l[1] == XEMBED_EMBEDDED_NOTIFY) {
					// Window has been embedded
					pTrayIcon->OnXEmbedEmbeddedNotify(static_cast< ::Window>(pEvent->xclient.data.l[3]), pEvent->xclient.data.l[4]);
				} else if (pEvent->xclient.data.l[1] == XEMBED_WINDOW_ACTIVATE) {
					// Window has been activated
					pTrayIcon->OnXEmbedWindowActivate();
				} else if (pEvent->xclient.data.l[1] == XEMBED_WINDOW_DEACTIVATE) {
					// Window has been de-activated
					pTrayIcon->OnXEmbedWindowDeactivate();
				} else if (pEvent->xclient.data.l[1] == XEMBED_FOCUS_IN) {
					// Window has got the focus
					pTrayIcon->OnXEmbedFocusIn(pEvent->xclient.data.l[2]);
				} else if (pEvent->xclient.data.l[1] == XEMBED_FOCUS_OUT) {
					// Window has lost the focus
					pTrayIcon->OnXEmbedFocusOut();
				} else if (pEvent->xclient.data.l[1] == XEMBED_MODALITY_ON) {
					// Window becomes shadowed by a modal dialog
					pTrayIcon->OnXEmbedModalityOn();
				} else if (pEvent->xclient.data.l[1] == XEMBED_MODALITY_OFF) {
					// Window has been released from a modal dialog
					pTrayIcon->OnXEmbedModalityOff();
				} else if (pEvent->xclient.data.l[1] == XEMBED_ACTIVATE_ACCELERATOR) {
					// An accelerator has been activated
					pTrayIcon->OnXEmbedActivateAccelerator(pEvent->xclient.data.l[2], pEvent->xclient.data.l[3]);
				}
			}
			break;
		}
	}

	// We need a widget now
	if (pWidgetLinux) {
		// Process widget-events
		switch (pEvent->type)
		{
			// Custom messages
			case ClientMessage:
			{
				// Destroy widget event
				if (static_cast<Atom>(pEvent->xclient.data.l[0]) == m_sClientProtocols.DestroyWidget) {
					// Destroy the widget and all of it's client widgets
					DestroyWidget(pWidget);
				}

				// Close event
				if (static_cast<Atom>(pEvent->xclient.data.l[0]) == m_sClientProtocols.Close) {
					// Send message
					pGui->SendMessage(GuiMessage::OnClose(pWidget));
				}

				// Focus event
				if (static_cast<Atom>(pEvent->xclient.data.l[0]) == m_sClientProtocols.Focus) {
					// Try to set focus to this widget
					// [sbusch] ToDo
	//				pWidget->SetFocus();
				}

				// Theme has changed
				if (static_cast<Atom>(pEvent->xclient.data.l[0]) == m_sClientProtocols.ThemeChanged) {
					// Send message
					pGui->SendMessage(GuiMessage::OnThemeChanged(pWidget));
				}

				break;
			}
			// Create widget
				// There is no such event in X (CreateNotify is only sent when creating child windows).
				// So, OnCreate() is sent directly to the Gui from WidgetLinux::CreateRegularWidget().

			// Destroy widget
			case DestroyNotify:
				// Mark widget destroyed
				pWidgetLinux->m_bDestroyed			= true;
				pWidgetLinux->m_nNativeWindowHandle	= 0;

				// Send OnDestroy message
				pGui->SendMessage(GuiMessage::OnDestroy(pWidget));
				break;

			/* [sbusch] --> everything above is ok, everything beyond is not yet reviewed <-- */

			// Draw widget
			case Expose:
				// Redraw only if no more expose events will follow
				if (pEvent->xexpose.count == 0) {
					// Check if the widget has already been destroyed
					if (!pWidgetLinux->m_bDestroyed) {
						// Create Linux graphics object
						GraphicsLinux cGraphicsLinux(pWidgetLinux->m_pDisplay, pWidgetLinux->m_nScreen, pEvent->xexpose.window);
						Graphics cGraphics(*pGui, cGraphicsLinux);

						// Send OnDrawBackground message
						pGui->SendMessage(GuiMessage::OnDrawBackground(pWidget, &cGraphics));

						// Send OnDraw message
						pGui->SendMessage(GuiMessage::OnDraw(pWidget, &cGraphics));

						// Sync draw calls
						XSync(m_pDisplay, False);
					}
				}
				break;

			// Show widget
			case MapNotify:
				pGui->SendMessage(GuiMessage::OnShow(pWidget));
				break;

			// Hide widget
			case UnmapNotify:
				pGui->SendMessage(GuiMessage::OnHide(pWidget));
				break;

			// Window configuration changed (position or size)
			case ConfigureNotify:
			{
				// Get position and size
				int nX = pEvent->xconfigure.x;
				int nY = pEvent->xconfigure.y;
				int nW = pEvent->xconfigure.width;
				int nH = pEvent->xconfigure.height;

				// [HACK] Seems like the window manager sends (x=0, y=0) (w, h) for resize events, but I couldn't find
				//        a way to determine whether the x/y values are valid or not. Therefore, we consider x/y values
				//        to be false and *ignore* them, whenever the size has changed.

				// Size changed
				if ((pWidgetLinux->m_nWidth != nW) || (pWidgetLinux->m_nHeight != nH))
				{
					// Save new size
					pWidgetLinux->m_nWidth  = nW;
					pWidgetLinux->m_nHeight = nH;

					// Send message
					pGui->SendMessage(GuiMessage::OnSize(pWidget, Vector2i(pWidgetLinux->m_nWidth, pWidgetLinux->m_nHeight)));
				}

				// Position changed
				else if ((pWidgetLinux->m_nX != nX) || (pWidgetLinux->m_nY != nY))
				{
					// Save new position
					pWidgetLinux->m_nX = nX;
					pWidgetLinux->m_nY = nY;

					// Send message
					pGui->SendMessage(GuiMessage::OnMove(pWidget, Vector2i(pWidgetLinux->m_nX, pWidgetLinux->m_nY)));

					// Inform about new mouse position
					Window nRoot, nNativeWindowHandle;
					int nRootX, nRootY, nWindowX, nWindowY;
					unsigned int nMask;
					if (XQueryPointer(pWidgetLinux->m_pDisplay, pWidgetLinux->m_nNativeWindowHandle, &nRoot, &nNativeWindowHandle, &nRootX, &nRootY, &nWindowX, &nWindowY, &nMask)) {
						// Get mouse position
						if (nWindowX >= 32768) nWindowX -= 65536;
						if (nWindowY >= 32768) nWindowY -= 65536;

						// Send message
						pGui->SendMessage(GuiMessage::OnMousePosUpdate(pWidget, Vector2i(nWindowX, nWindowY)));
					}
				}
				break;
			}

			// Window property changed
			case PropertyNotify:
				// WM_STATE
				if (pEvent->xproperty.atom == m_sAtoms._NET_WM_STATE) {
					// Update window states
					pWidgetLinux->UpdateWMStates();
				}
				break;

			// Mouse entered the widget area
			case EnterNotify:
				// Process this message only if the widget is not disabled
				if (pWidgetLinux->m_bEnabled) {
					// Send OnMouseEnter message
					pGui->SendMessage(GuiMessage::OnMouseEnter(pWidget));
				}
				break;

			// Mouse left the widget area
			case LeaveNotify:
				// Process this message only if the widget is not disabled
				if (pWidgetLinux->m_bEnabled) {
					// Send OnMouseLeave message
					pGui->SendMessage(GuiMessage::OnMouseLeave(pWidget));
				}
				break;

			// Mouse moved
			case MotionNotify:
			{
				// Process this message only if the widget is not disabled
				if (pWidgetLinux->m_bEnabled) {
					// Get mouse position
					int x = pEvent->xmotion.x;
					if (x >= 32768) x -= 65536;
					int y = pEvent->xmotion.y;
					if (y >= 32768) y -= 65536;

					// Send OnMouseMove message
					pGui->SendMessage(GuiMessage::OnMouseMove(pWidget, Vector2i(x, y)));
				}
				break;
			}

			// Mouse button pressed
			case ButtonPress:
				// Process this message only if the widget is not disabled
				if (pWidgetLinux->m_bEnabled) {
					// Try to set focus to this widget
					pWidget->SetFocus();

					// Left mouse button
					if (pEvent->xbutton.button == XLib::Button1)
						pGui->SendMessage(GuiMessage::OnMouseButtonDown(pWidget, LeftButton, Vector2i(pEvent->xbutton.x, pEvent->xbutton.y)));

					// Right mouse button
					if (pEvent->xbutton.button == XLib::Button3)
						pGui->SendMessage(GuiMessage::OnMouseButtonDown(pWidget, RightButton, Vector2i(pEvent->xbutton.x, pEvent->xbutton.y)));
				}
				break;

			// Mouse button released
			case ButtonRelease:
				// Process this message only if the widget is not disabled
				if (pWidgetLinux->m_bEnabled) {
					// Get mouse button
					EMouseButton nButton = static_cast<EMouseButton>(static_cast<uint32>(Button0) + (pEvent->xbutton.button - XLib::Button1));

					// Send OnMouseButtonUp message
					pGui->SendMessage(GuiMessage::OnMouseButtonUp(pWidget, nButton, Vector2i(pEvent->xbutton.x, pEvent->xbutton.y)));

					// [TODO] When to send a click-event, and when not?
					// Send OnMouseButtonClick message
					pGui->SendMessage(GuiMessage::OnMouseButtonClick(pWidget, nButton, Vector2i(pEvent->xbutton.x, pEvent->xbutton.y)));

					// Mouse wheel
					if (pEvent->xbutton.button == XLib::Button4)		// Up
						pGui->SendMessage(GuiMessage::OnMouseWheel(pWidget, -3));
					if (pEvent->xbutton.button == XLib::Button5)		// Down
						pGui->SendMessage(GuiMessage::OnMouseWheel(pWidget, +3));
				}
				break;

			// Widget got focus
			case FocusIn:
			{
				// Process this message only if the widget is not disabled
				if (pWidgetLinux->m_bEnabled) {
					// Do not trust window messages and set focus only, if the widget really want's it
					if ((pWidget->GetFocusStyle() == AcceptFocus || pWidget->GetFocusStyle() == ChildFocusOrSelf) && pWidget->IsEnabled()) {
						// Send OnGetFocus message
						pGui->SendMessage(GuiMessage::OnGetFocus(pWidget));
					}
				} else {
					// Revoke focus
					// [sbusch] ToDo
				}

				/*
				// [sbusch] ToDo
				// Emit focus event
				if (pEvent->xfocus.mode == NotifyNormal) m_pWidget->OnGetFocus();
				*/
				break;
			}

			// Widget lost focus
			case FocusOut:
			{
				// Process this message only if the widget is not disabled
				if (pWidgetLinux->m_bEnabled) {
					// Send OnLooseFocus message
					pGui->SendMessage(GuiMessage::OnLooseFocus(pWidget));
				}
				/*
				// [sbusch] ToDo
				// Emit loose-focus event
				if (pEvent->xfocus.mode == NotifyNormal) m_pWidget->OnLooseFocus();
				*/
				break;
			}

			// Key pressed
			case KeyPress:
			{
				// Process this message only if the widget is not disabled
				if (pWidgetLinux->m_bEnabled) {
					// Get key
					KeySym nKey = XLookupKeysym(&pEvent->xkey, 0);

					// Keep track of modifier keys
					pGuiLinux->TrackModifierKeys(nKey, true);

					// Get state of modifier keys
					uint32 nModifiers = pGuiLinux->GetModifierKeys();

					// Send message
					pGui->SendMessage(GuiMessage::OnKeyDown(pWidget, static_cast<uint32>(nKey), nModifiers));
				}
				break;
			}

			// Key released
			case KeyRelease:
			{
				// Process this message only if the widget is not disabled
				if (pWidgetLinux->m_bEnabled) {
					// Get key
					KeySym nKey = XLookupKeysym(&pEvent->xkey, 0);

					// Keep track of modifier keys
					pGuiLinux->TrackModifierKeys(nKey, false);

					// Get state of modifier keys
					uint32 nModifiers = pGuiLinux->GetModifierKeys();

					// Send message
					pGui->SendMessage(GuiMessage::OnKeyUp(pWidget, static_cast<uint32>(nKey), nModifiers));
				}
				break;
			}
		}
	}
}

/**
*  @brief
*    Destroy a widget and it's child widgets
*/
void GuiLinux::DestroyWidget(Widget *pWidget)
{
	// Destroy child widgets first
	for (uint32 i=0; i<pWidget->GetChildren().GetNumOfElements(); i++) {
		DestroyWidget(pWidget->GetChildren()[i]);
	}

	// Get backend
	WidgetLinux *pWidgetLinux = static_cast<WidgetLinux*>(pWidget->GetImpl());

	// Mark window destroyed
	pWidgetLinux->m_bDestroyed = true;

	// Destroy widget
	XDestroyWindow(m_pDisplay, pWidgetLinux->m_nNativeWindowHandle);
	XSync(m_pDisplay, False);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
