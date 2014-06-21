/*********************************************************\
 *  File: ShaderFunctionClipRayTemplate.h                *
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


#ifndef __PLVOLUMERENDERER_SHADERFUNCTION_CLIPRAY_TEMPLATE_H__
#define __PLVOLUMERENDERER_SHADERFUNCTION_CLIPRAY_TEMPLATE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLVolumeRenderer/ClipRay/ShaderFunctionClipRay.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Template shader function implementation class for clip ray
*/
class ShaderFunctionClipRayTemplate : public ShaderFunctionClipRay {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLVOLUMERENDERER_RTTI_EXPORT, ShaderFunctionClipRayTemplate, "PLVolumeRenderer", PLVolumeRenderer::ShaderFunctionClipRay, "Template shader function implementation class for clip ray")
		// Properties
		pl_properties
			pl_property("FunctionTemplate",	"1")
		pl_properties_end
		// Constructors
		pl_constructor_2(ParamsConstructor,	PLCore::uint8,	PLCore::uint8,	"Parameter constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sShaderLanguage
		*    Number of clip planes
		*  @param[in] sShaderLanguage
		*    Number of depth textures
		*/
		PLVOLUMERENDERER_API ShaderFunctionClipRayTemplate(PLCore::uint8 nNumOfClipPlanes, PLCore::uint8 nNumOfDepthTextures);

		/**
		*  @brief
		*    Destructor
		*/
		PLVOLUMERENDERER_API virtual ~ShaderFunctionClipRayTemplate();


	//[-------------------------------------------------------]
	//[ Public virtual ShaderFunctionClipRay functions        ]
	//[-------------------------------------------------------]
	public:
		PLVOLUMERENDERER_API virtual void SetPlanes(PLRenderer::Program &cProgram, const PLScene::VisNode &cVisNode, const PLCore::Array<const PLScene::VisNode*> &lstClipPlanes) override;


	//[-------------------------------------------------------]
	//[ Public virtual ShaderFunction functions               ]
	//[-------------------------------------------------------]
	public:
		PLVOLUMERENDERER_API virtual PLCore::String GetSourceCode(const PLCore::String &sShaderLanguage, ESourceCodeType nSourceCodeType) override;
		PLVOLUMERENDERER_API virtual void SetProgram(PLRenderer::Program &cProgram, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode, SRPVolume &cSRPVolume) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Generates a template instance source code
		*
		*  @param[in] sShaderLanguage
		*    The name of the shader language (for example "GLSL" or "Cg") to return the source code for
		*
		*  @return
		*    Generated template instance source code
		*/
		PLCore::String GenerateSourceCode(const PLCore::String &sShaderLanguage);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::uint8				   m_nNumOfClipPlanes;		/**< Number of clip planes */
		PLCore::uint8				   m_nNumOfDepthTextures;	/**< Number of depth textures */
		PLCore::Array<ShaderFunction*> m_lstShaderFunction;		/**< Shader function instances this shader composition consists of, the given instance are just shared (do not destroy them) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer


#endif // __PLVOLUMERENDERER_SHADERFUNCTION_CLIPRAY_TEMPLATE_H__
