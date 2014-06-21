/*********************************************************\
 *  File: SystemLinux.cpp                                *
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
#include <pwd.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <locale.h>
#include <sys/time.h>
#include <dlfcn.h>
#include "PLCore/String/RegEx.h"
#include "PLCore/File/File.h"
#include "PLCore/System/SystemLinux.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SystemLinux::SystemLinux() :
	m_bSysInfoInit(!uname(&m_sName))
{
	// Initialize the random generator
	srand(GetMicroseconds());
}

/**
*  @brief
*    Destructor
*/
SystemLinux::~SystemLinux()
{
}

/**
*  @brief
*    Reads memory information from the '/proc/meminfo'-file
*/
bool SystemLinux::GetMemoryInformation(MemoryInformation &sMemoryInformation) const
{
	// Initialize memory information
	sMemoryInformation.nTotalPhysicalMemory	= 0;
	sMemoryInformation.nFreePhysicalMemory	= 0;
	sMemoryInformation.nTotalSwapMemory		= 0;
	sMemoryInformation.nFreeSwapMemory		= 0;

	// Parse kernel information file
	File cFile("/proc/meminfo");
	if (cFile.Open(File::FileRead | File::FileText)) {
		static RegEx cRegEx("^\\s*(MemTotal|MemFree|SwapTotal|SwapFree):\\s*(\\d+).*$");
		while (!cFile.IsEof()) {
			const String sLine = cFile.GetS();
			if (cRegEx.Match(sLine)) {
				const String sName   = cRegEx.GetResult(0);
				const String sResult = cRegEx.GetResult(1);
				if (sName == "MemTotal")
					sMemoryInformation.nTotalPhysicalMemory = sResult.GetInt() * 1024;
				if (sName == "MemFree")
					sMemoryInformation.nFreePhysicalMemory = sResult.GetInt() * 1024;
				if (sName == "SwapTotal")
					sMemoryInformation.nTotalSwapMemory = sResult.GetInt() * 1024;
				if (sName == "SwapFree")
					sMemoryInformation.nFreeSwapMemory = sResult.GetInt() * 1024;
			}
		}

		// Done
		return true;
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Protected virtual SystemImpl functions                ]
//[-------------------------------------------------------]
String SystemLinux::GetPlatform() const
{
	static const String sString = "Linux";
	return sString;
}

String SystemLinux::GetOS() const
{
	// Get system info
	if (m_bSysInfoInit) {
		String sVersion = m_sName.sysname;
		sVersion += ' ';
		sVersion += m_sName.machine;
		sVersion += ' ';
		sVersion += m_sName.release;
		return sVersion;
	} else {
		return GetPlatform() + " unknown";
	}
}

char SystemLinux::GetSeparator() const
{
	return '/';
}

String SystemLinux::GetSharedLibraryPrefix() const
{
	static const String sString = "lib";
	return sString;
}

String SystemLinux::GetSharedLibraryExtension() const
{
	static const String sString = "so";
	return sString;
}

uint32 SystemLinux::GetCPUMhz() const
{
	// Initialize
	uint32 nMhz = 0;

	// Parse kernel information file
	File cFile("/proc/cpuinfo");
	if (cFile.Open(File::FileRead | File::FileText)) {
		static RegEx cRegEx("^\\s*cpu MHz\\s*:\\s*(\\d+(\\.\\d+)).*$");
		while (!cFile.IsEof()) {
			const String sLine = cFile.GetS();
			if (cRegEx.Match(sLine)) {
				const String sResult = cRegEx.GetResult(0);
				nMhz = sResult.GetInt();
			}
		}
	}

	// Done
	return nMhz;
}

String SystemLinux::GetComputerName() const
{
	// Get computer name
	return m_bSysInfoInit ? String::FromUTF8(m_sName.nodename) : "";
}

String SystemLinux::GetUserName() const
{
	// Get user name
	const struct passwd *pPass = getpwuid(getuid());
	return pPass ? String::FromUTF8(pPass->pw_name) : "";
}

String SystemLinux::GetUserHomeDir() const
{
	// Get user home directory
	const struct passwd *pPass = getpwuid(getuid());
	return pPass ? String::FromUTF8(pPass->pw_dir) : "";
}

String SystemLinux::GetUserDataDir() const
{
	// Use home directory on Linux
	return GetUserHomeDir();
}

String SystemLinux::GetDataDirName(const String &sName) const
{
	// Return ".<name>"
	String sLower = sName;
	sLower.ToLower();
	return "." + sLower;
}

String SystemLinux::GetExecutableFilename() const
{
	// Get PID of current process
	const pid_t nPID = getpid();
	char szLinkName[512];
	if (snprintf(szLinkName, 512, "/proc/%d/exe", nPID) >= 0) {
		// Read symbolic link
		char szProgram[512];
		const int nRet = readlink(szLinkName, szProgram, 512);
		if (nRet < 512) {
			szProgram[nRet] = 0;
			return String::FromUTF8(szProgram);
		}
	}

	// Error: Could not determine path to application
	return "";
}

String SystemLinux::GetModuleFilenameByMemoryAddress(const void *pMemoryAddress) const
{
	// Our method documentation states: "can be a null pointer in which case the result will be an empty string"
	if (pMemoryAddress) {
		// dladdr is a glibc extension to the dlfcn function set
		Dl_info dl_info;
		if (dladdr((void *)pMemoryAddress, &dl_info))
			return String::FromUTF8(dl_info.dli_fname);
	}

	// No filename found
	return "";
}

String SystemLinux::GetEnvironmentVariable(const String &sName) const
{
	return String::FromUTF8(getenv(sName));
}

bool SystemLinux::SetEnvironmentVariable(const String &sName, const String &sValue) const
{
	return (setenv((sName .GetFormat() == String::ASCII) ? sName .GetASCII() : sName .GetUTF8(),
				   (sValue.GetFormat() == String::ASCII) ? sValue.GetASCII() : sValue.GetUTF8(),
				   1) > -1);
}

void SystemLinux::DeleteEnvironmentVariable(const String &sName) const
{
	unsetenv((sName.GetFormat() == String::ASCII) ? sName.GetASCII() : sName.GetUTF8());
}

bool SystemLinux::Execute(const String &sCommand, const String &sParameters, const String &sWorkingDir) const
{
	// Check if a command has been given
	if (sCommand.GetLength()) {
		errno = 0;

		// Backup the current working directory and change to the given new one
		char *pszOldWorkingDir = nullptr;
		if (sWorkingDir.GetLength()) {
			// Allocates the buffer dynamically...
			pszOldWorkingDir = getcwd(nullptr, 0);

			// Change directory
			if (chdir((sWorkingDir.GetFormat() == String::ASCII) ? sWorkingDir.GetASCII() : sWorkingDir.GetUTF8()) != 0) {
				// Cleanup
				free(pszOldWorkingDir);

				// Error!
				return false;
			}
		}

		// Execute command
		const String sCommandAndParameters = sCommand + ' ' + sParameters;
		const int nResult = system((sCommandAndParameters.GetFormat() == String::ASCII) ? sCommandAndParameters.GetASCII() : sCommandAndParameters.GetUTF8());

		// Restore the old working directory
		if (pszOldWorkingDir) {
			if (chdir(pszOldWorkingDir) != 0) {
				// Cleanup
				free(pszOldWorkingDir);

				// Error!
				return false;
			}
			free(pszOldWorkingDir);
		}

		// Has the execution been successful?
		if (nResult != 127 && !errno)
			return true;
	}

	// Error!
	return false;
}

String SystemLinux::GetLocaleLanguage() const
{
	// Get the locale
	const char *pszSaveLocale = setlocale(LC_ALL, nullptr);
	setlocale(LC_ALL, "");
	const String sLocal = setlocale(LC_ALL, nullptr);
	setlocale(LC_ALL, pszSaveLocale);

	// Find the '_'
	const int nIndex = sLocal.IndexOf("_");
	return (nIndex >= 0) ? sLocal.GetSubstring(0, nIndex) : "";
}

String SystemLinux::GetCurrentDir() const
{
	// Get current directory and allocates the buffer dynamically
	char *pszTemp = getcwd(nullptr, 0);

	// Convert to PixelLight string
	const String sDirectory = String::FromUTF8(pszTemp);

	// Free the memory returned by "getcwd()"
	free(pszTemp);

	// Return the URL
	return sDirectory;
}

bool SystemLinux::SetCurrentDir(const String &sPath)
{
	// Get path in Unix style
	const String sUnixPath = Url(sPath).GetUnixPath();

	// Set current directory
	return (chdir((sUnixPath.GetFormat() == String::ASCII) ? sUnixPath.GetASCII() : sUnixPath.GetUTF8()) == 0);
}

Thread *SystemLinux::GetCurrentThread() const
{
	// [TODO] Implement me! We probably can use 'pthread_getspecific()' for this...
	return nullptr;
}

void SystemLinux::Exit(int nReturn)
{
	// Exit
	exit(nReturn);
}

const Console &SystemLinux::GetConsole() const
{
	return m_cConsole;
}

void SystemLinux::UrgentMessage(const String &sMessage) const
{
	// Do also feed the console, safe is safe
	fputs((sMessage.GetFormat() == String::ASCII) ? sMessage.GetASCII() : sMessage.GetUTF8(), stdout);
	fputs("\n", stdout);
	fflush(stdout);

	// There's no such thing as "MessageBox()" from MS Windows and using a GUI system
	// like Qt would be a total overkill in here, so, go the easiest possible way...
	char szCommand[1024];
	sprintf(szCommand, "xmessage -center \"%s\"", sMessage.GetASCII());
	system(szCommand);
}

Time SystemLinux::GetTime() const
{
	struct tm sTime;
	struct timeval now;

	gettimeofday(&now, nullptr);
	localtime_r(&now.tv_sec, &sTime);

	return Time(sTime.tm_year,
				static_cast<Time::EMonth>(sTime.tm_mon+1),
				sTime.tm_mday,
				static_cast<Time::EDay>(sTime.tm_wday),
				sTime.tm_hour,
				sTime.tm_min,
				sTime.tm_sec,
				static_cast<uint32>(now.tv_sec*1000+now.tv_usec/1000));
}

uint64 SystemLinux::GetMilliseconds() const
{
	struct timeval now;
	gettimeofday(&now, nullptr);
	return static_cast<uint64>(now.tv_sec*1000 + now.tv_usec/1000);
}

uint64 SystemLinux::GetMicroseconds() const
{
	struct timeval now;
	gettimeofday(&now, nullptr);
	return static_cast<uint64>(now.tv_sec*1000000 + now.tv_usec);
}

void SystemLinux::Sleep(uint64 nMilliseconds) const
{
	// We have to split up the given number of milliseconds to sleep into seconds and milliseconds

	// Calculate the number of seconds to sleep
	const time_t nSeconds = static_cast<time_t>(nMilliseconds/1000);

	// Overwrite the given number of milliseconds with the remaining calculated number of milliseconds to sleep
	nMilliseconds = nMilliseconds - (nSeconds*1000);

	// Now sleep well my friend...
	struct timespec sTimespec;
	sTimespec.tv_sec  = nSeconds;
	sTimespec.tv_nsec = nMilliseconds*1000000L;
	nanosleep(&sTimespec, 0);
}

void SystemLinux::Yield() const
{
	sched_yield();
}

float SystemLinux::GetPercentageOfUsedPhysicalMemory() const
{
	MemoryInformation sMemoryInformation;
	if (GetMemoryInformation(sMemoryInformation))
		return (static_cast<float>(sMemoryInformation.nTotalPhysicalMemory-sMemoryInformation.nFreePhysicalMemory)/sMemoryInformation.nTotalPhysicalMemory)*100.0f;
	else
		return 0.0f;
}

uint64 SystemLinux::GetTotalPhysicalMemory() const
{
	MemoryInformation sMemoryInformation;
	return GetMemoryInformation(sMemoryInformation) ? sMemoryInformation.nTotalPhysicalMemory : 0;
}

uint64 SystemLinux::GetFreePhysicalMemory() const
{
	MemoryInformation sMemoryInformation;
	return GetMemoryInformation(sMemoryInformation) ? sMemoryInformation.nFreePhysicalMemory : 0;
}

uint64 SystemLinux::GetTotalVirtualMemory() const
{
	MemoryInformation sMemoryInformation;
	return GetMemoryInformation(sMemoryInformation) ? sMemoryInformation.nTotalSwapMemory : 0;
}

uint64 SystemLinux::GetFreeVirtualMemory() const
{
	MemoryInformation sMemoryInformation;
	return GetMemoryInformation(sMemoryInformation) ? sMemoryInformation.nFreeSwapMemory : 0;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
