/*********************************************************\
 *  File: SNMMeshUpdate.cpp                              *
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
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLMesh/MeshHandler.h>
#include <PLMesh/SkeletonHandler.h>
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneNodeModifiers/SNMMeshUpdate.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
using namespace PLMesh;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMMeshUpdate)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMMeshUpdate::SNMMeshUpdate(SceneNode &cSceneNode) : SNMMesh(cSceneNode),
	Flags(this),
	EventHandlerAddedToVisibilityTree(&SNMMeshUpdate::OnAddedToVisibilityTree, this),
	EventHandlerUpdate(&SNMMeshUpdate::OnUpdate, this),
	m_bUpdate(true),	// Should be true so for example a valid bounding box, used for visibility detection, is calculated
	m_bFirstUpdate(true)
{
}

/**
*  @brief
*    Destructor
*/
SNMMeshUpdate::~SNMMeshUpdate()
{
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNodeModifier functions         ]
//[-------------------------------------------------------]
void SNMMeshUpdate::OnActivate(bool bActivate)
{
	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate) {
			GetSceneNode().SignalAddedToVisibilityTree.Connect(EventHandlerAddedToVisibilityTree);
			pSceneContext->EventUpdate.Connect(EventHandlerUpdate);

			// Make a first update to ensure everything is up-to-date when we're going active (synchronization and logic update)
			OnUpdate();
		} else {
			GetSceneNode().SignalAddedToVisibilityTree.Disconnect(EventHandlerAddedToVisibilityTree);
			pSceneContext->EventUpdate.Disconnect(EventHandlerUpdate);
		}
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the owner scene node was added to a visibility tree
*/
void SNMMeshUpdate::OnAddedToVisibilityTree(VisNode &cVisNode)
{
	m_bUpdate = true;
}

/**
*  @brief
*    Called when the scene node modifier needs to be updated
*/
void SNMMeshUpdate::OnUpdate()
{
	// If this scene node wasn't drawn at the last frame, we can skip some update stuff
	MeshHandler *pMeshHandler = GetSceneNode().GetMeshHandler();
	if (pMeshHandler && (m_bUpdate || (GetFlags() & UpdateUnseen))) {
		m_bUpdate = false;

		// Do we need a frequent bounding box update?
		const bool bJointAABoundingBox = (pMeshHandler->GetVertexBuffer() && pMeshHandler->GetSkeletonHandler());

		// Lock the vertex buffer once to avoid many internal locks...
		if (bJointAABoundingBox)
			pMeshHandler->GetVertexBuffer()->Lock(Lock::ReadWrite);

		// Update mesh handler
		pMeshHandler->Update(Timing::GetInstance()->GetTimeDifference());

		if (bJointAABoundingBox) {
			if (m_bFirstUpdate) {
				m_bFirstUpdate = false;

				// Set default mesh axis align bounding box
				m_cDefaultMeshAABoundingBox = GetSceneNode().GetAABoundingBox();

				// Set default joint axis align bounding box
				pMeshHandler->CalculateJointBoundingBox(m_cDefaultJointAABoundingBox);
			}

			// Unlock the vertex buffer
			pMeshHandler->GetVertexBuffer()->Unlock();

			// Get current joint axis align bounding box
			AABoundingBox cJointBB;
			pMeshHandler->CalculateJointBoundingBox(cJointBB);

			// Update the bounding box of the scene node
			// [TODO] Remove 'm_cDefaultMeshAABoundingBox'
			GetSceneNode().SetAABoundingBox(cJointBB);
//			SetAABoundingBox(AABoundingBox(m_cDefaultMeshAABoundingBox.vMin + (cJointBB.vMin - m_cDefaultJointAABoundingBox.vMin),
//										   m_cDefaultMeshAABoundingBox.vMax + (cJointBB.vMax - m_cDefaultJointAABoundingBox.vMax)));
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
