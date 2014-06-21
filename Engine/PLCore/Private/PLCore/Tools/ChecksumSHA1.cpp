/*********************************************************\
 *  File: ChecksumSHA1.cpp                               *
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
#include <stdlib.h> // For '_rotl()'
#include "PLCore/Core/MemoryManager.h"
#include "PLCore/Tools/ChecksumSHA1.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
// You can define the endian mode in your files, without modifying the SHA1
// source files. Just #define SHA1_LITTLE_ENDIAN or #define SHA1_BIG_ENDIAN
// in your files, before including the SHA1.h header file. If you don't
// define anything, the class defaults to little endian.
#if !defined(SHA1_LITTLE_ENDIAN) && !defined(SHA1_BIG_ENDIAN)
	#define SHA1_LITTLE_ENDIAN
#endif

// Rotate x bits to the left
#ifndef ROL32
	#ifdef _MSC_VER
		#define ROL32(_val32, _nBits) _rotl(_val32, _nBits)
	#else
		#define ROL32(_val32, _nBits) (((_val32)<<(_nBits))|((_val32)>>(32-(_nBits))))
	#endif
#endif

#ifdef SHA1_LITTLE_ENDIAN
	#define SHABLK0(i) (m_pBlock->l[i] = \
		(ROL32(m_pBlock->l[i],24) & 0xFF00FF00) | (ROL32(m_pBlock->l[i],8) & 0x00FF00FF))
#else
	#define SHABLK0(i) (m_pBlock->l[i])
#endif

#define SHABLK(i) (m_pBlock->l[i&15] = ROL32(m_pBlock->l[(i+13)&15] ^ m_pBlock->l[(i+8)&15] \
	^ m_pBlock->l[(i+2)&15] ^ m_pBlock->l[i&15],1))

// SHA-1 rounds
#define _R0(v, w, x, y, z, i) { z += ((w&(x^y))^y)+SHABLK0(i)+0x5A827999+ROL32(v,5); w=ROL32(w,30); }
#define _R1(v, w, x, y, z, i) { z += ((w&(x^y))^y)+SHABLK(i)+0x5A827999+ROL32(v,5); w=ROL32(w,30); }
#define _R2(v, w, x, y, z, i) { z += (w^x^y)+SHABLK(i)+0x6ED9EBA1+ROL32(v,5); w=ROL32(w,30); }
#define _R3(v, w, x, y, z, i) { z += (((w|x)&y)|(w&x))+SHABLK(i)+0x8F1BBCDC+ROL32(v,5); w=ROL32(w,30); }
#define _R4(v, w, x, y, z, i) { z += (w^x^y)+SHABLK(i)+0xCA62C1D6+ROL32(v,5); w=ROL32(w,30); }


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ChecksumSHA1::ChecksumSHA1() :
	m_pBlock(reinterpret_cast<WorkspaceBlock*>(m_nWorkspace))
{
	// SHA1 initialization constants
	m_nState[0] = 0x67452301;
	m_nState[1] = 0xEFCDAB89;
	m_nState[2] = 0x98BADCFE;
	m_nState[3] = 0x10325476;
	m_nState[4] = 0xC3D2E1F0;

	m_nCount[0] = 0;
	m_nCount[1] = 0;

	// We do not need to initialize 'm_nBuffer' and 'm_nWorkspace'
}

/**
*  @brief
*    Destructor
*/
ChecksumSHA1::~ChecksumSHA1()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    SHA-1 transformation
*/
void ChecksumSHA1::Transform(uint32 nState[], const uint8 nBuffer[]) const
{
	uint32 a = nState[0], b = nState[1], c = nState[2], d = nState[3], e = nState[4];

	MemoryManager::Copy(m_pBlock, nBuffer, 64);

	// 4 rounds of 20 operations each. Loop unrolled.
	_R0(a,b,c,d,e, 0); _R0(e,a,b,c,d, 1); _R0(d,e,a,b,c, 2); _R0(c,d,e,a,b, 3);
	_R0(b,c,d,e,a, 4); _R0(a,b,c,d,e, 5); _R0(e,a,b,c,d, 6); _R0(d,e,a,b,c, 7);
	_R0(c,d,e,a,b, 8); _R0(b,c,d,e,a, 9); _R0(a,b,c,d,e,10); _R0(e,a,b,c,d,11);
	_R0(d,e,a,b,c,12); _R0(c,d,e,a,b,13); _R0(b,c,d,e,a,14); _R0(a,b,c,d,e,15);
	_R1(e,a,b,c,d,16); _R1(d,e,a,b,c,17); _R1(c,d,e,a,b,18); _R1(b,c,d,e,a,19);
	_R2(a,b,c,d,e,20); _R2(e,a,b,c,d,21); _R2(d,e,a,b,c,22); _R2(c,d,e,a,b,23);
	_R2(b,c,d,e,a,24); _R2(a,b,c,d,e,25); _R2(e,a,b,c,d,26); _R2(d,e,a,b,c,27);
	_R2(c,d,e,a,b,28); _R2(b,c,d,e,a,29); _R2(a,b,c,d,e,30); _R2(e,a,b,c,d,31);
	_R2(d,e,a,b,c,32); _R2(c,d,e,a,b,33); _R2(b,c,d,e,a,34); _R2(a,b,c,d,e,35);
	_R2(e,a,b,c,d,36); _R2(d,e,a,b,c,37); _R2(c,d,e,a,b,38); _R2(b,c,d,e,a,39);
	_R3(a,b,c,d,e,40); _R3(e,a,b,c,d,41); _R3(d,e,a,b,c,42); _R3(c,d,e,a,b,43);
	_R3(b,c,d,e,a,44); _R3(a,b,c,d,e,45); _R3(e,a,b,c,d,46); _R3(d,e,a,b,c,47);
	_R3(c,d,e,a,b,48); _R3(b,c,d,e,a,49); _R3(a,b,c,d,e,50); _R3(e,a,b,c,d,51);
	_R3(d,e,a,b,c,52); _R3(c,d,e,a,b,53); _R3(b,c,d,e,a,54); _R3(a,b,c,d,e,55);
	_R3(e,a,b,c,d,56); _R3(d,e,a,b,c,57); _R3(c,d,e,a,b,58); _R3(b,c,d,e,a,59);
	_R4(a,b,c,d,e,60); _R4(e,a,b,c,d,61); _R4(d,e,a,b,c,62); _R4(c,d,e,a,b,63);
	_R4(b,c,d,e,a,64); _R4(a,b,c,d,e,65); _R4(e,a,b,c,d,66); _R4(d,e,a,b,c,67);
	_R4(c,d,e,a,b,68); _R4(b,c,d,e,a,69); _R4(a,b,c,d,e,70); _R4(e,a,b,c,d,71);
	_R4(d,e,a,b,c,72); _R4(c,d,e,a,b,73); _R4(b,c,d,e,a,74); _R4(a,b,c,d,e,75);
	_R4(e,a,b,c,d,76); _R4(d,e,a,b,c,77); _R4(c,d,e,a,b,78); _R4(b,c,d,e,a,79);

	// Add the working vars back into state
	nState[0] += a;
	nState[1] += b;
	nState[2] += c;
	nState[3] += d;
	nState[4] += e;
}


//[-------------------------------------------------------]
//[ Private virtual Checksum functions                    ]
//[-------------------------------------------------------]
void ChecksumSHA1::Update(const uint8 nInput[], uint32 nInputLen)
{
	uint32 j = (m_nCount[0] >> 3) & 63;

	if ((m_nCount[0] += (nInputLen << 3)) < (nInputLen << 3))
		m_nCount[1]++;

	m_nCount[1] += (nInputLen >> 29);

	uint32 i;
	if ((j + nInputLen) > 63) {
		i = 64 - j;
		MemoryManager::Copy(&m_nBuffer[j], nInput, i);
		Transform(m_nState, m_nBuffer);

		for (; (i+63)<nInputLen; i+=64)
			Transform(m_nState, &nInput[i]);

		j = 0;
	} else {
		i = 0;
	}

	if ((nInputLen - i) != 0)
		MemoryManager::Copy(&m_nBuffer[j], &nInput[i], nInputLen - i);
}

String ChecksumSHA1::Final()
{
	uint8 nFinalCount[8];

	for (int i=0; i<8; i++)
		nFinalCount[i] = static_cast<uint8>((m_nCount[((i >= 4) ? 0 : 1)]
			>> ((3 - (i & 3)) * 8) ) & 255); // Endian independent

	Update(reinterpret_cast<const uint8*>("\200"), 1);

	while ((m_nCount[0] & 504) != 448)
		Update(reinterpret_cast<const uint8*>("\0"), 1);

	Update(nFinalCount, 8); // Cause a SHA1Transform()

	// Convert the hexadecimal checksum to a string
	String sSHA1;
	for (int i=0; i<20; i++)
		sSHA1 += String::Format("%02x", static_cast<uint8>((m_nState[i >> 2] >> ((3 - (i & 3)) * 8)) & 255));
	return sSHA1;
}

void ChecksumSHA1::Reset()
{
	// SHA1 initialization constants
	m_nState[0] = 0x67452301;
	m_nState[1] = 0xEFCDAB89;
	m_nState[2] = 0x98BADCFE;
	m_nState[3] = 0x10325476;
	m_nState[4] = 0xC3D2E1F0;

	m_nCount[0] = 0;
	m_nCount[1] = 0;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
