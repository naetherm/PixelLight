/*********************************************************\
 *  File: SNMPhysicsBodyBox.cpp                          *
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
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLScene/Visibility/VisNode.h>
#include "PLPhysics/Body.h"
#include "PLPhysics/World.h"
#include "PLPhysics/SceneNodes/SCPhysicsWorld.h"
#include "PLPhysics/SceneNodeModifiers/SNMPhysicsBodyBox.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLScene;
namespace PLPhysics {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMPhysicsBodyBox)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
const Vector3 &SNMPhysicsBodyBox::GetDimension() const
{
	return m_vDimension;
}

void SNMPhysicsBodyBox::SetDimension(const Vector3 &vValue)
{
	if (m_vDimension != vValue) {
		m_vDimension = vValue;
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
SNMPhysicsBodyBox::SNMPhysicsBodyBox(SceneNode &cSceneNode) : SNMPhysicsBody(cSceneNode),
	Dimension(this),
	SlotOnDrawDebug(this),
	m_vDimension(0.0f, 0.0f, 0.0f)
{
}

/**
*  @brief
*    Destructor
*/
SNMPhysicsBodyBox::~SNMPhysicsBodyBox()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called on scene node debug draw
*/
void SNMPhysicsBodyBox::OnDrawDebug(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Is there a PL physics body?
	const Body *pBody = GetBody();
	if (pBody && pVisNode) {
		// Get the scene node
		const SceneNode &cSceneNode = GetSceneNode();

		// Calculate the world matrix
		Matrix3x4 mWorld = pVisNode->GetWorldMatrix();
		mWorld.SetTranslation(mWorld.GetTranslation() + cSceneNode.GetTransform().GetRotation()*m_vPositionOffset);
		Matrix3x4 mScale;
		mScale.SetScaleMatrix((1.0f/cSceneNode.GetTransform().GetScale().x)*m_vDimension.x*0.5f,
							  (1.0f/cSceneNode.GetTransform().GetScale().y)*m_vDimension.y*0.5f,
							  (1.0f/cSceneNode.GetTransform().GetScale().z)*m_vDimension.z*0.5f);
		mWorld *= mScale;

		// Calculate the world view projection matrix
		const Matrix4x4 mWorldViewProjection = pVisNode->GetViewProjectionMatrix()*mWorld;

		// Setup render states
		cRenderer.GetRendererContext().GetEffectManager().Use();

		// Draw wireframe
		cRenderer.GetDrawHelpers().DrawBox(Color4::White, Vector3::NegativeOne, Vector3::One, mWorldViewProjection, 1.0f);

		// Setup render states
		cRenderer.SetRenderState(RenderState::BlendEnable,  true);
		cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
		cRenderer.SetRenderState(RenderState::ZEnable,      false);

		// Draw transparent
		cRenderer.GetDrawHelpers().DrawBox(Color4(1.0f, 0.5f, 0.5f, 0.5f), Vector3::NegativeOne, Vector3::One, mWorldViewProjection, 0.0f);
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneNodeModifier functions  ]
//[-------------------------------------------------------]
void SNMPhysicsBodyBox::OnActivate(bool bActivate)
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
void SNMPhysicsBodyBox::CreatePhysicsBody()
{
	if (m_pWorldContainer && m_pWorldContainer->GetWorld()) {
		// Use the axis align bounding box in 'scene node space' as default?
		if (m_vDimension.IsNull()) {
			const Vector3 &vScale = GetSceneNode().GetTransform().GetScale();
			AABoundingBox cAABB = GetSceneNode().GetAABoundingBox();
			cAABB.vMin *= vScale;
			cAABB.vMax *= vScale;
			m_vDimension.SetXYZ(cAABB.GetWidth(), cAABB.GetHeight(), cAABB.GetDepth());
			m_vPositionOffset = cAABB.GetCenter();
		}

		// Create the PL physics body
		m_pBodyHandler->SetElement(m_pWorldContainer->GetWorld()->CreateBodyBox(m_vDimension, !m_fMass));

		// Call base implementation
		SNMPhysicsBody::CreatePhysicsBody();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics
