/*********************************************************\
 *  File: SRPDeferredGodRays.cpp                         *
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
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Program.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ProgramAttribute.h>
#include <PLRenderer/Renderer/TextureBufferRectangle.h>
#include <PLRenderer/Effect/EffectManager.h>
#include "PLCompositing/FullscreenQuad.h"
#include "PLCompositing/Shaders/Deferred/SRPDeferredGBuffer.h"
#include "PLCompositing/Shaders/Deferred/SRPDeferredGodRays.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLScene;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SRPDeferredGodRays)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPDeferredGodRays::SRPDeferredGodRays() :
	ShaderLanguage(this),
	NumberOfSamples(this),
	Density(this),
	Weight(this),
	Decay(this),
	LightPosition(this),
	Color(this),
	Flags(this),
	m_pProgramGenerator(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
SRPDeferredGodRays::~SRPDeferredGodRays()
{
	// Destroy the program generator
	if (m_pProgramGenerator)
		delete m_pProgramGenerator;
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRPDeferredGodRays::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Skip black god rays, they have no visible influence!
	if (Color.Get() != Color3::Black && NumberOfSamples != 0) {
		// Get the instance of the "PLCompositing::SRPDeferredGBuffer" scene renderer pass and check whether or not color target 3 has real information in it
		SRPDeferredGBuffer *pSRPDeferredGBuffer = GetGBuffer();
		if (pSRPDeferredGBuffer && pSRPDeferredGBuffer->IsColorTarget3Used()) {
			// Get the fullscreen quad instance
			FullscreenQuad *pFullscreenQuad = pSRPDeferredGBuffer->GetFullscreenQuad();
			if (pFullscreenQuad) {
				// Get the vertex buffer of the fullscreen quad
				VertexBuffer *pVertexBuffer = pFullscreenQuad->GetVertexBuffer();
				if (pVertexBuffer) {
					// Get the texture buffer to use
					TextureBufferRectangle *pTextureBuffer = pSRPDeferredGBuffer->GetRenderTargetTextureBuffer(3);
					if (pTextureBuffer) {
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
								#include "SRPDeferredGodRays_GLSL.h"
								m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sDeferredGodRays_GLSL_VS, "110", sDeferredGodRays_GLSL_FS, "110");	// OpenGL 2.0 ("#version 110")
							} else if (sShaderLanguage == "Cg") {
								#include "SRPDeferredGodRays_Cg.h"
								m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sDeferredGodRays_Cg_VS, "glslv", sDeferredGodRays_Cg_FS, "glslf");
							}
						}

						// If there's no program generator, we don't need to continue
						if (m_pProgramGenerator) {
							// Reset all render states to default
							cRenderer.GetRendererContext().GetEffectManager().Use();

							// Reset the program flags
							m_cProgramFlags.Reset();

							// Set program flags
							if (!(GetFlags() & NoDiscard))
								PL_ADD_FS_FLAG(m_cProgramFlags, FS_DISCARD)

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
									// Vertex shader uniforms
									static const String sTextureSize = "TextureSize";
									pGeneratedProgramUserData->pTextureSize		= pProgram->GetUniform(sTextureSize);
									// Fragment shader uniforms
									static const String sNumberOfSamples = "NumberOfSamples";
									pGeneratedProgramUserData->pNumberOfSamples	= pProgram->GetUniform(sNumberOfSamples);
									static const String sDensity = "Density";
									pGeneratedProgramUserData->pDensity			= pProgram->GetUniform(sDensity);
									static const String sWeight = "Weight";
									pGeneratedProgramUserData->pWeight			= pProgram->GetUniform(sWeight);
									static const String sDecay = "Decay";
									pGeneratedProgramUserData->pDecay			= pProgram->GetUniform(sDecay);
									static const String sLightPosition = "LightPosition";
									pGeneratedProgramUserData->pLightPosition	= pProgram->GetUniform(sLightPosition);
									static const String sColor = "Color";
									pGeneratedProgramUserData->pColor			= pProgram->GetUniform(sColor);
									static const String sMap = "Map";
									pGeneratedProgramUserData->pMap				= pProgram->GetUniform(sMap);
								}

								// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
								if (pGeneratedProgramUserData->pVertexPosition)
									pGeneratedProgramUserData->pVertexPosition->Set(pVertexBuffer, PLRenderer::VertexBuffer::Position);

								// Set texture size
								if (pGeneratedProgramUserData->pTextureSize)
									pGeneratedProgramUserData->pTextureSize->Set(pTextureBuffer->GetSize());

								// Set number of samples
								if (pGeneratedProgramUserData->pNumberOfSamples)
									pGeneratedProgramUserData->pNumberOfSamples->Set(static_cast<float>((NumberOfSamples > 0) ? NumberOfSamples : 1));	// (should be of integer type, but that's causing troubles on some GPU drivers, see PLCompositing diary entry 02.06.2011 for more details)

								// Set density
								if (pGeneratedProgramUserData->pDensity)
									pGeneratedProgramUserData->pDensity->Set((Density > FLT_MIN) ? Density : FLT_MIN);

								// Set weight
								if (pGeneratedProgramUserData->pWeight)
									pGeneratedProgramUserData->pWeight->Set(Weight);

								// Set decay
								if (pGeneratedProgramUserData->pDecay)
									pGeneratedProgramUserData->pDecay->Set(Decay);

								// Set light position
								if (pGeneratedProgramUserData->pLightPosition)
									pGeneratedProgramUserData->pLightPosition->Set(LightPosition.Get().x*pTextureBuffer->GetSize().x, LightPosition.Get().y*pTextureBuffer->GetSize().y);

								// Set color
								if (pGeneratedProgramUserData->pColor)
									pGeneratedProgramUserData->pColor->Set(Color.Get());

								// Map
								if (pGeneratedProgramUserData->pMap) {
									const int nTextureUnit = pGeneratedProgramUserData->pMap->Set(pTextureBuffer);
									if (nTextureUnit >= 0) {
										cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU,  TextureAddressing::Clamp);
										cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV,  TextureAddressing::Clamp);
										cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
										cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
										cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
									}
								}

								// Draw the fullscreen quad
								cRenderer.SetRenderState(RenderState::ScissorTestEnable, false);
								if (!(GetFlags() & NoBlending)) {
									// Set blend mode
									cRenderer.SetRenderState(RenderState::BlendEnable,	true);
									cRenderer.SetRenderState(RenderState::SrcBlendFunc,	BlendFunc::One);
									cRenderer.SetRenderState(RenderState::DstBlendFunc,	BlendFunc::One);
								}
								pFullscreenQuad->Draw(true);
							}
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
