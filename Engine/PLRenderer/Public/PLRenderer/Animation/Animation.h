/*********************************************************\
 *  File: Animation.h                                    *
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


#ifndef __PLRENDERER_ANIMATION_H__
#define __PLRENDERER_ANIMATION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/Event.h>
#include <PLCore/Container/Element.h>
#include "PLRenderer/PLRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class AnimationInfo;
class AnimationManager;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Animation control
*/
class Animation : public PLCore::Element<Animation> {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Animation flags
		*/
		enum EFlags {
			Loop	 = 1<<0,	/**< Loop animation */
			PingPong = 1<<1		/**< Ping pong animation */
		};


	//[-------------------------------------------------------]
	//[ Events                                                ]
	//[-------------------------------------------------------]
	public:
		PLCore::Event<>		EventStart;			/**< Animation has been started */
		PLCore::Event<>		EventStop;			/**< Animation has been stopped */
		PLCore::Event<>		EventFrameChange;	/**< Current frame change event */
		PLCore::Event<int>	EventSpecialFrame;	/**< Special marked frame reached event, event ID as parameter (see 'AnimationEvent') */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sName
		*    Element name to set
		*  @param[in] pManager
		*    Animation manager using this element, can be a null pointer
		*/
		PLRENDERER_API Animation(const PLCore::String &sName = "", AnimationManager *pManager = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~Animation();

		/**
		*  @brief
		*    Resets the animation
		*/
		PLRENDERER_API void Reset();

		/**
		*  @brief
		*    Get the animation active status
		*
		*  @return
		*    'true' if the animation is active, else 'false'
		*/
		inline bool IsActive() const;

		/**
		*  @brief
		*    Set the animation active status
		*
		*  @param[in] bActive
		*    Should the animation be active?
		*
		*  @note
		*    - If an animation is inactive it is ignored
		*/
		inline void SetActive(bool bActive = true);

		/**
		*  @brief
		*    Type of animation
		*
		*  @return
		*    Animation type (for instance  0=skeleton  1=vertex)
		*/
		inline PLCore::uint32 GetType() const;

		/**
		*  @brief
		*    Sets the animation type
		*
		*  @param[in] nType
		*    Animation type (for instance  0=skeleton  1=vertex)
		*/
		inline void SetType(PLCore::uint32 nType = 0);

		/**
		*  @brief
		*    Starts an animation
		*
		*  @param[in] nStart
		*    Start frame (inclusive)
		*  @param[in] nEnd
		*    End frame (inclusive)
		*  @param[in] fSpeed
		*    Animation speed, if negative, the animation is played reversed
		*  @param[in] nFlags
		*    Animation flags (see EFlags)
		*
		*  @note
		*    - Use the start function below which accepts a AnimationInfo
		*      as parameter for more complex animations
		*/
		PLRENDERER_API void Start(PLCore::uint32 nStart, PLCore::uint32 nEnd, float fSpeed = 24.0f, PLCore::uint32 nFlags = 0);

		/**
		*  @brief
		*    Starts an animation
		*
		*  @param[in] cAnimationInfo
		*    The animation information
		*  @param[in] bRestart
		*    Restart animation if it is already currently played?
		*
		*  @note
		*    - cAnimationInfo must stay a valid reference as long as the animation is played!
		*    - Because events are stored in the animation information it's possible that events
		*      are caused during the animation playback if an scene node is set
		*/
		PLRENDERER_API void Start(AnimationInfo &cAnimationInfo, bool bRestart = false);

		/**
		*  @brief
		*    Stops the animation
		*/
		PLRENDERER_API void Stop();

		/**
		*  @brief
		*    Pause the animation
		*/
		inline void Pause();

		/**
		*  @brief
		*    Continues the animation
		*/
		inline void Continue();

		/**
		*  @brief
		*    Returns the animation information
		*
		*  @return
		*    Animation information, a null pointer if there's no one
		*/
		inline AnimationInfo *GetAnimation() const;

		/**
		*  @brief
		*    Gets the animation weight
		*
		*  @return
		*    Animation weight (normally 0.0-1.0)
		*/
		inline float GetWeight() const;

		/**
		*  @brief
		*    Sets the animation weight
		*
		*  @param[in] fWeight
		*    Animation weight (normally 0.0-1.0)
		*/
		inline void SetWeight(float fWeight = 1.0f);

		/**
		*  @brief
		*    Get the animation speed
		*
		*  @return
		*    Current speed of the animation, if negative, the animation is played reversed
		*/
		inline float GetSpeed() const;

		/**
		*  @brief
		*    Set the animation speed
		*
		*  @param[in] fSpeed
		*    New speed for the animation, if negative, the animation is played reversed
		*/
		inline void SetSpeed(float fSpeed = 24.0f);

		/**
		*  @brief
		*    Get the animation information flags
		*
		*  @return
		*    Animation information flags (see EFlags)
		*/
		inline PLCore::uint32 GetFlags() const;

		/**
		*  @brief
		*    Sets the animation information flags
		*
		*  @param[in] nFlags
		*    Animation information flags (see EFlags)
		*/
		inline void SetFlags(PLCore::uint32 nFlags = 0);

		/**
		*  @brief
		*    Get the animation running status
		*
		*  @return
		*    'true' if the animation is currently running, else 'false'
		*/
		inline bool IsRunning() const;

		/**
		*  @brief
		*    Returns the start frame
		*
		*  @return
		*    The start frame (inclusive)
		*/
		inline PLCore::uint32 GetStartFrame() const;

		/**
		*  @brief
		*    Returns the end frame
		*
		*  @return
		*    The end frame (inclusive)
		*/
		inline PLCore::uint32 GetEndFrame() const;

		/**
		*  @brief
		*    Returns the number of frames
		*
		*  @return
		*    The number of frames
		*/
		inline PLCore::uint32 GetNumOfFrames() const;

		/**
		*  @brief
		*    Get the current animation frame
		*
		*  @return
		*    Current animation frame (for instance 2.6)
		*/
		PLRENDERER_API float GetFrame() const;

		/**
		*  @brief
		*    Gets the progress from the current state to the next state (0.0-1.0)
		*
		*  @return
		*    Current animation frame (for instance 0.6)
		*/
		PLRENDERER_API float GetProgress() const;

		/**
		*  @brief
		*    Get the current animation frame as an integer
		*
		*  @return
		*    Current animation frame as an integer
		*
		*  @note
		*    - This function returns an integer for the current animation frame, e.g. if the
		*      animation frame is at 2.6f, it will return 2
		*/
		PLRENDERER_API PLCore::uint32 GetCurrentFrame() const;

		/**
		*  @brief
		*    Set the current animation frame
		*
		*  @param[in] fFrame
		*    Current animation frame
		*
		*  @note
		*    - If the given frame is out of bounds, it's clamped automatically
		*/
		PLRENDERER_API void SetCurrentFrame(float fFrame);

		/**
		*  @brief
		*    Get the next animation frame as an integer
		*
		*  @return
		*    Next animation frame as an integer
		*
		*  @see
		*    - GetCurrentFrame()
		*/
		PLRENDERER_API PLCore::uint32 GetNextFrame() const;

		/**
		*  @brief
		*    Updates the animation
		*
		*  @param[in] fTimeDifference
		*    Past time since last frame (use e.g. PLCore::Timing::GetInstance()->GetTimeDifference())
		*/
		PLRENDERER_API void Update(float fTimeDifference);

		/**
		*  @brief
		*    Returns whether animation events are enabled or not
		*
		*  @return
		*    'true' if animation events are enabled, else 'false'
		*
		*  @note
		*    - If animation events are enabled special frames are able to send
		*      an event message to the scene node connected with the animation
		*/
		inline bool GetEvents() const;

		/**
		*  @brief
		*    Sets whether animation events are enabled or not
		*
		*  @param[in] bEvents
		*    Are animation events enabled?
		*
		*  @see
		*    - GetEvents()
		*/
		inline void SetEvents(bool bEvents = true);

		//[-------------------------------------------------------]
		//[ Operators                                             ]
		//[-------------------------------------------------------]
		PLRENDERER_API Animation &operator =(const Animation &cSource);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Check if events should be caused
		*
		*  @param[in] nPreviousFrame
		*    Previous frame
		*
		*  @note
		*    - An event is caused if the current frame isn't equal to
		*      the previous and if there is an scene node receiving the caused event
		*/
		void CheckEvent(int nPreviousFrame = -1) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float			m_fFrame;			/**< Current animation frame */
		bool			m_bActive;			/**< Active status (if the channel is used in calculations) */
		bool			m_bRunning;			/**< Running status */
		PLCore::uint32  m_nType;			/**< Animation type (for instance  0=skeleton, 1=vertex) */
		PLCore::uint32  m_nFlags;			/**< Animation flags */
		PLCore::uint32  m_nStart;			/**< Start frame of the animation */
		PLCore::uint32  m_nEnd;				/**< End frame of the animation */
		float		    m_fSpeed;			/**< Playback speed */
		bool		    m_bEvents;			/**< Are animation events enabled? */
		AnimationInfo  *m_pAnimationInfo;	/**< Pointer to the animation information, can be a null pointer */
		float		    m_fWeight;			/**< Animation weight */
		bool		    m_bBounced;			/**< If ping pong playback, was there already a 'bounce'? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Animation/Animation.inl"


#endif // __PLRENDERER_ANIMATION_H__
