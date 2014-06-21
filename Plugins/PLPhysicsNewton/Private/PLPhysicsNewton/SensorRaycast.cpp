/*********************************************************\
 *  File: SensorRaycast.cpp                              *
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
#include "PLPhysicsNewton/SensorRaycast.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLPhysicsNewton {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
SensorRaycast::~SensorRaycast()
{
}


//[-------------------------------------------------------]
//[ Public Newton callback functions                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Newton function to be called for each body hit during the ray scan
*/
float SensorRaycast::RayFilterCallback(const Newton::NewtonBody *pPhysicsBody, const float *pfNormal,
									   int nCollisionID, void *pUserData, float fIntersectParam)
{
	// Get the pointer to the ray cast sensor
	SensorRaycast *pSensor = static_cast<SensorRaycast*>(pUserData);
	if (pSensor) {
		// Get the pointer to the PL physics body
		PLPhysics::Body *pBody = static_cast<PLPhysics::Body*>(NewtonBodyGetUserData(pPhysicsBody));
		if (pBody && pBody->IsActive()) {
			// Call the sensor callback function
			if (pSensor->Callback(*pBody, fIntersectParam, pfNormal, nCollisionID)) {
				// Return 'fIntersectParam' if 'ClosestBody' performance hint flag is set
				return (pSensor->GetFlags() & ClosestBody) ? fIntersectParam : 1.0f; // Continue
			} else {
				// Stop right now!
				return 0.0f;
			}
		}
	}

	// Continue
	return 1.0f;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SensorRaycast::SensorRaycast(PLPhysics::World &cWorld, const Vector3 &vStart, const Vector3 &vEnd, uint32 nFlags) :
	PLPhysics::SensorRaycast(cWorld, vStart, vEnd, nFlags)
{
}


//[-------------------------------------------------------]
//[ Public virtual PLPhysics::Sensor functions            ]
//[-------------------------------------------------------]
uint32 SensorRaycast::Check()
{
	// Call base implementation
	Sensor::Check();

	// Get the Newton physics world
	Newton::NewtonWorld *pNewtonWorld = static_cast<World&>(GetWorld()).GetNewtonWorld();

	// Perform Newton physics ray cast
	NewtonWorldRayCast(pNewtonWorld, m_vStart, m_vEnd, RayFilterCallback, this, nullptr);

	// Done
	return GetNumOfHitBodies();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsNewton
