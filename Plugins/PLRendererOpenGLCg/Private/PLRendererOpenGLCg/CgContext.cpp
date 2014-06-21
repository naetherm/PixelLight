/*********************************************************\
 *  File: CgContext.cpp                                  *
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
#include <PLCore/Log/Log.h>
#include "PLRendererOpenGLCg/CgContext.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRendererOpenGLCg {


//[-------------------------------------------------------]
//[ Private static data                                   ]
//[-------------------------------------------------------]
CGcontext CgContext::m_pCgContext       = nullptr;
uint32    CgContext::m_nCgContexCounter = 0;


//[-------------------------------------------------------]
//[ Public static methods                                 ]
//[-------------------------------------------------------]
/**
*  @brief
*    Adds a Cg context reference
*/
void CgContext::AddCgContextReference()
{
	// Check Cg context
	if (!m_nCgContexCounter && !m_pCgContext) {
		PL_LOG(Info, String("Initialize Cg ") + CG_VERSION_NUM)
		m_pCgContext = cgCreateContext();
		cgSetErrorCallback(CgErrorCallback);

		{ // Enumerate the supported Cg profiles and print the information into the log
			const int nNumSupportedProfiles = cgGetNumSupportedProfiles();
			PL_LOG(Info, "Supported Cg profiles:")
			for (int i=0; i<nNumSupportedProfiles; i++) {
				CGprofile pCGProfile = cgGetSupportedProfile(i);
				if (cgGLIsProfileSupported(pCGProfile))
					PL_LOG(Info, String("- ") + cgGetProfileString(pCGProfile))
			}
		}

		// [TEST] Disable debug mode for better performance
//		cgGLSetDebugMode(CG_FALSE);

		// Delay program recompilation until the program object code is needed
		cgSetAutoCompile(m_pCgContext, CG_COMPILE_LAZY);

		// Our renderer is not multi-threaded, so, avoid lock overhead
		cgSetLockingPolicy(CG_NO_LOCKS_POLICY);
	}
	m_nCgContexCounter++;
}

/**
*  @brief
*    Releases a Cg context reference
*/
void CgContext::ReleaseCgContextReference()
{
	// Check Cg context
	m_nCgContexCounter--;
	if (!m_nCgContexCounter && m_pCgContext) {
		PL_LOG(Info, String("De-initialize Cg ") + CG_VERSION_NUM)
		cgSetErrorCallback(nullptr); // Just for sure :)
		cgDestroyContext(m_pCgContext);
		m_pCgContext = nullptr;
	}
}

/**
*  @brief
*    Returns the Cg context
*/
CGcontext CgContext::GetCgContext()
{
	return m_pCgContext;
}

/**
*  @brief
*    Creates a Cg program
*/
CGprogram CgContext::CreateCgProgram(CGprofile pCgProfile, const String &sSourceCode, const String &sArguments, const String &sEntry)
{
	// Set optimal profile options - this will set implicit compiler arguments chosen based on the the available compiler arguments, GPU, and driver
	cgGLSetOptimalOptions(pCgProfile);

	// Create the program
	const char *pszArguments[2] = { sArguments.GetLength() ? sArguments.GetASCII() : nullptr, nullptr };
	CGprogram pCgProgram = cgCreateProgram(m_pCgContext, CG_SOURCE, sSourceCode.GetASCII(), pCgProfile, sEntry.GetLength() ? sEntry.GetASCII() : "main", pszArguments);
	if (!pCgProgram) {
		// Error!
		PL_LOG(Error, String("Can't create Cg program! Error: ") + cgGetErrorString(cgGetError()))
	}

	// Done
	return pCgProgram;
}


//[-------------------------------------------------------]
//[ Private static Cg callback functions                  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Internal Cg error callback function
*/
void CgContext::CgErrorCallback()
{
	CGerror nLastError = cgGetError();
	if (nLastError) {
		const char *pszListing = cgGetLastListing(m_pCgContext);
		PL_LOG(Error, "\n---------------------------------------------------")
		PL_LOG(Error, cgGetErrorString(nLastError))
		if (pszListing)
			PL_LOG(Error, pszListing)
		PL_LOG(Error, "---------------------------------------------------\n")
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGLCg
