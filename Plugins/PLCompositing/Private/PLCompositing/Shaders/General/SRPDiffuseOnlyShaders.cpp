/*********************************************************\
 *  File: SRPDiffuseOnlyShaders.cpp                      *
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
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Renderer/RenderStates.h>
#include <PLRenderer/Renderer/Shader.h>
#include <PLRenderer/Renderer/Program.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ProgramAttribute.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLRenderer/Material/Material.h>
#include <PLRenderer/Material/Parameter.h>
#include <PLMesh/MeshHandler.h>
#include <PLMesh/MeshLODLevel.h>
#include <PLScene/Visibility/VisNode.h>
#include "PLCompositing/Shaders/General/SRPDiffuseOnlyShaders.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLMesh;
using namespace PLScene;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SRPDiffuseOnlyShaders)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPDiffuseOnlyShaders::SRPDiffuseOnlyShaders() :
	ShaderLanguage(this),
	Flags(this),
	m_pProgramGenerator(nullptr),
	m_pRenderStates(new RenderStates())
{
}

/**
*  @brief
*    Destructor
*/
SRPDiffuseOnlyShaders::~SRPDiffuseOnlyShaders()
{
	// Destroy render states 'translator'
	delete m_pRenderStates;

	// Destroy the program generator
	if (m_pProgramGenerator)
		delete m_pProgramGenerator;
}


//[-------------------------------------------------------]
//[ Private SRPDiffuseOnly functions                      ]
//[-------------------------------------------------------]
void SRPDiffuseOnlyShaders::DrawMesh(Renderer &cRenderer, const SQCull &cCullQuery, const VisNode &cVisNode, SceneNode &cSceneNode, const MeshHandler &cMeshHandler, const Mesh &cMesh, const MeshLODLevel &cMeshLODLevel, VertexBuffer &cVertexBuffer)
{
	// Get buffers
		  IndexBuffer     *pIndexBuffer  = cMeshLODLevel.GetIndexBuffer();
	const Array<Geometry> &lstGeometries = *cMeshLODLevel.GetGeometries();

	// Bind buffers
	cRenderer.SetIndexBuffer(pIndexBuffer);

	// Get available vertex buffer attributes
	const bool bHasVertexTexCoord = (cVertexBuffer.GetVertexAttribute(VertexBuffer::TexCoord) != nullptr);

	// Draw mesh
	for (uint32 nMat=0; nMat<cMeshHandler.GetNumOfMaterials(); nMat++) {
		// Get mesh material
		const Material *pMaterial = cMeshHandler.GetMaterial(nMat);
		if (pMaterial) {
			// Draw geometries
			for (uint32 nGeo=0; nGeo<lstGeometries.GetNumOfElements(); nGeo++) {
				// Is this geometry active and is it using the current used mesh material?
				const Geometry &cGeometry = lstGeometries[nGeo];
				if (cGeometry.IsActive() && nMat == cGeometry.GetMaterial()) {
					// Transparent material?
					static const String sOpacity = "Opacity";
					const Parameter *pParameter = pMaterial->GetParameter(sOpacity);
					const bool bTransparentPass = (GetFlags() & TransparentPass) != 0;
					if (bTransparentPass ? (pParameter && pParameter->GetValue1f() < 1.0f) : (!pParameter || pParameter->GetValue1f() >= 1.0f)) {
						bool bDraw = true;

						// "OpacityOcclusion" mode?
						if (bTransparentPass && (GetFlags() & ZWriteOnly)) {
							static const String sOpacityOcclusion = "OpacityOcclusion";
							pParameter = pMaterial->GetParameter(sOpacityOcclusion);
							bDraw = (pParameter && pParameter->GetValue1f() >= 1.0f);
						}

						// Draw?
						if (bDraw) {
							// Reset the program flags
							m_cProgramFlags.Reset();

							// Get opacity
							const float fOpacity = pParameter ? pParameter->GetValue1f() : 1.0f;
							if (fOpacity < 1) {
								// Get and set source blend function
								uint32 nValue = BlendFunc::SrcAlpha;
								static const String sSrcBlendFunc = "SrcBlendFunc";
								pParameter = pMaterial->GetParameter(sSrcBlendFunc);
								if (pParameter) {
									m_pRenderStates->SetAttribute("SrcBlendFunc", pParameter->GetParameterString());
									nValue = m_pRenderStates->Get(RenderState::SrcBlendFunc);
								}
								cRenderer.SetRenderState(RenderState::SrcBlendFunc, nValue);

								// Get and set destination blend function
								nValue = BlendFunc::InvSrcAlpha;
								static const String sDstBlendFunc = "DstBlendFunc";
								pParameter = pMaterial->GetParameter(sDstBlendFunc);
								if (pParameter) {
									m_pRenderStates->SetAttribute("DstBlendFunc", pParameter->GetParameterString());
									nValue = m_pRenderStates->Get(RenderState::DstBlendFunc);
								}
								cRenderer.SetRenderState(RenderState::DstBlendFunc, nValue);
							}

							{ // Two sided
								static const String sTwoSided = "TwoSided";
								pParameter = pMaterial->GetParameter(sTwoSided);
								cRenderer.SetRenderState(RenderState::CullMode, (pParameter && pParameter->GetValue1f()) == 1.0f ? Cull::None : Cull::CCW);
							}

							// Use gamma correction? Not required if this is a z write only render pass
							if (!(GetFlags() & NoGammaCorrection) && !(GetFlags() & ZWriteOnly))
								PL_ADD_FS_FLAG(m_cProgramFlags, FS_GAMMACORRECTION)

							// Diffuse map and alpha reference
							float fAlphaReference = 0.0f;
							TextureBuffer *pDiffuseMap = (!bHasVertexTexCoord || (GetFlags() & NoDiffuseMap)) ? nullptr : pMaterial->GetParameterTextureBuffer(Material::DiffuseMap);
							if (pDiffuseMap) {
								// Get alpha reference
								if (pDiffuseMap->GetComponentsPerPixel() == 4) {
									static const String sAlphaReference = "AlphaReference";
									pParameter = pMaterial->GetParameter(sAlphaReference);
									fAlphaReference = pParameter ? pParameter->GetValue1f() : 0.5f;
									if (fAlphaReference != 0.0f)
										PL_ADD_FS_FLAG(m_cProgramFlags, FS_ALPHATEST)
								}

								// If this a z write only render pass, we only need the diffuse map to perform an alpha test...
								if (!(GetFlags() & ZWriteOnly) || (m_cProgramFlags.GetFragmentShaderFlags() & FS_ALPHATEST)) {
									PL_ADD_VS_FLAG(m_cProgramFlags, VS_TEXCOORD0)
									PL_ADD_FS_FLAG(m_cProgramFlags, FS_DIFFUSEMAP)
								} else {
									// We don't need the diffuse map, just ignore it
									pDiffuseMap = nullptr;
								}
							}

							// Get a program instance from the program generator using the given program flags
							ProgramGenerator::GeneratedProgram *pGeneratedProgram = m_pProgramGenerator->GetProgram(m_cProgramFlags);

							// Make our program to the current one
							if (pGeneratedProgram && cRenderer.SetProgram(pGeneratedProgram->pProgram)) {
								// Set pointers to uniforms & attributes of a generated program if they are not set yet
								static const String sDiffuseColor = "DiffuseColor";
								GeneratedProgramUserData *pGeneratedProgramUserData = static_cast<GeneratedProgramUserData*>(pGeneratedProgram->pUserData);
								if (!pGeneratedProgramUserData) {
									pGeneratedProgram->pUserData = pGeneratedProgramUserData = new GeneratedProgramUserData;
									Program *pProgram = pGeneratedProgram->pProgram;
									// Vertex shader attributes
									static const String sVertexPosition = "VertexPosition";
									pGeneratedProgramUserData->pVertexPosition					= pProgram->GetAttribute(sVertexPosition);
									static const String sVertexTexCoord0 = "VertexTexCoord0";
									pGeneratedProgramUserData->pVertexTexCoord0					= pProgram->GetAttribute(sVertexTexCoord0);
									// Vertex shader uniforms
									static const String sObjectSpaceToClipSpaceMatrix = "ObjectSpaceToClipSpaceMatrix";
									pGeneratedProgramUserData->pObjectSpaceToClipSpaceMatrix	= pProgram->GetUniform(sObjectSpaceToClipSpaceMatrix);
									// Fragment shader uniforms
									pGeneratedProgramUserData->pDiffuseColor					= pProgram->GetUniform(sDiffuseColor);
									pGeneratedProgramUserData->pDiffuseMap						= pProgram->GetUniform(Material::DiffuseMap);
									static const String sAlphaReference = "AlphaReference";
									pGeneratedProgramUserData->pAlphaReference					= pProgram->GetUniform(sAlphaReference);
								}
								if (pGeneratedProgramUserData) {
									// Diffuse color - but only if this is not a z write only render pass
									if (!(GetFlags() & ZWriteOnly)) { 
										if (pGeneratedProgramUserData->pDiffuseColor) {
											pParameter = pMaterial->GetParameter(sDiffuseColor);
											if (pParameter) {
												float fDiffuseColor[3] = { 1.0f, 1.0f, 1.0f };
												pParameter->GetValue3f(fDiffuseColor[0], fDiffuseColor[1], fDiffuseColor[2]);
												pGeneratedProgramUserData->pDiffuseColor->Set(fDiffuseColor[0], fDiffuseColor[1], fDiffuseColor[2], fOpacity);
											} else {
												pGeneratedProgramUserData->pDiffuseColor->Set(1.0f, 1.0f, 1.0f, fOpacity);
											}
										}
									}

									// Diffuse map
									if (pGeneratedProgramUserData->pDiffuseMap) {
										const int nTextureUnit = pGeneratedProgramUserData->pDiffuseMap->Set(pDiffuseMap);
										if (nTextureUnit >= 0) {
											cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
											cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
											SetupTextureFiltering(cRenderer, nTextureUnit);

											// Set the "AlphaReference" fragment shader parameter
											if (pGeneratedProgramUserData->pAlphaReference)
												pGeneratedProgramUserData->pAlphaReference->Set(fAlphaReference);
										}
									}

									// Set object space to clip space matrix uniform
									if (pGeneratedProgramUserData->pObjectSpaceToClipSpaceMatrix)
										pGeneratedProgramUserData->pObjectSpaceToClipSpaceMatrix->Set(cVisNode.GetWorldViewProjectionMatrix());

									// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
									if (pGeneratedProgramUserData->pVertexPosition)
										pGeneratedProgramUserData->pVertexPosition->Set(&cVertexBuffer, PLRenderer::VertexBuffer::Position);
									if (pGeneratedProgramUserData->pVertexTexCoord0)
										pGeneratedProgramUserData->pVertexTexCoord0->Set(&cVertexBuffer, PLRenderer::VertexBuffer::TexCoord);

									// Draw the geometry
									cRenderer.DrawIndexedPrimitives(
										cGeometry.GetPrimitiveType(),
										0,
										cVertexBuffer.GetNumOfElements()-1,
										cGeometry.GetStartIndex(),
										cGeometry.GetIndexSize()
									);
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
void SRPDiffuseOnlyShaders::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// First, perform a sanity check: If this is a z write only scene renderer pass, but z write is disabled...
	if (!(GetFlags() & ZWriteOnly) || !(GetFlags() & NoZWrite)) {
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
				#include "SRPDiffuseOnlyShaders_GLSL.h"
				if (cRenderer.GetAPI() == "OpenGL ES 2.0") {
					// Get shader source codes
					m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sDiffuseOnly_GLSL_VS, "100", sDiffuseOnly_GLSL_FS, "100");
				} else {
					// Remove precision qualifiers because they usually create some nasty driver issues!
					m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, Shader::RemovePrecisionQualifiersFromGLSL(sDiffuseOnly_GLSL_VS), "110", Shader::RemovePrecisionQualifiersFromGLSL(sDiffuseOnly_GLSL_FS), "110");	// OpenGL 2.0 ("#version 110")
				}
			} else if (sShaderLanguage == "Cg") {
				#include "SRPDiffuseOnlyShaders_Cg.h"
				m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sDiffuseOnly_Cg_VS, "arbvp1", sDiffuseOnly_Cg_FS, "arbfp1");
			}
		}

		// If there's no program generator, we don't need to continue
		if (m_pProgramGenerator) {
			// Reset all render states to default
			cRenderer.GetRendererContext().GetEffectManager().Use();

			// Is this a transparent renderer pass?
			if (GetFlags() & TransparentPass) {
				cRenderer.SetRenderState(RenderState::BlendEnable,  true);
				cRenderer.SetRenderState(RenderState::ZWriteEnable, GetFlags() & ZWriteOnly);	// "OpacityOcclusion" mode?
			} else {
				// Maybe the z-buffer was already filled by another scene renderer pass? Let the user decide...
				cRenderer.SetRenderState(RenderState::ZWriteEnable, !(GetFlags() & NoZWrite));
			}

			// Backup the color mask
			bool bRed, bGreen, bBlue, bAlpha;
			cRenderer.GetColorMask(bRed, bGreen, bBlue, bAlpha);

			// Setup the color mask and enable scissor test
			const bool bWriteColors = !(GetFlags() & ZWriteOnly);
			cRenderer.SetColorMask(bWriteColors, bWriteColors, bWriteColors, bWriteColors);
			cRenderer.SetRenderState(RenderState::ScissorTestEnable, true);

			// Draw recursive from front to back
			DrawRec(cRenderer, cCullQuery);

			// Restore the color mask
			cRenderer.SetColorMask(bRed, bGreen, bBlue, bAlpha);
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
