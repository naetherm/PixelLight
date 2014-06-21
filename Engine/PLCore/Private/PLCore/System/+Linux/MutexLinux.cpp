/*********************************************************\
 *  File: MutexLinux.cpp                                 *
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
#include "PLCore/System/MutexLinux.h"


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
MutexLinux::MutexLinux()
{
	// Create system mutex
	pthread_mutex_init(&m_sMutex, nullptr);
}

/**
*  @brief
*    Destructor
*/
MutexLinux::~MutexLinux()
{
	// Destroy system mutex
	pthread_mutex_destroy(&m_sMutex);
}


//[-------------------------------------------------------]
//[ Private virtual MutexImpl functions                   ]
//[-------------------------------------------------------]
bool MutexLinux::Lock()
{
	// Lock mutex
	return !pthread_mutex_lock(&m_sMutex);
}

bool MutexLinux::TryLock(uint64 nTimeout)
{
	#if (ANDROID || APPLE)
		// There's no implementation of "pthread_mutex_timedlock()" within the Android NDK, even
		// if the function is listed within the headers. Depending on the API level, it's defined
		// out, meaning no compiler error when using API level 8, but a compiler error when using API
		// level 9 (some sources stating this is an error within the shipped "pthread.h"-header).

		// Apple has no "pthread_mutex_lock()", too

		// Lock mutex - just use the version without timeout :/
		return !pthread_mutex_lock(&m_sMutex);
	#else
		// Setup timeout structure
		struct timespec timeout;
		timeout.tv_sec  = nTimeout/1000;
		timeout.tv_nsec = (nTimeout-timeout.tv_sec)*1000;

		// Lock mutex
		return !pthread_mutex_timedlock(&m_sMutex, &timeout);
	#endif
}

bool MutexLinux::Unlock()
{
	// Release mutex
	return !pthread_mutex_unlock(&m_sMutex);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
