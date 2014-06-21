/*********************************************************\
 *  File: MutexGuard.inl                                 *
 *      Mutex guard implementation
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
#include "PLCore/System/Mutex.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
MutexGuard::MutexGuard(Mutex &cMutex) :
	m_pMutex(&cMutex)
{
	// Lock the given mutex, or at least try it! ("usually", this lock is ensuring that we're getting only out of here if we've got a lock!)
	m_bLocked = cMutex.Lock();
}

/**
*  @brief
*    Constructor
*/
MutexGuard::MutexGuard(Mutex &cMutex, uint64 nTimeout) :
	m_pMutex(&cMutex)
{
	// Lock the given mutex, or at least try it!
	m_bLocked = cMutex.TryLock(nTimeout);
}

/**
*  @brief
*    Destructor
*/
MutexGuard::~MutexGuard()
{
	// If the mutex was locked successfully, unlock it right now!
	if (m_bLocked)
		m_pMutex->Unlock();
}

/**
*  @brief
*    Returns whether the used mutex was locked successfully
*/
bool MutexGuard::IsLocked() const
{
	return m_bLocked;
}

/**
*  @brief
*    Returns the used mutex
*/
Mutex &MutexGuard::GetMutex() const
{
	return *m_pMutex;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
MutexGuard::MutexGuard(const MutexGuard &cSource) :
	m_pMutex(nullptr),
	m_bLocked(false)
{
	// Not implemented
}

/**
*  @brief
*    Copy operator
*/
MutexGuard &MutexGuard::operator =(const MutexGuard &cSource)
{
	// Not implemented
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
