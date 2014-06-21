/*********************************************************\
 *  File: SNMPhysicsJoint.cpp                            *
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
#include "PLPhysics/Body.h"
#include "PLPhysics/Joint.h"
#include "PLPhysics/ElementHandler.h"
#include "PLPhysics/SceneNodes/SCPhysicsWorld.h"
#include "PLPhysics/SceneNodeModifiers/SNMPhysicsJoint.h"


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
pl_implement_class(SNMPhysicsJoint)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SNMPhysicsJoint::GetTarget() const
{
	return m_sTarget;
}

void SNMPhysicsJoint::SetTarget(const String &sValue)
{
	if (m_sTarget != sValue) {
		m_sTarget = sValue;
		RecreatePhysicsJoint();
	}
}

void SNMPhysicsJoint::SetFlags(uint32 nValue)
{
	if (GetFlags() != nValue) {
		// Backup the previous local pin direction setting
		const bool bPreviousLocalPinDirection = (GetFlags() & LocalPinDirection) != 0;

		// Call base implementation
		SNMPhysics::SetFlags(nValue);

		// Local pin direction setting changed?
		if (((nValue & LocalPinDirection) != 0) != bPreviousLocalPinDirection)
			RecreatePhysicsJoint();
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the PL physics world scene node container the physics joint is in
*/
SCPhysicsWorld *SNMPhysicsJoint::GetWorldContainer() const
{
	return m_pWorldContainer;
}

/**
*  @brief
*    Returns the PL physics joint
*/
Joint *SNMPhysicsJoint::GetJoint() const
{
	return static_cast<Joint*>(m_pJointHandler->GetElement());
}

/**
*  @brief
*    Returns the owner PL physics body scene node modifier the joint is attached to
*/
SNMPhysicsBody *SNMPhysicsJoint::GetOwnerBodyModifier() const
{
	return reinterpret_cast<SNMPhysicsBody*>(GetSceneNode().GetModifier("PLPhysics::SNMPhysicsBody"));
}

/**
*  @brief
*    Returns the target PL physics body scene node modifier the joint is attached to
*/
SNMPhysicsBody *SNMPhysicsJoint::GetTargetBodyModifier() const
{
	const SceneNode *pSceneNode = GetSceneNode().GetContainer()->GetByName(m_sTarget);
	return pSceneNode ? reinterpret_cast<SNMPhysicsBody*>(pSceneNode->GetModifier("PLPhysics::SNMPhysicsBody")) : nullptr;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPhysicsJoint::SNMPhysicsJoint(SceneNode &cSceneNode) : SNMPhysics(cSceneNode),
	Target(this),
	Flags(this),
	m_pWorldContainer(nullptr),
	m_pJointHandler(new ElementHandler())
{
}

/**
*  @brief
*    Destructor
*/
SNMPhysicsJoint::~SNMPhysicsJoint()
{
	// Destroy the PL physics joint
	if (m_pJointHandler->GetElement())
		delete m_pJointHandler->GetElement();
	delete m_pJointHandler;
}

/**
*  @brief
*    Recreates the PL physics joint
*/
void SNMPhysicsJoint::RecreatePhysicsJoint()
{
	// Destroy the old physics joint
	if (m_pJointHandler->GetElement())
		delete m_pJointHandler->GetElement();

	// Create the physics joint
	CreatePhysicsJoint();
}

/**
*  @brief
*    Calculates the transform matrix that transform from the local owner scene node space into the physics world scene container
*/
void SNMPhysicsJoint::CalculateJointTransformMatrix(Matrix3x4 &mTransform) const
{
	// Is the owner scene node directly within the physics world scene container? Are both containers valid?
	SceneContainer *pThisSceneContainer = GetSceneNode().GetContainer();
	if (m_pWorldContainer != pThisSceneContainer && m_pWorldContainer && pThisSceneContainer) {
		// Get container transform matrix...
		pThisSceneContainer->GetTransformMatrixTo(*m_pWorldContainer, mTransform);

		// ... and do also apply the scene node transform matrix
		mTransform *= GetSceneNode().GetTransform().GetMatrix();
	} else {
		// Just use directly the scene node transform matrix
		mTransform = GetSceneNode().GetTransform().GetMatrix();
	}
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneNodeModifier functions ]
//[-------------------------------------------------------]
void SNMPhysicsJoint::InformedOnInit()
{
	// Get PL physics world this joint is in
	SceneContainer *pContainer = GetSceneNode().GetContainer();
	while (pContainer && !pContainer->IsInstanceOf("PLPhysics::SCPhysicsWorld"))
		pContainer = pContainer->GetContainer();

	// Setup physics joint
	if (pContainer) {
		// Backup pointer to the world container
		m_pWorldContainer = static_cast<SCPhysicsWorld*>(pContainer);
		if (m_pWorldContainer->GetWorld()) {
			// Create the physics joint
			CreatePhysicsJoint();
		} else {
			// ??
			m_pWorldContainer = nullptr;
		}
	} else {
		// No world, no physics...
		m_pWorldContainer = nullptr;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics
