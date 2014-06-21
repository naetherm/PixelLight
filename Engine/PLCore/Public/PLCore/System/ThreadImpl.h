/*********************************************************\
 *  File: ThreadImpl.h                                   *
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


#ifndef __PLCORE_THREAD_IMPL_H__
#define __PLCORE_THREAD_IMPL_H__
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
class Thread;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract base class for platform specific 'Thread' implementations
*
*  @note
*    - Implementation of the bridge design pattern, this class is the implementor of the 'Thread' abstraction
*/
class ThreadImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Thread;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cThread
		*    Reference to the owning thread
		*/
		ThreadImpl(Thread &cThread);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ThreadImpl();

		/**
		*  @brief
		*    Returns a reference to the owning thread
		*
		*  @return
		*    Reference to the owning thread
		*/
		Thread &GetThread() const;


	//[-------------------------------------------------------]
	//[ Protected virtual ThreadImpl functions                ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Returns the unique system ID of the thread
		*
		*  @return
		*    Thread ID
		*/
		virtual handle GetID() const = 0;

		/**
		*  @brief
		*    Returns whether the thread is active
		*
		*  @return
		*    'true' if the thread is active
		*/
		virtual bool IsActive() const = 0;

		/**
		*  @brief
		*    Starts the execution of the thread
		*
		*  @return
		*    'true' if the thread could be started
		*/
		virtual bool Start() = 0;

		/**
		*  @brief
		*    Stops the execution of the thread
		*
		*  @return
		*    'true' if the thread could be stopped
		*
		*  @remarks
		*    Terminates the thread ungracefully (does not allow proper thread clean up!). Instead of
		*    using this function you should signal the thread and wait until it has quit by itself.
		*    The internal platform implementation may or may not accept this violent act. For example,
		*    Androids Bionic doesn't support it and a call of this method will have no effect at all.
		*/
		virtual bool Terminate() = 0;

		/**
		*  @brief
		*    Waits until the thread has been stopped
		*
		*  @return
		*    'true' if the thread has been stopped
		*/
		virtual bool Join() = 0;

		/**
		*  @brief
		*    Waits until the thread has been stopped
		*
		*  @param[in] nTimeout
		*    Time in milliseconds to wait
		*
		*  @return
		*    'true' if the thread has been stopped
		*/
		virtual bool Join(uint64 nTimeout) = 0;

		/**
		*  @brief
		*    Returns the priority class the thread is in
		*
		*  @return
		*    The priority class the thread is in (type: Thread::EPriorityClass)
		*/
		virtual uint32 GetPriorityClass() const = 0;

		/**
		*  @brief
		*    Sets the priority class the thread is in
		*
		*  @param[in] nPriorityClass
		*    New priority class the thread is in (type: Thread::EPriorityClass)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool SetPriorityClass(uint32 nPriorityClass) = 0;

		/**
		*  @brief
		*    Returns the thread priority within the priority class it is in
		*
		*  @return
		*    The thread priority within the priority class it is in (type: Thread::EPriority)
		*/
		virtual uint32 GetPriority() const = 0;

		/**
		*  @brief
		*    Sets the thread priority within the priority class it is in
		*
		*  @param[in] nPriority
		*    New thread priority within the priority class it is in (type: Thread::EPriority)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool SetPriority(uint32 nPriority) = 0;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Thread *m_pThread;	/**< Pointer to the owning thread (always valid!) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_THREAD_IMPL_H__
