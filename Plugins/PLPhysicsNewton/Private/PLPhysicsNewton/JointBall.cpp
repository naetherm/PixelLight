/*********************************************************\
 *  File: JointBall.cpp                                  *
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
#include <PLPhysics/Body.h>
#include "PLPhysicsNewton/BodyImpl.h"
#include "PLPhysicsNewton/JointImpl.h"
#include "PLPhysicsNewton/World.h"
#include "PLPhysicsNewton/JointBall.h"


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
*    Destructor
*/
JointBall::~JointBall()
{
}


//[-------------------------------------------------------]
//[ Public Newton callback functions                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Static Newton joint user callback function
*/
void JointBall::JointUserCallback(const Newton::NewtonJoint *pNewtonJoint, dFloat fTimeStep)
{
	// Get joint
	JointBall *pJoint = static_cast<JointBall*>(NewtonJointGetUserData(pNewtonJoint));
	if (pJoint) {
		// Check limits
		// [TODO]
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
JointBall::JointBall(PLPhysics::World &cWorld, PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody,
					 const Vector3 &vPivotPoint, const Vector3 &vPinDir) :
	PLPhysics::JointBall(cWorld, static_cast<World&>(cWorld).CreateJointImpl(), pParentBody, pChildBody, vPivotPoint, vPinDir)
{
	// Deactivate the physics simulation if required
	const bool bSimulationActive = cWorld.IsSimulationActive();
	if (bSimulationActive)
		cWorld.SetSimulationActive(false);

	// Get the Newton physics world
	Newton::NewtonWorld *pNewtonWorld = static_cast<World&>(cWorld).GetNewtonWorld();

	// Flush assigned bodies (MUST be done before creating the joint!)
	if (pParentBody)
		static_cast<BodyImpl&>(pParentBody->GetBodyImpl()).Flush();
	if (pChildBody)
		static_cast<BodyImpl&>(pChildBody ->GetBodyImpl()).Flush();

	// Get the Newton physics parent and child bodies
	const Newton::NewtonBody *pNewtonParentBody = pParentBody ? static_cast<BodyImpl&>(pParentBody->GetBodyImpl()).GetNewtonBody() : nullptr;
	const Newton::NewtonBody *pNewtonChildBody  = pChildBody  ? static_cast<BodyImpl&>(pChildBody ->GetBodyImpl()).GetNewtonBody() : nullptr;

	// Create the Newton physics joint
	Newton::NewtonJoint *pNewtonJoint = NewtonConstraintCreateBall(pNewtonWorld, m_vPivotPoint,
																   pNewtonChildBody, pNewtonParentBody);

	// Set Newton ball callback function
	NewtonBallSetUserCallback(pNewtonJoint, JointUserCallback);

	// [TODO]
	NewtonBallSetConeLimits(pNewtonJoint, m_vPinDir, 0.0f, 0.0f);
//	NewtonBallSetConeLimits(pNewtonJoint, m_vPinDir, static_cast<dFloat>(m_fMaxConeAngle*Math::DegToRad), static_cast<dFloat>(m_fMaxTwistAngle*Math::DegToRad));

	// Initialize the Newton physics joint
	static_cast<JointImpl&>(GetJointImpl()).InitializeNewtonJoint(*this, *pNewtonJoint);

	// Reactivate the physics simulation if required
	if (bSimulationActive)
		cWorld.SetSimulationActive(bSimulationActive);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsNewton
