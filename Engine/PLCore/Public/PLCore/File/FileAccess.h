/*********************************************************\
 *  File: FileAccess.h                                   *
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


#ifndef __PLCORE_FILEACCESS_H__
#define __PLCORE_FILEACCESS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/String.h"
#include "PLCore/Container/HashMap.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    This class contains information about how to access a file
*
*  @remarks
*    It can be used to supply authentication for files, e.g. username and password for a HTTP-connection
*    or a password for an encrypted ZIP-file, as well as other access options.
*    Please consider this information as hints that can be passed to the file-functions, it is not guaranteed
*    that everything you enter here will be used! (e.g. a ZIP-file won't be open with Write-Access, regardless
*    of what you pass in FlagsMask/FlagsValue)
*/
class FileAccess {


	//[-------------------------------------------------------]
	//[ File access entry                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Contains access information for a certain file/URL
		*/
		class Entry {


			//[-------------------------------------------------------]
			//[ Friends                                               ]
			//[-------------------------------------------------------]
			friend class FileAccess;


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Constructor
				*/
				PLCORE_API Entry();

				/**
				*  @brief
				*    Copy constructor
				*
				*  @param[in] cEntry
				*    Entry to copy
				*/
				PLCORE_API Entry(const Entry &cEntry);

				/**
				*  @brief
				*    Destructor
				*/
				PLCORE_API ~Entry();

				/**
				*  @brief
				*    Get ID
				*
				*  @return
				*    Entry ID
				*/
				PLCORE_API uint32 GetID() const;

				/**
				*  @brief
				*    Assignment operator
				*
				*  @param[in] cEntry
				*    Entry to copy
				*
				*  @return
				*    Reference to this instance
				*/
				PLCORE_API Entry &operator =(const Entry &cEntry);

				/**
				*  @brief
				*    Comparison operator
				*
				*  @param[in] cEntry
				*    Entry to compare to
				*
				*  @return
				*    'true' if both entries are equal, else 'false'
				*/
				PLCORE_API bool operator ==(const Entry &cEntry) const;

				/**
				*  @brief
				*    Get username
				*
				*  @return
				*    Username
				*/
				PLCORE_API String GetUsername() const;

				/**
				*  @brief
				*    Set username
				*
				*  @param[in] sUsername
				*    Username
				*/
				PLCORE_API void SetUsername(const String &sUsername);

				/**
				*  @brief
				*    Get password
				*
				*  @return
				*    Password
				*/
				PLCORE_API String GetPassword() const;

				/**
				*  @brief
				*    Set password
				*
				*  @param[in] sPassword
				*    Password
				*/
				PLCORE_API void SetPassword(const String &sPassword);

				/**
				*  @brief
				*    Set username and password
				*
				*  @param[in] sUsername
				*    Username
				*  @param[in] sPassword
				*    Password
				*/
				PLCORE_API void SetAuth(const String &sUsername, const String &sPassword);

				/**
				*  @brief
				*    Get case-sensitive
				*
				*  @return
				*    Case-sensitive-flag
				*/
				PLCORE_API bool GetCaseSensitive() const;

				/**
				*  @brief
				*    Set case-sensitive
				*
				*  @param[in] bCaseSensitive
				*    Case-sensitive-flag
				*/
				PLCORE_API void SetCaseSensitive(bool bCaseSensitive);

				/**
				*  @brief
				*    Get flags mask
				*
				*  @return
				*    Flags mask
				*/
				PLCORE_API uint32 GetFlagsMask() const;

				/**
				*  @brief
				*    Set flags mask
				*
				*  @param[in] nFlagsMask
				*    Flags mask
				*/
				PLCORE_API void SetFlagsMask(uint32 nFlagsMask);

				/**
				*  @brief
				*    Get flags value
				*
				*  @return
				*    Flags value
				*/
				PLCORE_API uint32 GetFlagsValue() const;

				/**
				*  @brief
				*    Set flags value
				*
				*  @param[in] nFlagsValue
				*    Flags value
				*/
				PLCORE_API void SetFlagsValue(uint32 nFlagsValue);

				/**
				*  @brief
				*    Get user flags
				*
				*  @return
				*    User flags
				*/
				PLCORE_API uint32 GetUserFlags() const;

				/**
				*  @brief
				*    Set user flags
				*
				*  @param[in] nUserFlags
				*    User flags
				*/
				PLCORE_API void SetUserFlags(uint32 nUserFlags);

				/**
				*  @brief
				*    Get user data
				*
				*  @return
				*    User data, can be a null pointer
				*/
				PLCORE_API void *GetUserData() const;

				/**
				*  @brief
				*    Set user data
				*
				*  @param[in] pUserData
				*    User data, can be a null pointer
				*/
				PLCORE_API void SetUserData(void *pUserData);

				/**
				*  @brief
				*    Get user info
				*
				*  @return
				*    User info
				*/
				PLCORE_API String GetUserInfo() const;

				/**
				*  @brief
				*    Set user info
				*
				*  @param[in] sUserInfo
				*    User info
				*/
				PLCORE_API void SetUserInfo(const String &sUserInfo);


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				uint32  m_nID;				/**< Unique entry ID */
				String  m_sUsername;		/**< Username for restricted access */
				String  m_sPassword;		/**< Password for restricted access */
				bool    m_bCaseSensitive;	/**< Case sensitivity */
				uint32  m_nFlagsMask;		/**< Mask of file-flags (see File::EAccess) */
				uint32  m_nFlagsValue;		/**< Values of file-flags (see File::EAccess) */
				uint32  m_nUserFlags;		/**< User defined flags */
				void   *m_pUserData;		/**< User defined data, can be a null pointer */
				String  m_sUserInfo;		/**< USer defined string */


		};


	//[-------------------------------------------------------]
	//[ Public static data                                    ]
	//[-------------------------------------------------------]
	public:
		static const Entry NullEntry;	/**< Empty entry */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API FileAccess();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cFileAccess
		*    FileAccess object to copy
		*/
		PLCORE_API FileAccess(const FileAccess &cFileAccess);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API ~FileAccess();

		/**
		*  @brief
		*    Assignment operator
		*
		*  @param[in] cFileAccess
		*    'FileAccess' object to copy
		*
		*  @return
		*    Reference to this instance
		*/
		PLCORE_API FileAccess &operator =(const FileAccess &cFileAccess);

		/**
		*  @brief
		*    Check if an entry with a given name is available
		*
		*  @param[in] sName
		*    Name of the entry
		*
		*  @return
		*    'true' if the entry is set, else 'false'
		*/
		PLCORE_API bool HasEntry(const String &sName) const;

		/**
		*  @brief
		*    Returns an entry with a given name
		*
		*  @param[in] sName
		*    Name of the entry
		*
		*  @return
		*    Reference to that entry if it exists, else 'NullEntry'
		*/
		PLCORE_API const Entry &GetEntry(const String &sName) const;
		PLCORE_API const Entry &operator [](const String &sName) const;

		/**
		*  @brief
		*    Returns an entry with a given name
		*
		*  @param[in] sName
		*    Name of the entry
		*
		*  @return
		*    Reference to that entry, creates a new entry if it didn't exist before
		*/
		PLCORE_API Entry &GetEntry(const String &sName);
		PLCORE_API Entry &operator [](const String &sName);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		HashMap<String, Entry>	m_mapEntries;	/**< List of FileAccess entries */
		uint32					m_nNextID;		/**< ID counter for entries */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_FILEACCESS_H__
