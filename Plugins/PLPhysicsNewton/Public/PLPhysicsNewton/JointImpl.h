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


#ifndef __PLPHYSICSNEWTON_JOINTIMPL_H__
#define __PLPHYSICSNEWTON_JOINTIMPL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector3.h>
#include <PLPhysics/JointImpl.h>
#include "PLPhysicsNewton/PLPhysicsNewton.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysicsNewton {


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
		*    Returns the Newton physics joint
		*
		*  @return
		*    The Newton physics joint, a null pointer on error (of that's the case, something went totally wrong :)
		*
		*  @note
		*    - Do NOT change the user data of the Newton physics joínt! (using NewtonJointSetUserData())
		*      This data MUST hold a pointer to the PL physics joint.
		*/
		PLPHYSICSNEWTON_API Newton::NewtonJoint *GetNewtonJoint() const;

		/**
		*  @brief
		*    Initializes the Newton physics joint
		*
		*  @param[in] cJoint
		*    PL physics joint this is the implementation from
		*  @param[in] cNewtonJoint
		*    Newton physics joint
		*/
		void InitializeNewtonJoint(PLPhysics::Joint &cJoint, Newton::NewtonJoint &cNewtonJoint);


	//[-------------------------------------------------------]
	//[ Public virtual PLPhysics::JointImpl functions         ]
	//[-------------------------------------------------------]
	public:
		PLPHYSICSNEWTON_API virtual PLPhysics::Joint *GetJoint() const override;
		PLPHYSICSNEWTON_API virtual bool IsBreakable() const override;
		PLPHYSICSNEWTON_API virtual void SetBreakable(bool bBreakable) override;
		PLPHYSICSNEWTON_API virtual void GetBreakForce(PLMath::Vector3 &vForce) const override;
		PLPHYSICSNEWTON_API virtual void SetBreakForce(const PLMath::Vector3 &vForce) override;
		PLPHYSICSNEWTON_API virtual void GetBreakTorque(PLMath::Vector3 &vTorque) const override;
		PLPHYSICSNEWTON_API virtual void SetBreakTorque(const PLMath::Vector3 &vTorque) override;


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
		Newton::NewtonJoint *m_pNewtonJoint;	/**< Newton physics joint, can be a null pointer */
		bool				 m_bBreakable;		/**< Is this joint breakable? */
		PLMath::Vector3		 m_vBreakForce;		/**< Break force */
		PLMath::Vector3		 m_vBreakTorque;	/**< Break torque */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsNewton


#endif // __PLPHYSICSNEWTON_JOINTIMPL_H__
