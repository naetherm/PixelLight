/*********************************************************\
 *  File: AnimationEvent.h                               *
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


#ifndef __PLRENDERER_ANIMATIONEVENT_H__
#define __PLRENDERER_ANIMATIONEVENT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Element.h>
#include <PLCore/Container/ElementManager.h>
#include "PLRenderer/PLRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class AnimationEventManager;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Animation event
*/
class AnimationEvent : public PLCore::Element<AnimationEvent> {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nID
		*    Event ID
		*  @param[in] nFrame
		*    Frame causing this event
		*  @param[in] pManager
		*    Event manager using this element, can be a null pointer
		*  @param[in] sName
		*    Name of the event, if empty, a default name is set automatically
		*
		*  @note
		*    - If you provide the pointer to the owner manager the animation
		*      event will check in the animation event manager name list
		*/
		PLRENDERER_API AnimationEvent(int nID, PLCore::uint32 nFrame, AnimationEventManager *pManager = nullptr, const PLCore::String &sName = "");

		/**
		*  @brief
		*    Returns the event ID
		*
		*  @return
		*    Event ID
		*/
		inline int GetID() const;

		/**
		*  @brief
		*    Sets the event ID
		*
		*  @param[in] nID
		*    Event ID
		*/
		inline void SetID(int nID);

		/**
		*  @brief
		*    Returns the frame which will cause this event
		*
		*  @return
		*    The frame which will cause this event
		*/
		inline PLCore::uint32 GetFrame() const;

		/**
		*  @brief
		*    Sets the frame causing this event
		*
		*  @param[in] nFrame
		*    Frame causing this event
		*/
		inline void SetFrame(PLCore::uint32 nFrame);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		int			   m_nID;		/**< Event ID */
		PLCore::uint32 m_nFrame;	/**< Frame causing this event */


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::Element functions              ]
	//[-------------------------------------------------------]
	public:
		PLRENDERER_API virtual AnimationEvent &operator =(const AnimationEvent &cSource);


};

/**
*  @brief
*    Animation event manager
*/
class AnimationEventManager : public PLCore::ElementManager<AnimationEvent> {


	//[-------------------------------------------------------]
	//[ Private virtual PLCore::ElementManager functions      ]
	//[-------------------------------------------------------]
	private:
		virtual AnimationEvent *CreateElement(const PLCore::String &sName) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Animation/AnimationEvent.inl"


#endif // __PLRENDERER_ANIMATIONEVENT_H__
