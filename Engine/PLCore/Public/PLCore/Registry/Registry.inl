/*********************************************************\
 *  File: Registry.inl                                   *
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
#include "PLCore/Registry/RegistryImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get type of registry
*/
inline Registry::ERegistry Registry::GetRegistryType() const
{
	return m_pRegistryImpl ? static_cast<Registry::ERegistry>(m_pRegistryImpl->GetRegistryType()) : None;
}

/**
*  @brief
*    Open registry key
*/
inline bool Registry::Open(EKey nKey, const String &sSubKey, uint32 nAccess)
{
	return m_pRegistryImpl ? m_pRegistryImpl->Open(nKey, sSubKey, nAccess) : false;
}

/**
*  @brief
*    Create a new registry key
*/
inline bool Registry::Create(EKey nKey, const String &sSubKey, uint32 nAccess)
{
	return m_pRegistryImpl ? m_pRegistryImpl->Create(nKey, sSubKey, nAccess) : false;
}

/**
*  @brief
*    Delete the registry key
*/
inline bool Registry::Delete()
{
	return m_pRegistryImpl ? m_pRegistryImpl->Delete() : false;
}

/**
*  @brief
*    Close registry key
*/
inline void Registry::Close()
{
	if (m_pRegistryImpl)
		return m_pRegistryImpl->Close();
}

/**
*  @brief
*    Get ID of opened registry key
*/
inline Registry::EKey Registry::GetOpenKey() const
{
	return m_pRegistryImpl ? static_cast<Registry::EKey>(m_pRegistryImpl->GetOpenKey()) : KeyNone;
}

/**
*  @brief
*    Get name of opened sub-key
*/
inline String Registry::GetOpenSubKey() const
{
	return m_pRegistryImpl ? m_pRegistryImpl->GetOpenSubKey() : "";
}

/**
*  @brief
*    Get access modes
*/
inline uint32 Registry::GetOpenAccessMode() const
{
	return m_pRegistryImpl ? m_pRegistryImpl->GetOpenAccessMode() : 0;
}

/**
*  @brief
*    Returns the number of sub-keys
*/
inline uint32 Registry::GetNumOfSubKeys() const
{
	return m_pRegistryImpl ? m_pRegistryImpl->GetNumOfSubKeys() : 0;
}

/**
*  @brief
*    Get a sub-key of the opened registry key
*/
inline String Registry::GetSubKey(uint32 nIndex) const
{
	return m_pRegistryImpl ? m_pRegistryImpl->GetSubKey(nIndex) : "";
}

/**
*  @brief
*    Returns the number of values
*/
inline uint32 Registry::GetNumOfValues() const
{
	return m_pRegistryImpl ? m_pRegistryImpl->GetNumOfValues() : 0;
}

/**
*  @brief
*    Get a value of the opened registry key
*/
inline String Registry::GetValue(uint32 nIndex) const
{
	return m_pRegistryImpl ? m_pRegistryImpl->GetValue(nIndex) : "";
}

/**
*  @brief
*    Get the type of a given value
*/
inline Registry::EType Registry::GetValueType(const String &sName) const
{
	return m_pRegistryImpl ? static_cast<Registry::EType>(m_pRegistryImpl->GetValueType(sName)) : TypeNone;
}

/**
*  @brief
*    Get a value of type string
*/
inline String Registry::GetValueString(const String &sName) const
{
	return m_pRegistryImpl ? m_pRegistryImpl->GetValueString(sName) : "";
}

/**
*  @brief
*    Get a value of type dword
*/
inline uint32 Registry::GetValueDWord(const String &sName) const
{
	return m_pRegistryImpl ? m_pRegistryImpl->GetValueDWord(sName) : 0;
}

/**
*  @brief
*    Get a value of type binary
*/
inline uint32 Registry::GetValueBinary(const String &sName, uint8 *pBuffer, uint32 nSize) const
{
	return m_pRegistryImpl ? m_pRegistryImpl->GetValueBinary(sName, pBuffer, nSize) : 0;
}

/**
*  @brief
*    Set a value of type string
*/
inline bool Registry::SetValueString(const String &sName, const String &sValue)
{
	return m_pRegistryImpl ? m_pRegistryImpl->SetValueString(sName, sValue) : false;
}

/**
*  @brief
*    Set a value of type dword
*/
inline bool Registry::SetValueDWord(const String &sName, uint32 nValue)
{
	return m_pRegistryImpl ? m_pRegistryImpl->SetValueDWord(sName, nValue) : false;
}

/**
*  @brief
*    Set a value of type binary
*/
inline bool Registry::SetValueBinary(const String &sName, const uint8 *pBuffer, uint32 nSize)
{
	return m_pRegistryImpl ? m_pRegistryImpl->SetValueBinary(sName, pBuffer, nSize) : false;
}

/**
*  @brief
*    Write all values to the registry
*/
inline void Registry::Flush()
{
	if (m_pRegistryImpl)
		return m_pRegistryImpl->Flush();
}

/**
*  @brief
*    Copy operator
*/
inline Registry &Registry::operator =(const Registry &cRegistry)
{
	// Open the same key again
	Open(cRegistry.GetOpenKey(), cRegistry.GetOpenSubKey(), cRegistry.GetOpenAccessMode());

	// Return this object
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
