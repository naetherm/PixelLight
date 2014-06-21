/*********************************************************\
 *  File: JointImpl.cpp                                  *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLPhysics/Joint.h>
#include "PLPhysicsBullet/World.h"
#include "PLPhysicsBullet/JointImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLMath;
namespace PLPhysicsBullet {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the Newton physics joint
*/
btTypedConstraint *JointImpl::GetBulletConstraint() const
{
	return m_pBulletConstraint;
}

/**
*  @brief
*    Initializes the Bullet physics constraint
*/
void JointImpl::InitializeBulletConstraint(PLPhysics::Joint &cJoint, btTypedConstraint *pBulletConstraint)
{
	// Set data
	m_pBulletConstraint = pBulletConstraint;
	m_pJoint = &cJoint;
	if (m_pBulletConstraint)
	{
		btDynamicsWorld *pBulletWorld = static_cast<World&>(cJoint.GetWorld()).GetBulletWorld();
		if (pBulletWorld) {
			pBulletWorld->addConstraint(m_pBulletConstraint);
		}
		// Save the pointer to the PL physics joint
		m_pBulletConstraint->setUserConstraintPtr(&cJoint);
	}

}


//[-------------------------------------------------------]
//[ Public virtual PLPhysics::JointImpl functions         ]
//[-------------------------------------------------------]
PLPhysics::Joint *JointImpl::GetJoint() const
{
	return m_pJoint;
}

bool JointImpl::IsBreakable() const
{
	return m_bBreakable;
}

void JointImpl::SetBreakable(bool bBreakable)
{
	m_bBreakable = bBreakable;
}

void JointImpl::GetBreakForce(Vector3 &vForce) const
{
	vForce = m_vBreakForce;
}

void JointImpl::SetBreakForce(const Vector3 &vForce)
{
	m_vBreakForce = vForce;
}

void JointImpl::GetBreakTorque(Vector3 &vTorque) const
{
	vTorque = m_vBreakTorque;
}

void JointImpl::SetBreakTorque(const Vector3 &vTorque)
{
	m_vBreakTorque = vTorque;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
JointImpl::JointImpl() :
	m_pBulletConstraint(nullptr),
	m_bBreakable(false),
	m_pJoint(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
JointImpl::~JointImpl()
{
	// Is there a Newton physics joint?
	if (m_pBulletConstraint) {
		// Get the physics world
		PLPhysics::Joint *pJoint = GetJoint();
		if (pJoint) {
			PLPhysics::World &cWorld = pJoint->GetWorld();

			// Deactivate the physics simulation if required
			const bool bSimulationActive = cWorld.IsSimulationActive();
			if (bSimulationActive)
				cWorld.SetSimulationActive(false);

			btDynamicsWorld *pBulletWorld = static_cast<World&>(cWorld).GetBulletWorld();
			if (pBulletWorld) {
				pBulletWorld->removeConstraint(m_pBulletConstraint);
			}

			delete m_pBulletConstraint;

			// Reactivate the physics simulation if required
			if (bSimulationActive)
				cWorld.SetSimulationActive(bSimulationActive);
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsBullet
