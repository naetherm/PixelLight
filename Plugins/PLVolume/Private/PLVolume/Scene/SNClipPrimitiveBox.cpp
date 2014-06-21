/*********************************************************\
 *  File: SNClipPrimitiveBox.cpp                         *
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
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLScene/Visibility/VisNode.h>
#include "PLVolume/Scene/SNClipPrimitiveBox.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLMath;
using namespace PLRenderer;
using namespace PLScene;
namespace PLVolume {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNClipPrimitiveBox)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNClipPrimitiveBox::SNClipPrimitiveBox() :
	AABBMin(this),
	AABBMax(this)
{
	// Overwritten PLScene::SceneNode variables
	SetAABoundingBox(AABoundingBox(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f)));
}

/**
*  @brief
*    Destructor
*/
SNClipPrimitiveBox::~SNClipPrimitiveBox()
{
}


//[-------------------------------------------------------]
//[ Public virtual PLScene::SceneNode functions           ]
//[-------------------------------------------------------]
void SNClipPrimitiveBox::DrawDebug(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Call base implementation
	SNClipPrimitive::DrawDebug(cRenderer, pVisNode);

	// Draw the box visualization?
	if (pVisNode && !(GetDebugFlags() & DebugNoVisualization)) {
		// Get the 'scene node space' bounding box?
		const AABoundingBox &cAABB = GetAABoundingBox();

		// Setup render states
		// -> Lookout! Enable depth buffer else we will see both transparent box sides which is quite confusing
		cRenderer.GetRendererContext().GetEffectManager().Use();
		cRenderer.SetRenderState(RenderState::ZEnable,      true);
		cRenderer.SetRenderState(RenderState::ZWriteEnable, true);
		cRenderer.SetRenderState(RenderState::BlendEnable,  true);

		// Invert clipping?
		const bool bInvertClipping = (GetFlags() & InvertClipping) != 0;

		// View side allowing to see the clipped part *camera outside the clipping primitive*
		// -> Negative
		cRenderer.SetRenderState(RenderState::CullMode, Cull::CCW);
		cRenderer.GetDrawHelpers().DrawBox(bInvertClipping ? DebugColorPositive.Get() : DebugColorNegative.Get(), cAABB.vMin, cAABB.vMax, pVisNode->GetWorldViewProjectionMatrix());

		// View side allowing to see the non-clipped rest *camera inside the clipping primitive*
		// -> Positive
		cRenderer.SetRenderState(RenderState::CullMode, Cull::CW);
		cRenderer.GetDrawHelpers().DrawBox(bInvertClipping ? DebugColorNegative.Get() : DebugColorPositive.Get(), cAABB.vMin, cAABB.vMax, pVisNode->GetWorldViewProjectionMatrix());
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume
