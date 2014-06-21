/*********************************************************\
 *  File: ThreadWindows.h                                *
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


#ifndef __PLCORE_THREAD_WINDOWS_H__
#define __PLCORE_THREAD_WINDOWS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/+Windows/PLCoreWindowsIncludes.h"
#include "PLCore/System/ThreadImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Windows 'Thread' implementation
*/
class ThreadWindows : public ThreadImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Thread;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cThread
		*    Reference to the owning thread
		*  @param[in] bThreadID
		*    If true, use the given thread ID
		*  @param[in] nThreadID
		*    System specific thread ID
		*
		*  @remarks
		*    If 'bThreadID == true' and 'nThreadID == NULL_HANDLE', the current thread
		*    ID will be used. If called from the main thread, this is the
		*    main thread ID.
		*/
		ThreadWindows(Thread &cThread, bool bThreadID, handle nThreadID);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ThreadWindows();


	//[-------------------------------------------------------]
	//[ Private virtual ThreadImpl functions                  ]
	//[-------------------------------------------------------]
	private:
		virtual handle GetID() const override;
		virtual bool IsActive() const override;
		virtual bool Start() override;
		virtual bool Terminate() override;
		virtual bool Join() override;
		virtual bool Join(uint64 nTimeout) override;
		virtual uint32 GetPriorityClass() const override;
		virtual bool SetPriorityClass(uint32 nPriorityClass) override;
		virtual uint32 GetPriority() const override;
		virtual bool SetPriority(uint32 nPriority) override;


	//[-------------------------------------------------------]
	//[ Private static functions                              ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Static thread callback function
		*
		*  @param[in] lpParameter
		*    Parameter, can be a null pointer
		*
		*  @return
		*    Return value
		*/
		static DWORD WINAPI ThreadProc(LPVOID lpParameter);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		HANDLE m_hThread;	/**< Thread handle */
		DWORD  m_nThreadID;	/**< Thread ID */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_THREAD_WINDOWS_H__
