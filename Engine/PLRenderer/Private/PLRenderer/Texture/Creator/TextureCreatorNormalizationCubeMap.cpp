/*********************************************************\
 *  File: TextureCreatorNormalizationCubeMap.cpp         *
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
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImagePart.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include "PLRenderer/RendererContext.h"
#include "PLRenderer/Texture/TextureManager.h"
#include "PLRenderer/Texture/Creator/TextureCreatorNormalizationCubeMap.h"


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
pl_implement_class(TextureCreatorNormalizationCubeMap)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
TextureCreatorNormalizationCubeMap::TextureCreatorNormalizationCubeMap() :
	Size(this)
{
}

/**
*  @brief
*    Destructor
*/
TextureCreatorNormalizationCubeMap::~TextureCreatorNormalizationCubeMap()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/*
*  @brief
*    Returns a cube vector
*/
Vector3 TextureCreatorNormalizationCubeMap::GetCubeVector(uint8 i, uint32 x, uint32 y) const
{
	const float s = (static_cast<float>(x) + 0.5f)/static_cast<float>(Size)*2.0f - 1.0f;
	const float t = (static_cast<float>(y) + 0.5f)/static_cast<float>(Size)*2.0f - 1.0f;
	Vector3 v;
	switch (i) {
		case 0:
			v = Vector3( 1.0f,    -t,    -s);
			break;

		case 1:
			v = Vector3(-1.0f,    -t,     s);
			break;

		case 2:
			v = Vector3(    s,  1.0f,     t);
			break;

		case 3:
			v = Vector3(    s, -1.0f,    -t);
			break;

		case 4:
			v = Vector3(    s,    -t,  1.0f);
			break;

		case 5:
			v = Vector3(   -s,    -t, -1.0f);
			break;
	}
	v.Normalize();

	// Return the cube vector
	return v;
}


//[-------------------------------------------------------]
//[ Private virtual TextureCreator functions              ]
//[-------------------------------------------------------]
Texture *TextureCreatorNormalizationCubeMap::Create(TextureManager &cTextureManager, Texture *pTexture) const
{
	// Create the texture
	pTexture = CreateTexture(cTextureManager, pTexture);

	// Create the normalization cube map data
	Image cImage;
	for (uint8 i=0; i<6; i++) {
		ImageBuffer *pImageBuffer = cImage.CreatePart(static_cast<uint32>(ImagePartCubeSidePosX) + i)->CreateMipmap();
		pImageBuffer->CreateImage(DataByte, ColorRGB, Vector3i(Size, Size, 1));

		uint8 *d = pImageBuffer->GetData();
		for (uint32 y=0; y<Size; y++) {
			for (uint32 x=0; x<Size; x++) {
				Vector3 vDir = GetCubeVector(i, x, y);
				vDir = (vDir + Vector3::One)*0.5f;
				*d++ = static_cast<uint8>(vDir.x*255.0f);
				*d++ = static_cast<uint8>(vDir.y*255.0f);
				*d++ = static_cast<uint8>(vDir.z*255.0f);
			}
		}
	}

	// Create the cube texture buffer
	pTexture->SetTextureBuffer(reinterpret_cast<TextureBuffer*>(cTextureManager.GetRendererContext().GetRenderer().CreateTextureBufferCube(cImage, TextureBuffer::Unknown, 0)));

	// Return the created texture
	return pTexture;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
