/*********************************************************\
 *  File: TextureCreatorASCII3D.cpp                      *
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
#include <PLCore/Log/Log.h>
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include "PLRenderer/RendererContext.h"
#include "PLRenderer/Texture/TextureManager.h"
#include "PLRenderer/Texture/Creator/TextureCreatorASCII3D.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(TextureCreatorASCII3D)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
TextureCreatorASCII3D::TextureCreatorASCII3D() :
	ASCIIFilename(this)
{
}

/**
*  @brief
*    Destructor
*/
TextureCreatorASCII3D::~TextureCreatorASCII3D()
{
}


//[-------------------------------------------------------]
//[ Private virtual TextureCreator functions              ]
//[-------------------------------------------------------]
Texture *TextureCreatorASCII3D::Create(TextureManager &cTextureManager, Texture *pTexture) const
{
	// Try to load the 2D ASCII texture
	Image cImage2D;
	if (cImage2D.LoadByFilename(ASCIIFilename)) {
		// Get the image buffer
		ImageBuffer *p2DImageBuffer = cImage2D.GetBuffer();
		if (p2DImageBuffer) {
			// Create the texture
			pTexture = CreateTexture(cTextureManager, pTexture);

			// Create the image
			Image cImage = Image::CreateImage(DataByte, ColorGrayscale, Vector3i(16, 16, 256));
			ImageBuffer *pImageBuffer = cImage.GetBuffer();

			const uint8 *src = p2DImageBuffer->GetData();
			float avg[256];
			for (uint32 j=0; j<16; j++) {
				for (uint32 i=0; i<16; i++) {
					avg[j*16 + i] = 0;

					uint32 base = j*256*16 + i*16;
					for (uint32 b=0; b<16; b++) {
						for (uint32 a=0; a<16; a++)
							avg[j*16 + i] += static_cast<float>(src[base + 256*b + a]);
					}
					avg[j*16 + i] *= 1.0f/(255.0f*16*16);
				}
			}

			uint8 *pixels = pImageBuffer->GetData();
			uint8 *dest = pixels;

			for (uint32 j=0; j<256; j++) {
				const float av = static_cast<float>(j)/255.0f;

				uint32 index = 0;
				float minDiff = 1e10f;
				for (uint32 i=0; i<256; i++) {
					const float d = Math::Abs(avg[i] - av);
					if (d < minDiff) {
						minDiff = d;
						index = i;
					}
				}

				const uint32 r    = index/16;
				const uint32 c    = index%16;
				const uint32 base = r*256*16 + c*16;
				for (uint32 b=0; b<16; b++) {
					for (uint32 a=0; a<16; a++)
						*dest++ = src[base + 256*b + a];
				}
			}

			// Create the 3D texture buffer
			pTexture->SetTextureBuffer(reinterpret_cast<TextureBuffer*>(cTextureManager.GetRendererContext().GetRenderer().CreateTextureBuffer3D(cImage)));
		}

		// Return the created texture
		return pTexture;
	} else {
		PL_LOG(Error, "Can't open: " + ASCIIFilename.GetString())
	}

	// Error!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
