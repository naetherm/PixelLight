/*********************************************************\
 *  File: CursorLinux.cpp                                *
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
#include <PLGraphics/Image/Image.h>
#include "PLGui/Gui/Gui.h"
#include "PLGui/Gui/Resources/Cursor.h"
#include "PLGui/Backend/Linux/GuiLinux.h"
#include "PLGui/Backend/Linux/ToolsLinux.h"
#include "PLGui/Backend/Linux/CursorLinux.h"
#include <X11/cursorfont.h>


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
CursorLinux::CursorLinux(Cursor &cCursor) : CursorImpl(cCursor),
	m_pDisplay(static_cast<GuiLinux*>(cCursor.GetGui()->GetImpl())->GetDisplay()),
	m_pXCursor(NULL_HANDLE)
{
}

/**
*  @brief
*    Destructor
*/
CursorLinux::~CursorLinux()
{
	// Release cursor
	if (m_pXCursor) XFreeCursor(m_pDisplay, m_pXCursor);
}

/**
*  @brief
*    Get X cursor
*/
::Cursor CursorLinux::GetXCursor() const
{
	// Return cursor
	return m_pXCursor;
}


//[-------------------------------------------------------]
//[ Public virtual CursorImpl functions                   ]
//[-------------------------------------------------------]
void CursorLinux::Load(EMouseCursor nCursor)
{
	// Release cursor
	if (m_pXCursor) {
		XFreeCursor(m_pDisplay, m_pXCursor);
		m_pXCursor = NULL_HANDLE;
	}

	// Store data
	m_sFilename		= "";
	m_nMouseCursor	= nCursor;

	// Get X font cursor
	int nShape = 0;
	switch (nCursor) {
		// Standard cursor
		case CursorArrow:					nShape = XC_arrow;					break;
		case CursorUpArrow:					nShape = XC_center_ptr;				break;
		case CursorWaitStart:				nShape = XC_watch;					break;
		case CursorWait:					nShape = XC_watch;					break;
		case CursorHelp:					nShape = XC_question_arrow;			break;
		case CursorNo:						nShape = XC_X_cursor;				break;
		case CursorCrosshair:				nShape = XC_crosshair;				break;
		case CursorHand:					nShape = XC_hand1;					break;
		case CursorText:					nShape = XC_xterm;					break;
		case CursorMove:					nShape = XC_fleur;					break;
		case CursorSize:					nShape = XC_sizing;					break;
		case CursorSizeTop:					nShape = XC_sb_up_arrow;			break;
		case CursorSizeBottom:				nShape = XC_sb_down_arrow;			break;
		case CursorSizeLeft:				nShape = XC_sb_left_arrow;			break;
		case CursorSizeRight:				nShape = XC_sb_right_arrow;			break;
		case CursorSizeTopBottom:			nShape = XC_sb_v_double_arrow;		break;
		case CursorSizeLeftRight:			nShape = XC_sb_h_double_arrow;		break;
		case CursorSizeTopLeft:				nShape = XC_top_left_corner;		break;
		case CursorSizeTopRight:			nShape = XC_top_right_corner;		break;
		case CursorSizeBottomLeft:			nShape = XC_bottom_left_corner;		break;
		case CursorSizeBottomRight:			nShape = XC_bottom_right_corner;	break;
		case CursorSizeTopLeftBottomRight:	nShape = XC_diamond_cross;			break;
		case CursorSizeTopRightBottomLeft:	nShape = XC_diamond_cross;			break;
		default:							nShape = XC_arrow;					break;

		// No cursor
		case CursorNone:
			m_pXCursor = ToolsLinux::CreateEmptyCursor(m_pDisplay);
			return;

		case CursorCustom:
		case Cursor_Count:
			// [TODO] Check me: Do we need to handle those?
			return;
	}

	// Load cursor
	m_pXCursor = XCreateFontCursor(m_pDisplay, nShape);
}

void CursorLinux::Load(const String &sFilename, const Vector2i &vHotspot)
{
	// Release cursor
	if (m_pXCursor) {
		XFreeCursor(m_pDisplay, m_pXCursor);
		m_pXCursor = NULL_HANDLE;
	}

	// Store data
	m_sFilename		= sFilename;
	m_nMouseCursor	= CursorCustom;
	if (sFilename.GetLength()) {
		// Load image
		PLGraphics::Image cImage;
		cImage.LoadByFilename(sFilename);

		// Create cursor from image
		ToolsLinux::CreateCursorFromImage(m_pDisplay, cImage, vHotspot, m_pXCursor);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
