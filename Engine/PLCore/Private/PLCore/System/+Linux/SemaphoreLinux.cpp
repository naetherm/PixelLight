/*********************************************************\
 *  File: SemaphoreLinux.cpp                             *
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
#ifdef ANDROID
	#include <sys/types.h>	// For "time_t" used within "linux/time.h" required for "timespec" (yyyeeeehhhaaa! *g*)
	#include <linux/time.h>	// For "timespec", else we get the error "error: aggregate 'timespec timeout' has incomplete type and cannot be defined"
#endif
#include "PLCore/System/SemaphoreLinux.h"


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
SemaphoreLinux::SemaphoreLinux(uint32 nValue, uint32 nMaxValue) :
	m_nMaxValue(nMaxValue),
	m_bCreated(sem_init(&m_hSemaphore, 0, nValue) == 0)
{
}

/**
*  @brief
*    Destructor
*/
SemaphoreLinux::~SemaphoreLinux()
{
	// Destroy system semaphore
	if (m_bCreated)
		sem_destroy(&m_hSemaphore);
}


//[-------------------------------------------------------]
//[ Private virtual SemaphoreImpl functions               ]
//[-------------------------------------------------------]
bool SemaphoreLinux::Lock()
{
	// Lock semaphore
	return (m_bCreated && sem_wait(&m_hSemaphore) == 0);
}

bool SemaphoreLinux::TryLock(uint64 nTimeout)
{
	#ifdef APPLE
		// Apple doesn't provide "sem_timedwait()"

		// Lock semaphore
		return (m_bCreated && sem_wait(&m_hSemaphore) == 0);
	#else
		// Setup control structure
		struct timespec timeout;
		timeout.tv_sec  = nTimeout/1000;
		timeout.tv_nsec = (nTimeout-timeout.tv_sec)*1000;

		// Lock semaphore
		return (m_bCreated && sem_timedwait(&m_hSemaphore, &timeout) == 0);
	#endif
}

bool SemaphoreLinux::Unlock()
{
	// Get the current value of the semaphore
	int32 nSemaphoreValue = 0;
	const int nResult = sem_getvalue(&m_hSemaphore, &nSemaphoreValue);

	// Is the value -1 or greater or equal to the maximum value?
	// Note: If the value is greater or equal to the maximum value, then no unlock is needed because
	// the semaphore is already unlocked.
	if (nResult == 0 && nSemaphoreValue >= 0 && m_nMaxValue > static_cast<uint32>(nSemaphoreValue)) {
		// Release semaphore
		if (m_bCreated && sem_post(&m_hSemaphore) == 0)
			return true; // Success
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
