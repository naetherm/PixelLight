/*********************************************************\
 *  File: ImageLoaderTGA.cpp                             *
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
#include <PLCore/File/File.h>
#include "PLGraphics/Color/Color3.h"
#include "PLGraphics/Color/Color4.h"
#include "PLGraphics/Image/Image.h"
#include "PLGraphics/Image/ImagePart.h"
#include "PLGraphics/Image/ImageBuffer.h"
#include "PLGraphics/Image/ImagePalette.h"
#include "PLGraphics/Image/Loader/ImageLoaderTGA.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLGraphics {


//[-------------------------------------------------------]
//[ Structures                                            ]
//[-------------------------------------------------------]
// Define structures -> Note: If we don't set "pack(1)" we may get a struct size of 16 bytes which is terrible wrong!
#pragma pack(push)	// Push current alignment to stack
#pragma pack(1)		// Set alignment to 1 byte boundary
struct TGAHeader {
	uint8  IDLength;			/**< Length of the Image ID field in bytes */
	uint8  nColorMapType;		/**< Whether a color map is included */
	uint8  nImageType;			/**< Compression and color types */
	uint16 nColorMapStart;		/**< Describes the color map (= palette) */
	uint16 nColorMapEntries;	/**< Describes the color map (= palette) */
	uint8  nColorMapBits;		/**< Describes the color map (= palette) */
	uint16 nXOffset;			/**< x offset */
	uint16 nYOffset;			/**< y offset */
	uint16 nWidth;				/**< Image width */
	uint16 nHeight;				/**< Image height */
	uint8  nBitsPerPixel;		/**< The number of bits per pixel, which is the color depth of the image. Typical values are 8, 16, 24 and 32. */
	uint8  nImageDescriptor;	/**< Image descriptor */
};
#pragma pack(pop)	// Restore original alignment from stack


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ImageLoaderTGA)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool ImageLoaderTGA::Load(Image &cImage, File &cFile)
{
	// Get the file size
	const uint32 nFileSize = cFile.GetSize();

	// Read the header
	TGAHeader sHeader;
	cFile.Read(&sHeader, sizeof(sHeader), 1);

	// Skip image ID
	cFile.Seek(sHeader.IDLength, File::SeekCurrent);

	// Read the palette, if there's one
	uint8 *pnPalette = nullptr;
	EColorFormat nColorFormat;
	const uint32 nPaletteLength = sHeader.nColorMapEntries*sHeader.nColorMapBits/8;
	if (nPaletteLength) {
		// Allocate the palette
		pnPalette = new uint8[nPaletteLength];

		// Read the palette
		cFile.Read(pnPalette, nPaletteLength, 1);

		// Set the color format
		nColorFormat = ColorPalette;
	} else {
		// Get the color format
		switch (sHeader.nBitsPerPixel){
			case 8:
				nColorFormat = ColorRGB;
				break;

			case 16:
				nColorFormat = ColorRGBA;
				break;

			case 24:
				nColorFormat = ColorRGB;
				break;

			case 32:
				nColorFormat = ColorRGBA;
				break;

			default:
				// Error: Unsupported color format
				return false;
		}
	}

	// Read the file data
	const uint32 nReadBufferSize = nFileSize - sizeof(sHeader) - nPaletteLength;
	uint8 *pnReadBuffer = new uint8[nReadBufferSize];
	cFile.Read(pnReadBuffer, nReadBufferSize, 1);

	// Decode if RLE compressed. Bit 3 of 'nImageType' tells if the file is compressed.
	uint8 *pnSourceBuffer;
	if (sHeader.nImageType & 0x08) {
		// Get the number of bytes per pixel
		const uint32 nBytesPerPixel = sHeader.nBitsPerPixel/8;

		// Create the buffer holding the decompressed data
		int    nSize        = sHeader.nWidth*sHeader.nHeight*nBytesPerPixel;
		uint8 *pnTempBuffer = new uint8[nSize];

		// Setup source and destination buffer pointers
		uint8 *pnTempSource      = pnReadBuffer;
		uint8 *pnTempDestination = pnTempBuffer;

		// Decompress
		while (nSize > 0) {
			// Get packet header
			uint8 nRepetitionCount = *pnTempSource;

			// Go to the value part
			pnTempSource++;

			// Read RLE or raw packet depending on the highest order bit
			if (nRepetitionCount & 0x80) {
				// RLE packet

				// Clear the highest order bit
				nRepetitionCount &= ~0x80;

				// Get the number of pixels
				uint8 nNumOfPixels = nRepetitionCount + 1; // +1 because 0 means 1 pixel!

				// Update the left size after we read the RLE packet
				nSize -= nNumOfPixels*nBytesPerPixel;

				// Fill all pixels with the same value
				do {
					// Fill destination
					MemoryManager::Copy(pnTempDestination, pnTempSource, nBytesPerPixel);

					// Update the destination pointer
					pnTempDestination += nBytesPerPixel;
				} while (--nNumOfPixels);

				// Update the source pointer
				pnTempSource += nBytesPerPixel;
			} else {
				// Raw packet

				// Get the number of bytes to read
				const uint32 nNumOfBytes = (nRepetitionCount + 1)*nBytesPerPixel; // +1 because 0 means 1 pixel!

				// Just copy the bytes from the source to the destination buffer!
				MemoryManager::Copy(pnTempDestination, pnTempSource, nNumOfBytes);

				// Update the buffer pointers
				pnTempSource      += nNumOfBytes;
				pnTempDestination += nNumOfBytes;

				// Update the left size
				nSize -= nNumOfBytes;
			}
		}

		// We now have a new source buffer holding the decompressed data
		pnSourceBuffer = pnTempBuffer;

		// Destroy the no longer required original read data
		delete [] pnReadBuffer;
	} else {
		// Source buffer = original read data
		pnSourceBuffer = pnReadBuffer;
	}

	// Create image buffer
	ImageBuffer *pImageBuffer = cImage.CreatePart()->CreateMipmap();
	pImageBuffer->CreateImage(DataByte, nColorFormat, Vector3i(sHeader.nWidth, sHeader.nHeight, 1));

	// We need to y-flip the image
	switch (sHeader.nBitsPerPixel) {
		case 8:
		{
			// Is there a palette?
			if (nPaletteLength) {
				// Create palette
				ImagePalette *pPalette = new ImagePalette();
				pPalette->Create(sHeader.nColorMapEntries);

				// Copy palette data
				MemoryManager::Copy(pPalette->GetData(), pnPalette, nPaletteLength);

				// Set palette
				pImageBuffer->SetPalette(pPalette);
			}

			// Copy all rows in reversed order
			const uint32  nBytesPerRow = pImageBuffer->GetBytesPerRow();
				  uint8  *pnData       = pImageBuffer->GetData();
			const uint8  *pSourceData  = pnSourceBuffer + pImageBuffer->GetDataSize() - nBytesPerRow; // ... ignore the last row...
			for (const uint8 *pnDataEnd=pImageBuffer->GetData()+pImageBuffer->GetDataSize(); pnData<pnDataEnd; pnData+=nBytesPerRow, pSourceData-=nBytesPerRow)
				MemoryManager::Copy(pnData, pSourceData, nBytesPerRow);
			break;
		}

		case 16:
		{
			// Extract all rows in reversed order
			const uint32  nSourceRowSize = (sHeader.nBitsPerPixel/8)*sHeader.nWidth;
			const uint8  *pSourceData    = pnSourceBuffer + nSourceRowSize*sHeader.nHeight - nSourceRowSize; // Start of last row
				  uint8  *pnData         = pImageBuffer->GetData();
			for (const uint8 *pnDataEnd=pImageBuffer->GetData()+pImageBuffer->GetDataSize(); pnData<pnDataEnd; pSourceData-=nSourceRowSize*2) {
				// Go through the current row
				for (uint32 nX=0; nX<sHeader.nWidth; nX++){
					// Get 16 bit pixel
					const uint16 nTempPixel = *reinterpret_cast<const uint16*>(pSourceData);

					// Expand to 32 bit pixel and swap BGRA to RGBA
					pnData[2] = (nTempPixel & 0x001F) << 3;						// Blue
					pnData[1] = static_cast<uint8>((nTempPixel & 0x03E0) >> 2);	// Green
					pnData[0] = static_cast<uint8>((nTempPixel & 0x7C00) >> 7);	// Red
					pnData[3] = (nTempPixel & 0xfc00) ? 0xFF : 0;				// Alpha

					// Next, please
					pnData      += 4;
					pSourceData += 2;
				}
			}
			break;
		}

		case 24:
		case 32:
		{
			// Copy all rows in reversed order
			const uint32  nBytesPerRow = pImageBuffer->GetBytesPerRow();
				  uint8  *pnData       = pImageBuffer->GetData();
			const uint8  *pSourceData  = pnSourceBuffer + pImageBuffer->GetDataSize() - nBytesPerRow; // ... ignore the last row...
			for (const uint8 *pnDataEnd=pImageBuffer->GetData()+pImageBuffer->GetDataSize(); pnData<pnDataEnd; pnData+=nBytesPerRow, pSourceData-=nBytesPerRow)
				MemoryManager::Copy(pnData, pSourceData, nBytesPerRow);

			// Convert BGR(A) to RGB(A)
			if (sHeader.nBitsPerPixel == 24)
				Color3::SwapRB(pImageBuffer->GetData(), pImageBuffer->GetNumOfPixels());
			else
				Color4::SwapRB(pImageBuffer->GetData(), pImageBuffer->GetNumOfPixels());
			break;
		}
	}

	// Cleanup
	delete [] pnSourceBuffer;

	// Cleanup the palette - if there's one
	if (pnPalette)
		delete [] pnPalette;

	// Done
	return true;
}

bool ImageLoaderTGA::Save(const Image &cImage, File &cFile)
{
	// Get the image buffer
	ImageBuffer *pImageBuffer = cImage.GetBuffer();
	if (pImageBuffer && pImageBuffer->GetBytesPerRow()) {
		// We only support 1 byte per pixel component
		if (pImageBuffer->GetBytesPerPixelComponent() == 1) {
			// Palette?
			if (pImageBuffer->GetColorFormat() == ColorPalette && (!pImageBuffer->GetPalette() || pImageBuffer->GetPalette()->GetNumOfColors() != 256))
				return false; // Error!
			const bool bPalette = (pImageBuffer->GetColorFormat() == ColorGrayscale || pImageBuffer->GetColorFormat() == ColorPalette);

			// Write the header
			TGAHeader sHeader;
			sHeader.IDLength			= 0;
			sHeader.nColorMapType		= bPalette ? 1 : 0;
			sHeader.nImageType			= bPalette ? 1 : 2;
			sHeader.nColorMapStart		= 0;
			sHeader.nColorMapEntries	= bPalette ? 256 : 0;
			sHeader.nColorMapBits		= bPalette ? 24  : 0;
			sHeader.nXOffset			= 0;
			sHeader.nYOffset			= 0;
			sHeader.nWidth				= static_cast<uint16>(pImageBuffer->GetSize().x);
			sHeader.nHeight				= static_cast<uint16>(pImageBuffer->GetSize().y);
			sHeader.nBitsPerPixel		= static_cast<uint8>(pImageBuffer->GetBytesPerPixel()*8);
			sHeader.nImageDescriptor	= 0;
			cFile.Write(&sHeader, 1, sizeof(TGAHeader));

			// Get the color format
			switch (pImageBuffer->GetColorFormat()) {
				case ColorRGB:
				{
					// Write row for row y-flipped
					const uint32  nBytesPerRow = pImageBuffer->GetBytesPerRow();
					const uint8  *pnData       = pImageBuffer->GetData() + pImageBuffer->GetDataSize() - nBytesPerRow;
					for (const uint8 *pnDataEnd=pImageBuffer->GetData(); pnData>=pnDataEnd; pnData-=nBytesPerRow) {
						// Save all pixels of the row in BGR
						const uint8 *pRowData = pnData;
						for (const uint8 *pnRowEnd=pRowData+nBytesPerRow; pRowData<pnRowEnd; pRowData+=3) {
							// Write blue
							uint8 nByte = pRowData[2];
							cFile.Write(&nByte, 1, 1);

							// Write green
							nByte = pRowData[1];
							cFile.Write(&nByte, 1, 1);

							// Write red
							nByte = pRowData[0];
							cFile.Write(&nByte, 1, 1);
						}
					}

					// Done
					return true;
				}

				case ColorRGBA:
				{
					// Write row for row y-flipped
					const uint32  nBytesPerRow = pImageBuffer->GetBytesPerRow();
					const uint8  *pnData       = pImageBuffer->GetData() + pImageBuffer->GetDataSize() - nBytesPerRow;
					for (const uint8 *pnDataEnd=pImageBuffer->GetData(); pnData>=pnDataEnd; pnData-=nBytesPerRow) {
						// Save all pixels of the row in BGRA
						const uint8 *pRowData = pnData;
						for (const uint8 *pnRowEnd=pRowData+nBytesPerRow; pRowData<pnRowEnd; pRowData+=4) {
							// Write blue
							uint8 nByte = pRowData[2];
							cFile.Write(&nByte, 1, 1);

							// Write green
							nByte = pRowData[1];
							cFile.Write(&nByte, 1, 1);

							// Write red
							nByte = pRowData[0];
							cFile.Write(&nByte, 1, 1);

							// Write alpha
							nByte = pRowData[3];
							cFile.Write(&nByte, 1, 1);
						}
					}

					// Done
					return true;
				}

				case ColorBGR:
				case ColorBGRA:
				{
					// Write row for row y-flipped
					const uint32  nBytesPerRow = pImageBuffer->GetBytesPerRow();
					const uint8  *pnData       = pImageBuffer->GetData() + pImageBuffer->GetDataSize() - nBytesPerRow;
					for (const uint8 *pnDataEnd=pImageBuffer->GetData(); pnData>=pnDataEnd; pnData-=nBytesPerRow) {
						// Write row
						cFile.Write(pnData, 1, nBytesPerRow);
					}

					// Done
					return true;
				}

				case ColorGrayscale:
				{
					// Write palette
					uint8 nPalette[768];
					for (uint32 i=0, p=0; i<256; i++){
						nPalette[p++] = static_cast<uint8>(i);
						nPalette[p++] = static_cast<uint8>(i);
						nPalette[p++] = static_cast<uint8>(i);
					}
					cFile.Write(nPalette, sizeof(nPalette), 1);

					// Write row for row y-flipped
					const uint32  nBytesPerRow = pImageBuffer->GetBytesPerRow();
					const uint8  *pnData       = pImageBuffer->GetData() + pImageBuffer->GetDataSize() - nBytesPerRow;
					for (const uint8 *pnDataEnd=pImageBuffer->GetData(); pnData>=pnDataEnd; pnData-=nBytesPerRow) {
						// Write row
						cFile.Write(pnData, 1, nBytesPerRow);
					}

					// Done
					return true;
				}

				case ColorPalette:
				{
					// Write palette
					ImagePalette *pImagePalette = pImageBuffer->GetPalette();
					if (pImagePalette) {
						cFile.Write(pImagePalette->GetData(), 768, 1);

						// Write row for row y-flipped
						const uint32  nBytesPerRow = pImageBuffer->GetBytesPerRow();
						const uint8  *pnData       = pImageBuffer->GetData() + pImageBuffer->GetDataSize() - nBytesPerRow;
						for (const uint8 *pnDataEnd=pImageBuffer->GetData(); pnData>=pnDataEnd; pnData-=nBytesPerRow) {
							// Write row
							cFile.Write(pnData, 1, nBytesPerRow);
						}

						// Done
						return true;
					}
					break;
				}

				case ColorGrayscaleA:
				default:
					// Error; Unsupported color format
					break;
			}
		} else {
			// Error: Unsupported number of bytes per pixel component
		}
	} else {
		// Error: Failed to get image buffer
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ImageLoaderTGA::ImageLoaderTGA()
{
}

/**
*  @brief
*    Destructor
*/
ImageLoaderTGA::~ImageLoaderTGA()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics
