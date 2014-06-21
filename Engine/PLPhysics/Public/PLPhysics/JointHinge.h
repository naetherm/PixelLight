/*********************************************************\
 *  File: JointHinge.h                                   *
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


#ifndef __PLPHYSICS_JOINTHINGE_H__
#define __PLPHYSICS_JOINTHINGE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector3.h>
#include "PLPhysics/Joint.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysics {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract PL physics hinge (other name: revolute) joint
*
*  @remarks
*    DOFs removed: 5 DOFs remaining: 1
*/
class JointHinge : public Joint {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~JointHinge();

		/**
		*  @brief
		*    Returns the origin of the hinge in world space
		*
		*  @return
		*    The origin of the hinge in world space
		*/
		PLPHYSICS_API const PLMath::Vector3 &GetPivotPoint() const;

		/**
		*  @brief
		*    Returns the line of action of the hinge in world space
		*
		*  @return
		*    The line of action of the hinge in world space
		*/
		PLPHYSICS_API const PLMath::Vector3 &GetPinDir() const;

		/**
		*  @brief
		*    Returns the low range
		*
		*  @return
		*    The low range in degree
		*/
		PLPHYSICS_API float GetLowRange() const;

		/**
		*  @brief
		*    Sets the low range
		*
		*  @param[in] fLowRange
		*    New low range in degree
		*/
		PLPHYSICS_API void SetLowRange(float fLowRange = -180.0f);

		/**
		*  @brief
		*    Returns the high range
		*
		*  @return
		*    The high range in degree
		*/
		PLPHYSICS_API float GetHighRange() const;

		/**
		*  @brief
		*    Sets the high range
		*
		*  @param[in] fHighRange
		*    New high range in degree
		*/
		PLPHYSICS_API void SetHighRange(float fHighRange = 180.0f);


	//[-------------------------------------------------------]
	//[ Public virtual JointHinge functions                   ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Adds omega to the bodies assigned with this joint
		*
		*  @param[in] fOmega
		*    Omega to add
		*/
		virtual void AddOmega(float fOmega) = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cWorld
		*    World this joint is in
		*  @param[in] cJointImpl
		*    Reference to the physics API specific joint implementation
		*  @param[in] pParentBody
		*    Pointer to the parent rigid body, can be a null pointer
		*  @param[in] pChildBody
		*    Pointer to the attached rigid body, can be a null pointer
		*  @param[in] vPivotPoint
		*    Origin of the hinge in world space
		*  @param[in] vPinDir
		*    The line of action of the hinge in world space
		*/
		PLPHYSICS_API JointHinge(World &cWorld, JointImpl &cJointImpl, Body *pParentBody, Body *pChildBody,
								 const PLMath::Vector3 &vPivotPoint, const PLMath::Vector3 &vPinDir);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLMath::Vector3 m_vPivotPoint;	/**< Origin of the hinge in world space */
		PLMath::Vector3 m_vPinDir;		/**< The line of action of the hinge in world space */


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float m_fLowRange;	/**< Low range in degree */
		float m_fHighRange;	/**< High range in degree */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLPHYSICS_JOINTHINGE_H__
