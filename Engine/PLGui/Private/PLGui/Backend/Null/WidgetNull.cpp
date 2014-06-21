/*********************************************************\
 *  File: WidgetNull.cpp                                 *
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
#include "PLGui/Backend/Null/WidgetNull.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLGui {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
WidgetNull::WidgetNull(Widget &cWidget) :
	WidgetImpl(cWidget)
{
}

/**
*  @brief
*    Destructor
*/
WidgetNull::~WidgetNull()
{
	// If not done already, destroy the widget
	Destroy();
}


//[-------------------------------------------------------]
//[ Public virtual WidgetImpl functions                   ]
//[-------------------------------------------------------]
void WidgetNull::CreateWidget()
{
	// Not implemented
}

void WidgetNull::CreateWrapperWidget(handle nNativeWindowHandle)
{
	// Not implemented
}

bool WidgetNull::IsDestroyed() const
{
	// Return destroyed-flag - not implemented
	return false;
}

void WidgetNull::Destroy()
{
	// Not implemented
}

handle WidgetNull::GetNativeWindowHandle() const
{
	// Return native window handle - not implemented
	return NULL_HANDLE;
}

void WidgetNull::SetParent(WidgetImpl *pParent)
{
	// Not implemented
}

bool WidgetNull::IsShowInTaskbar() const
{
	// Return taskbar flag - not implemented
	return false;
}

void WidgetNull::SetShowInTaskbar(bool bShowInTaskbar)
{
	// Not implemented
}

void WidgetNull::MoveToScreen(Screen &cScreen)
{
	// Not implemented
}

Vector2i WidgetNull::GetPos() const
{
	// Return position - not implemented
	return Vector2i::Zero;
}

void WidgetNull::SetPos(const Vector2i &vPos)
{
	// Not implemented
}

Vector2i WidgetNull::GetSize() const
{
	// Return size - not implemented
	return Vector2i::Zero;
}

void WidgetNull::SetSize(const Vector2i &vSize)
{
	// Not implemented
}

void WidgetNull::SetZPos(EZPos nZPos, Widget *pWidget)
{
	// Not implemented
}

bool WidgetNull::IsTopmost() const
{
	// Return topmost-state - not implemented
	return false;
}

void WidgetNull::SetTopmost(bool bTopmost)
{
	// Not implemented
}

EWindowState WidgetNull::GetWindowState() const
{
	// Not implemented
	return StateNormal;
}

void WidgetNull::SetWindowState(EWindowState nWindowState)
{
	// Not implemented
}

void WidgetNull::SetTransparency(ETransparency nTransparency, const Color4 &cColor)
{
	// Not implemented
}

bool WidgetNull::IsVisible() const
{
	// Return visible-flag - not implemented
	return false;
}

void WidgetNull::SetVisible(bool bVisible)
{
	// Not implemented
}

bool WidgetNull::IsEnabled() const
{
	// Return enabled-flag - not implemented
	return false;
}

void WidgetNull::SetEnabled(bool bEnabled)
{
	// Not implemented
}

void WidgetNull::Activate()
{
	// Not implemented
}

void WidgetNull::SetFocus()
{
	// Not implemented
}

void WidgetNull::Redraw()
{
	// Not implemented
}

void WidgetNull::SetTrapMouse(bool bTrap)
{
	// Not implemented
}

void WidgetNull::SetCaptureMouse(bool bCapture)
{
	// Not implemented
}

uint32 WidgetNull::RegisterHotkey(uint32 nModKey, uint32 nKey)
{
	// Not implemented
	return 0;
}

void WidgetNull::UnregisterHotkey(uint32 nID)
{
	// Not implemented
}

void WidgetNull::SetTitle(const String &sTitle)
{
	// Not implemented
}

void WidgetNull::SetIcon(const Image &cIcon)
{
	// Not implemented
}

void WidgetNull::SetCursor(const Cursor &cCursor)
{
	// Not implemented
}

bool WidgetNull::GetMousePos(Vector2i &vPos)
{
	// Error - not implemented
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
