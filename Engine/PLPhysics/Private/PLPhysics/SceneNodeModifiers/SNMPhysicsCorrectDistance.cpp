/*********************************************************\
 *  File: SNMPhysicsCorrectDistance.cpp                  *
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
#include <PLScene/Scene/SceneContext.h>
#include <PLScene/Scene/SceneContainer.h>
#include "PLPhysics/Body.h"
#include "PLPhysics/World.h"
#include "PLPhysics/Sensor.h"
#include "PLPhysics/SceneNodes/SCPhysicsWorld.h"
#include "PLPhysics/SceneNodeModifiers/SNMPhysicsBody.h"
#include "PLPhysics/SceneNodeModifiers/SNMPhysicsCorrectDistance.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLScene;
namespace PLPhysics {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMPhysicsCorrectDistance)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPhysicsCorrectDistance::SNMPhysicsCorrectDistance(SceneNode &cSceneNode) : SNMPhysics(cSceneNode),
	Target(this),
	BlendOutTarget(this),
	BlendOutDistance(this),
	EventHandlerUpdate(&SNMPhysicsCorrectDistance::OnUpdate, this)
{
}

/**
*  @brief
*    Destructor
*/
SNMPhysicsCorrectDistance::~SNMPhysicsCorrectDistance()
{
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneNodeModifier functions ]
//[-------------------------------------------------------]
void SNMPhysicsCorrectDistance::OnActivate(bool bActivate)
{
	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate) {
			pSceneContext->EventUpdate.Connect(EventHandlerUpdate);

			// Make a first update to ensure everything is up-to-date when we're going active (synchronization, no logic update)
			OnUpdate();
		} else {
			pSceneContext->EventUpdate.Disconnect(EventHandlerUpdate);
		}
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node modifier needs to be updated
*/
void SNMPhysicsCorrectDistance::OnUpdate()
{
	// Get the scene node
	SceneNode &cSceneNode = GetSceneNode();

	// Get the target scene node
	SceneNode *pTarget, *pBlendOutTarget;
	if (cSceneNode.GetContainer()) {
		pTarget = cSceneNode.GetContainer()->GetByName(Target.Get());
		if (BlendOutTarget.Get().GetLength())
			pBlendOutTarget = cSceneNode.GetContainer()->GetByName(BlendOutTarget.Get());
		else
			pBlendOutTarget = pTarget;
	} else {
		// This must be the root :()
		pTarget = static_cast<SceneContainer&>(GetSceneNode()).GetByName(Target.Get());
		if (BlendOutTarget.Get().GetLength())
			pBlendOutTarget = static_cast<SceneContainer&>(GetSceneNode()).GetByName(BlendOutTarget.Get());
		else
			pBlendOutTarget = pTarget;
	}
	if (pTarget && pBlendOutTarget) {
		pBlendOutTarget->SetVisible(true);

		// Get the position of the target scene node
		Vector3 vTargetPos = pTarget->GetTransform().GetPosition();

		// Are we in luck and the target is within the same container as the owner node?
		if (cSceneNode.GetContainer() != pTarget->GetContainer()) {
			// Nope, we have to translate the position into the correct space :(
			SceneContainer *pContainer = pTarget->GetContainer();
			while (pContainer) {
				vTargetPos *= pContainer->GetTransform().GetMatrix();
				pContainer = pContainer->GetContainer();
			}

			// To container space of the owner node
			pContainer = cSceneNode.GetContainer();
			Stack<SceneContainer*> lstStack;
			while (pContainer) {
				lstStack.Push(pContainer);
				pContainer = pContainer->GetContainer();
			}
			while (lstStack.GetNumOfElements()) {
				pContainer = lstStack.Top();
				lstStack.Pop();
				vTargetPos *= pContainer->GetTransform().GetInverseMatrix();
			}
		}

		// Get the PL physics world the owner scene node is in
		const SceneContainer *pContainer = cSceneNode.GetContainer();
		while (pContainer && !pContainer->IsInstanceOf("PLPhysics::SCPhysicsWorld"))
			pContainer = pContainer->GetContainer();
		if (pContainer) {
			World *pPhysicsWorld = static_cast<const SCPhysicsWorld*>(pContainer)->GetWorld();
			if (pPhysicsWorld) {
				// Perform physics ray cast from target to the owner scene node
				const Vector3 &vPos    = cSceneNode.GetTransform().GetPosition();
				Sensor		  *pSensor = pPhysicsWorld->CreateSensorRaycast(vTargetPos, vPos, Sensor::ClosestBody);
				if (pSensor) {
					// Perform an immediate check
					if (pSensor->Check()) {
						// Closest distance
						float fDistance = -1.0f;

						// Get the PL physics body scene node modifier of the target scene node
						const SNMPhysicsBody *pModifier = static_cast<const SNMPhysicsBody*>(pBlendOutTarget->GetModifier("PLPhysics::SNMPhysicsBody"));
						if (pModifier && pModifier->GetBody()) {
							// Get the PL physics body this modifier is using
							const Body *pTargetBody = pModifier->GetBody();

							// Get the closest hit physics body that is not the physics body of the target scene node
							const Sensor::BodyInfo *pClosestBodyInfo = nullptr;
							for (uint32 i=0; i<pSensor->GetNumOfHitBodies(); i++) {
								// Ignore the physics body of the target scene node
								const Sensor::BodyInfo *pBodyInfo = pSensor->GetHitBody(i);
								if (pBodyInfo->pBody != pTargetBody) {
									// Is this the first checked body information?
									if (pClosestBodyInfo) {
										// Is this body closer?
										if (pBodyInfo->fDistance < pClosestBodyInfo->fDistance)
											pClosestBodyInfo = pBodyInfo;
									} else {
										pClosestBodyInfo = pBodyInfo;
									}
								}
							}
							if (pClosestBodyInfo)
								fDistance = pClosestBodyInfo->fDistance;
						} else {
							// Just get the closest hit physics body
							const Sensor::BodyInfo *pBodyInfo = pSensor->GetClosestBody();
							if (pBodyInfo && pBodyInfo->pBody)
								fDistance = pBodyInfo->fDistance;
						}

						// Move the owner scene node to a correct position
						if (fDistance > 0.0f && fDistance < 1.0f) {
							// Calculate the new position
							const Vector3 vNewPosition = vTargetPos + (vPos - vTargetPos)*fDistance;

							// Is there a target blend out distance?
							if (BlendOutDistance) {
								// Calculate the current distance between the target and the new position
								const Vector3 vDiff = vNewPosition - vTargetPos;
								fDistance = vDiff.GetLength();

								// Do we need to blend out because it's TO close to the target?
								if (fDistance < BlendOutDistance)
									pBlendOutTarget->SetVisible(false);
							}

							// Move the owner scene node to a correct position
							cSceneNode.MoveTo(vNewPosition);
						}
					}

					// Cleanup
					delete pSensor;
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics
