/*********************************************************\
 *  File: SRPDeferredSPAAO.h                             *
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


#ifndef __PLCOMPOSITING_DEFERRED_SPAAO_H__
#define __PLCOMPOSITING_DEFERRED_SPAAO_H__
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
*    Scene renderer pass for deferred rendering "A Simple and Practical Approach to SSAO" (SPAAO), a "Screen-Space Ambient Occlusion" (SSAO) technique
*
*  @note
*    - Using "A Simple and Practical Approach to SSAO" as described at http://www.gamedev.net/page/reference/index.html/_/reference/programming/140/lighting-and-shading/a-simple-and-practical-approach-to-ssao-r2753
*/
class SRPDeferredSPAAO : public SRPDeferredSSAO {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SRPDeferredSPAAO, "PLCompositing", PLCompositing::SRPDeferredSSAO, "Scene renderer pass for deferred rendering 'A Simple and Practical Approach to SSAO' (SPAAO), a 'Screen-Space Ambient Occlusion' (SSAO) technique")
		// Attributes
		pl_attribute(NumberOfDirections,	PLCore::uint32,			8,		ReadWrite,	DirectValue,	"The number of randomly-rotated 2D directions in image space distributed around the current pixel. The higher this parameter, the lower is the noise in the ambient occlusion.",	"Min='1'")
		pl_attribute(SampleRadius,			float,					0.1f,	ReadWrite,	DirectValue,	"The sampling radius",																																								"Min='0.0'")
		pl_attribute(Intensity,				float,					4.0f,	ReadWrite,	DirectValue,	"The ambient occlusion intensity",																																					"Min='0.0'")
		pl_attribute(Scale,					float,					10.0f,	ReadWrite,	DirectValue,	"Scales distance between occluders and occlude",																																	"Min='0.0'")
		pl_attribute(Bias,					float,					0.2f,	ReadWrite,	DirectValue,	"Controls the width of the occlusion cone considered by the occlude",																												"Min='0.0'")
			// Overwritten PLScene::SceneRendererPass attributes
		pl_attribute(Flags,					pl_flag_type(EFlags),	0,		ReadWrite,	GetSet,			"Flags",																																											"")
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
		PLCOM_API SRPDeferredSPAAO();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SRPDeferredSPAAO();


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
			PLRenderer::ProgramUniform *pSampleRadius;
			PLRenderer::ProgramUniform *pIntensity;
			PLRenderer::ProgramUniform *pScale;
			PLRenderer::ProgramUniform *pBias;
			PLRenderer::ProgramUniform *pInvFocalLen;
			PLRenderer::ProgramUniform *pRandomSize;
			PLRenderer::ProgramUniform *pRandomNormalsMap;
			PLRenderer::ProgramUniform *pResolution;
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


#endif // __PLCOMPOSITING_DEFERRED_SPAAO_H__
