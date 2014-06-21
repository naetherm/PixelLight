/*********************************************************\
 *  File: ToolsWindows.h                                 *
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


#ifndef __PLGUI_TOOLSWINDOWS_H__
#define __PLGUI_TOOLSWINDOWS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/+Windows/PLCoreWindowsIncludes.h>
#include "PLGui/PLGui.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Tool functions for the Windows GUI implementation
*/
class ToolsWindows {


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Create AND and XOR masks from a HBITMAP
		*
		*  @param[in]  hBitmap
		*    Source bitmap
		*  @param[in]  sColorTransparent
		*    Transparent color
		*  @param[out] hMaskAnd
		*    And bitmap mask
		*  @param[out] hMaskXor
		*    Xor bitmap mask
		*
		*  @note
		*    - Pass bitmaps having standard cursor sizes like 16*16, 32*32...
		*/
		static PLGUI_API void GetMaskBitmaps(HBITMAP hBitmap, COLORREF sColorTransparent, HBITMAP &hMaskAnd, HBITMAP &hMaskXor);

		/**
		*  @brief
		*    Create an icon from a HBITMAP
		*
		*  @param[in] hBitmap
		*    Source bitmap
		*
		*  @return
		*    Icon handle
		*/
		static PLGUI_API HICON CreateIconFromBitmap(HBITMAP hBitmap);

		/**
		*  @brief
		*    Create a cursor from a HBITMAP
		*
		*  @param[in] hBitmap
		*    Source bitmap
		*  @param[in] sColorTransparent
		*    Transparent color
		*  @param[in] nHotspotX
		*    Hot spot x position
		*  @param[in] nHotspotY
		*    Hot spot y position
		*
		*  @return
		*    The converted HBITMAP
		*/
		static PLGUI_API HCURSOR CreateCursorFromBitmap(HBITMAP hBitmap, COLORREF sColorTransparent, DWORD nHotspotX, DWORD nHotspotY);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_TOOLSWINDOWS_H__
