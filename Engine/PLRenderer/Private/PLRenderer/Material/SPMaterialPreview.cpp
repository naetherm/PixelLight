/*********************************************************\
 *  File: SPMaterialPreview.cpp                          *
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
#include "PLRenderer/RendererContext.h"
#include "PLRenderer/Renderer/DrawHelpers.h"
#include "PLRenderer/Renderer/SamplerStates.h"
#include "PLRenderer/Material/MaterialManager.h"
#include "PLRenderer/Material/SPMaterialPreview.h"


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
pl_implement_class(SPMaterialPreview)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SPMaterialPreview::SPMaterialPreview(Renderer &cRenderer) : SPPreview(cRenderer)
{
}

/**
*  @brief
*    Destructor
*/
SPMaterialPreview::~SPMaterialPreview()
{
}


//[-------------------------------------------------------]
//[ Private virtual SurfacePainter functions              ]
//[-------------------------------------------------------]
void SPMaterialPreview::OnPaint(Surface &cSurface)
{
	// Get the used renderer
	Renderer &cRenderer = GetRenderer();

	// Set all render states to known default settings
	cRenderer.Reset();

	// Clear the frame buffer
	cRenderer.Clear();

	// Get the used material
	Material *pMaterial = cRenderer.GetRendererContext().GetMaterialManager().GetByName(m_sMaterial);
	if (pMaterial) {
		// Begin 2D mode
		DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
		cDrawHelpers.Begin2DMode();

			// Draw image (we have to flip the result upside-down...)
			SamplerStates cSamplerStates;
			for (uint32 i=0; i<pMaterial->GetNumOfPasses(); i++) {
				pMaterial->SetupPass(i);
				TextureBuffer *pTextureBuffer = cRenderer.GetTextureBuffer(0);
				if (pTextureBuffer) {
					cRenderer.SetRenderState(RenderState::CullMode, Cull::None);
					cDrawHelpers.DrawImage(*pTextureBuffer, cSamplerStates, Vector2(m_vCameraPosition.x-m_vCameraPosition.z, m_vCameraPosition.y-m_vCameraPosition.z), Vector2(m_vCameraPosition.z*2, m_vCameraPosition.z*2),
										   Color4::White, 1.0f, Vector2::One, Vector2::NegativeOne);
				}
			}

		// End 2D mode
		cDrawHelpers.End2DMode();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
