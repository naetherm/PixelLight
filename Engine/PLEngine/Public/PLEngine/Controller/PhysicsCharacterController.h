/*********************************************************\
 *  File: PhysicsCharacterController.h                   *
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


#ifndef __PLENGINE_CONTROLLER_PHYSICSCHARACTERCONTROLLER_H__
#define __PLENGINE_CONTROLLER_PHYSICSCHARACTERCONTROLLER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLInput/Input/Controller.h>
#include <PLInput/Input/Controls/Axis.h>
#include <PLInput/Input/Controls/Button.h>
#include "PLEngine/PLEngine.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLEngine {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Physics character input controller
*
*  @note
*    - Primary intended for rapid prototyping
*/
class PhysicsCharacterController : public PLInput::Controller {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PL_RTTI_EXPORT, PhysicsCharacterController, "PLEngine", PLInput::Controller, "Physics character input controller")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Controller definition                                 ]
	//[-------------------------------------------------------]
	public:
		PLInput::Axis		TransX;			/**< X translation axis: Strafe left/right (+/-) */
		PLInput::Axis		TransY;			/**< Y translation axis: Move up/down (+/-) */
		PLInput::Axis		TransZ;			/**< Z translation axis: Move forwards/backwards (+/-) */
		PLInput::Axis		RotY;			/**< Y rotation axis: Yaw (also called 'heading') change is turning to the left or right */
		PLInput::Button		Forward;		/**< Move forwards */
		PLInput::Button		Backward;		/**< Move backwards */
		PLInput::Button		Left;			/**< Move (rotate) left */
		PLInput::Button		Right;			/**< Move (rotate) right */
		PLInput::Button		StrafeLeft;		/**< Strafe left */
		PLInput::Button		StrafeRight;	/**< Strafe right */
		PLInput::Button		Up;				/**< Move up */
		PLInput::Button		Down;			/**< Move down */
		PLInput::Button		Run;			/**< Keep pressed to run */
		PLInput::Button		Crouch;			/**< Keep pressed to crouch */
		PLInput::Button		Jump;			/**< Jump */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PL_API PhysicsCharacterController();

		/**
		*  @brief
		*    Destructor
		*/
		PL_API virtual ~PhysicsCharacterController();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_CONTROLLER_PHYSICSCHARACTERCONTROLLER_H__
