/*********************************************************\
 *  File: SNMTransformGizmoScaleController.cpp           *
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
#include <PLCore/Frontend/Frontend.h>
#include <PLCore/Frontend/FrontendApplication.h>
#include <PLMath/Ray.h>
#include <PLMath/Intersect.h>
#include <PLInput/Input/InputManager.h>
#include <PLInput/Input/Devices/Mouse.h>
#include <PLInput/Input/Devices/Keyboard.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLScene/Visibility/VisNode.h>
#include "PLEngine/Controller/SNMTransformGizmoScaleController.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLInput;
using namespace PLRenderer;
using namespace PLScene;
namespace PLEngine {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SNMTransformGizmoScaleController, "PLEngine", PLScene::SNMTransformGizmoScale, "Scale transform gizmo controller scene node modifier")
	// Constructors
	pl_constructor_1_metadata(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
pl_class_metadata_end(SNMTransformGizmoScaleController)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMTransformGizmoScaleController::SNMTransformGizmoScaleController(SceneNode &cSceneNode) : SNMTransformGizmoScale(cSceneNode)
{
}

/**
*  @brief
*    Destructor
*/
SNMTransformGizmoScaleController::~SNMTransformGizmoScaleController()
{
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SNMTransformGizmo functions  ]
//[-------------------------------------------------------]
void SNMTransformGizmoScaleController::UpdateSelection(Renderer &cRenderer, const VisNode &cVisNode)
{
	// Get the frontend instance
	Frontend &cFrontend = static_cast<FrontendApplication*>(CoreApplication::GetApplication())->GetFrontend();

	// Check whether or not the mouse is currently over the frontend
	if (cFrontend.IsMouseOver()) {
		// Get current mouse cursor position inside the widget
		const int nMousePosX = cFrontend.GetMousePositionX();
		const int nMousePosY = cFrontend.GetMousePositionY();

		// Calculate the clip space to object space matrix
		Matrix4x4 mClipSpaceToObjectSpace = m_mObjectSpaceToClipSpace;
		mClipSpaceToObjectSpace.Invert();

		// Check where the mouse is over
		// Get the ray starting from the camera position in direction of the mouse position
		Vector3 v2DPos(static_cast<float>(nMousePosX), static_cast<float>(nMousePosY), 0.0001f);
		Vector3 vCamPos = v2DPos.To3DCoordinate(mClipSpaceToObjectSpace, cRenderer.GetViewport());
		v2DPos.z = 0.9999f;
		Vector3 vEndPos = v2DPos.To3DCoordinate(mClipSpaceToObjectSpace, cRenderer.GetViewport());

		// Determine the current selected axis by using a picking ray
		Ray cRay;
		cRay.Set(vCamPos, vEndPos);
		m_nSelected = DetermineSelected(cRay);
	} else {
		m_nSelected = 0;
	}
}

void SNMTransformGizmoScaleController::PerformTransform()
{
	// Get the mouse device, there's no point in abstracting this in here because the transform gizmos are used through a mouse
	Mouse *pMouse = InputManager::GetInstance()->GetMouse();
	if (pMouse) {
		// General transform speed
		static const float TransformSpeed = 1.0f;

		// Transform speed factor
		float fTransformSpeedFactor = 1.0f;
		Keyboard *pKeyboard = InputManager::GetInstance()->GetKeyboard();
		if (pKeyboard) {
			// Speed up?
			if (pKeyboard->Shift.IsPressed())
				fTransformSpeedFactor = 10.0f;

			// Slow down?
			else if (pKeyboard->Control.IsPressed())
				fTransformSpeedFactor = 0.1f;
		}

		// Get the current time difference
		const float fTimeDifference = Timing::GetInstance()->GetTimeDifference()*TransformSpeed*fTransformSpeedFactor;

		// Get timed transform speed along each mouse axis, this speed is "eaten up" as soon as it's used
		float fTransformSpeedX = fTimeDifference*pMouse->X.GetValue();
		float fTransformSpeedY = fTimeDifference*pMouse->Y.GetValue();

		// Get the current scale of the owner scene node
		Vector3 vScale = GetSceneNode().GetScale();

		// Update scale
		if (m_nSelected & XAxis) {
			vScale.x -= fTransformSpeedX;
			fTransformSpeedX = 0.0f;
		}
		if (m_nSelected & YAxis) {
			vScale.y -= fTransformSpeedY;
			fTransformSpeedY = 0.0f;
		}
		if (m_nSelected & ZAxis)
			vScale.z -= fTransformSpeedX;	// Do always use x, everything else is difficult to use

		// Set the new scale of the owner scene node
		GetSceneNode().SetScale(vScale);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
