/*********************************************************\
 *  File: SystemWindows.h                                *
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


#ifndef __PLCORE_SYSTEM_WINDOWS_H__
#define __PLCORE_SYSTEM_WINDOWS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/+Windows/PLCoreWindowsIncludes.h"
#include "PLCore/System/+Windows/ConsoleWindows.h"
#include "PLCore/System/SystemImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Windows 'System' implementation
*/
class SystemWindows : public SystemImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class System;


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Converts a given Windows error code received by 'GetLastError()' into a human readable string
		*
		*  @param[in] nErrorCode
		*    Windows error code to convert into a string
		*
		*  @return
		*    The resulting string
		*/
		static PLCORE_API String ErrorCodeToString(DWORD nErrorCode);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*/
		SystemWindows();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SystemWindows();


	//[-------------------------------------------------------]
	//[ Private virtual SystemImpl functions                  ]
	//[-------------------------------------------------------]
	private:
		virtual String GetPlatform() const override;
		virtual String GetOS() const override;
		virtual char GetSeparator() const override;
		virtual String GetSharedLibraryPrefix() const override;
		virtual String GetSharedLibraryExtension() const override;
		virtual uint32 GetCPUMhz() const override;
		virtual String GetComputerName() const override;
		virtual String GetUserName() const override;
		virtual String GetUserHomeDir() const override;
		virtual String GetUserDataDir() const override;
		virtual String GetDataDirName(const String &sName) const override;
		virtual String GetExecutableFilename() const override;
		virtual String GetModuleFilenameByMemoryAddress(const void *pMemoryAddress) const override;
		virtual String GetEnvironmentVariable(const String &sName) const override;
		virtual bool SetEnvironmentVariable(const String &sName, const String &sValue) const override;
		virtual void DeleteEnvironmentVariable(const String &sName) const override;
		virtual bool Execute(const String &sCommand, const String &sParameters, const String &sWorkingDir) const override;
		virtual String GetLocaleLanguage() const override;
		virtual String GetCurrentDir() const override;
		virtual bool SetCurrentDir(const String &sPath) override;
		virtual Thread *GetCurrentThread() const override;
		virtual void Exit(int nReturn) override;
		virtual const Console &GetConsole() const override;
		virtual void UrgentMessage(const String &sMessage) const override;
		virtual Time GetTime() const override;
		virtual uint64 GetMilliseconds() const override;
		virtual uint64 GetMicroseconds() const override;
		virtual void Sleep(uint64 nMilliseconds) const override;
		virtual void Yield() const override;
		virtual float GetPercentageOfUsedPhysicalMemory() const override;
		virtual uint64 GetTotalPhysicalMemory() const override;
		virtual uint64 GetFreePhysicalMemory() const override;
		virtual uint64 GetTotalVirtualMemory() const override;
		virtual uint64 GetFreeVirtualMemory() const override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ConsoleWindows m_cConsole;				/**< Console instance */
		LARGE_INTEGER  m_nPerformanceFrequency;	/**< Frequency of the performance counter */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_SYSTEM_WINDOWS_H__
