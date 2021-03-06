/*********************************************************\
 *  File: SNLine.cpp                                     *
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
#include "PLScene/Scene/SceneNodes/SNLine.h"


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
pl_class_metadata(SNLine, "PLScene", PLScene::SceneNode, "Line scene node")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor, "Default constructor", "")
	// Attributes
	pl_attribute_metadata(StartPosition,	PLMath::Vector3,					PLMath::Vector3::Zero,		ReadWrite,	"Line start position (relative to this node)",				"")
	pl_attribute_metadata(EndPosition,		PLMath::Vector3,					PLMath::Vector3::UnitZ,		ReadWrite,	"Line end position (relative to this node)",				"")
	pl_attribute_metadata(Width,			float,								1.0f,						ReadWrite,	"Line width (if supported by the the used renderer API)",	"Min='1.0'")
	pl_attribute_metadata(Color,			PLGraphics::Color4,					PLGraphics::Color4::White,	ReadWrite,	"Line color (r/g/b/a)",										"")
		// Overwritten SceneNode attributes
	pl_attribute_metadata(Flags,			pl_flag_type_def3(SNLine, EFlags),	0,							ReadWrite,	"Flags",													"")
pl_class_metadata_end(SNLine)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
const Vector3 &SNLine::GetStartPosition() const
{
	return m_vStartPosition;
}

void SNLine::SetStartPosition(const Vector3 &vValue)
{
	if (m_vStartPosition != vValue) {
		m_vStartPosition = vValue;

		// We have to recalculate the current axis align bounding box in 'scene node space'
		DirtyAABoundingBox();
	}
}

const Vector3 &SNLine::GetEndPosition() const
{
	return m_vEndPosition;
}

void SNLine::SetEndPosition(const Vector3 &vValue)
{
	if (m_vEndPosition != vValue) {
		m_vEndPosition = vValue;

		// We have to recalculate the current axis align bounding box in 'scene node space'
		DirtyAABoundingBox();
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNLine::SNLine() :
	StartPosition(this),
	EndPosition(this),
	Width(this),
	Color(this),
	Flags(this),
	m_vEndPosition(0.0f, 0.0f, 1.0f)
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
SNLine::~SNLine()
{
}


//[-------------------------------------------------------]
//[ Public virtual SceneNode functions                    ]
//[-------------------------------------------------------]
void SNLine::DrawSolid(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Call base implementation
	SceneNode::DrawSolid(cRenderer, pVisNode);

	// Transparent?
	if (Color.Get().a == 1.0f) {
		// Draw the line
		cRenderer.GetRendererContext().GetEffectManager().Use();
		cRenderer.SetRenderState(RenderState::ZEnable, !(GetFlags() & NoDepthTest));

		// 3D position or screen space position?
		DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
		if (GetFlags() & No3DPosition) {
			// Begin 2D mode
			cDrawHelpers.Begin2DMode();

				// Draw the line
				cDrawHelpers.DrawLine(Color.Get(), m_vStartPosition, m_vEndPosition, Width);

			// End 2D mode
			cDrawHelpers.End2DMode();
		} else {
			// Draw the line
			cDrawHelpers.DrawLine(Color.Get(), m_vStartPosition, m_vEndPosition, pVisNode->GetWorldViewProjectionMatrix(), Width);
		}
	}
}

void SNLine::DrawTransparent(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Transparent?
	if (Color.Get().a != 1.0f) {
		// Draw the line
		cRenderer.GetRendererContext().GetEffectManager().Use();
		cRenderer.SetRenderState(RenderState::ZEnable,      !(GetFlags() & NoDepthTest));
		cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
		cRenderer.SetRenderState(RenderState::BlendEnable,  true);

		// 3D position or screen space position?
		DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
		if (GetFlags() & No3DPosition) {
			// Begin 2D mode
			cDrawHelpers.Begin2DMode();

				// Draw the line
				cDrawHelpers.DrawLine(Color.Get(), m_vStartPosition, m_vEndPosition, Width);

			// End 2D mode
			cDrawHelpers.End2DMode();
		} else {
			// Draw the line
			cDrawHelpers.DrawLine(Color.Get(), m_vStartPosition, m_vEndPosition, pVisNode->GetWorldViewProjectionMatrix(), Width);
		}
	}

	// Call base implementation
	SceneNode::DrawTransparent(cRenderer, pVisNode);
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNode functions                 ]
//[-------------------------------------------------------]
void SNLine::UpdateAABoundingBox()
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
