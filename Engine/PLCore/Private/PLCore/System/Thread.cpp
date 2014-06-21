/*********************************************************\
 *  File: Thread.cpp                                     *
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
	#include "PLCore/System/+Windows/ThreadWindows.h"
#elif defined(LINUX)
	#include "PLCore/System/+Linux/ThreadLinux.h"
#endif
#include "PLCore/System/Thread.h"


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
Thread::Thread() :
	m_pThreadImpl(nullptr),
	m_pThreadFunction(nullptr),
	m_pStaticFunction(nullptr),
	m_pStaticData(nullptr)
{
	// Create system implementation for the right platform
	#if defined(WIN32)
		// Create Windows implementation
		m_pThreadImpl = new ThreadWindows(*this, false, NULL_HANDLE);
	#elif defined(LINUX)
		// Create Linux implementation
		m_pThreadImpl = new ThreadLinux(*this, false, NULL_HANDLE);
	#else
		// Unknown system
		#error "Unsupported platform"
	#endif
}

/**
*  @brief
*    Constructor
*/
Thread::Thread(ThreadFunction *pThreadFunction) :
	m_pThreadImpl(nullptr),
	m_pThreadFunction(pThreadFunction),
	m_pStaticFunction(nullptr),
	m_pStaticData(nullptr)
{
	// Create system implementation for the right platform
	#if defined(WIN32)
		// Create Windows implementation
		m_pThreadImpl = new ThreadWindows(*this, false, NULL_HANDLE);
	#elif defined(LINUX)
		// Create Linux implementation
		m_pThreadImpl = new ThreadLinux(*this, false, NULL_HANDLE);
	#else
		// Unknown system
		#error "Unsupported platform"
	#endif
}

/**
*  @brief
*    Constructor
*/
Thread::Thread(THREADFUNCTION pStaticFunction, void *pData) :
	m_pThreadImpl(nullptr),
	m_pThreadFunction(nullptr),
	m_pStaticFunction(pStaticFunction),
	m_pStaticData(pData)
{
	// Create system implementation for the right platform
	#if defined(WIN32)
		// Create Windows implementation
		m_pThreadImpl = new ThreadWindows(*this, false, NULL_HANDLE);
	#elif defined(LINUX)
		// Create Linux implementation
		m_pThreadImpl = new ThreadLinux(*this, false, NULL_HANDLE);
	#else
		// Unknown system
		#error "Unsupported platform"
	#endif
}

/**
*  @brief
*    Constructor
*/
Thread::Thread(handle nThreadID) :
	m_pThreadImpl(nullptr),
	m_pThreadFunction(nullptr),
	m_pStaticFunction(nullptr),
	m_pStaticData(nullptr)
{
	// Create system implementation for the right platform
	#if defined(WIN32)
		// Create Windows implementation
		m_pThreadImpl = new ThreadWindows(*this, true, nThreadID);
	#elif defined(LINUX)
		// Create Linux implementation
		m_pThreadImpl = new ThreadLinux(*this, true, nThreadID);
	#else
		// Unknown system
		#error "Unsupported platform"
	#endif
}

/**
*  @brief
*    Destructor
*/
Thread::~Thread()
{
	// Destroy system specific implementation
	if (m_pThreadImpl)
		delete m_pThreadImpl;
}


//[-------------------------------------------------------]
//[ Public virtual ThreadFunction functions               ]
//[-------------------------------------------------------]
int Thread::Run()
{
	// Determine type of thread function
	if (m_pThreadFunction) {
		// Call Run() method of thread function
		return m_pThreadFunction->Run();
	} else if (m_pStaticFunction) {
		// Call static thread function
		return m_pStaticFunction(m_pStaticData);
	} else {
		// Empty thread
		return 0;
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Thread::Thread(const Thread &cSource) :
	m_pThreadImpl(nullptr),
	m_pThreadFunction(nullptr),
	m_pStaticFunction(nullptr),
	m_pStaticData(nullptr)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
Thread &Thread::operator =(const Thread &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
