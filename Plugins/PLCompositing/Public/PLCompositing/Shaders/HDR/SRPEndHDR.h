/*********************************************************\
 *  File: SRPEndHDR.h                                    *
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


#ifndef __PLCOMPOSITING_HDR_END_H__
#define __PLCOMPOSITING_HDR_END_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGraphics/Color/Color3.h>
#include <PLRenderer/Renderer/ProgramGenerator.h>
#include "PLCompositing/SRPEnd.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class ProgramUniform;
	class ProgramAttribute;
}
namespace PLCompositing {
	class HDRBloom;
	class FullscreenQuad;
	class HDRLightAdaptation;
	class HDRAverageLuminance;
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
*    HDR render pipeline finishing scene renderer pass
*
*  @note
*    - This scene renderer pass should be at the very bottom of the HDR scene render pipeline,
*      but it's not required that it's at the very bottom of the whole scene render pipeline
*    - Shader based
*    - Using Reinhard tone mapping as described within http://www.cs.ucf.edu/~reinhard/cdrom/
*    - HDR logarithmic average luminance calculation
*    - HDR light adaptation
*    - HDR bloom effect
*    - Performs gamma correction (linear space -> sRGB space)
*/
class SRPEndHDR : public SRPEnd {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene renderer pass flags (SceneRendererPass flags extension)
		*/
		enum EFlags {
			NoToneMapping               = 1<<1,	/**< Do not perform tone mapping */
			NoAutomaticAverageLuminance = 1<<2,	/**< Do not calculate the average luminance automatically (used for tone mapping) */
			NoLightAdaptation           = 1<<3,	/**< Do not perform light adaptation (used for tone mapping with automatically calculated average luminance) */
			NoBloom                     = 1<<4,	/**< Do not add bloom */
			NoGammaCorrection           = 1<<5,	/**< Do not perform gamma correction */
			WriteToOriginalSurface      = 1<<6,	/**< The result is drawn into the given original render surface (for example a render window) and there's no further need for an additional SRPEnd instance, if not set, the current render pipeline is continued */
			UseVertexTextureFetch       = 1<<7,	/**< Allow the usage of vertex texture fetch (VTF) - results in usually slightly better performance but there may be driver issues */
			DebugShowBloomTexture       = 1<<8,	/**< Show the bloom texture (for debugging) */
			DebugCheckAndFixNANValues   = 1<<9	/**< Check for NAN and fix NAN values by using a red color (for debugging, SLOW!) */
		};
		pl_enum(EFlags)
			pl_enum_base(SRPEnd::EFlags)
			pl_enum_value(NoToneMapping,				"Do not perform tone mapping")
			pl_enum_value(NoAutomaticAverageLuminance,	"Do not calculate the average luminance automatically (used for tone mapping)")
			pl_enum_value(NoLightAdaptation,			"Do not perform light adaptation (used for tone mapping with automatically calculated average luminance)")
			pl_enum_value(NoBloom,						"Do not add bloom")
			pl_enum_value(NoGammaCorrection,			"Do not perform gamma correction")
			pl_enum_value(WriteToOriginalSurface,		"The result is drawn into the given original render surface (for example a render window) and there's no further need for an additional SRPEnd instance, if not set, the current render pipeline is continued")
			pl_enum_value(UseVertexTextureFetch,		"Allow the usage of vertex texture fetch (VTF) - results in usually slightly better performance but there may be driver issues")
			pl_enum_value(DebugShowBloomTexture,		"Show the bloom texture (for debugging)")
			pl_enum_value(DebugCheckAndFixNANValues,	"Check for NAN and fix NAN values by using a red color (for debugging, SLOW!)")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SRPEndHDR, "PLCompositing", PLCompositing::SRPEnd, "HDR render pipeline finishing scene renderer pass")
		// Attributes
		pl_attribute(ShaderLanguage,		PLCore::String,			"",												ReadWrite,	DirectValue,	"Shader language to use (for example \"GLSL\" or \"Cg\"), if empty string, the default shader language of the renderer will be used",		"")
		pl_attribute(LuminanceConvert,		PLGraphics::Color3,		PLGraphics::Color3(0.2125f, 0.7154f, 0.0721f),	ReadWrite,	DirectValue,	"Luminance convert (tone mapping)",																											"")
		pl_attribute(Key,					float,					0.72f,											ReadWrite,	DirectValue,	"Key value, midzone luminance (tone mapping)",																								"")
		pl_attribute(WhiteLevel,			float,					100.0f,											ReadWrite,	DirectValue,	"The smallest luminance that will be mapped to pure white, values above 'burn out' (tone mapping)",											"")
		pl_attribute(AverageLuminance,		float,					0.02f,											ReadWrite,	DirectValue,	"User set average luminance (tone mapping, only used if NoAutomaticAverageLuminance is set)",												"")
		pl_attribute(Tau,					float,					0.5f,											ReadWrite,	DirectValue,	"Light adaptation 'tau', lower value for longer light adaption time (tone mapping, only used if NoAutomaticAverageLuminance is not set)",	"")
		pl_attribute(BloomBrightThreshold,	float,					0.5f,											ReadWrite,	DirectValue,	"Bloom bright threshold, only color components >= this value bloom (only used if NoBloom is not set)",										"")
		pl_attribute(BloomFactor,			float,					1.0f,											ReadWrite,	DirectValue,	"Bloom factor (only used if NoBloom is not set)",																							"")
		pl_attribute(BloomBlurPasses,		PLCore::uint32,			4,												ReadWrite,	DirectValue,	"Number of bloom blur passes, should be a multiple of 2 (only used if NoBloom is not set)",													"")
		pl_attribute(BloomDownscale,		float,					8.0f,											ReadWrite,	DirectValue,	"Bloom downscale factor, should be a multiple of 2 (only used if NoBloom is not set)",														"Min='1'")
		pl_attribute(Gamma,					float,					2.2f,											ReadWrite,	DirectValue,	"Gamma correction value, higher values lighten the image, smaller values darken the image",													"")
			// Overwritten PLScene::SceneRendererPass attributes
		pl_attribute(Flags,					pl_flag_type(EFlags),	0,												ReadWrite,	GetSet,			"Flags",																																	"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCOM_API SRPEndHDR();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SRPEndHDR();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Vertex shader flags, flag names become to source code definitions
		*/
		enum EVertexShaderFlags {
			VS_AUTOMATIC_AVERAGE_LUMINANCE_VTF = 1<<0	/**< Use automatic average luminance through vertex texture fetch (VTF) - results in usually slightly better performance */
		};

		/**
		*  @brief
		*    Fragment shader flags, flag names become to source code definitions
		*/
		enum EFragmentShaderFlags {
			FS_TONE_MAPPING							   = 1<<0,	/**< Perform tone mapping */
				FS_AUTOMATIC_AVERAGE_LUMINANCE		   = 1<<1,	/**< Use automatic average luminance through (FS_TONE_MAPPING must be set, too) */
					FS_AUTOMATIC_AVERAGE_LUMINANCE_VTF = 1<<2,	/**< Use automatic average luminance through vertex texture fetch (VTF) - results in usually slightly better performance (FS_AUTOMATIC_AVERAGE_LUMINANCE must be set, too) */
			FS_BLOOM								   = 1<<3,	/**< Add bloom */
			FS_GAMMA_CORRECTION						   = 1<<4	/**< Perform gamma correction */
		};

		/**
		*  @brief
		*    Direct pointers to uniforms & attributes of a generated program
		*/
		struct GeneratedProgramUserData {
			// Vertex shader attributes
			PLRenderer::ProgramAttribute *pVertexPosition;
			// Vertex shader uniforms
			PLRenderer::ProgramUniform *pTextureSize;
			PLRenderer::ProgramUniform *pAverageLuminanceTexture;
			// Fragment shader uniforms
			PLRenderer::ProgramUniform *pLuminanceConvert;
			PLRenderer::ProgramUniform *pKey;
			PLRenderer::ProgramUniform *pWhiteLevel;
			PLRenderer::ProgramUniform *pAverageLuminance;
			PLRenderer::ProgramUniform *pBloomFactor;
			PLRenderer::ProgramUniform *pBloomDownscale;
			PLRenderer::ProgramUniform *pBloomTexture;
			PLRenderer::ProgramUniform *pInvGamma;
			PLRenderer::ProgramUniform *pHDRTexture;
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		FullscreenQuad						*m_pFullscreenQuad;			/**< Fullscreen quad instance, can be a null pointer */
		HDRAverageLuminance					*m_pHDRAverageLuminance;	/**< HDR logarithmic average luminance calculation component, can be a null pointer */
		HDRLightAdaptation					*m_pHDRLightAdaptation;		/**< HDR light adaptation calculation component, can be a null pointer */
		HDRBloom							*m_pHDRBloom;				/**< HDR bloom calculation component, can be a null pointer */
		PLRenderer::ProgramGenerator		*m_pProgramGenerator;		/**< Program generator, can be a null pointer */
		PLRenderer::ProgramGenerator::Flags	 m_cProgramFlags;			/**< Program flags as class member to reduce dynamic memory allocations */


	//[-------------------------------------------------------]
	//[ Protected virtual PLScene::SceneRendererPass functions ]
	//[-------------------------------------------------------]
	protected:
		virtual void Draw(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLCOMPOSITING_HDR_END_H__
