/*********************************************************\
 *  File: ShaderLanguageGLSL.h                           *
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


#ifndef __PLRENDEREROPENGL_SHADERLANGUAGEGLSL_H__
#define __PLRENDEREROPENGL_SHADERLANGUAGEGLSL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRendererOpenGL/ShaderLanguage.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class Renderer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Build in OpenGL GLSL shader language class
*/
class ShaderLanguageGLSL : public ShaderLanguage {


	//[-------------------------------------------------------]
	//[ Public static data                                    ]
	//[-------------------------------------------------------]
	public:
		static const PLCore::String GLSL;	/**< 'GLSL' string */


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLRENDEREROPENGL_RTTI_EXPORT, ShaderLanguageGLSL, "PLRendererOpenGL", PLRendererOpenGL::ShaderLanguage, "Build in OpenGL GLSL shader language class")
		// Properties
		pl_properties
			pl_property("ShaderLanguage",	"GLSL")
		pl_properties_end
		// Constructors
		pl_constructor_1(DefaultConstructor,	PLRenderer::Renderer&,	"Constructor with used renderer as parameter",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public virtual PLRenderer::ShaderLanguage functions   ]
	//[-------------------------------------------------------]
	public:
		virtual PLCore::String GetShaderLanguage() const override;
		virtual PLRenderer::VertexShader *CreateVertexShader() override;
		virtual PLRenderer::TessellationControlShader *CreateTessellationControlShader() override;
		virtual PLRenderer::TessellationEvaluationShader *CreateTessellationEvaluationShader() override;
		virtual PLRenderer::GeometryShader *CreateGeometryShader() override;
		virtual PLRenderer::FragmentShader *CreateFragmentShader() override;
		virtual PLRenderer::Program *CreateProgram() override;
		virtual PLRenderer::UniformBuffer *CreateUniformBuffer() override;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    The used renderer
		*/
		ShaderLanguageGLSL(PLRenderer::Renderer &cRenderer);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ShaderLanguageGLSL();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLRenderer::Renderer *m_pRenderer;	/**< The used renderer, always valid! */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL


#endif // __PLRENDEREROPENGL_SHADERLANGUAGEGLSL_H__
