/*********************************************************\
 *  File: System.cpp                                     *
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
#include "PLCore/System/SystemImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the name of the platform
*/
inline String System::GetPlatform() const
{
	// Call system function
	return m_pSystemImpl->GetPlatform();
}

/**
*  @brief
*    Returns the name and version of the operating system
*/
inline String System::GetOS() const
{
	// Call system function
	return m_pSystemImpl->GetOS();
}

/**
*  @brief
*    Returns the directory separator used by the operation system
*/
inline char System::GetSeparator() const
{
	// Call system function
	return m_pSystemImpl->GetSeparator();
}

/**
*  @brief
*    Returns the shared library filename prefix used by the operation system
*/
inline String System::GetSharedLibraryPrefix() const
{
	// Call system function
	return m_pSystemImpl->GetSharedLibraryPrefix();
}

/**
*  @brief
*    Returns the shared library file extension used by the operation system
*/
inline String System::GetSharedLibraryExtension() const
{
	// Call system function
	return m_pSystemImpl->GetSharedLibraryExtension();
}

/**
*  @brief
*    Get the CPU MHz
*/
inline uint32 System::GetCPUMhz() const
{
	// Call system function
	return m_pSystemImpl->GetCPUMhz();
}

/**
*  @brief
*    Returns the name of the computer
*/
inline String System::GetComputerName() const
{
	// Call system function
	return m_pSystemImpl->GetComputerName();
}

/**
*  @brief
*    Returns the current user name
*/
inline String System::GetUserName() const
{
	// Call system function
	return m_pSystemImpl->GetUserName();
}

/**
*  @brief
*    Get home directory of the current user
*/
inline String System::GetUserHomeDir() const
{
	// Call system function
	return m_pSystemImpl->GetUserHomeDir();
}

/**
*  @brief
*    Get data directory of the current user
*/
inline String System::GetUserDataDir() const
{
	// Call system function
	return m_pSystemImpl->GetUserDataDir();
}

/**
*  @brief
*    Get name of data directory for given application name
*/
inline String System::GetDataDirName(const String &sName) const
{
	// Call system function
	return m_pSystemImpl->GetDataDirName(sName);
}

/**
*  @brief
*    Get absolute path of application executable
*/
inline String System::GetExecutableFilename() const
{
	// Call system function
	return m_pSystemImpl->GetExecutableFilename();
}

/**
*  @brief
*    Returns the absolute filename of the shared library or executable a given memory address is located in
*/
inline String System::GetModuleFilenameByMemoryAddress(const void *pMemoryAddress) const
{
	// Call system function
	return m_pSystemImpl->GetModuleFilenameByMemoryAddress(pMemoryAddress);
}

/**
*  @brief
*    Reads an environment variable
*/
inline String System::GetEnvironmentVariable(const String &sName) const
{
	// Call system function
	return m_pSystemImpl->GetEnvironmentVariable(sName);
}

/**
*  @brief
*    Writes an environment variable
*/
inline bool System::SetEnvironmentVariable(const String &sName, const String &sValue) const
{
	// Call system function
	return m_pSystemImpl->SetEnvironmentVariable(sName, sValue);
}

/**
*  @brief
*    Deletes an environment variable
*/
inline void System::DeleteEnvironmentVariable(const String &sName) const
{
	// Call system function
	return m_pSystemImpl->DeleteEnvironmentVariable(sName);
}

/**
*  @brief
*    Executes a system command (create a new process)
*/
inline bool System::Execute(const String &sCommand, const String &sParameters, const String &sWorkingDir) const
{
	// Call system function
	return m_pSystemImpl->Execute(sCommand, sParameters, sWorkingDir);
}

/**
*  @brief
*    Returns the current program locale language information
*/
inline String System::GetLocaleLanguage() const
{
	// Call system function
	return m_pSystemImpl->GetLocaleLanguage();
}

/**
*  @brief
*    Returns the current directory
*/
inline String System::GetCurrentDir() const
{
	// Call system function
	String sCurrentDir = m_pSystemImpl->GetCurrentDir();

	// No empty string, please (it should always be possible to add e.g. '/Data' without problems)
	if (!sCurrentDir.GetLength())
		sCurrentDir = '.';

	// Done
	return sCurrentDir;
}

/**
*  @brief
*    Sets the current directory
*/
inline bool System::SetCurrentDir(const String &sPath)
{
	// Call system function
	return m_pSystemImpl->SetCurrentDir(sPath);
}

/**
*  @brief
*    Returns a pointer to the main thread
*/
inline Thread *System::GetMainThread() const
{
	// Return main thread
	return m_pMainThread;
}

/**
*  @brief
*    Returns a pointer to the current thread
*/
inline Thread *System::GetCurrentThread() const
{
	// Call system function
	Thread *pThread = m_pSystemImpl->GetCurrentThread();
	return pThread ? pThread : m_pMainThread;
}

/**
*  @brief
*    Exit the application immediately
*/
inline void System::Exit(int nReturn)
{
	// Call system function
	m_pSystemImpl->Exit(nReturn);
}

/**
*  @brief
*    Returns the console instance
*/
inline const Console &System::GetConsole() const
{
	// Call system function
	return m_pSystemImpl->GetConsole();
}

/**
*  @brief
*    Primitive way (e.g. by using a message box) to be able to tell the user that something went terrible wrong
*/
inline void System::UrgentMessage(const String &sMessage) const
{
	// Call system function
	m_pSystemImpl->UrgentMessage(sMessage);
}

/**
*  @brief
*    Returns the current date and time
*/
inline Time System::GetTime() const
{
	// Call system function
	return m_pSystemImpl->GetTime();
}

/**
*  @brief
*    Returns the number of milliseconds since the system was started
*/
inline uint64 System::GetMilliseconds() const
{
	// Call system function
	return m_pSystemImpl->GetMilliseconds();
}

/**
*  @brief
*    Retrieves the number of microseconds since the system was started
*/
inline uint64 System::GetMicroseconds() const
{
	// Call system function
	return m_pSystemImpl->GetMicroseconds();
}

/**
*  @brief
*    Suspend the current thread for a specified time period
*/
inline void System::Sleep(uint64 nMilliseconds) const
{
	// Call system function
	m_pSystemImpl->Sleep(nMilliseconds);
}

/**
*  @brief
*    Yields the rest of the current threads time slice
*/
inline void System::Yield() const
{
	// Call system implementation
	m_pSystemImpl->Yield();
}

/**
*  @brief
*    Returns an approximation of the percentage of used physical memory (0.0-100.0)
*/
inline float System::GetPercentageOfUsedPhysicalMemory() const
{
	// Call system function
	return m_pSystemImpl->GetPercentageOfUsedPhysicalMemory();
}

/**
*  @brief
*    Returns the total physical memory in bytes
*/
inline uint64 System::GetTotalPhysicalMemory() const
{
	// Call system function
	return m_pSystemImpl->GetTotalPhysicalMemory();
}

/**
*  @brief
*    Returns the current free physical memory in bytes
*/
inline uint64 System::GetFreePhysicalMemory() const
{
	// Call system function
	return m_pSystemImpl->GetFreePhysicalMemory();
}

/**
*  @brief
*    Returns the total virtual memory in bytes
*/
inline uint64 System::GetTotalVirtualMemory() const
{
	// Call system function
	return m_pSystemImpl->GetTotalVirtualMemory();
}

/**
*  @brief
*    Returns the current free virtual memory in bytes
*/
inline uint64 System::GetFreeVirtualMemory() const
{
	// Call system function
	return m_pSystemImpl->GetFreeVirtualMemory();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
