/*********************************************************\
 *  File: AnimationFrameInfo.h                           *
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


#ifndef __PLRENDERER_ANIMATIONFRAMEINFO_H__
#define __PLRENDERER_ANIMATIONFRAMEINFO_H__
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Animation frame information
*/
class AnimationFrameInfo {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline AnimationFrameInfo();

		/**
		*  @brief
		*    Destructor
		*/
		inline ~AnimationFrameInfo();

		/**
		*  @brief
		*    Returns the speed of an animation frame
		*
		*  @return
		*    Animation frame speed
		*
		*  @note
		*    - If speed is 0 the frame will be skipped
		*    - If speed is less 0 there's no frame interpolation
		*/
		inline float GetSpeed() const;

		/**
		*  @brief
		*    Sets the animation frame speed
		*
		*  @param[in] fSpeed
		*    Animation frame speed
		*
		*  @see
		*    - GetSpeed()
		*/
		inline void SetSpeed(float fSpeed = 1.0f);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this class instance
		*/
		inline AnimationFrameInfo &operator =(const AnimationFrameInfo &cSource);

		/**
		*  @brief
		*    Compare operator
		*
		*  @param[in] cSource
		*    Source to compare with
		*
		*  @return
		*    'true' if the two animation frame information are equal, else 'false'
		*/
		inline bool operator ==(const AnimationFrameInfo &cSource) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float m_fSpeed;	/**< Frame speed */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Animation/AnimationFrameInfo.inl"


#endif // __PLRENDERER_ANIMATIONFRAMEINFO_H__
