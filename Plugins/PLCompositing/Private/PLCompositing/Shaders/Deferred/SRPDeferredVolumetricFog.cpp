/*********************************************************\
 *  File: SRPDeferredVolumetricFog.cpp                   *
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
#include <PLScene/Scene/SNSphereFog.h>
#include <PLScene/Visibility/SQCull.h>
#include <PLScene/Visibility/VisPortal.h>
#include <PLScene/Visibility/VisContainer.h>
#include "PLCompositing/FullscreenQuad.h"
#include "PLCompositing/Shaders/Deferred/SRPDeferredGBuffer.h"
#include "PLCompositing/Shaders/Deferred/SRPDeferredVolumetricFog.h"


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
pl_implement_class(SRPDeferredVolumetricFog)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPDeferredVolumetricFog::SRPDeferredVolumetricFog() :
	ShaderLanguage(this),
	m_pProgramGenerator(nullptr),
	m_bFirstFog(false)
{
}

/**
*  @brief
*    Destructor
*/
SRPDeferredVolumetricFog::~SRPDeferredVolumetricFog()
{
	// Destroy the program generator
	if (m_pProgramGenerator)
		delete m_pProgramGenerator;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Draws recursive
*/
void SRPDeferredVolumetricFog::DrawRec(Renderer &cRenderer, const SQCull &cCullQuery, SRPDeferredGBuffer &cSRPDeferredGBuffer)
{
	// Get scene container
	const VisContainer &cVisContainer = cCullQuery.GetVisContainer();

	// Render all visible scene nodes of this scene container
	Iterator<VisNode*> cIterator = cVisContainer.GetVisNodes().GetIterator();
	while (cIterator.HasNext()) {
		// Get visibility node and scene node
		const VisNode   *pVisNode   = cIterator.Next();
			  SceneNode *pSceneNode = pVisNode->GetSceneNode();
		if (pSceneNode) {
			// Is this scene node a portal?
			if (pVisNode->IsPortal()) {
				// Get the target cell visibility container
				const VisContainer *pVisCell = static_cast<const VisPortal*>(pVisNode)->GetTargetVisContainer();
				if (pVisCell && pVisCell->GetCullQuery()) {
					// Draw the target cell
					DrawRec(cRenderer, *pVisCell->GetCullQuery(), cSRPDeferredGBuffer);
				}

			// Is this scene node a container? We do not need to check for cells because we will
			// NEVER receive cells from SQCull directly, they are ONLY visible through portals! (see above)
			} else if (pVisNode->IsContainer()) {
				// Draw this container without special processing
				if (static_cast<const VisContainer*>(pVisNode)->GetCullQuery())
					DrawRec(cRenderer, *static_cast<const VisContainer*>(pVisNode)->GetCullQuery(), cSRPDeferredGBuffer);

			// Is this a fog node?
			} else if (pSceneNode->IsFog()) {
				SNFog *pFog = static_cast<SNFog*>(pSceneNode);
				// Spherical fog?
				if (pFog->IsSphereFog())
					RenderFog(cRenderer, cCullQuery, cSRPDeferredGBuffer, *static_cast<SNSphereFog*>(pSceneNode), *pVisNode);

			// This must just be a quite boring scene node, ignore it
			} else {
			}
		}
	}
}

/**
*  @brief
*    Renders a fog
*/
void SRPDeferredVolumetricFog::RenderFog(Renderer &cRenderer, const SQCull &cCullQuery, SRPDeferredGBuffer &cSRPDeferredGBuffer, SNSphereFog &cFog, const VisNode &cFogVisNode)
{
	// Calculate the scissor rectangle
	Rectangle cScissorRectangle;
	{
		// Get the vertices of the light scene node bounding box
		Array<Vector3> vVertex(8);
		cFog.GetAABoundingBox().GetVertices(vVertex.GetData());

		// Get viewport parameters
		const uint32 nX      = static_cast<uint32>(cRenderer.GetViewport().GetX());
		const uint32 nY      = static_cast<uint32>(cRenderer.GetViewport().GetY());
		const uint32 nWidth  = static_cast<uint32>(cRenderer.GetViewport().GetWidth());
		const uint32 nHeight = static_cast<uint32>(cRenderer.GetViewport().GetHeight());

		// Calculate the rectangle on the screen enclosing the given vertices
		cScissorRectangle.ScreenRectangle(vVertex, true, cFogVisNode.GetWorldViewProjectionMatrix(), nX, nY, nWidth, nHeight);
	}

	// Is the scissor rectangle visible?
	if (cScissorRectangle.GetWidth() || cScissorRectangle.GetHeight()) {
		// Set the scissor rectangle
		cRenderer.SetScissorRect(&cScissorRectangle);

		// Is this the first for rendered in the current pass?
		if (m_bFirstFog) {
			RendererSetup(cRenderer, cSRPDeferredGBuffer);
			m_bFirstFog = false;
		}

		// If there's no program generator, we don't need to continue
		if (m_pProgramGenerator) {
			// Get a program instance from the program generator using the given program flags
			ProgramGenerator::GeneratedProgram *pGeneratedProgram = m_pProgramGenerator->GetProgram(m_cProgramFlags);
			if (pGeneratedProgram) {
				// Set pointers to uniforms & attributes of a generated program if they are not set yet
				GeneratedProgramUserData *pGeneratedProgramUserData = static_cast<GeneratedProgramUserData*>(pGeneratedProgram->pUserData);
				if (pGeneratedProgramUserData) {
					// Get the fullscreen quad instance
					FullscreenQuad *pFullscreenQuad = cSRPDeferredGBuffer.GetFullscreenQuad();
					if (pFullscreenQuad) {
						// Adjusted inverse square fall-off function that falls to zero at the light radius
						static const float a0 = 0.2f;
						static const float a1 = a0 + 1.0f;
						static const float c  = Math::Sqrt(a1 / a0 - 1.0f);

						// View space fog position
						if (pGeneratedProgramUserData->pFogPosition)
							pGeneratedProgramUserData->pFogPosition->Set(cFogVisNode.GetWorldViewMatrix().GetTranslation());

						// View space fog inverse radius
						if (pGeneratedProgramUserData->pFogInvRadius)
							pGeneratedProgramUserData->pFogInvRadius->Set(c/cFog.GetRange());

						// Fog color 0
						if (pGeneratedProgramUserData->pFogColor0)
							pGeneratedProgramUserData->pFogColor0->Set(cFog.Color.Get()*a1*cFog.Volumetricy);

						// Fog color 1
						if (pGeneratedProgramUserData->pFogColor1)
							pGeneratedProgramUserData->pFogColor1->Set(cFog.Color.Get()*a0*cFog.Volumetricy);

						// Draw the fullscreen quad
						pFullscreenQuad->Draw(true);
					}
				}
			}
		}
	}
}

/**
*  @brief
*    Renderer setup
*/
void SRPDeferredVolumetricFog::RendererSetup(Renderer &cRenderer, SRPDeferredGBuffer &cSRPDeferredGBuffer)
{
	// Get the fullscreen quad instance
	FullscreenQuad *pFullscreenQuad = cSRPDeferredGBuffer.GetFullscreenQuad();
	if (pFullscreenQuad) {
		// Get the vertex buffer of the fullscreen quad
		VertexBuffer *pVertexBuffer = pFullscreenQuad->GetVertexBuffer();
		if (pVertexBuffer) {
			// Get the texture buffer to use
			TextureBufferRectangle *pTextureBuffer = cSRPDeferredGBuffer.GetRenderTargetTextureBuffer(0);
			if (pTextureBuffer) {
				// Get the normal/depth texture buffer to use
				TextureBufferRectangle *pNormalDepthTextureBuffer = cSRPDeferredGBuffer.GetRenderTargetTextureBuffer(1);
				if (pNormalDepthTextureBuffer) {
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
							#include "SRPDeferredVolumetricFog_GLSL.h"
							m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sDeferredVolumetricFog_GLSL_VS, "110", sDeferredVolumetricFog_GLSL_FS, "110");	// OpenGL 2.0 ("#version 110")
						} else if (sShaderLanguage == "Cg") {
							#include "SRPDeferredVolumetricFog_Cg.h"
							m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sDeferredVolumetricFog_Cg_VS, "arbvp1", sDeferredVolumetricFog_Cg_FS, "arbfp1");
						}
					}

					// If there's no program generator, we don't need to continue
					if (m_pProgramGenerator) {
						// Reset all render states to default
						cRenderer.GetRendererContext().GetEffectManager().Use();

						// Setup render states
						cRenderer.SetRenderState(RenderState::ScissorTestEnable, true);
						cRenderer.SetRenderState(RenderState::FixedFillMode,	 Fill::Solid);
						cRenderer.SetRenderState(RenderState::CullMode,			 Cull::None);
						cRenderer.SetRenderState(RenderState::ZEnable,			 false);
						cRenderer.SetRenderState(RenderState::ZWriteEnable,		 false);

						// Set blend mode
						cRenderer.SetRenderState(RenderState::BlendEnable,	true);
						cRenderer.SetRenderState(RenderState::SrcBlendFunc,	BlendFunc::One);
						cRenderer.SetRenderState(RenderState::DstBlendFunc,	BlendFunc::One);

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
								static const String sFogPosition = "FogPosition";
								pGeneratedProgramUserData->pFogPosition	= pProgram->GetUniform(sFogPosition);
								static const String sFogInvRadius = "FogInvRadius";
								pGeneratedProgramUserData->pFogInvRadius	= pProgram->GetUniform(sFogInvRadius);
								static const String sFogColor0 = "FogColor0";
								pGeneratedProgramUserData->pFogColor0	= pProgram->GetUniform(sFogColor0);
								static const String sFogColor1 = "FogColor1";
								pGeneratedProgramUserData->pFogColor1	= pProgram->GetUniform(sFogColor1);
								static const String sInvFocalLen = "InvFocalLen";
								pGeneratedProgramUserData->pInvFocalLen	= pProgram->GetUniform(sInvFocalLen);
								static const String sResolution = "Resolution";
								pGeneratedProgramUserData->pResolution	= pProgram->GetUniform(sResolution);
								static const String sNormalDepthMap = "NormalDepthMap";
								pGeneratedProgramUserData->pNormalDepthMap	= pProgram->GetUniform(sNormalDepthMap);
							}

							// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
							if (pGeneratedProgramUserData->pVertexPosition)
								pGeneratedProgramUserData->pVertexPosition->Set(pVertexBuffer, PLRenderer::VertexBuffer::Position);

							{ // Focal length
								// Get the width and height of the texture buffer
								const float fWidth  = static_cast<float>(pNormalDepthTextureBuffer->GetSize().x);
								const float fHeight = static_cast<float>(pNormalDepthTextureBuffer->GetSize().y);

								// Calculate the focal length
								const float fFovY         = static_cast<float>((SNCamera::GetCamera() ? SNCamera::GetCamera()->GetFOV() : 45.0f) * Math::DegToRad);
								const float fFocalLengthY = 1.0f / Math::Tan(fFovY * 0.5f);
								const float fFocalLengthX = fFocalLengthY * ((fHeight * (SNCamera::GetCamera() ? SNCamera::GetCamera()->GetAspect() : 1.0f)) / fWidth);

								// InvFocalLen
								if (pGeneratedProgramUserData->pInvFocalLen)
									pGeneratedProgramUserData->pInvFocalLen->Set(1.0f/fFocalLengthX, 1.0f/fFocalLengthY);
							}

							// Resolution
							if (pGeneratedProgramUserData->pResolution)
								pGeneratedProgramUserData->pResolution->Set(pNormalDepthTextureBuffer->GetSize());

							// Set the normal depth texture
							if (pGeneratedProgramUserData->pNormalDepthMap) {
								const int nTextureUnit = pGeneratedProgramUserData->pNormalDepthMap->Set(pNormalDepthTextureBuffer);
								if (nTextureUnit >= 0) {
									cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU,  TextureAddressing::Clamp);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV,  TextureAddressing::Clamp);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
								}
							}
						}
					}
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRPDeferredVolumetricFog::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Get the instance of the "PLCompositing::SRPDeferredGBuffer" scene renderer pass
	SRPDeferredGBuffer *pSRPDeferredGBuffer = GetGBuffer();
	if (pSRPDeferredGBuffer) {
		// Backup fixed fill mode render state
		const uint32 nFixedFillModeBackup = cRenderer.GetRenderState(RenderState::FixedFillMode);

		// Set the color mask, we don't write out alpha
		cRenderer.SetColorMask(true, true, true, false);

		// Draw recursive from front to back
		m_bFirstFog = true;
		DrawRec(cRenderer, cCullQuery, *pSRPDeferredGBuffer);

		// Restore color mask
		cRenderer.SetColorMask(true, true, true, true);

		// Restore fixed fill mode render state
		cRenderer.SetRenderState(RenderState::FixedFillMode, nFixedFillModeBackup);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
