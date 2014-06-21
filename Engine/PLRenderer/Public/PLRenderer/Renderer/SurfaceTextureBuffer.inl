/*********************************************************\
 *  File: SurfaceTextureBuffer.inl                       *
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
*    Returns the texture buffer format
*/
inline TextureBuffer::EPixelFormat SurfaceTextureBuffer::GetFormat() const
{
	const PLRenderer::TextureBuffer *pTextureBuffer = GetTextureBuffer();
	return pTextureBuffer ? pTextureBuffer->GetFormat() : TextureBuffer::Unknown;
}

/**
*  @brief
*    Returns the texture buffer surface flags
*/
inline PLCore::uint32 SurfaceTextureBuffer::GetFlags() const
{
	return m_nFlags;
}

/**
*  @brief
*    Returns the maximum number of color render targets
*/
inline PLCore::uint8 SurfaceTextureBuffer::GetMaxColorTargets() const
{
	return m_nMaxColorTargets;
}

/**
*  @brief
*    Sets the maximum number of color render targets
*/
inline void SurfaceTextureBuffer::SetMaxColorTargets(PLCore::uint8 nMaxColorTargets)
{
	m_nMaxColorTargets = nMaxColorTargets;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
