/*********************************************************\
 *  File: SRPVolume.cpp                                  *
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
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLScene/Scene/SNLight.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLScene/Visibility/SQCull.h>
#include <PLScene/Visibility/VisPortal.h>
#include <PLScene/Visibility/VisContainer.h>
#include "PLVolume/Scene/SRPVolume.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
using namespace PLScene;
namespace PLVolume {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SRPVolume)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
SRPVolume::~SRPVolume()
{
}

/**
*  @brief
*    Returns a list of all visible scene node instances intersecting with the given scene node
*/
void SRPVolume::GetIntersectingInstancesOf(const SQCull &cCullQuery, SceneNode &cSceneNode, const Class &cClass, Array<const VisNode*> &lstIntersecting) const
{
	// Search recursive from front to back
	GetIntersectingInstancesOfRec(cCullQuery, cSceneNode, cClass, lstIntersecting);
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPVolume::SRPVolume() :
	Flags(this)
{
}

/**
*  @brief
*    Searches for the first (= nearest) visible clip plane scene node
*/
const VisNode *SRPVolume::GetFirstClipPlane(const SQCull &cCullQuery) const
{
	// Search recursive from front to back
	return GetFirstClipPlaneRec(cCullQuery);
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneRendererPass functions ]
//[-------------------------------------------------------]
void SRPVolume::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Reset all render states to default
	cRenderer.GetRendererContext().GetEffectManager().Use();

	// Enable scissor test
	cRenderer.SetRenderState(RenderState::ScissorTestEnable, true);

	// Draw recursive from front to back
	DrawRec(cRenderer, cCullQuery);
}



//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Draws recursive
*/
void SRPVolume::DrawRec(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Set the new scissor rectangle
	const VisContainer &cVisContainer = cCullQuery.GetVisContainer();
	cRenderer.SetScissorRect(&cVisContainer.GetProjection().cRectangle);

	// Render all visible scene nodes of this scene container
	Iterator<VisNode*> cIterator = (GetFlags() & DrawOrderFrontToBack) ? cVisContainer.GetVisNodes().GetIterator() : cVisContainer.GetVisNodes().GetEndIterator();
	while ((GetFlags() & DrawOrderFrontToBack) ? cIterator.HasNext() : cIterator.HasPrevious()) {
		// Get visibility node and scene node
		const VisNode   *pVisNode   = (GetFlags() & DrawOrderFrontToBack) ? cIterator.Next() : cIterator.Previous();
			  SceneNode *pSceneNode = pVisNode->GetSceneNode();
		if (pSceneNode) {
			// Is this scene node a portal?
			if (pVisNode->IsPortal()) {
				// Get the target cell visibility container
				const VisContainer *pVisCell = static_cast<const VisPortal*>(pVisNode)->GetTargetVisContainer();
				if (pVisCell && pVisCell->GetCullQuery()) {
					// Draw the target cell
					DrawRec(cRenderer, *pVisCell->GetCullQuery());

					// Set the previous scissor rectangle
					cRenderer.SetScissorRect(&cVisContainer.GetProjection().cRectangle);
				}

			// Is this scene node a container? We do not need to check for cells because we will
			// NEVER receive cells from SQCull directly, they are ONLY visible through portals! (see above)
			} else if (pVisNode->IsContainer()) {
				// Draw this container without special processing
				if (static_cast<const VisContainer*>(pVisNode)->GetCullQuery()) {
					DrawRec(cRenderer, *static_cast<const VisContainer*>(pVisNode)->GetCullQuery());

					// Set the previous scissor rectangle
					cRenderer.SetScissorRect(&cVisContainer.GetProjection().cRectangle);
				}

			// This must just be a quite boring scene node :)
			} else {
				// Do only take "PLVolume::SNVolume"-instances into account in here
				const static Class *pSNVolumeClass = ClassManager::GetInstance()->GetClass("PLVolume::SNVolume");
				if (pSceneNode->GetClass() == pSNVolumeClass)
					DrawVolumeSceneNode(cRenderer, cCullQuery, *pVisNode);
			}
		}
	}
}

/**
*  @brief
*    Returns a list of all visible scene node instances intersecting with the given scene node, recursive part
*/
void SRPVolume::GetIntersectingInstancesOfRec(const SQCull &cCullQuery, SceneNode &cSceneNode, const Class &cClass, Array<const VisNode*> &lstIntersecting) const
{
	// Get visibility container
	const VisContainer &cVisContainer = cCullQuery.GetVisContainer();

	// Search through all visible scene nodes of this scene container
	Iterator<VisNode*> cIterator = cVisContainer.GetVisNodes().GetIterator();
	while (cIterator.HasNext()) {
		// Get visibility node and scene node
		const VisNode   *pVisNode   = cIterator.Next();
			  SceneNode *pSceneNode = pVisNode->GetSceneNode();
		if (pSceneNode) {
			// Is this scene node a portal?
			if (pVisNode->IsPortal()) {
				// Get the target cell visibility container
				const VisContainer *pVisCell = static_cast<const VisPortal*>(pVisNode)->GetTargetVisContainer();
				if (pVisCell && pVisCell->GetCullQuery()) {
					// Search within the target cell
					GetIntersectingInstancesOfRec(*pVisCell->GetCullQuery(), cSceneNode, cClass, lstIntersecting);
				}

			// Is this scene node a container? We do not need to check for cells because we will
			// NEVER receive cells from SQCull directly, they are ONLY visible through portals! (see above)
			} else if (pVisNode->IsContainer()) {
				// Search within this container without special processing
				if (static_cast<const VisContainer*>(pVisNode)->GetCullQuery())
					GetIntersectingInstancesOfRec(*static_cast<const VisContainer*>(pVisNode)->GetCullQuery(), cSceneNode, cClass, lstIntersecting);

			// This must just be a quite boring scene node :)
			} else {
				// Is this a directional light scene node?
				if (pSceneNode->IsInstanceOfByReference(cClass)) {
					// [TODO] Intersection test
					lstIntersecting.Add(pVisNode);
				}
			}
		}
	}
}

/**
*  @brief
*    Searches for the first (= nearest) visible clip plane scene node, recursive part
*/
const VisNode *SRPVolume::GetFirstClipPlaneRec(const SQCull &cCullQuery) const
{
	// Get visibility container
	const VisContainer &cVisContainer = cCullQuery.GetVisContainer();

	// Search through all visible scene nodes of this scene container
	Iterator<VisNode*> cIterator = cVisContainer.GetVisNodes().GetIterator();
	while (cIterator.HasNext()) {
		// Get visibility node and scene node
		const VisNode   *pVisNode   = cIterator.Next();
			  SceneNode *pSceneNode = pVisNode->GetSceneNode();
		if (pSceneNode) {
			// Is this scene node a portal?
			if (pVisNode->IsPortal()) {
				// Get the target cell visibility container
				const VisContainer *pVisCell = static_cast<const VisPortal*>(pVisNode)->GetTargetVisContainer();
				if (pVisCell && pVisCell->GetCullQuery()) {
					// Search within the target cell
					const VisNode *pFoundVisNode = GetFirstClipPlaneRec(*pVisCell->GetCullQuery());
					if (pFoundVisNode)
						return pFoundVisNode;
				}

			// Is this scene node a container? We do not need to check for cells because we will
			// NEVER receive cells from SQCull directly, they are ONLY visible through portals! (see above)
			} else if (pVisNode->IsContainer()) {
				// Search within this container without special processing
				if (static_cast<const VisContainer*>(pVisNode)->GetCullQuery()) {
					const VisNode *pFoundVisNode = GetFirstClipPlaneRec(*static_cast<const VisContainer*>(pVisNode)->GetCullQuery());
					if (pFoundVisNode)
						return pFoundVisNode;
				}

			// This must just be a quite boring scene node :)
			} else {
				// Is this a clip plane scene node?
				if (pSceneNode->IsInstanceOf("PLVolume::SNClipPrimitivePlane"))
					return pVisNode;
			}
		}
	}

	// Sorry, no result :/
	return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume
