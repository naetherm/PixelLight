/*********************************************************\
 *  File: OrbitingController.h                           *
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


#ifndef __PLENGINE_CONTROLLER_ORBITINGCONTROLLER_H__
#define __PLENGINE_CONTROLLER_ORBITINGCONTROLLER_H__
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
*    Orbiting input controller
*
*  @note
*    - Primary intended for rapid prototyping
*/
class OrbitingController : public PLInput::Controller {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PL_RTTI_EXPORT, OrbitingController, "PLEngine", PLInput::Controller, "Orbiting input controller")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Controller definition                                 ]
	//[-------------------------------------------------------]
	public:
		PLInput::Axis	RotX;		/**< X rotation axis: Pitch (also called 'bank') change is moving the nose down and the tail up (or vice-versa) */
		PLInput::Axis	RotY;		/**< Y rotation axis: Yaw (also called 'heading') change is turning to the left or right */
		PLInput::Axis	RotZ;		/**< Z rotation axis: Roll (also called 'attitude') change is moving one wingtip up and the other down */
		PLInput::Axis	PanX;		/**< X pan translation axis: Strafe left/right (+/-) */
		PLInput::Axis	PanY;		/**< Y pan translation axis: Move up/down (+/-) */
		PLInput::Axis	PanZ;		/**< Z pan translation axis: Move forwards/backwards (+/-) */
		PLInput::Button	Rotate;		/**< Keep pressed to rotate */
		PLInput::Button	Pan;		/**< Keep pressed to pan */
		PLInput::Button	Zoom;		/**< Keep pressed to zoom */
		PLInput::Axis	ZoomAxis;	/**< Zoom axis to zoom in or out (+/-) */
		PLInput::Button	Run;		/**< Keep pressed to run */
		PLInput::Button	Crouch;		/**< Keep pressed to crouch */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PL_API OrbitingController();

		/**
		*  @brief
		*    Destructor
		*/
		PL_API virtual ~OrbitingController();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_CONTROLLER_ORBITINGCONTROLLER_H__
