/*********************************************************\
 *  File: SRPDebugDepthShaders.h                         *
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


#ifndef __PLCOMPOSITING_GENERAL_DEBUGDEPTH_H__
#define __PLCOMPOSITING_GENERAL_DEBUGDEPTH_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/ProgramGenerator.h>
#include "PLCompositing/SRPDebug.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class ProgramUniform;
	class ProgramAttribute;
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
*    Shaders based scene renderer pass visualizing the depth buffer for debugging purposes
*/
class SRPDebugDepthShaders : public SRPDebug {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene renderer pass flags (SceneRendererPass flags extension)
		*/
		enum EFlags {
			NoLinearizedDepth = 1<<1	/**< Do not linearize the depth */
		};
		pl_enum(EFlags)
			pl_enum_base(SRPDebug::EFlags)
			pl_enum_value(NoLinearizedDepth, "Do not linearize the depth")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SRPDebugDepthShaders, "PLCompositing", PLCompositing::SRPDebug, "Shaders based scene renderer pass visualizing the depth buffer for debugging purposes")
		// Attributes
		pl_attribute(ShaderLanguage,	PLCore::String,			"",	ReadWrite,	DirectValue,	"Shader language to use (for example \"GLSL\" or \"Cg\"), if empty string, the default shader language of the renderer will be used",	"")
			// Overwritten PLScene::SceneRendererPass attributes
		pl_attribute(Flags,				pl_flag_type(EFlags),	0,	ReadWrite,	GetSet,			"Flags",																																"")
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
		PLCOM_API SRPDebugDepthShaders();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SRPDebugDepthShaders();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Fragment shader flags, flag names become to source code definitions
		*/
		enum EFragmentShaderFlags {
			FS_LINEARIZE_DEPTH = 1<<0	/**< Linearize the depth */
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
			PLRenderer::ProgramUniform *pTexture;
			PLRenderer::ProgramUniform *pZNearFar;
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		FullscreenQuad						*m_pFullscreenQuad;		/**< Fullscreen quad instance, can be a null pointer */
		PLRenderer::ProgramGenerator		*m_pProgramGenerator;	/**< Program generator, can be a null pointer */
		PLRenderer::ProgramGenerator::Flags	 m_cProgramFlags;		/**< Program flags as class member to reduce dynamic memory allocations */


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


#endif // __PLCOMPOSITING_GENERAL_DEBUGDEPTH_H__
