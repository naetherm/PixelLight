/*********************************************************\
 *  File: BodyTerrain.cpp                                *
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
#include <PLMath/Matrix4x4.h>
#include "PLPhysicsBullet/BodyImpl.h"
#include "PLPhysicsBullet/World.h"
#include "PLPhysicsBullet/BodyTerrain.h"
#include <bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>


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
BodyTerrain::~BodyTerrain()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
BodyTerrain::BodyTerrain(PLPhysics::World &cWorld,  PLCore::uint32 nWidth, PLCore::uint32 nHeight,
					const float fTerrain[], const PLMath::Vector3 &vBoxMin, const PLMath::Vector3 &vBoxMax,
					const PLMath::Vector3 &vScale) :
	PLPhysics::BodyTerrain(cWorld, static_cast<World&>(cWorld).CreateBodyImpl()),
	m_nWidth(nWidth),
	m_nHeight(nHeight),
	m_pfTerrain(fTerrain),
	m_vBoxMin(vBoxMin),
	m_vBoxMax(vBoxMax),
	m_vScale(vScale)
{
	// Deactivate the physics simulation if required
	const bool bSimulationActive = cWorld.IsSimulationActive();
	if (bSimulationActive)
		cWorld.SetSimulationActive(false);

	// Get the Bullet physics world
	btDynamicsWorld *pBulletWorld = ((World&)cWorld).GetBulletWorld();
	if (pBulletWorld) {
		// [TODO] use correct values for min/max height (can they calculated from vBoxMin and vBoxMax?)
		//        and is the up axis always the y-axis?
		btCollisionShape* collisionShape = new btHeightfieldTerrainShape(m_nWidth, m_nHeight, const_cast<float*>(m_pfTerrain),
																		 0 /* scaling factor, ignored when using float values */,
																		 0/* <min height, float>*/,
																		 0/* <max height, float>*/,
																		 1 /* y-axis is up */,
																		 PHY_FLOAT /* Highfield values are float*/,
																		 false /*flip quatEdges*/
																		);
		
		// Initialize the Bullet physics body
		((BodyImpl&)GetBodyImpl()).InitializeBulletBody(*this, *collisionShape, true);
	}

	// Reactivate the physics simulation if required
	if (bSimulationActive)
		cWorld.SetSimulationActive(bSimulationActive);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsBullet
