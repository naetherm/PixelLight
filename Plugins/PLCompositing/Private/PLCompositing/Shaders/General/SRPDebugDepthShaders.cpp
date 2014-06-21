/*********************************************************\
 *  File: SRPDebugDepthShaders.cpp                       *
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
#include <PLScene/Scene/SNCamera.h>
#include "PLCompositing/FullscreenQuad.h"
#include "PLCompositing/SRPBegin.h"
#include "PLCompositing/Shaders/General/SRPDebugDepthShaders.h"


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
pl_implement_class(SRPDebugDepthShaders)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPDebugDepthShaders::SRPDebugDepthShaders() :
	ShaderLanguage(this),
	Flags(this),
	m_pFullscreenQuad(nullptr),
	m_pProgramGenerator(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
SRPDebugDepthShaders::~SRPDebugDepthShaders()
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
void SRPDebugDepthShaders::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Get the "PLCompositing::SRPBegin" instance
	SRPBegin *pSRPBegin = static_cast<SRPBegin*>(GetFirstInstanceOfSceneRendererPassClass("PLCompositing::SRPBegin"));
	if (pSRPBegin) {
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
						#include "SRPDebugDepthShaders_GLSL.h"
						m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sDebugDepth_GLSL_VS, "110", sDebugDepth_GLSL_FS, "110");	// OpenGL 2.0 ("#version 110")
					} else if (sShaderLanguage == "Cg") {
						#include "SRPDebugDepthShaders_Cg.h"
						m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sDebugDepth_Cg_VS, "glslv", sDebugDepth_Cg_FS, "glslf");
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

					// Reset the program flags
					m_cProgramFlags.Reset();

					// Get the currently used camera scene node
					float fZNear = 0.0f;
					float fZFar  = 1.0f;
					SNCamera *pSNCamera = SNCamera::GetCamera();
					if (pSNCamera) {
						// Get Z near plane and Z far plane
						fZNear = pSNCamera->ZNear.Get();
						fZFar  = pSNCamera->ZFar.Get();

						// Set program flags
						if (!(GetFlags() & NoLinearizedDepth))
							PL_ADD_FS_FLAG(m_cProgramFlags, FS_LINEARIZE_DEPTH)
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
							pGeneratedProgramUserData->pVertexPosition	= pProgram->GetAttribute(sVertexPosition);
							// Vertex shader uniforms
							static const String sTextureSize = "TextureSize";
							pGeneratedProgramUserData->pTextureSize		= pProgram->GetUniform(sTextureSize);
							// Fragment shader uniforms
							static const String sTexture = "Texture";
							pGeneratedProgramUserData->pTexture			= pProgram->GetUniform(sTexture);
							static const String sZNearFar = "ZNearFar";
							pGeneratedProgramUserData->pZNearFar		= pProgram->GetUniform(sZNearFar);
						}

						// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
						if (pGeneratedProgramUserData->pVertexPosition)
							pGeneratedProgramUserData->pVertexPosition->Set(pVertexBuffer, PLRenderer::VertexBuffer::Position);

						// Set texture size
						if (pGeneratedProgramUserData->pTextureSize)
							pGeneratedProgramUserData->pTextureSize->Set(pTextureBuffer->GetSize());

						// Set Z near plane and Z far plane (x=Z near plane, y=Z far plane)
						if (pGeneratedProgramUserData->pZNearFar)
							pGeneratedProgramUserData->pZNearFar->Set(fZNear, fZFar);

						// Set the "Texture" fragment shader parameter
						if (pGeneratedProgramUserData->pTexture) {
							const int nTextureUnit = pGeneratedProgramUserData->pTexture->Set(pTextureBuffer);
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
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
