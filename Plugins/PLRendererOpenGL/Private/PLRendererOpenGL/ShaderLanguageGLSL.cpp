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
#include "PLRendererOpenGL/Context.h"
#include "PLRendererOpenGL/Renderer.h"
#include "PLRendererOpenGL/Extensions.h"
#include "PLRendererOpenGL/ProgramGLSL.h"
#include "PLRendererOpenGL/UniformBuffer.h"
#include "PLRendererOpenGL/VertexShaderGLSL.h"
#include "PLRendererOpenGL/GeometryShaderGLSL.h"
#include "PLRendererOpenGL/FragmentShaderGLSL.h"
#include "PLRendererOpenGL/TessellationControlShaderGLSL.h"
#include "PLRendererOpenGL/TessellationEvaluationShaderGLSL.h"
#include "PLRendererOpenGL/ShaderLanguageGLSL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
const String ShaderLanguageGLSL::GLSL = "GLSL";


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ShaderLanguageGLSL)


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::ShaderLanguage functions   ]
//[-------------------------------------------------------]
String ShaderLanguageGLSL::GetShaderLanguage() const
{
	return GLSL;
}

PLRenderer::VertexShader *ShaderLanguageGLSL::CreateVertexShader()
{
	// Is the OpenGL extension GL_ARB_shader_objects available?
	return static_cast<Renderer*>(m_pRenderer)->GetContext().GetExtensions().IsGL_ARB_shader_objects() ? new VertexShaderGLSL(*m_pRenderer) : nullptr;
}

PLRenderer::TessellationControlShader *ShaderLanguageGLSL::CreateTessellationControlShader()
{
	// Is the OpenGL extension GL_ARB_tessellation_shader available?
	return static_cast<Renderer*>(m_pRenderer)->GetContext().GetExtensions().IsGL_ARB_tessellation_shader() ? new TessellationControlShaderGLSL(*m_pRenderer) : nullptr;
}

PLRenderer::TessellationEvaluationShader *ShaderLanguageGLSL::CreateTessellationEvaluationShader()
{
	// Is the OpenGL extension GL_ARB_tessellation_shader available?
	return static_cast<Renderer*>(m_pRenderer)->GetContext().GetExtensions().IsGL_ARB_tessellation_shader() ? new TessellationEvaluationShaderGLSL(*m_pRenderer) : nullptr;
}

PLRenderer::GeometryShader *ShaderLanguageGLSL::CreateGeometryShader()
{
	// Are the OpenGL extensions GL_ARB_shader_objects and GL_EXT_geometry_shader4 available?
	const Extensions &cExtensions = static_cast<Renderer*>(m_pRenderer)->GetContext().GetExtensions();
	return (cExtensions.IsGL_ARB_shader_objects() && cExtensions.IsGL_EXT_geometry_shader4()) ? new GeometryShaderGLSL(*m_pRenderer) : nullptr;
}

PLRenderer::FragmentShader *ShaderLanguageGLSL::CreateFragmentShader()
{
	// Is the OpenGL extension GL_ARB_shader_objects available?
	return static_cast<Renderer*>(m_pRenderer)->GetContext().GetExtensions().IsGL_ARB_shader_objects() ? new FragmentShaderGLSL(*m_pRenderer) : nullptr;
}

PLRenderer::Program *ShaderLanguageGLSL::CreateProgram()
{
	// Is the OpenGL extension GL_ARB_shader_objects available?
	return static_cast<Renderer*>(m_pRenderer)->GetContext().GetExtensions().IsGL_ARB_shader_objects() ? new ProgramGLSL(*m_pRenderer) : nullptr;
}

PLRenderer::UniformBuffer *ShaderLanguageGLSL::CreateUniformBuffer()
{
	// Are the OpenGL extensions GL_ARB_uniform_buffer_object and GL_EXT_transform_feedback available?
	Renderer *pRenderer = static_cast<Renderer*>(m_pRenderer);
	const Extensions &cExtensions = pRenderer->GetContext().GetExtensions();
	return (cExtensions.IsGL_ARB_uniform_buffer_object() && cExtensions.IsGL_EXT_transform_feedback()) ? new UniformBuffer(*m_pRenderer, GLSL) : nullptr;
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
} // PLRendererOpenGL
