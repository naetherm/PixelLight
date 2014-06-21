/*********************************************************\
 *  File: Joint.h                                        *
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


#ifndef __PLPHYSICS_JOINT_H__
#define __PLPHYSICS_JOINT_H__
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
#include "PLPhysics/Element.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLMath {
	class Vector3;
};
namespace PLPhysics {
	class Body;
	class JointImpl;
	class ElementHandler;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysics {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract PL physics joint (also called 'constraint') base class
*
*  @remarks
*    A joint will constrain the motion of the two bodies it's attached to. A joint can be
*    attached to two joints and one of this to bodies may be a null pointer which means that it's
*    attached to 'the world'. (static)
*/
class Joint : public Element {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~Joint();

		/**
		*  @brief
		*    Returns a reference to the physics API specific joint implementation
		*
		*  @return
		*    Reference to the physics API specific joint implementation
		*/
		PLPHYSICS_API JointImpl &GetJointImpl() const;

		/**
		*  @brief
		*    Returns a pointer to the parent rigid body
		*
		*  @return
		*    Pointer to the parent rigid body, can be a null pointer
		*/
		PLPHYSICS_API Body *GetParentBody() const;

		/**
		*  @brief
		*    Returns a pointer to the attached rigid body
		*
		*  @return
		*    Pointer to the attached rigid body, can be a null pointer
		*/
		PLPHYSICS_API Body *GetChildBody() const;


	//[-------------------------------------------------------]
	//[ Public virtual Joint functions                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Gets the current pivot point of the joint
		*
		*  @param[out] vPosition
		*    Will receive the current pivot point of the joint
		*/
		PLPHYSICS_API virtual void GetCurrentPivotPoint(PLMath::Vector3 &vPosition) const;

		/**
		*  @brief
		*    Returns whether the joint is breakable or not
		*
		*  @return
		*    'true' if the joint is breakable, else 'false'
		*/
		PLPHYSICS_API virtual bool IsBreakable() const;

		/**
		*  @brief
		*    Sets whether the joint is breakable or not
		*
		*  @param[in] bBreakable
		*    'true' if the joint is breakable, else 'false' (default setting)
		*
		*  @see
		*    - IsBreakable()
		*/
		PLPHYSICS_API virtual void SetBreakable(bool bBreakable);

		/**
		*  @brief
		*    Returns the break force
		*
		*  @param[out] vForce
		*    Will receive the break force
		*
		*  @see
		*    - IsBreakable()
		*/
		PLPHYSICS_API virtual void GetBreakForce(PLMath::Vector3 &vForce) const;

		/**
		*  @brief
		*    Sets the break force
		*
		*  @param[in] vForce
		*    The break force
		*
		*  @see
		*    - IsBreakable()
		*/
		PLPHYSICS_API virtual void SetBreakForce(const PLMath::Vector3 &vForce);

		/**
		*  @brief
		*    Returns the break torque
		*
		*  @param[out] vTorque
		*    Will receive the break torque
		*
		*  @see
		*    - IsBreakable()
		*/
		PLPHYSICS_API virtual void GetBreakTorque(PLMath::Vector3 &vTorque) const;

		/**
		*  @brief
		*    Sets the break torque
		*
		*  @param[in] vTorque
		*    The break torque
		*
		*  @see
		*    - IsBreakable()
		*/
		PLPHYSICS_API virtual void SetBreakTorque(const PLMath::Vector3 &vTorque);


	//[-------------------------------------------------------]
	//[ Public virtual Element functions                      ]
	//[-------------------------------------------------------]
	public:
		PLPHYSICS_API virtual bool IsBody() const override;
		PLPHYSICS_API virtual bool IsJoint() const override;
		PLPHYSICS_API virtual bool IsSensor() const override;


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
		*/
		PLPHYSICS_API Joint(World &cWorld, JointImpl &cJointImpl, Body *pParentBody, Body *pChildBody);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		JointImpl	   *m_pJointImpl;			/**< Pointer to the physics API specific joint implementation (always valid!) */
		ElementHandler *m_pParentBodyHandler;	/**< Parent body handler */
		ElementHandler *m_pChildBodyHandler;	/**< Child body handler */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLPHYSICS_JOINT_H__
