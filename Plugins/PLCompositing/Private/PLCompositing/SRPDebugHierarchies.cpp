/*********************************************************\
 *  File: SRPDebugHierarchies.cpp                        *
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
#include <PLCore/Tools/Tools.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLScene/Scene/SceneHierarchy.h>
#include <PLScene/Visibility/SQCull.h>
#include <PLScene/Visibility/VisPortal.h>
#include <PLScene/Visibility/VisContainer.h>
#include "PLCompositing/SRPDebugHierarchies.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLScene;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SRPDebugHierarchies)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPDebugHierarchies::SRPDebugHierarchies() :
	LineWidth(this),
	LineColor(this)
{
}

/**
*  @brief
*    Destructor
*/
SRPDebugHierarchies::~SRPDebugHierarchies()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Draws recursive
*/
void SRPDebugHierarchies::DrawRec(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Get scene container
	const VisContainer &cVisContainer = cCullQuery.GetVisContainer();

	// Set the new scissor rectangle
	cRenderer.SetScissorRect(&cVisContainer.GetProjection().cRectangle);

	{ // Render all visible scene nodes of this scene container
		Iterator<VisNode*> cIterator = cVisContainer.GetVisNodes().GetEndIterator();
		while (cIterator.HasPrevious()) {
			// Get visibility node and scene node
			const VisNode   *pVisNode   = cIterator.Previous();
				  SceneNode *pSceneNode = pVisNode->GetSceneNode();
			if (pSceneNode) {
				// Is this scene node a portal?
				if (pVisNode->IsPortal()) {
					// Get the target cell visibility container
					const VisContainer *pVisCell = static_cast<const VisPortal*>(pVisNode)->GetTargetVisContainer();
					if (pVisCell) {
						// Draw the target cell
						if (pVisCell->GetCullQuery())
							DrawRec(cRenderer, *pVisCell->GetCullQuery());

						// Set the previous scissor rectangle
						cRenderer.SetScissorRect(&cVisContainer.GetProjection().cRectangle);
					}

				// Is this scene node a container? We do not need to check for cells because we will
				// NEVER receive cells from SQCull directly, they are ONLY visible through portals! (see above)
				} else if (pVisNode->IsContainer()) {
					// Draw this container without special processing
					if (static_cast<const VisContainer*>(pVisNode)->GetCullQuery())
						DrawRec(cRenderer, *static_cast<const VisContainer*>(pVisNode)->GetCullQuery());

					// Set the previous scissor rectangle
					cRenderer.SetScissorRect(&cVisContainer.GetProjection().cRectangle);

				// This must just be a quite boring scene node :)
				} else {
					// Ignore it :)
				}
			}
		}
	}

	// Draw the hierarchy of the scene container
	cCullQuery.GetSceneContainer().GetHierarchyInstance()->Draw(cRenderer, LineColor.Get(), cVisContainer.GetWorldViewProjectionMatrix(), LineWidth.Get());
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRPDebugHierarchies::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Reset all render states to default
	cRenderer.GetRendererContext().GetEffectManager().Use();
	cRenderer.SetRenderState(RenderState::ZEnable,			 false);
	cRenderer.SetRenderState(RenderState::ZWriteEnable,		 false);
	cRenderer.SetRenderState(RenderState::BlendEnable,		 (LineColor.Get().a < 1.0f));
	cRenderer.SetRenderState(RenderState::ScissorTestEnable, true);

	// Draw recursive from back to front
	DrawRec(cRenderer, cCullQuery);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
