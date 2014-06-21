/*********************************************************\
 *  File: AnimationFrameInfo.inl                         *
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
*    Constructor
*/
inline AnimationFrameInfo::AnimationFrameInfo() :
	m_fSpeed(1.0f)
{
}

/**
*  @brief
*    Destructor
*/
inline AnimationFrameInfo::~AnimationFrameInfo()
{
}

/**
*  @brief
*    Returns the speed of an animation frame
*/
float AnimationFrameInfo::GetSpeed() const
{
	return m_fSpeed;
}

/**
*  @brief
*    Sets the animation frame speed
*/
inline void AnimationFrameInfo::SetSpeed(float fSpeed)
{
	m_fSpeed = fSpeed;
}

/**
*  @brief
*    Copy operator
*/
inline AnimationFrameInfo &AnimationFrameInfo::operator =(const AnimationFrameInfo &cSource)
{
	// Copy data
	m_fSpeed = cSource.m_fSpeed;

	// Return pointer
	return *this;
}

/**
*  @brief
*    Compare operator
*/
inline bool AnimationFrameInfo::operator ==(const AnimationFrameInfo &cSource) const
{
	return (m_fSpeed == cSource.m_fSpeed);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
