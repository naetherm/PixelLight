/*********************************************************\
 *  File: TextureCreatorSpot2D.cpp                       *
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
#include <PLGraphics/Image/ImageBuffer.h>
#include "PLRenderer/RendererContext.h"
#include "PLRenderer/Texture/TextureManager.h"
#include "PLRenderer/Texture/Creator/TextureCreatorSpot2D.h"


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
pl_class_metadata(TextureCreatorSpot2D, "PLRenderer", PLRenderer::TextureCreator, "Spot 2D texture (TextureBuffer::L8) creator class")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Attributes
	pl_attribute_metadata(Width,		PLCore::uint32,	64,		ReadWrite,	"Texture width (must be a power of 2)",		"Min='8'")
	pl_attribute_metadata(Height,		PLCore::uint32,	64,		ReadWrite,	"Texture height (must be a power of 2)",	"Min='8'")
	pl_attribute_metadata(Brightness,	PLCore::uint8,	255,	ReadWrite,	"Spot brightness",							"")
pl_class_metadata_end(TextureCreatorSpot2D)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
TextureCreatorSpot2D::TextureCreatorSpot2D() :
	Width(this),
	Height(this),
	Brightness(this)
{
}

/**
*  @brief
*    Destructor
*/
TextureCreatorSpot2D::~TextureCreatorSpot2D()
{
}


//[-------------------------------------------------------]
//[ Private virtual TextureCreator functions              ]
//[-------------------------------------------------------]
Texture *TextureCreatorSpot2D::Create(TextureManager &cTextureManager, Texture *pTexture) const
{
	// Create the texture
	pTexture = CreateTexture(cTextureManager, pTexture);

	// Create the image
	Image cImage = Image::CreateImage(DataByte, ColorGrayscale, Vector3i(Width, Height, 1));
	ImageBuffer *pImageBuffer = cImage.GetBuffer();

	// Create the texture data
	const float fWidthHalf  = static_cast<float>(Width/2)-0.5f;
	const float fHeightHalf = static_cast<float>(Height/2)-0.5f;
	uint8 *pData = pImageBuffer->GetData();
	for (uint32 j=0; j<Width; j++) {
		for (uint32 i=0; i<Height; i++) {
			const float x = (i - fWidthHalf)/fWidthHalf;
			const float y = (j - fHeightHalf)/fHeightHalf;

			const float ls = Math::Max(1 - (x*x + y*y), 0.0f);
			pData[j*Width+i] = static_cast<uint8>(Brightness*ls);
		}
	}

	// Create the 2D texture buffer
	pTexture->SetTextureBuffer(reinterpret_cast<TextureBuffer*>(cTextureManager.GetRendererContext().GetRenderer().CreateTextureBuffer2D(cImage)));

	// Return the created texture
	return pTexture;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
