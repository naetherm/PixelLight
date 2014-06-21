/*********************************************************\
 *  File: SNMCameraZoomController.h                      *
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


#ifndef __PLENGINE_CONTROLLER_SNMCAMERAZOOMCONTROLLER_H__
#define __PLENGINE_CONTROLLER_SNMCAMERAZOOMCONTROLLER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLScene/Scene/SceneNodeModifiers/SNMCameraZoom.h>
#include "PLEngine/PLEngine.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLInput {
	class Control;
}
namespace PLEngine {
	class CameraZoomController;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLEngine {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Scene node modifier class implementing camera zoom input control
*
*  @remarks
*    When using the unchanged virtual standard controller:
*    Hold the right mouse button to zoom.
*
*  @note
*    - Primary intended for rapid prototyping
*/
class SNMCameraZoomController : public PLScene::SNMCameraZoom {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PL_RTTI_EXPORT, SNMCameraZoomController, "PLEngine", PLScene::SNMCameraZoom, "Scene node modifier class implementing camera zoom input control")
		// Attributes
		pl_attribute(InputSemantic,	PLCore::String,	"",	ReadWrite,	DirectValue,	"Semantic of this input controller (e.g. \"Camera\")",	"")
		// Constructors
		pl_constructor_1(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
		// Slot
		pl_slot_1(OnControl,	PLInput::Control&,	"Called when a control event has occurred, the control causing the event as first parameter",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cSceneNode
		*    Owner scene node
		*/
		PL_API SNMCameraZoomController(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PL_API virtual ~SNMCameraZoomController();


	//[-------------------------------------------------------]
	//[ Public virtual PLScene::SceneNodeModifier functions   ]
	//[-------------------------------------------------------]
	public:
		PL_API virtual PLInput::Controller *GetInputController() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual PLScene::SceneNodeModifier functions ]
	//[-------------------------------------------------------]
	protected:
		PL_API virtual void InformedOnInit() override;
		PL_API virtual void OnActivate(bool bActivate) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when a control event has occurred
		*
		*  @param[in] cControl
		*    The control causing the event
		*/
		void OnControl(PLInput::Control &cControl);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		CameraZoomController *m_pController;	/**< Camera zoom input controller instance, always valid! */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_CONTROLLER_SNMCAMERAZOOMCONTROLLER_H__
