/*********************************************************\
 *  File: ProgramGLSL.cpp                                *
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
#include "PLRendererOpenGL/VertexShaderGLSL.h"
#include "PLRendererOpenGL/ShaderLanguageGLSL.h"
#include "PLRendererOpenGL/GeometryShaderGLSL.h"
#include "PLRendererOpenGL/FragmentShaderGLSL.h"
#include "PLRendererOpenGL/TessellationControlShaderGLSL.h"
#include "PLRendererOpenGL/TessellationEvaluationShaderGLSL.h"
#include "PLRendererOpenGL/Context.h"
#include "PLRendererOpenGL/Renderer.h"
#include "PLRendererOpenGL/Extensions.h"
#include "PLRendererOpenGL/ShaderToolsGLSL.h"
#include "PLRendererOpenGL/ProgramAttributeGLSL.h"
#include "PLRendererOpenGL/ProgramUniformGLSL.h"
#include "PLRendererOpenGL/ProgramUniformBlockGLSL.h"
#include "PLRendererOpenGL/ProgramGLSL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
ProgramGLSL::~ProgramGLSL()
{
	// Call the relink method, this automatically destroys the used resources
	RelinkRequired();

	// Destroy the OpenGL program
	glDeleteObjectARB(m_nOpenGLProgram);
}

/**
*  @brief
*    Returns the OpenGL program
*/
GLuint ProgramGLSL::GetOpenGLProgram(bool bAutomaticLink)
{
	// Is the program already linked?
	if (!m_bLinked && bAutomaticLink) {
		// Program linked already failed? (so we don't see the linker errors only once!)
		if (!m_bLinkedFailed) {
			// Attach vertex shader
			if (!m_bVertexShaderAttached) {
				VertexShaderGLSL *pVertexShaderGLSL = static_cast<VertexShaderGLSL*>(m_cVertexShaderHandler.GetResource());
				if (pVertexShaderGLSL) {
					glAttachObjectARB(m_nOpenGLProgram, pVertexShaderGLSL->GetOpenGLVertexShader());
					m_bVertexShaderAttached = true;
				}
			}

			// Attach tessellation control shader
			if (!m_bTessellationControlShaderAttached) {
				TessellationControlShaderGLSL *pTessellationControlShaderGLSL = static_cast<TessellationControlShaderGLSL*>(m_cTessellationControlShaderHandler.GetResource());
				if (pTessellationControlShaderGLSL) {
					glAttachObjectARB(m_nOpenGLProgram, pTessellationControlShaderGLSL->GetOpenGLTessellationControlShader());
					m_bTessellationControlShaderAttached = true;
				}
			}

			// Attach tessellation evaluation shader
			if (!m_bTessellationEvaluationShaderAttached) {
				TessellationEvaluationShaderGLSL *pTessellationEvaluationShaderGLSL = static_cast<TessellationEvaluationShaderGLSL*>(m_cTessellationEvaluationShaderHandler.GetResource());
				if (pTessellationEvaluationShaderGLSL) {
					glAttachObjectARB(m_nOpenGLProgram, pTessellationEvaluationShaderGLSL->GetOpenGLTessellationEvaluationShader());
					m_bTessellationEvaluationShaderAttached = true;
				}
			}

			// Attach geometry shader
			if (!m_bGeometryShaderAttached) {
				GeometryShaderGLSL *pGeometryShaderGLSL = static_cast<GeometryShaderGLSL*>(m_cGeometryShaderHandler.GetResource());
				if (pGeometryShaderGLSL) {
					glAttachObjectARB(m_nOpenGLProgram, pGeometryShaderGLSL->GetOpenGLGeometryShader());
					m_bGeometryShaderAttached = true;
				}
			}

			// Attach fragment shader
			if (!m_bFragmentShaderAttached) {
				FragmentShaderGLSL *pFragmentShaderGLSL = static_cast<FragmentShaderGLSL*>(m_cFragmentShaderHandler.GetResource());
				if (pFragmentShaderGLSL) {
					glAttachObjectARB(m_nOpenGLProgram, pFragmentShaderGLSL->GetOpenGLFragmentShader());
					m_bFragmentShaderAttached = true;
				}
			}

			// The user may request a copy of the compiled program...
			if (static_cast<Renderer&>(GetRenderer()).GetContext().GetExtensions().IsGL_ARB_get_program_binary())
				glProgramParameteri(m_nOpenGLProgram, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);

			// Link the program
			glLinkProgramARB(m_nOpenGLProgram);

			// Check the link status
			GLint nLinked = GL_FALSE;
			glGetObjectParameterivARB(m_nOpenGLProgram, GL_OBJECT_LINK_STATUS_ARB, &nLinked);
			if (nLinked == GL_TRUE) {
				// Congratulations, the program is now linked!
				m_bLinked = true;
			} else {
				// Error, program link failed!
				m_bLinkedFailed = true;
				ShaderToolsGLSL::WriteInformationIntoLog(m_nOpenGLProgram);
			}
		}
	}
	return m_nOpenGLProgram;
}

/**
*  @brief
*    Returns whether or not the program is currently linked
*/
bool ProgramGLSL::IsLinked() const
{
	return m_bLinked;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ProgramGLSL::ProgramGLSL(PLRenderer::Renderer &cRenderer) : Program(cRenderer),
	m_nOpenGLProgram(glCreateProgramObjectARB()),
	m_bLinked(false),
	m_bLinkedFailed(false),
	m_bVertexShaderAttached(false),
	m_bTessellationControlShaderAttached(false),
	m_bTessellationEvaluationShaderAttached(false),
	m_bGeometryShaderAttached(false),
	m_bFragmentShaderAttached(false),
	m_bAttributeInformationBuild(false),
	m_bUniformInformationBuild(false),
	m_bUniformBlockInformationBuild(false)
{
}

/**
*  @brief
*    Denotes that a program relink is required
*/
void ProgramGLSL::RelinkRequired()
{
	// Is the program currently linked?
	if (m_bLinked) {
		// The linked state is now dirty
		m_bLinked = m_bLinkedFailed = false;

		// Destroy the attribute information
		DestroyAttributeInformation();

		// Destroy the uniform information
		DestroyUniformInformation();

		// Destroy the uniform block information
		DestroyUniformBlockInformation();

		// The program is now dirty
		EventDirty(this);
	}
}

/**
*  @brief
*    Builds the attribute information
*/
void ProgramGLSL::BuildAttributeInformation()
{
	// Attribute information already build?
	if (!m_bAttributeInformationBuild) {
		// Get the OpenGL program - this also ensures that the program is linked
		const GLuint nOpenGLProgram = GetOpenGLProgram();

		// Is the program linked?
		if (m_bLinked) {
			// Get the number of active attributes
			GLint nNumOfActiveAttributes = 0;
			glGetObjectParameterivARB(nOpenGLProgram, GL_OBJECT_ACTIVE_ATTRIBUTES_ARB, &nNumOfActiveAttributes);
			if (nNumOfActiveAttributes) {
				// Get the length of the longest active attribute name, including a null terminator
				GLint nMaxNameLength = 0;
				glGetObjectParameterivARB(nOpenGLProgram, GL_OBJECT_ACTIVE_ATTRIBUTE_MAX_LENGTH_ARB, &nMaxNameLength);
				if (nMaxNameLength) {
					// Allocate the name buffer
					char *pszName = new char[nMaxNameLength];

					// Iterate over all attributes
					for (int i=0; i<nNumOfActiveAttributes; i++) {
						// Get the current attribute
						GLsizei nCurrentNameLength = 0;	// The actual number of characters written into pszName, excluding the null terminator
						int		nTypeSize		   = 0;	// The size of the attribute, the value is in units of the type, for arrays, normally 1
						GLenum  nType			   = 0;	// Attribute type, e.g. GL_FLOAT_VEC3
						glGetActiveAttribARB(nOpenGLProgram, i, nMaxNameLength, &nCurrentNameLength, &nTypeSize, &nType, pszName);

						// Get the attribute location
						const int nOpenGLAttributeLocation = glGetAttribLocationARB(nOpenGLProgram, pszName);
						if (nOpenGLAttributeLocation != -1) {
							// Create a new program attribute instance
							ProgramAttributeGLSL *pProgramAttribute = new ProgramAttributeGLSL(nOpenGLAttributeLocation);

							// Register the new program attribute
							m_lstAttributes.Add(pProgramAttribute);
							m_mapAttributes.Add(String(pszName, true, nCurrentNameLength), pProgramAttribute);
						}
					}

					// Destroy the name buffer
					delete [] pszName;
				}
			}

			// The attribute information build is now build
			m_bAttributeInformationBuild = true;
		}
	}
}

/**
*  @brief
*    Destroys the attribute information
*/
void ProgramGLSL::DestroyAttributeInformation()
{
	// Is there anything to destroy?
	const uint32 nNumOfAttributes = m_lstAttributes.GetNumOfElements();
	if (nNumOfAttributes) {
		// Destroy the attribute instances
		for (uint32 i=0; i<nNumOfAttributes; i++)
			delete static_cast<ProgramAttributeGLSL*>(m_lstAttributes[i]);

		// Clear the attribute list and hash map
		m_lstAttributes.Clear();
		m_mapAttributes.Clear();
	}

	// Currently, there's no attribute information build
	m_bAttributeInformationBuild = false;
}

/**
*  @brief
*    Builds the uniform information
*/
void ProgramGLSL::BuildUniformInformation()
{
	// Uniform information already build?
	if (!m_bUniformInformationBuild) {
		// Get the OpenGL program - this also ensures that the program is linked
		const GLuint nOpenGLProgram = GetOpenGLProgram();

		// Is the program linked?
		if (m_bLinked) {
			// Get the number of active uniforms
			GLint nNumOfActiveUniforms = 0;
			glGetObjectParameterivARB(nOpenGLProgram, GL_OBJECT_ACTIVE_UNIFORMS_ARB, &nNumOfActiveUniforms);
			if (nNumOfActiveUniforms) {
				// Get the length of the longest active uniform name, including a null terminator
				GLint nMaxNameLength = 0;
				glGetObjectParameterivARB(nOpenGLProgram, GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB, &nMaxNameLength);
				if (nMaxNameLength) {
					// Allocate the name buffer
					char *pszName = new char[nMaxNameLength];

					// Next free texture unit which can be assigned to a sampler uniform
					int nNextFreeTextureUnit = 0;

					// Iterate over all uniforms
					for (int i=0; i<nNumOfActiveUniforms; i++) {
						// Get the current uniform
						GLsizei nCurrentNameLength = 0;	// The actual number of characters written into pszName, excluding the null terminator
						int		nTypeSize		   = 0;	// The size of the uniform, the value is in units of the type, for arrays, normally 1
						GLenum  nType			   = 0;	// Uniform type, e.g. GL_FLOAT_VEC3
						glGetActiveUniformARB(nOpenGLProgram, i, nMaxNameLength, &nCurrentNameLength, &nTypeSize, &nType, pszName);

						// Get the uniform location
						const int nOpenGLUniformLocation = glGetUniformLocationARB(nOpenGLProgram, pszName);
						if (nOpenGLUniformLocation != -1) {
							// Is this a sampler uniform?
							int nTextureUnit = -1;
							if (nType == GL_SAMPLER_1D_ARB || nType == GL_SAMPLER_2D_ARB || nType == GL_SAMPLER_2D_ARRAY_EXT || nType == GL_SAMPLER_3D_ARB || nType == GL_SAMPLER_CUBE_ARB ||
								nType == GL_SAMPLER_1D_SHADOW_ARB || nType == GL_SAMPLER_2D_SHADOW_ARB || nType == GL_SAMPLER_2D_RECT_ARB || nType == GL_SAMPLER_2D_RECT_SHADOW_ARB) {
								// Assign the next free texture unit
								nTextureUnit = nNextFreeTextureUnit;

								// Update the next free texture unit
								nNextFreeTextureUnit++;
							}

							// Create a new program uniform instance
							ProgramUniformGLSL *pProgramUniform = new ProgramUniformGLSL(nOpenGLProgram, nOpenGLUniformLocation, nTextureUnit);

							// Register the new program uniform
							m_lstUniforms.Add(pProgramUniform);
							m_mapUniforms.Add(String(pszName, true, nCurrentNameLength), pProgramUniform);
						}
					}

					// Destroy the name buffer
					delete [] pszName;
				}
			}

			// The uniform information build is now build
			m_bUniformInformationBuild = true;
		}
	}
}

/**
*  @brief
*    Destroys the uniform information
*/
void ProgramGLSL::DestroyUniformInformation()
{
	// Is there anything to destroy?
	const uint32 nNumOfUniforms = m_lstUniforms.GetNumOfElements();
	if (nNumOfUniforms) {
		// Destroy the uniform instances
		for (uint32 i=0; i<nNumOfUniforms; i++)
			delete static_cast<ProgramUniformGLSL*>(m_lstUniforms[i]);

		// Clear the uniform list and hash map
		m_lstUniforms.Clear();
		m_mapUniforms.Clear();
	}

	// Currently, there's no uniform information build
	m_bUniformInformationBuild = false;
}

/**
*  @brief
*    Builds the uniform block information
*/
void ProgramGLSL::BuildUniformBlockInformation()
{
	// Uniform block information already build?
	if (!m_bUniformBlockInformationBuild) {
		// Get the OpenGL program - this also ensures that the program is linked
		const GLuint nOpenGLProgram = GetOpenGLProgram();

		// Is the program linked?
		if (m_bLinked) {
			// [TODO] GLSL uniform block enumeration doesn't work in the way I expected when reading http://www.opengl.org/registry/specs/ARB/uniform_buffer_object.txt
			{ // Somewhat of a hack...
				static const int nMaxNameLength = 256;
				char szName[nMaxNameLength];

				// Iterate over all uniform blocks
				GLuint nUniformBlockIndex = 0;
				GLsizei nCurrentNameLength = 0;	// The actual number of characters written into pszName, excluding the null terminator
				do {
					// Get the current uniform block
					nCurrentNameLength = 0;
					glGetActiveUniformBlockName(nOpenGLProgram, nUniformBlockIndex, nMaxNameLength, &nCurrentNameLength, szName);
					if (nCurrentNameLength) {
						GLint nUniformBlockSize = 0;
						glGetActiveUniformBlockiv(nOpenGLProgram, nUniformBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &nUniformBlockSize);

						// Create a new program uniform block instance
						ProgramUniformBlockGLSL *pProgramUniformBlock = new ProgramUniformBlockGLSL(nOpenGLProgram, nUniformBlockIndex, nUniformBlockSize, ShaderLanguageGLSL::GLSL);

						// Register the new program uniform block
						m_lstUniformBlocks.Add(pProgramUniformBlock);
						m_mapUniformBlocks.Add(String(szName, true, nCurrentNameLength), pProgramUniformBlock);

						// Next!
						nUniformBlockIndex++;
					}
				} while (nCurrentNameLength);
			}
			/*
			{ // My first attempt
				// Get the number of active uniform blocks
				GLint nNumOfActiveUniformBlocks = 0;
				glGetObjectParameterivARB(nOpenGLProgram, GL_ACTIVE_UNIFORM_BLOCKS, &nNumOfActiveUniformBlocks);
				nNumOfActiveUniformBlocks = 1;
				if (nNumOfActiveUniformBlocks) {
					// Get the length of the longest active uniform block name, including a null terminator
					GLint nMaxNameLength = 0;
					glGetObjectParameterivARB(nOpenGLProgram, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &nMaxNameLength);
					if (nMaxNameLength) {
						// Allocate the name buffer
						char *pszName = new char[nMaxNameLength];

						// Iterate over all uniform blocks
						for (int i=0; i<nNumOfActiveUniformBlocks; i++) {
							// Get the current uniform block
							GLsizei nCurrentNameLength = 0;	// The actual number of characters written into pszName, excluding the null terminator
							glGetActiveUniformBlockName(nOpenGLProgram, i, nMaxNameLength, &nCurrentNameLength, pszName);
							GLint nUniformBlockSize = 0;
							glGetActiveUniformBlockiv(nOpenGLProgram, i, GL_UNIFORM_BLOCK_DATA_SIZE, &nUniformBlockSize);

							// Create a new program uniform block instance
							ProgramUniformBlockGLSL *pProgramUniformBlock = new ProgramUniformBlockGLSL(nOpenGLProgram, glGetUniformBlockIndex(nOpenGLProgram, pszName), nUniformBlockSize);

							// Register the new program uniform block
							m_lstUniformBlocks.Add(pProgramUniformBlock);
							m_mapUniformBlocks.Add(String(pszName, true, nCurrentNameLength), pProgramUniformBlock);
						}

						// Destroy the name buffer
						delete [] pszName;
					}
				}
			}*/

			// The uniform block information build is now build
			m_bUniformBlockInformationBuild = true;
		}
	}
}

/**
*  @brief
*    Destroys the uniform block information
*/
void ProgramGLSL::DestroyUniformBlockInformation()
{
	// Is there anything to destroy?
	const uint32 nNumOfUniformBlocks = m_lstUniformBlocks.GetNumOfElements();
	if (nNumOfUniformBlocks) {
		// Destroy the uniform block instances
		for (uint32 i=0; i<nNumOfUniformBlocks; i++)
			delete static_cast<ProgramUniformBlockGLSL*>(m_lstUniformBlocks[i]);

		// Clear the uniform block list and hash map
		m_lstUniformBlocks.Clear();
		m_mapUniformBlocks.Clear();
	}

	// Currently, there's no uniform block information build
	m_bUniformBlockInformationBuild = false;
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::Program functions          ]
//[-------------------------------------------------------]
String ProgramGLSL::GetShaderLanguage() const
{
	return ShaderLanguageGLSL::GLSL;
}

PLRenderer::VertexShader *ProgramGLSL::GetVertexShader() const
{
	return static_cast<PLRenderer::VertexShader*>(m_cVertexShaderHandler.GetResource());
}

bool ProgramGLSL::SetVertexShader(PLRenderer::VertexShader *pVertexShader)
{
	// Is the new vertex shader the same one as the current one?
	PLRenderer::VertexShader *pCurrentVertexShader = static_cast<PLRenderer::VertexShader*>(m_cVertexShaderHandler.GetResource());
	if (pCurrentVertexShader != pVertexShader) {
		// The shader language of the program and the vertex shader must match
		if (pVertexShader && pVertexShader->GetShaderLanguage() != ShaderLanguageGLSL::GLSL)
			return false; // Error, shader language mismatch!

		// Detach the current vertex shader from the program
		if (pCurrentVertexShader && m_bVertexShaderAttached)
			glDetachObjectARB(m_nOpenGLProgram, static_cast<VertexShaderGLSL*>(pCurrentVertexShader)->GetOpenGLVertexShader());

		// Update the vertex shader resource handler
		m_cVertexShaderHandler.SetResource(pVertexShader);

		// Attached within "ProgramGLSL::GetOpenGLProgram()" on demand
		m_bVertexShaderAttached = false;

		// Denote that a program relink is required
		RelinkRequired();
	}

	// Done
	return true;
}

PLRenderer::TessellationControlShader *ProgramGLSL::GetTessellationControlShader() const
{
	return static_cast<PLRenderer::TessellationControlShader*>(m_cTessellationControlShaderHandler.GetResource());
}

bool ProgramGLSL::SetTessellationControlShader(PLRenderer::TessellationControlShader *pTessellationControlShader)
{
	// Is the new tessellation control shader the same one as the current one?
	PLRenderer::TessellationControlShader *pCurrentTessellationControlShader = static_cast<PLRenderer::TessellationControlShader*>(m_cTessellationControlShaderHandler.GetResource());
	if (pCurrentTessellationControlShader != pTessellationControlShader) {
		// The shader language of the program and the tessellation control shader must match
		if (pTessellationControlShader && pTessellationControlShader->GetShaderLanguage() != ShaderLanguageGLSL::GLSL)
			return false; // Error, shader language mismatch!

		// Detach the current tessellation control shader from the program
		if (pCurrentTessellationControlShader && m_bTessellationControlShaderAttached)
			glDetachObjectARB(m_nOpenGLProgram, static_cast<TessellationControlShaderGLSL*>(pCurrentTessellationControlShader)->GetOpenGLTessellationControlShader());

		// Update the tessellation control shader resource handler
		m_cTessellationControlShaderHandler.SetResource(pTessellationControlShader);

		// Attached within "ProgramGLSL::GetOpenGLProgram()" on demand
		m_bTessellationControlShaderAttached = false;

		// Denote that a program relink is required
		RelinkRequired();
	}

	// Done
	return true;
}

PLRenderer::TessellationEvaluationShader *ProgramGLSL::GetTessellationEvaluationShader() const
{
	return static_cast<PLRenderer::TessellationEvaluationShader*>(m_cTessellationEvaluationShaderHandler.GetResource());
}

bool ProgramGLSL::SetTessellationEvaluationShader(PLRenderer::TessellationEvaluationShader *pTessellationEvaluationShader)
{
	// Is the new tessellation evaluation shader the same one as the current one?
	PLRenderer::TessellationEvaluationShader *pCurrentTessellationEvaluationShader = static_cast<PLRenderer::TessellationEvaluationShader*>(m_cTessellationEvaluationShaderHandler.GetResource());
	if (pCurrentTessellationEvaluationShader != pTessellationEvaluationShader) {
		// The shader language of the program and the tessellation evaluation shader must match
		if (pTessellationEvaluationShader && pTessellationEvaluationShader->GetShaderLanguage() != ShaderLanguageGLSL::GLSL)
			return false; // Error, shader language mismatch!

		// Detach the current tessellation evaluation shader from the program
		if (pCurrentTessellationEvaluationShader && m_bTessellationEvaluationShaderAttached)
			glDetachObjectARB(m_nOpenGLProgram, static_cast<TessellationEvaluationShaderGLSL*>(pCurrentTessellationEvaluationShader)->GetOpenGLTessellationEvaluationShader());

		// Update the tessellation evaluation shader resource handler
		m_cTessellationEvaluationShaderHandler.SetResource(pTessellationEvaluationShader);

		// Attached within "ProgramGLSL::GetOpenGLProgram()" on demand
		m_bTessellationEvaluationShaderAttached = false;

		// Denote that a program relink is required
		RelinkRequired();
	}

	// Done
	return true;
}

PLRenderer::GeometryShader *ProgramGLSL::GetGeometryShader() const
{
	return static_cast<PLRenderer::GeometryShader*>(m_cGeometryShaderHandler.GetResource());
}

bool ProgramGLSL::SetGeometryShader(PLRenderer::GeometryShader *pGeometryShader)
{
	// Is the new geometry shader the same one as the current one?
	PLRenderer::GeometryShader *pCurrentGeometryShader = static_cast<PLRenderer::GeometryShader*>(m_cGeometryShaderHandler.GetResource());
	if (pCurrentGeometryShader != pGeometryShader) {
		// The shader language of the program and the geometry shader must match
		if (pGeometryShader && pGeometryShader->GetShaderLanguage() != ShaderLanguageGLSL::GLSL)
			return false; // Error, shader language mismatch!

		// Detach the current geometry shader from the program
		if (pCurrentGeometryShader && m_bGeometryShaderAttached)
			glDetachObjectARB(m_nOpenGLProgram, static_cast<GeometryShaderGLSL*>(pCurrentGeometryShader)->GetOpenGLGeometryShader());

		// Update the geometry shader resource handler
		m_cGeometryShaderHandler.SetResource(pGeometryShader);

		// Attached within "ProgramGLSL::GetOpenGLProgram()" on demand
		m_bGeometryShaderAttached = false;

		// Set the input primitive type
		GLint nTemp;
		switch (pGeometryShader->GetInputPrimitiveType()) {
			case PLRenderer::GeometryShader::InputPoints:
				nTemp = GL_POINTS;
				break;

			case PLRenderer::GeometryShader::InputLines:
				nTemp = GL_LINES;
				break;

			case PLRenderer::GeometryShader::InputLinesAdjacency:
				nTemp = GL_LINES_ADJACENCY_EXT;
				break;

			case PLRenderer::GeometryShader::InputTriangles:
				nTemp = GL_TRIANGLES;
				break;

			case PLRenderer::GeometryShader::InputTrianglesAdjacency:
				nTemp = GL_TRIANGLES_ADJACENCY_EXT;
				break;

			default:
				nTemp = GL_TRIANGLES;
				break;
		}
		glProgramParameteriEXT(m_nOpenGLProgram, GL_GEOMETRY_INPUT_TYPE_EXT, nTemp);

		// Set the output primitive type
		switch (pGeometryShader->GetOutputPrimitiveType()) {
			case PLRenderer::GeometryShader::OutputPoints:
				nTemp = GL_POINTS;
				break;

			case PLRenderer::GeometryShader::OutputLines:
				nTemp = GL_LINE_STRIP;
				break;

			case PLRenderer::GeometryShader::OutputTriangles:
				nTemp = GL_TRIANGLE_STRIP;
				break;

			default:
				nTemp = GL_TRIANGLE_STRIP;
				break;
		}
		glProgramParameteriEXT(m_nOpenGLProgram, GL_GEOMETRY_OUTPUT_TYPE_EXT, nTemp);

		// Set the number of output vertices
		nTemp = pGeometryShader->GetNumOfOutputVertices();
		if (!nTemp)
			glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &nTemp);	// Maximum possible number of output vertices should be used
		glProgramParameteriEXT(m_nOpenGLProgram, GL_GEOMETRY_VERTICES_OUT_EXT, nTemp);

		// Denote that a program relink is required
		RelinkRequired();
	}

	// Done
	return true;
}

PLRenderer::FragmentShader *ProgramGLSL::GetFragmentShader() const
{
	return static_cast<PLRenderer::FragmentShader*>(m_cFragmentShaderHandler.GetResource());
}

bool ProgramGLSL::SetFragmentShader(PLRenderer::FragmentShader *pFragmentShader)
{
	// Is the new fragment shader the same one as the current one?
	PLRenderer::FragmentShader *pCurrentFragmentShader = static_cast<PLRenderer::FragmentShader*>(m_cFragmentShaderHandler.GetResource());
	if (pCurrentFragmentShader != pFragmentShader) {
		// The shader language of the program and the fragment shader must match
		if (pFragmentShader && pFragmentShader->GetShaderLanguage() != ShaderLanguageGLSL::GLSL)
			return false; // Error, shader language mismatch!

		// Detach the current fragment shader from the program
		if (pCurrentFragmentShader && m_bFragmentShaderAttached)
			glDetachObjectARB(m_nOpenGLProgram, static_cast<FragmentShaderGLSL*>(pCurrentFragmentShader)->GetOpenGLFragmentShader());

		// Update the fragment shader resource handler
		m_cFragmentShaderHandler.SetResource(pFragmentShader);

		// Attached within "ProgramGLSL::GetOpenGLProgram()" on demand
		m_bFragmentShaderAttached = false;

		// Denote that a program relink is required
		RelinkRequired();
	}

	// Done
	return true;
}

bool ProgramGLSL::IsValid()
{
	// Get the OpenGL program - this also ensures that the program is linked
	GetOpenGLProgram();

	// Is the OpenGL program linked?
	return IsLinked();
}

String ProgramGLSL::GetCompiledProgram()
{
	// GL_ARB_get_program_binary extension required
	if (static_cast<Renderer&>(GetRenderer()).GetContext().GetExtensions().IsGL_ARB_get_program_binary()) {
		// Get the OpenGL program - this also ensures that the program is linked
		const GLuint nOpenGLProgram = GetOpenGLProgram();

		// Is the OpenGL program linked?
		if (IsLinked()) {
			// Retrieve the binary length from the program object
			GLint nBinaryLength = 0;
			// [TODO] Check this! For me, this function just returns always 0? (Catalyst 11.3 - there are forum posts reporting similar problems, a driver issue... same on NVIDIA (driver: "266.58 WHQL")?)
			glGetProgramivARB(nOpenGLProgram, GL_PROGRAM_BINARY_LENGTH, &nBinaryLength);
			if (nBinaryLength) {
				// Allocate enough memory for the binary
				char *pszBinary = new char[nBinaryLength];

				// Get program binary formats
				GLint nNumProgramBinaryFormats = 0;
				glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &nNumProgramBinaryFormats);
				GLint *pnProgramBinaryFormags = new GLint[nNumProgramBinaryFormats];
				glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, pnProgramBinaryFormags);

				// Retrieve the binary from the program object
				glGetProgramBinary(nOpenGLProgram, nBinaryLength, nullptr, reinterpret_cast<GLenum*>(pnProgramBinaryFormags), pszBinary);

				// Cleanup program binary formats
				delete [] pnProgramBinaryFormags;

				// Return the retrieve the binary, do not copy, please
				return String(pszBinary, false, nBinaryLength);
			}
		}
	}

	// Error!
	return "";
}

const Array<PLRenderer::ProgramAttribute*> &ProgramGLSL::GetAttributes()
{
	// Build the attribute information, if required
	if (!m_bAttributeInformationBuild)
		BuildAttributeInformation();

	// Return the list of all attributes
	return m_lstAttributes;
}

PLRenderer::ProgramAttribute *ProgramGLSL::GetAttribute(const String &sName)
{
	// Build the attribute information, if required
	if (!m_bAttributeInformationBuild)
		BuildAttributeInformation();

	// Return the requested attribute
	return m_mapAttributes.Get(sName);
}

const Array<PLRenderer::ProgramUniform*> &ProgramGLSL::GetUniforms()
{
	// Build the uniform information, if required
	if (!m_bUniformInformationBuild)
		BuildUniformInformation();

	// Return the list of all uniforms
	return m_lstUniforms;
}

PLRenderer::ProgramUniform *ProgramGLSL::GetUniform(const String &sName)
{
	// Build the uniform information, if required
	if (!m_bUniformInformationBuild)
		BuildUniformInformation();

	// Return the requested uniform
	return m_mapUniforms.Get(sName);
}

const Array<PLRenderer::ProgramUniformBlock*> &ProgramGLSL::GetUniformBlocks()
{
	// Build the uniform block information, if required
	if (!m_bUniformBlockInformationBuild)
		BuildUniformBlockInformation();

	// Return the list of all uniform blocks
	return m_lstUniformBlocks;
}

PLRenderer::ProgramUniformBlock *ProgramGLSL::GetUniformBlock(const String &sName)
{
	// Build the uniform block information, if required
	if (!m_bUniformBlockInformationBuild)
		BuildUniformBlockInformation();

	// Return the requested uniform block
	return m_mapUniformBlocks.Get(sName);
}


//[-------------------------------------------------------]
//[ Protected virtual PLRenderer::Program functions       ]
//[-------------------------------------------------------]
bool ProgramGLSL::MakeCurrent()
{
	// Get the OpenGL program - this also ensures that the program is linked
	const GLuint nOpenGLProgram = GetOpenGLProgram();

	// Is the OpenGL program linked?
	if (IsLinked()) {
		// Make this to the currently used program
		glUseProgramObjectARB(nOpenGLProgram);

		// Build the attribute information, if required
		if (!m_bAttributeInformationBuild)
			BuildAttributeInformation();

		// Enable all vertex attribute arrays
		for (uint32 i=0; i<m_lstAttributes.GetNumOfElements(); i++)
			glEnableVertexAttribArrayARB(static_cast<ProgramAttributeGLSL*>(m_lstAttributes[i])->m_nOpenGLAttributeLocation);

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}

bool ProgramGLSL::UnmakeCurrent()
{
	// Disable all vertex attribute arrays
	for (uint32 i=0; i<m_lstAttributes.GetNumOfElements(); i++)
		glDisableVertexAttribArrayARB(static_cast<ProgramAttributeGLSL*>(m_lstAttributes[i])->m_nOpenGLAttributeLocation);

	// Currently, no program is set
	glUseProgramObjectARB(0);

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Private virtual PLRenderer::Resource functions        ]
//[-------------------------------------------------------]
void ProgramGLSL::BackupDeviceData(uint8 **ppBackup)
{
	// Is there currently a OpenGL program?
	if (m_nOpenGLProgram) {
		// Destroy the OpenGL program
		glDeleteObjectARB(m_nOpenGLProgram);
		m_nOpenGLProgram = 0;
	}
}

void ProgramGLSL::RestoreDeviceData(uint8 **ppBackup)
{
	if (!m_nOpenGLProgram) {
		m_nOpenGLProgram = glCreateProgramObjectARB();
		m_bVertexShaderAttached = m_bTessellationControlShaderAttached = m_bTessellationEvaluationShaderAttached = m_bGeometryShaderAttached = m_bFragmentShaderAttached = false;

		// Call the relink method, this automatically destroys the used resources and the user will be informed through an dirty-event
		RelinkRequired();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL
