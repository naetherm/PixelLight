/*********************************************************\
 *  File: CursorWindows.cpp                              *
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
#include "PLGui/Gui/Resources/Image.h"
#include "PLGui/Gui/Resources/Cursor.h"
#include "PLGui/Backend/+Windows/ToolsWindows.h"
#include "PLGui/Backend/+Windows/ImageWindows.h"
#include "PLGui/Backend/+Windows/CursorWindows.h"


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
CursorWindows::CursorWindows(Cursor &cCursor) : CursorImpl(cCursor),
	m_hCursor(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
CursorWindows::~CursorWindows()
{
	// Release cursor
	if (m_hCursor) DeleteObject(m_hCursor);
}

/**
*  @brief
*    Returns the cursor handle
*/
HCURSOR CursorWindows::GetHandle() const
{
	// Return cursor handle
	return m_hCursor;
}


//[-------------------------------------------------------]
//[ Public virtual CursorImpl functions                   ]
//[-------------------------------------------------------]
void CursorWindows::Load(EMouseCursor nCursor)
{
	// Store data
	m_sFilename		= "";
	m_nMouseCursor	= nCursor;
	m_hCursor		= nullptr;

	// Get Windows system cursor
	#ifdef _UNICODE
		LPWSTR SysCursor;
	#else
		LPSTR SysCursor;
	#endif
	switch (nCursor) {
		// Standard cursor
		case CursorArrow:					SysCursor = IDC_ARROW;			break;
		case CursorUpArrow:					SysCursor = IDC_UPARROW;		break;
		case CursorWaitStart:				SysCursor = IDC_APPSTARTING;	break;
		case CursorWait:					SysCursor = IDC_WAIT;			break;
		case CursorHelp:					SysCursor = IDC_HELP;			break;
		case CursorNo:						SysCursor = IDC_NO;				break;
		case CursorCrosshair:				SysCursor = IDC_CROSS;			break;
		case CursorHand:					SysCursor = IDC_HAND;			break;
		case CursorText:					SysCursor = IDC_IBEAM;			break;
		case CursorMove:					SysCursor = IDC_SIZEALL;		break;
		case CursorSize:					SysCursor = IDC_SIZEALL;		break;
		case CursorSizeTop:					SysCursor = IDC_SIZENS;			break;
		case CursorSizeBottom:				SysCursor = IDC_SIZENS;			break;
		case CursorSizeLeft:				SysCursor = IDC_SIZEWE;			break;
		case CursorSizeRight:				SysCursor = IDC_SIZEWE;			break;
		case CursorSizeTopBottom:			SysCursor = IDC_SIZENS;			break;
		case CursorSizeLeftRight:			SysCursor = IDC_SIZEWE;			break;
		case CursorSizeTopLeft:				SysCursor = IDC_SIZENWSE;		break;
		case CursorSizeTopRight:			SysCursor = IDC_SIZENESW;		break;
		case CursorSizeBottomLeft:			SysCursor = IDC_SIZENESW;		break;
		case CursorSizeBottomRight:			SysCursor = IDC_SIZENWSE;		break;
		case CursorSizeTopLeftBottomRight:	SysCursor = IDC_SIZENWSE;		break;
		case CursorSizeTopRightBottomLeft:	SysCursor = IDC_SIZENESW;		break;
		default:							SysCursor = IDC_ARROW;			break;

		// No cursor
		case CursorNone:
			return;
	}

	// Load cursor
	m_hCursor = LoadCursor(nullptr, SysCursor);
}

void CursorWindows::Load(const String &sFilename, const Vector2i &vHotspot)
{
	// Store data
	m_sFilename = sFilename;
	m_nMouseCursor = CursorCustom;
	m_hCursor = nullptr;

	// Load image
	if (sFilename.GetLength()) {
		// Load image
		Image cImage(*m_pCursor->GetGui());
		cImage.Load(sFilename);

		// Create cursor from image
		m_hCursor = ToolsWindows::CreateCursorFromBitmap(static_cast<ImageWindows*>(cImage.GetImpl())->GetBitmapHandle(), RGB(255, 255, 255), vHotspot.x, vHotspot.y);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
