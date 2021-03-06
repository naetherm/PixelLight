/*********************************************************\
 *  File: SRPDeferredSSAO.h                              *
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


#ifndef __PLCOMPOSITING_DEFERRED_SSAO_H__
#define __PLCOMPOSITING_DEFERRED_SSAO_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCompositing/Shaders/Deferred/SRPDeferred.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class Program;
	class VertexBuffer;
	class VertexShader;
	class FragmentShader;
	class ProgramUniform;
	class ProgramAttribute;
	class SurfaceTextureBuffer;
	class TextureBufferRectangle;
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
*    Abstract scene renderer pass for deferred rendering "Screen-Space Ambient Occlusion" (SSAO)
*
*  @note
*    - Overwrites the static ambient occlusion value from GBuffer RT0
*    - Using "Cross Bilateral Filter" as described within NVIDIA Direct3D SDK 10 Code Samples
*      (http://developer.download.nvidia.com/SDK/10.5/direct3d/Source/ScreenSpaceAO/doc/ScreenSpaceAO.pdf)
*/
class SRPDeferredSSAO : public SRPDeferred {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene renderer pass flags (SceneRendererPass flags extension)
		*/
		enum EFlags {
			NoBlur = 1<<1	/**< Do not blur the calculated ambient occlusion */
		};
		pl_flag(EFlags)
			pl_enum_base(SRPDeferred::EFlags)
			pl_enum_value(NoBlur, "Do not blur the calculated ambient occlusion")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PLCOM_API)
		// Attributes
		pl_attribute_directvalue(					ShaderLanguage,		PLCore::String,	"",		ReadWrite)
		pl_attribute_directvalue(					Contrast,			float,			1.25f,	ReadWrite)
		pl_attribute_directvalue(					BlurRadius,			float,			2.0f,	ReadWrite)
		pl_attribute_directvalue(					BlurSharpness,		float,			16.0f,	ReadWrite)
		pl_attribute_directvalue(					ResolutionScale,	float,			0.5f,	ReadWrite)
		pl_attribute_directvalue(					SceneScale,			float,			1.0f,	ReadWrite)
			// Overwritten PLScene::SceneRendererPass attributes
		pl_attribute_getset		(SRPDeferredSSAO,	Flags,				PLCore::uint32,	0,		ReadWrite)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCOM_API SRPDeferredSSAO();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SRPDeferredSSAO();


	//[-------------------------------------------------------]
	//[ Protected virtual SRPDeferredSSAO functions           ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Draws the AO
		*
		*  @param[in] sShaderLanguage
		*    Shader language to use (for example "GLSL" or "Cg"), don't change the shader language on each call (performance!)
		*  @param[in] cVertexBuffer
		*    Vertex buffer of the fullscreen quad
		*  @param[in] cNormalDepthTextureBuffer
		*    RG components of RT1 store the normal vector, B component of RT1 stores the linear view space depth
		*/
		virtual void DrawAO(const PLCore::String &sShaderLanguage, PLRenderer::VertexBuffer &cVertexBuffer, PLRenderer::TextureBufferRectangle &cNormalDepthTextureBuffer) = 0;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Draws the blur
		*
		*  @param[in] sShaderLanguage
		*    Shader language to use (for example "GLSL" or "Cg"), don't change the shader language on each call (performance!)
		*  @param[in] cVertexBuffer
		*    Vertex buffer of the fullscreen quad
		*  @param[in] cInputTextureBuffer
		*    Input texture buffer to apply to blur to
		*  @param[in] cNormalDepthTextureBuffer
		*    RG components of RT1 store the normal vector, B component of RT1 stores the linear view space depth
		*  @param[in] bXBlur
		*    X blur, else it's y blur
		*/
		void DrawBlur(const PLCore::String &sShaderLanguage, PLRenderer::VertexBuffer &cVertexBuffer, PLRenderer::TextureBufferRectangle &cInputTextureBuffer, PLRenderer::TextureBufferRectangle &cNormalDepthTextureBuffer, bool bXBlur);

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
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		PLRenderer::SurfaceTextureBuffer *m_pRenderTargetAO;					/**< Render target catching the result of AO, can be a null pointer */
		PLRenderer::SurfaceTextureBuffer *m_pRenderTargetXBlur;					/**< Render target catching the result of x blur, can be a null pointer */
		PLRenderer::VertexShader		 *m_pVertexShader;						/**< Vertex shader, can be a null pointer */
		PLRenderer::FragmentShader		 *m_pFragmentShader;					/**< Fragment shader, can be a null pointer */
		PLRenderer::Program				 *m_pProgram;							/**< GPU program, can be a null pointer */
		PLRenderer::ProgramAttribute	 *m_pPositionProgramAttribute;			/**< Position program attribute, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pTextureSizeProgramUniform;			/**< Texture size program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pInputTextureSizeProgramUniform;	/**< Input texture size program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pBlurRadiusProgramUniform;			/**< Blur radius program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pBlurFalloffProgramUniform;			/**< Blur falloff program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pSharpnessProgramUniform;			/**< Sharpness program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pUVScaleProgramUniform;				/**< UV scale program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pInputTextureProgramUniform;		/**< Input texture program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pNormalDepthTextureProgramUniform;	/**< Normal depth texture program uniform, can be a null pointer */


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


#endif // __PLCOMPOSITING_DEFERRED_SSAO_H__
