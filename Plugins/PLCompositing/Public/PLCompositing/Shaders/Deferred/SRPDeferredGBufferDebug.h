/*********************************************************\
 *  File: SRPDeferredGBufferDebug.h                      *
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


#ifndef __PLCOMPOSITING_DEFERRED_GBUFFERDEBUG_H__
#define __PLCOMPOSITING_DEFERRED_GBUFFERDEBUG_H__
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
*    Scene renderer pass for deferred rendering GBuffer debug
*/
class SRPDeferredGBufferDebug : public SRPDeferred {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Debug modes
		*/
		enum EMode {
			ShowAlbedo           = 0,	/**< Show albedo data (RGB) */
			ShowAmbientOcclusion = 1,	/**< Show ambient occlusion data (Gray scale) */
			ShowNormals          = 2,	/**< Show normal data (RGB, view space) */
			ShowDepth            = 3,	/**< Show depth data (Gray scale, view space linear depth, dark=near, bright=far -> may look just black if the distance between far and near plane is to high) */
			ShowSpecularColor    = 4,	/**< Show specular color data (RGB) */
			ShowSpecularExponent = 5,	/**< Show specular exponent data (Gray scale) */
			ShowSelfIllumination = 6,	/**< Show self illumination (RGB, emissive maps, light maps) */
			ShowGlow             = 7,	/**< Show glow (Gray scale) */
			UnknownMode          = 8,	/**< Unknown debug mode (just black result) */
			NumberOfModes        = 9	/**< Number of debug modes */
		};
		pl_enum(EMode)
			pl_enum_value(ShowAlbedo,			"Show albedo data (RGB)")
			pl_enum_value(ShowAmbientOcclusion,	"Show ambient occlusion data (Gray scale)")
			pl_enum_value(ShowNormals,			"Show normal data (RGB, view space)")
			pl_enum_value(ShowDepth,			"Show depth data (Gray scale, view space linear depth, dark=near, bright=far -> may look just black if the distance between far and near plane is to high)")
			pl_enum_value(ShowSpecularColor,	"Show specular color data (RGB)")
			pl_enum_value(ShowSpecularExponent,	"Show specular exponent data (Gray scale)")
			pl_enum_value(ShowSelfIllumination,	"Show self illumination (RGB, emissive maps, light maps)")
			pl_enum_value(ShowGlow,				"Show glow (Gray scale)")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SRPDeferredGBufferDebug, "PLCompositing", PLCompositing::SRPDeferred, "Scene renderer pass for deferred rendering GBuffer debug")
		// Attributes
		pl_attribute(ShaderLanguage,	PLCore::String,			"",			ReadWrite,	DirectValue,	"Shader language to use (for example \"GLSL\" or \"Cg\"), if empty string, the default shader language of the renderer will be used",	"")
		pl_attribute(Mode,				pl_enum_type(EMode),	ShowAlbedo,	ReadWrite,	DirectValue,	"Debug mode",																															"")
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
		PLCOM_API SRPDeferredGBufferDebug();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SRPDeferredGBufferDebug();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Fragment shader flags, flag names become to source code definitions
		*/
		enum EFragmentShaderFlags {
			FS_SHOW_ALBEDO			  = 1<<0,	/**< Show albedo */
			FS_SHOW_AMBIENTOCCLUSION  = 1<<1,	/**< Show ambient occlusion */
			FS_SHOW_NORMALS			  = 1<<2,	/**< Show normals */
			FS_SHOW_DEPTH			  = 1<<3,	/**< Show depth */
			FS_SHOW_SPECULAR_COLOR	  = 1<<4,	/**< Show specular color */
			FS_SHOW_SPECULAR_EXPONENT = 1<<5,	/**< Show specular exponent */
			FS_SHOW_SELFILLUMINATION  = 1<<6,	/**< Show self illumination */
			FS_SHOW_GLOW			  = 1<<7,	/**< Show glow */
			FS_SHOW_BLACK			  = 1<<8	/**< Show black */
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
			PLRenderer::ProgramUniform *pNearPlane;
			PLRenderer::ProgramUniform *pRange;
			PLRenderer::ProgramUniform *pMap;
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


#endif // __PLCOMPOSITING_DEFERRED_GBUFFERDEBUG_H__
