/*********************************************************\
 *  File: TimerThread.cpp                                *
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
#include <PLCore/System/CriticalSection.h>
#include "PLGui/Gui/Base/GuiMessage.h"
#include "PLGui/Gui/Resources/Timer.h"
#include "PLGui/Gui/Resources/TimerThread.h"
#include "PLGui/Gui/Gui.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLGui {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
TimerThread::TimerThread(Timer &cTimer, uint64 nTimeout, bool bOnce) :
	m_cTimer(cTimer),
	m_pCriticalSection(new CriticalSection()),
	m_nTimeout(nTimeout),
	m_bOnce(bOnce),
	m_bShutdown(false)
{
}

/**
*  @brief
*    Destructor
*/
TimerThread::~TimerThread()
{
	// Delete critical section
	delete m_pCriticalSection;
}

/**
*  @brief
*    Stop timer thread
*/
void TimerThread::StopTimer()
{
	// Deactivate timer
	m_pCriticalSection->Lock();
	m_bShutdown = true;
	m_pCriticalSection->Unlock();

	// Wait for thread to exit
	if (!Join(200)) {
		// We asked politely, but the thread won't listen... so pull out the gun!
		// (the internal platform implementation may or may not accept this violent act)
		Terminate();
		m_pCriticalSection->Unlock();
	}
}

/**
*  @brief
*    Fire timer
*/
void TimerThread::Fire()
{
	// Send timer message
	m_cTimer.GetGui()->PostMessage(GuiMessage::OnTimer(&m_cTimer));
}


//[-------------------------------------------------------]
//[ Private virtual ThreadFunction functions              ]
//[-------------------------------------------------------]
int TimerThread::Run()
{
	// Start timer loop
	bool bRunning = true;
	do {
		// Wait for timeout
		System::GetInstance()->Sleep(m_nTimeout);

		// Timer fired
		Fire();

		// Check if timer is still active
		m_pCriticalSection->Lock();
		if (m_bOnce)	 bRunning = false;
		if (m_bShutdown) bRunning = false;
		m_pCriticalSection->Unlock();
	} while (bRunning);

	// Timer is no longer active
	m_cTimer.m_bActive = false;

	// Done
	return 0;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
