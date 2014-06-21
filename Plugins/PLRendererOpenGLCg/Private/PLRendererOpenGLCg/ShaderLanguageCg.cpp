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
#include "PLRendererOpenGLCg/ProgramCg.h"
#include "PLRendererOpenGLCg/VertexShaderCg.h"
#include "PLRendererOpenGLCg/UniformBufferCg.h"
#include "PLRendererOpenGLCg/GeometryShaderCg.h"
#include "PLRendererOpenGLCg/FragmentShaderCg.h"
#include "PLRendererOpenGLCg/TessellationControlShaderCg.h"
#include "PLRendererOpenGLCg/TessellationEvaluationShaderCg.h"
#include "PLRendererOpenGLCg/ShaderLanguageCg.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRendererOpenGLCg {


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
const String ShaderLanguageCg::Cg = "Cg";


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ShaderLanguageCg)


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
	return new TessellationControlShaderCg(*m_pRenderer);
}

PLRenderer::TessellationEvaluationShader *ShaderLanguageCg::CreateTessellationEvaluationShader()
{
	return new TessellationEvaluationShaderCg(*m_pRenderer);
}

PLRenderer::GeometryShader *ShaderLanguageCg::CreateGeometryShader()
{
	return new GeometryShaderCg(*m_pRenderer);
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
	return new UniformBufferCg(*m_pRenderer, Cg);
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
} // PLRendererOpenGLCg
