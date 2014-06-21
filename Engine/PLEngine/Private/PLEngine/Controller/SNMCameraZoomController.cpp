/*********************************************************\
 *  File: SNMCameraZoomController.cpp                    *
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
#include <PLInput/Input/InputManager.h>
#include <PLScene/Scene/SceneContext.h>
#include "PLEngine/Controller/CameraZoomController.h"
#include "PLEngine/Controller/SNMCameraZoomController.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLInput;
using namespace PLScene;
namespace PLEngine {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMCameraZoomController)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMCameraZoomController::SNMCameraZoomController(SceneNode &cSceneNode) : SNMCameraZoom(cSceneNode),
	InputSemantic(this),
	SlotOnControl(this),
	m_pController(new CameraZoomController())
{
	// Connect input control event handler
	m_pController->SignalOnControl.Connect(SlotOnControl);
}

/**
*  @brief
*    Destructor
*/
SNMCameraZoomController::~SNMCameraZoomController()
{
	// Destroy the input controller
	delete m_pController;
}


//[-------------------------------------------------------]
//[ Public virtual PLScene::SceneNodeModifier functions   ]
//[-------------------------------------------------------]
Controller *SNMCameraZoomController::GetInputController() const
{
	return m_pController;
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneNodeModifier functions ]
//[-------------------------------------------------------]
void SNMCameraZoomController::InformedOnInit()
{
	// Emit the input controller found event of the scene context to tell everyone about our input controller
	InputManager::GetInstance()->EventInputControllerFound(m_pController, InputSemantic);
}

void SNMCameraZoomController::OnActivate(bool bActivate)
{
	// Call base implementation
	SNMCameraZoom::OnActivate(bActivate);

	// Activate/deactivate the input controller
	m_pController->SetActive(bActivate);
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a control event has occurred
*/
void SNMCameraZoomController::OnControl(Control &cControl)
{
	if (&cControl == &m_pController->Zoom)
		ZoomFactor = static_cast<Button&>(cControl).IsPressed() ? 1.0f : 0.0f;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
