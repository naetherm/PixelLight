/*********************************************************\
 *  File: ImageLoaderBMP.cpp                             *
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
#include "PLGraphics/Image/Loader/ImageLoaderBMP.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLGraphics {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ImageLoaderBMP)


//[-------------------------------------------------------]
//[ Structures                                            ]
//[-------------------------------------------------------]
// Define structures -> Note: If we don't set "pack(1)" we may get a struct size of 16 bytes which is terrible wrong!
#pragma pack(push)	// Push current alignment to stack
#pragma pack(1)		// Set alignment to 1 byte boundary
struct BitmapFileHeader {
	uint16 nType;		/**< The magic number used to identify the BMP file: 0x42 0x4D (Hex code points for B and M) */
	uint32 nSize;		/**< The size of the BMP file in bytes */
	uint32 nReserved;	/**< Reserved */
	uint32 nOffBits;	/**< The offset, i.e. starting address, of the byte where the bitmap data can be found */
};
struct BitmapInfoHeader {
	uint32 nSize;			/**< The size of this header (40 bytes) */
	uint32 nWidth;			/**< The bitmap width in pixels */
	 int32 nHeight;			/**< The bitmap height in pixels, positive="bottom-up", negative="top-down" */
	uint16 nPlanes;			/**< The number of color planes being used. Must be set to 1. */
	uint16 nBitCount;		/**< The number of bits per pixel, which is the color depth of the image. Typical values are 1, 4, 8, 16, 24 and 32. */
	uint32 nCompression;	/**< The compression method being used */
	uint32 nSizeImage;		/**< The image size. This is the size of the raw bitmap data, and should not be confused with the file size. */
	 int32 nXPelsPerMeter;	/**< The horizontal resolution of the image (pixel per meter) */
	 int32 nYPelsPerMeter;	/**< The vertical resolution of the image (pixel per meter) */
	uint32 nClrUsed;		/**< The number of colors in the color palette, or 0 to default to 2n */
	uint32 nClrImportant;	/**< The number of important colors used, or 0 when every color is important; generally ignored. */
};
#pragma pack(pop)	// Restore original alignment from stack


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool ImageLoaderBMP::Load(Image &cImage, File &cFile)
{
	// Read the header (14 bytes)
	BitmapFileHeader sHeader;
	cFile.Read(&sHeader, 1, sizeof(BitmapFileHeader));

	// Check the bitmap type ("BM")
	if (sHeader.nType == 19778) {
		// Read the header (40 bytes)
		BitmapInfoHeader sInfo;
		cFile.Read(&sInfo, 1, sizeof(BitmapInfoHeader));

		// Compression used?
		if (!sInfo.nCompression) {
			// Get the color format
			EColorFormat nColorFormat;
			switch (sInfo.nBitCount){
				case 8:
					nColorFormat = ColorGrayscale;
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

			// Create image buffer
			ImageBuffer *pImageBuffer = cImage.CreatePart()->CreateMipmap();
			pImageBuffer->CreateImage(DataByte, nColorFormat, Vector3i(sInfo.nWidth, sInfo.nHeight, 1));

			// Line padding: Each new line must start on an even 32 bit boundary! (... else it would be TO simple...)
			int nPaddBytes = (pImageBuffer->GetSize().x*pImageBuffer->GetComponentsPerPixel()) & 3;
			if (nPaddBytes)
				nPaddBytes = 4 - nPaddBytes;

			// Do we need to flip the y-axis? Height: negative="top-down"... the way we store our image
			const bool bFlipY = (sInfo.nHeight >= 0);

			// Load the data
			switch (sInfo.nBitCount) {
				case 8:
				{
					// [TODO] Line padding for this too?
					// [TODO] Take care of y-flip
					// [TODO] Keep palette?

					// Read palette
					uint8 nPalette[1024];
					cFile.Read(nPalette, sizeof(nPalette), 1);

					// Read the data
					cFile.Read(pImageBuffer->GetData(), 1, pImageBuffer->GetDataSize());

					// We don't want to keep the palette
					const uint32  nNumOfPixels = pImageBuffer->GetNumOfPixels();
						  uint8  *pData        = pImageBuffer->GetData();
					for (uint32 i=0; i<nNumOfPixels; i++, pData++)
						*pData = nPalette[((*pData) << 2)];
					break;
				}

				case 24:
				case 32:
					// Do we need to flip the y-axis?
					if (bFlipY) {
						// Do we need to take care of padd bytes?
						if (nPaddBytes) {
							// Read row for row y-flipped and ignore padd bytes
							const uint32  nBytesPerRow = pImageBuffer->GetBytesPerRow();
								  uint8  *pData        = pImageBuffer->GetData() + pImageBuffer->GetDataSize() - nBytesPerRow;
								  uint32  nPadd        = 0;
							for (const uint8 *pDataEnd=pImageBuffer->GetData(); pData>=pDataEnd; pData-=nBytesPerRow) {
								// Read row
								cFile.Read(pData, 1, nBytesPerRow);

								// Read padd bytes
								cFile.Read(&nPadd, 1, nPaddBytes);
							}
						} else {
							// Read row for row y-flipped
							const uint32  nBytesPerRow = pImageBuffer->GetBytesPerRow();
								  uint8  *pData        = pImageBuffer->GetData() + pImageBuffer->GetDataSize() - nBytesPerRow;
							for (const uint8 *pDataEnd=pImageBuffer->GetData(); pData>=pDataEnd; pData-=nBytesPerRow) {
								// Read row
								cFile.Read(pData, 1, nBytesPerRow);
							}
						}
					} else {
						// Do we need to take care of padd bytes?
						if (nPaddBytes) {
							// Read row for row and ignore padd bytes
							const uint32  nBytesPerRow = pImageBuffer->GetBytesPerRow();
								  uint8  *pData        = pImageBuffer->GetData();
								  uint32  nPadd        = 0;
							for (const uint8 *pDataEnd=pImageBuffer->GetData()+pImageBuffer->GetDataSize(); pData<pDataEnd; pData+=nBytesPerRow) {
								// Read row
								cFile.Read(pData, 1, nBytesPerRow);

								// Read padd bytes
								cFile.Read(&nPadd, 1, nPaddBytes);
							}
						} else {
							// That's easy, just read in the data
							cFile.Read(pImageBuffer->GetData(), 1, pImageBuffer->GetDataSize());
						}
					}

					// Convert BGR(A) to RGB(A)
					if (sInfo.nBitCount == 24)
						Color3::SwapRB(pImageBuffer->GetData(), pImageBuffer->GetNumOfPixels());
					else
						Color4::SwapRB(pImageBuffer->GetData(), pImageBuffer->GetNumOfPixels());
					break;
			}

			// Done
			return true;
		} else {
			// Error: Compression not supported
		}
	} else {
		// Error: Invalid bitmap type
	}

	// Error!
	return false;
}

bool ImageLoaderBMP::Save(const Image &cImage, File &cFile)
{
	// Get the image buffer
	ImageBuffer *pImageBuffer = cImage.GetBuffer();
	if (pImageBuffer && pImageBuffer->GetBytesPerRow()) {
		// We only support 1 byte per pixel component
		if (pImageBuffer->GetBytesPerPixelComponent() == 1) {
			// Write the header (14 bytes)
			BitmapFileHeader sHeader;
			sHeader.nType		= 19778; // "BM"
			sHeader.nSize		= sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + pImageBuffer->GetDataSize();
			sHeader.nReserved	= 0;
			sHeader.nOffBits	= sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);
			cFile.Write(&sHeader, 1, sizeof(BitmapFileHeader));

			// Write the header (40 bytes)
			BitmapInfoHeader sInfo;
			sInfo.nSize				= sizeof(BitmapInfoHeader);
			sInfo.nWidth			= pImageBuffer->GetSize().x;
			sInfo.nHeight			= -static_cast<int32>(pImageBuffer->GetSize().y); // negative="top-down"... the way we store our image
			sInfo.nPlanes			= 1;
			sInfo.nBitCount			= static_cast<uint8>(pImageBuffer->GetBytesPerPixel()*8);
			sInfo.nCompression		= 0;
			sInfo.nSizeImage		= pImageBuffer->GetDataSize();
			sInfo.nXPelsPerMeter	= 0;
			sInfo.nYPelsPerMeter	= 0;
			sInfo.nClrUsed			= 0;
			sInfo.nClrImportant		= 0;
			cFile.Write(&sInfo, 1, sizeof(BitmapInfoHeader));

			// Line padding: Each new line must start on an even 32 bit boundary! (... else it would be TO simple...)
			int nPaddBytes = (pImageBuffer->GetSize().x*pImageBuffer->GetComponentsPerPixel()) & 3;
			if (nPaddBytes)
				nPaddBytes = 4 - nPaddBytes;

			// Get the color format
			switch (pImageBuffer->GetColorFormat()) {
				case ColorRGB:
				{
					// Do we need to take care of padd bytes?
					if (nPaddBytes) {
						// Write row for row and add padd bytes
						const uint32  nHeight      = pImageBuffer->GetSize().y;
						const uint32  nBytesPerRow = pImageBuffer->GetBytesPerRow();
						const uint8  *pData        = pImageBuffer->GetData();
						const uint32  nPadd        = 0;
						for (uint32 nY=0; nY<nHeight; nY++) {
							// Write row in BGR
							for (const uint8 *pDataEnd=pData+nBytesPerRow; pData<pDataEnd; pData+=3) {
								// Write blue
								uint8 nByte = pData[2];
								cFile.Write(&nByte, 1, 1);

								// Write green
								nByte = pData[1];
								cFile.Write(&nByte, 1, 1);

								// Write red
								nByte = pData[0];
								cFile.Write(&nByte, 1, 1);
							}

							// Write padd bytes
							cFile.Write(&nPadd, 1, nPaddBytes);
						}
					} else {
						// Save all pixels in BGR
						const uint8 *pData = pImageBuffer->GetData();
						for (const uint8 *pDataEnd=pData+pImageBuffer->GetDataSize(); pData<pDataEnd; pData+=3) {
							// Write blue
							uint8 nByte = pData[2];
							cFile.Write(&nByte, 1, 1);

							// Write green
							nByte = pData[1];
							cFile.Write(&nByte, 1, 1);

							// Write red
							nByte = pData[0];
							cFile.Write(&nByte, 1, 1);
						}
					}

					// Done
					return true;
				}

				case ColorRGBA:
				{
					// Do we need to take care of padd bytes?
					if (nPaddBytes) {
						// Write row for row and add padd bytes
						const uint32  nHeight      = pImageBuffer->GetSize().y;
						const uint32  nBytesPerRow = pImageBuffer->GetBytesPerRow();
						const uint8  *pData        = pImageBuffer->GetData();
						const uint32  nPadd        = 0;
						for (uint32 nY=0; nY<nHeight; nY++) {
							// Write row in BGRA
							for (const uint8 *pDataEnd=pData+nBytesPerRow; pData<pDataEnd; pData+=4) {
								// Write blue
								uint8 nByte = pData[2];
								cFile.Write(&nByte, 1, 1);

								// Write green
								nByte = pData[1];
								cFile.Write(&nByte, 1, 1);

								// Write red
								nByte = pData[0];
								cFile.Write(&nByte, 1, 1);

								// Write alpha
								nByte = pData[3];
								cFile.Write(&nByte, 1, 1);
							}

							// Write padd bytes
							cFile.Write(&nPadd, 1, nPaddBytes);
						}
					} else {
						// Save all pixels in BGRA
						const uint8 *pData = pImageBuffer->GetData();
						for (const uint8 *pDataEnd=pData+pImageBuffer->GetDataSize(); pData<pDataEnd; pData+=4) {
							// Write blue
							uint8 nByte = pData[2];
							cFile.Write(&nByte, 1, 1);

							// Write green
							nByte = pData[1];
							cFile.Write(&nByte, 1, 1);

							// Write red
							nByte = pData[0];
							cFile.Write(&nByte, 1, 1);

							// Write alpha
							nByte = pData[3];
							cFile.Write(&nByte, 1, 1);
						}
					}

					// Done
					return true;
				}

				case ColorBGR:
				case ColorBGRA:
					// Do we need to take care of padd bytes?
					if (nPaddBytes) {
						// Write row for row and add padd bytes
						const uint32  nBytesPerRow = pImageBuffer->GetBytesPerRow();
						const uint8  *pData        = pImageBuffer->GetData();
						const uint32  nPadd        = 0;
						for (const uint8 *pDataEnd=pImageBuffer->GetData()+pImageBuffer->GetDataSize(); pData<pDataEnd; pData+=nBytesPerRow) {
							// Write row
							cFile.Write(pData, 1, nBytesPerRow);

							// Write padd bytes
							cFile.Write(&nPadd, 1, nPaddBytes);
						}
					} else {
						// That's easy, just throw the data into the file :)
						cFile.Write(pImageBuffer->GetData(), 1, pImageBuffer->GetDataSize());
					}

					// Done
					return true;

				case ColorGrayscale:
				{
					// [TODO] Line padding for this too?

					// Write palette
					uint8 nPalette[1024];
					for (uint32 i=0; i<1024; i++)
						nPalette[i] = static_cast<uint8>(i >> 2);
					cFile.Write(nPalette, sizeof(nPalette), 1);

					// Write the data
					cFile.Write(pImageBuffer->GetData(), 1, pImageBuffer->GetDataSize());

					// Done
					return true;
				}

				case ColorGrayscaleA:
				case ColorPalette:
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
ImageLoaderBMP::ImageLoaderBMP()
{
}

/**
*  @brief
*    Destructor
*/
ImageLoaderBMP::~ImageLoaderBMP()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics
