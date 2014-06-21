/*********************************************************\
 *  File: DataObject.cpp                                 *
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
#include <PLCore/Core/MemoryManager.h>
#include "PLGui/Gui/Data/DataObject.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLGui {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
DataObject::DataObject() :
	m_nDataType(DataEmpty),
	m_sString(""),
	m_nValue(0),
	m_pData(nullptr),
	m_nDataSize(0)
{
}

/**
*  @brief
*    Constructor
*/
DataObject::DataObject(const String &sString) :
	m_nDataType(DataString),
	m_sString(sString),
	m_nValue(0),
	m_pData(nullptr),
	m_nDataSize(0)
{
}

/**
*  @brief
*    Constructor
*/
DataObject::DataObject(const Container<String> &lstFiles) :
	m_nDataType(DataFiles),
	m_sString(""),
	m_nValue(0),
	m_pData(nullptr),
	m_nDataSize(0)
{
	// Copy file names
	m_lstFiles = lstFiles;
}

/**
*  @brief
*    Constructor
*/
DataObject::DataObject(uint32 nValue) :
	m_nDataType(DataCustom),
	m_sString(""),
	m_nValue(nValue),
	m_pData(nullptr),
	m_nDataSize(0)
{
}

/**
*  @brief
*    Constructor
*/
DataObject::DataObject(uint8 *pData, uint32 nSize) :
	m_nDataType(DataBinary),
	m_sString(""),
	m_nValue(0),
	m_pData(nullptr),
	m_nDataSize(0)
{
	// Check if data is valid
	if (pData && nSize > 0) {
		// Allocate buffer
		m_pData		= new uint8[nSize];
		m_nDataSize = nSize;

		// Copy data
		MemoryManager::Copy(m_pData, pData, nSize);
	}
}

/**
*  @brief
*    Copy constructor
*/
DataObject::DataObject(const DataObject &cOther) :
	m_nDataType(DataEmpty),
	m_sString(""),
	m_nValue(0),
	m_pData(nullptr),
	m_nDataSize(0)
{
	// Copy string
	if (cOther.m_nDataType == DataString) {
		m_nDataType	= DataString;
		m_sString	= cOther.m_sString;
	}

	// Copy file names
	else if (cOther.m_nDataType == DataFiles) {
		m_nDataType	= DataFiles;
		m_lstFiles	= cOther.m_lstFiles;
	}

	// Copy custom data
	else if (cOther.m_nDataType == DataCustom) {
		m_nDataType	= DataCustom;
		m_nValue	= cOther.m_nValue;
	}

	// Copy binary data
	else if (cOther.m_nDataType == DataBinary && cOther.m_pData && cOther.m_nDataSize > 0) {
		// Allocate buffer
		m_nDataType	= DataBinary;
		m_pData		= new uint8[cOther.m_nDataSize];
		m_nDataSize = cOther.m_nDataSize;

		// Copy data
		MemoryManager::Copy(m_pData, cOther.m_pData, m_nDataSize);
	}
}

/**
*  @brief
*    Destructor
*/
DataObject::~DataObject()
{
	// Clear data
	Clear();
}

/**
*  @brief
*    Comparison operator
*/
bool DataObject::operator ==(const DataObject &cOther) const
{
	// Compare types
	if (m_nDataType == DataString && cOther.m_nDataType == DataString) {
		// Compare strings
		return (m_sString == cOther.m_sString);
	} else if (m_nDataType == DataFiles && cOther.m_nDataType == DataFiles) {
		// Compare file names
		return (m_lstFiles == cOther.m_lstFiles);
	} else if (m_nDataType == DataCustom && cOther.m_nDataType == DataCustom) {
		// Compare custom values
		return (m_nValue == cOther.m_nValue);
	} else if (m_nDataType == DataBinary && cOther.m_nDataType == DataBinary) {
		// Compare binary data size
		if (m_nDataSize == cOther.m_nDataSize) {
			// Compare binary data
			if (m_nDataSize > 0) return (MemoryManager::Compare(cOther.m_pData, m_pData, m_nDataSize) == 0);
			else				 return true;
		}
	}

	// Data types are different
	return false;
}

/**
*  @brief
*    Assignment operator
*/
DataObject &DataObject::operator =(const DataObject &cOther)
{
	// Clear data
	Clear();

	// Copy data
	if (cOther.m_nDataType == DataString) {
		// Copy string
		m_nDataType	= DataString;
		m_sString	= cOther.m_sString;
	} else if (cOther.m_nDataType == DataFiles) {
		// Copy file names
		m_nDataType	= DataFiles;
		m_lstFiles	= cOther.m_lstFiles;
	} else if (cOther.m_nDataType == DataCustom) {
		// Copy custom value
		m_nDataType	= DataCustom;
		m_nValue	= cOther.m_nValue;
	} else if (cOther.m_nDataType == DataBinary) {
		// Copy binary data
		if (cOther.m_pData && cOther.m_nDataSize > 0) {
			// Allocate buffer
			m_nDataType	= DataBinary;
			m_pData		= new uint8[cOther.m_nDataSize];
			m_nDataSize	= cOther.m_nDataSize;

			// Copy data
			MemoryManager::Copy(m_pData, cOther.m_pData, m_nDataSize);
		}
	}

	// Return reference to this object
	return *this;
}

/**
*  @brief
*    Get data type
*/
EDataType DataObject::GetType() const
{
	// Return command ID
	return m_nDataType;
}

/**
*  @brief
*    Get string data
*/
String DataObject::GetString() const
{
	// Return string
	return m_sString;
}

/**
*  @brief
*    Set string data
*/
void DataObject::Set(const String &sString)
{
	// Clear data
	Clear();

	// Set string
	m_nDataType	= DataString;
	m_sString	= sString;
}

/**
*  @brief
*    Get file names
*/
const Container<String> &DataObject::GetFiles() const
{
	// Return list of file names
	return m_lstFiles;
}

/**
*  @brief
*    Set file names
*/
void DataObject::Set(const Container<String> &lstFiles)
{
	// Clear data
	Clear();

	// Set file names
	m_nDataType	= DataFiles;
	m_lstFiles	= lstFiles;
}

/**
*  @brief
*    Get custom data
*/
uint32 DataObject::GetValue() const
{
	// Return value
	return m_nValue;
}

/**
*  @brief
*    Set custom data
*/
void DataObject::Set(uint32 nValue)
{
	// Clear data
	Clear();

	// Set value
	m_nDataType	= DataCustom;
	m_nValue	= nValue;
}

/**
*  @brief
*    Get binary data size
*/
uint32 DataObject::GetBinarySize() const
{
	// Return binary data size
	return m_nDataSize;
}

/**
*  @brief
*    Get binary data
*/
uint8 *DataObject::GetBinaryData() const
{
	// Return binary data
	return m_pData;
}

/**
*  @brief
*    Set binary data
*/
void DataObject::Set(uint8 *pData, uint32 nSize)
{
	// Clear data
	Clear();

	// Set binary data
	m_nDataType	= DataBinary;
	m_pData		= nullptr;
	m_nDataSize	= 0;

	// Check if data is valid
	if (pData && nSize > 0) {
		// Allocate buffer
		m_pData		= new uint8[nSize];
		m_nDataSize = nSize;

		// Copy data
		MemoryManager::Copy(m_pData, pData, nSize);
	}
}

/**
*  @brief
*    Clear data
*/
void DataObject::Clear()
{
	// Destroy buffer
	if (m_pData) {
		delete [] m_pData;
	}

	// Reset data
	m_nDataType	= DataEmpty;
	m_sString	= "";
	m_nValue	= 0;
	m_pData		= nullptr;
	m_nDataSize = 0;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
