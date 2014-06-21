/*********************************************************\
 *  File: SNMPhysicsJointHinge.cpp                       *
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
#include "PLPhysics/Joint.h"
#include "PLPhysics/World.h"
#include "PLPhysics/SceneNodes/SCPhysicsWorld.h"
#include "PLPhysics/SceneNodeModifiers/SNMPhysicsBody.h"
#include "PLPhysics/SceneNodeModifiers/SNMPhysicsJointHinge.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLScene;
namespace PLPhysics {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMPhysicsJointHinge)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
const Vector3 &SNMPhysicsJointHinge::GetPivotPoint() const
{
	return m_vPivotPoint;
}

void SNMPhysicsJointHinge::SetPivotPoint(const Vector3 &vValue)
{
	if (m_vPivotPoint != vValue) {
		m_vPivotPoint = vValue;
		RecreatePhysicsJoint();
	}
}

const Vector3 &SNMPhysicsJointHinge::GetPinDir() const
{
	return m_vPinDir;
}

void SNMPhysicsJointHinge::SetPinDir(const Vector3 &vValue)
{
	if (m_vPinDir != vValue) {
		m_vPinDir = vValue;
		RecreatePhysicsJoint();
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPhysicsJointHinge::SNMPhysicsJointHinge(SceneNode &cSceneNode) : SNMPhysicsJoint(cSceneNode),
	PivotPoint(this),
	PinDir(this),
	m_vPinDir(0.0f, 1.0f, 0.0f)
{
}

/**
*  @brief
*    Destructor
*/
SNMPhysicsJointHinge::~SNMPhysicsJointHinge()
{
}


//[-------------------------------------------------------]
//[ Private virtual SNMPhysicsJoint functions             ]
//[-------------------------------------------------------]
void SNMPhysicsJointHinge::CreatePhysicsJoint()
{
	if (m_pWorldContainer && m_pWorldContainer->GetWorld()) {
		// Get the owner and target physics bodies the joint is attached to
		const SNMPhysicsBody *pOwnerBody  = GetOwnerBodyModifier();
		const SNMPhysicsBody *pTargetBody = GetTargetBodyModifier();

		// Calculate the transform matrix that transform from the local owner scene node space into the physics world scene container
		Matrix3x4 mTransform;
		CalculateJointTransformMatrix(mTransform);

		// Get the pin direction
		const Vector3 vPinDir = ((GetFlags() & LocalPinDirection) ? mTransform.RotateVector(m_vPinDir) : m_vPinDir).Normalize();

		// Create the joint
		m_pJointHandler->SetElement(m_pWorldContainer->GetWorld()->CreateJointHinge(pTargetBody ? pTargetBody->GetBody() : nullptr,
																					pOwnerBody  ? pOwnerBody->GetBody()  : nullptr,
																					mTransform*m_vPivotPoint,
																					vPinDir));
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics
