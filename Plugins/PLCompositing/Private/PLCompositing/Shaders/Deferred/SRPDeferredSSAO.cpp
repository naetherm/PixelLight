/*********************************************************\
 *  File: SRPDeferredSSAO.cpp                            *
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
#include <PLRenderer/Renderer/Program.h>
#include <PLRenderer/Renderer/VertexShader.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ShaderLanguage.h>
#include <PLRenderer/Renderer/ProgramAttribute.h>
#include <PLRenderer/Renderer/FragmentShader.h>
#include <PLRenderer/Renderer/TextureBufferRectangle.h>
#include <PLRenderer/Effect/EffectManager.h>
#include "PLCompositing/FullscreenQuad.h"
#include "PLCompositing/Shaders/Deferred/SRPDeferredGBuffer.h"
#include "PLCompositing/Shaders/Deferred/SRPDeferredSSAO.h"


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
pl_implement_class(SRPDeferredSSAO)


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPDeferredSSAO::SRPDeferredSSAO() :
	ShaderLanguage(this),
	Contrast(this),
	BlurRadius(this),
	BlurSharpness(this),
	ResolutionScale(this),
	SceneScale(this),
	Flags(this),
	EventHandlerDirty(&SRPDeferredSSAO::OnDirty, this),
	m_pRenderTargetAO(nullptr),
	m_pRenderTargetXBlur(nullptr),
	m_pVertexShader(nullptr),
	m_pFragmentShader(nullptr),
	m_pProgram(nullptr),
	m_pPositionProgramAttribute(nullptr),
	m_pTextureSizeProgramUniform(nullptr),
	m_pInputTextureSizeProgramUniform(nullptr),
	m_pBlurRadiusProgramUniform(nullptr),
	m_pBlurFalloffProgramUniform(nullptr),
	m_pSharpnessProgramUniform(nullptr),
	m_pUVScaleProgramUniform(nullptr),
	m_pInputTextureProgramUniform(nullptr),
	m_pNormalDepthTextureProgramUniform(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
SRPDeferredSSAO::~SRPDeferredSSAO()
{
	// Destroy the render targets required for blur
	if (m_pRenderTargetAO)
		delete m_pRenderTargetAO;
	if (m_pRenderTargetXBlur)
		delete m_pRenderTargetXBlur;

	// Destroy the shader stuff
	if (m_pProgram)
		delete m_pProgram;
	if (m_pFragmentShader)
		delete m_pFragmentShader;
	if (m_pVertexShader)
		delete m_pVertexShader;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Draws the blur
*/
void SRPDeferredSSAO::DrawBlur(const String &sShaderLanguage, VertexBuffer &cVertexBuffer, TextureBufferRectangle &cInputTextureBuffer, TextureBufferRectangle &cNormalDepthTextureBuffer, bool bXBlur)
{
	// Get the renderer instance
	Renderer &cRenderer = cNormalDepthTextureBuffer.GetRenderer();

	// Create the shaders and programs right now?
	if (!m_pVertexShader || m_pVertexShader->GetShaderLanguage() != sShaderLanguage) {
		// If there's an previous instance of the program, destroy it first
		if (m_pProgram) {
			delete m_pProgram;
			m_pProgram = nullptr;
		}
		if (m_pFragmentShader) {
			delete m_pFragmentShader;
			m_pFragmentShader = nullptr;
		}
		if (m_pVertexShader) {
			delete m_pVertexShader;
			m_pVertexShader = nullptr;
		}
		m_pTextureSizeProgramUniform		= nullptr;
		m_pInputTextureSizeProgramUniform	= nullptr;
		m_pBlurRadiusProgramUniform			= nullptr;
		m_pBlurFalloffProgramUniform		= nullptr;
		m_pSharpnessProgramUniform			= nullptr;
		m_pUVScaleProgramUniform			= nullptr;
		m_pInputTextureProgramUniform		= nullptr;
		m_pNormalDepthTextureProgramUniform	= nullptr;

		// Get the shader language instance
		PLRenderer::ShaderLanguage *pShaderLanguage = cRenderer.GetShaderLanguage(sShaderLanguage);
		if (pShaderLanguage) {
			// Shader source code
			String sVertexShaderSourceCode;
			String sFragmentShaderSourceCode;
			if (sShaderLanguage == "GLSL") {
				#include "SRPDeferredSSAO_GLSL.h"
				sVertexShaderSourceCode	  = sDeferredSSAO_GLSL_VS;
				sFragmentShaderSourceCode = sDeferredSSAO_GLSL_FS;
			} else if (sShaderLanguage == "Cg") {
				#include "SRPDeferredSSAO_Cg.h"
				sVertexShaderSourceCode	  = sDeferredSSAO_Cg_VS;
				sFragmentShaderSourceCode = sDeferredSSAO_Cg_FS;
			}

			// Create a vertex shader instance
			m_pVertexShader = pShaderLanguage->CreateVertexShader(sVertexShaderSourceCode);

			// Create a fragment shader instance
			m_pFragmentShader = pShaderLanguage->CreateFragmentShader(sFragmentShaderSourceCode);

			// Create a program instance and assign the created vertex and fragment shaders to it
			m_pProgram = pShaderLanguage->CreateProgram(m_pVertexShader, m_pFragmentShader);
			if (m_pProgram) {
				// Add our nark which will inform us as soon as the program gets dirty
				m_pProgram->EventDirty.Connect(EventHandlerDirty);

				// Get attributes and uniforms
				OnDirty(m_pProgram);
			}
		}
	}

	// Make the depth blur GPU program to the currently used one
	if (cRenderer.SetProgram(m_pProgram)) {
		// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
		if (m_pPositionProgramAttribute)
			m_pPositionProgramAttribute->Set(&cVertexBuffer, PLRenderer::VertexBuffer::Position);

		// Set the "TextureSize" fragment shader parameter
		if (m_pTextureSizeProgramUniform)
			m_pTextureSizeProgramUniform->Set(cNormalDepthTextureBuffer.GetSize());

		// Set the "InputTextureSize" fragment shader parameter
		if (m_pInputTextureSizeProgramUniform)
			m_pInputTextureSizeProgramUniform->Set(cInputTextureBuffer.GetSize());

		// Set the "BlurRadius" fragment shader parameter
		if (m_pBlurRadiusProgramUniform)
			m_pBlurRadiusProgramUniform->Set(BlurRadius);

		// Set the "BlurFalloff" fragment shader parameter
		if (m_pBlurFalloffProgramUniform) {
			const float fSigma = (BlurRadius + 1)/2;
			m_pBlurFalloffProgramUniform->Set(1.0f/(2*fSigma*fSigma));
		}

		// Set the "Sharpness" fragment shader parameter
		if (m_pSharpnessProgramUniform) {
			const float fValue = BlurSharpness / SceneScale;
			m_pSharpnessProgramUniform->Set(fValue*fValue);
		}

		// Set the "UVScale" fragment shader parameter
		if (m_pUVScaleProgramUniform) {
			if (bXBlur)
				m_pUVScaleProgramUniform->Set(0.0f, 1.0f);
			else
				m_pUVScaleProgramUniform->Set(1.0f, 0.0f);
		}

		// Set the "InputTexture" fragment shader parameter
		if (m_pInputTextureProgramUniform) {
			const int nTextureUnit = m_pInputTextureProgramUniform->Set(&cInputTextureBuffer);
			if (nTextureUnit >= 0) {
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Clamp);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Clamp);
				if (cInputTextureBuffer.GetSize() != cNormalDepthTextureBuffer.GetSize()) {
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::Linear);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::Linear);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
				} else {
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
				}
			}
		}

		// Set the "NormalDepthTexture" fragment shader parameter
		if (m_pNormalDepthTextureProgramUniform) {
			const int nTextureUnit = m_pNormalDepthTextureProgramUniform->Set(&cNormalDepthTextureBuffer);
			if (nTextureUnit >= 0) {
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Clamp);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Clamp);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
			}
		}

		// Draw the fullscreen quad
		cRenderer.DrawPrimitives(Primitive::TriangleStrip, 0, 4);
	}
}

/**
*  @brief
*    Called when a program became dirty
*/
void SRPDeferredSSAO::OnDirty(Program *pProgram)
{
	// Get attributes and uniforms
	if (pProgram == m_pProgram) {
		m_pPositionProgramAttribute			= m_pProgram->GetAttribute("VertexPosition");
		m_pTextureSizeProgramUniform		= m_pProgram->GetUniform("TextureSize");
		m_pInputTextureSizeProgramUniform	= m_pProgram->GetUniform("InputTextureSize");
		m_pBlurRadiusProgramUniform			= m_pProgram->GetUniform("BlurRadius");
		m_pBlurFalloffProgramUniform		= m_pProgram->GetUniform("BlurFalloff");
		m_pSharpnessProgramUniform			= m_pProgram->GetUniform("Sharpness");
		m_pUVScaleProgramUniform			= m_pProgram->GetUniform("UVScale");
		m_pInputTextureProgramUniform		= m_pProgram->GetUniform("InputTexture");
		m_pNormalDepthTextureProgramUniform	= m_pProgram->GetUniform("NormalDepthTexture");
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRPDeferredSSAO::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Ensure that the resolution scale is really between 0-1, if this is <= 0, just skip this scene renderer pass!
	if (ResolutionScale > 0.0f) {
		if (ResolutionScale > 1.0f)
			ResolutionScale = 1.0f;

		// Get the instance of the "PLCompositing::SRPDeferredGBuffer" scene renderer pass
		SRPDeferredGBuffer *pSRPDeferredGBuffer = GetGBuffer();
		if (pSRPDeferredGBuffer && pSRPDeferredGBuffer->GetRenderTarget()) {
			// Get the fullscreen quad instance
			FullscreenQuad *pFullscreenQuad = pSRPDeferredGBuffer->GetFullscreenQuad();
			if (pFullscreenQuad) {
				// Get the vertex buffer of the fullscreen quad
				VertexBuffer *pVertexBuffer = pFullscreenQuad->GetVertexBuffer();
				if (pVertexBuffer) {
					// RG components of RT1 store the normal vector, B component of RT1 stores the linear view space depth
					TextureBufferRectangle *pNormalDepthTextureBuffer = pSRPDeferredGBuffer->GetRenderTargetTextureBuffer(1);
					if (pNormalDepthTextureBuffer) {
						// Get the shader language to use
						String sShaderLanguage = ShaderLanguage;
						if (!sShaderLanguage.GetLength())
							sShaderLanguage = cRenderer.GetDefaultShaderLanguage();

						// Reset all render states to default
						cRenderer.GetRendererContext().GetEffectManager().Use();

						// Setup renderer
						const uint32 nFixedFillModeBackup = cRenderer.GetRenderState(RenderState::FixedFillMode);
						cRenderer.SetRenderState(RenderState::ScissorTestEnable, false);
						cRenderer.SetRenderState(RenderState::FixedFillMode,	 Fill::Solid);
						cRenderer.SetRenderState(RenderState::CullMode,			 Cull::None);
						cRenderer.SetRenderState(RenderState::ZEnable,			 false);
						cRenderer.SetRenderState(RenderState::ZWriteEnable,		 false);

						// Get and prepare the render target of the GBuffer (we store the AO result within the alpha channel of the GBuffer RT0)
						SurfaceTextureBuffer *pGBufferRenderTarget = pSRPDeferredGBuffer->GetRenderTarget();
						const uint8 nGBufferRenderTargetMaxColorTargetsBackup = pGBufferRenderTarget->GetMaxColorTargets();
						pGBufferRenderTarget->SetMaxColorTargets(1); // We only render into one target right now!

						// Backup current render target
						Surface *pSurfaceBackup = cRenderer.GetRenderTarget();

						// Backup the color mask
						bool bRed, bGreen, bBlue, bAlpha;
						cRenderer.GetColorMask(bRed, bGreen, bBlue, bAlpha);

						// Ambient occlusion with blur?
						if (GetFlags() & NoBlur) {
							// Nope, just blank pixel chaos ambient occlusion

							// [TODO] ResolutionScale support

							// Set the new render target
							cRenderer.SetRenderTarget(pGBufferRenderTarget);

							// Set the color mask - we will ONLY write into the alpha channel, do NOT overwrite the other channels!
							cRenderer.SetColorMask(false, false, false, true);

							// Draws the AO
							DrawAO(sShaderLanguage, *pVertexBuffer, *pNormalDepthTextureBuffer);
						} else {
							// Jap, we want to have smooth ambient occlusion

							// Get the size of the GBuffer render target
							const Vector2i vRTSize = pGBufferRenderTarget->GetSize();
							if (vRTSize.x != 0 && vRTSize.y != 0) {
								// Get the texture format. A simple 1 byte per pixel format is good enough,
								// a fancy 16 or even 32 bit floating point format doesn't bring a visible improvement.
	//							const TextureBuffer::EPixelFormat nInternalFormat = TextureBuffer::L32F;
								const TextureBuffer::EPixelFormat nInternalFormat = TextureBuffer::L8;

								// Get the size of the AO render target
								const Vector2i vAOSize = vRTSize*ResolutionScale;

								// Render target size or internal format change?
								if (m_pRenderTargetAO && (m_pRenderTargetAO->GetSize() != vAOSize || m_pRenderTargetAO->GetFormat() != nInternalFormat)) {
									delete m_pRenderTargetAO;
									m_pRenderTargetAO = nullptr;
								}
								if (m_pRenderTargetXBlur && (m_pRenderTargetXBlur->GetSize() != vRTSize || m_pRenderTargetXBlur->GetFormat() != nInternalFormat)) {
									delete m_pRenderTargetXBlur;
									m_pRenderTargetXBlur = nullptr;
								}

								// Create/update the render targets required for AO blur - if necessary
								if (!m_pRenderTargetAO)
									m_pRenderTargetAO = cRenderer.CreateSurfaceTextureBufferRectangle(vAOSize, nInternalFormat, SurfaceTextureBuffer::NoMultisampleAntialiasing);
								if (!m_pRenderTargetXBlur)
									m_pRenderTargetXBlur = cRenderer.CreateSurfaceTextureBufferRectangle(vRTSize, nInternalFormat, SurfaceTextureBuffer::NoMultisampleAntialiasing);

								// Render the AO with blur
								if (m_pRenderTargetAO && m_pRenderTargetXBlur) {
									// Set color mask
									cRenderer.SetColorMask(true, true, true, true);

									{ // Render AO
										// Set the new render target and color mask
										cRenderer.SetRenderTarget(m_pRenderTargetAO);

										// Draws the AO
										DrawAO(sShaderLanguage, *pVertexBuffer, *pNormalDepthTextureBuffer);
									}

									{ // X blur
										// Set the new render target and color mask
										cRenderer.SetRenderTarget(m_pRenderTargetXBlur);

										// Draws the blur
										DrawBlur(sShaderLanguage, *pVertexBuffer, static_cast<TextureBufferRectangle&>(*m_pRenderTargetAO->GetTextureBuffer()), *pNormalDepthTextureBuffer, true);
									}

									{ // Y blur, the result is written into the alpha channel of the GBuffer RT0
										// Set the new render target
										cRenderer.SetRenderTarget(pGBufferRenderTarget);

										// Set the color mask - we will ONLY write into the alpha channel, do NOT overwrite the other channels!
										cRenderer.SetColorMask(false, false, false, true);

										// Draws the blur
										DrawBlur(sShaderLanguage, *pVertexBuffer, static_cast<TextureBufferRectangle&>(*m_pRenderTargetXBlur->GetTextureBuffer()), *pNormalDepthTextureBuffer, false);
									}
								}
							}
						}

						// Restore fixed fill mode render state
						cRenderer.SetRenderState(RenderState::FixedFillMode, nFixedFillModeBackup);

						// Restore the color mask
						cRenderer.SetColorMask(bRed, bGreen, bBlue, bAlpha);

						// Restore the previously set render target and GBuffer render target settings
						cRenderer.SetRenderTarget(pSurfaceBackup);
						pGBufferRenderTarget->SetMaxColorTargets(nGBufferRenderTargetMaxColorTargetsBackup);
					}
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
