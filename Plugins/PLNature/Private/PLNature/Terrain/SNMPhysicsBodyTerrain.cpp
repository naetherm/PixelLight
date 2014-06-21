/*********************************************************\
 *  File: SNMPhysicsBodyTerrain.cpp                      *
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
#include <PLPhysics/Body.h>
#include <PLPhysics/World.h>
#include <PLPhysics/SceneNodes/SCPhysicsWorld.h>
#include "PLNature/Terrain/SNTerrain.h"
#include "PLNature/Terrain/GMMSurface.h"
#include "PLNature/Terrain/SNMPhysicsBodyTerrain.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLScene;
using namespace PLPhysics;
namespace PLNature {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMPhysicsBodyTerrain)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
const Vector3 &SNMPhysicsBodyTerrain::GetScale() const
{
	return m_vScale;
}

void SNMPhysicsBodyTerrain::SetScale(const Vector3 &vValue)
{
	if (m_vScale != vValue) {
		m_vScale = vValue;
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
SNMPhysicsBodyTerrain::SNMPhysicsBodyTerrain(SceneNode &cSceneNode) : SNMPhysicsBody(cSceneNode),
	Scale(this),
	m_vScale(1.0f, 1.0f, 1.0f)
{
}

/**
*  @brief
*    Destructor
*/
SNMPhysicsBodyTerrain::~SNMPhysicsBodyTerrain()
{
}


//[-------------------------------------------------------]
//[ Private virtual SNMPhysicsBody functions              ]
//[-------------------------------------------------------]
void SNMPhysicsBodyTerrain::CreatePhysicsBody()
{
	if (m_pWorldContainer && m_pWorldContainer->GetWorld()) {
		// Get the gmm surface of the terrain scene node
		SNTerrain &cTerrainSceneNode = static_cast<SNTerrain&>(GetSceneNode());
		const GMMSurface *pGMMSurface = cTerrainSceneNode.GetGMMSurface();
		if (pGMMSurface && pGMMSurface->GetHeightMap()) {
			// Create the PL physics body
			const AABoundingBox &cAABBox = cTerrainSceneNode.GetAABoundingBox();
			m_pBodyHandler->SetElement(m_pWorldContainer->GetWorld()->CreateBodyTerrain(pGMMSurface->GetHeightMapSize(),
																						pGMMSurface->GetHeightMapSize(),
																						pGMMSurface->GetHeightMap(),
																						cAABBox.vMin, cAABBox.vMax, m_vScale));

			// Call base implementation
			SNMPhysicsBody::CreatePhysicsBody();
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLNature
