/*********************************************************\
 *  File: SPDefault.cpp                                  *
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
#include <PLCore/Tools/Timing.h>
#include <PLCore/Tools/Localization.h>
#include "PLRenderer/RendererContext.h"
#include "PLRenderer/Renderer/Font.h"
#include "PLRenderer/Renderer/Renderer.h"
#include "PLRenderer/Renderer/DrawHelpers.h"
#include "PLRenderer/Renderer/FontManager.h"
#include "PLRenderer/Renderer/SamplerStates.h"
#include "PLRenderer/Effect/EffectManager.h"
#include "PLRenderer/Material/MaterialManager.h"
#include "PLRenderer/SPDefault.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SPDefault)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SPDefault::SPDefault(Renderer &cRenderer) : SurfacePainter(cRenderer),
	m_fRotation(0.0f)
{
}

/**
*  @brief
*    Destructor
*/
SPDefault::~SPDefault()
{
}


//[-------------------------------------------------------]
//[ Private virtual SurfacePainter functions              ]
//[-------------------------------------------------------]
void SPDefault::OnPaint(Surface &cSurface)
{
	// Get the used renderer
	Renderer &cRenderer = GetRenderer();

	// Clear the content of the current used render target (color to black and depth to 1.0 by default)
	cRenderer.Clear();

	// Begin 2D mode
	SamplerStates cSamplerStates;
	DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
	cDrawHelpers.Begin2DMode();

		// PixelLight logo
		Material *pMaterial = m_cLogoMaterial.GetResource();
		if (!pMaterial) {
			pMaterial = cRenderer.GetRendererContext().GetMaterialManager().LoadResource("Data/Textures/PLLogo.dds");
			if (pMaterial)
				m_cLogoMaterial.SetResource(pMaterial);
		}
		if (pMaterial) {
			TextureBuffer *pDiffuseMap = pMaterial->GetParameterTextureBuffer(Material::DiffuseMap);
			if (pDiffuseMap) {
				cRenderer.SetRenderState(RenderState::CullMode, Cull::None);
				cDrawHelpers.DrawImage(*pDiffuseMap, cSamplerStates, Vector2(0.4f + Math::Sin(m_fRotation)*0.4f, 0.4f + Math::Cos(m_fRotation*0.3f)*0.4f), Vector2(0.2f, 0.2f), Color4::White);
			}
		}

		// Draw text
		Font *pFont = cRenderer.GetFontManager().GetDefaultFontTexture();
		if (pFont)
			cDrawHelpers.DrawText(*pFont, PLT("Using surface painter PLRenderer::SPDefault, see PLRenderer::RendererApplication on how to use your own painter"), Color4::White, Vector2(0.5f, 0.9f), Font::CenterText);

	// End 2D mode
	cDrawHelpers.End2DMode();

	// Increase the rotation by the current time difference (time past since the last frame)
	// -> Normally, such work should NOT be performed within the rendering step, but we want
	//    to keep the implementation simple in here...
	m_fRotation += Timing::GetInstance()->GetTimeDifference()*0.5f;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
