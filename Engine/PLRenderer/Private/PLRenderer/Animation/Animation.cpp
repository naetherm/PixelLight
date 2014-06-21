/*********************************************************\
 *  File: Animation.cpp                                  *
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
#include "PLRenderer/Animation/AnimationInfo.h"
#include "PLRenderer/Animation/AnimationEvent.h"
#include "PLRenderer/Animation/AnimationManager.h"
#include "PLRenderer/Animation/Animation.h"


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
*    Constructor
*/
Animation::Animation(const String &sName, AnimationManager *pManager) : Element<Animation>(sName, pManager),
	m_fFrame(0.0f),
	m_bActive(false),
	m_bRunning(false),
	m_nType(0),
	m_nFlags(0),
	m_nStart(0),
	m_nEnd(0),
	m_fSpeed(24.0f),
	m_bEvents(true),
	m_pAnimationInfo(nullptr),
	m_fWeight(1.0f),
	m_bBounced(false)
{
}

/**
*  @brief
*    Destructor
*/
Animation::~Animation()
{
}

/**
*  @brief
*    Resets the animation
*/
void Animation::Reset()
{
	// Init animation status
	m_bActive		 = false;
	m_bRunning		 = false;
	m_nType			 = 0;
	m_nFlags		 = 0;
	m_nStart		 = 0;
	m_nEnd			 = 0;
	m_fSpeed		 = 24.0f;
	m_pAnimationInfo = nullptr;
	m_fWeight		 = 1.0f;
	m_bBounced		 = false;

	// Set the current frame
	SetCurrentFrame(0.0f);
}

/**
*  @brief
*    Starts an animation
*/
void Animation::Start(uint32 nStart, uint32 nEnd, float fSpeed, uint32 nFlags)
{
	m_bActive		 = true;
	m_bRunning		 = true;
	m_nFlags		 = nFlags;
	m_nStart		 = nStart;
	m_nEnd			 = nEnd;
	m_fSpeed		 = fSpeed;
	m_pAnimationInfo = nullptr;
	m_bBounced		 = false;

	// Set the current frame
	SetCurrentFrame(static_cast<float>(nStart));

	// Emit start event
	EventStart();
}

/**
*  @brief
*    Starts an animation
*/
void Animation::Start(AnimationInfo &cAnimationInfo, bool bRestart)
{
	// Is this animation already running?
	if (bRestart || !m_pAnimationInfo || (m_pAnimationInfo && m_pAnimationInfo != &cAnimationInfo)) {
		// Start/restart the animation
		m_bActive		 = true;
		m_bRunning		 = true;
		m_nType			 = cAnimationInfo.GetType();
		m_nFlags		 = cAnimationInfo.GetFlags();
		m_nStart		 = cAnimationInfo.GetStartFrame();
		m_nEnd			 = cAnimationInfo.GetEndFrame();
		m_fSpeed		 = cAnimationInfo.GetSpeed();
		m_pAnimationInfo = &cAnimationInfo;
		m_bBounced		 = false;

		// Set the current frame
		SetCurrentFrame(static_cast<float>(cAnimationInfo.GetStartFrame()));

		// Check if a event is caused
		CheckEvent();

		// Emit start event
		EventStart();
	}
}

/**
*  @brief
*    Stops the animation
*/
void Animation::Stop()
{
	// Is the animation currently active?
	if (m_bActive) {
		// [TODO] Review this. It should be correct though, because only the running-state should be reset here,
		//        no the whole animation information.
	//	Reset();

		// Stop animation, but keep current state and options
		m_bActive		 = false;
		m_bRunning		 = false;

		// Emit stop event
		EventStop();
	}
}

/**
*  @brief
*    Get the current animation frame
*/
float Animation::GetFrame() const
{
	// Is there a more complex animation information?
	if (m_pAnimationInfo) {
		// Get the 'real' start frame
		const uint32 nStart = (m_nEnd > m_nStart) ? m_nStart : m_nEnd;

		// Use frame interpolation?
		const AnimationFrameInfo *pFrameInfo = m_pAnimationInfo->GetFrameInfo(static_cast<uint32>(m_fFrame-nStart));
		if (pFrameInfo && pFrameInfo->GetSpeed() < 0.0f)
			return static_cast<float>(static_cast<uint32>(m_fFrame)); // No interpolation, please
	}

	// Just return the current frame
	return m_fFrame;
}

/**
*  @brief
*    Gets the progress from the current state to the next state (0.0-1.0)
*/
float Animation::GetProgress() const
{
	return Math::Abs(GetFrame()-GetCurrentFrame());
}

/**
*  @brief
*    Get the current animation frame as an integer
*/
uint32 Animation::GetCurrentFrame() const
{
	if (m_fSpeed >= 0.0f)
		return static_cast<uint32>(m_fFrame);
	else {
		// Get 'real' end frame
		const uint32 nEnd = (m_nEnd > m_nStart) ? m_nEnd : m_nStart;

		// Get the current frame
		const uint32 nFrame = static_cast<uint32>(m_fFrame)+1;
		return (nFrame > nEnd) ? nEnd : nFrame;
	}
}

/**
*  @brief
*    Set the current animation frame
*/
void Animation::SetCurrentFrame(float fFrame)
{
	// Clamp the given frame if required
	if (m_nEnd > m_nStart) {
		if (fFrame < m_nStart)
			fFrame = static_cast<float>(m_nStart);
		else if (fFrame > m_nEnd)
			fFrame = static_cast<float>(m_nEnd);
	} else {
		if (fFrame < m_nEnd)
			fFrame = static_cast<float>(m_nEnd);
		else if (fFrame > m_nStart)
			fFrame = static_cast<float>(m_nStart);
	}

	// Frame change?
	if (m_fFrame != fFrame) {
		// Set the current animation frame
		m_fFrame = fFrame;

		// Emit frame change event
		if (m_bEvents)
			EventFrameChange();
	}
}

/**
*  @brief
*    Get the next animation frame as an integer
*/
uint32 Animation::GetNextFrame() const
{
	// Start frame = end frame?
	if (m_nStart == m_nEnd)
		return m_nStart; // That was pretty easy :)

	// Get 'real' start and end frames
	int nStart, nEnd;
	if (m_nEnd > m_nStart) {
		nStart = m_nStart;
		nEnd   = m_nEnd;
	} else {
		nStart = m_nEnd;
		nEnd   = m_nStart;
	}

	// Next frame please
	int nNextFrame = nStart;
	if (m_fSpeed >= 0.0f) {
		nNextFrame = static_cast<int>(m_fFrame)+1;

		// Ping pong (with or without loop)
		if (m_nFlags & PingPong) {
			if (m_nFlags & Loop) {
				// End reached?
				if (nNextFrame > nEnd)
					nNextFrame = nEnd-1;
			} else {
				// End reached?
				if (nNextFrame > nEnd)
					nNextFrame = m_bBounced ? nEnd : nEnd-1;
			}

		// Loop
		} else if (m_nFlags & Loop) {
			// End reached?
			if (nNextFrame > nEnd)
				nNextFrame = nStart;

		// Normal
		} else {
			// End reached?
			if (nNextFrame > nEnd)
				nNextFrame = nEnd;
		}

	// Reversed playback
	} else {
		nNextFrame = static_cast<int>(m_fFrame);

		// Ping pong (with or without loop)
		if (m_nFlags & PingPong) {
			if (m_nFlags & Loop) {
				// Start reached?
				if (nNextFrame < nStart)
					nNextFrame = nStart+1;
			} else {
				// Start reached?
				if (nNextFrame < nStart)
					nNextFrame = m_bBounced ? nStart : nStart+1;
			}

		// Loop
		} else if (m_nFlags & Loop) {
			// Start reached?
			if (nNextFrame < nStart)
				nNextFrame = nEnd;

		// Normal
		} else {
			// Start reached?
			if (nNextFrame < nStart)
				nNextFrame = nStart;
		}
	}

	// Return the next frame
	return static_cast<uint32>(nNextFrame);
}

/**
*  @brief
*    Updates the animation
*/
void Animation::Update(float fTimeDifference)
{
	// Update animation
	if (m_bRunning && m_nStart != m_nEnd && m_fSpeed) {
		// Backup the current frame as previous frame
		const float fPreviousFrame = m_fFrame;

		// Get 'real' start and end frames
		uint32 nStart, nEnd;
		if (m_nEnd > m_nStart) {
			nStart = m_nStart;
			nEnd   = m_nEnd;
		} else {
			nStart = m_nEnd;
			nEnd   = m_nStart;
		}

		// Get step size
		float fStep = 0.0;
		if (m_pAnimationInfo) {
			// Is there an advanced frame information?
			const AnimationFrameInfo *pFrameInfo = m_pAnimationInfo->GetFrameInfo(static_cast<uint32>(m_fFrame-nStart));
			if (pFrameInfo) {
				// Get frame speed
				float fFrameSpeed = pFrameInfo->GetSpeed();

				// Skip the frame?
				if (fFrameSpeed) {
					// Ignore the sign in here
					if (fFrameSpeed < 0.0f)
						fFrameSpeed = -fFrameSpeed;

					// Get step size
					fStep = fTimeDifference*fFrameSpeed;
				} else {
					fStep = 1.0f;
				}
			} else {
				fStep = fTimeDifference;
			}
		} else {
			fStep = fTimeDifference;
		}

		// Update the current frame
		m_fFrame += fStep*m_fSpeed;

		// Check bounds
		if (m_fSpeed >= 0.0f) {
			// Ping pong (with or without loop)
			if (m_nFlags & PingPong) {
				if (m_nFlags & Loop) {
					// End reached?
					if (m_fFrame >= static_cast<float>(nEnd)) {
						m_fFrame   = static_cast<float>(nEnd);
						m_fSpeed   = -m_fSpeed;
						m_bBounced = true;
					}
				} else {
					// End reached?
					if (m_fFrame >= static_cast<float>(nEnd)) {
						m_fFrame = static_cast<float>(nEnd);
						if (m_bBounced)
							Stop();
						else {
							m_fSpeed   = -m_fSpeed;
							m_bBounced = true;
						}
					}
				}

			// Loop
			} else if (m_nFlags & Loop) {
				// End reached?
				if (m_fFrame >= static_cast<float>(nEnd))
					m_fFrame = nStart+(m_fFrame-nEnd);

			// Normal
			} else {
				// End reached?
				if (m_fFrame >= static_cast<float>(nEnd)) {
					m_fFrame = static_cast<float>(nEnd);
					Stop();
				}
			}

		// Reversed playback
		} else {
			// Ping pong (with or without loop)
			if (m_nFlags & PingPong) {
				if (m_nFlags & Loop) {
					// Start reached?
					if (m_fFrame <= static_cast<float>(nStart)) {
						m_fFrame   = static_cast<float>(nStart);
						m_fSpeed   = -m_fSpeed;
						m_bBounced = true;
					}
				} else {
					// Start reached?
					if (m_fFrame <= static_cast<float>(nStart)) {
						m_fFrame = static_cast<float>(nStart);
						if (m_bBounced)
							Stop();
						else {
							m_fSpeed   = -m_fSpeed;
							m_bBounced = true;
						}
					}
				}

			// Loop
			} else if (m_nFlags & Loop) {
				// Start reached?
				if (m_fFrame <= static_cast<float>(nStart))
					m_fFrame = nEnd+(m_fFrame-nStart);

			// Normal
			} else {
				// Start reached?
				if (m_fFrame <= static_cast<float>(nStart)) {
					m_fFrame = static_cast<float>(nStart);
					Stop();
				}
			}
		}

		// Frame change?
		if (fPreviousFrame != m_fFrame && m_bEvents) {
			// Emit frame change event
			EventFrameChange();
		}

		// Check if a event is caused
		CheckEvent(static_cast<int>(fPreviousFrame));
	}
}


//[-------------------------------------------------------]
//[ Operators                                             ]
//[-------------------------------------------------------]
Animation &Animation::operator =(const Animation &cSource)
{
	// Copy data
	m_bActive			= cSource.m_bActive;
	m_bRunning			= cSource.m_bRunning;
	m_nType				= cSource.m_nType;
	m_nFlags			= cSource.m_nFlags;
	m_nStart			= cSource.m_nStart;
	m_nEnd				= cSource.m_nEnd;
	m_fSpeed			= cSource.m_fSpeed;
	m_bEvents			= cSource.m_bEvents;
	m_pAnimationInfo	= cSource.m_pAnimationInfo;
	m_fWeight			= cSource.m_fWeight;
	m_bBounced			= cSource.m_bBounced;

	// Set the current frame
	SetCurrentFrame(cSource.m_fFrame);

	// Done
	return *this;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Check if events should be caused
*/
void Animation::CheckEvent(int nPreviousFrame) const
{
	// Are events enabled?
	if (m_bEvents && m_pAnimationInfo && static_cast<int>(m_fFrame) != nPreviousFrame) {
		for (uint32 i=0; i<m_pAnimationInfo->GetEventManager().GetNumOfElements(); i++) {
			const AnimationEvent *pAnimationEvent = m_pAnimationInfo->GetEventManager().GetByIndex(i);
			if (pAnimationEvent->GetFrame() == static_cast<uint32>(m_fFrame)) {
				// Emit event
				EventSpecialFrame(pAnimationEvent->GetID());
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
