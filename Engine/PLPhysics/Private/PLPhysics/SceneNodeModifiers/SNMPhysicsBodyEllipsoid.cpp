/*********************************************************\
 *  File: SNMPhysicsBodyEllipsoid.cpp                    *
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
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/FixedFunctions.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLMesh/MeshManager.h>
#include <PLScene/Scene/SceneContext.h>
#include <PLScene/Visibility/VisNode.h>
#include "PLPhysics/Body.h"
#include "PLPhysics/World.h"
#include "PLPhysics/SceneNodes/SCPhysicsWorld.h"
#include "PLPhysics/SceneNodeModifiers/SNMPhysicsBodyEllipsoid.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLMesh;
using namespace PLScene;
namespace PLPhysics {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMPhysicsBodyEllipsoid)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
const Vector3 &SNMPhysicsBodyEllipsoid::GetRadius() const
{
	return m_vRadius;
}

void SNMPhysicsBodyEllipsoid::SetRadius(const Vector3 &vValue)
{
	if (m_vRadius != vValue) {
		m_vRadius = vValue;
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
SNMPhysicsBodyEllipsoid::SNMPhysicsBodyEllipsoid(SceneNode &cSceneNode) : SNMPhysicsBody(cSceneNode),
	Radius(this),
	SlotOnDrawDebug(this),
	m_vRadius(0.0f, 0.0f, 0.0f)
{
}

/**
*  @brief
*    Destructor
*/
SNMPhysicsBodyEllipsoid::~SNMPhysicsBodyEllipsoid()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called on scene node debug draw
*/
void SNMPhysicsBodyEllipsoid::OnDrawDebug(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Is there a PL physics body?
	const Body *pBody = GetBody();
	if (pBody) {
		// Create the sphere mesh if required
		static MeshHandler cMeshHandler;
		if (!cMeshHandler.GetMesh())
			cMeshHandler.SetMesh(GetSceneNode().GetSceneContext()->GetMeshManager().CreateMesh("PLMesh::MeshCreatorSphere", true, "Radius=1 Detail=26"));
		if (cMeshHandler.GetMesh()) {
			// Fixed functions support required
			FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
			if (pFixedFunctions) {
				// Get the scene node
				const SceneNode &cSceneNode = GetSceneNode();

				// Set world matrix
				Matrix3x4 mWorld = pVisNode->GetWorldMatrix();
				mWorld.SetTranslation(mWorld.GetTranslation() + cSceneNode.GetTransform().GetRotation()*m_vPositionOffset);
				Matrix3x4 mScale;
				mScale.SetScaleMatrix((1.0f/cSceneNode.GetTransform().GetScale().x)*m_vRadius.x,
									  (1.0f/cSceneNode.GetTransform().GetScale().y)*m_vRadius.y,
									  (1.0f/cSceneNode.GetTransform().GetScale().z)*m_vRadius.z);
				mWorld *= mScale;
				pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, mWorld);

				// Setup render states
				cRenderer.GetRendererContext().GetEffectManager().Use();
				cRenderer.SetRenderState(RenderState::BlendEnable,  true);
				cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
				cRenderer.SetRenderState(RenderState::ZEnable,      false);
				pFixedFunctions->SetColor(Color4(1.0f, 0.5f, 0.5f, 0.5f));

				// Draw transparent
				cMeshHandler.Draw(false, false);
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneNodeModifier functions  ]
//[-------------------------------------------------------]
void SNMPhysicsBodyEllipsoid::OnActivate(bool bActivate)
{
	// Call base implementation
	SNMPhysicsBody::OnActivate(bActivate);

	// Connect/disconnect event handler
	if (bActivate)
		GetSceneNode().SignalDrawDebug.Connect(SlotOnDrawDebug);
	else
		GetSceneNode().SignalDrawDebug.Disconnect(SlotOnDrawDebug);
}


//[-------------------------------------------------------]
//[ Private virtual SNMPhysicsBody functions              ]
//[-------------------------------------------------------]
void SNMPhysicsBodyEllipsoid::CreatePhysicsBody()
{
	if (m_pWorldContainer && m_pWorldContainer->GetWorld()) {
		// Use a ellipsoid 'inside' the axis align bounding box in 'scene node space' as default?
		if (m_vRadius.IsNull()) {
			const Vector3 &vScale = GetSceneNode().GetTransform().GetScale();
			AABoundingBox cAABB = GetSceneNode().GetAABoundingBox();
			cAABB.vMin *= vScale;
			cAABB.vMax *= vScale;
			m_vRadius.SetXYZ(cAABB.GetWidth()*0.5f, cAABB.GetHeight()*0.5f, cAABB.GetDepth()*0.5f);
			m_vPositionOffset = cAABB.GetCenter();
		}

		// Create the PL physics body
		m_pBodyHandler->SetElement(m_pWorldContainer->GetWorld()->CreateBodyEllipsoid(m_vRadius, !m_fMass));

		// Call base implementation
		SNMPhysicsBody::CreatePhysicsBody();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics
