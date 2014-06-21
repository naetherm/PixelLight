/*********************************************************\
 *  File: UTF8Tools.h                                    *
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


#ifndef __PLCORE_UTF8TOOLS_H__
#define __PLCORE_UTF8TOOLS_H__
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
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Static class with UTF8 tool functions
*/
class UTF8Tools {


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns whether the given character is an octal digit
		*
		*  @param[in] nCharacter
		*    Character to check
		*
		*  @return
		*    'true' if the given character is an octal digit, else 'false'
		*/
		static inline bool IsOctalDigit(char nCharacter);

		/**
		*  @brief
		*    Returns whether the given character is a hex digit
		*
		*  @param[in] nCharacter
		*    Character to check
		*
		*  @return
		*    'true' if the given character is a hex digit, else 'false'
		*/
		static inline bool IsHexDigit(char nCharacter);

		/**
		*  @brief
		*    Returns whether the given character is the start of an UTF8 sequence or not
		*
		*  @param[in] nCharacter
		*    Character to check
		*
		*  @return
		*    'true' if the given character is the start of a UTF8 sequence, else 'false'
		*/
		static inline bool IsSequenceStart(char nCharacter);

		/**
		*  @brief
		*    Returns the number of bytes a given character requires
		*
		*  @param[in] nCharacter
		*    Character to check
		*
		*  @return
		*    Returns the number of bytes the given character requires
		*/
		static PLCORE_API uint8 GetNumOfCharacterBytes(char nCharacter);

		/**
		*  @brief
		*    Returns the number of bytes a given wide character requires
		*
		*  @param[in] nWideCharacter
		*    Wide character to check
		*
		*  @return
		*    Returns the number of bytes the given wide character requires
		*/
		static PLCORE_API uint8 GetNumOfCharacterBytes(wchar_t nWideCharacter);

		/**
		*  @brief
		*    Returns a character as wide character
		*
		*  @param[in] pnCharacter
		*    Pointer to character to return, if a null pointer '\0' will be returned
		*
		*  @return
		*    The character as wide character
		*/
		static PLCORE_API wchar_t GetWideCharacter(const char *pnCharacter);

		/**
		*  @brief
		*    Returns the next character as wide character
		*
		*  @param[in] ppszString
		*    String where to return the next wide character from, MUST be valid!
		*
		*  @return
		*    The next character as wide character
		*
		*  @note
		*    - The given string pointer is updated
		*/
		static PLCORE_API wchar_t GetNextWideCharacter(const char **ppszString);

		/**
		*  @brief
		*    Moves to the next character
		*
		*  @param[in] ppszString
		*    String where to move to the next character, MUST be valid!
		*
		*  @return
		*    Number of skipped bytes
		*/
		static PLCORE_API uint8 MoveToNextCharacter(const char **ppszString);

		/**
		*  @brief
		*    Moves to the previous character
		*
		*  @param[in] ppszString
		*    String where to move to the previous character, MUST be valid!
		*
		*  @return
		*    Number of skipped bytes
		*/
		static PLCORE_API uint8 MoveToPreviousCharacter(const char **ppszString);

		/**
		*  brief
		*    Character index => byte offset were the character starts within the given UTF8 string
		*
		*  @param[in] pszString
		*    Pointer to string to use, if a null pointer '0' will be returned
		*  @param[in] nCharacterIndex
		*    Character index, MUST be valid!
		*
		*  @return
		*    Byte offset
		*/
		static PLCORE_API uint32 CharacterIndexToByteOffset(const char *pszString, uint32 nCharacterIndex);

		/**
		*  brief
		*    Byte offset => character index were the character starts within the given UTF8 string
		*
		*  @param[in] pszString
		*    Pointer to string to use, if a null pointer '0' will be returned
		*  @param[in] nOffset
		*    Byte offset, MUST be valid!
		*
		*  @return
		*    Character index
		*/
		static PLCORE_API uint32 ByteOffsetToCharacterIndex(const char *pszString, uint32 nOffset);

		/**
		*  @brief
		*    Returns the number of bytes a given string requires
		*
		*  @param[in] pszString
		*    Pointer to string to check, if a null pointer '0' will be returned
		*  @param[in] nCount
		*    Number of string character to take into account, if 0, take all characters into account
		*
		*  @return
		*    Returns the number of bytes the given string requires (excluding the terminating zero)
		*/
		static PLCORE_API uint32 GetNumOfStringBytes(const char *pszString, uint32 nCount = 0);

		/**
		*  @brief
		*    Returns the number of characters within a given string
		*
		*  @param[in] pszString
		*    Pointer to string to check, if a null pointer '0' will be returned
		*
		*  @return
		*    The number of characters within a given string (excluding the terminating zero)
		*/
		static PLCORE_API uint32 GetNumOfCharacters(const char *pszString);

		/**
		*  @brief
		*    Returns the number of characters within a given string
		*
		*  @param[in] pszString
		*    Pointer to string to check, if a null pointer '0' will be returned
		*  @param[in] nNumOfBytes
		*    String size in bytes, MUST be valid!
		*
		*  @return
		*    The number of characters within a given string (excluding the terminating zero)
		*/
		static PLCORE_API uint32 GetNumOfCharacters(const char *pszString, uint32 nNumOfBytes);

		/**
		*  @brief
		*    Returns the number of characters and bytes within a given string
		*
		*  @param[in]  pszString
		*    Pointer to string to check, if a null pointer '0' will be returned
		*  @param[out] nNumOfBytes
		*    Receives the string size in bytes
		*
		*  @return
		*    The number of characters within a given string (excluding the terminating zero)
		*/
		static PLCORE_API uint32 GetNumOfCharactersAndBytes(const char *pszString, uint32 &nNumOfBytes);

		/**
		*  @brief
		*    Returns a pointer to the first occurrence of the given wide character in the given string, or a null pointer if not found
		*
		*  @param[in]  pszString
		*    Pointer to string to use, if a null pointer 'a null pointer' will be returned
		*  @param[in]  nWideCharacter
		*    Wide character to find
		*  @param[out] pnCharacterIndex
		*    Receives the character index of found character returned, if not a null pointer
		*
		*  @return
		*    Pointer to the first occurrence of the given wide character in the given string, or a null pointer if not found
		*/
		static PLCORE_API const char *FindCharacter(const char *pszString, wchar_t nWideCharacter, uint32 *pnCharacterIndex);

		/**
		*  @brief
		*    Same as the above, but searches a buffer of a given size instead of a zero-terminated string
		*
		*  @param[in]  pszString
		*    Pointer to string to use, if a null pointer 'a null pointer' will be returned
		*  @param[in]  nNumOfBytes
		*    String size in bytes, MUST be valid!
		*  @param[in]  nWideCharacter
		*    Wide character to find
		*  @param[out] pnCharacterIndex
		*    Receives the character index of found character returned, if not a null pointer
		*
		*  @return
		*    Pointer to the first occurrence of the given character in the given string, or a null pointer if not found
		*/
		static PLCORE_API const char *FindCharacter(const char *pszString, uint32 nNumOfBytes, wchar_t nWideCharacter, uint32 *pnCharacterIndex);

		/**
		*  @brief
		*    Single wide character character to UTF8
		*
		*  @param[out] pszDestination
		*    Receives the converted character, MUST be large enough to keep the result! If a null pointer '0' will be returned.
		*  @param[in]  nWideCharacter
		*    Wide character to convert
		*
		*  @return
		*    Returns the number of bytes of the converted wide character
		*
		*  @note
		*    - Use GetNumOfCharacterBytes() to get the number of bytes a wide character requires
		*/
		static PLCORE_API uint8 FromWideCharacter(char *pszDestination, wchar_t nWideCharacter);

		/**
		*  @brief
		*    Wide character string to UTF8
		*
		*  @param[out] pszDestination
		*    Receives the converted string, MUST be large enough to hold the result or a null pointer!
		*  @param[in]  nNumOfBytes
		*    Length in bytes of the destination buffer, MUST be valid if 'pszDestination' is not a null pointer!
		*    (+1 if a terminating zero should be added)
		*  @param[in]  pszSource
		*    Pointer to wide character string to convert, if a null pointer '0' will be returned and 'destination' not manipulated
		*  @param[in]  nSourceLength
		*    Number of source characters, or '0' if zero-terminated
		*
		*  @return
		*    The number of used destination bytes (excluding the terminating zero)
		*
		*  @note
		*    - If 'pszDestination' is a null pointer, the the resulting length in bytes of the converted string
		*      (excluding the terminating zero) is returned
		*/
		static PLCORE_API uint32 FromWideCharacterString(char *pszDestination, uint32 nNumOfBytes, const wchar_t *pszSource, uint32 nSourceLength);

		/**
		*  @brief
		*    UTF8 to wide character string
		*
		*  @param[out] pszDestination
		*    Receives the converted string, MUST be large enough to hold the result! If a null pointer '0' will be returned.
		*  @param[in]  nLength
		*    Length of the destination buffer, MUST be valid! (including the terminating zero)
		*  @param[in]  pszSource
		*    Pointer to wide character string to convert, if a null pointer '0' will be returned and 'destination' not manipulated
		*  @param[in]  nSourceNumOfBytes
		*    Number of source bytes to process, or '0' if zero-terminated
		*
		*  @return
		*    Number of converted characters
		*
		*  @note
		*    - Only works for valid UTF8, i.e. no 5- or 6-byte sequences
		*/
		static PLCORE_API uint32 ToWideCharacterString(wchar_t *pszDestination, uint32 nLength, const char *pszSource, uint32 nSourceNumOfBytes);

		/**
		*  @brief
		*    Given a wide character, convert it to an ASCII escape sequence stored in the given destination buffer
		*
		*  @param[out] pszDestination
		*    Will received the converted result, MUST be large enough to hold the result or a null pointer!
		*  @param[in]  nNumOfBytes
		*    Length in bytes of the destination buffer, MUST be valid if 'pszDestination' is not a null pointer!
		*  @param[in]  nWideCharacter
		*    Wide character to convert
		*
		*  @return
		*    Number of characters within the destination buffer (excluding the terminating zero)
		*
		*  @note
		*    - If 'pszDestination' is a null pointer, the the resulting length in bytes of the converted string
		*      (excluding the terminating zero) is returned
		*/
		static PLCORE_API uint32 EscapeWideCharacter(char *pszDestination, uint32 nNumOfBytes, wchar_t nWideCharacter);

		/**
		*  @brief
		*    Converts an UTF8 string to an ASCII string with escape sequences
		*
		*  @param[out] pszDestination
		*    Will received the converted result, MUST be large enough to hold the result or a null pointer!
		*  @param[in]  nNumOfBytes
		*    Length in bytes of the destination buffer, MUST be valid if 'pszDestination' is not a null pointer!
		*  @param[in]  pszSource
		*    Pointer to source to read from, if a null pointer '0' will be returned and 'destination' not manipulated
		*  @param[in]  bEscapeQuotes
		*    If 'true', quote characters will be preceded by backslashes as well
		*
		*  @return
		*    Number of characters within the destination buffer (excluding the terminating zero)
		*
		*  @note
		*    - If 'pszDestination' is a null pointer, the the resulting length in bytes of the converted string
		*      (excluding the terminating zero) is returned
		*/
		static PLCORE_API uint32 Escape(char *pszDestination, uint32 nNumOfBytes, const char *pszSource, bool bEscapeQuotes);

		/**
		*  @brief
		*    Assuming 'pszSource' points to the character after a backslash, read an escape sequence,
		*    storing the result in 'nDestination' and returning the number of input characters processed
		*
		*  @param[out] nDestination
		*    Will receive the result
		*  @param[in]  pszSource
		*    Pointer to source to read from, if a null pointer '0' will be returned and 'destination' not manipulated
		*
		*  @return
		*    Number of read characters (excluding the terminating zero)
		*/
		static PLCORE_API uint32 ReadEscapeSequence(wchar_t &nDestination, const char *pszSource);

		/**
		*  @brief
		*    Converts a string with literal \uxxxx or \Uxxxxxxxx characters to UTF8
		*
		*  @param[out] pszDestination
		*    Will received the converted result, MUST be large enough to hold the result or a null pointer!
		*  @param[in]  nNumOfBytes
		*    Length in bytes of the destination buffer, MUST be valid if 'pszDestination' is not a null pointer!
		*    (+1 if a terminating zero should be added)
		*  @param[in]  pszSource
		*    Pointer to source string to convert, if a null pointer '0' will be returned and 'destination' not manipulated
		*
		*  @return
		*    The resulting length in bytes of the converted string (excluding the terminating zero)
		*
		*  @note
		*    - If 'pszDestination' is a null pointer, the the resulting length in bytes of the converted string
		*      (excluding the terminating zero) is returned
		*/
		static PLCORE_API uint32 Unescape(char *pszDestination, uint32 nNumOfBytes, const char *pszSource);

		/**
		*  @brief
		*    Compares two UTF8 strings (case sensitive)
		*
		*  @param[in] pszFirst
		*    Pointer to first string for comparison, if a null pointer '0' will be returned
		*  @param[in] pszSecond
		*    Pointer to second string for comparison, if a null pointer '0' will be returned
		*  @param[in] nCount
		*    Amount of character to compare, can be 0 if the whole strings should be compared
		*
		*  @return
		*    -1 if the first string is smaller then the second one
		*     0 if both strings are equal
		*     1 if the first string is greater then the second one
		*/
		static PLCORE_API int Compare(const char *pszFirst, const char *pszSecond, uint32 nCount = 0);

		/**
		*  @brief
		*    Find a substring in a UTF8 string
		*
		*  @param[in] pszSource
		*    Pointer to the string which should be searched through, if a null pointer 'a null pointer' will be returned
		*  @param[in] pszSubstring
		*    Pointer to the substring to be searched for, if a null pointer 'a null pointer' will be returned
		*
		*  @return
		*    A pointer to the start position of the substring or a null pointer if the search fails
		*/
		static PLCORE_API const char *FindSubstring(const char *pszSource, const char *pszSubstring);

		/**
		*  @brief
		*    Copies the given UTF8 source string into the destination string
		*
		*  @param[out] pszDestination
		*    Will receive the copied string, if a null pointer 'a null pointer' will be returned
		*  @param[in]  pszSource
		*    Pointer to the source string, if a null pointer 'a null pointer' will be returned
		*  @param[in]  nCount
		*    Amount of character to copy, can be 0 if the whole strings should be copied
		*
		*  @return
		*    A pointer to 'szDestination' (always valid!)
		*
		*  @note
		*    - If 'nCount' is 0 or the source string is < 'nCount' then the '\0'-terminator is also copied
		*/
		static PLCORE_API char *Copy(char *pszDestination, const char *pszSource, uint32 nCount = 0);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/String/UTF8Tools.inl"


#endif // __PLCORE_UTF8TOOLS_H__
