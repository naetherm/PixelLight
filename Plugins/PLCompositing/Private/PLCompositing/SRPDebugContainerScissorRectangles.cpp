/*********************************************************\
 *  File: SRPDebugContainerScissorRectangles.cpp         *
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
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLScene/Visibility/SQCull.h>
#include <PLScene/Visibility/VisPortal.h>
#include <PLScene/Visibility/VisContainer.h>
#include "PLCompositing/SRPDebugContainerScissorRectangles.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLScene;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SRPDebugContainerScissorRectangles)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPDebugContainerScissorRectangles::SRPDebugContainerScissorRectangles() :
	LineWidth(this),
	LineColor(this)
{
}

/**
*  @brief
*    Destructor
*/
SRPDebugContainerScissorRectangles::~SRPDebugContainerScissorRectangles()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Draws recursive
*/
void SRPDebugContainerScissorRectangles::DrawRec(Renderer &cRenderer, const SQCull &cCullQuery) const
{
	// Draw the scissor rectangle
	const VisContainer &cVisContainer = cCullQuery.GetVisContainer();
	const Rectangle    &cRect         = cVisContainer.GetProjection().cRectangle;
	cRenderer.GetDrawHelpers().DrawQuad(LineColor.Get(),
										Vector3(cRect.vMin.x, cRect.vMin.y, 0.0f),
										Vector3(cRect.vMax.x, cRect.vMin.y, 0.0f),
										Vector3(cRect.vMax.x, cRect.vMax.y, 0.0f),
										Vector3(cRect.vMin.x, cRect.vMax.y, 0.0f),
										Matrix4x4::Identity,
										LineWidth.Get());

	// Recursive
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
					// Draw the target cell
					DrawRec(cRenderer, *pVisCell->GetCullQuery());
				}

			// Is this scene node a container? We do not need to check for cells because we will
			// NEVER receive cells from SQCull directly, they are ONLY visible through portals! (see above)
			} else if (pVisNode->IsContainer()) {
				// Draw this container without special processing
				if (static_cast<const VisContainer*>(pVisNode)->GetCullQuery())
					DrawRec(cRenderer, *static_cast<const VisContainer*>(pVisNode)->GetCullQuery());

			// This must just be a quite boring scene node :)
			} else {
				// Nothing to do here
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRPDebugContainerScissorRectangles::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Setup renderer states
	cRenderer.GetRendererContext().GetEffectManager().Use();
	cRenderer.SetRenderState(RenderState::ZEnable,			 false);
	cRenderer.SetRenderState(RenderState::ZWriteEnable,		 false);
	cRenderer.SetRenderState(RenderState::ScissorTestEnable, true);

	// Begin 2D mode
	DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
	cDrawHelpers.Begin2DMode(0.0f, 0.0f, 0.0f, 0.0f);

		// Draw recursive from front to back
		DrawRec(cRenderer, cCullQuery);

	// End 2D mode
	cDrawHelpers.End2DMode();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
