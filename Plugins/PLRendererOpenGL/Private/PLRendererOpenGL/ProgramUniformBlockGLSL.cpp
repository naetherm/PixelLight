/*********************************************************\
 *  File: ProgramUniformBlockGLSL.cpp                    *
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
#include "PLRendererOpenGL/Extensions.h"
#include "PLRendererOpenGL/UniformBuffer.h"
#include "PLRendererOpenGL/ProgramUniformBlockGLSL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ProgramUniformBlockGLSL::ProgramUniformBlockGLSL(GLuint nOpenGLProgram, GLuint nUniformBlockIndex, GLint nUniformBlockSize, const String &sShaderLanguage) :
	m_sShaderLanguage(sShaderLanguage),
	m_nOpenGLProgram(nOpenGLProgram),
	m_nUniformBlockIndex(nUniformBlockIndex),
	m_nOpenGLUniformBlockSize(nUniformBlockSize),
	m_nBindingPoint(-1)
{
}

/**
*  @brief
*    Destructor
*/
ProgramUniformBlockGLSL::~ProgramUniformBlockGLSL()
{
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::ProgramUniformBlock functions ]
//[-------------------------------------------------------]
bool ProgramUniformBlockGLSL::SetUniformBuffer(PLRenderer::UniformBuffer *pUniformBuffer, uint32 nBindingPoint)
{
	// Is there an uniform buffer given?
	if (pUniformBuffer) {
		// The given uniform buffer must have at least the same number of bytes as this uniform block and the shader language must match!
		if (m_nOpenGLUniformBlockSize <= static_cast<GLint>(pUniformBuffer->GetSize()) && m_sShaderLanguage == pUniformBuffer->GetShaderLanguage()) {
			// Attach the buffer to the given UBO binding point
			glBindBufferBaseEXT(GL_UNIFORM_BUFFER, nBindingPoint, static_cast<UniformBuffer*>(pUniformBuffer)->GetOpenGLUniformBuffer());
		} else {
			// Error!
			return false;
		}
	} else {
		// Attach no buffer to the given UBO binding point
		glBindBufferBaseEXT(GL_UNIFORM_BUFFER, nBindingPoint, 0);
	}

	// Is the uniform block already associated with this binding point? (this test safes some graphics API calls)
	if (m_nBindingPoint != static_cast<int>(nBindingPoint)) {
		// Associate the uniform block with the given binding point
		m_nBindingPoint = nBindingPoint;
		glUniformBlockBinding(m_nOpenGLProgram, m_nUniformBlockIndex, m_nBindingPoint);
	}

	// Done
	return true;
}

uint32 ProgramUniformBlockGLSL::GetIndex() const
{
	return m_nUniformBlockIndex;
}

uint32 ProgramUniformBlockGLSL::GetSize() const
{
	return m_nOpenGLUniformBlockSize;
}

uint32 ProgramUniformBlockGLSL::GetUniformOffset(const String &sUniformName) const
{
	GLuint nIndex = 0;
	const GLchar *pszName = sUniformName.GetASCII();
	glGetUniformIndices(m_nOpenGLProgram, 1, &pszName, &nIndex);
	if (nIndex != GL_INVALID_INDEX) {
		GLint nOffset = 0;
		glGetActiveUniformsiv(m_nOpenGLProgram, 1, &nIndex, GL_UNIFORM_OFFSET, &nOffset);
		return nOffset;
	} else {
		// Error!
		return 0;
	}
}

uint32 ProgramUniformBlockGLSL::GetUniformSize(const String &sUniformName) const
{
	GLuint nIndex = 0;
	const GLchar *pszName = sUniformName.GetASCII();
	glGetUniformIndices(m_nOpenGLProgram, 1, &pszName, &nIndex);
	if (nIndex != GL_INVALID_INDEX) {
		// Get the number of array elements
		GLint nNumOfArrayElements = 0;
		glGetActiveUniformsiv(m_nOpenGLProgram, 1, &nIndex, GL_UNIFORM_SIZE, &nNumOfArrayElements);

		// Get the uniform type
		GLint nType = 0;
		glGetActiveUniformsiv(m_nOpenGLProgram, 1, &nIndex, GL_UNIFORM_TYPE, &nType);

		// Get the size (in bytes) of one element within the array
		GLint nSize = 0;
		switch (nType) {
			case GL_FLOAT:					nSize = sizeof(float);			break;
			case GL_FLOAT_VEC2:				nSize = sizeof(float)*2;		break;
			case GL_FLOAT_VEC3:				nSize = sizeof(float)*3;		break;
			case GL_FLOAT_VEC4:				nSize = sizeof(float)*4;		break;
			case GL_INT:					nSize = sizeof(int);			break;
			case GL_INT_VEC2:				nSize = sizeof(int)*2;			break;
			case GL_INT_VEC3:				nSize = sizeof(int)*3;			break;
			case GL_INT_VEC4:				nSize = sizeof(int)*4;			break;
			case GL_UNSIGNED_INT:			nSize = sizeof(unsigned int);	break;
			case GL_UNSIGNED_INT_VEC2_EXT:	nSize = sizeof(unsigned int)*2;	break;
			case GL_UNSIGNED_INT_VEC3_EXT:	nSize = sizeof(unsigned int)*3;	break;
			case GL_UNSIGNED_INT_VEC4_EXT:	nSize = sizeof(unsigned int)*4;	break;
			case GL_BOOL:					nSize = sizeof(bool);			break;
			case GL_BOOL_VEC2:				nSize = sizeof(bool)*2;			break;
			case GL_BOOL_VEC3:				nSize = sizeof(bool)*3;			break;
			case GL_BOOL_VEC4:				nSize = sizeof(bool)*4;			break;
			case GL_FLOAT_MAT2:				nSize = sizeof(float)*2*2;		break;
			case GL_FLOAT_MAT3:				nSize = sizeof(float)*3*3;		break;
			case GL_FLOAT_MAT4:				nSize = sizeof(float)*4*4;		break;
			case GL_FLOAT_MAT2x3:			nSize = sizeof(float)*2*3;		break;
			case GL_FLOAT_MAT2x4:			nSize = sizeof(float)*2*4;		break;
			case GL_FLOAT_MAT3x2:			nSize = sizeof(float)*3*2;		break;
			case GL_FLOAT_MAT3x4:			nSize = sizeof(float)*3*4;		break;
			case GL_FLOAT_MAT4x2:			nSize = sizeof(float)*4*2;		break;
			case GL_FLOAT_MAT4x3:			nSize = sizeof(float)*4*3;		break;
			default:						nSize = 0;	// Samplers are not supported
		}

		// Get the number of bytes
		return nNumOfArrayElements*nSize;
	} else {
		// Error!
		return 0;
	}
}

uint32 ProgramUniformBlockGLSL::GetUniformNumOfArrayElements(const String &sUniformName) const
{
	GLuint nIndex = 0;
	const GLchar *pszName = sUniformName.GetASCII();
	glGetUniformIndices(m_nOpenGLProgram, 1, &pszName, &nIndex);
	if (nIndex != GL_INVALID_INDEX) {
		GLint nSize = 0;
		glGetActiveUniformsiv(m_nOpenGLProgram, 1, &nIndex, GL_UNIFORM_SIZE, &nSize);
		return nSize;
	} else {
		// Error!
		return 0;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL
