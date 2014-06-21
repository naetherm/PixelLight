/*********************************************************\
 *  File: ImageWindows.h                                 *
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


#ifndef __PLGUI_IMAGEWINDOWS_H__
#define __PLGUI_IMAGEWINDOWS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/+Windows/PLCoreWindowsIncludes.h>
#include "PLGui/Backend/ImageImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Windows image implementation
*/
class ImageWindows : public ImageImpl {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cImage
		*    Reference to platform independent image
		*/
		PLGUI_API ImageWindows(Image &cImage);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~ImageWindows();

		/**
		*  @brief
		*    Get bitmap handle
		*
		*  @return
		*    Bitmap handle, can be a null pointer
		*/
		PLGUI_API HBITMAP GetBitmapHandle() const;

		/**
		*  @brief
		*    Get icon handle of the image
		*
		*  @return
		*    Icon handle, can be a null pointer
		*/
		PLGUI_API HICON GetIconHandle();

		/**
		*  @brief
		*    Unload image
		*/
		PLGUI_API void Unload();


	//[-------------------------------------------------------]
	//[ Public virtual ImageImpl functions                    ]
	//[-------------------------------------------------------]
	public:
		virtual bool Load(const PLCore::String &sFilename) override;
		virtual bool LoadWithColorKey(const PLCore::String &sFilename, const PLGraphics::Color3 &cColor) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		HBITMAP m_hBitmap;	/**< Bitmap handle, can be a null pointer */
		HICON	m_hIcon;	/**< Icon handle of bitmap */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_IMAGEWINDOWS_H__
