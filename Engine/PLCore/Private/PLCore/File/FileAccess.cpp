/*********************************************************\
 *  File: FileAccess.cpp                                 *
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
#include "PLCore/File/FileAccess.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
const FileAccess::Entry FileAccess::NullEntry;


//[-------------------------------------------------------]
//[ File access entry                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FileAccess::Entry::Entry() :
	m_nID(0),
	m_bCaseSensitive(true),
	m_nFlagsMask(0),
	m_nFlagsValue(0),
	m_nUserFlags(0),
	m_pUserData(nullptr)
{
}

/**
*  @brief
*    Copy constructor
*/
FileAccess::Entry::Entry(const Entry &cEntry) :
	m_nID(cEntry.m_nID),
	m_sUsername(cEntry.m_sUsername),
	m_sPassword(cEntry.m_sPassword),
	m_bCaseSensitive(cEntry.m_bCaseSensitive),
	m_nFlagsMask(cEntry.m_nFlagsMask),
	m_nFlagsValue(cEntry.m_nFlagsValue),
	m_nUserFlags(cEntry.m_nUserFlags),
	m_pUserData(cEntry.m_pUserData),
	m_sUserInfo(cEntry.m_sUserInfo)
{
}

/**
*  @brief
*    Destructor
*/
FileAccess::Entry::~Entry()
{
}

/**
*  @brief
*    Get ID
*/
uint32 FileAccess::Entry::GetID() const
{
	return m_nID;
}

/**
*  @brief
*    Assignment operator
*/
FileAccess::Entry &FileAccess::Entry::operator =(const Entry &cEntry)
{
	// Copy entry
	m_nID		  = cEntry.m_nID;
	m_sUsername   = cEntry.m_sUsername;
	m_sPassword   = cEntry.m_sPassword;
	m_nFlagsMask  = cEntry.m_nFlagsMask;
	m_nFlagsValue = cEntry.m_nFlagsValue;
	m_nUserFlags  = cEntry.m_nUserFlags;
	m_pUserData   = cEntry.m_pUserData;
	m_sUserInfo   = cEntry.m_sUserInfo;
	return *this;
}

/**
*  @brief
*    Comparison operator
*/
bool FileAccess::Entry::operator ==(const Entry &cEntry) const
{
	// Fast comparison
	return (m_nID == cEntry.m_nID);
}

/**
*  @brief
*    Get username
*/
String FileAccess::Entry::GetUsername() const
{
	return m_sUsername;
}

/**
*  @brief
*    Set username
*/
void FileAccess::Entry::SetUsername(const String &sUsername)
{
	m_sUsername = sUsername;
}

/**
*  @brief
*    Get password
*/
String FileAccess::Entry::GetPassword() const
{
	return m_sPassword;
}

/**
*  @brief
*    Set password
*/
void FileAccess::Entry::SetPassword(const String &sPassword)
{
	m_sPassword = sPassword;
}

/**
*  @brief
*    Set username and password
*/
void FileAccess::Entry::SetAuth(const String &sUsername, const String &sPassword)
{
	m_sUsername = sUsername;
	m_sPassword = sPassword;
}

/**
*  @brief
*    Get case-sensitive
*/
bool FileAccess::Entry::GetCaseSensitive() const
{
	return m_bCaseSensitive;
}

/**
*  @brief
*    Set case-sensitive
*/
void FileAccess::Entry::SetCaseSensitive(bool bCaseSensitive)
{
	m_bCaseSensitive = bCaseSensitive;
}

/**
*  @brief
*    Get flags mask
*/
uint32 FileAccess::Entry::GetFlagsMask() const
{
	return m_nFlagsMask;
}

/**
*  @brief
*    Set flags mask
*/
void FileAccess::Entry::SetFlagsMask(uint32 nFlagsMask)
{
	m_nFlagsMask = nFlagsMask;
}

/**
*  @brief
*    Get flags value
*/
uint32 FileAccess::Entry::GetFlagsValue() const
{
	return m_nFlagsValue;
}

/**
*  @brief
*    Set flags value
*/
void FileAccess::Entry::SetFlagsValue(uint32 nFlagsValue)
{
	m_nFlagsValue = nFlagsValue;
}

/**
*  @brief
*    Get user flags
*/
uint32 FileAccess::Entry::GetUserFlags() const
{
	return m_nUserFlags;
}

/**
*  @brief
*    Set user flags
*/
void FileAccess::Entry::SetUserFlags(uint32 nUserFlags)
{
	m_nUserFlags = nUserFlags;
}

/**
*  @brief
*    Get user data
*/
void *FileAccess::Entry::GetUserData() const
{
	return m_pUserData;
}

/**
*  @brief
*    Set user data
*/
void FileAccess::Entry::SetUserData(void *pUserData)
{
	m_pUserData = pUserData;
}

/**
*  @brief
*    Get user info
*/
String FileAccess::Entry::GetUserInfo() const
{
	return m_sUserInfo;
}

/**
*  @brief
*    Set user info
*/
void FileAccess::Entry::SetUserInfo(const String &sUserInfo)
{
	m_sUserInfo = sUserInfo;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FileAccess::FileAccess() :
	m_nNextID(1)
{
}

/**
*  @brief
*    Copy constructor
*/
FileAccess::FileAccess(const FileAccess &cFileAccess) :
	m_mapEntries(cFileAccess.m_mapEntries),
	m_nNextID(cFileAccess.m_nNextID)
{
}

/**
*  @brief
*    Destructor
*/
FileAccess::~FileAccess()
{
}

/**
*  @brief
*    Assignment operator
*/
FileAccess &FileAccess::operator =(const FileAccess &cFileAccess)
{
	m_mapEntries = cFileAccess.m_mapEntries;
	m_nNextID	 = cFileAccess.m_nNextID;
	return *this;
}

/**
*  @brief
*    Check if an entry with a given name is available
*/
bool FileAccess::HasEntry(const String &sName) const
{
	const Entry &cEntry = m_mapEntries.Get(sName);
	return (&cEntry != &HashMap<String, Entry>::Null);
}

/**
*  @brief
*    Returns an entry with a given name
*/
const FileAccess::Entry &FileAccess::GetEntry(const String &sName) const
{
	const Entry &cEntry = m_mapEntries.Get(sName);
	return ((&cEntry != &HashMap<String, Entry>::Null) ? cEntry : NullEntry);
}

const FileAccess::Entry &FileAccess::operator [](const String &sName) const
{
	const Entry &cEntry = m_mapEntries.Get(sName);
	return ((&cEntry != &HashMap<String, Entry>::Null) ? cEntry : NullEntry);
}

/**
*  @brief
*    Returns an entry with a given name
*/
FileAccess::Entry &FileAccess::GetEntry(const String &sName)
{
	Entry &cEntry = m_mapEntries.Get(sName);
	if (&cEntry == &HashMap<String, Entry>::Null) {
		Entry cNewEntry;
		cNewEntry.m_nID = m_nNextID++;
		m_mapEntries.Add(sName, cNewEntry);
		return m_mapEntries.Get(sName);
	}
	return cEntry;
}

FileAccess::Entry &FileAccess::operator [](const String &sName)
{
	Entry &cEntry = m_mapEntries.Get(sName);
	if (&cEntry == &HashMap<String, Entry>::Null) {
		Entry cNewEntry;
		cNewEntry.m_nID = m_nNextID++;
		m_mapEntries.Add(sName, cNewEntry);
		return m_mapEntries.Get(sName);
	}
	return cEntry;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
