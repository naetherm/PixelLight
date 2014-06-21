/*********************************************************\
 *  File: MutexWindows_Mutex.cpp                         *
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
#include "PLCore/System/+Windows/MutexWindows_Mutex.h"


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
MutexWindows_Mutex::MutexWindows_Mutex() :
	m_hMutex(::CreateMutex(nullptr, false, nullptr))
{
}

/**
*  @brief
*    Destructor
*/
MutexWindows_Mutex::~MutexWindows_Mutex()
{
	// Destroy system mutex
	if (m_hMutex)
		CloseHandle(m_hMutex);
}


//[-------------------------------------------------------]
//[ Private virtual MutexImpl functions                   ]
//[-------------------------------------------------------]
bool MutexWindows_Mutex::Lock()
{
	// Lock mutex
	return (m_hMutex && WaitForSingleObject(m_hMutex, INFINITE) == WAIT_OBJECT_0);
}

bool MutexWindows_Mutex::TryLock(uint64 nTimeout)
{
	// Lock mutex
	return (m_hMutex && WaitForSingleObject(m_hMutex, static_cast<DWORD>(nTimeout)) == WAIT_OBJECT_0);
}

bool MutexWindows_Mutex::Unlock()
{
	// Release mutex
	return (m_hMutex && ReleaseMutex(m_hMutex));
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
