/*********************************************************\
 *  File: SemaphoreImpl.h                                *
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


#ifndef __PLCORE_SEMAPHORE_IMPL_H__
#define __PLCORE_SEMAPHORE_IMPL_H__
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
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract base class for platform specific 'Semaphore' implementations
*
*  @note
*    - Implementation of the bridge design pattern, this class is the implementor of the 'Semaphore' abstraction
*/
class SemaphoreImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Semaphore;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		SemaphoreImpl();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SemaphoreImpl();


	//[-------------------------------------------------------]
	//[ Protected virtual functions                           ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Locks the semaphore
		*
		*  @return
		*    'true' if successful, 'false' on error
		*/
		virtual bool Lock() = 0;

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
		*    - 'nTimeout = 0' means no timeout at all
		*/
		virtual bool TryLock(uint64 nTimeout) = 0;

		/**
		*  @brief
		*    Unlocks the semaphore
		*
		*  @return
		*    'true' if successful, 'false' on error
		*/
		virtual bool Unlock() = 0;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_SEMAPHORE_IMPL_H__
