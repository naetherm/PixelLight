/*********************************************************\
 *  File: SRPMotionBlurShaders.cpp                       *
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
#include <PLCore/Tools/Timing.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Program.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ProgramAttribute.h>
#include <PLRenderer/Renderer/TextureBufferRectangle.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLScene/Scene/SNCamera.h>
#include "PLCompositing/FullscreenQuad.h"
#include "PLCompositing/SRPBegin.h"
#include "PLCompositing/Shaders/PostProcessing/SRPMotionBlurShaders.h"


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
pl_implement_class(SRPMotionBlurShaders)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPMotionBlurShaders::SRPMotionBlurShaders() :
	ShaderLanguage(this),
	NumberOfSamples(this),
	MotionBlurFactor(this),
	TargetFPS(this),
	Flags(this),
	m_pFullscreenQuad(nullptr),
	m_pProgramGenerator(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
SRPMotionBlurShaders::~SRPMotionBlurShaders()
{
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
void SRPMotionBlurShaders::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Get the "PLCompositing::SRPBegin" instance
	SRPBegin *pSRPBegin = static_cast<SRPBegin*>(GetFirstInstanceOfSceneRendererPassClass("PLCompositing::SRPBegin"));
	if (pSRPBegin && NumberOfSamples != 0) {
		// Get the back render target of SRPBegin, this holds the current content
		SurfaceTextureBuffer *pBackSurfaceTextureBuffer = pSRPBegin->GetBackRenderTarget();
		if (pBackSurfaceTextureBuffer && pBackSurfaceTextureBuffer->GetTextureBuffer()) {
			// Get the depth render target of SRPBegin
			TextureBufferRectangle *pTextureBuffer = pSRPBegin->GetTextureBufferDepth();
			if (pTextureBuffer) {
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
							#include "SRPMotionBlurShaders_GLSL.h"
							m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sMotionBlur_GLSL_VS, "110", sMotionBlur_GLSL_FS, "110");	// OpenGL 2.0 ("#version 110")
						} else if (sShaderLanguage == "Cg") {
							#include "SRPMotionBlurShaders_Cg.h"
							m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sMotionBlur_Cg_VS, "glslv", sMotionBlur_Cg_FS, "glslf");
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

						// We need up-to-date back render target content, so swap the render targets
						pSRPBegin->SwapRenderTargets();

						// Reset the program flags
						m_cProgramFlags.Reset();

						// Show only velocity, everything else black? (for debugging)
						if (GetFlags() & ShowOnlyVelocity) {
							PL_ADD_FS_FLAG(m_cProgramFlags, FS_SHOW_ONLY_VELOCITY)
						} else {
							// Show velocity? (for debugging)
							if (GetFlags() & ShowVelocity)
								PL_ADD_FS_FLAG(m_cProgramFlags, FS_SHOW_VELOCITY)
						}

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
								pGeneratedProgramUserData->pVertexPosition					= pProgram->GetAttribute(sVertexPosition);
								// Fragment shader uniforms
								static const String sTextureSize = "TextureSize";
								pGeneratedProgramUserData->pTextureSize						= pProgram->GetUniform(sTextureSize);
								static const String sColorTexture = "ColorTexture";
								pGeneratedProgramUserData->pColorTexture					= pProgram->GetUniform(sColorTexture);
								static const String sDepthTexture = "DepthTexture";
								pGeneratedProgramUserData->pDepthTexture					= pProgram->GetUniform(sDepthTexture);
								static const String sClipSpaceToWorldSpace = "ClipSpaceToWorldSpace";
								pGeneratedProgramUserData->pClipSpaceToWorldSpace			= pProgram->GetUniform(sClipSpaceToWorldSpace);
								static const String sPreviousWorldSpaceToClipSpace = "PreviousWorldSpaceToClipSpace";
								pGeneratedProgramUserData->pPreviousWorldSpaceToClipSpace	= pProgram->GetUniform(sPreviousWorldSpaceToClipSpace);
								static const String sNumberOfSamples = "NumberOfSamples";
								pGeneratedProgramUserData->pNumberOfSamples					= pProgram->GetUniform(sNumberOfSamples);
								static const String sVelocityFactor = "VelocityFactor";
								pGeneratedProgramUserData->pVelocityFactor					= pProgram->GetUniform(sVelocityFactor);
							}

							// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
							if (pGeneratedProgramUserData->pVertexPosition)
								pGeneratedProgramUserData->pVertexPosition->Set(pVertexBuffer, PLRenderer::VertexBuffer::Position);

							// Set texture size
							if (pGeneratedProgramUserData->pTextureSize)
								pGeneratedProgramUserData->pTextureSize->Set(pTextureBuffer->GetSize());

							// Set the "ColorTexture" fragment shader parameter
							if (pGeneratedProgramUserData->pColorTexture) {
								const int nTextureUnit = pGeneratedProgramUserData->pColorTexture->Set(pBackSurfaceTextureBuffer->GetTextureBuffer());
								if (nTextureUnit >= 0) {
									cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
								}
							}

							// Set the "DepthTexture" fragment shader parameter
							if (pGeneratedProgramUserData->pDepthTexture) {
								const int nTextureUnit = pGeneratedProgramUserData->pDepthTexture->Set(pTextureBuffer);
								if (nTextureUnit >= 0) {
									cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
								}
							}

							// Set the "ClipSpaceToWorldSpace" and "PreviousWorldSpaceToClipSpace" fragment shader parameter
							if (pGeneratedProgramUserData->pClipSpaceToWorldSpace && pGeneratedProgramUserData->pPreviousWorldSpaceToClipSpace) {
								// Get the currently used camera scene node
								SNCamera *pSNCamera = SNCamera::GetCamera();
								if (pSNCamera) {
									// Get the projection matrix
									const Matrix4x4 &mProjection = pSNCamera->GetProjectionMatrix(cRenderer.GetViewport());

									// Get the view matrix
									const Matrix4x4 &mView = pSNCamera->GetViewMatrix();

									// Calculate the world space to clip space transform matrix
									// -> Concatenate (multiply) the view matrix and the projection matrix
									Matrix4x4 mWorldSpaceToClipSpace = mProjection;
									mWorldSpaceToClipSpace *= mView;

									// Calculate the clip space to world space transform matrix
									Matrix4x4 mClipSpaceToWorldSpace = mWorldSpaceToClipSpace;
									mClipSpaceToWorldSpace.Invert();

									// Set the fragment shader parameters
									pGeneratedProgramUserData->pClipSpaceToWorldSpace->Set(mClipSpaceToWorldSpace);
									pGeneratedProgramUserData->pPreviousWorldSpaceToClipSpace->Set(m_mPreviousWorldSpaceToClipSpace);

									// Backup the current world space to clip space matrix
									// [TODO] Would be nice if the "previous" data would not be stored in this instance...
									m_mPreviousWorldSpaceToClipSpace = mWorldSpaceToClipSpace;
								}
							}

							// Set number of samples
							if (pGeneratedProgramUserData->pNumberOfSamples)
								pGeneratedProgramUserData->pNumberOfSamples->Set(static_cast<float>((NumberOfSamples > 0) ? NumberOfSamples : 1));	// (should be of integer type, but that's causing troubles on some GPU drivers, see PLCompositing diary entry 02.06.2011 for more details)

							// Set velocity factor
							// -> /2.0 is build into VelocityFactor
							if (pGeneratedProgramUserData->pVelocityFactor) {
								// Adaptive motion blur factor by using a target FPS?
								// -> Described at http://www.john-chapman.net/content.php?id=11
								const float fFactor = (GetFlags() & NoTargetFPS) ? 0.5f : (Timing::GetInstance()->GetFramesPerSecond()/TargetFPS.Get())*0.5f;

								// Set uniform
								pGeneratedProgramUserData->pVelocityFactor->Set(MotionBlurFactor.Get()*fFactor);
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
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
