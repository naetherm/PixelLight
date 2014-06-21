/*********************************************************\
 *  File: HDRAverageLuminance.h                          *
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


#ifndef __PLCOMPOSITING_HDR_AVERAGELUMINANCE_H__
#define __PLCOMPOSITING_HDR_AVERAGELUMINANCE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
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
	class ProgramUniform;
	class FragmentShader;
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
*    HDR logarithmic average luminance calculation component
*
*  @note
*    - The technique described in http://developer.download.nvidia.com/SDK/9.5/Samples/DEMOS/Direct3D9/HDR_FP16x2.zip is used
*/
class HDRAverageLuminance {


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
		PLCOM_API HDRAverageLuminance(PLRenderer::Renderer &cRenderer);

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API ~HDRAverageLuminance();

		/**
		*  @brief
		*    Calculates the logarithmic average luminance
		*
		*  @param[in] sShaderLanguage
		*    Shader language to use (for example "GLSL" or "Cg"), if empty string, the default shader language of the renderer will be used, don't change the shader language on each call (performance!)
		*  @param[in] cOriginalTexture
		*    Original HDR texture buffer to calculate the logarithmic average luminance from
		*  @param[in] cLuminanceConvert
		*    Luminance convert
		*
		*  @note
		*    - Use GetTextureBuffer() to receive the result of the calculation
		*/
		PLCOM_API void CalculateAverageLuminance(const PLCore::String &sShaderLanguage, PLRenderer::TextureBufferRectangle &cOriginalTexture, const PLGraphics::Color3 &cLuminanceConvert);

		/**
		*  @brief
		*    Returns the 1x1 2D texture buffer storing the logarithmic average luminance
		*
		*  @return
		*    The 1x1 2D texture buffer storing the logarithmic average luminance, a null pointer on error
		*
		*  @note
		*    - The texture buffer is only valid after CalculateAverageLuminance() was called
		*/
		PLCOM_API PLRenderer::TextureBuffer *GetTextureBuffer() const;


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
		PLRenderer::Renderer			 *m_pRenderer;										/**< Renderer to use, always valid! */
		FullscreenQuad					 *m_pFullscreenQuad;								/**< Fullscreen quad instance, can be a null pointer */
		PLRenderer::VertexShader		 *m_pVertexShader;									/**< Vertex shader, can be a null pointer */
		// Downsample 2x2, calculate pixel luminance and log
		PLRenderer::FragmentShader		 *m_pDownsampleLogFragmentShader;					/**< Downsample 2x2 log fragment shader, can be a null pointer */
		PLRenderer::Program				 *m_pDownsampleLogProgram;							/**< Downsample 2x2 log GPU program, can be a null pointer */
		PLRenderer::ProgramAttribute	 *m_pDownsampleLogPositionProgramAttribute;			/**< Downsample 2x2 log position program attribute, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pDownsampleLogTextureSizeProgramUniform;		/**< Downsample 2x2 log texture size program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pDownsampleLogTextureProgramUniform;			/**< Downsample 2x2 log texture program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pDownsampleLogLuminanceConvertProgramUniform;	/**< Downsample 2x2 log luminance convert program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pDownsampleLogEpsilonProgramUniform;			/**< Downsample 2x2 log epsilon program uniform, can be a null pointer */
		PLRenderer::SurfaceTextureBuffer *m_pDownsampleLogRenderTarget;						/**< Downsample 2x2 log render target, can be a null pointer */
		// Downsample 4x4
		PLRenderer::VertexShader		 *m_pDownsampleVertexShader;						/**< Downsample vertex shader, can be a null pointer */
		PLRenderer::FragmentShader		 *m_pDownsampleFragmentShader;						/**< Downsample 4x4 fragment shader, can be a null pointer */
		PLRenderer::Program				 *m_pDownsampleProgram;								/**< Downsample 4x4 GPU program, can be a null pointer */
		PLRenderer::ProgramAttribute	 *m_pDownsamplePositionProgramAttribute;			/**< Downsample 4x4 position program attribute, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pDownsampleTextureSizeProgramUniform;			/**< Downsample 4x4 texture size program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pDownsampleSizeProgramUniform;					/**< Downsample 4x4 size program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pDownsampleTextureProgramUniform;				/**< Downsample 4x4 texture program uniform, can be a null pointer */
		PLRenderer::SurfaceTextureBuffer *m_pDownsampleRenderTarget;						/**< Downsample 4x4 render target, can be a null pointer */
		// Reduce <4>x<4> to <1>x<1> and calculate the exponent
		PLRenderer::FragmentShader		 *m_pDownsampleExpFragmentShader;					/**< Downsample 4x4 exp fragment shader, can be a null pointer */
		PLRenderer::Program				 *m_pDownsampleExpProgram;							/**< Downsample 4x4 exp GPU program, can be a null pointer */
		PLRenderer::ProgramAttribute	 *m_pDownsampleExpPositionProgramAttribute;			/**< Downsample 4x4 exp position program attribute, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pDownsampleExpTextureSizeProgramUniform;		/**< Downsample 4x4 exp texture size program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pDownsampleExpTextureProgramUniform;			/**< Downsample 4x4 exp texture program uniform, can be a null pointer */
		PLRenderer::SurfaceTextureBuffer *m_pAverageLuminanceTextureBuffer2D;				/**< 1x1 2D texture buffer storing the logarithmic average luminance, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLCOMPOSITING_HDR_AVERAGELUMINANCE_H__
