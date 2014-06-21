/*********************************************************\
 *  File: ImageEffect.cpp                                *
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
#include <PLCore/Base/Class.h>
#include "PLGraphics/Image/Image.h"
#include "PLGraphics/Image/ImagePart.h"
#include "PLGraphics/Image/ImageBuffer.h"
#include "PLGraphics/Image/ImageEffect.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLGraphics {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ImageEffect)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ImageEffect::ImageEffect()
{
}

/**
*  @brief
*    Destructor
*/
ImageEffect::~ImageEffect()
{
}

/**
*  @brief
*    Get effect description
*/
String ImageEffect::GetDescription() const
{
	// [TODO] New RTTI usage: Remove this method?
	const PLCore::Class *pClass = GetClass();
	return (pClass != nullptr) ? pClass->GetDescription() : "";
}


//[-------------------------------------------------------]
//[ Public virtual ImageEffect functions                  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Apply effect to image
*/
bool ImageEffect::Apply(Image &cImage) const
{
	// Usually, applying an image effect to an image means to apply it to every part
	if (cImage.GetNumOfParts() > 0) {
		// Apply to all parts
		bool bResult = true;
		for (uint32 i=0; i<cImage.GetNumOfParts(); i++) {
			// Get part
			ImagePart *pPart = cImage.GetPart(i);
			if (pPart) {
				// Apply effect
				if (!Apply(*pPart))
					bResult = false;
			}
		}

		// Return result
		return bResult;
	}

	// No parts available
	return false;
}

/**
*  @brief
*    Apply effect to image part
*/
bool ImageEffect::Apply(ImagePart &cImagePart) const
{
	// Usually, applying an image effect to a part means to apply it to the first image buffer and rebuild mipmaps
	ImageBuffer *pBuffer = cImagePart.GetMipmap(0);
	if (pBuffer) {
		// Apply effect
		bool bResult = Apply(*pBuffer);

		// Rebuild mipmaps
		if (cImagePart.GetNumOfMipmaps() > 1)
			cImagePart.BuildMipmaps();

		// Return result
		return bResult;
	}

	// No image buffers available
	return false;
}

/**
*  @brief
*    Apply effect to image buffer
*/
bool ImageEffect::Apply(ImageBuffer &cImageBuffer) const
{
	// Not implemented
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics
