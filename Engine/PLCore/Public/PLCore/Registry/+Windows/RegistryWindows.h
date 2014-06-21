/*********************************************************\
 *  File: RegistryWindows.h                              *
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


#ifndef __PLCORE_REGISTRY_WINDOWS_H__
#define __PLCORE_REGISTRY_WINDOWS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/+Windows/PLCoreWindowsIncludes.h"
#include "PLCore/Registry/RegistryImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Windows 'Registry' implementation
*/
class RegistryWindows : public RegistryImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Registry;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*/
		RegistryWindows();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RegistryWindows();


	//[-------------------------------------------------------]
	//[ Private virtual RegistryImpl functions                ]
	//[-------------------------------------------------------]
	private:
		virtual uint32 GetRegistryType() const override;
		virtual bool Open(uint32 nKey, const String &sSubKey, uint32 nAccess) override;
		virtual bool Create(uint32 nKey, const String &sSubKey, uint32 nAccess) override;
		virtual bool Delete() override;
		virtual void Close() override;
		virtual uint32 GetOpenKey() const override;
		virtual String GetOpenSubKey() const override;
		virtual uint32 GetOpenAccessMode() const override;
		virtual uint32 GetNumOfSubKeys() const override;
		virtual String GetSubKey(uint32 nIndex) const override;
		virtual uint32 GetNumOfValues() const override;
		virtual String GetValue(uint32 nIndex) const override;
		virtual uint32 GetValueType(const String &sName) const override;
		virtual String GetValueString(const String &sName) const override;
		virtual uint32 GetValueDWord(const String &sName) const override;
		virtual uint32 GetValueBinary(const String &sName, uint8 *pBuffer, uint32 nSize) const override;
		virtual bool SetValueString(const String &sName, const String &sValue) override;
		virtual bool SetValueDWord(const String &sName, uint32 nValue) override;
		virtual bool SetValueBinary(const String &sName, const uint8 *pBuffer, uint32 nSize) override;
		virtual void Flush() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32	m_nKey;		/**< Registry key (type: "Registry::EKey") */
		String	m_sSubKey;	/**< Sub key */
		uint32	m_nAccess;	/**< Access modes */
		HKEY	m_hKey;		/**< Windows registry key handle */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_REGISTRY_WINDOWS_H__
