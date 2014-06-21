/*********************************************************\
 *  File: Mutex.h                                        *
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


#ifndef __PLCORE_MUTEX_H__
#define __PLCORE_MUTEX_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/PLCore.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class MutexImpl;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Mutex (Mutual Exclusion, binary semaphore) class
*
*  @note
*    - Implementation of the bridge design pattern, this class is the abstraction
*    - The mutex is non-recursive, meaning that you have to be careful to avoid creating a "self-deadlock" by calling the lock method
*      multiple times directly or indirectly by other method calls
*    - It's a good idea to lock/unlock a mutex by using the MutexGuard helper class on the runtime stack so there's always an unlock for each lock!
*/
class Mutex {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @note
		*    - A new constructed mutex is unlocked by default
		*/
		PLCORE_API Mutex();

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] bCriticalSection
		*    If supported by the platform: 'true' to use "Critical Section" (lock/unlock only inside the same process) instead of "Mutex" (lock/unlock across multiple processes) implementation, else 'false'
		*
		*  @remarks
		*    Platforms handle mutual exclusion implementations differently. POSIX only offers "Mutex", while MS Windows
		*    makes a difference between "Mutex" (lock/unlock across multiple processes) and "Critical Section"
		*    (lock/unlock only inside the same process).
		*
		*  @note
		*    - A new constructed mutex is unlocked by default
		*/
		PLCORE_API Mutex(bool bCriticalSection);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API ~Mutex();

		/**
		*  @brief
		*    Locks the mutex
		*
		*  @return
		*    'true' if successful, 'false' on error
		*
		*  @note
		*    - Blocking if already locked
		*    - Use the lock method with a timeout to avoid potential deadlocks
		*    - In the literature, this operation is also known as "acquire"
		*/
		inline bool Lock();

		/**
		*  @brief
		*    Locks the mutex, but only wait until timeout
		*
		*  @param[in] nTimeout
		*    Timeout in milliseconds
		*
		*  @return
		*    'true' if successful, 'false' on error
		*
		*  @note
		*    - 'nTimeout = 0' means: Return immediately if already locked
		*
		*  @see
		*    - Lock()
		*/
		inline bool TryLock(uint64 nTimeout);

		/**
		*  @brief
		*    Unlocks the mutex
		*
		*  @return
		*    'true' if successful, 'false' on error
		*
		*  @note
		*    - In the literature, this operation is also known as "release"
		*/
		inline bool Unlock();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Mutex(const Mutex &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Mutex &operator =(const Mutex &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MutexImpl *m_pMutexImpl;	/**< Pointer to the system specific implementation (assumed to be never a null pointer!) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/System/Mutex.inl"


#endif // __PLCORE_MUTEX_H__
