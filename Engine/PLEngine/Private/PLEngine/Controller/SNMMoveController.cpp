/*********************************************************\
 *  File: SNMMoveController.cpp                          *
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
#include <PLInput/Input/InputManager.h>
#include <PLScene/Scene/SceneContext.h>
#include "PLEngine/Controller/MoveController.h"
#include "PLEngine/Controller/SNMMoveController.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLInput;
using namespace PLScene;
namespace PLEngine {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMMoveController)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMMoveController::SNMMoveController(SceneNode &cSceneNode) : SNMTransform(cSceneNode),
	InputSemantic(this),
	Speed(this),
	EventHandlerUpdate(&SNMMoveController::OnUpdate, this),
	m_pController(new MoveController())
{
}

/**
*  @brief
*    Destructor
*/
SNMMoveController::~SNMMoveController()
{
	// Destroy the input controller
	delete m_pController;
}


//[-------------------------------------------------------]
//[ Public virtual PLScene::SceneNodeModifier functions   ]
//[-------------------------------------------------------]
Controller *SNMMoveController::GetInputController() const
{
	return m_pController;
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneNodeModifier functions ]
//[-------------------------------------------------------]
void SNMMoveController::InformedOnInit()
{
	// Emit the input controller found event of the scene context to tell everyone about our input controller
	InputManager::GetInstance()->EventInputControllerFound(m_pController, InputSemantic);
}

void SNMMoveController::OnActivate(bool bActivate)
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
void SNMMoveController::OnUpdate()
{
	// Check if input is active
	if (m_pController->GetActive()) {
		// Get the scene node
		SceneNode &cSceneNode = GetSceneNode();

		// Get direction vectors
		const Quaternion &qRot = cSceneNode.GetTransform().GetRotation();
		const Vector3 vDirLeftVector = qRot.GetXAxis();
		const Vector3 vDirUpVector   = qRot.GetYAxis();
		const Vector3 vDirVector     = qRot.GetZAxis();

		// Set movement speed and don't forget to apply the current time difference
		float fCurrentSpeed = Speed*Timing::GetInstance()->GetTimeDifference();

		// Speed up
		if (m_pController->Run.IsPressed())
			fCurrentSpeed *= 4;

		// Slow down
		else if (m_pController->Crouch.IsPressed())
			fCurrentSpeed /= 4;

		// Movement vector
		Vector3 vMovement;

		// Forward/backward
		if (m_pController->Forward.IsPressed())
			vMovement += vDirVector*fCurrentSpeed;
		if (m_pController->Backward.IsPressed())
			vMovement -= vDirVector*fCurrentSpeed;
		vMovement += vDirVector*(m_pController->TransZ.IsValueRelative() ? m_pController->TransZ.GetValue() : m_pController->TransZ.GetValue()*fCurrentSpeed);

		// Left/right
		if (m_pController->StrafeLeft.IsPressed())
			vMovement += vDirLeftVector*fCurrentSpeed;
		if (m_pController->StrafeRight.IsPressed())
			vMovement -= vDirLeftVector*fCurrentSpeed;
		vMovement += vDirLeftVector*(m_pController->TransX.IsValueRelative() ? m_pController->TransX.GetValue() : m_pController->TransX.GetValue()*fCurrentSpeed);

		// Upward/downward
		if (m_pController->Up.IsPressed())
			vMovement += vDirUpVector*fCurrentSpeed;
		if (m_pController->Down.IsPressed())
			vMovement -= vDirUpVector*fCurrentSpeed;
		vMovement += vDirUpVector*(m_pController->TransY.IsValueRelative() ? m_pController->TransY.GetValue() : m_pController->TransY.GetValue()*fCurrentSpeed);

		// 'Move' to the new position
		if (!vMovement.IsNull())
			cSceneNode.MoveTo(cSceneNode.GetTransform().GetPosition() + vMovement);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
