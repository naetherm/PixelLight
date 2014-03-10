/*********************************************************\
 *  File: TrayIconWidgetLinux.h                          *
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


#ifndef __PLGUI_TRAYICONWIDGETLINUX_H__
#define __PLGUI_TRAYICONWIDGETLINUX_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGui/PLGuiLinuxIncludes.h"
#include "PLGui/Widgets/Widget.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class TrayIcon;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Tray icon widget class
*
*  @remarks
*    A widget that serves as an (embedded) tray icon widget
*/
class TrayIconWidgetLinux : public Widget {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, TrayIconWidgetLinux, "PLGui", PLGui::Widget, "Tray icon widget class")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pParent
		*    Pointer to the parent widget
		*
		*  @remarks
		*    If pParent == nullptr, the root widget of the system GUI is used as parent widget
		*/
		PLGUI_API TrayIconWidgetLinux(Widget *pParent = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~TrayIconWidgetLinux();

		/**
		*  @brief
		*    Set tray icon to which the window belongs
		*
		*  @param[in] pTrayIcon
		*    Tray icon
		*/
		PLGUI_API void SetTrayIcon(TrayIcon *pTrayIcon);

		/**
		*  @brief
		*    Show a notification text
		*
		*  @param[in] sTitle
		*    Title
		*  @param[in] sText
		*    Text
		*/
		PLGUI_API void ShowNotification(const PLCore::String &sTitle, const PLCore::String &sText);

		/**
		*  @brief
		*    Embed window into system tray
		*/
		PLGUI_API void EmbedIntoSysTray();

		/**
		*  @brief
		*    Window requests to get the focus
		*/
		PLGUI_API void XEmbedRequestFocus();

		/**
		*  @brief
		*    Window has reached the end of it's logical tab (tabbing forward)
		*/
		PLGUI_API void XEmbedFocusNext();

		/**
		*  @brief
		*    Window has reached the beginning of it's logical tab (tabbing backwards)
		*/
		PLGUI_API void XEmbedFocusPrev();

		/**
		*  @brief
		*    Register a key accelerator
		*
		*  @param[in] nID
		*    Accelerator ID
		*  @param[in] nKey
		*    Key code
		*  @param[in] nModifiers
		*    Modifier keys
		*/
		PLGUI_API void XEmbedRegisterAccelerator(long nID, long nKey, long nModifiers);

		/**
		*  @brief
		*    Unregister a key accelerator
		*
		*  @param[in] nID
		*    Accelerator ID
		*/
		PLGUI_API void XEmbedUnregisterAccelerator(long nID);


	//[-------------------------------------------------------]
	//[ Public virtual TrayIconWidgetLinux functions          ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Window has been embedded by a XEMBED compliant embedder
		*
		*  @param[in] nEmbedderWindow
		*    Window handle of embedder (also called 'site' or 'socket')
		*  @param[in] nProtocolVersion
		*    Protocol version
		*/
		PLGUI_API virtual void OnXEmbedEmbeddedNotify(::Window nEmbedderWindow, long nProtocolVersion);

		/**
		*  @brief
		*    Window has been activated (e.g. got the focus)
		*/
		PLGUI_API virtual void OnXEmbedWindowActivate();

		/**
		*  @brief
		*    Window has been deactivated (e.g. lost the focus)
		*/
		PLGUI_API virtual void OnXEmbedWindowDeactivate();

		/**
		*  @brief
		*    Window has got the focus
		*
		*  @param[in] nFocus
		*    Where to move the logical focus to (XEMBED_FOCUS_CURRENT, XEMBED_FOCUS_FIRST or XEMBED_FOCUS_LAST)
		*/
		PLGUI_API virtual void OnXEmbedFocusIn(long nFocus);

		/**
		*  @brief
		*    Window has lost the focus
		*/
		PLGUI_API virtual void OnXEmbedFocusOut();

		/**
		*  @brief
		*    Window becomes shadowed by a modal dialog
		*/
		PLGUI_API virtual void OnXEmbedModalityOn();

		/**
		*  @brief
		*    Window has been released from a modal dialog
		*/
		PLGUI_API virtual void OnXEmbedModalityOff();

		/**
		*  @brief
		*    An accelerator has been activated
		*
		*  @param[in] nID
		*    Accelerator ID
		*  @param[in] nFlags
		*    Flags
		*/
		PLGUI_API virtual void OnXEmbedActivateAccelerator(long nID, long nFlags);


	//[-------------------------------------------------------]
	//[ Public virtual Widget functions                       ]
	//[-------------------------------------------------------]
	public:
		PLGUI_API virtual void OnDraw(PLGui::Graphics &cGraphics) override;
		PLGUI_API virtual void OnMouseMove(const PLMath::Vector2i &vPos) override;
		PLGUI_API virtual void OnMouseButtonDown(PLCore::uint32 nButton, const PLMath::Vector2i &vPos) override;
		PLGUI_API virtual void OnMouseButtonUp(PLCore::uint32 nButton, const PLMath::Vector2i &vPos) override;
		PLGUI_API virtual void OnMouseButtonClick(PLCore::uint32 nButton, const PLMath::Vector2i &vPos) override;
		PLGUI_API virtual void OnMouseButtonDoubleClick(PLCore::uint32 nButton, const PLMath::Vector2i &vPos) override;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Send message to XEMBED embedder window
		*
		*  @param[in] nMessage
		*    Message opcode
		*  @param[in] nDetail
		*    Message detail
		*  @param[in] nData1
		*    First parameter
		*  @param[in] nData2
		*    Second parameter
		*/
		void SendXEmbedMessage(long nMessage, long nDetail, long nData1, long nData2);

		/**
		*  @brief
		*    Send system tray message to XEMBED embedder window
		*
		*  @param[in] nNativeWindowHandle
		*    System native window handle
		*  @param[in] nMessage
		*    Message opcode
		*  @param[in] nDetail
		*    Message detail
		*  @param[in] nData1
		*    First parameter
		*  @param[in] nData2
		*    Second parameter
		*/
		void SendSysTrayMessage(::Window nNativeWindowHandle, long nMessage, long nDetail, long nData1, long nData2);

		/**
		*  @brief
		*    Send system tray data message to XEMBED embedder window
		*
		*  @param[in] nNativeWindowHandle
		*    System native window handle
		*  @param[in] pszString
		*    String data
		*  @param[in] nSize
		*    Size of string data (excluding the terminating zero), max. 20!
		*/
		void SendSysTrayMessageData(::Window nNativeWindowHandle, const char *pszString, long nSize);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		TrayIcon *m_pTrayIcon;			/**< Tray icon instance */
		::Window  m_nEmbedderWindow;	/**< Embedder window */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_TRAYICONWIDGETLINUX_H__
