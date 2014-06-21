/*********************************************************\
 *  File: SRPDeferredHBAO.cpp                            *
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
#include <PLGraphics/Image/ImageBuffer.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Program.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ProgramAttribute.h>
#include <PLRenderer/Renderer/TextureBuffer2D.h>
#include <PLRenderer/Renderer/TextureBufferRectangle.h>
#include <PLRenderer/Texture/TextureManager.h>
#include <PLScene/Scene/SNCamera.h>
#include "PLCompositing/Shaders/Deferred/SRPDeferredGBuffer.h"
#include "PLCompositing/Shaders/Deferred/SRPDeferredHBAO.h"


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
pl_implement_class(SRPDeferredHBAO)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
uint32 SRPDeferredHBAO::GetNumberOfDirections() const
{
	return m_nNumberOfDirections;
}

void SRPDeferredHBAO::SetNumberOfDirections(uint32 nValue)
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
SRPDeferredHBAO::SRPDeferredHBAO() :
	NumberOfSteps(this),
	NumberOfDirections(this),
	AORadius(this),
	AngleBias(this),
	Attenuation(this),
	RandomUVScale(this),
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
SRPDeferredHBAO::~SRPDeferredHBAO()
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
void SRPDeferredHBAO::DrawAO(const String &sShaderLanguage, VertexBuffer &cVertexBuffer, TextureBufferRectangle &cNormalDepthTextureBuffer)
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
			#include "SRPDeferredHBAO_GLSL.h"
			m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sDeferredHBAO_GLSL_VS, "130", sDeferredHBAO_GLSL_FS, "130");	// OpenGL 3.0 ("#version 130")
		} else if (sShaderLanguage == "Cg") {
			#include "SRPDeferredHBAO_Cg.h"
			m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sDeferredHBAO_Cg_VS, "glslv", sDeferredHBAO_Cg_FS, "glslf");
		}
	}

	// If there's no program generator, we don't need to continue
	if (m_pProgramGenerator) {
		// Reset the program flags
		m_cProgramFlags.Reset();

		// Set program flags
		if (!(GetFlags() & NoNormals))
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_NORMAL)

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
				static const String sNumSteps = "NumSteps";
				pGeneratedProgramUserData->pNumSteps			= pProgram->GetUniform(sNumSteps);
				static const String sNumDir = "NumDir";
				pGeneratedProgramUserData->pNumDir				= pProgram->GetUniform(sNumDir);
				static const String sRadius = "Radius";
				pGeneratedProgramUserData->pRadius				= pProgram->GetUniform(sRadius);
				static const String sInvR = "InvR";
				pGeneratedProgramUserData->pInvR				= pProgram->GetUniform(sInvR);
				static const String sSqrR = "SqrR";
				pGeneratedProgramUserData->pSqrR				= pProgram->GetUniform(sSqrR);
				static const String sAngleBias = "AngleBias";
				pGeneratedProgramUserData->pAngleBias			= pProgram->GetUniform(sAngleBias);
				static const String sTanAngleBias = "TanAngleBias";
				pGeneratedProgramUserData->pTanAngleBias		= pProgram->GetUniform(sTanAngleBias);
				static const String sContrast = "Contrast";
				pGeneratedProgramUserData->pContrast			= pProgram->GetUniform(sContrast);
				static const String sAttenuation = "Attenuation";
				pGeneratedProgramUserData->pAttenuation			= pProgram->GetUniform(sAttenuation);
				static const String sFocalLen = "FocalLen";
				pGeneratedProgramUserData->pFocalLen			= pProgram->GetUniform(sFocalLen);
				static const String sInvFocalLen = "InvFocalLen";
				pGeneratedProgramUserData->pInvFocalLen			= pProgram->GetUniform(sInvFocalLen);
				static const String sResolution = "Resolution";
				pGeneratedProgramUserData->pResolution			= pProgram->GetUniform(sResolution);
				static const String sInvResolution = "InvResolution";
				pGeneratedProgramUserData->pInvResolution		= pProgram->GetUniform(sInvResolution);
				static const String sRandomUVScale = "RandomUVScale";
				pGeneratedProgramUserData->pRandomUVScale		= pProgram->GetUniform(sRandomUVScale);
				static const String sRandomNormalsMap = "RandomNormalsMap";
				pGeneratedProgramUserData->pRandomNormalsMap	= pProgram->GetUniform(sRandomNormalsMap);
				static const String sNormalDepthMap = "NormalDepthMap";
				pGeneratedProgramUserData->pNormalDepthMap		= pProgram->GetUniform(sNormalDepthMap);
			}

			// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
			if (pGeneratedProgramUserData->pVertexPosition)
				pGeneratedProgramUserData->pVertexPosition->Set(&cVertexBuffer, PLRenderer::VertexBuffer::Position);

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

			// Get the width and height of the texture buffer
			const float fWidth  = static_cast<float>(cNormalDepthTextureBuffer.GetSize().x);
			const float fHeight = static_cast<float>(cNormalDepthTextureBuffer.GetSize().y);

			// NumSteps
			if (pGeneratedProgramUserData->pNumSteps)
				pGeneratedProgramUserData->pNumSteps->Set(static_cast<int>(NumberOfSteps));

			// NumDir
			if (pGeneratedProgramUserData->pNumDir)
				pGeneratedProgramUserData->pNumDir->Set(static_cast<float>(m_nNumberOfDirections)); // (should be of integer type, but that's causing troubles on some GPU drivers, see PLCompositing diary entry 02.06.2011 for more details)

			{ // Radius
				// Calculate the radius to use
				const float fRadius = SceneScale * AORadius;

				// Radius
				if (pGeneratedProgramUserData->pRadius)
					pGeneratedProgramUserData->pRadius->Set(fRadius);

				// InvR
				if (pGeneratedProgramUserData->pInvR)
					pGeneratedProgramUserData->pInvR->Set(1.0f/fRadius);

				// SqrR
				if (pGeneratedProgramUserData->pSqrR)
					pGeneratedProgramUserData->pSqrR->Set(fRadius*fRadius);
			}

			{ // Angle bias and contrast
				// Calculate the angle in radians
				const float fAngle = static_cast<float>(AngleBias * Math::DegToRad);

				// AngleBias
				if (pGeneratedProgramUserData->pAngleBias)
					pGeneratedProgramUserData->pAngleBias->Set(fAngle);

				// TanAngleBias
				if (pGeneratedProgramUserData->pTanAngleBias)
					pGeneratedProgramUserData->pTanAngleBias->Set(Math::Tan(fAngle));

				// Contrast
				const float fContrast = Contrast / (1.0f - Math::Sin(fAngle));
				if (pGeneratedProgramUserData->pContrast)
					pGeneratedProgramUserData->pContrast->Set(fContrast);
			}

			// Attenuation
			if (pGeneratedProgramUserData->pAttenuation)
				pGeneratedProgramUserData->pAttenuation->Set(Attenuation);

			{ // Focal length
				// Calculate the focal length
				const float fFovY         = static_cast<float>((SNCamera::GetCamera() ? SNCamera::GetCamera()->GetFOV() : 45.0f) * Math::DegToRad);
				const float fFocalLengthY = 1.0f / Math::Tan(fFovY * 0.5f);
				const float fFocalLengthX = fFocalLengthY * ((fHeight * (SNCamera::GetCamera() ? SNCamera::GetCamera()->GetAspect() : 1.0f)) / fWidth);

				// FocalLen
				if (pGeneratedProgramUserData->pFocalLen)
					pGeneratedProgramUserData->pFocalLen->Set(fFocalLengthX, fFocalLengthY);

				// InvFocalLen
				if (pGeneratedProgramUserData->pInvFocalLen)
					pGeneratedProgramUserData->pInvFocalLen->Set(1.0f/fFocalLengthX, 1.0f/fFocalLengthY);
			}

			// Resolution
			if (pGeneratedProgramUserData->pResolution)
				pGeneratedProgramUserData->pResolution->Set(fWidth, fHeight);

			// InvResolution
			if (pGeneratedProgramUserData->pInvResolution)
				pGeneratedProgramUserData->pInvResolution->Set(1.0f/fWidth, 1.0f/fHeight);

			// RandomUVScale
			if (pGeneratedProgramUserData->pRandomUVScale)
				pGeneratedProgramUserData->pRandomUVScale->Set(RandomUVScale.Get());

			{ // Set the "RandomNormalsMap" fragment shader parameter
				// Create texture if required
				if (!m_pRandomNormalsTexture) {
					const uint32 nWidth  = 64;
					const uint32 nHeight = 64;

					// Create the image
					Image cImage = Image::CreateImage(DataFloat, ColorRGBA, Vector3i(nWidth, nHeight, 1));
					ImageBuffer *pImageBuffer = cImage.GetBuffer();

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
					m_pRandomNormalsTexture = cRenderer.CreateTextureBuffer2D(cImage, TextureBuffer::Unknown, 0);
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

			// Draw the fullscreen quad
			cRenderer.DrawPrimitives(Primitive::TriangleStrip, 0, 4);
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
