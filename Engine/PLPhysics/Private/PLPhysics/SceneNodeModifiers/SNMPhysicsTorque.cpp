/*********************************************************\
 *  File: SNMPhysicsTorque.cpp                           *
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
#include <PLScene/Scene/SceneContext.h>
#include "PLPhysics/Body.h"
#include "PLPhysics/SceneNodeModifiers/SNMPhysicsBody.h"
#include "PLPhysics/SceneNodeModifiers/SNMPhysicsTorque.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLScene;
namespace PLPhysics {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMPhysicsTorque)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPhysicsTorque::SNMPhysicsTorque(SceneNode &cSceneNode) : SNMPhysics(cSceneNode),
	Torque(this),
	EventHandlerUpdate(&SNMPhysicsTorque::OnUpdate, this)
{
}

/**
*  @brief
*    Destructor
*/
SNMPhysicsTorque::~SNMPhysicsTorque()
{
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneNodeModifier functions ]
//[-------------------------------------------------------]
void SNMPhysicsTorque::OnActivate(bool bActivate)
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
void SNMPhysicsTorque::OnUpdate()
{
	const SNMPhysicsBody *pModifier = static_cast<SNMPhysicsBody*>(GetSceneNode().GetModifier("PLPhysics::SNMPhysicsBody"));
	if (pModifier) {
		Body *pBody = pModifier->GetBody();
		if (pBody)
			pBody->SetTorque(Torque.Get());
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics
