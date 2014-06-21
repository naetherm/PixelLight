/*********************************************************\
 *  File: SNMEgoLookController.cpp                       *
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
#include "PLEngine/Controller/LookController.h"
#include "PLEngine/Controller/SNMEgoLookController.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLScene;
namespace PLEngine {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMEgoLookController)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMEgoLookController::SNMEgoLookController(SceneNode &cSceneNode) : SNMLookController(cSceneNode),
	UpVector(this),
	RollFactor(this),
	SlotOnRotation(this),
	m_fPitch(0.0f),
	m_fYaw(0.0f),
	m_bListen(true)
{
}

/**
*  @brief
*    Destructor
*/
SNMEgoLookController::~SNMEgoLookController()
{
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneNodeModifier functions ]
//[-------------------------------------------------------]
void SNMEgoLookController::OnActivate(bool bActivate)
{
	// Call the base implementation
	SNMLookController::OnActivate(bActivate);

	// Connect/disconnect event handler
	SceneNode &cSceneNode = GetSceneNode();
	if (bActivate) {
		// Connect event handler
		cSceneNode.GetTransform().EventRotation.Connect(SlotOnRotation);

		// Ensure our Euler angles are representing the current scene node rotation (synchronization, no logic update)
		OnRotation();
	} else {
		// Disconnect event handler
		cSceneNode.GetTransform().EventRotation.Disconnect(SlotOnRotation);
	}
}


//[-------------------------------------------------------]
//[ Protected virtual SNMLookController functions         ]
//[-------------------------------------------------------]
void SNMEgoLookController::OnUpdate()
{
	// [HACK][TODO](same as in " SNMLookController::OnUpdate()") Currently it's not possible to define/script a control logic within the control connection to, for instance
	// "pass through" a rotation value from a space mouse, but "passing" movements from the mouse only if, for example, the left
	// mouse button is currently pressed (so we don't look around the every time when moving the mouse to, for instance, move
	// the mouse cursor to an ingame GUI widget). Because it's REALLY comfortable to use the space mouse, I added this hack so
	// the space mouse (provides us with absolute values!) can be used as expected during the last steps of the input system refactoring.
	const bool bSpaceMouseRotationHack = ((GetFlags() & UseRotationKey) && (!m_pController->RotX.IsValueRelative() && !m_pController->RotY.IsValueRelative()));

	// Check if input is active and whether or not the rotation key required and pressed
	if (m_pController->GetActive() && (!(GetFlags() & UseRotationKey) || m_pController->Rotate.IsPressed() || bSpaceMouseRotationHack)) {
		// Get rotation, ignore z-axis (roll)
		float fX = m_pController->RotX.GetValue();
		float fY = m_pController->RotY.GetValue();
		float fZ = m_pController->RotZ.IsValueRelative() ? 0.0f : m_pController->RotZ.GetValue()*RollFactor;
		if (fX || fY || fZ) {
			// Get the current time difference
			const float fTimeDiff = Timing::GetInstance()->GetTimeDifference();

			// Do we need to take the current time difference into account?
			if (!m_pController->RotX.IsValueRelative())
				fX *= fTimeDiff;
			if (!m_pController->RotY.IsValueRelative())
				fY *= fTimeDiff;

			// X rotation axis: Update pitch (also called 'bank', change is moving the nose down and the tail up or vice-versa) - in degree
			m_fPitch += fX;

			// Y rotation axis: Update yaw (also called 'heading', change is turning to the left or right) - in degree
			m_fYaw += fY;

			// Limit the pitch (no full 90° to avoid dead angles)
			if (m_fPitch > 89.9f)
				m_fPitch = 89.9f;
			if (m_fPitch < -89.9f)
				m_fPitch = -89.9f;

			// Limit the yaw (to huge values may cause problems, so, bring them into a well known interval)
			m_fYaw = Math::WrapToInterval(m_fYaw, 0.0f, 360.0f);

			// Get a quaternion representation of your pitch
			Quaternion qRotation, qRotationDelta;
			const Vector3 &vUpVector = UpVector.Get();
			qRotation.FromAxisAngle(vUpVector.x, vUpVector.y, vUpVector.z, static_cast<float>(m_fYaw*Math::DegToRad));

			// If there's an absolute z-axis (e.g. when using a 3D mouse), apply roll -> looks & feels more natural
			if (fZ) {
				EulerAngles::ToQuaternion(0.0f, 0.0f, static_cast<float>(fZ*Math::DegToRad), qRotationDelta);
				qRotation *= qRotationDelta;
			}

			// Get a quaternion representation of your yaw
			EulerAngles::ToQuaternion(static_cast<float>(m_fPitch*Math::DegToRad), 0.0f, 0.0f, qRotationDelta);
			qRotation *= qRotationDelta;

			// Set the new rotation quaternion and don't listen to signals while doing so
			m_bListen = false;
			GetSceneNode().GetTransform().SetRotation(qRotation);
			m_bListen = true;
		}
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node rotation changed
*/
void SNMEgoLookController::OnRotation()
{
	// Listen to the signal?
	if (m_bListen) {
		// Get the transform of the owner scene node
		const Transform3 &cTransform = GetSceneNode().GetTransform();

		// X rotation axis: Calculate pitch (also called 'bank', change is moving the nose down and the tail up or vice-versa) - in degree
		// Calculate the dot product (= cosine) between z-direction- and up-vector
		m_fPitch = cTransform.GetRotation().GetZAxis().DotProduct(UpVector.Get());
		// Get the angle in radian
		m_fPitch = Math::ACos(m_fPitch);
		// Get the angle in degree
		m_fPitch = static_cast<float>(m_fPitch*Math::RadToDeg) - 90.0f;

		// Y rotation axis: Calculate yaw (also called 'heading', change is turning to the left or right) - in degree
		Vector3 vRight, vUp;
		UpVector.Get().GetRightUp(vRight, vUp);
		// Calculate the dot product (= cosine) between x-direction-vector and the right-vector of the up-vector (already confused? :)
		m_fYaw = cTransform.GetRotation().GetXAxis().DotProduct(vRight);
		// Get the angle in radian
		m_fYaw = Math::ACos(m_fYaw);
		// Get the angle in degree
		m_fYaw = static_cast<float>(m_fYaw*Math::RadToDeg);
		// Flip sign if required
		if (cTransform.GetRotation().GetXAxis().DotProduct(vUp) < 0.0f)
			m_fYaw = -m_fYaw;

		// Limit the yaw (to huge values may cause problems, so, bring them into a well known interval)
		m_fYaw = Math::WrapToInterval(m_fYaw, 0.0f, 360.0f);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine

