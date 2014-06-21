/*********************************************************\
 *  File: WorldThread.cpp                                *
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
#include <PLCore/Tools/Timing.h>
#include <PLCore/System/System.h>
#include <PLCore/System/MutexGuard.h>
#include "PLPhysicsNewton/World.h"
#include "PLPhysicsNewton/WorldThread.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLPhysicsNewton {


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
WorldThread::WorldThread(World &cWorld) : WorldUpdate(cWorld),
	m_pMutex(new Mutex()),
	m_bShutDown(false),
	m_fTimeScaleFactor(Timing::GetInstance()->GetTimeScaleFactor()),
	m_fMaxTimeDifference(Timing::GetInstance()->GetMaxTimeDifference()),
	m_fSimulationSpeed(cWorld.m_fSimulationSpeed),
	m_fFrameRate(cWorld.m_fFrameRate)
{
	// Setup priority
	SetPriorityClass(static_cast<Thread::EPriorityClass>(cWorld.m_nThreadPriorityClass));
	SetPriority(static_cast<Thread::EPriority>(cWorld.m_nThreadPriority));

	// Give the thread a neat name
	SetName("PixelLightNewtonGameDynamicsWorldUpdate");

	// Lock the thread?
	if (!cWorld.m_bSimulationActive)
		m_pMutex->Lock();

	// Let's rock and roll! ;-)
	Start();
}

/**
*  @brief
*    Destructor
*/
WorldThread::~WorldThread()
{
	// Stop the physics simulation
	m_pMutex->Lock();
		m_bShutDown = true;
	m_pMutex->Unlock();
	Join(100); // Use timeout, else this may block forever...

	// Destroy the synchronization object
	delete m_pMutex;
}


//[-------------------------------------------------------]
//[ Private virtual WorldUpdate functions                 ]
//[-------------------------------------------------------]
bool WorldThread::IsThread()
{
	return true;
}

bool WorldThread::Lock()
{
	// Lock the synchronization object
	return m_pMutex->Lock();
}

bool WorldThread::Update(float fTimeScaleFactor, float fMaxTimeDifference, float fSimulationSpeed, float fFrameRate)
{
	// Just update settings - the physics itself is NOT updated in here!
	m_fTimeScaleFactor   = fTimeScaleFactor;
	m_fMaxTimeDifference = fMaxTimeDifference;
	m_fSimulationSpeed   = fSimulationSpeed;
	m_fFrameRate         = fFrameRate;

	// Done
	return true;
}

bool WorldThread::Unlock()
{
	// Unlock the synchronization object
	return m_pMutex->Unlock();
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Thread functions               ]
//[-------------------------------------------------------]
int WorldThread::Run()
{
	// Simulation loop
	while (!m_bShutDown) {
		// Currently paused?
		if (Timing::GetInstance()->IsPaused()) {
			m_nLastTime = System::GetInstance()->GetMilliseconds();
		} else {
			// Lock the synchronization object
			const MutexGuard cMutexGuard(*m_pMutex);

			// Update the physics
			WorldUpdate::Update(m_fTimeScaleFactor, m_fMaxTimeDifference, m_fSimulationSpeed, m_fFrameRate);
		}

		// Sleep...
		System::GetInstance()->Sleep(static_cast<uint32>(1000.0f/m_fFrameRate));
	}

	// Done
	return 0;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsNewton
