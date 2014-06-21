/*********************************************************\
 *  File: TextureManager.inl                             *
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
*    Returns the owner renderer context of this manager
*/
inline RendererContext &TextureManager::GetRendererContext() const
{
	return *m_pRendererContext;
}

/**
*  @brief
*    Returns the texture quality
*/
inline float TextureManager::GetTextureQuality() const
{
	return m_fTextureQuality;
}

/**
*  @brief
*    Returns whether the next lower valid texture size should be taken or the higher one
*/
inline bool TextureManager::GetTextureFit() const
{
	return m_bTextureFitLower;
}

/**
*  @brief
*    Sets whether the next lower valid texture size should be taken or the higher one
*/
inline void TextureManager::SetTextureFit(bool bLower)
{
	m_bTextureFitLower = bLower;
}

/**
*  @brief
*    Returns whether or not the usage of texture mipmaps is allowed
*/
inline bool TextureManager::AreTextureMipmapsAllowed() const
{
	return m_bTextureMipmapsAllowed;
}

/**
*  @brief
*    Sets whether or not the usage of texture mipmaps is allowed
*/
inline void TextureManager::SetTextureMipmapsAllowed(bool bAllowed)
{
	m_bTextureMipmapsAllowed = bAllowed;
}

/**
*  @brief
*    Returns whether or not the usage of texture compression is allowed
*/
inline bool TextureManager::IsTextureCompressionAllowed() const
{
	return m_bTextureCompressionAllowed;
}

/**
*  @brief
*    Sets whether or not the usage of texture compression is allowed
*/
inline void TextureManager::SetTextureCompressionAllowed(bool bAllowed)
{
	m_bTextureCompressionAllowed = bAllowed;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
