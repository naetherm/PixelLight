/*********************************************************\
 *  File: SPRTTFixedFunctions.cpp                        *
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
#include <PLMath/Rectangle.h>
#include <PLMath/Matrix4x4.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/FixedFunctions.h>
#include <PLRenderer/Effect/EffectPass.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLRenderer/Effect/EffectTechnique.h>
#include <PLRenderer/Material/MaterialManager.h>
#include <PLMesh/MeshManager.h>
#include "SPRTTFixedFunctions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLMesh;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SPRTTFixedFunctions, "", SPRTT, "Fixed functions based render to texture surface painter")
	// Constructors
	pl_constructor_1_metadata(ParameterConstructor,	PLRenderer::Renderer&,	"Parameter constructor",	"")
pl_class_metadata_end(SPRTTFixedFunctions)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SPRTTFixedFunctions::SPRTTFixedFunctions(Renderer &cRenderer) : SPRTT(cRenderer),
	m_pRenderTarget(nullptr)
{
	// Get the renderer context
	RendererContext &cRendererContext = GetRenderer().GetRendererContext();

	// Create and set a special material & effect
	Material *pMaterial = cRendererContext.GetMaterialManager().Create("MyTeapot");
	if (pMaterial) {
		Effect *pFX = cRendererContext.GetEffectManager().Create("MyTeapot");
		if (pFX) {
			EffectTechnique *pTechnique = pFX->AddTechnique();
			if (pTechnique) {
				pFX->SelectTechnique(0);
				EffectPass *pEffectPass = pTechnique->AddPass();
				if (pEffectPass)
					pEffectPass->GetFixedFunctionsRenderStates().SetLighting(true);
			}
			pMaterial->SetEffect(pFX);
		}
		m_pMeshHandler->SetMaterial(0, pMaterial);
	}

	// Create the render target. We will create a very low resolution 2D texture buffer to see funny pixels.
	m_pRenderTarget = cRenderer.CreateSurfaceTextureBuffer2D(Vector2i(64, 64), TextureBuffer::R8G8B8, SurfaceTextureBuffer::Depth);
}

/**
*  @brief
*    Destructor
*/
SPRTTFixedFunctions::~SPRTTFixedFunctions()
{
	// Destroy the render target
	if (m_pRenderTarget)
		delete m_pRenderTarget;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Draws the scene
*/
void SPRTTFixedFunctions::DrawScene(Renderer &cRenderer)
{
	// Get the fixed functions interface - if we're in here, we now it exists...
	FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();

	// Clear the content of the current used render target by using gray (this way, in case on an graphics error we might still see at least something)
	cRenderer.Clear(Clear::Color | Clear::ZBuffer, Color4::Gray);

	{ // Setup light
		FixedFunctions::Light sLight;
		pFixedFunctions->GetDefaultLightSettings(sLight);
		sLight.cAmbient.SetRGBA(0.1f, 0.1f, 0.1f, 1.0f);
		sLight.cDiffuse.SetRGBA(1.0f, 1.0f, 1.0f, 1.0f);
		sLight.nType = FixedFunctions::LightType::Point;
		sLight.vPosition.SetXYZ(0.0f, 0.0f, 1.5f);
		sLight.fLinearAttenuation = 0.3f;
		pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, Matrix4x4::Identity);
		pFixedFunctions->SetLightEnabled(0, false);
		pFixedFunctions->SetLight		(0, sLight);
		pFixedFunctions->SetLightEnabled(0, true);
	}

	{ // Set the world matrix
		// Build a rotation matrix by using a given Euler angle around the y-axis
		Matrix4x4 mWorld;
		mWorld.FromEulerAngleY(static_cast<float>(m_fRotation*Math::DegToRad));
		pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, mWorld);
	}

	{ // Set the view matrix
		Matrix4x4 mView;
		mView.SetTranslation(0.0f, -0.1f, -0.5f);
		pFixedFunctions->SetTransformState(FixedFunctions::Transform::View, mView);
	}

	{ // Set the projection matrix
		Matrix4x4 mProj;
		const float fAspect      = 1.0f;
		const float fAspectRadio = cRenderer.GetViewport().GetWidth()/(cRenderer.GetViewport().GetHeight()*fAspect);
		mProj.PerspectiveFov(static_cast<float>(45.0f*Math::DegToRad), fAspectRadio, 0.001f, 1000.0f);
		pFixedFunctions->SetTransformState(FixedFunctions::Transform::Projection, mProj);
	}

	// Draw the mesh handlers mesh
	m_pMeshHandler->Draw();
}


//[-------------------------------------------------------]
//[ Private virtual PLRenderer::SurfacePainter functions  ]
//[-------------------------------------------------------]
void SPRTTFixedFunctions::OnPaint(Surface &cSurface)
{
	// Get the used renderer
	Renderer &cRenderer = GetRenderer();

	// Clear the content of the current used render target by using gray (this way, in case on an graphics error we might still see at least something)
	cRenderer.Clear(Clear::Color | Clear::ZBuffer, Color4::Gray);

	// Fixed functions support required
	FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
	if (pFixedFunctions && m_pPositionVertexBuffer) {
		// Backup current render target and set the new one to render in our texture buffer
		Surface *pRenderSurfaceBackup = cRenderer.GetRenderTarget();
		if (cRenderer.SetRenderTarget(m_pRenderTarget)) {
			// Draw the scene
			DrawScene(cRenderer);

			// Reset render target
			cRenderer.SetRenderTarget(pRenderSurfaceBackup);
		}

		// This code is similar to the code of the triangle sample. But instead of a
		// triangle we will draw three rotating quadrangles. Further the used vertex
		// buffer has texture coordinates and we apply the 'teapot' texture buffer on the primitives.
		// Clear the content of the current used render target

		{ // Set the view matrix
			Matrix4x4 mView;
			mView.SetTranslation(0.0f, 0.0f, -5.0f);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::View, mView);
		}

		{ // Set the projection matrix
			Matrix4x4 mProj;
			const float fAspect      = 1.0f;
			const float fAspectRadio = cRenderer.GetViewport().GetWidth()/(cRenderer.GetViewport().GetHeight()*fAspect);
			mProj.PerspectiveFov(static_cast<float>(45.0f*Math::DegToRad), fAspectRadio, 0.001f, 1000.0f);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::Projection, mProj);
		}

		// Bind our vertex buffer holding the positions and texture coordinates (vertex stream 0 - main stream)
		pFixedFunctions->SetVertexBuffer(m_pPositionVertexBuffer);

		// No back face culling, please. Else we can only see one 'side' of the quadrangle
		cRenderer.SetRenderState(RenderState::CullMode, Cull::None);

		// Now draw the primitives of our cool quadrangles.
		// The primitive type is 'triangle strip', we start at vertex 0 and draw '4' vertices.

		Matrix4x4 mWorld;
		{ // Draw quadrangle 1: Primary render target
			// Set the texture buffer we rendered our teapot in as the current texture buffer
			cRenderer.SetTextureBuffer(0, m_pRenderTarget->GetTextureBuffer());

			// Disable mipmapping mapping - this is required because we created/filled no mipmaps
			// for your texture buffer
			cRenderer.SetSamplerState(0, Sampler::MagFilter, TextureFiltering::Linear);
			cRenderer.SetSamplerState(0, Sampler::MinFilter, TextureFiltering::Linear);
			cRenderer.SetSamplerState(0, Sampler::MipFilter, TextureFiltering::None);

			// Draw
			mWorld.SetTranslationMatrix(2.0f, 1.0f, 0.0f);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, mWorld);
			cRenderer.DrawPrimitives(Primitive::TriangleStrip, 0, 4);
		}

		{ // Draw quadrangle 2: MRT is not supported, so just use the primary render target again
			cRenderer.SetTextureBuffer(0, m_pRenderTarget->GetTextureBuffer());
			cRenderer.SetSamplerState(0, Sampler::MagFilter, TextureFiltering::Linear);
			cRenderer.SetSamplerState(0, Sampler::MinFilter, TextureFiltering::Linear);
			cRenderer.SetSamplerState(0, Sampler::MipFilter, TextureFiltering::None);

			// Draw
			mWorld.FromEulerAngleY(static_cast<float>(m_fRotation*Math::DegToRad));
			mWorld.SetTranslation(-2.0f, 1.0f, 0.0f);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, mWorld);
			cRenderer.DrawPrimitives(Primitive::TriangleStrip, 0, 4);
		}

		{ // Draw quadrangle 3: MRT is not supported, so just use the primary render target again
			cRenderer.SetTextureBuffer(0, m_pRenderTarget->GetTextureBuffer());
			cRenderer.SetSamplerState(0, Sampler::MagFilter, TextureFiltering::Linear);
			cRenderer.SetSamplerState(0, Sampler::MinFilter, TextureFiltering::Linear);
			cRenderer.SetSamplerState(0, Sampler::MipFilter, TextureFiltering::None);

			// Draw
			mWorld.FromEulerAngleZ(static_cast<float>(m_fRotation*Math::DegToRad));
			mWorld.SetTranslation(0.0f, 1.0f, 0.0f);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, mWorld);
			cRenderer.DrawPrimitives(Primitive::TriangleStrip, 0, 4);
		}

		{ // Draw quadrangle 4: MRT is not supported, so just use the primary render target again
			cRenderer.SetTextureBuffer(0, m_pRenderTarget->GetTextureBuffer());
			cRenderer.SetSamplerState(0, Sampler::MagFilter, TextureFiltering::Linear);
			cRenderer.SetSamplerState(0, Sampler::MinFilter, TextureFiltering::Linear);
			cRenderer.SetSamplerState(0, Sampler::MipFilter, TextureFiltering::None);

			// Draw
			mWorld.FromEulerAngleZ(static_cast<float>(-m_fRotation*Math::DegToRad));
			mWorld.SetTranslation(-2.0f, -1.0f, 0.0f);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, mWorld);
			cRenderer.DrawPrimitives(Primitive::TriangleStrip, 0, 4);
		}

		// 'Add' our vertex buffer holding the color data, set this vertex buffer to
		// vertex stream 1. This will make our output more colorful. :)
		pFixedFunctions->SetVertexBuffer(m_pColorVertexBuffer, 0, 1);

		{ // Draw quadrangle 4: Primary render target, but because there's a second vertex stream
		  // providing color data, this primitive will be quite colorful. :)
			cRenderer.SetTextureBuffer(0, m_pRenderTarget->GetTextureBuffer());
			cRenderer.SetSamplerState(0, Sampler::MagFilter, TextureFiltering::Linear);
			cRenderer.SetSamplerState(0, Sampler::MinFilter, TextureFiltering::Linear);
			cRenderer.SetSamplerState(0, Sampler::MipFilter, TextureFiltering::None);

			// Draw
			mWorld.FromEulerAngleZ(static_cast<float>(-m_fRotation*Math::DegToRad));
			mWorld.SetTranslation(2.0f, -1.0f, 0.0f);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, mWorld);
			cRenderer.DrawPrimitives(Primitive::TriangleStrip, 0, 4);
		}

		// Increase the rotation by the current time difference (time past since the last frame)
		// -> Normally, such work should NOT be performed within the rendering step, but we want
		//    to keep the implementation simple in here...
		m_fRotation += Timing::GetInstance()->GetTimeDifference()*50;
	}
}
