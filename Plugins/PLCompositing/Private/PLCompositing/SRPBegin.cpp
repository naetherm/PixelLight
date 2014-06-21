/*********************************************************\
 *  File: SRPBegin.cpp                                   *
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
#include <PLMath/Rectangle.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Renderer/FixedFunctions.h>
#include <PLScene/Scene/SNCamera.h>
#include "PLCompositing/SRPBegin.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
using namespace PLScene;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SRPBegin)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPBegin::SRPBegin() :
	TextureFormat(this),
	TextureSize(this),
	ClearFlags(this),
	ColorClear(this),
	FillMode(this),
	Flags(this),
	m_pOriginalRenderTarget(nullptr),
	m_bCurrentFrontRenderTarget(0),
	m_pTextureBufferDepth(nullptr)
{
	// Initialize the pointers to the render targets
	m_pRenderTarget[0] = m_pRenderTarget[1] = nullptr;
}

/**
*  @brief
*    Destructor
*/
SRPBegin::~SRPBegin()
{
	// Destroy the render targets
	if (m_pRenderTarget[0])
		delete m_pRenderTarget[0];
	if (m_pRenderTarget[1])
		delete m_pRenderTarget[1];
	if (m_pTextureBufferDepth)
		delete m_pTextureBufferDepth;
}

/**
*  @brief
*    Returns the render target that was set before the current render target was changed to the front/back render target
*/
Surface *SRPBegin::GetOriginalRenderTarget() const
{
	return m_pOriginalRenderTarget;
}

/**
*  @brief
*    Returns the current front render target
*/
SurfaceTextureBuffer *SRPBegin::GetFrontRenderTarget() const
{
	return m_pRenderTarget[m_bCurrentFrontRenderTarget];
}

/**
*  @brief
*    Returns the current back render target
*/
SurfaceTextureBuffer *SRPBegin::GetBackRenderTarget() const
{
	return m_pRenderTarget[!m_bCurrentFrontRenderTarget];
}

/**
*  @brief
*    Returns the depth texture (used when rendering into a texture)
*/
TextureBufferRectangle *SRPBegin::GetTextureBufferDepth() const
{
	return (!(GetFlags() & NoDepthTexture) && m_pTextureBufferDepth) ? reinterpret_cast<TextureBufferRectangle*>(m_pTextureBufferDepth->GetTextureBuffer()) : nullptr;
}

/**
*  @brief
*    Swaps the render targets
*/
void SRPBegin::SwapRenderTargets()
{
	// Both render targets must exist
	if (m_pRenderTarget[0] && m_pRenderTarget[1]) {
		// Swap the current front render target
		m_bCurrentFrontRenderTarget = !m_bCurrentFrontRenderTarget;

		// Set the new render target, we'll render into the back render target
		m_pRenderTarget[m_bCurrentFrontRenderTarget]->GetRenderer().SetRenderTarget(m_pRenderTarget[!m_bCurrentFrontRenderTarget]);

		// Provide a depth texture?
		if (!(GetFlags() & NoDepthTexture) && m_pTextureBufferDepth)
			m_pRenderTarget[m_bCurrentFrontRenderTarget]->GetRenderer().SetDepthRenderTarget(m_pTextureBufferDepth->GetTextureBuffer());
	}
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneRendererPass functions ]
//[-------------------------------------------------------]
void SRPBegin::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	{ // Set all render states to known default settings
		cRenderer.ResetRenderStates();
	//	cRenderer.ResetTransformStates(); // Would also reset camera settings, not ok
		cRenderer.ResetSamplerStates();
		cRenderer.SetTextureBuffer();
		cRenderer.SetIndexBuffer();
		cRenderer.SetViewport();
		cRenderer.SetScissorRect();
		cRenderer.SetColorMask();

		// Fixed functions
		FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
		if (pFixedFunctions) {
			pFixedFunctions->SetColor();
			pFixedFunctions->ResetRenderStates();
			pFixedFunctions->ResetTextureStageStates();
			pFixedFunctions->ResetMaterialStates();
			pFixedFunctions->ResetLights();
			pFixedFunctions->SetClipPlaneEnabled();
			pFixedFunctions->SetClipPlane();
			pFixedFunctions->SetVertexBuffer();
		}

		// Draw helpers
		cRenderer.GetDrawHelpers().End2DMode();
		cRenderer.GetDrawHelpers().Set2DZValue();
	}

	// Get the currently set render target
	m_pOriginalRenderTarget = cRenderer.GetRenderTarget();
	if (m_pOriginalRenderTarget) {
		// Get the width and height of the viewport
		const Vector2i vRenderTargetSize = (GetFlags() & CustomTextureSize) ? TextureSize : m_pOriginalRenderTarget->GetSize();

		// Create render targets or recreate them if required
		for (uint32 i=0; i<2; i++) {
			// Get the render target
			SurfaceTextureBuffer *pRenderTarget = m_pRenderTarget[i];
			if (pRenderTarget) {
				// Do we need to recreate this render target?
				const bool bRequestedMultisampleAntialiasingSetting = !(GetFlags() & NoMultisampleAntialiasing);
				const bool bCurrentMultisampleAntialiasingSetting   = !(pRenderTarget->GetFlags() & SurfaceTextureBuffer::NoMultisampleAntialiasing);
				if (pRenderTarget->GetFormat() != TextureFormat || pRenderTarget->GetSize() != vRenderTargetSize || bRequestedMultisampleAntialiasingSetting != bCurrentMultisampleAntialiasingSetting) {
					// Dam'n we have to recreate this render target :(
					delete pRenderTarget;
					pRenderTarget = nullptr;
					m_pRenderTarget[i] = nullptr;
				}
			}

			// Create render target if necessary
			if (!pRenderTarget && TextureFormat != TextureBuffer::Unknown) {
				// Flags
				uint32 nFlags = (GetFlags() & NoMultisampleAntialiasing) ? SurfaceTextureBuffer::NoMultisampleAntialiasing : 0;

				// Create the render target
				m_pRenderTarget[i] = cRenderer.CreateSurfaceTextureBufferRectangle(vRenderTargetSize, static_cast<TextureBuffer::EPixelFormat>(TextureFormat.GetInt()), nFlags);
				m_bCurrentFrontRenderTarget = 1;
			}
		}

		// Create a depth texture?
		if (!(GetFlags() & NoDepthTexture)) {
			// Destroy previous texture?
			if (m_pTextureBufferDepth) {
				// Do we need to recreate this render target?
				const bool bRequestedStencilSetting = !(GetFlags() & NoStencil);
				const bool bCurrentStencilSetting   = (m_pTextureBufferDepth->GetFlags() & SurfaceTextureBuffer::Stencil) != 0;
				const bool bRequestedMultisampleAntialiasingSetting = !(GetFlags() & NoMultisampleAntialiasing);
				const bool bCurrentMultisampleAntialiasingSetting   = !(m_pTextureBufferDepth->GetFlags() & SurfaceTextureBuffer::NoMultisampleAntialiasing);
				if (m_pTextureBufferDepth->GetSize() != vRenderTargetSize || bRequestedStencilSetting != bCurrentStencilSetting || bRequestedMultisampleAntialiasingSetting != bCurrentMultisampleAntialiasingSetting) {
					// We have to recreate this render target :(
					delete m_pTextureBufferDepth;
					m_pTextureBufferDepth = nullptr;
				}
			}

			// Create texture?
			if (!m_pTextureBufferDepth) {
				uint32 nFlags = (GetFlags() & NoMultisampleAntialiasing) ? SurfaceTextureBuffer::Depth|SurfaceTextureBuffer::NoMultisampleAntialiasing : SurfaceTextureBuffer::Depth;
				if (!(GetFlags() & NoStencil))
					nFlags |= SurfaceTextureBuffer::Stencil;
				m_pTextureBufferDepth = cRenderer.CreateSurfaceTextureBufferRectangle(vRenderTargetSize, TextureBuffer::D24, nFlags);
			}
		}
	}

	// Set the current render target
	if (m_pRenderTarget[0] && m_pRenderTarget[1]) {
		// Set the new render target, we'll render into the back render target
		cRenderer.SetRenderTarget(m_pRenderTarget[!m_bCurrentFrontRenderTarget]);

		// Provide a depth texture?
		if (!(GetFlags() & NoDepthTexture) && m_pTextureBufferDepth)
			cRenderer.SetDepthRenderTarget(m_pTextureBufferDepth->GetTextureBuffer());
	}

	// Set fill mode
	switch (FillMode) {
		case SolidMode:
			cRenderer.SetRenderState(RenderState::FixedFillMode, Fill::Solid);
			break;

		case LineMode:
			cRenderer.SetRenderState(RenderState::FixedFillMode, Fill::Line);
			break;

		case PointMode:
			cRenderer.SetRenderState(RenderState::FixedFillMode, Fill::Point);
			break;

		default:
			cRenderer.SetRenderState(RenderState::FixedFillMode, Fill::Solid);
			break;
	}

	// [TODO] Remove this
	// Setup cull mode
	cRenderer.SetRenderState(RenderState::InvCullMode, false);
	SNCamera *pCamera = SNCamera::GetCamera();
	if (pCamera)
		cRenderer.SetRenderState(RenderState::InvCullMode, pCamera->GetFlags() & SNCamera::InvCullMode);

	{ // Clear the color, depth and stencil buffer
		uint32 nFlags = 0;
		if (ClearFlags & ClearColor)
			nFlags |= Clear::Color;
		if (ClearFlags & ClearDepth)
			nFlags |= Clear::ZBuffer;
		if (!(GetFlags() & NoStencil) && (ClearFlags & ClearStencil))
			nFlags |= Clear::Stencil;
		cRenderer.Clear(nFlags, ColorClear.Get());
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
