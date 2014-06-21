/*********************************************************\
 *  File: Thread.h                                       *
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


#ifndef __PLCORE_THREAD_H__
#define __PLCORE_THREAD_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/String.h"
#include "PLCore/System/ThreadFunction.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class ThreadImpl;


//[-------------------------------------------------------]
//[ Data types                                            ]
//[-------------------------------------------------------]
typedef int (*THREADFUNCTION)(void*);	/**< Static thread function */


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Thread class
*
*  @note
*    - Implementation of the bridge design pattern, this class is the abstraction
*/
class Thread : public ThreadFunction {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class System;
	friend class ThreadLinux;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Thread priority class
		*/
		enum EPriorityClass {
			IdlePriorityClass,			/**< Idle priority class */
			BelowNormalPriorityClass,	/**< Below normal priority class */
			NormalPriorityClass,		/**< Normal priority class (default) */
			AboveNormalPriorityClass,	/**< Above normal priority class */
			HighPriorityClass,			/**< High priority class */
			RealtimePriorityClass		/**< Realtime priority class (ONLY use this if you REALLY need it!) */
		};

		/**
		*  @brief
		*    Thread priority within the priority class it is in
		*/
		enum EPriority {
			IdlePriority,			/**< Idle priority */
			LowestPriority,			/**< Lowest priority */
			BelowNormalPriority,	/**< Below normal priority */
			NormalPriority,			/**< Normal priority (default) */
			AboveNormalPriority,	/**< Above normal priority */
			HighestPriority,		/**< Highest priority */
			TimeCriticalPriority	/**< Time critical priority */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API Thread();

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pThreadFunction
		*    Pointer to the thread function, can be a null pointer
		*/
		PLCORE_API Thread(ThreadFunction *pThreadFunction);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pThreadFunction
		*    Pointer to a static thread function, can be a null pointer
		*  @param[in] pData
		*    Data to be passed to the thread function, can be a null pointer
		*/
		PLCORE_API Thread(THREADFUNCTION pThreadFunction, void *pData);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nThreadID
		*    A system specific thread ID, NULL_HANDLE for current thread
		*/
		PLCORE_API Thread(handle nThreadID);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~Thread();

		/**
		*  @brief
		*    Returns the name of the thread
		*
		*  @return
		*    Thread name
		*
		*  @remarks
		*    The thread name is not used internally, it's for user usage only and especially
		*    for debugging purposes quite useful. It's recommended to give threads unique
		*    names. By default, no thread name is set.
		*/
		inline String GetName() const;

		/**
		*  @brief
		*    Sets the name of the thread
		*
		*  @param[in] sName
		*    Thread name
		*
		*  @see
		*    - GetName()
		*/
		inline void SetName(const String &sName);

		/**
		*  @brief
		*    Returns the unique system ID of the thread
		*
		*  @return
		*    Thread ID
		*/
		inline handle GetID() const;

		/**
		*  @brief
		*    Returns whether the thread is active
		*
		*  @return
		*    'true' if the thread is active, else 'false'
		*/
		inline bool IsActive() const;

		/**
		*  @brief
		*    Starts the execution of the thread
		*
		*  @return
		*    'true' if the thread could be started
		*/
		inline bool Start();

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
		inline bool Terminate();

		/**
		*  @brief
		*    Waits until the thread has been stopped
		*
		*  @return
		*    'true' if the thread has been stopped
		*
		*  @note
		*    - It's recommended to use the join version with a timeout instead of this function to
		*      ensure that the thread is stopped within a defined time (no deadlock's)
		*/
		inline bool Join();

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
		inline bool Join(uint64 nTimeout);

		/**
		*  @brief
		*    Returns the priority class the thread is in
		*
		*  @return
		*    The priority class the thread is in
		*
		*  @remarks
		*    The thread priority settings are only 'hints' for the OS how to deal with the thread. They
		*    are no 'facts' and may differ from OS to OS.
		*/
		inline EPriorityClass GetPriorityClass() const;

		/**
		*  @brief
		*    Sets the priority class the thread is in
		*
		*  @param[in] nPriorityClass
		*    New priority class the thread is in
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @see
		*    - GetPriorityClass()
		*/
		inline bool SetPriorityClass(EPriorityClass nPriorityClass = NormalPriorityClass);

		/**
		*  @brief
		*    Returns the thread priority within the priority class it is in
		*
		*  @return
		*    The thread priority within the priority class it is in
		*
		*  @see
		*    - GetPriorityClass()
		*/
		inline EPriority GetPriority() const;

		/**
		*  @brief
		*    Sets the thread priority within the priority class it is in
		*
		*  @param[in] nPriority
		*    New thread priority within the priority class it is in
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @see
		*    - GetPriorityClass()
		*/
		inline bool SetPriority(EPriority nPriority = NormalPriority);


	//[-------------------------------------------------------]
	//[ Public virtual ThreadFunction functions               ]
	//[-------------------------------------------------------]
	public:
		PLCORE_API virtual int Run() override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Thread(const Thread &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Thread &operator =(const Thread &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ThreadImpl	   *m_pThreadImpl;		/**< Pointer to the system specific implementation (assumed to be never a null pointer!) */
		ThreadFunction *m_pThreadFunction;	/**< Pointer to a thread function to execute, can be a null pointer */
		THREADFUNCTION  m_pStaticFunction;	/**< Pointer to a static function to execute, can be a null pointer */
		void		   *m_pStaticData;		/**< Additional data to pass to the static function, can be a null pointer */
		String			m_sName;			/**< Thread name */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/System/Thread.inl"


#endif // __PLCORE_THREAD_H__
