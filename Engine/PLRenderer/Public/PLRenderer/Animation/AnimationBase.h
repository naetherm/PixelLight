/*********************************************************\
 *  File: AnimationBase.h                                *
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


#ifndef __PLRENDERER_ANIMATIONBASE_H__
#define __PLRENDERER_ANIMATIONBASE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRenderer/Animation/AnimationInfo.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Animation base class
*/
class AnimationBase : public AnimationInfo {


	//[-------------------------------------------------------]
	//[ Public structures                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Animation frame keys
		*/
		class FrameKeys {


			//[-------------------------------------------------------]
			//[ Public data                                           ]
			//[-------------------------------------------------------]
			public:
				PLCore::Array<float> lstFrameKeys;


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
			public:
				PLRENDERER_API bool operator ==(const FrameKeys &cKeys) const
				{
					return lstFrameKeys == cKeys.lstFrameKeys;
				}


		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLRENDERER_API AnimationBase();

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~AnimationBase();

		/**
		*  @brief
		*    Returns the frame rate
		*
		*  @return
		*    Frames per second
		*/
		inline float GetFrameRate() const;

		/**
		*  @brief
		*    Sets the frame rate
		*
		*  @param[in] fFrameRate
		*    Frames per second
		*/
		inline void SetFrameRate(float fFrameRate = 24.0f);

		/**
		*  @brief
		*    Returns the frame keys
		*
		*  @return
		*    Frame keys
		*/
		inline PLCore::Array<FrameKeys> &GetFrameKeys();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		float					 m_fFrameRate;		/**< Frame rate */
		PLCore::Array<FrameKeys> m_lstFrameKeys;	/**< Frame keys */


	//[-------------------------------------------------------]
	//[ Public virtual AnimationInfo functions                ]
	//[-------------------------------------------------------]
	public:
		PLRENDERER_API virtual AnimationBase &operator =(const AnimationBase &cSource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Animation/AnimationBase.inl"


#endif // __PLRENDERER_ANIMATIONBASE_H__
