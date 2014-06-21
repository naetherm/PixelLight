/*********************************************************\
 *  File: TextureBuffer.inl                              *
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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the texture buffer flags
*/
inline PLCore::uint32 TextureBuffer::GetFlags() const
{
	return m_nFlags;
}

/**
*  @brief
*    Returns the texture buffer pixel format
*/
inline TextureBuffer::EPixelFormat TextureBuffer::GetFormat() const
{
	return m_nFormat;
}

/**
*  @brief
*    Returns whether or not the used texture buffer format is compressed
*/
inline bool TextureBuffer::IsCompressedFormat() const
{
	return IsCompressedFormat(m_nFormat);
}

/**
*  @brief
*    Returns whether or not the used texture buffer format is a depth buffer format
*/
inline bool TextureBuffer::IsDepthFormat() const
{
	return IsDepthFormat(m_nFormat);
}

/**
*  @brief
*    If the used texture buffer format is compressed, a proper uncompressed texture buffer format will be returned
*/
inline TextureBuffer::EPixelFormat TextureBuffer::GetUncompressedFormat() const
{
	return GetUncompressedFormat(m_nFormat);
}

/**
*  @brief
*    Returns whether or not the used texture buffer format is a floating point format
*/
inline bool TextureBuffer::IsFloatingPointFormat() const
{
	return IsFloatingPointFormat(m_nFormat);
}

/**
*  @brief
*    Returns the number of components per pixel
*/
inline PLCore::uint32 TextureBuffer::GetComponentsPerPixel() const
{
	return GetComponentsPerPixel(m_nFormat);
}

/**
*  @brief
*    Returns the number of bytes per pixel component
*/
inline PLCore::uint32 TextureBuffer::GetBytesPerPixelComponent() const
{
	return GetBytesPerPixelComponent(m_nFormat);
}

/**
*  @brief
*    Returns the number of bytes per pixel
*/
inline PLCore::uint32 TextureBuffer::GetBytesPerPixel() const
{
	return GetBytesPerPixel(m_nFormat);
}

/**
*  @brief
*    Returns the number of mipmap levels
*/
inline PLCore::uint32 TextureBuffer::GetNumOfMipmaps() const
{
	return m_nNumOfMipmaps;
}

/**
*  @brief
*    Returns the number of faces
*/
inline PLCore::uint8 TextureBuffer::GetNumOfFaces() const
{
	return (GetType() == TypeTextureBufferCube) ? 6 : 1;
}

/**
*  @brief
*    Returns the total number of bytes required for the texture buffer data including all mipmaps
*/
inline PLCore::uint32 TextureBuffer::GetTotalNumOfBytes() const
{
	return m_nTotalNumOfBytes;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
