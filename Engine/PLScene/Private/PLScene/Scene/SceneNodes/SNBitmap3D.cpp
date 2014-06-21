/*********************************************************\
 *  File: SNBitmap3D.cpp                                 *
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
#include <PLRenderer/Renderer/SamplerStates.h>
#include <PLRenderer/Material/Parameter.h>
#include "PLScene/Visibility/VisNode.h"
#include "PLScene/Scene/SceneNodes/SNBitmap3D.h"


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
pl_implement_class(SNBitmap3D)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
const Vector3 &SNBitmap3D::GetPositionOffset() const
{
	return m_vPositionOffset;
}

void SNBitmap3D::SetPositionOffset(const Vector3 &vValue)
{
	if (m_vPositionOffset != vValue) {
		m_vPositionOffset = vValue;

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
SNBitmap3D::SNBitmap3D() :
	PositionOffset(this)
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
SNBitmap3D::~SNBitmap3D()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Draws the bitmap shape
*/
void SNBitmap3D::DrawShape(Renderer &cRenderer, const VisNode &cVisNode)
{
	// Get material
	PLRenderer::Material *pMaterial = GetMaterialHandler().GetResource();
	if (pMaterial) {
		// Get alpha reference
		static const String sAlphaReference = "AlphaReference";
		const Parameter *pParameter = pMaterial->GetParameter(sAlphaReference);
		const float fAlphaReference = pParameter ? pParameter->GetValue1f() : 0.5f;

		// Draw primitives
		SamplerStates cSamplerStates;
		for (uint32 nPass=0; nPass<pMaterial->GetNumOfPasses(); nPass++) {
			pMaterial->SetupPass(nPass);
			TextureBuffer *pTextureBuffer = cRenderer.GetTextureBuffer(0);
			if (pTextureBuffer) {
				cRenderer.GetDrawHelpers().DrawImage(*pTextureBuffer, cSamplerStates, Vector3(-0.5f, -0.5f, 0.0f) + m_vPositionOffset, cVisNode.GetWorldViewProjectionMatrix(),
													 Vector2::One, GetColor(), fAlphaReference, GetTexelStart(), GetTexelEnd() - GetTexelStart());
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual SceneNode functions                    ]
//[-------------------------------------------------------]
void SNBitmap3D::DrawSolid(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Call base implementation
	SNBitmap::DrawSolid(cRenderer, pVisNode);

	if (pVisNode) {
		// Check if the bitmap material is transparent or not
		PLRenderer::Material *pMaterial = GetMaterialHandler().GetResource();
		if (pMaterial && !pMaterial->GetBlend()) {
			// Draw the bitmap shape
			DrawShape(cRenderer, *pVisNode);
		}
	}
}

void SNBitmap3D::DrawTransparent(Renderer &cRenderer, const VisNode *pVisNode)
{
	if (pVisNode) {
		// Check if the bitmap material is transparent or not
		PLRenderer::Material *pMaterial = GetMaterialHandler().GetResource();
		if (pMaterial && pMaterial->GetBlend()) {
			// Draw the bitmap shape
			DrawShape(cRenderer, *pVisNode);
		}
	}

	// Call base implementation
	SNBitmap::DrawTransparent(cRenderer, pVisNode);
}


//[-------------------------------------------------------]
//[ Private virtual SceneNode functions                   ]
//[-------------------------------------------------------]
void SNBitmap3D::UpdateAABoundingBox()
{
	SetAABoundingBox(AABoundingBox(-0.5f+m_vPositionOffset.x, -0.5f+m_vPositionOffset.y, -0.5f+m_vPositionOffset.z,
									0.5f+m_vPositionOffset.x,  0.5f+m_vPositionOffset.y,  0.5f+m_vPositionOffset.z));
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
