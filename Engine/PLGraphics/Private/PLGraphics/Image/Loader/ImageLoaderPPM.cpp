/*********************************************************\
 *  File: ImageLoaderPPM.cpp                             *
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
#include <PLCore/String/Tokenizer.h>
#include "PLGraphics/Image/Image.h"
#include "PLGraphics/Image/ImagePart.h"
#include "PLGraphics/Image/ImageBuffer.h"
#include "PLGraphics/Image/Loader/ImageLoaderPPM.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLGraphics {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ImageLoaderPPM)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool ImageLoaderPPM::Load(Image &cImage, File &cFile)
{
	// Load in the whole file data
	const uint32 nFileSize = cFile.GetSize();
	char *pFileData = new char[nFileSize];
	cFile.Read(pFileData, 1, nFileSize);

	// Get the read data as a 'String' for the tokenizer - but don't copy the data!
	const String sStringData = String(pFileData, false, nFileSize);

	// Setup tokenizer
	Tokenizer cTokenizer;
	cTokenizer.Start(sStringData);
	cTokenizer.SetSingleLineComment("#");

	// Read the "magic number", we only support "P3"
	const String sMagicNumber = cTokenizer.GetNextToken();
	if (sMagicNumber == "P3") {
		// Get the image width
		const int nWidth = cTokenizer.GetNextToken().GetInt();
		if (nWidth > 0) {
			// Get the image height
			const int nHeight = cTokenizer.GetNextToken().GetInt();
			if (nHeight > 0) {
				// Get the maximum color value, we only support 1 byte per color component
				const int nMaxColorValue = cTokenizer.GetNextToken().GetInt();
				if (nMaxColorValue < 256 && nMaxColorValue > 0) {
					// Create image buffer
					ImageBuffer *pImageBuffer = cImage.CreatePart()->CreateMipmap();
					pImageBuffer->CreateImage(DataByte, ColorRGB, Vector3i(nWidth, nHeight, 1));

					// Loop through all integers (within error checks...)
					uint8 *pImageData = pImageBuffer->GetData();
					for (const uint8 *pImageDataEnd=pImageData+pImageBuffer->GetDataSize(); pImageData<pImageDataEnd; pImageData++)
						*pImageData = cTokenizer.GetNextToken().GetChar();

					// Done
					return true;
				} else {
					// Error: Unsupported color value
				}
			} else {
				// Error: Invalid height
			}
		} else {
			// Error: Invalid width
		}
	} else {
		// Error: Unsupported magic number
	}

	// Error!
	return false;
}

bool ImageLoaderPPM::Save(const Image &cImage, File &cFile)
{
	// Get the image buffer
	ImageBuffer *pImageBuffer = cImage.GetBuffer();
	if (pImageBuffer && pImageBuffer->GetBytesPerRow()) {
		// We only support 1 byte per pixel component
		if (pImageBuffer->GetBytesPerPixelComponent() == 1) {
			// We only support RGB as color format
			if (pImageBuffer->GetColorFormat() == ColorRGB) {
				// Write the "magic number"
				cFile.PrintLn("P3");

				// Write the image width and height
				cFile.PrintLn(String::Format("%d %d", pImageBuffer->GetSize().x, pImageBuffer->GetSize().y));

				// Write the maximum color value
				cFile.PrintLn("255");

				// Save all pixels
				const uint8 *pData = pImageBuffer->GetData();
				for (const uint8 *pDataEnd=pData+pImageBuffer->GetDataSize(); pData<pDataEnd; pData+=3) {
					// Write red, green and blue
					cFile.Print(String::Format("%d %d %d ", pData[0], pData[1], pData[2]));
				}

				// Done
				return true;
			} else {
				// Error: Unsupported color format
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
ImageLoaderPPM::ImageLoaderPPM()
{
}

/**
*  @brief
*    Destructor
*/
ImageLoaderPPM::~ImageLoaderPPM()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics
