/*********************************************************\
 *  File: SRPDeferredGBufferMaterial.cpp                 *
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
#include <PLRenderer/Material/Material.h>
#include <PLRenderer/Material/Parameter.h>
#include <PLRenderer/Texture/TextureManager.h>
#include <PLRenderer/Renderer/Program.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ProgramGenerator.h>
#include <PLRenderer/Renderer/TextureBufferRectangle.h>
#include "PLCompositing/Shaders/Deferred/SRPDeferredGBuffer.h"
#include "PLCompositing/Shaders/Deferred/SRPDeferredGBufferMaterial.h"


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
const String SRPDeferredGBufferMaterial::TwoSided					= "TwoSided";
const String SRPDeferredGBufferMaterial::DisplacementMap			= "DisplacementMap";
const String SRPDeferredGBufferMaterial::DisplacementScale			= "DisplacementScale";
const String SRPDeferredGBufferMaterial::DisplacementBias			= "DisplacementBias";
const String SRPDeferredGBufferMaterial::IndexOfRefraction			= "IndexOfRefraction";
const String SRPDeferredGBufferMaterial::FresnelReflectionPower		= "FresnelReflectionPower";
const String SRPDeferredGBufferMaterial::ReflectionColor			= "ReflectionColor";
const String SRPDeferredGBufferMaterial::Reflectivity				= "Reflectivity";
const String SRPDeferredGBufferMaterial::Parallax					= "Parallax";
const String SRPDeferredGBufferMaterial::Glow						= "Glow";
const String SRPDeferredGBufferMaterial::GlowMap					= "GlowMap";
const String SRPDeferredGBufferMaterial::AlphaReference				= "AlphaReference";
const String SRPDeferredGBufferMaterial::SpecularColor				= "SpecularColor";
const String SRPDeferredGBufferMaterial::SpecularExponent			= "SpecularExponent";
const String SRPDeferredGBufferMaterial::NormalMapBumpiness			= "NormalMapBumpiness";
const String SRPDeferredGBufferMaterial::DetailNormalMap			= "DetailNormalMap";
const String SRPDeferredGBufferMaterial::DetailNormalMapBumpiness	= "DetailNormalMapBumpiness";
const String SRPDeferredGBufferMaterial::DetailNormalMapUVScale		= "DetailNormalMapUVScale";
// Shader parameter
const String SRPDeferredGBufferMaterial::VertexPosition				= "VertexPosition";
const String SRPDeferredGBufferMaterial::VertexTexCoord0			= "VertexTexCoord0";
const String SRPDeferredGBufferMaterial::VertexTexCoord1			= "VertexTexCoord1";
const String SRPDeferredGBufferMaterial::VertexNormal				= "VertexNormal";
const String SRPDeferredGBufferMaterial::VertexTangent				= "VertexTangent";
const String SRPDeferredGBufferMaterial::VertexBinormal				= "VertexBinormal";
const String SRPDeferredGBufferMaterial::NormalScale				= "NormalScale";
const String SRPDeferredGBufferMaterial::EyePos						= "EyePos";
const String SRPDeferredGBufferMaterial::WorldVP					= "WorldVP";
const String SRPDeferredGBufferMaterial::WorldV						= "WorldV";
const String SRPDeferredGBufferMaterial::DisplacementScaleBias		= "DisplacementScaleBias";
const String SRPDeferredGBufferMaterial::DiffuseColor				= "DiffuseColor";
const String SRPDeferredGBufferMaterial::DiffuseMap					= "DiffuseMap";
// const String SRPDeferredGBufferMaterial::AlphaReference			= "AlphaReference";	// Already defined
const String SRPDeferredGBufferMaterial::SpecularMap				= "SpecularMap";
const String SRPDeferredGBufferMaterial::NormalMap					= "NormalMap";
const String SRPDeferredGBufferMaterial::HeightMap					= "HeightMap";
const String SRPDeferredGBufferMaterial::ParallaxScaleBias			= "ParallaxScaleBias";
const String SRPDeferredGBufferMaterial::AmbientOcclusionMap		= "AmbientOcclusionMap";
const String SRPDeferredGBufferMaterial::AmbientOcclusionFactor		= "AmbientOcclusionFactor";
const String SRPDeferredGBufferMaterial::LightMap					= "LightMap";
const String SRPDeferredGBufferMaterial::LightMapColor				= "LightMapColor";
const String SRPDeferredGBufferMaterial::EmissiveMap				= "EmissiveMap";
const String SRPDeferredGBufferMaterial::EmissiveMapColor			= "EmissiveMapColor";
const String SRPDeferredGBufferMaterial::GlowFactor					= "GlowFactor";
// const String SRPDeferredGBufferMaterial::GlowMap					= "GlowMap";	// Already defined
const String SRPDeferredGBufferMaterial::ReflectivityMap			= "ReflectivityMap";
const String SRPDeferredGBufferMaterial::FresnelConstants			= "FresnelConstants";
const String SRPDeferredGBufferMaterial::ReflectionMap				= "ReflectionMap";
const String SRPDeferredGBufferMaterial::ViewSpaceToWorldSpace		= "ViewSpaceToWorldSpace";


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPDeferredGBufferMaterial::SRPDeferredGBufferMaterial(Material &cMaterial, ProgramGenerator &cProgramGenerator) :
	EventHandlerParameterChanged(&SRPDeferredGBufferMaterial::OnParameterChanged, this),
	// General
	m_pMaterial(&cMaterial),
	m_pProgramGenerator(&cProgramGenerator),
	m_nRendererFlags(0),
	m_bSynchronized(false),
	// Synchronized data
		// Two sided
	m_bTwoSided(false),
		// Displacement mapping settings
	m_fDisplacementScale(0.1f),
	m_fDisplacementBias(0.0f),
	m_pDisplacementMap(nullptr),
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
		// Parallax mapping settings
	m_fParallax(0.04f),
	m_pHeightMap(nullptr),
		// Glow
	m_fGlowFactor(0.0f),
	m_pGlowMap(nullptr),
		// Ambient occlusion map
	m_fAmbientOcclusionFactor(1.0f),
	m_pAmbientOcclusionMap(nullptr),
		// Diffuse map and alpha reference
	m_cDiffuseColor(Color3::White),
	m_fAlphaReference(0.0f),
	m_pDiffuseMap(nullptr),
		// Specular settings
	m_bSpecular(false),
	m_cSpecularColor(Color3::White),
	m_pSpecularMap(nullptr),
	m_fSpecularExponent(45.0f),
		// Normal map
	m_pNormalMap(nullptr),
	m_fNormalMapBumpiness(1.0f),
	m_bNormalMap_DXT5_xGxR(false),
	m_bNormalMap_LATC2(false),
		// Detail normal map
	m_pDetailNormalMap(nullptr),
	m_fDetailNormalMapBumpiness(1.0f),
	m_vDetailNormalMapUVScale(4.0f, 4.0f),
	m_bDetailNormalMap_DXT5_xGxR(false),
	m_bDetailNormalMap_LATC2(false),
		// Light map
	m_cLightMapColor(Color3::White),
	m_pLightMap(nullptr),
		// Emissive map
	m_cEmissiveMapColor(Color3::White),
	m_pEmissiveMap(nullptr)
{
	// Connect event handler
	m_pMaterial->EventParameterChanged.Connect(EventHandlerParameterChanged);
}

/**
*  @brief
*    Destructor
*/
SRPDeferredGBufferMaterial::~SRPDeferredGBufferMaterial()
{
}

/**
*  @brief
*    Makes this material to the currently used one
*/
SRPDeferredGBufferMaterial::GeneratedProgramUserData *SRPDeferredGBufferMaterial::MakeMaterialCurrent(uint32 nRendererFlags, ETextureFiltering nTextureFiltering, bool &bColorTarget3Used, bool &bColorTarget3AlphaUsed)
{
	// Get the used renderer
	Renderer &cRenderer = m_pProgramGenerator->GetRenderer();

	// Synchronize this material cache with the owner
	if (m_nRendererFlags != nRendererFlags || !m_bSynchronized)
		Synchronize(nRendererFlags);

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
			pGeneratedProgramUserData->pVertexPosition				= pProgram->GetAttribute(VertexPosition);
			pGeneratedProgramUserData->pVertexTexCoord0				= pProgram->GetAttribute(VertexTexCoord0);
			pGeneratedProgramUserData->pVertexTexCoord1				= pProgram->GetAttribute(VertexTexCoord1);
			pGeneratedProgramUserData->pVertexNormal				= pProgram->GetAttribute(VertexNormal);
			pGeneratedProgramUserData->pVertexTangent				= pProgram->GetAttribute(VertexTangent);
			pGeneratedProgramUserData->pVertexBinormal				= pProgram->GetAttribute(VertexBinormal);
			// Vertex shader uniforms
			pGeneratedProgramUserData->pNormalScale					= pProgram->GetUniform(NormalScale);
			pGeneratedProgramUserData->pEyePos						= pProgram->GetUniform(EyePos);
			pGeneratedProgramUserData->pWorldVP						= pProgram->GetUniform(WorldVP);
			pGeneratedProgramUserData->pWorldV						= pProgram->GetUniform(WorldV);
			pGeneratedProgramUserData->pDisplacementMap				= pProgram->GetUniform(DisplacementMap);
			pGeneratedProgramUserData->pDisplacementScaleBias		= pProgram->GetUniform(DisplacementScaleBias);
			// Fragment shader uniforms
			pGeneratedProgramUserData->pDiffuseColor				= pProgram->GetUniform(DiffuseColor);
			pGeneratedProgramUserData->pDiffuseMap					= pProgram->GetUniform(DiffuseMap);
			pGeneratedProgramUserData->pAlphaReference				= pProgram->GetUniform(AlphaReference);
			pGeneratedProgramUserData->pSpecularColor				= pProgram->GetUniform(SpecularColor);
			pGeneratedProgramUserData->pSpecularExponent			= pProgram->GetUniform(SpecularExponent);
			pGeneratedProgramUserData->pSpecularMap					= pProgram->GetUniform(SpecularMap);
			pGeneratedProgramUserData->pNormalMap					= pProgram->GetUniform(NormalMap);
			pGeneratedProgramUserData->pNormalMapBumpiness			= pProgram->GetUniform(NormalMapBumpiness);
			pGeneratedProgramUserData->pDetailNormalMap				= pProgram->GetUniform(DetailNormalMap);
			pGeneratedProgramUserData->pDetailNormalMapBumpiness	= pProgram->GetUniform(DetailNormalMapBumpiness);
			pGeneratedProgramUserData->pDetailNormalMapUVScale		= pProgram->GetUniform(DetailNormalMapUVScale);
			pGeneratedProgramUserData->pHeightMap					= pProgram->GetUniform(HeightMap);
			pGeneratedProgramUserData->pParallaxScaleBias			= pProgram->GetUniform(ParallaxScaleBias);
			pGeneratedProgramUserData->pAmbientOcclusionMap			= pProgram->GetUniform(AmbientOcclusionMap);
			pGeneratedProgramUserData->pAmbientOcclusionFactor		= pProgram->GetUniform(AmbientOcclusionFactor);
			pGeneratedProgramUserData->pLightMap					= pProgram->GetUniform(LightMap);
			pGeneratedProgramUserData->pLightMapColor				= pProgram->GetUniform(LightMapColor);
			pGeneratedProgramUserData->pEmissiveMap					= pProgram->GetUniform(EmissiveMap);
			pGeneratedProgramUserData->pEmissiveMapColor			= pProgram->GetUniform(EmissiveMapColor);
			pGeneratedProgramUserData->pGlowFactor					= pProgram->GetUniform(GlowFactor);
			pGeneratedProgramUserData->pGlowMap						= pProgram->GetUniform(GlowMap);
			pGeneratedProgramUserData->pReflectionColor				= pProgram->GetUniform(ReflectionColor);
			pGeneratedProgramUserData->pReflectivity				= pProgram->GetUniform(Reflectivity);
			pGeneratedProgramUserData->pReflectivityMap				= pProgram->GetUniform(ReflectivityMap);
			pGeneratedProgramUserData->pFresnelConstants			= pProgram->GetUniform(FresnelConstants);
			pGeneratedProgramUserData->pReflectionMap				= pProgram->GetUniform(ReflectionMap);
			pGeneratedProgramUserData->pViewSpaceToWorldSpace		= pProgram->GetUniform(ViewSpaceToWorldSpace);
		}

		// Set displacement map
		if (pGeneratedProgramUserData->pDisplacementMap && m_pDisplacementMap) {
			const int nTextureUnit = pGeneratedProgramUserData->pDisplacementMap->Set(m_pDisplacementMap->GetValueTextureBuffer());
			if (nTextureUnit >= 0) {
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
				SetupTextureFiltering(cRenderer, nTextureUnit, nTextureFiltering);
			}

			// Set displacement scale bias
			if (pGeneratedProgramUserData->pDisplacementScaleBias)
				pGeneratedProgramUserData->pDisplacementScaleBias->Set(m_fDisplacementScale, m_fDisplacementBias);
		}

		// Set the "DiffuseColor" fragment shader parameter
		if (pGeneratedProgramUserData->pDiffuseColor)
			pGeneratedProgramUserData->pDiffuseColor->Set(m_cDiffuseColor);

		// Diffuse
		if (pGeneratedProgramUserData->pDiffuseMap && m_pDiffuseMap) {
			// Set the "DiffuseMap" fragment shader parameter
			const int nTextureUnit = pGeneratedProgramUserData->pDiffuseMap->Set(m_pDiffuseMap->GetValueTextureBuffer());
			if (nTextureUnit >= 0) {
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
				SetupTextureFiltering(cRenderer, nTextureUnit, nTextureFiltering);
			}

			// Set the "AlphaReference" fragment shader parameter
			if (pGeneratedProgramUserData->pAlphaReference)
				pGeneratedProgramUserData->pAlphaReference->Set(m_fAlphaReference);
		}

		// Specular
		if (m_bSpecular) {
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
		}

		// Normal map
		if (pGeneratedProgramUserData->pNormalMap && m_pNormalMap) {
			{ // Set normal map
				const int nTextureUnit = pGeneratedProgramUserData->pNormalMap->Set(m_pNormalMap->GetValueTextureBuffer());
				if (nTextureUnit >= 0) {
					cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
					SetupTextureFiltering(cRenderer, nTextureUnit, nTextureFiltering);
				}

				// Set normal map bumpiness
				if (pGeneratedProgramUserData->pNormalMapBumpiness)
					pGeneratedProgramUserData->pNormalMapBumpiness->Set(m_fNormalMapBumpiness);
			}

			// Detail normal map
			if (pGeneratedProgramUserData->pDetailNormalMap && m_pDetailNormalMap) {
				{ // Set detail normal map
					const int nTextureUnit = pGeneratedProgramUserData->pDetailNormalMap->Set(m_pDetailNormalMap->GetValueTextureBuffer());
					if (nTextureUnit >= 0) {
						cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
						cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
						SetupTextureFiltering(cRenderer, nTextureUnit, nTextureFiltering);
					}
				}

				// Set detail normal map bumpiness and uv scale
				if (pGeneratedProgramUserData->pDetailNormalMapBumpiness)
					pGeneratedProgramUserData->pDetailNormalMapBumpiness->Set(m_fDetailNormalMapBumpiness);
				if (pGeneratedProgramUserData->pDetailNormalMapUVScale)
					pGeneratedProgramUserData->pDetailNormalMapUVScale->Set(m_vDetailNormalMapUVScale);
			}
		}

		// Set height map
		if (pGeneratedProgramUserData->pHeightMap && m_pHeightMap) {
			const int nTextureUnit = pGeneratedProgramUserData->pHeightMap->Set(m_pHeightMap->GetValueTextureBuffer());
			if (nTextureUnit >= 0) {
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
				SetupTextureFiltering(cRenderer, nTextureUnit, nTextureFiltering);
			}

			// Set parallax scale bias
			if (pGeneratedProgramUserData->pParallaxScaleBias)
				pGeneratedProgramUserData->pParallaxScaleBias->Set(m_fParallax, -0.02f);
		}

		// Set ambient occlusion map
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

		// Set light map
		if (pGeneratedProgramUserData->pLightMap && m_pLightMap) {
			const int nTextureUnit = pGeneratedProgramUserData->pLightMap->Set(m_pLightMap->GetValueTextureBuffer());
			if (nTextureUnit >= 0) {
				// Setup sampler states
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Clamp);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Clamp);
				SetupTextureFiltering(cRenderer, nTextureUnit, nTextureFiltering);

				// Set light map color
				if (pGeneratedProgramUserData->pLightMapColor)
					pGeneratedProgramUserData->pLightMapColor->Set(m_cLightMapColor);
			}

			// Color target 3 has real information
			bColorTarget3Used = true;
		}

		// Set emissive map
		if (pGeneratedProgramUserData->pEmissiveMap && m_pEmissiveMap) {
			const int nTextureUnit = pGeneratedProgramUserData->pEmissiveMap->Set(m_pEmissiveMap->GetValueTextureBuffer());
			if (nTextureUnit >= 0) {
				// Setup sampler states
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Wrap);
				SetupTextureFiltering(cRenderer, nTextureUnit, nTextureFiltering);

				// Set emissive color
				if (pGeneratedProgramUserData->pEmissiveMapColor)
					pGeneratedProgramUserData->pEmissiveMapColor->Set(m_cEmissiveMapColor);
			}

			// Color target 3 has real information
			bColorTarget3Used = true;
		}

		// Set the "GlowFactor" fragment shader parameter
		if (m_fGlowFactor != 0.0f) {
			if (pGeneratedProgramUserData->pGlowFactor)
				pGeneratedProgramUserData->pGlowFactor->Set(m_fGlowFactor);

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

			// The alpha channel of target 3 has real information
			bColorTarget3AlphaUsed = true;
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
void SRPDeferredGBufferMaterial::Synchronize(uint32 nRendererFlags)
{
	const Parameter *pParameter = nullptr;

	// Backup the flags
	m_nRendererFlags = nRendererFlags;

	{ // Two sided
		pParameter = m_pMaterial->GetParameter(TwoSided);
		m_bTwoSided = (pParameter && pParameter->GetValue1f() == 1.0f);
	}

	{ // Displacement mapping settings
		m_fDisplacementScale = 0.1f;
		m_fDisplacementBias  = 0.0f;
		m_pDisplacementMap   = nullptr;
		if (!(m_nRendererFlags & SRPDeferredGBuffer::NoDisplacementMapping)) {
			// Get displacement scale
			pParameter = m_pMaterial->GetParameter(DisplacementScale);
			if (pParameter)
				pParameter->GetValue1f(m_fDisplacementScale);

			// Get displacement bias
			pParameter = m_pMaterial->GetParameter(DisplacementBias);
			if (pParameter)
				pParameter->GetValue1f(m_fDisplacementBias);

			// Get displacement map
			if (m_fDisplacementScale != 0.0f) {
				m_pDisplacementMap = m_pMaterial->GetParameter(DisplacementMap);
				if (m_pDisplacementMap) {
					TextureBuffer *pTextureBuffer = m_pDisplacementMap->GetValueTextureBuffer();
					if (!pTextureBuffer || pTextureBuffer->GetType() != TextureBuffer::TypeTextureBuffer2D)
						m_pDisplacementMap = nullptr;
				}
			}
		}
	}

	{ // Index of refraction and fresnel reflection power
		m_fIndexOfRefraction      = 0.0f;
		m_fFresnelReflectionPower = 5.0f;
		if (!(m_nRendererFlags & SRPDeferredGBuffer::NoFresnelReflection)) {
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
		m_pReflectionMap   = (m_nRendererFlags & SRPDeferredGBuffer::NoReflectionMap) ? nullptr : m_pMaterial->GetParameter(Material::ReflectionMap);
		m_b2DReflectionMap = true;
		if (m_pReflectionMap) {
			TextureBuffer *pTextureBuffer = m_pReflectionMap->GetValueTextureBuffer();
			if (pTextureBuffer) {
				if (pTextureBuffer->GetType() == TextureBuffer::TypeTextureBuffer2D)
					m_b2DReflectionMap = true;
				else if (pTextureBuffer->GetType() == TextureBuffer::TypeTextureBufferCube)
					m_b2DReflectionMap = false;
				else
					m_pReflectionMap = nullptr; // NOT supported!
			} else {
				m_pReflectionMap = nullptr;
			}
		}
	}

	{ // Figure out whether or not there's reflection on this material
		m_bReflection = ((m_fIndexOfRefraction > 0.0f) || m_pReflectionMap);
	}

	{ // Reflection parameters
		m_pReflectivityMap = nullptr;
		m_fReflectivity    = 1.0f;
		m_cReflectionColor = Color3::White;
		if (m_bReflection) {
			// ReflectionColor
			pParameter = m_pMaterial->GetParameter(ReflectionColor);
			if (pParameter)
				m_cReflectionColor = pParameter->GetValue3fv();

			// Get reflectivity map
			if (!(m_nRendererFlags & SRPDeferredGBuffer::NoReflectivityMap))
				m_pReflectivityMap = m_pMaterial->GetParameter(Material::ReflectivityMap);

			// Get reflectivity 
			pParameter = m_pMaterial->GetParameter(Reflectivity);
			if (pParameter)
				m_fReflectivity = pParameter->GetValue1f();
		}
	}

	{ // Parallax mapping settings
		m_fParallax  = 0.04f;
		m_pHeightMap = nullptr;
		if (!(m_nRendererFlags & SRPDeferredGBuffer::NoParallaxMapping)) {
			// Get parallax
			pParameter = m_pMaterial->GetParameter(Parallax);
			if (pParameter)
				pParameter->GetValue1f(m_fParallax);

			// Get height map
			if (m_fParallax != 0.0f)
				m_pHeightMap = m_pMaterial->GetParameter(Material::HeightMap);
		}
		// No height map = no parallax mapping possible
		if (!m_pHeightMap)
			m_fParallax = 0.0f;
	}

	{ // Glow
		m_fGlowFactor = 0.0f;
		m_pGlowMap    = nullptr;
		if (!(m_nRendererFlags & SRPDeferredGBuffer::NoGlow)) {
			pParameter = m_pMaterial->GetParameter(Glow);
			if (pParameter) {
				m_fGlowFactor = pParameter->GetValue1f();

				// Get glow map
				if (m_fGlowFactor != 0.0f)
					m_pGlowMap = (m_nRendererFlags & SRPDeferredGBuffer::NoGlowMap) ? nullptr : m_pMaterial->GetParameter(GlowMap);
			}
		}
	}

	{ // Ambient occlusion map
		m_fAmbientOcclusionFactor = 1.0f;
		pParameter = m_pMaterial->GetParameter(AmbientOcclusionFactor);
		if (pParameter)
			pParameter->GetValue1f(m_fAmbientOcclusionFactor);
		m_pAmbientOcclusionMap = (m_nRendererFlags & SRPDeferredGBuffer::NoAmbientOcclusionMap) ? nullptr : m_pMaterial->GetParameter(Material::AmbientOcclusionMap);
	}

	{ // Diffuse map and alpha reference
		m_cDiffuseColor = Color3::White;
		pParameter = m_pMaterial->GetParameter(DiffuseColor);
		if (pParameter)
			pParameter->GetValue3f(m_cDiffuseColor.r, m_cDiffuseColor.g, m_cDiffuseColor.b);
		m_fAlphaReference = 0.0f;
		m_pDiffuseMap     = (m_nRendererFlags & SRPDeferredGBuffer::NoDiffuseMap) ? nullptr : m_pMaterial->GetParameter(Material::DiffuseMap);

		// Enable/disable alpha test (fragments are thrown away inside the fragment shader using the 'discard' keyword)
		if (m_pDiffuseMap && m_pDiffuseMap->GetValueTextureBuffer() && m_pDiffuseMap->GetValueTextureBuffer()->GetComponentsPerPixel() == 4) {
			// Get alpha reference
			pParameter = m_pMaterial->GetParameter(AlphaReference);
			m_fAlphaReference = pParameter ? pParameter->GetValue1f() : 0.5f;
		}
	}

	{ // Specular settings
		m_bSpecular			= false;
		m_cSpecularColor	= Color3::White;
		m_pSpecularMap		= nullptr;
		m_fSpecularExponent = 45.0f;
		if (!(m_nRendererFlags & SRPDeferredGBuffer::NoSpecular)) {
			// First, get specular color - if it's 0, we don't have any specular at all
			pParameter = m_pMaterial->GetParameter(SpecularColor);
			if (pParameter)
				m_cSpecularColor = pParameter->GetValue3fv();
			m_bSpecular = (m_cSpecularColor != 0.0f);
		}
		if (m_bSpecular) {
			// Get the specular map
			if (!(m_nRendererFlags & SRPDeferredGBuffer::NoSpecularMap))
				m_pSpecularMap = m_pMaterial->GetParameter(Material::SpecularMap);

			// Get specular exponent
			pParameter = m_pMaterial->GetParameter(SpecularExponent);
			if (pParameter)
				pParameter->GetValue1f(m_fSpecularExponent);
		}
	}

	{ // Normal map
		m_pNormalMap           = (m_nRendererFlags & SRPDeferredGBuffer::NoNormalMap) ? nullptr : m_pMaterial->GetParameter(Material::NormalMap);
		m_fNormalMapBumpiness  = 1.0f;
		m_bNormalMap_DXT5_xGxR = false;
		m_bNormalMap_LATC2     = false;
		if (m_pNormalMap) {
			// Get normal map bumpiness
			const Parameter *pNormalMapParameter = m_pMaterial->GetParameter(NormalMapBumpiness);
			if (pNormalMapParameter)
				m_fNormalMapBumpiness = pNormalMapParameter->GetValue1f();
			if (m_fNormalMapBumpiness != 0.0f) {
				// DXT5 xGxR normal map?
				const Texture *pNormalMapTexture = m_pNormalMap->GetValueTexture();
				if (pNormalMapTexture) {
					if (pNormalMapTexture->GetCompressionHint() == Texture::DXT5_xGxR || pNormalMapTexture->GetCompressionHint() == Texture::LATC2_XYSwizzle)
						m_bNormalMap_DXT5_xGxR = true;	// We can use one and the same shader for DXT5_xGxR and LATC2_XYSwizzle :D
					else if (pNormalMapTexture->GetCompressionHint() == Texture::LATC2)
						m_bNormalMap_LATC2 = true;
				}
			} else {
				// The normal map has no longer an influence!
				m_pNormalMap = nullptr;
			}
		}
	}

	{ // Detail normal map
		m_pDetailNormalMap			 = (!m_pNormalMap || (m_nRendererFlags & SRPDeferredGBuffer::NoDetailNormalMap)) ? nullptr : m_pMaterial->GetParameter(DetailNormalMap);
		m_fDetailNormalMapBumpiness  = 1.0f;
		m_vDetailNormalMapUVScale.SetXY(4.0f, 4.0f);
		m_bDetailNormalMap_DXT5_xGxR = false;
		m_bDetailNormalMap_LATC2     = false;
		if (m_pDetailNormalMap) {
			// Get detail normal map bumpiness
			const Parameter *pDetailNormalMapParameter = m_pMaterial->GetParameter(DetailNormalMapBumpiness);
			if (pDetailNormalMapParameter)
				m_fDetailNormalMapBumpiness = pDetailNormalMapParameter->GetValue1f();
			if (m_fDetailNormalMapBumpiness != 0.0f) {
				// Get detail normal map uv scale
				pDetailNormalMapParameter = m_pMaterial->GetParameter(DetailNormalMapUVScale);
				if (pDetailNormalMapParameter)
					m_vDetailNormalMapUVScale = pDetailNormalMapParameter->GetValue2fv();

				// DXT5 xGxR detail normal map?
				const Texture *pDetailNormalMapTexture = m_pDetailNormalMap->GetValueTexture();
				if (pDetailNormalMapTexture) {
					if (pDetailNormalMapTexture->GetCompressionHint() == Texture::DXT5_xGxR || pDetailNormalMapTexture->GetCompressionHint() == Texture::LATC2_XYSwizzle)
						m_bDetailNormalMap_DXT5_xGxR = true;	// We can use one and the same shader for DXT5_xGxR and LATC2_XYSwizzle :D
					else if (pDetailNormalMapTexture->GetCompressionHint() == Texture::LATC2)
						m_bDetailNormalMap_LATC2 = true;
				}
			} else {
				// The detail normal map has no longer an influence!
				m_pDetailNormalMap = nullptr;
			}
		}
	}

	{ // Light map
		m_cLightMapColor = Color3::White;
		pParameter = m_pMaterial->GetParameter(LightMapColor);
		if (pParameter)
			pParameter->GetValue3f(m_cLightMapColor.r, m_cLightMapColor.g, m_cLightMapColor.b);
		m_pLightMap = (m_nRendererFlags & SRPDeferredGBuffer::NoLightMap) ? nullptr : m_pMaterial->GetParameter(Material::LightMap);
	}

	{ // Emissive map
		m_cEmissiveMapColor = Color3::White;
		pParameter = m_pMaterial->GetParameter(EmissiveMapColor);
		if (pParameter)
			pParameter->GetValue3f(m_cEmissiveMapColor.r, m_cEmissiveMapColor.g, m_cEmissiveMapColor.b);
		m_pEmissiveMap = (m_nRendererFlags & SRPDeferredGBuffer::NoEmissiveMap) ? nullptr : m_pMaterial->GetParameter(Material::EmissiveMap);
	}

	// Reset the program flags
	m_cProgramFlags.Reset();

	// Set vertex program flags
	if (m_bTwoSided)
		PL_ADD_VS_FLAG(m_cProgramFlags, VS_TWOSIDED)
	if (m_pDisplacementMap != nullptr)
		PL_ADD_VS_FLAG(m_cProgramFlags, VS_DISPLACEMENTMAP)
	if (m_pAmbientOcclusionMap != nullptr || m_pLightMap != nullptr)
		PL_ADD_VS_FLAG(m_cProgramFlags, VS_SECONDTEXTURECOORDINATE)
	if (m_pNormalMap != nullptr || m_fParallax != 0.0f)
		PL_ADD_VS_FLAG(m_cProgramFlags, VS_TANGENT_BINORMAL)
	if (m_bReflection)
		PL_ADD_VS_FLAG(m_cProgramFlags, VS_VIEWSPACEPOSITION)
	
	// Set fragment program flags
	if (m_pDiffuseMap != nullptr) {
		PL_ADD_FS_FLAG(m_cProgramFlags, FS_DIFFUSEMAP)
		if (m_fAlphaReference != 0.0f)
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_ALPHATEST)
	}
	if (m_bSpecular) {
		PL_ADD_FS_FLAG(m_cProgramFlags, FS_SPECULAR)
		if (m_pSpecularMap != nullptr)
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_SPECULARMAP)
	}
	if (m_pNormalMap != nullptr) {
		PL_ADD_FS_FLAG(m_cProgramFlags, FS_NORMALMAP)
		if (m_bNormalMap_DXT5_xGxR)
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_NORMALMAP_DXT5_XGXR)
		else if (m_bNormalMap_LATC2)
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_NORMALMAP_LATC2)
		if (m_pDetailNormalMap != nullptr) {
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_DETAILNORMALMAP)
			if (m_bDetailNormalMap_DXT5_xGxR)
				PL_ADD_FS_FLAG(m_cProgramFlags, FS_DETAILNORMALMAP_DXT5_XGXR)
			else if (m_bDetailNormalMap_LATC2)
				PL_ADD_FS_FLAG(m_cProgramFlags, FS_DETAILNORMALMAP_LATC2)
		}
	}
	if (m_fParallax != 0.0f)
		PL_ADD_FS_FLAG(m_cProgramFlags, FS_PARALLAXMAPPING)
	if (m_pAmbientOcclusionMap != nullptr)
		PL_ADD_FS_FLAG(m_cProgramFlags, FS_AMBIENTOCCLUSIONMAP)
	if (m_pLightMap != nullptr)
		PL_ADD_FS_FLAG(m_cProgramFlags, FS_LIGHTMAP)
	if (m_pEmissiveMap != nullptr)
		PL_ADD_FS_FLAG(m_cProgramFlags, FS_EMISSIVEMAP)
	if (m_fGlowFactor != 0.0f) {
		PL_ADD_FS_FLAG(m_cProgramFlags, FS_GLOW)
		if (m_pGlowMap != nullptr)
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_GLOWMAP)
	}
	if ((m_fIndexOfRefraction > 0.0f || (m_b2DReflectionMap && m_pReflectionMap) || (!m_b2DReflectionMap && m_pReflectionMap))) {
		PL_ADD_FS_FLAG(m_cProgramFlags, FS_REFLECTION)
		if (m_fIndexOfRefraction > 0.0f)
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_FRESNELREFLECTION)
		if (m_pReflectivityMap != nullptr)
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_REFLECTIVITYMAP)
		if (m_b2DReflectionMap && m_pReflectionMap)
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_2DREFLECTIONMAP)
		else if (!m_b2DReflectionMap && m_pReflectionMap)
			PL_ADD_FS_FLAG(m_cProgramFlags, FS_CUBEREFLECTIONMAP)
	}
	// Use gamma correction?
	if (!(m_nRendererFlags & SRPDeferredGBuffer::NoGammaCorrection))
		PL_ADD_FS_FLAG(m_cProgramFlags, FS_GAMMACORRECTION)

	// Synchronized!
	m_bSynchronized = true;
}

/**
*  @brief
*    Sets correct texture filtering modes
*/
void SRPDeferredGBufferMaterial::SetupTextureFiltering(Renderer &cRenderer, uint32 nStage, ETextureFiltering nTextureFiltering) const
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
void SRPDeferredGBufferMaterial::OnParameterChanged(Parameter &cParameter)
{
	// The this cached material is now dirty!
	m_bSynchronized = false;

	// Look out! Some material parameters may have gone - so reset pointers
	m_pDisplacementMap		= nullptr;
	m_pReflectionMap		= nullptr;
	m_pReflectivityMap		= nullptr;
	m_pHeightMap			= nullptr;
	m_pGlowMap				= nullptr;
	m_pAmbientOcclusionMap	= nullptr;
	m_pDiffuseMap			= nullptr;
	m_pSpecularMap			= nullptr;
	m_pNormalMap			= nullptr;
	m_pDetailNormalMap		= nullptr;
	m_pLightMap				= nullptr;
	m_pEmissiveMap			= nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
