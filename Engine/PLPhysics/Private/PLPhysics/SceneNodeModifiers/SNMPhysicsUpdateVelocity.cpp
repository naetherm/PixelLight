/*********************************************************\
 *  File: SNMPhysicsUpdateVelocity.cpp                   *
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
#include <PLCore/Tools/Timing.h>
#include <PLMath/EulerAngles.h>
#include "PLPhysics/Body.h"
#include "PLPhysics/SceneNodeModifiers/SNMPhysicsBody.h"
#include "PLPhysics/SceneNodeModifiers/SNMPhysicsUpdateVelocity.h"


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
pl_implement_class(SNMPhysicsUpdateVelocity)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPhysicsUpdateVelocity::SNMPhysicsUpdateVelocity(SceneNode &cSceneNode) : SNMPhysics(cSceneNode),
	EventHandlerPosition(&SNMPhysicsUpdateVelocity::OnPosition, this),
	EventHandlerRotation(&SNMPhysicsUpdateVelocity::OnRotation, this),
	m_bPreviousPosition(false),
	m_bPreviousRotation(false)
{
}

/**
*  @brief
*    Destructor
*/
SNMPhysicsUpdateVelocity::~SNMPhysicsUpdateVelocity()
{
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneNodeModifier functions ]
//[-------------------------------------------------------]
void SNMPhysicsUpdateVelocity::OnActivate(bool bActivate)
{
	// Connect/disconnect event handler
	SceneNode &cSceneNode = GetSceneNode();
	if (bActivate) {
		// Connect event handler
		cSceneNode.GetTransform().EventPosition.Connect(EventHandlerPosition);
		cSceneNode.GetTransform().EventRotation.Connect(EventHandlerRotation);

		// Make a first update to ensure position and rotation are up-to-date when we're going active (synchronization, no logic update)
		OnPosition();
		OnRotation();
	} else {
		// Disconnect event handler
		cSceneNode.GetTransform().EventPosition.Disconnect(EventHandlerPosition);
		cSceneNode.GetTransform().EventRotation.Disconnect(EventHandlerRotation);
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node position changed
*/
void SNMPhysicsUpdateVelocity::OnPosition()
{
	// Get the scene node
	const SceneNode &cSceneNode = GetSceneNode();

	// Is there a previous scene node position?
	if (m_bPreviousPosition) {
		// Get the time difference between the previous and the current frame
		const float fTimeDiff = Timing::GetInstance()->GetTimeDifference();

		// Calculate the difference between the previous and the current scene node position
		const Vector3 vPositionDifference = cSceneNode.GetTransform().GetPosition() - m_vPreviousPosition;

		// Calculate the current linear velocity of the scene node
		const Vector3 vVelocity = vPositionDifference*(1/fTimeDiff);

		// Get the number of physics body scene node modifiers
		static const String sSNMPhysicsBody = "PLPhysics::SNMPhysicsBody";
		const uint32 nNumOfModifiers = cSceneNode.GetNumOfModifiers(sSNMPhysicsBody);

		// Loop through all scene node modifiers and update the velocities of all static physics bodies
		for (uint32 i=0; i<nNumOfModifiers; i++) {
			// Get the physics body scene node modifier
			const SNMPhysicsBody *pSNMPhysicsBody = static_cast<SNMPhysicsBody*>(cSceneNode.GetModifier(sSNMPhysicsBody, i));
			if (pSNMPhysicsBody) {
				// Get the physics body, mass of 0 = static body
				Body *pBody = pSNMPhysicsBody->GetBody();
				if (pBody && pBody->GetMass() == 0.0f) {
					// Set the current linear velocity of the physics body
					pBody->SetLinearVelocity(vVelocity);
				}
			}
		}

		// Update the previous scene node position
		m_vPreviousPosition = cSceneNode.GetTransform().GetPosition();
	} else {
		// We now have a previous scene node position
		m_bPreviousPosition = true;
		m_vPreviousPosition = cSceneNode.GetTransform().GetPosition();
	}
}

/**
*  @brief
*    Called when the scene node rotation changed
*/
void SNMPhysicsUpdateVelocity::OnRotation()
{
	// Get the scene node
	const SceneNode &cSceneNode = GetSceneNode();

	// Is there a previous scene node rotation?
	if (m_bPreviousRotation) {
		// Get the time difference between the previous and the current frame
		const float fTimeDiff = Timing::GetInstance()->GetTimeDifference();

		// Calculate the difference between the previous and the current scene node rotation
		const Quaternion qRotationDifference = cSceneNode.GetTransform().GetRotation()*m_qPreviousRotation.GetInverted();

		// Calculate the current angular velocity of the scene node
		Vector3 vVelocity;
		EulerAngles::FromQuaternion(qRotationDifference, vVelocity.x, vVelocity.y, vVelocity.z);
		vVelocity *= (1/fTimeDiff);

		// Get the number of physics body scene node modifiers
		static const String sSNMPhysicsBody = "PLPhysics::SNMPhysicsBody";
		const uint32 nNumOfModifiers = cSceneNode.GetNumOfModifiers(sSNMPhysicsBody);

		// Loop through all scene node modifiers and update the velocities of all static physics bodies
		for (uint32 i=0; i<nNumOfModifiers; i++) {
			// Get the physics body scene node modifier
			const SNMPhysicsBody *pSNMPhysicsBody = static_cast<SNMPhysicsBody*>(cSceneNode.GetModifier(sSNMPhysicsBody, i));
			if (pSNMPhysicsBody) {
				// Get the physics body, mass of 0 = static body
				Body *pBody = pSNMPhysicsBody->GetBody();
				if (pBody && pBody->GetMass() == 0.0f) {
					// Set the current angular velocity of the physics body
					pBody->SetAngularVelocity(vVelocity);
				}
			}
		}

		// Update the previous scene node rotation
		m_qPreviousRotation = cSceneNode.GetTransform().GetRotation();
	} else {
		// We now have a previous scene node rotation
		m_bPreviousRotation = true;
		m_qPreviousRotation = cSceneNode.GetTransform().GetRotation();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics
