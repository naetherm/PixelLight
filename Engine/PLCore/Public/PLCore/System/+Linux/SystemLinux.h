/*********************************************************\
 *  File: SystemLinux.h                                  *
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


#ifndef __PLCORE_SYSTEM_LINUX_H__
#define __PLCORE_SYSTEM_LINUX_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <sys/utsname.h>
#include "PLCore/System/ConsoleLinux.h"
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
*    Linux 'System' implementation
*/
class SystemLinux : public SystemImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class System;


	//[-------------------------------------------------------]
	//[ Protected structures                                  ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Holds memory information
		*/
		struct MemoryInformation {
			uint64 nTotalPhysicalMemory;	/**< Total physical memory in bytes */
			uint64 nFreePhysicalMemory;		/**< Free physical memory in bytes */
			uint64 nTotalSwapMemory;		/**< Total swap memory in bytes */
			uint64 nFreeSwapMemory;			/**< Free swap memory in bytes */
		};


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		SystemLinux();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SystemLinux();

		/**
		*  @brief
		*    Reads memory information from the '/proc/meminfo'-file
		*
		*  @param[out] sMemoryInformation
		*    Receives the memory information
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool GetMemoryInformation(MemoryInformation &sMemoryInformation) const;


	//[-------------------------------------------------------]
	//[ Protected virtual SystemImpl functions                ]
	//[-------------------------------------------------------]
	protected:
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
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		ConsoleLinux   m_cConsole;		/**< Console instance */
		bool		   m_bSysInfoInit;	/**< Has 'm_sName' a valid value? */
		struct utsname m_sName;			/**< Basic system information */
		String		   m_sFilename;		/**< Application filename */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_SYSTEM_LINUX_H__
