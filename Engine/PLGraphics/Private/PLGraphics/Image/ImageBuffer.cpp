/*********************************************************\
 *  File: ImageBuffer.cpp                                *
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
#include "PLGraphics/Image/ImageEffect.h"
#include "PLGraphics/Image/ImagePalette.h"
#include "PLGraphics/Image/ImageBuffer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLGraphics {


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the number of components per pixel
*/
uint32 ImageBuffer::GetComponentsPerPixel(EColorFormat nColorFormat)
{
	// Get number of components depending on color format
	switch (nColorFormat) {
		case ColorRGB:
			return 3;

		case ColorRGBA:
			return 4;

		case ColorBGR:
			return 3;

		case ColorBGRA:
			return 4;

		case ColorGrayscale:
			return 1;

		case ColorGrayscaleA:
			return 2;

		case ColorPalette:
			return 1;

		default:
			return 0;
	}
}

/**
*  @brief
*    Returns the number of bytes per pixel component
*/
uint32 ImageBuffer::GetBytesPerPixelComponent(EDataFormat nDataFormat)
{
	// Get number of bytes depending on data format
	switch (nDataFormat) {
		case DataByte:
			return 1;

		case DataWord:
			return 2;

		case DataHalf:
			return 2;

		case DataFloat:
			return 4;

		case DataDouble:
			return 8;

		default:
			return 0;
	}
}

/**
*  @brief
*    Returns the size of a certain mipmap
*/
uint32 ImageBuffer::GetMipmapSize(uint32 nBaseSize, uint32 nMipmap)
{
	// Get the size of the mipmap level
	uint32 nSize          = nBaseSize;
	uint32 nCurrentMipmap = 0;
	while (nCurrentMipmap < nMipmap && nSize > 1) {
		nSize >>= 1;
		nCurrentMipmap++;
	}

	// Return the size
	return nSize;
}

/**
*  @brief
*    Returns a version with alpha channel of the given color format
*/
EColorFormat ImageBuffer::GetAlphaChannelColorFormat(EColorFormat nColorFormat)
{
	// Convert
	switch (nColorFormat) {
		case ColorRGB:
			return ColorRGBA;

		case ColorBGR:
			return ColorBGRA;

		case ColorGrayscale:
			return ColorGrayscaleA;

		case ColorPalette:
			// Error, this is not supported!
			return ColorPalette;

		case ColorRGBA:
		case ColorBGRA:
		case ColorGrayscaleA:
		default:
			return nColorFormat;
	}
}

/**
*  @brief
*    Returns a version without alpha channel of the given color format
*/
EColorFormat ImageBuffer::GetNoneAlphaChannelColorFormat(EColorFormat nColorFormat)
{
	// Convert
	switch (nColorFormat) {
		case ColorRGBA:
			return ColorRGB;

		case ColorBGRA:
			return ColorBGR;

		case ColorGrayscaleA:
			return ColorGrayscale;

		case ColorPalette:
			// Error, this is not supported!
			return ColorPalette;

		case ColorRGB:
		case ColorBGR:
		case ColorGrayscale:
		default:
			return nColorFormat;
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Apply image effect
*/
void ImageBuffer::ApplyEffect(const ImageEffect &cEffect)
{
	cEffect.Apply(*this);
}

/**
*  @brief
*    Check image for possible problems
*/
ECheckConsistency ImageBuffer::CheckConsistency() const
{
	// Check if image size is valid
	const Vector3i vSize = m_pImageData->GetSize();
	if (vSize.x <= 0 || vSize.y <= 0 || vSize.z <= 0) {
		// Image size is not valid
		return CheckImageBufferInvalidSize;
	}

	// Check if any image data is available (compressed or uncompressed)
	if (!m_pImageData->HasData() && !m_pImageData->HasCompressedData()) {
		// No image data available
		return CheckImageBufferNoData;
	}

	// Check color palette
	if (m_pImageData->GetColorFormat() == ColorPalette) {
		// Check if palette is defined
		const ImagePalette *pPalette = m_pImageData->GetPalette();
		if (!pPalette) {
			// No palette assigned
			return CheckPaletteNotAssigned;
		}

		// Check if palette is empty
		if (pPalette->GetNumOfColors() == 0 || !pPalette->GetData()) {
			// Palette is empty
			return CheckPaletteEmpty;
		}
	}

	// No problem found
	return CheckOk;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Make new empty image data
*/
void ImageBuffer::MakeNewBuffer()
{
	// Create new image data, but do not copy data from old one
	ImageData *pImageData = new ImageData();

	// Release old image data
	m_pImageData->Release();

	// Set pointer to new image data
	m_pImageData = pImageData;
	m_pImageData->AddReference();
}

/**
*  @brief
*    Make image data unique
*/
void ImageBuffer::MakeBufferUnique()
{
	// If the image data is used more than once
	if (m_pImageData->GetRefCount() > 1) {
		// Create new image data and copy data from old one
		ImageData *pImageData = new ImageData(*m_pImageData);

		// Release old image data
		m_pImageData->Release();

		// Set pointer to new image data
		m_pImageData = pImageData;
		m_pImageData->AddReference();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics
