/*********************************************************\
 *  File: SRPDeferredAmbient.cpp                         *
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
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ProgramAttribute.h>
#include <PLRenderer/Renderer/TextureBufferRectangle.h>
#include <PLRenderer/Effect/EffectManager.h>
#include "PLCompositing/FullscreenQuad.h"
#include "PLCompositing/Shaders/Deferred/SRPDeferredGBuffer.h"
#include "PLCompositing/Shaders/Deferred/SRPDeferredAmbient.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
using namespace PLScene;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SRPDeferredAmbient)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPDeferredAmbient::SRPDeferredAmbient() :
	ShaderLanguage(this),
	AmbientColor(this),
	Flags(this),
	m_pProgramGenerator(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
SRPDeferredAmbient::~SRPDeferredAmbient()
{
	// Destroy the program generator
	if (m_pProgramGenerator)
		delete m_pProgramGenerator;
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRPDeferredAmbient::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Get the instance of the "PLCompositing::SRPDeferredGBuffer" scene renderer pass
	SRPDeferredGBuffer *pSRPDeferredGBuffer = GetGBuffer();
	if (pSRPDeferredGBuffer) {
		// Get the fullscreen quad instance
		FullscreenQuad *pFullscreenQuad = pSRPDeferredGBuffer->GetFullscreenQuad();
		if (pFullscreenQuad) {
			// Get the vertex buffer of the fullscreen quad
			VertexBuffer *pVertexBuffer = pFullscreenQuad->GetVertexBuffer();
			if (pVertexBuffer) {
				// Get the texture buffer to use
				TextureBufferRectangle *pTextureBuffer = pSRPDeferredGBuffer->GetRenderTargetTextureBuffer(0);
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
							#include "SRPDeferredAmbient_GLSL.h"
							m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sDeferredAmbient_GLSL_VS, "110", sDeferredAmbient_GLSL_FS, "110");	// OpenGL 2.0 ("#version 110")
						} else if (sShaderLanguage == "Cg") {
							#include "SRPDeferredAmbient_Cg.h"
							m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sDeferredAmbient_Cg_VS, "arbvp1", sDeferredAmbient_Cg_FS, "arbfp1");
						}
					}

					// If there's no program generator, we don't need to continue
					if (m_pProgramGenerator) {
						// Reset all render states to default
						cRenderer.GetRendererContext().GetEffectManager().Use();

						// Use stencil buffer?
						if (!(GetFlags() & NoStencil)) {
							// Enable stencil test - ignore pixels tagged with 1 within the stencil buffer
							cRenderer.SetRenderState(RenderState::StencilEnable, true);
							cRenderer.SetRenderState(RenderState::StencilRef,    1);
							cRenderer.SetRenderState(RenderState::StencilFunc,   Compare::NotEqual);
						}

						// Reset the program flags
						m_cProgramFlags.Reset();

						// Albedo used?
						if (!(GetFlags() & NoAlbedo))
							PL_ADD_FS_FLAG(m_cProgramFlags, FS_ALBEDO)

						// Ambient occlusion used?
						if (!(GetFlags() & NoAmbientOcclusion))
							PL_ADD_FS_FLAG(m_cProgramFlags, FS_AMBIENTOCCLUSION)

						// Self illumination used?
						if (pSRPDeferredGBuffer->IsColorTarget3Used() && !(GetFlags() & NoSelfIllumination))
							PL_ADD_FS_FLAG(m_cProgramFlags, FS_SELFILLUMINATION)

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
								pGeneratedProgramUserData->pVertexPosition		= pProgram->GetAttribute(sVertexPosition);
								// Vertex shader uniforms
								static const String sTextureSize = "TextureSize";
								pGeneratedProgramUserData->pTextureSize			= pProgram->GetUniform(sTextureSize);
								// Fragment shader uniforms
								static const String sAmbientColor = "AmbientColor";
								pGeneratedProgramUserData->pAmbientColor		= pProgram->GetUniform(sAmbientColor);
								static const String sAlbedoMap = "AlbedoMap";
								pGeneratedProgramUserData->pAlbedoMap			= pProgram->GetUniform(sAlbedoMap);
								static const String sSelfIlluminationMap = "SelfIlluminationMap";
								pGeneratedProgramUserData->pSelfIlluminationMap	= pProgram->GetUniform(sSelfIlluminationMap);
							}

							// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
							if (pGeneratedProgramUserData->pVertexPosition)
								pGeneratedProgramUserData->pVertexPosition->Set(pVertexBuffer, PLRenderer::VertexBuffer::Position);

							// Set texture size
							if (pGeneratedProgramUserData->pTextureSize)
								pGeneratedProgramUserData->pTextureSize->Set(pTextureBuffer->GetSize());

							// Ambient color
							if (pGeneratedProgramUserData->pAmbientColor)
								pGeneratedProgramUserData->pAmbientColor->Set(AmbientColor.Get());

							// Albedo map
							if (pGeneratedProgramUserData->pAlbedoMap) {
								const int nTextureUnit = pGeneratedProgramUserData->pAlbedoMap->Set(pTextureBuffer);
								if (nTextureUnit >= 0) {
									cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU,  TextureAddressing::Clamp);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV,  TextureAddressing::Clamp);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
								}
							}

							// Self illumination map
							if (pGeneratedProgramUserData->pSelfIlluminationMap) {
								const int nTextureUnit = pGeneratedProgramUserData->pSelfIlluminationMap->Set(pSRPDeferredGBuffer->GetRenderTargetTextureBuffer(3));
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
							pFullscreenQuad->Draw(true);
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
