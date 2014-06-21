/*********************************************************\
 *  File: SNCoordinateAxis3D.cpp                         *
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
#include <PLRenderer/Renderer/Font.h>
#include <PLRenderer/Renderer/FontManager.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Effect/EffectManager.h>
#include "PLScene/Visibility/VisNode.h"
#include "PLScene/Scene/SceneNodes/SNCoordinateAxis3D.h"


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
pl_implement_class(SNCoordinateAxis3D)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNCoordinateAxis3D::SNCoordinateAxis3D() :
	AABBMax(this)
{
	// Overwritten SceneNode variables
	SetAABoundingBox(AABoundingBox(Vector3::Zero, Vector3::One));

	// Set draw function flags
	SetDrawFunctionFlags(static_cast<uint8>(GetDrawFunctionFlags() | UseDrawPost));
}

/**
*  @brief
*    Destructor
*/
SNCoordinateAxis3D::~SNCoordinateAxis3D()
{
}


//[-------------------------------------------------------]
//[ Public virtual SceneNode functions                    ]
//[-------------------------------------------------------]
void SNCoordinateAxis3D::DrawPost(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Call base implementation
	SNCoordinateAxis::DrawPost(cRenderer, pVisNode);

	// There must be a visibility node
	if (pVisNode) {
		// Setup render states
		cRenderer.GetRendererContext().GetEffectManager().Use();
		if (!(GetFlags() & DepthTest)) {
			cRenderer.SetRenderState(RenderState::ZEnable,      false);
			cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
		}

		// Draw coordinate axis at the origin of the system
		DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
		cDrawHelpers.DrawLine(XColor.Get(), Vector3::Zero, Vector3::UnitX, pVisNode->GetWorldViewProjectionMatrix(), Width);
		cDrawHelpers.DrawLine(YColor.Get(), Vector3::Zero, Vector3::UnitY, pVisNode->GetWorldViewProjectionMatrix(), Width);
		cDrawHelpers.DrawLine(ZColor.Get(), Vector3::Zero, Vector3::UnitZ, pVisNode->GetWorldViewProjectionMatrix(), Width);

		// Draw texts
		Font *pFont = nullptr;
		if (!(GetFlags() & NoXText) || !(GetFlags() & NoYText) || !(GetFlags() & NoZText)) {
			pFont = cRenderer.GetFontManager().GetDefaultFontTexture();
			if (GetFlags() & DepthTest) {
				cRenderer.SetRenderState(RenderState::ZEnable,      true);
				cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
			} else {
				cRenderer.SetRenderState(RenderState::ZEnable,      false);
				cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
			}
		}
		if (pFont) {
			// X-axis
			if (!(GetFlags() & NoXText))
				cDrawHelpers.DrawText(*pFont, (GetTransform().GetScale().x >= 0.0f) ? "X" : "-X", XColor.Get(), Vector3::UnitX, pVisNode->GetWorldViewProjectionMatrix(), Font::CenterText);
			// Y-axis
			if (!(GetFlags() & NoYText))
				cDrawHelpers.DrawText(*pFont, (GetTransform().GetScale().y >= 0.0f) ? "Y" : "-Y", YColor.Get(), Vector3::UnitY, pVisNode->GetWorldViewProjectionMatrix(), Font::CenterText);
			// Z-axis
			if (!(GetFlags() & NoZText))
				cDrawHelpers.DrawText(*pFont, (GetTransform().GetScale().z >= 0.0f) ? "Z" : "-Z", ZColor.Get(), Vector3::UnitZ, pVisNode->GetWorldViewProjectionMatrix(), Font::CenterText);
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
