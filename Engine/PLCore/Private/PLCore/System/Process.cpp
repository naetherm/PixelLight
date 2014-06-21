/*********************************************************\
 *  File: Process.cpp                                    *
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
	#include <signal.h>
#endif
#include "PLCore/String/Tokenizer.h"
#include "PLCore/Container/List.h"
#include "PLCore/System/Process.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
Process::~Process()
{
	// Windows implementation
	#ifdef WIN32
		if (m_hProcess)
			CloseHandle(reinterpret_cast<HANDLE>(m_hProcess));
	#endif

	// Linux implementation
	#ifdef LINUX
	#endif
}

/**
*  @brief
*    Starts a process
*/
void Process::Execute(const String &sCommand, const String &sArguments)
{
	// Windows implementation
	#ifdef WIN32
		// Create process
		PROCESS_INFORMATION piProcInfo;
		ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
		STARTUPINFO siStartInfo;
		ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
		siStartInfo.cb = sizeof(STARTUPINFO);
		String sCmdLine = sCommand + ' ' + sArguments;
		BOOL bResult = CreateProcess(
							nullptr,
							const_cast<LPWSTR>(sCmdLine.GetUnicode()),	// Command line
							nullptr,									// Process security attributes
							nullptr,									// Primary thread security attributes
							TRUE,										// Handles are inherited
							0,											// Creation flags
							nullptr,									// Use parent's environment
							nullptr,									// Use parent's current directory
							&siStartInfo,								// STARTUPINFO pointer
							&piProcInfo									// Receives PROCESS_INFORMATION
						);

		// Check result
		if (bResult) {
			m_hProcess = reinterpret_cast<handle>(piProcInfo.hProcess);
			CloseHandle(piProcInfo.hThread);
		}
	#endif

	// Linux implementation
	#ifdef LINUX
		// Create process
		m_hProcess = fork();
		if (m_hProcess == 0) {	// Child process
			// Get arguments
			List<String> lstArgs;
			lstArgs.Add(sCommand);
			Tokenizer tokenizer;
			tokenizer.SetSingleChars("");
			tokenizer.SetSingleLineComment("");
			tokenizer.Start(sArguments);
			String sToken = tokenizer.GetNextToken();
			while (sToken.GetLength()) {
				lstArgs.Add(sToken);
				sToken = tokenizer.GetNextToken();
			}

			// Make array for arguments
			const int nSize = lstArgs.GetNumOfElements();
			if (nSize > 0) {
				char **ppszParams = new char*[nSize+1];
				if (sCommand.GetFormat() == String::ASCII) {
					for (int i=0; i<nSize; i++)
						ppszParams[i] = const_cast<char*>(lstArgs[i].GetASCII());
				} else {
					for (int i=0; i<nSize; i++)
						ppszParams[i] = const_cast<char*>(lstArgs[i].GetUTF8());
				}
				ppszParams[nSize] = nullptr;

				// Execute application
				execv((lstArgs[0].GetFormat() == String::ASCII) ? lstArgs[0].GetASCII() : lstArgs[0].GetUTF8(), ppszParams);
			}
		}
	#endif
}

/**
*  @brief
*    Starts a process with redirected input/output
*/
void Process::ExecuteRedirectIO(const String &sCommand, const String &sArguments)
{
	// Create pipes
	m_cPipeInput .Create();
	m_cPipeOutput.Create();
	m_cPipeError = m_cPipeOutput;

	// Create process with pipes
	CreateProcessRedirectIO(
		sCommand,
		sArguments,
		m_cPipeInput .GetReadHandle(),
		m_cPipeInput .GetWriteHandle(),
		m_cPipeOutput.GetReadHandle(),
		m_cPipeOutput.GetWriteHandle(),
		m_cPipeError .GetReadHandle(),
		m_cPipeError .GetWriteHandle()
	);

	// Close pipe handles that are only used by the new process now
	m_cPipeInput .CloseRead();
	m_cPipeOutput.CloseWrite();
}

/**
*  @brief
*    Starts a process with redirected input/output
*/
void Process::ExecuteRedirectIO(const String &sCommand, const String &sArguments, const Pipe &cPipeIn, const Pipe &cPipeOut, const Pipe &cPipeErr)
{
	// Copy pipes
	m_cPipeInput  = cPipeIn;
	m_cPipeOutput = cPipeOut;
	m_cPipeError  = cPipeErr;

	// Create process with pipe handles
	CreateProcessRedirectIO(
		sCommand,
		sArguments,
		m_cPipeInput .GetReadHandle(),
		m_cPipeInput .GetWriteHandle(),
		m_cPipeOutput.GetReadHandle(),
		m_cPipeOutput.GetWriteHandle(),
		m_cPipeError .GetReadHandle(),
		m_cPipeError .GetWriteHandle()
	);

	// Close pipe handles that are only used by the new process now
	m_cPipeInput .CloseRead();
	m_cPipeOutput.CloseWrite();
	if (cPipeErr != cPipeOut)
		m_cPipeError.CloseWrite();
}

/**
*  @brief
*    Starts a process with redirected input/output
*/
void Process::ExecuteRedirectIO(const String &sCommand, const String &sArguments, handle hPipeIn, handle hPipeOut, handle hPipeErr)
{
	// Copy pipes
	m_cPipeInput .Open(hPipeIn, INVALID_HANDLE);
	m_cPipeOutput.Open(INVALID_HANDLE, hPipeOut);
	m_cPipeError .Open(INVALID_HANDLE, hPipeErr);

	// Create process with pipe handles
	CreateProcessRedirectIO(
		sCommand,
		sArguments,
		m_cPipeInput .GetReadHandle(),
		INVALID_HANDLE,
		INVALID_HANDLE,
		m_cPipeOutput.GetWriteHandle(),
		INVALID_HANDLE,
		m_cPipeError .GetWriteHandle()
	);
}

/**
*  @brief
*    Returns if the process is running
*/
bool Process::IsRunning() const
{
	// Windows implementation
	#ifdef WIN32
		// Check process handle
		return (m_hProcess != NULL_HANDLE);
	#endif

	// Linux implementation
	#ifdef LINUX
		return false;
	#endif
}

/**
*  @brief
*    Terminates the process
*/
void Process::Terminate()
{
	// Windows implementation
	#ifdef WIN32
		TerminateProcess(reinterpret_cast<HANDLE>(m_hProcess), 0);
		CloseHandle(reinterpret_cast<HANDLE>(m_hProcess));
		m_hProcess = NULL_HANDLE;
	#endif

	// Linux implementation
	#ifdef LINUX
		kill(m_hProcess, SIGKILL);
		m_hProcess = NULL_HANDLE;
	#endif

	// Reset streams and files
	m_cPipeInput .Open(INVALID_HANDLE, INVALID_HANDLE);
	m_cPipeOutput.Open(INVALID_HANDLE, INVALID_HANDLE);
	m_cPipeError .Open(INVALID_HANDLE, INVALID_HANDLE);
	m_cFileInput .Assign("");
	m_cFileOutput.Assign("");
	m_cFileError .Assign("");
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Process::Process(const Process &cSource) :
	m_hProcess(NULL_HANDLE)
{
}

/**
*  @brief
*    Copy operator
*/
Process &Process::operator =(const Process &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}

/**
*  @brief
*    Creates a process with redirected IO
*/
bool Process::CreateProcessRedirectIO(const String &sCommand, const String &sArguments, handle hPipeInRd, handle hPipeInWr, handle hPipeOutRd, handle hPipeOutWr, handle hPipeErrRd, handle hPipeErrWr)
{
	// Windows implementation
	#ifdef WIN32
		// Create process
		PROCESS_INFORMATION piProcInfo;
		ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
		const String sCmdLine = sCommand + ' ' + sArguments;
		BOOL bResult = FALSE;
		if (sCmdLine.GetFormat() == String::ASCII) {
			STARTUPINFOA siStartInfo;
			ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
			siStartInfo.cb = sizeof(STARTUPINFO);
			siStartInfo.hStdInput  = reinterpret_cast<HANDLE>(hPipeInRd);
			siStartInfo.hStdOutput = reinterpret_cast<HANDLE>(hPipeOutWr);
			siStartInfo.hStdError  = reinterpret_cast<HANDLE>(hPipeErrWr);
			siStartInfo.dwFlags = STARTF_USESTDHANDLES;
			bResult = CreateProcessA(
							nullptr,
							const_cast<LPSTR>(sCmdLine.GetASCII()),	// Command line
							nullptr,								// Process security attributes
							nullptr,								// Primary thread security attributes
							TRUE,									// Handles are inherited
							0,										// Creation flags
							nullptr,								// Use parent's environment
							nullptr,								// Use parent's current directory
							&siStartInfo,							// STARTUPINFO pointer
							&piProcInfo								// Receives PROCESS_INFORMATION
						);
		} else {
			STARTUPINFOW siStartInfo;
			ZeroMemory(&siStartInfo, sizeof(STARTUPINFOW));
			siStartInfo.cb = sizeof(STARTUPINFOW);
			siStartInfo.hStdInput  = reinterpret_cast<HANDLE>(hPipeInRd);
			siStartInfo.hStdOutput = reinterpret_cast<HANDLE>(hPipeOutWr);
			siStartInfo.hStdError  = reinterpret_cast<HANDLE>(hPipeErrWr);
			siStartInfo.dwFlags = STARTF_USESTDHANDLES;
			bResult = CreateProcessW(
							nullptr,
							const_cast<LPWSTR>(sCmdLine.GetUnicode()),	// Command line
							nullptr,									// Process security attributes
							nullptr,									// Primary thread security attributes
							TRUE,										// Handles are inherited
							0,											// Creation flags
							nullptr,									// Use parent's environment
							nullptr,									// Use parent's current directory
							&siStartInfo,								// STARTUPINFO pointer
							&piProcInfo									// Receives PROCESS_INFORMATION
						);
		}

		// Check result
		if (bResult) {
			m_hProcess = reinterpret_cast<handle>(piProcInfo.hProcess);
			CloseHandle(piProcInfo.hThread);
		}

		// Connect pipes to files
		m_cFileInput .Assign(m_cPipeInput .GetWriteHandle());
		m_cFileOutput.Assign(m_cPipeOutput.GetReadHandle());
		m_cFileError .Assign(m_cPipeError .GetReadHandle());

		// Return result
		return (bResult != 0);
	#endif

	// Linux implementation
	#ifdef LINUX
		// Create process
		m_hProcess = fork();
		if (m_hProcess == 0) {	// Child process
			dup2(hPipeInRd, 0);		// stdin
			dup2(hPipeOutWr, 1);	// stdout
			dup2(hPipeErrWr, 2);	// stderr
			if (hPipeInWr  != INVALID_HANDLE)
				close(hPipeInWr);
			if (hPipeOutRd != INVALID_HANDLE)
				close(hPipeOutRd);
			if (hPipeErrRd != INVALID_HANDLE)
				close(hPipeErrRd);

			// Get arguments
			List<String> lstArgs;
			lstArgs.Add(sCommand);
			Tokenizer tokenizer;
			tokenizer.SetSingleChars("");
			tokenizer.SetSingleLineComment("");
			tokenizer.Start(sArguments);
			String sToken = tokenizer.GetNextToken();
			while (sToken.GetLength()) {
				lstArgs.Add(sToken);
				sToken = tokenizer.GetNextToken();
			}

			// Make array for arguments
			const int nSize = lstArgs.GetNumOfElements();
			if (nSize > 0) {
				char **ppszParams = new char*[nSize+1];
				for (int i=0; i<nSize; i++)
					ppszParams[i] = const_cast<char*>(lstArgs[i].GetASCII());
				ppszParams[nSize] = nullptr;

				// Execute application
				execv(lstArgs[0].GetASCII(), ppszParams);
			}
		} else if (m_hProcess > 0) { //	Parent process
			// Connect pipes to files
			m_cFileInput .Assign(m_cPipeInput .GetWriteHandle());
			m_cFileOutput.Assign(m_cPipeOutput.GetReadHandle());
			m_cFileError .Assign(m_cPipeError .GetReadHandle());
		} else {
			// Error
			return false;
		}

		// Done
		return true;
	#endif
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
