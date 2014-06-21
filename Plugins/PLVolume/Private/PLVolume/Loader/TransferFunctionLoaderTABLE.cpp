/*********************************************************\
 *  File: TransferFunctionLoaderTABLE.cpp                *
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
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImagePart.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include "PLVolume/TransferFunction.h"
#include "PLVolume/Loader/TransferFunctionLoaderTABLE.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLVolume {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(TransferFunctionLoaderTABLE)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool TransferFunctionLoaderTABLE::Load(TransferFunction &cTransferFunction, File &cFile)
{
	// Get the image holding the transfer function
	Image &cImage = cTransferFunction.GetImage();

	// A "table"-file (simple ASCII) looks like this
	/*
		1 1 1 1
		baseOpacity: 0.698039
		NoTags
		0 0 0 0
		. . . .
		. . . .
		. . . .
	*/

	// Use the tokenizer in order to gather all required information, ignore the rest
	// Startup the tokenizer
	Tokenizer cTokenizer;
	cTokenizer.Start(cFile.GetContentAsString());

	// Ignore the first line ("1 1 1 1")
	cTokenizer.GetNextToken();
	cTokenizer.GetNextToken();
	cTokenizer.GetNextToken();
	cTokenizer.GetNextToken();

	// Ignore base opacity ("baseOpacity: 0.698039")
	cTokenizer.GetNextToken();
	cTokenizer.GetNextToken();

	// Ignore tags to keep this loader simple ("NoTags")
	cTokenizer.GetNextToken();

	// Create image buffer
	ImageBuffer *pImageBuffer = cImage.CreatePart()->CreateMipmap();
	pImageBuffer->CreateImage(DataByte, ColorRGBA, Vector3i(256, 1, 1));

	// Read in the palette
	uint8 *pImageData = pImageBuffer->GetData();
	for (uint32 i=0; i<256; i++) {
		// Read RGBA entry
		*pImageData = cTokenizer.GetNextToken().GetUInt8();
		pImageData++;
		*pImageData = cTokenizer.GetNextToken().GetUInt8();
		pImageData++;
		*pImageData = cTokenizer.GetNextToken().GetUInt8();
		pImageData++;
		*pImageData = cTokenizer.GetNextToken().GetUInt8();
		pImageData++;
	}

	// Done
	return true;
}

bool TransferFunctionLoaderTABLE::Save(const TransferFunction &cTransferFunction, File &cFile)
{
	// Not implemented

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
TransferFunctionLoaderTABLE::TransferFunctionLoaderTABLE()
{
}

/**
*  @brief
*    Destructor
*/
TransferFunctionLoaderTABLE::~TransferFunctionLoaderTABLE()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume
