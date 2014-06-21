/*********************************************************\
 *  File: SRPDebugSceneNodeNames.cpp                     *
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
#include <PLMath/Intersect.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Font.h>
#include <PLRenderer/Renderer/FontManager.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLScene/Visibility/SQCull.h>
#include <PLScene/Visibility/VisPortal.h>
#include <PLScene/Visibility/VisContainer.h>
#include "PLCompositing/SRPDebugSceneNodeNames.h"


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
pl_implement_class(SRPDebugSceneNodeNames)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPDebugSceneNodeNames::SRPDebugSceneNodeNames() :
	FontName(this),
	FontSize(this),
	FontScale(this),
	FontColor(this),
	MaxDrawDistance(this),
	Flags(this)
{
}

/**
*  @brief
*    Destructor
*/
SRPDebugSceneNodeNames::~SRPDebugSceneNodeNames()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Draws recursive
*/
void SRPDebugSceneNodeNames::DrawRec(Font &cFont, const SQCull &cCullQuery) const
{
	// Get scene container
	const SceneContainer &cSceneContainer = cCullQuery.GetSceneContainer();
	const VisContainer   &cVisContainer   = cCullQuery.GetVisContainer();

	// Draw the container scene node
	DrawName(cFont, cCullQuery, cVisContainer, cSceneContainer);

	// Render all visible scene nodes of this scene container
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
					DrawRec(cFont, *pVisCell->GetCullQuery());
				}

				// Draw the portal itself
				DrawName(cFont, cCullQuery, *pVisNode, *pSceneNode);

			// Is this scene node a container? We do not need to check for cells because we will
			// NEVER receive cells from SQCull directly, they are ONLY visible through portals! (see above)
			} else if (pVisNode->IsContainer()) {
				// Draw this container without special processing
				if (static_cast<const VisContainer*>(pVisNode)->GetCullQuery())
					DrawRec(cFont, *static_cast<const VisContainer*>(pVisNode)->GetCullQuery());

			// This must just be a quite boring scene node :)
			} else {
				DrawName(cFont, cCullQuery, *pVisNode, *pSceneNode);
			}
		}
	}
}

/**
*  @brief
*    Draws a scene node name
*/
void SRPDebugSceneNodeNames::DrawName(Font &cFont, const SQCull &cCullQuery, const VisNode &cVisNode, const SceneNode &cSceneNode) const
{
	// Check for distance limitation and do only draw the name if the scene node position is within the frustum
	if ((MaxDrawDistance <= 0.0f || cVisNode.GetSquaredDistanceToCamera() <= MaxDrawDistance*MaxDrawDistance) &&
		Intersect::PlaneSetPoint(cCullQuery.GetViewFrustum(), cSceneNode.GetTransform().GetPosition())) {
		// Draw
		cFont.GetRenderer().GetDrawHelpers().DrawText(cFont, (GetFlags() & AbsoluteNames) ? cSceneNode.GetAbsoluteName() : cSceneNode.GetName(), FontColor.Get(), Vector3::Zero, cVisNode.GetWorldViewProjectionMatrix(), Font::CenterText);
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRPDebugSceneNodeNames::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Get the font
		  Font *pDefaultFont = cRenderer.GetFontManager().GetDefaultFontTexture();
	const String sFont		 = (FontName.Get().GetLength() || !pDefaultFont) ? FontName.Get() : pDefaultFont->GetFilename();
	const uint32 nFontSize	 = (FontSize || !pDefaultFont) ? FontSize : pDefaultFont->GetSize();
		  Font *pFont		 = reinterpret_cast<Font*>(cRenderer.GetFontManager().GetFontTexture(sFont, nFontSize));
	if (pFont)  {
		// Setup render states
		cRenderer.GetRendererContext().GetEffectManager().Use();
		cRenderer.SetRenderState(RenderState::ZEnable,			 false);
		cRenderer.SetRenderState(RenderState::ZWriteEnable,		 false);
		cRenderer.SetRenderState(RenderState::BlendEnable,		 true);
		cRenderer.SetRenderState(RenderState::ScissorTestEnable, true);

		// We do not use scissor rectangles because we need to see the COMPLETE names :)
		cRenderer.SetScissorRect();

		// Draw recursive from front to back
		DrawRec(*pFont, cCullQuery);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
