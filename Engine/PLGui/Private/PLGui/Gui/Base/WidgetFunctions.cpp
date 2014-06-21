/*********************************************************\
 *  File: WidgetFunctions.cpp                            *
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
#include <PLMath/Vector2i.h>
#include "PLGui/Gui/Base/WidgetFunctions.h"


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_PUSH
PL_WARNING_DISABLE(4355) // "'this' : used in base member initializer list"


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
WidgetFunctions::WidgetFunctions()
{
}

/**
*  @brief
*    Destructor
*/
WidgetFunctions::~WidgetFunctions()
{
}


//[-------------------------------------------------------]
//[ Protected virtual WidgetFunctions functions           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Process GUI message
*/
void WidgetFunctions::OnMessage(const GuiMessage &cMessage)
{
}

/**
*  @brief
*    Called when the widget has been changed
*/
void WidgetFunctions::OnUpdateContent()
{
}

/**
*  @brief
*    Called when the theme has been changed
*/
void WidgetFunctions::OnThemeChanged()
{
}

/**
*  @brief
*    Called when a child widget has been changed
*/
void WidgetFunctions::OnUpdateChildWidget(Widget *pWidget)
{
}

/**
*  @brief
*    Called when a child window has been added
*/
void WidgetFunctions::OnAddChildWidget(Widget *pWidget)
{
}

/**
*  @brief
*    Called when a child window has been removed
*/
void WidgetFunctions::OnRemoveChildWidget(Widget *pWidget)
{
}

/**
*  @brief
*    Called when the widget shall be closed
*/
void WidgetFunctions::OnClose()
{
}

/**
*  @brief
*    Called after the widget has been created
*/
void WidgetFunctions::OnCreate()
{
}

/**
*  @brief
*    Called before the widget is destroyed
*/
void WidgetFunctions::OnDestroy()
{
}

/**
*  @brief
*    Called when the widget is shown
*/
void WidgetFunctions::OnShow()
{
}

/**
*  @brief
*    Called when the widget gets hidden
*/
void WidgetFunctions::OnHide()
{
}

/**
*  @brief
*    Called when the widget is enabled
*/
void WidgetFunctions::OnEnable()
{
}

/**
*  @brief
*    Called when the widget gets disabled
*/
void WidgetFunctions::OnDisable()
{
}

/**
*  @brief
*    Called when the widget gets the keyboard focus
*/
void WidgetFunctions::OnGetFocus()
{
}

/**
*  @brief
*    Called when the widget looses the keyboard focus
*/
void WidgetFunctions::OnLooseFocus()
{
}

/**
*  @brief
*    Called when the widget has been activated or deactivated (focus has changed)
*/
void WidgetFunctions::OnActivate(bool bActivate)
{
}

/**
*  @brief
*    Called to draw the widget background
*/
void WidgetFunctions::OnDrawBackground(Graphics &cGraphics)
{
}

/**
*  @brief
*    Called to draw the widget
*/
void WidgetFunctions::OnDraw(Graphics &cGraphics)
{
}

/**
*  @brief
*    Called when the widget gets moved
*/
void WidgetFunctions::OnMove(const Vector2i &vPos)
{
}

/**
*  @brief
*    Called when the widget gets resized
*/
void WidgetFunctions::OnSize(const Vector2i &vSize)
{
}

/**
*  @brief
*    Called when the window state has changed
*/
void WidgetFunctions::OnWindowState(EWindowState nWindowState)
{
}

/**
*  @brief
*    Called when fullscreen mode is activated
*/
void WidgetFunctions::OnEnterFullscreen()
{
}

/**
*  @brief
*    Called when fullscreen mode is deactivated
*/
void WidgetFunctions::OnLeaveFullscreen()
{
}

/**
*  @brief
*    Called when the widget is to calculate it's preferred size
*/
Vector2i WidgetFunctions::OnPreferredSize(const Vector2i &vRefSize) const
{
	// Return (-1, -1) as default size to use "as much space as possible" in layouts
	return Vector2i(-1, -1);
}

/**
*  @brief
*    Called when the widget content has to be adjusted
*/
void WidgetFunctions::OnAdjustContent()
{
}

/**
*  @brief
*    Called when the mouse has entered the widget
*/
void WidgetFunctions::OnMouseEnter()
{
}

/**
*  @brief
*    Called when the mouse has left the widget
*/
void WidgetFunctions::OnMouseLeave()
{
}

/**
*  @brief
*    Called when the mouse-over widget has changed
*/
void WidgetFunctions::OnMouseOver(bool bMouseOver)
{
}

/**
*  @brief
*    Called when the mouse is moved within the widget or captured by it
*/
void WidgetFunctions::OnMouseMove(const Vector2i &vPos)
{
}

/**
*  @brief
*    Called when the mouse hovers over the widget
*/
void WidgetFunctions::OnMouseHover()
{
}

/**
*  @brief
*    Called when the mouse position inside the widget has changed
*/
void WidgetFunctions::OnMousePosUpdate(const Vector2i &vPos)
{
}

/**
*  @brief
*    Called when a mouse button is pressed
*/
void WidgetFunctions::OnMouseButtonDown(uint32 nButton, const Vector2i &vPos)
{
}

/**
*  @brief
*    Called when a mouse button is released
*/
void WidgetFunctions::OnMouseButtonUp(uint32 nButton, const Vector2i &vPos)
{
}

/**
*  @brief
*    Called when a mouse button is clicked
*/
void WidgetFunctions::OnMouseButtonClick(uint32 nButton, const Vector2i &vPos)
{
}

/**
*  @brief
*    Called when a mouse button is double-clicked
*/
void WidgetFunctions::OnMouseButtonDoubleClick(uint32 nButton, const Vector2i &vPos)
{
}

/**
*  @brief
*    Called when the mouse wheel is used
*/
void WidgetFunctions::OnMouseWheel(int nDelta)
{
}

/**
*  @brief
*    Called when a key was pressed
*/
void WidgetFunctions::OnKeyDown(uint32 nKey, uint32 nModifiers)
{
}

/**
*  @brief
*    Called when a key was released
*/
void WidgetFunctions::OnKeyUp(uint32 nKey, uint32 nModifiers)
{
}

/**
*  @brief
*    Called when a hotkey was pressed
*/
void WidgetFunctions::OnHotkey(uint32 nHotkey)
{
}

/**
*  @brief
*    Called when data has been dropped onto the widget
*/
void WidgetFunctions::OnDrop(const DataObject &cData)
{
}

/**
*  @brief
*    Called when a user defined message has been sent
*/
void WidgetFunctions::OnUserMessage(uint32 nData, void *pDataPtr)
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP
