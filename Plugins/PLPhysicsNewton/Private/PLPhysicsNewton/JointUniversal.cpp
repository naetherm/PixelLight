/*********************************************************\
 *  File: JointUniversal.cpp                             *
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
#include <PLMath/Matrix3x4.h>
#include <PLPhysics/Body.h>
#include "PLPhysicsNewton/BodyImpl.h"
#include "PLPhysicsNewton/JointImpl.h"
#include "PLPhysicsNewton/World.h"
#include "PLPhysicsNewton/JointUniversal.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLPhysicsNewton {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
JointUniversal::~JointUniversal()
{
}


//[-------------------------------------------------------]
//[ Public Newton callback functions                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Static Newton joint user callback function
*/
unsigned JointUniversal::JointUserCallback(const Newton::NewtonJoint *pNewtonJoint, Newton::NewtonHingeSliderUpdateDesc *pDesc)
{
	// Get joint
	const JointUniversal *pJoint = static_cast<JointUniversal*>(NewtonJointGetUserData(pNewtonJoint));
	if (!pJoint)
		return 0;

	// [TODO] Breakable

	// Get min/max angle limits
	const float fAngleMinLimit1 = pJoint->GetLowRange1();
	const float fAngleMaxLimit1 = pJoint->GetHighRange1();
	const float fAngleMinLimit2 = pJoint->GetLowRange2();
	const float fAngleMaxLimit2 = pJoint->GetHighRange2();

	// Check limits
	uint32 nReturn = 0;
	// Limit 1
	float fAngle = NewtonUniversalGetJointAngle0(pNewtonJoint);
	if (fAngle > fAngleMaxLimit1) {
		// If the joint angle is large than the defined value, stop the universal
		pDesc[0].m_accel = NewtonUniversalCalculateStopAlpha0(pNewtonJoint, pDesc, fAngleMaxLimit1);
		nReturn |= 1;
	} else if (fAngle < fAngleMinLimit1) {
		// If the joint angle is smaller than the defined value, stop the universal
		pDesc[0].m_accel = NewtonUniversalCalculateStopAlpha0(pNewtonJoint, pDesc, fAngleMinLimit1);
		nReturn |= 1;
	}
	// Limit 2
	fAngle = NewtonUniversalGetJointAngle1(pNewtonJoint);
	if (fAngle > fAngleMaxLimit2) {
		// If the joint angle is large than the defined value, stop the universal
		pDesc[1].m_accel = NewtonUniversalCalculateStopAlpha1(pNewtonJoint, &pDesc[1], fAngleMaxLimit2);
		nReturn |= 2;
	} else if (fAngle < fAngleMinLimit2) {
		// If the joint angle is smaller than the defined value, stop the universal
		pDesc[1].m_accel = NewtonUniversalCalculateStopAlpha1(pNewtonJoint, &pDesc[1], fAngleMinLimit2);
		nReturn |= 2;
	}

	// No action need it if the joint angle is with the limits
	return nReturn;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
JointUniversal::JointUniversal(PLPhysics::World &cWorld, PLPhysics::Body *pParentBody, PLPhysics::Body *pChildBody,
							   const Vector3 &vPivotPoint, const Vector3 &vPinDir1, const Vector3 &vPinDir2) :
	PLPhysics::JointUniversal(cWorld, static_cast<World&>(cWorld).CreateJointImpl(), pParentBody, pChildBody, vPivotPoint, vPinDir1, vPinDir2)
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


	// [TODO] ??
	/*
	if (pNewtonParentBody)
		NewtonBodySetUserData(pNewtonParentBody, nullptr);
	if (pNewtonChildBody)
		NewtonBodySetUserData(pNewtonChildBody, nullptr);
	*/


	// Get body initial transform matrix
	if (pParentBody) {
		// Get transform matrix
		Quaternion qQ;
		pParentBody->GetRotation(qQ);
		Vector3 vPos;
		pParentBody->GetPosition(vPos);
		Matrix3x4 mTrans;
		mTrans.FromQuatTrans(qQ, vPos);

		// And transform the initial joint anchor into the body object space
		mTrans.Invert();
		m_vLocalAnchor = mTrans*vPivotPoint;
	}

	// Create the Newton physics joint
	Newton::NewtonJoint *pNewtonJoint = NewtonConstraintCreateUniversal(pNewtonWorld, m_vPivotPoint, m_vPinDir1,
																		m_vPinDir2, pNewtonChildBody, pNewtonParentBody);

	// Set Newton universal callback function
	NewtonUniversalSetUserCallback(pNewtonJoint, JointUserCallback);

	// Initialize the Newton physics joint
	static_cast<JointImpl&>(GetJointImpl()).InitializeNewtonJoint(*this, *pNewtonJoint);

	// Reactivate the physics simulation if required
	if (bSimulationActive)
		cWorld.SetSimulationActive(bSimulationActive);
}


//[-------------------------------------------------------]
//[ Public virtual PLPhysics::Joint functions             ]
//[-------------------------------------------------------]
void JointUniversal::GetCurrentPivotPoint(Vector3 &vPosition) const
{
	const PLPhysics::Body *pParentBody = GetParentBody();
	if (pParentBody) {
		// Get transform matrix
		Quaternion qQ;
		pParentBody->GetRotation(qQ);
		Vector3 vPos;
		pParentBody->GetPosition(vPos);
		Matrix3x4 mTrans;
		mTrans.FromQuatTrans(qQ, vPos);

		// Get the current joint anchor in world space
		vPosition = m_vLocalAnchor;
		vPosition *= mTrans;
	}
}


//[-------------------------------------------------------]
//[ Public virtual PLPhysics::JointUniversal functions    ]
//[-------------------------------------------------------]
void JointUniversal::AddOmega(float fOmega1, float fOmega2)
{
	// [TODO]
/*
	// [TESTME] ... unlike ODE Newton offers no add omega functions... :(
	const Matrix3x4 &mTrans = GetSceneNode()->GetTransformMatrix();
	m_vPinDir0Transformed = mTrans.RotateVector(m_vPinDir0);
	m_vPinDir1Transformed = mTrans.RotateVector(m_vPinDir1);
	Vector3 vOmega0 = m_vPinDir0Transformed;
	Vector3 vOmega1 = m_vPinDir1Transformed;
	vOmega0.x = vOmega0.x * fOmega1 + vOmega1.x * fOmega2;
	vOmega0.y = vOmega0.y * fOmega1 + vOmega1.y * fOmega2;
	vOmega0.z = vOmega0.z * fOmega1 + vOmega1.z * fOmega2;
	Newton::NewtonBody *pPhysicsBody = GetPhysicsBody();
	if (pPhysicsBody) {
		float fOmega[3];
		NewtonBodyGetOmega(pPhysicsBody, fOmega);
		fOmega[0] += vOmega0.x;
		fOmega[1] += vOmega0.y;
		fOmega[2] += vOmega0.z;
		NewtonBodySetOmega(pPhysicsBody, fOmega);
	}
	if (GetParentModifier() && (pPhysicsBody=GetParentModifier()->GetPhysicsBody())) {
		float fOmega[3];
		NewtonBodyGetOmega(pPhysicsBody, fOmega);
		fOmega[0] -= vOmega0.x;
		fOmega[1] -= vOmega0.y;
		fOmega[2] -= vOmega0.z;
		NewtonBodySetOmega(pPhysicsBody, fOmega);
	}
	*/
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsNewton
