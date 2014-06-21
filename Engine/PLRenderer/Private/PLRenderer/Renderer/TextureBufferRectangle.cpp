/*********************************************************\
 *  File: TextureBufferRectangle.cpp                     *
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
#include <PLMath/Math.h>
#include "PLRenderer/Renderer/TextureBufferRectangle.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
TextureBufferRectangle::~TextureBufferRectangle()
{
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
TextureBufferRectangle::TextureBufferRectangle(Renderer &cRenderer, uint32 nFlags) : TextureBuffer(cRenderer, TypeTextureBufferRectangle, nFlags)
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
TextureBufferRectangle::TextureBufferRectangle(const TextureBufferRectangle &cSource) : TextureBuffer(cSource.GetRenderer(), cSource.GetType(), TypeTextureBufferRectangle),
	m_vSize(cSource.m_vSize)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
TextureBufferRectangle &TextureBufferRectangle::operator =(const TextureBufferRectangle &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Public virtual TextureBuffer functions                ]
//[-------------------------------------------------------]
bool TextureBufferRectangle::IsPowerOfTwo() const
{
	return (Math::IsPowerOfTwo(m_vSize.x) && Math::IsPowerOfTwo(m_vSize.y));
}

uint32 TextureBufferRectangle::GetNumOfPixels(uint32 nMipmap) const
{
	// Check the given parameter
	if (nMipmap <= m_nNumOfMipmaps) {
		// [TODO] Currently mipmaps are not supported for rectangle texture buffers

		// Get the size of the mipmap level
//		const Vector2i vSize = GetSize(nMipmap);

		// Return the number of pixels of the mipmap level
		return m_vSize.x*m_vSize.y;
	}

	// Error!
	return 0;
}

uint32 TextureBufferRectangle::GetNumOfBytes(uint32 nMipmap, EPixelFormat nFormat) const
{
	// Check the given parameter
	if (nMipmap <= m_nNumOfMipmaps) {

		// [TODO] Currently mipmaps are not supported for rectangle texture buffers

		if (nFormat == Unknown)
			nFormat = m_nFormat;
		if (IsCompressedFormat(nFormat)) {
			// DXT formats divide the texture buffer into 4x4 blocks, each block has a fixed number of bytes
			switch (nFormat) {
				case DXT1:
				case LATC1:
					return ((m_vSize.x+3)/4)*((m_vSize.y+3)/4)*8;

				case DXT3:
				case DXT5:
				case LATC2:
					return ((m_vSize.x+3)/4)*((m_vSize.y+3)/4)*16;

				case L8:
				case L16:
				case A8:
				case L4A4:
				case L8A8:
				case D16:
				case D24:
				case D32:
				case R3G3B2:
				case R5G6B5:
				case R5G5B5A1:
				case R4G4B4A4:
				case R8G8B8:
				case R8G8B8A8:
				case R10G10B10A2:
				case R16G16B16A16:
				case L16F:
				case L32F:
				case R16G16B16A16F:
				case R32G32B32A32F:
				case Unknown:
				default:
					// Those cases are handled below
					break;
			}
		} else {
			return GetBytesPerPixel(nFormat)*m_vSize.x*m_vSize.y;
		}
	}

	// Error!
	return 0;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
