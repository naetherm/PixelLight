/*********************************************************\
 *  File: Modifier.cpp                                   *
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
#include "PLGui/Gui/Base/GuiMessage.h"
#include "PLGui/Modifiers/Modifier.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLGui {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(Modifier)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Modifier::Modifier() :
	m_pWidget(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
Modifier::~Modifier()
{
}

/**
*  @brief
*    Return the widget to which the modifier belongs
*/
Widget *Modifier::GetWidget() const
{
	// Return widget
	return m_pWidget;
}

/**
*  @brief
*    Get name of modifier
*/
String Modifier::GetName() const
{
	// Return name
	return m_sName;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Set name of modifier
*/
void Modifier::SetName(const String &sName)
{
	// Set name
	m_sName = sName;
}

/**
*  @brief
*    Attach the modifier to a widget
*/
void Modifier::Attach(Widget &cWidget)
{
	// Only possible if the modifier is not yet attached
	if (!m_pWidget) {
		// Save pointer to widget
		m_pWidget = &cWidget;

		// Call virtual function
		OnAttach(cWidget);
	}
}

/**
*  @brief
*    Detach the modifier
*/
void Modifier::Detach()
{
	// Only possible if the modifier is attached
	if (m_pWidget) {
		// Call virtual function
		OnDetach(*m_pWidget);

		// Clear pointer to widget
		m_pWidget = nullptr;
	}
}


//[-------------------------------------------------------]
//[ Protected virtual Modifier functions                  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the modifier is attached to a widget
*/
void Modifier::OnAttach(Widget &cWidget)
{
}

/**
*  @brief
*    Called when the modifier is detached from a widget
*/
void Modifier::OnDetach(Widget &cWidget)
{
}


//[-------------------------------------------------------]
//[ Protected virtual WidgetFunctions functions           ]
//[-------------------------------------------------------]
void Modifier::OnMessage(const GuiMessage &cMessage)
{
	// Get message type
	switch (cMessage.GetType()) {
		// Widget content has been changed
		case MessageOnUpdateContent:
			// Call virtual function
			OnUpdateContent();
			break;

		// Child widget has been changed
		case MessageOnUpdateChildWidget:
			// Call virtual function
			OnUpdateChildWidget(cMessage.GetChildWidget());
			break;

		// Child widget has been added
		case MessageOnAddChildWidget:
			// Call virtual function
			OnAddChildWidget(cMessage.GetChildWidget());
			break;

		// Child widget has been removed
		case MessageOnRemoveChildWidget:
			// Call virtual function
			OnRemoveChildWidget(cMessage.GetChildWidget());
			break;

		// Widget shall be closed (ALT+F4 or X-Button pressed)
		case MessageOnClose:
			// Call virtual function
			OnClose();
			break;

		// Widget has just been created
		case MessageOnCreate:
			// Call virtual function
			OnCreate();
			break;

		// Widget is going to be destroyed
		case MessageOnDestroy:
			// Call virtual function
			OnDestroy();
			break;

		// Widget gets shown
		case MessageOnShow:
			// Call virtual function
			OnShow();
			break;

		// Widget gets hidden
		case MessageOnHide:
			// Call virtual function
			OnHide();
			break;

		// Widget gets enabled
		case MessageOnEnable:
			// Call virtual function
			OnEnable();
			break;

		// Widget gets disabled
		case MessageOnDisable:
			// Call virtual function
			OnDisable();
			break;

		// Widget gets focus
		case MessageOnGetFocus:
			// Call virtual function
			OnGetFocus();
			break;

		// Widget looses focus
		case MessageOnLooseFocus:
			// Call virtual function
			OnLooseFocus();
			break;

		// Widget has been activated or deactivated (focus-widget has changed)
		case MessageOnActivate:
			// Call virtual function
			OnActivate(cMessage.GetState());
			break;

		// Widget background gets drawn, graphics object used for painting as parameter
		case MessageOnDrawBackground:
			// Call virtual function
			OnDrawBackground(*cMessage.GetGraphics());
			break;

		// Widget gets drawn, graphics object used for painting as parameter
		case MessageOnDraw:
			// Call virtual function
			OnDraw(*cMessage.GetGraphics());
			break;

		// Widget gets moved, new widget position as parameter
		case MessageOnMove:
			// Call virtual function
			OnMove(cMessage.GetPosSize());
			break;

		// Widget gets resized, new widget size as parameter
		case MessageOnSize:
			// Call virtual function
			OnSize(cMessage.GetPosSize());
			break;

		// Window state has changed, new window state as parameter
		case MessageOnWindowState:
			// Call virtual function
			OnWindowState(cMessage.GetWindowState());
			break;

		// Widget enters fullscreen mode
		case MessageOnEnterFullscreen:
			// Call virtual function
			OnEnterFullscreen();
			break;

		// Widget leaves fullscreen mode
		case MessageOnLeaveFullscreen:
			// Call virtual function
			OnLeaveFullscreen();
			break;

		// Widget has calculated it's preferred size
		case MessageOnPreferredSize:
			// Call virtual function
			OnPreferredSize(cMessage.GetPosSize());
			break;

		// Widget content has to be adjusted
		case MessageOnAdjustContent:
			// Call virtual function
			OnAdjustContent();
			break;

		// Mouse enters widget
		case MessageOnMouseEnter:
			// Call virtual function
			OnMouseEnter();
			break;

		// Mouse leaves widget
		case MessageOnMouseLeave:
			// Call virtual function
			OnMouseLeave();
			break;

		// Mouse-over widget has changed
		case MessageOnMouseOver:
			// Call virtual function
			OnMouseOver(cMessage.GetState());
			break;

		// Mouse moves inside the widget, mouse position within the widget as parameter
		case MessageOnMouseMove:
			// Call virtual function
			OnMouseMove(cMessage.GetPosSize());
			break;

		// Mouse hovers over the widget
		case MessageOnMouseHover:
			// Call virtual function
			OnMouseHover();
			break;

		// Mouse position inside the widget has changed due to the movement of widget
		case MessageOnMousePosUpdate:
			// Call virtual function
			OnMousePosUpdate(cMessage.GetPosSize());
			break;

		// Mouse button is pressed, mouse button and mouse position within the widget as parameters
		case MessageOnMouseButtonDown:
			// Call virtual function
			OnMouseButtonDown(cMessage.GetMouseButton(), cMessage.GetPosSize());
			break;

		// Mouse button is released, mouse button and mouse position within the widget as parameters
		case MessageOnMouseButtonUp:
			// Call virtual function
			OnMouseButtonUp(cMessage.GetMouseButton(), cMessage.GetPosSize());
			break;

		// Mouse button has been clicked, mouse button and mouse position within the widget as parameters
		case MessageOnMouseButtonClick:
			// Call virtual function
			OnMouseButtonClick(cMessage.GetMouseButton(), cMessage.GetPosSize());
			break;

		// Mouse button has been double-clicked, mouse button and mouse position within the widget as parameters
		case MessageOnMouseButtonDoubleClick:
			// Call virtual function
			OnMouseButtonDoubleClick(cMessage.GetMouseButton(), cMessage.GetPosSize());
			break;

		// Mouse wheel moved, mouse wheel movement as parameter
		case MessageOnMouseWheel:
			// Call virtual function
			OnMouseWheel(cMessage.GetDelta());
			break;

		// Key gets pressed, pressed key and modifier keys pressed as parameters
		case MessageOnKeyDown:
		{
			// Call virtual function
			OnKeyDown(cMessage.GetKey(), cMessage.GetModifiers());
			break;
		}

		// Key gets released, released key and modifier keys pressed as parameters
		case MessageOnKeyUp:
			// Call virtual function
			OnKeyUp(cMessage.GetKey(), cMessage.GetModifiers());
			break;

		// Hotkey pressed, hotkey ID as parameter
		case MessageOnHotkey:
			// Call virtual function
			OnHotkey(cMessage.GetKey());
			break;

		// Data has been dropped onto the widget
		case MessageOnDrop:
			// Call virtual function
			OnDrop(*cMessage.GetDataObject());
			break;

		// User message
		case MessageOnUserMessage:
			// Call virtual function
			OnUserMessage(cMessage.GetData(), cMessage.GetDataPointer());
			break;

		case MessageOnUnknown:
		case MessageOnInternalMessage:
		case MessageOnWakeup:
		case MessageOnExit:
		case MessageOnTimer:
		case MessageOnThemeChanged:
		default:
			// [TODO] Check me: Do we need to handle those cases?
			break;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
