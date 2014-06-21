/*********************************************************\
 *  File: SRPBackgroundBitmap.cpp                        *
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
#include <PLCore/File/Url.h>
#include <PLMath/Matrix4x4.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Renderer/SamplerStates.h>
#include <PLRenderer/Material/MaterialManager.h>
#include "PLCompositing/SRPBackgroundBitmap.h"


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
pl_implement_class(SRPBackgroundBitmap)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SRPBackgroundBitmap::GetMaterial() const
{
	return m_sMaterial;
}

void SRPBackgroundBitmap::SetMaterial(const String &sValue)
{
	// Material change?
	if (m_sMaterial != sValue) {
		m_sMaterial = sValue;
		m_bLoaded = false;
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPBackgroundBitmap::SRPBackgroundBitmap() :
	Material(this),
	Color(this),
	Position(this),
	Size(this),
	TexelStart(this),
	TexelEnd(this),
	Flags(this),
	m_bLoaded(false)
{
}

/**
*  @brief
*    Destructor
*/
SRPBackgroundBitmap::~SRPBackgroundBitmap()
{
}

/**
*  @brief
*    Returns the handler of the material
*/
const MaterialHandler &SRPBackgroundBitmap::GetMaterialHandler()
{
	// Load material?
	if (!m_bLoaded) {
		LoadMaterial();
		m_bLoaded = true;
	}
	return m_cMaterial;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loads the material
*/
void SRPBackgroundBitmap::LoadMaterial()
{
	// Get the renderer
	Renderer *pRenderer = GetRenderer();
	if (pRenderer) {
		// Load material
		PLRenderer::Material *pMaterial = m_sMaterial.GetLength() ? pRenderer->GetRendererContext().GetMaterialManager().LoadResource(m_sMaterial) : nullptr;
		m_cMaterial.SetResource(pMaterial);

		// Check whether this is a material or an effect, if not, we need to set a default effect
		if (pMaterial) {
			const String sExtension = Url(pMaterial->GetName()).GetExtension();
			if (sExtension != "mat" && sExtension != "plfx") {
				PLRenderer::Material *pDefaultMaterial = GetRenderer()->GetRendererContext().GetMaterialManager().LoadResource("Data/Effects/DefaultBitmap.plfx");
				if (pDefaultMaterial)
					pMaterial->SetEffect(pDefaultMaterial->GetEffect());
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRPBackgroundBitmap::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Get the material
	PLRenderer::Material *pMaterial = GetMaterialHandler().GetResource();
	if (pMaterial) {
		// Begin 2D mode
		DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
		cDrawHelpers.Begin2DMode();

			// Loop through all material passes
			SamplerStates cSamplerStates;
			for (uint32 nPass=0; nPass<pMaterial->GetNumOfPasses(); nPass++) {
				// Setup render states
				pMaterial->SetupPass(nPass);
				TextureBuffer *pTextureBuffer = cRenderer.GetTextureBuffer(0);
				if (pTextureBuffer) {
					// Setup renderer
					cRenderer.SetRenderState(RenderState::CullMode,     Cull::None);
					cRenderer.SetRenderState(RenderState::ZEnable,      false);
					cRenderer.SetRenderState(RenderState::ZWriteEnable, false);

					// Set special bitmap color
					Color4 cColor = Color.Get();
					// [TODO] Figure out the color without using fixed functions to read back a previously set color!
		//			if (GetFlags() & IgnoreColor)
		//				cColor = pFixedFunctions->GetColor();

					// Draw the bitmap
					cDrawHelpers.DrawImage(*pTextureBuffer, cSamplerStates, Position.Get(), Size.Get(), cColor, 0.5f, TexelStart.Get(), TexelEnd.Get() - TexelStart.Get());
				}
			}

		// End 2D mode
		cDrawHelpers.End2DMode();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
