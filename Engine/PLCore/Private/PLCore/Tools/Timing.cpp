/*********************************************************\
 *  File: Timing.cpp                                     *
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
#include "PLCore/System/System.h"
#include "PLCore/Tools/Timing.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Template instance                                     ]
//[-------------------------------------------------------]
template class Singleton<Timing>;


//[-------------------------------------------------------]
//[ Public static PLCore::Singleton functions             ]
//[-------------------------------------------------------]
Timing *Timing::GetInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<Timing>::GetInstance();
}

bool Timing::HasInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<Timing>::HasInstance();
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ General                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Resets the timing
*/
void Timing::Reset()
{
	m_nLastFPSUpdateTime = m_nTimeLast = m_nTimeNow = m_nFPSLimitLastTime = System::GetInstance()->GetMilliseconds();
}

/**
*  @brief
*    Checks whether the timing has to be updated or not
*/
bool Timing::CheckUpdate(uint64 *pnTimeToWait) const
{
	// By default we don't need to wait
	if (pnTimeToWait)
		*pnTimeToWait = 0;

	// Is there a FPS limitation?
	if (m_fFPSLimit) {
		const uint64 nPastTime     = System::GetInstance()->GetMilliseconds()-m_nFPSLimitLastTime;
		const uint32 nTimePerFrame = static_cast<uint32>(1000/m_fFPSLimit);
		if (nPastTime < nTimePerFrame) {
			if (pnTimeToWait)
				*pnTimeToWait = nTimePerFrame-nPastTime;

			// It's not time for an update!
			return false;
		}
	}

	// Done
	return true;
}

/**
*  @brief
*    Updates all time relevant stuff
*/
bool Timing::Update(uint64 *pnTimeToWait)
{
	// Is the timing currently freezed?
	if (m_bFreezed)
		m_fTimeDifference = 0.0f;

	// Check FPS limitation
	if (m_fFPSLimit) {
		// Time for an update?
		if (CheckUpdate(pnTimeToWait))
			m_nFPSLimitLastTime = System::GetInstance()->GetMilliseconds();
		else
			return false; // Error - it's not time for an update
	} else {
		if (pnTimeToWait)
			pnTimeToWait = 0;
	}

	// Is the timing currently freezed?
	if (!m_bFreezed) {
		// Get time difference
		float fPrevTimeDifference = m_fTimeDifference;
		m_nTimeNow = System::GetInstance()->GetMilliseconds();
		m_fTimeDifference = m_fTimeDifferenceNoCompensating = (m_nTimeNow-m_nTimeLast)/1000.0f;
		if (!m_bActive || m_fTimeDifference > m_fMaxTimeDifference)
			m_fTimeDifference = m_fMaxTimeDifference;
		m_nTimeLast = m_nTimeNow;

		// Calculate the current FPS
		m_nPastFrames++;
		m_nFramesSinceCheck++;
		m_fFPSUpdateTimer += m_fTimeDifference;
		if (m_fFPSUpdateTimer > 1.0f) {
			m_fFPSUpdateTimer = 0.0f;
			m_fFramesPerSecond = static_cast<float>(m_nFramesSinceCheck/((m_nTimeNow-m_nLastFPSUpdateTime)/1000.0f));
			if (m_fFramesPerSecond < 0.0f)
				m_fFramesPerSecond = 0.0f;
			m_nFramesSinceCheck   = 0;
			m_nLastFPSUpdateTime = m_nTimeNow;
		}

		// Perform time scale
		m_fTimeDifference               *= m_fTimeScaleFactor;
		m_fTimeDifferenceNoCompensating *= m_fTimeScaleFactor;

		// Perform slow motion
		if (m_bSlowMotion) {
			m_fTimeDifference               *= m_fSlowMotionFactor*m_fCustomSlowMotionFactor;
			m_fTimeDifferenceNoCompensating *= m_fSlowMotionFactor*m_fCustomSlowMotionFactor;
		}

		// Try to compensate to abrupt timing slowdowns that may cause several problems
		float fDiff        = m_fTimeDifference-fPrevTimeDifference;
		float fMaxStepDiff = m_fMaxTimeDifference/3;
		if (fDiff > fMaxStepDiff)
			m_fTimeDifference = fMaxStepDiff+(fDiff-fMaxStepDiff)*0.1f;

		// Update the general timing
		m_fTimer += m_fTimeDifference;
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Past time                                             ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the past time in milliseconds since the application start
*/
uint64 Timing::GetPastTime() const
{
	return System::GetInstance()->GetMilliseconds() - m_nTimeStart;
}


//[-------------------------------------------------------]
//[ Time scale                                            ]
//[-------------------------------------------------------]
/**
*  @brief
*    Set freezed mode
*/
void Timing::Freeze(bool bFreeze)
{
	if (bFreeze) {
		if (!m_bFreezed) {
			m_bFreezed    = true;
			m_nFreezeTime = m_nTimeNow;
		}
	} else {
		if (m_bFreezed) { // Update timing
			const uint64 nTimeDifference = System::GetInstance()->GetMilliseconds() - m_nFreezeTime;
			m_nTimeLast			 += nTimeDifference;
			m_nLastFPSUpdateTime += nTimeDifference;
		}
		m_bFreezed = false;
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Timing::Timing() :
	m_bActive(true),
	m_nTimeStart(System::GetInstance()->GetMilliseconds()),
	m_nTimeNow(m_nTimeStart),
	m_nTimeLast(m_nTimeStart),
	m_fTimeDifference(0.0f),
	m_fTimeDifferenceNoCompensating(0.0f),
	m_fMaxTimeDifference(0.15f),
	m_fFramesPerSecond(0.0f),
	m_nFramesSinceCheck(0),
	m_nPastFrames(0),
	m_nLastFPSUpdateTime(m_nTimeStart),
	m_fFPSUpdateTimer(0.0f),
	m_bFreezed(false),
	m_nFreezeTime(0),
	m_bPause(false),
	m_fTimeScaleFactor(1.0f),
	m_bSlowMotion(true),
	m_fSlowMotionFactor(1.0f),
	m_fCustomSlowMotionFactor(1.0f),
	m_nFPSLimitLastTime(m_nTimeStart),
	m_fFPSLimit(0.0f),
	m_fTimer(0.0f)
{
}

/**
*  @brief
*    Copy constructor
*/
Timing::Timing(const Timing &cSource) :
	m_bActive(true),
	m_nTimeStart(System::GetInstance()->GetMilliseconds()),
	m_nTimeNow(m_nTimeStart),
	m_nTimeLast(m_nTimeStart),
	m_fTimeDifference(0.0f),
	m_fTimeDifferenceNoCompensating(0.0f),
	m_fMaxTimeDifference(0.15f),
	m_fFramesPerSecond(0.0f),
	m_nFramesSinceCheck(0),
	m_nPastFrames(0),
	m_nLastFPSUpdateTime(m_nTimeStart),
	m_fFPSUpdateTimer(0.0f),
	m_bFreezed(false),
	m_nFreezeTime(0),
	m_bPause(false),
	m_fTimeScaleFactor(1.0f),
	m_bSlowMotion(true),
	m_fSlowMotionFactor(1.0f),
	m_fCustomSlowMotionFactor(1.0f),
	m_nFPSLimitLastTime(m_nTimeStart),
	m_fFPSLimit(0.0f),
	m_fTimer(0.0f)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Destructor
*/
Timing::~Timing()
{
}

/**
*  @brief
*    Copy operator
*/
Timing &Timing::operator =(const Timing &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
