/*********************************************************\
 *  File: Semaphore.h                                    *
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


#ifndef __PLCORE_SEMAPHORE_H__
#define __PLCORE_SEMAPHORE_H__
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
class SemaphoreImpl;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Counting semaphore class
*
*  @note
*    - Implementation of the bridge design pattern, this class is the abstraction
*/
class Semaphore {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nValue
		*    Initial value of the semaphore (usually >0)
		*  @param[in] nMaxValue
		*    Maximum value of the semaphore
		*/
		PLCORE_API Semaphore(uint32 nValue, uint32 nMaxValue);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API ~Semaphore();

		/**
		*  @brief
		*    Locks the semaphore
		*
		*  @return
		*    'true' if successful, 'false' on error
		*
		*  @note
		*    - Other known names for this operation: p, wait, acquire, down
		*    - Blocking if no 'signal' left
		*    - Use the lock method with a timeout to avoid potential deadlocks
		*/
		inline bool Lock();

		/**
		*  @brief
		*    Locks the semaphore, but only wait until timeout
		*
		*  @param[in] nTimeout
		*    Timeout in milliseconds
		*
		*  @return
		*    'true' if successful, 'false' on error
		*
		*  @note
		*    - 'nTimeout = 0' means: Return immediately if no 'signal' left
		*
		*  @see
		*    - Lock()
		*/
		inline bool TryLock(uint64 nTimeout);

		/**
		*  @brief
		*    Unlocks the semaphore
		*
		*  @return
		*    'true' if successful, 'false' on error
		*
		*  @note
		*    - Other known names for this operation: v, signal, release, up
		*/
		inline bool Unlock();

		/**
		*  @brief
		*    Gets the current value of the semaphore
		*
		*  @return
		*    Current semaphore value (number of 'signals' left)
		*/
		inline uint32 GetValue() const;


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
		Semaphore(const Semaphore &cSource);

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
		Semaphore &operator =(const Semaphore &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		SemaphoreImpl *m_pSemaphoreImpl;	/**< Pointer to the system specific implementation (assumed to be never a null pointer!) */
		uint32         m_nValue;			/**< Current value of the semaphore */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/System/Semaphore.inl"


#endif // __PLCORE_SEMAPHORE_H__
