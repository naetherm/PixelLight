/*********************************************************\
 *  File: UTF8Tools.cpp                                  *
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
#include <stdlib.h>
#ifdef LINUX
	#include "PLCore/PLCoreLinuxWrapper.h"
#endif
#include "PLCore/Core/MemoryManager.h"
#include "PLCore/String/UTF8Tools.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Global variables                                      ]
//[-------------------------------------------------------]
static const uint32 g_nOffsetsFromUTF8[6] = {
	0x00000000UL, 0x00003080UL, 0x000E2080UL,
	0x03C82080UL, 0xFA082080UL, 0x82082080UL
};

static const uint8 g_nTrailingBytesForUTF8[256] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//   0. -  31. byte
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//  32. -  63. byte
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//  64. -  95. byte
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	//  96. - 127. byte -> ASCII
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	// 128. - 159. byte
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	// 160. - 191. byte
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,	// 192. - 223. byte
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5	// 224. - 255. byte
};


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the number of bytes a character requires
*/
uint8 UTF8Tools::GetNumOfCharacterBytes(char nCharacter)
{
	return g_nTrailingBytesForUTF8[static_cast<unsigned char>(nCharacter)] + 1;
}

/**
*  @brief
*    Returns the number of bytes a given wide character requires
*/
uint8 UTF8Tools::GetNumOfCharacterBytes(wchar_t nWideCharacter)
{
		 if (nWideCharacter < 0x80)		return 1;
	else if (nWideCharacter < 0x800)	return 2;
	else if (nWideCharacter < 0x10000)	return 3;
	else if (nWideCharacter < 0x110000)	return 4;
	else								return 0; // Error!
}

/**
*  @brief
*    Returns a character as wide character
*/
wchar_t UTF8Tools::GetWideCharacter(const char *pnCharacter)
{
	// Check the given pointer
	if (pnCharacter) {
		uint32 nWideCharacter   = 0; // wchar_t may be just 2 bytes long, so use uint32 during the conversion to be on the safe side...
		uint32 nNumOfCharacters = 0;

		// Get the wide character
		const char *pnCurrentCharacter = pnCharacter;
		do {
			nWideCharacter <<= 6;
			nWideCharacter += static_cast<unsigned char>(*pnCurrentCharacter);
			pnCurrentCharacter++;
			nNumOfCharacters++;
		} while (*pnCurrentCharacter != '\0' && !IsSequenceStart(*pnCurrentCharacter));
		nWideCharacter -= g_nOffsetsFromUTF8[nNumOfCharacters - 1];

		// Return the wide character
		return static_cast<wchar_t>(nWideCharacter);
	} else {
		// Error!
		return L'\0';
	}
}

/**
*  @brief
*    Returns the next character as wide character
*/
wchar_t UTF8Tools::GetNextWideCharacter(const char **ppszString)
{
	// Check for terminating zero
	if (**ppszString == '\0')
		return 0; // Error!

	// Get the wide character
	uint32 nWideCharacter   = 0;	// wchar_t may be just 2 bytes long, so use uint32 during the conversion to be on the safe side...
	uint32 nNumOfCharacters = 0;
	do {
		nWideCharacter <<= 6;
		nWideCharacter += static_cast<unsigned char>(**ppszString);
		(*ppszString)++;
		nNumOfCharacters++;
	} while (**ppszString != '\0' && !IsSequenceStart(**ppszString));
	nWideCharacter -= g_nOffsetsFromUTF8[nNumOfCharacters - 1];

	// Return the wide character
	return static_cast<wchar_t>(nWideCharacter);
}

/**
*  @brief
*    Moves to the next character
*/
uint8 UTF8Tools::MoveToNextCharacter(const char **ppszString)
{
	// Check for terminating zero
	if (**ppszString == '\0')
		return 0; // Error!

	(*ppszString)++; // Next character, please
	if (!IsSequenceStart(**ppszString)) {
		(*ppszString)++; // Next character, please
		if (!IsSequenceStart(**ppszString)) {
			(*ppszString)++; // Next character, please
			if (!IsSequenceStart(**ppszString)) {
				(*ppszString)++; // Next character, please
				 return 4;
			} else {
				return 3;
			}
		} else {
			return 2;
		}
	} else {
		return 1;
	}
}

/**
*  @brief
*    Moves to the previous character
*/
uint8 UTF8Tools::MoveToPreviousCharacter(const char **ppszString)
{
	(*ppszString)--; // Previous character, please
	if (!IsSequenceStart(**ppszString)) {
		(*ppszString)--; // Previous character, please
		if (!IsSequenceStart(**ppszString)) {
			(*ppszString)--; // Previous character, please
			if (!IsSequenceStart(**ppszString)) {
				(*ppszString)--; // Previous character, please
				 return 4;
			} else {
				return 3;
			}
		} else {
			return 2;
		}
	} else {
		return 1;
	}
}

/**
*  brief
*    Character index => byte offset were the character starts within the given UTF8 string
*/
uint32 UTF8Tools::CharacterIndexToByteOffset(const char *pszString, uint32 nCharacterIndex)
{
	// Check the given pointer
	if (pszString) {
		uint32 nOffset = 0;

		while (nCharacterIndex > 0 && pszString[nOffset] != '\0') {
			(void)(IsSequenceStart(pszString[++nOffset]) || IsSequenceStart(pszString[++nOffset]) ||
				IsSequenceStart(pszString[++nOffset]) || ++nOffset);
			nCharacterIndex--;
		}
		return nOffset;
	} else {
		// Error!
		return 0;
	}
}

/**
*  brief
*    Byte offset => character index were the character starts within the given UTF8 string
*/
uint32 UTF8Tools::ByteOffsetToCharacterIndex(const char *pszString, uint32 nOffset)
{
	// Check the given pointer
	if (pszString) {
		uint32 nCharacterIndex = 0, nOffsetT = 0;

		while (nOffsetT < nOffset && pszString[nOffsetT] != '\0') {
			(void)(IsSequenceStart(pszString[++nOffsetT]) || IsSequenceStart(pszString[++nOffsetT]) ||
				IsSequenceStart(pszString[++nOffsetT]) || ++nOffsetT);
			nCharacterIndex++;
		}
		return nCharacterIndex;
	} else {
		// Error!
		return 0;
	}
}

/**
*  @brief
*    Returns the number of bytes a given string requires
*/
uint32 UTF8Tools::GetNumOfStringBytes(const char *pszString, uint32 nCount)
{
	// Check the given pointer
	if (pszString) {
		uint32 nNumOfBytes = 0;

		// Whole string or just a part of it?
		if (nCount) {
			// Get the number of bytes of a string part
			uint32 nCharacterIndex = 0;
			for (const char *pszCurrent=pszString; *pszCurrent!='\0'&&nCharacterIndex<nCount; nCharacterIndex++) {
				// Next character, please
				const uint32 nCharacterSize = GetNumOfCharacterBytes(*pszCurrent);
				nNumOfBytes += nCharacterSize;
				if (nCharacterSize)
					pszCurrent += nCharacterSize;
				else
					return 0; // Error!
			}
		} else {
			// Get the number of bytes of the whole string
			for (const char *pszCurrent=pszString; *pszCurrent!='\0'; nNumOfBytes++, pszCurrent++)
				; // Nothing to do
		}

		// Return the number of bytes
		return nNumOfBytes;
	} else {
		// Error!
		return 0;
	}
}

/**
*  @brief
*    Returns the number of characters within a given string
*/
uint32 UTF8Tools::GetNumOfCharacters(const char *pszString)
{
	// Check the given pointer
	if (pszString) {
		// Get the number of characters
		uint32 nNumOfCharacters = 0;
		for (const char *pszCurrent=pszString; *pszCurrent!='\0'; nNumOfCharacters++) {
			// Next character, please
			const uint32 nCharacterSize = GetNumOfCharacterBytes(*pszCurrent);
			if (nCharacterSize)
				pszCurrent += nCharacterSize;
			else
				return 0; // Error!
		}

		// Return the number of characters
		return nNumOfCharacters;
	} else {
		// Error!
		return 0;
	}
}

/**
*  @brief
*    Returns the number of characters within a given string
*/
uint32 UTF8Tools::GetNumOfCharacters(const char *pszString, uint32 nNumOfBytes)
{
	// Check the given pointer
	if (pszString) {
		// Get the number of characters
		const char   *pszStringEnd     = pszString + nNumOfBytes;
		uint32        nNumOfCharacters = 0;
		for (const char *pszCurrent=pszString; pszCurrent<pszStringEnd; nNumOfCharacters++) {
			// Next character, please
			const uint32 nCharacterSize = GetNumOfCharacterBytes(*pszCurrent);
			if (nCharacterSize)
				pszCurrent += nCharacterSize;
			else
				return 0; // Error!
		}

		// Return the number of characters
		return nNumOfCharacters;
	} else {
		// Error!
		return 0;
	}
}

/**
*  @brief
*    Returns the number of characters and bytes within a given string
*/
uint32 UTF8Tools::GetNumOfCharactersAndBytes(const char *pszString, uint32 &nNumOfBytes)
{
	// Check the given pointer
	if (pszString) {
		// Get the number of characters
		const char   *pszCurrent       = pszString;
		uint32        nNumOfCharacters = 0;
		for (; *pszCurrent!='\0'; nNumOfCharacters++) {
			// Next character, please
			const uint32 nCharacterSize = GetNumOfCharacterBytes(*pszCurrent);
			if (nCharacterSize)
				pszCurrent += nCharacterSize;
			else {
				nNumOfBytes = 0;
				return 0; // Error!
			}
		}

		// Get the number of bytes
		nNumOfBytes = static_cast<uint32>(pszCurrent - pszString);

		// Return the number of characters
		return nNumOfCharacters;
	} else {
		// Error!
		return 0;
	}
}

/**
*  @brief
*    Returns a pointer to the first occurrence of the given wide character in the given string, or a null pointer if not found
*/
const char *UTF8Tools::FindCharacter(const char *pszString, wchar_t nWideCharacter, uint32 *pnCharacterIndex)
{
	// Check the given pointer
	if (pszString) {
		const char *pszStringCurrent = pszString;
		const char *pszStringLast    = pszString;

		// Find the character
		if (pnCharacterIndex) {
			*pnCharacterIndex = 0;
			while (*pszStringCurrent != '\0') {
				if (GetNextWideCharacter(&pszStringCurrent) == nWideCharacter)
					return pszStringLast; // Character found
				pszStringLast = pszStringCurrent;
				(*pnCharacterIndex)++;
			}

			// Character not found
			*pnCharacterIndex = 0;
		} else {
			while (*pszStringCurrent != '\0') {
				if (GetNextWideCharacter(&pszStringCurrent) == nWideCharacter)
					return pszStringLast; // Character found
				pszStringLast = pszStringCurrent;
			}
		}
	}

	// Character not found
	return nullptr;
}

/*
*  @brief
*    Same as the above, but searches a buffer of a given size instead of a zero-terminated string
*/
const char *UTF8Tools::FindCharacter(const char *pszString, uint32 nNumOfBytes, wchar_t nWideCharacter, uint32 *pnCharacterIndex)
{
	// Check the given pointer
	if (pszString) {
		const char *pszStringEnd     = pszString + nNumOfBytes;
		const char *pszStringCurrent = pszString;
		const char *pszStringLast    = pszString;
		uint32 c, csz;	// wchar_t may be just 2 bytes long, so use uint32 during the conversion to be on the safe side...

		// Find character
		if (pnCharacterIndex) {
			*pnCharacterIndex = 0;
			while (pszStringCurrent < pszStringEnd) {
				c = csz = 0;
				do {
					c <<= 6;
					c += static_cast<unsigned char>(*pszStringCurrent);
					pszStringCurrent++;
					csz++;
				} while (pszStringCurrent < pszStringEnd && !IsSequenceStart(*pszStringCurrent));
				c -= g_nOffsetsFromUTF8[csz - 1];

				if (static_cast<wchar_t>(c) == nWideCharacter)
					return pszStringLast; // Character found
				pszStringLast = pszStringCurrent;
				(*pnCharacterIndex)++;
			}

			// Character not found
			*pnCharacterIndex = 0;
		} else {
			while (pszStringCurrent < pszStringEnd) {
				c = csz = 0;
				do {
					c <<= 6;
					c += static_cast<unsigned char>(*pszStringCurrent);
					pszStringCurrent++;
					csz++;
				} while (pszStringCurrent < pszStringEnd && !IsSequenceStart(*pszStringCurrent));
				c -= g_nOffsetsFromUTF8[csz - 1];

				if (static_cast<wchar_t>(c) == nWideCharacter)
					return pszStringLast; // Character found
				pszStringLast = pszStringCurrent;
			}
		}
	}

	// Character not found
	return nullptr;
}

/**
*  @brief
*    Single wide character character to UTF8
*/
uint8 UTF8Tools::FromWideCharacter(char *pszDestination, wchar_t nWideCharacter)
{
	// Check the given pointer
	if (pszDestination) {
		const uint32 nWideCharacter32 = nWideCharacter; // wchar_t may be just 2 bytes long, so use uint32 during the conversion to be on the safe side...
		if (nWideCharacter32 < 0x80) {
			pszDestination[0] = static_cast<char>(nWideCharacter32);
			pszDestination[1] = '\0';
			return 1;
		} else if (nWideCharacter32 < 0x800) {
			pszDestination[0] = static_cast<char>((nWideCharacter32 >> 6) | 0xC0);
			pszDestination[1] = (nWideCharacter32 & 0x3F) | 0x80;
			pszDestination[2] = '\0';
			return 2;
		} else if (nWideCharacter32 < 0x10000) {
			pszDestination[0] = static_cast<char>((nWideCharacter32 >> 12) | 0xE0);
			pszDestination[1] = ((nWideCharacter32 >> 6) & 0x3F) | 0x80;
			pszDestination[2] = (nWideCharacter32 & 0x3F) | 0x80;
			pszDestination[3] = '\0';
			return 3;
		} else if (nWideCharacter32 < 0x110000) {
			pszDestination[0] = static_cast<char>((nWideCharacter32 >> 18) | 0xF0);
			pszDestination[1] = ((nWideCharacter32 >> 12) & 0x3F) | 0x80;
			pszDestination[2] = ((nWideCharacter32 >> 6) & 0x3F) | 0x80;
			pszDestination[3] = (nWideCharacter32 & 0x3F) | 0x80;
			pszDestination[4] = '\0';
			return 4;
		}
	}

	// Error!
	return 0;
}

/**
*  @brief
*    Wide character string to UTF8
*/
uint32 UTF8Tools::FromWideCharacterString(char *pszDestination, uint32 nNumOfBytes, const wchar_t *pszSource, uint32 nSourceLength)
{
	// Check the given source pointer
	if (pszSource) {
		uint32 nNumOfCharacters = 0, nNumOfUsedBytes = 0;

		// Just return the number of required bytes?
		if (pszDestination) {
			const wchar_t *pszCurrentSource = pszSource;
			char *pnDestinationEnd = pszDestination + nNumOfBytes;
			while (!nSourceLength ? *pszCurrentSource!='\0' : nNumOfCharacters<nSourceLength) {
				if (*pszCurrentSource < 0x80) {
					if (pszDestination >= pnDestinationEnd)
						return nNumOfBytes;
					*pszDestination++ = static_cast<char>(*pszCurrentSource);
					nNumOfUsedBytes += 1;
				} else if (*pszCurrentSource < 0x800) {
					if (pszDestination >= pnDestinationEnd-1)
						return nNumOfBytes;
					*pszDestination++ = static_cast<char>((*pszCurrentSource>>6) | 0xC0);
					*pszDestination++ = (*pszCurrentSource & 0x3F) | 0x80;
					nNumOfUsedBytes += 2;
				} else if (*pszCurrentSource < 0x10000) {
					if (pszDestination >= pnDestinationEnd-2)
						return nNumOfBytes;
					*pszDestination++ = (*pszCurrentSource>>12) | 0xE0;
					*pszDestination++ = ((*pszCurrentSource>>6) & 0x3F) | 0x80;
					*pszDestination++ = (*pszCurrentSource & 0x3F) | 0x80;
					nNumOfUsedBytes += 3;
				} else if (*pszCurrentSource < 0x110000) {
					if (pszDestination >= pnDestinationEnd-3)
						return nNumOfBytes;
					*pszDestination++ = (*pszCurrentSource>>18) | 0xF0;
					*pszDestination++ = ((*pszCurrentSource>>12) & 0x3F) | 0x80;
					*pszDestination++ = ((*pszCurrentSource>>6) & 0x3F) | 0x80;
					*pszDestination++ = (*pszCurrentSource & 0x3F) | 0x80;
					nNumOfUsedBytes += 4;
				}
				pszCurrentSource++;
				nNumOfCharacters++;
			}
			if (pszDestination < pnDestinationEnd)
				*pszDestination = '\0';
		} else {
			const wchar_t *pszCurrentSource = pszSource;
			while (!nSourceLength ? *pszCurrentSource!='\0' : nNumOfCharacters<nSourceLength) {
				if (*pszCurrentSource < 0x80) {
					nNumOfUsedBytes += 1;
				} else if (*pszCurrentSource < 0x800) {
					nNumOfUsedBytes += 2;
				} else if (*pszCurrentSource < 0x10000) {
					nNumOfUsedBytes += 3;
				} else if (*pszCurrentSource < 0x110000) {
					nNumOfUsedBytes += 4;
				}
				pszCurrentSource++;
				nNumOfCharacters++;
			}
		}

		// Return the number of used bytes
		return nNumOfUsedBytes;
	} else {
		// Error!
		return 0;
	}
}

/**
*  @brief
*    UTF8 to wide character string
*/
uint32 UTF8Tools::ToWideCharacterString(wchar_t *pszDestination, uint32 nLength, const char *pszSource, uint32 nSourceNumOfBytes)
{
	// Check the given pointers
	if (pszDestination && pszSource) {
		const char *pszCurrentSource = pszSource;
		const char *pszSourceEnd     = pszCurrentSource + nSourceNumOfBytes;
		uint32 nNumOfCharacters = 0;

		while (nNumOfCharacters < nLength-1) {
			const uint8 nTrailingBytes = g_nTrailingBytesForUTF8[static_cast<unsigned char>(*pszCurrentSource)];
			if (nSourceNumOfBytes) {
				if (pszCurrentSource + nTrailingBytes >= pszSourceEnd)
					break;
			} else {
				if (*pszCurrentSource == '\0')
					break;
			}
			uint32 nWideCharacter = 0;	// wchar_t may be just 2 bytes long, so use uint32 during the conversion to be on the safe side...
			switch (nTrailingBytes) {
				// These fall through deliberately
				case 3: nWideCharacter += static_cast<unsigned char>(*pszCurrentSource++); nWideCharacter <<= 6;
				case 2: nWideCharacter += static_cast<unsigned char>(*pszCurrentSource++); nWideCharacter <<= 6;
				case 1: nWideCharacter += static_cast<unsigned char>(*pszCurrentSource++); nWideCharacter <<= 6;
				case 0: nWideCharacter += static_cast<unsigned char>(*pszCurrentSource++);
			}
			nWideCharacter -= g_nOffsetsFromUTF8[nTrailingBytes];
			pszDestination[nNumOfCharacters++] = static_cast<wchar_t>(nWideCharacter);
		}

		// Set terminating zero
		pszDestination[nNumOfCharacters] = '\0';

		// Return the number of converted characters
		return nNumOfCharacters;
	} else {
		// Error!
		return 0;
	}
}

/**
*  @brief
*    Given a wide character, convert it to an ASCII escape sequence stored in the given destination buffer
*/
uint32 UTF8Tools::EscapeWideCharacter(char *pszDestination, uint32 nNumOfBytes, wchar_t nWideCharacter)
{
	// Just return the number of required bytes?
	if (pszDestination) {
			 if (nWideCharacter == L'\n')							 return _snprintf(pszDestination, nNumOfBytes, "\\n");
		else if (nWideCharacter == L'\t')							 return _snprintf(pszDestination, nNumOfBytes, "\\t");
		else if (nWideCharacter == L'\r')							 return _snprintf(pszDestination, nNumOfBytes, "\\r");
		else if (nWideCharacter == L'\b')							 return _snprintf(pszDestination, nNumOfBytes, "\\b");
		else if (nWideCharacter == L'\f')							 return _snprintf(pszDestination, nNumOfBytes, "\\f");
		else if (nWideCharacter == L'\v')							 return _snprintf(pszDestination, nNumOfBytes, "\\v");
		else if (nWideCharacter == L'\a')							 return _snprintf(pszDestination, nNumOfBytes, "\\a");
		else if (nWideCharacter == L'\\')							 return _snprintf(pszDestination, nNumOfBytes, "\\\\");
		else if (nWideCharacter < 32 || nWideCharacter == 0x7f)		 return _snprintf(pszDestination, nNumOfBytes, "\\x%hhX", static_cast<unsigned char>(nWideCharacter));
		else if (nWideCharacter > 0xFFFF)							 return _snprintf(pszDestination, nNumOfBytes, "\\U%.8X", static_cast<wchar_t>(nWideCharacter));
		else if (nWideCharacter >= 0x80 && nWideCharacter <= 0xFFFF) return _snprintf(pszDestination, nNumOfBytes, "\\u%.4hX", static_cast<unsigned short>(nWideCharacter));
		else														 return _snprintf(pszDestination, nNumOfBytes, "%c", static_cast<char>(nWideCharacter));
	} else {
			 if (nWideCharacter == L'\n')							 return 2;
		else if (nWideCharacter == L'\t')							 return 2;
		else if (nWideCharacter == L'\r')							 return 2;
		else if (nWideCharacter == L'\b')							 return 2;
		else if (nWideCharacter == L'\f')							 return 2;
		else if (nWideCharacter == L'\v')							 return 2;
		else if (nWideCharacter == L'\a')							 return 2;
		else if (nWideCharacter == L'\\')							 return 2;
		else if (nWideCharacter < 32 || nWideCharacter == 0x7f)		 return 4; // [TODO] Check me!
		else if (nWideCharacter > 0xFFFF)							 return 10; // [TODO] Check me!
		else if (nWideCharacter >= 0x80 && nWideCharacter <= 0xFFFF) return 6;
		else														 return 1;
	}
}

/**
*  @brief
*    Converts an UTF8 string to an ASCII string with escape sequences
*/
uint32 UTF8Tools::Escape(char *pszDestination, uint32 nNumOfBytes, const char *pszSource, bool bEscapeQuotes)
{
	// Check the given source pointer
	if (pszSource) {
		uint32 nNumOfCharacters = 0, nCount;
		const char *pszCurrentSource = pszSource;

		// Just return the number of required bytes?
		if (pszDestination) {
			while (*pszCurrentSource != '\0' && nNumOfCharacters < nNumOfBytes) {
				if (bEscapeQuotes && *pszCurrentSource == '"') {
					nCount = _snprintf(pszDestination, nNumOfBytes - nNumOfCharacters, "\\\"");
					pszCurrentSource++;
				} else {
					nCount = EscapeWideCharacter(pszDestination, nNumOfBytes - nNumOfCharacters, GetNextWideCharacter(&pszCurrentSource));
				}
				nNumOfCharacters += nCount;
				pszDestination	 += nCount;
			}
			if (nNumOfCharacters < nNumOfBytes)
				*pszDestination = '\0';
		} else {
			while (*pszCurrentSource != '\0') {
				if (bEscapeQuotes && *pszCurrentSource == '"') {
					nCount = 2;
					pszCurrentSource++;
				} else {
					nCount = EscapeWideCharacter(nullptr, 0, GetNextWideCharacter(&pszCurrentSource));
				}
				nNumOfCharacters += nCount;
			}
		}

		// Return the number of resulting characters
		return nNumOfCharacters;
	} else {
		// Error!
		return 0;
	}
}

/**
*  @brief
*    Assuming 'pszSource' points to the character after a backslash, read an escape sequence,
*    storing the result in 'nDestination' and returning the number of input characters processed
*/
uint32 UTF8Tools::ReadEscapeSequence(wchar_t &nDestination, const char *pszSource)
{
	// Check the given source pointer
	if (pszSource) {
		char nDigs[10] = "\0\0\0\0\0\0\0\0\0";
		uint32 nNumOfCharacters = 1;

		wchar_t nWideCharacter = static_cast<wchar_t>(pszSource[0]); // Take literal character
			 if (pszSource[0] == 'n') nWideCharacter = L'\n';
		else if (pszSource[0] == 't') nWideCharacter = L'\t';
		else if (pszSource[0] == 'r') nWideCharacter = L'\r';
		else if (pszSource[0] == 'b') nWideCharacter = L'\b';
		else if (pszSource[0] == 'f') nWideCharacter = L'\f';
		else if (pszSource[0] == 'v') nWideCharacter = L'\v';
		else if (pszSource[0] == 'a') nWideCharacter = L'\a';
		else if (IsOctalDigit(pszSource[0])) {
			uint32 nDigNumber = 0;
			nNumOfCharacters = 0;
			do {
				nDigs[nDigNumber++] = pszSource[nNumOfCharacters++];
			} while (IsOctalDigit(pszSource[nNumOfCharacters]) && nDigNumber < 3);
			nWideCharacter = static_cast<wchar_t>(strtol(nDigs, nullptr, 8));
		} else if (pszSource[0] == 'x') {
			uint32 nDigNumber = 0;
			while (IsHexDigit(pszSource[nNumOfCharacters]) && nDigNumber < 2) {
				nDigs[nDigNumber++] = pszSource[nNumOfCharacters++];
			}
			if (nDigNumber > 0)
				nWideCharacter = static_cast<wchar_t>(strtol(nDigs, nullptr, 16));
		} else if (pszSource[0] == 'u') {
			uint32 nDigNumber = 0;
			while (IsHexDigit(pszSource[nNumOfCharacters]) && nDigNumber < 4) {
				nDigs[nDigNumber++] = pszSource[nNumOfCharacters++];
			}
			if (nDigNumber > 0)
				nWideCharacter = static_cast<wchar_t>(strtol(nDigs, nullptr, 16));
		} else if (pszSource[0] == 'U') {
			uint32 nDigNumber = 0;
			while (IsHexDigit(pszSource[nNumOfCharacters]) && nDigNumber < 8) {
				nDigs[nDigNumber++] = pszSource[nNumOfCharacters++];
			}
			if (nDigNumber > 0)
				nWideCharacter = static_cast<wchar_t>(strtol(nDigs, nullptr, 16));
		}
		nDestination = nWideCharacter;

		// Return the number of read characters
		return nNumOfCharacters;
	} else {
		// Error!
		return 0;
	}
}

/**
*  @brief
*    Converts a string with literal \uxxxx or \Uxxxxxxxx characters to UTF8
*/
uint32 UTF8Tools::Unescape(char *pszDestination, uint32 nNumOfBytes, const char *pszSource)
{
	// Check the given source pointer for a null pointer
	if (pszSource) {
		uint32 nNumOfProcessedBytes = 0, nCount;
		const char *pszCurrentSource = pszSource;
		wchar_t nWideCharacter;
		char szTemp[4];

		// Just return the number of required bytes?
		if (pszDestination) {
			while (*pszCurrentSource != '\0' && nNumOfProcessedBytes < nNumOfBytes) {
				if (*pszCurrentSource == '\\') {
					pszCurrentSource++; // Skip the backslash
					nCount = ReadEscapeSequence(nWideCharacter, pszCurrentSource);
				} else {
					nWideCharacter = static_cast<wchar_t>(*pszCurrentSource);
					nCount = 1;
				}
				pszCurrentSource += nCount;
				nCount = FromWideCharacter(szTemp, nWideCharacter);
				if (nCount > nNumOfBytes - nNumOfProcessedBytes)
					break;
				MemoryManager::Copy(&pszDestination[nNumOfProcessedBytes], szTemp, nCount);
				nNumOfProcessedBytes += nCount;
			}
			if (nNumOfProcessedBytes < nNumOfBytes)
				pszDestination[nNumOfProcessedBytes] = '\0';
		} else {
			while (*pszCurrentSource != '\0') {
				if (*pszCurrentSource == '\\') {
					pszCurrentSource++; // Skip the backslash
					nCount = ReadEscapeSequence(nWideCharacter, pszCurrentSource);
				} else {
					nWideCharacter = static_cast<wchar_t>(*pszCurrentSource);
					nCount = 1;
				}
				pszCurrentSource += nCount;
				nCount = FromWideCharacter(szTemp, nWideCharacter);
				nNumOfProcessedBytes += nCount;
			}
		}

		// Return the number of processed bytes
		return nNumOfProcessedBytes;
	} else {
		// Error!
		return 0;
	}
}

/**
*  @brief
*    Compares two UTF8 strings
*/
int UTF8Tools::Compare(const char *pszFirst, const char *pszSecond, uint32 nCount)
{
	// Check the given pointers
	if (pszFirst && pszSecond) {
		// Get the length of both strings
		uint32 nFirstLength  = GetNumOfCharacters(pszFirst);
		uint32 nSecondLength = GetNumOfCharacters(pszSecond);

		// Check if the count is valid
		if (nCount > nFirstLength && nCount > nSecondLength)
			nCount = 0;

		if (nCount) {
			// Is the first UTF8 string shorter then the amount of characters to compare
			if (nFirstLength < nCount)
				return -1;

			// Is the second UTF8 string shorter then the amount of characters to compare
			if (nSecondLength < nCount)
				return 1;
		} else {
			// Is the first UTF8 string shorter then the second one (character-wise)?
			if (nFirstLength < nSecondLength)
				return -1;

			// Is the first UTF8 string longer then the second one (character-wise)?
			if (nFirstLength > nSecondLength)
				return 1;
		}

		// Both strings have the same length (character-wise)
		const char *pString1 = pszFirst;
		const char *pString2 = pszSecond;
		uint32 nLength = nFirstLength;

		// Should only 'nCount' characters be compared?
		if (nCount)
			nLength = nCount;

		// Compare characters
		for (uint32 i=0; i<nLength; i++) {
			// Check if character is a ASCII or UTF8 one
			if (IsSequenceStart(*pString1) && IsSequenceStart(*pString2)) {
				// Both characters are either a ASCII character or a UTF8 character
				const uint32 nFirstCharacterLength  = GetNumOfCharacterBytes(*pString1);
				const uint32 nSecondCharacterLength = GetNumOfCharacterBytes(*pString2);

				if (nFirstCharacterLength > nSecondCharacterLength)
					return 1;
				if (nFirstCharacterLength < nSecondCharacterLength)
					return -1;

				// Both characters have the same amount of bytes so we have to check each byte
				for (uint32 j=0; j<nFirstCharacterLength; j++) {
					if (pString1[j] > pString2[j])
						return 1;
					if (pString1[j] < pString2[j])
						return -1;
				}
			}

			// Move to the next character in both UTF8 strings
			MoveToNextCharacter(&pString1);
			MoveToNextCharacter(&pString2);
		}
	}

	// Both strings are equal
	return 0;
}

/**
*  @brief
*    Find a substring in a UTF8 string
*/
const char *UTF8Tools::FindSubstring(const char *pszSource, const char *pszSubstring)
{
	// Check the given pointers
	if (pszSource && pszSubstring) {
		// Get the length of the strings
		uint32 nSourceNumOfBytes    = 0;
		uint32 nSubstringNumOfBytes = 0;
		uint32 nSourceLength    = GetNumOfCharactersAndBytes(pszSource,    nSourceNumOfBytes);
		uint32 nSubstringLength = GetNumOfCharactersAndBytes(pszSubstring, nSubstringNumOfBytes);

		const char *pszSubStart = nullptr;
		uint32 nNextSubChar = 0;
		const char *pszSourceTemp    = pszSource;
		const char *pszSubstringTemp = pszSubstring;

		for (uint32 i=0; i<nSourceLength; i++) {
			// Check if both characters are either ASCII or UTF8
			bool bSame = false;
			if (IsSequenceStart(*pszSourceTemp) && IsSequenceStart(*pszSubstringTemp)) {
				// Both characters are either a ASCII character or a UTF8 character
				uint32 nSourceCharacterLength    = GetNumOfCharacterBytes(*pszSourceTemp);
				uint32 nSubstringCharacterLength = GetNumOfCharacterBytes(*pszSubstringTemp);

				if (nSourceCharacterLength == nSubstringCharacterLength) {
					// Both characters have the same amount of bytes
					for (uint32 j=0; j<nSourceCharacterLength; j++) {
						if (pszSourceTemp[j] != pszSubstringTemp[j]) {
							bSame = false;
							break;
						} else {
							if (!bSame)
								bSame = true;
						}
					}
				}
			}
			if (bSame) {
				// Both characters are the same
				if (!pszSubStart)
					pszSubStart = pszSourceTemp;

				nNextSubChar++;
				MoveToNextCharacter(&pszSubstringTemp);

				// Have we found the substring?
				if (nNextSubChar == nSubstringLength)
					break;

			} else if (pszSubStart) {
				pszSubStart      = nullptr;
				pszSubstringTemp = pszSubstring;
				nNextSubChar     = 0;
			}

			// Move to the next character in both UTF8 strings
			MoveToNextCharacter(&pszSourceTemp);
		}

		// Done
		return pszSubStart;
	} else {
		// Error!
		return nullptr;
	}
}

/**
*  @brief
*    Copies the given UTF8 source string into the destination string
*/
char *UTF8Tools::Copy(char *pszDestination, const char *pszSource, uint32 nCount)
{
	// Check the given pointers
	if (pszDestination && pszSource) {
		// Get the length of the source string
		uint32 nSourceNumOfBytes = 0;
		uint32 nSourceLength     = GetNumOfCharactersAndBytes(pszSource, nSourceNumOfBytes);

		if (nCount) {
			// Only 'nCount' characters should be copied
			if (nSourceLength < nCount) {
				// 'nCount' is greater then 'nSourceLength' so fill the rest (nCount-nSourceLength) of 'pszDestination' with '\0'
				MemoryManager::Copy(pszDestination, pszSource, nSourceNumOfBytes+1);
				for (uint32 i=0; i<nCount-nSourceNumOfBytes; i++)
					pszDestination[i + nSourceNumOfBytes] = '\0';
			} else {
				nSourceNumOfBytes = CharacterIndexToByteOffset(pszSource, nCount);
				MemoryManager::Copy(pszDestination, pszSource, nSourceNumOfBytes);
			}
		} else {
			// The complete source string should be copied
			MemoryManager::Copy(pszDestination, pszSource, nSourceNumOfBytes+1);
			pszDestination[nSourceNumOfBytes] = '\0';
		}

		return pszDestination;
	} else {
		// Error!
		return nullptr;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
