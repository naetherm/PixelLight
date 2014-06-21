/*********************************************************\
 *  File: JointImpl.h                                    *
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


#ifndef __PLPHYSICSBULLET_JOINTIMPL_H__
#define __PLPHYSICSBULLET_JOINTIMPL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector3.h>
#include <PLPhysics/JointImpl.h>
#include "PLPhysicsBullet/PLPhysicsBullet.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysicsBullet {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Newton physics joint implementation class
*/
class JointImpl : public PLPhysics::JointImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class World;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the Bullet physics constraint
		*
		*  @return
		*    The Bullet physics constraint, a null pointer when no constraint is used (e.g. for the JointUpVector, there is no constraint needed)
		*/
		PLPHYSICSBULLET_API btTypedConstraint *GetBulletConstraint() const;

		/**
		*  @brief
		*    Initializes the Bullet physics constraint
		*
		*  @param[in] cJoint
		*    PL physics joint this is the implementation from
		*  @param[in] pBulletConstraint
		*    Bullet physics Constraint
		*/
		void InitializeBulletConstraint(PLPhysics::Joint &cJoint, btTypedConstraint *pBulletConstraint);


	//[-------------------------------------------------------]
	//[ Public virtual PLPhysics::JointImpl functions         ]
	//[-------------------------------------------------------]
	public:
		PLPHYSICSBULLET_API virtual PLPhysics::Joint *GetJoint() const override;
		PLPHYSICSBULLET_API virtual bool IsBreakable() const override;
		PLPHYSICSBULLET_API virtual void SetBreakable(bool bBreakable) override;
		PLPHYSICSBULLET_API virtual void GetBreakForce(PLMath::Vector3 &vForce) const override;
		PLPHYSICSBULLET_API virtual void SetBreakForce(const PLMath::Vector3 &vForce) override;
		PLPHYSICSBULLET_API virtual void GetBreakTorque(PLMath::Vector3 &vTorque) const override;
		PLPHYSICSBULLET_API virtual void SetBreakTorque(const PLMath::Vector3 &vTorque) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*/
		JointImpl();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~JointImpl();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		btTypedConstraint	*m_pBulletConstraint;	/**< Bullet physics Constraint, can be a null pointer */
		bool				 m_bBreakable;		/**< Is this joint breakable? */
		PLMath::Vector3		 m_vBreakForce;		/**< Break force */
		PLMath::Vector3		 m_vBreakTorque;	/**< Break torque */
		PLPhysics::Joint	*m_pJoint;			/**< The associated PLPhysics::Joint instance*/


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsBullet


#endif // __PLPHYSICSBULLET_JOINTIMPL_H__
