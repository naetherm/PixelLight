/*********************************************************\
 *  File: SRPDeferredDOF.cpp                             *
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
#include <PLRenderer/Renderer/VertexShader.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ShaderLanguage.h>
#include <PLRenderer/Renderer/ProgramAttribute.h>
#include <PLRenderer/Renderer/FragmentShader.h>
#include <PLRenderer/Renderer/TextureBufferRectangle.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLScene/Scene/SNCamera.h>
#include <PLScene/Scene/SceneNodeModifier.h>
#include "PLCompositing/FullscreenQuad.h"
#include "PLCompositing/SRPBegin.h"
#include "PLCompositing/Shaders/Deferred/SRPDeferredGBuffer.h"
#include "PLCompositing/Shaders/Deferred/SRPDeferredDOF.h"


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
pl_implement_class(SRPDeferredDOF)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPDeferredDOF::SRPDeferredDOF() :
	ShaderLanguage(this),
	EffectWeight(this),
	NearPlaneDepth(this),
	FocalPlaneDepth(this),
	FarPlaneDepth(this),
	BlurrinessCutoff(this),
	BlurPasses(this),
	BlurDownscale(this),
	Flags(this),
	EventHandlerDirty(&SRPDeferredDOF::OnDirty, this),
	m_bResultIndex(0),
	m_pVertexShader(nullptr),
	m_pDepthBlurFragmentShader(nullptr),
	m_pDepthBlurProgram(nullptr),
	m_pDepthBlurPositionProgramAttribute(nullptr),
	m_pDepthBlurTextureSizeProgramUniform(nullptr),
	m_pDepthBlurDOFParamsProgramUniform(nullptr),
	m_pDepthBlurRGBTextureProgramUniform(nullptr),
	m_pDepthBlurNormalDepthTextureProgramUniform(nullptr),
	m_pDownscaleFragmentShader(nullptr),
	m_pDownscaleProgram(nullptr),
	m_pDownscalePositionProgramAttribute(nullptr),
	m_pDownscaleTextureSizeProgramUniform(nullptr),
	m_pDownscaleTextureProgramUniform(nullptr),
	m_pBlurFragmentShader(nullptr),
	m_pBlurProgram(nullptr),
	m_pBlurPositionProgramAttribute(nullptr),
	m_pBlurTextureSizeProgramUniform(nullptr),
	m_pBlurUVScaleProgramUniform(nullptr),
	m_pBlurTextureProgramUniform(nullptr),
	m_pResultFragmentShader(nullptr),
	m_pResultProgram(nullptr),
	m_pResultPositionProgramAttribute(nullptr),
	m_pResultTextureSizeProgramUniform(nullptr),
	m_pResultEffectWeightProgramUniform(nullptr),
	m_pResultBlurDownscaleProgramUniform(nullptr),
	m_pResultBlurTextureProgramUniform(nullptr),
	m_pResultTextureProgramUniform(nullptr),
	m_pDebugFragmentShader(nullptr),
	m_pDebugProgram(nullptr),
	m_pDebugPositionProgramAttribute(nullptr),
	m_pDebugTextureSizeProgramUniform(nullptr),
	m_pDebugTextureProgramUniform(nullptr),
	m_nDebugType(0)
{
	// Init data
	m_pRenderTarget[0] = m_pRenderTarget[1] = nullptr;
}

/**
*  @brief
*    Destructor
*/
SRPDeferredDOF::~SRPDeferredDOF()
{
	// Destroy the render targets
	for (int i=0; i<2; i++) {
		if (m_pRenderTarget[i])
			delete m_pRenderTarget[i];
	}

	// Destroy the depth blur stuff
	if (m_pDepthBlurProgram)
		delete m_pDepthBlurProgram;
	if (m_pDepthBlurFragmentShader)
		delete m_pDepthBlurFragmentShader;

	// Destroy the downscale stuff
	if (m_pDownscaleProgram)
		delete m_pDownscaleProgram;
	if (m_pDownscaleFragmentShader)
		delete m_pDownscaleFragmentShader;

	// Destroy the blur stuff
	if (m_pBlurProgram)
		delete m_pBlurProgram;
	if (m_pBlurFragmentShader)
		delete m_pBlurFragmentShader;

	// Destroy the result stuff
	if (m_pResultProgram)
		delete m_pResultProgram;
	if (m_pResultFragmentShader)
		delete m_pResultFragmentShader;

	// Destroy the debug stuff
	if (m_pDebugProgram)
		delete m_pDebugProgram;
	if (m_pDebugFragmentShader)
		delete m_pDebugFragmentShader;

	// Destroy the vertex shader
	if (m_pVertexShader)
		delete m_pVertexShader;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Calculates the depth blur
*/
void SRPDeferredDOF::CalculateDepthBlur(const String &sShaderLanguage, VertexBuffer &cVertexBuffer, TextureBufferRectangle &cRGBTexture,
										TextureBufferRectangle &cNormalDepthTexture, float fNearPlaneDepth, float fFocalPlaneDepth, float fFarPlaneDepth, float fBlurrinessCutoff)
{
	// Get the renderer instance
	Renderer &cRenderer = cRGBTexture.GetRenderer();

	// Create the shaders and programs right now?
	if (!m_pVertexShader || m_pVertexShader->GetShaderLanguage() != sShaderLanguage) {
		// If there's an previous instance of the program, destroy it first
		if (m_pDepthBlurProgram) {
			delete m_pDepthBlurProgram;
			m_pDepthBlurProgram = nullptr;
		}
		if (m_pDepthBlurFragmentShader) {
			delete m_pDepthBlurFragmentShader;
			m_pDepthBlurFragmentShader = nullptr;
		}
		if (m_pVertexShader) {
			delete m_pVertexShader;
			m_pVertexShader = nullptr;
		}
		m_pDepthBlurPositionProgramAttribute			= nullptr;
		m_pDepthBlurTextureSizeProgramUniform			= nullptr;
		m_pDepthBlurDOFParamsProgramUniform				= nullptr;
		m_pDepthBlurRGBTextureProgramUniform			= nullptr;
		m_pDepthBlurNormalDepthTextureProgramUniform	= nullptr;

		// Get the shader language instance
		PLRenderer::ShaderLanguage *pShaderLanguage = cRenderer.GetShaderLanguage(sShaderLanguage);
		if (pShaderLanguage) {
			// Shader source code
			String sVertexShaderSourceCode;
			String sFragmentShaderSourceCode;
			if (sShaderLanguage == "GLSL") {
				#include "SRPDeferredDOF_GLSL.h"
				sVertexShaderSourceCode	  = sDeferredDOF_GLSL_VS;
				sFragmentShaderSourceCode = sDeferredDOF_GLSL_FS_DepthBlur;
			} else if (sShaderLanguage == "Cg") {
				#include "SRPDeferredDOF_Cg.h"
				sVertexShaderSourceCode	  = sDeferredDOF_Cg_VS;
				sFragmentShaderSourceCode = sDeferredDOF_Cg_FS_DepthBlur;
			}

			// Create a vertex shader instance
			m_pVertexShader = pShaderLanguage->CreateVertexShader(sVertexShaderSourceCode);

			// Create a fragment shader instance
			m_pDepthBlurFragmentShader = pShaderLanguage->CreateFragmentShader(sFragmentShaderSourceCode);

			// Create a program instance and assign the created vertex and fragment shaders to it
			m_pDepthBlurProgram = pShaderLanguage->CreateProgram(m_pVertexShader, m_pDepthBlurFragmentShader);
			if (m_pDepthBlurProgram) {
				// Add our nark which will inform us as soon as the program gets dirty
				m_pDepthBlurProgram->EventDirty.Connect(EventHandlerDirty);

				// Get attributes and uniforms
				OnDirty(m_pDepthBlurProgram);
			}
		}
	}

	// Make the depth blur GPU program to the currently used one
	if (cRenderer.SetProgram(m_pDepthBlurProgram)) {
		// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
		if (m_pDepthBlurPositionProgramAttribute)
			m_pDepthBlurPositionProgramAttribute->Set(&cVertexBuffer, PLRenderer::VertexBuffer::Position);

		// Set the "TextureSize" fragment shader parameter
		if (m_pDepthBlurTextureSizeProgramUniform)
			m_pDepthBlurTextureSizeProgramUniform->Set(cRGBTexture.GetSize());

		// Set the "DOFParams" fragment shader parameter and ensure that all values are correct
		if (m_pDepthBlurDOFParamsProgramUniform) {
			// fFocalPlaneDepth - fNearPlaneDepth is not allowed to be 0!
			if (fNearPlaneDepth > fFocalPlaneDepth)
				fNearPlaneDepth = fFocalPlaneDepth;
			if (fFocalPlaneDepth - fNearPlaneDepth < FLT_MIN)
				fNearPlaneDepth -= FLT_MIN;

			// fFarPlaneDepth - fFocalPlaneDepth is not allowed to be 0!
			if (fFarPlaneDepth < fFocalPlaneDepth)
				fFarPlaneDepth = fFocalPlaneDepth;
			if (fFarPlaneDepth - fFocalPlaneDepth < FLT_MIN)
				fFarPlaneDepth += FLT_MIN;

			// Set parameters
			m_pDepthBlurDOFParamsProgramUniform->Set(fNearPlaneDepth, fFocalPlaneDepth, fFarPlaneDepth, fBlurrinessCutoff);
		}

		// Set the "RGBTexture" fragment shader parameter
		if (m_pDepthBlurRGBTextureProgramUniform) {
			const int nTextureUnit = m_pDepthBlurRGBTextureProgramUniform->Set(&cRGBTexture);
			if (nTextureUnit >= 0) {
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Clamp);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Clamp);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
			}
		}

		// Set the "NormalDepthTexture" fragment shader parameter
		if (m_pDepthBlurNormalDepthTextureProgramUniform) {
			const int nTextureUnit = m_pDepthBlurNormalDepthTextureProgramUniform->Set(&cNormalDepthTexture);
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
*    Calculates the blur
*/
void SRPDeferredDOF::CalculateBlur(const String &sShaderLanguage, VertexBuffer &cVertexBuffer, TextureBufferRectangle &cOriginalTexture, float fBrightThreshold, uint32 nBlurPasses, float fDownscale)
{
	// Get the internal texture format to use
	const TextureBuffer::EPixelFormat nInternalFormat = cOriginalTexture.GetFormat();

	// Get the renderer instance
	Renderer &cRenderer = cOriginalTexture.GetRenderer();

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
					m_pRenderTarget[i] = cRenderer.CreateSurfaceTextureBufferRectangle(vRTSize, nInternalFormat, SurfaceTextureBuffer::NoMultisampleAntialiasing);
			}
		}
	}

	// Create the shaders and programs right now?
	if (!m_pDownscaleFragmentShader || m_pDownscaleFragmentShader->GetShaderLanguage() != sShaderLanguage) {
		// If there's an previous instance of the program, destroy it first
		if (m_pDownscaleProgram) {
			delete m_pDownscaleProgram;
			m_pDownscaleProgram = nullptr;
		}
		if (m_pDownscaleFragmentShader) {
			delete m_pDownscaleFragmentShader;
			m_pDownscaleFragmentShader = nullptr;
		}
		if (m_pBlurProgram) {
			delete m_pBlurProgram;
			m_pBlurProgram = nullptr;
		}
		if (m_pBlurFragmentShader) {
			delete m_pBlurFragmentShader;
			m_pBlurFragmentShader = nullptr;
		}
		m_pDownscalePositionProgramAttribute	= nullptr;
		m_pDownscaleTextureSizeProgramUniform	= nullptr;
		m_pDownscaleTextureProgramUniform		= nullptr;
		m_pBlurPositionProgramAttribute			= nullptr;
		m_pBlurTextureSizeProgramUniform		= nullptr;
		m_pBlurUVScaleProgramUniform			= nullptr;
		m_pBlurTextureProgramUniform			= nullptr;

		// Get the shader language instance
		PLRenderer::ShaderLanguage *pShaderLanguage = cRenderer.GetShaderLanguage(sShaderLanguage);
		if (pShaderLanguage) {
			// Shader source code
			String sVertexShaderSourceCode;
			String sFragmentShaderSourceCode_Downscale;
			String sFragmentShaderSourceCode_Blur;
			if (sShaderLanguage == "GLSL") {
				#include "SRPDeferredDOF_GLSL.h"
				sVertexShaderSourceCode				= sDeferredDOF_GLSL_VS;
				sFragmentShaderSourceCode_Downscale	= sDeferredDOF_GLSL_FS_Downscale;
				sFragmentShaderSourceCode_Blur		= sDeferredDOF_GLSL_FS_Blur;
			} else if (sShaderLanguage == "Cg") {
				#include "SRPDeferredDOF_Cg.h"
				sVertexShaderSourceCode				= sDeferredDOF_Cg_VS;
				sFragmentShaderSourceCode_Downscale = sDeferredDOF_Cg_FS_Downscale;
				sFragmentShaderSourceCode_Blur		= sDeferredDOF_Cg_FS_Blur;
			}

			// Create a fragment shader instance
			m_pDownscaleFragmentShader = pShaderLanguage->CreateFragmentShader(sFragmentShaderSourceCode_Downscale);
			m_pBlurFragmentShader      = pShaderLanguage->CreateFragmentShader(sFragmentShaderSourceCode_Blur);

			// Create a program instance and assign the created vertex and fragment shaders to it
			m_pDownscaleProgram = pShaderLanguage->CreateProgram(m_pVertexShader, m_pDownscaleFragmentShader);
			if (m_pDownscaleProgram) {
				// Add our nark which will inform us as soon as the program gets dirty
				m_pDownscaleProgram->EventDirty.Connect(EventHandlerDirty);

				// Get attributes and uniforms
				OnDirty(m_pDownscaleProgram);
			}
			m_pBlurProgram = pShaderLanguage->CreateProgram(m_pVertexShader, m_pBlurFragmentShader);
			if (m_pBlurProgram) {
				// Add our nark which will inform us as soon as the program gets dirty
				m_pBlurProgram->EventDirty.Connect(EventHandlerDirty);

				// Get attributes and uniforms
				OnDirty(m_pBlurProgram);
			}
		}
	}

	// First step: Downscale -> Make the downscale GPU program to the currently used one
	if (cRenderer.SetProgram(m_pDownscaleProgram)) {
		// Make the render target 0 to the current render target
		cRenderer.SetRenderTarget(m_pRenderTarget[0]);
		m_bResultIndex = 0;

		// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
		if (m_pDownscalePositionProgramAttribute)
			m_pDownscalePositionProgramAttribute->Set(&cVertexBuffer, PLRenderer::VertexBuffer::Position);

		// Set the "TextureSize" fragment shader parameter
		if (m_pDownscaleTextureSizeProgramUniform)
			m_pDownscaleTextureSizeProgramUniform->Set(cOriginalTexture.GetSize());

		// Set the "Texture" fragment shader parameter
		if (m_pDownscaleTextureProgramUniform) {
			const int nTextureUnit = m_pDownscaleTextureProgramUniform->Set(&cOriginalTexture);
			if (nTextureUnit >= 0) {
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::Linear);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::Linear);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
			}
		}

		// Draw the fullscreen quad
		cRenderer.DrawPrimitives(Primitive::TriangleStrip, 0, 4);
	}

	// Second step: Gaussian convolution filter to glow - Make the bloom GPU program to the currently used one
	if (cRenderer.SetProgram(m_pBlurProgram)) {
		// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
		if (m_pBlurPositionProgramAttribute)
			m_pBlurPositionProgramAttribute->Set(&cVertexBuffer, PLRenderer::VertexBuffer::Position);

		// Set the "TextureSize" fragment shader parameter - both render targets have the same size
		if (m_pBlurTextureSizeProgramUniform)
			m_pBlurTextureSizeProgramUniform->Set(m_pRenderTarget[0]->GetSize());

		// Horizontal and vertical blur
		for (uint32 i=0; i<nBlurPasses; i++) {
			// Make the render target 1 to the current render target
			cRenderer.SetRenderTarget(m_pRenderTarget[!m_bResultIndex]);

			// Set the "UVScale" fragment shader parameter
			if (m_pBlurUVScaleProgramUniform) {
				if (i%2 != 0)
					m_pBlurUVScaleProgramUniform->Set(0.0f, 1.0f);
				else
					m_pBlurUVScaleProgramUniform->Set(1.0f, 0.0f);
			}

			// Set the "Texture" fragment shader parameter
			if (m_pBlurTextureProgramUniform) {
				const int nTextureUnit = m_pBlurTextureProgramUniform->Set(m_pRenderTarget[m_bResultIndex != 0]->GetTextureBuffer());
				if (nTextureUnit >= 0) {
					cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::Linear);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::Linear);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
				}
			}

			// Draw the fullscreen quad
			cRenderer.DrawPrimitives(Primitive::TriangleStrip, 0, 4);

			// The result is now within the other render target
			m_bResultIndex = !m_bResultIndex;
		}
	}
}

/**
*  @brief
*    Debugs data
*/
void SRPDeferredDOF::Debug(const String &sShaderLanguage, VertexBuffer &cVertexBuffer, TextureBufferRectangle &cTexture, uint32 nType)
{
	// Get the renderer instance
	Renderer &cRenderer = cTexture.GetRenderer();

	// Create the shaders and programs right now?
	if (!m_pDebugFragmentShader || m_pDebugFragmentShader->GetShaderLanguage() != sShaderLanguage || m_nDebugType != nType) {
		// If there's an previous instance of the program, destroy it first
		if (m_pDebugProgram) {
			delete m_pDebugProgram;
			m_pDebugProgram = nullptr;
		}
		if (m_pDebugFragmentShader) {
			delete m_pDebugFragmentShader;
			m_pDebugFragmentShader = nullptr;
		}
		m_pDebugPositionProgramAttribute	= nullptr;
		m_pDebugTextureSizeProgramUniform	= nullptr;
		m_pDebugTextureProgramUniform		= nullptr;

		// Get the shader language instance
		PLRenderer::ShaderLanguage *pShaderLanguage = cRenderer.GetShaderLanguage(sShaderLanguage);
		if (pShaderLanguage) {
			String sDefinition;

			// When using GLSL, the profile is the GLSL version to use - #version must occur before any other statement in the program!
			if (sShaderLanguage == "GLSL")
				sDefinition += "#version 110\n";	// OpenGL 2.0

			// Get the definition
			switch (nType) {
				 case 0:
					 sDefinition += "#define FS_DEPTH_BLUR\n";
					 break;

				 case 1:
					 sDefinition += "#define FS_BLUR\n";
					 break;
			}

			// Shader source code
			String sVertexShaderSourceCode;
			String sFragmentShaderSourceCode;
			if (sShaderLanguage == "GLSL") {
				#include "SRPDeferredDOF_GLSL.h"
				sVertexShaderSourceCode	  = sDeferredDOF_GLSL_VS;
				sFragmentShaderSourceCode = sDefinition + sDeferredDOF_GLSL_FS_Debug;
			} else if (sShaderLanguage == "Cg") {
				#include "SRPDeferredDOF_Cg.h"
				sVertexShaderSourceCode	  = sDeferredDOF_Cg_VS;
				sFragmentShaderSourceCode = sDefinition + sDeferredDOF_Cg_FS_Debug;
			}

			// Create a fragment shader instance
			m_pDebugFragmentShader = pShaderLanguage->CreateFragmentShader(sFragmentShaderSourceCode);

			// Create a program instance and assign the created vertex and fragment shaders to it
			m_pDebugProgram = pShaderLanguage->CreateProgram(m_pVertexShader, m_pDebugFragmentShader);
			if (m_pDebugProgram) {
				// Add our nark which will inform us as soon as the program gets dirty
				m_pDebugProgram->EventDirty.Connect(EventHandlerDirty);

				// Get attributes and uniforms
				OnDirty(m_pDebugProgram);

				// Backup the current debug type
				m_nDebugType = nType;
			}
		}
	}

	// Make the depth blur GPU program to the currently used one
	if (cRenderer.SetProgram(m_pDebugProgram)) {
		// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
		if (m_pDebugPositionProgramAttribute)
			m_pDebugPositionProgramAttribute->Set(&cVertexBuffer, PLRenderer::VertexBuffer::Position);

		// Set the "TextureSize" fragment shader parameter
		if (m_pDebugTextureSizeProgramUniform)
			m_pDebugTextureSizeProgramUniform->Set(cTexture.GetSize());

		// Set the "Texture" fragment shader parameter
		if (m_pDebugTextureProgramUniform) {
			const int nTextureUnit = m_pDebugTextureProgramUniform->Set(&cTexture);
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
*    Returns the rectangle texture buffer storing the blur
*/
TextureBuffer *SRPDeferredDOF::GetBlurTextureBuffer() const
{
	return m_pRenderTarget[m_bResultIndex] ? m_pRenderTarget[m_bResultIndex]->GetTextureBuffer() : nullptr;
}

/**
*  @brief
*    Called when a program became dirty
*/
void SRPDeferredDOF::OnDirty(Program *pProgram)
{
	// Get attributes and uniforms
	if (pProgram == m_pDepthBlurProgram) {
		m_pDepthBlurPositionProgramAttribute		 = m_pDepthBlurProgram->GetAttribute("VertexPosition");
		m_pDepthBlurTextureSizeProgramUniform		 = m_pDepthBlurProgram->GetUniform("TextureSize");
		m_pDepthBlurDOFParamsProgramUniform			 = m_pDepthBlurProgram->GetUniform("DOFParams");
		m_pDepthBlurRGBTextureProgramUniform		 = m_pDepthBlurProgram->GetUniform("RGBTexture");
		m_pDepthBlurNormalDepthTextureProgramUniform = m_pDepthBlurProgram->GetUniform("NormalDepthTexture");
	} else if (pProgram == m_pDownscaleProgram) {
		m_pDownscalePositionProgramAttribute  = m_pDownscaleProgram->GetAttribute("VertexPosition");
		m_pDownscaleTextureSizeProgramUniform = m_pDownscaleProgram->GetUniform("TextureSize");
		m_pDownscaleTextureProgramUniform	  = m_pDownscaleProgram->GetUniform("Texture");
	} else if (pProgram == m_pBlurProgram) {
		m_pBlurPositionProgramAttribute  = m_pBlurProgram->GetAttribute("VertexPosition");
		m_pBlurTextureSizeProgramUniform = m_pBlurProgram->GetUniform("TextureSize");
		m_pBlurUVScaleProgramUniform	 = m_pBlurProgram->GetUniform("UVScale");
		m_pBlurTextureProgramUniform	 = m_pBlurProgram->GetUniform("Texture");
	} else if (pProgram == m_pResultProgram) {
		m_pResultPositionProgramAttribute    = m_pResultProgram->GetAttribute("VertexPosition");
		m_pResultTextureSizeProgramUniform   = m_pResultProgram->GetUniform("TextureSize");
		m_pResultEffectWeightProgramUniform	 = m_pResultProgram->GetUniform("EffectWeight");
		m_pResultBlurDownscaleProgramUniform = m_pResultProgram->GetUniform("BlurDownscale");
		m_pResultBlurTextureProgramUniform	 = m_pResultProgram->GetUniform("BlurTexture");
		m_pResultTextureProgramUniform		 = m_pResultProgram->GetUniform("Texture");
	} else if (pProgram == m_pDebugProgram) {
		m_pDebugPositionProgramAttribute  = m_pDebugProgram->GetAttribute("VertexPosition");
		m_pDebugTextureSizeProgramUniform = m_pDebugProgram->GetUniform("TextureSize");
		m_pDebugTextureProgramUniform	  = m_pDebugProgram->GetUniform("Texture");
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRPDeferredDOF::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Get DOF settings
	float fEffectWeight		= EffectWeight;
	float fNearPlaneDepth   = NearPlaneDepth;
	float fFocalPlaneDepth  = FocalPlaneDepth;
	float fFarPlaneDepth    = FarPlaneDepth;
	float fBlurrinessCutoff = BlurrinessCutoff;
	bool  bDOFActive        = true;

	// Get depth of field settings from the 'SNMPostProcessDepthOfField' modifier of the used camera?
	if (!(GetFlags() & IgnoreCameraSettings)) {
		bDOFActive = false;
		SNCamera *pCamera = SNCamera::GetCamera();
		if (pCamera) {
			// Is there an active "PLCompositing::SNMPostProcessDepthOfField" modifier attached to the current camera?
			SceneNodeModifier *pModifier = pCamera->GetModifier("PLCompositing::SNMPostProcessDepthOfField");
			if (pModifier && pModifier->IsActive()) {
				// Is the effect weight greater than zero?
				fEffectWeight = pModifier->GetAttribute("EffectWeight")->GetFloat();
				if (fEffectWeight > 0.0f) {
					fNearPlaneDepth   = pModifier->GetAttribute("NearBlurDepth")   ->GetFloat();
					fFocalPlaneDepth  = pModifier->GetAttribute("FocalPlaneDepth") ->GetFloat();
					fFarPlaneDepth    = pModifier->GetAttribute("FarBlurDepth")    ->GetFloat();
					fBlurrinessCutoff = pModifier->GetAttribute("BlurrinessCutoff")->GetFloat();
					bDOFActive        = true;
				}
			}
		}
	}

	// DOF active?
	if (bDOFActive) {
		// Get the instance of the "PLCompositing::SRPDeferredGBuffer" scene renderer pass
		SRPDeferredGBuffer *pSRPDeferredGBuffer = GetGBuffer();
		if (pSRPDeferredGBuffer) {
			// Get the normal/depth texture buffer to use
			TextureBufferRectangle *pNormalDepthTextureBuffer = pSRPDeferredGBuffer->GetRenderTargetTextureBuffer(1);
			if (pNormalDepthTextureBuffer) {
				// Get the fullscreen quad instance
				FullscreenQuad *pFullscreenQuad = pSRPDeferredGBuffer->GetFullscreenQuad();
				if (pFullscreenQuad) {
					// Get the vertex buffer of the fullscreen quad
					VertexBuffer *pVertexBuffer = pFullscreenQuad->GetVertexBuffer();
					if (pVertexBuffer) {
						// Get the "PLCompositing::SRPBegin" instance
						SRPBegin *pSRPBegin = static_cast<SRPBegin*>(GetFirstInstanceOfSceneRendererPassClass("PLCompositing::SRPBegin"));
						if (pSRPBegin) {
							// We need up-to-date front render target content, so swap the render targets
							pSRPBegin->SwapRenderTargets();

							// Get the front render target of SRPBegin, this holds the current content
							SurfaceTextureBuffer *pFrontSurfaceTextureBuffer = pSRPBegin->GetFrontRenderTarget();
							if (pFrontSurfaceTextureBuffer && pFrontSurfaceTextureBuffer->GetTextureBuffer()) {
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

								// Calculate the depth blur
								cRenderer.SetColorMask(false, false, false, true);
								CalculateDepthBlur(sShaderLanguage, *pVertexBuffer, static_cast<TextureBufferRectangle&>(*pFrontSurfaceTextureBuffer->GetTextureBuffer()), *pNormalDepthTextureBuffer, fNearPlaneDepth, fFocalPlaneDepth, fFarPlaneDepth, fBlurrinessCutoff);
								cRenderer.SetColorMask();
								if (GetFlags() & DebugDepthBlur) {
									// Debug depth blur data
									pSRPBegin->SwapRenderTargets();
									pFrontSurfaceTextureBuffer = pSRPBegin->GetFrontRenderTarget();
									if (pFrontSurfaceTextureBuffer && pFrontSurfaceTextureBuffer->GetTextureBuffer())
										Debug(sShaderLanguage, *pVertexBuffer, static_cast<TextureBufferRectangle&>(*pFrontSurfaceTextureBuffer->GetTextureBuffer()), 0);
								} else {
									// Calculate the blur texture buffer
									if (BlurDownscale < 1.0f)
										BlurDownscale = 1.0f;
									CalculateBlur(sShaderLanguage, *pVertexBuffer, static_cast<TextureBufferRectangle&>(*pFrontSurfaceTextureBuffer->GetTextureBuffer()), 0.0f, BlurPasses, BlurDownscale);

									// We need up-to-date front render target content, so swap the render targets
									pSRPBegin->SwapRenderTargets();

									// Get blur texture buffer
									TextureBufferRectangle *pTextureBuffer = static_cast<TextureBufferRectangle*>(GetBlurTextureBuffer());
									if (pTextureBuffer) {
										// Debug blur data?
										if (GetFlags() & DebugBlur) {
											Debug(sShaderLanguage, *pVertexBuffer, *pTextureBuffer, 1);
										} else {
											pFrontSurfaceTextureBuffer = pSRPBegin->GetFrontRenderTarget();
											if (pFrontSurfaceTextureBuffer && pFrontSurfaceTextureBuffer->GetTextureBuffer()) {
												// Create the shaders and programs right now?
												if (!m_pResultFragmentShader || m_pResultFragmentShader->GetShaderLanguage() != sShaderLanguage) {
													// If there's an previous instance of the program, destroy it first
													if (m_pResultProgram) {
														delete m_pResultProgram;
														m_pResultProgram = nullptr;
													}
													if (m_pResultFragmentShader) {
														delete m_pResultFragmentShader;
														m_pResultFragmentShader = nullptr;
													}
													m_pResultPositionProgramAttribute		= nullptr;
													m_pResultTextureSizeProgramUniform		= nullptr;
													m_pResultEffectWeightProgramUniform		= nullptr;
													m_pResultBlurDownscaleProgramUniform	= nullptr;
													m_pResultBlurTextureProgramUniform		= nullptr;
													m_pResultTextureProgramUniform			= nullptr;

													// Get the shader language instance
													PLRenderer::ShaderLanguage *pShaderLanguage = cRenderer.GetShaderLanguage(sShaderLanguage);
													if (pShaderLanguage) {
														// Shader source code
														String sVertexShaderSourceCode;
														String sFragmentShaderSourceCode;
														if (sShaderLanguage == "GLSL") {
															#include "SRPDeferredDOF_GLSL.h"
															sVertexShaderSourceCode	  = sDeferredDOF_GLSL_VS;
															sFragmentShaderSourceCode = sDeferredDOF_GLSL_FS_Result;
														} else if (sShaderLanguage == "Cg") {
															#include "SRPDeferredDOF_Cg.h"
															sVertexShaderSourceCode	  = sDeferredDOF_Cg_VS;
															sFragmentShaderSourceCode = sDeferredDOF_Cg_FS_Result;
														}

														// Create a fragment shader instance
														m_pResultFragmentShader = pShaderLanguage->CreateFragmentShader(sFragmentShaderSourceCode);

														// Create a program instance and assign the created vertex and fragment shaders to it
														m_pResultProgram = pShaderLanguage->CreateProgram(m_pVertexShader, m_pResultFragmentShader);
														if (m_pResultProgram) {
															// Add our nark which will inform us as soon as the program gets dirty
															m_pResultProgram->EventDirty.Connect(EventHandlerDirty);

															// Get attributes and uniforms
															OnDirty(m_pResultProgram);
														}
													}
												}

												// Make the result GPU program to the currently used one
												if (cRenderer.SetProgram(m_pResultProgram)) {
													// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
													if (m_pResultPositionProgramAttribute)
														m_pResultPositionProgramAttribute->Set(pVertexBuffer, PLRenderer::VertexBuffer::Position);

													// Set the "TextureSize" fragment shader parameter
													if (m_pResultTextureSizeProgramUniform)
														m_pResultTextureSizeProgramUniform->Set(static_cast<TextureBufferRectangle*>(pFrontSurfaceTextureBuffer->GetTextureBuffer())->GetSize());

													// Set the "EffectWeight" fragment shader parameter
													if (m_pResultEffectWeightProgramUniform)
														m_pResultEffectWeightProgramUniform->Set(fEffectWeight);

													// Set the "BlurDownscale" fragment shader parameter
													if (m_pResultBlurDownscaleProgramUniform)
														m_pResultBlurDownscaleProgramUniform->Set(BlurDownscale);

													// Set the "BlurTexture" fragment shader parameter
													if (m_pResultBlurTextureProgramUniform) {
														const int nTextureUnit = m_pResultBlurTextureProgramUniform->Set(pTextureBuffer);
														if (nTextureUnit >= 0) {
															cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Clamp);
															cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Clamp);
															cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::Linear);
															cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::Linear);
															cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
														}
													}

													// Set the "Texture" fragment shader parameter
													if (m_pResultTextureProgramUniform) {
														const int nTextureUnit = m_pResultTextureProgramUniform->Set(pFrontSurfaceTextureBuffer->GetTextureBuffer());
														if (nTextureUnit >= 0) {
															cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Clamp);
															cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Clamp);
															cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
															cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
															cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
														}
													}

													// Draw the fullscreen quad
													pFullscreenQuad->Draw();
												}
											}
										}
									}
								}

								// Restore fixed fill mode render state
								cRenderer.SetRenderState(RenderState::FixedFillMode, nFixedFillModeBackup);
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
