/*********************************************************\
 *  File: SNLineGrid.cpp                                 *
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
#include "PLScene/Scene/SceneNodes/SNLineGrid.h"


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
pl_implement_class(SNLineGrid)


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNLineGrid::SNLineGrid() :
	NumOfXLines(this),
	NumOfYLines(this),
	MainWidth(this),
	MainColor(this),
	MinorWidth(this),
	MinorColor(this),
	Flags(this),
	AABBMin(this),
	AABBMax(this)
{
	// Overwritten SceneNode variables
	SetAABoundingBox(AABoundingBox(Vector3(-0.5f, -0.5f, 0.0f), Vector3(0.5f, 0.5f, 0.0f)));

	// Set draw function flags
	SetDrawFunctionFlags(static_cast<uint8>(GetDrawFunctionFlags() | UseDrawSolid | UseDrawTransparent));
}

/**
*  @brief
*    Destructor
*/
SNLineGrid::~SNLineGrid()
{
}

/**
*  @brief
*    Draws the grid
*/
void SNLineGrid::DrawGrid(Renderer &cRenderer, const VisNode &cVisNode)
{
	// Anything to do in here?
	if (!(GetFlags() & NoMainLines) || !(GetFlags() & NoMinorLines)) {
		DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();

		// Setup render states
		cRenderer.GetRendererContext().GetEffectManager().Use();
		if (GetFlags() & NoDepthTest) {
			cRenderer.SetRenderState(RenderState::ZEnable,      false);
			cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
		}

		// Draw the main grid lines
		if (!(GetFlags() & NoMainLines)) {
			// Setup render states
			cRenderer.SetRenderState(RenderState::BlendEnable, MainColor.Get().a != 1.0f);

			// Draw the lines
			cDrawHelpers.DrawLine(MainColor.Get(), Vector3(-0.5f, 0.0f, 0.0f), Vector3(0.5f, 0.0f, 0.0f), cVisNode.GetWorldViewProjectionMatrix(), MainWidth);
			cDrawHelpers.DrawLine(MainColor.Get(), Vector3(0.0f, -0.5f, 0.0f), Vector3(0.0f, 0.5f, 0.0f), cVisNode.GetWorldViewProjectionMatrix(), MainWidth);
		}

		// Draw the minor grid lines
		if (!(GetFlags() & NoMinorLines)) {
			// Setup render states
			cRenderer.SetRenderState(RenderState::BlendEnable, MinorColor.Get().a != 1.0f);

			// Draw the main grid lines
			if (GetFlags() & NoMainLines) {
				// Draw the lines
				cDrawHelpers.DrawLine(MinorColor.Get(), Vector3(-0.5f, 0.0f, 0.0f), Vector3(0.5f, 0.0f, 0.0f), cVisNode.GetWorldViewProjectionMatrix(), MinorWidth);
				cDrawHelpers.DrawLine(MinorColor.Get(), Vector3(0.0f, -0.5f, 0.0f), Vector3(0.0f, 0.5f, 0.0f), cVisNode.GetWorldViewProjectionMatrix(), MinorWidth);
			}

			// Draw the lines along the x axis
			float fDelta = 0.5f/NumOfXLines;
			for (uint32 i=1; i<=NumOfXLines; i++) {
				const float fPosition = static_cast<float>(i*fDelta);
				// Positive axis
				cDrawHelpers.DrawLine(MinorColor.Get(), Vector3(fPosition, -0.5f, 0.0f), Vector3(fPosition, 0.5f, 0.0f), cVisNode.GetWorldViewProjectionMatrix(), MinorWidth);
				// Negative axis
				cDrawHelpers.DrawLine(MinorColor.Get(), Vector3(-fPosition, -0.5f, 0.0f), Vector3(-fPosition, 0.5f, 0.0f), cVisNode.GetWorldViewProjectionMatrix(), MinorWidth);
			}

			// Draw the lines along the y axis
			fDelta = 0.5f/NumOfYLines;
			for (uint32 i=1; i<=NumOfYLines; i++) {
				const float fPosition = static_cast<float>(i*fDelta);
				// Positive axis
				cDrawHelpers.DrawLine(MinorColor.Get(), Vector3(-0.5f, fPosition, 0.0f), Vector3(0.5f, fPosition, 0.0f), cVisNode.GetWorldViewProjectionMatrix(), MinorWidth);
				// Negative axis
				cDrawHelpers.DrawLine(MinorColor.Get(), Vector3(-0.5f, -fPosition, 0.0f), Vector3(0.5f, -fPosition, 0.0f), cVisNode.GetWorldViewProjectionMatrix(), MinorWidth);
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual SceneNode functions                    ]
//[-------------------------------------------------------]
void SNLineGrid::DrawSolid(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Call base implementation
	SceneNode::DrawSolid(cRenderer, pVisNode);

	// Perform depth test?
	if (!(GetFlags() & NoDepthTest)) {
		// Draw the grid
		if (pVisNode)
			DrawGrid(cRenderer, *pVisNode);
	}
}

void SNLineGrid::DrawTransparent(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Perform depth test?
	if (GetFlags() & NoDepthTest) {
		// Draw the grid
		if (pVisNode)
			DrawGrid(cRenderer, *pVisNode);
	}

	// Call base implementation
	SceneNode::DrawTransparent(cRenderer, pVisNode);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
