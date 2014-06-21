/*********************************************************\
 *  File: String.h                                       *
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


#ifndef __PLCORE_STRING_H__
#define __PLCORE_STRING_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/PLCore.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class StringBuffer;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    String class
*
*  @remarks
*    Because strings are quite fundamental, there are several optimizations in place to make dealing
*    with strings as fast as possible. The string class uses a 'copy on change' technique -
*    therefore copying one string into another is quite performant because the internal string buffer
*    is shared as long as a string doesn't change. As result comparing strings 'can' also be very fast
*    and the internal string buffer can be ASCII OR Unicode in a quite flexible way. To
*    enhance the string performance, the internal string buffers are managed by a string buffer
*    manager to avoid to many memory allocations/deallocations. For an additional performance
*    improvement, memory is traded for speed, meaning that additional characters are allocated
*    within the internal string buffer manager for future use. This way, appending new characters
*    to a string is usually quite fast.
*
*    As long as you don't save your source codes in an UTF8 format you can also use the ASCII
*    extension Ansi, meaning characters between 128-256. But with an UTF8 format, this may cause
*    serious problems and you should use Unicode instead ASCII for characters above 128 (using
*    codepage based ASCII is not recommended) to avoid encoding troubles!
*
*    For best compatibility with other string techniques, this class is using pointers instead of
*    [] references, so a null pointer is a valid parameter.
*
*  @verbatim
*    Usage example:
*    String sS;                        // Test string instance
*    sS = "Mini";                      // Set string to 'Mini' as ASCII
*    sS += " Me";                      // Concatenate 'Me' (ASCII) to string
*    const char *pS = sS.GetASCII();   // Get pointer to ASCII string content
*    const char *pS = sS;              // Get pointer to ASCII string content (same as above)
*    sS = L"Mini";                     // Set string to 'Mini' as Unicode
*    sS = L"\u65e5\u672c\u8a9e";       // Set string to 'nihon' (= Japanese) as Unicode
*    sS = String::FromUTF8("Mini");    // Set string to 'Mini' as UTF8
*  @endverbatim
*
*  @note
*    - With 'length' we mean the 'number of characters' and not the 'number of bytes'
*    - Don't forget that wchar_t does not have the same number of bytes on every OS, so DO NOT use wchar_t for serialization, use UTF8 for that!
*    - PixelLight is using the compiler option "wchar_t is treated as built-in type", while other libraries like Qt are not - so you may want to
*      use UTF8 for string exchange between different libraries...
*    - If you care about best possible performance (even if nearly not measurable), use character string methods when dealing with characters - for
*      example, write "String sS = 'A';" instead of "String sS = "A";", this way, the string method already knows that you provided just a single
*      character and don't need to count characters internally
*    - Implementation of the bridge design pattern (with a pinch of the proxy design pattern for the copy-on-write/change), this class is the abstraction
*/
class String {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Internal string format
		*/
		enum EFormat {
			ASCII   = 0,	/**< ASCII, 1 byte per character (American Standard Code for Information Interchange, 0-128 defined, above undefined) */
			Unicode = 1,	/**< Unicode, (sometimes called 'multi-byte' or 'wide character') normally two bytes (UTF-16) on Microsoft Windows per character, four bytes long on UNIX systems (UTF-32) */
			UTF8    = 2		/**< UTF8, only used internally */
		};


	//[-------------------------------------------------------]
	//[ Static functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Creates a new formatted string (like sprintf)
		*
		*  @param[in] pszFormat
		*    The format string ... -> The variable arguments (can be a null pointer)
		*
		*  @return
		*    New string
		*
		*  @remarks
		*    If possible, try to avoid to use this method because it's usually considered to be "slow" due to
		*    it's internal complexity. Therefore, write something like
		*      String sMyString = String("The number ") + 42 + " is fantastic!";
		*    instead of
		*      String sMyString = String::Format("The number %d is fantastic!", 42);
		*/
		static PLCORE_API String Format(const char *pszFormat, ...);
		static PLCORE_API String Format(const wchar_t *pszFormat, ...);

		/**
		*  @brief
		*    Sets the character string as UTF8
		*
		*  @param[in] pszUTF8
		*    Pointer to the character string as UTF8, can be a null pointer
		*  @param[in] nLength
		*    Length of the given string, (excluding the terminating zero) if negative, the length is
		*    calculated automatically
		*  @param[in] nNumOfBytes
		*    Number of bytes of the string buffer (excluding the terminating zero, MUST be valid if not 0!)
		*
		*  @return
		*    New UTF8 string
		*/
		static PLCORE_API String FromUTF8(const char *pszUTF8, int nLength = -1, uint32 nNumOfBytes = 0);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline String();

		/**
		*  @brief
		*    Character constructor
		*
		*  @param[in] nValue
		*    Single character to put into the string
		*/
		PLCORE_API String(char nValue);
		PLCORE_API String(wchar_t nValue);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pszString
		*    String to save, can be a null pointer
		*  @param[in] bCopy
		*    Copy the given string or use this one directly? Do ONLY set bCopy to 'false'
		*    if you are sure there can't go anything wrong - the string class will take over
		*    control of the string buffer and also destroy it
		*  @param[in] nLength
		*    Length of the given string, (excluding the terminating zero) if negative, the length is
		*    calculated automatically
		*/
		PLCORE_API String(const char *pszString, bool bCopy = true, int nLength = -1);
		PLCORE_API String(const wchar_t *pszString, bool bCopy = true, int nLength = -1);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] sString
		*    String
		*/
		PLCORE_API String(const String &sString);

		/**
		*  @brief
		*    Number constructor
		*
		*  @param[in] nValue
		*    Number to put into the string
		*/
		inline String(bool bValue);
		PLCORE_API String(int nValue);
		inline String(int64 nValue);
		inline String(uint8 nValue);
	//	inline String(uint16 nValue);	// We can't do that because wchar_t may be defined the same way as uint16
		inline String(uint32 nValue);
		inline String(uint64 nValue);
		inline String(float fValue);
		inline String(double fValue);

		/**
		*  @brief
		*    Pointer constructor
		*
		*  @param[in] pValue
		*    Pointer to put into the string
		*/
		inline String(void *pValue);

		/**
		*  @brief
		*    Internal copy constructor
		*
		*  @param[in] pStringBuffer
		*    String buffer to use, can be a null pointer
		*
		*  @note
		*    - This constructor is ONLY used inside PLCore and therefore it's not exported
		*      and usable from 'outside'
		*/
		String(StringBuffer *pStringBuffer);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~String();

		/**
		*  @brief
		*    Returns the length of the string
		*
		*  @return
		*    Length of the string (excluding the terminating zero)
		*/
		inline uint32 GetLength() const;

		/**
		*  @brief
		*    Returns the internal string format
		*
		*  @return
		*    Internal string format
		*/
		inline EFormat GetFormat() const;

		/**
		*  @brief
		*    Returns the number of bytes the string is using in a specified format (ASCII, Unicode, UTF8)
		*
		*  @param[in] nFormat
		*    The internal string format
		*
		*  @return
		*    The number of bytes the string is using in the specified format (excluding the terminating zero)
		*/
		PLCORE_API uint32 GetNumOfBytes(EFormat nFormat) const;

		/**
		*  @brief
		*    Get a ASCII character of the string
		*
		*  @param[in] nIndex
		*    Position of the ASCII character to retrieve
		*
		*  @return
		*    ASCII character at the given position within the string, terminating zero (\0) on error
		*/
		inline char operator [](uint32 nIndex) const;

		/**
		*  @brief
		*    Returns the character string as ASCII
		*
		*  @return
		*    Pointer to the character string as ASCII, never a null pointer
		*
		*  @remarks
		*    If the internal format of this string doesn't match the requested format, an internal temp
		*    string of the requested format is created and backuped until this original string is manipulated.
		*    Then, the internal backup get's dirty/invalid - so, do NEVER backup the returned pointer by self!
		*
		*  @note
		*    - Do NOT manipulate or even mess up the memory by writing outside the given buffer!
		*/
		inline const char *GetASCII() const;

		/**
		*  @brief
		*    Returns the character string as ASCII
		*
		*  @return
		*    Pointer to the character string as ASCII, never a null pointer
		*
		*  @see
		*    - GetASCII()
		*/
		inline operator const char *() const;

		/**
		*  @brief
		*    Returns the character string as Unicode
		*
		*  @return
		*    Pointer to the character string as Unicode, never a null pointer
		*
		*  @see
		*    - GetASCII()
		*/
		inline const wchar_t *GetUnicode() const;

		/**
		*  @brief
		*    Returns the character string as Unicode
		*
		*  @return
		*    Pointer to the character string as Unicode, never a null pointer
		*
		*  @see
		*    - GetASCII()
		*/
		inline operator const wchar_t *() const;

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] sString
		*    String to copy
		*
		*  @return
		*    Reference to this string
		*/
		inline String &operator =(const String &sString);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] pszString
		*    String to copy, can be a null pointer
		*
		*  @return
		*    Reference to this string
		*/
		PLCORE_API String &operator =(const char *pszString);
		PLCORE_API String &operator =(const wchar_t *pszString);

		/**
		*  @brief
		*    Concatenate strings
		*
		*  @param[in] sString
		*    String to add
		*
		*  @return
		*    Reference to the new string
		*/
		PLCORE_API String operator +(const String &sString) const;

		/**
		*  @brief
		*    Concatenate strings
		*
		*  @param[in] pszString
		*    String to add, can be a null pointer
		*
		*  @return
		*    Reference to the new string
		*/
		PLCORE_API String operator +(const char *pszString) const;
		PLCORE_API String operator +(const wchar_t *pszString) const;

		/**
		*  @brief
		*    Concatenate strings
		*
		*  @param[in] pszString
		*    String to add, can be a null pointer
		*  @param[in] sString
		*    String to add
		*
		*  @return
		*    Reference to the new string
		*/
		PLCORE_API friend String operator +(const char *pszString, const String &sString);
		PLCORE_API friend String operator +(const wchar_t *pszString, const String &sString);

		/**
		*  @brief
		*    Concatenate strings
		*
		*  @param[in] sString
		*    String to add
		*
		*  @return
		*    Reference to this string
		*/
		PLCORE_API String &operator +=(const String &sString);

		/**
		*  @brief
		*    Concatenate strings
		*
		*  @param[in] pszString
		*    String to add, can be a null pointer
		*
		*  @return
		*    Reference to this string
		*/
		PLCORE_API String &operator +=(const char *pszString);
		PLCORE_API String &operator +=(const wchar_t *pszString);

		/**
		*  @brief
		*    Compares this string and the given one lexicographically
		*
		*  @param[in] sString
		*    String to compare with
		*
		*  @return
		*    'true' if this string is less than the given one
		*/
		PLCORE_API bool operator <(const String &sString) const;

		/**
		*  @brief
		*    Compares this string and the given one lexicographically
		*
		*  @param[in] pszString
		*    String to compare with, can be a null pointer
		*
		*  @return
		*    'true' if this string is less than the given one
		*/
		PLCORE_API bool operator <(const char *pszString) const;
		PLCORE_API bool operator <(const wchar_t *pszString) const;

		/**
		*  @brief
		*    Compares this string and the given one lexicographically
		*
		*  @param[in] sString
		*    String to compare with
		*
		*  @return
		*    'true' if this string is greater than the given one
		*/
		PLCORE_API bool operator >(const String &sString) const;

		/**
		*  @brief
		*    Compares this string and the given one lexicographically
		*
		*  @param[in] pszString
		*    String to compare with, can be a null pointer
		*
		*  @return
		*    'true' if this string is greater than the given one
		*/
		PLCORE_API bool operator >(const char *pszString) const;
		PLCORE_API bool operator >(const wchar_t *pszString) const;

		/**
		*  @brief
		*    Compare operator (case sensitive)
		*
		*  @param[in] sString
		*    String to compare with
		*
		*  @return
		*    'true' if the two strings are identical, else 'false'
		*/
		inline bool operator ==(const String &sString) const;

		/**
		*  @brief
		*    Compare operator (case sensitive)
		*
		*  @param[in] pszString
		*    String to compare with, can be a null pointer
		*
		*  @return
		*    'true' if the two strings are identical, else 'false'
		*/
		inline bool operator ==(const char *pszString) const;
		inline bool operator ==(const wchar_t *pszString) const;

		/**
		*  @brief
		*    Compare operator (case sensitive)
		*
		*  @param[in] sString
		*    String to compare with
		*
		*  @return
		*    'true' if the two strings are different, else 'false'
		*/
		inline bool operator !=(const String &sString) const;

		/**
		*  @brief
		*    Compare operator (case sensitive)
		*
		*  @param[in] pszString
		*    String to compare with, can be a null pointer
		*
		*  @return
		*    'true' if the two strings are different, else 'false'
		*/
		inline bool operator !=(const char *pszString) const;
		inline bool operator !=(const wchar_t *pszString) const;

		/**
		*  @brief
		*    Compare function (case sensitive)
		*
		*  @param[in] sString
		*    String to compare with
		*  @param[in] nPos
		*    Start position within this string
		*  @param[in] nCount
		*    Number of characters to compare, if negative compare all characters, if 0 the result is "true"
		*
		*  @return
		*    'true' if the two strings are identical, else 'false'
		*/
		PLCORE_API bool Compare(const String &sString, uint32 nPos = 0, int nCount = -1) const;

		/**
		*  @brief
		*    Compare function (case sensitive)
		*
		*  @param[in] pszString
		*    String to compare with, can be a null pointer
		*  @param[in] nPos
		*    Start position within this string
		*  @param[in] nCount
		*    Number of characters to compare, if negative compare all characters, if 0 the result is "true"
		*
		*  @return
		*    'true' if the two strings are identical, else 'false'
		*/
		PLCORE_API bool Compare(const char *pszString, uint32 nPos = 0, int nCount = -1) const;
		PLCORE_API bool Compare(const wchar_t *pszString, uint32 nPos = 0, int nCount = -1) const;

		/**
		*  @brief
		*    Compare function (case insensitive)
		*
		*  @param[in] sString
		*    String to compare with
		*  @param[in] nPos
		*    Start position within this string
		*  @param[in] nCount
		*    Number of characters to compare, if negative compare all characters, if 0 the result is "true"
		*
		*  @return
		*    'true' if the two strings are identical, else 'false'
		*/
		PLCORE_API bool CompareNoCase(const String &sString, uint32 nPos = 0, int nCount = -1) const;

		/**
		*  @brief
		*    Compare function (case insensitive)
		*
		*  @param[in] pszString
		*    String to compare with, can be a null pointer
		*  @param[in] nPos
		*    Start position within this string
		*  @param[in] nCount
		*    Number of characters to compare, if negative compare all characters, if 0 the result is "true"
		*
		*  @return
		*    'true' if the two strings are identical, else 'false'
		*/
		PLCORE_API bool CompareNoCase(const char *pszString, uint32 nPos = 0, int nCount = -1) const;
		PLCORE_API bool CompareNoCase(const wchar_t *pszString, uint32 nPos = 0, int nCount = -1) const;

		/**
		*  @brief
		*    Determines whether the string is alphabetic or not
		*
		*  @return
		*    'true' if the string is alphabetic, else 'false' (maybe the string is empty?)
		*
		*  @note
		*    - Examples: 'abc' is alphabetic while 'abc12' or 'ab-c' are not
		*/
		inline bool IsAlphabetic() const;

		/**
		*  @brief
		*    Determines whether the string is alpha-numeric or not
		*
		*  @return
		*    'true' if the string is alpha-numeric, else 'false' (maybe the string is empty?)
		*
		*  @note
		*    - Examples: 'abc', '12' or 'abc12' are alpha-numeric while 'abc-12' is not
		*/
		inline bool IsAlphaNumeric() const;

		/**
		*  @brief
		*    Determines whether the string is numeric
		*
		*  @return
		*    'true' if the string is numeric, else 'false' (maybe the string is empty?)
		*
		*  @note
		*    - Examples: '5' or '0' are numeric,
		*      while  '5.1', '.', 'AD', '3D', '5,5', '5.2.8' are not
		*/
		inline bool IsNumeric() const;

		/**
		*  @brief
		*    Checks whether the given string is a substring of this string or not
		*
		*  @param[in] sString
		*    String to check, if empty string, 'true' will be returned
		*
		*  @return
		*    'true', if the given string is a substring of this string, (or both are empty) else 'false'
		*/
		PLCORE_API bool IsSubstring(const String &sString) const;

		/**
		*  @brief
		*    Checks whether the given string is a substring of this string or not
		*
		*  @param[in] pszString
		*    String to check, if a null pointer or empty, 'true' will be returned
		*
		*  @return
		*    'true', if the given string is a substring of this string, (or both are empty) else 'false' else 'false'
		*/
		PLCORE_API bool IsSubstring(const char *pszString) const;
		PLCORE_API bool IsSubstring(const wchar_t *pszString) const;

		/**
		*  @brief
		*    Returns the index of the substring if contained in this string
		*
		*  @param[in] sString
		*    String to check, if empty string, < 0 will be returned
		*  @param[in] nPos
		*    Start position within this string
		*
		*  @return
		*    Index of the substring if found within this string, < 0 on error
		*    (maybe the given position is invalid) or if both strings are empty
		*/
		PLCORE_API int IndexOf(const String &sString, uint32 nPos = 0) const;

		/**
		*  @brief
		*    Returns the index of the substring if contained in this string
		*
		*  @param[in] pszString
		*    String to check, if a null pointer or empty, < 0 will be returned
		*  @param[in] nPos
		*    Start position within this string
		*
		*  @return
		*    Index of the substring if found within this string, < 0 on error
		*    (maybe the given position is invalid) or if both strings are empty
		*/
		PLCORE_API int IndexOf(const char *pszString, uint32 nPos = 0) const;
		PLCORE_API int IndexOf(const wchar_t *pszString, uint32 nPos = 0) const;

		/**
		*  @brief
		*    Searches from backwards for the index of the substring within this string
		*
		*  @param[in] sString
		*    String to check, if empty string, < 0 will be returned
		*  @param[in] nPos
		*    Start position within this string, if < 0 start at the last character
		*
		*  @return
		*    Index of the substring if found within this string, < 0 on error
		*    (maybe the given position is invalid) or if both strings are empty
		*/
		PLCORE_API int LastIndexOf(const String &sString, int nPos = -1) const;

		/**
		*  @brief
		*    Searches from backwards for the index of the substring within this string
		*
		*  @param[in] pszString
		*    String to check, if a null pointer or empty, < 0 will be returned
		*  @param[in] nPos
		*    Start position within this string, if < 0 start at the last character
		*
		*  @return
		*    Index of the substring if found within this string, < 0 on error
		*    (maybe the given position is invalid) or if both strings are empty
		*/
		PLCORE_API int LastIndexOf(const char *pszString, int nPos = -1) const;
		PLCORE_API int LastIndexOf(const wchar_t *pszString, int nPos = -1) const;

		/**
		*  @brief
		*    Get a substring from the string
		*
		*  @param[in] nPos
		*    Start position
		*  @param[in] nCount
		*    Number of characters to copy, negative for everything from nPos to the end of the string
		*
		*  @return
		*    The substring
		*/
		PLCORE_API String GetSubstring(uint32 nPos, int nCount = -1) const;

		/**
		*  @brief
		*    Change all characters to lower case
		*
		*  @return
		*    Reference to this string
		*/
		inline String &ToLower();

		/**
		*  @brief
		*    Change all characters to upper case
		*
		*  @return
		*    Reference to this string
		*/
		inline String &ToUpper();

		/**
		*  @brief
		*    Delete a substring
		*
		*  @param[in] nPos
		*    Start position of deletion
		*  @param[in] nCount
		*    Number of characters to delete, if negative delete all characters
		*
		*  @return
		*    Reference to this string
		*/
		PLCORE_API String &Delete(uint32 nPos = 0, int nCount = -1);

		/**
		*  @brief
		*    Insert a character at a given location
		*
		*  @param[in] nCharacter
		*    Character to insert
		*  @param[in] nPos
		*    Position at which to insert the character
		*
		*  @return
		*    Reference to this string
		*
		*  @note
		*    - In case you just want to append a character, you may want to use the faster "+="-operator instead
		*/
		inline String &Insert(char nCharacter, uint32 nPos = 0);
		inline String &Insert(wchar_t nCharacter, uint32 nPos = 0);

		/**
		*  @brief
		*    Insert a string at a given location
		*
		*  @param[in] sString
		*    String to insert
		*  @param[in] nPos
		*    Position at which to insert the string
		*  @param[in] nCount
		*    Number of characters to add, if negative insert the whole string
		*
		*  @return
		*    Reference to this string
		*
		*  @note
		*    - In case you just want to append a string, you may want to use the faster "+="-operator instead
		*/
		PLCORE_API String &Insert(const String &sString, uint32 nPos = 0, int nCount = -1);

		/**
		*  @brief
		*    Insert a string at a given location
		*
		*  @param[in] pszString
		*    String to insert, can be a null pointer
		*  @param[in] nPos
		*    Position at which to insert the string
		*  @param[in] nCount
		*    Number of characters to add, if negative insert the whole string
		*
		*  @return
		*    Reference to this string
		*
		*  @note
		*    - In case you just want to append a string, you may want to use the faster "+="-operator instead
		*/
		PLCORE_API String &Insert(const char *pszString, uint32 nPos = 0, int nCount = -1);
		PLCORE_API String &Insert(const wchar_t *pszString, uint32 nPos = 0, int nCount = -1);

		/**
		*  @brief
		*    Copies a string
		*
		*  @param[in] pszString
		*    String to copy, can be a null pointer
		*  @param[in] nCount
		*    Number of characters to copy, if negative copy the whole string
		*
		*  @return
		*    Reference to this string
		*/
		PLCORE_API String &Copy(const char *pszString, int nCount = -1);
		PLCORE_API String &Copy(const wchar_t *pszString, int nCount = -1);

		/**
		*  @brief
		*    Replaces all occurrences of a character by another character
		*
		*  @param[in] nOld
		*    Character to be replaced
		*  @param[in] nNew
		*    Character to replace with
		*
		*  @return
		*    Number of replaced characters (0 if new = old)
		*/
		PLCORE_API uint32 Replace(char nOld, char nNew);
		PLCORE_API uint32 Replace(wchar_t nOld, wchar_t nNew);

		/**
		*  @brief
		*    Replaces all occurrences of a substring by another string
		*
		*  @param[in] sOld
		*    Substring to be replaced
		*  @param[in] sNew
		*    String to replace with
		*
		*  @return
		*    Number of replaced substrings (0 if sNew = sOld or sOld is empty)
		*/
		PLCORE_API uint32 Replace(const String &sOld, const String &sNew);

		/**
		*  @brief
		*    Replaces all occurrences of a substring by another string
		*
		*  @param[in] pszOld
		*    Substring to be replaced, can be a null pointer
		*  @param[in] pszNew
		*    String to replace with, can be a null pointer
		*
		*  @return
		*    Number of replaced substrings (0 if pszNew = pszOld or pszOld is a null pointer or empty)
		*/
		PLCORE_API uint32 Replace(const char *pszOld, const char *pszNew);
		PLCORE_API uint32 Replace(const wchar_t *pszOld, const wchar_t *pszNew);

		/**
		*  @brief
		*    Sets a character at the given index
		*
		*  @param[in] nIndex
		*    Character index
		*  @param[in] nCharacter
		*    Character to set at the given index
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCORE_API bool SetCharacter(uint32 nIndex, char nCharacter);
		PLCORE_API bool SetCharacter(uint32 nIndex, wchar_t nCharacter);

		/**
		*  @brief
		*    Removes all whitespace (tabs and spaces) at the beginning of the string
		*
		*  @return
		*    Reference to this string
		*/
		inline String &TrimLeading();

		/**
		*  @brief
		*    Removes all whitespace (tabs and spaces) at the end of the string
		*
		*  @return
		*    Reference to this string
		*/
		inline String &TrimTrailing();

		/**
		*  @brief
		*    Removes all whitespace (tabs and spaces) at the beginning and the end of the string
		*
		*  @return
		*    Reference to this string
		*/
		PLCORE_API String &Trim();

		/**
		*  @brief
		*    Removes line endings ("\r" or "\n") at the end of the string
		*
		*  @return
		*    Reference to this string
		*
		*  @note
		*    - Carriage return = CR = '\r' = 0x0D = 13 in decimal
		*    - Line feed = LF = '\n' = 0x0A = 10 in decimal
		*/
		inline String &RemoveLineEndings();


	//[-------------------------------------------------------]
	//[ Conversion functions                                  ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the character string as UTF8
		*
		*  @return
		*    Pointer to the character string as UTF8, never a null pointer - do NOT destroy the returned memory, it's owned by this string instance!
		*
		*  @see
		*    - GetASCII()
		*/
		inline const char *GetUTF8() const;

		// Is valid tests
		PLCORE_API bool IsValidInteger() const;
		PLCORE_API bool IsValidFloat() const;

		// Convert from string
		PLCORE_API bool     GetBool()     const;
		PLCORE_API char     GetChar()     const;
		PLCORE_API wchar_t  GetWideChar() const;
		PLCORE_API int      GetInt()      const;
		PLCORE_API int64    GetInt64()    const;
		PLCORE_API uint8    GetUInt8()    const;
		PLCORE_API uint16   GetUInt16()   const;
		PLCORE_API uint32   GetUInt32()   const;
		PLCORE_API uint64   GetUInt64()   const;
		inline     uint_ptr GetUIntPtr()  const;
		PLCORE_API float    GetFloat()    const;
		PLCORE_API double   GetDouble()   const;

		// Convert to string
		inline     String &operator =(bool     bValue);
		PLCORE_API String &operator =(char     nValue);
		PLCORE_API String &operator =(wchar_t  nValue);
		PLCORE_API String &operator =(int      nValue);
		PLCORE_API String &operator =(int64    nValue);
		PLCORE_API String &operator =(uint8    nValue);
	//	PLCORE_API String &operator =(uint16   nValue);	// We can't do that because wchar_t may be defined the same way as uint16
		PLCORE_API String &operator =(uint32   nValue);
		PLCORE_API String &operator =(uint64   nValue);
		PLCORE_API String &operator =(float    fValue);
		PLCORE_API String &operator =(double   dValue);

		// Concatenation
		PLCORE_API String operator +(bool    bValue) const;
		PLCORE_API String operator +(char    nValue) const;
		PLCORE_API String operator +(wchar_t nValue) const;
		PLCORE_API String operator +(int     nValue) const;
		inline     String operator +(int64   nValue) const;
		inline     String operator +(uint8   nValue) const;
	//	inline     String operator +(uint16  nValue) const;	// We can't do that because wchar_t may be defined the same way as uint16
		inline     String operator +(uint32  nValue) const;
		inline     String operator +(uint64  nValue) const;
		inline     String operator +(float   fValue) const;
		inline     String operator +(double  dValue) const;
		PLCORE_API friend String operator +(bool     bValue, const String &sString);
		PLCORE_API friend String operator +(char     nValue, const String &sString);
		PLCORE_API friend String operator +(wchar_t  nValue, const String &sString);
		inline     friend String operator +(int      nValue, const String &sString);
		inline     friend String operator +(int64    nValue, const String &sString);
		inline     friend String operator +(uint8    nValue, const String &sString);
	//	inline     friend String operator +(uint16   nValue, const String &sString);	// We can't do that because wchar_t may be defined the same way as uint16
		inline     friend String operator +(uint32   nValue, const String &sString);
		inline     friend String operator +(uint64   nValue, const String &sString);
		inline     friend String operator +(float    fValue, const String &sString);
		inline     friend String operator +(double   dValue, const String &sString);
		PLCORE_API String &operator +=(bool     bValue);
		PLCORE_API String &operator +=(char     nValue);
		PLCORE_API String &operator +=(wchar_t  nValue);
		inline     String &operator +=(int      nValue);
		inline     String &operator +=(int64    nValue);
		inline     String &operator +=(uint8    nValue);
	//	inline     String &operator +=(uint16   nValue);	// We can't do that because wchar_t may be defined the same way as uint16
		inline     String &operator +=(uint32   nValue);
		inline     String &operator +=(uint64   nValue);
		inline     String &operator +=(float    fValue);
		inline     String &operator +=(double   dValue);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Use a new string buffer
		*
		*  @param[in] pStringBuffer
		*    New string buffer to use, can be a null pointer
		*/
		PLCORE_API void SetStringBuffer(StringBuffer *pStringBuffer);

		/**
		*  @brief
		*    Release string buffer
		*/
		void ReleaseStringBuffer();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		StringBuffer *m_pStringBuffer;	/**< Pointer to the string buffer, if a null pointer, string is empty */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/String/String.inl"


#endif // __PLCORE_STRING_H__
