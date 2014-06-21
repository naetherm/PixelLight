/*********************************************************\
 *  File: Timer.cpp                                      *
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
#include "PLGui/Gui/Gui.h"
#include "PLGui/Gui/Resources/Timer.h"
#include "PLGui/Gui/Resources/TimerThread.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLGui {


//[-------------------------------------------------------]
//[ Private static data                                   ]
//[-------------------------------------------------------]
uint32 Timer::m_nNextID = 1000;


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Timer::Timer(Gui &cGui) :
	m_pGui(&cGui),
	m_nID(++m_nNextID),
	m_pThread(nullptr),
	m_bActive(false),
	m_nTimeout(0)
{
	// Add timer to GUI
	m_pGui->AddTimer(this);
}

/**
*  @brief
*    Destructor
*/
Timer::~Timer()
{
	// Remove timer from GUI
	m_pGui->RemoveTimer(this);

	// Stop timer
	Stop();
}

/**
*  @brief
*    Get owner GUI
*/
Gui *Timer::GetGui() const
{
	// Return GUI
	return m_pGui;
}

/**
*  @brief
*    Get timer ID
*/
uint32 Timer::GetID() const
{
	// Return timer ID
	return m_nID;
}

/**
*  @brief
*    Set timer ID
*/
void Timer::SetID(uint32 nID)
{
	// Set timer ID
	m_nID = nID;
}

/**
*  @brief
*    Check if timer is active
*/
bool Timer::IsActive() const
{
	// Return if the timer is active
	return m_bActive;
}

/**
*  @brief
*    Check if timer fires only once
*/
bool Timer::IsOnce() const
{
	// Return if the timer is firing only once
	return m_bOnce;
}

/**
*  @brief
*    Get timeout interval
*/
uint64 Timer::GetTimeout() const
{
	// Return timeout
	return m_nTimeout;
}

/**
*  @brief
*    Start timer
*/
void Timer::Start(uint64 nTimeout)
{
	// Stop timer
	if (IsActive()) Stop();

	// Save options
	m_nTimeout	= nTimeout;
	m_bActive	= true;
	m_bOnce		= false;

	// Create timer thread
	m_pThread = new TimerThread(*this, m_nTimeout, m_bOnce);
	m_pThread->Start();

	// Emit Start-Event
	EventStart();
}

/**
*  @brief
*    Start timer only once, not periodically
*/
void Timer::StartOnce(uint64 nTimeout)
{
	// Stop timer
	if (IsActive()) Stop();

	// Save options
	m_nTimeout	= nTimeout;
	m_bActive	= true;
	m_bOnce		= true;

	// Create timer thread
	m_pThread = new TimerThread(*this, m_nTimeout, m_bOnce);
	m_pThread->Start();

	// Emit Start-Event
	EventStart();
}

/**
*  @brief
*    Stop timer
*/
void Timer::Stop()
{
	// Is timer active?
	if (IsActive()) {
		// Stop timer thread
		m_pThread->StopTimer();
		delete m_pThread;
		m_pThread = nullptr;

		// Emit Stop-Event
		EventStop();

		// Timer stopped
		m_bActive = false;
	}
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Fire timer
*/
void Timer::Fire()
{
	// Emit event
	EventFire();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
