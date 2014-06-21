/*********************************************************\
 *  File: SRPDeferredGodRays.h                           *
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


#ifndef __PLCOMPOSITING_DEFERRED_GODRAYS_H__
#define __PLCOMPOSITING_DEFERRED_GODRAYS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2.h>
#include <PLGraphics/Color/Color3.h>
#include <PLRenderer/Renderer/ProgramGenerator.h>
#include "PLCompositing/Shaders/Deferred/SRPDeferred.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class ProgramUniform;
	class ProgramAttribute;
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
*    Scene renderer pass for deferred rendering god rays effect (volumetric light scattering as a post-process)
*
*  @note
*    - The technique described in http://http.developer.nvidia.com/GPUGems3/gpugems3_ch13.html is used
*    - The content of the emissive/light map render target of the GBuffer is used
*/
class SRPDeferredGodRays : public SRPDeferred {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene renderer pass flags (SceneRendererPass flags extension)
		*/
		enum EFlags {
			NoDiscard  = 1<<1,	/**< Disable discard (may result in better or worse performance) */
			NoBlending = 1<<2	/**< Disable blending (for debugging) */
		};
		pl_enum(EFlags)
			pl_enum_base(SRPDeferred::EFlags)
			pl_enum_value(NoDiscard,	"Disable discard (may result in better or worse performance)")
			pl_enum_value(NoBlending,	"Disable blending (for debugging)")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SRPDeferredGodRays, "PLCompositing", PLCompositing::SRPDeferred, "Scene renderer pass for deferred rendering god rays effect (volumetric light scattering as a post-process)")
		// Attributes
		pl_attribute(ShaderLanguage,	PLCore::String,			"",											ReadWrite,	DirectValue,	"Shader language to use (for example \"GLSL\" or \"Cg\"), if empty string, the default shader language of the renderer will be used",	"")
		pl_attribute(NumberOfSamples,	PLCore::uint32,			20,											ReadWrite,	DirectValue,	"Number of samples, higher is better but costs more performance",																		"")
		pl_attribute(Density,			float,					0.3f,										ReadWrite,	DirectValue,	"Density",																																"")
		pl_attribute(Weight,			float,					0.2f,										ReadWrite,	DirectValue,	"The weight constant to influences the brightness",																						"")
		pl_attribute(Decay,				float,					0.9f,										ReadWrite,	DirectValue,	"Exponential decay attenuation coefficient",																							"")
		pl_attribute(LightPosition,		PLMath::Vector2,		PLMath::Vector2(0.5f, 0.5f),				ReadWrite,	DirectValue,	"Light position on screen, lower/left is (0,0) and upper/right is (1,1)",																"")
		pl_attribute(Color,				PLGraphics::Color3,		PLGraphics::Color3(0.15f, 0.15f, 0.15f),	ReadWrite,	DirectValue,	"God rays color",																														"")
			// Overwritten PLScene::SceneRendererPass attributes
		pl_attribute(Flags,				pl_flag_type(EFlags),	0,											ReadWrite,	GetSet,			"Flags",																																"")
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
		PLCOM_API SRPDeferredGodRays();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SRPDeferredGodRays();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Fragment shader flags, flag names become to source code definitions
		*/
		enum EFragmentShaderFlags {
			FS_DISCARD = 1<<0	/**< Use discard */
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
			// Fragment shader uniforms
			PLRenderer::ProgramUniform *pNumberOfSamples;
			PLRenderer::ProgramUniform *pDensity;
			PLRenderer::ProgramUniform *pWeight;
			PLRenderer::ProgramUniform *pDecay;
			PLRenderer::ProgramUniform *pLightPosition;
			PLRenderer::ProgramUniform *pColor;
			PLRenderer::ProgramUniform *pMap;
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLRenderer::ProgramGenerator		*m_pProgramGenerator;	/**< Program generator, can be a null pointer */
		PLRenderer::ProgramGenerator::Flags	 m_cProgramFlags;		/**< Program flags as class member to reduce dynamic memory allocations */


	//[-------------------------------------------------------]
	//[ Private virtual PLScene::SceneRendererPass functions  ]
	//[-------------------------------------------------------]
	private:
		virtual void Draw(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLCOMPOSITING_DEFERRED_GODRAYS_H__
