/*********************************************************\
 *  File: HDRAverageLuminance.cpp                        *
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
#include <PLMath/Math.h>
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
#include "PLCompositing/Shaders/HDR/HDRAverageLuminance.h"


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
HDRAverageLuminance::HDRAverageLuminance(Renderer &cRenderer) :
	EventHandlerDirty(&HDRAverageLuminance::OnDirty, this),
	m_pRenderer(&cRenderer),
	m_pFullscreenQuad(nullptr),
	m_pVertexShader(nullptr),
	m_pDownsampleLogFragmentShader(nullptr),
	m_pDownsampleLogProgram(nullptr),
	m_pDownsampleLogPositionProgramAttribute(nullptr),
	m_pDownsampleLogTextureSizeProgramUniform(nullptr),
	m_pDownsampleLogTextureProgramUniform(nullptr),
	m_pDownsampleLogLuminanceConvertProgramUniform(nullptr),
	m_pDownsampleLogEpsilonProgramUniform(nullptr),
	m_pDownsampleLogRenderTarget(nullptr),
	m_pDownsampleVertexShader(nullptr),
	m_pDownsampleFragmentShader(nullptr),
	m_pDownsampleProgram(nullptr),
	m_pDownsamplePositionProgramAttribute(nullptr),
	m_pDownsampleTextureSizeProgramUniform(nullptr),
	m_pDownsampleSizeProgramUniform(nullptr),
	m_pDownsampleTextureProgramUniform(nullptr),
	m_pDownsampleRenderTarget(nullptr),
	m_pDownsampleExpFragmentShader(nullptr),
	m_pDownsampleExpProgram(nullptr),
	m_pDownsampleExpPositionProgramAttribute(nullptr),
	m_pDownsampleExpTextureSizeProgramUniform(nullptr),
	m_pDownsampleExpTextureProgramUniform(nullptr),
	m_pAverageLuminanceTextureBuffer2D(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
HDRAverageLuminance::~HDRAverageLuminance()
{
	// Destroy the fullscreen quad
	if (m_pFullscreenQuad)
		delete m_pFullscreenQuad;

	// Destroy the downsample 2x2 log fragment shader and render target
	if (m_pDownsampleLogProgram)
		delete m_pDownsampleLogProgram;
	if (m_pDownsampleLogFragmentShader)
		delete m_pDownsampleLogFragmentShader;
	if (m_pDownsampleLogRenderTarget)
		delete m_pDownsampleLogRenderTarget;

	// Destroy the downsample 4x4 fragment shader and render target
	if (m_pDownsampleProgram)
		delete m_pDownsampleProgram;
	if (m_pDownsampleFragmentShader)
		delete m_pDownsampleFragmentShader;
	if (m_pDownsampleVertexShader)
		delete m_pDownsampleVertexShader;
	if (m_pDownsampleRenderTarget)
		delete m_pDownsampleRenderTarget;

	// Destroy the average luminance result step stuff
	if (m_pDownsampleExpProgram)
		delete m_pDownsampleExpProgram;
	if (m_pDownsampleExpFragmentShader)
		delete m_pDownsampleExpFragmentShader;
	if (m_pAverageLuminanceTextureBuffer2D)
		delete m_pAverageLuminanceTextureBuffer2D;

	// Destroy the vertex shader
	if (m_pVertexShader)
		delete m_pVertexShader;
}

/**
*  @brief
*    Calculates the logarithmic average luminance
*/
void HDRAverageLuminance::CalculateAverageLuminance(const String &sShaderLanguage, TextureBufferRectangle &cOriginalTexture, const Color3 &cLuminanceConvert)
{
	// Get the internal texture format to use
	const TextureBuffer::EPixelFormat nInternalFormat = (cOriginalTexture.GetFormat() == TextureBuffer::R16G16B16A16F) ? TextureBuffer::L16F : TextureBuffer::L32F;

	// Get the shader language to use
	String sUsedShaderLanguage = sShaderLanguage;
	if (!sUsedShaderLanguage.GetLength())
		sUsedShaderLanguage = m_pRenderer->GetDefaultShaderLanguage();

	// Create the shaders and programs right now?
	if (!m_pVertexShader || m_pVertexShader->GetShaderLanguage() != sUsedShaderLanguage) {
		// If there's an previous instance of the program, destroy it first
		if (m_pDownsampleLogProgram) {
			delete m_pDownsampleLogProgram;
			m_pDownsampleLogProgram = nullptr;
		}
		if (m_pDownsampleLogFragmentShader) {
			delete m_pDownsampleLogFragmentShader;
			m_pDownsampleLogFragmentShader = nullptr;
		}
		if (m_pDownsampleProgram) {
			delete m_pDownsampleProgram;
			m_pDownsampleProgram = nullptr;
		}
		if (m_pDownsampleFragmentShader) {
			delete m_pDownsampleFragmentShader;
			m_pDownsampleFragmentShader = nullptr;
		}
		if (m_pDownsampleVertexShader) {
			delete m_pDownsampleVertexShader;
			m_pDownsampleVertexShader = nullptr;
		}
		if (m_pDownsampleExpProgram) {
			delete m_pDownsampleExpProgram;
			m_pDownsampleExpProgram = nullptr;
		}
		if (m_pDownsampleExpFragmentShader) {
			delete m_pDownsampleExpFragmentShader;
			m_pDownsampleExpFragmentShader = nullptr;
		}
		if (m_pVertexShader) {
			delete m_pVertexShader;
			m_pVertexShader = nullptr;
		}
		m_pDownsampleLogPositionProgramAttribute		= nullptr;
		m_pDownsampleLogTextureSizeProgramUniform		= nullptr;
		m_pDownsampleLogTextureProgramUniform			= nullptr;
		m_pDownsampleLogLuminanceConvertProgramUniform	= nullptr;
		m_pDownsampleLogEpsilonProgramUniform			= nullptr;
		m_pDownsamplePositionProgramAttribute			= nullptr;
		m_pDownsampleTextureSizeProgramUniform			= nullptr;
		m_pDownsampleSizeProgramUniform					= nullptr;
		m_pDownsampleTextureProgramUniform				= nullptr;
		m_pDownsampleExpPositionProgramAttribute		= nullptr;
		m_pDownsampleExpTextureSizeProgramUniform		= nullptr;
		m_pDownsampleExpTextureProgramUniform			= nullptr;

		// Get the shader language instance
		ShaderLanguage *pShaderLanguage = m_pRenderer->GetShaderLanguage(sUsedShaderLanguage);
		if (pShaderLanguage) {
			// Shader source code
			String sVertexShaderSourceCode;
			String sVertexShaderSourceCode_Downsample;
			String sFragmentShaderSourceCode_DownsampleLog;
			String sFragmentShaderSourceCode_Downsample;
			String sFragmentShaderSourceCode_DownsampleExp;
			if (sUsedShaderLanguage == "GLSL") {
				#include "HDRAverageLuminance_GLSL.h"
				sVertexShaderSourceCode					= sHDRAverageLuminance_GLSL_VS;
				sVertexShaderSourceCode_Downsample		= sHDRAverageLuminance_GLSL_VS_Downsample;
				sFragmentShaderSourceCode_DownsampleLog	= sHDRAverageLuminance_GLSL_FS_Common + sHDRAverageLuminance_GLSL_FS_DownsampleLog;
				sFragmentShaderSourceCode_Downsample	= sHDRAverageLuminance_GLSL_FS_Common + sHDRAverageLuminance_GLSL_FS_Downsample;
				sFragmentShaderSourceCode_DownsampleExp	= sHDRAverageLuminance_GLSL_FS_Common + sHDRAverageLuminance_GLSL_FS_DownsampleExp;
			} else if (sUsedShaderLanguage == "Cg") {
				#include "HDRAverageLuminance_Cg.h"
				sVertexShaderSourceCode					= sHDRAverageLuminance_Cg_VS;
				sVertexShaderSourceCode_Downsample		= sHDRAverageLuminance_Cg_VS_Downsample;
				sFragmentShaderSourceCode_DownsampleLog	= sHDRAverageLuminance_Cg_FS_Common + sHDRAverageLuminance_Cg_FS_DownsampleLog;
				sFragmentShaderSourceCode_Downsample	= sHDRAverageLuminance_Cg_FS_Common + sHDRAverageLuminance_Cg_FS_Downsample;
				sFragmentShaderSourceCode_DownsampleExp	= sHDRAverageLuminance_Cg_FS_Common + sHDRAverageLuminance_Cg_FS_DownsampleExp;
			}

			// Create a vertex shader instance
			m_pVertexShader           = pShaderLanguage->CreateVertexShader(sVertexShaderSourceCode);
			m_pDownsampleVertexShader = pShaderLanguage->CreateVertexShader(sVertexShaderSourceCode_Downsample);

			// Create a fragment shader instance
			m_pDownsampleLogFragmentShader = pShaderLanguage->CreateFragmentShader(sFragmentShaderSourceCode_DownsampleLog);
			m_pDownsampleFragmentShader    = pShaderLanguage->CreateFragmentShader(sFragmentShaderSourceCode_Downsample);
			m_pDownsampleExpFragmentShader = pShaderLanguage->CreateFragmentShader(sFragmentShaderSourceCode_DownsampleExp);

			// Create a program instance and assign the created vertex and fragment shaders to it
			m_pDownsampleLogProgram = pShaderLanguage->CreateProgram(m_pVertexShader, m_pDownsampleLogFragmentShader);
			if (m_pDownsampleLogProgram) {
				// Add our nark which will inform us as soon as the program gets dirty
				m_pDownsampleLogProgram->EventDirty.Connect(EventHandlerDirty);

				// Get attributes and uniforms
				OnDirty(m_pDownsampleLogProgram);
			}
			m_pDownsampleProgram = pShaderLanguage->CreateProgram(m_pDownsampleVertexShader, m_pDownsampleFragmentShader);
			if (m_pDownsampleProgram) {
				// Add our nark which will inform us as soon as the program gets dirty
				m_pDownsampleProgram->EventDirty.Connect(EventHandlerDirty);

				// Get attributes and uniforms
				OnDirty(m_pDownsampleProgram);
			}
			m_pDownsampleExpProgram = pShaderLanguage->CreateProgram(m_pVertexShader, m_pDownsampleExpFragmentShader);
			if (m_pDownsampleExpProgram) {
				// Add our nark which will inform us as soon as the program gets dirty
				m_pDownsampleExpProgram->EventDirty.Connect(EventHandlerDirty);

				// Get attributes and uniforms
				OnDirty(m_pDownsampleExpProgram);
			}
		}
	}

	// Create the fullscreen quad instance if required
	if (!m_pFullscreenQuad)
		m_pFullscreenQuad = new FullscreenQuad(*m_pRenderer);

	// Get the vertex buffer of the fullscreen quad
	VertexBuffer *pVertexBuffer = m_pFullscreenQuad->GetVertexBuffer();
	if (pVertexBuffer) {
		// First step: Downsample 2x2, calculate pixel luminance and log - I don't use a "bilinear filter" because this would mess up the incoming texel data "before" the log calculation was performed!
		if (m_pRenderer->SetProgram(m_pDownsampleLogProgram)) {
			// Get the size of the original HDR texture
			const Vector2i vRTSize = cOriginalTexture.GetSize()/2;
			if (vRTSize.x != 0 && vRTSize.y != 0) {
				// Render target size change?
				if (m_pDownsampleLogRenderTarget && (m_pDownsampleLogRenderTarget->GetSize() != vRTSize || m_pDownsampleLogRenderTarget->GetFormat() != nInternalFormat)) {
					// Destroy the downsample 2x2 log render target
					if (m_pDownsampleLogRenderTarget) {
						delete m_pDownsampleLogRenderTarget;
						m_pDownsampleLogRenderTarget = nullptr;
					}
				}

				// Create the downsample 2x2 log render target right now?
				if (!m_pDownsampleLogRenderTarget)
					m_pDownsampleLogRenderTarget = m_pRenderer->CreateSurfaceTextureBufferRectangle(vRTSize, nInternalFormat, SurfaceTextureBuffer::NoMultisampleAntialiasing);
			}
			
			// Make the downsample 2x2 log render target to the current render target
			m_pRenderer->SetRenderTarget(m_pDownsampleLogRenderTarget);

			// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
			if (m_pDownsampleLogPositionProgramAttribute)
				m_pDownsampleLogPositionProgramAttribute->Set(pVertexBuffer, PLRenderer::VertexBuffer::Position);

			// Set the "TextureSize" fragment shader parameter
			if (m_pDownsampleLogTextureSizeProgramUniform)
				m_pDownsampleLogTextureSizeProgramUniform->Set(cOriginalTexture.GetSize());

			// Set the "Texture" fragment shader parameter
			if (m_pDownsampleLogTextureProgramUniform) {
				const int nTextureUnit = m_pDownsampleLogTextureProgramUniform->Set(&cOriginalTexture);
				if (nTextureUnit >= 0) {
					m_pRenderer->SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
					m_pRenderer->SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
					m_pRenderer->SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
					m_pRenderer->SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
					m_pRenderer->SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
				}
			}

			// Set the "LuminanceConvert" fragment shader parameter
			if (m_pDownsampleLogLuminanceConvertProgramUniform)
				m_pDownsampleLogLuminanceConvertProgramUniform->Set(cLuminanceConvert);

			// Set the "Epsilon" fragment shader parameter
			if (m_pDownsampleLogEpsilonProgramUniform)
				m_pDownsampleLogEpsilonProgramUniform->Set(0.0001f);

			// Draw the fullscreen quad
			m_pFullscreenQuad->Draw();
		}

		Vector2i vFinalTextureSize;
		TextureBufferRectangle *pFinalTextureBufferRectangle = nullptr;

		// Second step: Reduce to <4>x<4>
		if (m_pDownsampleLogRenderTarget && m_pRenderer->SetProgram(m_pDownsampleProgram)) {
			const uint32 TextureScaleFactor = 2;

			// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
			if (m_pDownsamplePositionProgramAttribute)
				m_pDownsamplePositionProgramAttribute->Set(pVertexBuffer, PLRenderer::VertexBuffer::Position);

			// Get the size of the original HDR texture
			const Vector2i vRTSize = m_pDownsampleLogRenderTarget->GetSize()/TextureScaleFactor;
			if (vRTSize.x != 0 && vRTSize.y != 0) {
				// Render target size change?
				if (m_pDownsampleRenderTarget && (m_pDownsampleRenderTarget->GetSize() != vRTSize || m_pDownsampleRenderTarget->GetFormat() != nInternalFormat)) {
					// Destroy the downsample 4x4 render target
					if (m_pDownsampleRenderTarget) {
						delete m_pDownsampleRenderTarget;
						m_pDownsampleRenderTarget = nullptr;
					}
				}

				// Create the downsample 4x4 render target right now?
				if (!m_pDownsampleRenderTarget)
					m_pDownsampleRenderTarget = m_pRenderer->CreateSurfaceTextureBufferRectangle(vRTSize, nInternalFormat, SurfaceTextureBuffer::NoMultisampleAntialiasing);
			}

			// Get the maximum size
			Vector2i vCurrentSize = m_pDownsampleLogRenderTarget->GetSize();
			uint32   nSize		  = Math::Max(vCurrentSize.x, vCurrentSize.y);

			// Downscale in multiple render passes, we render into m_pDownsampleRenderTarget and m_pDownsampleLogRenderTarget in a ping-pong way
			bool bRenderTarget = false;
			for (; nSize>TextureScaleFactor; nSize/=TextureScaleFactor, bRenderTarget=!bRenderTarget, vCurrentSize/=TextureScaleFactor) {
				// Get the render target we will render into
				SurfaceTextureBuffer *pRenderTargetTexture = bRenderTarget ? m_pDownsampleLogRenderTarget : m_pDownsampleRenderTarget;

				// Get the texture buffer we will downscale
				TextureBufferRectangle *pTextureBufferRectangle = static_cast<TextureBufferRectangle*>(bRenderTarget ? m_pDownsampleRenderTarget->GetTextureBuffer() : m_pDownsampleLogRenderTarget->GetTextureBuffer());

				// Set the current render target
				m_pRenderer->SetRenderTarget(pRenderTargetTexture);

				// Backup texture width
				vFinalTextureSize			 = vCurrentSize/TextureScaleFactor;
				pFinalTextureBufferRectangle = static_cast<TextureBufferRectangle*>(pRenderTargetTexture->GetTextureBuffer());

				// Set the "TextureSize" fragment shader parameter
				if (m_pDownsampleTextureSizeProgramUniform)
					m_pDownsampleTextureSizeProgramUniform->Set(vCurrentSize);

				// Set the "Size" fragment shader parameter
				if (m_pDownsampleSizeProgramUniform)
					m_pDownsampleSizeProgramUniform->Set(static_cast<float>(vFinalTextureSize.x)/pRenderTargetTexture->GetSize().x, static_cast<float>(vFinalTextureSize.y)/pRenderTargetTexture->GetSize().y);

				// Set the "Texture" fragment shader parameter
				if (m_pDownsampleTextureProgramUniform) {
					const int nTextureUnit = m_pDownsampleTextureProgramUniform->Set(pTextureBufferRectangle);
					if (nTextureUnit >= 0) {
						m_pRenderer->SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
						m_pRenderer->SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
						m_pRenderer->SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);	// [TODO] Use bilinear filter
						m_pRenderer->SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);	// [TODO] Use bilinear filter
						m_pRenderer->SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
					}
				}

				// Draw the fullscreen quad
				m_pFullscreenQuad->Draw();
			}
		}

		// Third step: Reduce <4>x<4> to <1>x<1> and calculate the exponent
		if (m_pRenderer->SetProgram(m_pDownsampleExpProgram)) {
			// Render format change?
			if (m_pAverageLuminanceTextureBuffer2D && m_pAverageLuminanceTextureBuffer2D->GetFormat() != nInternalFormat) {
				// Destroy the result render target
				if (m_pAverageLuminanceTextureBuffer2D) {
					delete m_pAverageLuminanceTextureBuffer2D;
					m_pAverageLuminanceTextureBuffer2D = nullptr;
				}
			}

			// Create the result render target right now?
			if (!m_pAverageLuminanceTextureBuffer2D)
				m_pAverageLuminanceTextureBuffer2D = m_pRenderer->CreateSurfaceTextureBuffer2D(Vector2i::One, nInternalFormat, SurfaceTextureBuffer::NoMultisampleAntialiasing);

			// Make the result render target to the current render target
			m_pRenderer->SetRenderTarget(m_pAverageLuminanceTextureBuffer2D);

			// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
			if (m_pDownsampleExpPositionProgramAttribute)
				m_pDownsampleExpPositionProgramAttribute->Set(pVertexBuffer, PLRenderer::VertexBuffer::Position);

			// Set the "TextureSize" fragment shader parameter
			if (m_pDownsampleExpTextureSizeProgramUniform)
				m_pDownsampleExpTextureSizeProgramUniform->Set(vFinalTextureSize);

			// Set the "Texture" fragment shader parameter
			if (m_pDownsampleExpTextureProgramUniform) {
				const int nTextureUnit = m_pDownsampleExpTextureProgramUniform->Set(pFinalTextureBufferRectangle);
				if (nTextureUnit >= 0) {
					m_pRenderer->SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
					m_pRenderer->SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
					m_pRenderer->SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);	// [TODO] Use bilinear filter
					m_pRenderer->SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);	// [TODO] Use bilinear filter
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
*    Returns the 1x1 2D texture buffer storing the logarithmic average luminance
*/
TextureBuffer *HDRAverageLuminance::GetTextureBuffer() const
{
	return m_pAverageLuminanceTextureBuffer2D ? m_pAverageLuminanceTextureBuffer2D->GetTextureBuffer() : nullptr;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a program became dirty
*/
void HDRAverageLuminance::OnDirty(Program *pProgram)
{
	// Get attributes and uniforms
	if (pProgram == m_pDownsampleLogProgram) {
		m_pDownsampleLogPositionProgramAttribute	   = m_pDownsampleLogProgram->GetAttribute("VertexPosition");
		m_pDownsampleLogTextureSizeProgramUniform	   = m_pDownsampleLogProgram->GetUniform("TextureSize");
		m_pDownsampleLogTextureProgramUniform		   = m_pDownsampleLogProgram->GetUniform("Texture");
		m_pDownsampleLogLuminanceConvertProgramUniform = m_pDownsampleLogProgram->GetUniform("LuminanceConvert");
		m_pDownsampleLogEpsilonProgramUniform		   = m_pDownsampleLogProgram->GetUniform("Epsilon");
	} else if (pProgram == m_pDownsampleProgram) {
		m_pDownsamplePositionProgramAttribute  = m_pDownsampleProgram->GetAttribute("VertexPosition");
		m_pDownsampleTextureSizeProgramUniform = m_pDownsampleProgram->GetUniform("TextureSize");
		m_pDownsampleSizeProgramUniform		   = m_pDownsampleProgram->GetUniform("Size");
		m_pDownsampleTextureProgramUniform	   = m_pDownsampleProgram->GetUniform("Texture");
	} else if (pProgram == m_pDownsampleExpProgram) {
		m_pDownsampleExpPositionProgramAttribute  = m_pDownsampleExpProgram->GetAttribute("VertexPosition");
		m_pDownsampleExpTextureSizeProgramUniform = m_pDownsampleExpProgram->GetUniform("TextureSize");
		m_pDownsampleExpTextureProgramUniform     = m_pDownsampleExpProgram->GetUniform("Texture");
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
