/*********************************************************\
 *  File: SNMPhysicsBodyCapsule.cpp                      *
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
#include "PLPhysics/Body.h"
#include "PLPhysics/World.h"
#include "PLPhysics/SceneNodes/SCPhysicsWorld.h"
#include "PLPhysics/SceneNodeModifiers/SNMPhysicsBodyCapsule.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLScene;
namespace PLPhysics {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SNMPhysicsBodyCapsule, "PLPhysics", PLPhysics::SNMPhysicsBody, "Physics capsule body scene node modifier")
	// Constructors
	pl_constructor_1_metadata(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
	// Attributes
	pl_attribute_metadata(Radius,	float,	1.0f,	ReadWrite,	"Capsule radius at the base",								"Min='0.0'")
	pl_attribute_metadata(Height,	float,	1.0f,	ReadWrite,	"Capsule height along the x local axis from base to top",	"Min='0.0'")
pl_class_metadata_end(SNMPhysicsBodyCapsule)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
float SNMPhysicsBodyCapsule::GetRadius() const
{
	return m_fRadius;
}

void SNMPhysicsBodyCapsule::SetRadius(float fValue)
{
	if (m_fRadius != fValue) {
		m_fRadius = fValue;
		RecreatePhysicsBody();
	}
}

float SNMPhysicsBodyCapsule::GetHeight() const
{
	return m_fHeight;
}

void SNMPhysicsBodyCapsule::SetHeight(float fValue)
{
	if (m_fHeight != fValue) {
		m_fHeight = fValue;
		RecreatePhysicsBody();
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPhysicsBodyCapsule::SNMPhysicsBodyCapsule(SceneNode &cSceneNode) : SNMPhysicsBody(cSceneNode),
	Radius(this),
	Height(this),
	m_fRadius(1.0f),
	m_fHeight(1.0f)
{
}

/**
*  @brief
*    Destructor
*/
SNMPhysicsBodyCapsule::~SNMPhysicsBodyCapsule()
{
}


//[-------------------------------------------------------]
//[ Private virtual SNMPhysicsBody functions              ]
//[-------------------------------------------------------]
void SNMPhysicsBodyCapsule::CreatePhysicsBody()
{
	if (m_pWorldContainer && m_pWorldContainer->GetWorld()) {
		// Create the PL physics body
		m_pBodyHandler->SetElement(m_pWorldContainer->GetWorld()->CreateBodyCapsule(m_fRadius, m_fHeight, !m_fMass));

		// Call base implementation
		SNMPhysicsBody::CreatePhysicsBody();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics
