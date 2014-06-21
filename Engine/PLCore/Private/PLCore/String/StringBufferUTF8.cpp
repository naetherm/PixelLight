/*********************************************************\
 *  File: StringBufferUTF8.cpp                           *
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
#include "PLCore/String/String.h"
#include "PLCore/String/UTF8Tools.h"
#include "PLCore/String/StringBufferUTF8.h"


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
StringBufferUTF8::StringBufferUTF8(char szString[], uint32 nLength) : StringBuffer(nLength, nLength, String::UTF8),
	m_pszString(nullptr),
	m_nNumOfBytes(0)
{
	// Get the number of bytes the UTF8 string requires
	m_nNumOfBytes = UTF8Tools::Unescape(nullptr, 0, szString);

	// Get the UTF8 string
	m_pszString = new char[m_nNumOfBytes + 1];
	UTF8Tools::Unescape(m_pszString, m_nNumOfBytes + 1, szString);
}

/**
*  @brief
*    Constructor
*/
StringBufferUTF8::StringBufferUTF8(const wchar_t szString[], uint32 nLength) : StringBuffer(nLength, nLength, String::UTF8),
	m_pszString(nullptr),
	m_nNumOfBytes(0)
{
	// Get the number of bytes the UTF8 string requires
	m_nNumOfBytes = UTF8Tools::FromWideCharacterString(nullptr, 0, szString, 0);

	// Get the UTF8 string
	m_pszString = new char[m_nNumOfBytes + 1];
	UTF8Tools::FromWideCharacterString(m_pszString, m_nNumOfBytes + 1, szString, 0);
}

/**
*  @brief
*    Destructor
*/
StringBufferUTF8::~StringBufferUTF8()
{
	delete [] m_pszString; // (there MUST by such a string :)
}


//[-------------------------------------------------------]
//[ Public virtual StringBuffer functions                 ]
//[-------------------------------------------------------]
uint32 StringBufferUTF8::GetFormat() const
{
	return String::UTF8;
}

StringBufferASCII *StringBufferUTF8::GetASCII()
{
	// Not implemented
	return nullptr;
}

StringBufferUnicode *StringBufferUTF8::GetUnicode()
{
	// Not implemented
	return nullptr;
}

StringBufferUTF8 *StringBufferUTF8::GetUTF8()
{
	return this;
}

uint32 StringBufferUTF8::GetNumOfBytes() const
{
	return m_nNumOfBytes;
}

StringBuffer *StringBufferUTF8::Clone() const
{
	// Not implemented
	return nullptr;
}

StringBuffer *StringBufferUTF8::Duplicate()
{
	// Not implemented
	return nullptr;
}

bool StringBufferUTF8::IsLessThan(const char szString[], uint32 nLength) const
{
	// Not implemented
	return false;
}

bool StringBufferUTF8::IsLessThan(const wchar_t szString[], uint32 nLength) const
{
	// Not implemented
	return false;
}

bool StringBufferUTF8::IsGreaterThan(const char szString[], uint32 nLength) const
{
	// Not implemented
	return false;
}

bool StringBufferUTF8::IsGreaterThan(const wchar_t szString[], uint32 nLength) const
{
	// Not implemented
	return false;
}

bool StringBufferUTF8::Compare(const char szString[], uint32 nLength, uint32 nPos, uint32 nCount) const
{
	// Not implemented
	return false;
}

bool StringBufferUTF8::Compare(const wchar_t szString[], uint32 nLength, uint32 nPos, uint32 nCount) const
{
	// Not implemented
	return false;
}

bool StringBufferUTF8::CompareNoCase(const char szString[], uint32 nLength, uint32 nPos, uint32 nCount) const
{
	// Not implemented
	return false;
}

bool StringBufferUTF8::CompareNoCase(const wchar_t szString[], uint32 nLength, uint32 nPos, uint32 nCount) const
{
	// Not implemented
	return false;
}

bool StringBufferUTF8::IsAlphabetic() const
{
	// Not implemented
	return false;
}

bool StringBufferUTF8::IsAlphaNumeric() const
{
	// Not implemented
	return false;
}

bool StringBufferUTF8::IsNumeric() const
{
	// Not implemented
	return false;
}

bool StringBufferUTF8::IsSubstring(const char szString[], uint32 nLength) const
{
	// Not implemented
	return false;
}

bool StringBufferUTF8::IsSubstring(const wchar_t szString[], uint32 nLength) const
{
	// Not implemented
	return false;
}

int StringBufferUTF8::IndexOf(const char szString[], uint32 nPos, uint32 nLength) const
{
	// Not implemented
	return -1;
}

int StringBufferUTF8::IndexOf(const wchar_t szString[], uint32 nPos, uint32 nLength) const
{
	// Not implemented
	return -1;
}

int StringBufferUTF8::LastIndexOf(const char szString[], int nPos, uint32 nLength) const
{
	// Not implemented
	return -1;
}

int StringBufferUTF8::LastIndexOf(const wchar_t szString[], int nPos, uint32 nLength) const
{
	// Not implemented
	return -1;
}

StringBuffer *StringBufferUTF8::GetSubstring(uint32 nPos, uint32 nCount) const
{
	// Not implemented
	return nullptr;
}

StringBuffer *StringBufferUTF8::ToLower()
{
	// Not implemented
	return nullptr;
}

StringBuffer *StringBufferUTF8::ToUpper()
{
	// Not implemented
	return nullptr;
}

StringBuffer *StringBufferUTF8::Delete(uint32 nPos, uint32 nCount)
{
	// Not implemented
	return nullptr;
}

StringBuffer *StringBufferUTF8::Append(const char szString[], uint32 nCount)
{
	// Not implemented
	return nullptr;
}

StringBuffer *StringBufferUTF8::Append(const wchar_t szString[], uint32 nCount)
{
	// Not implemented
	return nullptr;
}

StringBuffer *StringBufferUTF8::Insert(const char szString[], uint32 nPos, uint32 nCount)
{
	// Not implemented
	return nullptr;
}

StringBuffer *StringBufferUTF8::Insert(const wchar_t szString[], uint32 nPos, uint32 nCount)
{
	// Not implemented
	return nullptr;
}

StringBuffer *StringBufferUTF8::Replace(char nOld, char nNew, uint32 &nReplaced)
{
	// Not implemented
	return nullptr;
}

StringBuffer *StringBufferUTF8::Replace(wchar_t nOld, wchar_t nNew, uint32 &nReplaced)
{
	// Not implemented
	return nullptr;
}

StringBuffer *StringBufferUTF8::Replace(const char szOld[], uint32 nOldLength, const char szNew[], uint32 nNewLength, uint32 &nReplaced)
{
	// Not implemented
	return nullptr;
}

StringBuffer *StringBufferUTF8::Replace(const wchar_t szOld[], uint32 nOldLength, const wchar_t szNew[], uint32 nNewLength, uint32 &nReplaced)
{
	// Not implemented
	return nullptr;
}

StringBuffer *StringBufferUTF8::SetCharacter(uint32 nIndex, char nCharacter)
{
	// Not implemented
	return nullptr;
}

StringBuffer *StringBufferUTF8::SetCharacter(uint32 nIndex, wchar_t nCharacter)
{
	// Not implemented
	return nullptr;
}

StringBuffer *StringBufferUTF8::TrimLeading()
{
	// Not implemented
	return nullptr;
}

StringBuffer *StringBufferUTF8::TrimTrailing()
{
	// Not implemented
	return nullptr;
}

StringBuffer *StringBufferUTF8::RemoveLineEndings()
{
	// Not implemented
	return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
