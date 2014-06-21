/*********************************************************\
 *  File: SRPDeferredGBufferMaterial.h                   *
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


#ifndef __PLCOMPOSITING_DEFERRED_GBUFFER_MATERIAL_H__
#define __PLCOMPOSITING_DEFERRED_GBUFFER_MATERIAL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2.h>
#include <PLGraphics/Color/Color3.h>
#include <PLRenderer/Renderer/ProgramGenerator.h>
#include "PLCompositing/PLCompositing.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class Material;
	class Renderer;
	class Parameter;
	class ProgramUniform;
	class ProgramAttribute;
	class ProgramGenerator;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCompositing {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    SRPDeferredGBuffer-material
*/
class SRPDeferredGBufferMaterial {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Texture filtering modes
		*/
		enum ETextureFiltering {
			NoFiltering   = 0,	/**< No filtering */
			Bilinear      = 1,	/**< Bilinear */
			Anisotropic2  = 2,	/**< Anisotropic x2 */
			Anisotropic4  = 4,	/**< Anisotropic x4 */
			Anisotropic8  = 8,	/**< Anisotropic x8 */
			Anisotropic16 = 16	/**< Anisotropic x16 */
		};
		/**
		*  @brief
		*    Texture filtering modes
		*/
		pl_enum(ETextureFiltering)
			pl_enum_value(NoFiltering,		"No filtering")
			pl_enum_value(Bilinear,			"Bilinear")
			pl_enum_value(Anisotropic2,		"Anisotropic x2")
			pl_enum_value(Anisotropic4,		"Anisotropic x4")
			pl_enum_value(Anisotropic8,		"Anisotropic x8")
			pl_enum_value(Anisotropic16,	"Anisotropic x16")
		pl_enum_end

		/**
		*  @brief
		*    Direct pointers to uniforms & attributes of a generated program
		*/
		struct GeneratedProgramUserData {
			// Vertex shader attributes
			PLRenderer::ProgramAttribute *pVertexPosition;
			PLRenderer::ProgramAttribute *pVertexTexCoord0;
			PLRenderer::ProgramAttribute *pVertexTexCoord1;
			PLRenderer::ProgramAttribute *pVertexNormal;
			PLRenderer::ProgramAttribute *pVertexTangent;
			PLRenderer::ProgramAttribute *pVertexBinormal;
			// Vertex shader uniforms
			PLRenderer::ProgramUniform *pNormalScale;
			PLRenderer::ProgramUniform *pEyePos;
			PLRenderer::ProgramUniform *pWorldVP;
			PLRenderer::ProgramUniform *pWorldV;
			PLRenderer::ProgramUniform *pDisplacementMap;
			PLRenderer::ProgramUniform *pDisplacementScaleBias;
			// Fragment shader uniforms
			PLRenderer::ProgramUniform *pDiffuseColor;
			PLRenderer::ProgramUniform *pDiffuseMap;
			PLRenderer::ProgramUniform *pAlphaReference;
			PLRenderer::ProgramUniform *pSpecularColor;
			PLRenderer::ProgramUniform *pSpecularExponent;
			PLRenderer::ProgramUniform *pSpecularMap;
			PLRenderer::ProgramUniform *pNormalMap;
			PLRenderer::ProgramUniform *pNormalMapBumpiness;
			PLRenderer::ProgramUniform *pDetailNormalMap;
			PLRenderer::ProgramUniform *pDetailNormalMapBumpiness;
			PLRenderer::ProgramUniform *pDetailNormalMapUVScale;
			PLRenderer::ProgramUniform *pHeightMap;
			PLRenderer::ProgramUniform *pParallaxScaleBias;
			PLRenderer::ProgramUniform *pAmbientOcclusionMap;
			PLRenderer::ProgramUniform *pAmbientOcclusionFactor;
			PLRenderer::ProgramUniform *pLightMap;
			PLRenderer::ProgramUniform *pLightMapColor;
			PLRenderer::ProgramUniform *pEmissiveMap;
			PLRenderer::ProgramUniform *pEmissiveMapColor;
			PLRenderer::ProgramUniform *pGlowFactor;
			PLRenderer::ProgramUniform *pGlowMap;
			PLRenderer::ProgramUniform *pReflectionColor;
			PLRenderer::ProgramUniform *pReflectivity;
			PLRenderer::ProgramUniform *pReflectivityMap;
			PLRenderer::ProgramUniform *pFresnelConstants;
			PLRenderer::ProgramUniform *pReflectionMap;
			PLRenderer::ProgramUniform *pViewSpaceToWorldSpace;
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cMaterial
		*    Owner material
		*  @param[in] cProgramGenerator
		*    Program generator
		*/
		SRPDeferredGBufferMaterial(PLRenderer::Material &cMaterial, PLRenderer::ProgramGenerator &cProgramGenerator);

		/**
		*  @brief
		*    Destructor
		*/
		~SRPDeferredGBufferMaterial();

		/**
		*  @brief
		*    Makes this material to the currently used one
		*
		*  @param[in]  nRendererFlags
		*    SRPDeferredGBuffer-flags to use
		*  @param[in]  nTextureFiltering
		*    Texture filtering
		*  @param[out] bColorTarget3Used
		*    Has color target 3 real information?
		*  @param[out] bColorTarget3AlphaUsed
		*    Has the alpha channel of target 3 real information?
		*
		*  @return
		*    Generated program user data, do NOT delete the memory the pointer points to
		*/
		GeneratedProgramUserData *MakeMaterialCurrent(PLCore::uint32 nRendererFlags, ETextureFiltering nTextureFiltering, bool &bColorTarget3Used, bool &bColorTarget3AlphaUsed);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Vertex shader flags, flag names become to source code definitions
		*/
		enum EVertexShaderFlags {
			VS_TWOSIDED					= 1<<0,	/**< Two sided material */
			VS_DISPLACEMENTMAP			= 1<<1,	/**< Displacement map */
			VS_SECONDTEXTURECOORDINATE	= 1<<2,	/**< Second texture coordinate */
			VS_TANGENT_BINORMAL			= 1<<3,	/**< Tangent and binormal vectors */
			VS_VIEWSPACEPOSITION		= 1<<4	/**< Calculate viewspace position */
		};

		/**
		*  @brief
		*    Fragment shader flags, flag names become to source code definitions
		*/
		enum EFragmentShaderFlags {
			FS_DIFFUSEMAP							= 1<<0,		/**< Take diffuse map into account */
				FS_ALPHATEST						= 1<<1,		/**< Use alpha test to discard fragments (FS_DIFFUSEMAP must be defined!) */
			FS_SPECULAR								= 1<<2,		/**< Use specular */
				FS_SPECULARMAP						= 1<<3,		/**< Take specular map into account (FS_SPECULAR must be set, too) */
			FS_NORMALMAP							= 1<<4,		/**< Take normal map into account */
				FS_NORMALMAP_DXT5_XGXR				= 1<<5,		/**< DXT5 XGXR compressed normal map (FS_NORMALMAP must be defined and FS_NORMALMAP_LATC2 not!) */
				FS_NORMALMAP_LATC2					= 1<<6,		/**< LATC2 compressed normal map (FS_NORMALMAP must be defined and FS_NORMALMAP_DXT5_XGXR not!) */
				FS_DETAILNORMALMAP					= 1<<7,		/**< Take detail normal map into account (FS_NORMALMAP must be defined!) */
					FS_DETAILNORMALMAP_DXT5_XGXR	= 1<<8,		/**< DXT5 XGXR compressed detail normal map (FS_NORMALMAP & FS_DETAILNORMALMAP must be defined and FS_DETAILNORMALMAP_LATC2 not!) */
					FS_DETAILNORMALMAP_LATC2		= 1<<9,		/**< LATC2 compressed detail normal map (FS_NORMALMAP & FS_DETAILNORMALMAP must be defined and FS_DETAILNORMALMAP_DXT5_XGXR not!) */
			FS_PARALLAXMAPPING						= 1<<10,	/**< Perform parallax mapping */
			FS_AMBIENTOCCLUSIONMAP					= 1<<11,	/**< Use ambient occlusion map */
			FS_LIGHTMAP								= 1<<12,	/**< Use light map */
			FS_EMISSIVEMAP							= 1<<13,	/**< Use emissive map */
			FS_GLOW									= 1<<14,	/**< Use glow */
				FS_GLOWMAP							= 1<<15,	/**< Use glow map (FS_GLOW must be defined!) */
			FS_REFLECTION							= 1<<16,	/**< Use reflection */
				FS_FRESNELREFLECTION				= 1<<17,	/**< Use fresnel reflection (FS_REFLECTION must be defined!) */
				FS_REFLECTIVITYMAP					= 1<<18,	/**< Use reflectivity map (FS_REFLECTION and FS_FRESNELREFLECTION or FS_2DREFLECTIONMAP or FS_CUBEREFLECTIONMAP must be defined!) */
				FS_2DREFLECTIONMAP					= 1<<19,	/**< Use 2D reflection mapping (FS_REFLECTION must be defined, can't be set together with FS_CUBEREFLECTIONMAP!) */
				FS_CUBEREFLECTIONMAP				= 1<<20,	/**< Use cube reflection mapping (FS_REFLECTION must be defined, can't be set together with FS_2DREFLECTIONMAP!) */
			FS_GAMMACORRECTION						= 1<<21		/**< Use gamma correction (sRGB to linear space) */
		};


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		// Material parameter
		static const PLCore::String TwoSided;
		static const PLCore::String DisplacementMap;
		static const PLCore::String DisplacementScale;
		static const PLCore::String DisplacementBias;
		static const PLCore::String IndexOfRefraction;
		static const PLCore::String FresnelReflectionPower;
		static const PLCore::String ReflectionColor;
		static const PLCore::String Reflectivity;
		static const PLCore::String Parallax;
		static const PLCore::String Glow;
		static const PLCore::String GlowMap;
		static const PLCore::String AlphaReference;
		static const PLCore::String SpecularColor;
		static const PLCore::String SpecularExponent;
		static const PLCore::String NormalMapBumpiness;
		static const PLCore::String DetailNormalMap;
		static const PLCore::String DetailNormalMapBumpiness;
		static const PLCore::String DetailNormalMapUVScale;
		// Shader parameter
		static const PLCore::String VertexPosition;
		static const PLCore::String VertexTexCoord0;
		static const PLCore::String VertexTexCoord1;
		static const PLCore::String VertexNormal;
		static const PLCore::String VertexTangent;
		static const PLCore::String VertexBinormal;
		static const PLCore::String NormalScale;
		static const PLCore::String EyePos;
		static const PLCore::String WorldVP;
		static const PLCore::String WorldV;
		static const PLCore::String DisplacementScaleBias;
		static const PLCore::String DiffuseColor;
		static const PLCore::String DiffuseMap;
		// static const PLCore::String AlphaReference;	// Already defined
		static const PLCore::String SpecularMap;
		static const PLCore::String NormalMap;
		static const PLCore::String HeightMap;
		static const PLCore::String ParallaxScaleBias;
		static const PLCore::String AmbientOcclusionMap;
		static const PLCore::String AmbientOcclusionFactor;
		static const PLCore::String LightMap;
		static const PLCore::String LightMapColor;
		static const PLCore::String EmissiveMap;
		static const PLCore::String EmissiveMapColor;
		static const PLCore::String GlowFactor;
		// static const PLCore::String GlowMap;	// Already defined
		static const PLCore::String ReflectivityMap;
		static const PLCore::String FresnelConstants;
		static const PLCore::String ReflectionMap;
		static const PLCore::String ViewSpaceToWorldSpace;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Synchronize this material cache with the owner
		*
		*  @param[in] nRendererFlags
		*    SRPDeferredGBuffer-flags to use
		*/
		void Synchronize(PLCore::uint32 nRendererFlags);

		/**
		*  @brief
		*    Sets correct texture filtering modes
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] nStage
		*    Texture stage
		*  @param[in] nTextureFiltering
		*    Texture filtering
		*/
		void SetupTextureFiltering(PLRenderer::Renderer &cRenderer, PLCore::uint32 nStage, ETextureFiltering nTextureFiltering) const;

		/**
		*  @brief
		*    Called when a parameter has been changed (created, destroyed, value changed)
		*
		*  @param[in] cParameter
		*    The changed parameter
		*/
		void OnParameterChanged(PLRenderer::Parameter &cParameter);


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<PLRenderer::Parameter&> EventHandlerParameterChanged;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// General
		PLRenderer::Material				*m_pMaterial;			/**< Owner material, always valid! */
		PLRenderer::ProgramGenerator		*m_pProgramGenerator;	/**< Program generator, always valid! */
		PLCore::uint32					 	 m_nRendererFlags;		/**< Used SRPDeferredGBuffer-flags */
		bool								 m_bSynchronized;		/**< Synchronized? */
		// Generated program
		PLRenderer::ProgramGenerator::Flags	 m_cProgramFlags;		/**< Program flags as class member to reduce dynamic memory allocations */
		// Synchronized data
			// Two sided
		bool					   m_bTwoSided;
			// Displacement mapping settings
		float					   m_fDisplacementScale;
		float					   m_fDisplacementBias;
		PLRenderer::Parameter	  *m_pDisplacementMap;
			// Index of refraction and fresnel reflection power
		float					   m_fIndexOfRefraction;
		float					   m_fFresnelReflectionPower;
			// (2D/cube) reflection map
		PLRenderer::Parameter	  *m_pReflectionMap;
		bool					   m_b2DReflectionMap;
			// Figure out whether or not there's reflection on this material
		bool					   m_bReflection;
			// Reflection parameters
		PLRenderer::Parameter	  *m_pReflectivityMap;
		float					   m_fReflectivity;
		PLGraphics::Color3		   m_cReflectionColor;
			// Parallax mapping settings
		float					   m_fParallax;
		PLRenderer::Parameter	  *m_pHeightMap;
			// Glow
		float					   m_fGlowFactor;
		PLRenderer::Parameter	  *m_pGlowMap;
			// Ambient occlusion map
		float					   m_fAmbientOcclusionFactor;
		PLRenderer::Parameter	  *m_pAmbientOcclusionMap;
			// Diffuse map and alpha reference
		PLGraphics::Color3		   m_cDiffuseColor;
		float					   m_fAlphaReference;
		PLRenderer::Parameter	  *m_pDiffuseMap;
			// Specular settings
		bool					   m_bSpecular;
		PLGraphics::Color3		   m_cSpecularColor;
		PLRenderer::Parameter	  *m_pSpecularMap;
		float					   m_fSpecularExponent;
			// Normal map
		PLRenderer::Parameter	  *m_pNormalMap;
		float					   m_fNormalMapBumpiness;
		bool					   m_bNormalMap_DXT5_xGxR;
		bool					   m_bNormalMap_LATC2;
			// Detail normal map
		PLRenderer::Parameter	  *m_pDetailNormalMap;
		float					   m_fDetailNormalMapBumpiness;
		PLMath::Vector2			   m_vDetailNormalMapUVScale;
		bool					   m_bDetailNormalMap_DXT5_xGxR;
		bool					   m_bDetailNormalMap_LATC2;
			// Light map
		PLGraphics::Color3		   m_cLightMapColor;
		PLRenderer::Parameter	  *m_pLightMap;
			// Emissive map
		PLGraphics::Color3		   m_cEmissiveMapColor;
		PLRenderer::Parameter	  *m_pEmissiveMap;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLCOMPOSITING_DEFERRED_GBUFFER_MATERIAL_H__
