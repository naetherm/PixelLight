/*********************************************************\
 *  File: ImageLoaderPNG.cpp                             *
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
#include <png.h>
#include <PLCore/File/File.h>
#include <PLCore/File/Url.h>
#include "PLGraphics/Image/Image.h"
#include "PLGraphics/Image/ImagePart.h"
#include "PLGraphics/Image/ImageBuffer.h"
#include "PLGraphics/Image/ImagePalette.h"
#include "PLGraphics/Image/Loader/ImageLoaderPNG.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLGraphics {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(ImageLoaderPNG, "PLGraphics", PLGraphics::ImageLoader, "Image loader implementation for PNG file formats")
	// Properties
	pl_properties
		pl_property("Formats",	"png,PNG")
		pl_property("Load",		"1")
		pl_property("Save",		"1")
	pl_properties_end
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Methods
	pl_method_2_metadata(Load,	pl_ret_type(bool),	Image&,			PLCore::File&,	"Load method",	"")
	pl_method_2_metadata(Save,	pl_ret_type(bool),	const Image&,	PLCore::File&,	"Save method",	"")
pl_class_metadata_end(ImageLoaderPNG)


//[-------------------------------------------------------]
//[ Static helper functions                               ]
//[-------------------------------------------------------]
void PngError(png_structp pPtr, png_const_charp pszError)
{
}

void PngWarning(png_structp pPtr, png_const_charp pszWarning)
{
}

void PngReadData(png_structp pPng, png_bytep pBuffer, png_size_t nSize)
{
	// Get file pointer
	File *pFile = static_cast<File*>(png_get_io_ptr(pPng));

	// Read data from file
	pFile->Read(pBuffer, nSize, 1);
}

void PngWriteData(png_structp pPng, png_bytep pBuffer, png_size_t nSize)
{
	// Get file pointer
	File *pFile = static_cast<File*>(png_get_io_ptr(pPng));

	// Write data to file
	pFile->Write(pBuffer, nSize, 1);
}

void PngFlushData(png_structp pPng)
{
	// Get file pointer
	File *pFile = static_cast<File*>(png_get_io_ptr(pPng));

	// Flush the file buffer
	pFile->Flush();
}

png_voidp PngMalloc(png_structp pPng, png_size_t nSize)
{
	return MemoryManager::Allocator(MemoryManager::NewArray, nSize);
}

void PngFree(png_structp pPng, png_voidp pBuffer)
{
	MemoryManager::Deallocator(MemoryManager::DeleteArray, pBuffer);
}


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool ImageLoaderPNG::Load(Image &cImage, File &cFile)
{
	// Initialize libpng
	png_structp pPng = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, PngError, PngWarning);
	if (pPng) {
		// Create PNG info struct
		png_infop pPngInfo = png_create_info_struct(pPng);
		if (pPngInfo) {
			// Set read function to use PL file functions
			png_set_read_fn(pPng, &cFile, PngReadData);

			// Load image headers
			png_read_info(pPng, pPngInfo);

			// Get image info
			png_byte	nBitDepth	= png_get_bit_depth	  (pPng, pPngInfo);
			png_byte	nColorType	= png_get_color_type  (pPng, pPngInfo);
			png_uint_32 nWidth		= png_get_image_width (pPng, pPngInfo);
			png_uint_32 nHeight		= png_get_image_height(pPng, pPngInfo);

			// Get data format
			EDataFormat nDataFormat = DataByte;
			unsigned char nBytes = 0;
			switch (nBitDepth) {
				case 1:
				case 2:
				case 4:
					// 1, 2 or 4 Bit (Unsupported, we will use 8 Bit instead)
					nDataFormat = DataByte;
					nBytes		= 1;

					// Convert 1, 2 and 4 Bit grayscale images to 8 Bit
					png_set_expand_gray_1_2_4_to_8(pPng);
					break;

				case 8:
					// 8 Bit (Color/Palette)
					nDataFormat = DataByte;
					nBytes		= 1;
					break;

				case 16:
					// 16 Bit (Color)
					nDataFormat = DataWord;
					nBytes		= 2;
					break;
			}
			if (nBytes != 0) {
				// Get color format
				EColorFormat nColorFormat = ColorRGB;
				unsigned char nComponents = 0;
				switch (nColorType) {
					case PNG_COLOR_TYPE_GRAY:			// (Bit depths 1, 2, 4, 8, 16)
						// Gray scale image
						nColorFormat = ColorGrayscale;
						nComponents  = 1;

						// We want grayscale in direct values, no palette please
						png_set_palette_to_rgb(pPng);
						break;

					case PNG_COLOR_TYPE_GRAY_ALPHA:		// (Bit depths 8, 16)
						// Gray scale image with additional alpha channel
						nColorFormat = ColorGrayscaleA;
						nComponents  = 2;
						break;

					case PNG_COLOR_TYPE_PALETTE:		// (Bit depths 1, 2, 4, 8)
						// Color palette (indexed colors)
						nColorFormat = ColorPalette;
						nComponents  = 1;
						break;

					case PNG_COLOR_TYPE_RGB:			// (Bit depths 8, 16)
						// RGB colors
						nColorFormat = ColorRGB;
						nComponents  = 3;
						break;

					case PNG_COLOR_TYPE_RGB_ALPHA:		// (Bit depths 8, 16)
						// RGB colors and alpha channel
						nColorFormat = ColorRGBA;
						nComponents  = 4;
						break;

					default:
						// Whatever this is, we don't support it at the moment ;-)
						break;
				}
				if (nComponents != 0) {
					// Convert transparency info into RGBA, but only if we have RGB/RGBA, not indexed colors
					if (nColorFormat == ColorRGB || nColorFormat == ColorRGBA)
						png_set_tRNS_to_alpha(pPng);

					// Create image buffer
					ImageBuffer *pImageBuffer = cImage.CreatePart()->CreateMipmap();
					pImageBuffer->CreateImage(nDataFormat, nColorFormat, Vector3i(nWidth, nHeight, 1));

					// Get pointer to image buffer
					uint8 *pBuffer = pImageBuffer->GetData();

					// Make array with pointers to each individual row inside the image buffer
					uint32		nBytesPerRow	= nBytes * nComponents * nWidth;
					png_bytepp	ppRowPointers	= new png_bytep[nHeight];
					for (uint32 i=0; i<nHeight; i++)
						ppRowPointers[i] = &pBuffer[i*nBytesPerRow];

					// Read image
					png_read_image(pPng, ppRowPointers);

					// Read palette
					if (nColorFormat == ColorPalette) {
						// Get color palette
						png_colorp pPLTE;
						int nColors;
						if (png_get_PLTE(pPng, pPngInfo, &pPLTE, &nColors) == PNG_INFO_PLTE) {
							// Create palette
							ImagePalette *pPalette = new ImagePalette();
							pPalette->Create(nColors);

							// Copy palette data
							MemoryManager::Copy(pPalette->GetData(), pPLTE, nColors*3);

							// Set palette
							pImageBuffer->SetPalette(pPalette);
						}
					}

					// Clean up
					png_read_end(pPng, nullptr);
					png_destroy_read_struct(&pPng, &pPngInfo, nullptr);
					delete [] ppRowPointers;

					// Done
					return true;
				}
			}

			// Error: Unsupported data or color format
			return false;
		} else {
			// Error: Could not create png_info structure
			png_destroy_read_struct(&pPng, nullptr, nullptr);
			return false;
		}
	} else {
		// Error: Could not create png_struct structure
		return false;
	}
}

bool ImageLoaderPNG::Save(const Image &cImage, File &cFile)
{
	// Get the image buffer
	ImageBuffer *pImageBuffer = cImage.GetBuffer();
	if (pImageBuffer && pImageBuffer->GetBytesPerRow()) {
		// Get the type
		int nType = -1;
		switch (pImageBuffer->GetColorFormat()) {
			case ColorRGB:
				if (pImageBuffer->GetDataFormat() == DataByte  || pImageBuffer->GetDataFormat() == DataWord)
					nType = PNG_COLOR_TYPE_RGB;
				break;

			case ColorRGBA:
				if (pImageBuffer->GetDataFormat() == DataByte  || pImageBuffer->GetDataFormat() == DataWord)
					nType = PNG_COLOR_TYPE_RGBA;
				break;

			case ColorBGR:
				// [TODO]
				break;

			case ColorBGRA:
				// [TODO]
				break;

			case ColorGrayscale:
				if (pImageBuffer->GetDataFormat() == DataByte  || pImageBuffer->GetDataFormat() == DataWord)
					nType = PNG_COLOR_TYPE_GRAY;
				break;

			case ColorGrayscaleA:
				if (pImageBuffer->GetDataFormat() == DataByte  || pImageBuffer->GetDataFormat() == DataWord)
					nType = PNG_COLOR_TYPE_GRAY_ALPHA;
				break;

			case ColorPalette:
				// [TODO]
				break;
		}

		// Type found?
		if (nType != -1) {
			// Initialize libpng
			png_structp pPng = png_create_write_struct_2(PNG_LIBPNG_VER_STRING, nullptr, PngError, PngWarning, nullptr, PngMalloc, PngFree);
			if (pPng) {
				// Create PNG info struct
				png_infop pPngInfo = png_create_info_struct(pPng);
				if (pPngInfo) {
					// Set write function to use PL file functions
					png_set_write_fn(pPng, &cFile, PngWriteData, PngFlushData);

					// Get the height of the image
					const uint32 nHeight = pImageBuffer->GetSize().y;

					// Setup
					png_set_IHDR(pPng, pPngInfo, pImageBuffer->GetSize().x, nHeight, pImageBuffer->GetBytesPerPixelComponent()*8,
								 nType, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
					png_write_info(pPng, pPngInfo);

					// Get the data pointer and the number of bytes per row
					const uint8 *pnData		  = pImageBuffer->GetData();
					const uint32 nBytesPerRow = pImageBuffer->GetBytesPerRow();

					// Do we need to take care of endian?
					if (pImageBuffer->GetBytesPerPixelComponent() > 1) {
						// [TODO] An endian helper class would be nice :D
					} else {
						// Write down all rows
						const uint8 *pnCurrentData = pnData;
						for (uint32 nY=0; nY<nHeight; nY++, pnCurrentData+=nBytesPerRow)
							png_write_row(pPng, reinterpret_cast<png_bytep>(const_cast<png_bytep>(pnCurrentData)));
					}
				}

				// Cleanup
				png_write_end(pPng, pPngInfo);
				png_destroy_write_struct(&pPng, &pPngInfo);

				// Done
				return true;
			}
		} else {
			// Error: Unsupported color or data format
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
ImageLoaderPNG::ImageLoaderPNG()
{
}

/**
*  @brief
*    Destructor
*/
ImageLoaderPNG::~ImageLoaderPNG()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics
