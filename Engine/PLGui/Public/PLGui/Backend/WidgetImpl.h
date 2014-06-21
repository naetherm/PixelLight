/*********************************************************\
 *  File: WidgetImpl.h                                   *
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


#ifndef __PLGUI_WIDGETIMPL_H__
#define __PLGUI_WIDGETIMPL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2i.h>
#include "PLGui/PLGui.h"
#include "PLGui/PLGuiDefinitions.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLGraphics {
	class Color4;
}
namespace PLGui {
	class Widget;
	class Graphics;
	class Screen;
	class Image;
	class Cursor;
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
*    Widget implementation base class
*
*  @remarks
*    This class is the base class for platform specific widget implementations
*
*  @note
*    - Implementation of the bridge design pattern, this class is the implementor of the 'Widget' abstraction
*/
class WidgetImpl {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cWidget
		*    Reference to platform independent widget
		*/
		PLGUI_API WidgetImpl(Widget &cWidget);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~WidgetImpl();


	//[-------------------------------------------------------]
	//[ Public virtual WidgetImpl functions                   ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Create widget
		*/
		virtual void CreateWidget() = 0;

		/**
		*  @brief
		*    Create wrapper widget
		*
		*  @param[in] nNativeWindowHandle
		*    System native window handle
		*/
		virtual void CreateWrapperWidget(PLCore::handle nNativeWindowHandle) = 0;

		/**
		*  @brief
		*    Check if the widget has been destroyed
		*
		*  @return
		*    'true' if the widget is destroyed, else 'false'
		*/
		virtual bool IsDestroyed() const = 0;

		/**
		*  @brief
		*    Destroy widget
		*/
		virtual void Destroy() = 0;

		/**
		*  @brief
		*    Get system native window handle
		*
		*  @return
		*    Native window handle
		*/
		virtual PLCore::handle GetNativeWindowHandle() const = 0;

		/**
		*  @brief
		*    Set parent widget
		*/
		virtual void SetParent(WidgetImpl *pParent) = 0;

		/**
		*  @brief
		*    Check if widget is visible in the taskbar
		*
		*  @return
		*    'true' if the widget is shown in the taskbar, else 'false'
		*/
		virtual bool IsShowInTaskbar() const = 0;

		/**
		*  @brief
		*    Set if widget is visible in the taskbar
		*
		*  @param[in] bShowInTaskbar
		*    'true' to show the widget in the taskbar, else 'false'
		*/
		virtual void SetShowInTaskbar(bool bShowInTaskbar) = 0;

		/**
		*  @brief
		*    Move widget to another screen
		*
		*  @param[in] pScreen
		*    Screen to which the widget shall be moved (does nothing if pScreen == nullptr)
		*/
		virtual void MoveToScreen(Screen &cScreen) = 0;

		/**
		*  @brief
		*    Get widget position
		*
		*  @return
		*    Position (X, Y)
		*/
		virtual PLMath::Vector2i GetPos() const = 0;

		/**
		*  @brief
		*    Set widget position
		*
		*  @param[in] vPos
		*    Position (X, Y)
		*/
		virtual void SetPos(const PLMath::Vector2i &vPos) = 0;

		/**
		*  @brief
		*    Get widget size
		*
		*  @return
		*    Size (width, height)
		*/
		virtual PLMath::Vector2i GetSize() const = 0;

		/**
		*  @brief
		*    Set widget size
		*
		*  @param[in] vSize
		*    Size (width, height)
		*/
		virtual void SetSize(const PLMath::Vector2i &vSize) = 0;

		/**
		*  @brief
		*    Set Z position
		*
		*  @param[in] nZPos
		*    Z position (see EZPos)
		*  @param[in] pWidget
		*    Other widget to align relative (only matters for 'Above' or 'Behind')
		*/
		virtual void SetZPos(EZPos nZPos, Widget *pWidget) = 0;

		/**
		*  @brief
		*    Get topmost state
		*
		*  @return
		*    'true', if the widget is topmost (always on top of all other widgets)
		*/
		virtual bool IsTopmost() const = 0;

		/**
		*  @brief
		*    Set topmost state
		*
		*  @param[in] bTopmost
		*    'true', if the widget is topmost (always on top of all other widgets)
		*/
		virtual void SetTopmost(bool bTopmost) = 0;

		/**
		*  @brief
		*    Get window state
		*
		*  @return
		*    Window state (see EWindowState)
		*/
		virtual EWindowState GetWindowState() const = 0;

		/**
		*  @brief
		*    Set window state
		*
		*  @param[in] nWindowState
		*    Window state (see EWindowState)
		*/
		virtual void SetWindowState(EWindowState nWindowState) = 0;

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
		virtual void SetTransparency(ETransparency nTransparency, const PLGraphics::Color4 &cColor) = 0;

		/**
		*  @brief
		*    Check if widget is visible
		*
		*  @return
		*    'true' if the widget is visible, else 'false'
		*/
		virtual bool IsVisible() const = 0;

		/**
		*  @brief
		*    Show or hide widget
		*
		*  @param[in] bVisible
		*    Is the widget visible?
		*/
		virtual void SetVisible(bool bVisible) = 0;

		/**
		*  @brief
		*    Check if widget is enabled
		*
		*  @return
		*    'true' if the widget is enabled, else 'false'
		*/
		virtual bool IsEnabled() const = 0;

		/**
		*  @brief
		*    Enable or disable widget
		*
		*  @param[in] bEnabled
		*    Is the widget enabled?
		*/
		virtual void SetEnabled(bool bEnabled) = 0;

		/**
		*  @brief
		*    Activate widget (put to foreground and set focus)
		*/
		virtual void Activate() = 0;

		/**
		*  @brief
		*    Set keyboard focus to the widget
		*/
		virtual void SetFocus() = 0;

		/**
		*  @brief
		*    Redraw widget
		*/
		virtual void Redraw() = 0;

		/**
		*  @brief
		*    Capture mouse input
		*
		*  @param[in] bCapture
		*    'true' if the mouse should be captured by the widget, else 'false'
		*/
		virtual void SetCaptureMouse(bool bCapture) = 0;

		/**
		*  @brief
		*    Trap mouse inside the widget
		*
		*  @param[in] bTrap
		*    'true' if the mouse should be trapped inside the widget, else 'false'
		*/
		virtual void SetTrapMouse(bool bTrap) = 0;

		/**
		*  @brief
		*    Set title
		*
		*  @param[in] sTitle
		*    Title
		*/
		virtual void SetTitle(const PLCore::String &sTitle) = 0;

		/**
		*  @brief
		*    Set icon
		*
		*  @param[in] cIcon
		*    Icon image
		*/
		virtual void SetIcon(const Image &cIcon) = 0;

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
		virtual PLCore::uint32 RegisterHotkey(PLCore::uint32 nModKey, PLCore::uint32 nKey) = 0;

		/**
		*  @brief
		*    Unregister a hotkey
		*
		*  @param[in] nID
		*    Hotkey ID
		*/
		virtual void UnregisterHotkey(PLCore::uint32 nID) = 0;

		/**
		*  @brief
		*    Set cursor
		*
		*  @param[in] cCursor
		*    Mouse cursor
		*/
		virtual void SetCursor(const Cursor &cCursor) = 0;

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
		virtual bool GetMousePos(PLMath::Vector2i &vPos) = 0;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Widget *m_pWidget;		/**< Pointer to the platform independent widget object */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_WIDGETIMPL_H__
