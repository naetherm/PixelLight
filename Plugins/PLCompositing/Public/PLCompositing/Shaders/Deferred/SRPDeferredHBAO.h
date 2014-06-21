/*********************************************************\
 *  File: SRPDeferredHBAO.h                              *
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


#ifndef __PLCOMPOSITING_DEFERRED_HBAO_H__
#define __PLCOMPOSITING_DEFERRED_HBAO_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2.h>
#include <PLRenderer/Renderer/ProgramGenerator.h>
#include "PLCompositing/Shaders/Deferred/SRPDeferredSSAO.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class TextureBuffer2D;
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
*    Scene renderer pass for deferred rendering "Horizon Based Ambient Occlusion" (HBAO), a "Screen-Space Ambient Occlusion" (SSAO) technique
*
*  @note
*    - Using "Horizon-Based Ambient Occlusion" as described within NVIDIA Direct3D SDK 10 Code Samples
*      (http://developer.download.nvidia.com/SDK/10.5/direct3d/Source/ScreenSpaceAO/doc/ScreenSpaceAO.pdf)
*/
class SRPDeferredHBAO : public SRPDeferredSSAO {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene renderer pass flags (SceneRendererPass flags extension)
		*/
		enum EFlags {
			NoNormals = 1<<2,	/**< Ignore the per fragment normals when calculating the ambient occlusion (less AO details for heavy normal mapped scenes) */
		};
		pl_enum(EFlags)
			pl_enum_base(SRPDeferredSSAO::EFlags)
			pl_enum_value(NoNormals, "Ignore the per fragment normals when calculating the ambient occlusion (less AO details for heavy normal mapped scenes)")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SRPDeferredHBAO, "PLCompositing", PLCompositing::SRPDeferredSSAO, "Scene renderer pass for deferred rendering 'Horizon Based Ambient Occlusion' (HBAO), a 'Screen-Space Ambient Occlusion' (SSAO) technique")
		// Attributes
		pl_attribute(NumberOfSteps,			PLCore::uint32,			8,								ReadWrite,	DirectValue,	"The maximum number samples per direction",																																														"Min='1'")
		pl_attribute(NumberOfDirections,	PLCore::uint32,			8,								ReadWrite,	DirectValue,	"The number of randomly-rotated 2D directions in image space distributed around the current pixel. The higher this parameter, the lower is the noise in the ambient occlusion.",												"Min='1'")
		pl_attribute(AORadius,				float,					0.25f,							ReadWrite,	DirectValue,	"AO radius in scene metrics. The radius is the distance outside which occludes are ignored.",																																	"Min='0.0'")
		pl_attribute(AngleBias,				float,					30.0f,							ReadWrite,	DirectValue,	"For low-tessellated geometry, occlusion variations tend to appear at creases and ridges, which betray the underlying tessellation. To remove these artifacts, we use an angle bias parameter which restricts the hemisphere.",	"Min='0.0'")
		pl_attribute(Attenuation,			float,					1.0f,							ReadWrite,	DirectValue,	"This scale factor W0 is applied to the per-sample attenuation function. The occlusion contribution of a given sample is attenuated by W0 * W(r/ R) where W(x) = 1 – x2.",														"Min='0.0'")
		pl_attribute(RandomUVScale,			PLMath::Vector2,		PLMath::Vector2(20.0f, 20.f),	ReadWrite,	DirectValue,	"UV scale for the random normals texture",																																														"")
			// Overwritten PLScene::SceneRendererPass attributes
		pl_attribute(Flags,					pl_flag_type(EFlags),	0,								ReadWrite,	GetSet,			"Flags",																																																						"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLCOM_API PLCore::uint32 GetNumberOfDirections() const;
		PLCOM_API void SetNumberOfDirections(PLCore::uint32 nValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCOM_API SRPDeferredHBAO();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SRPDeferredHBAO();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Fragment shader flags, flag names become to source code definitions
		*/
		enum EFragmentShaderFlags {
			FS_NORMAL = 1<<0	/**< Use per fragment normal vector */
		};

		/**
		*  @brief
		*    Direct pointers to uniforms & attributes of a generated program
		*/
		struct GeneratedProgramUserData {
			// Vertex shader attributes
			PLRenderer::ProgramAttribute *pVertexPosition;
			// Fragment shader uniforms
			PLRenderer::ProgramUniform *pNumSteps;
			PLRenderer::ProgramUniform *pNumDir;
			PLRenderer::ProgramUniform *pRadius;
			PLRenderer::ProgramUniform *pInvR;
			PLRenderer::ProgramUniform *pSqrR;
			PLRenderer::ProgramUniform *pAngleBias;
			PLRenderer::ProgramUniform *pTanAngleBias;
			PLRenderer::ProgramUniform *pContrast;
			PLRenderer::ProgramUniform *pAttenuation;
			PLRenderer::ProgramUniform *pFocalLen;
			PLRenderer::ProgramUniform *pInvFocalLen;
			PLRenderer::ProgramUniform *pResolution;
			PLRenderer::ProgramUniform *pInvResolution;
			PLRenderer::ProgramUniform *pRandomUVScale;
			PLRenderer::ProgramUniform *pRandomNormalsMap;
			PLRenderer::ProgramUniform *pNormalDepthMap;
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::uint32						 m_nNumberOfDirections;		/**< The number of randomly-rotated 2D directions in image space distributed around the current pixel. The higher this parameter, the lower is the noise in the ambient occlusion. */
		PLRenderer::TextureBuffer2D			*m_pRandomNormalsTexture;	/**< Texture with random normal vectors, can be a null pointer */
		PLRenderer::ProgramGenerator		*m_pProgramGenerator;		/**< Program generator, can be a null pointer */
		PLRenderer::ProgramGenerator::Flags	 m_cProgramFlags;			/**< Program flags as class member to reduce dynamic memory allocations */


	//[-------------------------------------------------------]
	//[ Protected virtual SRPDeferredSSAO functions           ]
	//[-------------------------------------------------------]
	protected:
		virtual void DrawAO(const PLCore::String &sShaderLanguage, PLRenderer::VertexBuffer &cVertexBuffer, PLRenderer::TextureBufferRectangle &cNormalDepthTextureBuffer) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLCOMPOSITING_DEFERRED_HBAO_H__
