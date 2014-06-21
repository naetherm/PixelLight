/*********************************************************\
 *  File: SRPEnd.cpp                                     *
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
#include <PLRenderer/Renderer/Surface.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Renderer/SamplerStates.h>
#include <PLRenderer/Effect/EffectManager.h>
#include "PLCompositing/SRPBegin.h"
#include "PLCompositing/SRPEnd.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLScene;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SRPEnd)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPEnd::SRPEnd()
{
}

/**
*  @brief
*    Destructor
*/
SRPEnd::~SRPEnd()
{
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRPEnd::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Get the "PLCompositing::SRPBegin" instance
	SRPBegin *pSRPBegin = static_cast<SRPBegin*>(GetFirstInstanceOfSceneRendererPassClass("PLCompositing::SRPBegin"));
	if (pSRPBegin) {
		// Get the original set render target
		Surface *pSurface = pSRPBegin->GetOriginalRenderTarget();
		if (pSurface) {
			// Is this already the current render target?
			if (pSurface != cRenderer.GetRenderTarget()) {
				// Get the back render target of SRPBegin, this holds the current content
				SurfaceTextureBuffer *pSurfaceTextureBuffer = pSRPBegin->GetBackRenderTarget();
				if (pSurfaceTextureBuffer) {
					// Set the current render target back to the original one
					cRenderer.SetRenderTarget(pSurface);

					// Setup renderer
					const uint32 nFixedFillModeBackup = cRenderer.GetRenderState(RenderState::FixedFillMode);
					cRenderer.GetRendererContext().GetEffectManager().Use();
					cRenderer.SetRenderState(RenderState::FixedFillMode, Fill::Solid);
					cRenderer.SetRenderState(RenderState::CullMode,     Cull::None);
					cRenderer.SetRenderState(RenderState::ZEnable,      false);
					cRenderer.SetRenderState(RenderState::ZWriteEnable, false);

					// Begin 2D mode
					DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
					cDrawHelpers.Begin2DMode();

						// Draw the result without filtering (we have to flip the result upside-down...)
						SamplerStates cSamplerStates;
						cSamplerStates.Set(Sampler::MagFilter, TextureFiltering::None);
						cSamplerStates.Set(Sampler::MinFilter, TextureFiltering::None);
						cSamplerStates.Set(Sampler::MipFilter, TextureFiltering::None);
						cDrawHelpers.DrawImage(*pSurfaceTextureBuffer->GetTextureBuffer(), cSamplerStates, Vector2::Zero, Vector2::One, Color4::White, 1.0f, Vector2(0.0f, 1.0f), Vector2(1.0f, -1.0f));

					// End 2D mode
					cDrawHelpers.End2DMode();

					// Restore fixed fill mode render state
					cRenderer.SetRenderState(RenderState::FixedFillMode, nFixedFillModeBackup);
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
