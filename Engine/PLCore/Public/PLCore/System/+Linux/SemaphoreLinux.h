/*********************************************************\
 *  File: SemaphoreLinux.h                               *
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


#ifndef __PLCORE_SEMAPHORE_LINUX_H__
#define __PLCORE_SEMAPHORE_LINUX_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <semaphore.h>
#include "PLCore/System/SemaphoreImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Linux 'Semaphore' implementation
*/
class SemaphoreLinux : public SemaphoreImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Semaphore;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nValue
		*    Initial value of the semaphore
		*  @param[in] nMaxValue
		*    Maximum value of the semaphore
		*/
		SemaphoreLinux(uint32 nValue, uint32 nMaxValue);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SemaphoreLinux();


	//[-------------------------------------------------------]
	//[ Private virtual SemaphoreImpl functions               ]
	//[-------------------------------------------------------]
	private:
		virtual bool Lock() override;
		virtual bool TryLock(uint64 nTimeout) override;
		virtual bool Unlock() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		sem_t	m_hSemaphore;	/**< System semaphore handle */
		uint32	m_nMaxValue;	/**< Maximum value of the semaphore */
		bool	m_bCreated;		/**< Indicates if the semaphore was created */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_SEMAPHORE_LINUX_H__
