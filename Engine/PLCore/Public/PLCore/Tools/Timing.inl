/*********************************************************\
 *  File: Timing.inl                                     *
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
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ General                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns whether the timing is active or not
*/
inline bool Timing::IsActive() const
{
	return m_bActive;
}

/**
*  @brief
*    Sets whether the timing is active or not
*/
inline void Timing::SetActive(bool bActive)
{
	m_bActive = bActive;
}


//[-------------------------------------------------------]
//[ Past time                                             ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the past time since last frame (in seconds)
*/
inline float Timing::GetTimeDifference() const
{
	return m_bFreezed ? 0.0f : m_fTimeDifference;
}

/**
*  @brief
*    Returns the past time since last frame without any clamping (seconds)
*/
inline float Timing::GetTimeDifferenceNoCompensating() const
{
	return m_bFreezed ? 0.0f : m_fTimeDifferenceNoCompensating;
}

/**
*  @brief
*    Returns the maximum time difference
*/
inline float Timing::GetMaxTimeDifference() const
{
	return m_fMaxTimeDifference;
}

/**
*  @brief
*    Sets the maximum time difference
*/
inline void Timing::SetMaxTimeDifference(float fMaxTimeDifference)
{
	m_fMaxTimeDifference = (fMaxTimeDifference > 0.0f) ? fMaxTimeDifference : 0.0001f;
}

/**
*  @brief
*    Returns a general timing
*/
inline float Timing::GetTimer() const
{
	return m_fTimer;
}


//[-------------------------------------------------------]
//[ Frames                                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the current frames per second (FPS)
*/
inline float Timing::GetFramesPerSecond() const
{
	return m_fFramesPerSecond;
}

/**
*  @brief
*    Returns the number of past frames since timing start
*/
inline uint32 Timing::GetPastFrames() const
{
	return m_nPastFrames;
}

/**
*  @brief
*    Returns the FPS limit
*/
inline float Timing::GetFPSLimit() const
{
	return m_fFPSLimit;
}

/**
*  @brief
*    Sets the FPS limit
*/
inline void Timing::SetFPSLimit(float fFPSLimit)
{
	m_fFPSLimit = (fFPSLimit >= 0.0f) ? fFPSLimit : 0.0f;
}


//[-------------------------------------------------------]
//[ Time scale                                            ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns if the timing is currently freezed or not
*/
inline bool Timing::IsFreezed() const
{
	return m_bFreezed;
}

/**
*  @brief
*    Returns whether the timing is paused of not
*/
inline bool Timing::IsPaused() const
{
	return m_bPause;
}

/**
*  @brief
*    Set pause mode
*/
inline void Timing::Pause(bool bPause)
{
	m_bPause = bPause;
}

/**
*  @brief
*    Returns the time scale factor
*/
inline float Timing::GetTimeScaleFactor() const
{
	return m_fTimeScaleFactor;
}

/**
*  @brief
*    Sets the time scale factor
*/
inline bool Timing::SetTimeScaleFactor(float fFactor)
{
	if (fFactor > 0.0f) {
		// Set the new factor
		m_fTimeScaleFactor = fFactor;

		// Done
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Returns if the slow motion is activated or not
*/
inline bool Timing::IsSlowMotion() const
{
	return m_bSlowMotion;
}

/**
*  @brief
*    Activates/deactivates the slow motion mode
*/
inline void Timing::SetSlowMotion(bool bSlowMotion)
{
	m_bSlowMotion = bSlowMotion;
}

/**
*  @brief
*    Sets the custom slow motion factor
*/
inline float Timing::GetSlowMotionFactor(bool bRealUsed) const
{
	return (bRealUsed && !m_bSlowMotion) ? 1.0f : m_fSlowMotionFactor;
}

/**
*  @brief
*    Sets the slow motion factor
*/
inline bool Timing::SetSlowMotionFactor(float fSlowMotionFactor)
{
	if (fSlowMotionFactor > 0.0f) {
		// Set the new factor
		m_fSlowMotionFactor = fSlowMotionFactor;

		// Done
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Returns the custom slow motion factor
*/
inline float Timing::GetCustomSlowMotionFactor(bool bRealUsed) const
{
	return (bRealUsed && !m_bSlowMotion) ? 1.0f : m_fCustomSlowMotionFactor;
}

/**
*  @brief
*    Sets the custom slow motion factor
*/
inline bool Timing::SetCustomSlowMotionFactor(float fSlowMotionFactor)
{
	if (fSlowMotionFactor > 0.0f) {
		// Set the new factor
		m_fCustomSlowMotionFactor = fSlowMotionFactor;

		// Done
		return true;
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
