/*********************************************************\
 *  File: ToolsLinux.h                                   *
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


#ifndef __PLGUI_TOOLSLINUX_H__
#define __PLGUI_TOOLSLINUX_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGraphics/Color/Color4.h>
#include "PLGui/PLGui.h"
#include "PLGui/PLGuiLinuxIncludes.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLMath {
	class Vector2i;
}
namespace PLGraphics {
	class Image;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Tool functions for the Linux GUI implementation
*/
class ToolsLinux {


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Convert color to X color
		*
		*  @param[in] cColor
		*    Color
		*  @param[in] nColorDepth
		*    Color depth (currently supported: 16, 24)
		*
		*  @return
		*    Color value
		*/
		static PLGUI_API long GetXColor(const PLGraphics::Color3 &cColor, int nColorDepth);

		/**
		*  @brief
		*    Set hints for not applying a window decoration for a window
		*
		*  @param[in] pDisplay
		*    Display
		*  @param[in] nNativeWindowHandle
		*    System native window handle
		*  @param[in] bDecorate
		*    'true' to apply default window decoration, else 'false'
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		static PLGUI_API bool SetNoWindowDecoration(Display *pDisplay, ::Window nNativeWindowHandle, bool bDecorate);

		/**
		*  @brief
		*    Create X pixmap from image
		*
		*  @param[in]  pDisplay
		*    Display to use, must be valid
		*  @param[in]  cImage
		*    Image to use
		*  @param[in]  cColorKey
		*    Color key to use (pass Color4::Transparent to use no color key)
		*  @param[out] pPixmap
		*    Will receive the pixmap
		*  @param[out] pMaskPixmap
		*    Will receive the mask pixmap
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - Creates a X pixmap of the image
		*    - Use XFreePixmap() to free the pixmap
		*/
		static PLGUI_API bool CreatePixmapFromImage(Display *pDisplay, PLGraphics::Image &cImage, Pixmap &pPixmap, Pixmap &pMaskPixmap, const PLGraphics::Color4 &cColorKey = PLGraphics::Color4::Transparent);

		/**
		*  @brief
		*    Create X cursor from image
		*
		*  @param[in]  pDisplay
		*    Display to use, must be valid
		*  @param[in]  cImage
		*    Image to use
		*  @param[in]  vHotspot
		*    Position of hotspot
		*  @param[out] pCursor
		*    Will receive the cursor
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - Creates a X cursor of the image
		*    - Use XFreeCursor() to free the cursor
		*/
		static PLGUI_API bool CreateCursorFromImage(Display *pDisplay, PLGraphics::Image &cImage, const PLMath::Vector2i &vHotspot, ::Cursor &pCursor);

		/**
		*  @brief
		*    Create empty X cursor
		*
		*  @param[in]  pDisplay
		*    Display to use, must be valid
		*
		*  @return
		*    Cursor
		*/
		static PLGUI_API ::Cursor CreateEmptyCursor(Display *pDisplay);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_TOOLSLINUX_H__
