/*********************************************************\
 *  File: SRPEndHDR.cpp                                  *
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
#include <float.h>
#include <PLCore/Tools/Timing.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Font.h>
#include <PLRenderer/Renderer/Program.h>
#include <PLRenderer/Renderer/FontManager.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ProgramAttribute.h>
#include <PLRenderer/Renderer/TextureBufferRectangle.h>
#include <PLRenderer/Effect/EffectManager.h>
#include "PLCompositing/FullscreenQuad.h"
#include "PLCompositing/SRPBegin.h"
#include "PLCompositing/Shaders/HDR/HDRAverageLuminance.h"
#include "PLCompositing/Shaders/HDR/HDRLightAdaptation.h"
#include "PLCompositing/Shaders/HDR/HDRBloom.h"
#include "PLCompositing/Shaders/HDR/SRPEndHDR.h"


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
pl_implement_class(SRPEndHDR)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPEndHDR::SRPEndHDR() :
	ShaderLanguage(this),
	LuminanceConvert(this),
	Key(this),
	WhiteLevel(this),
	AverageLuminance(this),
	Tau(this),
	BloomBrightThreshold(this),
	BloomFactor(this),
	BloomBlurPasses(this),
	BloomDownscale(this),
	Gamma(this),
	Flags(this),
	m_pFullscreenQuad(nullptr),
	m_pHDRAverageLuminance(nullptr),
	m_pHDRLightAdaptation(nullptr),
	m_pHDRBloom(nullptr),
	m_pProgramGenerator(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
SRPEndHDR::~SRPEndHDR()
{
	// Destroy the HDR logarithmic average luminance calculation component
	if (m_pHDRAverageLuminance)
		delete m_pHDRAverageLuminance;

	// Destroy the HDR light adaptation calculation component
	if (m_pHDRLightAdaptation)
		delete m_pHDRLightAdaptation;

	// Destroy the HDR bloom calculation component
	if (m_pHDRBloom)
		delete m_pHDRBloom;

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
void SRPEndHDR::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
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

				// Create the program generator if there's currently no instance of it
				if (!m_pProgramGenerator || m_pProgramGenerator->GetShaderLanguage() != sShaderLanguage) {
					// If there's an previous instance of the program generator, destroy it first
					if (m_pProgramGenerator) {
						delete m_pProgramGenerator;
						m_pProgramGenerator = nullptr;
					}

					// Choose the shader source codes depending on the requested shader language
					if (sShaderLanguage == "GLSL") {
						#include "SRPEndHDR_GLSL.h"
						m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sEndHDR_GLSL_VS, "110", sEndHDR_GLSL_FS, "110");	// OpenGL 2.0 ("#version 110")
					} else if (sShaderLanguage == "Cg") {
						#include "SRPEndHDR_Cg.h"
						m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sEndHDR_Cg_VS, "glslv", sEndHDR_Cg_FS, "glslf");
					}
				}

				// If there's no program generator, we don't need to continue
				if (m_pProgramGenerator) {
					// Get the texture we will use as 'source texture' when rendering the fullscreen quad
					TextureBufferRectangle *pHDRTextureBuffer = static_cast<TextureBufferRectangle*>(pSurfaceTextureBuffer->GetTextureBuffer());

					// Debug feature: Check and NAN fix values (SLOW! But it's only for debugging...)
					uint32 nNumOfFixedNANValues = 0;
					if ((GetFlags() & DebugCheckAndFixNANValues) && pHDRTextureBuffer)
						nNumOfFixedNANValues = pHDRTextureBuffer->FixNANValues(PLGraphics::Color4::Red);

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

					// Get the requested features
					bool bToneMapping				= !(GetFlags() & NoToneMapping);
					bool bAutomaticAverageLuminance = !(GetFlags() & NoAutomaticAverageLuminance);
					bool bBloom						= !(GetFlags() & NoBloom);
					bool bGammaCorrection			= !(GetFlags() & NoGammaCorrection);

					// Calculate average luminance, required for tone mapping with automatically calculated average luminance
					TextureBuffer *pHDRAverageLuminanceTextureBuffer = nullptr;
					if (bAutomaticAverageLuminance) {
						if (!m_pHDRAverageLuminance)
							m_pHDRAverageLuminance = new HDRAverageLuminance(cRenderer);
						m_pHDRAverageLuminance->CalculateAverageLuminance(sShaderLanguage, static_cast<TextureBufferRectangle&>(*pSurfaceTextureBuffer->GetTextureBuffer()), LuminanceConvert.Get());

						// Light adaptation
						if (!(GetFlags() & NoLightAdaptation) && m_pHDRAverageLuminance->GetTextureBuffer()) {
							// [TODO] Would be nice if the "previous" data would not be stored in the scene renderer pass instance...
							if (!m_pHDRLightAdaptation)
								m_pHDRLightAdaptation = new HDRLightAdaptation(cRenderer);
							m_pHDRLightAdaptation->CalculateLightAdaptation(sShaderLanguage, *m_pHDRAverageLuminance->GetTextureBuffer(), Timing::GetInstance()->GetTimeDifference()*Tau);
							pHDRAverageLuminanceTextureBuffer = m_pHDRLightAdaptation->GetTextureBuffer();
						} else {
							pHDRAverageLuminanceTextureBuffer = m_pHDRAverageLuminance->GetTextureBuffer();
						}
					}
					if (!pHDRAverageLuminanceTextureBuffer)
						bAutomaticAverageLuminance = false;

					// Calculate bloom
					if (bBloom) {
						if (BloomDownscale < 1.0f)
							BloomDownscale = 1.0f;
						if (!m_pHDRBloom)
							m_pHDRBloom = new HDRBloom(cRenderer);
						m_pHDRBloom->CalculateBloom(sShaderLanguage, static_cast<TextureBufferRectangle&>(*pSurfaceTextureBuffer->GetTextureBuffer()), BloomBrightThreshold, bToneMapping, bAutomaticAverageLuminance,
													LuminanceConvert.Get(), Key, WhiteLevel, AverageLuminance, pHDRAverageLuminanceTextureBuffer, BloomBlurPasses, BloomDownscale, (GetFlags() & UseVertexTextureFetch) != 0);

						// Show bloom texture (for debugging)
						if (GetFlags() & DebugShowBloomTexture) {
							bToneMapping			   = false;
							bAutomaticAverageLuminance = false;
							bBloom					   = false;
							bGammaCorrection		   = false;
							pHDRTextureBuffer = static_cast<TextureBufferRectangle*>(m_pHDRBloom->GetTextureBuffer());
						}
					}

					// Set the current render target back to the original one?
					if (GetFlags() & WriteToOriginalSurface)
						cRenderer.SetRenderTarget(pSRPBegin->GetOriginalRenderTarget());
					else
						pSRPBegin->SwapRenderTargets();

					// Reset the program flags
					m_cProgramFlags.Reset();

					// Set program flags
					if (bToneMapping) {
						PL_ADD_FS_FLAG(m_cProgramFlags, FS_TONE_MAPPING)
						if (bAutomaticAverageLuminance) {
							PL_ADD_FS_FLAG(m_cProgramFlags, FS_AUTOMATIC_AVERAGE_LUMINANCE)
							if (GetFlags() & UseVertexTextureFetch) {
								PL_ADD_VS_FLAG(m_cProgramFlags, VS_AUTOMATIC_AVERAGE_LUMINANCE_VTF)
								PL_ADD_FS_FLAG(m_cProgramFlags, FS_AUTOMATIC_AVERAGE_LUMINANCE_VTF)
							}
						}
					}
					if (bBloom)
						PL_ADD_FS_FLAG(m_cProgramFlags, FS_BLOOM)
					if (bGammaCorrection)
						PL_ADD_FS_FLAG(m_cProgramFlags, FS_GAMMA_CORRECTION)

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
							pGeneratedProgramUserData->pVertexPosition			= pProgram->GetAttribute(sVertexPosition);
							// Vertex shader uniforms
							static const String sTextureSize = "TextureSize";
							pGeneratedProgramUserData->pTextureSize				= pProgram->GetUniform(sTextureSize);
							static const String sAverageLuminanceTexture = "AverageLuminanceTexture";
							pGeneratedProgramUserData->pAverageLuminanceTexture	= pProgram->GetUniform(sAverageLuminanceTexture);
							// Fragment shader uniforms
							static const String sLuminanceConvert = "LuminanceConvert";
							pGeneratedProgramUserData->pLuminanceConvert		= pProgram->GetUniform(sLuminanceConvert);
							static const String sKey = "Key";
							pGeneratedProgramUserData->pKey						= pProgram->GetUniform(sKey);
							static const String sWhiteLevel = "WhiteLevel";
							pGeneratedProgramUserData->pWhiteLevel				= pProgram->GetUniform(sWhiteLevel);
							static const String sAverageLuminance = "AverageLuminance";
							pGeneratedProgramUserData->pAverageLuminance		= pProgram->GetUniform(sAverageLuminance);
							static const String sBloomFactor = "BloomFactor";
							pGeneratedProgramUserData->pBloomFactor				= pProgram->GetUniform(sBloomFactor);
							static const String sBloomDownscale = "BloomDownscale";
							pGeneratedProgramUserData->pBloomDownscale			= pProgram->GetUniform(sBloomDownscale);
							static const String sBloomTexture = "BloomTexture";
							pGeneratedProgramUserData->pBloomTexture			= pProgram->GetUniform(sBloomTexture);
							static const String sInvGamma = "InvGamma";
							pGeneratedProgramUserData->pInvGamma				= pProgram->GetUniform(sInvGamma);
							static const String sHDRTexture = "HDRTexture";
							pGeneratedProgramUserData->pHDRTexture				= pProgram->GetUniform(sHDRTexture);
						}

						// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
						if (pGeneratedProgramUserData->pVertexPosition)
							pGeneratedProgramUserData->pVertexPosition->Set(pVertexBuffer, PLRenderer::VertexBuffer::Position);

						// Set texture size
						if (pGeneratedProgramUserData->pTextureSize)
							pGeneratedProgramUserData->pTextureSize->Set(pHDRTextureBuffer->GetSize());

						// Set tone mapping fragment shader parameters
						if (bToneMapping) { 
							// Set the "LuminanceConvert" fragment shader parameter
							if (pGeneratedProgramUserData->pLuminanceConvert)
								pGeneratedProgramUserData->pLuminanceConvert->Set(LuminanceConvert.Get());

							// Set the "Key" fragment shader parameter
							if (pGeneratedProgramUserData->pKey)
								pGeneratedProgramUserData->pKey->Set((Key > 0.0f) ? Key : 0.0f);

							// Set the "WhiteLevel" fragment shader parameter
							if (pGeneratedProgramUserData->pWhiteLevel)
								pGeneratedProgramUserData->pWhiteLevel->Set((WhiteLevel > 0.0f) ? WhiteLevel : 0.0f);

							// Set the "AverageLuminance" fragment shader parameter
							if (bAutomaticAverageLuminance) {
								if (pGeneratedProgramUserData->pAverageLuminanceTexture) {
									const int nTextureUnit = pGeneratedProgramUserData->pAverageLuminanceTexture->Set(pHDRAverageLuminanceTextureBuffer);
									if (nTextureUnit >= 0) {
										cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU,  TextureAddressing::Wrap);
										cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV,  TextureAddressing::Wrap);
										cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
										cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
										cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
									}
								}
							} else {
								if (pGeneratedProgramUserData->pAverageLuminance)
									pGeneratedProgramUserData->pAverageLuminance->Set(AverageLuminance);
							}
						}

						// Set bloom fragment shader parameters
						if (bBloom) {
							// Set the "BloomFactor" fragment shader parameter
							if (pGeneratedProgramUserData->pBloomFactor)
								pGeneratedProgramUserData->pBloomFactor->Set(BloomFactor);

							// Set the "BloomDownscale" fragment shader parameter
							if (pGeneratedProgramUserData->pBloomDownscale)
								pGeneratedProgramUserData->pBloomDownscale->Set(BloomDownscale);

							// Set the "BloomTexture" fragment shader parameter
							if (pGeneratedProgramUserData->pBloomTexture) {
								const int nTextureUnit = pGeneratedProgramUserData->pBloomTexture->Set(m_pHDRBloom->GetTextureBuffer());
								if (nTextureUnit >= 0) {
									cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Clamp);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Clamp);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::Linear);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::Linear);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
								}
							}
						}

						// Set the "InvGamma" fragment shader parameter
						if (pGeneratedProgramUserData->pInvGamma) { 
							float fInvGamma = Gamma;
							if (fInvGamma > 0.0f)
								fInvGamma = 1.0f/fInvGamma;
							pGeneratedProgramUserData->pInvGamma->Set((fInvGamma > FLT_MIN) ? fInvGamma : FLT_MIN);
						}

						// Set the "HDRTexture" fragment shader parameter
						if (pGeneratedProgramUserData->pHDRTexture) {
							const int nTextureUnit = pGeneratedProgramUserData->pHDRTexture->Set(pHDRTextureBuffer);
							if (nTextureUnit >= 0) {
								cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Clamp);
								cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Clamp);
								cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
								cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
								cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
							}
						}

						// Draw the fullscreen quad
						m_pFullscreenQuad->Draw();
					}

					// Restore fixed fill mode render state
					cRenderer.SetRenderState(RenderState::FixedFillMode, nFixedFillModeBackup);

					// Debug feature: NAN values fixed?
					if (nNumOfFixedNANValues) {
						// Get the font
						Font *pFont = cRenderer.GetFontManager().GetDefaultFontTexture();
						if (pFont) {
							// Setup render states
							cRenderer.GetRendererContext().GetEffectManager().Use();
							cRenderer.SetRenderState(RenderState::ZEnable,      false);
							cRenderer.SetRenderState(RenderState::ZWriteEnable, false);

							// Begin 2D mode
							DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
							cDrawHelpers.Begin2DMode(0.0f, 0.0f, 800.0f, 600.0f);

								// Draw the text
								cDrawHelpers.DrawText(*pFont, String::Format("%d NAN Detected!", nNumOfFixedNANValues), PLGraphics::Color4::Red, Vector2(400.0f, 300.0f), Font::CenterText, Vector2(5.0f, 5.0f));

							// End 2D mode
							cDrawHelpers.End2DMode();
						}
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
