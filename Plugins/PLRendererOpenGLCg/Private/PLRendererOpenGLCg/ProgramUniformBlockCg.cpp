/*********************************************************\
 *  File: ProgramUniformBlockCg.cpp                      *
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
#include <PLCore/String/String.h>
#include "PLRendererOpenGLCg/UniformBufferCg.h"
#include "PLRendererOpenGLCg/ProgramUniformBlockCg.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRendererOpenGLCg {


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ProgramUniformBlockCg::ProgramUniformBlockCg(CGparameter pCgParameter, const String &sShaderLanguage) :
	m_sShaderLanguage(sShaderLanguage),
	m_pCgParameter(pCgParameter)
{
}

/**
*  @brief
*    Destructor
*/
ProgramUniformBlockCg::~ProgramUniformBlockCg()
{
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::ProgramUniformBlock functions ]
//[-------------------------------------------------------]
bool ProgramUniformBlockCg::SetUniformBuffer(PLRenderer::UniformBuffer *pUniformBuffer, uint32 nBindingPoint)
{
	// Cg hides the binding point internally, so the given binding point is ignored in here...

	// Is there an uniform buffer given?
	if (pUniformBuffer) {
		// The given uniform buffer must have at least the same number of bytes as this uniform block and the shader language must match!
		if (cgGetParameterResourceSize(m_pCgParameter) <= static_cast<long>(pUniformBuffer->GetSize()) && m_sShaderLanguage == pUniformBuffer->GetShaderLanguage()) {
			// The Cg-documentation states: (http://http.developer.nvidia.com/Cg/cgSetProgramBuffer.html)
			// "When the next program bind operation occurs, each buffer index which is set to a valid buffer handle is bound (along with the program) for use by the 3D API"
			cgSetProgramBuffer(cgGetParameterProgram(m_pCgParameter), cgGetParameterBufferIndex(m_pCgParameter), static_cast<UniformBufferCg*>(pUniformBuffer)->GetCgBuffer());
		} else {
			// Error!
			return false;
		}
	} else {
		cgSetProgramBuffer(cgGetParameterProgram(m_pCgParameter), cgGetParameterBufferIndex(m_pCgParameter), nullptr);
	}

	// Done
	return true;
}

uint32 ProgramUniformBlockCg::GetIndex() const
{
	// Cg returns -1 if the parameter does not belong to a buffer or an error occurs... but in here, this shouldn't happen...
	const int nIndex = cgGetParameterBufferIndex(m_pCgParameter);
	return (nIndex > -1) ? nIndex : 0;
}

uint32 ProgramUniformBlockCg::GetSize() const
{
	return cgGetParameterResourceSize(m_pCgParameter);
}

uint32 ProgramUniformBlockCg::GetUniformOffset(const String &sUniformName) const
{
	CGparameter pCgParameter = cgGetNamedStructParameter(m_pCgParameter, sUniformName);
	return pCgParameter ? cgGetParameterBufferOffset(pCgParameter) : 0;
}

uint32 ProgramUniformBlockCg::GetUniformSize(const String &sUniformName) const
{
	CGparameter pCgParameter = cgGetNamedStructParameter(m_pCgParameter, sUniformName);
	return pCgParameter ? cgGetParameterResourceSize(pCgParameter) : 0;
}

uint32 ProgramUniformBlockCg::GetUniformNumOfArrayElements(const String &sUniformName) const
{
	CGparameter pCgParameter = cgGetNamedStructParameter(m_pCgParameter, sUniformName);
	return (pCgParameter && cgGetParameterClass(pCgParameter) == CG_PARAMETERCLASS_ARRAY) ? cgGetArraySize(pCgParameter, 0) : 1;	// We currently only support one dimension...
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGLCg
