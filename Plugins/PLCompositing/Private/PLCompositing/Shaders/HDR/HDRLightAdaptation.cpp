/*********************************************************\
 *  File: HDRLightAdaptation.cpp                         *
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
#include <PLGraphics/Color/Color3.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Program.h>
#include <PLRenderer/Renderer/VertexShader.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ShaderLanguage.h>
#include <PLRenderer/Renderer/ProgramAttribute.h>
#include <PLRenderer/Renderer/FragmentShader.h>
#include <PLRenderer/Renderer/TextureBuffer2D.h>
#include <PLRenderer/Renderer/TextureBufferRectangle.h>
#include "PLCompositing/FullscreenQuad.h"
#include "PLCompositing/Shaders/HDR/HDRLightAdaptation.h"


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
HDRLightAdaptation::HDRLightAdaptation(Renderer &cRenderer) :
	EventHandlerDirty(&HDRLightAdaptation::OnDirty, this),
	m_pRenderer(&cRenderer),
	m_pFullscreenQuad(nullptr),
	m_pVertexShader(nullptr),
	m_pFragmentShader(nullptr),
	m_pProgram(nullptr),
	m_pPositionProgramAttribute(nullptr),
	m_pFactorProgramUniform(nullptr),
	m_pPreviousTextureProgramUniform(nullptr),
	m_pCurrentTextureProgramUniform(nullptr),
	m_bPreviousIndex(0)
{
	m_pLightAdaptationTextureBuffer2D[0] = m_pLightAdaptationTextureBuffer2D[1] = nullptr;
}

/**
*  @brief
*    Destructor
*/
HDRLightAdaptation::~HDRLightAdaptation()
{
	// Destroy the fullscreen quad
	if (m_pFullscreenQuad)
		delete m_pFullscreenQuad;

	// Destroy shaders
	if (m_pProgram)
		delete m_pProgram;
	if (m_pFragmentShader)
		delete m_pFragmentShader;
	if (m_pVertexShader)
		delete m_pVertexShader;

	// Destroy the render target
	for (int i=0; i<2; i++) {
		if (m_pLightAdaptationTextureBuffer2D[i])
			delete m_pLightAdaptationTextureBuffer2D[i];
	}
}

/**
*  @brief
*    Calculates the light adaptation
*/
void HDRLightAdaptation::CalculateLightAdaptation(const String &sShaderLanguage, PLRenderer::TextureBuffer &cAverageLuminanceTextureBuffer2D, float fTauTimeDifference)
{
	// Get the internal texture format to use
	const TextureBuffer::EPixelFormat nInternalFormat = (cAverageLuminanceTextureBuffer2D.GetFormat() == TextureBuffer::R16G16B16A16F) ? TextureBuffer::L16F : TextureBuffer::L32F;

	// Get the shader language to use
	String sUsedShaderLanguage = sShaderLanguage;
	if (!sUsedShaderLanguage.GetLength())
		sUsedShaderLanguage = m_pRenderer->GetDefaultShaderLanguage();

	// Create the GPU program right now?
	if (!m_pProgram || m_pProgram->GetShaderLanguage() != sUsedShaderLanguage) {
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
		m_pPositionProgramAttribute			= nullptr;
		m_pFactorProgramUniform				= nullptr;
		m_pPreviousTextureProgramUniform	= nullptr;
		m_pCurrentTextureProgramUniform		= nullptr;

		// Get the shader language instance
		ShaderLanguage *pShaderLanguage = m_pRenderer->GetShaderLanguage(sUsedShaderLanguage);
		if (pShaderLanguage) {
			// Shader source code
			String sVertexShaderSourceCode;
			String sFragmentShaderSourceCode;
			if (sUsedShaderLanguage == "GLSL") {
				#include "HDRLightAdaptation_GLSL.h"
				sVertexShaderSourceCode   = sHDRLightAdaptation_GLSL_VS;
				sFragmentShaderSourceCode = sHDRLightAdaptation_GLSL_FS;
			} else if (sUsedShaderLanguage == "Cg") {
				#include "HDRLightAdaptation_Cg.h"
				sVertexShaderSourceCode   = sHDRLightAdaptation_Cg_VS;
				sFragmentShaderSourceCode = sHDRLightAdaptation_Cg_FS;
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

	// Make our program to the current one
	if (m_pRenderer->SetProgram(m_pProgram)) {
		// Create the fullscreen quad instance if required
		if (!m_pFullscreenQuad)
			m_pFullscreenQuad = new FullscreenQuad(*m_pRenderer);

		// Get the vertex buffer of the fullscreen quad
		VertexBuffer *pVertexBuffer = m_pFullscreenQuad->GetVertexBuffer();
		if (pVertexBuffer) {
			// Swap previous/current texture buffer
			m_bPreviousIndex = !m_bPreviousIndex;
			const bool bCurrentIndex = !m_bPreviousIndex;

			// Render format change?
			if (m_pLightAdaptationTextureBuffer2D[bCurrentIndex] && m_pLightAdaptationTextureBuffer2D[bCurrentIndex]->GetFormat() != nInternalFormat) {
				// Destroy the result render target
				if (m_pLightAdaptationTextureBuffer2D[bCurrentIndex]) {
					delete m_pLightAdaptationTextureBuffer2D[bCurrentIndex];
					m_pLightAdaptationTextureBuffer2D[bCurrentIndex] = nullptr;
				}
			}

			// Create the result render target right now?
			if (!m_pLightAdaptationTextureBuffer2D[bCurrentIndex])
				m_pLightAdaptationTextureBuffer2D[bCurrentIndex] = m_pRenderer->CreateSurfaceTextureBuffer2D(Vector2i::One, nInternalFormat, SurfaceTextureBuffer::NoMultisampleAntialiasing);

			// Make the result render target to the current render target
			m_pRenderer->SetRenderTarget(m_pLightAdaptationTextureBuffer2D[bCurrentIndex]);

			// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
			if (m_pPositionProgramAttribute)
				m_pPositionProgramAttribute->Set(pVertexBuffer, PLRenderer::VertexBuffer::Position);

			// Set the "Factor" fragment shader parameter
			if (m_pFactorProgramUniform)
				m_pFactorProgramUniform->Set(1.0f - Math::Exp(-fTauTimeDifference));

			// Set the "PreviousTexture" fragment shader parameter
			if (m_pPreviousTextureProgramUniform) {
				const int nTextureUnit = m_pPreviousTextureProgramUniform->Set(m_pLightAdaptationTextureBuffer2D[m_bPreviousIndex] ? m_pLightAdaptationTextureBuffer2D[m_bPreviousIndex]->GetTextureBuffer() : &cAverageLuminanceTextureBuffer2D);
				if (nTextureUnit >= 0) {
					m_pRenderer->SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
					m_pRenderer->SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
					m_pRenderer->SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
					m_pRenderer->SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
					m_pRenderer->SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
				}
			}

			// Set the "CurrentTexture" fragment shader parameter
			if (m_pCurrentTextureProgramUniform) {
				const int nTextureUnit = m_pCurrentTextureProgramUniform->Set(&cAverageLuminanceTextureBuffer2D);
				if (nTextureUnit >= 0) {
					m_pRenderer->SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
					m_pRenderer->SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
					m_pRenderer->SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
					m_pRenderer->SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
					m_pRenderer->SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
				}
			}

			// Draw the fullscreen quad
			m_pFullscreenQuad->Draw();
		}
	}
}

/**
*  @brief
*    Returns the 1x1 2D texture buffer storing the light adaptation
*/
TextureBuffer *HDRLightAdaptation::GetTextureBuffer() const
{
	return m_pLightAdaptationTextureBuffer2D[!m_bPreviousIndex] ? m_pLightAdaptationTextureBuffer2D[!m_bPreviousIndex]->GetTextureBuffer() : nullptr;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a program became dirty
*/
void HDRLightAdaptation::OnDirty(Program *pProgram)
{
	// Get attributes and uniforms
	if (pProgram == m_pProgram) {
		m_pPositionProgramAttribute		 = m_pProgram->GetAttribute("VertexPosition");
		m_pFactorProgramUniform			 = m_pProgram->GetUniform("Factor");
		m_pPreviousTextureProgramUniform = m_pProgram->GetUniform("PreviousTexture");
		m_pCurrentTextureProgramUniform  = m_pProgram->GetUniform("CurrentTexture");
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
