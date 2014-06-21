/*********************************************************\
 *  File: TextureCreatorHorizonMap3D.cpp                 *
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
#include "PLRenderer/Texture/Creator/TextureCreatorHorizonMap3D.h"


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
pl_implement_class(TextureCreatorHorizonMap3D)


//[-------------------------------------------------------]
//[ Internal helper functions                             ]
//[-------------------------------------------------------]
#define ROUND(x) static_cast<int>(x + 0.5f)
inline uint8 Pack2(float x)
{
	return static_cast<uint8>(255.0f*x);
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
TextureCreatorHorizonMap3D::TextureCreatorHorizonMap3D() :
	Filename2D(this),
	XSize(this),
	YSize(this),
	ZSize(this),
	Height(this)
{
}

/**
*  @brief
*    Destructor
*/
TextureCreatorHorizonMap3D::~TextureCreatorHorizonMap3D()
{
}


//[-------------------------------------------------------]
//[ Private virtual TextureCreator functions              ]
//[-------------------------------------------------------]
Texture *TextureCreatorHorizonMap3D::Create(TextureManager &cTextureManager, Texture *pTexture) const
{
	// Try to load the 2D ASCII texture
	Image cImage2D;
	if (cImage2D.LoadByFilename(Filename2D)) {
		// Get the image buffer
		ImageBuffer *pImageBuffer = cImage2D.GetBuffer();
		if (pImageBuffer) {
			const uint8 *src = pImageBuffer->GetData();
			const uint32 w = pImageBuffer->GetSize().x;
			const uint32 h = pImageBuffer->GetSize().y;

			// Create the texture
			pTexture = CreateTexture(cTextureManager, pTexture);

			// Create the image
			Image cImage = Image::CreateImage(DataByte, ColorGrayscale, Vector3i(XSize, YSize, ZSize));
			pImageBuffer = cImage.GetBuffer();

			// Create the buffer
			uint8 *pBuffer = pImageBuffer->GetData();
			uint8 *pDest = pBuffer;
			for (uint32 z=0; z<ZSize; z++) {
				float xv = Math::Cos(static_cast<float>(z*Math::Pi2/ZSize));
				float yv = Math::Sin(static_cast<float>(z*Math::Pi2/ZSize));

				const float invMax = 1.0f/Math::Max(Math::Abs(xv), Math::Abs(yv));
				xv *= invMax;
				yv *= invMax;

				const float len = Math::Sqrt(xv*xv + yv*yv);

				for (uint32 y=0; y<YSize; y++) {
					for (uint32 x=0; x<XSize; x++) {
						float maxAng = 0;

						float xp = static_cast<float>(x*w)/XSize;
						float yp = static_cast<float>(y*h)/YSize;

						const float bh = static_cast<float>(src[static_cast<int>(yp)*w + static_cast<int>(xp)]);

						float dist = 0;

						for (uint32 i=0; i<256; i++) {
							xp += xv;
							yp += yv;
							dist += len;

							int xt = ROUND(xp);
							int yt = ROUND(yp);

							while (xt < 0)
								xt += w;
							while (yt < 0)
								yt += h;

							xt %= w;
							yt %= h;

							const float ang = (static_cast<float>(src[yt*w + xt]) - bh)/dist;
							if (ang > maxAng)
								maxAng = ang;
						}

						*pDest++ = Pack2(Math::Sin(Math::ATan(Height*maxAng)));
					}
				}
			}

			// Create the 3D texture buffer
			pTexture->SetTextureBuffer(reinterpret_cast<TextureBuffer*>(cTextureManager.GetRendererContext().GetRenderer().CreateTextureBuffer3D(cImage)));

			// Return the created texture
			return pTexture;
		}
	} else {
		PL_LOG(Error, "Can't open: " + Filename2D.GetString())
	}

	// Error!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
