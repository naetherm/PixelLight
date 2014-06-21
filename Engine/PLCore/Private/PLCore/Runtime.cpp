/*********************************************************\
 *  File: Runtime.cpp                                    *
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
#include <PixelLight.h>
#ifdef WIN32
	// [TODO] Use the registry class within "Runtime::SetDirectory()"
	#include <windows.h>
	#include <stdio.h>
	// We undef these to avoid name conflicts
	#undef DrawText
	#undef LoadImage
	#undef MessageBox
	#undef GetClassName
	#undef CreateDirectory
	#undef SetCurrentDirectory
	#undef GetCurrentDirectory
	#undef GetSystemDirectory
	#undef GetEnvironmentVariable
	#undef SetEnvironmentVariable
	#undef GetComputerName
	#undef GetUserName
	#undef CreateFont
	#undef Yield
	#undef SendMessage
	#undef PostMessage
#endif
#include "PLCore/System/DynLib.h"
#include "PLCore/System/System.h"
#include "PLCore/File/Url.h"
#include "PLCore/File/Directory.h"
#include "PLCore/Base/ClassManager.h"
#include "PLCore/Registry/Registry.h"
#include "PLCore/Tools/LoadableManager.h"
#include "PLCore/Runtime.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get used PixelLight installation type
*/
Runtime::EType Runtime::GetType()
{
	#ifdef PLCORE_STATIC
		// No doubt, the executable is using the static linked version of PLCore
		return StaticInstallation;
	#else
		// Get the absolute path the PLCore shared library is in
		// (should never ever fail because this executable is already using this shared library...)
		// -> The result is e.g. "file://C:/PixelLight/Runtime/x86" on Windows
		// -> To ensure we have a comparable url, add '/'
		const String sAbsPLCorePath = GetPLCoreSharedLibraryDirectory() + '/';

		// Get the absolute filename of the running process
		// -> "Url::CutFilename()": The protocol is omitted for a local path (e.g. the result will not be "file://C:/Programs/App/")
		// -> We have to ensure that we have a comparable URL, so use the result of "Url::GetUrl()"
		const String sAbsProcessPath = Url(Url(System::GetInstance()->GetExecutableFilename()).CutFilename()).GetUrl();

		// Is the PLCore shared library within the same directory as the running process?
		if (sAbsPLCorePath == sAbsProcessPath) {
			// The PixelLight runtime is in the same directory as the running process, making this to a local installation
			return LocalInstallation;
		} else {
			// The PixelLight runtime is registered within the system, making this to a system installation
			return SystemInstallation;
		}
	#endif
}

/**
*  @brief
*    Get PixelLight version
*/
Version Runtime::GetVersion()
{
	return Version(
		PIXELLIGHT_NAME,
		"",
		PIXELLIGHT_VERSION_MAJOR,
		PIXELLIGHT_VERSION_MINOR,
		PIXELLIGHT_VERSION_PATCH,
		PIXELLIGHT_VERSION_RELEASE
	);
}

/**
*  @brief
*    Get PixelLight suffix
*/
String Runtime::GetSuffix()
{
	return PIXELLIGHT_SUFFIX;
}

/**
*  @brief
*    Try to find the local PixelLight runtime data directory
*/
String Runtime::GetLocalDataDirectory()
{
	// Get PixelLight local runtime directory
	const String sPLDirectory = GetDirectory(LocalInstallation);
	if (sPLDirectory.GetLength()) {
		// Return the local runtime data directory
		return Url(sPLDirectory + "/../Data").Collapse().GetUrl();
	} else {
		// Error!
		return "";
	}
}

/**
*  @brief
*    Try to find the system PixelLight runtime directory
*/
String Runtime::GetSystemDirectory()
{
	// First: Try to find the system PixelLight runtime directory by reading the registry
	// -> We really need to check for the registry, first: When building for Linux, there
	//    are fixed build in locations like "/home/bob/pixellight/cmakeout/Base/PLCore/" and
	//    build executables will link against those shared libraries -> You'll receive
	//    "/home/bob/pixellight/cmakeout/Base/PLCore/" as PixelLight system directory, not e.g.
	//    "/home/bob/pixellight/Bin-Linux/Runtime/x86" as expected.
	// -> When using 32 bit and 64 bit on the same system, using the registry first is also the better way
	String sPLDirectory = GetRegistryDirectory();
	if (!sPLDirectory.GetLength()) {
		// Second: Try to find the PixelLight runtime directory by using the PLCore shared library
		sPLDirectory = GetDirectory(SystemInstallation);
	}

	// Done
	return sPLDirectory;
}

/**
*  @brief
*    Try to find the system PixelLight runtime data directory
*/
String Runtime::GetSystemDataDirectory()
{
	// Get PixelLight system runtime directory
	const String sPLDirectory = GetSystemDirectory();
	if (sPLDirectory.GetLength()) {
		// Return the system runtime data directory
		return Url(sPLDirectory + "/../Data").Collapse().GetUrl();
	} else {
		// Error!
		return "";
	}
}

/**
*  @brief
*    Try to find the system PixelLight runtime directory by reading the registry
*/
String Runtime::GetRegistryDirectory()
{
	// Windows
	#ifdef WIN32
		// Get path to PixelLight runtime
		Registry cRegistry;
		if (cRegistry.GetRegistryType() == Registry::Windows) {
			// Get suffix
			String sSuffix = GetSuffix();
			if (sSuffix.GetLength() > 0)
				sSuffix = '-' + sSuffix;

			// On 64 bit *Windows* systems, "Wow6432Node" is added automatically for 32 bit applications by the OS

			// Read registry key "PixelLight-SDK" ("SOFTWARE\\Wow6432Node\\PixelLight\\PixelLight-SDK" on a 64 bit version and 32 bit application)
			String sSubkey = "SOFTWARE\\PixelLight\\PixelLight-SDK" + sSuffix;
			if (cRegistry.Open(Registry::KeyLocalMachine, sSubkey, Registry::RegRead)) {
				const String sRuntime = cRegistry.GetValueString("Runtime");
				if (sRuntime.GetLength()) {
					// Ensure that the given directory really exists, if not, ignore the registry entry
					Directory cDirectory(sRuntime);
					if (cDirectory.Exists() && cDirectory.IsDirectory()) {
						// Convert to valid URL
						return Url(sRuntime).GetUrl();	// Done
					}
				}
			}

			// Read registry key "PixelLight-Runtime" ("SOFTWARE\\Wow6432Node\\PixelLight\\PixelLight-Runtime" on a 64 bit version and 32 bit application)
			sSubkey = "SOFTWARE\\PixelLight\\PixelLight-Runtime" + sSuffix;
			if (cRegistry.Open(Registry::KeyLocalMachine, sSubkey, Registry::RegRead)) {
				const String sRuntime = cRegistry.GetValueString("Runtime");
				if (sRuntime.GetLength()) {
					// Ensure that the given directory really exists, if not, ignore the registry entry
					Directory cDirectory(sRuntime);
					if (cDirectory.Exists() && cDirectory.IsDirectory()) {
						// Convert to valid URL
						return Url(sRuntime).GetUrl();	// Done
					}
				}
			}
		}

		// No runtime directory was found in the registry
		return "";
	#endif

	// Linux
	#ifdef LINUX
		// Check if a local pixellight runtime is specified in the environment variable PL_RUNTIME
		const String sRuntime = System::GetInstance()->GetEnvironmentVariable("PL_RUNTIME");
		if (sRuntime.GetLength() > 0) {
			// Ensure that the given directory really exists, if not, ignore the registry entry
			Directory cDirectory(sRuntime);
			if (cDirectory.Exists() && cDirectory.IsDirectory()) {
				// Use local runtime
				return sRuntime;
			}
		}

		{
			// Let's first check if a local installation is present in '/usr/local/share/pixellight/Runtime/<architecture>'.
			// If it is, we will use that.
			const String sLocalShareDirectory = "/usr/local/share/pixellight/Runtime/" + System::GetInstance()->GetPlatformArchitecture();
			Directory cDirectory(sLocalShareDirectory);
			if (cDirectory.Exists() && cDirectory.IsDirectory())
				return sLocalShareDirectory;
		}

		{
			// Let's check for the global installation in '/usr/share/pixellight/Runtime/<architecture>'
			// If it is, we will use that.
			const String sShareDirectory = "/usr/share/pixellight/Runtime/" + System::GetInstance()->GetPlatformArchitecture();
			Directory cDirectory(sShareDirectory);
			if (cDirectory.Exists() && cDirectory.IsDirectory())
				return sShareDirectory;
		}

		// No "local" or global installation found
		return "";
	#endif
}

/**
*  @brief
*    Try to find the system PixelLight runtime data directory by reading the registry
*/
String Runtime::GetRegistryDataDirectory()
{
	// Get PixelLight registry runtime directory
	const String sPLDirectory = GetRegistryDirectory();
	if (sPLDirectory.GetLength()) {
		// Return the registry runtime data directory
		return Url(sPLDirectory + "/../Data").Collapse().GetUrl();
	} else {
		// Error!
		return "";
	}
}

/**
*  @brief
*    Try to find the PixelLight runtime directory used by the running process
*/
String Runtime::GetDirectory()
{
	// First: Try to find the local PixelLight runtime directory
	String sPLDirectory = GetLocalDirectory();
	if (!sPLDirectory.GetLength()) {
		// No local PixelLight runtime directory found

		// Second: Try to find the system PixelLight runtime directory
		sPLDirectory = GetSystemDirectory();
	}

	// Done
	return sPLDirectory;
}

/**
*  @brief
*    Try to find the PixelLight runtime data directory used by the running process
*/
String Runtime::GetDataDirectory()
{
	// Get PixelLight runtime directory used by the running process
	const String sPLDirectory = GetDirectory();
	if (sPLDirectory.GetLength()) {
		// Return the runtime data directory used by the running process
		return Url(sPLDirectory + "/../Data").Collapse().GetUrl();
	} else {
		// Error!
		return "";
	}
}

/**
*  @brief
*    Returns the name of the PLCore shared library
*/
String Runtime::GetPLCoreSharedLibraryName()
{
	#ifdef _DEBUG
		// The executable is using a debug version of PixelLight
		static const String sString = System::GetInstance()->GetSharedLibraryPrefix() + "PLCoreD." + System::GetInstance()->GetSharedLibraryExtension();
		return sString;
	#else
		// The executable is using a release version of PixelLight
		static const String sString = System::GetInstance()->GetSharedLibraryPrefix() + "PLCore." + System::GetInstance()->GetSharedLibraryExtension();
		return sString;
	#endif
}

/**
*  @brief
*    Returns the absolute path to the directory the PLCore shared library is in
*/
String Runtime::GetPLCoreSharedLibraryDirectory()
{
	// Load the shared library
	DynLib cPLCoreDynLib;
	if (cPLCoreDynLib.Load(GetPLCoreSharedLibraryName())) {
		// Get the absolute path the PLCore shared library is in (e.g. "C:/Programs/App/PLCore.dll"->"C:/Programs/App/")
		String sDirectory = Url(cPLCoreDynLib.GetAbsPath()).CutFilename();

		// Ensure that there's no "/" at the end, else this method would have another behavior as the other runtime methods -> bad
		if (sDirectory.GetLength() && sDirectory[sDirectory.GetLength()-1] == '/')
			sDirectory.Delete(sDirectory.GetLength()-1);

		// Return the absolute path the PLCore shared library is in
		return Url(sDirectory).GetUrl();
	}

	// Error!
	return "";
}

/**
*  @brief
*    Sets the given PixelLight runtime directory
*/
bool Runtime::SetDirectory(const String &sDirectory, String *pszErrorMessage)
{
	// Windows
	#ifdef WIN32
		// [TODO] Use the registry class

		// Add directory to PATH variable
		HKEY hKey;
		if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Environment", 0, KEY_READ | KEY_WRITE, &hKey) == ERROR_SUCCESS) {
			// Query size of PATH variable
			DWORD nPathSize = 0;
			RegQueryValueEx(hKey, L"PATH", 0, nullptr, nullptr, &nPathSize);
			nPathSize += 256 + 8;
			wchar_t *pszPath = new wchar_t[nPathSize];

			// Query value of PATH variable
			DWORD nType;
			DWORD nSize = nPathSize;
			LSTATUS nResult = RegQueryValueEx(hKey, L"PATH", 0, &nType, reinterpret_cast<BYTE*>(pszPath), &nSize);
			if (nResult == ERROR_FILE_NOT_FOUND) {
				// Write to registry
				RegSetValueEx(hKey, L"PATH", 0, REG_SZ, reinterpret_cast<const BYTE*>(sDirectory.GetUnicode()), static_cast<DWORD>(sizeof(wchar_t)*wcslen(sDirectory.GetUnicode())));
				RegFlushKey(hKey);
			} else if (nResult == ERROR_SUCCESS) {
				// Get upper case version
				wchar_t *pszPathUpcase = new wchar_t[nSize];
				wcscpy(pszPathUpcase, pszPath);
				_wcsupr(pszPathUpcase);

				// Check if path is already set to this directory
				String sDirectorUpcase = sDirectory;
				sDirectorUpcase.ToUpper();
				if (!wcsstr(pszPathUpcase, sDirectorUpcase.GetUnicode())) {
					// Add directory to path
					if (wcslen(pszPath) && pszPath[wcslen(pszPath) - 1] != L';')
						wcscat(pszPath, L";");
					wcscat(pszPath, sDirectory.GetUnicode());
					wcscat(pszPath, L";");

					// Write back to registry
					pszPath[wcslen(pszPath) + 1] = L'\0';
					RegSetValueEx(hKey, L"PATH", 0, nType, reinterpret_cast<const BYTE*>(pszPath), static_cast<DWORD>(sizeof(wchar_t)*wcslen(pszPath)));
					RegFlushKey(hKey);
				}

				// Release data
				if (pszPathUpcase)
					delete [] pszPathUpcase;
			}

			// Clean up
			if (pszPath)
				delete [] pszPath;
			RegCloseKey(hKey);
		} else {
			if (pszErrorMessage)
				*pszErrorMessage = "Failed to open \"HKEY_CURRENT_USER\\Environment\"";
		}

		// Get PixelLight suffix and compose registry key from it
		wchar_t szSuffix[256];
		mbstowcs(szSuffix, PIXELLIGHT_SUFFIX, strlen(PIXELLIGHT_SUFFIX)+1);
		wchar_t szSubkey[256];
		wcscpy(szSubkey, L"SOFTWARE\\PixelLight\\PixelLight-SDK");
		if (wcslen(szSuffix) > 0) {
			wcscat(szSubkey, L"-");
			wcscat(szSubkey, szSuffix);
		}

		// Add directory to registry key "SOFTWARE\\PixelLight\\PixelLight-SDK"
		const LONG nErrorCode = RegCreateKeyEx(HKEY_LOCAL_MACHINE, szSubkey, 0, nullptr, 0, KEY_READ | KEY_WRITE, nullptr, &hKey, nullptr);
		if (nErrorCode == ERROR_SUCCESS) {
			// Set value
			RegSetValueEx(hKey, L"Runtime", 0, REG_SZ, reinterpret_cast<const BYTE*>(sDirectory.GetUnicode()), static_cast<DWORD>(sizeof(wchar_t)*wcslen(sDirectory.GetUnicode())));
			RegFlushKey(hKey);

			// Clean up
			RegCloseKey(hKey);
		} else {
			if (pszErrorMessage) {
				// Converts a given Windows error code received by 'GetLastError()' into a human readable string
				LPTSTR pszError;
				if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr, nErrorCode, 0, reinterpret_cast<LPTSTR>(&pszError), 0, nullptr)) {
					wchar_t szMessage[256];
					swprintf(szMessage, 256, L"Failed to open or create \"HKEY_LOCAL_MACHINE\\SOFTWARE\\PixelLight\\PixelLight-SDK\" (%s)", pszError);
					*pszErrorMessage = szMessage;
					LocalFree(pszError);
				} else {
					*pszErrorMessage = "Failed to open or create \"HKEY_LOCAL_MACHINE\\SOFTWARE\\PixelLight\\PixelLight-SDK\"";
				}
			}
		}

		// We need to send a broadcast so other processes will be informed about the change. If this is not done,
		// 'dlls' still will not be found although the "PATH" environment variable was updated!
		ULONG_PTR nResult;
		SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, reinterpret_cast<LPARAM>(L"Environment"), SMTO_NORMAL, 4000, &nResult);
	#endif

	// [TODO] Implement Linux equivalent (see "GetRegistryDirectory()", I know that there's no MS Windows like registry under Linux)

	// Done
	return true;
}

/**
*  @brief
*    Scan PixelLight runtime directory for compatible plugins and load them in
*/
void Runtime::ScanDirectoryPlugins(const String &sDirectory, bool bDelayedPluginLoading)
{
	// Get PixelLight runtime directory
	const String sPLDirectory = (sDirectory.GetLength()) ? sDirectory : GetDirectory();
	if (sPLDirectory.GetLength()) {
		// Scan for plugins in the PixelLight runtime directory, but not recursively, please. This is quite useful
		// for projects which can be used completely dynamically, but can also be used in other C++ projects
		// to access certain features.
		ClassManager::GetInstance()->ScanPlugins(sPLDirectory, NonRecursive, bDelayedPluginLoading);
	}
}

/**
*  @brief
*    Scan PixelLight runtime directory for compatible data and register it
*/
void Runtime::ScanDirectoryData(const String &sDirectory)
{
	// Get PixelLight runtime data directory
	const String sPLDataDirectory = (sDirectory.GetLength()) ? sDirectory : GetDataDirectory();
	if (sPLDataDirectory.GetLength()) {
		// Add runtime directory
		LoadableManager::GetInstance()->AddBaseDir(sPLDataDirectory);

		// Add packages from PixelLight runtime directory
		LoadableManager::GetInstance()->ScanPackages(sPLDataDirectory);
	}
}

/**
*  @brief
*    Scan PixelLight runtime directory for compatible plugins and load them in as well as scan for compatible data and register it
*/
bool Runtime::ScanDirectoryPluginsAndData(bool bUrgentMessageAllowed)
{
	// Check PixelLight runtime directory
	const String sPLDirectory = GetDirectory();
	if (sPLDirectory.GetLength()) {
		// Wow, just have a look at this ueber-complex implementation
		ScanDirectoryPlugins(sPLDirectory);
		ScanDirectoryData();

		// Done
		return true;
	} else {
		// Error!
		if (bUrgentMessageAllowed)
			System::GetInstance()->UrgentMessage("Failed to locate the PixelLight runtime.\n\n" \
												 "-> Is the PixelLight runtime registered correctly? (see e.g. \"PixelLightBuild.pdf\" for details)");
		return false;
	}
}


//[-------------------------------------------------------]
//[ Private static functions                              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Try to find the PixelLight runtime directory by using the PLCore shared library
*/
String Runtime::GetDirectory(EType nType)
{
	// Runtime installation type must be "LocalInstallation" or "SystemInstallation"
	if (nType == LocalInstallation || nType == SystemInstallation) {
		#ifdef PLCORE_STATIC
			// No doubt, the executable is using the static linked version of PLCore... We don't know any so called "runtime"...
		#else
			// Get the absolute path the PLCore shared library is in
			// (should never ever fail because this executable is already using this shared library...)
			// -> The result is e.g. "file://C:/PixelLight/Runtime/x86" on Windows
			// -> To ensure we have a comparable url, add '/'
			const String sAbsPLCorePath = GetPLCoreSharedLibraryDirectory() + '/';

			// Get the absolute filename of the running process
			// -> "Url::CutFilename()": The protocol is omitted for a local path (e.g. the result will not be "file://C:/Programs/App/")
			// -> To ensure we have a comparable url, use "Url::GetUrl()"
			const String sAbsProcessPath = Url(Url(System::GetInstance()->GetExecutableFilename()).CutFilename()).GetUrl();

			// Is the PLCore shared library within the same directory as the running process?
			if (sAbsPLCorePath == sAbsProcessPath) {
				// The PixelLight runtime is in the same directory as the running process, making this to a local installation
				return (nType == LocalInstallation) ? sAbsPLCorePath : "";	// Done
			} else {
				// The PixelLight runtime is registered within the system, making this to a system installation
				return (nType == SystemInstallation) ? sAbsPLCorePath : "";	// Done
			}
		#endif
	}

	// Error!
	return "";
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
