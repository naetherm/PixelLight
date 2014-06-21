/*********************************************************\
 *  File: Mutex.cpp                                      *
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
#if defined(WIN32)
	#include "PLCore/System/+Windows/MutexWindows_Mutex.h"
	#include "PLCore/System/+Windows/MutexWindows_CriticalSection.h"
#elif defined(LINUX)
	#include "PLCore/System/+Linux/MutexLinux.h"
#endif
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
Mutex::Mutex() :
	m_pMutexImpl(nullptr)
{
	// Create system implementation for the right platform
	#if defined(WIN32)
		// Create Windows implementation - Windows has two different mutex implementations:
		// - "Mutex":            Lock/unlock across multiple processes
		// - "Critical Section": Lock/unlock only inside the same process
		// -> Default has to be "Mutex" to be on the safe universal side
		m_pMutexImpl = new MutexWindows_Mutex();
	#elif defined(LINUX)
		// Create Linux implementation
		m_pMutexImpl = new MutexLinux();
	#else
		// Unknown system
		#error "Unsupported platform"
	#endif
}

/**
*  @brief
*    Constructor
*/
Mutex::Mutex(bool bCriticalSection)
{
	// Create system implementation for the right platform
	#if defined(WIN32)
		// Create Windows implementation - Windows has two different mutex implementations:
		// - "Mutex":            Lock/unlock across multiple processes
		// - "Critical Section": Lock/unlock only inside the same process
		if (bCriticalSection)
			m_pMutexImpl = new MutexWindows_CriticalSection();
		else
			m_pMutexImpl = new MutexWindows_Mutex();
	#elif defined(LINUX)
		// Create Linux implementation
		m_pMutexImpl = new MutexLinux();
	#else
		// Unknown system
		#error "Unsupported platform"
	#endif
}

/**
*  @brief
*    Destructor
*/
Mutex::~Mutex()
{
	// Destroy system specific implementation
	if (m_pMutexImpl)
		delete m_pMutexImpl;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Mutex::Mutex(const Mutex &cSource) :
	m_pMutexImpl(nullptr)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
Mutex &Mutex::operator =(const Mutex &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
