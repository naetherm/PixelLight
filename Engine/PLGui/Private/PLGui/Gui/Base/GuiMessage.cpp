/*********************************************************\
 *  File: GuiMessage.cpp                                 *
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
#include "PLGui/Widgets/Widget.h"
#include "PLGui/Gui/Base/GuiMessage.h"
#include "PLGui/Gui/Data/DataObject.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLGui {


//[-------------------------------------------------------]
//[ Public named constructors                             ]
//[-------------------------------------------------------]
GuiMessage GuiMessage::OnWakeup()
{
	// Create message
	GuiMessage cMessage(nullptr, MessageOnWakeup);
	return cMessage;
}

GuiMessage GuiMessage::OnExit()
{
	// Create message
	GuiMessage cMessage(nullptr, MessageOnExit);
	return cMessage;
}

GuiMessage GuiMessage::OnTimer(Timer *pTimer)
{
	// Create message
	GuiMessage cMessage(nullptr, MessageOnTimer);
	cMessage.m_nData  = 0;
	cMessage.m_pTimer = pTimer;
	return cMessage;
}

GuiMessage GuiMessage::OnUserMessage(Widget *pWidget, uint32 nData, void *pData)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnUserMessage);
	cMessage.m_nData	= nData;
	cMessage.m_pDataPtr = pData;
	return cMessage;
}

GuiMessage GuiMessage::OnInternalMessage(uint32 nData, uint32 nExtData)
{
	// Create message
	GuiMessage cMessage(nullptr, MessageOnInternalMessage);
	cMessage.m_nData	= nData;
	cMessage.m_nExtData = nExtData;
	return cMessage;
}

GuiMessage GuiMessage::OnThemeChanged(Widget *pWidget)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnThemeChanged);
	return cMessage;
}

GuiMessage GuiMessage::OnUpdateContent(Widget *pWidget)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnUpdateContent);
	return cMessage;
}

GuiMessage GuiMessage::OnUpdateChildWidget(Widget *pWidget, Widget *pChildWidget)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnUpdateChildWidget);
	cMessage.m_pChildWidget = pChildWidget;
	return cMessage;
}

GuiMessage GuiMessage::OnAddChildWidget(Widget *pWidget, Widget *pChildWidget)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnAddChildWidget);
	cMessage.m_pChildWidget = pChildWidget;
	return cMessage;
}

GuiMessage GuiMessage::OnRemoveChildWidget(Widget *pWidget, Widget *pChildWidget)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnRemoveChildWidget);
	cMessage.m_pChildWidget = pChildWidget;
	return cMessage;
}

GuiMessage GuiMessage::OnClose(Widget *pWidget)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnClose);
	return cMessage;
}

GuiMessage GuiMessage::OnCreate(Widget *pWidget)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnCreate);
	return cMessage;
}

GuiMessage GuiMessage::OnDestroy(Widget *pWidget)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnDestroy);
	return cMessage;
}

GuiMessage GuiMessage::OnShow(Widget *pWidget)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnShow);
	return cMessage;
}

GuiMessage GuiMessage::OnHide(Widget *pWidget)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnHide);
	return cMessage;
}

GuiMessage GuiMessage::OnEnable(Widget *pWidget)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnEnable);
	return cMessage;
}

GuiMessage GuiMessage::OnDisable(Widget *pWidget)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnDisable);
	return cMessage;
}

GuiMessage GuiMessage::OnGetFocus(Widget *pWidget)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnGetFocus);
	return cMessage;
}

GuiMessage GuiMessage::OnLooseFocus(Widget *pWidget)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnLooseFocus);
	return cMessage;
}

GuiMessage GuiMessage::OnActivate(Widget *pWidget, bool bActivate)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnActivate);
	cMessage.m_bState = bActivate;
	return cMessage;
}

GuiMessage GuiMessage::OnDrawBackground(Widget *pWidget, Graphics *pGraphics)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnDrawBackground);
	cMessage.m_pGraphics = pGraphics;
	return cMessage;
}

GuiMessage GuiMessage::OnDraw(Widget *pWidget, Graphics *pGraphics)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnDraw);
	cMessage.m_pGraphics = pGraphics;
	return cMessage;
}

GuiMessage GuiMessage::OnMove(Widget *pWidget, const PLMath::Vector2i &vPos)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnMove);
	cMessage.m_vPosSize = vPos;
	return cMessage;
}

GuiMessage GuiMessage::OnSize(Widget *pWidget, const PLMath::Vector2i &vSize)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnSize);
	cMessage.m_vPosSize = vSize;
	return cMessage;
}

GuiMessage GuiMessage::OnWindowState(Widget *pWidget, EWindowState nWindowState)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnWindowState);
	cMessage.m_nWindowState = nWindowState;
	return cMessage;
}

GuiMessage GuiMessage::OnEnterFullscreen(Widget *pWidget)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnEnterFullscreen);
	return cMessage;
}

GuiMessage GuiMessage::OnLeaveFullscreen(Widget *pWidget)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnLeaveFullscreen);
	return cMessage;
}

GuiMessage GuiMessage::OnPreferredSize(Widget *pWidget, const PLMath::Vector2i &vSize)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnPreferredSize);
	cMessage.m_vPosSize = vSize;
	return cMessage;
}

GuiMessage GuiMessage::OnAdjustContent(Widget *pWidget)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnAdjustContent);
	return cMessage;
}

GuiMessage GuiMessage::OnMouseEnter(Widget *pWidget)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnMouseEnter);
	return cMessage;
}

GuiMessage GuiMessage::OnMouseLeave(Widget *pWidget)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnMouseLeave);
	return cMessage;
}

GuiMessage GuiMessage::OnMouseOver(Widget *pWidget, bool bMouseOver)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnMouseOver);
	cMessage.m_bState = bMouseOver;
	return cMessage;
}

GuiMessage GuiMessage::OnMouseMove(Widget *pWidget, const PLMath::Vector2i &vPos)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnMouseMove);
	cMessage.m_vPosSize = vPos;
	return cMessage;
}

GuiMessage GuiMessage::OnMouseHover(Widget *pWidget)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnMouseHover);
	return cMessage;
}

GuiMessage GuiMessage::OnMousePosUpdate(Widget *pWidget, const PLMath::Vector2i &vPos)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnMousePosUpdate);
	cMessage.m_vPosSize = vPos;
	return cMessage;
}

GuiMessage GuiMessage::OnMouseButtonDown(Widget *pWidget, EMouseButton nButton, const PLMath::Vector2i &vPos)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnMouseButtonDown);
	cMessage.m_nMouseButton	= nButton;
	cMessage.m_vPosSize		= vPos;
	return cMessage;
}

GuiMessage GuiMessage::OnMouseButtonUp(Widget *pWidget, EMouseButton nButton, const PLMath::Vector2i &vPos)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnMouseButtonUp);
	cMessage.m_nMouseButton	= nButton;
	cMessage.m_vPosSize		= vPos;
	return cMessage;
}

GuiMessage GuiMessage::OnMouseButtonClick(Widget *pWidget, EMouseButton nButton, const PLMath::Vector2i &vPos)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnMouseButtonClick);
	cMessage.m_nMouseButton	= nButton;
	cMessage.m_vPosSize		= vPos;
	return cMessage;
}

GuiMessage GuiMessage::OnMouseButtonDoubleClick(Widget *pWidget, EMouseButton nButton, const PLMath::Vector2i &vPos)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnMouseButtonDoubleClick);
	cMessage.m_nMouseButton	= nButton;
	cMessage.m_vPosSize		= vPos;
	return cMessage;
}

GuiMessage GuiMessage::OnMouseWheel(Widget *pWidget, int nDelta)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnMouseWheel);
	cMessage.m_nDelta = nDelta;
	return cMessage;
}

GuiMessage GuiMessage::OnKeyDown(Widget *pWidget, uint32 nKey, uint32 nModifiers)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnKeyDown);
	cMessage.m_nKey		  = nKey;
	cMessage.m_nModifiers = nModifiers;
	return cMessage;
}

GuiMessage GuiMessage::OnKeyUp(Widget *pWidget, uint32 nKey, uint32 nModifiers)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnKeyUp);
	cMessage.m_nKey		  = nKey;
	cMessage.m_nModifiers = nModifiers;
	return cMessage;
}

GuiMessage GuiMessage::OnHotkey(Widget *pWidget, uint32 nHotkey)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnHotkey);
	cMessage.m_nKey = nHotkey;
	return cMessage;
}

GuiMessage GuiMessage::OnDrop(Widget *pWidget, DataObject *pDataObject)
{
	// Create message
	GuiMessage cMessage(pWidget, MessageOnDrop);
	cMessage.m_pDataObject = pDataObject;
	return cMessage;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
GuiMessage::GuiMessage(Widget *pWidget, EMessageType nType) :
	m_pWidget(pWidget),
	m_nType(nType),
	m_nData(0),
	m_pDataPtr(nullptr),
	m_vPosSize(0, 0)
{
}

/**
*  @brief
*    Copy constructor
*/
GuiMessage::GuiMessage(const GuiMessage &cOther) :
	m_pWidget (cOther.m_pWidget),
	m_nType	  (cOther.m_nType),
	m_nData	  (cOther.m_nData),
	m_pDataPtr(cOther.m_pDataPtr),
	m_vPosSize(cOther.m_vPosSize)
{
}

/**
*  @brief
*    Destructor
*/
GuiMessage::~GuiMessage()
{
}

/**
*  @brief
*    Comparison operator
*/
bool GuiMessage::operator ==(const GuiMessage &cOther) const
{
	// Compare messages
	return (cOther.m_pWidget  == m_pWidget &&
			cOther.m_nType	  == m_nType &&
			cOther.m_nData	  == m_nData &&
			cOther.m_pDataPtr == m_pDataPtr &&
			cOther.m_vPosSize == m_vPosSize);
}

/**
*  @brief
*    Assignment operator
*/
GuiMessage &GuiMessage::operator =(const GuiMessage &cOther)
{
	// Copy message
	m_pWidget	= cOther.m_pWidget;
	m_nType		= cOther.m_nType;
	m_nData		= cOther.m_nData;
	m_pDataPtr	= cOther.m_pDataPtr;
	m_vPosSize	= cOther.m_vPosSize;
	return *this;
}

/**
*  @brief
*    Get message representation as string
*/
String GuiMessage::ToString() const
{
	// Get window handle
	String sMessage = '[' + (m_pWidget ? m_pWidget->GetDescriptor() : String('%')) + "] ";

	// Get message type
	switch (m_nType) {
		// Unknown message type
		case MessageOnUnknown:
			sMessage += "OnUnknown";
			break;

		// Wakeup message
		case MessageOnWakeup:
			sMessage += "Wakeup";
			break;

		// Exit message
		case MessageOnExit:
			sMessage += "OnExit";
			break;

		// Timer message
		case MessageOnTimer:
			sMessage += "OnTimer";
			break;

		// Internal message
		case MessageOnInternalMessage:
			sMessage += "OnInternalMessage";
			sMessage += String() + " (Data = " + m_nData + ", DataPtr = " + reinterpret_cast<uint_ptr>(m_pDataPtr) + ')';
			break;

		// User message
		case MessageOnUserMessage:
			sMessage += "OnUserMessage";
			sMessage += String() + " (Data = " + m_nData + ", DataPtr = " + reinterpret_cast<uint_ptr>(m_pDataPtr) + ')';
			break;

		// Theme has been changed
		case MessageOnThemeChanged:
			sMessage += "MessageOnThemeChanged";
			break;

		// Widget content has been changed
		case MessageOnUpdateContent:
			sMessage += "OnUpdateContent";
			break;

		// Child widget has been changed
		case MessageOnUpdateChildWidget:
			sMessage += "OnUpdateChildWidget";
			sMessage += String() + " (Child=" + reinterpret_cast<uint_ptr>(m_pChildWidget) + ')';
			break;

		// Child widget has been added
		case MessageOnAddChildWidget:
			sMessage += "OnAddChildWidget";
			sMessage += String() + " (Child=" + reinterpret_cast<uint_ptr>(m_pChildWidget) + ')';
			break;

		// Child widget has been removed
		case MessageOnRemoveChildWidget:
			sMessage += "OnRemoveChildWidget";
			sMessage += String() + " (Child=" + reinterpret_cast<uint_ptr>(m_pChildWidget) + ')';
			break;

		// Widget shall be closed (ALT+F4 or X-Button pressed)
		case MessageOnClose:
			sMessage += "OnClose";
			break;

		// Widget has just been created
		case MessageOnCreate:
			sMessage += "OnCreate";
			break;

		// Widget is going to be destroyed
		case MessageOnDestroy:
			sMessage += "OnDestroy";
			break;

		// Widget gets shown
		case MessageOnShow:
			sMessage += "OnShow";
			break;

		// Widget gets hidden
		case MessageOnHide:
			sMessage += "OnHide";
			break;

		// Widget gets enabled
		case MessageOnEnable:
			sMessage += "OnEnable";
			break;

		// Widget gets disabled
		case MessageOnDisable:
			sMessage += "OnDisable";
			break;

		// Widget gets focus
		case MessageOnGetFocus:
			sMessage += "OnGetFocus";
			break;

		// Widget looses focus
		case MessageOnLooseFocus:
			sMessage += "OnLooseFocus";
			break;

		// Widget has been activated or deactivated (focus-widget has changed)
		case MessageOnActivate:
			sMessage += "OnActivate";
			sMessage += String() + " (State=" + (m_bState ? "true" : "false") + ')';
			break;

		// Widget background gets drawn, graphics object used for painting as parameter
		case MessageOnDrawBackground:
			sMessage += "OnDrawBackground";
			break;

		// Widget gets drawn, graphics object used for painting as parameter
		case MessageOnDraw:
			sMessage += "OnDraw";
			break;

		// Widget gets moved, new widget position as parameter
		case MessageOnMove:
			sMessage += "OnMove";
			sMessage += String() + " (Pos=" + m_vPosSize.ToString() + ')';
			break;

		// Widget gets resized, new widget size as parameter
		case MessageOnSize:
			sMessage += "OnSize";
			sMessage += String() + " (Pos=" + m_vPosSize.ToString() + ')';
			break;

		// Window state has changed, new window state as parameter
		case MessageOnWindowState:
		{
			sMessage += "OnWindowState";
			String sWindowState;
			if (m_nWindowState == StateNormal)		sWindowState = "Normal";
			if (m_nWindowState == StateMinimized)	sWindowState = "Minimized";
			if (m_nWindowState == StateMaximized)	sWindowState = "Maximized";
			if (m_nWindowState == StateFullscreen)	sWindowState = "Fullscreen";
			sMessage += String() + " (WindowState=" + sWindowState + ')';
			break;
		}

		// Widget enters fullscreen mode
		case MessageOnEnterFullscreen:
			sMessage += "OnEnterFullscreen";
			break;

		// Widget leaves fullscreen mode
		case MessageOnLeaveFullscreen:
			sMessage += "OnLeaveFullscreen";
			break;

		// Widget has calculated it's preferred size
		case MessageOnPreferredSize:
			sMessage += "OnPreferredSize";
			sMessage += String() + " (Pos=" + m_vPosSize.ToString() + ')';
			break;

		// Widget content has to be adjusted
		case MessageOnAdjustContent:
			sMessage += "OnAdjustContent";
			break;

		// Mouse enters widget
		case MessageOnMouseEnter:
			sMessage += "OnMouseEnter";
			break;

		// Mouse leaves widget
		case MessageOnMouseLeave:
			sMessage += "OnMouseLeave";
			break;

		// Mouse-over widget has changed
		case MessageOnMouseOver:
			sMessage += "OnMouseOver";
			sMessage += String() + " (State=" + (m_bState ? "true" : "false") + ')';
			break;

		// Mouse moves inside the widget, mouse position within the widget as parameter
		case MessageOnMouseMove:
			sMessage += "OnMouseMove";
			sMessage += String() + " (Pos=" + m_vPosSize.ToString() + ')';
			break;

		// Mouse hovers over the widget
		case MessageOnMouseHover:
			sMessage += "OnMouseHover";
			break;

		// Mouse position inside the widget has changed due to the movement of widget
		case MessageOnMousePosUpdate:
			sMessage += "OnMousePosUpdate";
			sMessage += String() + " (Pos=" + m_vPosSize.ToString() + ')';
			break;

		// Mouse button is pressed, mouse button and mouse position within the widget as parameters
		case MessageOnMouseButtonDown:
			sMessage += "OnMouseButtonDown";
			sMessage += String() + " Button=" + static_cast<uint32>(m_nMouseButton) + ", Pos=" + m_vPosSize.ToString() + ')';
			break;

		// Mouse button is released, mouse button and mouse position within the widget as parameters
		case MessageOnMouseButtonUp:
			sMessage += "OnMouseButtonUp";
			sMessage += String() + " (Button=" + static_cast<uint32>(m_nMouseButton) + ", Pos=" + m_vPosSize.ToString() + ')';
			break;

		// Mouse button has been clicked, mouse button and mouse position within the widget as parameters
		case MessageOnMouseButtonClick:
			sMessage += "OnMouseButtonClick";
			sMessage += String() + " (Button=" + static_cast<uint32>(m_nMouseButton) + ", Pos=" + m_vPosSize.ToString() + ')';
			break;

		// Mouse button has been double-clicked, mouse button and mouse position within the widget as parameters
		case MessageOnMouseButtonDoubleClick:
			sMessage += "OnMouseButtonDoubleClick";
			sMessage += String() + " (Button=" + static_cast<uint32>(m_nMouseButton) + ", Pos=" + m_vPosSize.ToString() + ')';
			break;

		// Mouse wheel moved, mouse wheel movement as parameter
		case MessageOnMouseWheel:
			sMessage += "OnMouseWheel";
			sMessage += String() + " (Delta=" + m_nDelta + ')';
			break;

		// Key gets pressed, pressed key and modifier keys pressed as parameters
		case MessageOnKeyDown:
			sMessage += "OnKeyDown";
			sMessage += String() + " (Key=" + m_nKey + ", Modifier=" + m_nModifiers + ')';
			break;

		// Key gets released, released key and modifier keys pressed as parameters
		case MessageOnKeyUp:
			sMessage += "OnKeyUp";
			sMessage += String() + " (Key=" + m_nKey + ", Modifier=" + m_nModifiers + ')';
			break;

		// Hotkey pressed, hotkey ID as parameter
		case MessageOnHotkey:
			sMessage += "OnHotkey";
			sMessage += String() + " (Hotkey=" + m_nKey + ')';
			break;

		// Data has been dropped onto the widget
		case MessageOnDrop:
			sMessage += "OnDrop";
			sMessage += String() + " (Data=" + (m_pDataObject ? m_pDataObject->GetString() : "nullptr") + ')';
			break;
	}

	// Return string representation of message
	return sMessage;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
