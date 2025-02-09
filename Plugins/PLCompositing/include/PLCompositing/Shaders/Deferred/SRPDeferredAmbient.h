/*********************************************************\
 *  File: SRPDeferredAmbient.h                           *
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


#ifndef __PLCOMPOSITING_DEFERRED_AMBIENT_H__
#define __PLCOMPOSITING_DEFERRED_AMBIENT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
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
*    Scene renderer pass for deferred rendering ambient
*/
class SRPDeferredAmbient : public SRPDeferred {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene renderer pass flags (SceneRendererPass flags extension)
		*/
		enum EFlags {
			NoAlbedo            = 1<<1,	/**< Ignore albedo data */
			NoAmbientOcclusion  = 1<<2,	/**< Ignore ambient occlusion data */
			NoSelfIllumination  = 1<<3,	/**< Ignore self illumination data */
			NoStencil           = 1<<4	/**< Ignore stencil buffer */
		};
		pl_flag(EFlags)
			pl_enum_base(SRPDeferred::EFlags)
			pl_enum_value(NoAlbedo,				"Ignore albedo data")
			pl_enum_value(NoAmbientOcclusion,	"Ignore ambient occlusion data")
			pl_enum_value(NoSelfIllumination,	"Ignore self illumination data")
			pl_enum_value(NoStencil,			"Ignore stencil buffer")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PLCOM_API)
		// Attributes
		pl_attribute_directvalue(						ShaderLanguage,	PLCore::String,		"",										ReadWrite)
		pl_attribute_directvalue(						AmbientColor,	PLGraphics::Color3,	PLGraphics::Color3(0.2f, 0.2f, 0.2f),	ReadWrite)
			// Overwritten PLScene::SceneRendererPass attributes
		pl_attribute_getset		(SRPDeferredAmbient,	Flags,			PLCore::uint32,		0,										ReadWrite)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCOM_API SRPDeferredAmbient();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SRPDeferredAmbient();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Fragment shader flags, flag names become to source code definitions
		*/
		enum EFragmentShaderFlags {
			FS_ALBEDO			= 1<<0,	/**< Use albedo data */
			FS_AMBIENTOCCLUSION	= 1<<1,	/**< Use ambient occlusion data */
			FS_SELFILLUMINATION	= 1<<2	/**< Self illumination data used */
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
			PLRenderer::ProgramUniform *pAmbientColor;
			PLRenderer::ProgramUniform *pAlbedoMap;
			PLRenderer::ProgramUniform *pSelfIlluminationMap;
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


#endif // __PLCOMPOSITING_DEFERRED_AMBIENT_H__
