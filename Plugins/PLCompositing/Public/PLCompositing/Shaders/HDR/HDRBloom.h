/*********************************************************\
 *  File: HDRBloom.h                                     *
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


#ifndef __PLCOMPOSITING_HDR_BLOOM_H__
#define __PLCOMPOSITING_HDR_BLOOM_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/ProgramGenerator.h>
#include "PLCompositing/PLCompositing.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLGraphics {
	class Color3;
}
namespace PLRenderer {
	class Program;
	class VertexShader;
	class TextureBuffer;
	class FragmentShader;
	class ProgramUniform;
	class TextureBuffer2D;
	class ProgramAttribute;
	class SurfaceTextureBuffer;
	class TextureBufferRectangle;
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
*    HDR bloom calculation component
*/
class HDRBloom {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*/
		PLCOM_API HDRBloom(PLRenderer::Renderer &cRenderer);

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API ~HDRBloom();

		/**
		*  @brief
		*    Calculates the bloom
		*
		*  @param[in] sShaderLanguage
		*    Shader language to use (for example "GLSL" or "Cg"), if empty string, the default shader language of the renderer will be used, don't change the shader language on each call (performance!)
		*  @param[in] cOriginalTexture
		*    Original HDR texture buffer to calculate the bloom from
		*  @param[in] fBrightThreshold
		*    Bright threshold
		*  @param[in] bToneMapping
		*    Perform tone mapping
		*  @param[in] bAutomaticAverageLuminance
		*    Automatic average luminance
		*  @param[in] cLuminanceConvert
		*    Luminance convert
		*  @param[in] fKey
		*    Key
		*  @param[in] fWhiteLevel
		*    White level
		*  @param[in] fAverageLuminance
		*    Average luminance
		*  @param[in] pHDRAverageLuminanceTextureBuffer
		*    Average luminance texture buffer, can be a null pointer
		*  @param[in] nBloomBlurPasses
		*    Number of blur passes
		*  @param[in] fDownscale
		*    Downscale factor
		*  @param[in] bUseVertexTextureFetch
		*    Allow the usage of vertex texture fetch (VTF) - results in usually slightly better performance but there may be driver issues
		*
		*  @note
		*    - Use GetTextureBuffer() to receive the result of the calculation
		*/
		PLCOM_API void CalculateBloom(const PLCore::String &sShaderLanguage, PLRenderer::TextureBufferRectangle &cOriginalTexture, float fBrightThreshold, bool bToneMapping, bool bAutomaticAverageLuminance, const PLGraphics::Color3 &cLuminanceConvert,
									  float fKey, float fWhiteLevel, float fAverageLuminance, PLRenderer::TextureBuffer *pHDRAverageLuminanceTextureBuffer, PLCore::uint32 nBloomBlurPasses, float fDownscale, bool bUseVertexTextureFetch);

		/**
		*  @brief
		*    Returns the rectangle texture buffer storing the bloom
		*
		*  @return
		*    The rectangle texture buffer storing the bloom, a null pointer on error
		*
		*  @note
		*    - The texture buffer is only valid after CalculateBloom() was called
		*/
		PLCOM_API PLRenderer::TextureBuffer *GetTextureBuffer() const;


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
			PLRenderer::ProgramUniform *pBrightThreshold;
			PLRenderer::ProgramUniform *pHDRTexture;
		};


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when a program became dirty
		*
		*  @param[in] pProgram
		*    Program which became dirty
		*/
		void OnDirty(PLRenderer::Program *pProgram);


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<PLRenderer::Program*> EventHandlerDirty;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLRenderer::Renderer				*m_pRenderer;						/**< Renderer to use, always valid! */
		FullscreenQuad						*m_pFullscreenQuad;					/**< Fullscreen quad instance, can be a null pointer */
		PLRenderer::SurfaceTextureBuffer	*m_pRenderTarget[2];				/**< Render targets, can be a null pointer */
		bool								 m_bResultIndex;					/**< Index of the result texture buffer */
		PLRenderer::ProgramGenerator		*m_pDownscaleProgramGenerator;		/**< Downscale program generator, can be a null pointer */
		PLRenderer::ProgramGenerator::Flags	 m_cDownscaleProgramFlags;			/**< Downscale program flags as class member to reduce dynamic memory allocations */
		PLRenderer::VertexShader			*m_pBloomVertexShader;				/**< Bloom vertex shader, can be a null pointer */
		PLRenderer::FragmentShader			*m_pBloomFragmentShader;			/**< Bloom fragment shader, can be a null pointer */
		PLRenderer::Program					*m_pBloomProgram;					/**< Bloom GPU program, can be a null pointer */
		PLRenderer::ProgramAttribute		*m_pBloomPositionProgramAttribute;	/**< Bloom position program attribute, can be a null pointer */
		PLRenderer::ProgramUniform			*m_pBloomTextureSizeProgramUniform;	/**< Bloom texture size program uniform, can be a null pointer */
		PLRenderer::ProgramUniform			*m_pBloomUVScaleProgramUniform;		/**< Bloom uv scale program uniform, can be a null pointer */
		PLRenderer::ProgramUniform			*m_pBloomHDRTextureProgramUniform;	/**< Bloom HDR texture program uniform, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLCOMPOSITING_HDR_BLOOM_H__
