/*********************************************************\
 *  File: TimerThread.h                                  *
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


#ifndef __PLGUI_TIMERTHREAD_H__
#define __PLGUI_TIMERTHREAD_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/System/Thread.h>
#include "PLGui/PLGui.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class CriticalSection;
}
namespace PLGui {
	class Timer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Timer operation thread
*/
class TimerThread : public PLCore::Thread {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cTimer
		*    Reference to the owning Timer object
		*  @param[in] nTimeout
		*    Timeout (in milliseconds)
		*  @param[in] bOnce
		*    'true' if the timer shall fire only once, else 'false'
		*/
		PLGUI_API TimerThread(Timer &cTimer, PLCore::uint64 nTimeout, bool bOnce);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~TimerThread();

		/**
		*  @brief
		*    Stop timer thread
		*/
		PLGUI_API void StopTimer();

		/**
		*  @brief
		*    Fire timer
		*/
		PLGUI_API void Fire();


	//[-------------------------------------------------------]
	//[ Private virtual PLCore::ThreadFunction functions      ]
	//[-------------------------------------------------------]
	private:
		virtual int Run() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Timer					 &m_cTimer;				/**< Timer to that this thread belongs */
		PLCore::CriticalSection *m_pCriticalSection;	/**< Timer critical section (always valid!) */
		PLCore::uint64			 m_nTimeout;			/**< Timeout value */
		bool					 m_bOnce;				/**< Timer is firing only once? */
		volatile bool			 m_bShutdown;			/**< Shutdown thread? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_TIMERTHREAD_H__
