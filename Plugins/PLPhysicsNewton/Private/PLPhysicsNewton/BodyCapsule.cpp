/*********************************************************\
 *  File: BodyCapsule.cpp                                *
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
#include "PLPhysicsNewton/BodyImpl.h"
#include "PLPhysicsNewton/World.h"
#include "PLPhysicsNewton/BodyCapsule.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLMath;
namespace PLPhysicsNewton {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
BodyCapsule::~BodyCapsule()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
BodyCapsule::BodyCapsule(PLPhysics::World &cWorld, float fRadius, float fHeight) :
	PLPhysics::BodyCapsule(cWorld, static_cast<World&>(cWorld).CreateBodyImpl(), fRadius, fHeight)
{
	// Deactivate the physics simulation if required
	const bool bSimulationActive = cWorld.IsSimulationActive();
	if (bSimulationActive)
		cWorld.SetSimulationActive(false);

	// Get the Newton physics world
	Newton::NewtonWorld *pNewtonWorld = static_cast<World&>(cWorld).GetNewtonWorld();

	// Create collision primitive
	Newton::NewtonCollision *pCollision = NewtonCreateCapsule(pNewtonWorld, m_fRadius, m_fHeight, 0, nullptr);

	// Create the rigid body
	// [TODO] Remove this as soon as there's an up-to-date Linux version of Newton Game Dynamics available!
	#if (NEWTON_MAJOR_VERSION == 2) && (NEWTON_MINOR_VERSION >= 28)
		Newton::NewtonBody *pNewtonBody = NewtonCreateBody(pNewtonWorld, pCollision, Matrix4x4::Identity);
	#else
		Newton::NewtonBody *pNewtonBody = NewtonCreateBody(pNewtonWorld, pCollision);
	#endif
	NewtonReleaseCollision(pNewtonWorld, pCollision);

	// Calculate the collision volume
	// [TODO] Find the correct formula...
	const float fCollisionVolume = static_cast<float>(Math::Pi*m_fRadius*m_fRadius*m_fHeight);

	// Initialize the Newton physics body
	static_cast<BodyImpl&>(GetBodyImpl()).InitializeNewtonBody(*this, *pNewtonBody, fCollisionVolume);

	// Reactivate the physics simulation if required
	if (bSimulationActive)
		cWorld.SetSimulationActive(bSimulationActive);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsNewton
