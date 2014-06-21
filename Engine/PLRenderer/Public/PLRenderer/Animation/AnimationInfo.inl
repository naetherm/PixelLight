/*********************************************************\
 *  File: AnimationInfo.inl                              *
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
*    Returns the type of the animation
*/
inline PLCore::uint32 AnimationInfo::GetType() const
{
	return m_nType;
}

/**
*  @brief
*    Sets the animation type
*/
inline void AnimationInfo::SetType(PLCore::uint32 nType)
{
	m_nType = nType;
}

/**
*  @brief
*    Returns the animation start frame
*/
inline PLCore::uint32 AnimationInfo::GetStartFrame() const
{
	return m_nStart;
}

/**
*  @brief
*    Returns the animation end frame
*/
inline PLCore::uint32 AnimationInfo::GetEndFrame() const
{
	return m_nEnd;
}

/**
*  @brief
*    Returns the number of animation frames
*/
inline PLCore::uint32 AnimationInfo::GetNumOfFrames() const
{
	return (m_nEnd > m_nStart) ? m_nEnd-m_nStart+1 : m_nStart-m_nEnd+1;
}

/**
*  @brief
*    Get the animation speed
*/
inline float AnimationInfo::GetSpeed() const
{
	return m_fSpeed;
}

/**
*  @brief
*    Set the animation speed
*/
inline void AnimationInfo::SetSpeed(float fSpeed)
{
	m_fSpeed = fSpeed;
}

/**
*  @brief
*    Get the animation information flags
*/
inline PLCore::uint32 AnimationInfo::GetFlags() const
{
	return m_nFlags;
}

/**
*  @brief
*    Sets the animation information flags
*/
inline void AnimationInfo::SetFlags(PLCore::uint32 nFlags)
{
	m_nFlags = nFlags;
}

/**
*  @brief
*    Returns information about a frame
*/
inline AnimationFrameInfo *AnimationInfo::GetFrameInfo(PLCore::uint32 nIndex) const
{
	AnimationFrameInfo &cAnimationFrameInfo = m_lstFrameInfos[nIndex];
	return (&cAnimationFrameInfo != &PLCore::Array<AnimationFrameInfo>::Null) ? &cAnimationFrameInfo : nullptr;
}

/**
*  @brief
*    Returns the event manager
*/
inline AnimationEventManager &AnimationInfo::GetEventManager()
{
	return m_cEventManager;
}

inline const AnimationEventManager &AnimationInfo::GetEventManager() const
{
	return m_cEventManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
