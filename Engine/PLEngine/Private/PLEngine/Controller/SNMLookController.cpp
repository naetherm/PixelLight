/*********************************************************\
 *  File: SNMLookController.cpp                          *
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
#include <PLScene/Scene/SceneContext.h>
#include "PLEngine/Controller/LookController.h"
#include "PLEngine/Controller/SNMLookController.h"


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
pl_implement_class(SNMLookController)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMLookController::SNMLookController(SceneNode &cSceneNode) : SNMTransform(cSceneNode),
	InputSemantic(this),
	Flags(this),
	SlotOnUpdate(this),
	m_pController(new LookController())
{
	// Overwrite the default setting of the flags
	SetFlags(GetFlags()|UseRotationKey);
}

/**
*  @brief
*    Destructor
*/
SNMLookController::~SNMLookController()
{
	// Destroy the input controller
	delete m_pController;
}


//[-------------------------------------------------------]
//[ Public virtual PLScene::SceneNodeModifier functions   ]
//[-------------------------------------------------------]
Controller *SNMLookController::GetInputController() const
{
	return m_pController;
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneNodeModifier functions ]
//[-------------------------------------------------------]
void SNMLookController::InformedOnInit()
{
	// Emit the input controller found event of the scene context to tell everyone about our input controller
	InputManager::GetInstance()->EventInputControllerFound(m_pController, InputSemantic);
}

void SNMLookController::OnActivate(bool bActivate)
{
	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate)
			pSceneContext->EventUpdate.Connect(SlotOnUpdate);
		else
			pSceneContext->EventUpdate.Disconnect(SlotOnUpdate);
	}
}


//[-------------------------------------------------------]
//[ Protected virtual SNMLookController functions         ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node modifier needs to be updated
*/
void SNMLookController::OnUpdate()
{
	// [HACK][TODO] Currently it's not possible to define/script a control logic within the control connection to, for instance
	// "pass through" a rotation value from a space mouse, but "passing" movements from the mouse only if, for example, the left
	// mouse button is currently pressed (so we don't look around the every time when moving the mouse to, for instance, move
	// the mouse cursor to an ingame GUI widget). Because it's REALLY comfortable to use the space mouse, I added this hack so
	// the space mouse (provides us with absolute values!) can be used as expected during the last steps of the input system refactoring.
	const bool bSpaceMouseRotationHack = ((GetFlags() & UseRotationKey) && (!m_pController->RotX.IsValueRelative() && !m_pController->RotY.IsValueRelative()));

	// Check if input is active and whether or not the rotation key required and pressed
	if (m_pController->GetActive() && (!(GetFlags() & UseRotationKey) || m_pController->Rotate.IsPressed() || bSpaceMouseRotationHack)) {
		// Get rotation
		float fX = m_pController->RotX.GetValue();
		float fY = m_pController->RotY.GetValue();
		float fZ = m_pController->RotZ.GetValue();
		if (fX || fY || fZ) {
			// Get the current time difference
			const float fTimeDiff = Timing::GetInstance()->GetTimeDifference();

			// Do we need to take the current time difference into account?
			if (!m_pController->RotX.IsValueRelative())
				fX *= fTimeDiff;
			if (!m_pController->RotY.IsValueRelative())
				fY *= fTimeDiff;
			if (!m_pController->RotZ.IsValueRelative())
				fZ *= fTimeDiff;

			// Get a quaternion representation of the rotation delta
			Quaternion qRotationDelta;
			EulerAngles::ToQuaternion(static_cast<float>(fX*Math::DegToRad),
									  static_cast<float>(fY*Math::DegToRad),
									  static_cast<float>(fZ*Math::DegToRad),
									  qRotationDelta);

			// Update rotation
			GetSceneNode().GetTransform().SetRotation(GetSceneNode().GetTransform().GetRotation()*qRotationDelta);
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
