/*********************************************************\
 *  File: Runtime.h                                      *
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


#ifndef __PLCORE_RUNTIME_H__
#define __PLCORE_RUNTIME_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Tools/Version.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    PixelLight framework runtime class
*
*  @note
*    - There can be a local and system runtime at the same time, although this is not recommended
*    - On Windows 64 bit it's possible to have a 32 bit and a 64 bit PixelLight runtime version
*      installed at the same time (even in the PATH environment variable, the OS will automatically pick the correct dll)
*/
class Runtime {


	//[-------------------------------------------------------]
	//[ Definitions                                           ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Runtime installation type
		*/
		enum EType {
			StaticInstallation,	/**< The running process is using the static linked version of PLCore */
			LocalInstallation,	/**< The PixelLight runtime is in the same directory as the running process, making this to a local installation */
			SystemInstallation	/**< The PixelLight runtime is registered within the system, making this to a system installation */
		};


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Passive information                                   ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get used PixelLight installation type
		*
		*  @return
		*    Used PixelLight installation type the running process is using
		*/
		static PLCORE_API EType GetType();

		/**
		*  @brief
		*    Get PixelLight version
		*
		*  @return
		*    Version of this PixelLight installation the running process is using
		*/
		static PLCORE_API Version GetVersion();

		/**
		*  @brief
		*    Return whether or not this is a debug version of PixelLight
		*
		*  @return
		*    'true' if the running process is using a debug version of PixelLight, else 'false'
		*/
		static inline bool IsDebugVersion();

		/**
		*  @brief
		*    Get PixelLight suffix
		*
		*  @return
		*    Suffix of this PixelLight installation the running process is using
		*/
		static PLCORE_API String GetSuffix();

		/**
		*  @brief
		*    Try to find the local PixelLight runtime directory
		*
		*  @return
		*    Path to the local PixelLight runtime directory (e.g. "file://C:/MyApplication/x86" on Windows), or ""
		*/
		static inline String GetLocalDirectory();

		/**
		*  @brief
		*    Try to find the local PixelLight runtime data directory
		*
		*  @return
		*    Path to the local PixelLight runtime data directory (e.g. "file://C:/MyApplication/Data" on Windows), or ""
		*/
		static PLCORE_API String GetLocalDataDirectory();

		/**
		*  @brief
		*    Try to find the system PixelLight runtime directory
		*
		*  @return
		*    Path to the system PixelLight runtime directory (e.g. "file://C:/PixelLight/Runtime/x86" on Windows), or ""
		*/
		static PLCORE_API String GetSystemDirectory();

		/**
		*  @brief
		*    Try to find the system PixelLight runtime data directory
		*
		*  @return
		*    Path to the system PixelLight runtime data directory (e.g. "file://C:/PixelLight/Runtime/Data" on Windows), or ""
		*/
		static PLCORE_API String GetSystemDataDirectory();

		/**
		*  @brief
		*    Try to find the system PixelLight runtime directory by reading the registry
		*
		*  @return
		*    Path to the system PixelLight runtime directory (e.g. "file://C:/PixelLight/Runtime/x86" on Windows), or ""
		*
		*  @remarks
		*    For this method, you also need to add a key to the registry (or environment, depending on the used OS, in here
		*    the MS Windows terminology is used), so that the path to the build PixelLight runtime can be found during runtime.
		*    This means that there's a high probability that this registry key does not exist or it's configuration is invalid.
		*    So, if you don't really need this method, try to avoid using it to reduce possible errors. When e.g. using a static
		*    build of PLCore, this registry method has to be used in order to be able to find the system PixelLight runtime directory.
		*
		*    On MS Windows, this key has to be at "HKEY_LOCAL_MACHINE/SOFTWARE/PixelLight/PixelLight-SDK/Runtime" (or at
		*    "HKEY_LOCAL_MACHINE/SOFTWARE/Wow6432Node/PixelLight/PixelLight-SDK/Runtime" if you are using a 32 bit PixelLight
		*    SDK on a 64 bit MS Windows). This "Runtime"-key has e.g. the string value "C:/PixelLight/Bin/Runtime/x86/" (same
		*    as the PATH environment variable entry pointing to the PixelLight runtime directory).
		*
		*    On Linux, the "PL_RUNTIME" environment variable is used. If this variable does not exist,
		*    "/usr/local/share/pixellight/Runtime" will be used, if it exists.
		*
		*  @see
		*    - 'Registry' class documentation
		*/
		static PLCORE_API String GetRegistryDirectory();

		/**
		*  @brief
		*    Try to find the system PixelLight runtime data directory by reading the registry
		*
		*  @return
		*    Path to the system PixelLight runtime data directory (e.g. "file://C:/PixelLight/Runtime/Data" on Windows), or ""
		*
		*  @see
		*    - "GetRegistryDirectory()"
		*/
		static PLCORE_API String GetRegistryDataDirectory();

		/**
		*  @brief
		*    Try to find the PixelLight runtime directory used by the running process
		*
		*  @return
		*    Path to the PixelLight runtime directory used by the running process (e.g. "file://C:/MyApplication/x86" or "file://C:/PixelLight/Runtime/x86" etc. on Windows), or ""
		*
		*  @remarks
		*    As soon as an application executable is started and therefore has become a running process, it's bound to a
		*    certain PixelLight runtime. Usually, an OS will first search for required shared libraries within the same
		*    directory the application executable is in. If the PLCore shared library was found within the application
		*    executable directory, this means that a local PixelLight runtime is used. Of course, there can be a local
		*    and system PixelLight runtime at the same time (although this is not recommended), but this still means, that
		*    in the described situation, the local PLCore shared library is used and therefore the local PixelLight runtime.
		*    The "GetDirectory()"-method returns the PixelLight runtime used by the running process. This means that internally
		*    "GetLocalDirectory()" is called at first and if this method returns an empty string, a "GetSystemDirectory()"
		*    call follows in exactly this order.
		*/
		static PLCORE_API String GetDirectory();

		/**
		*  @brief
		*    Try to find the PixelLight runtime data directory used by the running process
		*
		*  @return
		*    Path to the PixelLight runtime data directory used by the running process (e.g. "file://C:/MyApplication/Data" or "file://C:/PixelLight/Runtime/Data" etc. on Windows), or ""
		*
		*  @see
		*    - GetDirectory()
		*/
		static PLCORE_API String GetDataDirectory();

		/**
		*  @brief
		*    Returns the name of the PLCore shared library
		*
		*  @return
		*    The name of the PLCore shared library (e.g. "libPLCore.so" or "PLCoreD.dll")
		*/
		static PLCORE_API String GetPLCoreSharedLibraryName();

		/**
		*  @brief
		*    Returns the absolute path to the directory the PLCore shared library is in
		*
		*  @return
		*    The absolute path to the directory the PLCore shared library is in (e.g. "file://C:/PixelLight/Runtime/x86" on Windows), empty string on error
		*/
		static PLCORE_API String GetPLCoreSharedLibraryDirectory();


		//[-------------------------------------------------------]
		//[ Active actions                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Sets the given PixelLight runtime directory
		*
		*  @param[in]  sDirectory
		*    Path to the PixelLight runtime directory (e.g. "C:\PixelLight\Runtime\x86" on Windows), or ""
		*  @param[out] pszErrorMessage
		*    If this parameter is no null pointer and there was an error, this string will receive a human readable error description,
		*    if there was no error this string is not touched
		*
		*  @return
		*    Returns 'true' if all went fine, else 'false'
		*
		*  @see
		*    - "GetDirectory()"
		*/
		static PLCORE_API bool SetDirectory(const String &sDirectory, String *pszErrorMessage = nullptr);

		/**
		*  @brief
		*    Scan system PixelLight runtime directory for compatible plugins and load them in
		*
		*  @param[in] sDirectory
		*    Directory to use (for example the result of "GetDirectory()", "GetLocalDirectory()" or "GetSystemDirectory()"), if empty string the result of "GetDirectory()" will be used
		*  @param[in] bDelayedPluginLoading
		*    'true' if it's allowed to perform delayed shared library loading to speed up the program start, else 'false'
		*
		*  @remarks
		*    The implementation does the following tasks:
		*    - Scan for plugins in PixelLight runtime directory non-recursively
		*
		*  @see
		*    - Runtime::GetDirectory()
		*    - ClassManager::ScanPlugins()
		*/
		static PLCORE_API void ScanDirectoryPlugins(const String &sDirectory = "", bool bDelayedPluginLoading = true);

		/**
		*  @brief
		*    Scan system PixelLight runtime directory for compatible data and register it
		*
		*  @param[in] sDirectory
		*    Directory to use (for example the result of "GetDataDirectory()", "GetLocalDataDirectory()" or "GetSystemDataDirectory()"), if empty string the result of "GetDataDirectory()" will be used
		*
		*  @remarks
		*    The implementation does the following tasks:
		*    - Add PixelLight runtime directory "Data/" as a base directory to the loadable manager
		*    - Scan for data in PixelLight runtime directory "Data/"
		*
		*  @see
		*    - LoadableManager::AddBaseDir()
		*    - LoadableManager::ScanPackages()
		*/
		static PLCORE_API void ScanDirectoryData(const String &sDirectory = "");

		/**
		*  @brief
		*    Scan PixelLight runtime directory for compatible plugins and load them in as well as scan for compatible data and register it
		*
		*  @param[in] bUrgentMessageAllowed
		*    Is this method allowed to show an urgent message to the user in case of a failure?
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @remarks
		*    In case you don't care about technical details and just want the stuff runnable, then just call this method
		*    and be happy. The PixelLight runtime to use is detected automatically, all available compatible plugins are
		*    registered as well as compatible data.
		*
		*  @see
		*    - ScanDirectoryPlugins()
		*    - ScanDirectoryData()
		*/
		static PLCORE_API bool ScanDirectoryPluginsAndData(bool bUrgentMessageAllowed = true);


	//[-------------------------------------------------------]
	//[ Private static functions                              ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Try to find the PixelLight runtime directory by using the PLCore shared library
		*
		*  @param[in] nType
		*    Runtime installation type, must be "LocalInstallation" or "SystemInstallation"
		*
		*  @return
		*    Path to the PixelLight runtime directory (e.g. "C:\MyApplication\x86" on Windows), or ""
		*/
		static PLCORE_API String GetDirectory(EType nType);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Runtime.inl"


#endif // __PLCORE_RUNTIME_H__
