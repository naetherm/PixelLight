/*********************************************************\
 *  File: StringBufferUnicode.cpp                        *
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
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#ifdef LINUX
	#include <wchar.h>
	#include <wctype.h>
	#include "PLCore/PLCoreLinuxWrapper.h"
#endif
#include "PLCore/String/String.h"
#include "PLCore/String/UTF8Tools.h"
#include "PLCore/String/StringBufferASCII.h"
#include "PLCore/String/StringBufferUTF8.h"
#include "PLCore/String/StringBufferUnicode.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
StringBufferUnicode::StringBufferUnicode(wchar_t szString[], uint32 nLength, uint32 nMaxLength) : StringBuffer(nLength, nMaxLength, String::Unicode),
	m_pszString(szString),
	m_pASCII(nullptr),
	m_pUTF8(nullptr)
{
}

/**
*  @brief
*    Constructor
*/
StringBufferUnicode::StringBufferUnicode(const char szString[], uint32 nLength, uint32 nMaxLength) : StringBuffer(nLength, nMaxLength, String::Unicode),
	m_pszString(new wchar_t[nLength + 1]),
	m_pASCII(nullptr),
	m_pUTF8(nullptr)
{
	// We need to convert the given ASCII string into an Unicode one
	mbstowcs(m_pszString, szString, nLength + 1);
}

/**
*  @brief
*    Destructor
*/
StringBufferUnicode::~StringBufferUnicode()
{
	if (m_pASCII)
		Manager.ReleaseStringBuffer(*m_pASCII);
	if (m_pUTF8)
		Manager.ReleaseStringBuffer(*m_pUTF8);
	delete [] m_pszString; // (there MUST by such a string :)
}

/**
*  @brief
*    Sets the string
*/
void StringBufferUnicode::SetString(wchar_t szString[], uint32 nLength)
{
	// The ASCII version is now dirty
	if (m_pASCII) {
		Manager.ReleaseStringBuffer(*m_pASCII);
		m_pASCII = nullptr;
	}

	// The UTF8 version is now dirty
	if (m_pUTF8) {
		Manager.ReleaseStringBuffer(*m_pUTF8);
		m_pUTF8 = nullptr;
	}

	// Kill the old string (there MUST by such a string :)
	delete [] m_pszString;

	// Set new string
	m_pszString  = szString;
	m_nLength    = nLength;
	m_nMaxLength = nLength;
}

/**
*  @brief
*    Sets the new string length
*/
void StringBufferUnicode::SetNewStringLength(uint32 nLength)
{
	// The ASCII version is now dirty
	if (m_pASCII) {
		Manager.ReleaseStringBuffer(*m_pASCII);
		m_pASCII = nullptr;
	}

	// The UTF8 version is now dirty
	if (m_pUTF8) {
		Manager.ReleaseStringBuffer(*m_pUTF8);
		m_pUTF8 = nullptr;
	}

	// Set new string length
	m_nLength = nLength;
}

/**
*  @brief
*    Sets a character
*/
void StringBufferUnicode::SetCharacter(wchar_t nCharacter)
{
	// Optimization: If we have our own unique string buffer instance in the required format we 'just'
	// cut it down to use a single character... this is faster then allocation/deallocation and we ignore
	// the potential 'waste' if memory if the old string buffer was 'larger'

	// We know there MUST be at least one character, so the following is 'safe'
	*m_pszString = nCharacter;
	m_pszString[1] = '\0';

	// Just ignore the 'old stuff' behind the first character (=> NO memory leak!)
	SetNewStringLength(1);
}


//[-------------------------------------------------------]
//[ Public virtual StringBuffer functions                 ]
//[-------------------------------------------------------]
uint32 StringBufferUnicode::GetFormat() const
{
	return String::Unicode;
}

StringBufferASCII *StringBufferUnicode::GetASCII()
{
	// Is there already a current ASCII version?
	if (!m_pASCII) {
		// Nope, let's create one

		// Request an ASCII string buffer from the string buffer manager
		m_pASCII = Manager.GetStringBufferASCII(m_nLength);
		if (m_pASCII) {
			m_pASCII->AddReference();

			// We need to convert the given Unicode string into an ASCII one
			wcstombs(m_pASCII->m_pszString, m_pszString, m_nLength + 1);

			// Set the correct string length
			m_pASCII->m_nLength = m_nLength;
		}
	}

	// Return it
	return m_pASCII;
}

StringBufferUnicode *StringBufferUnicode::GetUnicode()
{
	return this;
}

StringBufferUTF8 *StringBufferUnicode::GetUTF8()
{
	// Is there already a current UTF8 version?
	if (!m_pUTF8) {
		// Nope, let's create one
		m_pUTF8 = new StringBufferUTF8(m_pszString, m_nLength);
		m_pUTF8->AddReference();
	}

	// Return it
	return m_pUTF8;
}

uint32 StringBufferUnicode::GetNumOfBytes() const
{
	// For Unicode: number of bytes = length*sizeof(wchar_t)
	return m_nLength*sizeof(wchar_t);
}

StringBuffer *StringBufferUnicode::Clone() const
{
	// Request an unicode string buffer from the string buffer manager
	StringBuffer *pStringBuffer = Manager.GetStringBufferUnicode(m_nLength);
	if (pStringBuffer)
		pStringBuffer->Append(m_pszString, m_nLength);

	// Done
	return pStringBuffer;
}

StringBuffer *StringBufferUnicode::Duplicate()
{
	// Do we need to clone this string buffer?
	if (GetRefCount() > 1) {
		return Clone();
	} else {
		// The ASCII version is now dirty
		if (m_pASCII) {
			Manager.ReleaseStringBuffer(*m_pASCII);
			m_pASCII = nullptr;
		}

		// The UTF8 version is now dirty
		if (m_pUTF8) {
			Manager.ReleaseStringBuffer(*m_pUTF8);
			m_pUTF8 = nullptr;
		}

		// Just return this string buffer
		return this;
	}
}

bool StringBufferUnicode::IsLessThan(const char szString[], uint32 nLength) const
{
	// We need to convert the given ASCII string into an Unicode one :(
	wchar_t *pUnicode = new wchar_t[nLength + 1];
	mbstowcs(pUnicode, szString, nLength + 1);
	const bool bResult = (wcscmp(m_pszString, pUnicode) < 0);
	delete [] pUnicode;
	return bResult;
}

bool StringBufferUnicode::IsLessThan(const wchar_t szString[], uint32 nLength) const
{
	// "nLength" is not required in this implementation
	return (wcscmp(m_pszString, szString) < 0);
}

bool StringBufferUnicode::IsGreaterThan(const char szString[], uint32 nLength) const
{
	// We need to convert the given ASCII string into an Unicode one :(
	wchar_t *pUnicode = new wchar_t[nLength + 1];
	mbstowcs(pUnicode, szString, nLength + 1);
	const bool bResult = (wcscmp(m_pszString, pUnicode) > 0);
	delete [] pUnicode;
	return bResult;
}

bool StringBufferUnicode::IsGreaterThan(const wchar_t szString[], uint32 nLength) const
{
	// "nLength" is not required in this implementation
	return (wcscmp(m_pszString, szString) > 0);
}

bool StringBufferUnicode::Compare(const char szString[], uint32 nLength, uint32 nPos, uint32 nCount) const
{
	if (!nPos && !nCount) {
		// Compare whole strings
		if (nLength != m_nLength) {
			// THAT was pretty easy, the length of the strings is not equal :)
			return false;
		} else {
			// We need to convert the given ASCII string into an Unicode one :(
			wchar_t *pUnicode = new wchar_t[nLength + 1];
			mbstowcs(pUnicode, szString, nLength + 1);
			const bool bResult = !wcscmp(m_pszString, pUnicode);
			delete [] pUnicode;
			return bResult;
		}

	} else {
		// Check start position
		if (nPos >= m_nLength)
			return false; // Strings are not equal

		// We need to convert the given ASCII string into an Unicode one :(
		wchar_t *pUnicode = new wchar_t[nLength + 1];
		mbstowcs(pUnicode, szString, nLength + 1);

		// Compare strings (count can be > maximum string length)
		bool bResult;
		if (nCount)
			bResult = !wcsncmp(&m_pszString[nPos], pUnicode, nCount);
		else
			bResult = !wcscmp (&m_pszString[nPos], pUnicode);
		delete [] pUnicode;
		return bResult;
	}
}

bool StringBufferUnicode::Compare(const wchar_t szString[], uint32 nLength, uint32 nPos, uint32 nCount) const
{
	if (!nPos && !nCount) {
		// Compare whole strings
		if (nLength != m_nLength)
			return false; // THAT was pretty easy, the length of the strings is not equal :)
		else
			return !wcscmp(m_pszString, szString);
	} else {
		// Check start position
		if (nPos >= m_nLength)
			return false; // Strings are not equal

		// Compare strings (count can be > maximum string length)
		if (nCount)
			return !wcsncmp(&m_pszString[nPos], szString, nCount);
		else
			return !wcscmp (&m_pszString[nPos], szString);
	}
}

bool StringBufferUnicode::CompareNoCase(const char szString[], uint32 nLength, uint32 nPos, uint32 nCount) const
{
	if (!nPos && !nCount) {
		// Compare whole strings
		if (nLength != m_nLength) {
			// THAT was pretty easy, the length of the strings is not equal :)
			return false;
		} else {
			// We need to convert the given ASCII string into an Unicode one :(
			wchar_t *pUnicode = new wchar_t[nLength + 1];
			mbstowcs(pUnicode, szString, nLength + 1);
			const bool bResult = !_wcsicmp(m_pszString, pUnicode);
			delete [] pUnicode;
			return bResult;
		}

	} else {
		// Check start position
		if (nPos >= m_nLength)
			return false; // Strings are not equal

		// We need to convert the given ASCII string into an Unicode one :(
		wchar_t *pUnicode = new wchar_t[nLength + 1];
		mbstowcs(pUnicode, szString, nLength + 1);

		// Compare strings (count can be > maximum string length)
		bool bResult;
		if (nCount)
			bResult = !_wcsnicmp(&m_pszString[nPos], pUnicode, nCount);
		else
			bResult = !_wcsicmp (&m_pszString[nPos], pUnicode);
		delete [] pUnicode;
		return bResult;
	}
}

bool StringBufferUnicode::CompareNoCase(const wchar_t szString[], uint32 nLength, uint32 nPos, uint32 nCount) const
{
	if (!nPos && !nCount) {
		// Compare whole strings
		if (nLength != m_nLength)
			return false; // THAT was pretty easy, the length of the strings is not equal :)
		else
			return !_wcsicmp(m_pszString, szString);
	} else {
		// Check start position
		if (nPos >= m_nLength)
			return false; // Strings are not equal

		// Compare strings (count can be > maximum string length)
		if (nCount)
			return !_wcsnicmp(&m_pszString[nPos], szString, nCount);
		else
			return !_wcsicmp (&m_pszString[nPos], szString);
	}
}

bool StringBufferUnicode::IsAlphabetic() const
{
	const wchar_t *pszString    = m_pszString;
	const wchar_t *pszStringEnd = pszString + m_nLength;
	for (; pszString<pszStringEnd; pszString++) {
		if (!iswalpha(*pszString))
			return false; // The string is not alphabetic
	}

	// The string is alphabetic
	return true;
}

bool StringBufferUnicode::IsAlphaNumeric() const
{
	const wchar_t *pszString    = m_pszString;
	const wchar_t *pszStringEnd = pszString + m_nLength;
	for (; pszString<pszStringEnd; pszString++) {
		if (!iswalpha(*pszString) && !iswdigit(*pszString))
			return false; // The string is not alpha-numeric
	}

	// The string is alpha-numeric
	return true;
}

bool StringBufferUnicode::IsNumeric() const
{
	const wchar_t *pszString    = m_pszString;
	const wchar_t *pszStringEnd = pszString + m_nLength;
	for (; pszString<pszStringEnd; pszString++) {
		if (!iswdigit(*pszString))
			return false; // The string is not numeric
	}

	// The string is numeric
	return true;
}

bool StringBufferUnicode::IsSubstring(const char szString[], uint32 nLength) const
{
	// We need to convert the given ASCII string into an Unicode one :(
	wchar_t *pUnicode = new wchar_t[nLength + 1];
	mbstowcs(pUnicode, szString, nLength + 1);
	const bool bResult = wcsstr(m_pszString, pUnicode) != nullptr;
	delete [] pUnicode;
	return bResult;
}

bool StringBufferUnicode::IsSubstring(const wchar_t szString[], uint32 nLength) const
{
	return (wcsstr(m_pszString, szString) != nullptr);
}

int StringBufferUnicode::IndexOf(const char szString[], uint32 nPos, uint32 nLength) const
{
	// We need to convert the given ASCII string into an Unicode one :(
	wchar_t *pUnicode = new wchar_t[nLength + 1];
	mbstowcs(pUnicode, szString, nLength + 1);
	const wchar_t *pszPos = wcsstr(&m_pszString[nPos], pUnicode);
	delete [] pUnicode;
	return pszPos ? static_cast<int>(pszPos - m_pszString) : -1;
}

int StringBufferUnicode::IndexOf(const wchar_t szString[], uint32 nPos, uint32 nLength) const
{
	const wchar_t *pszPos = wcsstr(&m_pszString[nPos], szString);
	return pszPos ? static_cast<int>(pszPos - m_pszString) : -1;
}

int StringBufferUnicode::LastIndexOf(const char szString[], int nPos, uint32 nLength) const
{
	// We need to convert the given ASCII string into an Unicode one :(
	wchar_t *pUnicode = new wchar_t[nLength + 1];
	mbstowcs(pUnicode, szString, nLength + 1);

	// Find substring
	const wchar_t *pszStringStart = m_pszString;
	const wchar_t *pszStringT     = pszStringStart + nPos;
	for (; pszStringT>=pszStringStart; pszStringT--) {
		if (!wcsncmp(pszStringT, pUnicode, nLength)) {
			delete [] pUnicode;
			return static_cast<int>(pszStringT - pszStringStart); // We have found something
		}
	}
	delete [] pUnicode;

	// No substring
	return -1;
}

int StringBufferUnicode::LastIndexOf(const wchar_t szString[], int nPos, uint32 nLength) const
{
	// Find substring
	const wchar_t *pszStringStart = m_pszString;
	const wchar_t *pszStringT     = pszStringStart + nPos;
	for (; pszStringT>=pszStringStart; pszStringT--) {
		if (!wcsncmp(pszStringT, szString, nLength))
			return static_cast<int>(pszStringT - pszStringStart); // We have found something
	}

	// No substring
	return -1;
}

StringBuffer *StringBufferUnicode::GetSubstring(uint32 nPos, uint32 nCount) const
{
	// Request an unicode string buffer from the string buffer manager
	StringBuffer *pStringBuffer = Manager.GetStringBufferUnicode(nCount);
	if (pStringBuffer)
		pStringBuffer->Append(m_pszString + nPos, nCount);

	// Done
	return pStringBuffer;
}

StringBuffer *StringBufferUnicode::ToLower()
{
	// Look for the first character that is NOT lower case
	const wchar_t *pszString    = m_pszString;
	const wchar_t *pszStringEnd = pszString + m_nLength;
	for (; pszString<pszStringEnd; pszString++) {
		// Is this already a lower character?
		if (!iswlower(*pszString)) {
			// Nope, now we have to clone the string buffer :(
			StringBufferUnicode *pStringBufferUnicodeClone = static_cast<StringBufferUnicode*>(Duplicate());

			// Start with an offset - we KNOW the characters before will not be changed!
			_wcslwr(pStringBufferUnicodeClone->m_pszString + (pszString - m_pszString));

			// Return the new, lower case string buffer
			return pStringBufferUnicodeClone;
		}
	}

	// If we are here, nothing was changed and we can just return this string buffer
	return this;
}

StringBuffer *StringBufferUnicode::ToUpper()
{
	// Look for the first character that is NOT upper case
	const wchar_t *pszString    = m_pszString;
	const wchar_t *pszStringEnd = pszString + m_nLength;
	for (; pszString<pszStringEnd; pszString++) {
		// Is this already a upper character?
		if (!iswupper(*pszString)) {
			// Nope, now we have to clone the string buffer :(
			StringBufferUnicode *pStringBufferUnicodeClone = static_cast<StringBufferUnicode*>(Duplicate());

			// Start with an offset - we KNOW the characters before will not be changed!
			_wcsupr(pStringBufferUnicodeClone->m_pszString + (pszString - m_pszString));

			// Return the new, upper case string buffer
			return pStringBufferUnicodeClone;
		}
	}

	// If we are here, nothing was changed and we can just return this string buffer
	return this;
}

StringBuffer *StringBufferUnicode::Delete(uint32 nPos, uint32 nCount)
{
	// We have to clone the string buffer
	StringBufferUnicode *pStringBufferUnicodeClone = static_cast<StringBufferUnicode*>(Duplicate());

	// Characters are deleted by moving up the data following the region to delete (FAST!)
	const uint32 nNewLength = m_nLength - nCount;
	memcpy(&pStringBufferUnicodeClone->m_pszString[nPos], pStringBufferUnicodeClone->m_pszString + nPos + nCount, (nNewLength - nPos)*sizeof(wchar_t));
	pStringBufferUnicodeClone->m_pszString[nNewLength] = L'\0';

	// Set the new length
	pStringBufferUnicodeClone->SetNewStringLength(nNewLength);

	// Done
	return pStringBufferUnicodeClone;
}

StringBuffer *StringBufferUnicode::Append(const char szString[], uint32 nCount)
{
	// Calculate the new total length of the string (excluding the terminating zero)
	const uint32 nNewLength = m_nLength + nCount;

	// Is it possible to just modify the current internal string in place? (FAST!)
	if (nNewLength <= m_nMaxLength && GetRefCount() < 2) {
		// Just modify the current internal string in place
		mbstowcs(&m_pszString[m_nLength], szString, nCount);	// Append the new string
		m_pszString[nNewLength] = L'\0';						// Set the terminating zero
		SetNewStringLength(nNewLength);							// Set the new string length

		// Return this string buffer
		return this;
	} else {
		// Request an unicode string buffer from the string buffer manager
		StringBuffer *pStringBuffer = Manager.GetStringBufferUnicode(nNewLength);
		if (pStringBuffer) {
			// Add old string
			if (m_nLength)
				pStringBuffer->Append(m_pszString, m_nLength);

			// Add string to append
			pStringBuffer->Append(szString, nCount);
		}

		// Done
		return pStringBuffer;
	}
}

StringBuffer *StringBufferUnicode::Append(const wchar_t szString[], uint32 nCount)
{
	// Calculate the new total length of the string (excluding the terminating zero)
	const uint32 nNewLength = m_nLength + nCount;

	// Is it possible to just modify the current internal string in place? (FAST!)
	if (nNewLength <= m_nMaxLength && GetRefCount() < 2) {
		// Just modify the current internal string in place
		memcpy(&m_pszString[m_nLength], szString, nCount*sizeof(wchar_t));	// Append the new string
		m_pszString[nNewLength] = L'\0';									// Set the terminating zero
		SetNewStringLength(nNewLength);										// Set the new string length

		// Return this string buffer
		return this;
	} else {
		// Request an unicode string buffer from the string buffer manager
		StringBuffer *pStringBuffer = Manager.GetStringBufferUnicode(nNewLength);
		if (pStringBuffer) {
			// Add old string
			if (m_nLength)
				pStringBuffer->Append(m_pszString, m_nLength);

			// Add string to append
			pStringBuffer->Append(szString, nCount);
		}

		// Done
		return pStringBuffer;
	}
}

StringBuffer *StringBufferUnicode::Insert(const char szString[], uint32 nPos, uint32 nCount)
{
	// Calculate the new total length of the string (excluding the terminating zero)
	const uint32 nNewLength = m_nLength + nCount;

	// Is it possible to just modify the current internal string in place? (FAST!)
	if (nNewLength <= m_nMaxLength && GetRefCount() < 2) {
		// Just modify the current internal string in place
		const int nLeftCharacters = m_nLength - nPos;
		if (nLeftCharacters > 0) {
			// Make space for the new string by moving everything to the right
			// (in here, we KNOW that there's enough memory in the right to hold the string!)
			memcpy(&m_pszString[nPos + nCount], &m_pszString[nPos], nLeftCharacters*sizeof(wchar_t));
		}
		mbstowcs(&m_pszString[nPos], szString, nCount);	// Append the new string at the now free space
		m_pszString[nNewLength] = L'\0';				// Set the terminating zero
		SetNewStringLength(nNewLength);					// Set the new string length

		// Return this string buffer
		return this;
	} else {
		// Request an unicode string buffer from the string buffer manager
		StringBuffer *pStringBuffer = Manager.GetStringBufferUnicode(nNewLength);
		if (pStringBuffer) {
			// Add old string in front of the insert place
			if (m_nLength && nPos)
				pStringBuffer->Append(m_pszString, nPos);

			// Add new string
			pStringBuffer->Append(szString, nCount);

			// Add old string in back of the insert place
			const int nLeftCharacters = m_nLength - nPos;
			if (m_nLength && nLeftCharacters > 0)
				pStringBuffer->Append(&m_pszString[nPos], nLeftCharacters);
		}

		// Done
		return pStringBuffer;
	}
}

StringBuffer *StringBufferUnicode::Insert(const wchar_t szString[], uint32 nPos, uint32 nCount)
{
	// Calculate the new total length of the string (excluding the terminating zero)
	const uint32 nNewLength = m_nLength + nCount;

	// Is it possible to just modify the current internal string in place? (FAST!)
	if (nNewLength <= m_nMaxLength && GetRefCount() < 2) {
		// Just modify the current internal string in place
		const int nLeftCharacters = m_nLength - nPos;
		if (nLeftCharacters > 0) {
			// Make space for the new string by moving everything to the right
			// (in here, we KNOW that there's enough memory in the right to hold the string!)
			memcpy(&m_pszString[nPos + nCount], &m_pszString[nPos], nLeftCharacters*sizeof(wchar_t));
		}
		memcpy(&m_pszString[nPos], szString, nCount*sizeof(wchar_t));	// Append the new string at the now free space
		m_pszString[nNewLength] = L'\0';								// Set the terminating zero
		SetNewStringLength(nNewLength);									// Set the new string length

		// Return this string buffer
		return this;
	} else {
		// Request an unicode string buffer from the string buffer manager
		StringBuffer *pStringBuffer = Manager.GetStringBufferUnicode(nNewLength);
		if (pStringBuffer) {
			// Add old string in front of the insert place
			if (m_nLength && nPos)
				pStringBuffer->Append(m_pszString, nPos);

			// Add new string
			pStringBuffer->Append(szString, nCount);

			// Add old string in back of the insert place
			const int nLeftCharacters = m_nLength - nPos;
			if (m_nLength && nLeftCharacters > 0)
				pStringBuffer->Append(&m_pszString[nPos], nLeftCharacters);
		}

		// Done
		return pStringBuffer;
	}
}

StringBuffer *StringBufferUnicode::Replace(char nOld, char nNew, uint32 &nReplaced)
{
	wchar_t *pszString    = m_pszString;
	wchar_t *pszStringEnd = m_pszString + m_nLength;

	// Get new and old as Unicode
	wchar_t nOldUnicode, nNewUnicode;
	if (mbtowc(&nOldUnicode, &nOld, 1) >= 0 && mbtowc(&nNewUnicode, &nNew, 1) >= 0) {
		// Search for the first character which should be replaced
		nReplaced = 0;
		for (; pszString<pszStringEnd; pszString++) {
			if (*pszString == nOldUnicode) {
				// Fork string buffer when the first character has been found
				StringBufferUnicode *pStringBufferUnicodeClone = static_cast<StringBufferUnicode*>(Duplicate());

				// Set pointers to new location
				pszString    = pStringBufferUnicodeClone->m_pszString + (pszString - m_pszString);
				pszStringEnd = pStringBufferUnicodeClone->m_pszString + m_nLength;

				// Replace characters
				for (; pszString<pszStringEnd; pszString++) {
					if (*pszString == nOldUnicode) {
						*pszString = nNewUnicode;
						nReplaced++;
					}
				}

				// Return the new string buffer
				return pStringBufferUnicodeClone;
			}
		}
	}

	// If we are here, nothing was changed and we can just return this string buffer
	return this;
}

StringBuffer *StringBufferUnicode::Replace(wchar_t nOld, wchar_t nNew, uint32 &nReplaced)
{
	wchar_t *pszString    = m_pszString;
	wchar_t *pszStringEnd = m_pszString + m_nLength;

	// Search for the first character which should be replaced
	nReplaced = 0;
	for (; pszString<pszStringEnd; pszString++) {
		if (*pszString == nOld) {
			// Fork string buffer when the first character has been found
			StringBufferUnicode *pStringBufferUnicodeClone = static_cast<StringBufferUnicode*>(Duplicate());

			// Set pointers to new location
			pszString    = pStringBufferUnicodeClone->m_pszString + (pszString - m_pszString);
			pszStringEnd = pStringBufferUnicodeClone->m_pszString + m_nLength;

			// Replace characters
			for (; pszString<pszStringEnd; pszString++) {
				if (*pszString == nOld) {
					*pszString = nNew;
					nReplaced++;
				}
			}

			// Return the new string buffer
			return pStringBufferUnicodeClone;
		}
	}

	// If we are here, nothing was changed and we can just return this string buffer
	return this;
}

StringBuffer *StringBufferUnicode::Replace(const char szOld[], uint32 nOldLength, const char szNew[], uint32 nNewLength, uint32 &nReplaced)
{
	// [TODO] Use the string buffer manager

	// Check for only character replace
	if (nOldLength == 1 && nNewLength == 1)
		return Replace(szOld[0], szNew[0], nReplaced);

	// We need to convert the given ASCII strings into an Unicode ones :(
	wchar_t *pszOldUnicode = new wchar_t[nOldLength + 1];
	mbstowcs(pszOldUnicode, szOld, nOldLength + 1);
	wchar_t *pszNewUnicode = new wchar_t[nNewLength + 1];
	mbstowcs(pszNewUnicode, szNew, nNewLength + 1);

	// First, get the number of substrings to replace
	nReplaced = 0;
	const wchar_t *pszString = m_pszString;
	const wchar_t *pszEnd    = m_pszString + m_nLength;
	while (pszString<pszEnd) {
		if (!wcsncmp(pszString, pszOldUnicode, nOldLength)) {
			nReplaced++;
			pszString += nOldLength;
		} else {
			pszString++;
		}
	}

	// Jipi, no substrings, no further work to do :)
	if (!nReplaced) {
		delete [] pszOldUnicode;
		delete [] pszNewUnicode;
		return this; // Nothing was changed and we can just return this string buffer
	}

	// Get and check the new length
	const uint32 nFinalLength = m_nLength + (nNewLength - nOldLength)*nReplaced;
	if (!nFinalLength) {
		delete [] pszOldUnicode;
		delete [] pszNewUnicode;
		return nullptr; // The string is now empty!
	}

	// Create the new string
	wchar_t *pszNewString  = new wchar_t[nFinalLength + 1];
	wchar_t *pszNewStringT = pszNewString;
	uint32 nSkipped = 0;
	pszString = m_pszString;
	for (uint32 i=0; i<nReplaced; i++) {
		// Find substring
		while (wcsncmp(pszString, pszOldUnicode, nOldLength)) {
			pszString++;
			nSkipped++;
		}

		// Copy previous none substring characters
		if (nSkipped) {
			memcpy(pszNewStringT, pszString - nSkipped, nSkipped*sizeof(wchar_t));
			pszNewStringT += nSkipped;
		}

		// Insert new substring
		memcpy(pszNewStringT, pszNewUnicode, nNewLength*sizeof(wchar_t));
		pszNewStringT += nNewLength;
		pszString += nOldLength;
		nSkipped = 0;
	}

	// Copy the rest of the old string to the new string
	if (*pszString != L'\0') {
		const uint32 i = static_cast<uint32>(pszNewString + nFinalLength - pszNewStringT);
		memcpy(pszNewStringT, pszString, i*sizeof(wchar_t));
		pszNewStringT += i;
	}

	// Add terminating zero
	*pszNewStringT = L'\0';

	// Cleanup
	delete [] pszOldUnicode;
	delete [] pszNewUnicode;

	// Return the new string buffer
	if (GetRefCount() > 1) {
		return new StringBufferUnicode(pszNewString, nFinalLength, nFinalLength);
	} else {
		// Just modify and return this string buffer
		SetString(pszNewString, nFinalLength);
		return this;
	}
}

StringBuffer *StringBufferUnicode::Replace(const wchar_t szOld[], uint32 nOldLength, const wchar_t szNew[], uint32 nNewLength, uint32 &nReplaced)
{
	// [TODO] Use the string buffer manager

	// Check for only character replace
	if (nOldLength == 1 && nNewLength == 1)
		return Replace(szOld[0], szNew[0], nReplaced);

	// First, get the number of substrings to replace
	nReplaced = 0;
	const wchar_t *pszString = m_pszString;
	const wchar_t *pszEnd    = m_pszString + m_nLength;
	while (pszString<pszEnd) {
		if (!wcsncmp(pszString, szOld, nOldLength)) {
			nReplaced++;
			pszString += nOldLength;
		} else {
			pszString++;
		}
	}

	// Jipi, no substrings, no further work to do :)
	if (!nReplaced)
		return this; // Nothing was changed and we can just return this string buffer

	// Get and check the new length
	const uint32 nFinalLength = m_nLength + (nNewLength - nOldLength)*nReplaced;
	if (!nFinalLength)
		return nullptr; // The string is now empty!

	// Create the new string
	wchar_t *pszNewString  = new wchar_t[nFinalLength + 1];
	wchar_t *pszNewStringT = pszNewString;
	uint32 nSkipped = 0;
	pszString = m_pszString;
	for (uint32 i=0; i<nReplaced; i++) {
		// Find substring
		while (wcsncmp(pszString, szOld, nOldLength)) {
			pszString++;
			nSkipped++;
		}

		// Copy previous none substring characters
		if (nSkipped) {
			memcpy(pszNewStringT, pszString - nSkipped, nSkipped*sizeof(wchar_t));
			pszNewStringT += nSkipped;
		}

		// Insert new substring
		memcpy(pszNewStringT, szNew, nNewLength*sizeof(wchar_t));
		pszNewStringT += nNewLength;
		pszString += nOldLength;
		nSkipped = 0;
	}

	// Copy the rest of the old string to the new string
	if (*pszString != L'\0') {
		const uint32 i = static_cast<uint32>(pszNewString + nFinalLength - pszNewStringT);
		memcpy(pszNewStringT, pszString, i*sizeof(wchar_t));
		pszNewStringT += i;
	}

	// Add terminating zero
	*pszNewStringT = L'\0';

	// Return the new string buffer
	if (GetRefCount() > 1) {
		return new StringBufferUnicode(pszNewString, nFinalLength, nFinalLength);
	} else {
		// Just modify and return this string buffer
		SetString(pszNewString, nFinalLength);
		return this;
	}
}

StringBuffer *StringBufferUnicode::SetCharacter(uint32 nIndex, char nCharacter)
{
	// Get this character as Unicode and check whether or not we need to clone this string buffer
	wchar_t nCharacterUnicode;
	if (mbtowc(&nCharacterUnicode, &nCharacter, 1) >= 0 && m_pszString[nIndex] != nCharacterUnicode) {
		// We have to clone the string buffer :(
		StringBufferUnicode *pStringBufferUnicodeClone = static_cast<StringBufferUnicode*>(Duplicate());

		// Set the new character
		pStringBufferUnicodeClone->m_pszString[nIndex] = nCharacterUnicode;

		// Return the new string buffer
		return pStringBufferUnicodeClone;
	}

	// Nothing was changed
	return this;
}

StringBuffer *StringBufferUnicode::SetCharacter(uint32 nIndex, wchar_t nCharacter)
{
	// Do we need to clone this string buffer?
	if (m_pszString[nIndex] == nCharacter) {
		// Nothing was changed
		return this;
	} else {
		// We have to clone the string buffer :(
		StringBufferUnicode *pStringBufferUnicodeClone = static_cast<StringBufferUnicode*>(Duplicate());

		// Set the new character
		pStringBufferUnicodeClone->m_pszString[nIndex] = nCharacter;

		// Return the new string buffer
		return pStringBufferUnicodeClone;
	}
}

StringBuffer *StringBufferUnicode::TrimLeading()
{
	const wchar_t *pszString = m_pszString;
	while (*pszString == L' ' || *pszString == L'\t')
		pszString++;
	if (pszString == m_pszString) {
		// Nothing to change
		return this;
	} else {
		// Get the number of characters to delete
		const uint32 nCount = static_cast<uint32>(pszString - m_pszString);

		// Is the string now empty?
		if (nCount == m_nLength)
			return nullptr;				// The string is now empty
		else
			return Delete(0, nCount);	// Return the new string buffer
	}
}

StringBuffer *StringBufferUnicode::TrimTrailing()
{
	const wchar_t *pszEnd    = m_pszString + m_nLength - 1;
	const wchar_t *pszString = pszEnd;
	while (*pszString == L' ' || *pszString == L'\t')
		pszString--;
	if (pszString == pszEnd) {
		// Nothing to change
		return this;
	} else {
		// Get the number of characters to delete
		const uint32 nCount = static_cast<uint32>(pszEnd - pszString);

		// Is the string now empty?
		if (nCount == m_nLength)
			return nullptr;																// The string is now empty
		else
			return Delete(static_cast<uint32>(pszString - m_pszString) + 1, nCount);	// Return the new string buffer
	}
}

StringBuffer *StringBufferUnicode::RemoveLineEndings()
{
	const wchar_t *pszEnd    = m_pszString + m_nLength - 1;
	const wchar_t *pszString = pszEnd;
	while (*pszString == L'\r' || *pszString == L'\n')
		pszString--;
	if (pszString == pszEnd) {
		// Nothing to change
		return this;
	} else {
		// Get the number of characters to delete
		const uint32 nCount = static_cast<uint32>(pszEnd - pszString);

		// Is the string now empty?
		if (nCount == m_nLength)
			return nullptr;																// The string is now empty
		else
			return Delete(static_cast<uint32>(pszString - m_pszString) + 1, nCount);	// Return the new string buffer
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
