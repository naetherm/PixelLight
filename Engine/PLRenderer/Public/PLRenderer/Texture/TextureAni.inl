/*********************************************************\
 *  File: TextureAni.inl                                 *
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
//[-------------------------------------------------------]
//[ Texture animation                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the texture animation manager
*/
inline AniInfoManager &TextureAni::GetTextureAnimationManager()
{
	return m_cTextureAnimationManager;
}

inline const AniInfoManager &TextureAni::GetTextureAnimationManager() const
{
	return m_cTextureAnimationManager;
}

/**
*  @brief
*    Returns the texture frames array
*/
inline PLCore::Array<TextureAni::TextureFrame*> &TextureAni::GetTextureFrames()
{
	return m_lstTextureFrames;
}

inline const PLCore::Array<TextureAni::TextureFrame*> &TextureAni::GetTextureFrames() const
{
	return m_lstTextureFrames;
}


//[-------------------------------------------------------]
//[ Matrix animation                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the matrix animation manager
*/
inline AniInfoManager &TextureAni::GetMatrixAnimationManager()
{
	return m_cMatrixAnimationManager;
}

inline const AniInfoManager &TextureAni::GetMatrixAnimationManager() const
{
	return m_cMatrixAnimationManager;
}

/**
*  @brief
*    Returns the matrix frames array
*/
inline PLCore::Array<TextureAni::MatrixFrame*> &TextureAni::GetMatrixFrames()
{
	return m_lstMatrixFrames;
}

inline const PLCore::Array<TextureAni::MatrixFrame*> &TextureAni::GetMatrixFrames() const
{
	return m_lstMatrixFrames;
}


//[-------------------------------------------------------]
//[ Color animation                                       ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the color animation manager
*/
inline AniInfoManager &TextureAni::GetColorAnimationManager()
{
	return m_cColorAnimationManager;
}

inline const AniInfoManager &TextureAni::GetColorAnimationManager() const
{
	return m_cColorAnimationManager;
}

/**
*  @brief
*    Returns the color frames array
*/
inline PLCore::Array<TextureAni::ColorFrame*> &TextureAni::GetColorFrames()
{
	return m_lstColorFrames;
}

inline const PLCore::Array<TextureAni::ColorFrame*> &TextureAni::GetColorFrames() const
{
	return m_lstColorFrames;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
