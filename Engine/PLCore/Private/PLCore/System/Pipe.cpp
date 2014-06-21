/*********************************************************\
 *  File: Pipe.cpp                                       *
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
#if defined(WIN32)
	#include "PLCore/+Windows/PLCoreWindowsIncludes.h"
#elif defined(LINUX)
	#include <unistd.h>
#endif
#include "PLCore/System/Pipe.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Creates a new unnamend pipe
*/
bool Pipe::Create()
{
	// Unnamed pipe
	m_sName = "";

	// Windows implementation
	#ifdef WIN32
		// Set security attributes
		SECURITY_ATTRIBUTES sSecAttr;
		sSecAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
		sSecAttr.bInheritHandle = TRUE;
		sSecAttr.lpSecurityDescriptor = nullptr;

		// Create pipe
		return (CreatePipe((HANDLE*)&m_hPipe[0], (HANDLE*)&m_hPipe[1], &sSecAttr, 0) != 0);
	#endif

	// Linux implementation
	#ifdef LINUX
		// Create a pipe
		return (pipe(reinterpret_cast<int*>(m_hPipe)) == 0);
	#endif
}

/**
*  @brief
*    Creates a new namend pipe
*/
bool Pipe::Create(const String &sName)
{
	// Save name
	m_sName = sName;

	// Windows implementation
	#ifdef WIN32
		// Set security attributes
		SECURITY_ATTRIBUTES sSecAttr;
		sSecAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
		sSecAttr.bInheritHandle = TRUE;
		sSecAttr.lpSecurityDescriptor = nullptr;

		// Create write end of the pipe
		if (sName.GetFormat() == String::ASCII)
			m_hPipe[1] = (handle)CreateNamedPipeA(sName.GetASCII(), PIPE_ACCESS_OUTBOUND, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 1, 0, 0, NMPWAIT_USE_DEFAULT_WAIT, &sSecAttr);
		else
			m_hPipe[1] = (handle)CreateNamedPipeW(sName.GetUnicode(), PIPE_ACCESS_OUTBOUND, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 1, 0, 0, NMPWAIT_USE_DEFAULT_WAIT, &sSecAttr);

		// Create read end of the pipe
		if (sName.GetFormat() == String::ASCII)
			m_hPipe[0] = (handle)CreateFileA(sName.GetASCII(), GENERIC_READ, 0, &sSecAttr, OPEN_EXISTING, 0, nullptr);
		else
			m_hPipe[0] = (handle)CreateFileW(sName.GetUnicode(), GENERIC_READ, 0, &sSecAttr, OPEN_EXISTING, 0, nullptr);

		// Return result
		return ((HANDLE)m_hPipe[0] != INVALID_HANDLE_VALUE && (HANDLE)m_hPipe[1] != INVALID_HANDLE_VALUE);
	#endif

	// Linux implementation
	#ifdef LINUX
		// [TODO]
		return false;
	#endif
}

/**
*  @brief
*    Close read side of the pipe
*/
bool Pipe::CloseRead()
{
	// Check if handle has already been closed
	if (m_hPipe[0] == INVALID_HANDLE) {
		// Error!
		return false;
	}

	// Windows implementation
	#ifdef WIN32
		const BOOL bResult = CloseHandle((HANDLE)m_hPipe[0]);
		m_hPipe[0] = INVALID_HANDLE;
		return (bResult != FALSE);
	#endif

	// Linux implementation
	#ifdef LINUX
		close(m_hPipe[0]);
		m_hPipe[0] = INVALID_HANDLE;
		return true;
	#endif
}

/**
*  @brief
*    Close write side of the pipe
*/
bool Pipe::CloseWrite()
{
	// Check if handle has already been closed
	if (m_hPipe[1] == INVALID_HANDLE) {
		// Error!
		return false;
	}

	// Windows implementation
	#ifdef WIN32
		const BOOL bResult = CloseHandle((HANDLE)m_hPipe[1]);
		m_hPipe[1] = INVALID_HANDLE;
		return (bResult != FALSE);
	#endif

	// Linux implementation
	#ifdef LINUX
		close(m_hPipe[1]);
		m_hPipe[1] = INVALID_HANDLE;
		return true;
	#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
