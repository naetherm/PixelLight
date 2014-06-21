/*********************************************************\
 *  File: SensorAABoundingBox.cpp                        *
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
#include <PLPhysics/BodyBox.h>
#include "PLPhysicsNewton/World.h"
#include "PLPhysicsNewton/SensorAABoundingBox.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLPhysicsNewton {


//[-------------------------------------------------------]
//[ Private static Newton callback variable               ]
//[-------------------------------------------------------]
SensorAABoundingBox *SensorAABoundingBox::m_pCurrentSensor = nullptr;


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
SensorAABoundingBox::~SensorAABoundingBox()
{
}


//[-------------------------------------------------------]
//[ Public Newton callback functions                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Newton function to be called for each body hit during the axis aligned bounding box check
*/
void SensorAABoundingBox::AABoundingBoxIteratorCallback(const Newton::NewtonBody *pPhysicsBody, void *pUserData)
{
	// Get the pointer to the axis aligned bounding box sensor
	if (m_pCurrentSensor) {
		// Get the pointer to the PL physics body
		PLPhysics::Body *pBody = static_cast<PLPhysics::Body*>(NewtonBodyGetUserData(pPhysicsBody));
		if (pBody && pBody->IsActive()) {
			// Call the sensor callback function
			m_pCurrentSensor->Callback(*pBody, 0.0f, Vector3::Zero, 0);
		}
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SensorAABoundingBox::SensorAABoundingBox(PLPhysics::World &cWorld, const Vector3 &vMin, const Vector3 &vMax, uint32 nFlags) :
	PLPhysics::SensorAABoundingBox(cWorld, vMin, vMax, nFlags)
{
}


//[-------------------------------------------------------]
//[ Public virtual PLPhysics::Sensor functions            ]
//[-------------------------------------------------------]
uint32 SensorAABoundingBox::Check()
{
	// Call base implementation
	Sensor::Check();

	// Get the Newton physics world
	Newton::NewtonWorld *pNewtonWorld = static_cast<World&>(GetWorld()).GetNewtonWorld();

	// Perform Newton physics axis aligned bounding box check
	m_pCurrentSensor = this;
	NewtonWorldForEachBodyInAABBDo(pNewtonWorld, m_vMin, m_vMax, AABoundingBoxIteratorCallback, nullptr);

	// Done
	return GetNumOfHitBodies();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsNewton
