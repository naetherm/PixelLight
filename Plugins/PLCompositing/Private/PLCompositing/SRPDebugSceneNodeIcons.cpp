/*********************************************************\
 *  File: SRPDebugSceneNodeIcons.cpp                     *
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
#include <PLCore/Base/Class.h>
#include <PLMath/Intersect.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Renderer/SamplerStates.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLRenderer/Texture/TextureManager.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLScene/Visibility/SQCull.h>
#include <PLScene/Visibility/VisPortal.h>
#include <PLScene/Visibility/VisContainer.h>
#include "PLCompositing/SRPDebugSceneNodeIcons.h"


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
pl_implement_class(SRPDebugSceneNodeIcons)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPDebugSceneNodeIcons::SRPDebugSceneNodeIcons() :
	Size(this),
	MaxDrawDistance(this)
{
}

/**
*  @brief
*    Destructor
*/
SRPDebugSceneNodeIcons::~SRPDebugSceneNodeIcons()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Draws recursive
*/
void SRPDebugSceneNodeIcons::DrawRec(Renderer &cRenderer, const SQCull &cCullQuery) const
{
	// Get scene container
	const SceneContainer &cSceneContainer = cCullQuery.GetSceneContainer();
	const VisContainer   &cVisContainer   = cCullQuery.GetVisContainer();

	// Set the new scissor rectangle
	cRenderer.SetScissorRect(&cVisContainer.GetProjection().cRectangle);

	// Draw the container scene node
	DrawIcon(cRenderer, cCullQuery, cVisContainer, cSceneContainer);

	// Render all visible scene nodes of this scene container
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
				if (pVisCell && pVisCell->GetCullQuery()) {
					// Draw the target cell
					DrawRec(cRenderer, *pVisCell->GetCullQuery());

					// Set the previous scissor rectangle
					cRenderer.SetScissorRect(&cVisContainer.GetProjection().cRectangle);
				}

				// Draw the portal itself
				DrawIcon(cRenderer, cCullQuery, *pVisNode, *pSceneNode);

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
				DrawIcon(cRenderer, cCullQuery, *pVisNode, *pSceneNode);
			}
		}
	}
}

/**
*  @brief
*    Draws a scene node icon
*/
void SRPDebugSceneNodeIcons::DrawIcon(Renderer &cRenderer, const SQCull &cCullQuery, const VisNode &cVisNode, const SceneNode &cSceneNode) const
{
	// Check for distance limitation and do only draw the icon if the scene node position is within the frustum
	if ((MaxDrawDistance <= 0.0f || cVisNode.GetSquaredDistanceToCamera() <= MaxDrawDistance*MaxDrawDistance) &&
		Intersect::PlaneSetPoint(cCullQuery.GetViewFrustum(), cSceneNode.GetTransform().GetPosition())) {
		// Get/load the icon texture
		static const String sIcon = "Icon";
		Texture *pTexture = cRenderer.GetRendererContext().GetTextureManager().LoadResource(cSceneNode.GetClass()->GetProperties().Get(sIcon));
		if (pTexture && pTexture->GetTextureBuffer()) {
			pTexture->SetProtected(true);

			// Calculate the 2D position
			const Vector3 vPos2D = Vector3::Zero.To2DCoordinate(cVisNode.GetWorldViewProjectionMatrix(), cRenderer.GetViewport());

			// Begin 2D mode
			DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
			cDrawHelpers.Begin2DMode(0.0f, 0.0f, 0.0f, 0.0f);

				// Draw image
				SamplerStates cSamplerStates;
				const float fHalfSize = Size/2;
				cDrawHelpers.DrawImage(*pTexture->GetTextureBuffer(), cSamplerStates, Vector2(vPos2D.x - fHalfSize, vPos2D.y - fHalfSize), Vector2(Size, Size), Color4::White, 0.5f);

			// End 2D mode
			cDrawHelpers.End2DMode();
		}
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRPDebugSceneNodeIcons::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Setup renderer states
	cRenderer.GetRendererContext().GetEffectManager().Use();
	cRenderer.SetRenderState(RenderState::CullMode,			 Cull::None);
	cRenderer.SetRenderState(RenderState::ZEnable,			 false);
	cRenderer.SetRenderState(RenderState::ZWriteEnable,		 false);
	cRenderer.SetRenderState(RenderState::ScissorTestEnable, true);

	// Draw recursive from back to front
	const uint32 nFixedFillModeBackup = cRenderer.GetRenderState(RenderState::FixedFillMode);
	cRenderer.SetRenderState(RenderState::FixedFillMode, Fill::Solid);
	DrawRec(cRenderer, cCullQuery);
	cRenderer.SetRenderState(RenderState::FixedFillMode, nFixedFillModeBackup);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
