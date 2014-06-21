/*********************************************************\
 *  File: SNMRotationTarget.cpp                          *
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
#include "PLScene/Scene/SceneNodeModifiers/SNMRotationTarget.h"


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
pl_implement_class(SNMRotationTarget)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMRotationTarget::SNMRotationTarget(SceneNode &cSceneNode) : SNMTransform(cSceneNode),
	Target(this),
	Offset(this),
	UpVector(this),
	EventHandlerPositionUpdate(&SNMRotationTarget::OnPositionUpdate, this),
	EventHandlerDrawDebug	  (&SNMRotationTarget::OnDrawDebug,		 this)
{
}

/**
*  @brief
*    Destructor
*/
SNMRotationTarget::~SNMRotationTarget()
{
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNodeModifier functions         ]
//[-------------------------------------------------------]
void SNMRotationTarget::OnActivate(bool bActivate)
{
	// Update the rotation also on position change: If this is not done in here, too, the camera may 'shiver'
	// if it's rotation is changed 'before' it's position is updated

	// Connect/disconnect event handlers
	SceneNode &cSceneNode = GetSceneNode();
	if (bActivate) {
		// Connect event handlers
		cSceneNode.SignalDrawDebug.Connect(EventHandlerDrawDebug);
		cSceneNode.GetTransform().EventPosition.Connect(EventHandlerPositionUpdate);
		SceneContext *pSceneContext = GetSceneContext();
		if (pSceneContext)
			pSceneContext->EventUpdate.Connect(EventHandlerPositionUpdate);

		// Make a first update to ensure everything is up-to-date when we're going active (synchronization, no logic update)
		OnPositionUpdate();
	} else {
		// Disconnect event handlers
		cSceneNode.SignalDrawDebug.Disconnect(EventHandlerDrawDebug);
		cSceneNode.GetTransform().EventPosition.Disconnect(EventHandlerPositionUpdate);
		SceneContext *pSceneContext = GetSceneContext();
		if (pSceneContext)
			pSceneContext->EventUpdate.Disconnect(EventHandlerPositionUpdate);
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the target position within the container space of the owner node
*/
bool SNMRotationTarget::GetTargetPosition(Vector3 &vPos) const
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
			vPos = pTarget->GetTransform().GetPosition();

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
void SNMRotationTarget::OnDrawDebug(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Get the position of the target scene node
	Vector3 vTargetPos;
	if (GetTargetPosition(vTargetPos)) {
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
*    Called on scene node position change or update request
*/
void SNMRotationTarget::OnPositionUpdate()
{
	// Get the position of the target scene node
	Vector3 vTargetPos;
	if (GetTargetPosition(vTargetPos)) {
		// Get a quaternion representation of the rotation offset
		Quaternion qRotOffset;
		EulerAngles::ToQuaternion(static_cast<float>(Offset.Get().x*Math::DegToRad), static_cast<float>(Offset.Get().y*Math::DegToRad), static_cast<float>(Offset.Get().z*Math::DegToRad), qRotOffset);

		// Set rotation
		Matrix3x3 mRot;
		mRot.LookAt(vTargetPos, GetSceneNode().GetTransform().GetPosition(), UpVector.Get());
		GetSceneNode().GetTransform().SetRotation(Quaternion(mRot).GetUnitInverted()*qRotOffset);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
