/*********************************************************\
 *  File: String.inl                                     *
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
#include "PLCore/String/StringBuffer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
inline String::String() :
	m_pStringBuffer(nullptr)
{
}

/**
*  @brief
*    Number constructor
*/
inline String::String(bool bValue) :
	m_pStringBuffer(nullptr)
{
	*this = bValue;
}

inline String::String(int64 nValue) :
	m_pStringBuffer(nullptr)
{
	*this = nValue;
}

inline String::String(uint8 nValue) :
	m_pStringBuffer(nullptr)
{
	*this = nValue;
}

inline String::String(uint32 nValue) :
	m_pStringBuffer(nullptr)
{
	*this = nValue;
}

inline String::String(uint64 nValue) :
	m_pStringBuffer(nullptr)
{
	*this = nValue;
}

inline String::String(float fValue) :
	m_pStringBuffer(nullptr)
{
	*this = fValue;
}

inline String::String(double fValue) :
	m_pStringBuffer(nullptr)
{
	*this = fValue;
}

/**
*  @brief
*    Pointer constructor
*/
inline String::String(void *pValue) :
	m_pStringBuffer(nullptr)
{
	*this = reinterpret_cast<uint_ptr>(pValue);
}

/**
*  @brief
*    Destructor
*/
inline String::~String()
{
	// Release string buffer
	if (m_pStringBuffer)
		StringBuffer::Manager.ReleaseStringBuffer(*m_pStringBuffer);
}

/**
*  @brief
*    Returns the length of the string
*/
inline uint32 String::GetLength() const
{
	return m_pStringBuffer ? m_pStringBuffer->GetLength() : 0;
}

/**
*  @brief
*    Returns the internal string format
*/
inline String::EFormat String::GetFormat() const
{
	return m_pStringBuffer ? static_cast<String::EFormat>(m_pStringBuffer->GetFormat()) : ASCII;
}

/**
*  @brief
*    Get a ASCII character of the string
*/
inline char String::operator [](uint32 nIndex) const
{
	// Terminating \0 by default...
	return (nIndex < GetLength()) ? GetASCII()[nIndex] : '\0';
}

/**
*  @brief
*    Returns the character string as ASCII
*/
inline const char *String::GetASCII() const
{
	return m_pStringBuffer ? m_pStringBuffer->GetASCII()->m_pszString : "";
}

/**
*  @brief
*    Returns the ASCII character string
*/
inline String::operator const char *() const
{
	return m_pStringBuffer ? m_pStringBuffer->GetASCII()->m_pszString : "";
}

/**
*  @brief
*    Returns the character string as Unicode
*/
inline const wchar_t *String::GetUnicode() const
{
	return m_pStringBuffer ? m_pStringBuffer->GetUnicode()->m_pszString : L"";
}

/**
*  @brief
*    Returns the Unicode character string
*/
inline String::operator const wchar_t *() const
{
	return m_pStringBuffer ? m_pStringBuffer->GetUnicode()->m_pszString : L"";
}

/**
*  @brief
*    Copy operator
*/
inline String &String::operator =(const String &sString)
{
	// Assign new string buffer (all security check are done within this function... :)
	SetStringBuffer(sString.m_pStringBuffer);

	// Return a reference to this instance
	return *this;
}

/**
*  @brief
*    Compare operator (case sensitive)
*/
inline bool String::operator ==(const String &sString) const
{
	return Compare(sString);
}

/**
*  @brief
*    Compare operator (case sensitive)
*/
inline bool String::operator ==(const char *pszString) const
{
	return Compare(pszString);
}

inline bool String::operator ==(const wchar_t *pszString) const
{
	return Compare(pszString);
}

/**
*  @brief
*    Compare operator (case sensitive)
*/
inline bool String::operator !=(const String &sString) const
{
	return !Compare(sString);
}

/**
*  @brief
*    Compare operator (case sensitive)
*/
inline bool String::operator !=(const char *pszString) const
{
	return !Compare(pszString);
}

inline bool String::operator !=(const wchar_t *pszString) const
{
	return !Compare(pszString);
}

/**
*  @brief
*    Determines whether the string is alphabetic or not
*/
inline bool String::IsAlphabetic() const
{
	// No string, no alphabetic
	return (m_pStringBuffer && m_pStringBuffer->IsAlphabetic());
}

/**
*  @brief
*    Determines whether the string is alpha-numeric or not
*/
inline bool String::IsAlphaNumeric() const
{
	// No string, no alpha-numeric
	return (m_pStringBuffer && m_pStringBuffer->IsAlphaNumeric());
}

/**
*  @brief
*    Determines whether the string is numeric or not
*/
inline bool String::IsNumeric() const
{
	// No string, no numeric
	return (m_pStringBuffer && m_pStringBuffer->IsNumeric());
}

/**
*  @brief
*    Change all characters to lower case
*/
inline String &String::ToLower()
{
	// Is there a string buffer?
	if (m_pStringBuffer) {
		// Change all characters to lower case, string 'may' be changed
		SetStringBuffer(m_pStringBuffer->ToLower());
	}

	// Done
	return *this;
}

/**
*  @brief
*    Change all characters to upper case
*/
inline String &String::ToUpper()
{
	// Is there a string buffer?
	if (m_pStringBuffer) {
		// Change all characters to upper case, string 'may' be changed
		SetStringBuffer(m_pStringBuffer->ToUpper());
	}

	// Done
	return *this;
}

/**
*  @brief
*    Insert a character at a given location
*/
inline String &String::Insert(char nCharacter, uint32 nPos)
{
	// Append or insert?
	if (nPos == GetLength()) {
		// Append -> Fast!
		*this += nCharacter;

		// Done
		return *this;
	} else {
		// Insert -> Slow!
		return Insert(&nCharacter, nPos, 1);
	}
}

inline String &String::Insert(wchar_t nCharacter, uint32 nPos)
{
	// Append or insert?
	if (nPos == GetLength()) {
		// Append -> Fast!
		*this += nCharacter;

		// Done
		return *this;
	} else {
		// Insert -> Slow!
		return Insert(&nCharacter, nPos, 1);
	}
}

/**
*  @brief
*    Removes all whitespace (tabs and spaces) at the beginning of the string
*/
inline String &String::TrimLeading()
{
	// Is this string empty?
	if (m_pStringBuffer) {
		// Delete whitespace at the beginning of the string
		SetStringBuffer(m_pStringBuffer->TrimLeading());
	}

	// Done
	return *this;
}

/**
*  @brief
*    Removes all whitespace (tabs and spaces) at the end of the string
*/
inline String &String::TrimTrailing()
{
	// Is this string empty?
	if (m_pStringBuffer) {
		// Delete whitespace at the end of the string
		SetStringBuffer(m_pStringBuffer->TrimTrailing());
	}

	// Done
	return *this;
}

/**
*  @brief
*    Removes line endings ("\r" or "\n") at the end of the string
*/
inline String &String::RemoveLineEndings()
{
	// Is this string empty?
	if (m_pStringBuffer) {
		// Remove line endings at the end of the string
		SetStringBuffer(m_pStringBuffer->RemoveLineEndings());
	}

	// Done
	return *this;
}


//[-------------------------------------------------------]
//[ Conversion functions                                  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the character string as UTF8
*/
inline const char *String::GetUTF8() const
{
	return m_pStringBuffer ? m_pStringBuffer->GetUTF8()->m_pszString : "";
}

inline uint_ptr String::GetUIntPtr() const
{
	#ifdef X64_ARCHITECTURE
		return GetUInt64();
	#else
		return GetUInt32();
	#endif
}

// Convert to string
inline String &String::operator =(bool bValue)
{
	// Set data
	*this = bValue ? '1' : '0';

	// Return a reference to this instance
	return *this;
}

inline String &String::operator =(int64 nValue)
{
	// Set data
	#ifdef WIN32
		*this = Format("%I64", nValue);
	#else
		// [TODO] Is this working correctly on Linux or do we need something like
		/*
		#include <inttypes.h>
		int64_t var;
		sprintf (buf, "%" PRId64, var);
		*/
		*this = Format("%ll", nValue);
	#endif

	// Return a reference to this instance
	return *this;
}

inline String &String::operator =(uint8 nValue)
{
	// Set data
	#ifdef WIN32
		*this = Format("%I32u", static_cast<uint32>(nValue));
	#else
		// [TODO] Is this working correctly on Linux?
		*this = Format("%u", static_cast<uint32>(nValue));
	#endif

	// Return a reference to this instance
	return *this;
}

inline String &String::operator =(uint32 nValue)
{
	// Set data
	#ifdef WIN32
		*this = Format("%I32u", nValue);
	#else
		// [TODO] Is this working correctly on Linux?
		*this = Format("%u", nValue);
	#endif

	// Return a reference to this instance
	return *this;
}

inline String &String::operator =(uint64 nValue)
{
	// Set data
	#ifdef WIN32
		*this = Format("%I64u", nValue);
	#else
		// [TODO] Is this working correctly on Linux or do we need something like
		/*
		#include <inttypes.h>
		int64_t var;
		sprintf (buf, "%" PRId64, var);
		*/
		*this = Format("%llu", nValue);
	#endif

	// Return a reference to this instance
	return *this;
}

inline String &String::operator =(float fValue)
{
	// Set data
	*this = Format("%g", fValue);

	// Return a reference to this instance
	return *this;
}

inline String &String::operator =(double dValue)
{
	// Set data
	*this = Format("%g", dValue);

	// Return a reference to this instance
	return *this;
}

// Concatenation
inline String String::operator +(int nValue) const
{
	return *this + String(nValue);
}

inline String String::operator +(int64 nValue) const
{
	// Set data
	#ifdef WIN32
		return (GetFormat() == Unicode) ? *this + Format(L"%I64", nValue) : *this + Format("%I64", nValue);
	#else
		// [TODO] Is this working correctly on Linux or do we need something like
		/*
		#include <inttypes.h>
		int64_t var;
		sprintf (buf, "%" PRId64, var);
		*/
		return (GetFormat() == Unicode) ? *this + Format(L"%ll", nValue) : *this + Format("%ll", nValue);
	#endif
}

inline String String::operator +(uint8 nValue) const
{
	#ifdef WIN32
		return (GetFormat() == Unicode) ? *this + Format(L"%I32u", static_cast<uint32>(nValue)) : *this + Format("%I32u", static_cast<uint32>(nValue));
	#else
		// [TODO] Is this working correctly on Linux?
		return (GetFormat() == Unicode) ? *this + Format(L"%u", static_cast<uint32>(nValue)) : *this + Format("%u", static_cast<uint32>(nValue));
	#endif
}

inline String String::operator +(uint32 nValue) const
{
	#ifdef WIN32
		return (GetFormat() == Unicode) ? *this + Format(L"%I32u", nValue) : *this + Format("%I32u", nValue);
	#else
		// [TODO] Is this working correctly on Linux?
		return (GetFormat() == Unicode) ? *this + Format(L"%u", nValue) : *this + Format("%u", nValue);
	#endif
}

inline String String::operator +(uint64 nValue) const
{
	// Set data
	#ifdef WIN32
		return (GetFormat() == Unicode) ? *this + Format(L"%I64u", nValue) : *this + Format("%I64u", nValue);
	#else
		// [TODO] Is this working correctly on Linux or do we need something like
		/*
		#include <inttypes.h>
		int64_t var;
		sprintf (buf, "%" PRId64, var);
		*/
		return (GetFormat() == Unicode) ? *this + Format(L"%llu", nValue) : *this + Format("%llu", nValue);
	#endif
}

inline String String::operator +(float fValue) const
{
	return (GetFormat() == Unicode) ? *this + Format(L"%g", fValue) : *this + Format("%g", fValue);
}

inline String String::operator +(double dValue) const
{
	return (GetFormat() == Unicode) ? *this + String::Format(L"%g", dValue) : *this + String::Format("%g", dValue);
}

inline String operator +(int nValue, const String &sString)
{
	return (sString.GetFormat() == String::Unicode) ? String::Format(L"%d", nValue) + sString : String::Format("%d", nValue) + sString;
}

inline String operator +(int64 nValue, const String &sString)
{
	#ifdef WIN32
		return (sString.GetFormat() == String::Unicode) ? String::Format(L"%I64", nValue) + sString : String::Format("%I64", nValue) + sString;
	#else
		// [TODO] Is this working correctly on Linux or do we need something like
		/*
		#include <inttypes.h>
		int64_t var;
		sprintf (buf, "%" PRId64, var);
		*/
		return (sString.GetFormat() == String::Unicode) ? String::Format(L"%ll", nValue) + sString : String::Format("%ll", nValue) + sString;
	#endif
}

inline String operator +(uint8 nValue, const String &sString)
{
	#ifdef WIN32
		return (sString.GetFormat() == String::Unicode) ? String::Format(L"%I32u", static_cast<uint32>(nValue)) + sString : String::Format("%I32u", static_cast<uint32>(nValue)) + sString;
	#else
		// [TODO] Is this working correctly on Linux?
		return (sString.GetFormat() == String::Unicode) ? String::Format(L"%u", static_cast<uint32>(nValue)) + sString : String::Format("%u", static_cast<uint32>(nValue)) + sString;
	#endif
}

inline String operator +(uint32 nValue, const String &sString)
{
	#ifdef WIN32
		return (sString.GetFormat() == String::Unicode) ? String::Format(L"%I32u", nValue) + sString : String::Format("%I32u", nValue) + sString;
	#else
		// [TODO] Is this working correctly on Linux?
		return (sString.GetFormat() == String::Unicode) ? String::Format(L"%u", nValue) + sString : String::Format("%u", nValue) + sString;
	#endif
}

inline String operator +(uint64 nValue, const String &sString)
{
	#ifdef WIN32
		return (sString.GetFormat() == String::Unicode) ? String::Format(L"%I64u", nValue) + sString : String::Format("%I64u", nValue) + sString;
	#else
		// [TODO] Is this working correctly on Linux or do we need something like
		/*
		#include <inttypes.h>
		int64_t var;
		sprintf (buf, "%" PRId64, var);
		*/
		return (sString.GetFormat() == String::Unicode) ? String::Format(L"%llu", nValue) + sString : String::Format("%llu", nValue) + sString;
	#endif
}

inline String operator +(float fValue, const String &sString)
{
	return (sString.GetFormat() == String::Unicode) ? String::Format(L"%g", fValue) + sString : String::Format("%g", fValue) + sString;
}

inline String operator +(double dValue, const String &sString)
{
	return (sString.GetFormat() == String::Unicode) ? String::Format(L"%g", dValue) + sString : String::Format("%g", dValue) + sString;
}

inline String &String::operator +=(int nValue)
{
	*this += String(nValue);
	return *this;
}

inline String &String::operator +=(int64 nValue)
{
	// Set data
	#ifdef WIN32
		if (GetFormat() == Unicode)
			*this += Format(L"%I64", nValue);
		else
			*this += Format("%I64", nValue);
	#else
		// [TODO] Is this working correctly on Linux or do we need something like
		/*
		#include <inttypes.h>
		int64_t var;
		sprintf (buf, "%" PRId64, var);
		*/
		if (GetFormat() == Unicode)
			*this += Format(L"%ll", nValue);
		else
			*this += Format("%ll", nValue);
	#endif
	return *this;
}

inline String &String::operator +=(uint8 nValue)
{
	#ifdef WIN32
		if (GetFormat() == Unicode)
			*this += Format(L"%I32u", static_cast<uint32>(nValue));
		else
			*this += Format("%I32u", static_cast<uint32>(nValue));
	#else
		// [TODO] Is this working correctly on Linux?
		if (GetFormat() == Unicode)
			*this += Format(L"%u", static_cast<uint32>(nValue));
		else
			*this += Format("%u", static_cast<uint32>(nValue));
	#endif
	return *this;
}

inline String &String::operator +=(uint32 nValue)
{
	#ifdef WIN32
		if (GetFormat() == Unicode)
			*this += Format(L"%I32u", nValue);
		else
			*this += Format("%I32u", nValue);
	#else
		// [TODO] Is this working correctly on Linux?
		if (GetFormat() == Unicode)
			*this += Format(L"%u", nValue);
		else
			*this += Format("%u", nValue);
	#endif
	return *this;
}

inline String &String::operator +=(uint64 nValue)
{
	// Set data
	#ifdef WIN32
		if (GetFormat() == Unicode)
			*this += Format(L"%I64u", nValue);
		else
			*this += Format("%I64u", nValue);
	#else
		// [TODO] Is this working correctly on Linux or do we need something like
		/*
		#include <inttypes.h>
		int64_t var;
		sprintf (buf, "%" PRId64, var);
		*/
		if (GetFormat() == Unicode)
			*this += Format(L"%llu", nValue);
		else
			*this += Format("%llu", nValue);
	#endif
	return *this;
}

inline String &String::operator +=(float fValue)
{
	if (GetFormat() == Unicode)
		*this += Format(L"%g", fValue);
	else
		*this += Format("%g", fValue);
	return *this;
}

inline String &String::operator +=(double dValue)
{
	if (GetFormat() == Unicode)
		*this += Format(L"%g", dValue);
	else
		*this += Format("%g", dValue);
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
