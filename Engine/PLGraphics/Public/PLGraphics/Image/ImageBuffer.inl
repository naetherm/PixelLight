/*********************************************************\
 *  File: ImageBuffer.inl                                *
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
#include "PLGraphics/Image/ImageData.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGraphics {


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the number of bytes per pixel
*/
inline PLCore::uint32 ImageBuffer::GetBytesPerPixel(EDataFormat nDataFormat, EColorFormat nColorFormat)
{
	// Get number of bytes of a whole pixel
	return GetComponentsPerPixel(nColorFormat) * GetBytesPerPixelComponent(nDataFormat);
}

/**
*  @brief
*    Returns whether or not the given color format has an alpha channel
*/
inline bool ImageBuffer::IsAlphaChannelColorFormat(EColorFormat nColorFormat)
{
	return (nColorFormat == ColorRGBA || nColorFormat == ColorBGRA || nColorFormat == ColorGrayscaleA);
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
inline ImageBuffer::ImageBuffer() :
	m_pImageData(new ImageData())
{
	// Hold reference to image buffer
	m_pImageData->AddReference();
}

/**
*  @brief
*    Copy constructor
*/
inline ImageBuffer::ImageBuffer(const ImageBuffer &cSource) :
	m_pImageData(cSource.m_pImageData)
{
	// Reference same image data as source
	m_pImageData->AddReference();
}

/**
*  @brief
*    Destructor
*/
inline ImageBuffer::~ImageBuffer()
{
	// Release image data
	m_pImageData->Release();
}

/**
*  @brief
*    Assignment operator
*/
inline ImageBuffer &ImageBuffer::operator =(const ImageBuffer &cSource)
{
	// Release image data
	m_pImageData->Release();

	// Reference same image data as source
	m_pImageData = cSource.m_pImageData;
	m_pImageData->AddReference();

	// Return this
	return *this;
}

/**
*  @brief
*    Create image
*/
inline void ImageBuffer::CreateImage(EDataFormat nDataFormat, EColorFormat nColorFormat, const PLMath::Vector3i &vSize, ECompression nCompression)
{
	// Image is going to be changed, so make image data unique
	if (m_pImageData->GetRefCount() > 1)
		MakeNewBuffer();	// Do not use "MakeBufferUnique()" in here because we know that we're going to destroy the current image data anyway, no need to copy the image data!

	// Create image
	m_pImageData->CreateImage(nDataFormat, nColorFormat, vSize, nCompression);
}

/**
*  @brief
*    Clear data
*/
inline void ImageBuffer::Clear()
{
	// Image is going to be changed, so make image data unique
	if (m_pImageData->GetRefCount() > 1)
		MakeNewBuffer();		// Do not use "MakeBufferUnique()" in here because we know that we're going to destroy the current image data anyway, no need to copy the image data!
	else
		m_pImageData->Clear();	// Clear image
}

/**
*  @brief
*    Create a test image
*/
inline void ImageBuffer::CreateTestImage(ETestImage nTestImage)
{
	// Image is going to be changed, so make image data unique
	if (m_pImageData->GetRefCount() > 1)
		MakeNewBuffer();	// Do not use "MakeBufferUnique()" in here because we know that we're going to destroy the current image data anyway, no need to copy the image data!

	// Create test image
	m_pImageData->CreateTestImage(nTestImage);
}

/**
*  @brief
*    Get data format
*/
inline EDataFormat ImageBuffer::GetDataFormat() const
{
	// Get data format
	return m_pImageData->GetDataFormat();
}

/**
*  @brief
*    Get color format
*/
inline EColorFormat ImageBuffer::GetColorFormat() const
{
	// Get color format
	return m_pImageData->GetColorFormat();
}

/**
*  @brief
*    Returns the number of components per pixel
*/
inline PLCore::uint32 ImageBuffer::GetComponentsPerPixel() const
{
	// Get from color format
	return GetComponentsPerPixel(m_pImageData->GetColorFormat());
}

/**
*  @brief
*    Returns the number of bytes per pixel component
*/
inline PLCore::uint32 ImageBuffer::GetBytesPerPixelComponent() const
{
	// Get from data format
	return GetBytesPerPixelComponent(m_pImageData->GetDataFormat());
}

/**
*  @brief
*    Returns the number of bytes per pixel
*/
inline PLCore::uint32 ImageBuffer::GetBytesPerPixel() const
{
	// Get from color and data format
	return GetBytesPerPixel(m_pImageData->GetDataFormat(), m_pImageData->GetColorFormat());
}

/**
*  @brief
*    Get compression type
*/
inline ECompression ImageBuffer::GetCompression() const
{
	// Get compression type
	return m_pImageData->GetCompression();
}

/**
*  @brief
*    Set compression type
*/
inline void ImageBuffer::SetCompression(ECompression nCompression)
{
	// Has the compression type actually changed?
	if (m_pImageData->GetCompression() != nCompression) {
		// Image is going to be changed, so make image data unique
		if (m_pImageData->GetRefCount() > 1)
			MakeBufferUnique();

		// Set compression type
		m_pImageData->SetCompression(nCompression);
	}
}

/**
*  @brief
*    Get image size
*/
inline PLMath::Vector3i ImageBuffer::GetSize() const
{
	// Get image size
	return m_pImageData->GetSize();
}

/**
*  @brief
*    Returns the number of pixels
*/
inline PLCore::uint32 ImageBuffer::GetNumOfPixels() const
{
	// Get number of pixels
	return m_pImageData->GetSize().x * m_pImageData->GetSize().y;
}

/**
*  @brief
*    Returns the number of voxels
*/
inline PLCore::uint32 ImageBuffer::GetNumOfVoxels() const
{
	// Get number of voxels
	return m_pImageData->GetSize().x * m_pImageData->GetSize().y * m_pImageData->GetSize().z;
}

/**
*  @brief
*    Check if uncompressed or compressed data is available
*/
inline bool ImageBuffer::HasAnyData() const
{
	// Check if uncompressed or compressed data is available
	return m_pImageData->HasAnyData();
}

/**
*  @brief
*    Check if uncompressed data is available
*/
inline bool ImageBuffer::HasData() const
{
	// Check if uncompressed data is available
	return m_pImageData->HasData();
}

/**
*  @brief
*    Get size of image data in bytes
*/
inline PLCore::uint32 ImageBuffer::GetDataSize() const
{
	// Get uncompressed data size
	return m_pImageData->GetDataSize();
}

/**
*  @brief
*    Get size of one row of image data in bytes
*/
inline PLCore::uint32 ImageBuffer::GetBytesPerRow() const
{
	// Get size of a single row
	return m_pImageData->GetBytesPerRow();
}

/**
*  @brief
*    Get size of one xy-plane (z/depth layer) of image data in bytes
*/
inline PLCore::uint32 ImageBuffer::GetBytesPerPlane() const
{
	// Get size of a single xy-plane
	return m_pImageData->GetBytesPerPlane();
}

/**
*  @brief
*    Get image data
*/
inline const PLCore::uint8 *ImageBuffer::GetData() const
{
	// Get uncompressed data
	return m_pImageData->GetData();
}

inline PLCore::uint8 *ImageBuffer::GetData()
{
	// Image is going to be changed, so make image data unique
	if (m_pImageData->GetRefCount() > 1)
		MakeBufferUnique();

	// Get uncompressed data
	return m_pImageData->GetData();
}

/**
*  @brief
*    Check if compressed data is available
*/
inline bool ImageBuffer::HasCompressedData() const
{
	// Check if compressed data is available
	return m_pImageData->HasCompressedData();
}

/**
*  @brief
*    Get size of compressed image data in bytes
*/
inline PLCore::uint32 ImageBuffer::GetCompressedDataSize() const
{
	// Get compressed data size
	return m_pImageData->GetCompressedDataSize();
}

/**
*  @brief
*    Get compressed image data
*/
inline const PLCore::uint8 *ImageBuffer::GetCompressedData() const
{
	// Get compressed data
	return m_pImageData->GetCompressedData();
}

inline PLCore::uint8 *ImageBuffer::GetCompressedData()
{
	// Image is going to be changed, so make image data unique
	if (m_pImageData->GetRefCount() > 1)
		MakeBufferUnique();

	// Get compressed data
	return m_pImageData->GetCompressedData();
}

/**
*  @brief
*    Compress image data
*/
inline bool ImageBuffer::Compress()
{
	// This does not actually change the image data, so we don't use MakeBufferUnique() here. This also has
	// the benefit of other images being able to access the compressed/uncompressed data in subsequent calls

	// Compress data
	return m_pImageData->Compress();
}

/**
*  @brief
*    Decompress image data
*/
inline bool ImageBuffer::Decompress()
{
	// This does not actually change the image data, so we don't use MakeBufferUnique() here. This also has
	// the benefit of other images being able to access the compressed/uncompressed data in subsequent calls

	// Decompress data
	return m_pImageData->Decompress();
}

/**
*  @brief
*    Set color palette
*/
inline void ImageBuffer::SetPalette(ImagePalette *pPalette)
{
	// Image is going to be changed, so make image data unique
	if (m_pImageData->GetRefCount() > 1)
		MakeBufferUnique();

	// Set palette
	m_pImageData->SetPalette(pPalette);
}

/**
*  @brief
*    Get color palette
*/
inline const ImagePalette *ImageBuffer::GetPalette() const
{
	// Get palette
	return m_pImageData->GetPalette();
}

inline ImagePalette *ImageBuffer::GetPalette()
{
	// Image is going to be changed, so make image data unique
	if (m_pImageData->GetRefCount() > 1)
		MakeBufferUnique();

	// Get palette
	return m_pImageData->GetPalette();
}

/**
*  @brief
*    Copy provided uncompressed image data into this image buffer
*/
inline void ImageBuffer::CopyData(const PLCore::uint8 *pnData)
{
	// Valid image data pointer given?
	if (pnData) {
		// Image is going to be changed, so make image data unique
		if (m_pImageData->GetRefCount() > 1)
			MakeBufferUnique();

		// Copy data
		return m_pImageData->CopyData(pnData);
	}
}

/**
*  @brief
*    Let this image buffer takeover provided uncompressed image data
*/
inline void ImageBuffer::TakeoverData(PLCore::uint8 *pnData)
{
	// Valid image data pointer given?
	if (pnData) {
		// Image is going to be changed, so make image data unique
		if (m_pImageData->GetRefCount() > 1)
			MakeBufferUnique();

		// Takeover data
		return m_pImageData->TakeoverData(pnData);
	}
}

/**
*  @brief
*    Let this image buffer share provided uncompressed image data
*/
inline void ImageBuffer::ShareData(PLCore::uint8 *pnData)
{
	// Valid image data pointer given?
	if (pnData) {
		// Image is going to be changed, so make image data unique
		if (m_pImageData->GetRefCount() > 1)
			MakeBufferUnique();

		// Share data
		return m_pImageData->ShareData(pnData);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics
