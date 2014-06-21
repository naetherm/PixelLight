/*********************************************************\
 *  File: PLGuiDefinitions.h                             *
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


#ifndef __PLGUI_DEFINITIONS_H__
#define __PLGUI_DEFINITIONS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Rtti.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ GUI messages                                          ]
//[-------------------------------------------------------]
/**
*  @brief
*    Message type
*/
enum EMessageType {
	MessageOnUnknown = 0,				/**< Unknown message type */
	MessageOnInternalMessage,			/**< Internal message that is not passed on to widgets and modifiers */
	MessageOnWakeup,					/**< Wakeup message loop */
	MessageOnExit,						/**< Exit GUI */
	MessageOnTimer,						/**< Timer message */
	MessageOnThemeChanged,				/**< Theme has been changed */
	MessageOnUpdateContent,				/**< Widget content has been changed */
	MessageOnUpdateChildWidget,			/**< Child widget has been changed */
	MessageOnAddChildWidget,			/**< Child widget has been added */
	MessageOnRemoveChildWidget,			/**< Child widget has been removed */
	MessageOnClose,						/**< Widget shall be closed (ALT+F4 or X-Button pressed) */
	MessageOnCreate,					/**< Widget has just been created */
	MessageOnDestroy,					/**< Widget is going to be destroyed */
	MessageOnShow,						/**< Widget gets shown */
	MessageOnHide,						/**< Widget gets hidden */
	MessageOnEnable,					/**< Widget gets enabled */
	MessageOnDisable,					/**< Widget gets disabled */
	MessageOnGetFocus,					/**< Widget gets focus */
	MessageOnLooseFocus,				/**< Widget looses focus */
	MessageOnActivate,					/**< Widget has been activated or deactivated (focus-widget has changed) */
	MessageOnDrawBackground,			/**< Widget background gets drawn, graphics object used for painting as parameter */
	MessageOnDraw,						/**< Widget gets drawn, graphics object used for painting as parameter */
	MessageOnMove,						/**< Widget gets moved, new widget position as parameter */
	MessageOnSize,						/**< Widget gets resized, new widget size as parameter */
	MessageOnWindowState,				/**< Window state has changed, new window state as parameter */
	MessageOnEnterFullscreen,			/**< Widget enters fullscreen mode */
	MessageOnLeaveFullscreen,			/**< Widget leaves fullscreen mode */
	MessageOnPreferredSize,				/**< Widget has calculated it's preferred size */
	MessageOnAdjustContent,				/**< Widget content has to be adjusted */
	MessageOnMouseEnter,				/**< Mouse enters widget */
	MessageOnMouseLeave,				/**< Mouse leaves widget */
	MessageOnMouseOver,					/**< Mouse-over widget has changed */
	MessageOnMouseMove,					/**< Mouse moves inside the widget, mouse position within the widget as parameter */
	MessageOnMouseHover,				/**< Mouse hovers over the widget */
	MessageOnMousePosUpdate,			/**< Mouse position inside the widget has changed due to the movement of widget */
	MessageOnMouseButtonDown,			/**< Mouse button is pressed, mouse button and mouse position within the widget as parameters */
	MessageOnMouseButtonUp,				/**< Mouse button is released, mouse button and mouse position within the widget as parameters */
	MessageOnMouseButtonClick,			/**< Mouse button has been clicked, mouse button and mouse position within the widget as parameters */
	MessageOnMouseButtonDoubleClick,	/**< Mouse button has been double-clicked, mouse button and mouse position within the widget as parameters */
	MessageOnMouseWheel,				/**< Mouse wheel moved, mouse wheel movement as parameter */
	MessageOnKeyDown,					/**< Key gets pressed, pressed key and modifier keys pressed as parameters */
	MessageOnKeyUp,						/**< Key gets released, released key and modifier keys pressed as parameters */
	MessageOnHotkey,					/**< Hotkey pressed, hotkey ID as parameter */
	MessageOnDrop,						/**< Data has been dropped onto the widget */
	MessageOnUserMessage				/**< User message */
};

/**
*  @brief
*    Internal message types
*/
enum EInternalMessage {
	InternalDummy = 1					/**< Unused, replace with first real internal message */
};

/**
*  @brief
*    Message type
*/
enum EMessageFilter {
	FilterInclusive = 0,				/**< Pass all specified messages, ignore all others */
	FilterExclusive						/**< Pass all messages except the specified ones */
};

/**
*  @brief
*    Debug modes
*/
enum EDebugMode {
	DebugModeOff = 0,					/**< Display no messages */
	DebugModeFocus,						/**< Display messages regarding the keyboard focus */
	DebugModeAll						/**< Display all messages */
};


//[-------------------------------------------------------]
//[ General definitions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Orientation
*/
enum EOrientation {
	Horizontal = 0,		/**< Horizontal orientation (X-axis) */
	Vertical			/**< Vertical orientation (Y-axis) */
};

// RTTI definition
pl_enum(EOrientation)
	pl_enum_value(Horizontal, "Horizontal orientation")
	pl_enum_value(Vertical,   "Vertical orientation")
pl_enum_end

/**
*  @brief
*    Side
*/
enum ESide {
	SideLeft = 0,		/**< Left side */
	SideRight,			/**< Right side */
	SideTop,			/**< Top side */
	SideBottom			/**< Bottom side */
};

// RTTI definition
pl_enum(ESide)
	pl_enum_value(SideLeft,		"Left side")
	pl_enum_value(SideRight,	"Right side")
	pl_enum_value(SideTop,		"Top side")
	pl_enum_value(SideBottom,	"Bottom side")
pl_enum_end

/**
*  @brief
*    Alignment
*/
enum EAlign {
	AlignLeft = 0,		/**< Align elements at the left side */
	AlignRight,			/**< Align elements at the right side */
	AlignCenter,		/**< Align elements centered */
};

// RTTI definition
pl_enum(EAlign)
	pl_enum_value(AlignLeft,	"Align elements at the left side")
	pl_enum_value(AlignRight,	"Align elements at the right side")
	pl_enum_value(AlignCenter,	"Align elements centered")
pl_enum_end

/**
*  @brief
*    Vertical alignment
*/
enum EVAlign {
	AlignTop = 0,		/**< Align elements at the top */
	AlignBottom,		/**< Align elements at the bottom */
	AlignMiddle,		/**< Align elements centered */
};

// RTTI definition
pl_enum(EVAlign)
	pl_enum_value(AlignTop,		"Align elements at the top")
	pl_enum_value(AlignBottom,	"Align elements at the bottom")
	pl_enum_value(AlignMiddle,	"Align elements centered")
pl_enum_end

/**
*  @brief
*    Border styles
*/
enum EBorderStyle {
	FlatBorder = 0,		/**< A flat border */
	RaisedBorder,		/**< A border with a raised 3D look */
	SunkenBorder		/**< A border with a sunken 3D look */
};

// RTTI definition
pl_enum(EBorderStyle)
	pl_enum_value(FlatBorder,	"A flat border")
	pl_enum_value(RaisedBorder,	"A border with a raised 3D look")
	pl_enum_value(SunkenBorder,	"A border with a sunken 3D look")
pl_enum_end

/**
*  @brief
*    Line styles
*/
enum ELineStyle {
	SolidLine = 0,		/**< A solid line */
	DashedLine,			/**< A line that is dashed (many small lines) */
	DottedLine			/**< A line that is dotted (many dots) */
};

// RTTI definition
pl_enum(ELineStyle)
	pl_enum_value(SolidLine,	"A solid line")
	pl_enum_value(DashedLine,	"A line that is dashed (many small lines)")
	pl_enum_value(DottedLine,	"A line that is dotted (many dots)")
pl_enum_end


//[-------------------------------------------------------]
//[ Widgets                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Widget state
*/
enum EWidgetState {
	WidgetEnabled	=  1,	/**< Widget is enabled */
	WidgetActive	=  2,	/**< Widget is active */
	WidgetFocus		=  4,	/**< Widget is inactive  */
	WidgetMouseOver	=  8,	/**< Widget is selected by the mouse */
	WidgetPressed	= 16	/**< Widget is selected by the mouse and mouse button is pressed (only valid for buttons!) */
};

/**
*  @brief
*    Window state
*/
enum EWindowState {
	StateNormal,		/**< Window is displayed as a normal window */
	StateMinimized,		/**< Window is minimized */
	StateMaximized,		/**< Window is maximized */
	StateFullscreen		/**< Window is in fullscreen mode */
};

/**
*  @brief
*    Z position of window
*/
enum EZPos {
	ZAbove,			/**< Bring a window on top of another window */
	ZBehind,		/**< Bring a window behind another window */
	ZTop,			/**< Bring a window on top of all other windows */
	ZBottom			/**< Bring a window behind all other windows */
};

/**
*  @brief
*    Center modes
*/
enum ECenter {
	CenterScreen,	/**< Center a window relative to the screen */
	CenterParent	/**< Center a window relative to it's parent window */
};

/**
*  @brief
*    Transparency modes
*/
enum ETransparency {
	NoTransparency,			/**< No transparency */
	ColorKeyTransparency,	/**< Use a color key to mark transparent areas */
	AlphaTransparency		/**< Use alpha blending */
};

/**
*  @brief
*    Focus styles
*/
enum EFocusStyle {
	NoFocus = 0,		/**< Widget will not accept the focus at all */
	ChildFocus,			/**< Widget will try to give the focus to a child widget */
	ChildFocusOrSelf,	/**< Widget will first try to give the focus to a child widget, then accept it itself*/
	AcceptFocus			/**< Widget will accept the keyboard focus */
};

/**
*  @brief
*    System buttons
*/
enum ESystemButton {
	SystemButtonMinimize = 1,	/**< Minimize button */
	SystemButtonMaximize = 2,	/**< Maximize button */
	SystemButtonClose	 = 4	/**< Close button */
};

// RTTI definition
pl_enum(ESystemButton)
	pl_enum_value(SystemButtonMinimize,	"Minimize button")
	pl_enum_value(SystemButtonMaximize,	"Maximize button")
	pl_enum_value(SystemButtonClose,	"Close button")
pl_enum_end

/**
*  @brief
*    System commands
*/
enum ESystemCommand {
	CommandMinimize,	/**< Minimize window */
	CommandMaximize,	/**< Maximize window */
	CommandRestore,		/**< Restore window size */
	CommandClose		/**< Close window */
};

// RTTI definition
pl_enum(ESystemCommand)
	pl_enum_value(CommandMinimize,	"Minimize window")
	pl_enum_value(CommandMaximize,	"Maximize window")
	pl_enum_value(CommandRestore,	"Restore window size")
	pl_enum_value(CommandClose,		"Close window")
pl_enum_end

/**
*  @brief
*    ScrollBar styles
*/
enum EScrollBarStyle {
	ScrollBarStyleNever = 0,	/**< Display no scrollbar at all */
	ScrollBarStyleAlways,		/**< Show scrollbar always */
	ScrollBarStyleDynamic		/**< Hide scrollbar when it is not needed */
};

// RTTI definition
pl_enum(EScrollBarStyle)
	pl_enum_value(ScrollBarStyleNever,		"Display no scrollbar at all")
	pl_enum_value(ScrollBarStyleAlways,		"Show scrollbar always")
	pl_enum_value(ScrollBarStyleDynamic,	"Hide scrollbar when it is not needed")
pl_enum_end


//[-------------------------------------------------------]
//[ Mouse definitions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Mouse buttons
*/
enum EMouseButton {
	LeftButton = 0,		/**< Left mouse button */
	RightButton,		/**< Right mouse button */
	MiddleButton,		/**< Middle mouse button */
	Button0 = 0,		/**< Button 0 (Left) */
	Button1,			/**< Button 1 (Right) */
	Button2,			/**< Button 2 (Middle) */
	Button3,			/**< Button 3 */
	Button4,			/**< Button 4 */
	Button5,			/**< Button 5 */
	Button6,			/**< Button 6 */
	Button7,			/**< Button 7 */
	Button8,			/**< Button 8 */
	Button9,			/**< Button 9 */
	Button10,			/**< Button 10 */
	Button11,			/**< Button 11 */
	Button12,			/**< Button 12 */
	Button13,			/**< Button 13 */
	Button14,			/**< Button 14 */
	Button15			/**< Button 15 */
};


//[-------------------------------------------------------]
//[ Mouse cursors                                         ]
//[-------------------------------------------------------]
/**
*  @brief
*    Standard mouse cursors
*/
enum EMouseCursor {
	CursorNone,						/**< No cursor (invisible) */
	CursorArrow,					/**< Standard arrow */
	CursorUpArrow,					/**< Up arrow */
	CursorWaitStart,				/**< Startup wait sign (small hourglass) */
	CursorWait,						/**< Wait sign (hourglass) */
	CursorHelp,						/**< Help cursor */
	CursorNo,						/**< Slashed circle */
	CursorCrosshair,				/**< Crosshair */
	CursorHand,						/**< Hand */
	CursorText,						/**< Text cursor */
	CursorMove,						/**< Moving (all directions) */
	CursorSize,						/**< Sizing (all directions) */
	CursorSizeTop,					/**< Sizing (up) */
	CursorSizeBottom,				/**< Sizing (down) */
	CursorSizeLeft,					/**< Sizing (left) */
	CursorSizeRight,				/**< Sizing (right) */
	CursorSizeTopBottom,			/**< Sizing (up and down) */
	CursorSizeLeftRight,			/**< Sizing (left and right) */
	CursorSizeTopLeft,				/**< Sizing (top left) */
	CursorSizeTopRight,				/**< Sizing (top right) */
	CursorSizeBottomLeft,			/**< Sizing (bottom left) */
	CursorSizeBottomRight,			/**< Sizing (bottom right) */
	CursorSizeTopLeftBottomRight,	/**< Sizing (top left and bottom right) */
	CursorSizeTopRightBottomLeft,	/**< Sizing (top right and bottom left) */
	CursorCustom,					/**< A custom (image) cursor is used */
	Cursor_Count
};


//[-------------------------------------------------------]
//[ Fonts and text                                        ]
//[-------------------------------------------------------]
/**
*  @brief
*    Font weight
*
*  @remarks
*    Font weight in a scale from 0..100
*/
enum EFontWeight {
	WeightLight	 = 25,		/**< Light text */
	WeightNormal = 50,		/**< Normal text */
	WeightBold	 = 75		/**< Bold text */
};

// RTTI definition
pl_enum(EFontWeight)
	pl_enum_value(WeightLight,	"Light text")
	pl_enum_value(WeightNormal,	"Normal text")
	pl_enum_value(WeightBold,	"Bold text")
pl_enum_end

/**
*  @brief
*    Font style
*/
enum EFontStyle {
	StyleNormal = 0,		/**< Normal text */
	StyleItalics			/**< Italics text */
};

// RTTI definition
pl_enum(EFontStyle)
	pl_enum_value(StyleNormal,	"Normal text")
	pl_enum_value(StyleItalics,	"Italics text")
pl_enum_end

/**
*  @brief
*    Text style
*/
enum ETextStyle {
	UnderlineText = 1,	/**< Underline the text */
	CrossoutText  = 2	/**< Cross out the text */
};

// RTTI definition
pl_enum(ETextStyle)
	pl_enum_value(UnderlineText, "Underline the text")
	pl_enum_value(CrossoutText,	 "Cross out the text")
pl_enum_end

/**
*  @brief
*    Text wrapping
*/
enum ETextWrap {
	NoTextWrap = 0,		/**< Do not wrap text */
	TextWrap,			/**< Wrap text, if the text does not fit on a line, and use standard alignment */
	TextWrapBlock		/**< Wrap text and use block text to align it */
};

// RTTI definition
pl_enum(ETextWrap)
	pl_enum_value(NoTextWrap,	 "Do not wrap text")
	pl_enum_value(TextWrap,		 "Wrap text, if the text does not fit on a line, and use standard alignment")
	pl_enum_value(TextWrapBlock, "Wrap text and use block text to align it")
pl_enum_end


//[-------------------------------------------------------]
//[ Button                                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Check state
*/
enum ECheckState {
	NotChecked = 0,		/**< The check-button is unchecked */
	Checked,			/**< The check-button is checked */
	PartiallyChecked	/**< The check-button is partial checked */
};

// RTTI definition
pl_enum(ECheckState)
	pl_enum_value(NotChecked,		"The check-button is unchecked")
	pl_enum_value(Checked,			"The check-button is checked")
	pl_enum_value(PartiallyChecked, "The check-button is partial checked")
pl_enum_end


//[-------------------------------------------------------]
//[ Menu                                                  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Menu item type
*/
enum EMenuItemType {
	TypeMenuItem = 0,	/**< A usual menu item */
	TypeMenu,			/**< A (sub)menu */
	TypeSeparator		/**< A separator */
};


//[-------------------------------------------------------]
//[ TabBar                                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    TabBar options
*/
enum ETabBarOption {
	TabOptionCreateTab	= 1,	/**< Allow user to create a new tab */
	TabOptionCloseTab	= 2,	/**< Allow user to close a tab */
	TabOptionMoveTab	= 4,	/**< Allow user to move a tab */
	TabOptionStaticSize = 8		/**< Each tab has the same static size */
};

/**
*  @brief
*    TabBar functions
*/
enum ETabBarFunction {
	TabFunctionCreateTab = 0,	/**< Create new tab */
	TabFunctionCloseTab,		/**< Close tab */
	TabFunctionScrollNext,		/**< Scroll (next) */
	TabFunctionScrollPrev		/**< Scroll (previous) */
};


//[-------------------------------------------------------]
//[ MessageBox                                            ]
//[-------------------------------------------------------]
/**
*  @brief
*    Message box types
*/
enum EMessageBox {
	MessageBoxOk = 0,		/**< Message box with button  'Ok' */
	MessageBoxOkCancel,		/**< Message box with buttons 'Ok', 'Cancel' */
	MessageBoxYesNo,		/**< Message box with buttons 'Yes', 'No' */
	MessageBoxYesNoCancel,	/**< Message box with buttons 'Yes', 'No', 'Cancel' */
};

/**
*  @brief
*    Message box buttons
*/
enum EMessageBoxButton {
	MessageBoxButtonCancel = -1,	/**< Message box button 'Cancel' */
	MessageBoxButtonOk = 0,			/**< Message box button 'Ok' */
	MessageBoxButtonYes,			/**< Message box button 'Yes' */
	MessageBoxButtonNo				/**< Message box button 'No' */
};


//[-------------------------------------------------------]
//[ Data definitions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Data type
*/
enum EDataType {
	DataEmpty = 0,		/**< Empty data */
	DataString,			/**< String data */
	DataFiles,			/**< List of file names */
	DataCustom,			/**< Custom data (uint32 value) */
	DataBinary			/**< Binary data (byte buffer and size) */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_DEFINITIONS_H__
