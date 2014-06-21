/*********************************************************\
 *  File: SNMPositionMoveToTarget.cpp                    *
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
#include <PLCore/Tools/Timing.h>
#include <PLCore/Container/Stack.h>
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneContainer.h"
#include "PLScene/Scene/SceneNodeModifiers/SNMPositionMoveToTarget.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMPositionMoveToTarget)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPositionMoveToTarget::SNMPositionMoveToTarget(SceneNode &cSceneNode) : SNMTransform(cSceneNode),
	Target(this),
	Speed(this),
	EventHandlerUpdate(&SNMPositionMoveToTarget::OnUpdate, this)
{
}

/**
*  @brief
*    Destructor
*/
SNMPositionMoveToTarget::~SNMPositionMoveToTarget()
{
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNodeModifier functions         ]
//[-------------------------------------------------------]
void SNMPositionMoveToTarget::OnActivate(bool bActivate)
{
	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate)
			pSceneContext->EventUpdate.Connect(EventHandlerUpdate);
		else
			pSceneContext->EventUpdate.Disconnect(EventHandlerUpdate);
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the target position within the container space of the owner node
*/
bool SNMPositionMoveToTarget::GetTargetPosition(Vector3 &vPos) const
{
	// Get the target scene node
	const SceneNode *pTarget;
	if (GetSceneNode().GetContainer())
		pTarget = GetSceneNode().GetContainer()->GetByName(Target.Get());
	else {
		// This must be the root :()
		pTarget = static_cast<const SceneContainer&>(GetSceneNode()).GetByName(Target.Get());
	}
	if (!pTarget)
		return false; // Error - no valid target scene node, no target rotation :(

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

/**
*  @brief
*    Called when the scene node modifier needs to be updated
*/
void SNMPositionMoveToTarget::OnUpdate()
{
	// Get the position of the target scene node
	Vector3 vTargetPos;
	if (GetTargetPosition(vTargetPos)) {
		// Get the scene node
		SceneNode &cSceneNode = GetSceneNode();

		// 'Move' the scene node a bit towards the new desired position...
		const float fFactor = Timing::GetInstance()->GetTimeDifference()*Speed;
		cSceneNode.MoveTo(cSceneNode.GetTransform().GetPosition() + (vTargetPos - cSceneNode.GetTransform().GetPosition())*fFactor);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
