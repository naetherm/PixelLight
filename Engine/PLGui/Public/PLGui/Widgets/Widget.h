/*********************************************************\
 *  File: Widget.h                                       *
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


#ifndef __PLGUI_WIDGET_H__
#define __PLGUI_WIDGET_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGraphics/Color/Color4.h>
#include <PLMath/Vector2i.h>
#include <PLCore/Base/Object.h>
#include <PLCore/Base/Event/Event.h>
#include "PLGui/PLGuiDefinitions.h"
#include "PLGui/Gui/Data/LayoutHints.h"
#include "PLGui/Gui/Resources/Image.h"
#include "PLGui/Gui/Resources/Cursor.h"
#include "PLGui/Gui/Base/WidgetFunctions.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLMath {
	class Vector2i;
}
namespace PLGui {
	class Gui;
	class GuiMessage;
	class Screen;
	class Theme;
	class WidgetImpl;
	class Graphics;
	class DataObject;
	class Modifier;
	class Layout;
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
*    Widget class
*
*  @remarks
*    This class represents a basic widget
*
*  @note
*    - Implementation of the bridge design pattern, this class is the abstraction
*/
class Widget : public PLCore::Object, public WidgetFunctions {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Gui;
	friend class Screen;
	friend class WidgetImpl;


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, Widget, "PLGui", PLCore::Object, "Widget base class")
		// Attributes
		pl_attribute(ID,				PLCore::uint32,		0,						ReadWrite,	GetSet,	"Widget ID",		"")
		pl_attribute(Name,				PLCore::String,		"",						ReadWrite,	GetSet,	"Widget name",		"")
		pl_attribute(Title,				PLCore::String,		"No Title",				ReadWrite,	GetSet,	"Widget title",		"")
		pl_attribute(Pos,				PLMath::Vector2i,	PLMath::Vector2i(0, 0),	ReadWrite,	GetSet,	"Widget position",	"")
		pl_attribute(Size,				PLMath::Vector2i,	PLMath::Vector2i(0, 0),	ReadWrite,	GetSet,	"Widget size",		"")
		pl_attribute(Topmost,			bool,				false,					ReadWrite,	GetSet,	"Topmost state",	"")
		pl_attribute(BackgroundColor,	PLGraphics::Color4,	PLGraphics::Color4(),	ReadWrite,	GetSet,	"Background color",	"")
		#ifdef PLGUI_EXPORTS	// The following is only required when compiling PLGui
			// Constructors
			pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
			// Methods
			pl_method_0(GetGui,				pl_ret_type(Gui*),				"Get the owner GUI. Returns pointer to GUI object, can be a null pointer.",														"")
			pl_method_0(GetContentWidget,	pl_ret_type(Widget*),			"Get the content widget, can be a null pointer.",																				"")
			pl_method_1(SetTrapMouse,		pl_ret_type(void),		bool,	"Trap the mouse inside the widget. 'true' as first parameter if the mouse should be trapped inside the widget, else 'false'.",	"")
		#endif
		// Signals
		pl_signal_2(SignalUserMessage,				PLCore::uint32,				void*,						"User message, two custom parameters",																		"")
		pl_signal_0(SignalOnThemeChanged,																	"Theme has been changed",																					"")
		pl_signal_0(SignalUpdateContent,																	"Widget content has been changed",																			"")
		pl_signal_1(SignalUpdateChildWidget,		Widget*,												"Child widget has been changed, child widget as parameter",													"")
		pl_signal_1(SignalAddChildWidget,			Widget*,												"Child widget has been added, child widget as parameter",													"")
		pl_signal_1(SignalRemoveChildWidget,		Widget*,												"Child widget has been removed, child widget as parameter",													"")
		pl_signal_0(SignalClose,																			"Widget shall be closed (ALT+F4 or X-Button pressed)",														"")
		pl_signal_0(SignalCreate,																			"Widget has just been created",																				"")
		pl_signal_0(SignalDestroy,																			"Widget is going to be destroyed",																			"")
		pl_signal_0(SignalShow,																				"Widget gets shown",																						"")
		pl_signal_0(SignalHide,																				"Widget gets hidden",																						"")
		pl_signal_0(SignalEnable,																			"Widget gets enabled",																						"")
		pl_signal_0(SignalDisable,																			"Widget gets disabled",																						"")
		pl_signal_0(SignalGetFocus,																			"Widget gets focus",																						"")
		pl_signal_0(SignalLooseFocus,																		"Widget looses focus",																						"")
		pl_signal_1(SignalActivate,					bool,													"Widget has been activated or deactivated (focus-widget has changed), state as parameter",					"")
		pl_signal_1(SignalDrawBackground,			Graphics&,												"Widget background gets drawn, graphics object used for painting as parameter",								"")
		pl_signal_1(SignalDraw,						Graphics&,												"Widget gets drawn, graphics object used for painting as parameter",										"")
		pl_signal_1(SignalMove,						const PLMath::Vector2i&,								"Widget gets moved, new widget position as parameter",														"")
		pl_signal_1(SignalSize,						const PLMath::Vector2i&,								"Widget gets resized, new widget size as parameter",														"")
		pl_signal_1(SignalWindowState,				EWindowState,											"Window state has changed, new window state as parameter",													"")
		pl_signal_0(SignalEnterFullscreen,																	"Widget enters fullscreen mode",																			"")
		pl_signal_0(SignalLeaveFullscreen,																	"Widget leaves fullscreen mode",																			"")
		pl_signal_1(SignalPreferredSize,			const PLMath::Vector2i&,								"Widget has calculated it's preferred size, preferred size as parameter",									"")
		pl_signal_0(SignalAdjustContent,																	"Widget content has to be adjusted",																		"")
		pl_signal_0(SignalMouseEnter,																		"Mouse enters widget",																						"")
		pl_signal_0(SignalMouseLeave,																		"Mouse leaves widget",																						"")
		pl_signal_1(SignalMouseOver,				bool,													"Mouse-over widget has changed, state as parameter",														"")
		pl_signal_1(SignalMouseMove,				const PLMath::Vector2i&,								"Mouse moves inside the widget, mouse position within the widget as parameter",								"")
		pl_signal_0(SignalMouseHover,																		"Mouse hovers over the widget",																				"")
		pl_signal_1(SignalMousePosUpdate,			const PLMath::Vector2i&,								"Mouse position inside the widget has changed due to the movement of widget, position as parameter",		"")
		pl_signal_2(SignalMouseButtonDown,			PLCore::uint32,				const PLMath::Vector2i&,	"Mouse button is pressed, mouse button and mouse position within the widget as parameters",					"")
		pl_signal_2(SignalMouseButtonUp,			PLCore::uint32,				const PLMath::Vector2i&,	"Mouse button is released, mouse button and mouse position within the widget as parameters",				"")
		pl_signal_2(SignalMouseButtonClick,			PLCore::uint32,				const PLMath::Vector2i&,	"Mouse button has been clicked, mouse button and mouse position within the widget as parameters",			"")
		pl_signal_2(SignalMouseButtonDoubleClick,	PLCore::uint32,				const PLMath::Vector2i&,	"Mouse button has been double-clicked, mouse button and mouse position within the widget as parameters",	"")
		pl_signal_1(SignalMouseWheel,				int,													"Mouse wheel moved, mouse wheel movement as parameter",														"")
		pl_signal_2(SignalKeyDown,					PLCore::uint32,				PLCore::uint32,				"Key gets pressed, pressed key and modifier keys pressed as parameters",									"")
		pl_signal_2(SignalKeyUp,					PLCore::uint32,				PLCore::uint32,				"Key gets released, released key and modifier keys pressed as parameters",									"")
		pl_signal_1(SignalHotkey,					PLCore::uint32,											"Hotkey pressed, hotkey ID as parameter",																	"")
		pl_signal_1(SignalDrop,						const DataObject&,										"Data has been dropped onto the widget, data as parameter",													"")
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
		*  @param[in] bManaged
		*    If 'true', the widget is not automatically destroyed by Gui and will not appear in the list of top-level windows
		*
		*  @remarks
		*    If pParent == nullptr, the root widget of the system GUI is used as parent widget
		*/
		PLGUI_API Widget(Widget *pParent = nullptr, bool bManaged = false);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nNativeWindowHandle
		*    System native window handle
		*
		*  @remarks
		*    This constructor can be used to wrap an already existing system window into a PLGui Widget,
		*    there you need to pass a valid system handle for the window being wrapped. This only works
		*    with the system GUI, which is being used automatically.
		*/
		PLGUI_API Widget(PLCore::handle nNativeWindowHandle);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~Widget();

		/**
		*  @brief
		*    Get owner GUI
		*
		*  @return
		*    Pointer to GUI object
		*/
		PLGUI_API Gui *GetGui() const;

		/**
		*  @brief
		*    Get implementation
		*
		*  @return
		*    Pointer to platform specific implementation
		*/
		PLGUI_API WidgetImpl *GetImpl() const;

		/**
		*  @brief
		*    Get system native window handle
		*
		*  @return
		*    Native window handle
		*/
		PLGUI_API PLCore::handle GetNativeWindowHandle() const;

		//[-------------------------------------------------------]
		//[ Widget information                                    ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get ID
		*
		*  @return
		*    Widget ID
		*/
		PLGUI_API PLCore::uint32 GetID() const;

		/**
		*  @brief
		*    Set ID
		*
		*  @param[in] nID
		*    Widget ID
		*/
		PLGUI_API void SetID(PLCore::uint32 nID);

		/**
		*  @brief
		*    Get name
		*
		*  @return
		*    Widget name
		*/
		PLGUI_API PLCore::String GetName() const;

		/**
		*  @brief
		*    Set name
		*
		*  @param[in] sName
		*    Widget name
		*/
		PLGUI_API void SetName(const PLCore::String &sName);

		/**
		*  @brief
		*    Get descriptor
		*
		*  @return
		*    Descriptor for widget (Name, ID or window handle)
		*
		*  @remarks
		*    This functions returns a readable descriptor for the widget, which is
		*    either the name (if a name has been set), the ID (if set) or the window
		*    handle if all of the previous mentioned weren't available.
		*    Use this function e.g. to output a widget descriptor in debug messages.
		*/
		PLGUI_API PLCore::String GetDescriptor() const;

		/**
		*  @brief
		*    Get user defined data
		*
		*  @return
		*    User data
		*/
		PLGUI_API void *GetUserData() const;

		/**
		*  @brief
		*    Set user defined data
		*
		*  @param[in] pData
		*    User data
		*/
		PLGUI_API void SetUserData(void *pData);

		/**
		*  @brief
		*    Get whether or not the widget is managed
		*
		*  @return
		*    'true' if the widget is managed, else 'false'
		*/
		PLGUI_API bool IsManaged() const;

		//[-------------------------------------------------------]
		//[ Appearance (title, icon etc.)                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get title
		*
		*  @return
		*    Widget title
		*/
		PLGUI_API PLCore::String GetTitle() const;

		/**
		*  @brief
		*    Set title
		*
		*  @param[in] sTitle
		*    Widget title
		*/
		PLGUI_API void SetTitle(const PLCore::String &sTitle);

		/**
		*  @brief
		*    Get icon
		*
		*  @return
		*    Image of window icon
		*/
		PLGUI_API const Image &GetIcon() const;

		/**
		*  @brief
		*    Set icon
		*
		*  @param[in] cIcon
		*    Image of window icon
		*/
		PLGUI_API void SetIcon(const Image &cIcon);

		/**
		*  @brief
		*    Get cursor
		*
		*  @return
		*    Mouse cursor
		*/
		PLGUI_API const Cursor &GetCursor() const;

		/**
		*  @brief
		*    Set cursor
		*
		*  @param[in] cCursor
		*    Mouse cursor
		*/
		PLGUI_API void SetCursor(const Cursor &cCursor);

		//[-------------------------------------------------------]
		//[ Hierarchy                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get parent widget
		*
		*  @return
		*    Parent widget, can be a null pointer
		*/
		PLGUI_API Widget *GetParent() const;

		/**
		*  @brief
		*    Get common parent of this and another widget
		*
		*  @param[in] cWidget
		*    The other widget
		*
		*  @return
		*    The first widget that is parent of both widgets, or a null pointer if there is none
		*/
		PLGUI_API Widget *GetCommonParent(const Widget &cWidget) const;

		/**
		*  @brief
		*    Get top level widget that is parent of this widget
		*
		*  @return
		*    The top level parent widget, can be this widget
		*/
		PLGUI_API Widget *GetTopLevelWidget() const;

		/**
		*  @brief
		*    Get content widget
		*
		*  @return
		*    Content widget
		*
		*  @remarks
		*    If a widget is a container for other widgets, a content widget is often used as the parent for all of
		*    the child widgets. This is e.g. necessary to separate the "outside" from the "inside", e.g. there may be
		*    other (internal) child widgets, that are also children of the same parent (e.g. buttons in the title bar or
		*    frame widgets). So if you want to insert widgets inside other widgets, you should use the widget return by
		*    GetContentWidget() as the parent widget, not the widget itself. If a widget don't have a content widget,
		*    it will return a pointer to itself as the content widget.
		*/
		PLGUI_API virtual Widget *GetContentWidget() const;

		/**
		*  @brief
		*    Get list of children
		*
		*  @return
		*    List of child widgets
		*/
		PLGUI_API const PLCore::List<Widget*> &GetChildren() const;

		/**
		*  @brief
		*    Get first child widget
		*
		*  @return
		*    Child widget, or a null pointer
		*/
		PLGUI_API Widget *GetFirstChild() const;

		/**
		*  @brief
		*    Get last child widget
		*
		*  @return
		*    Child widget, or a null pointer
		*/
		PLGUI_API Widget *GetLastChild() const;

		/**
		*  @brief
		*    Get previous sibling widget
		*
		*  @return
		*    Sibling widget, or a null pointer
		*/
		PLGUI_API Widget *GetPreviousSibling() const;

		/**
		*  @brief
		*    Get next sibling widget
		*
		*  @return
		*    Sibling widget, or a null pointer
		*/
		PLGUI_API Widget *GetNextSibling() const;

		/**
		*  @brief
		*    Check if widget is a top level window
		*
		*  @return
		*    'true' if the widget is a top level window (has no other parent than the desktop)
		*/
		PLGUI_API bool IsTopLevel() const;

		//[-------------------------------------------------------]
		//[ Position and appearance                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Check if widget is visible in the taskbar
		*
		*  @return
		*    'true' if the widget is visible in the taskbar, else 'false'
		*
		*  @note
		*    - If the system doesn't support a taskbar, the function will always return 'false'
		*/
		PLGUI_API bool IsShowInTaskbar() const;

		/**
		*  @brief
		*    Set if widget is visible in the taskbar
		*
		*  @param[in] bShowInTaskbar
		*    'true' if the widget is visible in the taskbar, else 'false'
		*
		*  @see
		*    - IsShowInTaskbar()
		*/
		PLGUI_API void SetShowInTaskbar(bool bShowInTaskbar);

		/**
		*  @brief
		*    Get screen the widget is on
		*
		*  @return
		*    Pointer to the screen that contains this widget
		*/
		PLGUI_API Screen *GetScreen() const;

		/**
		*  @brief
		*    Move widget to another screen
		*
		*  @param[in] cScreen
		*    Screen to which the widget shall be moved
		*/
		PLGUI_API void MoveToScreen(Screen &cScreen);

		/**
		*  @brief
		*    Get widget position
		*
		*  @return
		*    Position (X, Y)
		*/
		PLGUI_API PLMath::Vector2i GetPos() const;

		/**
		*  @brief
		*    Set widget position
		*
		*  @param[in] vPos
		*    Position (X, Y)
		*/
		PLGUI_API void SetPos(const PLMath::Vector2i &vPos);

		/**
		*  @brief
		*    Get widget size
		*
		*  @return
		*    Size (width, height)
		*/
		PLGUI_API PLMath::Vector2i GetSize() const;

		/**
		*  @brief
		*    Set widget size
		*
		*  @param[in] vSize
		*    Size (width, height)
		*/
		PLGUI_API void SetSize(const PLMath::Vector2i &vSize);

		/**
		*  @brief
		*    Check if widget is topmost (always above all other widgets)
		*
		*  @return
		*    'true' if the widget is topmost, else 'false'
		*/
		PLGUI_API bool GetTopmost() const;

		/**
		*  @brief
		*    Set if widget is topmost (always above all other widgets)
		*
		*  @param[in] bTopmost
		*    'true' if the widget is topmost, else 'false'
		*
		*  @note
		*    - Whenever possible, do not use topmost, this may backfire when you don't expect it
		*/
		PLGUI_API void SetTopmost(bool bTopmost);

		/**
		*  @brief
		*    Get absolute widget position
		*
		*  @return
		*    Position (X, Y)
		*/
		PLGUI_API PLMath::Vector2i GetAbsPos() const;

		/**
		*  @brief
		*    Get position relative to another widget
		*
		*  @param[in] cWidget
		*    Widget to which the relative position shall be computed
		*
		*  @return
		*    Position relative to the given widget (X, Y)
		*/
		PLGUI_API PLMath::Vector2i GetRelativePos(Widget &cWidget) const;

		/**
		*  @brief
		*    Center widget
		*
		*  @param[in] nCenter
		*    How to center the widget (see ECenter)
		*/
		PLGUI_API void Center(ECenter nCenter);

		/**
		*  @brief
		*    Center widget relative to the screen
		*
		*  @param[in] cScreen
		*    Screen to which this widget is centered
		*/
		PLGUI_API void CenterOnScreen(Screen &cScreen);

		/**
		*  @brief
		*    Center widget relative to another widget
		*
		*  @param[in] cWidget
		*    Widget to which this widget is centered
		*/
		PLGUI_API void CenterOnWidget(Widget &cWidget);

		/**
		*  @brief
		*    Calculate preferred size
		*
		*  @param[in] vRefSize
		*    Precalculated reference size, can be (-1, -1) if no reference size is available
		*
		*  @remarks
		*    This causes the widget to calculate it's preferred size by calling the
		*    virtual function OnPreferredSize() and stores the result.
		*    To retrieve the preferred size of a widget, call GetPreferredSize().
		*/
		PLGUI_API void CalculatePreferredSize(const PLMath::Vector2i &vRefSize = PLMath::Vector2i(-1, -1));

		/**
		*  @brief
		*    Get preferred size
		*
		*  @return
		*    Preferred size
		*
		*  @remarks
		*    This return the preferred size of a widget. If the widget does not do it itself (which it should),
		*    you have to call CalculatePreferredSize(), in order to calculate the preferred size before retrieving it.
		*/
		PLGUI_API PLMath::Vector2i GetPreferredSize() const;

		/**
		*  @brief
		*    Set size of widget to fit it's content
		*
		*  @remarks
		*    This sets the widget's size to the preferred size (see CalculatePreferredSize())
		*/
		PLGUI_API void FitSize();

		/**
		*  @brief
		*    Set Z position
		*
		*  @param[in] nZPos
		*    Z position (see EZPos)
		*  @param[in] pWidget
		*    Other widget used to align relative to (only matters for 'ZAbove' or 'ZBehind')
		*/
		PLGUI_API void SetZPos(EZPos nZPos, Widget *pWidget = nullptr);

		/**
		*  @brief
		*    Get window state
		*
		*  @return
		*    Window state (see EWindowState)
		*/
		PLGUI_API EWindowState GetWindowState() const;

		/**
		*  @brief
		*    Set window state
		*
		*  @param[in] nWindowState
		*    Window state (see EWindowState)
		*/
		PLGUI_API void SetWindowState(EWindowState nWindowState);

		/**
		*  @brief
		*    Get background color
		*
		*  @return
		*    Background color
		*
		*  @see
		*    SetBackgroundColor()
		*/
		PLGUI_API PLGraphics::Color4 GetBackgroundColor() const;

		/**
		*  @brief
		*    Set background color
		*
		*  @param[in] cColor
		*    Background color
		*
		*  @remarks
		*    The background color is used to automatically fill the background of the widget whenever
		*    the background is painted. If you don't want a background to be painted or if you
		*    paint it yourself (see OnDrawBackground), set this color to Color4::Transparent.
		*/
		PLGUI_API void SetBackgroundColor(const PLGraphics::Color4 &cColor);

		/**
		*  @brief
		*    Set transparency mode
		*
		*  @param[in] nTransparency
		*    Transparency mode (see ETransparency)
		*  @param[in] cColor
		*    If nMode is ColorKey, all pixels of the specified color will be transparent.
		*    If nMode is Alpha, only the alpha channel of the color will be used (1.0 = opaque, 0.0 = transparent)
		*/
		PLGUI_API void SetTransparency(ETransparency nTransparency, const PLGraphics::Color4 &cColor);

		//[-------------------------------------------------------]
		//[ State                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Close widget
		*
		*  @remarks
		*    "Closing a widget" means sending it a message, that it shall close. This message is sent
		*    automatically if you press the 'X'-Button or e.g. with "ALT+F4" on Windows. The default
		*    behavior of widgets is: nothing. The default behavior of windows is: destroying the window.
		*    You can control the behavior by overriding OnClose() or hooking into SignalClose.
		*/
		PLGUI_API void Close();

		/**
		*  @brief
		*    Check if widget has been destroyed
		*
		*  @return
		*    'true' if the widget is destroyed, else 'false'
		*/
		PLGUI_API bool IsDestroyed() const;

		/**
		*  @brief
		*    Destroy widget
		*/
		PLGUI_API void Destroy();

		/**
		*  @brief
		*    Check if widget is visible
		*
		*  @return
		*    'true' if the widget is visible, else 'false'
		*/
		PLGUI_API bool IsVisible() const;

		/**
		*  @brief
		*    Show or hide widget
		*
		*  @param[in] bVisible
		*    'true' shows the widget, 'false' hides it
		*
		*  @note
		*    - When a widget is created, it is invisible by default
		*/
		PLGUI_API void SetVisible(bool bVisible);

		/**
		*  @brief
		*    Check if widget is enabled
		*
		*  @return
		*    'true' if the widget is enabled, else 'false'
		*/
		PLGUI_API bool IsEnabled() const;

		/**
		*  @brief
		*    Enable or disable widget
		*
		*  @param[in] bEnabled
		*    'true' if the widget is enabled, else 'false'
		*/
		PLGUI_API void SetEnabled(bool bEnabled);

		/**
		*  @brief
		*    Get focus style
		*
		*  @return
		*    Focus style(s)
		*/
		PLGUI_API EFocusStyle GetFocusStyle() const;

		/**
		*  @brief
		*    Set focus style
		*
		*  @param[in] nFocusStyle
		*    Focus style(s)
		*
		*  @remarks
		*    The focus style determines how a widget is able to get the focus.
		*    If this is set to NoFocus (as by default), a widget will never get the focus.
		*    See EFocusStyle for other possible values.
		*/
		PLGUI_API void SetFocusStyle(EFocusStyle nFocusStyle);

		/**
		*  @brief
		*    Check if widget can accept the focus
		*
		*  @return
		*    'true' if widget can get the focus, else 'false'
		*
		*  @remarks
		*    Returns 'true' if the focus style is set to AcceptFocus, ChildFocus or ChildFocusOrSelf
		*    and the widget is not disabled.
		*/
		PLGUI_API bool CanAcceptFocus() const;

		/**
		*  @brief
		*    Get tab-stop
		*
		*  @return
		*    'true' if widget can get the focus by tabbing, else 'false'
		*/
		PLGUI_API bool IsTabStop();

		/**
		*  @brief
		*    Set tab-stop
		*
		*  @param[in] bTabStop
		*    'true' if widget can get the focus by tabbing, else 'false'
		*/
		PLGUI_API void SetTabStop(bool bTabStop);

		/**
		*  @brief
		*    Check if widget has the focus
		*
		*  @return
		*    'true' if this widget has the focus, else 'false'
		*/
		PLGUI_API bool HasFocus() const;

		/**
		*  @brief
		*    Set keyboard focus to the widget
		*
		*  @return
		*    'true' if keyboard focus is accepted by this widget or one of it's child widgets, else 'false'
		*
		*  @remarks
		*    This function should be called by a GUI backend in order to set the keyboard focus
		*    to this widget. The widget will then decide on base of it's parameter, if it accepts
		*    the keyboard focus, or if it delegates it to a child widget. It will then call
		*    SetFocus() from the backend, to finally set the focus. So don't call this function again
		*    when receiving the event caused by SetFocus(), otherwise it will end in an endless loop.
		*/
		PLGUI_API bool SetFocus();

		/**
		*  @brief
		*    Check if widget is active (either the widget or one of it's children has the focus)
		*
		*  @return
		*    'true' if this widget is active, else 'false'
		*/
		PLGUI_API bool IsActive() const;

		/**
		*  @brief
		*    Activate a widget (bring it to the top and set the focus to it)
		*/
		PLGUI_API void Activate();

		/**
		*  @brief
		*    Give keyboard focus to next tab-stop widget
		*
		*  @param[in] bForward
		*    'true' to go forward, 'false' to go backwards
		*
		*  @return
		*    'true' if a tab-stop widget could be found, else 'false'
		*/
		PLGUI_API bool NextTabStop(bool bForward);

		/**
		*  @brief
		*    Check if the mouse is currently inside the widget
		*
		*  @return
		*    'true' if this widget is the mouse-over widget, else 'false'
		*/
		PLGUI_API bool IsMouseIn() const;

		/**
		*  @brief
		*    Check if the mouse is currently over either the widget or one of it's child widgets
		*
		*  @return
		*    'true' if mouse-over, else 'false'
		*/
		PLGUI_API bool IsMouseOver() const;

		/**
		*  @brief
		*    Get current mouse cursor position inside the widget
		*
		*  @param[out] vPos
		*    Receives the mouse cursor position if all went fine, on error, the given variable is not manipulated
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe the mouse cursor is currently not over the widget?)
		*/
		PLGUI_API bool GetMousePos(PLMath::Vector2i &vPos) const;

		/**
		*  @brief
		*    Get widget state
		*
		*  @return
		*    Widget states as flags (see EWidgetState)
		*
		*  @remarks
		*    This returns the current states of the widget together as a single flags-variable
		*    containing a combination of the values defined by EWidgetState. Not that this virtual
		*    function can be overwritten in derived classes to modify it's behavior according to
		*    the widget or add new states (e.g. WidgetPressed for buttons)
		*/
		PLGUI_API virtual PLCore::uint32 GetWidgetState() const;

		/**
		*  @brief
		*    Redraw widget
		*/
		PLGUI_API void Redraw();

		/**
		*  @brief
		*    Redraw widget and all child widgets
		*/
		PLGUI_API void RedrawAll();

		//[-------------------------------------------------------]
		//[ Input                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Check if widget has captured mouse input
		*
		*  @return
		*    'true' if the mouse is captured by the widget, else 'false'
		*/
		PLGUI_API bool IsCaptureMouse() const;

		/**
		*  @brief
		*    Capture mouse input
		*
		*  @param[in] bCapture
		*    'true' if the mouse should be captured by the widget, else 'false'
		*/
		PLGUI_API void SetCaptureMouse(bool bCapture);

		/**
		*  @brief
		*    Trap mouse inside the widget
		*
		*  @param[in] bTrap
		*    'true' if the mouse should be trapped inside the widget, else 'false'
		*/
		PLGUI_API void SetTrapMouse(bool bTrap);

		/**
		*  @brief
		*    Register a hotkey
		*
		*  @param[in] nModKey
		*    Modification key (for example PLGUIMOD_ALT)
		*  @param[in] nKey
		*    Key (for example PLGUIKEY_RETURN)
		*
		*  @return
		*    Hotkey ID, 0 on error
		*/
		PLGUI_API PLCore::uint32 RegisterHotkey(PLCore::uint32 nModKey, PLCore::uint32 nKey);

		/**
		*  @brief
		*    Unregister a hotkey
		*
		*  @param[in] nID
		*    Hotkey ID
		*/
		PLGUI_API void UnregisterHotkey(PLCore::uint32 nID);

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

		//[-------------------------------------------------------]
		//[ Modifiers                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get modifiers
		*
		*  @return
		*    List of modifiers that belong to the widget
		*/
		PLGUI_API const PLCore::List<Modifier*> &GetModifiers() const;

		/**
		*  @brief
		*    Get modifier
		*
		*  @param[in] sName
		*    Name of modifier
		*
		*  @remarks
		*    Modifier, or a null pointer if no modifier with that name exists
		*/
		PLGUI_API Modifier *GetModifier(const PLCore::String &sName) const;

		/**
		*  @brief
		*    Add modifier
		*
		*  @param[in] pModifier
		*    Pointer to the new modifier
		*
		*  @remarks
		*    The widget will take over the modifier and also destroy it later
		*/
		PLGUI_API void AddModifier(Modifier *pModifier);

		/**
		*  @brief
		*    Add modifier
		*
		*  @param[in] sName
		*    Name of modifier (can be empty)
		*  @param[in] pModifier
		*    Pointer to the new modifier
		*
		*  @remarks
		*    The widget will take over the modifier and also destroy it later.
		*    The name of the modifier is optional and can be left empty, if a name is provided, it must be unique.
		*    If a name is provided and there is already a modifier with the same name, the old modifier is
		*    destroyed and replaced by the new one.
		*/
		PLGUI_API void AddModifier(const PLCore::String &sName, Modifier *pModifier);

		/**
		*  @brief
		*    Add modifier
		*
		*  @param[in] sClass
		*    Name of the modifier class
		*  @param[in] sOptions
		*    Options for the modifier
		*/
		PLGUI_API void AddModifier(const PLCore::String &sClass, const PLCore::String &sOptions);

		/**
		*  @brief
		*    Add modifier
		*
		*  @param[in] sName
		*    Name of modifier (can be empty)
		*  @param[in] sClass
		*    Name of the modifier class
		*  @param[in] sOptions
		*    Options for the modifier
		*
		*  @remarks
		*    The name of the modifier is optional and can be left empty, if a name is provided, it must be unique.
		*    If a name is provided and there is already a modifier with the same name, the old modifier is
		*    destroyed and replaced by the new one.
		*/
		PLGUI_API void AddModifier(const PLCore::String &sName, const PLCore::String &sClass, const PLCore::String &sOptions);

		/**
		*  @brief
		*    Remove modifier
		*
		*  @param[in] pModifier
		*    Pointer to the modifier
		*
		*  @remarks
		*    The widget will remove and delete the modifier
		*/
		PLGUI_API void RemoveModifier(Modifier *pModifier);

		/**
		*  @brief
		*    Remove modifier
		*
		*  @param[in] sName
		*    Name of modifier
		*
		*  @remarks
		*    The widget will remove and delete the modifier
		*/
		PLGUI_API void RemoveModifier(const PLCore::String &sName);

		//[-------------------------------------------------------]
		//[ Layout                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get layout
		*
		*  @return
		*    Layout
		*
		*  @remarks
		*    As a layout is just a modifier, this is equal to GetModifier("Layout")
		*/
		PLGUI_API Layout *GetLayout() const;

		/**
		*  @brief
		*    Set layout
		*
		*  @param[in] pLayout
		*    Layout
		*/
		PLGUI_API void SetLayout(Layout *pLayout);

		/**
		*  @brief
		*    Set layout
		*
		*  @param[in] sClass
		*    Layout class
		*  @param[in] sOptions
		*    Layout options
		*/
		PLGUI_API void SetLayout(const PLCore::String &sClass, const PLCore::String &sOptions);

		/**
		*  @brief
		*    Get layout hints
		*
		*  @remarks
		*    For widgets that are positioned by a layout manager, additional hints can be set for the layout
		*    manager on how to layout the specific widget. These options include e.g. the minimum and maximum
		*    size, hints on when and how to resize the widget etc. Note that it depends on the layout manager,
		*    which (if any) of the layout hints it uses to modify the layout behavior.
		*/
		PLGUI_API LayoutHints &GetLayoutHints();

		/**
		*  @brief
		*    Adjust content of the widget
		*/
		PLGUI_API void AdjustContent();

		/**
		*  @brief
		*    Update content
		*
		*  @remarks
		*    This function has to be called by a widget whenever it has been changed
		*    in a way that could alter it's appearance or size. E.G. if the text of a label
		*    has been altered, it will most certainly change the size of the text label on
		*    the screen. To indicate this, the widget has to call UpdateContent() after such a
		*    change, so that the layout of the parent widget can be updated accordingly.
		*/
		PLGUI_API void UpdateContent();


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pGui
		*    Pointer to the parent GUI
		*
		*  @remarks
		*    This create a dummy widget for the 'root' of a GUI. This widget is not a real widget, so
		*    don't call anything that would need a real widget backend to be present. It is only used
		*    to server as a parent for new widgets and should not be used for anything else. Also note
		*    that if you pass a root widget as parent, it will be used to obtain the GUI pointer but
		*    than be discarded (afterwards, GetParent() of the new widget will return a null pointer)
		*/
		PLGUI_API Widget(Gui *pGui);

		/**
		*  @brief
		*    Add child widget
		*
		*  @param[in] pWidget
		*    Widget to add
		*/
		PLGUI_API void AddChild(Widget *pWidget);

		/**
		*  @brief
		*    Remove child widget
		*
		*  @param[in] pWidget
		*    Widget to remove
		*/
		PLGUI_API void RemoveChild(Widget *pWidget);

		/**
		*  @brief
		*    Give focus to first tab-stop widget (including this and all children)
		*
		*  @return
		*    'true' if a tab-stop widget could be found, else 'false'
		*/
		PLGUI_API bool ActivateFirstTabStop();

		/**
		*  @brief
		*    Give focus to last tab-stop widget (including this and all children)
		*
		*  @return
		*    'true' if a tab-stop widget could be found, else 'false'
		*/
		PLGUI_API bool ActivateLastTabStop();

		/**
		*  @brief
		*    Draw background
		*
		*  @param[in] cGraphics
		*    Graphics object used for painting
		*/
		PLGUI_API void DrawBackground(Graphics &cGraphics);

		/**
		*  @brief
		*    Draw widget
		*
		*  @param[in] cGraphics
		*    Graphics object used for painting
		*/
		PLGUI_API void Draw(Graphics &cGraphics);


	//[-------------------------------------------------------]
	//[ Protected virtual WidgetFunctions functions           ]
	//[-------------------------------------------------------]
	protected:
		PLGUI_API virtual void OnMessage(const GuiMessage &cMessage) override;
		PLGUI_API virtual PLMath::Vector2i OnPreferredSize(const PLMath::Vector2i &vRefSize) const override;
		PLGUI_API virtual void OnThemeChanged() override;
		PLGUI_API virtual void OnUpdateContent() override;
		PLGUI_API virtual void OnAdjustContent() override;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Gui											 *m_pGui;				/**< Pointer to GUI instance */
		bool										  m_bManaged;			/**< If a widget is managed, it will not be listed in the list of top level widgets and not be deleted automatically! */
		bool										  m_bRootWidget;		/**< If 'true', the widget is a dummy object for the 'root' of a GUI implementation (no real widget at all!) */
		PLCore::uint32								  m_nID;				/**< Widget ID */
		PLCore::String								  m_sName;				/**< Widget name */
		Widget										 *m_pParent;			/**< Pointer to parent widget */
		WidgetImpl									 *m_pWidgetImpl;		/**< Widget implementation */
		Theme										 *m_pTheme;				/**< Current theme */
		PLCore::List<Widget*>						  m_lstChildren;		/**< List of child widgets */
		PLCore::List<Modifier*>						  m_lstModifiers;		/**< List of modifiers */
		PLCore::HashMap<PLCore::String, Modifier*>    m_mapModifiers;		/**< Map 'Name' -> 'Modifier' */
		Layout										 *m_pLayout;			/**< Layout */
		LayoutHints									  m_cLayoutHints;		/**< Layout hints for the widget */
		EWindowState								  m_nWindowState;		/**< Current window state */
		PLMath::Vector2i							  m_vPos;				/**< Position of the widget */
		PLMath::Vector2i							  m_vSize;				/**< Size of the widget */
		bool										  m_bTopmost;			/**< Topmost flag */
		PLCore::String								  m_sTitle;				/**< Widget title */
		Image										  m_cIcon;				/**< Widget icon */
		Cursor										  m_cCursor;			/**< Mouse cursor */
		EFocusStyle									  m_nFocusStyle;		/**< How does this widget accept the keyboard focus? */
		bool										  m_bTabStop;			/**< Does the widget accept the focus by tabbing? */
		bool										  m_bCaptureMouse;		/**< Is the mouse captured by the widget? */
		PLGraphics::Color4							  m_cBackgroundColor;	/**< Background color of the widget */
		PLMath::Vector2i							  m_vPreferredSize;		/**< Preferred size of the widget */
		void										 *m_pUserData;			/**< User defined data */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_WIDGET_H__
