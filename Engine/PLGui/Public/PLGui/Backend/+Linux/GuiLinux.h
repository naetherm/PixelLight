/*********************************************************\
 *  File: GuiLinux.h                                     *
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


#ifndef __PLGUI_GUILINUX_H__
#define __PLGUI_GUILINUX_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Array.h>
#include "PLGui/Backend/GuiImpl.h"
#include "PLGui/PLGuiLinuxIncludes.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class WidgetLinux;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Linux GUI implementation
*/
class GuiLinux : public GuiImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class WidgetLinux;
	friend class GraphicsLinux;
	friend class TrayIconLinux;
	friend class TrayIconWidgetLinux;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pGui
		*    Platform independent GUI
		*/
		PLGUI_API GuiLinux(Gui *pGui);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~GuiLinux();

		/**
		*  @brief
		*    Get display
		*
		*  @return
		*    Display
		*/
		PLGUI_API Display *GetDisplay() const;

		/**
		*  @brief
		*    Keep track of modifier keys (shift, control and alt)
		*
		*  @param[in] nKey
		*    Key code of modifier key
		*  @param[in] bPressed
		*    'true' if key is pressed down, else 'false'
		*/
		PLGUI_API void TrackModifierKeys(int nKey, bool bPressed);

		/**
		*  @brief
		*    Get current state of modifier keys (shift, control and alt)
		*
		*  @return
		*    Bitmask of currently pressed modifier keys
		*
		*  @remarks
		*    See Keys.h for symbolic values of modifier keys
		*/
		PLGUI_API PLCore::uint32 GetModifierKeys();


	//[-------------------------------------------------------]
	//[ Public virtual GuiImpl functions                      ]
	//[-------------------------------------------------------]
	public:
		virtual bool HasPendingMessages() override;
		virtual void ProcessMessage() override;
		virtual void PostMessage(const GuiMessage &cMessage) override;
		virtual void EnumerateScreens(PLCore::List<Screen*> &lstScreens) override;
		virtual PLMath::Vector2i GetScreenSize() const override;
		virtual bool HasTaskbar() override;
		virtual void SetMouseVisible(bool bVisible) override;
		virtual void ListFonts(PLCore::List<FontInfo> &lstFonts) const override;
		virtual WidgetImpl *CreateWidgetImpl(Widget &cWidget) const override;
		virtual GraphicsImpl *CreateGraphicsImpl(Graphics &cGraphics) const override;
		virtual ImageImpl *CreateImageImpl(Image &cImage) const override;
		virtual FontImpl *CreateFontImpl(Font &cFont) const override;
		virtual CursorImpl *CreateCursorImpl(Cursor &cCursor) const override;
		virtual TrayIconImpl *CreateTrayIconImpl(TrayIcon &cTrayIcon) const override;
		virtual ClipBoardImpl *CreateClipBoardImpl(ClipBoard &cClipBoard) const override;


	//[-------------------------------------------------------]
	//[ Private static functions                              ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    X error handler
		*
		*  @param[in] pDisplay
		*    Display
		*  @param[in] pError
		*    Error descriptor
		*
		*  @return
		*    Ignored
		*/
		static int ErrorHandler(Display *pDisplay, XErrorEvent *pError);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    X event procedure
		*
		*  @param[in] pEvent
		*    Pointer to an X-event
		*/
		PLGUI_API void ProcessXEvent(XEvent *pEvent);

		/**
		*  @brief
		*    Destroy a widget and it's child widgets
		*
		*  @param[in] pWidget
		*    Widget
		*/
		PLGUI_API void DestroyWidget(Widget *pWidget);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Used client protocols
		struct {
			::Atom Close;							/**< Message for closing the window */
			::Atom Focus;							/**< Message for taking the focus */
			::Atom Timer;							/**< Message for timer events */
			::Atom Exit;							/**< Message for quitting the GUI */
			::Atom Wakeup;							/**< Message for waking up the message loop */
			::Atom DestroyWidget;					/**< Message for destroying a widget (delayed) */
			::Atom ThemeChanged;					/**< Message for changing the theme of a widget */
		} m_sClientProtocols;

		// Other used atoms
		struct {
			::Atom UTF8_STRING;						/**< Atom for the type of a window title */
			::Atom WM_NAME;							/**< Window title (old?) */
			::Atom _NET_WM_NAME;					/**< Window title */
			::Atom _NET_WM_VISIBLE_NAME;			/**< Window title (visible title, can be different) */
			::Atom _NET_WM_STATE;					/**< Window state */
			::Atom _NET_WM_STATE_FULLSCREEN;		/**< Fullscreen mode */
			::Atom _NET_WM_STATE_MAXIMIZED_HORZ;	/**< Maximize horizontally */
			::Atom _NET_WM_STATE_MAXIMIZED_VERT;	/**< Maximize vertically */
			::Atom _NET_WM_STATE_HIDDEN;			/**< Minimize window */
			::Atom _NET_WM_STATE_SKIP_TASKBAR;		/**< Do not show window in taskbar */
			::Atom _NET_WM_STATE_ABOVE;				/**< Topmost state */
			::Atom _XEMBED;							/**< XEMBED protocol */
			::Atom _XEMBED_INFO;					/**< XEMBED information */
			::Atom _NET_SYSTEM_TRAY_OPCODE;			/**< System tray protocol */
			::Atom _NET_SYSTEM_TRAY_MESSAGE_DATA;	/**< System tray message data */
		} m_sAtoms;

		// System handles
		::Display *m_pDisplay;						/**< X display */
		::Window   m_nHiddenWindow;					/**< Hidden window for internal messages */

		// Modifier keys
		bool	   m_bModifierShift;				/**< Is the SHIFT key pressed? */
		bool	   m_bModifierCtrl;					/**< Is the CTRL key pressed? */
		bool	   m_bModifierAlt;					/**< Is the ALT key pressed? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_GUILINUX_H__
