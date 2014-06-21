/*********************************************************\
 *  File: SNMOrbitingController.cpp                      *
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
#include "PLEngine/Controller/OrbitingController.h"
#include "PLEngine/Controller/SNMOrbitingController.h"


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
pl_implement_class(SNMOrbitingController)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMOrbitingController::SNMOrbitingController(SceneNode &cSceneNode) : SNMOrbiting(cSceneNode),
	InputSemantic(this),
	EventHandlerUpdate(&SNMOrbitingController::OnUpdate, this),
	m_pController(new OrbitingController())
{
}

/**
*  @brief
*    Destructor
*/
SNMOrbitingController::~SNMOrbitingController()
{
	// Destroy the input controller
	delete m_pController;
}


//[-------------------------------------------------------]
//[ Public virtual PLScene::SceneNodeModifier functions   ]
//[-------------------------------------------------------]
Controller *SNMOrbitingController::GetInputController() const
{
	return m_pController;
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneNodeModifier functions ]
//[-------------------------------------------------------]
void SNMOrbitingController::InformedOnInit()
{
	// Emit the input controller found event of the scene context to tell everyone about our input controller
	InputManager::GetInstance()->EventInputControllerFound(m_pController, InputSemantic);
}

void SNMOrbitingController::OnActivate(bool bActivate)
{
	// Call base implementation
	SNMOrbiting::OnActivate(bActivate);

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
//[ Protected virtual SNMOrbitingController functions     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node modifier needs to be updated
*/
void SNMOrbitingController::OnUpdate()
{
	// Check if input is active
	if (m_pController->GetActive()) {
		// [HACK][TODO](same as in " SNMLookController::OnUpdate()") Currently it's not possible to define/script a control logic within the control connection to, for instance
		// "pass through" a rotation value from a space mouse, but "passing" movements from the mouse only if, for example, the left
		// mouse button is currently pressed (so we don't look around the every time when moving the mouse to, for instance, move
		// the mouse cursor to an ingame GUI widget). Because it's REALLY comfortable to use the space mouse, I added this hack so
		// the space mouse (provides us with absolute values!) can be used as expected during the last steps of the input system refactoring.
		const bool bSpaceMouseRotationHack = (!m_pController->RotX.IsValueRelative() && !m_pController->RotY.IsValueRelative());
		const bool bSpaceMouseZoomHack     = (!m_pController->ZoomAxis.IsValueRelative());

		// Get the current speed
		float fSpeed = 1.0f;
		if (m_pController->Run.IsPressed())
			fSpeed *= 4.0f;
		else if (m_pController->Crouch.IsPressed())
			fSpeed /= 4.0f;
		const float fTimedSpeed = fSpeed*Timing::GetInstance()->GetTimeDifference();

		// Rotation
		if (m_pController->Rotate.IsPressed() || bSpaceMouseRotationHack) {
			float fX = m_pController->RotX.GetValue();
			float fY = m_pController->RotY.GetValue();
			float fZ = m_pController->RotZ.GetValue();
			if (fX || fY || fZ) {
				// Do we need to take the current time difference into account?
				fX *= m_pController->RotX.IsValueRelative() ? fSpeed : fTimedSpeed;
				fY *= m_pController->RotY.IsValueRelative() ? fSpeed : fTimedSpeed;
				fZ *= m_pController->RotZ.IsValueRelative() ? fSpeed : fTimedSpeed;

				// Get a quaternion representation of the rotation delta
				Quaternion qRotInc;
				EulerAngles::ToQuaternion(static_cast<float>(fX*Math::DegToRad),
										  static_cast<float>(fY*Math::DegToRad),
										  static_cast<float>(fZ*Math::DegToRad),
										  qRotInc);

				// Update rotation
				GetSceneNode().GetTransform().SetRotation(GetSceneNode().GetTransform().GetRotation()*qRotInc);
			}
		}

		// Pan
		if (m_pController->Pan.IsPressed()) {
			float fX = m_pController->PanX.GetValue();
			float fY = m_pController->PanY.GetValue();
			float fZ = m_pController->PanZ.GetValue();
			if (fX || fY || fZ) {
				// Do we need to take the current time difference into account?
				fX *= m_pController->PanX.IsValueRelative() ? fSpeed : fTimedSpeed;
				fY *= m_pController->PanY.IsValueRelative() ? fSpeed : fTimedSpeed;
				fZ *= m_pController->PanZ.IsValueRelative() ? fSpeed : fTimedSpeed;

				// Set pan
				Vector3 vPan = Pan.Get();
				vPan.x += fX;
				vPan.y += fY;
				vPan.z += fZ;
				Pan.Set(vPan);
			}
		} else {
			// [HACK][TODO](See above)
			if (!m_pController->PanX.IsValueRelative() && !m_pController->PanY.IsValueRelative()) {
				float fX = m_pController->PanX.GetValue();
				float fY = m_pController->PanY.GetValue();
				if (fX || fY) {
					// Do we need to take the current time difference into account?
					fX *= m_pController->PanX.IsValueRelative() ? fSpeed : fTimedSpeed;
					fY *= m_pController->PanY.IsValueRelative() ? fSpeed : fTimedSpeed;

					// Set pan
					Vector3 vPan = Pan.Get();
					vPan.x += fX;
					vPan.y += fY;
					Pan.Set(vPan);
				}
			}
		}

		// Zoom
		if (m_pController->Zoom.IsPressed() || bSpaceMouseZoomHack) {
			float fZoomAxis = m_pController->ZoomAxis.GetValue();
			if (fZoomAxis) {
				// Do we need to take the current time difference into account?
				fZoomAxis *= m_pController->ZoomAxis.IsValueRelative() ? fSpeed : fTimedSpeed;

				// Set new distance
				SetDistance(GetDistance() - fZoomAxis);
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
