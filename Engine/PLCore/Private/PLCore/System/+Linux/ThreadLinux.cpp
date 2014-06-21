/*********************************************************\
 *  File: ThreadLinux.cpp                                *
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
#include <signal.h>
#include "PLCore/System/Mutex.h"
#include "PLCore/System/Thread.h"
#include "PLCore/System/ThreadLinux.h"

// [DEBUG]
#ifdef DEBUG
	#include <stdio.h>
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ThreadLinux::ThreadLinux(Thread &cThread, bool bThreadID, handle nThreadID) :
	ThreadImpl(cThread),
	m_nThreadID(0),
	m_pMutex(nullptr),
	m_nPriorityClass(Thread::NormalPriorityClass),
	m_nPriority(Thread::NormalPriority)
{
	// Shall the current thread ID be obtained?
	// [TODO] Implement nThreadID != NULL_HANDLE to open existing threads
	if (bThreadID && nThreadID == NULL_HANDLE) {
		// Get current thread
		m_nThreadID = pthread_self();
	} else {
		// Create mutex
		m_pMutex = new Mutex();
	}
}

/**
*  @brief
*    Destructor
*/
ThreadLinux::~ThreadLinux()
{
	// Delete mutex
	if (m_pMutex)
		delete m_pMutex;
}


//[-------------------------------------------------------]
//[ Private virtual ThreadImpl functions                  ]
//[-------------------------------------------------------]
handle ThreadLinux::GetID() const
{
	// Return thread ID
	// Please note that we can't use C++ style casts in here across all supported platforms
	// -> E.g. "static_cast<handle>(m_nThreadID)" will be "const pthread_t {aka _opaque_pthread_t* const}" on Mac OS X and result in an compiler error
	// -> So, we stick with the classic C style cast
	return (handle)m_nThreadID;
}

bool ThreadLinux::IsActive() const
{
	// Check if the thread is active
	return (m_nThreadID > 0);
}

bool ThreadLinux::Start()
{
	// Check if the thread has already been started
	if (!m_nThreadID) {
		// Create thread
		const int nStatus = pthread_create(&m_nThreadID, nullptr, &RunThread, static_cast<void*>(&GetThread()));
		if (!nStatus) {
			// Success
			return true;
		}
	}

	// Error, could not start thread
	return false;
}

bool ThreadLinux::Terminate()
{
	// Check if the thread has been started
	if (m_nThreadID) {
		// [DEBUG]
		#ifdef DEBUG
			printf("\n\n[DEBUG] Terminate thread %d\n\n", m_nThreadID);
		#endif

		// "pthread_cancel()" is not supported by the Android NDK.
		//   "android-ndk-r6/docs/system/libc/OVERVIEW.html" states:
		//   "pthread_cancel() will *not* be supported in Bionic, because doing this would
		//   involve making the C library significantly bigger for very little benefit."
		#ifndef ANDROID
			pthread_cancel(m_nThreadID);
		#endif
		m_nThreadID = 0;

		// Unlock the execution mutex because the thread was terminated
		// -> the thread didn't unlock the mutex itself on exiting
		if (m_pMutex)
			m_pMutex->Unlock();

		// Done
		return true;
	}

	// Error, could not terminate thread
	return false;
}

bool ThreadLinux::Join()
{
	// Check if the thread has been started
	if (m_nThreadID) {
		// Wait for thread to stop
		if (!pthread_join(m_nThreadID, nullptr)) {
			// Release thread
			m_nThreadID = 0;

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

bool ThreadLinux::Join(uint64 nTimeout)
{
	// [TODO] We need a timed join here, which can be implemented e.g. like this:
	//        http://www.opengroup.org/onlinepubs/000095399/xrat/xsh_chap02.html#tag_03_02_08_21
	return Join();

	/*
	if (m_nThreadID) {
		if (m_pMutex && m_pMutex->Lock(nTimeout)) {
			// Unlock the execution mutex after locking it, because
			// if we could lock the mutex, the thread itself has stopped
			m_pMutex->Unlock();

			// Wait for thread to stop
			if (!pthread_join(m_nThreadID, nullptr)) {
				// Release thread
				m_nThreadID = 0;

				// Done
				return true;
			}
		}
	}

	// Error!
	return false;
	*/
}

uint32 ThreadLinux::GetPriorityClass() const
{
	return m_nPriorityClass;
}

bool ThreadLinux::SetPriorityClass(uint32 nPriorityClass)
{
	// Backup given setting
	m_nPriorityClass = nPriorityClass;

	// pthread_setschedparam: No implementation because by default the priority is managed directly by the kernel

	// Done
	return true;
}

uint32 ThreadLinux::GetPriority() const
{
	return m_nPriority;
}

bool ThreadLinux::SetPriority(uint32 nPriority)
{
	// Backup given setting
	m_nPriority = nPriority;

	// pthread_setschedparam: No implementation because by default the priority is managed directly by the kernel

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Private static functions                              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Static thread callback function
*/
void *ThreadLinux::RunThread(void *pParameter)
{
	if (pParameter) {
		// Get thread object
		Thread *pThread = static_cast<Thread*>(pParameter);

		// Lock the thread's execution mutex
		Mutex *pMutex = static_cast<ThreadLinux*>(pThread->m_pThreadImpl)->m_pMutex;
		pMutex->Lock();

		// Run thread
		int nRetVal = pThread->Run();

		// Unlock the thread's execution mutex
		pMutex->Unlock();

		// Done
		return reinterpret_cast<void*>(nRetVal);
	} else {
		// No return value
		return nullptr;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
