/*********************************************************\
 *  File: JointAni.h                                     *
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


#ifndef __PLMESH_JOINTANI_H__
#define __PLMESH_JOINTANI_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Animation/AnimationBase.h>
#include "PLMesh/PLMesh.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMesh {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class JointHandler;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Joint animation class
*
*  @remarks
*    This class manages the predefined joint animation data which is sometimes
*    also called 'offline animation'.
*/
class JointAni : public PLRenderer::AnimationBase {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Tell which attribute the track modify
		*/
		enum EAttribute {
			AX     = 1<<0,	/**< The x component of the position is modified */
			AY     = 1<<1,	/**< The y component of the position is modified */
			AZ     = 1<<2,	/**< The z component of the position is modified */
			AYaw   = 1<<3,	/**< The yaw component of the rotation is modified */
			APitch = 1<<4,	/**< The pitch component of the rotation is modified */
			ARoll  = 1<<5,	/**< The roll component of the rotation is modified */
			AW     = 1<<6	/**< The w component of the rotation is modified */
		};


	//[-------------------------------------------------------]
	//[ Public structures                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Holds information about which joint components are animated
		*/
		class AniJoint {


			//[-------------------------------------------------------]
			//[ Public data                                           ]
			//[-------------------------------------------------------]
			public:
				char nAnimatedComponents;	/**< X, Y, Z, Yaw, Pitch, Roll, W, see EAttribute */


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
			public:
				PLMESH_API AniJoint() :
					nAnimatedComponents(0)
				{
				}

				PLMESH_API ~AniJoint()
				{
				}

				PLMESH_API bool operator ==(const AniJoint &cAniJoint) const
				{
					return nAnimatedComponents == cAniJoint.nAnimatedComponents;
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
		PLMESH_API JointAni();

		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API virtual ~JointAni();

		/**
		*  @brief
		*    Returns the animation joints
		*
		*  @return
		*    Skeleton animation joints
		*/
		PLMESH_API PLCore::Array<AniJoint> &GetJoints();

		/**
		*  @brief
		*    Applies the joint states from the given joint animation frame
		*    to the given joint states
		*
		*  @param[out] lstJointHandlers
		*    Joint handlers to manipulate
		*  @param[in]  nFrame
		*    Joint animation frame
		*  @param[in]  fWeight
		*    Weight (0.0-1.0)
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe no skeleton set?)
		*/
		PLMESH_API bool ApplyJointStates(PLCore::Array<JointHandler> &lstJointHandlers, PLCore::uint32 nFrame, float fWeight = 1.0f) const;

		/**
		*  @brief
		*    Applies the blended joint states from the given joint animation frames
		*    to the given joint states
		*
		*  @param[out] lstJointHandlers
		*    Joint handlers to manipulate
		*  @param[in]  nFrame1
		*    Joint animation frame 1
		*  @param[in]  nFrame2
		*    Joint animation frame 2
		*  @param[in]  fTime
		*    Joint animation time (0.0-1.0)
		*  @param[in]  fWeight
		*    Weight (0.0-1.0)
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe no skeleton set?)
		*/
		PLMESH_API bool ApplyJointStates(PLCore::Array<JointHandler> &lstJointHandlers, PLCore::uint32 nFrame1, PLCore::uint32 nFrame2, float fTime, float fWeight = 1.0f) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::Array<AniJoint> m_lstAniJoints;	/**< The animation joints */


	//[-------------------------------------------------------]
	//[ Public virtual AnimationInfo functions                ]
	//[-------------------------------------------------------]
	public:
		PLMESH_API virtual JointAni &operator =(const JointAni &cSource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh


#endif // __PLMESH_JOINTANI_H__
