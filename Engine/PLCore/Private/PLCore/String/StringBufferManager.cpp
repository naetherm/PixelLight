/*********************************************************\
 *  File: StringBufferManager.cpp                        *
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
#include "PLCore/Core/MemoryManager.h"
#include "PLCore/String/String.h"
#include "PLCore/String/StringBufferASCII.h"
#include "PLCore/String/StringBufferUnicode.h"
#include "PLCore/String/StringBufferManager.h"


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
StringBufferManager::StringBufferManager() :
	m_pStringBufferASCII(nullptr),
	m_pStringBufferUnicode(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
StringBufferManager::~StringBufferManager()
{
	// Destroy all buffered ASCII string buffers
	if (m_pStringBufferASCII) {
		for (uint32 i=0; i<MaxStringReuseLength; i++) {
			for (uint32 j=0; j<MaxStringsPerReuseLength; j++) {
				StringBufferASCII *pStringBufferASCII = m_pStringBufferASCII[i*MaxStringsPerReuseLength + j];
				if (pStringBufferASCII)
					delete pStringBufferASCII;
			}
		}
		delete [] m_pStringBufferASCII;
		m_pStringBufferASCII = nullptr;
	}

	// Destroy all buffered unicode string buffers
	if (m_pStringBufferUnicode) {
		for (uint32 i=0; i<MaxStringReuseLength; i++) {
			for (uint32 j=0; j<MaxStringsPerReuseLength; j++) {
				StringBufferUnicode *pStringBufferUnicode = m_pStringBufferUnicode[i*MaxStringsPerReuseLength + j];
				if (pStringBufferUnicode)
					delete pStringBufferUnicode;
			}
		}
		delete [] m_pStringBufferUnicode;
		m_pStringBufferUnicode = nullptr;
	}
}

/**
*  @brief
*    Returns an instance of a ASCII string buffer
*/
StringBufferASCII *StringBufferManager::GetStringBufferASCII(uint32 nLength)
{
	StringBufferASCII *pStringBufferASCII = nullptr;

	// Calculate the maximum available length of the string (excluding the terminating zero)
	const uint32 nMaxLength = nLength + NumOfReservedCharacters;

	// Can be reuse a previous string buffer?
	if (nMaxLength < MaxStringReuseLength && m_pStringBufferASCII) {
		// Try to find a used slot
		for (int j=MaxStringsPerReuseLength-1; j>=0; j--) {
			// Is this slot used?
			const uint32 nIndex = nMaxLength*MaxStringsPerReuseLength + j;
			if (m_pStringBufferASCII[nIndex]) {
				// Jap, revive this string buffer!
				pStringBufferASCII = m_pStringBufferASCII[nIndex];
				m_pStringBufferASCII[nIndex] = nullptr;

				// Get us out of the loop
				j = -1;
			}
		}
	}

	// Do we need to create a new string buffer?
	if (!pStringBufferASCII) {
		// Create the string
		char *pszString = new char[nMaxLength + 1];
		pszString[0] = '\0';

		// Create the ASCII string buffer instance
		pStringBufferASCII = new StringBufferASCII(pszString, 0, nMaxLength);
	}

	// Done
	return pStringBufferASCII;
}

/**
*  @brief
*    Returns an instance of a unicode string buffer
*/
StringBufferUnicode *StringBufferManager::GetStringBufferUnicode(uint32 nLength)
{
	StringBufferUnicode *pStringBufferUnicode = nullptr;

	// Calculate the maximum available length of the string (excluding the terminating zero)
	const uint32 nMaxLength = nLength + NumOfReservedCharacters;

	// Can be reuse a previous string buffer?
	if (nMaxLength < MaxStringReuseLength && m_pStringBufferUnicode) {
		// Try to find a used slot
		for (int j=MaxStringsPerReuseLength-1; j>=0; j--) {
			// Is this slot used?
			const uint32 nIndex = nMaxLength*MaxStringsPerReuseLength + j;
			if (m_pStringBufferUnicode[nIndex]) {
				// Jap, revive this string buffer!
				pStringBufferUnicode = m_pStringBufferUnicode[nIndex];
				m_pStringBufferUnicode[nIndex] = nullptr;

				// Get us out of the loop
				j = -1;
			}
		}
	}

	// Do we need to create a new string buffer?
	if (!pStringBufferUnicode) {
		// Create the string
		wchar_t *pszString = new wchar_t[nMaxLength + 1];
		pszString[0] = L'\0';

		// Create the unicode string buffer instance
		pStringBufferUnicode = new StringBufferUnicode(pszString, 0, nMaxLength);
	}

	// Done
	return pStringBufferUnicode;
}

/**
*  @brief
*    Releases an instance of a string buffer
*/
void StringBufferManager::ReleaseStringBuffer(StringBuffer &cStringBuffer)
{
	// Is the string buffer going to be destroyed if we release one more reference?
	if (cStringBuffer.GetFormat() != String::UTF8 && cStringBuffer.GetRefCount() < 2) {
		// Ok, this is the reason why we can't use the "RefCount"-template in here - the string buffer manager may keep the string buffer
		// alive for later reuse, but the "reused" string buffer really needs a "fresh" reference counter set to 0
		bool bBackuped = false;

		// Check the internal string format
		const uint32 nStringLength = cStringBuffer.m_nMaxLength;
		switch (cStringBuffer.GetFormat()) {
			case String::ASCII:
				// We don't want to keep alive long strings for later reuse...
				if (nStringLength < MaxStringReuseLength) {
					// Initialize?
					if (!m_pStringBufferASCII) {
						const uint32 nNumOfElements = MaxStringReuseLength*MaxStringsPerReuseLength;
						m_pStringBufferASCII = new StringBufferASCII*[nNumOfElements];
						MemoryManager::Set(m_pStringBufferASCII, 0, sizeof(StringBufferASCII*)*nNumOfElements);
					}
					if (m_pStringBufferASCII) {
						// Try to find a unused slot
						for (uint32 j=0; j<MaxStringsPerReuseLength; j++) {
							// Is this slot used?
							const uint32 nIndex = nStringLength*MaxStringsPerReuseLength + j;
							if (!m_pStringBufferASCII[nIndex]) {
								// Nope, our string buffer found a new home!
								StringBufferASCII *pStringBufferASCII = static_cast<StringBufferASCII*>(&cStringBuffer);
								m_pStringBufferASCII[nIndex] = pStringBufferASCII;
								bBackuped = true;

								// Reset the string buffer
								pStringBufferASCII->m_nRefCount = 0;
								pStringBufferASCII->SetNewStringLength(0);
								pStringBufferASCII->m_pszString[0] = '\0';

								// Get us out of the loop
								j = MaxStringsPerReuseLength;
							}
						}
					}
				}
				break;

			case String::Unicode:
				// We don't want to keep alive long strings for later reuse...
				if (nStringLength < MaxStringReuseLength) {
					// Initialize?
					if (!m_pStringBufferUnicode) {
						const uint32 nNumOfElements = MaxStringReuseLength*MaxStringsPerReuseLength;
						m_pStringBufferUnicode = new StringBufferUnicode*[nNumOfElements];
						MemoryManager::Set(m_pStringBufferUnicode, 0, sizeof(StringBufferUnicode*)*nNumOfElements);
					}
					if (m_pStringBufferUnicode) {
						// Try to find a unused slot
						for (uint32 j=0; j<MaxStringsPerReuseLength; j++) {
							// Is this slot used?
							const uint32 nIndex = nStringLength*MaxStringsPerReuseLength + j;
							if (!m_pStringBufferUnicode[nIndex]) {
								// Nope, our string buffer found a new home!
								StringBufferUnicode *pStringBufferUnicode = static_cast<StringBufferUnicode*>(&cStringBuffer);
								m_pStringBufferUnicode[nIndex] = pStringBufferUnicode;
								bBackuped = true;

								// Reset the string buffer
								pStringBufferUnicode->m_nRefCount = 0;
								pStringBufferUnicode->SetNewStringLength(0);
								pStringBufferUnicode->m_pszString[0] = '\0';

								// Get us out of the loop
								j = MaxStringsPerReuseLength;
							}
						}
					}
				}
				break;
		}

		// String buffer backuped?
		if (!bBackuped)
			cStringBuffer.Release();	// Sorry, no more backup space left!
	} else {
		// Just release the reference - after this, the string buffer will still be there because it's still referenced
		cStringBuffer.Release();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
