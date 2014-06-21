/*********************************************************\
 *  File: Animation.inl                                  *
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
*    Get the animation active status
*/
inline bool Animation::IsActive() const
{
	return m_bActive;
}

/**
*  @brief
*    Set the animation active status
*/
inline void Animation::SetActive(bool bActive)
{
	m_bActive = bActive;
}

/**
*  @brief
*    Type of animation
*/
inline PLCore::uint32 Animation::GetType() const
{
	return m_nType;
}

/**
*  @brief
*    Sets the animation type
*/
inline void Animation::SetType(PLCore::uint32 nType)
{
	m_nType = nType;
}

/**
*  @brief
*    Pause the animation
*/
inline void Animation::Pause()
{
	m_bRunning = false;
}

/**
*  @brief
*    Continues the animation
*/
inline void Animation::Continue()
{
	if (m_nStart != m_nEnd)
		m_bRunning = true;
}

/**
*  @brief
*    Returns the animation information
*/
inline AnimationInfo *Animation::GetAnimation() const
{
	return m_pAnimationInfo;
}

/**
*  @brief
*    Gets the animation weight
*/
inline float Animation::GetWeight() const
{
	return m_fWeight;
}

/**
*  @brief
*    Sets the animation weight
*/
inline void Animation::SetWeight(float fWeight)
{
	m_fWeight = fWeight;
}

/**
*  @brief
*    Get the animation speed
*/
inline float Animation::GetSpeed() const
{
	return m_fSpeed;
}

/**
*  @brief
*    Set the animation speed
*/
inline void Animation::SetSpeed(float fSpeed)
{
	m_fSpeed = fSpeed;
}

/**
*  @brief
*    Get the animation information flags
*/
inline PLCore::uint32 Animation::GetFlags() const
{
	return m_nFlags;
}

/**
*  @brief
*    Sets the animation information flags
*/
inline void Animation::SetFlags(PLCore::uint32 nFlags)
{
	m_nFlags = nFlags;
}

/**
*  @brief
*    Get the animation running status
*/
inline bool Animation::IsRunning() const
{
	return m_bRunning;
}

/**
*  @brief
*    Returns the start frame
*/
inline PLCore::uint32 Animation::GetStartFrame() const
{
	return m_nStart;
}

/**
*  @brief
*    Returns the end frame
*/
inline PLCore::uint32 Animation::GetEndFrame() const
{
	return m_nEnd;
}

/**
*  @brief
*    Returns the number of frames
*/
inline PLCore::uint32 Animation::GetNumOfFrames() const
{
	return (m_nEnd > m_nStart) ? m_nEnd-m_nStart : m_nStart-m_nEnd;
}


//[-------------------------------------------------------]
//[ Events                                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns whether animation events are enabled or not
*/
inline bool Animation::GetEvents() const
{
	return m_bEvents;
}

/**
*  @brief
*    Sets whether animation events are enabled or not
*/
inline void Animation::SetEvents(bool bEvents)
{
	m_bEvents = bEvents;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
