/*********************************************************\
 *  File: ImageWindows.cpp                               *
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
#include <PLGraphics/Color/Color3.h>
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include <PLGraphics/Image/+Windows/ImageToolsWindows.h>
#include <PLGraphics/Image/ImageEffects.h>
#include "PLGui/Backend/+Windows/ToolsWindows.h"
#include "PLGui/Backend/+Windows/ImageWindows.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLGraphics;
namespace PLGui {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ImageWindows::ImageWindows(Image &cImage) : ImageImpl(cImage),
	m_hBitmap(nullptr),
	m_hIcon(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
ImageWindows::~ImageWindows()
{
	// Clean up
	Unload();
}

/**
*  @brief
*    Get bitmap handle
*/
HBITMAP ImageWindows::GetBitmapHandle() const
{
	// Return image handle
	return m_hBitmap;
}

/**
*  @brief
*    Get icon handle of the image
*/
HICON ImageWindows::GetIconHandle()
{
	// Has the icon already been created?
	if (m_hBitmap != nullptr && m_hIcon == nullptr) {
		// Create icon
		m_hIcon = ToolsWindows::CreateIconFromBitmap(m_hBitmap);
	}

	// Return icon handle
	return m_hIcon;
}

/**
*  @brief
*    Unload image
*/
void ImageWindows::Unload()
{
	// Delete icon
	if (m_hIcon) {
		DeleteObject(m_hIcon);
		m_hIcon = nullptr;
	}

	// Delete bitmap
	if (m_hBitmap) {
		DeleteObject(m_hBitmap);
		m_hBitmap = nullptr;
	}
}


//[-------------------------------------------------------]
//[ Public virtual ImageImpl functions                    ]
//[-------------------------------------------------------]
bool ImageWindows::Load(const String &sFilename)
{
	// Check parameter
	if (sFilename.GetLength()) {
		// Delete bitmap
		Unload();

		// Load the image
		PLGraphics::Image cImage;
		if (cImage.LoadByFilename(sFilename)) {
			// Get the image buffer
			const PLGraphics::ImageBuffer *pImageBuffer = cImage.GetBuffer();
			if (pImageBuffer) {
				// Convert to Windows bitmap
				m_hBitmap = ImageToolsWindows::ConvertToHBitmap(cImage, GetDC(nullptr));
				if (m_hBitmap) {
					// Save attributes
					m_sFilename = sFilename;

					// Find bitmap dimensions
					BITMAP sBitmap;
					::GetObject(m_hBitmap, sizeof(BITMAP), &sBitmap);
					m_vSize.x = sBitmap.bmWidth;
					m_vSize.y = sBitmap.bmHeight;
					m_nColorDepth = pImageBuffer->GetComponentsPerPixel() * 8;

					// Done
					return true;
				}
			}
		}
	}

	// Error!
	m_sFilename		= "";
	m_vSize.x		= 0;
	m_vSize.y		= 0;
	m_nColorDepth	= 0;
	m_hBitmap		= nullptr;
	return false;
}

bool ImageWindows::LoadWithColorKey(const String &sFilename, const Color3 &cColor)
{
	// Check parameter
	if (sFilename.GetLength()) {
		// Delete bitmap
		Unload();

		// Load the image
		PLGraphics::Image cImage;
		if (cImage.LoadByFilename(sFilename)) {
			// Apply color key
			cImage.ApplyEffect(ImageEffects::ColorKey(cColor, 0.0f));

			// Get the image buffer
			const PLGraphics::ImageBuffer *pImageBuffer = cImage.GetBuffer();
			if (pImageBuffer) {
				// Convert to Windows bitmap
				m_hBitmap = ImageToolsWindows::ConvertToHBitmap(cImage, GetDC(nullptr));
				if (m_hBitmap) {
					// Save attributes
					m_sFilename = sFilename;

					// Find bitmap dimensions
					BITMAP sBitmap;
					::GetObject(m_hBitmap, sizeof(BITMAP), &sBitmap);
					m_vSize.x = sBitmap.bmWidth;
					m_vSize.y = sBitmap.bmHeight;
					m_nColorDepth = pImageBuffer->GetComponentsPerPixel() * 8;

					// Done
					return true;
				}
			}
		}
	}

	// Error!
	m_sFilename		= "";
	m_vSize.x		= 0;
	m_vSize.y		= 0;
	m_nColorDepth	= 0;
	m_hBitmap		= nullptr;
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
