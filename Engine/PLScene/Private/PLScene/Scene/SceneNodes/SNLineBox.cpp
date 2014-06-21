/*********************************************************\
 *  File: SNLineBox.cpp                                  *
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
#include "PLScene/Scene/SceneNodes/SNLineBox.h"


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
pl_implement_class(SNLineBox)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
const Vector3 &SNLineBox::GetStartPosition() const
{
	return m_vStartPosition;
}

void SNLineBox::SetStartPosition(const Vector3 &vValue)
{
	if (m_vStartPosition != vValue) {
		m_vStartPosition = vValue;

		// We have to recalculate the current axis align bounding box in 'scene node space'
		DirtyAABoundingBox();
	}
}

const Vector3 &SNLineBox::GetEndPosition() const
{
	return m_vEndPosition;
}

void SNLineBox::SetEndPosition(const Vector3 &vValue)
{
	if (m_vEndPosition != vValue) {
		m_vEndPosition = vValue;

		// We have to recalculate the current axis align bounding box in 'scene node space'
		DirtyAABoundingBox();
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNLineBox::SNLineBox() :
	StartPosition(this),
	EndPosition(this),
	Width(this),
	Color(this),
	Flags(this),
	m_vEndPosition(1.0f, 1.0f, 1.0f)
{
	// Set draw function flags
	SetDrawFunctionFlags(static_cast<uint8>(GetDrawFunctionFlags() | UseDrawSolid | UseDrawTransparent));

	// We have to recalculate the current axis align bounding box in 'scene node space'
	DirtyAABoundingBox();
}

/**
*  @brief
*    Destructor
*/
SNLineBox::~SNLineBox()
{
}


//[-------------------------------------------------------]
//[ Public virtual SceneNode functions                    ]
//[-------------------------------------------------------]
void SNLineBox::DrawSolid(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Call base implementation
	SceneNode::DrawSolid(cRenderer, pVisNode);

	// Transparent?
	if (Color.Get().a == 1.0f) {
		// Setup render states
		cRenderer.GetRendererContext().GetEffectManager().Use();
		cRenderer.SetRenderState(RenderState::ZEnable, !(GetFlags() & NoDepthTest));

		// 3D position or screen space position?
		DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
		if (GetFlags() & No3DPosition) {
			// Begin 2D mode
			cDrawHelpers.Begin2DMode();

				// Draw the line box
				cDrawHelpers.DrawBox(Color.Get(), m_vStartPosition, m_vEndPosition, pVisNode->GetWorldViewProjectionMatrix(), Width);

			// End 2D mode
			cDrawHelpers.End2DMode();
		} else {
			// Draw the line box
			cDrawHelpers.DrawBox(Color.Get(), m_vStartPosition, m_vEndPosition, pVisNode->GetWorldViewProjectionMatrix(), Width);
		}
	}
}

void SNLineBox::DrawTransparent(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Transparent?
	if (Color.Get().a != 1.0f) {
		// Setup render states
		cRenderer.GetRendererContext().GetEffectManager().Use();
		cRenderer.SetRenderState(RenderState::ZEnable,      !(GetFlags() & NoDepthTest));
		cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
		cRenderer.SetRenderState(RenderState::BlendEnable,  true);

		// 3D position or screen space position?
		DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
		if (GetFlags() & No3DPosition) {
			// Begin 2D mode
			cDrawHelpers.Begin2DMode();

				// Draw the line box
				cDrawHelpers.DrawBox(Color.Get(), m_vStartPosition, m_vEndPosition, pVisNode->GetWorldViewProjectionMatrix(), Width);

			// End 2D mode
			cDrawHelpers.End2DMode();
		} else {
			// Draw the line box
			cDrawHelpers.DrawBox(Color.Get(), m_vStartPosition, m_vEndPosition, pVisNode->GetWorldViewProjectionMatrix(), Width);
		}
	}

	// Call base implementation
	SceneNode::DrawTransparent(cRenderer, pVisNode);
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNode functions                 ]
//[-------------------------------------------------------]
void SNLineBox::UpdateAABoundingBox()
{
	const Vector3 vPos[2] = { m_vStartPosition, m_vEndPosition };
	Vector3 vMin = vPos[0];
	Vector3 vMax = vPos[0];
	for (int i=0; i<2; i++) {
		for (int j=0; j<3; j++) {
			const Vector3 &vT = vPos[i];
			if (vMin[j] > vT[j])
				vMin[j] = vT[j];
			if (vMax[j] < vT[j])
				vMax[j] = vT[j];
		}
	}
	SetAABoundingBox(AABoundingBox(vMin, vMax));
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
