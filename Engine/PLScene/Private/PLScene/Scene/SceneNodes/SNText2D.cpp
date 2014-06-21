/*********************************************************\
 *  File: SNText2D.cpp                                   *
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
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Renderer/FontManager.h>
#include <PLRenderer/Effect/EffectManager.h>
#include "PLScene/Visibility/VisNode.h"
#include "PLScene/Scene/SceneNodes/SNText2D.h"


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
pl_implement_class(SNText2D)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNText2D::SNText2D() :
	Flags(this)
{
	// Set draw function flags
	SetDrawFunctionFlags(static_cast<uint8>(GetDrawFunctionFlags() | UseDrawPost));
}

/**
*  @brief
*    Destructor
*/
SNText2D::~SNText2D()
{
}


//[-------------------------------------------------------]
//[ Public virtual SceneNode functions                    ]
//[-------------------------------------------------------]
void SNText2D::DrawPost(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Call base implementation
	SNText::DrawPost(cRenderer, pVisNode);

	// Is there any text to draw?
	if (((GetFlags() & No3DPosition) || pVisNode) && Text.Get().GetLength()) {
		// Get the font
		const PLRenderer::Font *pDefaultFont = cRenderer.GetFontManager().GetDefaultFontTexture();
		const String			sFont        = (GetFont().GetLength() || !pDefaultFont) ? GetFont() : pDefaultFont->GetFilename();
		const uint32			nFontSize    = (FontSize || !pDefaultFont) ? FontSize : pDefaultFont->GetSize();
			  PLRenderer::Font *pFont        = reinterpret_cast<PLRenderer::Font*>(cRenderer.GetFontManager().GetFontTexture(sFont, nFontSize));
		if (pFont) {
			// Get the full dynamic scale
			const Vector3 &vScale = GetTransform().GetScale();
			const float fScale = vScale.x*vScale.y*vScale.z;

			// Setup render states
			cRenderer.GetRendererContext().GetEffectManager().Use();
			cRenderer.SetRenderState(RenderState::ZEnable,      false);
			cRenderer.SetRenderState(RenderState::ZWriteEnable, false);

			// Get font draw flags
			uint32 nFlags = 0;
			if (!(GetFlags() & NoCenter))
				nFlags |= Font::CenterText;
			// [TODO] Update SNText flags
			// UnderlineText = 1<<1,	/**< Underline the text */
			// CrossoutText  = 1<<2	/**< Cross out the text */

			// 3D position or screen space position?
			if (GetFlags() & No3DPosition) {
				// Begin 2D mode
				DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
				cDrawHelpers.Begin2DMode();

					// Draw the text
					cDrawHelpers.DrawText(*pFont, Text.Get(), Color.Get(), GetTransform().GetPosition(), nFlags, Vector2(fScale, fScale));

				// End 2D mode
				cDrawHelpers.End2DMode();
			} else {
				// Draw the text
				cRenderer.GetDrawHelpers().DrawText(*pFont, Text.Get(), Color.Get(), Vector3::Zero, pVisNode->GetWorldViewProjectionMatrix(), (GetFlags() & NoCenter) ? 0 : Font::CenterText, Vector2(fScale, fScale));
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
