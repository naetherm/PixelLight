/*********************************************************\
 *  File: AnimationInfo.h                                *
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


#ifndef __PLRENDERER_ANIMATIONINFO_H__
#define __PLRENDERER_ANIMATIONINFO_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRenderer/Animation/AnimationEvent.h"
#include "PLRenderer/Animation/AnimationFrameInfo.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Animation information
*/
class AnimationInfo {


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
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLRENDERER_API AnimationInfo();

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~AnimationInfo();

		/**
		*  @brief
		*    Returns the type of the animation
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
		*    Returns the animation start frame
		*
		*  @return
		*    Animation start frame
		*/
		inline PLCore::uint32 GetStartFrame() const;

		/**
		*  @brief
		*    Sets the animation start frame
		*
		*  @param[in] nStartFrame
		*    Animation start frame
		*/
		PLRENDERER_API void SetStartFrame(PLCore::uint32 nStartFrame = 0);

		/**
		*  @brief
		*    Returns the animation end frame
		*
		*  @return
		*    Animation end frame
		*/
		inline PLCore::uint32 GetEndFrame() const;

		/**
		*  @brief
		*    Sets the animation end frame
		*
		*  @param[in] nEndFrame
		*    Animation end frame
		*/
		PLRENDERER_API void SetEndFrame(PLCore::uint32 nEndFrame = 0);

		/**
		*  @brief
		*    Returns the number of animation frames
		*
		*  @return
		*    Number of animation frames (normally EndFrame-StartFrame+1)
		*/
		inline PLCore::uint32 GetNumOfFrames() const;

		/**
		*  @brief
		*    Get the animation speed
		*
		*  @return
		*    Playback speed of the animation, if negative, the animation is played reversed
		*/
		inline float GetSpeed() const;

		/**
		*  @brief
		*    Set the animation speed
		*
		*  @param[in] fSpeed
		*    New playback speed for the animation, if negative, the animation is played reversed
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
		*    Returns information about a frame
		*
		*  @param[in] nIndex
		*    Index of the frame the information should be returned
		*
		*  @return
		*    Frame information, a null pointer if there's an error
		*/
		inline AnimationFrameInfo *GetFrameInfo(PLCore::uint32 nIndex = 0) const;

		/**
		*  @brief
		*    Returns the event manager
		*
		*  @return
		*    Event manager
		*/
		inline AnimationEventManager &GetEventManager();
		inline const AnimationEventManager &GetEventManager() const;


	//[-------------------------------------------------------]
	//[ Public virtual AnimationInfo functions                ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @note
		*    - The animation name itself is copied but at the end a
		*      number is added because each animation name must be unique!
		*
		*  @return
		*    Reference to this instance
		*/
		PLRENDERER_API virtual AnimationInfo &operator =(const AnimationInfo &cSource);

		/**
		*  @brief
		*    Get the animation source name
		*
		*  @return
		*    Animation source name, empty on error
		*
		*  @note
		*    - If this is a skeleton animation, the source name is the name of a skeleton.
		*      (SkeletonManager::Get(...))
		*    - If this is a vertex animation, the source name is the name of a morph target animation.
		*      (pMyMesh->GetMorphTargetAnimationManager().Get(...))
		*/
		PLRENDERER_API virtual PLCore::String GetSourceName() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::uint32 m_nType;		/**< Animation type (for instance  0=skeleton  1=vertex) */
		PLCore::uint32 m_nStart;	/**< Start frame of the animation */
		PLCore::uint32 m_nEnd;		/**< End frame of the animation */
		float		   m_fSpeed;	/**< Playback speed */
		PLCore::uint32 m_nFlags;	/**< Animation information flags */

		/** Frame information */
		PLCore::Array<AnimationFrameInfo> m_lstFrameInfos;

		/** Event manager */
		AnimationEventManager m_cEventManager;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Animation/AnimationInfo.inl"


#endif // __PLRENDERER_ANIMATIONINFO_H__
