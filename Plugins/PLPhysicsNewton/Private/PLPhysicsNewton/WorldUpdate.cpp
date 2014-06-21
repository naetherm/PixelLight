/*********************************************************\
 *  File: WorldUpdate.cpp                                *
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
#include <PLCore/System/System.h>
#include "PLPhysicsNewton/World.h"
#include "PLPhysicsNewton/WorldUpdate.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLPhysicsNewton {


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
WorldUpdate::WorldUpdate(World &cWorld) :
	m_pWorld(&cWorld),
	m_nTotalNumOfSteps(0),
	m_nLastTime(System::GetInstance()->GetMilliseconds()),
	m_fTimeElapsed(0.0f)
{
}


//[-------------------------------------------------------]
//[ Protected virtual WorldUpdate functions               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
WorldUpdate::~WorldUpdate()
{
}

/**
*  @brief
*    Returns whether or not the implementation is using a thread
*/
bool WorldUpdate::IsThread()
{
	return false;
}

/**
*  @brief
*    Locks the updated
*/
bool WorldUpdate::Lock()
{
	// Nothing to do in here

	// Done
	return true;
}

/**
*  @brief
*    Updates the physics
*/
bool WorldUpdate::Update(float fTimeScaleFactor, float fMaxTimeDifference, float fSimulationSpeed, float fFrameRate)
{
	// Get the current time
	const uint64 nTime = System::GetInstance()->GetMilliseconds();

	// Get the number of milliseconds past since the last slice
	const int64 nMilliseconds = nTime - m_nLastTime;

	// Get the number of seconds past since the last slice
	float fSeconds = static_cast<float>(nMilliseconds)/1000.0f*fTimeScaleFactor;
	if (fSeconds > fMaxTimeDifference)
		fSeconds = fMaxTimeDifference;
	m_fTimeElapsed += fSeconds;

	// If the time scale factor is smaller than 1, multiply the step size with this factor for smooth movement
	const float fFrequency = 1.0f/fFrameRate;
	const float fStepSize = (fTimeScaleFactor < 1.0f) ? fFrequency*fTimeScaleFactor*fSimulationSpeed : fFrequency*fSimulationSpeed;

	// Perform physics simulation
	while (m_fTimeElapsed > fStepSize) {
		// Physical world step
		Newton::NewtonUpdate(static_cast<World*>(m_pWorld)->GetNewtonWorld(), fStepSize);

		// Decrease elapsed time
		m_fTimeElapsed -= fStepSize;
	}

	// Update counter
	m_nLastTime = nTime;
	m_nTotalNumOfSteps++;

	// Done
	return true;
}

/**
*  @brief
*    Unlocks the updated
*/
bool WorldUpdate::Unlock()
{
	// Nothing to do in here

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsNewton
