/*********************************************************\
 *  File: ImageData.inl                                  *
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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGraphics {


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
inline ImageData::ImageData() :
	m_nDataFormat(DataByte),
	m_nColorFormat(ColorRGB),
	m_nCompression(CompressionNone),
	m_pData(nullptr),
	m_nDataSize(0),
	m_bDataShared(false),
	m_pCompressedData(nullptr),
	m_nCompressedSize(0),
	m_pPalette(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
inline ImageData::~ImageData()
{
	// Clear data
	Clear();
}

/**
*  @brief
*    Get data format
*/
inline EDataFormat ImageData::GetDataFormat() const
{
	// Return format
	return m_nDataFormat;
}

/**
*  @brief
*    Get color format
*/
inline EColorFormat ImageData::GetColorFormat() const
{
	// Return color format
	return m_nColorFormat;
}

/**
*  @brief
*    Get compression type
*/
inline ECompression ImageData::GetCompression() const
{
	// Return compression type
	return m_nCompression;
}

/**
*  @brief
*    Get image size
*/
inline PLMath::Vector3i ImageData::GetSize() const
{
	// Return size
	return m_vSize;
}

/**
*  @brief
*    Check if uncompressed or compressed data is available
*/
inline bool ImageData::HasAnyData() const
{
	// Check if image buffer is valid
	return (m_pData != nullptr || m_pCompressedData != nullptr);
}

/**
*  @brief
*    Check if uncompressed data is available
*/
inline bool ImageData::HasData() const
{
	// Check if image buffer is valid
	return (m_pData != nullptr);
}

/**
*  @brief
*    Get size of image data
*/
inline PLCore::uint32 ImageData::GetDataSize() const
{
	// Return size of image data
	return m_nDataSize;
}

/**
*  @brief
*    Get size of one row of image data in bytes
*/
inline PLCore::uint32 ImageData::GetBytesPerRow() const
{
	// Get size of a single row
	return m_vSize.y ? (GetBytesPerPlane() / m_vSize.y) : 0;
}

/**
*  @brief
*    Get size of one xy-plane (z/depth layer) of image data in bytes
*/
inline PLCore::uint32 ImageData::GetBytesPerPlane() const
{
	// Get size of a single xy-plane
	return m_vSize.z ? (m_nDataSize / m_vSize.z) : 0;
}

/**
*  @brief
*    Check if compressed data is available
*/
inline bool ImageData::HasCompressedData() const
{
	// Check if compressed image buffer is valid
	return (m_pCompressedData != nullptr);
}

/**
*  @brief
*    Get size of compressed image data
*/
inline PLCore::uint32 ImageData::GetCompressedDataSize() const
{
	// Return size of compressed image data
	return m_nCompressedSize;
}

/**
*  @brief
*    Get color palette
*/
inline const ImagePalette *ImageData::GetPalette() const
{
	// Return palette
	return m_pPalette;
}

/**
*  @brief
*    Get color palette
*/
inline ImagePalette *ImageData::GetPalette()
{
	// Return palette
	return m_pPalette;
}

/**
*  @brief
*    Create image buffer
*/
inline void ImageData::CreateBuffer()
{
	// Check if size is valid and buffer has not been created before
	if (m_nDataSize > 0 && !m_pData) {
		// Create image buffer
		m_pData = new PLCore::uint8[m_nDataSize];
	}
}


/**
*  @brief
*    Create compressed image buffer
*/
inline void ImageData::CreateCompressedBuffer()
{
	// Check if size is valid and buffer has not been created before
	if (m_nCompressedSize > 0 && !m_pCompressedData) {
		// Create compressed image buffer
		m_pCompressedData = new PLCore::uint8[m_nCompressedSize];
	}
}

/**
*  @brief
*    Destroy image buffer
*/
inline void ImageData::DestroyBuffer()
{
	// Destroy image data
	if (m_pData) {
		// Do we own the image data?
		if (m_bDataShared)
			m_bDataShared = false;
		else
			delete [] m_pData;

		// Reset data pointer
		m_pData = nullptr;
	}
}

/**
*  @brief
*    Destroy compressed image buffer
*/
inline void ImageData::DestroyCompressedBuffer()
{
	// Destroy compressed image data
	if (m_pCompressedData) {
		delete [] m_pCompressedData;
		m_pCompressedData = nullptr;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics
