/*********************************************************\
 *  File: PhysicsMouseInteraction.cpp                    *
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
#include "PLEngine/Controller/PhysicsMouseInteraction.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLInput;
namespace PLEngine {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(PhysicsMouseInteraction)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
PhysicsMouseInteraction::PhysicsMouseInteraction() : Controller(ControllerVirtual, "PhysicsMouseInteraction", "Mouse physics interaction input controller"),
	Pickup			(this, "Pickup",		"Keep pressed to pickup",											0x00),
	Throw			(this, "Throw",			"Throw the picked object",											0x00),
	IncreaseForce	(this, "IncreaseForce",	"Keep pressed to increase the force applied to the picked object",	0x00),
	DecreaseForce	(this, "DecreaseForce",	"Keep pressed to decrease the force applied to the picked object",	0x00),
	PushPull		(this, "PushPull",		"Used to push/pull the picked object"),
	MouseX			(this, "MouseX",		"X-position of the mouse"),
	MouseY			(this, "MouseY",		"Y-position of the mouse")
{
}

/**
*  @brief
*    Destructor
*/
PhysicsMouseInteraction::~PhysicsMouseInteraction()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
