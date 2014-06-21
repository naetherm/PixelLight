/*********************************************************\
 *  File: Semaphore.inl                                  *
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
#include "PLCore/System/SemaphoreImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Locks the semaphore
*/
inline bool Semaphore::Lock()
{
	// Lock semaphore
	if (m_pSemaphoreImpl->Lock()) {
		// Success
		m_nValue--;
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Locks the semaphore, but only wait until timeout
*/
inline bool Semaphore::TryLock(uint64 nTimeout)
{
	// Lock semaphore
	if (m_pSemaphoreImpl->TryLock(nTimeout)) {
		// Success
		m_nValue--;
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Unlocks the semaphore
*/
inline bool Semaphore::Unlock()
{
	// Release semaphore
	m_nValue++;
	if (m_pSemaphoreImpl->Unlock()) {
		// Success
		return true;
	}

	// Error!
	m_nValue--;
	return false;
}

/**
*  @brief
*    Gets the current value of the semaphore
*/
inline uint32 Semaphore::GetValue() const
{
	// Return semaphore value
	return m_nValue;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
