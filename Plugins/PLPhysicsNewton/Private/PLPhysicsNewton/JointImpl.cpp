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
#include "PLPhysicsNewton/World.h"
#include "PLPhysicsNewton/JointImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLMath;
namespace PLPhysicsNewton {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the Newton physics joint
*/
Newton::NewtonJoint *JointImpl::GetNewtonJoint() const
{
	return m_pNewtonJoint;
}

/**
*  @brief
*    Initializes the Newton physics joint
*/
void JointImpl::InitializeNewtonJoint(PLPhysics::Joint &cJoint, Newton::NewtonJoint &cNewtonJoint)
{
	// Set data
	m_pNewtonJoint = &cNewtonJoint;

	// Save the pointer to the PL physics joint
	NewtonJointSetUserData(m_pNewtonJoint, &cJoint);

	// Set a destructor for this joint
	NewtonJointSetDestructor(m_pNewtonJoint, World::PhysicsJointDestructor);
}


//[-------------------------------------------------------]
//[ Public virtual PLPhysics::JointImpl functions         ]
//[-------------------------------------------------------]
PLPhysics::Joint *JointImpl::GetJoint() const
{
	return static_cast<PLPhysics::Joint*>(NewtonJointGetUserData(m_pNewtonJoint));
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
	m_pNewtonJoint(nullptr),
	m_bBreakable(false)
{
}

/**
*  @brief
*    Destructor
*/
JointImpl::~JointImpl()
{
	// Is there a Newton physics joint?
	if (m_pNewtonJoint) {
		// Get the physics world
		PLPhysics::Joint *pJoint = GetJoint();
		if (pJoint) {
			PLPhysics::World &cWorld = pJoint->GetWorld();

			// Deactivate the physics simulation if required
			const bool bSimulationActive = cWorld.IsSimulationActive();
			if (bSimulationActive)
				cWorld.SetSimulationActive(false);

			// Reset the pointer to the PL physics joint
			NewtonJointSetUserData(m_pNewtonJoint, nullptr);

			// Destroy the Newton physics joint, there's no function like NewtonJointGetWorld(m_pNewtonJoint) :(
			NewtonDestroyJoint(static_cast<World&>(cWorld).GetNewtonWorld(), m_pNewtonJoint);

			// Reactivate the physics simulation if required
			if (bSimulationActive)
				cWorld.SetSimulationActive(bSimulationActive);
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsNewton
