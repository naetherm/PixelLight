/*********************************************************\
 *  File: SRPDeferredDOF.h                               *
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


#ifndef __PLCOMPOSITING_DEFERRED_DOF_H__
#define __PLCOMPOSITING_DEFERRED_DOF_H__
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
	class TextureBuffer;
	class ProgramUniform;
	class FragmentShader;
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
*    Scene renderer pass for deferred rendering DOF effect
*
*  @note
*    - The technique described within \url{http://ati.amd.com/developer/gdc/Scheuermann_DepthOfField.pdf} is used
*    - Can also be controlled by a "PLCompositing::SNMPostProcessDepthOfField" scene node modifier attached to the used camera
*/
class SRPDeferredDOF : public SRPDeferred {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene renderer pass flags (SceneRendererPass flags extension)
		*/
		enum EFlags {
			IgnoreCameraSettings = 1<<1,	/**< Don't use SNMPostProcessDepthOfField settings of the camera scene node */
			DebugDepthBlur       = 1<<2,	/**< Debug depth blur */
			DebugBlur            = 1<<3		/**< Debug blur */
		};
		pl_flag(EFlags)
			pl_enum_base(SRPDeferred::EFlags)
			pl_enum_value(IgnoreCameraSettings,	"Don't use SNMPostProcessDepthOfField settings of the camera scene node")
			pl_enum_value(DebugDepthBlur,		"Debug depth blur")
			pl_enum_value(DebugBlur,			"Debug blur")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PLCOM_API)
		// Attributes
		pl_attribute_directvalue(					ShaderLanguage,		PLCore::String,	"",		ReadWrite)
		pl_attribute_directvalue(					EffectWeight,		float,			1.0f,	ReadWrite)
		pl_attribute_directvalue(					NearPlaneDepth,		float,			0.5f,	ReadWrite)
		pl_attribute_directvalue(					FocalPlaneDepth,	float,			5.0f,	ReadWrite)
		pl_attribute_directvalue(					FarPlaneDepth,		float,			10.0f,	ReadWrite)
		pl_attribute_directvalue(					BlurrinessCutoff,	float,			0.8f,	ReadWrite)
		pl_attribute_directvalue(					BlurPasses,			PLCore::uint32,	2,		ReadWrite)
		pl_attribute_directvalue(					BlurDownscale,		float,			4.0f,	ReadWrite)
			// Overwritten PLScene::SceneRendererPass attributes
		pl_attribute_getset		(SRPDeferredDOF,	Flags,				PLCore::uint32,	0,		ReadWrite)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCOM_API SRPDeferredDOF();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SRPDeferredDOF();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Calculates the depth blur
		*
		*  @param[in] sShaderLanguage
		*    Shader language to use (for example "GLSL" or "Cg"), don't change the shader language on each call (performance!)
		*  @param[in] cVertexBuffer
		*    Vertex buffer of the fullscreen quad
		*  @param[in] cRGBTexture
		*    Texture buffer containing RGB
		*  @param[in] cNormalDepthTexture
		*    Normal depth texture buffer
		*  @param[in] fNearPlaneDepth
		*    Near plane depth
		*  @param[in] fFocalPlaneDepth
		*    Focal plane depth
		*  @param[in] fFarPlaneDepth
		*    Far plane depth
		*  @param[in] fBlurrinessCutoff
		*    Blurriness cutoff
		*/
		void CalculateDepthBlur(const PLCore::String &sShaderLanguage, PLRenderer::VertexBuffer &cVertexBuffer, PLRenderer::TextureBufferRectangle &cRGBTexture,
								PLRenderer::TextureBufferRectangle &cNormalDepthTexture, float fNearPlaneDepth, float fFocalPlaneDepth, float fFarPlaneDepth, float fBlurrinessCutoff);

		/**
		*  @brief
		*    Calculates the blur
		*
		*  @param[in] sShaderLanguage
		*    Shader language to use (for example "GLSL" or "Cg"), don't change the shader language on each call (performance!)
		*  @param[in] cVertexBuffer
		*    Vertex buffer of the fullscreen quad
		*  @param[in] cOriginalTexture
		*    Original HDR texture buffer to calculate the blur from
		*  @param[in] fBrightThreshold
		*    Bright threshold
		*  @param[in] nBlurPasses
		*    Number of blur passes
		*  @param[in] fDownscale
		*    Downscale factor
		*
		*  @note
		*    - Use GetBlurTextureBuffer() to receive the result of the calculation
		*/
		void CalculateBlur(const PLCore::String &sShaderLanguage, PLRenderer::VertexBuffer &cVertexBuffer, PLRenderer::TextureBufferRectangle &cOriginalTexture, float fBrightThreshold, PLCore::uint32 nBlurPasses, float fDownscale);

		/**
		*  @brief
		*    Debugs data
		*
		*  @param[in] sShaderLanguage
		*    Shader language to use (for example "GLSL" or "Cg"), don't change the shader language on each call (performance!)
		*  @param[in] cVertexBuffer
		*    Vertex buffer of the fullscreen quad
		*  @param[in] cTexture
		*    Texture buffer containing the data
		*  @param[in] nType
		*    0 for depth blur, 1 for blur
		*/
		void Debug(const PLCore::String &sShaderLanguage, PLRenderer::VertexBuffer &cVertexBuffer, PLRenderer::TextureBufferRectangle &cTexture, PLCore::uint32 nType);

		/**
		*  @brief
		*    Returns the rectangle texture buffer storing the blur
		*
		*  @return
		*    The rectangle texture buffer storing the blur, a null pointer on error
		*
		*  @note
		*    - The texture buffer is only valid after CalculateBlur() was called
		*/
		PLRenderer::TextureBuffer *GetBlurTextureBuffer() const;

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
		PLRenderer::SurfaceTextureBuffer *m_pRenderTarget[2];							/**< Render targets, can be a null pointer */
		bool							  m_bResultIndex;								/**< Index of the result texture buffer */
		PLRenderer::VertexShader		 *m_pVertexShader;								/**< Vertex shader, can be a null pointer */
		// Depth blur
		PLRenderer::FragmentShader		 *m_pDepthBlurFragmentShader;					/**< Depth blur fragment shader, can be a null pointer */
		PLRenderer::Program				 *m_pDepthBlurProgram;							/**< Depth blur GPU program, can be a null pointer */
		PLRenderer::ProgramAttribute	 *m_pDepthBlurPositionProgramAttribute;			/**< Depth blur position program attribute, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pDepthBlurTextureSizeProgramUniform;		/**< Depth blur texture size program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pDepthBlurDOFParamsProgramUniform;			/**< Depth blur DOF params program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pDepthBlurRGBTextureProgramUniform;			/**< Depth blur RGB texture program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pDepthBlurNormalDepthTextureProgramUniform;	/**< Depth blur normal depth texture program uniform, can be a null pointer */
		// Downscale
		PLRenderer::FragmentShader		 *m_pDownscaleFragmentShader;					/**< Downscale fragment shader, can be a null pointer */
		PLRenderer::Program				 *m_pDownscaleProgram;							/**< Downscale GPU program, can be a null pointer */
		PLRenderer::ProgramAttribute	 *m_pDownscalePositionProgramAttribute;			/**< Downscale position program attribute, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pDownscaleTextureSizeProgramUniform;		/**< Downscale texture size program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pDownscaleTextureProgramUniform;			/**< Downscale texture program uniform, can be a null pointer */
		// Blur
		PLRenderer::FragmentShader		 *m_pBlurFragmentShader;						/**< Blur fragment shader, can be a null pointer */
		PLRenderer::Program				 *m_pBlurProgram;								/**< Blur GPU program, can be a null pointer */
		PLRenderer::ProgramAttribute	 *m_pBlurPositionProgramAttribute;				/**< Blur position program attribute, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pBlurTextureSizeProgramUniform;				/**< Blur texture size program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pBlurUVScaleProgramUniform;					/**< Blur uv scale program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pBlurTextureProgramUniform;					/**< Blur texture program uniform, can be a null pointer */
		// Result
		PLRenderer::FragmentShader		 *m_pResultFragmentShader;						/**< Result fragment shader, can be a null pointer */
		PLRenderer::Program				 *m_pResultProgram;								/**< Result GPU program, can be a null pointer */
		PLRenderer::ProgramAttribute	 *m_pResultPositionProgramAttribute;			/**< Result position program attribute, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pResultTextureSizeProgramUniform;			/**< Result texture size program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pResultEffectWeightProgramUniform;			/**< Result effect weight program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pResultBlurDownscaleProgramUniform;			/**< Result blur downscale program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pResultBlurTextureProgramUniform;			/**< Result blur texture program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pResultTextureProgramUniform;				/**< Result texture program uniform, can be a null pointer */
		// Debug
		PLRenderer::FragmentShader		 *m_pDebugFragmentShader;						/**< Debug fragment shader, can be a null pointer */
		PLRenderer::Program				 *m_pDebugProgram;								/**< Debug GPU program, can be a null pointer */
		PLRenderer::ProgramAttribute	 *m_pDebugPositionProgramAttribute;				/**< Debug position program attribute, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pDebugTextureSizeProgramUniform;			/**< Debug texture size program uniform, can be a null pointer */
		PLRenderer::ProgramUniform		 *m_pDebugTextureProgramUniform;				/**< Debug texture program uniform, can be a null pointer */
		PLCore::uint32					  m_nDebugType;									/**< Current debug type we have a generated GPU program for */


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


#endif // __PLCOMPOSITING_DEFERRED_DOF_H__
