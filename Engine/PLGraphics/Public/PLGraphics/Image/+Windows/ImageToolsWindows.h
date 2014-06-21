/*********************************************************\
 *  File: ImageToolsWindows.h                            *
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


#ifndef __PLGRAPHICS_IMAGETOOLS_WINDOWS_H__
#define __PLGRAPHICS_IMAGETOOLS_WINDOWS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/+Windows/PLCoreWindowsIncludes.h>
#include "PLGraphics/PLGraphics.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGraphics {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Image;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Static class with some useful Windows image tools
*/
class ImageToolsWindows {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns a Windows-friendly bitmap handle
		*
		*  @param[in] cImage
		*    Image to use
		*  @param[in] hDC
		*    The device context that you want to receive the HBITMAP
		*
		*  @return
		*    Windows-friendly bitmap handle
		*
		*  @note
		*    - Creates a Windows bitmap handle (HBITMAP) copy of the current image, for
		*      direct use in Windows
		*    - Use the windows function DeleteObject() to free the bitmap
		*/
		static PLGRAPHICS_API HBITMAP ConvertToHBitmap(Image &cImage, HDC hDC);

		/**
		*  @brief
		*    Sets the current image to be a copy of a Windows bitmap
		*
		*  @param[in] cImage
		*    Image to use
		*  @param[in] hBitmap
		*    Bitmap to copy to the given image
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @remarks
		*    Copies hBitmap to the given image in a 'normal' format we can
		*    understand. The image can then be used just as if you had loaded an image
		*    via Image::Load(). This function is the opposite of ConvertToHBitmap().
		*/
		static PLGRAPHICS_API bool SetHBitmap(Image &cImage, HBITMAP hBitmap);

		/**
		*  @brief
		*    Copies the Windows clipboard to the current image
		*
		*  @param[in] cImage
		*    Image to use
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @remarks
		*    Copies the contents of the Windows clipboard into the given image,
		*    resizing as necessary.
		*/
		static PLGRAPHICS_API bool GetWinClipboard(Image &cImage);

		/**
		*  @brief
		*    Copies an image to the Windows clipboard
		*
		*  @param[in] cImage
		*    Image to use
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @return
		*    Copies the given image to the Windows clipboard
		*/
		static PLGRAPHICS_API bool SetWinClipboard(Image &cImage);

		/**
		*  @brief
		*    Loads a Windows resource as the current image
		*
		*  @param[in] cImage
		*    Image to use
		*  @param[in] hInst
		*    The application's HINSTANCE
		*  @param[in] nID
		*    The resource identifier of the resource to be loaded
		*  @param[in] sResourceType
		*    The type of user-defined resource (name used when creating)
		*  @param[in] sFileType
		*    The type of image to be loaded, e.g. "bmp". Pass empty string
		*    to let the type be determined automatically
		*
		*  @return
		*   'true' if all went fine, else 'false'
		*
		*  @remarks
		*    Windows-specific function that loads a resource as the given image.
		*    This feature allows you to have images directly in your .exe and not worry
		*    whether a particular file is present on the user's hard drive.
		*/
		static PLGRAPHICS_API bool LoadResource(Image &cImage, HINSTANCE hInst, int nID, const PLCore::String &sResourceType, const PLCore::String &sFileType);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Convert a 32 bit image with alpha map to a window HBITMAP
		*
		*  @param[in] cImage
		*    Image to use
		*  @param[in] hDC
		*    The device context that you want to receive the HBITMAP
		*
		*  @return
		*    Windows-friendly bitmap handle
		*
		*  @note
		*    - This function is used to convert 32 bit images with RGBA data, because
		*      the devil function strips the A information from the data
		*/
		static HBITMAP PLConvertToHBitmap(Image &cImage, HDC hDC);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics


#endif // __PLGRAPHICS_IMAGETOOLS_WINDOWS_H__
