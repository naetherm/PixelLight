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
#include <PLMath/Math.h>
#include "PLRenderer/Renderer/TextureBuffer3D.h"


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
TextureBuffer3D::~TextureBuffer3D()
{
}

/**
*  @brief
*    Returns the texture buffer size
*/
Vector3i TextureBuffer3D::GetSize(uint32 nMipmap) const
{
	// Check the given parameter
	if (nMipmap > m_nNumOfMipmaps)
		return Vector3i::Zero; // Error!

	// Get the size of the mipmap level
	Vector3i vSize = m_vSize;
	uint32 nCurrentMipmap = 0;
	while (nCurrentMipmap < nMipmap) {
		vSize.x >>= 1;
		vSize.y >>= 1;
		vSize.z >>= 1;
		nCurrentMipmap++;
	}
	if (vSize.x < 1)
		vSize.x = 1;
	if (vSize.y < 1)
		vSize.y = 1;
	if (vSize.z < 1)
		vSize.z = 1;

	// Return the size
	return vSize;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
TextureBuffer3D::TextureBuffer3D(Renderer &cRenderer, uint32 nFlags) : TextureBuffer(cRenderer, TypeTextureBuffer3D, nFlags)
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
TextureBuffer3D::TextureBuffer3D(const TextureBuffer3D &cSource) : TextureBuffer(cSource.GetRenderer(), cSource.GetType(), TypeTextureBuffer3D),
	m_vSize(cSource.m_vSize)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
TextureBuffer3D &TextureBuffer3D::operator =(const TextureBuffer3D &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Public virtual TextureBuffer functions                ]
//[-------------------------------------------------------]
bool TextureBuffer3D::IsPowerOfTwo() const
{
	return (Math::IsPowerOfTwo(m_vSize.x) && Math::IsPowerOfTwo(m_vSize.y) && Math::IsPowerOfTwo(m_vSize.z));
}

uint32 TextureBuffer3D::GetNumOfPixels(uint32 nMipmap) const
{
	// Check the given parameter
	if (nMipmap <= m_nNumOfMipmaps) {
		// Get the size of the mipmap level
		const Vector3i vSize = GetSize(nMipmap);

		// Return the number of pixels of the mipmap level
		return vSize.x*vSize.y*vSize.z;
	}

	// Error!
	return 0;
}

uint32 TextureBuffer3D::GetNumOfBytes(uint32 nMipmap, EPixelFormat nFormat) const
{
	// Check the given parameter
	if (nMipmap <= m_nNumOfMipmaps) {
		// Get the size of the mipmap level
		const Vector3i vSize = GetSize(nMipmap);

		// Calculate the number of bytes
		if (nFormat == Unknown)
			nFormat = m_nFormat;
		if (IsCompressedFormat(nFormat)) {
			// DXT formats divide the texture buffer into 4x4 blocks, each block has a fixed number of bytes
			switch (nFormat) {
				case DXT1:
				case LATC1:
					return ((vSize.x+3)/4)*((vSize.y+3)/4)*8*vSize.z;

				case DXT3:
				case DXT5:
				case LATC2:
					return ((vSize.x+3)/4)*((vSize.y+3)/4)*16*vSize.z;

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
			return GetBytesPerPixel(nFormat)*vSize.x*vSize.y*vSize.z;
		}
	}

	// Error!
	return 0;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
