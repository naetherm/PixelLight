/*********************************************************\
 *  File: Window.cpp                                     *
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
#include <PLGraphics/Color/Color4.h>
#include "PLGui/Themes/Theme.h"
#include "PLGui/Widgets/Controls/SystemButton.h"
#include "PLGui/Widgets/Windows/Window.h"


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
using namespace PLGraphics;
namespace PLGui {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(Window)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Window::Window(Widget *pParent) : ContainerWidget(pParent),
	SystemButtons(this),
	ShowTitleBar(this),
	ShowBorder(this),
	Resizable(this),
	MinSize(this),
	MaxSize(this),
	m_nSystemButtons(SystemButtonMinimize | SystemButtonMaximize | SystemButtonClose),
	m_bShowTitleBar(true),
	m_bShowBorder(true),
	m_bResizable(true),
	m_vMinSize(-1, -1),
	m_vMaxSize(-1, -1),
	m_pSysButtonMinimize(nullptr),
	m_pSysButtonMaximize(nullptr),
	m_pSysButtonClose(nullptr),
	m_bMouseTitle(false),
	m_bMouseLeft(false),
	m_bMouseRight(false),
	m_bMouseTop(false),
	m_bMouseBottom(false),
	m_bMove(false),
	m_bSizeXLeft(false),
	m_bSizeXRight(false),
	m_bSizeYTop(false),
	m_bSizeYBottom(false)
{
	// Don't draw a background
	SetBackgroundColor(Color4::Transparent);

	// A window is usually shown in the taskbar
	SetShowInTaskbar(true);

	// Set minimum size so that the title bar and a little bit of the content window are always visible
	SetMinSize(Vector2i(150, 50));

	// Set layout
	GetContentWidget()->SetLayout("LayoutBox", "Orientation=Vertical");

	// Create system button: Minimize
	m_pSysButtonMinimize = new SystemButton(this);
	m_pSysButtonMinimize->SetSize(Vector2i(16, 16));
	m_pSysButtonMinimize->SetSystemCommand(CommandMinimize);

	// Create system button: Maximize
	m_pSysButtonMaximize = new SystemButton(this);
	m_pSysButtonMaximize->SetSize(Vector2i(16, 16));
	m_pSysButtonMaximize->SetSystemCommand(CommandMaximize);

	// Create system button: Close
	m_pSysButtonClose = new SystemButton(this);
	m_pSysButtonClose->SetSize(Vector2i(16, 16));
	m_pSysButtonClose->SetSystemCommand(CommandClose);

	// Show system buttons
	SetSystemButtons(m_nSystemButtons);
}

/**
*  @brief
*    Destructor
*/
Window::~Window()
{
}

/**
*  @brief
*    Get system buttons
*/
uint32 Window::GetSystemButtons() const
{
	// Return system buttons
	return m_nSystemButtons;
}

/**
*  @brief
*    Set system buttons
*/
void Window::SetSystemButtons(uint32 nButtons)
{
	// Set system buttons
	m_nSystemButtons = nButtons;

	// Show system buttons
	m_pSysButtonMinimize->SetVisible(true);
	if (m_pSysButtonMinimize) m_pSysButtonMinimize->SetEnabled((m_nSystemButtons & SystemButtonMinimize) != 0);
	m_pSysButtonMaximize->SetVisible(true);
	if (m_pSysButtonMaximize) m_pSysButtonMaximize->SetEnabled((m_nSystemButtons & SystemButtonMaximize) != 0);
	m_pSysButtonClose->SetVisible(true);
	if (m_pSysButtonClose)	  m_pSysButtonClose	  ->SetEnabled((m_nSystemButtons & SystemButtonClose) != 0);
}

/**
*  @brief
*    Check if a title bar is displayed
*/
bool Window::GetShowTitleBar() const
{
	// Return title bar visibility
	return m_bShowTitleBar;
}

/**
*  @brief
*    Set if a title bar is displayed
*/
void Window::SetShowTitleBar(bool bVisible)
{
	// Set title bar visibility
	m_bShowTitleBar = bVisible;

	// Show or hide system buttons
	if (bVisible) {
		// Restore system button
		SetSystemButtons(m_nSystemButtons);
	} else {
		// Hide system buttons
		if (m_pSysButtonMinimize) m_pSysButtonMinimize->SetVisible(false);
		if (m_pSysButtonMaximize) m_pSysButtonMaximize->SetVisible(false);
		if (m_pSysButtonClose)	  m_pSysButtonClose->SetVisible	  (false);
	}

	// Adjust content
	AdjustContent();
}

/**
*  @brief
*    Check if a border is displayed
*/
bool Window::GetShowBorder() const
{
	// Return border visibility
	return m_bShowBorder;
}

/**
*  @brief
*    Set if a border is displayed
*/
void Window::SetShowBorder(bool bVisible)
{
	// Set border visibility
	m_bShowBorder = bVisible;

	// Adjust content
	AdjustContent();

	// Redraw
	Redraw();
}

/**
*  @brief
*    Check if the window is resizable
*/
bool Window::GetResizable() const
{
	// Return resizable-flag
	return m_bResizable;
}

/**
*  @brief
*    Set if the window is resizable
*/
void Window::SetResizable(bool bResizable)
{
	// Set resizable-flag
	m_bResizable = bResizable;
}

/**
*  @brief
*    Get minimum size for resizing
*/
Vector2i Window::GetMinSize() const
{
	// Return minimum size
	return m_vMinSize;
}

/**
*  @brief
*    Set minimum size for resizing
*/
void Window::SetMinSize(const Vector2i &vSize)
{
	// Set minimum size
	m_vMinSize = vSize;
}

/**
*  @brief
*    Get maximum size for resizing
*/
Vector2i Window::GetMaxSize() const
{
	// Return maximum size
	return m_vMaxSize;
}

/**
*  @brief
*    Set maximum size for resizing
*/
void Window::SetMaxSize(const Vector2i &vSize)
{
	// Set maximum size
	m_vMaxSize = vSize;
}


//[-------------------------------------------------------]
//[ Protected virtual WidgetFunctions functions           ]
//[-------------------------------------------------------]
void Window::OnDraw(Graphics &cGraphics)
{
	// Draw window border
	if (GetWindowState() != StateFullscreen) {
		m_pTheme->DrawWindow(cGraphics, Vector2i::Zero, GetSize() - Vector2i(1, 1), m_bShowBorder, GetWidgetState(), GetTitle(), GetIcon());
	}
}

void Window::OnAdjustContent()
{
	// Get title bar height
	int nTitleBarHeight = m_pTheme->GetWindowTitleBarHeight();
	int nBorderSize		= m_pTheme->GetWindowBorderSize();

	// Border width
	const int nBorder	= (m_bShowBorder   ? nBorderSize : 0);
	const int nTitleBar	= (m_bShowTitleBar ? nTitleBarHeight : 0);
	const int nButtonX	= m_pTheme->GetSysButtonSize().x;
	const int nButtonY	= m_pTheme->GetSysButtonSize().y;

	// Set position of content window according to border and title bar
	Vector2i vPos(nBorder, nTitleBar + nBorder);
	if (vPos.x < 0) vPos.x = 0;
	if (vPos.y < 0) vPos.y = 0;
	m_pContentWidget->SetPos(vPos);

	// Set size of content window according to border and title bar
	Vector2i vSize(m_vSize.x - 2*nBorder, m_vSize.y - nTitleBar - 2*nBorder);
	if (vSize.x < 1) vSize.x = 1;
	if (vSize.y < 1) vSize.y = 1;
	m_pContentWidget->SetSize(vSize);

	// Position system buttons
	if (m_pSysButtonMinimize)	m_pSysButtonMinimize->SetPos(Vector2i(GetSize().x - 4 - (nButtonX+1)*3, nBorder+(nTitleBar-nButtonY)/2));
	if (m_pSysButtonMaximize)	m_pSysButtonMaximize->SetPos(Vector2i(GetSize().x - 4 - (nButtonX+1)*2, nBorder+(nTitleBar-nButtonY)/2));
	if (m_pSysButtonClose)		m_pSysButtonClose	->SetPos(Vector2i(GetSize().x - 4 - (nButtonX+1),   nBorder+(nTitleBar-nButtonY)/2));
}

void Window::OnWindowState(EWindowState nWindowState)
{
	// Show/hide border based on whether or not the window is maximized
	if (nWindowState == StateMaximized)			SetShowBorder(false);
	else if (nWindowState != StateFullscreen)	SetShowBorder(true);

	// If maximized, switch to "restore"-button, else switch to "maximize"-button
	if (nWindowState == StateMaximized)	m_pSysButtonMaximize->SetSystemCommand(CommandRestore);
	else								m_pSysButtonMaximize->SetSystemCommand(CommandMaximize);
}

void Window::OnEnterFullscreen()
{
	// Hide title bar and border in fullscreen mode
	SetShowTitleBar(false);
	SetShowBorder(false);
}

void Window::OnLeaveFullscreen()
{
	// Hide title bar and border in fullscreen mode
	SetShowTitleBar(true);
	SetShowBorder(true);
}

void Window::OnActivate(bool bActivate)
{
	// Redraw window (title bar color has changed)
	RedrawAll();
}

void Window::OnMove(const Vector2i &vPos)
{
	// Save widget position
	m_vPos = vPos;
}

void Window::OnMouseMove(const Vector2i &vPos)
{
	// Get window state
	bool	  bResizable  = GetResizable();
	Vector2i  vSize		  = GetSize();
	uint8	  nBorderSize = 2;
	Vector2i  vMinSize	  = GetMinSize();
	Vector2i  vMaxSize	  = GetMaxSize();

	// Get title bar height
	int nTitleBarHeight = m_pTheme->GetWindowTitleBarHeight();

	// Check if the mouse cursor is positioned on a border or on the title bar
	m_bMouseTitle = m_bMouseLeft = m_bMouseRight = m_bMouseTop = m_bMouseBottom = false;
	if (vPos.x < nBorderSize)						m_bMouseLeft   = true;	// Left border
	else if (vPos.x > vSize.x - nBorderSize - 1)	m_bMouseRight  = true;	// Right border
	if (vPos.y < nBorderSize)						m_bMouseTop    = true;	// Top border
	else if (vPos.y > vSize.y - nBorderSize - 1)	m_bMouseBottom = true;	// Bottom border
	else if (vPos.y < nTitleBarHeight)				m_bMouseTitle  = true;	// Title bar

	// In corner cases, "extend" the area a bit to make it easier
	if ((m_bMouseLeft || m_bMouseRight) && vPos.y < 20)					m_bMouseTop		= true;	// Corner case top left/right
	if ((m_bMouseLeft || m_bMouseRight) && vPos.y > vSize.y - 20 - 1)	m_bMouseBottom	= true; // Corner case bottom left/right
	if ((m_bMouseTop || m_bMouseBottom) && vPos.x < 20)					m_bMouseLeft	= true; // Corner case top/bottom left
	if ((m_bMouseTop || m_bMouseBottom) && vPos.x > vSize.x - 20 - 1)	m_bMouseRight	= true; // Corner case top/bottom right

	// Don't recognize borders, if the window is not allowed to be resized
	if (!bResizable) {
		m_bMouseLeft = m_bMouseRight = m_bMouseTop = m_bMouseBottom = false;
	}

	// Set mouse cursor
	Cursor cCursor(*GetGui(), CursorArrow);
	if (GetWindowState() == StateNormal) {
			 if (m_bMove)							cCursor.Load(CursorMove);
		else if (m_bMouseLeft && m_bMouseTop)		cCursor.Load(CursorSizeTopLeft);
		else if (m_bMouseRight && m_bMouseTop)		cCursor.Load(CursorSizeTopRight);
		else if (m_bMouseLeft && m_bMouseBottom)	cCursor.Load(CursorSizeBottomLeft);
		else if (m_bMouseRight && m_bMouseBottom)	cCursor.Load(CursorSizeBottomRight);
		else if (m_bMouseLeft)						cCursor.Load(CursorSizeLeft);
		else if (m_bMouseRight)						cCursor.Load(CursorSizeRight);
		else if (m_bMouseTop)						cCursor.Load(CursorSizeTop);
		else if (m_bMouseBottom)					cCursor.Load(CursorSizeBottom);
	}
	SetCursor(cCursor);

	// Get mouse movement
	Vector2i vRel = vPos - m_vMousePos;

	// Move
	if (m_bMove) {
		SetPos(GetPos() + vRel);
	}

	// Size
	else if (m_bSizeXLeft | m_bSizeXRight | m_bSizeYTop | m_bSizeYBottom) {
		// Get current position and size
		Vector2i vPos_ = GetPos();
				 vSize = GetSize();

		// Alter size
		if (m_bSizeXLeft)   vSize.x -= vRel.x;
		if (m_bSizeXRight)  vSize.x += vRel.x;
		if (m_bSizeYTop)    vSize.y -= vRel.y;
		if (m_bSizeYBottom) vSize.y += vRel.y;

		// Alter position
		if (m_bSizeXLeft)	vPos_.x += vRel.x;
		if (m_bSizeYTop)	vPos_.y += vRel.y;

		// Apply minimum size
		if (vMinSize.x > -1 && vSize.x < vMinSize.x)	vSize.x = vMinSize.x;
		if (vMinSize.y > -1 && vSize.y < vMinSize.y)	vSize.y = vMinSize.y;
		if (vMaxSize.x > -1 && vSize.x > vMaxSize.x)	vSize.x = vMaxSize.x;
		if (vMaxSize.y > -1 && vSize.y > vMaxSize.y)	vSize.y = vMaxSize.y;

		// Set new position and size
		SetPos (vPos_);
		SetSize(vSize);

		// Update new position of mouse cursor
		if (m_bSizeXRight)  m_vMousePos.x = vPos.x;
		if (m_bSizeYBottom) m_vMousePos.y = vPos.y;
	}
}

void Window::OnMousePosUpdate(const Vector2i &vPos)
{
	// Update to new mouse position and don't do anything else
	m_vMousePos = vPos;
}

void Window::OnMouseLeave()
{
	// Reset cursor
	SetCursor(Cursor(*GetGui(), CursorArrow));
}

void Window::OnMouseButtonDown(uint32 nButton, const Vector2i &vPos)
{
	// Get resizable-flag
	bool bResizable = GetResizable();

	// Moving or sizing is only possible, if the window is not maximized, minimized or fullscreen
	if (GetWindowState() == StateNormal) {
		// Check if mouse is on the border
		bool bBorder = (m_bMouseLeft | m_bMouseRight | m_bMouseTop | m_bMouseBottom);

		// Start move
		if (nButton == LeftButton && m_bMouseTitle && !bBorder) {
			m_bMove = true;
			m_bSizeXLeft = m_bSizeXRight = m_bSizeYTop = m_bSizeYBottom = false;
			m_vMousePos = vPos;
			SetCaptureMouse(true);
		}

		// Start size
		else if (nButton == LeftButton && bBorder && bResizable) {
			m_bMove		   = false;
			m_bSizeXLeft   = m_bMouseLeft;
			m_bSizeXRight  = m_bMouseRight;
			m_bSizeYTop    = m_bMouseTop;
			m_bSizeYBottom = m_bMouseBottom;
			m_vMousePos = vPos;
			SetCaptureMouse(true);
		}
	}
}

void Window::OnMouseButtonUp(uint32 nButton, const Vector2i &vPos)
{
	// Stop move
	if (nButton == LeftButton && m_bMove) {
		m_bMove = false;
		SetCaptureMouse(false);
		SetCursor(Cursor(*GetGui(), CursorArrow));
	}

	// Stop size
	if (nButton == LeftButton && (m_bSizeXLeft | m_bSizeXRight | m_bSizeYTop | m_bSizeYBottom)) {
		m_bSizeXLeft = m_bSizeXRight = m_bSizeYTop = m_bSizeYBottom = false;
		SetCaptureMouse(false);
	}
}

void Window::OnMouseButtonDoubleClick(uint32 nButton, const Vector2i &vPos)
{
	// Get title bar height
	int nTitleBarHeight = m_pTheme->GetWindowTitleBarHeight();

	// If the user has double-clicked inside the title bar
	if (nButton == LeftButton && vPos.y >= 0 && vPos.y < nTitleBarHeight) {
		// Toggle between maximized and restored size
		if (GetWindowState() == StateNormal) SetWindowState(StateMaximized);
		else								 SetWindowState(StateNormal);
	}
}


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
