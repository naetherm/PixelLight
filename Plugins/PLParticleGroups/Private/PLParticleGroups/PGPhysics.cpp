/*********************************************************\
 *  File: PGPhysics.cpp                                  *
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
#include <PLPhysics/World.h>
#include <PLPhysics/SceneNodes/SCPhysicsWorld.h>
#include "PLParticleGroups/PGPhysics.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLMath;
using namespace PLScene;
using namespace PLPhysics;
namespace PLParticleGroups {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(PGPhysics)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the gravity vector
*/
Vector3 PGPhysics::GetGravity() const
{
	// Get the PL physics world this scene node is in
	SceneContainer *pContainer = GetContainer();
	while (pContainer && !pContainer->IsInstanceOf("PLPhysics::SCPhysicsWorld"))
		pContainer = pContainer->GetContainer();

	// Get the gravity vector
	Vector3 vGravity;
	if (pContainer && static_cast<SCPhysicsWorld*>(pContainer)->GetWorld())
		static_cast<SCPhysicsWorld*>(pContainer)->GetWorld()->GetGravity(vGravity);
	else
		vGravity.SetXYZ(0.0f, -9.81f, 0.0f);

	// Return the gravity vector
	return vGravity;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
PGPhysics::PGPhysics()
{
}

/**
*  @brief
*    Destructor
*/
PGPhysics::~PGPhysics()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLParticleGroups
