/*********************************************************\
 *  File: BodyEllipsoid.cpp                                 *
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
#include "PLPhysicsBullet/BodyEllipsoid.h"
#include "PLPhysicsBullet/BodyImpl.h"
#include "PLPhysicsBullet/World.h"
#include <PLPhysicsBullet/Helper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLMath;
namespace PLPhysicsBullet {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
BodyEllipsoid::~BodyEllipsoid()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
BodyEllipsoid::BodyEllipsoid(PLPhysics::World &cWorld, const PLMath::Vector3 &vRadius, bool bStatic) :
	PLPhysics::BodyEllipsoid(cWorld, static_cast<World&>(cWorld).CreateBodyImpl(), vRadius)
{
	// Deactivate the physics simulation if required
	const bool bSimulationActive = cWorld.IsSimulationActive();
	if (bSimulationActive)
		cWorld.SetSimulationActive(false);

	// Get the Bullet physics world
	btDynamicsWorld *pBulletWorld = ((World&)cWorld).GetBulletWorld();
	if (pBulletWorld) {
		float rad = 2;
		btVector3 vPos;
		
		btMultiSphereShape* collisionShape = new btMultiSphereShape(&vPos, &rad, 1);
		collisionShape->setLocalScaling(Helper::PLVector3TobtVector3(vRadius));
		
		// Initialize the Bullet physics body
		((BodyImpl&)GetBodyImpl()).InitializeBulletBody(*this, *collisionShape, bStatic);
	}

	// Reactivate the physics simulation if required
	if (bSimulationActive)
		cWorld.SetSimulationActive(bSimulationActive);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsBullet
