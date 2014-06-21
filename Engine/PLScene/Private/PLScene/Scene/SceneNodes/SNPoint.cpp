/*********************************************************\
 *  File: SNPoint.cpp                                    *
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
#include "PLScene/Visibility/VisNode.h"
#include "PLScene/Scene/SceneNodes/SNPoint.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNPoint)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNPoint::SNPoint() :
	Size(this),
	Color(this),
	Flags(this)
{
	// Set draw function flags
	SetDrawFunctionFlags(static_cast<uint8>(GetDrawFunctionFlags() | UseDrawSolid | UseDrawTransparent));
}

/**
*  @brief
*    Destructor
*/
SNPoint::~SNPoint()
{
}


//[-------------------------------------------------------]
//[ Public virtual SceneNode functions                    ]
//[-------------------------------------------------------]
void SNPoint::DrawSolid(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Call base implementation
	SceneNode::DrawSolid(cRenderer, pVisNode);

	// Perform depth test?
	if (!(GetFlags() & NoDepthTest)) {
		// Draw the point
		cRenderer.GetRendererContext().GetEffectManager().Use();
		cRenderer.SetRenderState(RenderState::BlendEnable, Color.Get().a != 1.0f);

		// 3D position or screen space position?
		DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
		if (GetFlags() & No3DPosition) {
			// Begin 2D mode
			cDrawHelpers.Begin2DMode();

				// Draw the point
				cDrawHelpers.DrawPoint(Color.Get(), GetTransform().GetPosition(), Size);

			// End 2D mode
			cDrawHelpers.End2DMode();
		} else {
			// Draw the point
			cDrawHelpers.DrawPoint(Color.Get(), Vector3::Zero, pVisNode->GetWorldViewProjectionMatrix(), Size);
		}
	}
}

void SNPoint::DrawTransparent(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Perform depth test?
	if (GetFlags() & NoDepthTest) {
		// Draw the point
		cRenderer.GetRendererContext().GetEffectManager().Use();
		cRenderer.SetRenderState(RenderState::ZEnable,      false);
		cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
		cRenderer.SetRenderState(RenderState::BlendEnable,  Color.Get().a != 1.0f);

		// 3D position or screen space position?
		DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
		if (GetFlags() & No3DPosition) {
			// Begin 2D mode
			cDrawHelpers.Begin2DMode();

				// Draw the point
				cDrawHelpers.DrawPoint(Color.Get(), GetTransform().GetPosition(), Size);

			// End 2D mode
			cDrawHelpers.End2DMode();
		} else {
			// Draw the point
			cDrawHelpers.DrawPoint(Color.Get(), Vector3::Zero, pVisNode->GetWorldViewProjectionMatrix(), Size);
		}
	}

	// Call base implementation
	SceneNode::DrawTransparent(cRenderer, pVisNode);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
