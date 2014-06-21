/*********************************************************\
 *  File: SNMPhysicsJointUpVector.cpp                    *
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
#include "PLPhysics/SceneNodeModifiers/SNMPhysicsJointUpVector.h"


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
pl_implement_class(SNMPhysicsJointUpVector)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
const Vector3 &SNMPhysicsJointUpVector::GetPinDir() const
{
	return m_vPinDir;
}

void SNMPhysicsJointUpVector::SetPinDir(const Vector3 &vValue)
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
SNMPhysicsJointUpVector::SNMPhysicsJointUpVector(SceneNode &cSceneNode) : SNMPhysicsJoint(cSceneNode),
	PinDir(this),
	m_vPinDir(0.0f, 1.0f, 0.0f)
{
}

/**
*  @brief
*    Destructor
*/
SNMPhysicsJointUpVector::~SNMPhysicsJointUpVector()
{
}


//[-------------------------------------------------------]
//[ Private virtual SNMPhysicsJoint functions             ]
//[-------------------------------------------------------]
void SNMPhysicsJointUpVector::CreatePhysicsJoint()
{
	if (m_pWorldContainer && m_pWorldContainer->GetWorld()) {
		// Get the owner physics bodies the joint is attached to - we really just need the owner, there's no target!
		const SNMPhysicsBody *pOwnerBody = GetOwnerBodyModifier();

		// Create the joint
		if (pOwnerBody && pOwnerBody->GetBody()) {
			// Get the pin direction
			const Vector3 vPinDir = ((GetFlags() & LocalPinDirection) ? GetSceneNode().GetTransform().GetMatrix().RotateVector(m_vPinDir) : m_vPinDir).Normalize();

			// Create the joint
			m_pJointHandler->SetElement(m_pWorldContainer->GetWorld()->CreateJointUpVector(*pOwnerBody->GetBody(), vPinDir));
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics
