/*********************************************************\
 *  File: GeometryShaderCg.cpp                           *
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
#include <PLCore/Tools/Wrapper.h>
#include <PLCore/Core/MemoryManager.h>
#include "PLRendererOpenGLCg/ShaderLanguageCg.h"
#include "PLRendererOpenGLCg/CgContext.h"
#include "PLRendererOpenGLCg/GeometryShaderCg.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRendererOpenGLCg {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
GeometryShaderCg::~GeometryShaderCg()
{
	// Destroy the Cg geometry program
	if (m_pCgGeometryProgram)
		cgDestroyProgram(m_pCgGeometryProgram);

	// Release a Cg context reference
	CgContext::ReleaseCgContextReference();
}

/**
*  @brief
*    Returns the Cg profile
*/
CGprofile GeometryShaderCg::GetCgProfile() const
{
	return m_pCgProfile;
}

/**
*  @brief
*    Returns the Cg geometry program
*/
CGprogram GeometryShaderCg::GetCgGeometryProgram() const
{
	return m_pCgGeometryProgram;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
GeometryShaderCg::GeometryShaderCg(PLRenderer::Renderer &cRenderer) : PLRenderer::GeometryShader(cRenderer),
	m_nInputPrimitiveType(InputTriangles),
	m_nOutputPrimitiveType(OutputTriangles),
	m_nNumOfOutputVertices(0),
	m_pCgProfile(CG_PROFILE_UNKNOWN),
	m_pCgGeometryProgram(nullptr)
{
	// Add a Cg context reference
	CgContext::AddCgContextReference();
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::GeometryShader functions   ]
//[-------------------------------------------------------]
GeometryShaderCg::EInputPrimitiveType GeometryShaderCg::GetInputPrimitiveType() const
{
	return m_nInputPrimitiveType;
}

GeometryShaderCg::EOutputPrimitiveType GeometryShaderCg::GetOutputPrimitiveType() const
{
	return m_nOutputPrimitiveType;
}

uint32 GeometryShaderCg::GetNumOfOutputVertices() const
{
	return m_nNumOfOutputVertices;
}

bool GeometryShaderCg::SetSourceCode(const String &sSourceCode, EInputPrimitiveType nInputPrimitiveType, EOutputPrimitiveType nOutputPrimitiveType, uint32 nNumOfOutputVertices, const String &sProfile, const String &sArguments, const String &sEntry)
{
	// Backup the input/output primitive type and the number of output vertices
	m_nInputPrimitiveType  = nInputPrimitiveType;
	m_nOutputPrimitiveType = nOutputPrimitiveType;
	m_nNumOfOutputVertices = nNumOfOutputVertices;

	// Destroy the previous Cg geometry program, if there's one
	if (m_pCgGeometryProgram) {
		cgDestroyProgram(m_pCgGeometryProgram);
		m_pCgGeometryProgram = nullptr;
	}

	// Get the profile from a user given string
	m_pCgProfile = cgGetProfile(sProfile.GetLength() ? sProfile : "glslg"); // We're using a GLSL profile as default so ATI users have reasonable shader support when using Cg

	// On unknown or invalid profile, choose a fallback profile
	if (m_pCgProfile == CG_PROFILE_UNKNOWN || (!cgGetProfileProperty(m_pCgProfile, CG_IS_GEOMETRY_PROFILE) && m_pCgProfile != CG_PROFILE_GLSLG)) {
		m_pCgProfile = cgGLGetLatestProfile(CG_GL_GEOMETRY);
		if (m_pCgProfile == CG_PROFILE_UNKNOWN && cgGLIsProfileSupported(CG_PROFILE_GLSLG))
			m_pCgProfile = CG_PROFILE_GLSLG;
	}

	// Create the Cg geometry program
	if (m_pCgProfile != CG_PROFILE_UNKNOWN) {
		// Ok, the next thing is definitively no fun in Cg because it looks like there's no unified way to do it:
		// Set the input/output primitive type and the number of output vertices
		String sComposedArguments = sArguments;
		sComposedArguments += ' ';
		switch (m_pCgProfile) {
			// GLSL geometry shader
			case CG_PROFILE_GLSLG:
				// Can't be handled in here, have a look at "ProgramCg::GetCgCombinedProgram()"
				// (... at least the Cg documentation doesn't list any GLSL geometry shader attributes, wasn't
				// able to find any information on this topic in the Internet either, so, I just "assume" that
				// one has to use "glProgramParameteriEXT" ...)
				break;

			// NV_gpu_program4 geometry program
			case CG_PROFILE_GP4GP:
			// NV_gpu_program5 geometry program
			case CG_PROFILE_GP5GP:
			// ... as well as later coming profiles...
			default:
				// Set the input primitive type
				switch (m_nInputPrimitiveType) {
					case InputPoints:
						sComposedArguments += "-po POINT ";
						break;

					case InputLines:
						sComposedArguments += "-po LINE ";
						break;

					case InputLinesAdjacency:
						sComposedArguments += "-po LINE_ADJ ";
						break;

					case InputTriangles:
						sComposedArguments += "-po TRIANGLE ";
						break;

					case InputTrianglesAdjacency:
						sComposedArguments += "-po TRIANGLE_ADJ ";
						break;

					default:
						sComposedArguments += "-po TRIANGLE ";
						break;
				}

				// Set the output primitive type
				switch (m_nOutputPrimitiveType) {
					case OutputPoints:
						sComposedArguments += "-po POINT_OUT ";
						break;

					case OutputLines:
						sComposedArguments += "-po LINE_OUT ";
						break;

					case OutputTriangles:
						sComposedArguments += "-po TRIANGLE_OUT ";
						break;

					default:
						sComposedArguments += "-po TRIANGLE_OUT ";
						break;
				}

				// Set the number of output vertices
				sComposedArguments += "-po Vertices=";
				sComposedArguments += m_nNumOfOutputVertices;
				break;
		}

		// Create the Cg geometry program
		m_pCgGeometryProgram = CgContext::CreateCgProgram(m_pCgProfile, sSourceCode, sComposedArguments, sEntry);
	}

	// Was the Cg program created successfully?
	if (m_pCgGeometryProgram) {
		// Backup the optional shader compiler arguments and the user defined entry point
		m_sArguments = sArguments;
		m_sEntry     = sEntry;

		// Done
		return true;
	} else {
		m_pCgProfile = CG_PROFILE_UNKNOWN;
		m_sArguments = "";
		m_sEntry     = "";

		// Error!
		return false;
	}
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::Shader functions           ]
//[-------------------------------------------------------]
String GeometryShaderCg::GetShaderLanguage() const
{
	return ShaderLanguageCg::Cg;
}

String GeometryShaderCg::GetSourceCode() const
{
	return m_pCgGeometryProgram ? cgGetProgramString(m_pCgGeometryProgram, CG_PROGRAM_SOURCE) : "";
}

String GeometryShaderCg::GetProfile() const
{
	return cgGetProfileString(m_pCgProfile);
}

String GeometryShaderCg::GetArguments() const
{
	return m_sArguments;
}

String GeometryShaderCg::GetEntry() const
{
	return m_sEntry;
}

bool GeometryShaderCg::SetSourceCode(const String &sSourceCode, const String &sProfile, const String &sArguments, const String &sEntry)
{
	// Call the extended version of "Shader::SetSourceCode()" for geometry shaders with default settings
	return SetSourceCode(sSourceCode, InputTriangles, OutputTriangles, 0, sProfile, m_sArguments, sEntry);
}


//[-------------------------------------------------------]
//[ Private virtual PLRenderer::Resource functions        ]
//[-------------------------------------------------------]
void GeometryShaderCg::BackupDeviceData(uint8 **ppBackup)
{
	// Backup data
	const char *pszProgram = m_pCgGeometryProgram ? cgGetProgramString(m_pCgGeometryProgram, CG_PROGRAM_SOURCE) : nullptr;
	if (pszProgram) {
		const uint32 nNumOfBytes = Wrapper::GetStringLength(pszProgram) + 1;
		*ppBackup = new uint8[nNumOfBytes];
		MemoryManager::Copy(*ppBackup, pszProgram, nNumOfBytes);
		cgDestroyProgram(m_pCgGeometryProgram);
		m_pCgGeometryProgram = nullptr;
	} else {
		*ppBackup = nullptr;
	}
}

void GeometryShaderCg::RestoreDeviceData(uint8 **ppBackup)
{
	// Restore data
	if (*ppBackup) {
		// The string class takes over the control of the string memory and also deletes it
		SetSourceCode(String(reinterpret_cast<char*>(*ppBackup), false), GetProfile(), m_sArguments, m_sEntry);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGLCg
