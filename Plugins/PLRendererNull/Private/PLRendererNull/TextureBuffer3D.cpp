/*********************************************************\
 *  File: TextureBuffer3D.cpp                            *
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
#include <PLRenderer/Renderer/Backend/RendererBackend.h>
#include "PLRendererNull/TextureBuffer3D.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLGraphics;
namespace PLRendererNull {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
TextureBuffer3D::~TextureBuffer3D()
{
	// Update renderer statistics
	static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nTextureBuffersNum--;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
TextureBuffer3D::TextureBuffer3D(PLRenderer::Renderer &cRenderer, Image &cImage, EPixelFormat nInternalFormat, uint32 nFlags) :
	PLRenderer::TextureBuffer3D(cRenderer, nFlags)
{
	// Update renderer statistics
	static_cast<PLRenderer::RendererBackend&>(cRenderer).GetWritableStatistics().nTextureBuffersNum++;

	// Get the image buffer
	ImageBuffer *pImageBuffer = cImage.GetBuffer();
	if (pImageBuffer) {
		// Set data
		m_vSize   = pImageBuffer->GetSize();
		m_nFormat = (nInternalFormat == Unknown) ? GetFormatFromImage(cImage) : nInternalFormat;
	}
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::TextureBuffer functions    ]
//[-------------------------------------------------------]
bool TextureBuffer3D::CopyDataFrom(uint32 nMipmap, EPixelFormat nFormat, const void *pData, uint8 nFace)
{
	// Error?
	return !(nMipmap != 0 || nFormat == Unknown || !pData || nFace);
}

bool TextureBuffer3D::CopyDataTo(uint32 nMipmap, EPixelFormat nFormat, void *pData, uint8 nFace) const
{
	// Error?
	return !(nMipmap != 0 || nFormat == Unknown || !pData || nFace);
}


//[-------------------------------------------------------]
//[ Private virtual PLRenderer::TextureBuffer functions   ]
//[-------------------------------------------------------]
bool TextureBuffer3D::MakeCurrent(uint32 nStage)
{
	// Check if there are renderer information
	if (GetRenderer().GetTextureBuffer(nStage) != this) {
		if (!GetRenderer().SetTextureBuffer(nStage, this))
			return true; // Done
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererNull
