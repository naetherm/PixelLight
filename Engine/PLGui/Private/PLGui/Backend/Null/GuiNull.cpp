/*********************************************************\
 *  File: GuiNull.cpp                                    *
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
#include "PLGui/Backend/Null/GraphicsNull.h"
#include "PLGui/Backend/Null/ImageNull.h"
#include "PLGui/Backend/Null/FontNull.h"
#include "PLGui/Backend/Null/CursorNull.h"
#include "PLGui/Backend/Null/TrayIconNull.h"
#include "PLGui/Backend/Null/ClipBoardNull.h"
#include "PLGui/Backend/Null/GuiNull.h"


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
GuiNull::GuiNull(Gui *pGui) : GuiImpl(pGui)
{
}

/**
*  @brief
*    Destructor
*/
GuiNull::~GuiNull()
{
}


//[-------------------------------------------------------]
//[ Public virtual GuiImpl functions                      ]
//[-------------------------------------------------------]
bool GuiNull::HasPendingMessages()
{
	// Not implemented
	return false;
}

void GuiNull::ProcessMessage()
{
	// Not implemented
}

void GuiNull::PostMessage(const GuiMessage &cMessage)
{
	// Not implemented
}

void GuiNull::EnumerateScreens(List<Screen*> &lstScreens)
{
	// Not implemented
}

Vector2i GuiNull::GetScreenSize() const
{
	// Not implemented
	return Vector2i::Zero;
}

bool GuiNull::HasTaskbar()
{
	// Not implemented
	return false;
}

void GuiNull::SetMouseVisible(bool bVisible)
{
	// Not implemented
}

void GuiNull::ListFonts(List<FontInfo> &lstFonts) const
{
	// Not implemented
}

WidgetImpl *GuiNull::CreateWidgetImpl(Widget &cWidget) const
{
	// Create a Null widget
	return new WidgetNull(cWidget);
}

GraphicsImpl *GuiNull::CreateGraphicsImpl(Graphics &cGraphics) const
{
	// Create a Null graphics object
	return new GraphicsNull(cGraphics);
}

ImageImpl *GuiNull::CreateImageImpl(Image &cImage) const
{
	// Create a Null image object
	return new ImageNull(cImage);
}

FontImpl *GuiNull::CreateFontImpl(Font &cFont) const
{
	// Create a Null font object
	return new FontNull(cFont);
}

CursorImpl *GuiNull::CreateCursorImpl(Cursor &cCursor) const
{
	// Create a Null cursor object
	return new CursorNull(cCursor);
}

TrayIconImpl *GuiNull::CreateTrayIconImpl(TrayIcon &cTrayIcon) const
{
	// Create a Null tray-icon object
	return new TrayIconNull(cTrayIcon);
}

ClipBoardImpl *GuiNull::CreateClipBoardImpl(ClipBoard &cClipBoard) const
{
	// Create a Null clipboard object
	return new ClipBoardNull(cClipBoard);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
