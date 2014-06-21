/*********************************************************\
 *  File: HDRBloom.cpp                                   *
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
#include <PLGraphics/Color/Color3.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Program.h>
#include <PLRenderer/Renderer/VertexShader.h>
#include <PLRenderer/Renderer/FragmentShader.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ShaderLanguage.h>
#include <PLRenderer/Renderer/TextureBuffer2D.h>
#include <PLRenderer/Renderer/ProgramAttribute.h>
#include <PLRenderer/Renderer/TextureBufferRectangle.h>
#include "PLCompositing/FullscreenQuad.h"
#include "PLCompositing/Shaders/HDR/HDRBloom.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
HDRBloom::HDRBloom(Renderer &cRenderer) :
	EventHandlerDirty(&HDRBloom::OnDirty, this),
	m_pRenderer(&cRenderer),
	m_pFullscreenQuad(nullptr),
	m_bResultIndex(0),
	m_pDownscaleProgramGenerator(nullptr),
	m_pBloomVertexShader(nullptr),
	m_pBloomFragmentShader(nullptr),
	m_pBloomProgram(nullptr),
	m_pBloomPositionProgramAttribute(nullptr),
	m_pBloomTextureSizeProgramUniform(nullptr),
	m_pBloomUVScaleProgramUniform(nullptr),
	m_pBloomHDRTextureProgramUniform(nullptr)
{
	// Init data
	m_pRenderTarget[0] = m_pRenderTarget[1] = nullptr;
}

/**
*  @brief
*    Destructor
*/
HDRBloom::~HDRBloom()
{
	// Destroy the fullscreen quad
	if (m_pFullscreenQuad)
		delete m_pFullscreenQuad;

	// Destroy the render targets
	for (int i=0; i<2; i++) {
		if (m_pRenderTarget[i])
			delete m_pRenderTarget[i];
	}

	// Destroy the downscale program generator
	if (m_pDownscaleProgramGenerator)
		delete m_pDownscaleProgramGenerator;

	// Destroy the bloom vertex and fragment shader
	if (m_pBloomProgram)
		delete m_pBloomProgram;
	if (m_pBloomVertexShader)
		delete m_pBloomVertexShader;
	if (m_pBloomFragmentShader)
		delete m_pBloomFragmentShader;
}

/**
*  @brief
*    Calculates the bloom
*/
void HDRBloom::CalculateBloom(const String &sShaderLanguage, TextureBufferRectangle &cOriginalTexture, float fBrightThreshold, bool bToneMapping, bool bAutomaticAverageLuminance, const Color3 &cLuminanceConvert,
							  float fKey, float fWhiteLevel, float fAverageLuminance, TextureBuffer *pHDRAverageLuminanceTextureBuffer, uint32 nBloomBlurPasses, float fDownscale, bool bUseVertexTextureFetch)
{
	// Get the internal texture format to use
	const TextureBuffer::EPixelFormat nInternalFormat = cOriginalTexture.GetFormat();

	{ // Create/update the two render targets
		// Get the size of the original HDR texture
		const Vector2i vRTSize = cOriginalTexture.GetSize()/fDownscale;
		if (vRTSize.x != 0 && vRTSize.y != 0) {
			for (int i=0; i<2; i++) {
				// Render target size change?
				if (m_pRenderTarget[i] && (m_pRenderTarget[i]->GetSize() != vRTSize || m_pRenderTarget[i]->GetFormat() != nInternalFormat)) {
					// Destroy the downscale render target
					if (m_pRenderTarget[i]) {
						delete m_pRenderTarget[i];
						m_pRenderTarget[i] = nullptr;
					}
				}

				// Create the downscale render target right now?
				if (!m_pRenderTarget[i])
					m_pRenderTarget[i] = m_pRenderer->CreateSurfaceTextureBufferRectangle(vRTSize, nInternalFormat, SurfaceTextureBuffer::NoMultisampleAntialiasing);
			}
		}
	}

	// Create the fullscreen quad instance if required
	if (!m_pFullscreenQuad)
		m_pFullscreenQuad = new FullscreenQuad(*m_pRenderer);

	// Get the vertex buffer of the fullscreen quad
	VertexBuffer *pVertexBuffer = m_pFullscreenQuad->GetVertexBuffer();
	if (pVertexBuffer) {
		// Get the shader language to use
		String sUsedShaderLanguage = sShaderLanguage;
		if (!sUsedShaderLanguage.GetLength())
			sUsedShaderLanguage = m_pRenderer->GetDefaultShaderLanguage();

		{ // First step: Downscale, apply bright pass filter and tone mapping
			// Create the downscale program generator if there's currently no instance of it
			if (!m_pDownscaleProgramGenerator || m_pDownscaleProgramGenerator->GetShaderLanguage() != sUsedShaderLanguage) {
				// If there's an previous instance of the downscale program generator, destroy it first
				if (m_pDownscaleProgramGenerator) {
					delete m_pDownscaleProgramGenerator;
					m_pDownscaleProgramGenerator = nullptr;
				}

				// Choose the shader source codes depending on the requested shader language
				if (sUsedShaderLanguage == "GLSL") {
					#include "HDRBloom_GLSL.h"
					m_pDownscaleProgramGenerator = new ProgramGenerator(*m_pRenderer, sUsedShaderLanguage, sHDRBloom_GLSL_VS, "120", sHDRBloom_GLSL_FS_Downscale, "120");	// OpenGL 2.1 ("#version 120")
				} else if (sUsedShaderLanguage == "Cg") {
					#include "HDRBloom_Cg.h"
					m_pDownscaleProgramGenerator = new ProgramGenerator(*m_pRenderer, sUsedShaderLanguage, sHDRBloom_Cg_VS, "glslv", sHDRBloom_Cg_FS_Downscale, "glslf");
				}
			}

			// If there's no downscale program generator, we don't need to continue
			if (m_pDownscaleProgramGenerator) {
				// Make the render target 0 to the current render target
				m_pRenderer->SetRenderTarget(m_pRenderTarget[0]);
				m_bResultIndex = 0;

				// Reset the downscale program flags
				m_cDownscaleProgramFlags.Reset();

				// Set downscale program flags
				if (bToneMapping) {
					PL_ADD_FS_FLAG(m_cDownscaleProgramFlags, FS_TONE_MAPPING)
					if (bAutomaticAverageLuminance) {
						PL_ADD_FS_FLAG(m_cDownscaleProgramFlags, FS_AUTOMATIC_AVERAGE_LUMINANCE)
						if (bUseVertexTextureFetch) {
							PL_ADD_VS_FLAG(m_cDownscaleProgramFlags, VS_AUTOMATIC_AVERAGE_LUMINANCE_VTF)
							PL_ADD_FS_FLAG(m_cDownscaleProgramFlags, FS_AUTOMATIC_AVERAGE_LUMINANCE_VTF)
						}
					}
				}

				// Get a downscale program instance from the downscale program generator using the given program flags
				ProgramGenerator::GeneratedProgram *pGeneratedProgram = m_pDownscaleProgramGenerator->GetProgram(m_cDownscaleProgramFlags);

				// Make our downscale program to the current one
				if (pGeneratedProgram && m_pRenderer->SetProgram(pGeneratedProgram->pProgram)) {
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
						static const String sBrightThreshold = "BrightThreshold";
						pGeneratedProgramUserData->pBrightThreshold			= pProgram->GetUniform(sBrightThreshold);
						static const String sHDRTexture = "HDRTexture";
						pGeneratedProgramUserData->pHDRTexture				= pProgram->GetUniform(sHDRTexture);
					}

					// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
					if (pGeneratedProgramUserData->pVertexPosition)
						pGeneratedProgramUserData->pVertexPosition->Set(pVertexBuffer, PLRenderer::VertexBuffer::Position);

					// Set texture size
					if (pGeneratedProgramUserData->pTextureSize)
						pGeneratedProgramUserData->pTextureSize->Set(cOriginalTexture.GetSize());

					// Set tone mapping fragment shader parameters
					if (bToneMapping) { 
						// Set the "LuminanceConvert" fragment shader parameter
						if (pGeneratedProgramUserData->pLuminanceConvert)
							pGeneratedProgramUserData->pLuminanceConvert->Set(cLuminanceConvert);

						// Set the "Key" fragment shader parameter
						if (pGeneratedProgramUserData->pKey)
							pGeneratedProgramUserData->pKey->Set((fKey > 0.0f) ? fKey : 0.0f);

						// Set the "WhiteLevel" fragment shader parameter
						if (pGeneratedProgramUserData->pWhiteLevel)
							pGeneratedProgramUserData->pWhiteLevel->Set((fWhiteLevel > 0.0f) ? fWhiteLevel : 0.0f);

						// Set the "AverageLuminance" fragment shader parameter
						if (bAutomaticAverageLuminance) {
							if (pGeneratedProgramUserData->pAverageLuminanceTexture) {
								const int nTextureUnit = pGeneratedProgramUserData->pAverageLuminanceTexture->Set(pHDRAverageLuminanceTextureBuffer);
								if (nTextureUnit >= 0) {
									m_pRenderer->SetSamplerState(nTextureUnit, Sampler::AddressU,  TextureAddressing::Clamp);
									m_pRenderer->SetSamplerState(nTextureUnit, Sampler::AddressV,  TextureAddressing::Clamp);
									m_pRenderer->SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
									m_pRenderer->SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
									m_pRenderer->SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
								}
							}
						} else {
							if (pGeneratedProgramUserData->pAverageLuminance)
								pGeneratedProgramUserData->pAverageLuminance->Set(fAverageLuminance);
						}
					}

					// Set the "BrightThreshold" fragment shader parameter
					if (pGeneratedProgramUserData->pBrightThreshold)
						pGeneratedProgramUserData->pBrightThreshold->Set(fBrightThreshold);

					// Set the "HDRTexture" fragment shader parameter
					if (pGeneratedProgramUserData->pHDRTexture) {
						const int nTextureUnit = pGeneratedProgramUserData->pHDRTexture->Set(&cOriginalTexture);
						if (nTextureUnit >= 0) {
							m_pRenderer->SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Clamp);
							m_pRenderer->SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Clamp);
							m_pRenderer->SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::Linear);
							m_pRenderer->SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::Linear);
							m_pRenderer->SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
						}
					}

					// Draw the fullscreen quad
					m_pFullscreenQuad->Draw();
				}
			}
		}

		{ // Gaussian convolution filter to bloom
			// Create the shaders and programs right now?
			if (!m_pBloomVertexShader || m_pBloomVertexShader->GetShaderLanguage() != sUsedShaderLanguage) {
				// If there's an previous instance of the program, destroy it first
				if (m_pBloomProgram) {
					delete m_pBloomProgram;
					m_pBloomProgram = nullptr;
				}
				if (m_pBloomVertexShader) {
					delete m_pBloomVertexShader;
					m_pBloomVertexShader = nullptr;
				}
				if (m_pBloomFragmentShader) {
					delete m_pBloomFragmentShader;
					m_pBloomFragmentShader = nullptr;
				}
				m_pBloomPositionProgramAttribute	= nullptr;
				m_pBloomTextureSizeProgramUniform	= nullptr;
				m_pBloomUVScaleProgramUniform		= nullptr;
				m_pBloomHDRTextureProgramUniform	= nullptr;

				// Get the shader language instance
				ShaderLanguage *pShaderLanguage = m_pRenderer->GetShaderLanguage(sUsedShaderLanguage);
				if (pShaderLanguage) {
					// Shader source code
					String sVertexShaderSourceCode;
					String sFragmentShaderSourceCode;
					if (sUsedShaderLanguage == "GLSL") {
						#include "HDRBloom_GLSL.h"
						sVertexShaderSourceCode	  = "#version 120\n" + sHDRBloom_GLSL_VS;	// OpenGL 2.1 ("#version 120")
						sFragmentShaderSourceCode = sHDRBloom_GLSL_FS;
					} else if (sUsedShaderLanguage == "Cg") {
						#include "HDRBloom_Cg.h"
						sVertexShaderSourceCode	  = sHDRBloom_Cg_VS;
						sFragmentShaderSourceCode = sHDRBloom_Cg_FS;
					}

					// Create a vertex shader instance
					m_pBloomVertexShader = pShaderLanguage->CreateVertexShader(sVertexShaderSourceCode);

					// Create a fragment shader instance
					m_pBloomFragmentShader = pShaderLanguage->CreateFragmentShader(sFragmentShaderSourceCode);

					// Create a program instance and assign the created vertex and fragment shaders to it
					m_pBloomProgram = pShaderLanguage->CreateProgram(m_pBloomVertexShader, m_pBloomFragmentShader);
					if (m_pBloomProgram) {
						// Add our nark which will inform us as soon as the program gets dirty
						m_pBloomProgram->EventDirty.Connect(EventHandlerDirty);

						// Get attributes and uniforms
						OnDirty(m_pBloomProgram);
					}
				}
			}

			// Make the bloom GPU program to the currently used one
			if (m_pRenderer->SetProgram(m_pBloomProgram)) {
				// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
				if (m_pBloomPositionProgramAttribute)
					m_pBloomPositionProgramAttribute->Set(pVertexBuffer, PLRenderer::VertexBuffer::Position);

				// Set the "TextureSize" fragment shader parameter - both render targets have the same size
				if (m_pBloomTextureSizeProgramUniform)
					m_pBloomTextureSizeProgramUniform->Set(m_pRenderTarget[0]->GetSize());

				// Horizontal and vertical blur
				for (uint32 i=0; i<nBloomBlurPasses; i++) {
					// Make the render target 1 to the current render target
					m_pRenderer->SetRenderTarget(m_pRenderTarget[!m_bResultIndex]);

					// Set the "UVScale" fragment shader parameter
					if (m_pBloomUVScaleProgramUniform) {
						if (i%2 != 0)
							m_pBloomUVScaleProgramUniform->Set(0.0f, 1.0f);
						else
							m_pBloomUVScaleProgramUniform->Set(1.0f, 0.0f);
					}

					// Set the "HDRTexture" fragment shader parameter
					if (m_pBloomHDRTextureProgramUniform) {
						const int nTextureUnit = m_pBloomHDRTextureProgramUniform->Set(m_pRenderTarget[m_bResultIndex]->GetTextureBuffer());
						if (nTextureUnit >= 0) {
							m_pRenderer->SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
							m_pRenderer->SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
							m_pRenderer->SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::Linear);
							m_pRenderer->SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::Linear);
							m_pRenderer->SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
						}
					}

					// Draw the fullscreen quad
					m_pFullscreenQuad->Draw();

					// The result is now within the other render target
					m_bResultIndex = !m_bResultIndex;
				}
			}
		}
	}
}

/**
*  @brief
*    Returns the rectangle texture buffer storing the bloom
*/
TextureBuffer *HDRBloom::GetTextureBuffer() const
{
	return m_pRenderTarget[m_bResultIndex] ? m_pRenderTarget[m_bResultIndex]->GetTextureBuffer() : nullptr;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a program became dirty
*/
void HDRBloom::OnDirty(Program *pProgram)
{
	// Get attributes and uniforms
	if (pProgram == m_pBloomProgram) {
		m_pBloomPositionProgramAttribute  = m_pBloomProgram->GetAttribute("VertexPosition");
		m_pBloomTextureSizeProgramUniform = m_pBloomProgram->GetUniform("TextureSize");
		m_pBloomUVScaleProgramUniform	  = m_pBloomProgram->GetUniform("UVScale");
		m_pBloomHDRTextureProgramUniform  = m_pBloomProgram->GetUniform("HDRTexture");
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
