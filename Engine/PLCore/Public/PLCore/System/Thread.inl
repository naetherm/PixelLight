/*********************************************************\
 *  File: Thread.inl                                     *
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
#include "PLCore/System/ThreadImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the name of the thread
*/
inline String Thread::GetName() const
{
	return m_sName;
}

/**
*  @brief
*    Sets the name of the thread
*/
inline void Thread::SetName(const String &sName)
{
	m_sName = sName;
}

/**
*  @brief
*    Returns the unique system ID of the thread
*/
inline handle Thread::GetID() const
{
	// Call system implementation
	return m_pThreadImpl->GetID();
}

/**
*  @brief
*    Returns whether the thread is active
*/
inline bool Thread::IsActive() const
{
	// Call system implementation
	return m_pThreadImpl->IsActive();
}

/**
*  @brief
*    Starts the execution of the thread
*/
inline bool Thread::Start()
{
	// Call system implementation
	return m_pThreadImpl->Start();
}

/**
*  @brief
*    Stops the execution of the thread
*/
inline bool Thread::Terminate()
{
	// Call system implementation
	return m_pThreadImpl->Terminate();
}

/**
*  @brief
*    Waits until the thread has been stopped
*/
inline bool Thread::Join()
{
	// Call system implementation
	return m_pThreadImpl->Join();
}

/**
*  @brief
*    Waits until the thread has been stopped
*/
inline bool Thread::Join(uint64 nTimeout)
{
	// Call system implementation
	return m_pThreadImpl->Join(nTimeout);
}

/**
*  @brief
*    Returns the priority class the thread is in
*/
inline Thread::EPriorityClass Thread::GetPriorityClass() const
{
	// Call system implementation
	return static_cast<Thread::EPriorityClass>(m_pThreadImpl->GetPriorityClass());
}

/**
*  @brief
*    Sets the priority class the thread is in
*/
inline bool Thread::SetPriorityClass(EPriorityClass nPriorityClass)
{
	// Call system implementation
	return m_pThreadImpl->SetPriorityClass(nPriorityClass);
}

/**
*  @brief
*    Returns the thread priority within the priority class it is in
*/
inline Thread::EPriority Thread::GetPriority() const
{
	// Call system implementation
	return static_cast<Thread::EPriority>(m_pThreadImpl->GetPriority());
}

/**
*  @brief
*    Sets the thread priority within the priority class it is in
*/
inline bool Thread::SetPriority(EPriority nPriority)
{
	// Call system implementation
	return m_pThreadImpl->SetPriority(nPriority);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
