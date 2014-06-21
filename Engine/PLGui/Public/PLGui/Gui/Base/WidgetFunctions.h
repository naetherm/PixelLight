/*********************************************************\
 *  File: WidgetFunctions.h                              *
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


#ifndef __PLGUI_WIDGETFUNCTIONS_H__
#define __PLGUI_WIDGETFUNCTIONS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGui/PLGui.h"
#include "PLGui/PLGuiDefinitions.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLMath {
	class Vector2i;
}
namespace PLGui {
	class GuiMessage;
	class DataObject;
	class Widget;
	class Graphics;
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
*    Widget functions base class
*
*  @remarks
*    This base class is used for classes, that respond to widget events, such as Widget itself and Modifier
*/
class WidgetFunctions {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLGUI_API WidgetFunctions();

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~WidgetFunctions();


	//[-------------------------------------------------------]
	//[ Protected virtual WidgetFunctions functions           ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Process GUI message
		*
		*  @param[in] cMessage
		*    GUI message
		*
		*  @remarks
		*    Usually, you should not override this method, but rather use the virtual functions
		*    that are called after a specific message arrived (e.g. OnShow(), OnClose() etc.).
		*    If you decide to override this method, make sure to call the base implementation first,
		*    otherwise the messages will not be processed and no virtual functions or events will
		*    be called anymore.
		*/
		PLGUI_API virtual void OnMessage(const GuiMessage &cMessage);

		/**
		*  @brief
		*    Called when the theme has been changed
		*
		*  @remarks
		*    This event is caused, when the widget gets a new theme. Use
		*    this to perform all tasks necessary to adjust to the new theme,
		*    e.g. resizing certain child widgets or redrawing the widget.
		*/
		PLGUI_API virtual void OnThemeChanged();

		/**
		*  @brief
		*    Called when the widget has been changed
		*
		*  @remarks
		*    This event is caused, when the widget calls UpdateContent() to
		*    indicate that the widget has been changed in a way that could
		*    affect it's size or appearance. This is used to perform an
		*    AdjustContent() on the parent widget, whenever the size of a
		*    child widget could have been altered.
		*/
		PLGUI_API virtual void OnUpdateContent();

		/**
		*  @brief
		*    Called when a child widget has been changed
		*
		*  @remarks
		*    After an UpdateContent()-event has been called on a widget,
		*    it will cause an UpdateChildWidget()-event on it's parent widget.
		*    See OnUpdateContent() for further detail on UpdateContent()-events.
		*/
		PLGUI_API virtual void OnUpdateChildWidget(Widget *pWidget);

		/**
		*  @brief
		*    Called when a child window has been added
		*
		*  @param[in] pWidget
		*    Widget
		*/
		PLGUI_API virtual void OnAddChildWidget(Widget *pWidget);

		/**
		*  @brief
		*    Called when a child window has been removed
		*
		*  @param[in] pWidget
		*    Widget
		*/
		PLGUI_API virtual void OnRemoveChildWidget(Widget *pWidget);

		/**
		*  @brief
		*    Called when the widget shall be closed
		*/
		PLGUI_API virtual void OnClose();

		/**
		*  @brief
		*    Called after the widget has been created
		*/
		PLGUI_API virtual void OnCreate();

		/**
		*  @brief
		*    Called before the widget is destroyed
		*/
		PLGUI_API virtual void OnDestroy();

		/**
		*  @brief
		*    Called when the widget is shown
		*/
		PLGUI_API virtual void OnShow();

		/**
		*  @brief
		*    Called when the widget gets hidden
		*/
		PLGUI_API virtual void OnHide();

		/**
		*  @brief
		*    Called when the widget is enabled
		*/
		PLGUI_API virtual void OnEnable();

		/**
		*  @brief
		*    Called when the widget gets disabled
		*/
		PLGUI_API virtual void OnDisable();

		/**
		*  @brief
		*    Called when the widget gets the keyboard focus
		*/
		PLGUI_API virtual void OnGetFocus();

		/**
		*  @brief
		*    Called when the widget looses the keyboard focus
		*/
		PLGUI_API virtual void OnLooseFocus();

		/**
		*  @brief
		*    Called when the widget has been activated or deactivated (focus has changed)
		*
		*  @param[in] bActivate
		*    'true' if the widget itself or one of it's child widgets had the focus, else 'false'
		*/
		PLGUI_API virtual void OnActivate(bool bActivate);

		/**
		*  @brief
		*    Called to draw the widget background
		*
		*  @param[in] cGraphics
		*    Graphics object used for painting
		*/
		PLGUI_API virtual void OnDrawBackground(Graphics &cGraphics);

		/**
		*  @brief
		*    Called to draw the widget
		*
		*  @param[in] cGraphics
		*    Graphics object used for painting
		*/
		PLGUI_API virtual void OnDraw(Graphics &cGraphics);

		/**
		*  @brief
		*    Called when the widget gets moved
		*
		*  @param[in] vPos
		*    Position
		*/
		PLGUI_API virtual void OnMove(const PLMath::Vector2i &vPos);

		/**
		*  @brief
		*    Called when the widget gets resized
		*
		*  @param[in] vSize
		*    Size
		*/
		PLGUI_API virtual void OnSize(const PLMath::Vector2i &vSize);

		/**
		*  @brief
		*    Called when the window state has changed
		*
		*  @param[in] nWindowState
		*    Window state
		*/
		PLGUI_API virtual void OnWindowState(EWindowState nWindowState);

		/**
		*  @brief
		*    Called when fullscreen mode is activated
		*/
		PLGUI_API virtual void OnEnterFullscreen();

		/**
		*  @brief
		*    Called when fullscreen mode is deactivated
		*/
		PLGUI_API virtual void OnLeaveFullscreen();

		/**
		*  @brief
		*    Called when the widget is to calculate it's preferred size
		*
		*  @param[in] vRefSize
		*    Precalculated reference size, can be (-1, -1) if no reference size is available
		*
		*  @return
		*    Preferred size
		*
		*  @remarks
		*    If the widget returns -1 in a component (X/Y), it means that there is
		*    no preferred size in that direction. A layout will in that case use
		*    'as much space as possible', while the FitSize() functions will use
		*    the current size.
		*/
		PLGUI_API virtual PLMath::Vector2i OnPreferredSize(const PLMath::Vector2i &vRefSize) const;

		/**
		*  @brief
		*    Called when the widget content has to be adjusted
		*/
		PLGUI_API virtual void OnAdjustContent();

		/**
		*  @brief
		*    Called when the mouse has entered the widget
		*/
		PLGUI_API virtual void OnMouseEnter();

		/**
		*  @brief
		*    Called when the mouse has left the widget
		*/
		PLGUI_API virtual void OnMouseLeave();

		/**
		*  @brief
		*    Called when the mouse-over state of the widget has changed
		*
		*  @param[in] bMouseOver
		*    'true' if the mouse is over the widget or a child widget, else 'false'
		*/
		PLGUI_API virtual void OnMouseOver(bool bMouseOver);

		/**
		*  @brief
		*    Called when the mouse is moved within the widget or captured by it
		*
		*  @param[in] vPos
		*    Mouse position within the widget
		*/
		PLGUI_API virtual void OnMouseMove(const PLMath::Vector2i &vPos);

		/**
		*  @brief
		*    Called when the mouse hovers over the widget
		*/
		PLGUI_API virtual void OnMouseHover();

		/**
		*  @brief
		*    Called when the mouse position inside the widget has changed
		*
		*  @param[in] vPos
		*    Mouse position within the widget
		*
		*  @remarks
		*    This function is called, when the mouse position inside the widget has changed without
		*    the user moving the mouse. This is the case e.g. when the widget has been moved while the
		*    mouse was inside. Usually, you should only need OnMouseMove() to react on mouse movements,
		*    but when implementing widget movement, you must also react on OnMousePosUpdate() to update
		*    the new mouse position without doing any action. This is especially important in order to
		*    let several Widget-Modifiers work together without interfering each other.
		*/
		PLGUI_API virtual void OnMousePosUpdate(const PLMath::Vector2i &vPos);

		/**
		*  @brief
		*    Called when a mouse button is pressed
		*
		*  @param[in] nButton
		*    Mouse button that is pressed
		*  @param[in] vPos
		*    Mouse position within the widget
		*/
		PLGUI_API virtual void OnMouseButtonDown(PLCore::uint32 nButton, const PLMath::Vector2i &vPos);

		/**
		*  @brief
		*    Called when a mouse button is released
		*
		*  @param[in] nButton
		*    Mouse button that is released
		*  @param[in] vPos
		*    Mouse position within the widget
		*/
		PLGUI_API virtual void OnMouseButtonUp(PLCore::uint32 nButton, const PLMath::Vector2i &vPos);

		/**
		*  @brief
		*    Called when a mouse button is clicked
		*
		*  @param[in] nButton
		*    Mouse button that is clicked
		*  @param[in] vPos
		*    Mouse position within the widget
		*/
		PLGUI_API virtual void OnMouseButtonClick(PLCore::uint32 nButton, const PLMath::Vector2i &vPos);

		/**
		*  @brief
		*    Called when a mouse button is double-clicked
		*
		*  @param[in] nButton
		*    Mouse button that is double-clicked
		*  @param[in] vPos
		*    Mouse position within the widget
		*/
		PLGUI_API virtual void OnMouseButtonDoubleClick(PLCore::uint32 nButton, const PLMath::Vector2i &vPos);

		/**
		*  @brief
		*    Called when the mouse wheel is used
		*
		*  @param[in] nDelta
		*    Mouse wheel movement
		*/
		PLGUI_API virtual void OnMouseWheel(int nDelta);

		/**
		*  @brief
		*    Called when a key was pressed
		*
		*  @param[in] nKey
		*    Pressed key
		*  @param[in] nModifiers
		*    Modifier keys pressed
		*
		*  @note
		*    - Because of a possible autorepeat the key may be 'pressed' multiple times
		*      before the 'OnKeyUp'-function is called
		*/
		PLGUI_API virtual void OnKeyDown(PLCore::uint32 nKey, PLCore::uint32 nModifiers);

		/**
		*  @brief
		*    Called when a key was released
		*
		*  @param[in] nKey
		*    Released key
		*  @param[in] nModifiers
		*    Modifier keys pressed
		*/
		PLGUI_API virtual void OnKeyUp(PLCore::uint32 nKey, PLCore::uint32 nModifiers);

		/**
		*  @brief
		*    Called when a hotkey was pressed
		*
		*  @param[in] nHotkey
		*    Hotkey ID
		*/
		PLGUI_API virtual void OnHotkey(PLCore::uint32 nHotkey);

		/**
		*  @brief
		*    Called when data has been dropped onto the widget
		*
		*  @param[in] cData
		*    Data
		*/
		PLGUI_API virtual void OnDrop(const DataObject &cData);

		/**
		*  @brief
		*    Called when a user defined message has been sent
		*
		*  @param[in] nData
		*    Message data
		*  @param[in] pDataPtr
		*    Data pointer
		*/
		PLGUI_API virtual void OnUserMessage(PLCore::uint32 nData, void *pDataPtr);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_WIDGETFUNCTIONS_H__
