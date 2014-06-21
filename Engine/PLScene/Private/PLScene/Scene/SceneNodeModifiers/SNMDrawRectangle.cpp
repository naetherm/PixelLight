/*********************************************************\
 *  File: SNMDrawRectangle.cpp                           *
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
#include <PLMath/Matrix4x4.h>
#include <PLMath/Rectangle.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Effect/EffectManager.h>
#include "PLScene/Visibility/VisNode.h"
#include "PLScene/Scene/SceneNodeModifiers/SNMDrawRectangle.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMDrawRectangle)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMDrawRectangle::SNMDrawRectangle(SceneNode &cSceneNode) : SNMDraw(cSceneNode),
	Width(this),
	Color(this),
	Flags(this),
	SlotOnDrawTransparent(this)
{
	// Set draw function flags
	cSceneNode.SetDrawFunctionFlags(static_cast<uint8>(cSceneNode.GetDrawFunctionFlags() | SceneNode::UseDrawTransparent));
}

/**
*  @brief
*    Destructor
*/
SNMDrawRectangle::~SNMDrawRectangle()
{
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNodeModifier functions         ]
//[-------------------------------------------------------]
void SNMDrawRectangle::OnActivate(bool bActivate)
{
	// Connect/disconnect event handler
	if (bActivate)
		GetSceneNode().SignalDrawTransparent.Connect(SlotOnDrawTransparent);
	else
		GetSceneNode().SignalDrawTransparent.Disconnect(SlotOnDrawTransparent);
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called on scene node transparent draw
*/
void SNMDrawRectangle::OnDrawTransparent(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Visibility node required
	if (pVisNode) {
		// Get the vertices of the bounding box
		Array<Vector3> vVertex(8);
		GetSceneNode().GetAABoundingBox().GetVertices(vVertex.GetData());

		// Get viewport parameters
		const uint32 nX      = static_cast<uint32>(cRenderer.GetViewport().GetX());
		const uint32 nY      = static_cast<uint32>(cRenderer.GetViewport().GetY());
		const uint32 nWidth  = static_cast<uint32>(cRenderer.GetViewport().GetWidth());
		const uint32 nHeight = static_cast<uint32>(cRenderer.GetViewport().GetHeight());

		// Calculate the scissor rectangle
		Rectangle cRectangle;
		cRectangle.ScreenRectangle(vVertex, true, pVisNode->GetWorldViewProjectionMatrix(), nX, nY, nWidth, nHeight);

		// Is the rectangle visible?
		if (cRectangle.GetWidth() || cRectangle.GetHeight()) {
			// Setup render states
			cRenderer.GetRendererContext().GetEffectManager().Use();
			cRenderer.SetRenderState(RenderState::ZEnable,      GetFlags() & DepthTest);
			cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
			cRenderer.SetRenderState(RenderState::BlendEnable,  Color.Get().a != 1.0f);

			// Begin 2D mode
			DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
			cDrawHelpers.Begin2DMode(static_cast<float>(nX), static_cast<float>(nY), static_cast<float>(nWidth), static_cast<float>(nHeight));

				// Draw the rectangle
				cDrawHelpers.DrawLine(Color.Get(), Vector2(cRectangle.vMin.x, cRectangle.vMin.y), Vector2(cRectangle.vMax.x, cRectangle.vMin.y), Width);
				cDrawHelpers.DrawLine(Color.Get(), Vector2(cRectangle.vMin.x, cRectangle.vMax.y), Vector2(cRectangle.vMax.x, cRectangle.vMax.y), Width);
				cDrawHelpers.DrawLine(Color.Get(), Vector2(cRectangle.vMin.x, cRectangle.vMin.y), Vector2(cRectangle.vMin.x, cRectangle.vMax.y), Width);
				cDrawHelpers.DrawLine(Color.Get(), Vector2(cRectangle.vMax.x, cRectangle.vMin.y), Vector2(cRectangle.vMax.x, cRectangle.vMax.y), Width);

			// End 2D mode
			cDrawHelpers.End2DMode();
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
