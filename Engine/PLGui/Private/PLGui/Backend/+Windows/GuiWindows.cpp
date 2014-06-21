/*********************************************************\
 *  File: GuiWindows.cpp                                 *
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
#include "PLGui/+Windows/PLGuiWindowsIncludes.h"
#include "PLGui/Gui/Gui.h"
#include "PLGui/Gui/Screen.h"
#include "PLGui/Gui/TrayIcon.h"
#include "PLGui/Gui/Base/GuiMessage.h"
#include "PLGui/Gui/Base/Keys.h"
#include "PLGui/Gui/Data/DataObject.h"
#include "PLGui/Gui/Resources/Graphics.h"
#include "PLGui/Widgets/Widget.h"
#include "PLGui/Backend/+Windows/GuiWindows.h"
#include "PLGui/Backend/+Windows/WidgetWindows.h"
#include "PLGui/Backend/+Windows/GraphicsWindows.h"
#include "PLGui/Backend/+Windows/ImageWindows.h"
#include "PLGui/Backend/+Windows/FontWindows.h"
#include "PLGui/Backend/+Windows/CursorWindows.h"
#include "PLGui/Backend/+Windows/TrayIconWindows.h"
#include "PLGui/Backend/+Windows/ClipBoardWindows.h"
#include "PLGui/Backend/+Windows/ToolsWindows.h"
#include <gdiplus.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLGui {


//[-------------------------------------------------------]
//[ Global definitions                                    ]
//[-------------------------------------------------------]
#define PL_INTERNAL		(WM_USER + 100)		// Internal message
#define PL_TIMER		(WM_USER + 101)		// Timer message
#define PL_WAKEUP		(WM_USER + 102)		// Wakeup message
#define PL_THEMECHANGED	(WM_USER + 103)		// ThemeChanged message
#define PL_TRAYICON		(WM_USER + 110)		// Tray icon message


//[-------------------------------------------------------]
//[ Global variables                                      ]
//[-------------------------------------------------------]
Gdiplus::GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR					 gdiplusToken;


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
GuiWindows::GuiWindows(Gui *pGui) : GuiImpl(pGui),
	m_hInstance(GetModuleHandle(nullptr)),
	m_nThreadID(GetCurrentThreadId()),
	m_pMouseOver(nullptr)
{
	// Create window class
	m_WndClass.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	m_WndClass.lpfnWndProc		= static_cast<WNDPROC>(WndProc);
	m_WndClass.cbClsExtra		= 0;
	m_WndClass.cbWndExtra		= 0;
	m_WndClass.hInstance		= m_hInstance;
	m_WndClass.hIcon			= LoadIcon(nullptr, IDI_APPLICATION);
	m_WndClass.hCursor			= nullptr;
	m_WndClass.hbrBackground	= nullptr;
	m_WndClass.lpszMenuName		= nullptr;
	m_WndClass.lpszClassName	= TEXT("PLGuiWidget");

	// Register window class
	if (!RegisterClass(&m_WndClass)) {
		// Error registering window class
	}

	// Initialize GDI+
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
}

/**
*  @brief
*    Destructor
*/
GuiWindows::~GuiWindows()
{
	// Shutdown GDI+
	Gdiplus::GdiplusShutdown(gdiplusToken);

	// Unregister window class
	if (!UnregisterClass(L"PLGuiWidget", m_hInstance)) {
		// Error unregistering window class
	}
}


//[-------------------------------------------------------]
//[ Public virtual GuiImpl functions                      ]
//[-------------------------------------------------------]
bool GuiWindows::HasPendingMessages()
{
	// Look if messages are waiting
	MSG sMsg;
	return PeekMessage(&sMsg, nullptr, 0, 0, PM_NOREMOVE) != 0;
}

void GuiWindows::ProcessMessage()
{
	// Wait and get message
	MSG sMsg;
	GetMessage(&sMsg, nullptr, 0, 0);

	// Process non-window messages
	if (sMsg.message == WM_QUIT) {
		// OnQuit message
		m_pGui->SendMessage(GuiMessage::OnExit());
	} else if (sMsg.message == PL_INTERNAL) {
		// OnInternalMessage message
		m_pGui->SendMessage(GuiMessage::OnInternalMessage(static_cast<uint32>(sMsg.wParam), static_cast<uint32>(sMsg.lParam)));
	} else if (sMsg.message == PL_TIMER) {
		// OnTimer message
		m_pGui->SendMessage(GuiMessage::OnTimer(reinterpret_cast<Timer*>(sMsg.lParam)));
	} else if (sMsg.message == PL_WAKEUP) {
		// OnWakeup message
		m_pGui->SendMessage(GuiMessage::OnWakeup());
	}

	// Process message
	TranslateMessage(&sMsg);
	DispatchMessage(&sMsg);
}

void GuiWindows::PostMessage(const GuiMessage &cMessage)
{
	// [TODO] Shall it be possible to post every kind of message?
	//        Currently, only specific kinds of messages can be posted, and we would have to map
	//        PL messages back to system messages, if we would like to support all messages

	// Get widget
	Widget *pWidget = cMessage.GetWidget();
	HWND hWnd = (pWidget ? reinterpret_cast<HWND>(pWidget->GetNativeWindowHandle()) : nullptr);

	// Post message
	switch (cMessage.GetType()) {
		case MessageOnExit:						/**< Exit message */
			::PostQuitMessage(0);
			break;

		case MessageOnTimer:					/**< Timer message */
			// Send timer message
			::PostThreadMessage(m_nThreadID, PL_TIMER, reinterpret_cast<WPARAM>(this), reinterpret_cast<LPARAM>(cMessage.GetTimer()));
			break;

		case MessageOnWakeup:					/**< Wakeup message loop */
			// Send wakeup message
			::PostThreadMessage(m_nThreadID, PL_WAKEUP, 0, 0);
			break;

		case MessageOnUserMessage:				/**< User message */
			// [TODO]
			break;

		case MessageOnInternalMessage:			/**< Internal message */
			// Send timer message
			::PostThreadMessage(m_nThreadID, PL_INTERNAL, static_cast<WPARAM>(cMessage.GetData()), reinterpret_cast<LPARAM>(cMessage.GetDataPointer()));
			break;

		case MessageOnThemeChanged:				/**< Theme has been changed */
			// Send OnThemeChanged message
			::PostMessageA(hWnd, PL_THEMECHANGED, 0, 0);
//			::PostThreadMessage(m_nThreadID, PL_THEMECHANGED, 0, 0);
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

		case MessageOnWindowState:				/**< Window state has changed, new window state as parameter */
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
	}
}

void GuiWindows::EnumerateScreens(List<Screen*> &lstScreens)
{
	// Enumerate display monitors
	m_lstScreens.Clear();
	if (EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, reinterpret_cast<LPARAM>(this))) {
		// Copy list of screens
		lstScreens = m_lstScreens;
	} else {
		// [TODO] Log error
	}
}

Vector2i GuiWindows::GetScreenSize() const
{
	// Get system metrics for screen-width and screen-height
	return Vector2i(::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));
}

bool GuiWindows::HasTaskbar()
{
	// Windows has a taskbar, yes
	return true;
}

void GuiWindows::SetMouseVisible(bool bVisible)
{
	// Set mouse visibility
	if (bVisible) {
		// Show mouse
		while (ShowCursor(true) < 0)
			; // Do nothing
	} else {
		// Hide mouse
		while (ShowCursor(false) >= 0)
			; // Do nothing
	}
}

void GuiWindows::ListFonts(List<FontInfo> &lstFonts) const
{
	// Enumerate all styles and charsets of all fonts
	List<String> lstFontNames;
	{
		LOGFONT lf;
		lf.lfFaceName[0]	= '\0';
		lf.lfCharSet		= DEFAULT_CHARSET;
		lf.lfPitchAndFamily	= 0;
		EnumFontFamiliesEx(::GetDC(nullptr), &lf, reinterpret_cast<FONTENUMPROCW>(FontEnumProc), reinterpret_cast<LPARAM>(static_cast<void*>(&lstFontNames)), 0);
	}

	// Enumerate all styles and charsets of all fonts
	for (uint32 i=0; i<lstFontNames.GetNumOfElements(); i++) {
		// Get font name
		String sName = lstFontNames[i];

		// Enumerate all styles of that font
		LOGFONT lf;
		wcscpy_s(lf.lfFaceName, sName.GetUnicode());
		lf.lfCharSet		= DEFAULT_CHARSET;
		lf.lfPitchAndFamily	= 0;
		EnumFontFamiliesEx(::GetDC(nullptr), &lf, reinterpret_cast<FONTENUMPROCW>(FontEnumProc2), reinterpret_cast<LPARAM>(static_cast<void*>(&lstFonts)), 0);
	}
}

WidgetImpl *GuiWindows::CreateWidgetImpl(Widget &cWidget) const
{
	// Create a Windows widget
	return new WidgetWindows(cWidget);
}

GraphicsImpl *GuiWindows::CreateGraphicsImpl(Graphics &cGraphics) const
{
	// Create a Windows graphics object
	return new GraphicsWindows(cGraphics);
}

ImageImpl *GuiWindows::CreateImageImpl(Image &cImage) const
{
	// Create a Windows image object
	return new ImageWindows(cImage);
}

FontImpl *GuiWindows::CreateFontImpl(Font &cFont) const
{
	// Create a Windows font object
	return new FontWindows(cFont);
}

CursorImpl *GuiWindows::CreateCursorImpl(Cursor &cCursor) const
{
	// Create a Windows cursor object
	return new CursorWindows(cCursor);
}

TrayIconImpl *GuiWindows::CreateTrayIconImpl(TrayIcon &cTrayIcon) const
{
	// Create a Windows tray-icon object
	return new TrayIconWindows(cTrayIcon);
}

ClipBoardImpl *GuiWindows::CreateClipBoardImpl(ClipBoard &cClipBoard) const
{
	// Create a Windows clipboard object
	return new ClipBoardWindows(cClipBoard);
}


//[-------------------------------------------------------]
//[ Private static functions                              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Static window callback function
*/
LRESULT CALLBACK GuiWindows::WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	// Get pointers to widget and GUI
	WidgetWindows *pWidgetWindows = nullptr;
	if (nMsg == WM_CREATE) pWidgetWindows = static_cast<WidgetWindows*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
	else if (hWnd != nullptr) pWidgetWindows = reinterpret_cast<WidgetWindows*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	Widget	   *pWidget		= (pWidgetWindows ? pWidgetWindows->m_pWidget				  : nullptr);
	Gui		   *pGui		= (pWidget		  ? pWidget->GetGui()						  : nullptr);
	GuiWindows *pGuiWindows	= (pGui			  ? static_cast<GuiWindows*>(pGui->GetImpl()) : nullptr);

	// Disable screen saver and monitor power management...
	// [TODO] Always? Shouldn't this be an option?
	if (nMsg == WM_SYSCOMMAND && (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER))
		return 0;

	// If we get a message for a dead widget, something is wrong
	if (pWidgetWindows && pWidgetWindows->m_bDestroyed) {
		// There is always a WM_NCDESTROY coming after WM_DESTROY, so this will be called which
		// is OK. But we don't need the message, so just give back here to be sure that nothing can go wrong...
		return DefWindowProc(hWnd, nMsg, wParam, lParam);
	}

	// We need a widget now
	if (pWidgetWindows != nullptr) {
		// Process message
		switch (nMsg) {
			// Initialize widget
			case WM_CREATE:
				// Set widget pointer and handle (SetWindowLongPtr is the 64bit equivalent to SetWindowLong)
				SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWidgetWindows));
				pWidgetWindows->m_hWnd = hWnd;

				// Send OnCreate message
				if (pGui)
					pGui->SendMessage(GuiMessage::OnCreate(pWidget));
				return 0;

			// Destroy widget
			case WM_DESTROY:
				// Send OnDestroy message
				if (pGui)
					pGui->SendMessage(GuiMessage::OnDestroy(pWidget));

				// Mark widget destroyed
				pWidgetWindows->m_bDestroyed = true;
				pWidgetWindows->m_hWnd		 = nullptr;
				return 0;

			// Close widget
			case WM_CLOSE:
				// Send OnClose message
				if (pGui)
					pGui->SendMessage(GuiMessage::OnClose(pWidget));
				return 0;

			// Show widget
			case WM_SHOWWINDOW:
				// Send OnShow/OnHide message
				if (pGui) {
					if (wParam)	pGui->SendMessage(GuiMessage::OnShow(pWidget));
					else		pGui->SendMessage(GuiMessage::OnHide(pWidget));
				}
				return 0;

			// Activate/Deactivate widget
			case WM_ACTIVATE:
				if (wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE) {
					// Does this widget or any child widget accept the focus?
					if (pWidget->CanAcceptFocus()) {
						// Try to set focus to the widget
						pWidget->SetFocus();
					}
				}
				return 0;

			// Activate widget by mouse click
			case WM_MOUSEACTIVATE:
				// Does this widget or any child widget accept the focus?
				if (pWidget->CanAcceptFocus()) {
					// Try to set focus to the widget
					pWidget->SetFocus();

					// Activate widget
					return MA_ACTIVATE;
				} else {
					// No not activate widget
					return MA_NOACTIVATE;
				}

			// Got focus
			case WM_SETFOCUS:
				// Ignore focus message?
				if (!pWidgetWindows->m_bIgnoreSetFocus) {
					// Do not trust window messages and set focus only, if the widget really want's it
					if ((pWidget->GetFocusStyle() == AcceptFocus || pWidget->GetFocusStyle() == ChildFocusOrSelf) && pWidget->IsEnabled()) {
						// Send OnGetFocus message
						if (pGui)
							pGui->SendMessage(GuiMessage::OnGetFocus(pWidget));
					}
				} else pWidgetWindows->m_bIgnoreSetFocus = false;
				return 0;

			// Lost focus
			case WM_KILLFOCUS:
			{
				// Get new focus widget
				Widget *pFocus = GetPLGuiWidget(reinterpret_cast<HWND>(wParam));
				if (pFocus && pFocus != pWidget) {
					// Send OnGetFocus message first!
					if (pGui)
						pGui->SendMessage(GuiMessage::OnGetFocus(pFocus));

					// Ignore coming WM_SETFOCUS message
					static_cast<WidgetWindows*>(pFocus->GetImpl())->m_bIgnoreSetFocus = true;
				}

				// Send OnLooseFocus message
				if (pFocus != pWidget && pGui) {
					pGui->SendMessage(GuiMessage::OnLooseFocus(pWidget));
				}
				return 0;
			}

			// Set window mouse cursor
			case WM_SETCURSOR:
				// [TODO] Test mouse-cursor functionality
				// Do not process message any further
//				return TRUE;
				break;

			case WM_PAINT:
			{
				// Begin paint
				PAINTSTRUCT sPaint;
				BeginPaint(hWnd, &sPaint);

				// Create Windows graphics object
				GraphicsWindows cGraphicsWindows(sPaint.hdc);
				Graphics cGraphics(*pGuiWindows->m_pGui, cGraphicsWindows);

				if (pGui) {
					// Send OnDrawBackground message
					if (sPaint.fErase != FALSE) {
						pGui->SendMessage(GuiMessage::OnDrawBackground(pWidget, &cGraphics));
					}

					// Send OnDraw message
					pGui->SendMessage(GuiMessage::OnDraw(pWidget, &cGraphics));
				}

				// End paint
				EndPaint(hWnd, &sPaint);
				return 0;
			}

			// Move widget
			case WM_MOVE:
			{
				if (pGui) {
					// Get position
					POINTS sPos = MAKEPOINTS(lParam);

					// Send OnMove message
					pGui->SendMessage(GuiMessage::OnMove(pWidget, Vector2i(sPos.x, sPos.y)));
				}

				// Done
				return 0;
			}

			// Size widget
			case WM_SIZE:
				// Restore from fullscreen mode
				if (pWidgetWindows->m_nWindowState == StateFullscreen && pGui) {
					// Send OnLeaveFullscreen message
					pGui->SendMessage(GuiMessage::OnLeaveFullscreen(pWidget));
				}

				// Save window state
					 if (pWidgetWindows->m_bFullscreen)	pWidgetWindows->m_nWindowState = StateFullscreen;
				else if (wParam == SIZE_MINIMIZED)		pWidgetWindows->m_nWindowState = StateMinimized;
				else if (wParam == SIZE_MAXIMIZED)		pWidgetWindows->m_nWindowState = StateMaximized;
				else if (wParam == SIZE_RESTORED)		pWidgetWindows->m_nWindowState = StateNormal;

				if (pGui) {
					// Send OnWindowState message
					pGui->SendMessage(GuiMessage::OnWindowState(pWidget, pWidgetWindows->m_nWindowState));

					// Send OnSize message
					pGui->SendMessage(GuiMessage::OnSize(pWidget, Vector2i(LOWORD(lParam), HIWORD(lParam))));
				}

				// Done
				return 0;

			// Mouse movement
			case WM_MOUSEMOVE:
			{
				// Keep us informed about mouse-leave events
				TRACKMOUSEEVENT sTrackMouseEvent;
				sTrackMouseEvent.cbSize		 = sizeof(TRACKMOUSEEVENT);
				sTrackMouseEvent.dwFlags	 = TME_LEAVE;
				sTrackMouseEvent.hwndTrack	 = pWidgetWindows->m_hWnd;
				sTrackMouseEvent.dwHoverTime = 0;
				TrackMouseEvent(&sTrackMouseEvent);

				// Get position
				POINTS sPos = MAKEPOINTS(lParam);
				Vector2i vPos(sPos.x, sPos.y);

				// Check if mouse-enter widget has changed
				if (pGuiWindows->m_pMouseOver != pWidget) {
					// Save mouse-enter widget
					pGuiWindows->m_pMouseOver = pWidget;

					// Send OnMouseEnter message
					if (pGui)
						pGui->SendMessage(GuiMessage::OnMouseEnter(pWidget));
				}

				// Send OnMouseMove message
				if (pGui)
					pGui->SendMessage(GuiMessage::OnMouseMove(pWidget, vPos));
				return 0;
			}

			// Mouse leaves the widget
			case WM_MOUSELEAVE:
				// Send OnMouseLeave message
				if (pGui)
					pGui->SendMessage(GuiMessage::OnMouseLeave(pWidget));

				// Check if mouse-enter widget has changed
				if (pGuiWindows->m_pMouseOver == pWidget) {
					// Reset mouse-enter widget
					pGuiWindows->m_pMouseOver = nullptr;
				}
				return 0;

			// Left mouse button down
			case WM_LBUTTONDOWN:
				// Send OnMouseButtonDown message
				if (pGui)
					pGui->SendMessage(GuiMessage::OnMouseButtonDown(pWidget, LeftButton, Vector2i(LOWORD(lParam), HIWORD(lParam))));
				return 0;

			// Left mouse button up
			case WM_LBUTTONUP:
				if (pGui) {
					// Send OnMouseButtonUp message
					pGui->SendMessage(GuiMessage::OnMouseButtonUp(pWidget, LeftButton, Vector2i(LOWORD(lParam), HIWORD(lParam))));

					// [TODO] When to send a click-event, and when not?
					// Send OnMouseButtonClick message
					pGui->SendMessage(GuiMessage::OnMouseButtonClick(pWidget, LeftButton, Vector2i(LOWORD(lParam), HIWORD(lParam))));
				}
				return 0;

			// Left mouse button double click
			case WM_LBUTTONDBLCLK:
				// Send OnMouseButtonDoubleClick message
				if (pGui)
					pGui->SendMessage(GuiMessage::OnMouseButtonDoubleClick(pWidget, LeftButton, Vector2i(LOWORD(lParam), HIWORD(lParam))));
				return 0;

			// Right mouse button down
			case WM_RBUTTONDOWN:
				// Send OnMouseButtonDown message
				if (pGui)
					pGui->SendMessage(GuiMessage::OnMouseButtonDown(pWidget, RightButton, Vector2i(LOWORD(lParam), HIWORD(lParam))));
				return 0;

			// Right mouse button up
			case WM_RBUTTONUP:
				// Send OnMouseButtonUp message
				if (pGui)
					pGui->SendMessage(GuiMessage::OnMouseButtonUp(pWidget, RightButton, Vector2i(LOWORD(lParam), HIWORD(lParam))));
				return 0;

			// Right mouse button double click
			case WM_RBUTTONDBLCLK:
				// Send OnMouseButtonDoubleClick message
				if (pGui)
					pGui->SendMessage(GuiMessage::OnMouseButtonDoubleClick(pWidget, RightButton, Vector2i(LOWORD(lParam), HIWORD(lParam))));
				return 0;

			// Middle mouse button down
			case WM_MBUTTONDOWN:
				// Send OnMouseButtonDown message
				if (pGui)
					pGui->SendMessage(GuiMessage::OnMouseButtonDown(pWidget, MiddleButton, Vector2i(LOWORD(lParam), HIWORD(lParam))));
				return 0;

			// Middle mouse button up
			case WM_MBUTTONUP:
				// Send OnMouseButtonUp message
				if (pGui)
					pGui->SendMessage(GuiMessage::OnMouseButtonUp(pWidget, MiddleButton, Vector2i(LOWORD(lParam), HIWORD(lParam))));
				return 0;

			// Middle mouse button double click
			case WM_MBUTTONDBLCLK:
				// Send OnMouseButtonDoubleClick message
				if (pGui)
					pGui->SendMessage(GuiMessage::OnMouseButtonDoubleClick(pWidget, MiddleButton, Vector2i(LOWORD(lParam), HIWORD(lParam))));
				return 0;

			// Mouse wheel
			case WM_MOUSEWHEEL:
				// Send OnMouseWheel message
				if (pGui)
					pGui->SendMessage(GuiMessage::OnMouseWheel(pWidget, static_cast<short>(HIWORD(wParam))/120));
				return 0;

			// Keyboard key down
			case WM_KEYDOWN:
			{
				// Get state of modifier keys
				uint32 nModifiers = 0;
				if (GetAsyncKeyState(VK_MENU))	  nModifiers |= PLGUIMOD_ALT;
				if (GetAsyncKeyState(VK_CONTROL)) nModifiers |= PLGUIMOD_CONTROL;
				if (GetAsyncKeyState(VK_SHIFT))	  nModifiers |= PLGUIMOD_SHIFT;
				if (GetAsyncKeyState(VK_LWIN))	  nModifiers |= PLGUIMOD_WIN;
				if (GetAsyncKeyState(VK_RWIN))	  nModifiers |= PLGUIMOD_WIN;

				// Send OnKeyDown message
				if (pGui)
					pGui->SendMessage(GuiMessage::OnKeyDown(pWidget, static_cast<uint32>(wParam), nModifiers));
				return 0;
			}

			// Keyboard key up
			case WM_KEYUP:
			{
				// Get state of modifier keys
				uint32 nModifiers = 0;
				if (GetAsyncKeyState(VK_MENU))	  nModifiers |= PLGUIMOD_ALT;
				if (GetAsyncKeyState(VK_CONTROL)) nModifiers |= PLGUIMOD_CONTROL;
				if (GetAsyncKeyState(VK_SHIFT))	  nModifiers |= PLGUIMOD_SHIFT;
				if (GetAsyncKeyState(VK_LWIN))	  nModifiers |= PLGUIMOD_WIN;
				if (GetAsyncKeyState(VK_RWIN))	  nModifiers |= PLGUIMOD_WIN;

				// Send OnKeyUp message
				if (pGui)
					pGui->SendMessage(GuiMessage::OnKeyUp(pWidget, static_cast<uint32>(wParam), nModifiers));
				return 0;
			}

			// Process system keys (ALT+<key>)
			case WM_SYSCHAR:
			{
				// Send OnKeyDown followed by an OnKeyUp message
				if (pGui) {
					pGui->SendMessage(GuiMessage::OnKeyDown(pWidget, static_cast<uint32>(wParam), PLGUIMOD_ALT));
					pGui->SendMessage(GuiMessage::OnKeyUp  (pWidget, static_cast<uint32>(wParam), PLGUIMOD_ALT));
				}
				return 0;
			}

			// Process hotkey
			case WM_HOTKEY:
				// Send OnHotkey message
				if (pGui)
					pGui->SendMessage(GuiMessage::OnHotkey(pWidget, static_cast<uint32>(wParam)));
				return 0;

			// Drag and drop of files
			case WM_DROPFILES:
			{
				// Get dropped filenames. Because there's no way - without extreme overhead :) - to check whether
				// we really need to use Unicode or ASCII is quite enough, we always use Unicode just to be sure.
				uint32 nNumOfFiles = DragQueryFileW(reinterpret_cast<HDROP>(wParam), 0xFFFFFFFF, static_cast<LPWSTR>(nullptr), 0);
				if (nNumOfFiles) {
					// Create the file list
					Array<String> lstFiles;
					lstFiles.Resize(nNumOfFiles);
					for (uint32 i=0; i<nNumOfFiles; i++) {
						// Get the length of the string (+1 for \0)
						UINT nSize = DragQueryFileW(reinterpret_cast<HDROP>(wParam), i, nullptr, 0) + 1;

						// Create the string and fill it
						wchar_t *pszFile = new wchar_t[nSize];
						DragQueryFileW(reinterpret_cast<HDROP>(wParam), i, pszFile, nSize);

						// Store the string (the PL string takes over the control)
						lstFiles[i] = String(pszFile, false, nSize - 1);
					}

					// Create data object
					DataObject cData(lstFiles);

					// Send OnDrop message
					if (pGui)
						pGui->SendMessage(GuiMessage::OnDrop(pWidget, &cData));
					return 0;
				}
			}

			// OnThemeChanged message
			case PL_THEMECHANGED:
				// Send OnThemeChanged message
				if (pGui)
					pGui->SendMessage(GuiMessage::OnThemeChanged(pWidget));
				return 0;

			// Tray icon message
			case PL_TRAYICON:
				// Process tray icon message
				ProcessTrayIconMessage(static_cast<TrayIcon*>(pWidget->GetUserData()), wParam, lParam);
				return 0;
		}
	}

	// Let the OS handle this message...
	return hWnd ? DefWindowProc(hWnd, nMsg, wParam, lParam) : 0;
}

/**
*  @brief
*    Static monitor enumeration callback function
*/
BOOL CALLBACK GuiWindows::MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	// Get pointer to GuiWindows object
	GuiWindows *pThis = reinterpret_cast<GuiWindows*>(dwData);

	// Get index of this monitor
	int nMonitor = pThis->m_lstScreens.GetNumOfElements();

	// Get monitor information
	MONITORINFO sMonitorInfo;
	sMonitorInfo.cbSize = sizeof(MONITORINFO);
	if (GetMonitorInfo(hMonitor, &sMonitorInfo)) {
		// Add screen
		Screen *pScreen = new Screen(pThis->m_pGui);
		pScreen->SetName   (String("Monitor") + nMonitor);
		pScreen->SetPos	   (Vector2i(sMonitorInfo.rcMonitor.left, sMonitorInfo.rcMonitor.top));
		pScreen->SetSize   (Vector2i(sMonitorInfo.rcMonitor.right - sMonitorInfo.rcMonitor.left, sMonitorInfo.rcMonitor.bottom - sMonitorInfo.rcMonitor.top));
		pScreen->SetDefault(static_cast<bool>(sMonitorInfo.dwFlags & MONITORINFOF_PRIMARY));
		pThis->m_lstScreens.Add(pScreen);
	} else {
		// [TODO] Log error

		// Stop enumeration
		return FALSE;
	}

	// Get next monitor
	return TRUE;
}

/**
*  @brief
*    Static font enumeration callback function (enumerating font names only)
*/
int CALLBACK GuiWindows::FontEnumProc(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, DWORD FontType, LPARAM lParam)
{
	// Get output list
	List<String> &lstFonts = *(static_cast<List<String>*>(reinterpret_cast<void*>(lParam)));

	// Get font name
	String sFont = lpelfe->elfLogFont.lfFaceName;

	// Add only if not already in list
	if (!lstFonts.IsElement(sFont))
		lstFonts.Add(sFont);

	// Next
	return 1;
}

/**
*  @brief
*    Static font enumeration callback function (enumerating all weights and styles of a font)
*/
int CALLBACK GuiWindows::FontEnumProc2(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, DWORD FontType, LPARAM lParam)
{
	// Get output list
	List<FontInfo> &lstFonts = *(static_cast<List<FontInfo>*>(reinterpret_cast<void*>(lParam)));

	// Create font info
	FontInfo cFontInfo(
		"",
		String(lpelfe->elfLogFont.lfFaceName),
		static_cast<uint32>(lpelfe->elfLogFont.lfHeight),
		(lpelfe->elfLogFont.lfWeight >= FW_BOLD ? WeightBold : (lpelfe->elfLogFont.lfWeight >= FW_NORMAL ? WeightNormal : WeightLight)),
		(lpelfe->elfLogFont.lfItalic ? StyleItalics : StyleNormal) );

	// Add font (avoid duplicates that are there due to difference character sets (western, arabic, kyrillic etc.)
	if (!lstFonts.IsElement(cFontInfo))
		lstFonts.Add(cFontInfo);

	// Next
	return 1;
}

/**
*  @brief
*    Windows timer callback
*/
void GuiWindows::TimerFunction(HWND hWnd, UINT nMsg, UINT_PTR nTimerID, DWORD nTime)
{
	// Nothing to do in here, but we need to register this function in order to get a WM_TIMER message
}

/**
*  @brief
*    Process a tray icon message
*/
void GuiWindows::ProcessTrayIconMessage(TrayIcon *pTrayIcon, uint32 nID, uint32 nMessage)
{
	// Check if tray icon is valid
	if (pTrayIcon) {
		// Get current cursor position
		POINT sPoint;
		GetCursorPos(&sPoint);
		Vector2i vPos(sPoint.x, sPoint.y);

		// Process message
		switch (nMessage) {
			// Mouse has moved
			case WM_MOUSEMOVE:
				pTrayIcon->OnMouseMove(vPos);
				break;

			// Left button down
			case WM_LBUTTONDOWN:
				pTrayIcon->OnMouseButtonDown(LeftButton, vPos);
				break;

			// Left button up
			case WM_LBUTTONUP:
				pTrayIcon->OnMouseButtonUp(LeftButton, vPos);
				pTrayIcon->OnMouseButtonClick(LeftButton, vPos);
				break;

			// Left button double-click
			case WM_LBUTTONDBLCLK:
				pTrayIcon->OnMouseButtonDoubleClick(LeftButton, vPos);
				break;

			// Right button down
			case WM_RBUTTONDOWN:
				pTrayIcon->OnMouseButtonDown(RightButton, vPos);
				break;

			// Right button up
			case WM_RBUTTONUP:
				pTrayIcon->OnMouseButtonUp(RightButton, vPos);
				pTrayIcon->OnMouseButtonClick(RightButton, vPos);
				break;

			// Right button double-click
			case WM_RBUTTONDBLCLK:
				pTrayIcon->OnMouseButtonDoubleClick(RightButton, vPos);
				break;
		}
	}
}

/**
*  @brief
*    Get PLGui widget that belongs to a certain window handle
*/
Widget *GuiWindows::GetPLGuiWidget(HWND hWnd)
{
	// Check window handle
	if (hWnd != nullptr) {
		// Check window class name
		wchar_t szClassName[32];
		GetClassNameW(hWnd, szClassName, 32);
		String sClassName = szClassName;
		if (sClassName == L"PLGuiWidget") {
			// We have a PLGui widget here!
			WidgetWindows *pWidgetWindows = reinterpret_cast<WidgetWindows*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			if (pWidgetWindows) {
				// Return widget
				return pWidgetWindows->m_pWidget;
			}
		}
	}

	// Not a valid PLGui widget
	return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui