/*********************************************************\
 *  File: SNCoordinateAxisRotation.cpp                   *
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
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Font.h>
#include <PLRenderer/Renderer/FontManager.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Effect/EffectManager.h>
#include "PLScene/Visibility/VisNode.h"
#include "PLScene/Scene/SNCamera.h"
#include "PLScene/Scene/SceneNodes/SNCoordinateAxisRotation.h"


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
pl_implement_class(SNCoordinateAxisRotation)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNCoordinateAxisRotation::SNCoordinateAxisRotation() :
	Flags(this)
{
	// Overwrite the default setting of the flags
	SetFlags(GetFlags()|NoCulling);

	// Set draw function flags, we ONLY want to use the post draw function!
	SetDrawFunctionFlags(static_cast<uint8>(UseDrawPost));
}

/**
*  @brief
*    Destructor
*/
SNCoordinateAxisRotation::~SNCoordinateAxisRotation()
{
}


//[-------------------------------------------------------]
//[ Public virtual SceneNode functions                    ]
//[-------------------------------------------------------]
void SNCoordinateAxisRotation::DrawPost(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Call base implementation
	SNCoordinateAxis::DrawPost(cRenderer, pVisNode);

	if (pVisNode) {
		// Get position and scale
		const Vector3 &vPosition = GetTransform().GetPosition();
		const Vector3 &vScale    = GetTransform().GetScale();

		// Get the view matrix
		Matrix4x4 mView;
		mView.SetTranslation(-4.0f, -2.5f);

		// Get the current view matrix
		const Matrix4x4 &mCurrentView = pVisNode->GetViewMatrix();

		// Get the world matrix
		Matrix4x4 mWorld = mCurrentView;
		mWorld.SetTranslation(vPosition.x, vPosition.y, -10.0f+vPosition.z);

		// Calculate the world view projection matrix
		Matrix4x4 mWorldViewProjection = pVisNode->GetProjectionMatrix();
		mWorldViewProjection *= mView;
		mWorldViewProjection *= mWorld;

		// Setup render states
		cRenderer.GetRendererContext().GetEffectManager().Use();
		if (!(GetFlags() & DepthTest)) {
			cRenderer.SetRenderState(RenderState::ZEnable,      false);
			cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
		}

		// Draw axis
		DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
		cDrawHelpers.DrawLine(XColor.Get(), Vector3::Zero, Vector3(vScale.x, 0.0f,     0.0f),     mWorldViewProjection, Width);
		cDrawHelpers.DrawLine(YColor.Get(), Vector3::Zero, Vector3(0.0f,     vScale.y, 0.0f),     mWorldViewProjection, Width);
		cDrawHelpers.DrawLine(ZColor.Get(), Vector3::Zero, Vector3(0.0f,     0.0f,     vScale.z), mWorldViewProjection, Width);

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
				cDrawHelpers.DrawText(*pFont, (vScale.x >= 0.0f) ? "X" : "-X", XColor.Get(), Vector3(vScale.x, 0.0f, 0.0f), mWorldViewProjection, Font::CenterText);
			// Y-axis
			if (!(GetFlags() & NoYText))
				cDrawHelpers.DrawText(*pFont, (vScale.y >= 0.0f) ? "Y" : "-Y", YColor.Get(), Vector3(0.0f, vScale.y, 0.0f), mWorldViewProjection, Font::CenterText);
			// Z-axis
			if (!(GetFlags() & NoZText))
				cDrawHelpers.DrawText(*pFont, (vScale.z >= 0.0f) ? "Z" : "-Z", ZColor.Get(), Vector3(0.0f, 0.0f, vScale.z), mWorldViewProjection, Font::CenterText);
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
