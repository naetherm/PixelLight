/*********************************************************\
 *  File: Timer.h                                        *
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


#ifndef __PLGUI_TIMER_H__
#define __PLGUI_TIMER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/Event.h>
#include "PLGui/PLGui.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Gui;
class TimerThread;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Timer class
*/
class Timer {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Gui;
	friend class TimerThread;


	//[-------------------------------------------------------]
	//[ Public events                                         ]
	//[-------------------------------------------------------]
	public:
		PLCore::Event<> EventStart;		/**< The timer has been started */
		PLCore::Event<> EventStop;		/**< The timer has been stopped */
		PLCore::Event<> EventFire;		/**< The timer has fired */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGui
		*    Owner GUI
		*/
		PLGUI_API Timer(Gui &cGui);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API ~Timer();

		/**
		*  @brief
		*    Get owner GUI
		*
		*  @return
		*    Pointer to GUI object (never a null pointer)
		*/
		PLGUI_API Gui *GetGui() const;

		/**
		*  @brief
		*    Get timer ID
		*
		*  @return
		*    Timer ID
		*/
		PLGUI_API PLCore::uint32 GetID() const;

		/**
		*  @brief
		*    Set timer ID
		*
		*  @param[in] nID
		*    Timer ID
		*/
		PLGUI_API void SetID(PLCore::uint32 nID);

		/**
		*  @brief
		*    Check if timer is active
		*
		*  @return
		*    'true' if timer is currently active
		*/
		PLGUI_API bool IsActive() const;

		/**
		*  @brief
		*    Check if timer fires only once
		*
		*  @return
		*    'true' if once
		*/
		PLGUI_API bool IsOnce() const;

		/**
		*  @brief
		*    Get timeout interval
		*
		*  @return
		*    Timeout interval in milliseconds
		*/
		PLGUI_API PLCore::uint64 GetTimeout() const;

		/**
		*  @brief
		*    Start timer
		*
		*  @param[in] nTimeout
		*    Timeout in milliseconds after which the timer fires repeatedly
		*
		*  @note
		*    - If the timer is already running it's stopped and started again
		*/
		PLGUI_API void Start(PLCore::uint64 nTimeout);

		/**
		*  @brief
		*    Start timer only once, not periodically
		*
		*  @param[in] nTimeout
		*    Timeout in milliseconds after which the timer fires
		*
		*  @note
		*    - If the timer is already running it's stopped and started again
		*/
		PLGUI_API void StartOnce(PLCore::uint64 nTimeout);

		/**
		*  @brief
		*    Stop timer
		*/
		PLGUI_API void Stop();


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Fire timer
		*/
		PLGUI_API void Fire();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Gui			   *m_pGui;		/**< Pointer to owner GUI */
		PLCore::uint32  m_nID;		/**< Timer ID */
		TimerThread	   *m_pThread;	/**< Current timer thread */
		bool		    m_bActive;	/**< Timer is active? */
		bool		    m_bOnce;	/**< Timer is firing only once? */
		PLCore::uint64  m_nTimeout;	/**< Timeout value */


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static PLCore::uint32 m_nNextID;	/**< ID counter */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_TIMER_H__
