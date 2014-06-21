/*********************************************************\
 *  File: JointUniversal.h                               *
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


#ifndef __PLPHYSICS_JOINTUNIVERSAL_H__
#define __PLPHYSICS_JOINTUNIVERSAL_H__
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
*    Abstract PL physics universal joint
*
*  @remarks
*    A universal joint is a constraint that restricts two rigid bodies to be connected to a point
*    fixed on both bodies, while and allowing one body to spin around a fix axis in is own frame,
*    and the other body to spin around another axis fixes on it own frame. Both axis must be
*    mutually perpendicular.
*
*    DOFs removed: 2 DOFs remaining: 4
*/
class JointUniversal : public Joint {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~JointUniversal();

		/**
		*  @brief
		*    Returns the origin of the universal in world space
		*
		*  @return
		*    The origin of the universal in world space
		*/
		PLPHYSICS_API const PLMath::Vector3 &GetPivotPoint() const;

		/**
		*  @brief
		*    Returns the first axis of rotation fixed on THIS body and perpendicular to 'PinDir2' in world space
		*
		*  @return
		*    The first axis of rotation fixed on THIS body and perpendicular to 'PinDir2' in world space
		*/
		PLPHYSICS_API const PLMath::Vector3 &GetPinDir1() const;

		/**
		*  @brief
		*    Returns the second axis of rotation fixed on 'Parent' body and perpendicular to 'PinDir1' in world space
		*
		*  @return
		*    The second axis of rotation fixed on 'Parent' body and perpendicular to 'PinDir1' in world space
		*/
		PLPHYSICS_API const PLMath::Vector3 &GetPinDir2() const;

		/**
		*  @brief
		*    Returns the low range of the first axis
		*
		*  @return
		*    The low range of the first axis in degree
		*/
		PLPHYSICS_API float GetLowRange1() const;

		/**
		*  @brief
		*    Sets the low range of the first axis
		*
		*  @param[in] fLowRange
		*    New low range of the first axis in degree
		*/
		PLPHYSICS_API void SetLowRange1(float fLowRange = -180.0f);

		/**
		*  @brief
		*    Returns the high range of the first axis
		*
		*  @return
		*    The high range of the first axis in degree
		*/
		PLPHYSICS_API float GetHighRange1() const;

		/**
		*  @brief
		*    Sets the high range of the first axis
		*
		*  @param[in] fHighRange
		*    New high range of the first axis in degree
		*/
		PLPHYSICS_API void SetHighRange1(float fHighRange = 180.0f);

		/**
		*  @brief
		*    Returns the low range of the second axis
		*
		*  @return
		*    The low range of the second axis in degree
		*/
		PLPHYSICS_API float GetLowRange2() const;

		/**
		*  @brief
		*    Sets the low range of the second axis
		*
		*  @param[in] fLowRange
		*    New low range of the second axis in degree
		*/
		PLPHYSICS_API void SetLowRange2(float fLowRange = -180.0f);

		/**
		*  @brief
		*    Returns the high range of the second axis
		*
		*  @return
		*    The high range of the second axis in degree
		*/
		PLPHYSICS_API float GetHighRange2() const;

		/**
		*  @brief
		*    Sets the high range of the second axis
		*
		*  @param[in] fHighRange
		*    New high range of the second axis in degree
		*/
		PLPHYSICS_API void SetHighRange2(float fHighRange = 180.0f);


	//[-------------------------------------------------------]
	//[ Public virtual JointUniversal functions               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Adds omega to the bodies assigned with this joint
		*
		*  @param[in] fOmega1
		*    Omega 1 to add
		*  @param[in] fOmega2
		*    Omega 2 to add
		*/
		virtual void AddOmega(float fOmega1, float fOmega2) = 0;


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
		*    Origin of the universal in world space
		*  @param[in] vPinDir1
		*    First axis of rotation fixed on THIS body and perpendicular to 'PinDir2' in world space
		*  @param[in] vPinDir2
		*    Second axis of rotation fixed on 'Parent' body and perpendicular to 'PinDir1' in world space
		*/
		PLPHYSICS_API JointUniversal(World &cWorld, JointImpl &cJointImpl, Body *pParentBody, Body *pChildBody, const PLMath::Vector3 &vPivotPoint,
									 const PLMath::Vector3 &vPinDir1, const PLMath::Vector3 &vPinDir2);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLMath::Vector3 m_vPivotPoint;	/**< Origin of the universal in world space */
		PLMath::Vector3 m_vPinDir1;		/**< First axis of rotation fixed on THIS body and perpendicular to 'PinDir2' in world space */
		PLMath::Vector3 m_vPinDir2;		/**< Second axis of rotation fixed on 'Parent' body and perpendicular to 'PinDir1' in world space */


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float m_fLowRange1;		/**< Low range of the first axis in degree */
		float m_fHighRange1;	/**< High range of the first axis  in degree */
		float m_fLowRange2;		/**< Low range of the second axis in degree */
		float m_fHighRange2;	/**< High range of the second axis  in degree */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLPHYSICS_JOINTUNIVERSAL_H__
