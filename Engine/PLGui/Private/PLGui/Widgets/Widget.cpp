/*********************************************************\
 *  File: Widget.cpp                                     *
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
#include <PLCore/Base/Class.h>
#include <PLCore/Container/List.h>
#include <PLGraphics/Color/Color4.h>
#include "PLGui/Gui/Base/GuiMessage.h"
#include "PLGui/Gui/Base/Keys.h"
#include "PLGui/Gui/Resources/Graphics.h"
#include "PLGui/Gui/Gui.h"
#include "PLGui/Gui/Screen.h"
#include "PLGui/Backend/GuiImpl.h"
#include "PLGui/Backend/WidgetImpl.h"
#include "PLGui/Widgets/Widget.h"
#include "PLGui/Modifiers/Modifier.h"
#include "PLGui/Themes/Theme.h"
#include "PLGui/Layout/Layout.h"


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_PUSH
PL_WARNING_DISABLE(4355) // "'this' : used in base member initializer list"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLGraphics;
using namespace PLMath;
namespace PLGui {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(Widget)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Widget::Widget(Widget *pParent, bool bManaged) :
	ID(this),
	Name(this),
	Title(this),
	Pos(this),
	Size(this),
	Topmost(this),
	BackgroundColor(this),
	m_pGui(pParent ? pParent->GetGui() : Gui::GetSystemGui()),
	m_bManaged(bManaged),
	m_bRootWidget(false),
	m_nID(m_pGui->GetUniqueWidgetID()),
	m_sName(""),
	m_pParent(pParent),
	m_pWidgetImpl(nullptr),
	m_pTheme(nullptr),
	m_pLayout(nullptr),
	m_nWindowState(StateNormal),
	m_bTopmost(false),
	m_sTitle("No Title"),
	m_cIcon(*m_pGui),
	m_cCursor(*m_pGui, CursorArrow),
	m_nFocusStyle(NoFocus),
	m_bTabStop(true),
	m_bCaptureMouse(false),
	m_cBackgroundColor(Color4::White),
	m_vPreferredSize(-1, -1),
	m_pUserData(nullptr)
{
	// The root widget is only there to get the Gui pointer, to discard it as a parent now
	if (m_pParent && m_pParent->m_bRootWidget)
		m_pParent = nullptr;

	// Create widget implementation
	m_pWidgetImpl = m_pGui->CreateWidgetImpl(*this);
	m_pWidgetImpl->CreateWidget();

	// Add widget to list
	if (m_pParent) {
		// Add child to parent widget
		m_pParent->AddChild(this);
	} else {
		// Add top-level widgets to list in Gui
		m_pGui->AddTopLevelWidget(this);
	}

	// Get initial position and size
	m_vPos  = m_pWidgetImpl->GetPos();
	m_vSize = m_pWidgetImpl->GetSize();

	// Set title
	m_pWidgetImpl->SetTitle(m_sTitle);

	// Set mouse cursor
	m_pWidgetImpl->SetCursor(m_cCursor);

	// Set default theme
	SetTheme(m_pParent ? m_pParent->GetTheme() : m_pGui->GetTheme());
}

/**
*  @brief
*    Constructor
*/
Widget::Widget(handle nNativeWindowHandle) :
	ID(this),
	Name(this),
	Title(this),
	Pos(this),
	Size(this),
	Topmost(this),
	BackgroundColor(this),
	m_pGui(Gui::GetSystemGui()),
	m_bManaged(false),
	m_nID(0),
	m_sName(""),
	m_pParent(nullptr),
	m_pWidgetImpl(nullptr),
	m_pTheme(nullptr),
	m_pLayout(nullptr),
	m_nWindowState(StateNormal),
	m_bTopmost(false),
	m_cIcon(*m_pGui),
	m_cCursor(*m_pGui, CursorArrow),
	m_nFocusStyle(NoFocus),
	m_bTabStop(true),
	m_bCaptureMouse(false),
	m_cBackgroundColor(Color4::White),
	m_vPreferredSize(-1, -1),
	m_pUserData(nullptr)
{
	// Create implementation
	m_pWidgetImpl = m_pGui->CreateWidgetImpl(*this);
	m_pWidgetImpl->CreateWrapperWidget(nNativeWindowHandle);

	// Add to list of wrapper widgets in Gui
	m_pGui->AddWrapperWidget(this);
}

/**
*  @brief
*    Destructor
*/
Widget::~Widget()
{
	// Delete child widgets
	/* // Do not, since all widgets are automatically added to the destroy-list after an OnDestroy-message
	List<Widget*> lstChildren = m_lstChildren;
	for (uint32 i=0; i<lstChildren.GetNumOfElements(); i++) {
		delete lstChildren[i];
	}
	*/

	// Destroy modifiers
	for (uint32 i=0; i<m_lstModifiers.GetNumOfElements(); i++) {
		// Detach and delete modifier
		Modifier *pModifier = m_lstModifiers[i];
		pModifier->Detach();
		delete pModifier;
	}

	// Delete widget implementation
	if (m_pWidgetImpl) delete m_pWidgetImpl;
}

/**
*  @brief
*    Get owner GUI
*/
Gui *Widget::GetGui() const
{
	// Return GUI
	return m_pGui;
}

/**
*  @brief
*    Get implementation
*/
WidgetImpl *Widget::GetImpl() const
{
	// Return widget implementation
	return m_pWidgetImpl;
}

/**
*  @brief
*    Get system native window handle
*/
handle Widget::GetNativeWindowHandle() const
{
	// Return native window handle
	return m_pWidgetImpl->GetNativeWindowHandle();
}

//[-------------------------------------------------------]
//[ Widget information                                    ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get ID
*/
uint32 Widget::GetID() const
{
	// Return ID
	return m_nID;
}

/**
*  @brief
*    Set ID
*/
void Widget::SetID(uint32 nID)
{
	// Set ID
	m_nID = nID;
}

/**
*  @brief
*    Get name
*/
String Widget::GetName() const
{
	// Return name
	return m_sName;
}

/**
*  @brief
*    Set name
*/
void Widget::SetName(const String &sName)
{
	// Set name
	m_sName = sName;
}

/**
*  @brief
*    Get descriptor
*/
String Widget::GetDescriptor() const
{
	// Name available?
	if (m_sName.GetLength() > 0) {
		return m_sName;
	}

	// ID available?
	else if (m_nID > 0) {
		return String() + m_nID;
	}

	// Native window handle available?
	else if (m_pWidgetImpl) {
		return String() + static_cast<int>(m_pWidgetImpl->GetNativeWindowHandle());
	}

	// Unknown widget
	return "invalid";
}

/**
*  @brief
*    Get user defined data
*/
void *Widget::GetUserData() const
{
	// Return user data
	return m_pUserData;
}

/**
*  @brief
*    Set user defined data
*/
void Widget::SetUserData(void *pData)
{
	// Set user data
	m_pUserData = pData;
}

/**
*  @brief
*    Get whether or not the widget is managed
*/
bool Widget::IsManaged() const
{
	return m_bManaged;
}

//[-------------------------------------------------------]
//[ Appearance (title, icon etc.)                         ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get title
*/
String Widget::GetTitle() const
{
	// Return title
	return m_sTitle;
}

/**
*  @brief
*    Set title
*/
void Widget::SetTitle(const String &sTitle)
{
	// Save title
	m_sTitle = sTitle;

	// Set title
	m_pWidgetImpl->SetTitle(m_sTitle);
}

/**
*  @brief
*    Get icon
*/
const Image &Widget::GetIcon() const
{
	// Return icon
	return m_cIcon;
}

/**
*  @brief
*    Set icon
*/
void Widget::SetIcon(const Image &cIcon)
{
	// Save icon
	m_cIcon = cIcon;

	// Set icon
	m_pWidgetImpl->SetIcon(m_cIcon);
}

/**
*  @brief
*    Get cursor
*/
const Cursor &Widget::GetCursor() const
{
	// Return cursor
	return m_cCursor;
}

/**
*  @brief
*    Set cursor
*/
void Widget::SetCursor(const Cursor &cCursor)
{
	// Set cursor
	m_cCursor = cCursor;

	// Call implementation
	m_pWidgetImpl->SetCursor(cCursor);
}

//[-------------------------------------------------------]
//[ Hierarchy                                             ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get parent widget
*/
Widget *Widget::GetParent() const
{
	// Return parent widget
	return m_pParent;
}

/**
*  @brief
*    Get common parent of this and another widget
*/
Widget *Widget::GetCommonParent(const Widget &cWidget) const
{
	// Same widget?
	if (&cWidget == this) {
		return const_cast<Widget*>(&cWidget);
	} else {
		// Go down the parents of this widget
		for (const Widget *pThisWidget=this; pThisWidget!=nullptr; pThisWidget=pThisWidget->GetParent()) {
			// Find the first common parent - go down the parents of the other widget
			for (Widget *pOtherWidget=const_cast<Widget*>(&cWidget); pOtherWidget!=nullptr; pOtherWidget=pOtherWidget->GetParent()) {
				// Common parent found?
				if (pThisWidget == pOtherWidget)
					return pOtherWidget;
			}
		}

		// No common parent found
		return nullptr;
	}
}

/**
*  @brief
*    Get top level widget that is parent of this widget
*/
Widget *Widget::GetTopLevelWidget() const
{
	// Start with this widget
	Widget *pWidget = const_cast<Widget*>(this);

	// Find top-level widget in hierarchy
	while (pWidget && !pWidget->IsTopLevel())
		pWidget = pWidget->GetParent();

	// Return widget
	return pWidget;
}

/**
*  @brief
*    Get content widget
*/
Widget *Widget::GetContentWidget() const
{
	// Return widget itself as content widget
	return const_cast<Widget*>(this);
}

/**
*  @brief
*    Get list of children
*/
const List<Widget*> &Widget::GetChildren() const
{
	// Return list of children
	return m_lstChildren;
}

/**
*  @brief
*    Get first child widget
*/
Widget *Widget::GetFirstChild() const
{
	// Return first child widget
	return m_lstChildren.Get(0);
}

/**
*  @brief
*    Get last child widget
*/
Widget *Widget::GetLastChild() const
{
	// Return last child widget
	return m_lstChildren.Get(m_lstChildren.GetNumOfElements()-1);
}

/**
*  @brief
*    Get previous sibling widget
*/
Widget *Widget::GetPreviousSibling() const
{
	// Get parent widget
	Widget *pParent = GetParent();
	if (pParent) {
		// Get index of this widget
		int nIndex = pParent->GetChildren().GetIndex(const_cast<Widget*>(this));

		// Return sibling
		return pParent->GetChildren().Get(nIndex-1);
	}

	// Could not find a sibling
	return nullptr;
}

/**
*  @brief
*    Get next sibling widget
*/
Widget *Widget::GetNextSibling() const
{
	// Get parent widget
	Widget *pParent = GetParent();
	if (pParent) {
		// Get index of this widget
		int nIndex = pParent->GetChildren().GetIndex(const_cast<Widget*>(this));

		// Return sibling
		return pParent->GetChildren().Get(nIndex+1);
	}

	// Could not find a sibling
	return nullptr;
}

/**
*  @brief
*    Check if widget is a top level widget
*/
bool Widget::IsTopLevel() const
{
	// Check if the widget has no parent
	return (m_pParent == nullptr);
}

//[-------------------------------------------------------]
//[ Position and appearance                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Check if widget is visible in the taskbar
*/
bool Widget::IsShowInTaskbar() const
{
	// Check if the Gui has a taskbar
	if (m_pGui->HasTaskbar()) {
		// Return taskbar state
		return m_pWidgetImpl->IsShowInTaskbar();
	}

	// Not shown in taskbar as the system does not provide a taskbar :-)
	return false;
}

/**
*  @brief
*    Set if widget is visible in the taskbar
*/
void Widget::SetShowInTaskbar(bool bShowInTaskbar)
{
	// Check if the Gui has a taskbar
	if (m_pGui->HasTaskbar()) {
		// Set taskbar state
		m_pWidgetImpl->SetShowInTaskbar(bShowInTaskbar);
	}
}

/**
*  @brief
*    Get screen the widget is on
*/
Screen *Widget::GetScreen() const
{
	// Get current position
	Vector2i vPos = GetAbsPos();

	// Check all screens
	for (uint32 i=0; i<GetGui()->GetScreens().GetNumOfElements(); i++) {
		// Get screen, position and size
		Screen *pScreen = GetGui()->GetScreens().Get(i);
		Vector2i vScreenPos  = pScreen->GetPos();
		Vector2i vScreenSize = pScreen->GetSize();

		// Check if widget is inside that screen
		if (vPos.x >= vScreenPos.x && vPos.x < vScreenPos.x + vScreenSize.x &&
			vPos.y >= vScreenPos.y && vPos.y < vScreenPos.y + vScreenSize.y)
		{
			// Screen found
			return pScreen;
		}
	}

	// ERROR, no screen found
	return nullptr;
}

/**
*  @brief
*    Move widget to another screen
*/
void Widget::MoveToScreen(Screen &cScreen)
{
	// Set screen
	m_pWidgetImpl->MoveToScreen(cScreen);
}

/**
*  @brief
*    Get widget position
*/
Vector2i Widget::GetPos() const
{
	// Return position of widget
	return m_pWidgetImpl->GetPos();
}

/**
*  @brief
*    Set widget position
*/
void Widget::SetPos(const Vector2i &vPos)
{
	// Set position of widget
	m_pWidgetImpl->SetPos(vPos);
}

/**
*  @brief
*    Get widget size
*/
Vector2i Widget::GetSize() const
{
	// Return size of widget
	return m_pWidgetImpl->GetSize();
}

/**
*  @brief
*    Set widget size
*/
void Widget::SetSize(const Vector2i &vSize)
{
	// Set size of widget
	m_pWidgetImpl->SetSize(vSize);
}

/**
*  @brief
*    Check if widget is topmost (always above all other widgets)
*/
bool Widget::GetTopmost() const
{
	// Return topmost-state
	return m_bTopmost;
}

/**
*  @brief
*    Set if widget is topmost (always above all other widgets)
*/
void Widget::SetTopmost(bool bTopmost)
{
	// Set topmost-state
	m_bTopmost = bTopmost;

	// Set topmost state of widget
	m_pWidgetImpl->SetTopmost(m_bTopmost);
}

/**
*  @brief
*    Get absolute widget position
*/
Vector2i Widget::GetAbsPos() const
{
	// Get position and add all parent positions
	Vector2i vPos = GetPos();
	for (const Widget *pParent = GetParent(); pParent != nullptr; pParent = pParent->GetParent())
		vPos += pParent->GetPos();

	// Return absolute position
	return vPos;
}

/**
*  @brief
*    Get position relative to another widget
*/
Vector2i Widget::GetRelativePos(Widget &cWidget) const
{
	// Get common parent
	Widget *pRoot = GetCommonParent(cWidget);
	if (!pRoot) {
		// Error, must be widgets of two different Gui's!
		return Vector2i::Zero;
	}

	// Get relative position of this widget to the parent
	Vector2i vPosThis;
	for (const Widget *pWnd = this; pWnd != pRoot; pWnd = pWnd->GetParent())
		vPosThis += pWnd->GetPos();

	// Get relative position of the other widget to the parent
	Vector2i vPosOther;
	for (const Widget *pWnd = &cWidget; pWnd != pRoot; pWnd = pWnd->GetParent())
		vPosOther += pWnd->GetPos();

	// Get relative position of this widget to the other widget
	return vPosThis - vPosOther;
}

/**
*  @brief
*    Center widget
*/
void Widget::Center(ECenter nCenter)
{
	// Get parent widget
	Widget *pParent = GetParent();

	// Center on parent
	if (nCenter == CenterParent && pParent) {
		// Center relative coordinates
		Vector2i vSize		 = GetSize();
		Vector2i vParentSize = pParent->GetSize();
		SetPos((vParentSize - vSize) / 2);
	}

	// Center on screen
	else if (nCenter == CenterScreen || !pParent) {
		// Center relative to the screen
		// [TODO] Better center to *current* screen? Something like CenterOnScreen(this->GetScreen())
		CenterOnScreen(*GetGui()->GetDefaultScreen());
	}
}

/**
*  @brief
*    Center widget relative to the screen
*/
void Widget::CenterOnScreen(Screen &cScreen)
{
	// Widget and screen must belong to the same GUI
	if (GetGui() == cScreen.GetGui()) {
		// Get sizes
		Vector2i vSize	 = GetSize();
		Vector2i vScreen = cScreen.GetSize();

		// Get relative position
		Vector2i vRelative = GetAbsPos() - cScreen.GetPos();

		// Get desired relative position
		Vector2i vDesired;
		vDesired.x = (vScreen.width  - vSize.width)  / 2;
		vDesired.y = (vScreen.height - vSize.height) / 2;

		// Set new position
		Vector2i vPos = GetPos() + vDesired - vRelative;
		SetPos(vPos);
	}
}

/**
*  @brief
*    Center widget relative to another widget
*/
void Widget::CenterOnWidget(Widget &cWidget)
{
	// Both widgets must belong to the same GUI
	if (GetGui() == cWidget.GetGui()) {
		// Get sizes
		Vector2i vSize		= GetSize();
		Vector2i vOtherSize = cWidget.GetSize();

		// Get relative position
		Vector2i vRelative = GetRelativePos(cWidget);

		// Get desired relative position
		Vector2i vDesired;
		vDesired.x = (vOtherSize.width  - vSize.width)  / 2;
		vDesired.y = (vOtherSize.height - vSize.height) / 2;

		// Set new position
		Vector2i vPos = GetPos() + vDesired - vRelative;
		SetPos(vPos);
	}
}

/**
*  @brief
*    Calculate preferred size
*/
void Widget::CalculatePreferredSize(const Vector2i &vRefSize)
{
	// Send message
	m_pGui->SendMessage(GuiMessage::OnPreferredSize(this, vRefSize));
}

/**
*  @brief
*    Get preferred size
*/
PLMath::Vector2i Widget::GetPreferredSize() const
{
	// Return preferred size
	return m_vPreferredSize;
}

/**
*  @brief
*    Set size of widget to fit it's content
*/
void Widget::FitSize()
{
	// Calculate preferred size
	CalculatePreferredSize(m_vSize);

	// Set size
	Vector2i vSize = m_vPreferredSize;
	if (vSize.x <= -1) vSize.x = m_vSize.x;
	if (vSize.y <= -1) vSize.y = m_vSize.y;
	SetSize(vSize);
}

/**
*  @brief
*    Set Z position
*/
void Widget::SetZPos(EZPos nZPos, Widget *pWidget)
{
	// Set Z position of widget
	m_pWidgetImpl->SetZPos(nZPos, pWidget);
}

/**
*  @brief
*    Get window state
*/
EWindowState Widget::GetWindowState() const
{
	// Return window state
	return m_pWidgetImpl->GetWindowState();
}

/**
*  @brief
*    Set window state
*/
void Widget::SetWindowState(EWindowState nWindowState)
{
	// Set window state
	m_pWidgetImpl->SetWindowState(nWindowState);
}

/**
*  @brief
*    Get background color
*/
Color4 Widget::GetBackgroundColor() const
{
	// Return color
	return m_cBackgroundColor;
}

/**
*  @brief
*    Set background color
*/
void Widget::SetBackgroundColor(const Color4 &cColor)
{
	// Set color
	m_cBackgroundColor = cColor;
}

/**
*  @brief
*    Set transparency mode
*/
void Widget::SetTransparency(ETransparency nTransparency, const Color4 &cColor)
{
	// Set transparency of widget
	m_pWidgetImpl->SetTransparency(nTransparency, cColor);
}

//[-------------------------------------------------------]
//[ State                                                 ]
//[-------------------------------------------------------]
/**
*  @brief
*    Close widget
*/
void Widget::Close()
{
	// Send OnClose-message
	m_pGui->SendMessage(GuiMessage::OnClose(this));
}

/**
*  @brief
*    Check if widget has been destroyed
*/
bool Widget::IsDestroyed() const
{
	// Return if the widget has been destroyed
	return m_pWidgetImpl->IsDestroyed();
}

/**
*  @brief
*    Destroy widget
*/
void Widget::Destroy()
{
	// Destroy widget
	m_pWidgetImpl->Destroy();
}

/**
*  @brief
*    Check if widget is visible
*/
bool Widget::IsVisible() const
{
	// Return if the widget is visible
	return m_pWidgetImpl->IsVisible();
}

/**
*  @brief
*    Show or hide widget
*/
void Widget::SetVisible(bool bVisible)
{
	// Set visibility
	m_pWidgetImpl->SetVisible(bVisible);
}

/**
*  @brief
*    Check if widget is enabled
*/
bool Widget::IsEnabled() const
{
	// Return enabled-state
	return m_pWidgetImpl->IsEnabled();
}

/**
*  @brief
*    Enable or disable widget
*/
void Widget::SetEnabled(bool bEnabled)
{
	// Set enabled-state of widget
	m_pWidgetImpl->SetEnabled(bEnabled);

	// Call virtual function
	if (bEnabled) OnEnable();
	else		  OnDisable();

	// Redraw widget
	Redraw();
}

/**
*  @brief
*    Get focus style
*/
EFocusStyle Widget::GetFocusStyle() const
{
	// Return style
	return m_nFocusStyle;
}

/**
*  @brief
*    Set focus style
*/
void Widget::SetFocusStyle(EFocusStyle nFocusStyle)
{
	// Set style
	m_nFocusStyle = nFocusStyle;
}

/**
*  @brief
*    Check if widget can accept the focus
*/
bool Widget::CanAcceptFocus() const
{
	// Check if widget can accept the focus
	return (m_nFocusStyle != NoFocus && IsEnabled());
}

/**
*  @brief
*    Get tab-stop
*/
bool Widget::IsTabStop()
{
	// Return tab-stop
	return m_bTabStop;
}

/**
*  @brief
*    Set tab-stop
*/
void Widget::SetTabStop(bool bTabStop)
{
	// Set tab-stop
	m_bTabStop = bTabStop;
}

/**
*  @brief
*    Check if widget has the focus
*/
bool Widget::HasFocus() const
{
	// Return if the widget has the focus
	return m_pGui->GetFocusWidget() == this;
}

/**
*  @brief
*    Set keyboard focus to the widget
*/
bool Widget::SetFocus()
{
	// Can we accept the focus?
	if (CanAcceptFocus()) {
		// Can this widget receive the focus?
		if (m_nFocusStyle == AcceptFocus) {
			// Yes. Set focus to this widget
			m_pWidgetImpl->SetFocus();
			return true;
		} else if (m_nFocusStyle == ChildFocus || m_nFocusStyle == ChildFocusOrSelf) {
			// Check, if no child already has the focus
			Widget *pFocus = m_pGui->GetFocusWidget();
			if (pFocus && pFocus->GetCommonParent(*this) == this) {
				// A child widget already has the focus, so don't change anything
				return false;
			}

			// Try to set focus to a child window
			for (uint32 i=0; i<m_lstChildren.GetNumOfElements(); i++) {
				// Set focus to child window
				if (m_lstChildren[i]->SetFocus())
					return true;
			}

			// Accept focus, when no child widget wanted it?
			if (m_nFocusStyle == ChildFocusOrSelf) {
				// Yes. Set focus to this widget
				m_pWidgetImpl->SetFocus();
				return true;
			}
		}
	}

	// Could not set focus at all
	return false;
}

/**
*  @brief
*    Check if widget is active (either the widget or one of it's children has the focus)
*/
bool Widget::IsActive() const
{
	// Return if the widget or one of it's children has the focus
	if (m_pGui->GetFocusWidget())
		return m_pGui->GetFocusWidget()->GetCommonParent(*this) == this;
	else
		return false;
}

/**
*  @brief
*    Activate a widget (bring it to the top and set the focus to it)
*/
void Widget::Activate()
{
	// Make widget visible, bring it to the top and set the focus
	SetVisible(true);
	m_pWidgetImpl->Activate();
}

/**
*  @brief
*    Give keyboard focus to next tab-stop widget
*/
bool Widget::NextTabStop(bool bForward)
{
	// Start with this widget
	Widget *pWidget = this;

	// Activate next/previous sibling
	do {
		// Get next sibling
		if (bForward) pWidget = pWidget->GetNextSibling();
		else		  pWidget = pWidget->GetPreviousSibling();
		if (pWidget) {
			// Activate first/last tab-stop
			bool bDone = (bForward ? pWidget->ActivateFirstTabStop() : pWidget->ActivateLastTabStop());
			if (bDone) {
				// Next/previous tab-stop has been found
				return true;
			}
		}
	} while (pWidget);

	// No more siblings, find next tab stop from parent
	Widget *pParent = GetParent();
	if (pParent) {
		return pParent->NextTabStop(bForward);
	}

	// Last resort: Activate first/last tab-stop
	return (bForward ? ActivateFirstTabStop() : ActivateLastTabStop());
}

/**
*  @brief
*    Check if the mouse is currently inside the widget
*/
bool Widget::IsMouseIn() const
{
	// Return if the widget is the mouse-over widget
	return m_pGui->GetMouseOverWidget() == this;
}

/**
*  @brief
*    Check if the mouse is currently over either the widget or one of it's child widgets
*/
bool Widget::IsMouseOver() const
{
	// Return if the widget or one of it's children is the mouse-over widget
	if (m_pGui->GetMouseOverWidget())
		return m_pGui->GetMouseOverWidget()->GetCommonParent(*this) == this;
	else
		return false;
}

/**
*  @brief
*    Get current mouse cursor position inside the widget
*/
bool Widget::GetMousePos(Vector2i &vPos) const
{
	// Check if the mouse is currently over either the widget or one of it's child widgets
	return IsMouseOver() ? m_pWidgetImpl->GetMousePos(vPos) : false;
}

/**
*  @brief
*    Get widget state
*/
uint32 Widget::GetWidgetState() const
{
	// Get widget state
	uint32 nWidgetState = 0;
	if (IsEnabled())	nWidgetState |= WidgetEnabled;
	if (IsActive())		nWidgetState |= WidgetActive;
	if (HasFocus())		nWidgetState |= WidgetFocus;
	if (IsMouseOver())	nWidgetState |= WidgetMouseOver;

	// Return state
	return nWidgetState;
}

/**
*  @brief
*    Mark widget to be redrawn in the next frame
*/
void Widget::Redraw()
{
	// Redraw widget
	m_pWidgetImpl->Redraw();
}

/**
*  @brief
*    Redraw widget and all child widgets
*/
void Widget::RedrawAll()
{
	// Redraw widget
	Redraw();

	// Redraw child widgets
	for (uint32 i=0; i<m_lstChildren.GetNumOfElements(); i++) {
		m_lstChildren[i]->Redraw();
	}
}

//[-------------------------------------------------------]
//[ Input                                                 ]
//[-------------------------------------------------------]
/**
*  @brief
*    Check if widget has captured mouse input
*/
bool Widget::IsCaptureMouse() const
{
	// Return capture-flag
	return m_bCaptureMouse;
}

/**
*  @brief
*    Capture mouse input
*/
void Widget::SetCaptureMouse(bool bCapture)
{
	// Capture mouse
	m_pWidgetImpl->SetCaptureMouse(bCapture);

	// Set capture-flag
	m_bCaptureMouse = bCapture;
}

/**
*  @brief
*    Trap mouse inside the widget
*/
void Widget::SetTrapMouse(bool bTrap)
{
	// Trap mouse
	m_pWidgetImpl->SetTrapMouse(bTrap);
}

/**
*  @brief
*    Register a hotkey
*/
uint32 Widget::RegisterHotkey(uint32 nModKey, uint32 nKey)
{
	// Add hotkey
	return m_pWidgetImpl->RegisterHotkey(nModKey, nKey);
}

/**
*  @brief
*    Unregister a hotkey
*/
void Widget::UnregisterHotkey(uint32 nID)
{
	// Remove hotkey
	m_pWidgetImpl->UnregisterHotkey(nID);
}

//[-------------------------------------------------------]
//[ Theme                                                 ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get theme
*/
Theme *Widget::GetTheme() const
{
	// Return theme
	return m_pTheme;
}

/**
*  @brief
*    Set theme
*/
void Widget::SetTheme(Theme *pTheme)
{
	// Is theme pointer valid?
	if (pTheme) {
		// Set new theme
		m_pTheme = pTheme;

		// Set theme on child widgets
		for (uint32 i=0; i<m_lstChildren.GetNumOfElements(); i++) {
			m_lstChildren[i]->SetTheme(pTheme);
		}

		// Inform about theme change
		m_pGui->PostMessage(GuiMessage::OnThemeChanged(this));
	}
}

//[-------------------------------------------------------]
//[ Modifier                                              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get modifiers
*/
const List<Modifier*> &Widget::GetModifiers() const
{
	// Return modifier list
	return m_lstModifiers;
}

/**
*  @brief
*    Get modifier
*/
Modifier *Widget::GetModifier(const String &sName) const
{
	// Return modifier
	return m_mapModifiers.Get(sName);
}

/**
*  @brief
*    Add modifier
*/
void Widget::AddModifier(Modifier *pModifier)
{
	// Add modifier (no name)
	AddModifier("", pModifier);
}

/**
*  @brief
*    Add modifier
*/
void Widget::AddModifier(const String &sName, Modifier *pModifier)
{
	// Check if modifier is valid
	if (pModifier) {
		// Destroy old modifier if name is already in use
		if (sName.GetLength() > 0)
			RemoveModifier(sName);

		// Set modifier name
		pModifier->SetName(sName);

		// Add modifier to list
		m_lstModifiers.Add(pModifier);
		if (sName.GetLength())
			m_mapModifiers.Add(sName, pModifier);

		// Attach modifier to widget
		pModifier->Attach(*this);
	}
}

/**
*  @brief
*    Add modifier
*/
void Widget::AddModifier(const String &sClass, const String &sOptions)
{
	// Add modifier (no name)
	AddModifier("", sClass, sOptions);
}

/**
*  @brief
*    Add modifier
*/
void Widget::AddModifier(const String &sName, const String &sClass, const String &sOptions)
{
	// Find modifier class
	const Class *pClass = ClassManager::GetInstance()->GetClass(sClass);
	if (!pClass) {
		// Try it with "PLGui::<Class>"
		pClass = ClassManager::GetInstance()->GetClass("PLGui::" + sClass);
	}

	// Class found?
	if (pClass && pClass->IsDerivedFrom("PLGui::Modifier")) {
		// Create modifier
		Modifier *pModifier = static_cast<Modifier*>(pClass->Create());
		if (pModifier) {
			// Set options
			if (sOptions.GetLength())
				pModifier->SetValues(sOptions);

			// Add modifier
			AddModifier(sName, pModifier);
		}
	}
}

/**
*  @brief
*    Remove modifier
*/
void Widget::RemoveModifier(Modifier *pModifier)
{
	// Check if modifier is valid and in the list
	if (pModifier && m_lstModifiers.IsElement(pModifier)) {
		// Remove modifier from list
		m_lstModifiers.Remove(pModifier);

		// Get name of modifier
		String sName = pModifier->GetName();
		if (sName.GetLength() > 0 && m_mapModifiers.Get(sName) == pModifier) {
			// Remove from hash map
			m_mapModifiers.Remove(sName);
		}

		// Delete modifier
		delete pModifier;
	}
}

/**
*  @brief
*    Remove modifier
*/
void Widget::RemoveModifier(const String &sName)
{
	// Get modifier
	Modifier *pModifier = GetModifier(sName);
	if (pModifier) {
		// Remove modifier
		RemoveModifier(pModifier);
	}
}

//[-------------------------------------------------------]
//[ Layout                                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get layout
*/
Layout *Widget::GetLayout() const
{
	// Return layout
	return m_pLayout;
}

/**
*  @brief
*    Set layout
*/
void Widget::SetLayout(Layout *pLayout)
{
	// Destroy old layout
	if (m_pLayout) {
		delete m_pLayout;
	}

	// Set layout
	m_pLayout = pLayout;
	if (m_pLayout) {
		m_pLayout->SetWidget(this);
		m_pLayout->ApplyLayout();
	}
}

/**
*  @brief
*    Set layout
*/
void Widget::SetLayout(const String &sClass, const String &sOptions)
{
	// Layout
	Layout *pLayout = nullptr;

	// Find layout class
	const Class *pClass = ClassManager::GetInstance()->GetClass(sClass);
	if (!pClass) {
		// Try it with "PLGui::<Class>"
		pClass = ClassManager::GetInstance()->GetClass("PLGui::" + sClass);
	}

	// Class found?
	if (pClass && pClass->IsDerivedFrom("PLGui::Layout")) {
		// Create modifier
		pLayout = static_cast<Layout*>(pClass->Create());
		if (pLayout) {
			// Set options
			if (sOptions.GetLength())
				pLayout->SetValues(sOptions);
		}
	}

	// Set layout
	SetLayout(pLayout);
}

/**
*  @brief
*    Get layout hints
*/
LayoutHints &Widget::GetLayoutHints()
{
	// Return layout hints
	return m_cLayoutHints;
}

/**
*  @brief
*    Adjust content of widget
*/
void Widget::AdjustContent()
{
	// Send OnAdjustContent message
	m_pGui->SendMessage(GuiMessage::OnAdjustContent(this));
}

/**
*  @brief
*    Update content
*/
void Widget::UpdateContent()
{
	// Send OnUpdateContent message
	m_pGui->SendMessage(GuiMessage::OnUpdateContent(this));
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Widget::Widget(Gui *pGui) :
	ID(this),
	Name(this),
	Title(this),
	Pos(this),
	Size(this),
	Topmost(this),
	BackgroundColor(this),
	m_pGui(pGui),
	m_bManaged(false),
	m_bRootWidget(true),
	m_nID(m_pGui->GetUniqueWidgetID()),
	m_sName(""),
	m_pParent(nullptr),
	m_pWidgetImpl(nullptr),
	m_pTheme(nullptr),
	m_pLayout(nullptr),
	m_nWindowState(StateNormal),
	m_bTopmost(false),
	m_sTitle("No Title"),
	m_cIcon(*m_pGui),
	m_cCursor(*m_pGui, CursorArrow),
	m_nFocusStyle(NoFocus),
	m_bTabStop(true),
	m_bCaptureMouse(false),
	m_cBackgroundColor(Color4::White),
	m_vPreferredSize(-1, -1),
	m_pUserData(nullptr)
{
	// This is a dummy for the root widget, nothing to do
}

/**
*  @brief
*    Add child widget
*/
void Widget::AddChild(Widget *pWidget)
{
	// Add widget to list
	m_lstChildren.Add(pWidget);

	// Call virtual function
	OnAddChildWidget(pWidget);
}

/**
*  @brief
*    Remove child widget
*/
void Widget::RemoveChild(Widget *pWidget)
{
	// Remove widget from list
	m_lstChildren.Remove(pWidget);

	// Call virtual function
	OnRemoveChildWidget(pWidget);
}

/**
*  @brief
*    Give focus to first tab-stop widget (including this and all children)
*/
bool Widget::ActivateFirstTabStop()
{
	// Check if we can accept the focus ourself
	if (IsEnabled() && m_nFocusStyle == AcceptFocus && m_bTabStop) {
		// Set focus
		SetFocus();
		return true;
	}

	// Otherwise try to give focus to child widgets
	for (uint32 i=0; i<m_lstChildren.GetNumOfElements(); i++) {
		// Try to activate child widget
		if (m_lstChildren[i]->ActivateFirstTabStop())
			return true;
	}

	// Could not find tab-stop widget
	return false;
}

/**
*  @brief
*    Give focus to last tab-stop widget (including this and all children)
*/
bool Widget::ActivateLastTabStop()
{
	// Check if we can accept the focus ourself
	if (IsEnabled() && m_nFocusStyle == AcceptFocus && m_bTabStop) {
		// Set focus
		SetFocus();
		return true;
	}

	// Otherwise try to give focus to child widgets
	uint32 nNum = m_lstChildren.GetNumOfElements();
	if (nNum > 0) {
		for (uint32 i=0; i<nNum; i++) {
			// Try to activate child widget
			if (m_lstChildren[nNum-i-1]->ActivateLastTabStop())
				return true;
		}
	}

	// Could not find tab-stop widget
	return false;
}

/**
*  @brief
*    Draw widget background
*/
void Widget::DrawBackground(Graphics &cGraphics)
{
	// Draw background color
	if (!m_cBackgroundColor.IsTransparent()) {
		cGraphics.DrawBox(m_cBackgroundColor, Vector2i(0, 0), GetSize(), 0, 0);
	}

	// Call signal and virtual function
	SignalDrawBackground(cGraphics);
	OnDrawBackground(cGraphics);
}

/**
*  @brief
*    Draw widget
*/
void Widget::Draw(Graphics &cGraphics)
{
	// Call signal and virtual function
	SignalDraw(cGraphics);
	OnDraw(cGraphics);
}


//[-------------------------------------------------------]
//[ Protected virtual WidgetFunctions functions           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Process GUI message
*/
void Widget::OnMessage(const GuiMessage &cMessage)
{
	// Inform modifiers
	for (uint32 i=0; i<m_lstModifiers.GetNumOfElements(); i++) {
		// Send message to modifier
		m_lstModifiers[i]->OnMessage(cMessage);
	}

	// Get message type
	switch (cMessage.GetType()) {
		// Theme has been changed
		case MessageOnThemeChanged:
			// Call signal and virtual function
			SignalOnThemeChanged();
			OnThemeChanged();
			break;

		// Widget content has been changed
		case MessageOnUpdateContent:
			// Call signal and virtual function
			SignalUpdateContent();
			OnUpdateContent();
			break;

		// Child widget has been changed
		case MessageOnUpdateChildWidget:
			// Call signal and virtual function
			SignalUpdateChildWidget(cMessage.GetChildWidget());
			OnUpdateChildWidget(cMessage.GetChildWidget());

			// Adjust content
			AdjustContent();
			break;

		// Child widget has been added
		case MessageOnAddChildWidget:
			// Call signal and virtual function
			SignalAddChildWidget(cMessage.GetChildWidget());
			OnAddChildWidget(cMessage.GetChildWidget());

			// Adjust content
			AdjustContent();
			break;

		// Child widget has been removed
		case MessageOnRemoveChildWidget:
			// Call signal and virtual function
			SignalRemoveChildWidget(cMessage.GetChildWidget());
			OnRemoveChildWidget(cMessage.GetChildWidget());

			// Adjust content
			AdjustContent();
			break;

		// Widget shall be closed (ALT+F4 or X-Button pressed)
		case MessageOnClose:
			// Call signal and virtual function
			SignalClose();
			OnClose();
			break;

		// Widget has just been created
		case MessageOnCreate:
			// Call signal and virtual function
			SignalCreate();
			OnCreate();
			break;

		// Widget is going to be destroyed
		case MessageOnDestroy:
			// Call signal and virtual function
			SignalDestroy();
			OnDestroy();
			break;

		// Widget gets shown
		case MessageOnShow:
			// Call signal and virtual function
			SignalShow();
			OnShow();
			break;

		// Widget gets hidden
		case MessageOnHide:
			// Call signal and virtual function
			SignalHide();
			OnHide();
			break;

		// Widget gets enabled
		case MessageOnEnable:
			// Call signal and virtual function
			SignalEnable();
			OnEnable();
			break;

		// Widget gets disabled
		case MessageOnDisable:
			// Call signal and virtual function
			SignalDisable();
			OnDisable();
			break;

		// Widget gets focus
		case MessageOnGetFocus:
			// Call signal and virtual function
			SignalGetFocus();
			OnGetFocus();
			break;

		// Widget looses focus
		case MessageOnLooseFocus:
			// Call signal and virtual function
			SignalLooseFocus();
			OnLooseFocus();
			break;

		// Widget has been activated or deactivated (focus-widget has changed)
		case MessageOnActivate:
			// Call signal and virtual function
			SignalActivate(cMessage.GetState());
			OnActivate(cMessage.GetState());
			break;

		// Widget background gets drawn, graphics object used for painting as parameter
		case MessageOnDrawBackground:
			// Draw widget background
			DrawBackground(*cMessage.GetGraphics());
			break;

		// Widget gets drawn, graphics object used for painting as parameter
		case MessageOnDraw:
			// Draw widget
			Draw(*cMessage.GetGraphics());
			break;

		// Widget gets moved, new widget position as parameter
		case MessageOnMove:
			// Call signal and virtual function
			SignalMove(cMessage.GetPosSize());
			OnMove(cMessage.GetPosSize());
			break;

		// Widget gets resized, new widget size as parameter
		case MessageOnSize:
			// Save new size
			m_vSize = cMessage.GetPosSize();

			// Call signal and virtual function
			SignalSize(cMessage.GetPosSize());
			OnSize(cMessage.GetPosSize());

			// Adjust content
			AdjustContent();
			break;

		// Window state has changed, new window state as parameter
		case MessageOnWindowState:
		{
			// Get new window state
			EWindowState nWindowState = cMessage.GetWindowState();

			// Enter fullscreen mode?
			if (m_nWindowState != StateFullscreen && nWindowState == StateFullscreen) {
				m_pGui->SendMessage(GuiMessage::OnEnterFullscreen(this));
			}

			// Call signal and virtual function
			SignalWindowState(nWindowState);
			OnWindowState(nWindowState);

			// Leave fullscreen mode?
			if (m_nWindowState == StateFullscreen && nWindowState != StateFullscreen) {
				m_pGui->SendMessage(GuiMessage::OnLeaveFullscreen(this));
			}

			// Set new window state
			m_nWindowState = nWindowState;

			// Adjust content
			AdjustContent();
			break;
		}

		// Widget enters fullscreen mode
		case MessageOnEnterFullscreen:
			// Call signal and virtual function
			SignalEnterFullscreen();
			OnEnterFullscreen();

			// Adjust content
			AdjustContent();
			break;

		// Widget leaves fullscreen mode
		case MessageOnLeaveFullscreen:
			// Call signal and virtual function
			SignalLeaveFullscreen();
			OnLeaveFullscreen();

			// Adjust content
			AdjustContent();
			break;

		// Widget has calculated it's preferred size
		case MessageOnPreferredSize:
			// Call signal and virtual function
			SignalPreferredSize(cMessage.GetPosSize());
			m_vPreferredSize = OnPreferredSize(cMessage.GetPosSize());
			break;

		// Widget content has to be adjusted
		case MessageOnAdjustContent:
			// Call signal and virtual function
			SignalAdjustContent();
			OnAdjustContent();
			break;

		// Mouse enters widget
		case MessageOnMouseEnter:
			// Call signal and virtual function
			SignalMouseEnter();
			OnMouseEnter();
			break;

		// Mouse leaves widget
		case MessageOnMouseLeave:
			// Call signal and virtual function
			SignalMouseLeave();
			OnMouseLeave();
			break;

		// Mouse-over widget has changed
		case MessageOnMouseOver:
			// Call signal and virtual function
			SignalMouseOver(cMessage.GetState());
			OnMouseOver(cMessage.GetState());
			break;

		// Mouse moves inside the widget, mouse position within the widget as parameter
		case MessageOnMouseMove:
			// Call signal and virtual function
			SignalMouseMove(cMessage.GetPosSize());
			OnMouseMove(cMessage.GetPosSize());
			break;

		// Mouse hovers over the widget
		case MessageOnMouseHover:
			// Call signal and virtual function
			SignalMouseHover();
			OnMouseHover();
			break;

		// Mouse position inside the widget has changed due to the movement of widget
		case MessageOnMousePosUpdate:
			// Call signal and virtual function
			SignalMousePosUpdate(cMessage.GetPosSize());
			OnMousePosUpdate(cMessage.GetPosSize());
			break;

		// Mouse button is pressed, mouse button and mouse position within the widget as parameters
		case MessageOnMouseButtonDown:
			// Call signal and virtual function
			SignalMouseButtonDown(cMessage.GetMouseButton(), cMessage.GetPosSize());
			OnMouseButtonDown(cMessage.GetMouseButton(), cMessage.GetPosSize());
			break;

		// Mouse button is released, mouse button and mouse position within the widget as parameters
		case MessageOnMouseButtonUp:
			// Call signal and virtual function
			SignalMouseButtonUp(cMessage.GetMouseButton(), cMessage.GetPosSize());
			OnMouseButtonUp(cMessage.GetMouseButton(), cMessage.GetPosSize());
			break;

		// Mouse button has been clicked, mouse button and mouse position within the widget as parameters
		case MessageOnMouseButtonClick:
			// Call signal and virtual function
			SignalMouseButtonClick(cMessage.GetMouseButton(), cMessage.GetPosSize());
			OnMouseButtonClick(cMessage.GetMouseButton(), cMessage.GetPosSize());
			break;

		// Mouse button has been double-clicked, mouse button and mouse position within the widget as parameters
		case MessageOnMouseButtonDoubleClick:
			// Call signal and virtual function
			SignalMouseButtonDoubleClick(cMessage.GetMouseButton(), cMessage.GetPosSize());
			OnMouseButtonDoubleClick(cMessage.GetMouseButton(), cMessage.GetPosSize());
			break;

		// Mouse wheel moved, mouse wheel movement as parameter
		case MessageOnMouseWheel:
			// Call signal and virtual function
			SignalMouseWheel(cMessage.GetDelta());
			OnMouseWheel(cMessage.GetDelta());
			break;

		// Key gets pressed, pressed key and modifier keys pressed as parameters
		case MessageOnKeyDown:
		{
			// Activate next/previous tab stop
			bool bShift = ((cMessage.GetModifiers() & PLGUIMOD_SHIFT) != 0);
			if (cMessage.GetKey() == PLGUIKEY_TAB) {
				NextTabStop(!bShift);
			}

			// Call signal and virtual function
			SignalKeyDown(cMessage.GetKey(), cMessage.GetModifiers());
			OnKeyDown(cMessage.GetKey(), cMessage.GetModifiers());
			break;
		}

		// Key gets released, released key and modifier keys pressed as parameters
		case MessageOnKeyUp:
			// Call signal and virtual function
			SignalKeyUp(cMessage.GetKey(), cMessage.GetModifiers());
			OnKeyUp(cMessage.GetKey(), cMessage.GetModifiers());
			break;

		// Hotkey pressed, hotkey ID as parameter
		case MessageOnHotkey:
			// Call signal and virtual function
			SignalHotkey(cMessage.GetKey());
			OnHotkey(cMessage.GetKey());
			break;

		// Data has been dropped onto the widget
		case MessageOnDrop:
			// Send message to parent
			if (m_pParent) {
				m_pGui->SendMessage(GuiMessage::OnDrop(m_pParent, cMessage.GetDataObject()));
			}

			// Call signal and virtual function
			SignalDrop(*cMessage.GetDataObject());
			OnDrop(*cMessage.GetDataObject());
			break;

		// User message
		case MessageOnUserMessage:
			// Call signal and virtual function
			SignalUserMessage(cMessage.GetData(), cMessage.GetDataPointer());
			OnUserMessage(cMessage.GetData(), cMessage.GetDataPointer());
			break;

		case MessageOnUnknown:
		case MessageOnInternalMessage:
		case MessageOnWakeup:
		case MessageOnExit:
		case MessageOnTimer:
			// [TODO] Check me: Do we need to handle those cases in here?
			break;
	}
}

Vector2i Widget::OnPreferredSize(const Vector2i &vRefSize) const
{
	// Return (-1, -1) as default size to use "as much space as possible" in layouts
	return Vector2i(-1, -1);
}

void Widget::OnThemeChanged()
{
	// Redraw
	Redraw();
}

void Widget::OnUpdateContent()
{
	// UpdateContent signal
	SignalUpdateContent();

	// Call virtual function from parent
	if (m_pParent) {
		m_pParent->OnUpdateChildWidget(this);
	}
}

void Widget::OnAdjustContent()
{
	// Emit signal
	SignalAdjustContent();

	// Apply layout
	if (m_pLayout) {
		m_pLayout->ApplyLayout();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP
