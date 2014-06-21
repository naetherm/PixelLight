/*********************************************************\
 *  File: SRPDeferredSPAAO.cpp                           *
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
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImagePart.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Program.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ProgramAttribute.h>
#include <PLRenderer/Renderer/TextureBuffer2D.h>
#include <PLRenderer/Renderer/TextureBufferRectangle.h>
#include <PLScene/Scene/SNCamera.h>
#include "PLCompositing/Shaders/Deferred/SRPDeferredGBuffer.h"
#include "PLCompositing/Shaders/Deferred/SRPDeferredSPAAO.h"


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
pl_implement_class(SRPDeferredSPAAO)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
uint32 SRPDeferredSPAAO::GetNumberOfDirections() const
{
	return m_nNumberOfDirections;
}

void SRPDeferredSPAAO::SetNumberOfDirections(uint32 nValue)
{
	if (m_nNumberOfDirections != nValue) {
		m_nNumberOfDirections = nValue;

		// Destroy the random normals texture because it is now out-of-date
		if (m_pRandomNormalsTexture) {
			delete m_pRandomNormalsTexture;
			m_pRandomNormalsTexture = nullptr;
		}
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPDeferredSPAAO::SRPDeferredSPAAO() :
	NumberOfDirections(this),
	SampleRadius(this),
	Intensity(this),
	Scale(this),
	Bias(this),
	Flags(this),
	m_nNumberOfDirections(8),
	m_pRandomNormalsTexture(nullptr),
	m_pProgramGenerator(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
SRPDeferredSPAAO::~SRPDeferredSPAAO()
{
	// Destroy the random normals texture
	if (m_pRandomNormalsTexture)
		delete m_pRandomNormalsTexture;

	// Destroy the program generator
	if (m_pProgramGenerator)
		delete m_pProgramGenerator;
}


//[-------------------------------------------------------]
//[ Protected virtual SRPDeferredSSAO functions           ]
//[-------------------------------------------------------]
void SRPDeferredSPAAO::DrawAO(const String &sShaderLanguage, VertexBuffer &cVertexBuffer, TextureBufferRectangle &cNormalDepthTextureBuffer)
{
	// Get the renderer instance
	Renderer &cRenderer = cNormalDepthTextureBuffer.GetRenderer();

	// Create the program generator if there's currently no instance of it
	if (!m_pProgramGenerator || m_pProgramGenerator->GetShaderLanguage() != sShaderLanguage) {
		// If there's an previous instance of the program generator, destroy it first
		if (m_pProgramGenerator) {
			delete m_pProgramGenerator;
			m_pProgramGenerator = nullptr;
		}

		// Choose the shader source codes depending on the requested shader language
		if (sShaderLanguage == "GLSL") {
			#include "SRPDeferredSPAAO_GLSL.h"
			m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sDeferredSPAAO_GLSL_VS, "120", sDeferredSPAAO_GLSL_FS, "120");	// OpenGL 2.1 ("#version 120")
		} else if (sShaderLanguage == "Cg") {
			#include "SRPDeferredSPAAO_Cg.h"
			m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sDeferredSPAAO_Cg_VS, "glslv", sDeferredSPAAO_Cg_FS, "glslf");
		}
	}

	// If there's no program generator, we don't need to continue
	if (m_pProgramGenerator) {
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
				pGeneratedProgramUserData->pVertexPosition		= pProgram->GetAttribute(sVertexPosition);
				// Fragment shader uniforms
				static const String sSampleRadius = "SampleRadius";
				pGeneratedProgramUserData->pSampleRadius		= pProgram->GetUniform(sSampleRadius);
				static const String sIntensity = "Intensity";
				pGeneratedProgramUserData->pIntensity			= pProgram->GetUniform(sIntensity);
				static const String sScale = "Scale";
				pGeneratedProgramUserData->pScale				= pProgram->GetUniform(sScale);
				static const String sBias = "Bias";
				pGeneratedProgramUserData->pBias				= pProgram->GetUniform(sBias);
				static const String sInvFocalLen = "InvFocalLen";
				pGeneratedProgramUserData->pInvFocalLen			= pProgram->GetUniform(sInvFocalLen);
				static const String sRandomSize = "RandomSize";
				pGeneratedProgramUserData->pRandomSize			= pProgram->GetUniform(sRandomSize);
				static const String sRandomNormalsMap = "RandomNormalsMap";
				pGeneratedProgramUserData->pRandomNormalsMap	= pProgram->GetUniform(sRandomNormalsMap);
				static const String sResolution = "Resolution";
				pGeneratedProgramUserData->pResolution			= pProgram->GetUniform(sResolution);
				static const String sNormalDepthMap = "NormalDepthMap";
				pGeneratedProgramUserData->pNormalDepthMap		= pProgram->GetUniform(sNormalDepthMap);
			}

			// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
			if (pGeneratedProgramUserData->pVertexPosition)
				pGeneratedProgramUserData->pVertexPosition->Set(&cVertexBuffer, PLRenderer::VertexBuffer::Position);

			// SampleRadius
			if (pGeneratedProgramUserData->pSampleRadius)
				pGeneratedProgramUserData->pSampleRadius->Set(SampleRadius);

			// Intensity
			if (pGeneratedProgramUserData->pIntensity)
				pGeneratedProgramUserData->pIntensity->Set(Intensity);

			// Scale
			if (pGeneratedProgramUserData->pScale)
				pGeneratedProgramUserData->pScale->Set(Scale);

			// Bias
			if (pGeneratedProgramUserData->pBias)
				pGeneratedProgramUserData->pBias->Set(Bias);

			{ // Focal length
				// Get the width and height of the texture buffer
				const float fWidth  = static_cast<float>(cNormalDepthTextureBuffer.GetSize().x);
				const float fHeight = static_cast<float>(cNormalDepthTextureBuffer.GetSize().y);

				// Calculate the focal length
				const float fFovY         = static_cast<float>((SNCamera::GetCamera() ? SNCamera::GetCamera()->GetFOV() : 45.0f) * Math::DegToRad);
				const float fFocalLengthY = 1.0f / Math::Tan(fFovY * 0.5f);
				const float fFocalLengthX = fFocalLengthY * ((fHeight * (SNCamera::GetCamera() ? SNCamera::GetCamera()->GetAspect() : 1.0f)) / fWidth);

				// InvFocalLen
				if (pGeneratedProgramUserData->pInvFocalLen)
					pGeneratedProgramUserData->pInvFocalLen->Set(1.0f/fFocalLengthX, 1.0f/fFocalLengthY);
			}

			// RandomSize
			static const int nRandomNormalsTextureSize = 64;
			if (pGeneratedProgramUserData->pRandomSize)
				pGeneratedProgramUserData->pRandomSize->Set(nRandomNormalsTextureSize);

			{ // Set the "RandomNormalsMap" fragment shader parameter
				// Create texture if required
				if (!m_pRandomNormalsTexture) {
					// Create the image
					Image cImage;
					ImageBuffer *pImageBuffer = cImage.CreatePart()->CreateMipmap();
					pImageBuffer->CreateImage(DataFloat, ColorRGBA, Vector3i(nRandomNormalsTextureSize, nRandomNormalsTextureSize, 1));

					// Create the texture data
					float *pfData = reinterpret_cast<float*>(pImageBuffer->GetData());
					for (int i=0; i<64*64*4; i+=4) {
						const float fAngle = static_cast<float>(2.0f*Math::Pi*Math::GetRandFloat()/static_cast<float>(m_nNumberOfDirections));
						pfData[i  ] = Math::Cos(fAngle);
						pfData[i+1] = Math::Sin(fAngle);
						pfData[i+2] = Math::GetRandFloat();
						pfData[i+3] = 0.0f;
					}

					// Create the texture
					m_pRandomNormalsTexture = cRenderer.CreateTextureBuffer2D(cImage, TextureBuffer::Unknown, TextureBuffer::Mipmaps);
				}

				// Set texture
				if (pGeneratedProgramUserData->pRandomNormalsMap) {
					const int nTextureUnit = pGeneratedProgramUserData->pRandomNormalsMap->Set(m_pRandomNormalsTexture);
					if (nTextureUnit >= 0) {
						cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU,  TextureAddressing::Wrap);
						cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV,  TextureAddressing::Wrap);
						cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
						cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
						cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
					}
				}
			}

			// Resolution
			if (pGeneratedProgramUserData->pResolution)
				pGeneratedProgramUserData->pResolution->Set(cNormalDepthTextureBuffer.GetSize());

			// Set the normal depth texture
			if (pGeneratedProgramUserData->pNormalDepthMap) {
				const int nTextureUnit = pGeneratedProgramUserData->pNormalDepthMap->Set(&cNormalDepthTextureBuffer);
				if (nTextureUnit >= 0) {
					cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU,  TextureAddressing::Clamp);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV,  TextureAddressing::Clamp);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
				}
			}

			// Draw the fullscreen quad
			cRenderer.DrawPrimitives(Primitive::TriangleStrip, 0, 4);
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
