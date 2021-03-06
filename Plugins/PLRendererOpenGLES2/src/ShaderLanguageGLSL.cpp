/*********************************************************\
 *  File: ShaderLanguageGLSL.cpp                         *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRendererOpenGLES2/ProgramGLSL.h"
#include "PLRendererOpenGLES2/VertexShaderGLSL.h"
#include "PLRendererOpenGLES2/FragmentShaderGLSL.h"
#include "PLRendererOpenGLES2/ShaderLanguageGLSL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRendererOpenGLES2 {


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
const String ShaderLanguageGLSL::GLSL = "GLSL";


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(ShaderLanguageGLSL, "PLRendererOpenGLES2", PLRenderer::ShaderLanguage, "OpenGL ES GLSL shader language class")
	// Properties
	pl_properties
		pl_property("ShaderLanguage",	"GLSL")
	pl_properties_end
	// Constructors
	pl_constructor_1_metadata(DefaultConstructor,	PLRenderer::Renderer&,	"Constructor with used renderer as parameter",	"")
pl_class_metadata_end(ShaderLanguageGLSL)


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::ShaderLanguage functions   ]
//[-------------------------------------------------------]
String ShaderLanguageGLSL::GetShaderLanguage() const
{
	return GLSL;
}

PLRenderer::VertexShader *ShaderLanguageGLSL::CreateVertexShader()
{
	return new VertexShaderGLSL(*m_pRenderer);
}

PLRenderer::TessellationControlShader *ShaderLanguageGLSL::CreateTessellationControlShader()
{
	// OpenGL ES 2.0 has no support for tessellation control shaders
	return nullptr;
}

PLRenderer::TessellationEvaluationShader *ShaderLanguageGLSL::CreateTessellationEvaluationShader()
{
	// OpenGL ES 2.0 has no support for tessellation evaluation shaders
	return nullptr;
}

PLRenderer::GeometryShader *ShaderLanguageGLSL::CreateGeometryShader()
{
	// OpenGL ES 2.0 has no support for geometry shaders
	return nullptr;
}

PLRenderer::FragmentShader *ShaderLanguageGLSL::CreateFragmentShader()
{
	return new FragmentShaderGLSL(*m_pRenderer);
}

PLRenderer::Program *ShaderLanguageGLSL::CreateProgram()
{
	return new ProgramGLSL(*m_pRenderer);
}

PLRenderer::UniformBuffer *ShaderLanguageGLSL::CreateUniformBuffer()
{
	// OpenGL ES 2.0 has no support for uniform buffers
	return nullptr;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ShaderLanguageGLSL::ShaderLanguageGLSL(PLRenderer::Renderer &cRenderer) :
	m_pRenderer(&cRenderer)
{
}

/**
*  @brief
*    Destructor
*/
ShaderLanguageGLSL::~ShaderLanguageGLSL()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGLES2
