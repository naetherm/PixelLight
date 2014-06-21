/*********************************************************\
 *  File: TextureBuffer.cpp                              *
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
#include <PLMath/Half.h>
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImagePart.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include "PLRenderer/Renderer/Renderer.h"
#include "PLRenderer/Renderer/TextureBuffer1D.h"		// For the comfort-method "CopyDataToImage()"
#include "PLRenderer/Renderer/TextureBuffer2D.h"		// For the comfort-method "CopyDataToImage()"
#include "PLRenderer/Renderer/TextureBuffer2DArray.h"	// For the comfort-method "CopyDataToImage()"
#include "PLRenderer/Renderer/TextureBufferRectangle.h"	// For the comfort-method "CopyDataToImage()"
#include "PLRenderer/Renderer/TextureBuffer3D.h"		// For the comfort-method "CopyDataToImage()"
#include "PLRenderer/Renderer/TextureBufferCube.h"		// For the comfort-method "CopyDataToImage()"
#include "PLRenderer/Renderer/TextureBuffer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the texture buffer pixel format of a given image (PLGraphics::Image)
*/
TextureBuffer::EPixelFormat TextureBuffer::GetFormatFromImage(const Image &cImage, bool bNoCompression)
{
	// Get the first image buffer
	ImageBuffer *pImageBuffer = cImage.GetBuffer();
	if (pImageBuffer) {
		// Check for word format
		if (pImageBuffer->GetDataFormat() == DataWord) {
			switch (pImageBuffer->GetComponentsPerPixel()) {
				// Luminance
				case 1:
					return L16;

				// Luminance & alpha
				//   -> Not supported

				// RGB
				//   -> Not supported

				// RGBA
				case 4:
					return R16G16B16A16;
			}

		// Check for half format
		} else if (pImageBuffer->GetDataFormat() == DataHalf) {
			switch (pImageBuffer->GetComponentsPerPixel()) {
				// Luminance
				case 1:
					return L16F;

				// Luminance & alpha
				//   -> Not supported

				// RGB
				//   -> Not supported

				// RGBA
				case 4:
					return R16G16B16A16F;
			}

		// Check for float format
		} else if (pImageBuffer->GetDataFormat() == DataFloat) {
			switch (pImageBuffer->GetComponentsPerPixel()) {
				// Luminance
				case 1:
					return L32F;

				// Luminance & alpha
				//   -> Not supported

				// RGB
				//   -> Not supported

				// RGBA
				case 4:
					return R32G32B32A32F;
			}

		// Another format...
		} else {
			// Check for supported compressed/uncompressed
			ECompression nCompression = pImageBuffer->GetCompression();
			if (bNoCompression)
				nCompression = CompressionNone;
			if (nCompression == CompressionNone) {
				switch (pImageBuffer->GetComponentsPerPixel()) {
					// Luminance
					case 1:
						return L8;

					// Luminance & alpha
					case 2:
						return L8A8;

					// RGB
					case 3:
						return R8G8B8;

					// RGBA
					case 4:
						return R8G8B8A8;
				}
			} else {
				switch (nCompression) {
					case CompressionDXT1:
						return DXT1;

					case CompressionDXT3:
						return DXT3;

					case CompressionDXT5:
						return DXT5;

					case CompressionLATC1:
						return LATC1;

					case CompressionLATC2:
						return LATC2;

					case CompressionNone:
					default:
						// Nothing to do in here, CompressionNone is already handled above
						break;
				}
			}
		}
	}

	// Error!
	return Unknown;
}

/**
*  @brief
*    Returns whether or not the given texture buffer format is compressed
*/
bool TextureBuffer::IsCompressedFormat(EPixelFormat nFormat)
{
	switch (nFormat) {
		case L8:			return false;
		case L16:			return false;
		case A8:			return false;
		case L4A4:			return false;
		case L8A8:			return false;
		case D16:			return false;
		case D24:			return false;
		case D32:			return false;
		case R3G3B2:		return false;
		case R5G6B5:		return false;
		case R5G5B5A1:		return false;
		case R4G4B4A4:		return false;
		case R8G8B8:		return false;
		case R8G8B8A8:		return false;
		case R10G10B10A2:	return false;
		case R16G16B16A16:	return false;
		case DXT1:			return true;
		case DXT3:			return true;
		case DXT5:			return true;
		case LATC1:			return true;
		case LATC2:			return true;
		case L16F:			return false;
		case L32F:			return false;
		case R16G16B16A16F:	return false;
		case R32G32B32A32F:	return false;
		case Unknown:		return false; // Error!
		default:			return false; // Error!
	}
}

/**
*  @brief
*    Returns whether or not the given texture buffer format is a depth buffer format
*/
bool TextureBuffer::IsDepthFormat(EPixelFormat nFormat)
{
	switch (nFormat) {
		case D16:
		case D24:
		case D32:
			return true;

		case L8:
		case L16:
		case A8:
		case L4A4:
		case L8A8:
		case R3G3B2:
		case R5G6B5:
		case R5G5B5A1:
		case R4G4B4A4:
		case R8G8B8:
		case R8G8B8A8:
		case R10G10B10A2:
		case R16G16B16A16:
		case DXT1:
		case DXT3:
		case DXT5:
		case LATC1:
		case LATC2:
		case L16F:
		case L32F:
		case R16G16B16A16F:
		case R32G32B32A32F:
		case Unknown:
		default:
			return false;
	}
}

/**
*  @brief
*    If the given texture buffer format is compressed, a proper uncompressed texture buffer format will be returned
*/
TextureBuffer::EPixelFormat TextureBuffer::GetUncompressedFormat(EPixelFormat nFormat)
{
	// Is the given format compressed?
	if (IsCompressedFormat(nFormat)) {
		// Return a proper uncompressed format
		switch (nFormat) {
			case DXT1:
				return R8G8B8;

			case DXT3:
				return R8G8B8A8;

			case DXT5:
				return R8G8B8A8;

			case LATC1:
				return L8;

			case LATC2:
				return L8A8;

			case L8:
			case L16:
			case A8:
			case L4A4:
			case L8A8:
			case D16:
			case D24:
			case D32:
			case R3G3B2:
			case R5G6B5:
			case R5G5B5A1:
			case R4G4B4A4:
			case R8G8B8:
			case R8G8B8A8:
			case R10G10B10A2:
			case R16G16B16A16:
			case L16F:
			case L32F:
			case R16G16B16A16F:
			case R32G32B32A32F:
			case Unknown:
				break;
		}
	}

	// Pass through the given format
	return nFormat;
}

/**
*  @brief
*    Returns whether or not the given texture buffer format is a floating point format
*/
bool TextureBuffer::IsFloatingPointFormat(EPixelFormat nFormat)
{
	switch (nFormat) {
		case L8:			return false;
		case L16:			return false;
		case A8:			return false;
		case L4A4:			return false;
		case L8A8:			return false;
		case D16:			return false;
		case D24:			return false;
		case D32:			return false;
		case R3G3B2:		return false;
		case R5G6B5:		return false;
		case R5G5B5A1:		return false;
		case R4G4B4A4:		return false;
		case R8G8B8:		return false;
		case R8G8B8A8:		return false;
		case R10G10B10A2:	return false;
		case R16G16B16A16:	return false;
		case DXT1:			return false;
		case DXT3:			return false;
		case DXT5:			return false;
		case LATC1:			return false;
		case LATC2:			return false;
		case L16F:			return true;
		case L32F:			return true;
		case R16G16B16A16F:	return true;
		case R32G32B32A32F:	return true;
		case Unknown:		return false; // Error!
		default:			return false; // Error!
	}
}

/**
*  @brief
*    Returns the number of components per pixel
*/
uint32 TextureBuffer::GetComponentsPerPixel(EPixelFormat nFormat)
{
	switch (nFormat) {
		case L8:			return 1;
		case L16:			return 1;
		case A8:			return 1;
		case L4A4:			return 2;
		case L8A8:			return 2;
		case D16:			return 1;
		case D24:			return 1;
		case D32:			return 1;
		case R3G3B2:		return 3;
		case R5G6B5:		return 3;
		case R5G5B5A1:		return 4;
		case R4G4B4A4:		return 4;
		case R8G8B8:		return 3;
		case R8G8B8A8:		return 4;
		case R10G10B10A2:	return 4;
		case R16G16B16A16:	return 4;
		case DXT1:			return 3;
		case DXT3:			return 4;
		case DXT5:			return 4;
		case LATC1:			return 1;
		case LATC2:			return 2;
		case L16F:			return 1;
		case L32F:			return 1;
		case R16G16B16A16F:	return 4;
		case R32G32B32A32F:	return 4;
		case Unknown:		return 0; // Error!
		default:			return 0; // Error!
	}
}

/**
*  @brief
*    Returns the number of bytes per pixel component
*/
uint32 TextureBuffer::GetBytesPerPixelComponent(EPixelFormat nFormat)
{
	switch (nFormat) {
		case L8:			return 1;
		case L16:			return 2;
		case A8:			return 1;
		case L4A4:			return 0; // No full byte per component...
		case L8A8:			return 1;
		case D16:			return 2;
		case D24:			return 4; // Look out! 4 NOT 3 bytes!
		case D32:			return 4;
		case R3G3B2:		return 0; // No full byte per component...
		case R5G6B5:		return 0; // No full byte per component...
		case R5G5B5A1:		return 0; // No full byte per component...
		case R4G4B4A4:		return 0; // No full byte per component...
		case R8G8B8:		return 1;
		case R8G8B8A8:		return 1;
		case R10G10B10A2:	return 0; // No full byte per component...
		case R16G16B16A16:	return 2;
		case DXT1:			return 0; // No full byte per component...
		case DXT3:			return 0; // No full byte per component...
		case DXT5:			return 0; // No full byte per component...
		case LATC1:			return 0; // No full byte per component...
		case LATC2:			return 0; // No full byte per component...
		case L16F:			return 2;
		case L32F:			return 4;
		case R16G16B16A16F:	return 2;
		case R32G32B32A32F:	return 4;
		case Unknown:		return 0; // Error!
		default:			return 0; // Error!
	}
}

/**
*  @brief
*    Returns the number of bytes per pixel
*/
uint32 TextureBuffer::GetBytesPerPixel(EPixelFormat nFormat)
{
	switch (nFormat) {
		case L8:			return 1;
		case L16:			return 2;
		case A8:			return 1;
		case L4A4:			return 1;
		case L8A8:			return 2;
		case D16:			return 2;
		case D24:			return 4; // Look out! 4 NOT 3 bytes!
		case D32:			return 4;
		case R3G3B2:		return 1;
		case R5G6B5:		return 2;
		case R5G5B5A1:		return 2;
		case R4G4B4A4:		return 2;
		case R8G8B8:		return 3;
		case R8G8B8A8:		return 4;
		case R10G10B10A2:	return 4;
		case R16G16B16A16:	return 8;
		case DXT1:			return 0; // No full byte per pixel...
		case DXT3:			return 0; // No full byte per pixel...
		case DXT5:			return 0; // No full byte per pixel...
		case LATC1:			return 0; // No full byte per pixel...
		case LATC2:			return 0; // No full byte per pixel...
		case L16F:			return 2;
		case L32F:			return 4;
		case R16G16B16A16F:	return 8;
		case R32G32B32A32F:	return 16;
		case Unknown:		return 0; // Error!
		default:			return 0; // Error!
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
TextureBuffer::~TextureBuffer()
{
	// Check if this is the renderers current texture buffer
	for (uint32 nStage=0; nStage<GetRenderer().GetCapabilities().nMaxTextureUnits; nStage++) {
		if (GetRenderer().GetTextureBuffer(nStage) == this)
			GetRenderer().SetTextureBuffer(nStage);
	}
}

/**
*  @brief
*    Returns the texture buffer size as a 3D vector
*/
Vector3i TextureBuffer::GetUniformSize(uint32 nMipmap) const
{
	// Get the image size
	Vector3i vSize;
	switch (GetType()) {
		case TypeTextureBuffer1D:
			vSize.x = reinterpret_cast<const TextureBuffer1D*>(this)->GetSize(nMipmap);
			vSize.y = vSize.z = 1;
			break;

		case TypeTextureBuffer2D:
			vSize.SetXYZ(reinterpret_cast<const TextureBuffer2D*>(this)->GetSize(nMipmap), 1);
			break;

		case TypeTextureBuffer2DArray:
			vSize = reinterpret_cast<const TextureBuffer2DArray*>(this)->GetSize(nMipmap);
			break;

		case TypeTextureBufferRectangle:
			vSize.SetXYZ(reinterpret_cast<const TextureBufferRectangle*>(this)->GetSize(), 1);	// Has no mipmaps
			break;

		case TypeTextureBuffer3D:
			vSize = reinterpret_cast<const TextureBuffer3D*>(this)->GetSize(nMipmap);
			break;

		case TypeTextureBufferCube:
			vSize.x = vSize.y = reinterpret_cast<const TextureBufferCube*>(this)->GetSize(nMipmap);
			vSize.z = 1;
			break;

		case TypeIndexBuffer:
		case TypeVertexBuffer:
		case TypeUniformBuffer:
		case TypeOcclusionQuery:
		case TypeVertexShader:
		case TypeTessellationControlShader:
		case TypeTessellationEvaluationShader:
		case TypeGeometryShader:
		case TypeFragmentShader:
		case TypeProgram:
		case TypeFont:
		default:
			break;	// Error! This methods only works for texture buffers...
	}

	// Return the size
	return vSize;
}

/**
*  @brief
*    Returns the total number of pixels including all mipmaps
*/
uint32 TextureBuffer::GetTotalNumOfPixels() const
{
	// The total number of pixels
	uint32 nTotalNumOfPixels = 0;

	// Sum up the pixels of all mipmaps
	for (uint32 i=0; i<m_nNumOfMipmaps; i++)
		nTotalNumOfPixels += GetNumOfPixels(i);

	// Return the total number of pixels
	return nTotalNumOfPixels;
}

/**
*  @brief
*    Returns the image (PLGraphics::Image) settings required to be able to store the texture buffer data within an image
*/
bool TextureBuffer::GetFormatForImage(EDataFormat &nDataFormat, EColorFormat &nColorFormat, ECompression &nCompression, TextureBuffer::EPixelFormat &nTextureBufferFomat) const
{
	// Set known default values
	nDataFormat			= DataByte;
	nColorFormat		= ColorRGB;
	nCompression		= CompressionNone;
	nTextureBufferFomat	= m_nFormat;

	// Perform the mapping
	switch (m_nFormat) {
		// 8-bit pixel format, all bits luminance
		case L8:
			nDataFormat  = DataByte;		// Byte (8 bit)
			nColorFormat = ColorGrayscale;	// Gray scale image
			break;

		// 16-bit pixel format, all bits luminance
		case L16:
			nDataFormat  = DataWord;		// Word (16 bit)
			nColorFormat = ColorGrayscale;	// Gray scale image
			break;

		// 8-bit pixel format, all bits alpha
		case A8:
			// Trade "A8" the same way as "L8"
			nDataFormat  = DataByte;		// Byte (8 bit)
			nColorFormat = ColorGrayscale;	// Gray scale image
			break;

		// 8-bit pixel format, 4 bits for luminance and alpha
		case L4A4:
			// Trade "L4A4" the same way as "L8A8"
			nDataFormat			= DataByte;			// Byte (8 bit)
			nColorFormat		= ColorGrayscaleA;	// Gray scale image with alpha
			nTextureBufferFomat	= L8A8;				// We need a 8 bits data format
			break;

		// 16-bit pixel format, 8 bits for luminance and alpha
		case L8A8:
			nDataFormat  = DataByte;		// Byte (8 bit)
			nColorFormat = ColorGrayscaleA;	// Gray scale image with alpha
			break;

		// 16-bit z-buffer bit depth
		case D16:
			nDataFormat  = DataWord;		// Word (16 bit)
			nColorFormat = ColorGrayscale;	// Gray scale image
			break;

		// 32-bit z-buffer bit depth using 24 bits for the depth channel
		case D24:
			// There's no double word (32 bit) "PLGraphics::DataDoubleWord" data format, so we have to truncate to 16 bit
			nDataFormat			= DataWord;			// Word (16 bit)
			nColorFormat		= ColorGrayscale;	// Gray scale image
			nTextureBufferFomat	= D16;				// We need a 16 bits data format
			break;

		// 32-bit z-buffer bit depth
		case D32:
			// There's no double word (32 bit) "PLGraphics::DataDoubleWord" data format, so we have to truncate to 16 bit
			nDataFormat			= DataWord;			// Word (16 bit)
			nColorFormat		= ColorGrayscale;	// Gray scale image
			nTextureBufferFomat	= D16;				// We need a 16 bits data format
			break;

		// 8-bit pixel format, 3 bits red, 3 bits green and 2 bits blue
		case R3G3B2:
			// Trade "R3G3B2" the same way as "R8G8B8"
			nDataFormat			= DataByte;	// Byte (8 bit)
			nColorFormat		= ColorRGB;	// RGB
			nTextureBufferFomat	= R8G8B8;	// We need a 8 bits data format
			break;

		// 16-bit pixel format, 5 bits red, 6 bits green and 5 bits blue
		case R5G6B5:
			// Trade "R5G6B5" the same way as "R8G8B8"
			nDataFormat			= DataByte;	// Byte (8 bit)
			nColorFormat		= ColorRGB;	// RGB
			nTextureBufferFomat	= R8G8B8;	// We need a 8 bits data format
			break;

		// 16-bit pixel format, 5 bits red, 5 bits green, 5 bits blue and 1 bits alpha
		case R5G5B5A1:
			// Trade "R5G5B5A1" the same way as "R8G8B8A8"
			nDataFormat			= DataByte;		// Byte (8 bit)
			nColorFormat		= ColorRGBA;	// RGBA
			nTextureBufferFomat	= R8G8B8A8;		// We need a 8 bits data format
			break;

		// 16-bit pixel format, 4 bits for red, green, blue and alpha
		case R4G4B4A4:
			// Trade "R4G4B4A4" the same way as "R8G8B8A8"
			nDataFormat			= DataByte;		// Byte (8 bit)
			nColorFormat		= ColorRGBA;	// RGBA
			nTextureBufferFomat	= R8G8B8A8;		// We need a 8 bits data format
			break;

		// 24-bit (or 32-bit if 24-bits are not supported by the hardware) pixel format, 8 bits for red, green and blue
		case R8G8B8:
			nDataFormat  = DataByte;	// Byte (8 bit)
			nColorFormat = ColorRGB;	// RGB
			break;

		// 32-bit pixel format, 8 bits for red, green, blue and alpha
		case R8G8B8A8:
			nDataFormat  = DataByte;	// Byte (8 bit)
			nColorFormat = ColorRGBA;	// RGBA
			break;

		// 32-bit pixel format, 10 bits for red, green, blue and 2 bits for alpha
		case R10G10B10A2:
			// Trade "R10G10B10A2" the same way as "R16G16B16A16"
			nDataFormat			= DataWord;		// Word (16 bit)
			nColorFormat		= ColorRGBA;	// RGBA
			nTextureBufferFomat	= R16G16B16A16;	// We need a 16 bits data format
			break;

		// 64-bit pixel format, 16 bits for red, green, blue and alpha
		case R16G16B16A16:
			nDataFormat  = DataWord;	// Word (16 bit)
			nColorFormat = ColorRGBA;	// RGBA
			break;

		// DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block)
		case DXT1:
			nDataFormat  = DataByte;		// Byte (8 bit)
			nColorFormat = ColorRGB;		// RGB
			nCompression = CompressionDXT1;	// DXT1
			break;

		// DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
		case DXT3:
			nDataFormat  = DataByte;		// Byte (8 bit)
			nColorFormat = ColorRGBA;		// RGBA
			nCompression = CompressionDXT3;	// DXT3
			break;

		// DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
		case DXT5:
			nDataFormat  = DataByte;		// Byte (8 bit)
			nColorFormat = ColorRGBA;		// RGBA
			nCompression = CompressionDXT5;	// DXT5
			break;

		// 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block)
		case LATC1:
			nDataFormat  = DataByte;			// Byte (8 bit)
			nColorFormat = ColorGrayscale;		// Gray scale image
			nCompression = CompressionLATC1;	// LATC1
			break;

		// 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block)
		case LATC2:
			nDataFormat  = DataByte;			// Byte (8 bit)
			nColorFormat = ColorGrayscaleA;		// Gray scale image with alpha
			nCompression = CompressionLATC2;	// LATC2
			break;

		// 16-bit float format using 16 bits for luminance
		case L16F:
			nDataFormat  = DataHalf;		// Half (16 bit float)
			nColorFormat = ColorGrayscale;	// Gray scale image
			break;

		// 32-bit float format using 32 bits for luminance
		case L32F:
			nDataFormat  = DataFloat;		// IEEE float (32 bit)
			nColorFormat = ColorGrayscale;	// Gray scale image
			break;

		// 64-bit float format using 16 bits for the each channel (red, green, blue, alpha)
		case R16G16B16A16F:
			nDataFormat  = DataHalf;	// Half (16 bit float)
			nColorFormat = ColorRGBA;	// RGBA
			break;

		// 128-bit float format using 32 bits for the each channel (red, green, blue, alpha)
		case R32G32B32A32F:
			nDataFormat  = DataFloat;	// IEEE float (32 bit)
			nColorFormat = ColorRGBA;	// RGBA
			break;

		case Unknown:
		default:
			return false; // Error!
	}

	// Done
	return true;
}

/**
*  @brief
*    Returns the texture buffer data as image
*/
bool TextureBuffer::CopyDataToImage(Image &cImage, bool bMipmaps) const
{
	// Map the internal texture buffer format as close as possible to image data format and image color format
	EDataFormat					nDataFormat			= DataByte;
	EColorFormat				nColorFormat		= ColorRGB;
	ECompression				nCompression		= CompressionNone;
	TextureBuffer::EPixelFormat	nTextureBufferFomat	= m_nFormat;
	if (GetFormatForImage(nDataFormat, nColorFormat, nCompression, nTextureBufferFomat)) {
		// Get the number of faces
		const uint8 nNumOfFaces = (GetType() == TypeTextureBufferCube) ? 6 : 1;

		// Clear the given image
		cImage.Unload();

		// Loop through all faces
		EImagePart nImagePartSemantic = (nNumOfFaces > 1) ? ImagePartCubeSidePosX : ImagePartStatic;
		for (uint8 nFace=0; nFace<nNumOfFaces; nFace++, nImagePartSemantic=static_cast<EImagePart>(static_cast<uint32>(nImagePartSemantic) + 1)) {
			// Create the image part
			ImagePart *pImagePart = cImage.CreatePart(nImagePartSemantic);

			// Loop through all mipmaps
			const uint32 nNumOfMipmaps = bMipmaps ? m_nNumOfMipmaps : 0;
			for (uint32 nMipmap=0; nMipmap<=nNumOfMipmaps; nMipmap++) {
				// Create and allocate the image buffer instance holding this mipmap
				ImageBuffer *pImageBuffer = pImagePart->CreateMipmap();
				pImageBuffer->CreateImage(nDataFormat, nColorFormat, GetUniformSize(nMipmap), nCompression);

				// Download the mipmap texture buffer data from the GPU
				if (!CopyDataTo(nMipmap, nTextureBufferFomat, (nCompression == CompressionNone) ? pImageBuffer->GetData() : pImageBuffer->GetCompressedData(), nFace))
					return false;	// Error!
			}
		}

		// Done
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Downloads the texture buffer content and returns the number of NAN values in it
*/
uint32 TextureBuffer::GetNumOfNANValues(uint32 nMipmap, uint8 nFace) const
{
	// The number of found NANs
	uint32 nNumOfNANs = 0;

	// The texture buffer format must be a floating point one
	if (IsFloatingPointFormat()) {
		// Get the number of pixels on the given mipmap level for one face
		const uint32 nNumOfPixels = GetNumOfPixels(nMipmap)/GetNumOfFaces();
		if (nNumOfPixels != 0) {
			// Get the number of elements
			const uint32 nNumOfElements = nNumOfPixels*GetComponentsPerPixel();

			// Get the number of bytes per pixel component
			const uint32 nBytesPerPixelComponent = GetBytesPerPixelComponent();

			// 32 bit floating point
			if (nBytesPerPixelComponent == 4) {
				// Allocate host side texture buffer
				float *pfData = new float[nNumOfElements];

				// Download the texture buffer from the GPU to the host
				if (CopyDataTo(nMipmap, m_nFormat, pfData, nFace)) {
					// Count the number of NANs
					for (float *pfCurrent=pfData, *pfEnd=pfData+nNumOfElements; pfCurrent<pfEnd; pfCurrent++) {
						// Is this value a NAN?
						if (Math::IsNotANumber(*pfCurrent))
							nNumOfNANs++;
					}
				}

				// Cleanup
				delete [] pfData;
			} else if (nBytesPerPixelComponent == 2) {
				// 16 bit floating point format (half)

				// Allocate host side texture buffer
				uint16 *pnData = new uint16[nNumOfElements];

				// Download the texture buffer from the GPU to the host
				if (CopyDataTo(nMipmap, m_nFormat, pnData, nFace)) {
					// Count the number of NANs
					for (uint16 *pnCurrent=pnData, *pnEnd=pnData+nNumOfElements; pnCurrent<pnEnd; pnCurrent++) {
						// Is this value a NAN?
						if (Half::IsNotANumber(*pnCurrent))
							nNumOfNANs++;
					}
				}

				// Cleanup
				delete [] pnData;
			}
		}
	}

	// Return the number of found NANs
	return nNumOfNANs;
}

/**
*  @brief
*    Downloads the texture buffer content, replaces pixels containing a NAN value by a given color, and uploads the fixed texture buffer
*/
uint32 TextureBuffer::FixNANValues(const Color4 &cColor, uint32 nMipmap, uint8 nFace)
{
	// The number of found NAN pixels
	uint32 nNumOfNANs = 0;

	// The texture buffer format must be a floating point one
	if (IsFloatingPointFormat()) {
		// Get the number of pixels on the given mipmap level for one face
		const uint32 nNumOfPixels = GetNumOfPixels(nMipmap)/GetNumOfFaces();
		if (nNumOfPixels != 0) {
			// Get the number of components per pixel
			const uint32 nComponentsPerPixel = GetComponentsPerPixel();

			// Get the number of elements
			const uint32 nNumOfElements = nNumOfPixels*nComponentsPerPixel;

			// Get the number of bytes per pixel component
			const uint32 nBytesPerPixelComponent = GetBytesPerPixelComponent();

			// 32 bit floating point
			if (nBytesPerPixelComponent == 4) {
				// Allocate host side texture buffer
				float *pfData = new float[nNumOfElements];

				// Download the texture buffer from the GPU to the host
				if (CopyDataTo(nMipmap, m_nFormat, pfData, nFace)) {
					// Count the number of NANs
					for (float *pfCurrent=pfData, *pfEnd=pfData+nNumOfElements; pfCurrent<pfEnd; pfCurrent+=nComponentsPerPixel) {
						// Loop through the components of the current pixel
						float *pfCurrentPixel = pfCurrent;
						for (uint32 nComponent=0; nComponent<nComponentsPerPixel; nComponent++) {
							// Is this value a NAN?
							if (Math::IsNotANumber(pfCurrentPixel[nComponent])) {
								// Write down the given color
								*pfCurrentPixel = cColor.r;
								if (nComponentsPerPixel > 1)
									pfCurrentPixel[1] = cColor.g;
								else if (nComponentsPerPixel > 2)
									pfCurrentPixel[2] = cColor.b;
								else if (nComponentsPerPixel > 3)
									pfCurrentPixel[3] = cColor.a;

								// Update the number of NAN pixels
								nNumOfNANs++;

								// Get us out of the pixel loop
								nComponent = nComponentsPerPixel;
							}
						}
					}
				}

				// Upload the fixed texture buffer to the GPU
				CopyDataFrom(nMipmap, m_nFormat, pfData, nFace);

				// Cleanup
				delete [] pfData;
			} else if (nBytesPerPixelComponent == 2) {
				// 16 bit floating point format (half)

				// Convert the given color into half
//				const uint16 nHalfColor[4] = {Half::FromFloat(cColor.r), Half::FromFloat(cColor.g), Half::FromFloat(cColor.b), Half::FromFloat(cColor.a)};
				// [TODO] Internal, Half::FromFloat() is not implemented yet and returns always 0
				const uint16 nHalfColor[4] = {Half::One, Half::Zero, Half::Zero, Half::One};

				// Allocate host side texture buffer
				uint16 *pnData = new uint16[nNumOfElements];

				// Download the texture buffer from the GPU to the host
				if (CopyDataTo(nMipmap, m_nFormat, pnData, nFace)) {
					// Count the number of NANs
					for (uint16 *pnCurrent=pnData, *pnEnd=pnData+nNumOfElements; pnCurrent<pnEnd; pnCurrent+=nComponentsPerPixel) {
						// Loop through the components of the current pixel
						uint16 *pnCurrentPixel = pnCurrent;
						for (uint32 nComponent=0; nComponent<nComponentsPerPixel; nComponent++) {
							// Is this value a NAN?
							if (Half::IsNotANumber(pnCurrentPixel[nComponent])) {
								// Write down the given color
								*pnCurrentPixel = nHalfColor[0];
								if (nComponentsPerPixel > 1)
									pnCurrentPixel[1] = nHalfColor[1];
								if (nComponentsPerPixel > 2)
									pnCurrentPixel[2] = nHalfColor[2];
								if (nComponentsPerPixel > 3)
									pnCurrentPixel[3] = nHalfColor[3];

								// Update the number of NAN pixels
								nNumOfNANs++;

								// Get us out of the components loop
								nComponent = nComponentsPerPixel;
							}
						}
					}
				}

				// Upload the fixed texture buffer to the GPU
				CopyDataFrom(nMipmap, m_nFormat, pnData, nFace);

				// Cleanup
				delete [] pnData;
			}
		}
	}

	// Return the number of found NAN pixels
	return nNumOfNANs;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
TextureBuffer::TextureBuffer(Renderer &cRenderer, EType nType, uint32 nFlags) : Resource(cRenderer, nType),
	m_nFlags(nFlags),
	m_nFormat(Unknown),
	m_nNumOfElements(0),
	m_nNumOfMipmaps(0),
	m_nTotalNumOfBytes(0)
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
TextureBuffer::TextureBuffer(const TextureBuffer &cSource) : Resource(cSource.GetRenderer(), cSource.GetType()),
	m_nFlags(cSource.m_nFlags),
	m_nFormat(cSource.m_nFormat),
	m_nNumOfElements(cSource.m_nNumOfElements),
	m_nNumOfMipmaps(cSource.m_nNumOfMipmaps),
	m_nTotalNumOfBytes(cSource.m_nTotalNumOfBytes)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
TextureBuffer &TextureBuffer::operator =(const TextureBuffer &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
