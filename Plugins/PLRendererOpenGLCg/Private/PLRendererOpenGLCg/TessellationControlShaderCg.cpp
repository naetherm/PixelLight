/*********************************************************\
 *  File: TessellationControlShaderCg.cpp                *
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
#include "PLRendererOpenGLCg/TessellationControlShaderCg.h"


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
TessellationControlShaderCg::~TessellationControlShaderCg()
{
	// Destroy the Cg tessellation control program
	if (m_pCgTessellationControlProgram)
		cgDestroyProgram(m_pCgTessellationControlProgram);

	// Release a Cg context reference
	CgContext::ReleaseCgContextReference();
}

/**
*  @brief
*    Returns the Cg profile
*/
CGprofile TessellationControlShaderCg::GetCgProfile() const
{
	return m_pCgProfile;
}

/**
*  @brief
*    Returns the Cg tessellation control program
*/
CGprogram TessellationControlShaderCg::GetCgTessellationControlProgram() const
{
	return m_pCgTessellationControlProgram;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
TessellationControlShaderCg::TessellationControlShaderCg(PLRenderer::Renderer &cRenderer) : PLRenderer::TessellationControlShader(cRenderer),
	m_pCgProfile(CG_PROFILE_UNKNOWN),
	m_pCgTessellationControlProgram(nullptr)
{
	// Add a Cg context reference
	CgContext::AddCgContextReference();
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::Shader functions           ]
//[-------------------------------------------------------]
String TessellationControlShaderCg::GetShaderLanguage() const
{
	return ShaderLanguageCg::Cg;
}

String TessellationControlShaderCg::GetSourceCode() const
{
	return m_pCgTessellationControlProgram ? cgGetProgramString(m_pCgTessellationControlProgram, CG_PROGRAM_SOURCE) : "";
}

String TessellationControlShaderCg::GetProfile() const
{
	return cgGetProfileString(m_pCgProfile);
}

String TessellationControlShaderCg::GetArguments() const
{
	return m_sArguments;
}

String TessellationControlShaderCg::GetEntry() const
{
	return m_sEntry;
}

bool TessellationControlShaderCg::SetSourceCode(const String &sSourceCode, const String &sProfile, const String &sArguments, const String &sEntry)
{
	// Destroy the previous Cg tessellation control program, if there's one
	if (m_pCgTessellationControlProgram) {
		cgDestroyProgram(m_pCgTessellationControlProgram);
		m_pCgTessellationControlProgram = nullptr;
	}

	// Get the profile from a user given string
	// -> [TODO] "February 2012 version of Cg 3.1": No GLSL tessellation control shader support, at least one that's not using special NVIDIA-only extensions
	m_pCgProfile = cgGLGetLatestProfile(CG_GL_TESSELLATION_CONTROL);
//	m_pCgProfile = cgGetProfile(sProfile.GetLength() ? sProfile : "?"); // We're using a GLSL profile as default so ATI users have reasonable shader support when using Cg

	// On unknown or invalid profile, choose a fallback profile
	// [TODO] GLSL tessellation control program: "February 2012 version of Cg 3.1": No GLSL tessellation control shader support, at least one that's not using special NVIDIA-only extensions
	// -> Currently, Cg has only "CG_PROFILE_GP5TCP" (NV_tessellation_program5 tessellation control program)
//	if (m_pCgProfile == CG_PROFILE_UNKNOWN || (!cgGetProfileProperty(m_pCgProfile, CG_IS_TESSELLATION_CONTROL_PROFILE) && m_pCgProfile != ?)) {
//		m_pCgProfile = cgGLGetLatestProfile(CG_GL_TESSELLATION_CONTROL);
//		if (m_pCgProfile == CG_PROFILE_UNKNOWN && cgGLIsProfileSupported(?))
//			m_pCgProfile = ?;
//	}

	// Create the Cg tessellation control program
	if (m_pCgProfile != CG_PROFILE_UNKNOWN)
		m_pCgTessellationControlProgram = CgContext::CreateCgProgram(m_pCgProfile, sSourceCode, sArguments, sEntry);

	// Was the Cg program created successfully?
	if (m_pCgTessellationControlProgram) {
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
//[ Private virtual PLRenderer::Resource functions        ]
//[-------------------------------------------------------]
void TessellationControlShaderCg::BackupDeviceData(uint8 **ppBackup)
{
	// Backup data
	const char *pszProgram = m_pCgTessellationControlProgram ? cgGetProgramString(m_pCgTessellationControlProgram, CG_PROGRAM_SOURCE) : nullptr;
	if (pszProgram) {
		const uint32 nNumOfBytes = Wrapper::GetStringLength(pszProgram) + 1;
		*ppBackup = new uint8[nNumOfBytes];
		MemoryManager::Copy(*ppBackup, pszProgram, nNumOfBytes);
		cgDestroyProgram(m_pCgTessellationControlProgram);
		m_pCgTessellationControlProgram = nullptr;
	} else {
		*ppBackup = nullptr;
	}
}

void TessellationControlShaderCg::RestoreDeviceData(uint8 **ppBackup)
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
