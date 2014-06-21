/*********************************************************\
 *  File: TextureHandler.inl                             *
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
//[ TextureHandler::AnimationState                        ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the texture animation
*/
inline Animation &TextureHandler::AnimationState::GetTextureAnimation()
{
	return *m_pTextureAnimation;
}

/**
*  @brief
*    Returns the matrix animation
*/
inline Animation &TextureHandler::AnimationState::GetMatrixAnimation()
{
	return *m_pMatrixAnimation;
}

/**
*  @brief
*    Returns the current texture transformation matrix
*/
inline PLMath::Matrix4x4 &TextureHandler::AnimationState::GetMatrix()
{
	return m_mTrans;
}

/**
*  @brief
*    Returns the color animation
*/
inline Animation &TextureHandler::AnimationState::GetColorAnimation()
{
	return *m_pColorAnimation;
}

/**
*  @brief
*    Returns the current texture color
*/
inline const PLGraphics::Color4 &TextureHandler::AnimationState::GetColor() const
{
	return m_cColor;
}




//[-------------------------------------------------------]
//[ TextureHandler                                        ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the texture animation state object (if there's one :)
*/
inline TextureHandler::AnimationState *TextureHandler::GetAnimationState() const
{
	return m_pAnimationState;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
