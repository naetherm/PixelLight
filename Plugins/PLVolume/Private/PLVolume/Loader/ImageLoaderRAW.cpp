/*********************************************************\
 *  File: ImageLoaderRAW.cpp                             *
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
#include <PLMath/Vector3i.h>
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImagePart.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include "PLVolume/Loader/ImageLoaderRAW.h"


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
pl_implement_class(ImageLoaderRAW)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool ImageLoaderRAW::LoadParams(Image &cImage, File &cFile, uint8 nDataType, uint16 nWidth, uint16 nHeight, uint16 nDepth)
{
	// Create image buffer
	ImageBuffer *pImageBuffer = cImage.CreatePart()->CreateMipmap();
	pImageBuffer->CreateImage(static_cast<EDataFormat>(nDataType), ColorGrayscale, Vector3i(nWidth, nHeight, nDepth));

	// Read the data
	cFile.Read(pImageBuffer->GetData(), 1, pImageBuffer->GetDataSize());

	// Done
	return true;
}

bool ImageLoaderRAW::Save(const Image &cImage, File &cFile)
{
	// Get image buffer
	ImageBuffer *pImageBuffer = cImage.GetBuffer();
	if (pImageBuffer) {
		// Save the data
		cFile.Write(pImageBuffer->GetData(), 1, pImageBuffer->GetDataSize());

		// Done
		return true;
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
ImageLoaderRAW::ImageLoaderRAW()
{
}

/**
*  @brief
*    Destructor
*/
ImageLoaderRAW::~ImageLoaderRAW()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume
