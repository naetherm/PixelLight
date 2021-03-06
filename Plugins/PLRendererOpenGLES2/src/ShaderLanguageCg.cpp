/*********************************************************\
 *  File: ShaderLanguageCg.cpp                           *
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
#include "PLRendererOpenGLES2/ProgramCg.h"
#include "PLRendererOpenGLES2/VertexShaderCg.h"
#include "PLRendererOpenGLES2/FragmentShaderCg.h"
#include "PLRendererOpenGLES2/ShaderLanguageCg.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRendererOpenGLES2 {


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
const String ShaderLanguageCg::Cg = "Cg";


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(ShaderLanguageCg, "PLRendererOpenGLES2", PLRenderer::ShaderLanguage, "OpenGL ES Cg shader language class using the \"GL_EXT_Cg_shader\"-extension")
	// Properties
	pl_properties
		pl_property("ShaderLanguage",	"Cg")
	pl_properties_end
	// Constructors
	pl_constructor_1_metadata(DefaultConstructor,	PLRenderer::Renderer&,	"Constructor with used renderer as parameter",	"")
pl_class_metadata_end(ShaderLanguageCg)


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::ShaderLanguage functions   ]
//[-------------------------------------------------------]
String ShaderLanguageCg::GetShaderLanguage() const
{
	return Cg;
}

PLRenderer::VertexShader *ShaderLanguageCg::CreateVertexShader()
{
	return new VertexShaderCg(*m_pRenderer);
}

PLRenderer::TessellationControlShader *ShaderLanguageCg::CreateTessellationControlShader()
{
	// OpenGL ES 2.0 has no support for tessellation control shaders
	return nullptr;
}

PLRenderer::TessellationEvaluationShader *ShaderLanguageCg::CreateTessellationEvaluationShader()
{
	// OpenGL ES 2.0 has no support for tessellation evaluation shaders
	return nullptr;
}

PLRenderer::GeometryShader *ShaderLanguageCg::CreateGeometryShader()
{
	// OpenGL ES 2.0 has no support for geometry shaders
	return nullptr;
}

PLRenderer::FragmentShader *ShaderLanguageCg::CreateFragmentShader()
{
	return new FragmentShaderCg(*m_pRenderer);
}

PLRenderer::Program *ShaderLanguageCg::CreateProgram()
{
	return new ProgramCg(*m_pRenderer);
}

PLRenderer::UniformBuffer *ShaderLanguageCg::CreateUniformBuffer()
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
ShaderLanguageCg::ShaderLanguageCg(PLRenderer::Renderer &cRenderer) :
	m_pRenderer(&cRenderer)
{
}

/**
*  @brief
*    Destructor
*/
ShaderLanguageCg::~ShaderLanguageCg()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGLES2
