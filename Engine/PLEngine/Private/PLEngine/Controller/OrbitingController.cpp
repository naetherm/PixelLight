/*********************************************************\
 *  File: OrbitingController.cpp                         *
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
#include "PLEngine/Controller/OrbitingController.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLInput;
namespace PLEngine {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(OrbitingController)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
OrbitingController::OrbitingController() : Controller(ControllerVirtual, "OrbitingController", "Orbiting input controller"),
	RotX	(this, "RotX",		"X rotation axis: Pitch (also called 'bank') change is moving the nose down and the tail up (or vice-versa)"),
	RotY	(this, "RotY",		"Y rotation axis: Yaw (also called 'heading') change is turning to the left or right"),
	RotZ	(this, "RotZ",		"Z rotation axis: Roll (also called 'attitude') change is moving one wingtip up and the other down"),
	PanX	(this, "PanX",		"X pan translation axis: Strafe left/right (+/-)"),
	PanY	(this, "PanY",		"Y pan translation axis: Move up/down (+/-)"),
	PanZ	(this, "PanZ",		"Z pan translation axis: Move forwards/backwards (+/-)"),
	Rotate	(this, "Rotate",	"Keep pressed to rotate"),
	Pan		(this, "Pan",		"Keep pressed to pan"),
	Zoom	(this, "Zoom",		"Keep pressed to zoom"),
	ZoomAxis(this, "ZoomAxis",	"Zoom axis to zoom in or out (+/-)"),
	Run		(this, "Run",		"Keep pressed to run"),
	Crouch	(this, "Crouch",	"Keep pressed to crouch")
{
}

/**
*  @brief
*    Destructor
*/
OrbitingController::~OrbitingController()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
