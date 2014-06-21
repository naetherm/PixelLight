/*********************************************************\
 *  File: TrayIconWidgetLinux.cpp                        *
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
#include "PLGui/Gui/Resources/Graphics.h"
#include "PLGui/Gui/TrayIcon.h"
#include "PLGui/Backend/Linux/GuiLinux.h"
#include "PLGui/Backend/Linux/WidgetLinux.h"
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
using namespace PLGraphics;
using namespace PLMath;
namespace PLGui {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(TrayIconWidgetLinux)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
TrayIconWidgetLinux::TrayIconWidgetLinux(Widget *pParent) : Widget(pParent, true),
	m_pTrayIcon(nullptr),
	m_nEmbedderWindow(0)
{
	// Make sure that the window is not destroyed automatically by Gui, as it is managed by TrayIcon and will be deleted with it
	//   This is done by the 'true' in Widget(pParent, BaseWidget, true)

	// Get Linux implementations
	GuiLinux	*pGuiLinux	  = static_cast<GuiLinux*>   (m_pGui->GetImpl());
	WidgetLinux *pWidgetLinux = static_cast<WidgetLinux*>(m_pWidgetImpl);

	// Find system tray window
	m_nEmbedderWindow = XGetSelectionOwner(pWidgetLinux->m_pDisplay, XInternAtom(pWidgetLinux->m_pDisplay, "_NET_SYSTEM_TRAY_S0", False));

/*	[sbusch] [TODO] Use correct visual:
         Atom actual_type;
            int actual_format;
            ulong nitems, bytes_remaining;
            uchar *data = 0;
            int result = XGetWindowProperty(display, win, ATOM(_NET_SYSTEM_TRAY_VISUAL), 0, 1,
                                            False, XA_VISUALID, &actual_type,
                                            &actual_format, &nitems, &bytes_remaining, &data);
            VisualID vid = 0;
            if (result == Success && data && actual_type == XA_VISUALID && actual_format == 32 &&
                nitems == 1 && bytes_remaining == 0)
                vid = *(VisualID*)data;
            if (data)
                XFree(data);
            if (vid == 0)
                return 0;

            uint mask = VisualIDMask;
            XVisualInfo *vi, rvi;
            int count;
            rvi.visualid = vid;
            vi = XGetVisualInfo(display, mask, &rvi, &count);
            if (vi) {
                sysTrayVisual = vi[0];
                XFree((char*)vi);
            }
            if (sysTrayVisual.depth != 32)
                memset(&sysTrayVisual, 0, sizeof(sysTrayVisual));
        }
    }

    return sysTrayVisual.visual ? &sysTrayVisual : 0;
*/

    // Set XEMBED info
	unsigned long sXEmbedInfo[2];
    sXEmbedInfo[0] = 0;
    sXEmbedInfo[1] = 0;
    XChangeProperty(pWidgetLinux->m_pDisplay, pWidgetLinux->m_nNativeWindowHandle, pGuiLinux->m_sAtoms._XEMBED_INFO, pGuiLinux->m_sAtoms._XEMBED_INFO, 32, PropModeReplace, reinterpret_cast<unsigned char*>(sXEmbedInfo), 2);

	// Don't draw a background
	SetBackgroundColor(Color4::Transparent);
}

/**
*  @brief
*    Destructor
*/
TrayIconWidgetLinux::~TrayIconWidgetLinux()
{
}

/**
*  @brief
*    Set tray icon to which the window belongs
*/
void TrayIconWidgetLinux::SetTrayIcon(TrayIcon *pTrayIcon)
{
	// Set pointer to tray icon
	m_pTrayIcon = pTrayIcon;
}

/**
*  @brief
*    Show a notification text
*/
void TrayIconWidgetLinux::ShowNotification(const String &sTitle, const String &sText)
{
	// Create a unique ID for every notification message
	static long nID = 0;
	nID++;

	// Get text size in bytes
	long nSize = sText.GetNumOfBytes(String::UTF8);

	// Start tray message
	SendSysTrayMessage(static_cast<WidgetLinux*>(m_pWidgetImpl)->m_nNativeWindowHandle, SYSTEM_TRAY_BEGIN_MESSAGE, 0, nSize, nID);

	// Send string
	const char *pszString = sText.GetUTF8();
	while (nSize > 0) {
		// Send a portion of the data, max. 20 bytes
		long nPortion = (nSize > 20 ? 20 : nSize);
		SendSysTrayMessageData(static_cast<WidgetLinux*>(m_pWidgetImpl)->m_nNativeWindowHandle, pszString, nPortion);

		// Go on
		pszString += nPortion;
		nSize -= nPortion;
	}
}

/**
*  @brief
*    Embed window into system tray
*/
void TrayIconWidgetLinux::EmbedIntoSysTray()
{
	// Send message
	SendSysTrayMessage(m_nEmbedderWindow, SYSTEM_TRAY_REQUEST_DOCK, static_cast<WidgetLinux*>(GetImpl())->m_nNativeWindowHandle, 0, 0);
}

/**
*  @brief
*    Window requests to get the focus
*/
void TrayIconWidgetLinux::XEmbedRequestFocus()
{
	// Send message
	SendXEmbedMessage(XEMBED_REQUEST_FOCUS, 0, 0, 0);
}

/**
*  @brief
*    Window has reached the end of it's logical tab (tabbing forward)
*/
void TrayIconWidgetLinux::XEmbedFocusNext()
{
	// Send message
	SendXEmbedMessage(XEMBED_FOCUS_NEXT, 0, 0, 0);
}

/**
*  @brief
*    Window has reached the beginning of it's logical tab (tabbing backwards)
*/
void TrayIconWidgetLinux::XEmbedFocusPrev()
{
	// Send message
	SendXEmbedMessage(XEMBED_FOCUS_PREV, 0, 0, 0);
}

/**
*  @brief
*    Register a key accelerator
*/
void TrayIconWidgetLinux::XEmbedRegisterAccelerator(long nID, long nKey, long nModifiers)
{
	// Send message
	SendXEmbedMessage(XEMBED_REGISTER_ACCELERATOR, nID, nKey, nModifiers);
}

/**
*  @brief
*    Unregister a key accelerator
*/
void TrayIconWidgetLinux::XEmbedUnregisterAccelerator(long nID)
{
	// Send message
	SendXEmbedMessage(XEMBED_UNREGISTER_ACCELERATOR, nID, 0, 0);
}


//[-------------------------------------------------------]
//[ Public virtual TrayIconWidgetLinux functions          ]
//[-------------------------------------------------------]
/**
*  @brief
*    Window has been embedded by a XEMBED compliant embedder
*/
void TrayIconWidgetLinux::OnXEmbedEmbeddedNotify(::Window nEmbedderWindow, long nProtocolVersion)
{
	// Show window
	SetVisible(true);
}

/**
*  @brief
*    Window has been activated (e.g. got the focus)
*/
void TrayIconWidgetLinux::OnXEmbedWindowActivate()
{
}

/**
*  @brief
*    Window has been deactivated (e.g. lost the focus)
*/
void TrayIconWidgetLinux::OnXEmbedWindowDeactivate()
{
}

/**
*  @brief
*    Window has got the focus
*/
void TrayIconWidgetLinux::OnXEmbedFocusIn(long nFocus)
{
}

/**
*  @brief
*    Window has lost the focus
*/
void TrayIconWidgetLinux::OnXEmbedFocusOut()
{
}

/**
*  @brief
*    Window becomes shadowed by a modal dialog
*/
void TrayIconWidgetLinux::OnXEmbedModalityOn()
{
}

/**
*  @brief
*    Window has been released from a modal dialog
*/
void TrayIconWidgetLinux::OnXEmbedModalityOff()
{
}

/**
*  @brief
*    An accelerator has been activated
*/
void TrayIconWidgetLinux::OnXEmbedActivateAccelerator(long nID, long nFlags)
{
}


//[-------------------------------------------------------]
//[ Public virtual Widget functions                       ]
//[-------------------------------------------------------]
void TrayIconWidgetLinux::OnDraw(Graphics &cGraphics)
{
	// Draw image
	cGraphics.DrawImage(GetIcon(), Vector2i::Zero, Vector2i::Zero);
}

void TrayIconWidgetLinux::OnMouseMove(const Vector2i &vPos)
{
	// Issue tray icon event
	if (m_pTrayIcon) {
		m_pTrayIcon->EventMouseMove(vPos);
	}
}

void TrayIconWidgetLinux::OnMouseButtonDown(uint32 nButton, const Vector2i &vPos)
{
	// Issue tray icon event
	if (m_pTrayIcon) {
		m_pTrayIcon->EventMouseButtonDown(nButton, vPos);
	}
}

void TrayIconWidgetLinux::OnMouseButtonUp(uint32 nButton, const Vector2i &vPos)
{
	// Issue tray icon event
	if (m_pTrayIcon) {
		m_pTrayIcon->EventMouseButtonUp(nButton, vPos);
	}
}

void TrayIconWidgetLinux::OnMouseButtonClick(uint32 nButton, const Vector2i &vPos)
{
	// Issue tray icon event
	if (m_pTrayIcon) {
		m_pTrayIcon->EventMouseButtonClick(nButton, vPos);
	}
}

void TrayIconWidgetLinux::OnMouseButtonDoubleClick(uint32 nButton, const Vector2i &vPos)
{
	// Issue tray icon event
	if (m_pTrayIcon) {
		m_pTrayIcon->EventMouseButtonDoubleClick(nButton, vPos);
	}
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Send message to XEMBED embedder window
*/
void TrayIconWidgetLinux::SendXEmbedMessage(long nMessage, long nDetail, long nData1, long nData2)
{
	// Have we found a system tray?
	if (m_nEmbedderWindow) {
		// Send event
		XEvent sEvent;
		sEvent.xclient.type			= ClientMessage;
		sEvent.xclient.window		= m_nEmbedderWindow;
		sEvent.xclient.message_type	= static_cast<GuiLinux*>(m_pGui->GetImpl())->m_sAtoms._XEMBED;
		sEvent.xclient.format		= 32;
		sEvent.xclient.data.l[0]	= CurrentTime;
		sEvent.xclient.data.l[1]	= nMessage;
		sEvent.xclient.data.l[2]	= nDetail;
		sEvent.xclient.data.l[3]	= nData1;
		sEvent.xclient.data.l[4]	= nData2;
		XSendEvent(static_cast<WidgetLinux*>(m_pWidgetImpl)->m_pDisplay, m_nEmbedderWindow, False, NoEventMask, &sEvent);
		XSync(static_cast<WidgetLinux*>(m_pWidgetImpl)->m_pDisplay, False);
	}
}

/**
*  @brief
*    Send system tray message to XEMBED embedder window
*/
void TrayIconWidgetLinux::SendSysTrayMessage(::Window nNativeWindowHandle, long nMessage, long nDetail, long nData1, long nData2)
{
	// Have we found a system tray?
	if (m_nEmbedderWindow) {
		// Send tray message
		XEvent sEvent;
		sEvent.xclient.type			= ClientMessage;
		sEvent.xclient.window		= nNativeWindowHandle;
		sEvent.xclient.message_type = static_cast<GuiLinux*>(m_pGui->GetImpl())->m_sAtoms._NET_SYSTEM_TRAY_OPCODE;
		sEvent.xclient.format		= 32;
		sEvent.xclient.data.l[0]	= CurrentTime;
		sEvent.xclient.data.l[1]	= nMessage;
		sEvent.xclient.data.l[2]	= nDetail;
		sEvent.xclient.data.l[3]	= nData1;
		sEvent.xclient.data.l[4]	= nData2;
		XSendEvent(static_cast<WidgetLinux*>(m_pWidgetImpl)->m_pDisplay, m_nEmbedderWindow, False, NoEventMask, &sEvent);
		XSync(static_cast<WidgetLinux*>(m_pWidgetImpl)->m_pDisplay, False);
	}
}

/**
*  @brief
*    Send system tray data message to XEMBED embedder window
*/
void TrayIconWidgetLinux::SendSysTrayMessageData(::Window nNativeWindowHandle, const char *pszString, long nSize)
{
	// Have we found a system tray?
	if (m_nEmbedderWindow) {
		// The string must be valid, not empty and have a length of max. 20
		if (pszString && nSize > 0 && nSize <= 20) {
			// Send tray message
			XEvent sEvent;
			sEvent.xclient.type			= ClientMessage;
			sEvent.xclient.window		= nNativeWindowHandle;
			sEvent.xclient.message_type = static_cast<GuiLinux*>(m_pGui->GetImpl())->m_sAtoms._NET_SYSTEM_TRAY_MESSAGE_DATA;
			sEvent.xclient.format		= 8;
			for (uint32 i=0; i<nSize; i++) {
				sEvent.xclient.data.b[i] = pszString[i];
			}
			XSendEvent(static_cast<WidgetLinux*>(m_pWidgetImpl)->m_pDisplay, m_nEmbedderWindow, False, StructureNotifyMask, &sEvent);
			XSync(static_cast<WidgetLinux*>(m_pWidgetImpl)->m_pDisplay, False);
		}
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
