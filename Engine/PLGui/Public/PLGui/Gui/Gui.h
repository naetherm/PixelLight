/*********************************************************\
 *  File: Gui.h                                          *
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


#ifndef __PLGUI_GUI_H__
#define __PLGUI_GUI_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include <PLCore/Base/Event/EventHandler.h>
#include <PLMath/Vector2i.h>
#include "PLGui/Gui/FontManager.h"
#include "PLGui/Gui/CursorManager.h"
#include "PLGui/Gui/ClipBoard.h"
#include "PLGui/PLGuiDefinitions.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLGui {
	class GuiMessage;
	class MessageFilter;
	class GuiImpl;
	class Screen;
	class Theme;
	class Widget;
	class WidgetImpl;
	class Graphics;
	class GraphicsImpl;
	class Image;
	class ImageImpl;
	class Font;
	class FontImpl;
	class Cursor;
	class CursorImpl;
	class TrayIcon;
	class TrayIconImpl;
	class ClipBoardImpl;
	class Timer;
	class Tooltip;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Main GUI class
*
*  @remarks
*    This class provides the main functions of a GUI system (desktop, widgets, ...)
*
*  @note
*    - Implementation of the bridge design pattern, this class is the abstraction
*/
class Gui : public PLCore::Object {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Widget;
	friend class Graphics;
	friend class Image;
	friend class Font;
	friend class Cursor;
	friend class TrayIcon;
	friend class ClipBoard;
	friend class Timer;


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, Gui, "PLGui", PLCore::Object, "Main GUI class")
		#ifdef PLGUI_EXPORTS	// The following is only required when compiling PLGui
			// Methods
			pl_method_1(SetMouseVisible,	pl_ret_type(void),	bool,	"Set the mouse cursor visibility. 'true' as first parameter if the mouse cursor shall be visible.",	"")
		#endif
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get system GUI
		*
		*  @return
		*    Pointer to system GUI
		*/
		static PLGUI_API Gui *GetSystemGui();


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sGui
		*    Name of the GUI backend that shall be used. Just leave this empty for the system GUI
		*
		*  @remarks
		*    To obtain a pointer to the system GUI, use GetSystemGui() instead of creating a new instance
		*/
		PLGUI_API Gui(const PLCore::String &sGui = "");

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~Gui();

		/**
		*  @brief
		*    Get implementation
		*
		*  @return
		*    Pointer to platform specific implementation
		*/
		PLGUI_API GuiImpl *GetImpl() const;

		/**
		*  @brief
		*    Shut down GUI and clean up
		*
		*  @remarks
		*    Call this for the system GUI at the end of your application, otherwise there
		*    might be trouble with the static deinitialization order!
		*/
		PLGUI_API void Shutdown();

		//[-------------------------------------------------------]
		//[ Control and message loop                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Check if the GUI is still active
		*
		*  @return
		*    'true', if the GUI is active, 'false' if the program shall quit
		*/
		PLGUI_API bool IsActive() const;

		/**
		*  @brief
		*    Exit application
		*/
		PLGUI_API void Exit();

		/**
		*  @brief
		*    Check if there are system messages waiting
		*
		*  @return
		*    'true' if there are messages in the queue
		*/
		PLGUI_API bool HasPendingMessages() const;

		/**
		*  @brief
		*    Process all waiting messages, blocks if no messages are waiting
		*/
		PLGUI_API void ProcessMessages();

		/**
		*  @brief
		*    Post message to the message queue
		*
		*  @param[in] cMessage
		*    Message to be posted
		*
		*  @remarks
		*    This function adds the given message to the message queue, so that it will be processed
		*    with subsequent call(s) to ProcessMessages(). To send a message directly (bypassing the
		*    message queue), see SendMessage().
		*/
		PLGUI_API void PostMessage(const GuiMessage &cMessage);

		/**
		*  @brief
		*    Send and process a message directly
		*
		*  @param[in] cMessage
		*    Message to be sent
		*
		*  @remarks
		*    This function sends a message directly to the GUI. Use this only if you know exactly, what you are
		*    doing, because this bypasses the message queue. Usually it is the right way to post a message to the
		*    queue for it to get processed in the right order (after calling ProcessMessages()).
		*/
		PLGUI_API void SendMessage(const GuiMessage &cMessage);

		/**
		*  @brief
		*    Wakeup message loop
		*
		*  @remarks
		*    This function posts a wakeup-message to the message loop, so that it wakes up
		*/
		PLGUI_API void WakeUp();

		/**
		*  @brief
		*    Get message filters
		*
		*  @return
		*    List of message filters
		*/
		PLGUI_API const PLCore::Container<MessageFilter*> &GetMessageFilters() const;

		/**
		*  @brief
		*    Add message filter
		*
		*  @param[in] pFilter
		*    Message filter, will be destroyed by Gui automatically
		*/
		PLGUI_API void AddMessageFilter(MessageFilter *pFilter);

		/**
		*  @brief
		*    Remove and destroy message filter
		*
		*  @param[in] pFilter
		*    Message filter
		*/
		PLGUI_API void RemoveMessageFilter(MessageFilter *pFilter);

		/**
		*  @brief
		*    Returns the root widget
		*
		*  @return
		*    Root widget, a null pointer on error
		*/
		PLGUI_API Widget *GetRootWidget() const;

		//[-------------------------------------------------------]
		//[ Screens                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get list of screens (monitors)
		*
		*  @return
		*    List of screens
		*/
		PLGUI_API const PLCore::Container<Screen*> &GetScreens() const;

		/**
		*  @brief
		*    Get screen by name
		*
		*  @param[in] sName
		*    Name of screen
		*
		*  @return
		*    Pointer to the specific screen, or a null pointer if that screen doesn't exist
		*/
		PLGUI_API Screen *GetScreen(const PLCore::String &sName) const;

		/**
		*  @brief
		*    Get default screen
		*
		*  @return
		*    Pointer to the default screen, never a null pointer
		*/
		PLGUI_API Screen *GetDefaultScreen() const;

		//[-------------------------------------------------------]
		//[ Resources                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get font manager
		*
		*  @return
		*    Font manager
		*/
		PLGUI_API FontManager &GetFontManager();

		/**
		*  @brief
		*    Get cursor manager
		*
		*  @return
		*    Cursor manager
		*/
		PLGUI_API CursorManager &GetCursorManager();

		/**
		*  @brief
		*    Get clipboard
		*
		*  @return
		*    Clipboard
		*/
		PLGUI_API ClipBoard &GetClipBoard();

		//[-------------------------------------------------------]
		//[ Gui info and state                                    ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Check if the GUI has a taskbar
		*
		*  @return
		*    'true', if the GUI has a taskbar, else 'false'
		*/
		PLGUI_API bool HasTaskbar() const;

		/**
		*  @brief
		*    Check if the mouse cursor is visible
		*
		*  @return
		*    'true' if the mouse cursor is visible, else 'false'
		*
		*  @note
		*    - If the mouse cursor is visible in general, it's still possible that it's
		*      invisible over some special widgets.
		*    - If the mouse cursor is invisible in general, it will NEVER be visible!
		*/
		PLGUI_API bool IsMouseVisible() const;

		/**
		*  @brief
		*    Set mouse cursor visibility
		*
		*  @param[in] bVisible
		*    Shall the mouse cursor be visible?
		*
		*  @see
		*    - IsMouseVisible()
		*/
		PLGUI_API void SetMouseVisible(bool bVisible);

		/**
		*  @brief
		*    Get widget that the mouse is currently in
		*
		*  @return
		*    Widget the mouse is currently in, or a null pointer
		*/
		PLGUI_API Widget *GetMouseOverWidget() const;

		/**
		*  @brief
		*    Get widget that currently has the focus
		*
		*  @return
		*    Widget that currently has the focus, or a null pointer
		*/
		PLGUI_API Widget *GetFocusWidget() const;

		/**
		*  @brief
		*    Display a tooltip
		*
		*  @param[in] vPos
		*    Position of tooltip
		*  @param[in] sTooltip
		*    Tooltip text
		*/
		PLGUI_API void ShowTooltip(const PLMath::Vector2i &vPos, const PLCore::String &sTooltip);

		//[-------------------------------------------------------]
		//[ Theme                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get theme
		*
		*  @return
		*    Pointer to current theme
		*/
		PLGUI_API Theme *GetTheme() const;

		/**
		*  @brief
		*    Set theme
		*
		*  @param[in] pTheme
		*    Theme
		*/
		PLGUI_API void SetTheme(Theme *pTheme);

		/**
		*  @brief
		*    Set theme
		*
		*  @param[in] sClass
		*    Theme class
		*  @param[in] sOptions
		*    Theme options
		*/
		PLGUI_API void SetTheme(const PLCore::String &sClass, const PLCore::String &sOptions);

		//[-------------------------------------------------------]
		//[ Options                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get time after which a hove event occurs
		*
		*  @return
		*    Timeout (in milliseconds)
		*/
		PLGUI_API PLCore::uint64 GetHoverTime() const;

		/**
		*  @brief
		*    Set time after which a hove event occurs
		*
		*  @param[in] nTimeout
		*    Timeout (in milliseconds)
		*
		*  @remarks
		*    If set to 0, hover events will be disabled completely.
		*/
		PLGUI_API void SetHoverTime(PLCore::uint64 nTimeout);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Get new widget ID
		*
		*  @return
		*    Widget ID for a newly created widget
		*/
		PLCore::uint32 GetUniqueWidgetID();

		/**
		*  @brief
		*    Delete destroyed widgets
		*
		*  @remarks
		*    Widgets, that are destroyed, will receive an OnDestroy-Event an have their system window destroyed
		*    immediately, while their actual Widget object remains intact for a while. This function actually
		*    deletes those Widgets. Usually, this is called occasionally by the GUI itself, therefor it should
		*    not be needed to call this directly.
		*/
		void DeleteDestroyedWidgets();

		/**
		*  @brief
		*    Add widget to list of top-level widgets
		*
		*  @param[in] pWidget
		*    Widget
		*/
		void AddTopLevelWidget(Widget *pWidget);

		/**
		*  @brief
		*    Add widget to list of wrapper widgets
		*
		*  @param[in] pWidget
		*    Widget
		*/
		void AddWrapperWidget(Widget *pWidget);

		/**
		*  @brief
		*    Add timer to list of active timers
		*
		*  @param[in] pTimer
		*    Timer
		*/
		void AddTimer(Timer *pTimer);

		/**
		*  @brief
		*    Remove timer from list of active timers
		*
		*  @param[in] pTimer
		*    Timer
		*/
		void RemoveTimer(Timer *pTimer);

		/**
		*  @brief
		*    Update mouse-over widget
		*/
		void UpdateMouseOverWidget();

		/**
		*  @brief
		*    Update focus widget
		*/
		void UpdateFocusWidget();

		/**
		*  @brief
		*    Callback for hover timer
		*/
		void OnHoverTimer();

		/**
		*  @brief
		*    Create a widget implementation
		*
		*  @param[in] cWidget
		*    Widget
		*
		*  @return
		*    Platform specific widget implementation
		*/
		WidgetImpl *CreateWidgetImpl(Widget &cWidget) const;

		/**
		*  @brief
		*    Create a graphics implementation
		*
		*  @param[in] cGraphics
		*    Graphics object
		*
		*  @return
		*    Platform specific graphics implementation
		*/
		GraphicsImpl *CreateGraphicsImpl(Graphics &cGraphics) const;

		/**
		*  @brief
		*    Create an image implementation
		*
		*  @param[in] cImage
		*    Image object
		*
		*  @return
		*    Platform specific image implementation
		*/
		ImageImpl *CreateImageImpl(Image &cImage) const;

		/**
		*  @brief
		*    Create a font implementation
		*
		*  @param[in] cFont
		*    Font object
		*
		*  @return
		*    Platform specific font implementation
		*/
		FontImpl *CreateFontImpl(Font &cFont) const;

		/**
		*  @brief
		*    Create a cursor implementation
		*
		*  @param[in] cCursor
		*    Cursor object
		*
		*  @return
		*    Platform specific cursor implementation
		*/
		CursorImpl *CreateCursorImpl(Cursor &cCursor) const;

		/**
		*  @brief
		*    Create a tray-icon implementation
		*
		*  @param[in] cTrayIcon
		*    TrayIcon object
		*
		*  @return
		*    Platform specific tray-icon implementation
		*/
		TrayIconImpl *CreateTrayIconImpl(TrayIcon &cTrayIcon) const;

		/**
		*  @brief
		*    Create a clipboard implementation
		*
		*  @param[in] cClipBoard
		*    ClipBoard object
		*
		*  @return
		*    Platform specific clipboard implementation
		*/
		ClipBoardImpl *CreateClipBoardImpl(ClipBoard &cClipBoard) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Data
		GuiImpl						 *m_pGuiImpl;				/**< Gui implementation, always valid! */
		bool						  m_bShutdown;				/**< Has the GUI shut down? */
		Theme						 *m_pTheme;					/**< Current theme */
		PLCore::uint64				  m_nHoverTime;				/**< Time in milliseconds before a hove event shall occur */
		FontManager					  m_cFontManager;			/**< Font manager */
		CursorManager				  m_cCursorManager;			/**< Cursor manager */
		ClipBoard					  m_cClipBoard;				/**< Clip board */
		bool						  m_bActive;				/**< 'true' if the GUI is active */
		PLCore::uint32				  m_nNextWidgetID;			/**< Sequential widget ID */
		PLCore::List<Screen*>		  m_lstScreens;				/**< List of available screens (monitors) */
		Screen						 *m_pDefaultScreen;			/**< Pointer to default (primary) screen */
		bool						  m_bMouseVisible;			/**< Is the mouse cursor visible? */
		Widget						 *m_pRootWidget;			/**< Dummy root widget */
		PLCore::List<Widget*>		  m_lstTopLevelWidgets;		/**< List of top-level widgets */
		PLCore::List<Widget*>		  m_lstWrapperWidgets;		/**< List of wrapper widgets */
		PLCore::List<Widget*>		  m_lstDestroyedWidgets;	/**< List of destroyed widgets */
		PLCore::List<Timer*>		  m_lstTimers;				/**< List of active timers */
		PLCore::List<MessageFilter*>  m_lstMessageFilters;		/**< Message filters */
		Widget						 *m_pMouseOverWidget;		/**< Widget that is currently selected by the mouse */
		Widget						 *m_pMouseOverWidgetNew;	/**< New mouse-over widget */
		Timer						 *m_pHoverTimer;			/**< Timer for hover messages */
		Widget						 *m_pFocusWidget;			/**< Widget that currently has the keyboard focus */
		Widget						 *m_pFocusWidgetNew;		/**< New focus widget */
		Tooltip						 *m_pTooltip;				/**< Tooltip widget */

		// Event handlers
		PLCore::EventHandler<>		  EventHandlerOnHoverTimer;	/**< Event handler for hover timer event */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_GUI_H__
