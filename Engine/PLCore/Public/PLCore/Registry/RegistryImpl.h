/*********************************************************\
 *  File: RegistryImpl.h                                 *
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


#ifndef __PLCORE_REGISTRY_IMPL_H__
#define __PLCORE_REGISTRY_IMPL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/String.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract base class for platform specific 'Registry' implementations
*/
class RegistryImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Registry;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		RegistryImpl();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RegistryImpl();


	//[-------------------------------------------------------]
	//[ Protected virtual RegistryImpl functions              ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Get type of registry
		*
		*  @return
		*    Type of registry ('RegistryNone' if no registry is available on your system!) (type: "Registry::ERegistry")
		*/
		virtual uint32 GetRegistryType() const = 0;

		/**
		*  @brief
		*    Open registry key
		*
		*  @param[in] nKey
		*    Registry key (type: "Registry::EKey")
		*  @param[in] sSubKey
		*    Name of subkey
		*  @param[in] nAccess
		*    Access modes (combination of 'EAccess' values)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool Open(uint32 nKey, const String &sSubKey, uint32 nAccess) = 0;

		/**
		*  @brief
		*    Create a new registry key
		*
		*  @param[in] nKey
		*    Registry key (type: "Registry::EKey")
		*  @param[in] sSubKey
		*    Name of subkey
		*  @param[in] nAccess
		*    Access modes (combination of 'EAccess' values)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool Create(uint32 nKey, const String &sSubKey, uint32 nAccess) = 0;

		/**
		*  @brief
		*    Delete the registry key
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool Delete() = 0;

		/**
		*  @brief
		*    Close registry key
		*/
		virtual void Close() = 0;

		/**
		*  @brief
		*    Get ID of opened registry key
		*
		*  @return
		*    Key ID (type: "Registry::EKey")
		*/
		virtual uint32 GetOpenKey() const = 0;

		/**
		*  @brief
		*    Get name of opened sub-key
		*
		*  @return
		*    Name of sub-key
		*/
		virtual String GetOpenSubKey() const = 0;

		/**
		*  @brief
		*    Get access mode
		*
		*  @return
		*    Access mode
		*/
		virtual uint32 GetOpenAccessMode() const = 0;

		/**
		*  @brief
		*    Returns the number of sub-keys
		*
		*  @return
		*    Number of sub-keys
		*/
		virtual uint32 GetNumOfSubKeys() const = 0;

		/**
		*  @brief
		*    Get a sub-key of the opened registry key
		*
		*  @param[in] nIndex
		*    Index of the sub-key
		*
		*  @return
		*    Name of the sub-key, or "" if no more sub-keys are present
		*/
		virtual String GetSubKey(uint32 nIndex) const = 0;

		/**
		*  @brief
		*    Returns the number of values
		*
		*  @return
		*    Number of values
		*/
		virtual uint32 GetNumOfValues() const = 0;

		/**
		*  @brief
		*    Get a value of the opened registry key
		*
		*  @param[in] nIndex
		*    Index of the value
		*
		*  @return
		*    Name of the value, or "" if no more values are present
		*/
		virtual String GetValue(uint32 nIndex) const = 0;

		/**
		*  @brief
		*    Get the type of a given value
		*
		*  @param[in] sName
		*    Name of the value
		*
		*  @return
		*    Type of the value (type: "Registry::EType")
		*/
		virtual uint32 GetValueType(const String &sName) const = 0;

		/**
		*  @brief
		*    Get a value of type string
		*
		*  @param[in] sName
		*    Name of the value
		*
		*  @return
		*    String value
		*/
		virtual String GetValueString(const String &sName) const = 0;

		/**
		*  @brief
		*    Get a value of type 'dword'
		*
		*  @param[in] sName
		*    Name of the value
		*
		*  @return
		*    Value
		*/
		virtual uint32 GetValueDWord(const String &sName) const = 0;

		/**
		*  @brief
		*    Get a value of type binary
		*
		*  @param[in] sName
		*    Name of the value
		*  @param[out] pBuffer
		*    Buffer to receive the value, if a null pointer, returns the number of  bytes required
		*  @param[in]  nSize
		*    Size of the given buffer in bytes, ignored if 'pBuffer' is a null pointer
		*
		*  @return
		*    Number of bytes written to the buffer
		*/
		virtual uint32 GetValueBinary(const String &sName, uint8 *pBuffer, uint32 nSize) const = 0;

		/**
		*  @brief
		*    Set a value of type string
		*
		*  @param[in] sName
		*    Name of the value
		*  @param[in] sValue
		*    String value to set
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool SetValueString(const String &sName, const String &sValue) = 0;

		/**
		*  @brief
		*    Set a value of type 'dword'
		*
		*  @param[in] sName
		*    Name of the value
		*  @param[in] nValue
		*    Value to set
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool SetValueDWord(const String &sName, uint32 nValue) = 0;

		/**
		*  @brief
		*    Set a value of type binary
		*
		*  @param[in] sName
		*    Name of the value
		*  @param[in] pBuffer
		*    Buffer containing the value to set (if a null pointer, the function fails)
		*  @param[in] nSize
		*    Size of the given buffer in bytes
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool SetValueBinary(const String &sName, const uint8 *pBuffer, uint32 nSize) = 0;

		/**
		*  @brief
		*    Write all values to the registry
		*/
		virtual void Flush() = 0;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_REGISTRY_IMPL_H__
