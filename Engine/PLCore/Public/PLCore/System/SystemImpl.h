/*********************************************************\
 *  File: SystemImpl.h                                   *
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


#ifndef __PLCORE_SYSTEM_IMPL_H__
#define __PLCORE_SYSTEM_IMPL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Tools/Time.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Thread;
class Console;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract base class for platform specific 'System' implementations
*
*  @note
*    - Implementation of the bridge design pattern, this class is the implementor of the 'System' abstraction
*/
class SystemImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class System;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		SystemImpl();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SystemImpl();


	//[-------------------------------------------------------]
	//[ Protected virtual SystemImpl functions                ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Returns the name of the platform
		*
		*  @return
		*    Platform string (for instance 'Windows' for Windows, 'Linux' for Linux and so on)
		*/
		virtual String GetPlatform() const = 0;

		/**
		*  @brief
		*    Returns the name and version of the operating system
		*
		*  @return
		*    OS information string (for instance 'Windows 7 Service Pack 1 (Build 7601)')
		*/
		virtual String GetOS() const = 0;

		/**
		*  @brief
		*    Returns the directory separator used by the operation system
		*
		*  @return
		*    The directory separator used by the operation system (e.g. '/' on Linux, '\' on Windows)
		*/
		virtual char GetSeparator() const = 0;

		/**
		*  @brief
		*    Returns the shared library filename prefix used by the operation system
		*
		*  @return
		*    The shared library filename prefix used by the operation system (e.g. 'lib' as in 'libPLCore.so' on Linux, no prefix as in 'PLCore.dll' on Windows)
		*/
		virtual String GetSharedLibraryPrefix() const = 0;

		/**
		*  @brief
		*    Returns the shared library file extension used by the operation system
		*
		*  @return
		*    The shared library file extension used by the operation system (e.g. 'so' on Linux, 'dll' on Windows)
		*/
		virtual String GetSharedLibraryExtension() const = 0;

		/**
		*  @brief
		*    Get the CPU MHz
		*
		*  @return
		*    CPU MHz
		*/
		virtual uint32 GetCPUMhz() const = 0;

		/**
		*  @brief
		*    Returns the name of the computer
		*
		*  @return
		*    Computer name
		*/
		virtual String GetComputerName() const = 0;

		/**
		*  @brief
		*    Returns the current user name
		*
		*  @return
		*    User name
		*/
		virtual String GetUserName() const = 0;

		/**
		*  @brief
		*    Get home directory of the current user
		*
		*  @return
		*    The home directory of the current user (native path style)
		*/
		virtual String GetUserHomeDir() const = 0;

		/**
		*  @brief
		*    Get data directory of the current user
		*
		*  @return
		*    The data directory of the current user (native path style)
		*/
		virtual String GetUserDataDir() const = 0;

		/**
		*  @brief
		*    Get name of data directory for given application name
		*
		*  @param[in] sName
		*    Application name
		*
		*  @return
		*    Name of data directory in native style of used OS
		*/
		virtual String GetDataDirName(const String &sName) const = 0;

		/**
		*  @brief
		*    Get absolute path of application executable
		*
		*  @return
		*    Path to executable (native path style, e.g. on Windows: 'C:\MyApplication\Test.exe')
		*/
		virtual String GetExecutableFilename() const = 0;

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
		virtual String GetModuleFilenameByMemoryAddress(const void *pMemoryAddress) const = 0;

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
		virtual String GetEnvironmentVariable(const String &sName) const = 0;

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
		virtual bool SetEnvironmentVariable(const String &sName, const String &sValue) const = 0;

		/**
		*  @brief
		*    Deletes an environment variable
		*
		*  @param[in] sName
		*    Name of the environment variable to delete
		*/
		virtual void DeleteEnvironmentVariable(const String &sName) const = 0;

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
		virtual bool Execute(const String &sCommand, const String &sParameters, const String &sWorkingDir) const = 0;

		/**
		*  @brief
		*    Returns the current program locale language information
		*
		*  @return
		*    The current program locale language information
		*/
		virtual String GetLocaleLanguage() const = 0;

		/**
		*  @brief
		*    Returns the current directory
		*
		*  @return
		*    Path to the current directory as the OS provided it (native path style)
		*
		*  @note
		*    - On Windows for example "C:\Programs\App"
		*/
		virtual String GetCurrentDir() const = 0;

		/**
		*  @brief
		*    Sets the current directory
		*
		*  @param[in] sPath
		*    Path to the current directory
		*
		*  @return
		*    'true', if all went fine, else 'false'
		*/
		virtual bool SetCurrentDir(const String &sPath) = 0;

		/**
		*  @brief
		*    Returns a pointer to the current thread
		*
		*  @return
		*    Current thread, a null pointer on main thread or error
		*/
		virtual Thread *GetCurrentThread() const = 0;

		/**
		*  @brief
		*    Exit the application immediately
		*
		*  @param[in] nReturn
		*    Return value
		*/
		virtual void Exit(int nReturn) = 0;

		/**
		*  @brief
		*    Returns the console instance
		*
		*  @return
		*    The console instance
		*/
		virtual const Console &GetConsole() const = 0;

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
		virtual void UrgentMessage(const String &sMessage) const = 0;

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
		virtual Time GetTime() const = 0;

		/**
		*  @brief
		*    Returns the number of milliseconds since the system was started
		*
		*  @return
		*    Number of milliseconds elapsed since the system was started
		*/
		virtual uint64 GetMilliseconds() const = 0;

		/**
		*  @brief
		*    Retrieves the number of microseconds since the system was started
		*
		*  @return
		*    Number of microseconds elapsed since the system was started
		*/
		virtual uint64 GetMicroseconds() const = 0;

		/**
		*  @brief
		*    Suspend the current thread for a specified time period
		*
		*  @param[in] nMilliseconds
		*    Number of milliseconds to sleep
		*/
		virtual void Sleep(uint64 nMilliseconds) const = 0;

		/**
		*  @brief
		*    Yields the rest of the current threads time slice
		*/
		virtual void Yield() const = 0;

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
		virtual float GetPercentageOfUsedPhysicalMemory() const = 0;

		/**
		*  @brief
		*    Returns the total physical memory in bytes
		*
		*  @return
		*    The total physical memory in bytes
		*/
		virtual uint64 GetTotalPhysicalMemory() const = 0;

		/**
		*  @brief
		*    Returns the current free physical memory in bytes
		*
		*  @return
		*    The current free physical memory in bytes
		*/
		virtual uint64 GetFreePhysicalMemory() const = 0;

		/**
		*  @brief
		*    Returns the total virtual memory in bytes
		*
		*  @return
		*    The total virtual memory in bytes
		*/
		virtual uint64 GetTotalVirtualMemory() const = 0;

		/**
		*  @brief
		*    Returns the current free virtual memory in bytes
		*
		*  @return
		*    The current free virtual memory in bytes
		*/
		virtual uint64 GetFreeVirtualMemory() const = 0;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_SYSTEM_IMPL_H__
