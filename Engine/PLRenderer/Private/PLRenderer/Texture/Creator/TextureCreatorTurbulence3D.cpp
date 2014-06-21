/*********************************************************\
 *  File: TextureCreatorTurbulence3D.cpp                 *
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
#include <PLMath/PerlinNoiseTurbulence.h>
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include "PLRenderer/RendererContext.h"
#include "PLRenderer/Texture/TextureManager.h"
#include "PLRenderer/Texture/Creator/TextureCreatorTurbulence3D.h"


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
pl_implement_class(TextureCreatorTurbulence3D)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
TextureCreatorTurbulence3D::TextureCreatorTurbulence3D() :
	XSize(this),
	YSize(this),
	ZSize(this),
	Scale(this)
{
}

/**
*  @brief
*    Destructor
*/
TextureCreatorTurbulence3D::~TextureCreatorTurbulence3D()
{
}


//[-------------------------------------------------------]
//[ Private virtual TextureCreator functions              ]
//[-------------------------------------------------------]
Texture *TextureCreatorTurbulence3D::Create(TextureManager &cTextureManager, Texture *pTexture) const
{
	// Create the texture
	pTexture = CreateTexture(cTextureManager, pTexture);

	// Get scale
	const Vector3 &vScale = Scale.Get();

	// Create the image
	Image cImage = Image::CreateImage(DataByte, ColorGrayscale, Vector3i(XSize, YSize, ZSize));
	ImageBuffer *pImageBuffer = cImage.GetBuffer();

	// Create the buffer
	uint8 *pTurbBuffer = pImageBuffer->GetData();
	uint8 *pDest = pTurbBuffer;
	uint32 nMin = 255, nMax = 0;
	for (uint32 nZ=0; nZ<ZSize; nZ++) {
		for (uint32 nY=0; nY<YSize; nY++) {
			for (uint32 nX=0; nX<XSize; nX++) {
				const uint8 nT = static_cast<uint8>(127.5f*(1 + PerlinNoiseTurbulence::TileableTurbulence3(vScale.x*nX, vScale.y*nY, vScale.z*nZ, XSize*vScale.x, YSize*vScale.y, ZSize*vScale.z, 16)));
				if (nT > nMax)
					nMax = nT;
				if (nT < nMin)
					nMin = nT;
				*pDest++ = nT;
			}
		}
	}
	const uint32 nNumOfPixels = XSize*YSize*ZSize;
	pDest = pTurbBuffer;
	for (uint32 i=0; i<nNumOfPixels; i++, pDest++)
		*pDest = static_cast<uint8>((255*(*pDest - nMin))/(nMax - nMin));

	// Create the 3D texture buffer
	pTexture->SetTextureBuffer(reinterpret_cast<TextureBuffer*>(cTextureManager.GetRendererContext().GetRenderer().CreateTextureBuffer3D(cImage)));

	// Return the created texture
	return pTexture;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
