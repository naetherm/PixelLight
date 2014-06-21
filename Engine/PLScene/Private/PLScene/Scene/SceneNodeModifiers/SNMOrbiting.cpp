/*********************************************************\
 *  File: SNMOrbiting.cpp                                *
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
#include <PLCore/Container/Stack.h>
#include <PLMath/Matrix3x3.h>
#include <PLMath/EulerAngles.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Effect/EffectManager.h>
#include "PLScene/Visibility/VisNode.h"
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneContainer.h"
#include "PLScene/Scene/SceneNodeModifiers/SNMOrbiting.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMOrbiting)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
float SNMOrbiting::GetMinDistance() const
{
	return m_fMinDistance;
}

void SNMOrbiting::SetMinDistance(float fValue)
{
	if (m_fMinDistance != fValue) {
		// Set distance
		m_fMinDistance = fValue;

		// Validate settings
		ValidateSettings();
	}
}

float SNMOrbiting::GetMaxDistance() const
{
	return m_fMaxDistance;
}

void SNMOrbiting::SetMaxDistance(float fValue)
{
	if (m_fMaxDistance != fValue) {
		// Set distance
		m_fMaxDistance = fValue;

		// Validate settings
		ValidateSettings();
	}
}

float SNMOrbiting::GetDistance()
{
	// Is the distance to the target scene node already initialized?
	if (m_fDistance < 0.0f && IsActive())
		OnPositionRotationUpdate();

	// Return the distance
	return m_fDistance;
}

void SNMOrbiting::SetDistance(float fValue)
{
	if (m_fDistance != fValue) {
		// Set distance
		m_fDistance = fValue;

		// Validate settings
		ValidateSettings();
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMOrbiting::SNMOrbiting(SceneNode &cSceneNode) : SceneNodeModifier(cSceneNode),
	Target(this),
	MinDistance(this),
	MaxDistance(this),
	Distance(this),
	Offset(this),
	UpVector(this),
	Pan(this),
	EventHandlerDrawDebug			  (&SNMOrbiting::OnDrawDebug,			   this),
	EventHandlerPositionRotationUpdate(&SNMOrbiting::OnPositionRotationUpdate, this),
	m_fMinDistance(0.0f),
	m_fMaxDistance(-1.0f),
	m_fDistance(-1.0f),
	m_bPositionListening(true)
{
	// Update the rotation also on position change: If this is not done in here, too, the camera may 'shiver'
	// if it's rotation is changed 'before' it's position is updated
}

/**
*  @brief
*    Destructor
*/
SNMOrbiting::~SNMOrbiting()
{
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNodeModifier functions         ]
//[-------------------------------------------------------]
void SNMOrbiting::OnActivate(bool bActivate)
{
	// Connect/disconnect event handlers
	SceneNode &cSceneNode = GetSceneNode();
	if (bActivate) {
		// Connect event handlers
		cSceneNode.SignalDrawDebug.Connect(EventHandlerDrawDebug);
		cSceneNode.GetTransform().EventPosition.Connect(EventHandlerPositionRotationUpdate);
		cSceneNode.GetTransform().EventRotation.Connect(EventHandlerPositionRotationUpdate);
		SceneContext *pSceneContext = GetSceneContext();
		if (pSceneContext)
			pSceneContext->EventUpdate.Connect(EventHandlerPositionRotationUpdate);

		// Make a first update to ensure position and rotation are up-to-date when we're going active (synchronization, no logic update)
		OnPositionRotationUpdate();
	} else {
		// Disconnect event handlers
		cSceneNode.SignalDrawDebug.Disconnect(EventHandlerDrawDebug);
		cSceneNode.GetTransform().EventPosition.Disconnect(EventHandlerPositionRotationUpdate);
		cSceneNode.GetTransform().EventRotation.Disconnect(EventHandlerPositionRotationUpdate);
		SceneContext *pSceneContext = GetSceneContext();
		if (pSceneContext)
			pSceneContext->EventUpdate.Disconnect(EventHandlerPositionRotationUpdate);
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Validates the minimum, maximum and distance settings
*/
void SNMOrbiting::ValidateSettings()
{
	// Ensure 'MaxDistance' is >= 'MinDistance'
	if (m_fMinDistance >= 0.0f && m_fMaxDistance >= 0.0f && m_fMaxDistance < m_fMinDistance) {
		const float fTemp = m_fMaxDistance;
		m_fMaxDistance = m_fMinDistance;
		m_fMinDistance = fTemp;
	}

	// Ensure 'Distance' is between 'MaxDistance' and 'MinDistance'
	if (m_fMinDistance >= 0.0f && m_fDistance < m_fMinDistance)
		m_fDistance = m_fMinDistance;
	if (m_fMaxDistance >= 0.0f && m_fDistance > m_fMaxDistance)
		m_fDistance = m_fMaxDistance;
}

/**
*  @brief
*    Returns the target position within the container space of the owner node
*/
bool SNMOrbiting::GetTargetPosition(Vector3 &vPos) const
{
	// Target scene node given?
	if (Target.Get().GetLength()) {
		// Get the target scene node
		const SceneNode *pTarget;
		if (GetSceneNode().GetContainer())
			pTarget = GetSceneNode().GetContainer()->GetByName(Target.Get());
		else {
			// This must be the root :()
			pTarget = static_cast<const SceneContainer&>(GetSceneNode()).GetByName(Target.Get());
		}
		if (pTarget) {
			// Set the position of the attached node
			vPos = pTarget->GetTransform().GetPosition() + Offset.Get();

			// Are we in luck and the target is within the same container as the owner node?
			if (GetSceneNode().GetContainer() != pTarget->GetContainer()) {
				// Nope, we have to translate the position into the correct space :(
				SceneContainer *pContainer = pTarget->GetContainer();
				while (pContainer) {
					vPos *= pContainer->GetTransform().GetMatrix();
					pContainer = pContainer->GetContainer();
				}

				// To container space of the owner node
				pContainer = GetSceneNode().GetContainer();
				Stack<SceneContainer*> lstStack;
				while (pContainer) {
					lstStack.Push(pContainer);
					pContainer = pContainer->GetContainer();
				}
				while (lstStack.GetNumOfElements()) {
					pContainer = lstStack.Top();
					lstStack.Pop();
					vPos *= pContainer->GetTransform().GetInverseMatrix();
				}
			}

			// Done
			return true;
		}
	}

	// Error - no valid target scene node, no target rotation :(
	return false;
}

/**
*  @brief
*    Called on scene node debug draw
*/
void SNMOrbiting::OnDrawDebug(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Get the position of the target scene node
	Vector3 vTargetPos;
	if (pVisNode && GetTargetPosition(vTargetPos)) {
		vTargetPos *= GetSceneNode().GetTransform().GetInverseMatrix();

		// Draw a line to the target
		cRenderer.GetRendererContext().GetEffectManager().Use();
		cRenderer.SetRenderState(RenderState::ZEnable,      false);
		cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
		cRenderer.GetDrawHelpers().DrawLine(Color4(0.0f, 1.0f, 1.0f, 1.0f), Vector3::Zero, vTargetPos, pVisNode->GetWorldViewProjectionMatrix(), 1.0f);
	}
}

/**
*  @brief
*    Called on scene node position or rotation change or update request
*/
void SNMOrbiting::OnPositionRotationUpdate()
{
	if (m_bPositionListening) {
		// Get the position of the target scene node
		Vector3 vTargetPos;
		if (GetTargetPosition(vTargetPos)) {
			// Get the owner scene node
			SceneNode &cSceneNode = GetSceneNode();

			// -> Please note that we have to stop listing for position events while setting the position
			m_bPositionListening = false;

			// Is the distance to the target scene node already initialized?
			if (m_fDistance >= 0.0f) {
				// Up vector given?
				if (!UpVector.Get().IsNull()) {
					Matrix3x3 mRot;
					mRot.LookAt(cSceneNode.GetTransform().GetPosition(), cSceneNode.GetTransform().GetPosition()-cSceneNode.GetTransform().GetRotation().GetZAxis(), UpVector.Get());
					Quaternion qQ = mRot;
					qQ.UnitInvert();
					cSceneNode.GetTransform().SetRotation(qQ);
				}

				// Get owner scene node direction vector
				Vector3 vDir = cSceneNode.GetTransform().GetRotation().GetZAxis();

				// Scale direction vector with the distance
				vDir *= m_fDistance;

				// Get direction vectors
				const Vector3 vDirLeftVector = GetSceneNode().GetTransform().GetRotation().GetXAxis();
				const Vector3 vDirUpVector   = GetSceneNode().GetTransform().GetRotation().GetYAxis();

				// Get 3D pan
				const Vector3 vPan = vDirLeftVector*Pan.Get().x + vDirUpVector*Pan.Get().y;

				// Set the new position of the owner scene node
				cSceneNode.GetTransform().SetPosition(vTargetPos - vDir + vPan);
			} else {
				// Get the length of the direction vector (= distance to the target scene node)
				m_fDistance = (vTargetPos-cSceneNode.GetTransform().GetPosition()).GetLength();

				// Set rotation of the owner scene node
				Matrix3x3 mRot;
				mRot.LookAt(vTargetPos, cSceneNode.GetTransform().GetPosition(), UpVector.Get().IsNull() ? Vector3::UnitY : UpVector.Get());
				cSceneNode.GetTransform().SetRotation(Quaternion(mRot).GetUnitInverted());
			}

			// Listen again, please
			m_bPositionListening = true;
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
