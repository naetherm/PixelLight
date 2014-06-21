/*********************************************************\
 *  File: Functions.cpp                                  *
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
#include <string.h>
#include "PLCore/String/String.h"
#include "PLCore/Container/Functions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public static HashFunction functions                  ]
//[-------------------------------------------------------]
uint32 HashFunction::Hash(const int32 &nKey)
{
	return static_cast<uint32>(nKey);
}

uint32 HashFunction::Hash(const uint32 &nKey)
{
	return nKey;
}

uint32 HashFunction::Hash(const int64 &nKey)
{
	return static_cast<uint32>(nKey);
}

uint32 HashFunction::Hash(const uint64 &nKey)
{
	return static_cast<uint32>(nKey);
}

uint32 HashFunction::Hash(const char *pszKey)
{
	// Check the given pointer for a null pointer
	if (pszKey) {
		uint32 h = 0;

		while (*pszKey) {
			h = (h << 4) + *pszKey++;
			uint32 g = h & 0xF0000000;
			if (g)
				h ^= g >> 24;
			h &= ~g;
		}

		return h;
	} else {
		// Error!
		return 0;
	}
}

uint32 HashFunction::Hash(const String &sKey)
{
	// Hash depending on the internal string format
	switch (sKey.GetFormat()) {
		case String::ASCII:
		{
			const char *pszKey = sKey.GetASCII();
			uint32 h = 0;

			while (*pszKey) {
				h = (h << 4) + *pszKey++;
				uint32 g = h & 0xF0000000;
				if (g)
					h ^= g >> 24;
				h &= ~g;
			}

			return h;
		}

		case String::Unicode:
		{
			const wchar_t *pszKey = sKey.GetUnicode();
			uint32 h = 0;

			while (*pszKey) {
				h = (h << 4) + *pszKey++;
				uint32 g = h & 0xF0000000;
				if (g)
					h ^= g >> 24;
				h &= ~g;
			}

			return h;
		}

		case String::UTF8:	// An UTF8 string is only cached inside the string class, but not used for actual string operations
		default:
			return 0; // Error!
	}
}


//[-------------------------------------------------------]
//[ Public static CompareFunction functions               ]
//[-------------------------------------------------------]
// int32
bool CompareFunction::AreEqual(const int32 &nKey1, const int32 &nKey2)
{
	return (nKey1 == nKey2);
}

bool CompareFunction::IsGreater(const int32 &nKey1, const int32 &nKey2)
{
	return (nKey1 > nKey2);
}

// uint32
bool CompareFunction::AreEqual(const uint32 &nKey1, const uint32 &nKey2)
{
	return (nKey1 == nKey2);
}

bool CompareFunction::IsGreater(const uint32 &nKey1, const uint32 &nKey2)
{
	return (nKey1 > nKey2);
}

// int64
bool CompareFunction::AreEqual(const int64 &nKey1, const int64 &nKey2)
{
	return (nKey1 == nKey2);
}

bool CompareFunction::IsGreater(const int64 &nKey1, const int64 &nKey2)
{
	return (nKey1 > nKey2);
}

// uint64
bool CompareFunction::AreEqual(const uint64 &nKey1, const uint64 &nKey2)
{
	return (nKey1 == nKey2);
}

bool CompareFunction::IsGreater(const uint64 &nKey1, const uint64 &nKey2)
{
	return (nKey1 > nKey2);
}

// float
bool CompareFunction::AreEqual(const float &fKey1, const float &fKey2)
{
	return (fKey1 == fKey2);
}

bool CompareFunction::IsGreater(const float &fKey1, const float &fKey2)
{
	return (fKey1 > fKey2);
}

// const char *
bool CompareFunction::AreEqual(const char *pszKey1, const char *pszKey2)
{
	return (pszKey1 && pszKey2 && !strcmp(pszKey1, pszKey2));
}

bool CompareFunction::IsGreater(const char *pszKey1, const char *pszKey2)
{
	return (pszKey1 && pszKey2 && strcmp(pszKey1, pszKey2) > 0);
}

// String
bool CompareFunction::AreEqual(const String &sKey1, const String &sKey2)
{
	return (sKey1 == sKey2);
}

bool CompareFunction::IsGreater(const String &sKey1, const String &sKey2)
{
	return (sKey1 > sKey2);
}


//[-------------------------------------------------------]
//[ Public static GrowFunction functions                  ]
//[-------------------------------------------------------]
uint32 GrowFunction::GetNewSize(uint32 nCurrentSize, uint32 nFreeSlots)
{
	uint32 nNewSize = nCurrentSize;
	while (nFreeSlots <= (nNewSize/10)) {
		// Get a prime number greater than the current size - to keep things simple and
		// fast we will do this by hand for some prime numbers :)
		if (nNewSize < 1297) {
			if (nNewSize < 199) {
				if (nNewSize < 109) {
					nNewSize = (nNewSize < 41) ? 41 : 109;
				} else {
					nNewSize = 199;
				}
			} else {
				if (nNewSize < 797) {
					nNewSize = (nNewSize < 397) ? 397 : 797;
				} else {
					nNewSize = 1297;
				}
			}
		} else {
			if (nNewSize < 3467) {
				if (nNewSize < 2467) {
					nNewSize = (nNewSize < 1699) ? 1699 : 2467;
				} else {
					nNewSize = 3467;
				}
			} else {
				if (nNewSize < 7559) {
					nNewSize = (nNewSize < 5231) ? 5231 : 7559;
				} else {
					nNewSize = (nNewSize < 9923) ? 9923 : nNewSize*2; // Fallback
				}
			}
		}

		// Because the array grows, more slots will be available now
		nFreeSlots = nFreeSlots + (nNewSize-nCurrentSize);
	}
	return nNewSize;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}
