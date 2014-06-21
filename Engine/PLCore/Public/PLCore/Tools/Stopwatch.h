/*********************************************************\
 *  File: Stopwatch.h                                    *
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


#ifndef __PLCORE_STOPWATCH_H__
#define __PLCORE_STOPWATCH_H__
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
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Stopwatch
*
*  @verbatim
*    Usage example:
*
*    // Start the stopwatch
*    Stopwatch cStopwatch(true);
*
*    // Do some stuff
*
*    // Measure elapsed time
*    float fElapsedSeconds = cStopwatch.GetSeconds();
*  @endverbatim
*
*  @note
*    - The stopwatch implementation is just using "System::GetInstance()->GetMicroseconds()" and is therefore quite lightweight
*/
class Stopwatch {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline Stopwatch();

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] bStartAtOnce
		*    If this parameter is 'true', the stopwatch is started automatically at once
		*/
		inline Stopwatch(bool bStartAtOnce);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~Stopwatch();

		/**
		*  @brief
		*    Starts the stopwatch
		*
		*  @note
		*    - If the stopwatch is already running it's restarted
		*/
		PLCORE_API void Start();

		/**
		*  @brief
		*    Stops the stopwatch
		*
		*  @return
		*    The elapsed time in microseconds since Start()
		*
		*  @note
		*    - Often it's adequate to just request the past time using
		*      e.g. "GetMilliseconds()" and not explicitly stopping the stopwatch
		*/
		PLCORE_API uint64 Stop();

		/**
		*  @brief
		*    Returns the number of weeks since the stopwatch was started
		*
		*  @return
		*    Number of weeks elapsed since the stopwatch was started
		*/
		inline float GetWeeks() const;

		/**
		*  @brief
		*    Returns the number of days since the stopwatch was started
		*
		*  @return
		*    Number of days elapsed since the stopwatch was started
		*/
		inline float GetDays() const;

		/**
		*  @brief
		*    Returns the number of hours since the stopwatch was started
		*
		*  @return
		*    Number of hours elapsed since the stopwatch was started
		*/
		inline float GetHours() const;

		/**
		*  @brief
		*    Returns the number of minutes since the stopwatch was started
		*
		*  @return
		*    Number of minutes elapsed since the stopwatch was started
		*/
		inline float GetMinutes() const;

		/**
		*  @brief
		*    Returns the number of seconds since the stopwatch was started
		*
		*  @return
		*    Number of seconds elapsed since the stopwatch was started
		*/
		inline float GetSeconds() const;

		/**
		*  @brief
		*    Returns the number of milliseconds since the stopwatch was started
		*
		*  @return
		*    Number of milliseconds elapsed since the stopwatch was started
		*/
		inline float GetMilliseconds() const;

		/**
		*  @brief
		*    Retrieves the number of microseconds since the stopwatch was started
		*
		*  @return
		*    Number of microseconds elapsed since the stopwatch was started
		*/
		PLCORE_API uint64 GetMicroseconds() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool   m_bRunning;	/**< Is the stopwatch currently running? */
		uint64 m_nStart;	/**< Stopwatch start time (microseconds) */
		uint64 m_nStop;		/**< Stopwatch stop time (microseconds) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Tools/Stopwatch.inl"


#endif // __PLCORE_STOPWATCH_H__
