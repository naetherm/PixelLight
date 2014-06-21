/*********************************************************\
 *  File: StringBuffer.h                                 *
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


#ifndef __PLCORE_STRINGBUFFER_H__
#define __PLCORE_STRINGBUFFER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/StringBufferManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract base class that contains the buffer for a string
*
*  @note
*    - Implementation of the bridge design pattern, this class is the implementor of the 'String' abstraction
*/
class StringBuffer {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class String;
	friend class StringBufferManager;


	//[-------------------------------------------------------]
	//[ Protected static data                                 ]
	//[-------------------------------------------------------]
	protected:
		static PLCORE_API StringBufferManager Manager;	/**< String buffer manager */


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nLength
		*    Length of the string buffer (excluding the terminating zero)
		*  @param[in] nMaxLength
		*    Maximum available length of the string buffer (excluding the terminating zero)
		*/
		StringBuffer(uint32 nLength, uint32 nMaxLength, uint8 nType);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~StringBuffer();

		/**
		*  @brief
		*    Returns the length of the string buffer (excluding the terminating zero, NEVER 0!)
		*
		*  @return
		*    Length of the string buffer, NEVER 0!
		*/
		PLCORE_API uint32 GetLength() const;

		/**
		*  @brief
		*    Increases the reference count
		*
		*  @return
		*    Current reference count
		*/
		uint32 AddReference();

		/**
		*  @brief
		*    Decreases the reference count
		*
		*  @return
		*    Current reference count
		*/
		uint32 Release();

		/**
		*  @brief
		*    Gets the current reference count
		*
		*  @return
		*    Current reference count
		*/
		uint32 GetRefCount() const;


	//[-------------------------------------------------------]
	//[ Public virtual StringBuffer functions                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the internal string format
		*
		*  @return
		*    Internal string format (type: "String::EFormat")
		*/
		virtual uint32 GetFormat() const = 0;

		/**
		*  @brief
		*    Returns an ASCII string buffer version of the current string
		*
		*  @return
		*    ASCII string buffer version of the current string, NEVER a null pointer! (can be THIS)
		*
		*  @remarks
		*    This string buffer keeps a reference to the ASCII string buffer as long as
		*    this string buffer content is NOT manipulated. (ASCII version becomes 'dirty')
		*/
		virtual StringBufferASCII *GetASCII() = 0;

		/**
		*  @brief
		*    Returns an Unicode string buffer version of the current string
		*
		*  @return
		*    Unicode string buffer version of the current string, NEVER a null pointer! (can be THIS)
		*
		*  @remarks
		*    This string buffer keeps a reference to the Unicode string buffer as long as
		*    this string buffer content is NOT manipulated. (Unicode version becomes 'dirty')
		*/
		virtual StringBufferUnicode *GetUnicode() = 0;

		/**
		*  @brief
		*    Returns an UTF8 string buffer version of the current string
		*
		*  @return
		*    UTF8 string buffer version of the current string, NEVER a null pointer! (can be THIS)
		*
		*  @remarks
		*    This string buffer keeps a reference to the UTF8 string buffer as long as
		*    this string buffer content is NOT manipulated. (UTF8 version becomes 'dirty')
		*/
		virtual StringBufferUTF8 *GetUTF8() = 0;

		/**
		*  @brief
		*    Returns the number of bytes the string buffer is using (excluding the terminating zero, NEVER 0!)
		*
		*  @return
		*    The number of bytes the string buffer is using, NEVER 0!
		*/
		virtual uint32 GetNumOfBytes() const = 0;

		/**
		*  @brief
		*    Returns a clone of this string buffer
		*
		*  @return
		*    Clone of this string buffer, a null pointer on terrible error
		*
		*  @note
		*    - The internal memory is also cloned
		*    - The clone has no initial reference
		*/
		virtual StringBuffer *Clone() const = 0;

		/**
		*  @brief
		*    Returns a duplicate of this string buffer
		*
		*  @return
		*    Duplicate of this string buffer, a null pointer on terrible error
		*
		*  @remarks
		*    Unlike Clone(), this function ONLY returns a duplicate if the string buffer
		*    is shared between multiple strings, else a pointer to this string buffer is returned.
		*/
		virtual StringBuffer *Duplicate() = 0;

		/**
		*  @brief
		*    Compares this string and the given one lexicographically
		*
		*  @param[in] szString
		*    String to compare with
		*  @param[in] nLength
		*    Length of the given string (excluding the terminating zero, if another format NEVER 0!)
		*
		*  @return
		*    'true' if this string is less than the given one
		*/
		virtual bool IsLessThan(const char szString[], uint32 nLength) const = 0;
		virtual bool IsLessThan(const wchar_t szString[], uint32 nLength) const = 0;

		/**
		*  @brief
		*    Compares this string and the given one lexicographically
		*
		*  @param[in] szString
		*    String to compare with
		*  @param[in] nLength
		*    Length of the given string (excluding the terminating zero, if another format NEVER 0!)
		*
		*  @return
		*    'true' if this string is greater than the given one
		*/
		virtual bool IsGreaterThan(const char szString[], uint32 nLength) const = 0;
		virtual bool IsGreaterThan(const wchar_t szString[], uint32 nLength) const = 0;

		/**
		*  @brief
		*    Compare function (case sensitive)
		*
		*  @param[in] szString
		*    String to compare with
		*  @param[in] nLength
		*    Length of the given string (excluding the terminating zero, NEVER 0!)
		*  @param[in] nPos
		*    Start position within this string (MUST be valid!)
		*  @param[in] nCount
		*    Number of characters to compare, if 0 compare all characters
		*
		*  @return
		*    'true' if the two strings are identical, else 'false'
		*/
		virtual bool Compare(const char szString[], uint32 nLength, uint32 nPos, uint32 nCount) const = 0;
		virtual bool Compare(const wchar_t szString[], uint32 nLength, uint32 nPos, uint32 nCount) const = 0;

		/**
		*  @brief
		*    Compare function (case insensitive)
		*
		*  @param[in] szString
		*    String to compare with
		*  @param[in] nLength
		*    Length of the given string (excluding the terminating zero, NEVER 0!)
		*  @param[in] nPos 
		*    Start position within this string (MUST be valid!)
		*  @param[in] nCount
		*    Number of characters to compare, if 0 compare all characters
		*
		*  @return
		*    'true' if the two strings are identical, else 'false'
		*/
		virtual bool CompareNoCase(const char szString[], uint32 nLength, uint32 nPos, uint32 nCount) const = 0;
		virtual bool CompareNoCase(const wchar_t szString[], uint32 nLength, uint32 nPos, uint32 nCount) const = 0;

		/**
		*  @brief
		*    Determines whether the string is alphabetic or not
		*
		*  @return
		*    'true' if the string is alphabetic (or empty)
		*
		*  @note
		*    - Examples: 'abc' is alphabetic while 'abc12' or 'ab-c' are not
		*/
		virtual bool IsAlphabetic() const = 0;

		/**
		*  @brief
		*    Determines whether the string is alpha-numeric or not
		*
		*  @return
		*    'true' if the string is alpha-numeric (or empty)
		*
		*  @note
		*    - Examples: 'abc', '12' or 'abc12' are alpha-numeric while 'abc-12' is not
		*/
		virtual bool IsAlphaNumeric() const = 0;

		/**
		*  @brief
		*    Determines whether the string is numeric
		*
		*  @return
		*    'true' if the string is numeric (or empty)
		*
		*  @note
		*    - Examples: '5' or '0' are numeric, 
		*      while  '5.1', '.', 'AD', '3D', '5,5', '5.2.8' are not
		*/
		virtual bool IsNumeric() const = 0;

		/**
		*  @brief
		*    Checks whether the given string is a substring of this string or not
		*
		*  @param[in] szString
		*    String to check
		*  @param[in] nLength
		*    Length of the given string (excluding the terminating zero, if another format NEVER 0!)
		*
		*  @return
		*    'true', if the given string is a substring of this string, else 'false'
		*/
		virtual bool IsSubstring(const char szString[], uint32 nLength) const = 0;
		virtual bool IsSubstring(const wchar_t szString[], uint32 nLength) const = 0;

		/**
		*  @brief
		*    Returns the index of the substring if contained in this string
		*
		*  @param[in] szString
		*    String to check
		*  @param[in] nPos
		*    Start position within this string (MUST be valid!)
		*  @param[in] nLength
		*    Length of the given string (excluding the terminating zero, if another format NEVER 0!)
		*
		*  @return
		*    Index of the substring if found within this string, < 0 on error
		*/
		virtual int IndexOf(const char szString[], uint32 nPos, uint32 nLength) const = 0;
		virtual int IndexOf(const wchar_t szString[], uint32 nPos, uint32 nLength) const = 0;

		/**
		*  @brief
		*    Searches from backwards for the index of the substring within this string
		*
		*  @param[in] szString
		*    String to check
		*  @param[in] nPos
		*    Start position within this string (MUST be valid!)
		*  @param[in] nLength
		*    Length of the given string (excluding the terminating zero, NEVER 0!)
		*
		*  @return
		*    Index of the substring if found within this string, < 0 on error
		*/
		virtual int LastIndexOf(const char szString[], int nPos, uint32 nLength) const = 0;
		virtual int LastIndexOf(const wchar_t szString[], int nPos, uint32 nLength) const = 0;

		/**
		*  @brief
		*    Get a substring from the string
		*
		*  @param[in] nPos
		*    Start position (MUST be valid!)
		*  @param[in] nCount
		*    Number of characters to copy (MUST be valid!)
		*
		*  @return
		*    The substring (without any initial reference)
		*/
		virtual StringBuffer *GetSubstring(uint32 nPos, uint32 nCount) const = 0;

		/**
		*  @brief
		*    Change all characters to lower case
		*
		*  @return
		*    This string buffer if nothing was changed, else new string buffer (without any initial reference)
		*/
		virtual StringBuffer *ToLower() = 0;

		/**
		*  @brief
		*    Change all characters to upper case
		*
		*  @return
		*    This string buffer if nothing was changed, else new string buffer (without any initial reference)
		*/
		virtual StringBuffer *ToUpper() = 0;

		/**
		*  @brief
		*    Delete a substring
		*
		*  @param[in] nPos
		*    Start position of deletion (MUST be valid!)
		*  @param[in] nCount
		*    Number of characters to delete (MUST be valid!)
		*
		*  @return
		*    The new string buffer (without any initial reference)
		*/
		virtual StringBuffer *Delete(uint32 nPos, uint32 nCount) = 0;

		/**
		*  @brief
		*    Appends a string at the end of the current string
		*
		*  @param[in] szString
		*    String to insert (NEVER empty!)
		*  @param[in] nCount
		*    Number of characters to add (MUST be valid!)
		*
		*  @return
		*    The new string buffer (without any initial reference)
		*/
		virtual StringBuffer *Append(const char szString[], uint32 nCount) = 0;
		virtual StringBuffer *Append(const wchar_t szString[], uint32 nCount) = 0;

		/**
		*  @brief
		*    Insert a string at a given location
		*
		*  @param[in] szString
		*    String to insert (NEVER empty!)
		*  @param[in] nPos
		*    Position at which to insert the string (MUST be valid!)
		*  @param[in] nCount
		*    Number of characters to add (MUST be valid!)
		*
		*  @return
		*    The new string buffer (without any initial reference)
		*
		*  @note
		*    - Due to the possibility to insert the new string at every position,
		*      the implementation is more complex as the one of "Append()", so,
		*      whenever possible use "Append()" for better performance!
		*/
		virtual StringBuffer *Insert(const char szString[], uint32 nPos, uint32 nCount) = 0;
		virtual StringBuffer *Insert(const wchar_t szString[], uint32 nPos, uint32 nCount) = 0;

		/**
		*  @brief
		*    Replaces all occurrences of a character by another character
		*
		*  @param[in]  nOld
		*    Character to be replaced
		*  @param[in]  nNew
		*    Character to replace with (nNew != nOld!!)
		*  @param[out] nReplaced
		*    Receives the number of replaced characters (0 if new = old, MUST be set!)
		*
		*  @return
		*    This string buffer if nothing was changed, else new string buffer (without any initial reference)
		*/
		virtual StringBuffer *Replace(char nOld, char nNew, uint32 &nReplaced) = 0;
		virtual StringBuffer *Replace(wchar_t nOld, wchar_t nNew, uint32 &nReplaced) = 0;

		/**
		*  @brief
		*    Replaces all occurrences of a substring by another string
		*
		*  @param[in]  szOld
		*    Substring to be replaced (NEVER empty!)
		*  @param[in]  nOldLength
		*    Length of the old substring (MUST be valid!)
		*  @param[in]  szNew
		*    String to replace with (szNew != szOld, can be empty!)
		*  @param[in]  nNewLength
		*    Length of the new substring (MUST be valid but can be 0!)
		*  @param[out] nReplaced
		*    Receives the number of replaced characters (0 if new = old, MUST be set!)
		*
		*  @return
		*    This string buffer if nothing was changed, else new string buffer (without any initial reference)
		*/
		virtual StringBuffer *Replace(const char szOld[], uint32 nOldLength, const char szNew[], uint32 nNewLength, uint32 &nReplaced) = 0;
		virtual StringBuffer *Replace(const wchar_t szOld[], uint32 nOldLength, const wchar_t szNew[], uint32 nNewLength, uint32 &nReplaced) = 0;

		/**
		*  @brief
		*    Sets a character at the given index
		*
		*  @param[in] nIndex
		*    Character index (MUST be valid!)
		*  @param[in] nCharacter
		*    Character to set at the given index
		*
		*  @return
		*    This string buffer if nothing was changed, else new string buffer (without any initial reference)
		*/
		virtual StringBuffer *SetCharacter(uint32 nIndex, char nCharacter) = 0;
		virtual StringBuffer *SetCharacter(uint32 nIndex, wchar_t nCharacter) = 0;

		/**
		*  @brief
		*    Removes all whitespace (tabs and spaces) at the beginning of the string
		*
		*  @return
		*    This string buffer if nothing was changed, else new string buffer, (without any initial reference)
		*    a null pointer if the string is now empty
		*/
		virtual StringBuffer *TrimLeading() = 0;

		/**
		*  @brief
		*    Removes all whitespace (tabs and spaces) at the end of the string
		*
		*  @return
		*    This string buffer if nothing was changed, else new string buffer, (without any initial reference)
		*    a null pointer if the string is now empty
		*/
		virtual StringBuffer *TrimTrailing() = 0;

		/**
		*  @brief
		*    Removes line endings ("\r" or "\n") at the end of the string
		*
		*  @return
		*    This string buffer if nothing was changed, else new string buffer, (without any initial reference)
		*    a null pointer if the string is now empty
		*/
		virtual StringBuffer *RemoveLineEndings() = 0;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		uint32 m_nRefCount;		/**< Reference count - the "RefCount"-template isn't used because the string buffer manager requires some "special" access to this reference counter */
		uint32 m_nLength;		/**< Length of the string (excluding the terminating zero, NEVER 0!) */
		uint32 m_nMaxLength;	/**< Maximum available length of the string (excluding the terminating zero, NEVER 0!) */
		uint8  m_nType;			/**< String buffer type for variable inspection in debug mode */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
// Include the string buffer implementation headers if we are in debug mode so we can inspect the string in a quite comfortable way
#include "PLCore/String/StringBufferUTF8.h"
#include "PLCore/String/StringBufferASCII.h"
#include "PLCore/String/StringBufferUnicode.h"


#endif // __PLCORE_STRINGBUFFER_H__
