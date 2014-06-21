/*********************************************************\
 *  File: SNMPhysicsCharacterController.cpp              *
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
#include <PLInput/Input/InputManager.h>
#include <PLRenderer/Animation/Animation.h>
#include <PLMesh/MeshHandler.h>
#include <PLMesh/MeshAnimationManager.h>
#include <PLScene/Scene/SceneContext.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLScene/Scene/SceneNodes/SNMesh.h>
#include "PLEngine/Controller/PhysicsCharacterController.h"
#include "PLEngine/Controller/SNMPhysicsCharacterController.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLInput;
using namespace PLRenderer;
using namespace PLMesh;
using namespace PLScene;
using namespace PLPhysics;
namespace PLEngine {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMPhysicsCharacterController)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPhysicsCharacterController::SNMPhysicsCharacterController(SceneNode &cSceneNode) : SNMPhysicsCharacter(cSceneNode),
	InputSemantic(this),
	YRotVelocity(this),
	IdleAnimation(this),
	IdleAnimationSpeed(this),
	WalkAnimation(this),
	WalkAnimationSpeed(this),
	RunAnimationSpeed(this),
	RotationNode(this),
	Flags(this),
	EventHandlerUpdate(&SNMPhysicsCharacterController::OnUpdate, this),
	m_bJumping(false),
	m_pController(new PhysicsCharacterController())
{
}

/**
*  @brief
*    Destructor
*/
SNMPhysicsCharacterController::~SNMPhysicsCharacterController()
{
	// Destroy the input controller
	delete m_pController;
}


//[-------------------------------------------------------]
//[ Public virtual PLScene::SceneNodeModifier functions   ]
//[-------------------------------------------------------]
Controller *SNMPhysicsCharacterController::GetInputController() const
{
	return m_pController;
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneNodeModifier functions ]
//[-------------------------------------------------------]
void SNMPhysicsCharacterController::InformedOnInit()
{
	// Emit the input controller found event of the scene context to tell everyone about our input controller
	InputManager::GetInstance()->EventInputControllerFound(m_pController, InputSemantic);
}

void SNMPhysicsCharacterController::OnActivate(bool bActivate)
{
	// Call base implementation
	SNMPhysicsCharacter::OnActivate(bActivate);

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
void SNMPhysicsCharacterController::OnUpdate()
{
	// Current time difference
	const float fTimeDiff = Timing::GetInstance()->GetTimeDifference()*10;

	// Get the scene node
	SceneNode &cSceneNode = GetSceneNode();

	// Revert animation?
	bool bRevert = false;

	// Movement
	Vector3 vMovement;

	// Check if input is active
	if (m_pController->GetActive()) {
		// Get direction vectors from the owner node or a special rotation node
		const SceneNode *pRotationNode = &cSceneNode;
		if (RotationNode.Get().GetLength() && cSceneNode.GetContainer()) {
			const SceneNode *pSceneNode = cSceneNode.GetContainer()->GetByName(RotationNode.Get());
			if (pSceneNode)
				pRotationNode = pSceneNode;
		}
		const Quaternion &qRot = pRotationNode->GetTransform().GetRotation();
		const Vector3 vDirLeftVector = qRot.GetXAxis();
		const Vector3 vDirUpVector   = qRot.GetYAxis();
		const Vector3 vDirVector     = qRot.GetZAxis();

		// Forward/backward
		if (m_pController->Forward.IsPressed())
			vMovement += vDirVector;
		if (m_pController->Backward.IsPressed()) {
			vMovement -= vDirVector;
			bRevert = true;
		}
		// For relative input values, we need to cancel out the time difference applied within the base method "SetMovement()"
		if (!m_pController->TransZ.IsValueRelative() || !fTimeDiff)
			vMovement += vDirVector*m_pController->TransZ.GetValue();
		else
			vMovement += vDirVector*(1.0f/fTimeDiff)*m_pController->TransZ.GetValue();

		// Strafe left/right
		if (m_pController->StrafeLeft.IsPressed())
			vMovement += vDirLeftVector;
		if (m_pController->StrafeRight.IsPressed())
			vMovement -= vDirLeftVector;
		// For relative input values, we need to cancel out the time difference applied within the base method "SetMovement()"
		if (!m_pController->TransX.IsValueRelative() || !fTimeDiff)
			vMovement += vDirLeftVector*m_pController->TransX.GetValue();
		else
			vMovement += vDirLeftVector*(1.0f/fTimeDiff)*m_pController->TransX.GetValue();

		// Upward/downward
		if (GetFlags() & YMovement) {
			if (m_pController->Up.IsPressed())
				vMovement += vDirUpVector;
			if (m_pController->Down.IsPressed())
				vMovement -= vDirUpVector;

			// For relative input values, we need to cancel out the time difference applied within the base method "SetMovement()"
			if (!m_pController->TransY.IsValueRelative() || !fTimeDiff)
				vMovement += vDirUpVector*m_pController->TransY.GetValue();
			else
				vMovement += vDirUpVector*(1.0f/fTimeDiff)*m_pController->TransY.GetValue();
		}
	}

	// Set new movement vector
	SetMovement(vMovement);

	// Playback idle animation?
	bool bIdle = vMovement.IsNull();

	// Check if input is active
	if (m_pController->GetActive()) {
		// Rotation
		if (YRotVelocity) {
			float fYRotationChange = 0.0f;
			if (m_pController->Left.IsPressed()) {
				fYRotationChange += fTimeDiff*YRotVelocity;
				bRevert = true;
			}
			if (m_pController->Right.IsPressed())
				fYRotationChange -= fTimeDiff*YRotVelocity;
			fYRotationChange += m_pController->RotY.GetValue()*fTimeDiff*YRotVelocity;
			if (fYRotationChange) {
				// Get a quaternion representation of the rotation delta
				Quaternion qRotInc;
				EulerAngles::ToQuaternion(0.0f, static_cast<float>(fYRotationChange*Math::DegToRad), 0.0f, qRotInc);

				// Update rotation
				GetSceneNode().GetTransform().SetRotation(GetSceneNode().GetTransform().GetRotation()*qRotInc);

				// We no longer idle
				bIdle = false;
			}
		}

		// Jump
		if (!(GetFlags() & NoJump) && JumpForce && m_pController->Jump.IsPressed()) {
			if (!m_bJumping) {
				Jump();
				bIdle = false;
				m_bJumping = true;
			}
		} else {
			m_bJumping = false;
		}

		// Toggle running mode
		SetRunning(!(GetFlags() & NoRun) && m_pController->Run.IsPressed());

		// Toggle creeping mode
		SetCreeping(!(GetFlags() & NoCreep) && m_pController->Crouch.IsPressed());
	}

	// Setup character animation
	MeshHandler *pMeshHandler = static_cast<SNMesh&>(cSceneNode).GetMeshHandler();
	if (pMeshHandler) {
		MeshAnimationManager *pAniManager = pMeshHandler->GetMeshAnimationManager();
		if (!pAniManager)
			pAniManager = pMeshHandler->CreateMeshAnimationManager();
		if (pAniManager) {
			if (bIdle) {
				if (IdleAnimation.Get().GetLength() && !pAniManager->GetByName(IdleAnimation.Get())) {
					pAniManager->Clear();
					// Start 'idle'-animation
					AnimationInfo *pAniInfo = pMeshHandler->GetAnimationInfo(IdleAnimation.Get());
					Animation     *pAni     = pAniManager->Create(IdleAnimation.Get());
					if (pAniInfo && pAni) {
						pAni->Start(*pAniInfo, true);
						pAni->SetSpeed(IdleAnimationSpeed);
					}
				}
			} else {
				Animation *pAni = pAniManager->GetByName(WalkAnimation.Get());
				if (!pAni && WalkAnimation.Get().GetLength()) {
					pAniManager->Clear();
					// Start 'walk'-animation
					AnimationInfo *pAniInfo = pMeshHandler->GetAnimationInfo(WalkAnimation.Get());
					if (pAniInfo) {
						pAni = pAniManager->Create(WalkAnimation.Get());
						if (pAni)
							pAni->Start(*pAniInfo, true);
					}
				}
				if (pAni) {
					if (bRevert)
						pAni->SetSpeed(IsRunning() ? -RunAnimationSpeed : -WalkAnimationSpeed);
					else
						pAni->SetSpeed(IsRunning() ?  RunAnimationSpeed.GetFloat() :  WalkAnimationSpeed);
					pAni->SetWeight(IsRunning() ? 1.5f : 1.0f);

					// Is the animation currently running?
					if (pAni->IsRunning()) {
						// Ensure that there's a "PLScene::SNMMeshUpdate" instance within the owner scene node which takes care of the frequent mesh update
						GetSNMMeshUpdate();
					}
				}
			}
		}
	}
}

/**
*  @brief
*    Returns a "PLScene::SNMMeshUpdate" instance from the owner scene node
*/
SNMMeshUpdate *SNMPhysicsCharacterController::GetSNMMeshUpdate() const
{
	// Is there already an instance of the "PLScene::SNMMeshUpdate" scene node modifier?
	static const String sSNMMeshUpdate = "PLScene::SNMMeshUpdate";
	SNMMeshUpdate *pSNMMeshUpdate = reinterpret_cast<SNMMeshUpdate*>(GetSceneNode().GetModifier(sSNMMeshUpdate));
	if (!pSNMMeshUpdate)
		pSNMMeshUpdate = reinterpret_cast<SNMMeshUpdate*>(GetSceneNode().AddModifier(sSNMMeshUpdate, "Flags=\"Automatic\""));

	// Return the SNMMeshUpdate instance
	return pSNMMeshUpdate;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
