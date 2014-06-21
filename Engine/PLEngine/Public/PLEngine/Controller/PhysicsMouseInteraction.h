/*********************************************************\
 *  File: PhysicsMouseInteraction.h                      *
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


#ifndef __PLENGINE_CONTROLLER_PHYSICSMOUSEINTERACTION_H__
#define __PLENGINE_CONTROLLER_PHYSICSMOUSEINTERACTION_H__
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
*    Mouse physics interaction input controller
*
*  @note
*    - Primary intended for rapid prototyping
*/
class PhysicsMouseInteraction : public PLInput::Controller {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PL_RTTI_EXPORT, PhysicsMouseInteraction, "PLEngine", PLInput::Controller, "Mouse physics interaction input controller")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Controller definition                                 ]
	//[-------------------------------------------------------]
	public:
		PLInput::Button	Pickup;			/**< Keep pressed to pickup */
		PLInput::Button	Throw;			/**< Throw the picked object */
		PLInput::Button	IncreaseForce;	/**< Keep pressed to increase the force applied to the picked object */
		PLInput::Button	DecreaseForce;	/**< Keep pressed to decrease the force applied to the picked object */
		PLInput::Axis	PushPull;		/**< Used to push/pull the picked object */
		PLInput::Axis	MouseX;			/**< X-position of the mouse */
		PLInput::Axis	MouseY;			/**< Y-position of the mouse */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PL_API PhysicsMouseInteraction();

		/**
		*  @brief
		*    Destructor
		*/
		PL_API virtual ~PhysicsMouseInteraction();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_CONTROLLER_PHYSICSMOUSEINTERACTION_H__
