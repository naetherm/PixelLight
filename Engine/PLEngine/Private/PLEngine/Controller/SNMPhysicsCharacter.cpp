/*********************************************************\
 *  File: SNMPhysicsCharacter.cpp                        *
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
#include <PLScene/Scene/SceneContext.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLPhysics/Body.h>
#include <PLPhysics/World.h>
#include <PLPhysics/Sensor.h>
#include <PLPhysics/SceneNodeModifiers/SNMPhysicsBody.h>
#include "PLEngine/Controller/SNMPhysicsCharacter.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLScene;
using namespace PLPhysics;
namespace PLEngine {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMPhysicsCharacter)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPhysicsCharacter::SNMPhysicsCharacter(SceneNode &cSceneNode) : SNMPhysics(cSceneNode),
	Speed(this),
	Acceleration(this),
	MaxAcceleration(this),
	JumpForce(this),
	JumpReadyTime(this),
	JumpGroundDistance(this),
	Flags(this),
	EventHandlerUpdate(&SNMPhysicsCharacter::OnUpdate, this),
	m_bRun(false),
	m_bCreep(false),
	m_fJumpReadyTimer(0.0f),
	m_bJump(false)
{
}

/**
*  @brief
*    Destructor
*/
SNMPhysicsCharacter::~SNMPhysicsCharacter()
{
}

/**
*  @brief
*    Gets the running mode
*/
bool SNMPhysicsCharacter::IsRunning() const
{
	return m_bRun;
}

/**
*  @brief
*    Sets the running mode
*/
void SNMPhysicsCharacter::SetRunning(bool bRun)
{
	m_bRun = bRun;
}

/**
*  @brief
*    Gets the creeping mode
*/
bool SNMPhysicsCharacter::IsCreeping() const
{
	return m_bCreep;
}

/**
*  @brief
*    Sets the creeping mode
*/
void SNMPhysicsCharacter::SetCreeping(bool bCreep)
{
	m_bCreep = bCreep;
}

/**
*  @brief
*    Gets the movement vector
*/
const Vector3 &SNMPhysicsCharacter::GetMovement() const
{
	return m_vMovement;
}

/**
*  @brief
*    Sets the movement vector
*/
void SNMPhysicsCharacter::SetMovement(const Vector3 &vMovement)
{
	// Set new movement vector
	m_vMovement = vMovement;
}

/**
*  @brief
*    Performs a jump
*/
bool SNMPhysicsCharacter::Jump()
{
	// Make a ground test, else we can even jump in the middle of the air ;-)
	m_bJump = false;

	// Are we allowed to jump?
	if (!m_fJumpReadyTimer) {
		// Get the owner scene node
		const SceneNode &cSceneNode = GetSceneNode();

		// Get the PL physics body scene node modifier of the scene node
		const SNMPhysicsBody *pModifier = static_cast<SNMPhysicsBody*>(cSceneNode.GetModifier("PLPhysics::SNMPhysicsBody"));
		if (pModifier) {
			// Get the PL physics body of the scene node
			const Body *pBody = pModifier->GetBody();
			if (pBody) {
				// Get the minimum and maximum of the axis aligned bounding box of the physics body
				if (JumpGroundDistance < 0.001f)
					JumpGroundDistance = 0.001f;
				AABoundingBox cAABoundingBox;
				pBody->GetAABoundingBox(cAABoundingBox);
				const float fWidth = cAABoundingBox.GetWidth()*0.25f;
				const float fDepth = cAABoundingBox.GetDepth()*0.25f;
				Vector3 vMin = cAABoundingBox.vMin;
				Vector3 vMax = cAABoundingBox.vMax;
				vMin.x -= fWidth;
				vMax.x += fWidth;
				vMax.y = vMin.y;
				vMin.y -= JumpGroundDistance;
				vMin.z -= fDepth;
				vMax.z += fDepth;

				// Perform physics axis aligned bounding box check
				Sensor *pSensor = pBody->GetWorld().CreateSensorAABoundingBox(vMin, vMax);
				if (pSensor) {
					// Perform an immediate check
					if (pSensor->Check()) {
						// Get a hit physics body that isn't the character body
						for (uint32 i=0; i<pSensor->GetNumOfHitBodies(); i++) {
							const Sensor::BodyInfo *pBodyInfo = pSensor->GetHitBody(i);
							if (pBodyInfo && pBodyInfo->pBody && pBodyInfo->pBody != pBody) {
								// We can jump...
								m_bJump			  = true;
								m_fJumpReadyTimer = JumpReadyTime;

								// Done
								i = pSensor->GetNumOfHitBodies();
							}
						}
					}

					// Cleanup
					delete pSensor;
				}
			}
		}
	}

	// Done
	return m_bJump;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the PL physics body of the scene node
*/
Body *SNMPhysicsCharacter::GetPhysicsBody() const
{
	// Get the PL physics body scene node modifier of the scene node
	const SNMPhysicsBody *pModifier = static_cast<SNMPhysicsBody*>(GetSceneNode().GetModifier("PLPhysics::SNMPhysicsBody"));

	// Return the PL physics body this modifier is using
	return pModifier ? pModifier->GetBody() : nullptr;
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneNodeModifier functions ]
//[-------------------------------------------------------]
void SNMPhysicsCharacter::OnActivate(bool bActivate)
{
	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate)
			pSceneContext->EventUpdate.Connect(EventHandlerUpdate);
		else
			pSceneContext->EventUpdate.Disconnect(EventHandlerUpdate);
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node modifier needs to be updated
*/
void SNMPhysicsCharacter::OnUpdate()
{
	// Get the body
	Body *pBody = GetPhysicsBody();
	if (pBody) {
		// Get the direction we're moving into and the movement speed (normalize movement vector if the length is >1)
		float fMovementSpeed = m_vMovement.GetLength();
		Vector3 vMovementDirection;
		if (GetFlags() & YMovement) {
			// Movement on all axis - normalize the movement direction vector
			vMovementDirection = m_vMovement;
			if (fMovementSpeed)
				vMovementDirection /= fMovementSpeed;
		} else {
			// Movement is just performed on the x/z plane
			vMovementDirection.SetXYZ(m_vMovement.x, 0.0f, m_vMovement.z);
			vMovementDirection.Normalize();
		}
		if (fMovementSpeed > 1.0f)
			fMovementSpeed = 1.0f;
		if (Speed < 0.0f)
			Speed = 0.0f;
		fMovementSpeed *= Speed;

		// Get the current velocity
		Vector3 vVelocity;
		pBody->GetLinearVelocity(vVelocity);

		// Get velocity we want to apply on our character
		Vector3 vGoalVelocity(vMovementDirection.x*fMovementSpeed, (GetFlags() & YMovement) ? vMovementDirection.y*fMovementSpeed : 0.0f, vMovementDirection.z*fMovementSpeed);

		// Scale it by 3 up if the character wants to run
		if (m_bRun)
			vGoalVelocity *= 3.0f;

		// Scale it by 3 down if the character wants to creep
		if (m_bCreep)
			vGoalVelocity /= 3.0f;

		// Calculate acceleration needed to get to our goal velocity
		if (Acceleration < 0.0f)
			Acceleration = 0.0f;
		Vector3 vAccel(Acceleration*(vGoalVelocity.x - vVelocity.x),
					   (GetFlags() & YMovement) ? Acceleration*(vGoalVelocity.y - vVelocity.y) : 0.0f,
					   Acceleration*(vGoalVelocity.z - vVelocity.z));

		// Limit acceleration
		if (MaxAcceleration < 0.0f)
			MaxAcceleration = 0.0f;
		if (vAccel.x >  MaxAcceleration)
			vAccel.x =  MaxAcceleration;
		if (vAccel.x < -MaxAcceleration)
			vAccel.x = -MaxAcceleration;
		if (vAccel.z >  MaxAcceleration)
			vAccel.z =  MaxAcceleration;
		if (vAccel.z < -MaxAcceleration)
			vAccel.z = -MaxAcceleration;

		// Now finally add the force to the characters's body
		pBody->SetForce(vAccel*pBody->GetMass());

		// Add jump force
		if (JumpForce < 0.0f)
			JumpForce = 0.0f;
		if (m_bJump && JumpForce) {
			vVelocity.y += JumpForce;
			pBody->SetLinearVelocity(vVelocity);
			m_bJump = false;
		}

		// Update the jump ready timer
		if (m_fJumpReadyTimer) {
			m_fJumpReadyTimer -= Timing::GetInstance()->GetTimeDifference();
			if (m_fJumpReadyTimer < 0.0f)
				m_fJumpReadyTimer = 0.0f;
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
