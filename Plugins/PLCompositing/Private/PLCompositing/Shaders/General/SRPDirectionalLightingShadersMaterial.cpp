/*********************************************************\
 *  File: SRPDirectionalLightingShadersMaterial.cpp      *
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
#include <PLMath/Matrix3x3.h>
#include <PLRenderer/Material/Material.h>
#include <PLRenderer/Material/Parameter.h>
#include <PLRenderer/Texture/TextureManager.h>
#include <PLRenderer/Renderer/Program.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/RenderStates.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ProgramGenerator.h>
#include <PLRenderer/Renderer/TextureBufferRectangle.h>
#include "PLCompositing/Shaders/General/SRPDirectionalLightingShaders.h"
#include "PLCompositing/Shaders/General/SRPDirectionalLightingShadersMaterial.h"


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
//[ Private static data                                   ]
//[-------------------------------------------------------]
// Material parameter
const String SRPDirectionalLightingShadersMaterial::Glow							= "Glow";
const String SRPDirectionalLightingShadersMaterial::GlowMap							= "GlowMap";
const String SRPDirectionalLightingShadersMaterial::Opacity							= "Opacity";
const String SRPDirectionalLightingShadersMaterial::SrcBlendFunc					= "SrcBlendFunc";
const String SRPDirectionalLightingShadersMaterial::DstBlendFunc					= "DstBlendFunc";
const String SRPDirectionalLightingShadersMaterial::TwoSided						= "TwoSided";
const String SRPDirectionalLightingShadersMaterial::AlphaReference					= "AlphaReference";
const String SRPDirectionalLightingShadersMaterial::DiffuseRampMap					= "DiffuseRampMap";
const String SRPDirectionalLightingShadersMaterial::IndexOfRefraction				= "IndexOfRefraction";
const String SRPDirectionalLightingShadersMaterial::FresnelReflectionPower			= "FresnelReflectionPower";
const String SRPDirectionalLightingShadersMaterial::ReflectionColor					= "ReflectionColor";
const String SRPDirectionalLightingShadersMaterial::Reflectivity					= "Reflectivity";
const String SRPDirectionalLightingShadersMaterial::NormalMapBumpiness				= "NormalMapBumpiness";
const String SRPDirectionalLightingShadersMaterial::DetailNormalMap					= "DetailNormalMap";
const String SRPDirectionalLightingShadersMaterial::DetailNormalMapBumpiness		= "DetailNormalMapBumpiness";
const String SRPDirectionalLightingShadersMaterial::DetailNormalMapUVScale			= "DetailNormalMapUVScale";
const String SRPDirectionalLightingShadersMaterial::Parallax						= "Parallax";
const String SRPDirectionalLightingShadersMaterial::SpecularColor					= "SpecularColor";
const String SRPDirectionalLightingShadersMaterial::SpecularExponent				= "SpecularExponent";
const String SRPDirectionalLightingShadersMaterial::SpecularRampMap					= "SpecularRampMap";
const String SRPDirectionalLightingShadersMaterial::EdgeRampMap						= "EdgeRampMap";
// Shader parameter
const String SRPDirectionalLightingShadersMaterial::DiffuseColor					= "DiffuseColor";
const String SRPDirectionalLightingShadersMaterial::LightMapColor					= "LightMapColor";
const String SRPDirectionalLightingShadersMaterial::AmbientOcclusionFactor			= "AmbientOcclusionFactor";
const String SRPDirectionalLightingShadersMaterial::EmissiveMapColor				= "EmissiveMapColor";
const String SRPDirectionalLightingShadersMaterial::VertexPosition					= "VertexPosition";
const String SRPDirectionalLightingShadersMaterial::VertexTexCoord0					= "VertexTexCoord0";
const String SRPDirectionalLightingShadersMaterial::VertexTexCoord1					= "VertexTexCoord1";
const String SRPDirectionalLightingShadersMaterial::VertexNormal					= "VertexNormal";
const String SRPDirectionalLightingShadersMaterial::VertexTangent					= "VertexTangent";
const String SRPDirectionalLightingShadersMaterial::VertexBinormal					= "VertexBinormal";
const String SRPDirectionalLightingShadersMaterial::NormalScale						= "NormalScale";
const String SRPDirectionalLightingShadersMaterial::ObjectSpaceToViewSpaceMatrix	= "ObjectSpaceToViewSpaceMatrix";
const String SRPDirectionalLightingShadersMaterial::ObjectSpaceToClipSpaceMatrix	= "ObjectSpaceToClipSpaceMatrix";
const String SRPDirectionalLightingShadersMaterial::EyePos							= "EyePos";
const String SRPDirectionalLightingShadersMaterial::AmbientColor					= "AmbientColor";
// const String SRPDirectionalLightingShadersMaterial::AlphaReference				= "AlphaReference";	// Already defined
const String SRPDirectionalLightingShadersMaterial::FresnelConstants				= "FresnelConstants";
const String SRPDirectionalLightingShadersMaterial::ViewSpaceToWorldSpace			= "ViewSpaceToWorldSpace";
const String SRPDirectionalLightingShadersMaterial::ParallaxScaleBias				= "ParallaxScaleBias";
const String SRPDirectionalLightingShadersMaterial::LightDirection					= "LightDirection";
const String SRPDirectionalLightingShadersMaterial::LightColor						= "LightColor";
// const String SRPDirectionalLightingShadersMaterial::Glow							= "Glow";	// Already defined
// const String SRPDirectionalLightingShadersMaterial::GlowMap						= "GlowMap";	// Already defined
const String SRPDirectionalLightingShadersMaterial::DOFParams						= "DOFParams";


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPDirectionalLightingShadersMaterial::SRPDirectionalLightingShadersMaterial(RenderStates &cRenderStates, Material &cMaterial, ProgramGenerator &cProgramGenerator) :
	EventHandlerParameterChanged(&SRPDirectionalLightingShadersMaterial::OnParameterChanged, this),
	// General
	m_pRenderStates(&cRenderStates),
	m_pMaterial(&cMaterial),
	m_pProgramGenerator(&cProgramGenerator),
	m_nRendererFlags(0),
	m_nEnvironmentFlags(0),
	m_bSynchronized(false),
	// Synchronized data
		// Glow
	m_fGlow(0.0f),
	m_pGlowMap(nullptr),
		// Opacity
	m_fOpacity(1.0f),
	m_nSrcBlendFunc(BlendFunc::SrcAlpha),
	m_nDstBlendFunc(BlendFunc::InvSrcAlpha),
		// Two sided
	m_bTwoSided(false),
		// Diffuse map and alpha reference
	m_cDiffuseColor(Color3::White),
	m_fAlphaReference(0.0f),
	m_pDiffuseMap(nullptr),
		// Diffuse ramp map
	m_pDiffuseRampMap(nullptr),
		// Ambient map and light map require texture coordinate set 1
	m_fAmbientOcclusionFactor(1.0f),
	m_pAmbientOcclusionMap(nullptr),
	m_cLightMapColor(Color3::White),
	m_pLightMap(nullptr),
		// Emissive map
	m_cEmissiveMapColor(Color3::White),
	m_pEmissiveMap(nullptr),
		// Index of refraction and fresnel reflection power
	m_fIndexOfRefraction(0.0f),
	m_fFresnelReflectionPower(5.0f),
		// (2D/cube) reflection map
	m_pReflectionMap(nullptr),
	m_b2DReflectionMap(true),
		// Figure out whether or not there's reflection on this material
	m_bReflection(false),
		// Reflection parameters
	m_pReflectivityMap(nullptr),
	m_fReflectivity(1.0f),
	m_cReflectionColor(Color3::White),
		// Normal map
	m_pNormalMap(nullptr),
	m_fNormalMapBumpiness(1.0f),
		// Detail normal map
	m_pDetailNormalMap(nullptr),
	m_fDetailNormalMapBumpiness(1.0f),
	m_vDetailNormalMapUVScale(4.0f, 4.0f),
		// Parallax mapping settings
	m_fParallax(0.04f),
	m_pHeightMap(nullptr),
		// Lighting and specular
	m_cSpecularColor(Color3::White),
	m_pSpecularMap(nullptr),
	m_fSpecularExponent(45.0f),
		// Specular ramp map
	m_pSpecularRampMap(nullptr),
		// Edge ramp map
	m_pEdgeRampMap(nullptr)
{
	// Connect event handler
	m_pMaterial->EventParameterChanged.Connect(EventHandlerParameterChanged);
}

/**
*  @brief
*    Destructor
*/
SRPDirectionalLightingShadersMaterial::~SRPDirectionalLightingShadersMaterial()
{
}

/**
*  @brief
*    Makes this material to the currently used one
*/
SRPDirectionalLightingShadersMaterial::GeneratedProgramUserData *SRPDirectionalLightingShadersMaterial::MakeMaterialCurrent(uint32 nRendererFlags, uint32 nEnvironmentFlags, ETextureFiltering nTextureFiltering)
{
	// Get the used renderer
	Renderer &cRenderer = m_pProgramGenerator->GetRenderer();

	// Synchronize this material cache with the owner
	if (m_nRendererFlags != nRendererFlags || m_nEnvironmentFlags != nEnvironmentFlags || !m_bSynchronized)
		Synchronize(nRendererFlags, nEnvironmentFlags);

	// Get a program instance from the program generator using the given program flags
	ProgramGenerator::GeneratedProgram *pGeneratedProgram = m_pProgramGenerator->GetProgram(m_cProgramFlags);

	// Make our program to the current one
	GeneratedProgramUserData *pGeneratedProgramUserData = nullptr;
	if (pGeneratedProgram && cRenderer.SetProgram(pGeneratedProgram->pProgram)) {
		// Set pointers to uniforms & attributes of a generated program if they are not set yet
		pGeneratedProgramUserData = static_cast<GeneratedProgramUserData*>(pGeneratedProgram->pUserData);
		if (!pGeneratedProgramUserData) {
			pGeneratedProgram->pUserData = pGeneratedProgramUserData = new GeneratedProgramUserData;
			Program *pProgram = pGeneratedProgram->pProgram;
			// Vertex shader attributes
			pGeneratedProgramUserData->pVertexPosition					= pProgram->GetAttribute(VertexPosition);
			pGeneratedProgramUserData->pVertexTexCoord0					= pProgram->GetAttribute(VertexTexCoord0);
			pGeneratedProgramUserData->pVertexTexCoord1					= pProgram->GetAttribute(VertexTexCoord1);
			pGeneratedProgramUserData->pVertexNormal					= pProgram->GetAttribute(VertexNormal);
			pGeneratedProgramUserData->pVertexTangent					= pProgram->GetAttribute(VertexTangent);
			pGeneratedProgramUserData->pVertexBinormal					= pProgram->GetAttribute(VertexBinormal);
			// Vertex shader uniforms
			pGeneratedProgramUserData->pNormalScale						= pProgram->GetUniform(NormalScale);
			pGeneratedProgramUserData->pObjectSpaceToViewSpaceMatrix	= pProgram->GetUniform(ObjectSpaceToViewSpaceMatrix);
			pGeneratedProgramUserData->pObjectSpaceToClipSpaceMatrix	= pProgram->GetUniform(ObjectSpaceToClipSpaceMatrix);
			pGeneratedProgramUserData->pEyePos							= pProgram->GetUniform(EyePos);
			// Fragment shader uniforms
			pGeneratedProgramUserData->pAmbientColor					= pProgram->GetUniform(AmbientColor);
			pGeneratedProgramUserData->pDiffuseColor					= pProgram->GetUniform(DiffuseColor);
			pGeneratedProgramUserData->pDiffuseMap						= pProgram->GetUniform(Material::DiffuseMap);
			pGeneratedProgramUserData->pAlphaReference					= pProgram->GetUniform(AlphaReference);
			pGeneratedProgramUserData->pDiffuseRampMap					= pProgram->GetUniform(DiffuseRampMap);
			pGeneratedProgramUserData->pAmbientOcclusionMap				= pProgram->GetUniform(Material::AmbientOcclusionMap);
			pGeneratedProgramUserData->pAmbientOcclusionFactor			= pProgram->GetUniform(AmbientOcclusionFactor);
			pGeneratedProgramUserData->pLightMap						= pProgram->GetUniform(Material::LightMap);
			pGeneratedProgramUserData->pLightMapColor					= pProgram->GetUniform(LightMapColor);
			pGeneratedProgramUserData->pEmissiveMap						= pProgram->GetUniform(Material::EmissiveMap);
			pGeneratedProgramUserData->pEmissiveMapColor				= pProgram->GetUniform(EmissiveMapColor);
			pGeneratedProgramUserData->pReflectionColor					= pProgram->GetUniform(ReflectionColor);
			pGeneratedProgramUserData->pReflectivity					= pProgram->GetUniform(Reflectivity);
			pGeneratedProgramUserData->pReflectivityMap					= pProgram->GetUniform(Material::ReflectivityMap);
			pGeneratedProgramUserData->pFresnelConstants				= pProgram->GetUniform(FresnelConstants);
			pGeneratedProgramUserData->pReflectionMap					= pProgram->GetUniform(Material::ReflectionMap);
			pGeneratedProgramUserData->pViewSpaceToWorldSpace			= pProgram->GetUniform(ViewSpaceToWorldSpace);
			pGeneratedProgramUserData->pNormalMap						= pProgram->GetUniform(Material::NormalMap);
			pGeneratedProgramUserData->pNormalMapBumpiness				= pProgram->GetUniform(NormalMapBumpiness);
			pGeneratedProgramUserData->pDetailNormalMap					= pProgram->GetUniform(DetailNormalMap);
			pGeneratedProgramUserData->pDetailNormalMapBumpiness		= pProgram->GetUniform(DetailNormalMapBumpiness);
			pGeneratedProgramUserData->pDetailNormalMapUVScale			= pProgram->GetUniform(DetailNormalMapUVScale);
			pGeneratedProgramUserData->pHeightMap						= pProgram->GetUniform(Material::HeightMap);
			pGeneratedProgramUserData->pParallaxScaleBias				= pProgram->GetUniform(ParallaxScaleBias);
			pGeneratedProgramUserData->pLightDirection					= pProgram->GetUniform(LightDirection);
			pGeneratedProgramUserData->pLightColor						= pProgram->GetUniform(LightColor);
			pGeneratedProgramUserData->pSpecularColor					= pProgram->GetUniform(SpecularColor);
			pGeneratedProgramUserData->pSpecularExponent				= pProgram->GetUniform(SpecularExponent);
			pGeneratedProgramUserData->pSpecularMap						= pProgram->GetUniform(Material::SpecularMap);
			pGeneratedProgramUserData->pSpecularRampMap					= pProgram->GetUniform(SpecularRampMap);
			pGeneratedProgramUserData->pEdgeRampMap						= pProgram->GetUniform(EdgeRampMap);
			pGeneratedProgramUserData->pGlow							= pProgram->GetUniform(Glow);
			pGeneratedProgramUserData->pGlowMap							= pProgram->GetUniform(GlowMap);
			pGeneratedProgramUserData->pDOFParams						= pProgram->GetUniform(DOFParams);
		}

		// Diffuse color
		if (pGeneratedProgramUserData->pDiffuseColor)
			pGeneratedProgramUserData->pDiffuseColor->Set(m_cDiffuseColor.r, m_cDiffuseColor.g, m_cDiffuseColor.b, m_fOpacity);

		// Diffuse map
		if (pGeneratedProgramUserData->pDiffuseMap && m_pDiffuseMap) {
			const int nTextureUnit = pGeneratedProgramUserData->pDiffuseMap->Set(m_pDiffuseMap->GetValueTextureBuffer());
			if (nTextureUnit >= 0) {
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
				SetupTextureFiltering(cRenderer, nTextureUnit, nTextureFiltering);

				// Set the "AlphaReference" fragment shader parameter
				if (pGeneratedProgramUserData->pAlphaReference)
					pGeneratedProgramUserData->pAlphaReference->Set(m_fAlphaReference);
			}
		}

		// Diffuse ramp map
		if (pGeneratedProgramUserData->pDiffuseRampMap && m_pDiffuseRampMap) {
			const int nTextureUnit = pGeneratedProgramUserData->pDiffuseRampMap->Set(m_pDiffuseRampMap->GetValueTextureBuffer());
			if (nTextureUnit >= 0) {
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU,  TextureAddressing::Clamp);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV,  TextureAddressing::Clamp);
				// No filtering, please
				cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
			}
		}

		// Ambient occlusion map
		if (pGeneratedProgramUserData->pAmbientOcclusionMap && m_pAmbientOcclusionMap) {
			const int nTextureUnit = pGeneratedProgramUserData->pAmbientOcclusionMap->Set(m_pAmbientOcclusionMap->GetValueTextureBuffer());
			if (nTextureUnit >= 0) {
				// Setup sampler states
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Clamp);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Clamp);
				SetupTextureFiltering(cRenderer, nTextureUnit, nTextureFiltering);

				// Set ambient occlusion factor
				if (pGeneratedProgramUserData->pAmbientOcclusionFactor)
					pGeneratedProgramUserData->pAmbientOcclusionFactor->Set(m_fAmbientOcclusionFactor);
			}
		}

		// Light map
		if (pGeneratedProgramUserData->pLightMap && m_pLightMap) {
			const int nTextureUnit = pGeneratedProgramUserData->pLightMap->Set(m_pLightMap->GetValueTextureBuffer());
			if (nTextureUnit >= 0) {
				// Setup sampler states
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Clamp);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Clamp);
				SetupTextureFiltering(cRenderer, nTextureUnit, nTextureFiltering);

				// Set light map color
				if (pGeneratedProgramUserData->pLightMapColor)
					pGeneratedProgramUserData->pLightMapColor->Set3(m_cLightMapColor);
			}
		}

		// Emissive map
		if (pGeneratedProgramUserData->pEmissiveMap && m_pEmissiveMap) {
			const int nTextureUnit = pGeneratedProgramUserData->pEmissiveMap->Set(m_pEmissiveMap->GetValueTextureBuffer());
			if (nTextureUnit >= 0) {
				// Setup sampler states
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
				SetupTextureFiltering(cRenderer, nTextureUnit, nTextureFiltering);

				// Set emissive color
				if (pGeneratedProgramUserData->pEmissiveMapColor)
					pGeneratedProgramUserData->pEmissiveMapColor->Set3(m_cEmissiveMapColor);
			}
		}

		// Set reflection parameters
		if (m_bReflection) {
			// Set the "ReflectionColor" fragment shader parameter
			if (pGeneratedProgramUserData->pReflectionColor)
				pGeneratedProgramUserData->pReflectionColor->Set(m_cReflectionColor);

			// Set the "Reflectivity" fragment shader parameter
			if (pGeneratedProgramUserData->pReflectivity)
				pGeneratedProgramUserData->pReflectivity->Set(m_fReflectivity);

			// Set the "ReflectivityMap" fragment shader parameter
			if (pGeneratedProgramUserData->pReflectivityMap && m_pReflectivityMap) {
				const int nTextureUnit = pGeneratedProgramUserData->pReflectivityMap->Set(m_pReflectivityMap->GetValueTextureBuffer());
				if (nTextureUnit >= 0) {
					cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
					SetupTextureFiltering(cRenderer, nTextureUnit, nTextureFiltering);
				}
			}

			// Use Fresnel reflection?
			if (pGeneratedProgramUserData->pFresnelConstants) {
				// Calculate the eta value
				// [TODO] Make the "from material" also setable per material or global?
				static const float AirIndexOfRefaction = 1.0f;
				const float fEtaValue = AirIndexOfRefaction / m_fIndexOfRefraction; // "from material" -> "to material"

				// Set the "FresnelConstants" fragment shader parameter
				const float fR0 = Math::Saturate(Math::Pow(1.0f - fEtaValue, 2.0f) / Math::Pow(1.0f + fEtaValue, 2.0f));

				// Set uniform
				pGeneratedProgramUserData->pFresnelConstants->Set(fR0, m_fFresnelReflectionPower);
			}

			// Set the "ReflectionMap" fragment shader parameter
			if (pGeneratedProgramUserData->pReflectionMap && m_pReflectionMap) {
				const int nTextureUnit = pGeneratedProgramUserData->pReflectionMap->Set(m_pReflectionMap->GetValueTextureBuffer());
				if (nTextureUnit >= 0) {
					// Setup sampler states
					if (m_b2DReflectionMap) {
						cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
						cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
					} else {
						cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Clamp);
						cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Clamp);
					}
					SetupTextureFiltering(cRenderer, nTextureUnit, nTextureFiltering);
				}
			}
		}

		// Normal map
		if (m_pNormalMap) {
			// Set normal map
			if (pGeneratedProgramUserData->pNormalMap && m_pNormalMap) {
				const int nTextureUnit = pGeneratedProgramUserData->pNormalMap->Set(m_pNormalMap->GetValueTextureBuffer());
				if (nTextureUnit >= 0) {
					cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
					SetupTextureFiltering(cRenderer, nTextureUnit, nTextureFiltering);

					// Set normal map bumpiness
					if (pGeneratedProgramUserData->pNormalMapBumpiness)
						pGeneratedProgramUserData->pNormalMapBumpiness->Set(m_fNormalMapBumpiness);
				}
			}

			// Set detail normal map
			if (pGeneratedProgramUserData->pDetailNormalMap && m_pDetailNormalMap) {
				const int nTextureUnit = pGeneratedProgramUserData->pDetailNormalMap->Set(m_pDetailNormalMap->GetValueTextureBuffer());
				if (nTextureUnit >= 0) {
					cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
					SetupTextureFiltering(cRenderer, nTextureUnit, nTextureFiltering);

					// Set detail normal map bumpiness and uv scale
					if (pGeneratedProgramUserData->pDetailNormalMapBumpiness)
						pGeneratedProgramUserData->pDetailNormalMapBumpiness->Set(m_fDetailNormalMapBumpiness);
					if (pGeneratedProgramUserData->pDetailNormalMapUVScale)
						pGeneratedProgramUserData->pDetailNormalMapUVScale->Set(m_vDetailNormalMapUVScale);
				}
			}
		}

		// Parallax mapping
		if (pGeneratedProgramUserData->pHeightMap && m_pHeightMap) {
			// Height map (for parallax mapping)
			const int nTextureUnit = pGeneratedProgramUserData->pHeightMap->Set(m_pHeightMap->GetValueTextureBuffer());
			if (nTextureUnit >= 0) {
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
				SetupTextureFiltering(cRenderer, nTextureUnit, nTextureFiltering);

				// Set parallax scale bias
				if (pGeneratedProgramUserData->pParallaxScaleBias)
					pGeneratedProgramUserData->pParallaxScaleBias->Set(m_fParallax, -0.02f);
			}
		}

		// Specular
		if (m_cProgramFlags.GetFragmentShaderFlags() & FS_SPECULAR) {
			// Set specular exponent and specular color
			if (pGeneratedProgramUserData->pSpecularColor)
				pGeneratedProgramUserData->pSpecularColor->Set(m_cSpecularColor);
			if (pGeneratedProgramUserData->pSpecularExponent)
				pGeneratedProgramUserData->pSpecularExponent->Set(m_fSpecularExponent);

			// Set the "SpecularMap" fragment shader parameter
			if (pGeneratedProgramUserData->pSpecularMap && m_pSpecularMap) {
				const int nTextureUnit = pGeneratedProgramUserData->pSpecularMap->Set(m_pSpecularMap->GetValueTextureBuffer());
				if (nTextureUnit >= 0) {
					cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
					SetupTextureFiltering(cRenderer, nTextureUnit, nTextureFiltering);
				}
			}

			// Specular ramp map
			if (pGeneratedProgramUserData->pSpecularRampMap && m_pSpecularRampMap) {
				const int nTextureUnit = pGeneratedProgramUserData->pSpecularRampMap->Set(m_pSpecularRampMap->GetValueTextureBuffer());
				if (nTextureUnit >= 0) {
					cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU,  TextureAddressing::Clamp);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV,  TextureAddressing::Clamp);
					// No filtering, please
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
				}
			}
		}

		// Set edge ramp map
		if (pGeneratedProgramUserData->pEdgeRampMap && m_pEdgeRampMap) {
			const int nTextureUnit = pGeneratedProgramUserData->pEdgeRampMap->Set(m_pEdgeRampMap->GetValueTextureBuffer());
			if (nTextureUnit >= 0) {
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU,  TextureAddressing::Clamp);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV,  TextureAddressing::Clamp);
				// No filtering, please
				cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
			}
		}

		// Glow
		if (m_cProgramFlags.GetFragmentShaderFlags() & FS_GLOW) {
			// Set glow parameter - note, internally we flip the value because
			// it's more handy because alpha is 1 by default = no glow
			if (pGeneratedProgramUserData->pGlow)
				pGeneratedProgramUserData->pGlow->Set(1.0f - m_fGlow);

			// Set glow map
			if (pGeneratedProgramUserData->pGlowMap && m_pGlowMap) {
				const int nTextureUnit = pGeneratedProgramUserData->pGlowMap->Set(m_pGlowMap->GetValueTextureBuffer());
				if (nTextureUnit >= 0) {
					// Setup sampler states
					cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Clamp);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Clamp);
					SetupTextureFiltering(cRenderer, nTextureUnit, nTextureFiltering);
				}
			}
		}
	}

	// Transparent stuff only: Write into the alpha channel only for glowing stuff
	if (m_nRendererFlags & SRPDirectionalLightingShaders::TransparentPass) {
		// Setup color mask
		cRenderer.SetColorMask(true, true, true, (m_cProgramFlags.GetFragmentShaderFlags() & FS_GLOW) != 0);

		// Opacity
		if (m_fOpacity < 1) {
			cRenderer.SetRenderState(RenderState::SrcBlendFunc, m_nSrcBlendFunc);
			cRenderer.SetRenderState(RenderState::DstBlendFunc, m_nDstBlendFunc);
		}
	}

	// Two sided
	if (m_bTwoSided) {
		// We need to take care of two sided lighting
		if (nEnvironmentFlags & EnvironmentVertexNormal) {
			// Perform backface culling: For proper two sided lighting, we draw the
			// geometry twice, the second time with flipped vertex normals
			cRenderer.SetRenderState(RenderState::CullMode, Cull::CCW);
		} else {
			// No lighting means that we can go the easy way: Just don't perform backface culling
			cRenderer.SetRenderState(RenderState::CullMode, Cull::None);
		}
	} else {
		// Usual backface culling
		cRenderer.SetRenderState(RenderState::CullMode, Cull::CCW);
	}

	// Done
	return pGeneratedProgramUserData;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Synchronize this material cache with the owner
*/
void SRPDirectionalLightingShadersMaterial::Synchronize(uint32 nRendererFlags, uint32 nEnvironmentFlags)
{
	const Parameter *pParameter = nullptr;

	// Backup the flags
	m_nRendererFlags = nRendererFlags;

	// Reset the program flags
	m_cProgramFlags.Reset();

	{ // Glow
		m_fGlow    = 0.0f;
		m_pGlowMap = nullptr;
		if (nEnvironmentFlags & EnvironmentGlowEnabled) {
			pParameter = m_pMaterial->GetParameter(Glow);
			if (pParameter) {
				m_fGlow = pParameter->GetValue1f();

				// Get glow map
				if (m_fGlow) {
					m_pGlowMap = (m_nRendererFlags & SRPDirectionalLightingShaders::NoGlowMap) ? nullptr : m_pMaterial->GetParameter(GlowMap);
					PL_ADD_FS_FLAG(m_cProgramFlags, FS_GLOW)
				}
			}
		}
	}

	{ // Opacity
		pParameter = m_pMaterial->GetParameter(Opacity);
		m_fOpacity      = pParameter ? pParameter->GetValue1f() : 1.0f;
		m_nSrcBlendFunc = BlendFunc::SrcAlpha;
		m_nDstBlendFunc = BlendFunc::InvSrcAlpha;
		if (m_fOpacity < 1) {
			// Get and set source blend function
			pParameter = m_pMaterial->GetParameter(SrcBlendFunc);
			if (pParameter) {
				m_pRenderStates->SetAttribute(SrcBlendFunc, pParameter->GetParameterString());
				m_nSrcBlendFunc = m_pRenderStates->Get(RenderState::SrcBlendFunc);
			}

			// Get and set destination blend function
			pParameter = m_pMaterial->GetParameter(DstBlendFunc);
			if (pParameter) {
				m_pRenderStates->SetAttribute(DstBlendFunc, pParameter->GetParameterString());
				m_nDstBlendFunc = m_pRenderStates->Get(RenderState::DstBlendFunc);
			}
		}
	}

	// Normal
	if (nEnvironmentFlags & EnvironmentVertexNormal) {
		PL_ADD_VS_FLAG(m_cProgramFlags, VS_NORMAL)
		PL_ADD_FS_FLAG(m_cProgramFlags, FS_NORMAL)
	}

	{ // Two sided
		pParameter = m_pMaterial->GetParameter(TwoSided);
		m_bTwoSided = (pParameter && pParameter->GetValue1f() == 1.0f);
		if (m_bTwoSided) {
			// We need to take care of two sided lighting
			if (nEnvironmentFlags & EnvironmentVertexNormal) {
				// Perform backface culling: For proper two sided lighting, we draw the
				// geometry twice, the second time with flipped vertex normals
				PL_ADD_VS_FLAG(m_cProgramFlags, VS_TWOSIDEDLIGHTING)
			}
		}
	}

	// Use gamma correction?
	if (!(m_nRendererFlags & SRPDirectionalLightingShaders::NoGammaCorrection))
		PL_ADD_FS_FLAG(m_cProgramFlags, FS_GAMMACORRECTION)

	{ // Diffuse map and alpha reference
		m_cDiffuseColor = Color3::White;
		pParameter = m_pMaterial->GetParameter(DiffuseColor);
		if (pParameter)
			pParameter->GetValue3f(m_cDiffuseColor.r, m_cDiffuseColor.g, m_cDiffuseColor.b);
		m_fAlphaReference = 0.0f;
		m_pDiffuseMap     = (!(nEnvironmentFlags & EnvironmentVertexTexCoord0) || (m_nRendererFlags & SRPDirectionalLightingShaders::NoDiffuseMap)) ? nullptr : m_pMaterial->GetParameter(Material::DiffuseMap);
		if (m_pDiffuseMap) {
			PL_ADD_VS_FLAG(m_cProgramFlags, VS_TEXCOORD0)
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_DIFFUSEMAP)

			// Get alpha reference
			if (m_pDiffuseMap->GetValueTextureBuffer() && m_pDiffuseMap->GetValueTextureBuffer()->GetComponentsPerPixel() == 4) {
				pParameter = m_pMaterial->GetParameter(AlphaReference);
				m_fAlphaReference = pParameter ? pParameter->GetValue1f() : 0.5f;
				if (m_fAlphaReference != 0.0f) {
					PL_ADD_FS_FLAG(m_cProgramFlags, FS_ALPHATEST)

					// We need to take care of glow even if this material currently does not glow
					if ((nEnvironmentFlags & EnvironmentGlowEnabled) && ((nEnvironmentFlags & EnvironmentDOFEnabled) || (nEnvironmentFlags & EnvironmentGlowEnabled)))
						PL_ADD_FS_FLAG(m_cProgramFlags, FS_GLOW)
				}
			}
		}
	}

	{ // Diffuse ramp map
		m_pDiffuseRampMap = (!(nEnvironmentFlags & EnvironmentLightingEnabled) || (m_nRendererFlags & SRPDirectionalLightingShaders::NoDiffuseRampMap)) ? nullptr : m_pMaterial->GetParameter(DiffuseRampMap);
		if (m_pDiffuseRampMap)
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_DIFFUSERAMPMAP)
	}

	{ // Ambient map and light map require texture coordinate set 1
		m_fAmbientOcclusionFactor = 1.0f;
		m_pAmbientOcclusionMap	  = nullptr;
		m_cLightMapColor		  = Color3::White;
		m_pLightMap				  = nullptr;
		if (nEnvironmentFlags & EnvironmentVertexTexCoord1) {
			// Get ambient occlusion map
			if (!(m_nRendererFlags & SRPDirectionalLightingShaders::NoAmbientOcclusionMap)) {
				m_pAmbientOcclusionMap = m_pMaterial->GetParameter(Material::AmbientOcclusionMap);
				if (m_pAmbientOcclusionMap) {
					PL_ADD_VS_FLAG(m_cProgramFlags, VS_TEXCOORD1)
					PL_ADD_FS_FLAG(m_cProgramFlags, FS_AMBIENTOCCLUSIONMAP)

					// Get ambient occlusion factor
					pParameter = m_pMaterial->GetParameter(AmbientOcclusionFactor);
					if (pParameter)
						pParameter->GetValue1f(m_fAmbientOcclusionFactor);
				}
			}

			// Get light map
			if (!(m_nRendererFlags & SRPDirectionalLightingShaders::NoLightMap)) {
				m_pLightMap = m_pMaterial->GetParameter(Material::LightMap);
				if (m_pLightMap) {
					PL_ADD_VS_FLAG(m_cProgramFlags, VS_TEXCOORD1)
					PL_ADD_FS_FLAG(m_cProgramFlags, FS_LIGHTMAP)

					// Get light map color
					pParameter = m_pMaterial->GetParameter(LightMapColor);
					if (pParameter)
						pParameter->GetValue3f(m_cLightMapColor.r, m_cLightMapColor.g, m_cLightMapColor.b);
				}
			}
		}
	}

	{ // Emissive map
		m_cEmissiveMapColor	= Color3::White;
		m_pEmissiveMap		= (!(nEnvironmentFlags & EnvironmentVertexTexCoord0) || (m_nRendererFlags & SRPDirectionalLightingShaders::NoEmissiveMap)) ? nullptr : m_pMaterial->GetParameter(Material::EmissiveMap);
		if (m_pEmissiveMap) {
			PL_ADD_VS_FLAG(m_cProgramFlags, VS_TEXCOORD0)
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_EMISSIVEMAP)

			// Get emissive map color
			pParameter = m_pMaterial->GetParameter(EmissiveMapColor);
			if (pParameter)
				pParameter->GetValue3f(m_cEmissiveMapColor.r, m_cEmissiveMapColor.g, m_cEmissiveMapColor.b);
		}
	}

	{ // Index of refraction and fresnel reflection power
		m_fIndexOfRefraction      = 0.0f;
		m_fFresnelReflectionPower = 5.0f;
		if (!(m_nRendererFlags & SRPDirectionalLightingShaders::NoFresnelReflection)) {
			// IndexOfRefraction
			pParameter = m_pMaterial->GetParameter(IndexOfRefraction);
			if (pParameter)
				m_fIndexOfRefraction = pParameter->GetValue1f();
			if (m_fIndexOfRefraction > 0.0f) {
				// FresnelReflectionPower
				pParameter = m_pMaterial->GetParameter(FresnelReflectionPower);
				if (pParameter) {
					m_fFresnelReflectionPower = pParameter->GetValue1f();

					// Ensure that the value is always >0 to avoid NAN production
					if (m_fFresnelReflectionPower < FLT_MIN)
						m_fFresnelReflectionPower = FLT_MIN;
				}
			}
		}
	}

	{ // (2D/cube) reflection map
		m_pReflectionMap   = (m_nRendererFlags & SRPDirectionalLightingShaders::NoReflectionMap) ? nullptr : m_pMaterial->GetParameter(Material::ReflectionMap);
		m_b2DReflectionMap = true;
		if (m_pReflectionMap) {
			TextureBuffer *pTextureBuffer = m_pReflectionMap->GetValueTextureBuffer();
			if (pTextureBuffer) {
				if (pTextureBuffer->GetType() == TextureBuffer::TypeTextureBuffer2D) {
					m_b2DReflectionMap = true;
					PL_ADD_FS_FLAG(m_cProgramFlags, FS_2DREFLECTIONMAP)
				} else if (pTextureBuffer->GetType() == TextureBuffer::TypeTextureBufferCube) {
					m_b2DReflectionMap = false;
					PL_ADD_FS_FLAG(m_cProgramFlags, FS_CUBEREFLECTIONMAP)
				} else {
					m_pReflectionMap = nullptr; // NOT supported!
				}
			} else {
				m_pReflectionMap = nullptr;
			}
		}
	}

	// Figure out whether or not there's reflection on this material
	m_bReflection = ((m_fIndexOfRefraction > 0.0f) || m_pReflectionMap);

	{ // Reflection parameters
		m_pReflectivityMap = nullptr;
		m_fReflectivity    = 1.0f;
		m_cReflectionColor = Color3::White;
		if (m_bReflection) {
			PL_ADD_VS_FLAG(m_cProgramFlags, VS_VIEWSPACEPOSITION)
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_REFLECTION)
			if (m_fIndexOfRefraction > 0.0f)
				PL_ADD_FS_FLAG(m_cProgramFlags, FS_FRESNELREFLECTION)

			// ReflectionColor
			pParameter = m_pMaterial->GetParameter(ReflectionColor);
			if (pParameter)
				m_cReflectionColor = pParameter->GetValue3fv();

			// Get reflectivity map
			if (!(m_nRendererFlags & SRPDirectionalLightingShaders::NoReflectivityMap)) {
				m_pReflectivityMap = m_pMaterial->GetParameter(Material::ReflectivityMap);
				if (m_pReflectivityMap)
					PL_ADD_FS_FLAG(m_cProgramFlags, FS_REFLECTIVITYMAP)
			}

			// Get reflectivity 
			pParameter = m_pMaterial->GetParameter(Reflectivity);
			if (pParameter)
				m_fReflectivity = pParameter->GetValue1f();
		}
	}

	{ // Normal map
		m_pNormalMap		  = (!(nEnvironmentFlags & EnvironmentNormalMappingPossible) || (m_nRendererFlags & SRPDirectionalLightingShaders::NoNormalMap)) ? nullptr : m_pMaterial->GetParameter(Material::NormalMap);
		m_fNormalMapBumpiness = 1.0f;
		if (m_pNormalMap) {
			// Get normal map bumpiness
			const Parameter *pNormalMapParameter = m_pMaterial->GetParameter(NormalMapBumpiness);
			if (pNormalMapParameter)
				m_fNormalMapBumpiness = pNormalMapParameter->GetValue1f();
			if (m_fNormalMapBumpiness != 0.0f) {
				PL_ADD_VS_FLAG(m_cProgramFlags, VS_TEXCOORD0)
				PL_ADD_VS_FLAG(m_cProgramFlags, VS_TANGENT_BINORMAL)
				PL_ADD_FS_FLAG(m_cProgramFlags, FS_TANGENT_BINORMAL)
				PL_ADD_FS_FLAG(m_cProgramFlags, FS_NORMALMAP)

				// DXT5 xGxR normal map?
				const Texture *pNormalMapTexture = m_pNormalMap->GetValueTexture();
				if (pNormalMapTexture) {
					if (pNormalMapTexture->GetCompressionHint() == Texture::DXT5_xGxR || pNormalMapTexture->GetCompressionHint() == Texture::LATC2_XYSwizzle)
						PL_ADD_FS_FLAG(m_cProgramFlags, FS_NORMALMAP_DXT5_XGXR)	// We can use one and the same shader for DXT5_xGxR and LATC2_XYSwizzle :D
					else if (pNormalMapTexture->GetCompressionHint() == Texture::LATC2)
						PL_ADD_FS_FLAG(m_cProgramFlags, FS_NORMALMAP_LATC2)
				}
			} else {
				// The normal map has no longer an influence!
				m_pNormalMap = nullptr;
			}
		}
	}

	{ // Detail normal map
		m_pDetailNormalMap          = (!m_pNormalMap || (m_nRendererFlags & SRPDirectionalLightingShaders::NoDetailNormalMap)) ? nullptr : m_pMaterial->GetParameter(DetailNormalMap);
		m_fDetailNormalMapBumpiness = 1.0f;
		m_vDetailNormalMapUVScale.SetXY(4.0f, 4.0f);
		if (m_pDetailNormalMap) {
			// Get detail normal map bumpiness
			const Parameter *pDetailNormalMapParameter = m_pMaterial->GetParameter(DetailNormalMapBumpiness);
			if (pDetailNormalMapParameter)
				m_fDetailNormalMapBumpiness = pDetailNormalMapParameter->GetValue1f();
			if (m_fDetailNormalMapBumpiness != 0.0f) {
				PL_ADD_FS_FLAG(m_cProgramFlags, FS_DETAILNORMALMAP)

				// Get detail normal map uv scale
				pDetailNormalMapParameter = m_pMaterial->GetParameter(DetailNormalMapUVScale);
				if (pDetailNormalMapParameter)
					m_vDetailNormalMapUVScale = pDetailNormalMapParameter->GetValue2fv();

				// DXT5 xGxR detail normal map?
				const Texture *pDetailNormalMapTexture = m_pDetailNormalMap->GetValueTexture();
				if (pDetailNormalMapTexture) {
					if (pDetailNormalMapTexture->GetCompressionHint() == Texture::DXT5_xGxR || pDetailNormalMapTexture->GetCompressionHint() == Texture::LATC2_XYSwizzle)
						PL_ADD_FS_FLAG(m_cProgramFlags, FS_DETAILNORMALMAP_DXT5_XGXR)	// We can use one and the same shader for DXT5_xGxR and LATC2_XYSwizzle :D
					else if (pDetailNormalMapTexture->GetCompressionHint() == Texture::LATC2)
						PL_ADD_FS_FLAG(m_cProgramFlags, FS_DETAILNORMALMAP_LATC2)
				}
			} else {
				// The detail normal map has no longer an influence!
				m_pDetailNormalMap = nullptr;
			}
		}
	}

	{ // Parallax mapping settings
		m_fParallax  = 0.04f;
		m_pHeightMap = nullptr;
		if (m_pNormalMap && !(m_nRendererFlags & SRPDirectionalLightingShaders::NoParallaxMapping)) {
			// Get parallax
			pParameter = m_pMaterial->GetParameter(Parallax);
			if (pParameter)
				pParameter->GetValue1f(m_fParallax);

			// Get height map
			if (m_fParallax != 0.0f)
				m_pHeightMap = m_pMaterial->GetParameter(Material::HeightMap);

			// No height map = no parallax mapping possible
			if (m_pHeightMap) {
				PL_ADD_VS_FLAG(m_cProgramFlags, VS_PARALLAXMAPPING)
				PL_ADD_FS_FLAG(m_cProgramFlags, FS_PARALLAXMAPPING)
			}
		}
	}

	{ // Lighting and specular
		m_cSpecularColor    = Color3::White;
		m_pSpecularMap      = nullptr;
		m_fSpecularExponent = 45.0f;
		if (nEnvironmentFlags & EnvironmentLightingEnabled) {
			PL_ADD_VS_FLAG(m_cProgramFlags, VS_VIEWSPACEPOSITION)
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_LIGHTING)

			// Specular
			if (!(m_nRendererFlags & SRPDirectionalLightingShaders::NoSpecular)) {
				// First, get specular color - if it's 0, we don't have any specular at all
				pParameter = m_pMaterial->GetParameter(SpecularColor);
				if (pParameter)
					m_cSpecularColor = pParameter->GetValue3fv();
				if (m_cSpecularColor != 0.0f) {
					PL_ADD_FS_FLAG(m_cProgramFlags, FS_SPECULAR)

					// Get specular exponent
					pParameter = m_pMaterial->GetParameter(SpecularExponent);
					if (pParameter)
						pParameter->GetValue1f(m_fSpecularExponent);

					// Get the specular map
					if (!(m_nRendererFlags & SRPDirectionalLightingShaders::NoSpecularMap)) {
						m_pSpecularMap = m_pMaterial->GetParameter(Material::SpecularMap);
						if (m_pSpecularMap)
							PL_ADD_FS_FLAG(m_cProgramFlags, FS_SPECULARMAP)
					}
				}
			}
		}
	}

	{ // Specular ramp map
		m_pSpecularRampMap = (!(m_cProgramFlags.GetFragmentShaderFlags() & FS_SPECULAR) || (m_nRendererFlags & SRPDirectionalLightingShaders::NoSpecularRampMap)) ? nullptr : m_pMaterial->GetParameter(SpecularRampMap);
		if (m_pSpecularRampMap)
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_SPECULARRAMPMAP)
	}

	{ // Edge ramp map
		m_pEdgeRampMap = (!(nEnvironmentFlags & EnvironmentLightingEnabled) || (m_nRendererFlags & SRPDirectionalLightingShaders::NoEdgeRampMap)) ? nullptr : m_pMaterial->GetParameter(EdgeRampMap);
		if (m_pEdgeRampMap)
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_EDGERAMPMAP)
	}

	// Glow or DOF - or nothing of both?
	if (m_cProgramFlags.GetFragmentShaderFlags() & FS_GLOW) {
		if (m_pGlowMap) {
			PL_ADD_VS_FLAG(m_cProgramFlags, VS_TEXCOORD0)
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_GLOWMAP)
		}
	} else {
		// Use DOF?
		if (nEnvironmentFlags & EnvironmentDOFEnabled) {
			PL_ADD_VS_FLAG(m_cProgramFlags, VS_VIEWSPACEPOSITION)
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_DOF)
		}
	}

	// Fragment shader needs texture coordinate flags, too
	if (m_cProgramFlags.GetVertexShaderFlags() & VS_TEXCOORD0)
		PL_ADD_FS_FLAG(m_cProgramFlags, FS_TEXCOORD0)
	if (m_cProgramFlags.GetVertexShaderFlags() & VS_TEXCOORD1)
		PL_ADD_FS_FLAG(m_cProgramFlags, FS_TEXCOORD1)

	// Synchronized!
	m_bSynchronized = true;
}

/**
*  @brief
*    Sets correct texture filtering modes
*/
void SRPDirectionalLightingShadersMaterial::SetupTextureFiltering(Renderer &cRenderer, uint32 nStage, ETextureFiltering nTextureFiltering) const
{
	// Anisotropic filtering
	if (nTextureFiltering > 1) {
		cRenderer.SetSamplerState(nStage, Sampler::MagFilter,     TextureFiltering::Anisotropic);
		cRenderer.SetSamplerState(nStage, Sampler::MinFilter,     TextureFiltering::Anisotropic);
		cRenderer.SetSamplerState(nStage, Sampler::MipFilter,     TextureFiltering::Anisotropic);
		cRenderer.SetSamplerState(nStage, Sampler::MaxAnisotropy, nTextureFiltering);

	// Bilinear filtering
	} else if (nTextureFiltering > 0) {
		cRenderer.SetSamplerState(nStage, Sampler::MagFilter, TextureFiltering::Linear);
		cRenderer.SetSamplerState(nStage, Sampler::MinFilter, TextureFiltering::Linear);
		cRenderer.SetSamplerState(nStage, Sampler::MipFilter, TextureFiltering::Linear);

	// No filtering
	} else {
		cRenderer.SetSamplerState(nStage, Sampler::MagFilter, TextureFiltering::None);
		cRenderer.SetSamplerState(nStage, Sampler::MinFilter, TextureFiltering::None);
		cRenderer.SetSamplerState(nStage, Sampler::MipFilter, TextureFiltering::None);
	}
}

/**
*  @brief
*    Called when a parameter has been changed (created, destroyed, value changed)
*/
void SRPDirectionalLightingShadersMaterial::OnParameterChanged(Parameter &cParameter)
{
	// The this cached material is now dirty!
	m_bSynchronized = false;

	// Look out! Some material parameters may have gone - so reset pointers
	m_pGlowMap				= nullptr;
	m_pDiffuseMap			= nullptr;
	m_pDiffuseRampMap		= nullptr;
	m_pAmbientOcclusionMap	= nullptr;
	m_pLightMap				= nullptr;
	m_pEmissiveMap			= nullptr;
	m_pReflectionMap		= nullptr;
	m_pReflectivityMap		= nullptr;
	m_pNormalMap			= nullptr;
	m_pDetailNormalMap		= nullptr;
	m_pHeightMap			= nullptr;
	m_pSpecularMap			= nullptr;
	m_pSpecularRampMap		= nullptr;
	m_pEdgeRampMap			= nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
