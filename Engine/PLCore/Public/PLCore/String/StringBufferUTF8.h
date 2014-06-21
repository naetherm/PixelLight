/*********************************************************\
 *  File: StringBufferUTF8.h                             *
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


#ifndef __PLCORE_STRINGBUFFER_UTF8_H__
#define __PLCORE_STRINGBUFFER_UTF8_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/StringBuffer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Class that contains the buffer for a UTF8 string
*
*  @note
*    - Unlike StringBufferASCII and StringBufferUnicode, the sole purpose of this implementation is to keep a cached UTF8 version of a string
*/
class StringBufferUTF8 : public StringBuffer {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class String;
	friend class StringBufferManager;
	friend class StringBufferASCII;
	friend class StringBufferUnicode;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] szString
		*    The string (converted to UTF8)
		*  @param[in] nLength
		*    Length of the string buffer (excluding the terminating zero)
		*/
		StringBufferUTF8(char szString[], uint32 nLength);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] szString
		*    The string (converted to UTF8)
		*  @param[in] nLength
		*    Length of the string buffer (excluding the terminating zero)
		*/
		StringBufferUTF8(const wchar_t szString[], uint32 nLength);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~StringBufferUTF8();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		char   *m_pszString;	/**< The string itself (NEVER a null pointer!) */
		uint32  m_nNumOfBytes;	/**< Number of bytes of the string */


	//[-------------------------------------------------------]
	//[ Public virtual StringBuffer functions                 ]
	//[-------------------------------------------------------]
	public:
		virtual uint32 GetFormat() const override;
		virtual StringBufferASCII *GetASCII() override;
		virtual StringBufferUnicode *GetUnicode() override;
		virtual StringBufferUTF8 *GetUTF8() override;
		virtual uint32 GetNumOfBytes() const override;
		virtual StringBuffer *Clone() const override;
		virtual StringBuffer *Duplicate() override;
		virtual bool IsLessThan(const char szString[], uint32 nLength) const override;
		virtual bool IsLessThan(const wchar_t szString[], uint32 nLength) const override;
		virtual bool IsGreaterThan(const char szString[], uint32 nLength) const override;
		virtual bool IsGreaterThan(const wchar_t szString[], uint32 nLength) const override;
		virtual bool Compare(const char szString[], uint32 nLength, uint32 nPos, uint32 nCount) const override;
		virtual bool Compare(const wchar_t szString[], uint32 nLength, uint32 nPos, uint32 nCount) const override;
		virtual bool CompareNoCase(const char szString[], uint32 nLength, uint32 nPos, uint32 nCount) const override;
		virtual bool CompareNoCase(const wchar_t szString[], uint32 nLength, uint32 nPos, uint32 nCount) const override;
		virtual bool IsAlphabetic() const override;
		virtual bool IsAlphaNumeric() const override;
		virtual bool IsNumeric() const override;
		virtual bool IsSubstring(const char szString[], uint32 nLength) const override;
		virtual bool IsSubstring(const wchar_t szString[], uint32 nLength) const override;
		virtual int IndexOf(const char szString[], uint32 nPos, uint32 nLength) const override;
		virtual int IndexOf(const wchar_t szString[], uint32 nPos, uint32 nLength) const override;
		virtual int LastIndexOf(const char szString[], int nPos, uint32 nLength) const override;
		virtual int LastIndexOf(const wchar_t szString[], int nPos, uint32 nLength) const override;
		virtual StringBuffer *GetSubstring(uint32 nPos, uint32 nCount) const override;
		virtual StringBuffer *ToLower() override;
		virtual StringBuffer *ToUpper() override;
		virtual StringBuffer *Delete(uint32 nPos, uint32 nCount) override;
		virtual StringBuffer *Append(const char szString[], uint32 nCount) override;
		virtual StringBuffer *Append(const wchar_t szString[], uint32 nCount) override;
		virtual StringBuffer *Insert(const char szString[], uint32 nPos, uint32 nCount) override;
		virtual StringBuffer *Insert(const wchar_t szString[], uint32 nPos, uint32 nCount) override;
		virtual StringBuffer *Replace(char nOld, char nNew, uint32 &nReplaced) override;
		virtual StringBuffer *Replace(wchar_t nOld, wchar_t nNew, uint32 &nReplaced) override;
		virtual StringBuffer *Replace(const char szOld[], uint32 nOldLength, const char szNew[], uint32 nNewLength, uint32 &nReplaced) override;
		virtual StringBuffer *Replace(const wchar_t szOld[], uint32 nOldLength, const wchar_t szNew[], uint32 nNewLength, uint32 &nReplaced) override;
		virtual StringBuffer *SetCharacter(uint32 nIndex, char nCharacter) override;
		virtual StringBuffer *SetCharacter(uint32 nIndex, wchar_t nCharacter) override;
		virtual StringBuffer *TrimLeading() override;
		virtual StringBuffer *TrimTrailing() override;
		virtual StringBuffer *RemoveLineEndings() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_STRINGBUFFER_UTF8_H__
