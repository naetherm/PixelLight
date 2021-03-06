/*********************************************************\
 *  File: SRPEndFXAA.cpp                                 *
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
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Renderer/SamplerStates.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ProgramAttribute.h>
#include <PLRenderer/Renderer/TextureBuffer2D.h>
#include <PLRenderer/Renderer/TextureBufferRectangle.h>
#include <PLRenderer/Effect/EffectManager.h>
#include "PLCompositing/FullscreenQuad.h"
#include "PLCompositing/SRPBegin.h"
#include "PLCompositing/Shaders/PostProcessing/SRPEndFXAA.h"


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
pl_class_metadata(SRPEndFXAA, "PLCompositing", PLCompositing::SRPEnd, "\"Fast Approximate Anti-Aliasing\" (FXAA) render pipeline finishing scene renderer pass")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Attributes
	pl_attribute_metadata(ShaderLanguage,	PLCore::String,							"",						ReadWrite,	"Shader language to use (for example \"GLSL\" or \"Cg\"), if empty string, the default shader language of the renderer will be used",	"")
	pl_attribute_metadata(Preset,			pl_enum_type_def3(SRPEndFXAA, EPreset),	SRPEndFXAA::Preset3,	ReadWrite,	"FXAA preset as described within http://developer.download.nvidia.com/assets/gamedev/files/sdk/11/FXAA_WhitePaper.pdf",					"")
		// Overwritten PLScene::SceneRendererPass attributes
	pl_attribute_metadata(Flags,			pl_flag_type_def3(SRPEndFXAA, EFlags),	0,						ReadWrite,	"Flags",																																"")
pl_class_metadata_end(SRPEndFXAA)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPEndFXAA::SRPEndFXAA() :
	ShaderLanguage(this),
	Preset(this),
	Flags(this),
	m_pRenderTarget(nullptr),
	m_pFullscreenQuad(nullptr),
	m_pProgramGenerator(nullptr),
	m_nCurrentPreset(Preset.Get())
{
}

/**
*  @brief
*    Destructor
*/
SRPEndFXAA::~SRPEndFXAA()
{
	// Destroy the render target
	if (m_pRenderTarget)
		delete m_pRenderTarget;

	// Destroy the fullscreen quad
	if (m_pFullscreenQuad)
		delete m_pFullscreenQuad;

	// Destroy the program generator
	if (m_pProgramGenerator)
		delete m_pProgramGenerator;
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneRendererPass functions ]
//[-------------------------------------------------------]
void SRPEndFXAA::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Get the "PLCompositing::SRPBegin" instance
	SRPBegin *pSRPBegin = static_cast<SRPBegin*>(GetFirstInstanceOfSceneRendererPassClass("PLCompositing::SRPBegin"));
	if (pSRPBegin) {
		// Get the back render target of SRPBegin, this holds the current content
		SurfaceTextureBuffer *pSurfaceTextureBuffer = pSRPBegin->GetBackRenderTarget();
		if (pSurfaceTextureBuffer) {
			// Create the fullscreen quad instance if required
			if (!m_pFullscreenQuad)
				m_pFullscreenQuad = new FullscreenQuad(cRenderer);

			// Get the vertex buffer of the fullscreen quad
			VertexBuffer *pVertexBuffer = m_pFullscreenQuad->GetVertexBuffer();
			if (pVertexBuffer) {
				// Get the shader language to use
				String sShaderLanguage = ShaderLanguage;
				if (!sShaderLanguage.GetLength())
					sShaderLanguage = cRenderer.GetDefaultShaderLanguage();

				// In case the render target size was changed, we need to recreate the shader due to the used shader constant
				bool bRenderTargetDimensionChanged = (m_nCurrentPreset != Preset.Get());

				{	// [HACK][TODO] Currently the deferred renderer is still using rectangle textures
					// and not the more universal NPOT texture capabilities which are common nowadays.
					// -> The FXAA shader isn't working well with rectangle textures (tried it), so we're "converting"
					//    our rectangle texture in here so we can feed the result into the FXAA shader
					// -> This is just a workaround for now, better to remove rectangle texture usage at all
					//    (just usage, not support!), shouldn't longer be a compatibility issue

					// Get the texture we will use as 'source texture' when rendering the fullscreen quad
					TextureBufferRectangle *pTextureBuffer = static_cast<TextureBufferRectangle*>(pSurfaceTextureBuffer->GetTextureBuffer());

					// Render target size change?
					const Vector2i &vRTSize = pTextureBuffer->GetSize();
					if (m_pRenderTarget && m_pRenderTarget->GetSize() != vRTSize) {
						bRenderTargetDimensionChanged = true;

						// Destroy the render target
						if (m_pRenderTarget) {
							delete m_pRenderTarget;
							m_pRenderTarget = nullptr;
						}
					}

					// Create the render target right now?
					if (!m_pRenderTarget)
						m_pRenderTarget = cRenderer.CreateSurfaceTextureBuffer2D(vRTSize, TextureBuffer::R8G8B8A8, SurfaceTextureBuffer::NoMultisampleAntialiasing);
					if (m_pRenderTarget) {
						// Backup the current render target and set the new one
						Surface *pRenderTargetBackup = cRenderer.GetRenderTarget();
						cRenderer.SetRenderTarget(m_pRenderTarget);

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

							// Draw the result (we have to flip the result upside-down...)
							SamplerStates cSamplerStates;
							cSamplerStates.Set(Sampler::MipFilter, TextureFiltering::None);
							cSamplerStates.Set(Sampler::MipFilter, TextureFiltering::None);
							cSamplerStates.Set(Sampler::MipFilter, TextureFiltering::None);
							cDrawHelpers.DrawImage(*pSurfaceTextureBuffer->GetTextureBuffer(), cSamplerStates, Vector2::Zero, Vector2::One, Color4::White, 1.0f, Vector2(0.0f, 1.0f), Vector2(1.0f, -1.0f));

						// End 2D mode
						cDrawHelpers.End2DMode();

						// Restore fixed fill mode render state
						cRenderer.SetRenderState(RenderState::FixedFillMode, nFixedFillModeBackup);

						// Restore previous render target
						cRenderer.SetRenderTarget(pRenderTargetBackup);
					}
				}

				// Create the program generator if there's currently no instance of it
				if (m_pRenderTarget && (!m_pProgramGenerator || m_pProgramGenerator->GetShaderLanguage() != sShaderLanguage) || bRenderTargetDimensionChanged) {
					// If there's an previous instance of the program generator, destroy it first
					if (m_pProgramGenerator) {
						delete m_pProgramGenerator;
						m_pProgramGenerator = nullptr;
					}

					// Get render target size as string for the shader constant
					const Vector2i &vRTSize = static_cast<TextureBuffer2D*>(m_pRenderTarget->GetTextureBuffer())->GetSize();
					const String sInvRTSize = String() + (1.0f/vRTSize.x) + ',' + (1.0f/vRTSize.y);

					// Preset
					m_nCurrentPreset = Preset.Get();
					const String sPreset = String("#define FXAA_PRESET ") + static_cast<int>(m_nCurrentPreset) + '\n';

					// Choose the shader source codes depending on the requested shader language
					#include "FxaaShader.h"	// FXAA shader source
					if (sShaderLanguage == "GLSL") {
						// Shader source
						#include "SRPEndFXAA_GLSL.h"

						// Compose shader
						String sFS = "#define FXAA_GLSL_130 1\n";
						sFS += sPreset;
						sFS += "const vec2 InvTextureSize = vec2(" + sInvRTSize + ");	// 1/(texture size) - set when creating the shadern\n";
						sFS += sEndFXAA_FS;
						sFS += sEndFXAA_GLSL_FS;

						// Generate final shader
						m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sEndFXAA_GLSL_VS, "130", sFS, "130");	// OpenGL 3.0 ("#version 130")
					} else if (sShaderLanguage == "Cg") {
						// Shader source
						#include "SRPEndFXAA_Cg.h"

						// Compose shader
						String sFS = "#define FXAA_HLSL_3 1\n";
						sFS += sPreset;
						sFS += "const float2 InvTextureSize = float2(" + sInvRTSize + ");	// 1/(texture size) - set when creating the shader\n";
						sFS += sEndFXAA_FS;
						sFS += sEndFXAA_Cg_FS;
						m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sEndFXAA_Cg_VS, "glslv", sFS, "glslf");
					}
				}

				// If there's no program generator, we don't need to continue
				if (m_pProgramGenerator) {
					// Reset all render states to default
					cRenderer.GetRendererContext().GetEffectManager().Use();
					cRenderer.SetColorMask();

					// Setup renderer
					const uint32 nFixedFillModeBackup = cRenderer.GetRenderState(RenderState::FixedFillMode);
					cRenderer.SetRenderState(RenderState::ScissorTestEnable, false);
					cRenderer.SetRenderState(RenderState::FixedFillMode,	 Fill::Solid);
					cRenderer.SetRenderState(RenderState::CullMode,			 Cull::None);
					cRenderer.SetRenderState(RenderState::ZEnable,			 false);
					cRenderer.SetRenderState(RenderState::ZWriteEnable,		 false);

					// Set the current render target back to the original one?
					if (GetFlags() & WriteToOriginalSurface)
						cRenderer.SetRenderTarget(pSRPBegin->GetOriginalRenderTarget());
					else
						pSRPBegin->SwapRenderTargets();

					// Reset the program flags
					m_cProgramFlags.Reset();

					// Get a program instance from the program generator using the given program flags
					ProgramGenerator::GeneratedProgram *pGeneratedProgram = m_pProgramGenerator->GetProgram(m_cProgramFlags);

					// Make our program to the current one
					if (pGeneratedProgram && cRenderer.SetProgram(pGeneratedProgram->pProgram)) {
						// Set pointers to uniforms & attributes of a generated program if they are not set yet
						GeneratedProgramUserData *pGeneratedProgramUserData = static_cast<GeneratedProgramUserData*>(pGeneratedProgram->pUserData);
						if (!pGeneratedProgramUserData) {
							pGeneratedProgram->pUserData = pGeneratedProgramUserData = new GeneratedProgramUserData;
							Program *pProgram = pGeneratedProgram->pProgram;
							// Vertex shader attributes
							static const String sVertexPosition = "VertexPosition";
							pGeneratedProgramUserData->pVertexPosition	= pProgram->GetAttribute(sVertexPosition);
							// Fragment shader uniforms
							static const String sTexture = "Texture";
							pGeneratedProgramUserData->pTexture			= pProgram->GetUniform(sTexture);
						}

						// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
						if (pGeneratedProgramUserData->pVertexPosition)
							pGeneratedProgramUserData->pVertexPosition->Set(pVertexBuffer, PLRenderer::VertexBuffer::Position);

						// Set the "Texture" fragment shader parameter
						if (pGeneratedProgramUserData->pTexture && m_pRenderTarget) {
							const int nTextureUnit = pGeneratedProgramUserData->pTexture->Set(m_pRenderTarget->GetTextureBuffer());
							if (nTextureUnit >= 0) {
								cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Clamp);
								cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Clamp);
								cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::Anisotropic);
								cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::Anisotropic);
								cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
								cRenderer.SetSamplerState(nTextureUnit, Sampler::MaxAnisotropy, 4);
							}
						}

						// Draw the fullscreen quad
						m_pFullscreenQuad->Draw();
					}

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
