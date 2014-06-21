/*********************************************************\
 *  File: HDRLightAdaptation.h                           *
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


#ifndef __PLCOMPOSITING_HDR_LIGHTADAPTION_H__
#define __PLCOMPOSITING_HDR_LIGHTADAPTION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCompositing/PLCompositing.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
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
*    HDR light adaptation component
*
*  @note
*    - The Pattanaik technique described in http://www.coretechniques.info/PostFX.zip is used
*/
class HDRLightAdaptation {


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
		PLCOM_API HDRLightAdaptation(PLRenderer::Renderer &cRenderer);

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API ~HDRLightAdaptation();

		/**
		*  @brief
		*    Calculates the light adaptation
		*
		*  @param[in] sShaderLanguage
		*    Shader language to use (for example "GLSL" or "Cg"), if empty string, the default shader language of the renderer will be used, don't change the shader language on each call (performance!)
		*  @param[in] cAverageLuminanceTextureBuffer2D
		*    Current average luminance texture buffer
		*  @param[in] fTauTimeDifference
		*    Tau multiplied with the current time difference
		*
		*  @note
		*    - Use GetTextureBuffer() to receive the result of the calculation
		*/
		PLCOM_API void CalculateLightAdaptation(const PLCore::String &sShaderLanguage, PLRenderer::TextureBuffer &cAverageLuminanceTextureBuffer2D, float fTauTimeDifference);

		/**
		*  @brief
		*    Returns the 1x1 2D texture buffer storing the light adaptation
		*
		*  @return
		*    The 1x1 2D texture buffer storing the light adaptation, a null pointer on error
		*
		*  @note
		*    - The texture buffer is only valid after CalculateLightAdaptation() was called
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
		PLRenderer::Renderer			 *m_pRenderer;							/**< Renderer to use, always valid! */
		FullscreenQuad					 *m_pFullscreenQuad;					/**< Fullscreen quad instance, can be a null pointer */
		PLRenderer::VertexShader		 *m_pVertexShader;						/**< Vertex shader, can be a null pointer */
		PLRenderer::FragmentShader		 *m_pFragmentShader;					/**< Fragment shader, can be a null pointer */
		PLRenderer::Program				 *m_pProgram;							/**< GPU program, can be a null pointer */
		PLRenderer::ProgramAttribute	 *m_pPositionProgramAttribute;			/**< Position program attribute, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pFactorProgramUniform;				/**< Factor program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pPreviousTextureProgramUniform;		/**< Previous texture program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pCurrentTextureProgramUniform;		/**< Current texture program uniform, can be a null pointer */
		PLRenderer::SurfaceTextureBuffer *m_pLightAdaptationTextureBuffer2D[2];	/**< 1x1 2D texture buffer storing the previous and current light adaptation, can be a null pointer */
		bool							  m_bPreviousIndex;						/**< Index of the previous 2D light adaptation texture buffer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLCOMPOSITING_HDR_LIGHTADAPTION_H__
