/*********************************************************\
 *  File: ImageData.cpp                                  *
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
#include <PLMath/Math.h>
#include "PLGraphics/Image/ImagePalette.h"
#include "PLGraphics/Image/ImageBuffer.h"
#include "PLGraphics/Image/ImageData.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLGraphics {


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
ImageData::ImageData(const ImageData &cSource) :
	m_nDataFormat(cSource.m_nDataFormat),
	m_nColorFormat(cSource.m_nColorFormat),
	m_nCompression(cSource.m_nCompression),
	m_vSize(cSource.m_vSize),
	m_pData(nullptr),
	m_nDataSize(cSource.m_nDataSize),
	m_bDataShared(false),
	m_pCompressedData(nullptr),
	m_nCompressedSize(cSource.m_nCompressedSize),
	m_pPalette(nullptr)
{
	// Copy image data
	if (cSource.m_pData && m_nDataSize) {
		m_pData = new uint8[m_nDataSize];
		MemoryManager::Copy(m_pData, cSource.m_pData, m_nDataSize);
	}

	// Copy compressed image data
	if (cSource.m_pCompressedData && m_nCompressedSize) {
		m_pCompressedData = new uint8[m_nCompressedSize];
		MemoryManager::Copy(m_pCompressedData, cSource.m_pCompressedData, m_nCompressedSize);
	}

	// Copy palette
	if (cSource.m_pPalette)
		m_pPalette = new ImagePalette(*cSource.m_pPalette);
}

/**
*  @brief
*    Assignment operator
*/
ImageData &ImageData::operator =(const ImageData &cSource)
{
	// Clear data
	Clear();

	// Copy image data
	m_nDataFormat		= cSource.m_nDataFormat;
	m_nColorFormat		= cSource.m_nColorFormat;
	m_nCompression		= cSource.m_nCompression;
	m_vSize				= cSource.m_vSize;
	m_pData				= nullptr;
	m_nDataSize			= cSource.m_nDataSize;
	m_bDataShared		= false;
	m_pCompressedData	= nullptr;
	m_nCompressedSize	= cSource.m_nCompressedSize;
	m_pPalette			= nullptr;

	// Copy image data
	if (cSource.m_pData && m_nDataSize) {
		m_pData = new uint8[m_nDataSize];
		MemoryManager::Copy(m_pData, cSource.m_pData, m_nDataSize);
	}

	// Copy compressed image data
	if (cSource.m_pCompressedData && m_nCompressedSize) {
		m_pCompressedData = new uint8[m_nCompressedSize];
		MemoryManager::Copy(m_pCompressedData, cSource.m_pCompressedData, m_nCompressedSize);
	}

	// Copy palette
	if (cSource.m_pPalette)
		m_pPalette = new ImagePalette(*cSource.m_pPalette);

	// Return this
	return *this;
}

/**
*  @brief
*    Create image
*/
void ImageData::CreateImage(EDataFormat nDataFormat, EColorFormat nColorFormat, const PLMath::Vector3i &vSize, ECompression nCompression)
{
	// Clear old data
	Clear();

	// Set compression type
	m_nCompression = nCompression;

	// Check size
	if (vSize.x > 0 && vSize.y > 0 && vSize.z > 0) {
		// Calculate size of image buffer
		const uint32 nSize			= ImageBuffer::GetBytesPerPixelComponent(nDataFormat);
		const uint32 nComponents	= ImageBuffer::GetComponentsPerPixel(nColorFormat);
		m_nDataSize					= vSize.x * vSize.y * vSize.z * nComponents * nSize;

		// Set image data
		m_nDataFormat	= nDataFormat;
		m_nColorFormat	= nColorFormat;
		m_nCompression	= nCompression;
		m_vSize			= vSize;

		// Calculate the size of the compressed image buffer
		CalculateCompressedImageBufferSize();
	} else {
		// Error: Invalid size
	}
}

/**
*  @brief
*    Clear data
*/
void ImageData::Clear()
{
	// Destroy image data
	DestroyBuffer();
	m_nDataSize = 0;

	// Destroy compressed image data
	DestroyCompressedBuffer();
	m_nCompressedSize = 0;

	// Destroy palette
	SetPalette(nullptr);

	// Reset image data
	m_nDataFormat	= DataByte;
	m_nColorFormat	= ColorRGB;
	m_nCompression	= CompressionNone;
	m_vSize			= Vector3i::Zero;
}

/**
*  @brief
*    Create a test image
*/
void ImageData::CreateTestImage(ETestImage nTestImage)
{
	// Get test image ID
	switch (nTestImage) {
		// Simple blue test image (256x256, RGB)
		case TestImage2DSimple:
		{
			// Create 256x256 RGB image
			CreateImage(DataByte, ColorRGB, Vector3i(256, 256, 1));
			CreateBuffer();

			// Write data
			for (int y=0; y<256; y++) {
				for (int x=0; x<256; x++) {
					m_pData[y*256*4+x*3+0] = 0;
					m_pData[y*256*4+x*3+1] = 0;
					m_pData[y*256*4+x*3+2] = 255;
				}
			}
			break;
		}

		// Gradient colored test image (256x256, RGB)
		case TestImage2DGradient:
		{
			// Create 256x256 RGB image
			CreateImage(DataByte, ColorRGB, Vector3i(256, 256, 1));
			CreateBuffer();

			// Write data
			for (int y=0; y<256; y++) {
				for (int x=0; x<256; x++) {
					m_pData[y*256*4+x*3+0] = static_cast<uint8>(x);
					m_pData[y*256*4+x*3+1] = 0;
					m_pData[y*256*4+x*3+2] = static_cast<uint8>(y);
				}
			}
			break;
		}

		// Two-colored checkers test image (256x256, RGB)
		case TestImage2DCheckers:
		{
			// Create 256x256 RGB image
			CreateImage(DataByte, ColorRGB, Vector3i(256, 256, 1));
			CreateBuffer();

			// Write data
			for (int y=0; y<256; y++) {
				for (int x=0; x<256; x++) {
					const bool bYellow = ((y/32)%2 == 0) ? !((x/32)%2 == 0) : ((x/32)%2 == 0);
					m_pData[y*256*4+x*3+0] = bYellow ? 255 : 0;
					m_pData[y*256*4+x*3+1] = bYellow ? 255 : 0;
					m_pData[y*256*4+x*3+2] = 0;
				}
			}
			break;
		}

		case TestImageCubeMap:
			// [TODO] Create a test image
			break;

		default:
			// Nothing to do in here
			break;
	}
}

/**
*  @brief
*    Set compression type
*/
void ImageData::SetCompression(ECompression nCompression)
{
	// Compression type changed?
	if (m_nCompression != nCompression) {
		// Safe image data if necessary
		if (m_pCompressedData && !m_pData) {
			// We only have compressed data, so decompress it in order to save it
			Decompress();
		}

		// Destroy compressed image data
		DestroyCompressedBuffer();

		// Set new compression type
		m_nCompression = nCompression;

		// Calculate the size of the compressed image buffer
		CalculateCompressedImageBufferSize();
	}
}

/**
*  @brief
*    Get image data
*/
const uint8 *ImageData::GetData() const
{
	// Make sure that buffer is created
	if (!m_pData) {
		// Create buffer
		const_cast<ImageData*>(this)->CreateBuffer();

		// If we already have compressed image data, decompress it
		if (m_pCompressedData)
			const_cast<ImageData*>(this)->Decompress();
	}

	// Return image data
	return m_pData;
}

uint8 *ImageData::GetData()
{
	// Make sure that buffer is created
	if (!m_pData) {
		// Create buffer
		CreateBuffer();

		// If we already have compressed image data, decompress it
		if (m_pCompressedData)
			Decompress();
	}

	// Invalidate compressed image buffer, because uncompressed data is going to be modified
	DestroyCompressedBuffer();

	// Return image data
	return m_pData;
}

/**
*  @brief
*    Get compressed image data
*/
const uint8 *ImageData::GetCompressedData() const
{
	// Make sure that compressed buffer is created
	if (!m_pCompressedData) {
		// Create compressed buffer
		const_cast<ImageData*>(this)->CreateCompressedBuffer();

		// If we already have uncompressed image data, compress it
		if (m_pData)
			const_cast<ImageData*>(this)->Compress();
	}

	// Return compressed image data
	return m_pCompressedData;
}

uint8 *ImageData::GetCompressedData()
{
	// Make sure that compressed buffer is created
	if (!m_pCompressedData) {
		// Create compressed buffer
		CreateCompressedBuffer();

		// If we already have uncompressed image data, compress it
		if (m_pData)
			Compress();
	}

	// Invalidate uncompressed image buffer, because compressed data is going to be modified
	DestroyBuffer();

	// Return compressed image data
	return m_pCompressedData;
}

/**
*  @brief
*    Compress image data
*/
bool ImageData::Compress()
{
	// Make sure that both buffers are created
	CreateBuffer();
	CreateCompressedBuffer();

	// [TODO] Implement
	return false;
}

/**
*  @brief
*    Decompress image data
*/
bool ImageData::Decompress()
{
	// Make sure that both buffers are created
	CreateBuffer();
	CreateCompressedBuffer();

	// Get the number of components per pixel
	const uint32 nComponentsPerPixel = ImageBuffer::GetComponentsPerPixel(m_nColorFormat);

	// Get the block size which is not allowed to be above 4 pixel
	const int nBlockWidth  = (m_vSize.x < 4) ? m_vSize.x : 4;
	const int nBlockHeight = (m_vSize.y < 4) ? m_vSize.y : 4;

	// Decompress
	const uint8 *pCompressedData = m_pCompressedData;
	switch (m_nCompression) {
		case CompressionDXT1:
		{
			// Loop trough all block rows
			const uint32 nBytesPerPlane = GetBytesPerPlane();
			uint8 *pCurrentData = m_pData;
			for (int z=0; z<m_vSize.z; z++) {
				// Loop trough all block rows
				for (int y=0; y<m_vSize.y; y+=4) {
					// Loop trough all blocks within the current row
					for (int x=0; x<m_vSize.x; x+=4) {
						// Get the current destination block start
						uint8 *pnCurrentDestination = pCurrentData + (y*m_vSize.x + x)*nComponentsPerPixel;

						// Get the current block size... Look out! Reduce the block size so we don't leave the area of the destination image!!
						const int nCurrentBlockWidth  = nBlockWidth  - Math::Max(0, (x+nBlockWidth)  - m_vSize.x);
						const int nCurrentBlockHeight = nBlockHeight - Math::Max(0, (y+nBlockHeight) - m_vSize.y);

						// Decode the current DXT color block
						DecodeDXTColorBlock(pnCurrentDestination, pCompressedData, nCurrentBlockWidth, nCurrentBlockHeight, nComponentsPerPixel, m_vSize.x*nComponentsPerPixel, m_nCompression, 0, 2);
						pCompressedData += 8;
					}
				}

				// Update current data pointer (xy-plane (z/depth layer) are independent when it comes to compressed blocks)
				pCurrentData += nBytesPerPlane;
			}
			break;
		}

		case CompressionDXT3:
		{
			// Loop trough all block rows
			const uint32 nBytesPerPlane = GetBytesPerPlane();
			uint8 *pCurrentData = m_pData;
			for (int z=0; z<m_vSize.z; z++) {
				// Loop trough all block rows
				for (int y=0; y<m_vSize.y; y+=4) {
					// Loop trough all blocks within the current row
					for (int x=0; x<m_vSize.x; x+=4) {
						// Get the current destination block start
						uint8 *pnCurrentDestination = pCurrentData + (y*m_vSize.x + x)*nComponentsPerPixel;

						// Get the current block size... Look out! Reduce the block size so we don't leave the area of the destination image!!
						const int nCurrentBlockWidth  = nBlockWidth  - Math::Max(0, (x+nBlockWidth)  - m_vSize.x);
						const int nCurrentBlockHeight = nBlockHeight - Math::Max(0, (y+nBlockHeight) - m_vSize.y);

						// Decode the current DXT3 alpha block
						DecodeDXT3AlphaBlock(pnCurrentDestination + 3, pCompressedData, nCurrentBlockWidth, nCurrentBlockHeight, nComponentsPerPixel, m_vSize.x*nComponentsPerPixel);
						pCompressedData += 8;

						// Decode the current DXT color block
						DecodeDXTColorBlock(pnCurrentDestination, pCompressedData, nCurrentBlockWidth, nCurrentBlockHeight, nComponentsPerPixel, m_vSize.x*nComponentsPerPixel, m_nCompression, 0, 2);
						pCompressedData += 8;
					}
				}

				// Update current data pointer (xy-plane (z/depth layer) are independent when it comes to compressed blocks)
				pCurrentData += nBytesPerPlane;
			}
			break;
		}

		case CompressionDXT5:
		{
			// Loop trough all block rows
			const uint32 nBytesPerPlane = GetBytesPerPlane();
			uint8 *pCurrentData = m_pData;
			for (int z=0; z<m_vSize.z; z++) {
				// Loop trough all block rows
				for (int y=0; y<m_vSize.y; y+=4) {
					// Loop trough all blocks within the current row
					for (int x=0; x<m_vSize.x; x+=4) {
						// Get the current destination block start
						uint8 *pnCurrentDestination = pCurrentData + (y*m_vSize.x + x)*nComponentsPerPixel;

						// Get the current block size... Look out! Reduce the block size so we don't leave the area of the destination image!!
						const int nCurrentBlockWidth  = nBlockWidth  - Math::Max(0, (x+nBlockWidth)  - m_vSize.x);
						const int nCurrentBlockHeight = nBlockHeight - Math::Max(0, (y+nBlockHeight) - m_vSize.y);

						// Decode the current DXT5 alpha block
						DecodeDXT5AlphaBlock(pnCurrentDestination + 3, pCompressedData, nCurrentBlockWidth, nCurrentBlockHeight, nComponentsPerPixel, m_vSize.x*nComponentsPerPixel);
						pCompressedData += 8;

						// Decode the current DXT color block
						DecodeDXTColorBlock(pnCurrentDestination, pCompressedData, nCurrentBlockWidth, nCurrentBlockHeight, nComponentsPerPixel, m_vSize.x*nComponentsPerPixel, m_nCompression, 0, 2);
						pCompressedData += 8;
					}
				}

				// Update current data pointer (xy-plane (z/depth layer) are independent when it comes to compressed blocks)
				pCurrentData += nBytesPerPlane;
			}
			break;
		}

		case CompressionLATC1:
		{
			// Loop trough all block rows
			const uint32 nBytesPerPlane = GetBytesPerPlane();
			uint8 *pCurrentData = m_pData;
			for (int z=0; z<m_vSize.z; z++) {
				// Loop trough all block rows
				for (int y=0; y<m_vSize.y; y+=4) {
					// Loop trough all blocks within the current row
					for (int x=0; x<m_vSize.x; x+=4) {
						// Get the current destination block start
						uint8 *pnCurrentDestination = pCurrentData + (y*m_vSize.x + x)*nComponentsPerPixel;

						// Get the current block size... Look out! Reduce the block size so we don't leave the area of the destination image!!
						const int nCurrentBlockWidth  = nBlockWidth  - Math::Max(0, (x+nBlockWidth)  - m_vSize.x);
						const int nCurrentBlockHeight = nBlockHeight - Math::Max(0, (y+nBlockHeight) - m_vSize.y);

						// Decode the current DXT5 alpha block
						DecodeDXT5AlphaBlock(pnCurrentDestination, pCompressedData, nCurrentBlockWidth, nCurrentBlockHeight, 1, m_vSize.x);
						pCompressedData += 8;
					}
				}

				// Update current data pointer (xy-plane (z/depth layer) are independent when it comes to compressed blocks)
				pCurrentData += nBytesPerPlane;
			}
			break;
		}

		case CompressionLATC2:
		{
			// Loop trough all block rows
			const uint32 nBytesPerPlane = GetBytesPerPlane();
			uint8 *pCurrentData = m_pData;
			for (int z=0; z<m_vSize.z; z++) {
				// Loop trough all block rows
				for (int y=0; y<m_vSize.y; y+=4) {
					// Loop trough all blocks within the current row
					for (int x=0; x<m_vSize.x; x+=4) {
						// Get the current destination block start
						uint8 *pnCurrentDestination = pCurrentData + (y*m_vSize.x + x)*nComponentsPerPixel;

						// Get the current block size... Look out! Reduce the block size so we don't leave the area of the destination image!!
						const int nCurrentBlockWidth  = nBlockWidth  - Math::Max(0, (x+nBlockWidth)  - m_vSize.x);
						const int nCurrentBlockHeight = nBlockHeight - Math::Max(0, (y+nBlockHeight) - m_vSize.y);

						// Decode the current DXT5 alpha block
						DecodeDXT5AlphaBlock(pnCurrentDestination,     pCompressedData + 8, nCurrentBlockWidth, nCurrentBlockHeight, 2, m_vSize.x*2);
						DecodeDXT5AlphaBlock(pnCurrentDestination + 1, pCompressedData,     nCurrentBlockWidth, nCurrentBlockHeight, 2, m_vSize.x*2);
						pCompressedData += 16;
					}
				}

				// Update current data pointer (xy-plane (z/depth layer) are independent when it comes to compressed blocks)
				pCurrentData += nBytesPerPlane;
			}
			break;
		}

		case CompressionNone:
		default:
			// Nothing to do in here
			break;
	}

	// Done
	return true;
}

/**
*  @brief
*    Set color palette
*/
void ImageData::SetPalette(ImagePalette *pPalette)
{
	// Destroy old palette
	if (m_pPalette) {
		delete m_pPalette;
		m_pPalette = nullptr;
	}

	// Set new palette
	m_pPalette = pPalette;
}

/**
*  @brief
*    Copy provided uncompressed image data into this image data
*/
void ImageData::CopyData(const uint8 *pnData)
{
	// Make sure that buffer is created
	if (!m_pData) {
		// Create buffer
		CreateBuffer();
	}

	// Invalidate compressed image buffer, because uncompressed data is going to be modified
	DestroyCompressedBuffer();

	// Copy over the provided data, no matter whether or not m_pData points to shared data
	MemoryManager::Copy(m_pData, pnData, m_nDataSize);
}

/**
*  @brief
*    Let this image data takeover provided uncompressed image data
*/
void ImageData::TakeoverData(uint8 *pnData)
{
	// Invalidate uncompressed image buffer, because we're getting brand new data
	DestroyBuffer();

	// Invalidate compressed image buffer, because we're getting brand new data
	DestroyCompressedBuffer();

	// Just takeover the given image data pointer. Within the method comments the user was informed
	// that the provided image data is destroyed automatically when it's no longer used.
	m_pData = pnData;
}

/**
*  @brief
*    Let this image data share provided uncompressed image data
*/
void ImageData::ShareData(uint8 *pnData)
{
	// Invalidate uncompressed image buffer, because we're getting brand new data
	DestroyBuffer();

	// Invalidate compressed image buffer, because we're getting brand new data
	DestroyCompressedBuffer();

	// Just share the given image data pointer. Within the method comments the user was informed
	// about the consequences and potential risks in sharing image data.
	m_pData       = pnData;
	m_bDataShared = true;
}

/**
*  @brief
*    Calculates the size of the compressed image buffer
*/
void ImageData::CalculateCompressedImageBufferSize()
{
	// Initialize the compressed size
	m_nCompressedSize = 0;

	// Calculate the size depending on the used compression
	switch (m_nCompression) {
		// DXT formats divide the texture buffer into 4x4 blocks, each block has a fixed number of bytes
		case CompressionDXT1:
		case CompressionLATC1:
			// Avoid slow division by using bit-shift
			m_nCompressedSize = ((m_vSize.x + 3) >> 2) * ((m_vSize.y + 3) >> 2) * 8;
		//	m_nCompressedSize = ((m_vSize.x + 3) / 4) * ((m_vSize.y + 3) / 4) * 8;
			break;

		case CompressionDXT3:
		case CompressionDXT5:
		case CompressionLATC2:
			// Avoid slow division by using bit-shift
			m_nCompressedSize = ((m_vSize.x + 3) >> 2) * ((m_vSize.y + 3) >> 2) * 16;
		//	m_nCompressedSize = ((m_vSize.x + 3) / 4) * ((m_vSize.y + 3) / 4) * 16;
			break;

		case CompressionNone:
		default:
			// Nothing to do in here
			break;
	}

	// Each slice of a 3D image is handled as usual 2D image
	m_nCompressedSize *= m_vSize.z;
}

/**
*  @brief
*    Decodes a DXT color block
*/
void ImageData::DecodeDXTColorBlock(uint8 *pnDestination, const uint8 *pnSource, int nBlockWidth, int nBlockHeight, int nXOffset, int nYOffset, ECompression nCompression, int nRedIndex, int nBlueIndex) const
{
	// This implementation is basing on code from Humus (http://www.humus.name)

	const uint16 c0 = *reinterpret_cast<const uint16*>(pnSource);
	const uint16 c1 = *reinterpret_cast<const uint16*>(pnSource + 2);

	uint8 nColors[4][3];

	nColors[0][0] = ((c0 >> 11) & 0x1F) << 3;
	nColors[0][1] = ((c0 >>  5) & 0x3F) << 2;
	nColors[0][2] =  (c0        & 0x1F) << 3;

	nColors[1][0] = ((c1 >> 11) & 0x1F) << 3;
	nColors[1][1] = ((c1 >>  5) & 0x3F) << 2;
	nColors[1][2] =  (c1        & 0x1F) << 3;

	if (c0 > c1 || nCompression == CompressionDXT5) {
		for (int i=0; i<3; i++) {
			nColors[2][i] = (2*nColors[0][i] +   nColors[1][i] + 1)/3;
			nColors[3][i] = (  nColors[0][i] + 2*nColors[1][i] + 1)/3;
		}
	} else {
		for (int i=0; i<3; i++) {
			nColors[2][i] = (nColors[0][i] + nColors[1][i] + 1) >> 1;
			nColors[3][i] = 0;
		}
	}

	pnSource += 4;
	for (int y=0; y<nBlockHeight; y++) {
		uint8 *pnCurrentDestination = pnDestination + nYOffset*y;
		uint32 nIndexes = pnSource[y];
		for (int x=0; x<nBlockWidth; x++) {
			const uint32 nIndex = nIndexes & 0x3;
			pnCurrentDestination[nRedIndex]  = nColors[nIndex][0];
			pnCurrentDestination[1]          = nColors[nIndex][1];
			pnCurrentDestination[nBlueIndex] = nColors[nIndex][2];
			nIndexes >>= 2;

			pnCurrentDestination += nXOffset;
		}
	}
}

/**
*  @brief
*    Decodes a DXT3 alpha block
*/
void ImageData::DecodeDXT3AlphaBlock(uint8 *pnDestination, const uint8 *pnSource, int nBlockWidth, int nBlockHeight, int nXOffset, int nYOffset) const
{
	// This implementation is basing on code from Humus (http://www.humus.name)

	for (int y=0; y<nBlockHeight; y++) {
		uint8 *pnCurrentDestination = pnDestination + nYOffset*y;
		uint32 nAlpha = reinterpret_cast<const uint16*>(pnSource)[y];
		for (int x=0; x<nBlockWidth; x++) {
			*pnCurrentDestination = (nAlpha & 0xF)*17;
			nAlpha >>= 4;
			pnCurrentDestination += nXOffset;
		}
	}
}

/**
*  @brief
*    Decodes a DXT5 alpha block
*/
void ImageData::DecodeDXT5AlphaBlock(uint8 *pnDestination, const uint8 *pnSource, int nBlockWidth, int nBlockHeight, int nXOffset, int nYOffset) const
{
	// This implementation is basing on code from Humus (http://www.humus.name)

	const uint8 a0 = pnSource[0];
	const uint8 a1 = pnSource[1];
	uint64 nAlpha = *reinterpret_cast<const uint64*>(pnSource) >> 16;

	for (int y=0; y<nBlockHeight; y++) {
		uint8 *pnCurrentDestination = pnDestination + nYOffset*y;
		for (int x=0; x<nBlockWidth; x++) {
			const int k = static_cast<uint32>(nAlpha) & 0x7;
			if (k == 0){
				*pnCurrentDestination = a0;
			} else if (k == 1){
				*pnCurrentDestination = a1;
			} else if (a0 > a1){
				*pnCurrentDestination = static_cast<uint8>(((8 - k)*a0 + (k - 1)*a1)/7);
			} else if (k >= 6){
				*pnCurrentDestination = (k == 6) ? 0 : 255;
			} else {
				*pnCurrentDestination = static_cast<uint8>(((6 - k)*a0 + (k - 1)*a1)/5);
			}
			nAlpha >>= 3;

			pnCurrentDestination += nXOffset;
		}
		if (nBlockWidth < 4)
			nAlpha >>= (3*(4 - nBlockWidth));
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics
