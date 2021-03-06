/*********************************************************\
 *  File: SRPEndFXAA.h                                   *
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


#ifndef __PLCOMPOSITING_FXAA_END_H__
#define __PLCOMPOSITING_FXAA_END_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/ProgramGenerator.h>
#include "PLCompositing/SRPEnd.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class ProgramUniform;
	class ProgramAttribute;
	class SurfaceTextureBuffer;
}
namespace PLCompositing {
	class FullscreenQuad;
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
*    "Fast Approximate Anti-Aliasing" (FXAA) render pipeline finishing scene renderer pass
*
*  @note
*    - Shader based
*    - Using FXAA as described within http://developer.download.nvidia.com/assets/gamedev/files/sdk/11/FXAA_WhitePaper.pdf and
*      "FxaaSamplePack.zip" (https://docs.google.com/leaf?id=0B2manFVVrzQAMzUzYWQ5MTEtYmE1NC00ZTQzLWI2YWYtNTk2MDcyMmY1ZWNm&sort=name&layout=list&num=50&pli=1)
*/
class SRPEndFXAA : public SRPEnd {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    FXAA preset
		*
		*  @note
		*    - Settings and comments are the original ones from http://developer.download.nvidia.com/assets/gamedev/files/sdk/11/FXAA_WhitePaper.pdf
		*/
		enum EPreset {
			Preset0 = 0,	/**< Preset 0 has the highest performance and lowest quality. It simply breaks the aliasing on edges and applies a lower quality sub-pixel aliasing removal filter. This preset is not designed to be practical, but rather to show the lower limits of the tunable parameters. */
			Preset1 = 1,	/**< Preset 1 extends the end-of-edge search radius, adds the high quality sub-pixel aliasing removal filter, and increases the effected local contrast range. This preset is designed for highly performance constrained situations like high resolution mixed with laptop GPUs. */
			Preset2 = 2,	/**< Preset 2 is a good default for high performance but still shows some artifacts do to the end-of-edge search acceleration. This preset decreases the search acceleration, increases the end-of-edge search radius, and increases the effected local contrast range. */
			Preset3 = 3,	/**< Preset 3 is the default, good performance and high quality without artifacts: end-of-edge search acceleration is turned off. */
			Preset4 = 4,	/**< Preset 4 and higher continue to increase the end-of-edge search radius. The improved quality is marginal for the increase in cost. These presets are designed to show the upper limits of the tunable parameters. */
			Preset5 = 5		/**< Preset 5 ultra, see preset 4. */
		};
		pl_enum(EPreset)
			pl_enum_value(Preset0,	"Preset 0 has the highest performance and lowest quality. It simply breaks the aliasing on edges and applies a lower quality sub-pixel aliasing removal filter. This preset is not designed to be practical, but rather to show the lower limits of the tunable parameters.")
			pl_enum_value(Preset1,	"Preset 1 extends the end-of-edge search radius, adds the high quality sub-pixel aliasing removal filter, and increases the effected local contrast range. This preset is designed for highly performance constrained situations like high resolution mixed with laptop GPUs.")
			pl_enum_value(Preset2,	"Preset 2 is a good default for high performance but still shows some artifacts do to the end-of-edge search acceleration. This preset decreases the search acceleration, increases the end-of-edge search radius, and increases the effected local contrast range.")
			pl_enum_value(Preset3,	"Preset 3 is the default, good performance and high quality without artifacts: end-of-edge search acceleration is turned off.")
			pl_enum_value(Preset4,	"Preset 4 and higher continue to increase the end-of-edge search radius. The improved quality is marginal for the increase in cost. These presets are designed to show the upper limits of the tunable parameters.")
			pl_enum_value(Preset5,	"Preset 5 ultra, see preset 4.")
		pl_enum_end

		/**
		*  @brief
		*    Scene renderer pass flags (SceneRendererPass flags extension)
		*/
		enum EFlags {
			WriteToOriginalSurface = 1<<1,	/**< The result is drawn into the given original render surface (for example a render window) and there's no further need for an additional SRPEnd instance, if not set, the current render pipeline is continued */
		};
		pl_flag(EFlags)
			pl_enum_base(SRPEnd::EFlags)
			pl_enum_value(WriteToOriginalSurface,	"The result is drawn into the given original render surface (for example a render window) and there's no further need for an additional SRPEnd instance, if not set, the current render pipeline is continued")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PLCOM_API)
		// Attributes
		pl_attribute_directvalue(				ShaderLanguage,	PLCore::String,	"",			ReadWrite)
		pl_attribute_directvalue(				Preset,			EPreset,		Preset3,	ReadWrite)
			// Overwritten PLScene::SceneRendererPass attributes
		pl_attribute_getset		(SRPEndFXAA,	Flags,			PLCore::uint32,	0,			ReadWrite)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCOM_API SRPEndFXAA();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SRPEndFXAA();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Direct pointers to uniforms & attributes of a generated program
		*/
		struct GeneratedProgramUserData {
			// Vertex shader attributes
			PLRenderer::ProgramAttribute *pVertexPosition;
			// Fragment shader uniforms
			PLRenderer::ProgramUniform *pTexture;
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLRenderer::SurfaceTextureBuffer	*m_pRenderTarget;		/**< Render target, can be a null pointer */
		FullscreenQuad						*m_pFullscreenQuad;		/**< Fullscreen quad instance, can be a null pointer */
		PLRenderer::ProgramGenerator		*m_pProgramGenerator;	/**< Program generator, can be a null pointer */
		PLRenderer::ProgramGenerator::Flags	 m_cProgramFlags;		/**< Program flags as class member to reduce dynamic memory allocations */
		EPreset								 m_nCurrentPreset;		/**< Preset the current program generator us using */


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


#endif // __PLCOMPOSITING_FXAA_END_H__
