/*********************************************************\
 *  File: ImageLoaderJPG.cpp                             *
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
#include <PLCore/File/StdIo.h> // Required for "jpg/jpeglib.h"
extern "C" {
	#include <jpeglib.h>
}
#include <PLCore/File/File.h>
#include "PLGraphics/Image/Image.h"
#include "PLGraphics/Image/ImagePart.h"
#include "PLGraphics/Image/ImageBuffer.h"
#include "PLGraphics/Image/Loader/+Jpg/ImageLoaderJPG.h"


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_begin_class(ImageLoaderJPG, PLGraphics)
	pl_base_class(PLGraphics::ImageLoader)
	pl_desc("Image loader implementation for JPEG (\"Joint Photographic Experts Group\") file formats")
	pl_ctor()
	pl_str_tag("Formats", "jpg,jpeg,jpe,jif,jfif,jfi,JPG,JPEG,JPE,JIF,JFIF,JFI")
	pl_tag("Load", true)
	pl_tag("Save", true)

	pl_method(Load)
		pl_desc("Load method, no fancy upsampling and no interblock smoothing is applied")

	pl_method(LoadParams)
		pl_desc("Load method. Parameters: First 'bool' parameter determines whether or not fancy upsampling is applied, second 'bool' parameter whether or not interblock smoothing is applied.")

	pl_method(Save)
		pl_desc("Save method, using 100% quality setting")

	pl_method(SaveParams)
		pl_desc("Save method, quality (0...100) as first parameter")

pl_end_class()


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLGraphics {


//[-------------------------------------------------------]
//[ Ugly global stuff for reading                         ]
//[-------------------------------------------------------]
typedef struct {
	struct jpeg_source_mgr  sPublic;		/**< Public fields */
	File				   *pFile;			/**< PixelLight file object */
	JOCTET				   *pBuffer;		/**< Start of buffer */
	bool					bStartOfFile;	/**< Have we gotten any data yet? */
} JpgReadStruct;
const uint32 InputBufferSize = 4096;
// [TODO] Error handling?
//static bool jpgErrorOccurred = false;

// Overrides libjpeg's stupid error/warning handlers. =P
void ExitErrorHandle (struct jpeg_common_struct *JpegInfo)
{
	// jpgErrorOccurred = true;
	return;
}

void init_source(j_decompress_ptr cinfo)
{
	// Cast to JpgReadStruct
	JpgReadStruct *pJpgReadStruct = reinterpret_cast<JpgReadStruct*>(cinfo->src);

	// Initialize
	pJpgReadStruct->bStartOfFile = true;
}

boolean fill_input_buffer(j_decompress_ptr cinfo)
{
	// Cast to JpgReadStruct
	JpgReadStruct *pJpgReadStruct = reinterpret_cast<JpgReadStruct*>(cinfo->src);

	// Read bytes from file
	const uint32 nReadBytes = pJpgReadStruct->pFile->Read(pJpgReadStruct->pBuffer, 1, InputBufferSize);
	if (nReadBytes) {
		if (nReadBytes < InputBufferSize) {
			// Nothing to do
		}

		pJpgReadStruct->sPublic.next_input_byte = pJpgReadStruct->pBuffer;
		pJpgReadStruct->sPublic.bytes_in_buffer = nReadBytes;
		pJpgReadStruct->bStartOfFile			= false;

		return TRUE;
	} else {
		if (pJpgReadStruct->bStartOfFile) {
			// Treat empty input file as fatal error
//			jpgErrorOccurred = true;
		}

		// Insert a fake EOI marker
		pJpgReadStruct->pBuffer[0] = static_cast<JOCTET>(0xFF);
		pJpgReadStruct->pBuffer[1] = static_cast<JOCTET>(JPEG_EOI);

		return FALSE;
	}
}

void skip_input_data(j_decompress_ptr cinfo, long num_bytes)
{
	// Cast to JpgReadStruct
	JpgReadStruct *pJpgReadStruct = reinterpret_cast<JpgReadStruct*>(cinfo->src);

	// Skip
	if (num_bytes > 0) {
		while (num_bytes > static_cast<long>(pJpgReadStruct->sPublic.bytes_in_buffer)) {
			num_bytes -= static_cast<long>(pJpgReadStruct->sPublic.bytes_in_buffer);
			fill_input_buffer(cinfo);
		}
		pJpgReadStruct->sPublic.next_input_byte += static_cast<size_t>(num_bytes);
		pJpgReadStruct->sPublic.bytes_in_buffer -= static_cast<size_t>(num_bytes);
	}
}

void term_source(j_decompress_ptr cinfo)
{
	// Nothing to do in here
}

void jpeg_read_init(j_decompress_ptr cinfo, File *pFile)
{
	// Get to JpgReadStruct
	JpgReadStruct *pJpgReadStruct = reinterpret_cast<JpgReadStruct*>(cinfo->src);
	if (!pJpgReadStruct) {
		// First time for this JPEG object
		cinfo->src = static_cast<struct jpeg_source_mgr*>((*cinfo->mem->alloc_small)(reinterpret_cast<j_common_ptr>(cinfo), JPOOL_PERMANENT, sizeof(JpgReadStruct)));
		pJpgReadStruct = reinterpret_cast<JpgReadStruct*>(cinfo->src);
		pJpgReadStruct->pFile   = pFile;
		pJpgReadStruct->pBuffer = static_cast<JOCTET*>((*cinfo->mem->alloc_small)(reinterpret_cast<j_common_ptr>(cinfo), JPOOL_PERMANENT, InputBufferSize*sizeof(JOCTET)));
	}

	// Setup
	pJpgReadStruct->sPublic.init_source			= init_source;
	pJpgReadStruct->sPublic.fill_input_buffer	= fill_input_buffer;
	pJpgReadStruct->sPublic.skip_input_data		= skip_input_data;
	pJpgReadStruct->sPublic.resync_to_restart	= jpeg_resync_to_restart; // Use default method
	pJpgReadStruct->sPublic.term_source			= term_source;
	pJpgReadStruct->sPublic.bytes_in_buffer		= 0;					  // Forces fill_input_buffer on first read
	pJpgReadStruct->sPublic.next_input_byte		= nullptr;				  // Until buffer loaded
}


//[-------------------------------------------------------]
//[ Ugly global stuff for writing                         ]
//[-------------------------------------------------------]
typedef struct
{
	struct jpeg_destination_mgr	 sPublic;	/**< Public fields */
	File						*pFile;		/**< PixelLight file object */
	JOCTET						*pBuffer;	/**< Start of buffer */
} JpgWriteStruct;
const uint32 OutputBufferSize = 4096;

void init_destination(j_compress_ptr cinfo)
{
	// Cast to JpgWriteStruct
	JpgWriteStruct *pJpgWriteStruct = reinterpret_cast<JpgWriteStruct*>(cinfo->dest);

	// Setup
	pJpgWriteStruct->pBuffer				  = static_cast<JOCTET*>((*cinfo->mem->alloc_small)(reinterpret_cast<j_common_ptr>(cinfo), JPOOL_IMAGE, OutputBufferSize*sizeof(JOCTET)));
	pJpgWriteStruct->sPublic.next_output_byte = pJpgWriteStruct->pBuffer;
	pJpgWriteStruct->sPublic.free_in_buffer	  = OutputBufferSize;
}

boolean empty_output_buffer(j_compress_ptr cinfo)
{
	// Cast to JpgWriteStruct
	JpgWriteStruct *pJpgWriteStruct = reinterpret_cast<JpgWriteStruct*>(cinfo->dest);

	// Write
	pJpgWriteStruct->pFile->Write(pJpgWriteStruct->pBuffer, 1, OutputBufferSize);
	pJpgWriteStruct->sPublic.next_output_byte = pJpgWriteStruct->pBuffer;
	pJpgWriteStruct->sPublic.free_in_buffer   = OutputBufferSize;

	return TRUE;
}

void term_destination(j_compress_ptr cinfo)
{
	// Cast to JpgWriteStruct
	JpgWriteStruct *pJpgWriteStruct = reinterpret_cast<JpgWriteStruct*>(cinfo->dest);

	// Write
	pJpgWriteStruct->pFile->Write(pJpgWriteStruct->pBuffer, 1, OutputBufferSize - pJpgWriteStruct->sPublic.free_in_buffer);
}

void jpeg_write_init(j_compress_ptr cinfo, File *pFile)
{
	// Get to JpgWriteStruct
	JpgWriteStruct *pJpgWriteStruct = reinterpret_cast<JpgWriteStruct*>(cinfo->dest);
	if (!pJpgWriteStruct) {
		// First time for this JPEG object
		cinfo->dest = static_cast<struct jpeg_destination_mgr*>((*cinfo->mem->alloc_small)(reinterpret_cast<j_common_ptr>(cinfo), JPOOL_PERMANENT, sizeof(JpgWriteStruct)));
		pJpgWriteStruct = reinterpret_cast<JpgWriteStruct*>(cinfo->dest);
		pJpgWriteStruct->pFile = pFile;
	}

	// Setup
	pJpgWriteStruct->sPublic.init_destination	 = init_destination;
	pJpgWriteStruct->sPublic.empty_output_buffer = empty_output_buffer;
	pJpgWriteStruct->sPublic.term_destination	 = term_destination;
}


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool ImageLoaderJPG::Load(Image &cImage, File &cFile)
{
	return LoadParams(cImage, cFile, false, false);
}

bool ImageLoaderJPG::LoadParams(Image &cImage, File &cFile, bool bBlockSmoothing, bool bFancyUpsampling)
{
	jpeg_decompress_struct sInfo;
	jpeg_error_mgr sError;

	sInfo.err = jpeg_std_error(&sError);
	sInfo.err->error_exit = ExitErrorHandle;

	jpeg_create_decompress(&sInfo);

	// Set the user given parameters
	sInfo.do_block_smoothing  = bBlockSmoothing ? TRUE : FALSE;
	sInfo.do_fancy_upsampling = bFancyUpsampling ? TRUE : FALSE;

	jpeg_read_init(&sInfo, &cFile);

	jpeg_read_header(&sInfo, TRUE);
	jpeg_start_decompress(&sInfo);

	// Get the color format
	EColorFormat nColorFormat;
	switch (sInfo.num_components){
		case 1:
			nColorFormat = ColorGrayscale;
			break;

		case 3:
			nColorFormat = ColorRGB;
			break;

		case 4:
			nColorFormat = ColorRGBA;
			break;

		default:
			// Error: Unsupported color format
			return false;
	}

	// Create image buffer
	ImageBuffer *pImageBuffer = cImage.CreatePart()->CreateMipmap();
	pImageBuffer->CreateImage(DataByte, nColorFormat, Vector3i(sInfo.output_width, sInfo.output_height, 1));

	// Read in the data
	uint8 *pCurrentData = pImageBuffer->GetData();
	while (sInfo.output_scanline < sInfo.output_height){
		jpeg_read_scanlines(&sInfo, &pCurrentData, 1);
		pCurrentData += pImageBuffer->GetBytesPerRow();
	}

	// Cleanup
	jpeg_finish_decompress(&sInfo);
	jpeg_destroy_decompress(&sInfo);

	// Done
	return true;
}

bool ImageLoaderJPG::Save(const Image &cImage, File &cFile)
{
	return SaveParams(cImage, cFile, 100);
}

bool ImageLoaderJPG::SaveParams(const Image &cImage, File &cFile, uint32 nQuality)
{
	// Get the image buffer
	ImageBuffer *pImageBuffer = cImage.GetBuffer();
	if (pImageBuffer && pImageBuffer->GetBytesPerRow()) {
		// We only support 1 or 3 byte per pixel component
		if (pImageBuffer->GetBytesPerPixelComponent() == 1 || pImageBuffer->GetBytesPerPixelComponent() == 3) {
			jpeg_compress_struct sInfo;
			jpeg_error_mgr sError;

			sInfo.err = jpeg_std_error(&sError);
			sInfo.err->error_exit = ExitErrorHandle;
			jpeg_create_compress(&sInfo);

			const int nComponents = pImageBuffer->GetComponentsPerPixel();

			sInfo.in_color_space = (nComponents == 1)? JCS_GRAYSCALE : JCS_RGB;
			jpeg_set_defaults(&sInfo);

			sInfo.input_components	= nComponents;
			sInfo.num_components	= (nComponents == 1) ? 1 : 3;
			sInfo.image_width		= pImageBuffer->GetSize().x;
			sInfo.image_height		= pImageBuffer->GetSize().y;
			sInfo.data_precision	= 8;
			sInfo.input_gamma		= 1.0;

			// Set the user given parameter
			jpeg_set_quality(&sInfo, nQuality, FALSE);

			jpeg_write_init(&sInfo, &cFile);
			jpeg_start_compress(&sInfo, TRUE);

			// Is the input image RGBA? If so, we really need to throw away the alpha channel...
			if (nComponents == 4) {
				// Allocate memory for an converted output row
				uint8 *pOutputRow = new uint8[sInfo.image_width*sInfo.num_components];

				// Write the data
				const uint8 *pCurrentImageData = pImageBuffer->GetData();
				for (uint32 y=0; y<sInfo.image_height; y++) {
					// Convert the current row
					for (uint32 x=0; x<sInfo.image_width; x++) {
						const uint8 *pnPixelIn = &pCurrentImageData[x*4];
						uint8 *pnPixelOut = &pOutputRow[x*3];
						pnPixelOut[0] = pnPixelIn[0];
						pnPixelOut[1] = pnPixelIn[1];
						pnPixelOut[2] = pnPixelIn[2];
					}

					// Write out the current row
					jpeg_write_scanlines(&sInfo, &pOutputRow, 1);

					// Next, please
					pCurrentImageData += pImageBuffer->GetBytesPerRow();
				}

				// Free the allocated output row memory
				delete [] pOutputRow;
			} else {
				// Write the data
				uint8 *pCurrentImageData = pImageBuffer->GetData();
				for (uint32 y=0; y<sInfo.image_height; y++) {
					jpeg_write_scanlines(&sInfo, &pCurrentImageData, 1);
					pCurrentImageData += pImageBuffer->GetBytesPerRow();
				}
			}

			// Cleanup
			jpeg_finish_compress(&sInfo);
			jpeg_destroy_compress(&sInfo);

			// Done
			return true;
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
ImageLoaderJPG::ImageLoaderJPG()
{
}

/**
*  @brief
*    Destructor
*/
ImageLoaderJPG::~ImageLoaderJPG()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics
