/*********************************************************\
 *  File: SRPDeferredLighting.cpp                        *
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
#include <PLMath/Matrix3x3.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Program.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ProgramAttribute.h>
#include <PLRenderer/Renderer/TextureBuffer2D.h>
#include <PLRenderer/Renderer/TextureBufferCube.h>
#include <PLRenderer/Renderer/TextureBufferRectangle.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLRenderer/Material/Parameter.h>
#include <PLRenderer/Material/ParameterManager.h>
#include <PLScene/Scene/SNCamera.h>
#include <PLScene/Scene/SNDirectionalLight.h>
#include <PLScene/Scene/SNProjectiveSpotLight.h>
#include <PLScene/Scene/SNProjectivePointLight.h>
#include <PLScene/Visibility/SQCull.h>
#include <PLScene/Visibility/VisPortal.h>
#include <PLScene/Visibility/VisContainer.h>
#include "PLCompositing/Shaders/ShadowMapping/SRPShadowMapping.h"
#include "PLCompositing/Shaders/Deferred/SRPDeferredGBuffer.h"
#include "PLCompositing/Shaders/Deferred/SRPDeferredLighting.h"


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
pl_implement_class(SRPDeferredLighting)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPDeferredLighting::SRPDeferredLighting() :
	ShaderLanguage(this),
	TextureFiltering(this),
	Flags(this),
	m_pProgramGenerator(nullptr)
{
	// Overwrite the default setting of the flags
	SetFlags(GetFlags()|NoShadowLOD|NoAmbientOcclusion);
}

/**
*  @brief
*    Destructor
*/
SRPDeferredLighting::~SRPDeferredLighting()
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
*    Sets correct texture filtering modes
*/
void SRPDeferredLighting::SetupTextureFiltering(Renderer &cRenderer, uint32 nStage, uint32 nTextureFiltering) const
{
	// Anisotropic filtering
	if (nTextureFiltering > 1) {
		cRenderer.SetSamplerState(nStage, Sampler::MagFilter,     TextureFiltering::Anisotropic);
		cRenderer.SetSamplerState(nStage, Sampler::MinFilter,     TextureFiltering::Anisotropic);
		cRenderer.SetSamplerState(nStage, Sampler::MipFilter,     TextureFiltering::None);	// GBuffer doesn't required mipmaps
		cRenderer.SetSamplerState(nStage, Sampler::MaxAnisotropy, nTextureFiltering);

	// Bilinear filtering
	} else if (nTextureFiltering > 0) {
		cRenderer.SetSamplerState(nStage, Sampler::MagFilter, TextureFiltering::Linear);
		cRenderer.SetSamplerState(nStage, Sampler::MinFilter, TextureFiltering::Linear);
		cRenderer.SetSamplerState(nStage, Sampler::MipFilter, TextureFiltering::None);	// GBuffer doesn't required mipmaps

	// No filtering
	} else {
		cRenderer.SetSamplerState(nStage, Sampler::MagFilter, TextureFiltering::None);
		cRenderer.SetSamplerState(nStage, Sampler::MinFilter, TextureFiltering::None);
		cRenderer.SetSamplerState(nStage, Sampler::MipFilter, TextureFiltering::None);	// GBuffer doesn't required mipmaps
	}
}

/**
*  @brief
*    Draws recursive
*/
void SRPDeferredLighting::DrawRec(Renderer &cRenderer, const SQCull &cCullQuery, SRPDeferredGBuffer &cSRPDeferredGBuffer)
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

			// Is this a light?
			} else if (pSceneNode->IsLight()) {
				if (static_cast<SNLight*>(pSceneNode)->IsRenderLight()) {
					// Render the light
					RenderLight(cRenderer, cCullQuery, cSRPDeferredGBuffer, *static_cast<SNLight*>(pSceneNode), *pVisNode);
				}

			// This must just be a quite boring scene node, ignore it
			} else {
			}
		}
	}
}

/**
*  @brief
*    Renders a light
*/
void SRPDeferredLighting::RenderLight(Renderer &cRenderer, const SQCull &cCullQuery, SRPDeferredGBuffer &cSRPDeferredGBuffer, SNLight &cLight, const VisNode &cLightVisNode)
{
	// Skip black lights, they have no visible influence!
	if (cLight.Color.Get() == Color3::Black)
		return;	// Get us out of here right now

	// Figure out the type of the given light
	const bool bPoint			= cLight.IsPointLight();
	const bool bProjectivePoint	= (GetFlags() & NoProjectivePointLights) ? false : (cLight.IsProjectivePointLight() && !(cLight.GetFlags() & SNProjectivePointLight::NoProjection));
	const bool bSpot			= cLight.IsSpotLight();
	const bool bProjectiveSpot	= (GetFlags() & NoProjectiveSpotLights) ? false : (cLight.IsProjectiveSpotLight() && !(cLight.GetFlags() & SNProjectiveSpotLight::NoProjection));
	const bool bDirectional		= cLight.IsDirectionalLight();

	// Get the light range
	const float fRange = bPoint ? static_cast<SNPointLight&>(cLight).GetRange() : 0.0f;

	// Skip none directional lights with a radius of <= 0, they have no visible influence!
	if (!bDirectional && fRange <= 0.0f)
		return;	// Get us out of here right now

	// Calculate the scissor rectangle
	Rectangle cScissorRectangle;
	if (bDirectional) {
		// Directional lights influence the whole screen, as a result, the scissor rectangle is equal to the rectangle of the viewport
		cScissorRectangle = cRenderer.GetViewport();
	} else {
		// Get the vertices of the light scene node bounding box
		Array<Vector3> vVertex(8);
		cLight.GetAABoundingBox().GetVertices(vVertex.GetData());

		// Get viewport parameters
		const uint32 nX      = static_cast<uint32>(cRenderer.GetViewport().GetX());
		const uint32 nY      = static_cast<uint32>(cRenderer.GetViewport().GetY());
		const uint32 nWidth  = static_cast<uint32>(cRenderer.GetViewport().GetWidth());
		const uint32 nHeight = static_cast<uint32>(cRenderer.GetViewport().GetHeight());

		// Calculate the rectangle on the screen enclosing the given vertices
		cScissorRectangle.ScreenRectangle(vVertex, true, cLightVisNode.GetWorldViewProjectionMatrix(), nX, nY, nWidth, nHeight);
	}

	// Is the scissor rectangle visible?
	if (cScissorRectangle.GetWidth() || cScissorRectangle.GetHeight()) {
		// Set the scissor rectangle
		cRenderer.SetScissorRect(&cScissorRectangle);

		// Get the shadow mapping scene renderer pass and update the shadow map
		bool bShadowMapping = false;
		SRPShadowMapping *pSRPShadowMapping = nullptr;
		if (!(GetFlags() & NoShadow) && (cLight.GetFlags() & SNLight::CastShadow)) {
			// Get the shadow mapping scene renderer pass
			static const String sClassName = "PLCompositing::SRPShadowMapping";
			pSRPShadowMapping = static_cast<SRPShadowMapping*>(GetFirstInstanceOfSceneRendererPassClass(sClassName));

			// Is the shadow mapping scene renderer pass active?
			if (pSRPShadowMapping && !pSRPShadowMapping->IsActive())
				pSRPShadowMapping = nullptr;	// Just do like there's no shadow mapping scene renderer pass at all

			// Is there a shadow mapping scene renderer pass?
			if (pSRPShadowMapping) {
				// Update the shadow map for this light
				pSRPShadowMapping->UpdateShadowMap(cRenderer, cLight, cCullQuery, (GetFlags() & NoShadowLOD) ? 0.0f : cLightVisNode.GetSquaredDistanceToCamera());

				// Perform shadow mapping?
				if (pSRPShadowMapping->GetCubeShadowRenderTarget() || pSRPShadowMapping->GetSpotShadowRenderTarget())
					bShadowMapping = true;

				// [TODO] Remove this, to much state changes!
				// Sets the initial render states
				cRenderer.GetRendererContext().GetEffectManager().Use();
				cRenderer.SetRenderState(RenderState::CullMode,     Cull::None);
				cRenderer.SetRenderState(RenderState::ZEnable,      false);
				cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
				cRenderer.SetRenderState(RenderState::BlendEnable,	true);
				cRenderer.SetRenderState(RenderState::SrcBlendFunc,	BlendFunc::One);
				cRenderer.SetRenderState(RenderState::DstBlendFunc,	BlendFunc::One);

				// Set the color mask, we don't write out alpha
				cRenderer.SetColorMask(true, true, true, false);
			}
		}

		// Use spot cone or even smooth spot cone for spot lights?
		bool bSpotCone       = false;
		bool bSpotSmoothCone = false;
		if (bSpot && !(cLight.GetFlags() & SNSpotLight::NoCone)) {
			// A spot cone is used
			bSpotCone = true;

			// The inner angle MUST be smaller than the outer one, else we do not use a smooth cone
			bSpotSmoothCone = static_cast<SNSpotLight&>(cLight).GetOuterAngle() > static_cast<SNSpotLight&>(cLight).GetInnerAngle();
		}

		// Reset the program flags
		m_cProgramFlags.Reset();

		// Set program flags
		if (bDirectional) {
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_DIRECTIONAL)
		} else {
			if (bProjectivePoint) {
				PL_ADD_FS_FLAG(m_cProgramFlags, FS_PROJECTIVE_POINT)
			} else {
				if (bSpot) {
					PL_ADD_FS_FLAG(m_cProgramFlags, FS_SPOT)
					if (bProjectiveSpot)
						PL_ADD_FS_FLAG(m_cProgramFlags, FS_PROJECTIVE_SPOT)
					if (bSpotCone) {
						PL_ADD_FS_FLAG(m_cProgramFlags, FS_SPOT_CONE)
						if (bSpotSmoothCone)
							PL_ADD_FS_FLAG(m_cProgramFlags, FS_SPOT_SMOOTHCONE)
					}
				}
			}
			if (bShadowMapping) {
				PL_ADD_FS_FLAG(m_cProgramFlags, FS_SHADOWMAPPING)
				if (!(GetFlags() & NoSoftShadow))
					PL_ADD_FS_FLAG(m_cProgramFlags, FS_SOFTSHADOWMAPPING)
			}
		}
		if (GetFlags() & NoAlbedo)
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_NO_ALBEDO)
		if (GetFlags() & NoAmbientOcclusion)
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_NO_AMBIENTOCCLUSION)
		if (GetFlags() & NoSpecular)
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_NO_SPECULAR)
		if (GetFlags() & NoSpecularColor)
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_NO_SPECULARCOLOR)
		if (GetFlags() & NoSpecularExponent)
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_NO_SPECULAREXPONENT)
		if (!(GetFlags() & NoDiscard))
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_DISCARD)
		if (!(GetFlags() & NoGammaCorrection))
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_GAMMACORRECTION)

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
				static const String sVertexTexCoord0 = "VertexTexCoord0";
				pGeneratedProgramUserData->pVertexTexCoord0	= pProgram->GetAttribute(sVertexTexCoord0);
				// Vertex shader uniforms
				static const String sTextureSize = "TextureSize";
				pGeneratedProgramUserData->pTextureSize						= pProgram->GetUniform(sTextureSize);
				// Fragment shader uniforms
				static const String sLightDirection = "LightDirection";
				pGeneratedProgramUserData->pLightDirection					= pProgram->GetUniform(sLightDirection);
				static const String sLightPosition = "LightPosition";
				pGeneratedProgramUserData->pLightPosition					= pProgram->GetUniform(sLightPosition);
				static const String sLightRadius = "LightRadius";
				pGeneratedProgramUserData->pLightRadius						= pProgram->GetUniform(sLightRadius);
				static const String sProjectivePointCubeMap = "ProjectivePointCubeMap";
				pGeneratedProgramUserData->pProjectivePointCubeMap			= pProgram->GetUniform(sProjectivePointCubeMap);
				static const String sViewSpaceToCubeMapSpace = "ViewSpaceToCubeMapSpace";
				pGeneratedProgramUserData->pViewSpaceToCubeMapSpace			= pProgram->GetUniform(sViewSpaceToCubeMapSpace);
				static const String sProjectiveSpotMap = "ProjectiveSpotMap";
				pGeneratedProgramUserData->pProjectiveSpotMap				= pProgram->GetUniform(sProjectiveSpotMap);
				static const String sViewSpaceToSpotMapSpace = "ViewSpaceToSpotMapSpace";
				pGeneratedProgramUserData->pViewSpaceToSpotMapSpace			= pProgram->GetUniform(sViewSpaceToSpotMapSpace);
				static const String sSpotConeCos = "SpotConeCos";
				pGeneratedProgramUserData->pSpotConeCos						= pProgram->GetUniform(sSpotConeCos);
				static const String sShadowMap = "ShadowMap";
				pGeneratedProgramUserData->pShadowMap						= pProgram->GetUniform(sShadowMap);
				static const String sViewSpaceToShadowMapSpace = "ViewSpaceToShadowMapSpace";
				pGeneratedProgramUserData->pViewSpaceToShadowMapSpace		= pProgram->GetUniform(sViewSpaceToShadowMapSpace);
				static const String sViewSpaceToShadowCubeMapSpace = "ViewSpaceToShadowCubeMapSpace";
				pGeneratedProgramUserData->pViewSpaceToShadowCubeMapSpace	= pProgram->GetUniform(sViewSpaceToShadowCubeMapSpace);
				static const String sInvLightRadius = "InvLightRadius";
				pGeneratedProgramUserData->pInvLightRadius					= pProgram->GetUniform(sInvLightRadius);
				static const String sTexelSize = "TexelSize";
				pGeneratedProgramUserData->pTexelSize						= pProgram->GetUniform(sTexelSize);
				static const String sLightColor = "LightColor";
				pGeneratedProgramUserData->pLightColor						= pProgram->GetUniform(sLightColor);
				static const String sInvFocalLen = "InvFocalLen";
				pGeneratedProgramUserData->pInvFocalLen						= pProgram->GetUniform(sInvFocalLen);
				static const String sRenderTargetTexture0 = "RenderTargetTexture0";
				pGeneratedProgramUserData->pRenderTargetTexture0			= pProgram->GetUniform(sRenderTargetTexture0);
				static const String sRenderTargetTexture1 = "RenderTargetTexture1";
				pGeneratedProgramUserData->pRenderTargetTexture1			= pProgram->GetUniform(sRenderTargetTexture1);
				static const String sRenderTargetTexture2 = "RenderTargetTexture2";
				pGeneratedProgramUserData->pRenderTargetTexture2			= pProgram->GetUniform(sRenderTargetTexture2);
			}

			// Get the vertex buffer
			VertexBuffer *pVertexBuffer = GetVertexBuffer();
			if (pVertexBuffer) {
				// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
				if (pGeneratedProgramUserData->pVertexPosition)
					pGeneratedProgramUserData->pVertexPosition->Set(pVertexBuffer, PLRenderer::VertexBuffer::Position);
				if (pGeneratedProgramUserData->pVertexTexCoord0)
					pGeneratedProgramUserData->pVertexTexCoord0->Set(pVertexBuffer, PLRenderer::VertexBuffer::TexCoord);
			}

			// Get the size of the texture buffer
			const Vector2i vSize = cSRPDeferredGBuffer.GetRenderTargetTextureBuffer(0)->GetSize();

			// Set texture size
			if (pGeneratedProgramUserData->pTextureSize)
				pGeneratedProgramUserData->pTextureSize->Set(vSize);

			// Directional light?
			if (bDirectional) {
				if (pGeneratedProgramUserData->pLightDirection) {
					// Set normalized view space light direction vector
					Vector3 vLightDirection = cLightVisNode.GetWorldViewMatrix().GetZAxis();
					vLightDirection.Invert();
					vLightDirection.Normalize();
					pGeneratedProgramUserData->pLightDirection->Set(vLightDirection);
				}
			} else {
				// Set the "LightPosition" fragment shader parameter
				if (pGeneratedProgramUserData->pLightPosition)
					pGeneratedProgramUserData->pLightPosition->Set(cLightVisNode.GetWorldViewMatrix().GetTranslation());

				// Set the "LightRadius" fragment shader parameter
				if (pGeneratedProgramUserData->pLightRadius)
					pGeneratedProgramUserData->pLightRadius->Set(fRange);

				// Set cube map
				if (bProjectivePoint) {
					// The texture buffer we're going to project
					TextureBuffer *pCubeMap = nullptr;

					// Get the projective material to use
					const Material *pProjectiveMaterial = static_cast<SNProjectivePointLight&>(cLight).GetProjectedMaterialHandler().GetResource();
					if (pProjectiveMaterial) {
						// Get the DiffuseMap-parameter of the projective material
						const Parameter *pParameter = pProjectiveMaterial->GetParameter(Material::DiffuseMap);
						if (pParameter) {
							// Get the texture we're going to project
							const Texture *pTexture = pParameter->GetValueTexture();
							if (pTexture)
								pCubeMap = pTexture->GetTextureBuffer();
						}
					}

					// Use the default cube map?
					if (!pCubeMap) {
						if (!m_cCubeMapHandler.GetResource())
							m_cCubeMapHandler.Load(cRenderer.GetRendererContext().GetTextureManager(), "Data/Textures/DefaultCubeMap.dds");
						if (m_cCubeMapHandler.GetResource())
							pCubeMap = m_cCubeMapHandler.GetResource()->GetTextureBuffer();
					}

					// Set the "ProjectivePointCubeMap" fragment shader parameter
					if (pGeneratedProgramUserData->pProjectivePointCubeMap) {
						const int nTextureUnit = pGeneratedProgramUserData->pProjectivePointCubeMap->Set(pCubeMap);
						if (nTextureUnit >= 0) {
							cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
							cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
							cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressW, TextureAddressing::Wrap);
							SetupTextureFiltering(cRenderer, nTextureUnit, TextureFiltering);
						}
					}

					// Set the "ViewSpaceToCubeMapSpace" fragment shader parameter
					if (pGeneratedProgramUserData->pViewSpaceToCubeMapSpace) {
						Matrix3x3 mRot = cLightVisNode.GetWorldViewMatrix();
						mRot.Invert();
						pGeneratedProgramUserData->pViewSpaceToCubeMapSpace->Set(mRot);
					}
				} else {
					// Set spot light parameters
					if (bSpot) {
						// Set light direction in view space
						if (pGeneratedProgramUserData->pLightDirection) {
							Vector3 vLightDirection = cLightVisNode.GetWorldViewMatrix().GetZAxis();
							vLightDirection.Normalize();
							pGeneratedProgramUserData->pLightDirection->Set(vLightDirection);
						}

						// Projective spot light
						if (bProjectiveSpot) {
							// The texture buffer we're going to project
							TextureBuffer *pSpotMap = nullptr;

							// Get the projective material to use
							const Material *pProjectiveMaterial = static_cast<SNProjectiveSpotLight&>(cLight).GetProjectedMaterialHandler().GetResource();
							if (pProjectiveMaterial) {
								// Get the DiffuseMap-parameter of the projective material
								const Parameter *pParameter = pProjectiveMaterial->GetParameter(Material::DiffuseMap);
								if (pParameter) {
									// Get the texture we're going to project
									const Texture *pTexture = pParameter->GetValueTexture();
									if (pTexture)
										pSpotMap = pTexture->GetTextureBuffer();
								}
							}

							// Use the default spot map?
							if (!pSpotMap) {
								if (!m_cSpotMapHandler.GetResource())
									m_cSpotMapHandler.Load(cRenderer.GetRendererContext().GetTextureManager(), "Data/Textures/PLLogo.dds");
								if (m_cSpotMapHandler.GetResource())
									pSpotMap = m_cSpotMapHandler.GetResource()->GetTextureBuffer();
							}

							// Set the "ProjectiveSpotMap" fragment shader parameter
							if (pGeneratedProgramUserData->pProjectiveSpotMap) {
								const int nTextureUnit = pGeneratedProgramUserData->pProjectiveSpotMap->Set(pSpotMap);
								if (nTextureUnit >= 0) {
									// Do NOT use wrapping for projective textures! :)
									cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Border);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Border);
									SetupTextureFiltering(cRenderer, nTextureUnit, TextureFiltering);
								}
							}

							// Set the "ViewSpaceToSpotMapSpace" fragment shader parameter
							if (pGeneratedProgramUserData->pViewSpaceToSpotMapSpace) {
								static const String sViewSpaceToSpotMapSpace = "ViewSpaceToSpotMapSpace";

								// This matrix brings us from clip space [-1...1] to texture space [0...1] (scale & bias)
								static const Matrix4x4 mClipSpaceToTextureSpace(0.5f, 0.0f, 0.0f, 0.5f,
																				0.0f, 0.5f, 0.0f, 0.5f,
																				0.0f, 0.0f, 0.5f, 0.5f,
																				0.0f, 0.0f, 0.0f, 1.0f);

								// Clip space [-1...1] to texture space [0...1]
								Matrix4x4 mTransform = mClipSpaceToTextureSpace;

								// View space to clip space [-1...1]
								mTransform *= static_cast<SNProjectiveSpotLight&>(cLight).GetProjectionMatrix();

								// Set the fragment shader parameter
								pGeneratedProgramUserData->pViewSpaceToSpotMapSpace->Set(mTransform*cLightVisNode.GetWorldViewMatrix().GetInverted());
							}
						}

						// Set spot light cone parameters
						if (pGeneratedProgramUserData->pSpotConeCos) {
							if (bSpotSmoothCone) {
								// Set spot light cone outer and inner angle in view space
								pGeneratedProgramUserData->pSpotConeCos->Set(static_cast<float>(Math::Cos(static_cast<SNSpotLight&>(cLight).GetOuterAngle()*Math::DegToRad*0.5f)),
																			 static_cast<float>(Math::Cos(static_cast<SNSpotLight&>(cLight).GetInnerAngle()*Math::DegToRad*0.5f)));

							} else {
								// Set spot light cone outer angle in view space
								pGeneratedProgramUserData->pSpotConeCos->Set(static_cast<float>(Math::Cos(static_cast<SNSpotLight&>(cLight).GetOuterAngle()*Math::DegToRad*0.5f)));
							}
						}
					}
				}

				// Shadow mapping
				if (pGeneratedProgramUserData->pShadowMap) {
					// Get the shadow map
					TextureBuffer *pShadowMap = nullptr;
					if (bSpot) {
						if (pSRPShadowMapping->GetSpotShadowRenderTarget())
							pShadowMap = pSRPShadowMapping->GetSpotShadowRenderTarget()->GetTextureBuffer();
					} else {
						if (pSRPShadowMapping->GetCubeShadowRenderTarget())
							pShadowMap = pSRPShadowMapping->GetCubeShadowRenderTarget()->GetTextureBuffer();
					}

					// Set shadow map
					if (pShadowMap) {
						const int nTextureUnit = pGeneratedProgramUserData->pShadowMap->Set(pShadowMap);
						if (nTextureUnit >= 0) {
							// Clamp, if this is not done, we may see some artifacts
							cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU,  TextureAddressing::Clamp);
							cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV,  TextureAddressing::Clamp);
							cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressW,  TextureAddressing::Clamp);

							// No mipmapping, please
							cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::Linear);
							cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::Linear);
							cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);

							// Set spot parameters
							if (bSpot) {
								// Set the "ViewSpaceToShadowMapSpace" fragment shader parameter
								if (pGeneratedProgramUserData->pViewSpaceToShadowMapSpace) {
									// This matrix brings us from clip space [-1...1] to texture space [0...1] (scale & bias)
									static const Matrix4x4 mClipSpaceToTextureSpace(0.5f, 0.0f, 0.0f, 0.5f,
																					0.0f, 0.5f, 0.0f, 0.5f,
																					0.0f, 0.0f, 0.5f, 0.5f,
																					0.0f, 0.0f, 0.0f, 1.0f);

									// Clip space [-1...1] to texture space [0...1]
									Matrix4x4 mTransform = mClipSpaceToTextureSpace;

									// Scene node space to view space and then view space to clip space [-1...1] combined within one matrix
									mTransform *= pSRPShadowMapping->GetLightViewProjectionMatrix();

									// Set the fragment shader parameter
									pGeneratedProgramUserData->pViewSpaceToShadowMapSpace->Set(mTransform*cLight.GetTransform().GetMatrix()*cLightVisNode.GetWorldViewMatrix().GetInverted());
								}
							} else {
								// Set the "ViewSpaceToShadowCubeMapSpace" fragment shader parameter
								if (pGeneratedProgramUserData->pViewSpaceToShadowCubeMapSpace) {
									const Matrix3x3 mTransform = cLight.GetTransform().GetMatrix()*cLightVisNode.GetWorldViewMatrix().GetInverted();
									pGeneratedProgramUserData->pViewSpaceToShadowCubeMapSpace->Set(mTransform);
								}

								// Set the "InvLightRadius" fragment shader parameter
								if (pGeneratedProgramUserData->pInvLightRadius)
									pGeneratedProgramUserData->pInvLightRadius->Set(1.0f/fRange);
							}

							// Set texel size
							if (!(GetFlags() & NoSoftShadow) && pGeneratedProgramUserData->pTexelSize) {
								float fSize = 0.0f;
								if (pShadowMap->GetType() == TextureBuffer::TypeTextureBuffer2D)
									fSize = static_cast<float>(static_cast<TextureBuffer2D*>(pShadowMap)->GetSize().x);
								else if (pShadowMap->GetType() == TextureBuffer::TypeTextureBufferCube)
									fSize = static_cast<float>(static_cast<TextureBufferCube*>(pShadowMap)->GetSize());
								pGeneratedProgramUserData->pTexelSize->Set(fSize ? 0.5f/fSize : 1.0f);
							}
						}
					}
				}
			}

			// Set the "LightColor" fragment shader parameter
			if (pGeneratedProgramUserData->pLightColor)
				pGeneratedProgramUserData->pLightColor->Set(cLight.Color.Get());

			// Set the "InvFocalLen" fragment shader parameter
			if (pGeneratedProgramUserData->pInvFocalLen) {
				// Calculate the focal length
				const float fFovY         = static_cast<float>((SNCamera::GetCamera() ? SNCamera::GetCamera()->GetFOV() : 45.0f) * Math::DegToRad);
				const float fFocalLengthY = 1.0f / Math::Tan(fFovY * 0.5f);
				const float fFocalLengthX = fFocalLengthY * ((vSize.y * (SNCamera::GetCamera() ? SNCamera::GetCamera()->GetAspect() : 1.0f)) / vSize.x);

				// Set the fragment shader parameter
				pGeneratedProgramUserData->pInvFocalLen->Set(1.0f/fFocalLengthX, 1.0f/fFocalLengthY);
			}

			// Set render target texture 0
			if (pGeneratedProgramUserData->pRenderTargetTexture0) {
				// Get the texture buffer to use
				TextureBufferRectangle *pTextureBuffer = cSRPDeferredGBuffer.GetRenderTargetTextureBuffer(0);
				if (pTextureBuffer) {
					const int nTextureUnit = pGeneratedProgramUserData->pRenderTargetTexture0->Set(pTextureBuffer);
					if (nTextureUnit >= 0) {
						cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Clamp);
						cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Clamp);
						cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
						cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
						cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
					}
				}
			}

			// Set render target texture 1
			if (pGeneratedProgramUserData->pRenderTargetTexture1) {
				// Get the texture buffer to use
				TextureBufferRectangle *pTextureBuffer = cSRPDeferredGBuffer.GetRenderTargetTextureBuffer(1);
				if (pTextureBuffer) {
					const int nTextureUnit = pGeneratedProgramUserData->pRenderTargetTexture1->Set(pTextureBuffer);
					if (nTextureUnit >= 0) {
						cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Clamp);
						cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Clamp);
						cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
						cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
						cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
					}
				}
			}

			// Set render target texture 2
			if (pGeneratedProgramUserData->pRenderTargetTexture2) {
				// Get the texture buffer to use
				TextureBufferRectangle *pTextureBuffer = cSRPDeferredGBuffer.GetRenderTargetTextureBuffer(2);
				if (pTextureBuffer) {
					const int nTextureUnit = pGeneratedProgramUserData->pRenderTargetTexture2->Set(pTextureBuffer);
					if (nTextureUnit >= 0) {
						cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Clamp);
						cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Clamp);
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
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRPDeferredLighting::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Get the instance of the "PLCompositing::SRPDeferredGBuffer" scene renderer pass
	SRPDeferredGBuffer *pSRPDeferredGBuffer = GetGBuffer();
	if (pSRPDeferredGBuffer && pSRPDeferredGBuffer->GetRenderTargetTextureBuffer(0)) {
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
				#include "SRPDeferredLighting_GLSL.h"
				m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sDeferredLighting_GLSL_VS, "110", sDeferredLighting_GLSL_FS, "110");	// OpenGL 2.0 ("#version 110")
			} else if (sShaderLanguage == "Cg") {
				#include "SRPDeferredLighting_Cg.h"
				m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sDeferredLighting_Cg_VS, "arbvp1", sDeferredLighting_Cg_FS, "arbfp1");
			}
		}

		// If there's no program generator, we don't need to continue
		if (m_pProgramGenerator) {
			// Reset all render states to default
			cRenderer.GetRendererContext().GetEffectManager().Use();

			// Setup renderer
			const uint32 nFixedFillModeBackup = cRenderer.GetRenderState(RenderState::FixedFillMode);
			cRenderer.SetRenderState(RenderState::ScissorTestEnable, true);
			cRenderer.SetRenderState(RenderState::FixedFillMode,	 Fill::Solid);
			cRenderer.SetRenderState(RenderState::CullMode,			 Cull::None);
			cRenderer.SetRenderState(RenderState::ZEnable,			 false);
			cRenderer.SetRenderState(RenderState::ZWriteEnable,		 false);

			// Set blend mode
			cRenderer.SetRenderState(RenderState::BlendEnable,	true);
			cRenderer.SetRenderState(RenderState::SrcBlendFunc,	BlendFunc::One);
			cRenderer.SetRenderState(RenderState::DstBlendFunc,	BlendFunc::One);

			// Set the color mask, we don't write out alpha
			cRenderer.SetColorMask(true, true, true, false);

			// Draw recursive from front to back
			DrawRec(cRenderer, cCullQuery, *pSRPDeferredGBuffer);

			// Restore fixed fill mode render state
			cRenderer.SetRenderState(RenderState::FixedFillMode, nFixedFillModeBackup);

			// Restore color mask
			cRenderer.SetColorMask(true, true, true, true);
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
