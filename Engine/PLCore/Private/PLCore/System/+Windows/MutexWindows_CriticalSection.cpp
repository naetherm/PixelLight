/*********************************************************\
 *  File: MutexWindows_CriticalSection.cpp               *
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
#include "PLCore/System/+Windows/MutexWindows_CriticalSection.h"


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
MutexWindows_CriticalSection::MutexWindows_CriticalSection()
{
	// Initialize system critical section
	InitializeCriticalSectionAndSpinCount(&m_hCriticalSection, 0x00000400);
}

/**
*  @brief
*    Destructor
*/
MutexWindows_CriticalSection::~MutexWindows_CriticalSection()
{
	// Release system critical section
	DeleteCriticalSection(&m_hCriticalSection);
}


//[-------------------------------------------------------]
//[ Private virtual MutexImpl functions                   ]
//[-------------------------------------------------------]
bool MutexWindows_CriticalSection::Lock()
{
	// Lock critical section
	EnterCriticalSection(&m_hCriticalSection);

	// Done
	return true;
}

bool MutexWindows_CriticalSection::TryLock(uint64 nTimeout)
{
	// Lock critical section - there's no timeout version of critical section lock
	EnterCriticalSection(&m_hCriticalSection);

	// Done
	return true;
}

bool MutexWindows_CriticalSection::Unlock()
{
	// Release critical section
	LeaveCriticalSection(&m_hCriticalSection);

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
