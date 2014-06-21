/*********************************************************\
 *  File: System.h                                       *
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


#ifndef __PLCORE_SYSTEM_H__
#define __PLCORE_SYSTEM_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Tools/Time.h"
#include "PLCore/Core/Singleton.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Thread;
class Console;
class SystemImpl;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Provides access to system and platform functions
*
*  @note
*    - Implementation of the bridge design pattern, this class is the abstraction
*/
class System : public Singleton<System> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Singleton<System>;


	//[-------------------------------------------------------]
	//[ Public static PLCore::Singleton functions             ]
	//[-------------------------------------------------------]
	// This solution enhances the compatibility with legacy compilers like GCC 4.2.1 used on Mac OS X 10.6
	// -> The C++11 feature "extern template" (C++11, see e.g. http://www2.research.att.com/~bs/C++0xFAQ.html#extern-templates) can only be used on modern compilers like GCC 4.6
	// -> We can't break legacy compiler support, especially when only the singletons are responsible for the break
	// -> See PLCore::Singleton for more details about singletons
	public:
		static PLCORE_API System *GetInstance();
		static PLCORE_API bool HasInstance();


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns relevant system information in one string
		*
		*  @return
		*    Information string
		*
		*  @remarks
		*    The returned information are
		*    - PLCore version information
		*    - PLCore build information
		*    - Endian
		*    - Platform
		*    - Operating system version
		*/
		PLCORE_API String GetInfo() const;

		/**
		*  @brief
		*    Detects the current machine's endian ("byte order")
		*
		*  @return
		*    'true' if the current machine is using 'Little Endian First', (LSB (least significant byte), also known as 'Intel-Format')
		*    'false' if the current machine is using 'Big Endian First', (MSB (most significant byte), also known as 'Motorola-Format')
		*
		*  @note
		*   - PixelLight is using 'Little Endian First' for it's binary file formats and so on
		*/
		PLCORE_API bool IsLittleEndian() const;

		/**
		*  @brief
		*    Returns the name of the platform
		*
		*  @return
		*    Platform string (for instance 'Windows' for Windows, 'Linux' for Linux and so on)
		*/
		inline String GetPlatform() const;

		/**
		*  @brief
		*    Returns the platform architecture
		*
		*  @return
		*    Platform architecture (for instance 'x86', 'x64', 'armeabi', 'armeabi-v7a' and so on)
		*/
		PLCORE_API String GetPlatformArchitecture() const;

		/**
		*  @brief
		*    Returns the platform bit architecture
		*
		*  @return
		*    Platform bit architecture (for instance '32' for x86, '64' for x64)
		*/
		PLCORE_API uint32 GetPlatformBitArchitecture() const;

		/**
		*  @brief
		*    Returns the name and version of the operating system
		*
		*  @return
		*    OS information string (for instance 'Windows 7 Service Pack 1 (Build 7601)')
		*/
		inline String GetOS() const;

		/**
		*  @brief
		*    Returns the directory separator used by the operation system
		*
		*  @return
		*    The directory separator used by the operation system (e.g. '/' on Linux, '\' on Windows)
		*/
		inline char GetSeparator() const;

		/**
		*  @brief
		*    Returns the shared library filename prefix used by the operation system
		*
		*  @return
		*    The shared library filename prefix used by the operation system (e.g. 'lib' as in 'libPLCore.so' on Linux, no prefix as in 'PLCore.dll' on Windows)
		*/
		inline String GetSharedLibraryPrefix() const;

		/**
		*  @brief
		*    Returns the shared library file extension used by the operation system
		*
		*  @return
		*    The shared library file extension used by the operation system (e.g. 'so' on Linux, 'dll' on Windows)
		*/
		inline String GetSharedLibraryExtension() const;

		/**
		*  @brief
		*    Get the CPU MHz
		*
		*  @return
		*    CPU MHz
		*/
		inline uint32 GetCPUMhz() const;

		/**
		*  @brief
		*    Returns the name of the computer
		*
		*  @return
		*    Computer name
		*/
		inline String GetComputerName() const;

		/**
		*  @brief
		*    Returns the current user name
		*
		*  @return
		*    User name
		*/
		inline String GetUserName() const;

		/**
		*  @brief
		*    Get home directory of the current user
		*
		*  @return
		*    The home directory of the current user (native path style)
		*
		*  @remarks
		*    Examples on different systems:
		*    - Linux:      /home/<username>
		*    - Windows XP: C:\Documents and Settings\<Username>
		*    - Windows 7:  C:\Users\<UserName>
		*/
		inline String GetUserHomeDir() const;

		/**
		*  @brief
		*    Get data directory of the current user
		*
		*  @return
		*    The data directory of the current user (native path style)
		*
		*  @remarks
		*    Examples on different systems:
		*    - Linux:      /home/<username>
		*    - Windows XP: C:\Documents and Settings\<Username>\Application Data
		*    - Windows 7:  C:\Users\<UserName>\AppData\Roaming
		*/
		inline String GetUserDataDir() const;

		/**
		*  @brief
		*    Get name of data directory for given application name
		*
		*  @param[in] sName
		*    Application name
		*
		*  @return
		*    Name of data directory in native style of used OS
		*
		*  @remarks
		*    Examples on different systems:
		*    - Linux:   GetDataDirName("PixelLight") -> ".pixellight"
		*    - Windows: GetDataDirName("PixelLight") -> "PixelLight"
		*/
		inline String GetDataDirName(const String &sName) const;

		/**
		*  @brief
		*    Get absolute path of application executable
		*
		*  @return
		*    Path to executable (native path style, e.g. on Windows: 'C:\MyApplication\Test.exe')
		*
		*  @note
		*    - Application executable = currently running process
		*/
		inline String GetExecutableFilename() const;

		/**
		*  @brief
		*    Returns the absolute filename of the shared library or executable a given memory address is located in
		*
		*  @param[in] pMemoryAddress
		*    Memory address ("memory anchor") inside the shared library or executable from which the absolute filename
		*    should be returned from, can be a null pointer in which case the result will be an empty string
		*
		*  @return
		*    Absolute filename of the shared library or executable the given memory address is located in (native path style),
		*    or an empty string in case it was impossible to determine the filename
		*
		*  @note
		*    - Result example on Windows: 'C:\MyApplication\PLCore.dll' or 'C:\MyApplication\Test.exe'
		*/
		inline String GetModuleFilenameByMemoryAddress(const void *pMemoryAddress) const;

		/**
		*  @brief
		*    Reads an environment variable
		*
		*  @param[in] sName
		*    Name of the environment variable to read
		*
		*  @return
		*    Content of the variable
		*/
		inline String GetEnvironmentVariable(const String &sName) const;

		/**
		*  @brief
		*    Writes an environment variable
		*
		*  @param[in] sName
		*    Name of the environment variable to write
		*  @param[in] sValue
		*    Value to write into the variable
		*
		*  @return
		*    'true' if the variable has been set successfully, else 'false'
		*/
		inline bool SetEnvironmentVariable(const String &sName, const String &sValue) const;

		/**
		*  @brief
		*    Deletes an environment variable
		*
		*  @param[in] sName
		*    Name of the environment variable to delete
		*/
		inline void DeleteEnvironmentVariable(const String &sName) const;

		/**
		*  @brief
		*    Executes a system command (create a new process)
		*
		*  @param[in] sCommand
		*    Command to execute
		*  @param[in] sParameters
		*    Parameters to pass to the command
		*  @param[in] sWorkingDir
		*    Working directory in which to execute the command
		*
		*  @return
		*    'true' if the command has been executed successfully, else 'false'
		*/
		inline bool Execute(const String &sCommand, const String &sParameters, const String &sWorkingDir = "") const;

		/**
		*  @brief
		*    Returns the current program locale language information
		*
		*  @remarks
		*    Internally 'setlocale' is used to collect the current program locale information - but
		*    only the 'language' information is returned as string. For instance, if the current locale
		*    is 'English_USA.1252', 'English' is returned, if the locale is 'French_France.1252', just
		*    'French' is returned and so on.
		*    This information can for instance be used to set a correct default language within the
		*    localization system of PixelLight.
		*
		*  @return
		*    The current program locale language information
		*/
		inline String GetLocaleLanguage() const;

		/**
		*  @brief
		*    Returns the current directory
		*
		*  @return
		*    Path to the current directory as the OS provided it (native path style)
		*
		*  @note
		*    - In case of an internally empty string you will receive "." in order to make it possible to add e.g. "/Data" and still end up in a valid path
		*    - On Windows for example "C:\Programs\App"
		*/
		inline String GetCurrentDir() const;

		/**
		*  @brief
		*    Sets the current directory
		*
		*  @param[in] sPath
		*    Path to the current directory
		*
		*  @return
		*    'true', if all went fine, else 'false'
		*
		*  @note
		*    - Whenever possible, do not manipulate the current directory, this may backfire when you don't expect it
		*/
		inline bool SetCurrentDir(const String &sPath);

		/**
		*  @brief
		*    Returns a pointer to the main thread
		*
		*  @return
		*    Main thread (assumed to be never a null pointer!)
		*/
		inline Thread *GetMainThread() const;

		/**
		*  @brief
		*    Returns a pointer to the current thread
		*
		*  @return
		*    Current thread (assumed to be never a null pointer!)
		*/
		inline Thread *GetCurrentThread() const;

		/**
		*  @brief
		*    Exit the application immediately
		*
		*  @param[in] nReturn
		*    Return value
		*/
		inline void Exit(int nReturn);

		/**
		*  @brief
		*    Returns the console instance
		*
		*  @return
		*    The console instance
		*/
		inline const Console &GetConsole() const;

		/**
		*  @brief
		*    Primitive way (e.g. by using a message box) to be able to tell the user that something went terrible wrong
		*
		*  @param[in] sMessage
		*    Message to show
		*
		*  @remarks
		*    Do not misuse this method in order to communicate with the user on a regular basis. This method does only
		*    exist to be able to tell the user that something went terrible wrong. There are situations were one can't
		*    use a log file, command line or something like this. Even when using e.g. a log file to write out error
		*    information - an application may e.g. just close directly after it's start without any further information
		*    and the user may even think that the application didn't start in the first place for an unknown reason.
		*    In such a situation, it's polite to inform the user that something went terrible wrong and providing
		*    a short hint how the issue may be solved. This method wasn't named "MessageBox()" by intend - because
		*    such a feature may not be available on the used platform or is handled in another way as a normal MS Windows
		*    message box.
		*/
		inline void UrgentMessage(const String &sMessage) const;

		//[-------------------------------------------------------]
		//[ Time functions                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the current date and time
		*
		*  @return
		*    Date and time string
		*/
		inline Time GetTime() const;

		/**
		*  @brief
		*    Returns the number of milliseconds since the system was started
		*
		*  @return
		*    Number of milliseconds elapsed since the system was started
		*/
		inline uint64 GetMilliseconds() const;

		/**
		*  @brief
		*    Retrieves the number of microseconds since the system was started
		*
		*  @return
		*    Number of microseconds elapsed since the system was started
		*/
		inline uint64 GetMicroseconds() const;

		/**
		*  @brief
		*    Suspend the current thread for a specified time period
		*
		*  @param[in] nMilliseconds
		*    Number of milliseconds to sleep, should not be 0 because the behavior is implementation dependent (use 'Yield()' instead)
		*/
		inline void Sleep(uint64 nMilliseconds) const;

		/**
		*  @brief
		*    Yields the rest of the current threads time slice
		*
		*  @remarks
		*    Yields the rest of the threads time slice so another active thread of equal or higher priority
		*    waiting for processor time can run. Note that this function may return immediately and the behavior
		*    is in general not exactly predictable. So, use this function to give the processor just a hint
		*    that you are willed to give processor time away.
		*/
		inline void Yield() const;

		//[-------------------------------------------------------]
		//[ Memory functions                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns an approximation of the percentage of used physical memory (0.0-100.0)
		*
		*  @return
		*    An approximation of the percentage of used physical memory
		*/
		inline float GetPercentageOfUsedPhysicalMemory() const;

		/**
		*  @brief
		*    Returns the total physical memory in bytes
		*
		*  @return
		*    The total physical memory in bytes
		*/
		inline uint64 GetTotalPhysicalMemory() const;

		/**
		*  @brief
		*    Returns the current free physical memory in bytes
		*
		*  @return
		*    The current free physical memory in bytes
		*/
		inline uint64 GetFreePhysicalMemory() const;

		/**
		*  @brief
		*    Returns the total virtual memory in bytes
		*
		*  @return
		*    The total virtual memory in bytes
		*/
		inline uint64 GetTotalVirtualMemory() const;

		/**
		*  @brief
		*    Returns the current free virtual memory in bytes
		*
		*  @return
		*    The current free virtual memory in bytes
		*/
		inline uint64 GetFreeVirtualMemory() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API System();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		System(const System &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~System();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		System &operator =(const System &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		SystemImpl	*m_pSystemImpl;	/**< Pointer to the system specific implementation (assumed to be never a null pointer!) */
		Thread		*m_pMainThread;	/**< Pointer to the main thread (assumed to be never a null pointer!) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/System/System.inl"


#endif // __PLCORE_SYSTEM_H__
