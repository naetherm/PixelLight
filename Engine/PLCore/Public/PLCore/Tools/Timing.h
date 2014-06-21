/*********************************************************\
 *  File: Timing.h                                       *
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


#ifndef __PLCORE_TIMING_H__
#define __PLCORE_TIMING_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Core/Singleton.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Timing stuff
*
*  @remarks
*     This timing class offers a lot of timing tool functions you can for
*     instance use to create timed movement. Have a look at the 'System'-singleton
*     for the basic timing functions.
*/
class Timing : public Singleton<Timing> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Singleton<Timing>;


	//[-------------------------------------------------------]
	//[ Public static PLCore::Singleton functions             ]
	//[-------------------------------------------------------]
	// This solution enhances the compatibility with legacy compilers like GCC 4.2.1 used on Mac OS X 10.6
	// -> The C++11 feature "extern template" (C++11, see e.g. http://www2.research.att.com/~bs/C++0xFAQ.html#extern-templates) can only be used on modern compilers like GCC 4.6
	// -> We can't break legacy compiler support, especially when only the singletons are responsible for the break
	// -> See PLCore::Singleton for more details about singletons
	public:
		static PLCORE_API Timing *GetInstance();
		static PLCORE_API bool HasInstance();


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ General                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns whether the timing is active or not
		*
		*  @return
		*    'true' if the timing is active, else 'false'
		*/
		inline bool IsActive() const;

		/**
		*  @brief
		*    Sets whether the timing is active or not
		*
		*  @param[in] bActive
		*    Should the timing be active?
		*
		*  @note
		*    - If the timing is disabled the time difference between
		*      two frames will be always the maximum time difference
		*    - Use it only for testing
		*/
		inline void SetActive(bool bActive = true);

		/**
		*  @brief
		*    Resets the timing
		*/
		PLCORE_API void Reset();

		/**
		*  @brief
		*    Checks whether the timing has to be updated or not
		*
		*  @param[in] pnTimeToWait
		*    If not a null pointer, if there's a FPS limit and it's not time for an update -> receives the time in
		*    milliseconds we need to wait until it's time for the next update, receives '0' in all other cases
		*
		*  @return
		*    'true' if the timing has to be updated, else 'false'
		*
		*  @remarks
		*    Normally the timing must be updated every frame. But if theres a frame
		*    rate limitation it's not allowed to update the timing every frame!
		*/
		PLCORE_API bool CheckUpdate(uint64 *pnTimeToWait = nullptr) const;

		/**
		*  @brief
		*    Updates all time relevant stuff
		*
		*  @param[in] pnTimeToWait
		*    If not a null pointer, if there's a FPS limit and it's not time for an update -> receives the time in
		*    milliseconds we need to wait until it's time for the next update, receives '0' in all other cases
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe there's a frame rate limitation)
		*
		*  @see
		*    - CheckUpdate()
		*/
		PLCORE_API bool Update(uint64 *pnTimeToWait = nullptr);

		//[-------------------------------------------------------]
		//[ Past time                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the past time since last frame (seconds)
		*
		*  @return
		*    Past time since last frame
		*
		*  @remarks
		*    The timing class tries to compensate irregularities in timing automatically to
		*    avoid for instance sudden jumps through half of the universe. Further a maximum
		*    time difference is applied to avoid any totally undesired results.
		*/
		inline float GetTimeDifference() const;

		/**
		*  @brief
		*    Returns the past time since last frame without any compensating (seconds)
		*
		*  @return
		*    Past time since last frame without any compensating
		*
		*  @note
		*   - Use GetTimeDifference() instead this function whenever you can
		*/
		inline float GetTimeDifferenceNoCompensating() const;

		/**
		*  @brief
		*    Returns the maximum time difference
		*
		*  @return
		*    The maximum time difference since the last frame in seconds
		*
		*  @note
		*    - The maximum time difference will avoid a too hight time difference
		*      value which would probably end in undefined problems.
		*    - A maximum time difference of 0.15 is a good value
		*/
		inline float GetMaxTimeDifference() const;

		/**
		*  @brief
		*    Sets the maximum time difference
		*
		*  @param[in] fMaxTimeDifference
		*    The maximum time difference since the last frame in seconds (> 0)
		*
		*  @see
		*    - GetMaxTimeDifference()
		*/
		inline void SetMaxTimeDifference(float fMaxTimeDifference = 0.15f);

		/**
		*  @brief
		*    Returns the past time in milliseconds since the application start
		*
		*  @return
		*    Past time in milliseconds since the application start
		*/
		PLCORE_API uint64 GetPastTime() const;

		/**
		*  @brief
		*    Returns a general timing
		*
		*  @return
		*    General timing
		*
		*  @remarks
		*    On each timing update, the current time difference is added to this general timing.
		*/
		inline float GetTimer() const;

		//[-------------------------------------------------------]
		//[ Frames                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the current frames per second (FPS)
		*
		*  @return
		*    Current frames per second
		*/
		inline float GetFramesPerSecond() const;

		/**
		*  @brief
		*    Returns the number of past frames since timing start
		*
		*  @return
		*    Current frames per second (FPS)
		*/
		inline uint32 GetPastFrames() const;

		/**
		*  @brief
		*    Returns the FPS limit
		*
		*  @return
		*    FPS limit, 0 if there's no FPS limitation
		*
		*  @note
		*    - FPS limitation could be used to avoid that the application
		*      uses to much system resources. (normally 30 FPS are enough)
		*    - Use a FPS limitation only in special situations!
		*      (users love FPS over 100 even if its more or less senseless ;-)
		*/
		inline float GetFPSLimit() const;

		/**
		*  @brief
		*    Sets the FPS limit
		*
		*  @param[in] fFPSLimit
		*    FPS limit, 0 if there should be no limitation (>= 0)
		*
		*  @see
		*    - GetFPSLimit()
		*/
		inline void SetFPSLimit(float fFPSLimit = 0.0f);

		//[-------------------------------------------------------]
		//[ Time scale                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns if the timing is currently freezed or not
		*
		*  @return
		*    'true' if the timing is freezed at the moment, else 'false'
		*/
		inline bool IsFreezed() const;

		/**
		*  @brief
		*    Set freezed mode
		*
		*  @param[in] bFreeze
		*    Should the timing be freezed or not?
		*
		*  @note
		*    - Freeze your application if you are not in rendering mode
		*      (e.g. you load something)
		*/
		PLCORE_API void Freeze(bool bFreeze = true);

		/**
		*  @brief
		*    Returns whether the timing is paused of not
		*
		*  @return
		*    'true' if the timing is paused, else 'false'
		*
		*  @note
		*    - If the timing is paused scene nodes, particles etc. are not updated
		*    - The timing will still be updated
		*/
		inline bool IsPaused() const;

		/**
		*  @brief
		*    Set pause mode
		*
		*  @param[in] bPause
		*    Should the timing be paused or not?
		*/
		inline void Pause(bool bPause = true);

		/**
		*  @brief
		*    Returns the time scale factor
		*
		*  @return
		*    Time scale factor
		*
		*  @note
		*    - The global time scale factor should only be manipulated for debugging
		*    - For 'slow motion' effects use the slow motion functions instead!
		*    - A factor of <= 0 is NOT allowed because this may cause problems in certain
		*      situations, pause the timer instead by hand!
		*    - Do NOT make the factor 'too' (for example > 4) extreme, this may cause
		*      problems in certain situations!
		*/
		inline float GetTimeScaleFactor() const;

		/**
		*  @brief
		*    Sets the time scale factor
		*
		*  @param[in] fFactor
		*    Time scale, a factor of <= 0 is NOT allowed!
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe the given factor is <= 0?)
		*
		*  @see
		*    - GetTimeScaleFactor()
		*/
		inline bool SetTimeScaleFactor(float fFactor = 1.0f);

		/**
		*  @brief
		*    Returns if the slow motion is activated or not
		*
		*  @return
		*    'true' is the slow motion mode is activated, else 'false'
		*/
		inline bool IsSlowMotion() const;

		/**
		*  @brief
		*    Activates/deactivates the slow motion mode
		*
		*  @param[in] bSlowMotion
		*    Should the slow motion mode be activated?
		*/
		inline void SetSlowMotion(bool bSlowMotion = false);

		/**
		*  @brief
		*    Returns the slow motion factor
		*
		*  @param[in] bRealUsed
		*    The real used slow motion factor will be returned. If the slow motion mode
		*    is deactivated this will be 1!
		*
		*  @return
		*    The current slow motion factor
		*
		*  @note
		*    - The slow motion mode must be activated that this factor
		*      changes the game speed
		*    - Use this function to change the speed in general
		*    - A factor of <= 0 is NOT allowed because this may cause problems in certain
		*      situations, pause the timer instead by hand!
		*    - Do NOT make the factor 'too' (for example > 4) extreme, this may cause
		*      problems in certain situations!
		*/
		inline float GetSlowMotionFactor(bool bRealUsed = true) const;

		/**
		*  @brief
		*    Sets the slow motion factor
		*
		*  @param[in] fSlowMotionFactor
		*    The slow motion factor, a factor of <= 0 is NOT allowed!
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe the given factor is <= 0?)
		*
		*  @see
		*    - GetSlowMotionFactor()
		*/
		inline bool SetSlowMotionFactor(float fSlowMotionFactor = 1.0f);

		/**
		*  @brief
		*    Returns the custom slow motion factor
		*
		*  @param[in] bRealUsed
		*    The real used slow motion factor will be returned. If the slow motion mode
		*    is deactivated this will be 1!
		*
		*  @return
		*    The current slow motion factor
		*
		*  @note
		*    - The slow motion mode must be activated that this
		*      factor changes the game speed
		*    - Use this factor to temporal slow motion effects
		*/
		inline float GetCustomSlowMotionFactor(bool bRealUsed = true) const;

		/**
		*  @brief
		*    Sets the custom slow motion factor
		*
		*  @param[in] fSlowMotionFactor
		*    The slow motion factor, a factor of <= 0 is NOT allowed!
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe the given factor is <= 0?)
		*
		*  @see
		*    - GetCustomSlowMotionFactor()
		*/
		inline bool SetCustomSlowMotionFactor(float fSlowMotionFactor = 1.0f);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*/
		Timing();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Timing(const Timing &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Timing();

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
		Timing &operator =(const Timing &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool   m_bActive;						/**< Is the timing active? */
		uint64 m_nTimeStart;					/**< The start system time */
		uint64 m_nTimeNow;						/**< Current system time */
		uint64 m_nTimeLast;						/**< System time of the last frame */
		float  m_fTimeDifference;				/**< The past time since last frame (seconds) */
		float  m_fTimeDifferenceNoCompensating;	/**< The past time since last frame without any compensating (seconds) */
		float  m_fMaxTimeDifference;			/**< The maximum possible time difference (seconds) */
		float  m_fFramesPerSecond;				/**< Current frames per second (FPS... hopefully not SPS... Seconds Per Frame ;) */
		int	   m_nFramesSinceCheck;				/**< Number of frames since last FPS update */
		uint32 m_nPastFrames;					/**< Number of past frames since timing start */
		uint64 m_nLastFPSUpdateTime;			/**< The time were the last FPS update was done */
		float  m_fFPSUpdateTimer;				/**< Timing for updating FPS (only each second) */
		bool   m_bFreezed;						/**< Is the application currently freezed? */
		uint64 m_nFreezeTime;					/**< The time were the application was frozen */
		bool   m_bPause;						/**< Is the timing paused? */
		float  m_fTimeScaleFactor;				/**< Time scale factor */
		bool   m_bSlowMotion;					/**< Is the slow motion modus activated? */
		float  m_fSlowMotionFactor;				/**< The slow motion factor */
		float  m_fCustomSlowMotionFactor;		/**< The custom slow motion factor */
		uint64 m_nFPSLimitLastTime;				/**< Time were the last update was done */
		float  m_fFPSLimit;						/**< FPS limitation */
		float  m_fTimer;						/**< General timing */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Tools/Timing.inl"


#endif // __PLCORE_TIMING_H__
