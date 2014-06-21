/*********************************************************\
 *  File: SystemWindows.cpp                              *
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
#include <Tchar.h>
#include <conio.h>
#include <stdlib.h>
#include <shlobj.h>
#include <locale.h>
#include <windows.h>
#include <Userenv.h>
#include <shellapi.h>
#include "PLCore/File/Url.h"
#include "PLCore/System/+Windows/SystemWindows.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);


//[-------------------------------------------------------]
//[ Global variables                                      ]
//[-------------------------------------------------------]
DWORD g_nTlsIndex = TLS_OUT_OF_INDEXES; // 'Thread Local Storage' (TLS), TLS_OUT_OF_INDEXES on error


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Converts a given Windows error code received by 'GetLastError()' into a human readable string
*/
String SystemWindows::ErrorCodeToString(DWORD nErrorCode)
{
	LPTSTR pszError;

	// Try to format the error code
	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
					  nullptr, nErrorCode, 0, reinterpret_cast<LPTSTR>(&pszError), 0, nullptr)) {
		// All went fine, now build the PL string
		LPTSTR pszErrorEnd = _tcschr(pszError, _T('\r'));
		if (pszErrorEnd)
			*pszErrorEnd = _T('\0');
		const String sError = pszError;
		LocalFree(pszError);

		// Done
		return sError;
	} else {
		return String("Unknown error code: ") + static_cast<uint32>(nErrorCode);
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SystemWindows::SystemWindows()
{
	// Force the main thread to always run on CPU 0.
	// This is done because on some systems QueryPerformanceCounter returns a bit different counter values
	// on the different CPUs (contrary to what it's supposed to do), which can cause negative frame times
	// if the thread is scheduled on the other CPU in the next frame. This can cause very jerky behavior and
	// appear as if frames return out of order.
	SetThreadAffinityMask(GetCurrentThread(), 1);

	// Initialize performance timer
	QueryPerformanceFrequency(&m_nPerformanceFrequency);

	// Initialize the random generator
	srand(static_cast<unsigned int>(GetMicroseconds()));

	// Allocate a global TLS index
	g_nTlsIndex = TlsAlloc();
}

/**
*  @brief
*    Destructor
*/
SystemWindows::~SystemWindows()
{
	// Free our global TLS index
	if (g_nTlsIndex != TLS_OUT_OF_INDEXES) {
		TlsFree(g_nTlsIndex);
		g_nTlsIndex = TLS_OUT_OF_INDEXES;
	}
}


//[-------------------------------------------------------]
//[ Protected virtual SystemImpl functions                ]
//[-------------------------------------------------------]
String SystemWindows::GetPlatform() const
{
	static const String sString = "Windows";
	return sString;
}

String SystemWindows::GetOS() const
{
	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;

	ZeroMemory(&si, sizeof(SYSTEM_INFO));
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

	// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	// If that fails, try using the OSVERSIONINFO structure.
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	BOOL bOsVersionInfoEx = GetVersionEx(reinterpret_cast<OSVERSIONINFO*>(&osvi));
	if (!bOsVersionInfoEx) {
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (!GetVersionEx(reinterpret_cast<OSVERSIONINFO*>(&osvi)))
			return "Windows";
	}

	{ // Call GetNativeSystemInfo if supported or GetSystemInfo otherwise
		HMODULE hModule = GetModuleHandle(TEXT("kernel32.dll"));
		if (hModule) {
			PGNSI pGNSI = reinterpret_cast<PGNSI>(GetProcAddress(hModule, "GetNativeSystemInfo"));
			if (pGNSI)
				pGNSI(&si);
			else
				GetSystemInfo(&si);
		} else {
			GetSystemInfo(&si);
		}
	}

	// Compose version string
	String sVersion;

	// Get Windows version
	switch (osvi.dwPlatformId) {
		// Test for the Windows NT product family
		case VER_PLATFORM_WIN32_NT:
			// Test for the specific product
			if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1) {
				if (osvi.wProductType == VER_NT_WORKSTATION)
					sVersion += "Windows 7 ";
				else
					sVersion += "Windows Server 2008 R2 ";

			} else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0) {
				if (osvi.wProductType == VER_NT_WORKSTATION)
					sVersion += "Windows Vista ";
				else
					sVersion += "Windows Server \"Longhorn\" ";

			} else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2) {
				#ifndef SM_SERVERR2
					#define SM_SERVERR2 89
				#endif
				if (GetSystemMetrics(SM_SERVERR2))
					sVersion += "Microsoft Windows Server 2003 \"R2\" ";
				else if (osvi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
					sVersion += "Microsoft Windows XP Professional x64 Edition ";
				else
					sVersion += "Microsoft Windows Server 2003, ";

			} else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
				sVersion += "Microsoft Windows XP ";

			else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
				sVersion += "Microsoft Windows 2000 ";

			else if (osvi.dwMajorVersion <= 4)
				sVersion += "Microsoft Windows NT ";

			// Test for specific product on Windows NT 4.0 SP6 and later
			if (bOsVersionInfoEx) {
				// Test for the workstation type
				if (osvi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture != PROCESSOR_ARCHITECTURE_AMD64) {
					if (osvi.dwMajorVersion == 4)
						sVersion += "Workstation 4.0 ";
					else if (osvi.wSuiteMask & VER_SUITE_PERSONAL)
						sVersion += "Home Edition ";
					else
						sVersion += "Professional ";
				}

				// Test for the server type
				else if (osvi.wProductType == VER_NT_SERVER || osvi.wProductType == VER_NT_DOMAIN_CONTROLLER) {
					if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2) {
						if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64) {
							if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
								sVersion += "Datacenter Edition for Itanium-based Systems";
							else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
								sVersion += "Enterprise Edition for Itanium-based Systems";

						} else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
							if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
								sVersion += "Datacenter x64 Edition ";
							else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
								sVersion += "Enterprise x64 Edition ";
							else
								sVersion += "Standard x64 Edition ";

						} else {
							if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
								sVersion += "Datacenter Edition ";
							else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
								sVersion += "Enterprise Edition ";
							else if (osvi.wSuiteMask == VER_SUITE_BLADE)
								sVersion += "Web Edition ";
							else
								sVersion += "Standard Edition ";
						}

					} else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0) {
						if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
							sVersion += "Datacenter Server ";
						else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
							sVersion += "Advanced Server ";
						else
							sVersion += "Server ";

					} else { // Windows NT 4.0
						if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
							sVersion += "Server 4.0, Enterprise Edition ";
						else
							sVersion += "Server 4.0 ";
					}
				}

			// Test for specific product on Windows NT 4.0 SP5 and earlier
			} else {
				HKEY hKey;
				LONG lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
										 TEXT("SYSTEM\\CurrentControlSet\\Control\\ProductOptions"),
										 0, KEY_QUERY_VALUE, &hKey);
				if (lRet != ERROR_SUCCESS)
					break;

				TCHAR szProductType[80];
				DWORD dwBufLen = 80*sizeof(TCHAR);
				lRet = RegQueryValueEx(hKey, TEXT("ProductType"), nullptr, nullptr, reinterpret_cast<LPBYTE>(szProductType), &dwBufLen);
				RegCloseKey(hKey);

				if (lRet != ERROR_SUCCESS || dwBufLen > 80*sizeof(TCHAR))
					break;

				if (CompareString(LOCALE_INVARIANT, NORM_IGNORECASE, TEXT("WINNT"), -1, szProductType, -1) == CSTR_EQUAL)
					sVersion += "Workstation ";
				if (CompareString(LOCALE_INVARIANT, NORM_IGNORECASE, TEXT("LANMANNT"), -1, szProductType, -1) == CSTR_EQUAL)
					sVersion += "Server ";
				if (CompareString(LOCALE_INVARIANT, NORM_IGNORECASE, TEXT("SERVERNT"), -1, szProductType, -1) == CSTR_EQUAL)
					sVersion += "Advanced Server ";
				sVersion += static_cast<uint32>(osvi.dwMajorVersion);
				sVersion += '.';
				sVersion += static_cast<uint32>(osvi.dwMinorVersion);
			}

			// Display service pack (if any) and build number
			if (osvi.dwMajorVersion == 4 && CompareString(LOCALE_INVARIANT, NORM_IGNORECASE, osvi.szCSDVersion, -1, TEXT("Service Pack 6"), -1) == CSTR_EQUAL) {
				// Test for SP6 versus SP6a
				HKEY hKey;
				LONG lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
										 TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009"),
										 0, KEY_QUERY_VALUE, &hKey);
				if (lRet == ERROR_SUCCESS)
					sVersion += String("Service Pack 6a (Build ") + static_cast<uint32>(osvi.dwBuildNumber & 0xFFFF) + ')';
				else { // Windows NT 4.0 prior to SP6a
					sVersion += String(osvi.szCSDVersion) + " (Build " + static_cast<uint32>(osvi.dwBuildNumber & 0xFFFF) + ')';
				}

				RegCloseKey(hKey);
			} else { // Not Windows NT 4.0 
				sVersion += String(osvi.szCSDVersion) + " (Build " + static_cast<uint32>(osvi.dwBuildNumber & 0xFFFF) + ')';
			}
		break;

		// Test for the Windows Me/98/95
		case VER_PLATFORM_WIN32_WINDOWS:
			if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0) {
				sVersion += "Microsoft Windows 95 ";
				if (osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B')
					sVersion += "OSR2 ";

			} else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10) {
				sVersion += "Microsoft Windows 98 ";
				if (osvi.szCSDVersion[1] == 'A' || osvi.szCSDVersion[1] == 'B')
					sVersion += "SE ";

			} else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90) {
				sVersion += "Microsoft Windows Millennium Edition";
			}
		break;

		case VER_PLATFORM_WIN32s:
			sVersion += "Microsoft Win32s";
			break;
	}

	// Fallback, if unknown version number combination found
	if (sVersion.GetLength())
		sVersion.Trim();
	else
		sVersion = "Windows";

	// Return the found Windows version
	return sVersion;
}

char SystemWindows::GetSeparator() const
{
	return '\\';
}

String SystemWindows::GetSharedLibraryPrefix() const
{
	static const String sString = "";
	return sString;
}

String SystemWindows::GetSharedLibraryExtension() const
{
	static const String sString = "dll";
	return sString;
}

uint32 SystemWindows::GetCPUMhz() const
{
#if !defined(WIN64) && !defined(NO_INLINE_ASM)
	// 32 Bit assembler method
	unsigned long nStartTicks = 0;
	unsigned long nEndTicks   = 0;
	uint64 nTimeStop;

	uint64 nTimeStart = GetMilliseconds();
	do {
		nTimeStop = GetMilliseconds();
		// Rollover past 1
	} while (nTimeStop - nTimeStart <= 1);
	__asm {
		xor eax, eax
		xor ebx, ebx
		xor ecx, ecx
		xor edx, edx
		_emit 0x0f // CPUID
		_emit 0xa2
		_emit 0x0f // RTDSC
		_emit 0x31
		mov [nStartTicks], eax // Tick counter starts here
	}

	nTimeStart = nTimeStop;
	do {
		nTimeStop = GetMilliseconds();
		// One second
	} while (nTimeStop - nTimeStart <= 1000);
	__asm {
		xor eax, eax
		xor ebx, ebx
		xor ecx, ecx
		xor edx, edx
		_emit 0x0f
		_emit 0xa2
		_emit 0x0f
		_emit 0x31
		mov [nEndTicks], eax
	}

	// Total tick count
	const uint64 nTotalTicks = nEndTicks - nStartTicks;

	// CPU Speed
	return static_cast<uint32>(nTotalTicks / 1000000);
#elif defined(WIN64) && !defined(NO_INLINE_ASM)
	// 64 Bit assembler method
	// [TODO] Implement 64 bit version
	return 0;
#else
	// C++ method
	// [TODO] Implement C++ version
	return 0;
#endif
}

String SystemWindows::GetComputerName() const
{
	// First of all, get the length of the computer name (including the terminating zero)
	DWORD nTempLength = 0;
	GetComputerNameW(nullptr, &nTempLength);
	if (nTempLength) {
		// Get computer name
		wchar_t *pszTemp = new wchar_t[nTempLength];
		if (GetComputerNameW(pszTemp, &nTempLength)) {
			return String(pszTemp, false, nTempLength); // Do not copy, please
		} else {
			// We no longer need the data, so free it
			delete [] pszTemp;
		}
	}

	// Error!
	return "";
}

String SystemWindows::GetUserName() const
{
	// First of all, get the length of the user name (including the terminating zero)
	DWORD nTempLength = 0;
	GetUserNameW(nullptr, &nTempLength);
	if (nTempLength) {
		// Get user name
		wchar_t *pszTemp = new wchar_t[nTempLength];
		if (GetUserNameW(pszTemp, &nTempLength)) {
			return String(pszTemp, false, nTempLength-1); // Do not copy, please
		} else {
			// We no longer need the data, so free it
			delete [] pszTemp;
		}
	}

	// Error!
	return "";
}

String SystemWindows::GetUserHomeDir() const
{
	// Open the access token associated with our process
	HANDLE hToken = 0;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken) != 0) {
		// First of all, get the length of the user home directory (including the terminating zero)
		DWORD nTempLength = 0;
		GetUserProfileDirectoryW(hToken, nullptr, &nTempLength);
		if (nTempLength) {
			// Get user home directory
			wchar_t *pszTemp = new wchar_t[nTempLength];
			if (GetUserProfileDirectoryW(hToken, pszTemp, &nTempLength)) {
				// Close the access token
				CloseHandle(hToken);

				// Do not copy, please
				return String(pszTemp, false, nTempLength-1);
			} else {
				// We no longer need the data, so free it
				delete [] pszTemp;
			}
		}

		// Close the access token
		CloseHandle(hToken);
	}

	// Error!
	return "";
}

String SystemWindows::GetUserDataDir() const
{
	// Get application data path
	wchar_t szPath[MAX_PATH];
	SHGetSpecialFolderPathW(nullptr, szPath, CSIDL_APPDATA, FALSE);

	// Return path (copy string)
	return szPath;
}

String SystemWindows::GetDataDirName(const String &sName) const
{
	// Use name directly
	return sName;
}

String SystemWindows::GetExecutableFilename() const
{
	// Allocate a buffer that is large enough to hold the information
	wchar_t *pszBuffer = new wchar_t[MAX_PATH];

	// Get user name as Unicode
	if (GetModuleFileNameW(nullptr, pszBuffer, MAX_PATH)) {
		return String(pszBuffer, false); // Do not copy, please
	} else {
		// We no longer need the data, so free it
		delete [] pszBuffer;

		// Error
		return "";
	}
}

String SystemWindows::GetModuleFilenameByMemoryAddress(const void *pMemoryAddress) const
{
	// Our method documentation states: "can be a null pointer in which case the result will be an empty string"
	if (pMemoryAddress) {
		// "::GetModuleHandle(nullptr)" returns the instance of the calling process, so we can't use this in here

		// Get filename
		MEMORY_BASIC_INFORMATION sMemoryBasicInformation;
		wchar_t szModule[MAX_PATH];
		szModule[0] = L'\0';
		::VirtualQuery(pMemoryAddress, &sMemoryBasicInformation, sizeof(MEMORY_BASIC_INFORMATION));
		::GetModuleFileName(static_cast<HMODULE>(sMemoryBasicInformation.AllocationBase), szModule, MAX_PATH);

		// Done
		return szModule;
	}

	// No filename found
	return "";
}

String SystemWindows::GetEnvironmentVariable(const String &sName) const
{
	// Get size of buffer to receive variable value
	// This ASCII/Unicode check may look weird: "It's based on the type of the name-string, used only for the size and then returns a unicode-string anyway.
	// Perhaps skip this completely?" - we could skip it, right... but then 'sName' MUST be converted internally ALWAYS to
	// Unicode even if in the most cases ASCII is given and in the case that no such environment variable was found, this additional work
	// was quite useless. (yeah, not really performance critical - but if there's a chance to skip some work, we should take it)
	DWORD nSize = (sName.GetFormat() == String::ASCII) ? GetEnvironmentVariableA(sName.GetASCII(), nullptr, 0) : GetEnvironmentVariableW(sName.GetUnicode(), nullptr, 0);
	if (nSize > 0) {
		// Get variable as Unicode
		wchar_t *pszBuffer = new wchar_t[nSize];
		if (GetEnvironmentVariableW(sName.GetUnicode(), pszBuffer, nSize) > 0) {
			return String(pszBuffer, false); // Do not copy, please
		} else {
			// We no longer need the data, so free it
			delete [] pszBuffer;
		}
	}

	// Error!
	return "";
}

bool SystemWindows::SetEnvironmentVariable(const String &sName, const String &sValue) const
{
	if (sName.GetFormat() == String::ASCII)
		return (SetEnvironmentVariableA(sName.GetASCII(),   sValue.GetASCII())   > 0);
	else
		return (SetEnvironmentVariableW(sName.GetUnicode(), sValue.GetUnicode()) > 0);
}

void SystemWindows::DeleteEnvironmentVariable(const String &sName) const
{
	if (sName.GetFormat() == String::ASCII)
		SetEnvironmentVariableA(sName.GetASCII(),   nullptr);
	else
		SetEnvironmentVariableW(sName.GetUnicode(), nullptr);
}

bool SystemWindows::Execute(const String &sCommand, const String &sParameters, const String &sWorkingDir) const
{
	// Check if a command has been given
	if (!sCommand.GetLength())
		return false; // Error!

	// Execute command
	HINSTANCE nResult;
	if (sCommand.GetFormat() == String::ASCII && sParameters.GetFormat() == String::ASCII && sWorkingDir.GetFormat() == String::ASCII) {
		nResult = ShellExecuteA(
			nullptr,
			"open",
			sCommand.GetASCII(),
			sParameters.GetLength() ? sParameters.GetASCII() : nullptr,
			sWorkingDir.GetLength() ? sWorkingDir.GetASCII() : nullptr,
			SW_SHOWDEFAULT
		);
	} else {
		nResult = ShellExecuteW(
			nullptr,
			L"open",
			sCommand.GetUnicode(),
			sParameters.GetLength() ? sParameters.GetUnicode() : nullptr,
			sWorkingDir.GetLength() ? sWorkingDir.GetUnicode() : nullptr,
			SW_SHOWDEFAULT
		);
	}

	// Has the execution been successful?
	return (reinterpret_cast<int>(nResult) > 32);
}

String SystemWindows::GetLocaleLanguage() const
{
	// Get the locale
	const char *pszSaveLocale = setlocale(LC_ALL, nullptr);
	setlocale(LC_ALL, "");
	String sLocal = setlocale(LC_ALL, nullptr);
	setlocale(LC_ALL, pszSaveLocale);

	// Find the '_'
	const int nIndex = sLocal.IndexOf("_");
	return (nIndex >= 0) ? sLocal.GetSubstring(0, nIndex) : "";
}

String SystemWindows::GetCurrentDir() const
{
	// First of all, get the length of the current directory (including the terminating zero)
	const uint32 nTempLength = GetCurrentDirectoryW(0, nullptr);
	if (nTempLength) {
		// Get current directory
		wchar_t *pszTemp = new wchar_t[nTempLength];
		GetCurrentDirectoryW(nTempLength, pszTemp);

		// Return the URL (do not copy, please)
		return String(pszTemp, false, nTempLength-1);
	} else {
		// Done
		return "";
	}
}

bool SystemWindows::SetCurrentDir(const String &sPath)
{
	// Get path in Windows style
	const String sWindowsPath = Url(sPath).GetWindowsPath();

	// Set current directory
	if (sWindowsPath.GetFormat() == String::ASCII)
		return (SetCurrentDirectoryA(sWindowsPath.GetASCII()) != 0);
	else
		return (SetCurrentDirectoryW(sWindowsPath.GetUnicode()) != 0);
}

Thread *SystemWindows::GetCurrentThread() const
{
	// Valid TLS?
	if (g_nTlsIndex != TLS_OUT_OF_INDEXES) {
		// Get the dynamic memory of the current thread, if a null pointer, this must be the main thread
		return static_cast<Thread*>(TlsGetValue(g_nTlsIndex));
	} else {
		// Error!
		return nullptr;
	}
}

void SystemWindows::Exit(int nReturn)
{
	// Exit
	exit(nReturn);
}

const Console &SystemWindows::GetConsole() const
{
	return m_cConsole;
}

void SystemWindows::UrgentMessage(const String &sMessage) const
{
	MessageBoxW(nullptr, sMessage, L"Urgent message", MB_OK | MB_ICONERROR);
}

Time SystemWindows::GetTime() const
{
	SYSTEMTIME sTime;
	GetLocalTime(&sTime);
	return Time(sTime.wYear,
				static_cast<Time::EMonth>(sTime.wMonth),
				static_cast<uint8>(sTime.wDay),
				static_cast<Time::EDay>(sTime.wDayOfWeek),
				static_cast<uint8>(sTime.wHour),
				static_cast<uint8>(sTime.wMinute),
				static_cast<uint8>(sTime.wSecond),
				sTime.wMilliseconds);
}

uint64 SystemWindows::GetMilliseconds() const
{
	// Get past time
	LARGE_INTEGER nCurTime;
	QueryPerformanceCounter(&nCurTime);
	LONGLONG nNewTicks = nCurTime.QuadPart;

	// Scale by 1000 in order to get millisecond precision
	nNewTicks *= 1000;
	if (m_nPerformanceFrequency.QuadPart)
		nNewTicks /= m_nPerformanceFrequency.QuadPart;

	// Return past time
	return static_cast<uint64>(nNewTicks);
}

uint64 SystemWindows::GetMicroseconds() const
{
	// Get past time
	LARGE_INTEGER nCurTime;
	QueryPerformanceCounter(&nCurTime);
	double dNewTicks = static_cast<double>(nCurTime.QuadPart);

	// Scale by 1000000 in order to get microsecond precision
	dNewTicks *= static_cast<double>(1000000.0)/static_cast<double>(m_nPerformanceFrequency.QuadPart);

	// Return past time
	return static_cast<uint64>(dNewTicks);
}

void SystemWindows::Sleep(uint64 nMilliseconds) const
{
	SleepEx(static_cast<DWORD>(nMilliseconds), false);
}

void SystemWindows::Yield() const
{
	// By passing 0 to the sleep Windows function it behaves like desired (this is from Microsoft documented behavior)
	SleepEx(0, false);
}

float SystemWindows::GetPercentageOfUsedPhysicalMemory() const
{
	MEMORYSTATUSEX sMemoryInfo;
	sMemoryInfo.dwLength = sizeof(MEMORYSTATUSEX);
	return GlobalMemoryStatusEx(&sMemoryInfo) ? static_cast<float>(sMemoryInfo.dwMemoryLoad) : 0.0f;
}

uint64 SystemWindows::GetTotalPhysicalMemory() const
{
	MEMORYSTATUSEX sMemoryInfo;
	sMemoryInfo.dwLength = sizeof(MEMORYSTATUSEX);
	return GlobalMemoryStatusEx(&sMemoryInfo) ? sMemoryInfo.ullTotalPhys : 0;
}

uint64 SystemWindows::GetFreePhysicalMemory() const
{
	MEMORYSTATUSEX sMemoryInfo;
	sMemoryInfo.dwLength = sizeof(MEMORYSTATUSEX);
	return GlobalMemoryStatusEx(&sMemoryInfo) ? sMemoryInfo.ullAvailPhys : 0;
}

uint64 SystemWindows::GetTotalVirtualMemory() const
{
	MEMORYSTATUSEX sMemoryInfo;
	sMemoryInfo.dwLength = sizeof(MEMORYSTATUSEX);
	return GlobalMemoryStatusEx(&sMemoryInfo) ? sMemoryInfo.ullTotalVirtual : 0;
}

uint64 SystemWindows::GetFreeVirtualMemory() const
{
	MEMORYSTATUSEX sMemoryInfo;
	sMemoryInfo.dwLength = sizeof(MEMORYSTATUSEX);
	return GlobalMemoryStatusEx(&sMemoryInfo) ? sMemoryInfo.ullAvailVirtual : 0;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
