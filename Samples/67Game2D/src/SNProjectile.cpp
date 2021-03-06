/*********************************************************\
 *  File: SNProjectile.cpp                               *
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
#include <PLScene/Scene/SceneContext.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLScene/Scene/SceneQueries/SQAABoundingBox.h>
#include "SNProjectile.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLScene;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SNProjectile, "", PLScene::SceneNode, "Projectile scene node")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Attributes
	pl_attribute_metadata(Direction,	PLMath::Vector2,	PLMath::Vector2::Zero,	ReadWrite,	"Projectile direction",	"")
	// Slots
	pl_slot_0_metadata(OnUpdate,													"Called when the scene node needs to be updated",																			"")
	pl_slot_2_metadata(OnSceneNode,	PLScene::SceneQuery&,	PLScene::SceneNode&,	"Called when a scene node was found, query found the scene node as first parameter, found scene node as second parameter",	"")
pl_class_metadata_end(SNProjectile)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNProjectile::SNProjectile() :
	Direction(this),
	SlotOnUpdate(this),
	SlotOnSceneNode(this)
{
	// Set the bounding box
	SetAABoundingBox(AABoundingBox(-3.0f, -3.0f, -1.0f, 3.0f, 3.0f, 1.0f));
}

/**
*  @brief
*    Destructor
*/
SNProjectile::~SNProjectile()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node needs to be updated
*/
void SNProjectile::OnUpdate()
{
	// Get time difference
	const float fTimeDiff = Timing::GetInstance()->GetTimeDifference();

	// Update the bomb timer
	GetTransform().SetPosition(GetTransform().GetPosition() + Vector3(Direction.Get().x, Direction.Get().y, 0.0f)*fTimeDiff*100);

	// Has the projectile left the screen?
	if (GetTransform().GetPosition().x < -3  || GetTransform().GetPosition().y < -3 || GetTransform().GetPosition().x > 323 || GetTransform().GetPosition().y > 203) {
		Delete(); // Destroy the projectile

	// Collision detection
	} else {
		// Create a new scene query we can use to check which scene nodes are within a given axis aligned bounding box
		SQAABoundingBox *pSceneQuery = static_cast<SQAABoundingBox*>(GetContainer()->CreateQuery("PLScene::SQAABoundingBox"));
		if (pSceneQuery) {
			// Connect event handler
			pSceneQuery->SignalSceneNode.Connect(SlotOnSceneNode);

			// Setup axis aligned bounding box
			pSceneQuery->GetAABoundingBox() = GetContainerAABoundingBox();

			// Let's perform the query...
			pSceneQuery->PerformQuery();

			// ... and clean up if we are done, we don't use 'GetContainer()' in here because this node
			// can be destroyed inside our query
			pSceneQuery->GetSceneContainer().DestroyQuery(*pSceneQuery);
		}
	}
}

/**
*  @brief
*    Called when a scene node was found
*/
void SNProjectile::OnSceneNode(SceneQuery &cQuery, SceneNode &cSceneNode)
{
	// Is this projectile still active?
	if (IsActive()) {
		// Is this a bomb?
		if (cSceneNode.IsInstanceOf("SNBomb")) {
			// Is the bomb still alive?
			if (!cSceneNode.GetAttribute("Killed")->GetBool()) {
				// Jap, kill the bomb right now
				cSceneNode.SetAttribute("Killed", "1");

				// Destroy this projectile
				Delete();

				// Done, do NOT continue the query!
				cQuery.Stop();
			}

		// Is this the UFO? (can not be killed :)
		} else if (cSceneNode.IsInstanceOf("SNUFO")) {
			// Jap, destroy this projectile
			Delete();

			// Done, do NOT continue the query!
			cQuery.Stop();
		}
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneNode functions          ]
//[-------------------------------------------------------]
void SNProjectile::OnActivate(bool bActivate)
{
	// Call the base implementation
	SceneNode::OnActivate(bActivate);

	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate)
			pSceneContext->EventUpdate.Connect(SlotOnUpdate);
		else
			pSceneContext->EventUpdate.Disconnect(SlotOnUpdate);
	}
}

