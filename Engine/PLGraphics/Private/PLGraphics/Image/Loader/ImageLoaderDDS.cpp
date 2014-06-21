/*********************************************************\
 *  File: ImageLoaderDDS.cpp                             *
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
#include "PLGraphics/Image/Loader/ImageLoaderDDS.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLGraphics {


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
static const uint32 DDSCAPS2_CUBEMAP			= 0x00000200;
static const uint32 DDS_FOURCC					= 0x00000004;
static const uint32 DDS_LUMINANCE				= 0x00020000;
static const uint32 DDS_ALPHAPIXELS				= 0x00000001;
static const uint32 DDS_LINEARSIZE				= 0x00080000;
static const uint32 DDS_PITCH					= 0x00000008;
static const uint32 DDSD_CAPS					= 0x00000001;
static const uint32 DDSD_PIXELFORMAT			= 0x00001000;
static const uint32 DDSD_HEIGHT					= 0x00000002;
static const uint32 DDSD_WIDTH					= 0x00000004;
static const uint32 DDSD_MIPMAPCOUNT			= 0x00020000;
static const uint32 DDSD_DEPTH					= 0x00800000;
static const uint32 DDPF_FOURCC					= 0x00000004;
static const uint32 DDSCAPS_TEXTURE				= 0x00001000;
static const uint32 DDSCAPS_MIPMAP				= 0x00400000;
static const uint32 DDSCAPS_COMPLEX				= 0x00000008;
static const uint32 DDSCAPS2_VOLUME				= 0x00200000;
static const uint32 DDSCAPS2_CUBEMAP_POSITIVEX	= 0x00000400;
static const uint32 DDSCAPS2_CUBEMAP_NEGATIVEX	= 0x00000800;
static const uint32 DDSCAPS2_CUBEMAP_POSITIVEY	= 0x00001000;
static const uint32 DDSCAPS2_CUBEMAP_NEGATIVEY	= 0x00002000;
static const uint32 DDSCAPS2_CUBEMAP_POSITIVEZ	= 0x00004000;
static const uint32 DDSCAPS2_CUBEMAP_NEGATIVEZ	= 0x00008000;
static const uint32 DDSCAPS2_CUBEMAP_ALL_FACES	= (DDSCAPS2_CUBEMAP_POSITIVEX | DDSCAPS2_CUBEMAP_NEGATIVEX | DDSCAPS2_CUBEMAP_POSITIVEY | DDSCAPS2_CUBEMAP_NEGATIVEY | DDSCAPS2_CUBEMAP_POSITIVEZ | DDSCAPS2_CUBEMAP_NEGATIVEZ);


//[-------------------------------------------------------]
//[ Structures                                            ]
//[-------------------------------------------------------]
struct DDSHeader {
	uint8  nMagic[4];
	uint32 nSize;
	uint32 nFlags;
	uint32 nHeight;
	uint32 nWidth;
	uint32 nPitchOrLinearSize;
	uint32 nDepth; 
	uint32 nMipMapCount;
	uint32 nReserved[11];

	struct {
		uint32 nSize;
		uint32 nFlags;
		uint32 nFourCC;
		uint32 nRGBBitCount;
		uint32 nRBitMask;
		uint32 nGBitMask;
		uint32 nBBitMask;
		uint32 nRGBAlphaBitMask;
	} ddpfPixelFormat;

	struct {
		uint32 nCaps1;
		uint32 nCaps2;
		uint32 nReserved[2];
	} ddsCaps;

	uint32 nReserved2;
};

struct DDSHeaderDX10 {
	uint32 nDXGIFormat;	// See http://msdn.microsoft.com/en-us/library/bb173059.aspx
	uint32 nResourceDimension;
	uint32 nMiscFlag;
	uint32 nArraySize;
	uint32 nReserved;
};


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ImageLoaderDDS)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool ImageLoaderDDS::Load(Image &cImage, File &cFile)
{
	#define MCHAR4(a, b, c, d) (a | (b << 8) | (c << 16) | (d << 24))

	// Read the header
	DDSHeader sHeader;
	cFile.Read(&sHeader, sizeof(sHeader), 1);
	if (sHeader.nMagic[0] == 'D' && sHeader.nMagic[1] == 'D' && sHeader.nMagic[2] == 'S' && sHeader.nMagic[3] == ' ' &&
		// Note that if "nSize" is "DDS " this is not a valid dds file according
		// to the file spec. Some broken tool out there seems to produce files
		// with this value in the size field, so we support reading them...
		(sHeader.nSize == 124 || sHeader.nSize != MCHAR4('D', 'D', 'S', ' '))) {

		// Get the color format and compression
		EDataFormat  nDataFormat = DataByte;
		EColorFormat nColorFormat;
		EColorFormat nInternalColorFormat;
		ECompression nCompression = CompressionNone;

		// Get the depth
		const uint32 nDepth = sHeader.nDepth ? sHeader.nDepth : 1;

		// Is this image compressed?
		if (sHeader.ddpfPixelFormat.nFlags & DDS_FOURCC) {
			// The image is compressed
			if (sHeader.ddpfPixelFormat.nFourCC == MCHAR4('D', 'X', '1', '0')) {
				// Read the DX10 header
				DDSHeaderDX10 sDX10Header;
				cFile.Read(&sDX10Header, sizeof(sDX10Header), 1);

				// Get the color format and compression
				switch (sDX10Header.nDXGIFormat) {
				// Integer
					// R8 UNORM
					case 61:
						nInternalColorFormat = nColorFormat = ColorGrayscale;
						break;

					// RG8 UNORM
					case 49:
						nInternalColorFormat = (sHeader.ddpfPixelFormat.nBBitMask == 0xFF) ? ColorBGR : ColorRGB;
						nColorFormat = ColorRGB;	// Store it as RGB
						break;

					// RGBA8 UNORM
					case 28:
						nInternalColorFormat = (sHeader.ddpfPixelFormat.nBBitMask == 0xFF) ? ColorBGRA : ColorRGBA;
						nColorFormat = ColorRGBA;
						break;

				// 16 bit float
					// R16F
					case 54:
						nInternalColorFormat = nColorFormat = ColorGrayscale;
						nDataFormat = DataHalf;
						break;

					// RG16F
					case 34:
						nInternalColorFormat = nColorFormat = ColorGrayscaleA;
						nDataFormat = DataHalf;
						break;

					// RGBA16F
					case 10:
						nInternalColorFormat = nColorFormat = ColorRGBA;
						nDataFormat = DataHalf;
						break;

				// IEEE 32 bit float
					// R32F
					case 41:
						nInternalColorFormat = nColorFormat = ColorGrayscale;
						nDataFormat = DataFloat;
						break;

					// RG32F
					case 16:
						nInternalColorFormat = nColorFormat = ColorGrayscaleA;
						nDataFormat = DataFloat;
						break;

					// RGB32F
					case 6:
						nInternalColorFormat = nColorFormat = ColorRGB;
						nDataFormat = DataFloat;
						break;

					// RGBA32F
					case 2:
						nInternalColorFormat = nColorFormat = ColorRGBA;
						nDataFormat = DataFloat;
						break;

				// Compressed
					// DXT1 (BC1 UNORM)
					case 71:
						nInternalColorFormat = nColorFormat = ColorRGB;
						nCompression = CompressionDXT1;
						break;

					// DXT3 (BC2 UNORM)
					case 74:
						nInternalColorFormat = nColorFormat = ColorRGBA;
						nCompression = CompressionDXT3;
						break;

					// DXT5 (BC3 UNORM)
					case 77:
						nInternalColorFormat = nColorFormat = ColorRGBA;
						nCompression = CompressionDXT5;
						break;

					// LATC1 (BC4 UNORM, previously known as ATI1N)
					case 80:
						nInternalColorFormat = nColorFormat = ColorGrayscale;
						nCompression = CompressionLATC1;
						break;

					// LATC2 (BC5 UNORM, previously known as ATI2N)
					case 83:
						nInternalColorFormat = nColorFormat = ColorGrayscaleA;
						nCompression = CompressionLATC2;
						break;

					default:
						// Error: Unsupported format
						return false;
				}
			} else {
				switch (sHeader.ddpfPixelFormat.nFourCC) {
				// 16 bit float
					// R16F
					case 111:
						nInternalColorFormat = nColorFormat = ColorGrayscale;
						nDataFormat = DataHalf;
						break;

					// RG16F
					case 112:
						nInternalColorFormat = nColorFormat = ColorGrayscaleA;
						nDataFormat = DataHalf;
						break;

					// RGBA16F
					case 113:
						nInternalColorFormat = nColorFormat = ColorRGBA;
						nDataFormat = DataHalf;
						break;

				// IEEE 32 bit float
					// R32F
					case 114:
						nInternalColorFormat = nColorFormat = ColorGrayscale;
						nDataFormat = DataFloat;
						break;

					// RG32F
					case 115:
						nInternalColorFormat = nColorFormat = ColorGrayscaleA;
						nDataFormat = DataFloat;
						break;

					// RGBA32F
					case 116:
						nInternalColorFormat = nColorFormat = ColorRGBA;
						nDataFormat = DataFloat;
						break;

				// Compressed
					// DXT1 (BC1 UNORM)
					case MCHAR4('D', 'X', 'T', '1'):
						nInternalColorFormat = nColorFormat = ColorRGB;
						nCompression = CompressionDXT1;
						break;

					// DXT3 (BC2 UNORM)
					case MCHAR4('D', 'X', 'T', '3'):
						nInternalColorFormat = nColorFormat = ColorRGBA;
						nCompression = CompressionDXT3;
						break;

					// DXT5 (BC3 UNORM)
					case MCHAR4('D', 'X', 'T', '5'):
						nInternalColorFormat = nColorFormat = ColorRGBA;
						nCompression = CompressionDXT5;
						break;

					// LATC1 (BC4 UNORM, previously known as ATI1N)
					case MCHAR4('A', 'T', 'I', '1'):
						nInternalColorFormat = nColorFormat = ColorGrayscale;
						nCompression = CompressionLATC1;
						break;

					// LATC2 (BC5 UNORM, previously known as ATI2N)
					case MCHAR4('A', 'T', 'I', '2'):
						nInternalColorFormat = nColorFormat = ColorGrayscaleA;
						nCompression = CompressionLATC2;
						break;

				// Uncompressed
					default:
						switch (sHeader.ddpfPixelFormat.nRGBBitCount) {
							// R8
							case 8:
								nInternalColorFormat = nColorFormat = ColorGrayscale;
								break;

							// LA8
							case 16:
								if (sHeader.ddpfPixelFormat.nRGBAlphaBitMask == 0xFF00)
									nInternalColorFormat = nColorFormat = ColorGrayscaleA;
								else {
									// Error: Unsupported format
									return false;
								}
								break;

							// RGB8
							case 24:
								nInternalColorFormat = (sHeader.ddpfPixelFormat.nBBitMask == 0xFF) ? ColorBGR : ColorRGB;
								nColorFormat         = ColorRGB;
								break;

							// RGBA8
							case 32:
								if (sHeader.ddpfPixelFormat.nRBitMask != 0x3FF00000) {
									nInternalColorFormat = (sHeader.ddpfPixelFormat.nBBitMask == 0xFF) ? ColorBGRA : ColorRGBA;
									nColorFormat         = ColorRGBA;
								} else {
									// Error: Unsupported format
									return false;
								}
								break;

							default:
								// Error: Unsupported format
								return false;
						}
				}
			}
		} else {
			// The image is not compressed
			if (sHeader.ddpfPixelFormat.nFlags & DDS_LUMINANCE) {
				if (sHeader.ddpfPixelFormat.nFlags & DDS_ALPHAPIXELS) {
					nInternalColorFormat = nColorFormat = ColorGrayscaleA;
				} else {
					nInternalColorFormat = nColorFormat = ColorGrayscale;
				}
			} else {
				if (sHeader.ddpfPixelFormat.nFlags & DDS_ALPHAPIXELS) {
					// Set color format, please not that all bit mask relevant stuff is done inside "DecompressRGBA()"
					nInternalColorFormat = nColorFormat = ColorRGBA;
				} else {
					// Set color format, please not that all bit mask relevant stuff is done inside "DecompressRGBA()"
					nInternalColorFormat = nColorFormat = ColorRGB;
				}
			}

			// Microsoft bug, they're not following their own documentation.
			if (!(sHeader.ddpfPixelFormat.nFlags & (DDS_LINEARSIZE | DDS_PITCH)) || !sHeader.nPitchOrLinearSize)
				sHeader.ddpfPixelFormat.nFlags |= DDS_LINEARSIZE;
		}

		// Get the number of mipmaps
		const uint32 nNumOfMipmaps = (!sHeader.nMipMapCount) ? 1 : sHeader.nMipMapCount;

		// Cube map?
		const uint32 nNumOfFaces = (sHeader.ddsCaps.nCaps2 & DDSCAPS2_CUBEMAP) ? 6 : 1;

		{ // Loop through all faces
			uint8 *pnTempData = nullptr;	// Used when "DDS_LINEARSIZE" is set
			for (uint32 nFace=0; nFace<nNumOfFaces; nFace++){
				// Create image part with reasonable semantic
				ImagePart *pImagePart = cImage.CreatePart((nNumOfFaces == 6) ? (static_cast<uint32>(ImagePartCubeSidePosX) + nFace) : 0);
				if (pImagePart) {
					// Load in all mipmaps
					for (uint32 nMipmap=0; nMipmap<nNumOfMipmaps; nMipmap++) {
						// Create image buffer
						ImageBuffer *pImageBuffer = pImagePart->CreateMipmap();
						pImageBuffer->CreateImage(nDataFormat,
												  nColorFormat,
												  Vector3i(ImageBuffer::GetMipmapSize(sHeader.nWidth,  nMipmap),
														   ImageBuffer::GetMipmapSize(sHeader.nHeight, nMipmap),
														   ImageBuffer::GetMipmapSize(nDepth,		   nMipmap)),
												  nCompression);

						// Read in compressed data?
						if (nCompression == CompressionNone) {
							// If the "DDS_LINEARSIZE" flag is set we need to do some more work...
							if (sHeader.ddpfPixelFormat.nFlags & DDS_LINEARSIZE) {
								// Calculate the current linear size
								const uint32 nPitchOrLinearSize = (pImageBuffer->GetSize().x * pImageBuffer->GetSize().y * nDepth * (sHeader.ddpfPixelFormat.nRGBBitCount >> 3));

								// Allocate temp data right now? (we can reuse it for the following smaller mipmaps)
								if (!pnTempData)
									pnTempData = new uint8[nPitchOrLinearSize];

								// Read the data
								cFile.Read(pnTempData, 1, nPitchOrLinearSize);

								// Decompress the image data
								DecompressRGBA(sHeader, *pImageBuffer, pnTempData);
							} else {
								// A simple one: Just read in the whole uncompressed data
								cFile.Read(pImageBuffer->GetData(), 1, pImageBuffer->GetDataSize());
							}
						} else {
							// A simple one: Just read in the whole compressed data
							cFile.Read(pImageBuffer->GetCompressedData(), 1, pImageBuffer->GetCompressedDataSize());
						}
					}
				}
			}

			// Cleanup temp data
			if (pnTempData)
				delete [] pnTempData;
		}

		// Convert BGR(A) to RGB(A)
		if (nInternalColorFormat == ColorBGR) {
			// Loop through all faces
			for (uint32 nFace=0; nFace<nNumOfFaces; nFace++){
				// Create image part with reasonable semantic
				ImagePart *pImagePart = cImage.GetPartBySemantics((nNumOfFaces == 6) ? (static_cast<uint32>(ImagePartCubeSidePosX) + nFace) : 0);
				if (pImagePart) {
					// Load in all mipmaps
					for (uint32 nMipmap=0; nMipmap<nNumOfMipmaps; nMipmap++) {
						// Get image buffer
						ImageBuffer *pImageBuffer = pImagePart->GetMipmap(nMipmap);

						// Swap R/B
						Color3::SwapRB(pImageBuffer->GetData(), pImageBuffer->GetNumOfPixels());
					}
				}
			}
		} else if (nInternalColorFormat == ColorBGRA) {
			// Loop through all faces
			for (uint32 nFace=0; nFace<nNumOfFaces; nFace++){
				// Create image part with reasonable semantic
				ImagePart *pImagePart = cImage.GetPartBySemantics((nNumOfFaces == 6) ? (static_cast<uint32>(ImagePartCubeSidePosX) + nFace) : 0);
				if (pImagePart) {
					// Load in all mipmaps
					for (uint32 nMipmap=0; nMipmap<nNumOfMipmaps; nMipmap++) {
						// Get image buffer
						ImageBuffer *pImageBuffer = pImagePart->GetMipmap(nMipmap);

						// Swap R/B
						Color4::SwapRB(pImageBuffer->GetData(), pImageBuffer->GetNumOfPixels());
					}
				}
			}
		}

		// Done
		return true;
	} else {
		// Error: Invalid magic number
	}

	#undef MCHAR4

	// Error!
	return false;
}

bool ImageLoaderDDS::Save(const Image &cImage, File &cFile)
{
	#define MCHAR4(a, b, c, d) (a | (b << 8) | (c << 16) | (d << 24))

	// Get the first image part
	const ImagePart *pImagePart = cImage.GetPart(0);
	if (pImagePart) {
		// Get the first image buffer
		const ImageBuffer *pImageBuffer = pImagePart->GetMipmap(0);
		if (pImageBuffer && pImageBuffer->GetBytesPerRow()) {
			// Get important image information
			const uint32	nNumOfMipmaps = pImagePart->GetNumOfMipmaps();
			const Vector3i &vSize		  = pImageBuffer->GetSize();

			// Set up the header
			DDSHeader sHeader;
			MemoryManager::Set(&sHeader, 0, sizeof(DDSHeader));
			DDSHeaderDX10 sHeaderDX10;
			MemoryManager::Set(&sHeaderDX10, 0, sizeof(DDSHeaderDX10));

			// Header
			sHeader.nMagic[0]			= 'D';
			sHeader.nMagic[1]			= 'D';
			sHeader.nMagic[2]			= 'S';
			sHeader.nMagic[3]			= ' ';
			sHeader.nSize				= 124;
			sHeader.nFlags				= DDSD_CAPS | DDSD_PIXELFORMAT | DDSD_WIDTH | DDSD_HEIGHT | ((nNumOfMipmaps > 1) ? DDSD_MIPMAPCOUNT : 0) | ((vSize.z > 1) ? DDSD_DEPTH : 0);
			sHeader.nHeight				= vSize.y;
			sHeader.nWidth				= vSize.x;
			sHeader.nPitchOrLinearSize	= 0;
			sHeader.nDepth				= (vSize.z > 1) ? vSize.z : 0;
			sHeader.nMipMapCount		= (nNumOfMipmaps > 1) ? nNumOfMipmaps : 0;

			// Check compression
			sHeader.ddpfPixelFormat.nSize = 32;
			switch (pImageBuffer->GetCompression()) {
				// No compression
				case CompressionNone:
					// [TODO] Implement DDS save without compression (there are multiple image data format and image color format variations)
					return false; // Error!

				// DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block)
				case CompressionDXT1:
					sHeader.ddpfPixelFormat.nFlags  = DDPF_FOURCC;
					sHeader.ddpfPixelFormat.nFourCC = MCHAR4('D','X','T','1');
					break;

				// DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
				case CompressionDXT3:
					sHeader.ddpfPixelFormat.nFlags  = DDPF_FOURCC;
					sHeader.ddpfPixelFormat.nFourCC = MCHAR4('D','X','T','3');
					break;

				// DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
				case CompressionDXT5:
					sHeader.ddpfPixelFormat.nFlags  = DDPF_FOURCC;
					sHeader.ddpfPixelFormat.nFourCC = MCHAR4('D','X','T','5');
					break;

				// 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block)
				case CompressionLATC1:
					sHeader.ddpfPixelFormat.nFlags  = DDPF_FOURCC;
					sHeader.ddpfPixelFormat.nFourCC = MCHAR4('A','T','I','1');
					break;

				// 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block)
				case CompressionLATC2:
					sHeader.ddpfPixelFormat.nFlags  = DDPF_FOURCC;
					sHeader.ddpfPixelFormat.nFourCC = MCHAR4('A','T','I','2');
					break;
			}

			// Header
			sHeader.ddsCaps.nCaps1			= DDSCAPS_TEXTURE | ((nNumOfMipmaps > 1) ? (DDSCAPS_MIPMAP | DDSCAPS_COMPLEX) : 0) | ((vSize.z != 1) ? DDSCAPS_COMPLEX : 0);
			sHeader.ddsCaps.nCaps2			= (vSize.z > 1) ? DDSCAPS2_VOLUME : ((vSize.z == 0) ? (DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_ALL_FACES) : 0);
			sHeader.ddsCaps.nReserved[0]	= 0;
			sHeader.ddsCaps.nReserved[1]	= 0;
			sHeader.nReserved2				= 0;

			// Write the header
			cFile.Write(&sHeader, 1, sizeof(DDSHeader));
			if (sHeaderDX10.nDXGIFormat)
				cFile.Write(&sHeaderDX10, 1, sizeof(DDSHeaderDX10));

			// Is it a cube map?
			if (cImage.GetNumOfParts() == 6) {
				// Save all faces
				for (uint32 nFace=0; nFace<6; nFace++){
					// Get image part
					pImagePart = cImage.GetPart(nFace);

					// Save in all mipmaps
					for (uint32 nMipmap=0; nMipmap<nNumOfMipmaps; nMipmap++) {
						// Get image buffer
						pImageBuffer = pImagePart->GetMipmap(nMipmap);

						// Write down the image data
						if (sHeader.ddpfPixelFormat.nFlags == DDPF_FOURCC)
							cFile.Write(pImageBuffer->GetCompressedData(), 1, pImageBuffer->GetCompressedDataSize());
						else
							cFile.Write(pImageBuffer->GetData(), 1, pImageBuffer->GetDataSize());
					}
				}
			} else {
				// In case there are multiple parts, but it's no cube map... just ignore the other image parts...

				// Save in all mipmaps
				for (uint32 nMipmap=0; nMipmap<nNumOfMipmaps; nMipmap++) {
					// Get image buffer
					pImageBuffer = pImagePart->GetMipmap(nMipmap);

					// Write down the image data
					if (sHeader.ddpfPixelFormat.nFlags == DDPF_FOURCC)
						cFile.Write(pImageBuffer->GetCompressedData(), 1, pImageBuffer->GetCompressedDataSize());
					else
						cFile.Write(pImageBuffer->GetData(), 1, pImageBuffer->GetDataSize());
				}
			}

			// Done
			return true;
		}
	}

	#undef MCHAR4

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
ImageLoaderDDS::ImageLoaderDDS()
{
}

/**
*  @brief
*    Destructor
*/
ImageLoaderDDS::~ImageLoaderDDS()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns bits from a given bit mask
*/
void ImageLoaderDDS::GetBitsFromMask(uint32 nMask, uint32 &nShiftLeft, uint32 &nShiftRight) const
{
	if (nMask) {
		uint32 nTemp = nMask;
		uint32 i;
		for (i=0; i<32; i++, nTemp >>= 1) {
			if (nTemp & 1)
				break;
		}
		nShiftRight = i;

		// Temp is preserved, so use it again:
		for (i=0; i<8; i++, nTemp >>= 1) {
			if (!(nTemp & 1))
				break;
		}
		nShiftLeft = 8 - i;
	} else {
		nShiftLeft = nShiftRight = 0;
	}
}

/**
*  @brief
*    Decompresses a RGB/RGBA image
*/
void ImageLoaderDDS::DecompressRGBA(const DDSHeader &sHeader, ImageBuffer &cImageBuffer, const uint8 *pnCompressedData) const
{
	// Get bits from mask
	uint32 nRedL, nRedR, nGreenL, nGreenR, nBlueL, nBlueR, nAlphaL, nAlphaR;
	GetBitsFromMask(sHeader.ddpfPixelFormat.nRBitMask,        nRedL,   nRedR);
	GetBitsFromMask(sHeader.ddpfPixelFormat.nGBitMask,        nGreenL, nGreenR);
	GetBitsFromMask(sHeader.ddpfPixelFormat.nBBitMask,        nBlueL,  nBlueR);
	GetBitsFromMask(sHeader.ddpfPixelFormat.nRGBAlphaBitMask, nAlphaL, nAlphaR);

	// Get data
	const uint32  nBpp             = sHeader.ddpfPixelFormat.nRGBBitCount / 8;
	const uint32  nDataSize        = cImageBuffer.GetDataSize();
	const uint32  nBytesPerPixel   = cImageBuffer.GetBytesPerPixel();
		  uint8  *pDestinationData = cImageBuffer.GetData();

	// Loop through all pixel
	for (uint32 i=0; i<nDataSize; i+=nBytesPerPixel) {
		uint32 nReadI = 0;
		if (nDataSize - i < 4) { // Less than 4 byte to write?
			if (nBpp == 3) {	// This branch is extra-SLOOOW
				nReadI =
					*pnCompressedData
					| ((*(pnCompressedData + 1)) << 8)
					| ((*(pnCompressedData + 2)) << 16);
			} else if (nBpp == 1) {
				nReadI = *pnCompressedData;
			} else if (nBpp == 2) {
				nReadI = pnCompressedData[0] | (pnCompressedData[1] << 8);
			}
		} else {
			nReadI = pnCompressedData[0] | (pnCompressedData[1] << 8) | (pnCompressedData[2] << 16) | (pnCompressedData[3] << 24);
		}
		pnCompressedData += nBpp;

		pDestinationData[i] = static_cast<uint8>(((nReadI & sHeader.ddpfPixelFormat.nRBitMask) >> nRedR) << nRedL);

		if (nBytesPerPixel >= 3) {
			pDestinationData[i+1] = static_cast<uint8>(((nReadI & sHeader.ddpfPixelFormat.nGBitMask) >> nGreenR) << nGreenL);
			pDestinationData[i+2] = static_cast<uint8>(((nReadI & sHeader.ddpfPixelFormat.nBBitMask) >> nBlueR) << nBlueL);

			if (nBytesPerPixel == 4) {
				pDestinationData[i+3] = static_cast<uint8>(((nReadI & sHeader.ddpfPixelFormat.nRGBAlphaBitMask) >> nAlphaR) << nAlphaL);
				if (nAlphaL >= 7) {
					pDestinationData[i+3] = pDestinationData[i+3] ? 0xFF : 0x00;
				} else if (nAlphaL >= 4) {
					pDestinationData[i+3] = pDestinationData[i+3] | (pDestinationData[i+3] >> 4);
				}
			}
		} else if (nBytesPerPixel == 2) {
			pDestinationData[i+1] = static_cast<uint8>(((nReadI & sHeader.ddpfPixelFormat.nRGBAlphaBitMask) >> nAlphaR) << nAlphaL);
			if (nAlphaL >= 7) {
				pDestinationData[i+1] = pDestinationData[i+1] ? 0xFF : 0x00;
			} else if (nAlphaL >= 4) {
				pDestinationData[i+1] = pDestinationData[i+1] | (pDestinationData[i+3] >> 4);
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics
