/*********************************************************\
 *  File: ThreadWindows.cpp                              *
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
#include "PLCore/System/Thread.h"
#include "PLCore/System/+Windows/ThreadWindows.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Global variables                                      ]
//[-------------------------------------------------------]
extern DWORD g_nTlsIndex; // 'Thread Local Storage' (TLS), TLS_OUT_OF_INDEXES on error


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ThreadWindows::ThreadWindows(Thread &cThread, bool bThreadID, handle nThreadID) :
	ThreadImpl(cThread),
	m_hThread(nullptr),
	m_nThreadID(0)
{
	// Open existing thread?
	if (bThreadID) {
		// Given thread ID or current thread?
		if (nThreadID == NULL_HANDLE) {
			// Duplicate current thread handle
			HANDLE hThread = GetCurrentThread();
			if (DuplicateHandle(GetCurrentProcess(), hThread,
								GetCurrentProcess(), &m_hThread,
								0, false, DUPLICATE_SAME_ACCESS)) {
				// Success
				m_nThreadID = GetCurrentThreadId();
			}
		} else {
			// Open thread handle by ID
			m_hThread	= OpenThread(THREAD_ALL_ACCESS, false, nThreadID);
			m_nThreadID	= nThreadID;
		}
	}
}

/**
*  @brief
*    Destructor
*/
ThreadWindows::~ThreadWindows()
{
	// Close thread handle
	if (m_hThread)
		CloseHandle(m_hThread);
}


//[-------------------------------------------------------]
//[ Private virtual ThreadImpl functions                  ]
//[-------------------------------------------------------]
handle ThreadWindows::GetID() const
{
	// Return thread ID
	return m_nThreadID;
}

bool ThreadWindows::IsActive() const
{
	// Check if the thread is active
	return (m_hThread != nullptr);
}

bool ThreadWindows::Start()
{
	// Check if the thread has already been started
	if (!m_hThread) {
		// Create thread
		m_hThread = ::CreateThread(nullptr, 0, ThreadProc, static_cast<void*>(&GetThread()), 0, &m_nThreadID);
		if (m_hThread)
			return true; // Done
	}

	// Error, could not start thread
	return false;
}

bool ThreadWindows::Terminate()
{
	// Check if the thread has been started
	if (m_hThread) {
		// Terminate thread - we warned the user within the method documentation that this does not allow proper thread clean up...
		if (TerminateThread(m_hThread, 0)) {
			// Termination successful
			m_hThread = nullptr;
			return true;
		}
	}

	// Error, could not terminate thread
	return false;
}

bool ThreadWindows::Join()
{
	// Check if the thread has been started
	if (m_hThread) {
		// Wait for thread to stop
		if (WaitForSingleObject(m_hThread, INFINITE) == WAIT_OBJECT_0) {
			// Release thread
			CloseHandle(m_hThread);
			m_hThread = nullptr;

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

bool ThreadWindows::Join(uint64 nTimeout)
{
	// Check if the thread has been started
	if (m_hThread) {
		// Wait for thread to stop
		if (WaitForSingleObject(m_hThread, static_cast<DWORD>(nTimeout)) == WAIT_OBJECT_0) {
			// Release thread
			CloseHandle(m_hThread);
			m_hThread = nullptr;

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

uint32 ThreadWindows::GetPriorityClass() const
{
	// Call OS function
	const DWORD dwPriorityClass = ::GetPriorityClass(m_hThread);

	// Wrap the priority setting
	switch (dwPriorityClass) {
		case IDLE_PRIORITY_CLASS:			return Thread::IdlePriorityClass;
		case BELOW_NORMAL_PRIORITY_CLASS:	return Thread::BelowNormalPriorityClass;
		case NORMAL_PRIORITY_CLASS:			return Thread::NormalPriorityClass;
		case ABOVE_NORMAL_PRIORITY_CLASS:	return Thread::AboveNormalPriorityClass;
		case HIGH_PRIORITY_CLASS:			return Thread::HighPriorityClass;
		case REALTIME_PRIORITY_CLASS:		return Thread::RealtimePriorityClass;
		default:							return Thread::NormalPriorityClass; // Error!
	}
}

bool ThreadWindows::SetPriorityClass(uint32 nPriorityClass)
{
	// Wrap the priority setting
	DWORD dwPriorityClass;
	switch (nPriorityClass) {
		case Thread::IdlePriorityClass:			dwPriorityClass = IDLE_PRIORITY_CLASS;			break;
		case Thread::BelowNormalPriorityClass:	dwPriorityClass = BELOW_NORMAL_PRIORITY_CLASS;	break;
		case Thread::NormalPriorityClass:		dwPriorityClass = NORMAL_PRIORITY_CLASS;		break;
		case Thread::AboveNormalPriorityClass:	dwPriorityClass = ABOVE_NORMAL_PRIORITY_CLASS;	break;
		case Thread::HighPriorityClass:			dwPriorityClass = HIGH_PRIORITY_CLASS;			break;
		case Thread::RealtimePriorityClass:		dwPriorityClass = REALTIME_PRIORITY_CLASS;		break;
		default:								return false; // Error!
	}

	// Call OS function
	return (::SetPriorityClass(m_hThread, dwPriorityClass) != 0);
}

uint32 ThreadWindows::GetPriority() const
{
	// Call OS function
	const int nPriority = ::GetThreadPriority(m_hThread);

	// Wrap the priority setting
	switch (nPriority) {
		case THREAD_PRIORITY_IDLE:			return Thread::IdlePriority;
		case THREAD_PRIORITY_LOWEST:		return Thread::LowestPriority;
		case THREAD_PRIORITY_BELOW_NORMAL:	return Thread::BelowNormalPriority;
		case THREAD_PRIORITY_NORMAL:		return Thread::NormalPriority;
		case THREAD_PRIORITY_ABOVE_NORMAL:	return Thread::AboveNormalPriority;
		case THREAD_PRIORITY_HIGHEST:		return Thread::HighestPriority;
		case THREAD_PRIORITY_TIME_CRITICAL:	return Thread::TimeCriticalPriority;
		default:							return Thread::NormalPriority; // Error!
	}
}

bool ThreadWindows::SetPriority(uint32 nPriority)
{
	// Wrap the priority setting
	int nWindowsPriority;
	switch (nPriority) {
		case Thread::IdlePriority:			nWindowsPriority = THREAD_PRIORITY_IDLE;			break;
		case Thread::LowestPriority:		nWindowsPriority = THREAD_PRIORITY_LOWEST;			break;
		case Thread::BelowNormalPriority:	nWindowsPriority = THREAD_PRIORITY_BELOW_NORMAL;	break;
		case Thread::NormalPriority:		nWindowsPriority = THREAD_PRIORITY_NORMAL;			break;
		case Thread::AboveNormalPriority:	nWindowsPriority = THREAD_PRIORITY_ABOVE_NORMAL;	break;
		case Thread::HighestPriority:		nWindowsPriority = THREAD_PRIORITY_HIGHEST;			break;
		case Thread::TimeCriticalPriority:	nWindowsPriority = THREAD_PRIORITY_TIME_CRITICAL;	break;
		default:							return false; // Error!
	}

	// Call OS function
	return (::SetThreadPriority(m_hThread, nWindowsPriority) != 0);
}


//[-------------------------------------------------------]
//[ Private static functions                              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Static thread callback function
*/
DWORD WINAPI ThreadWindows::ThreadProc(LPVOID lpParameter)
{
	// Get the PixelLight thread instance
	Thread *pThread = static_cast<Thread*>(lpParameter);
	if (pThread) {
		// Initialize the TLS index for this thread so we can find out the PixelLight
		// thread instance within 'SystemWindows::GetCurrentThread()'
		if (g_nTlsIndex != TLS_OUT_OF_INDEXES && TlsSetValue(g_nTlsIndex, pThread)) {
			// Run the thread
			return pThread->Run();
		}
	}

	// Error!
	return 0;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
