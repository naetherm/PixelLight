/*********************************************************\
 *  File: ChecksumMD5.cpp                                *
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
#include "PLCore/Tools/ChecksumMD5.h"


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
ChecksumMD5::ChecksumMD5()
{
	// Zero members
	MemoryManager::Set(m_nBuffer, 0, 64);
	m_nCount[0] = m_nCount[1] = 0;

	// Load magic state initialization constants
	m_nMD5[0] = 0x67452301;
	m_nMD5[1] = 0xefcdab89;
	m_nMD5[2] = 0x98badcfe;
	m_nMD5[3] = 0x10325476;
}

/**
*  @brief
*    Destructor
*/
ChecksumMD5::~ChecksumMD5()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Rotates the bits in a 32 bit uint32 left by a specified amount
*/
inline uint32 ChecksumMD5::RotateLeft(uint32 x, int n) const
{
	// Rotate and return x
	return (x << n) | (x >> (32-n));
}

/**
*  @brief
*    Implementation of basic MD5 transformation algorithm
*/
inline void ChecksumMD5::FF(uint32 &nA, uint32 nB, uint32 nC, uint32 nD, uint32 nX, uint32 nS, uint32 nT) const
{
	uint32 F = (nB & nC) | (~nB & nD);
	nA += F + nX + nT;
	nA = RotateLeft(nA, nS);
	nA += nB;
}

/**
*  @brief
*    Implementation of basic MD5 transformation algorithm
*/
inline void ChecksumMD5::GG(uint32 &nA, uint32 nB, uint32 nC, uint32 nD, uint32 nX, uint32 nS, uint32 nT) const
{
	uint32 G = (nB & nD) | (nC & ~nD);
	nA += G + nX + nT;
	nA = RotateLeft(nA, nS);
	nA += nB;
}

/**
*  @brief
*    Implementation of basic MD5 transformation algorithm
*/
inline void ChecksumMD5::HH(uint32 &nA, uint32 nB, uint32 nC, uint32 nD, uint32 nX, uint32 nS, uint32 nT) const
{
	uint32 H = (nB ^ nC ^ nD);
	nA += H + nX + nT;
	nA = RotateLeft(nA, nS);
	nA += nB;
}

/**
*  @brief
*    Implementation of basic MD5 transformation algorithm
*/
inline void ChecksumMD5::II(uint32 &nA, uint32 nB, uint32 nC, uint32 nD, uint32 nX, uint32 nS, uint32 nT) const
{
	uint32 I = (nC ^ (nB | ~nD));
	nA += I + nX + nT;
	nA = RotateLeft(nA, nS);
	nA += nB;
}

/**
*  @brief
*    Transfers the data in an 8 bit array to a 32 bit array
*/
inline void ChecksumMD5::ByteToDWord(uint32 nOutput[], const uint8 nInput[], uint32 nLength) const
{
	// Transfer the data by shifting and copying
	for (uint32 i=0, j=0; j<nLength; i++, j+=4) {
		nOutput[i] = static_cast<uint32>(nInput[j])		    |
					 static_cast<uint32>(nInput[j+1]) << 8  |
					 static_cast<uint32>(nInput[j+2]) << 16 |
					 static_cast<uint32>(nInput[j+3]) << 24;
	}
}

/**
*  @brief
*    Transfers the data in an 32 bit array to a 8 bit array
*/
inline void ChecksumMD5::DWordToByte(uint8 nOutput[], uint32 nInput[], uint32 nLength) const
{
	// Transfer the data by shifting and copying
	for (uint32 i=0, j=0; j<nLength; i++, j+=4) {
		nOutput[j]   = static_cast<uint8>( nInput[i]        & 0xff);
		nOutput[j+1] = static_cast<uint8>((nInput[i] >> 8)  & 0xff);
		nOutput[j+2] = static_cast<uint8>((nInput[i] >> 16) & 0xff);
		nOutput[j+3] = static_cast<uint8>((nInput[i] >> 24) & 0xff);
	}
}

/**
*  @brief
*    MD5 basic transformation algorithm; transforms 'm_nMD5'
*/
void ChecksumMD5::Transform(const uint8 nBlock[64])
{
	// Constants for transform routine
	static const uint32 MD5_S11 =  7;
	static const uint32 MD5_S12 = 12;
	static const uint32 MD5_S13 = 17;
	static const uint32 MD5_S14 = 22;
	static const uint32 MD5_S21 =  5;
	static const uint32 MD5_S22 =  9;
	static const uint32 MD5_S23 = 14;
	static const uint32 MD5_S24 = 20;
	static const uint32 MD5_S31 =  4;
	static const uint32 MD5_S32 = 11;
	static const uint32 MD5_S33 = 16;
	static const uint32 MD5_S34 = 23;
	static const uint32 MD5_S41 =  6;
	static const uint32 MD5_S42 = 10;
	static const uint32 MD5_S43 = 15;
	static const uint32 MD5_S44 = 21;
	// Transformation constants - round 1
	static const uint32 MD5_T01 = 0xd76aa478;
	static const uint32 MD5_T02 = 0xe8c7b756;
	static const uint32 MD5_T03 = 0x242070db;
	static const uint32 MD5_T04 = 0xc1bdceee;
	static const uint32 MD5_T05 = 0xf57c0faf;
	static const uint32 MD5_T06 = 0x4787c62a;
	static const uint32 MD5_T07 = 0xa8304613;
	static const uint32 MD5_T08 = 0xfd469501;
	static const uint32 MD5_T09 = 0x698098d8;
	static const uint32 MD5_T10 = 0x8b44f7af;
	static const uint32 MD5_T11 = 0xffff5bb1;
	static const uint32 MD5_T12 = 0x895cd7be;
	static const uint32 MD5_T13 = 0x6b901122;
	static const uint32 MD5_T14 = 0xfd987193;
	static const uint32 MD5_T15 = 0xa679438e;
	static const uint32 MD5_T16 = 0x49b40821;
	// Transformation constants - round 2
	static const uint32 MD5_T17 = 0xf61e2562;
	static const uint32 MD5_T18 = 0xc040b340;
	static const uint32 MD5_T19 = 0x265e5a51;
	static const uint32 MD5_T20 = 0xe9b6c7aa;
	static const uint32 MD5_T21 = 0xd62f105d;
	static const uint32 MD5_T22 = 0x02441453;
	static const uint32 MD5_T23 = 0xd8a1e681;
	static const uint32 MD5_T24 = 0xe7d3fbc8;
	static const uint32 MD5_T25 = 0x21e1cde6;
	static const uint32 MD5_T26 = 0xc33707d6;
	static const uint32 MD5_T27 = 0xf4d50d87;
	static const uint32 MD5_T28 = 0x455a14ed;
	static const uint32 MD5_T29 = 0xa9e3e905;
	static const uint32 MD5_T30 = 0xfcefa3f8;
	static const uint32 MD5_T31 = 0x676f02d9;
	static const uint32 MD5_T32 = 0x8d2a4c8a;
	// Transformation constants - round 3
	static const uint32 MD5_T33 = 0xfffa3942;
	static const uint32 MD5_T34 = 0x8771f681;
	static const uint32 MD5_T35 = 0x6d9d6122;
	static const uint32 MD5_T36 = 0xfde5380c;
	static const uint32 MD5_T37 = 0xa4beea44;
	static const uint32 MD5_T38 = 0x4bdecfa9;
	static const uint32 MD5_T39 = 0xf6bb4b60;
	static const uint32 MD5_T40 = 0xbebfbc70;
	static const uint32 MD5_T41 = 0x289b7ec6;
	static const uint32 MD5_T42 = 0xeaa127fa;
	static const uint32 MD5_T43 = 0xd4ef3085;
	static const uint32 MD5_T44 = 0x04881d05;
	static const uint32 MD5_T45 = 0xd9d4d039;
	static const uint32 MD5_T46 = 0xe6db99e5;
	static const uint32 MD5_T47 = 0x1fa27cf8;
	static const uint32 MD5_T48 = 0xc4ac5665;
	// Transformation constants - round 4
	static const uint32 MD5_T49 = 0xf4292244;
	static const uint32 MD5_T50 = 0x432aff97;
	static const uint32 MD5_T51 = 0xab9423a7;
	static const uint32 MD5_T52 = 0xfc93a039;
	static const uint32 MD5_T53 = 0x655b59c3;
	static const uint32 MD5_T54 = 0x8f0ccc92;
	static const uint32 MD5_T55 = 0xffeff47d;
	static const uint32 MD5_T56 = 0x85845dd1;
	static const uint32 MD5_T57 = 0x6fa87e4f;
	static const uint32 MD5_T58 = 0xfe2ce6e0;
	static const uint32 MD5_T59 = 0xa3014314;
	static const uint32 MD5_T60 = 0x4e0811a1;
	static const uint32 MD5_T61 = 0xf7537e82;
	static const uint32 MD5_T62 = 0xbd3af235;
	static const uint32 MD5_T63 = 0x2ad7d2bb;
	static const uint32 MD5_T64 = 0xeb86d391;

	// Initialize local data with current checksum
	uint32 a = m_nMD5[0];
	uint32 b = m_nMD5[1];
	uint32 c = m_nMD5[2];
	uint32 d = m_nMD5[3];

	// Copy uint8 from input 'nBlock' to an array of ULONGS 'nX'
	uint32 nX[16];
	ByteToDWord(nX, nBlock, 64);

	// Perform round 1 of the transformation
	FF(a, b, c, d, nX[ 0], MD5_S11, MD5_T01);
	FF(d, a, b, c, nX[ 1], MD5_S12, MD5_T02);
	FF(c, d, a, b, nX[ 2], MD5_S13, MD5_T03);
	FF(b, c, d, a, nX[ 3], MD5_S14, MD5_T04);
	FF(a, b, c, d, nX[ 4], MD5_S11, MD5_T05);
	FF(d, a, b, c, nX[ 5], MD5_S12, MD5_T06);
	FF(c, d, a, b, nX[ 6], MD5_S13, MD5_T07);
	FF(b, c, d, a, nX[ 7], MD5_S14, MD5_T08);
	FF(a, b, c, d, nX[ 8], MD5_S11, MD5_T09);
	FF(d, a, b, c, nX[ 9], MD5_S12, MD5_T10);
	FF(c, d, a, b, nX[10], MD5_S13, MD5_T11);
	FF(b, c, d, a, nX[11], MD5_S14, MD5_T12);
	FF(a, b, c, d, nX[12], MD5_S11, MD5_T13);
	FF(d, a, b, c, nX[13], MD5_S12, MD5_T14);
	FF(c, d, a, b, nX[14], MD5_S13, MD5_T15);
	FF(b, c, d, a, nX[15], MD5_S14, MD5_T16);

	// Perform round 2 of the transformation
	GG(a, b, c, d, nX[ 1], MD5_S21, MD5_T17);
	GG(d, a, b, c, nX[ 6], MD5_S22, MD5_T18);
	GG(c, d, a, b, nX[11], MD5_S23, MD5_T19);
	GG(b, c, d, a, nX[ 0], MD5_S24, MD5_T20);
	GG(a, b, c, d, nX[ 5], MD5_S21, MD5_T21);
	GG(d, a, b, c, nX[10], MD5_S22, MD5_T22);
	GG(c, d, a, b, nX[15], MD5_S23, MD5_T23);
	GG(b, c, d, a, nX[ 4], MD5_S24, MD5_T24);
	GG(a, b, c, d, nX[ 9], MD5_S21, MD5_T25);
	GG(d, a, b, c, nX[14], MD5_S22, MD5_T26);
	GG(c, d, a, b, nX[ 3], MD5_S23, MD5_T27);
	GG(b, c, d, a, nX[ 8], MD5_S24, MD5_T28);
	GG(a, b, c, d, nX[13], MD5_S21, MD5_T29);
	GG(d, a, b, c, nX[ 2], MD5_S22, MD5_T30);
	GG(c, d, a, b, nX[ 7], MD5_S23, MD5_T31);
	GG(b, c, d, a, nX[12], MD5_S24, MD5_T32);

	// Perform round 3 of the transformation
	HH(a, b, c, d, nX[ 5], MD5_S31, MD5_T33);
	HH(d, a, b, c, nX[ 8], MD5_S32, MD5_T34);
	HH(c, d, a, b, nX[11], MD5_S33, MD5_T35);
	HH(b, c, d, a, nX[14], MD5_S34, MD5_T36);
	HH(a, b, c, d, nX[ 1], MD5_S31, MD5_T37);
	HH(d, a, b, c, nX[ 4], MD5_S32, MD5_T38);
	HH(c, d, a, b, nX[ 7], MD5_S33, MD5_T39);
	HH(b, c, d, a, nX[10], MD5_S34, MD5_T40);
	HH(a, b, c, d, nX[13], MD5_S31, MD5_T41);
	HH(d, a, b, c, nX[ 0], MD5_S32, MD5_T42);
	HH(c, d, a, b, nX[ 3], MD5_S33, MD5_T43);
	HH(b, c, d, a, nX[ 6], MD5_S34, MD5_T44);
	HH(a, b, c, d, nX[ 9], MD5_S31, MD5_T45);
	HH(d, a, b, c, nX[12], MD5_S32, MD5_T46);
	HH(c, d, a, b, nX[15], MD5_S33, MD5_T47);
	HH(b, c, d, a, nX[ 2], MD5_S34, MD5_T48);

	// Perform round 4 of the transformation
	II(a, b, c, d, nX[ 0], MD5_S41, MD5_T49);
	II(d, a, b, c, nX[ 7], MD5_S42, MD5_T50);
	II(c, d, a, b, nX[14], MD5_S43, MD5_T51);
	II(b, c, d, a, nX[ 5], MD5_S44, MD5_T52);
	II(a, b, c, d, nX[12], MD5_S41, MD5_T53);
	II(d, a, b, c, nX[ 3], MD5_S42, MD5_T54);
	II(c, d, a, b, nX[10], MD5_S43, MD5_T55);
	II(b, c, d, a, nX[ 1], MD5_S44, MD5_T56);
	II(a, b, c, d, nX[ 8], MD5_S41, MD5_T57);
	II(d, a, b, c, nX[15], MD5_S42, MD5_T58);
	II(c, d, a, b, nX[ 6], MD5_S43, MD5_T59);
	II(b, c, d, a, nX[13], MD5_S44, MD5_T60);
	II(a, b, c, d, nX[ 4], MD5_S41, MD5_T61);
	II(d, a, b, c, nX[11], MD5_S42, MD5_T62);
	II(c, d, a, b, nX[ 2], MD5_S43, MD5_T63);
	II(b, c, d, a, nX[ 9], MD5_S44, MD5_T64);

	// Add the transformed values to the current checksum
	m_nMD5[0] += a;
	m_nMD5[1] += b;
	m_nMD5[2] += c;
	m_nMD5[3] += d;
}


//[-------------------------------------------------------]
//[ Private virtual Checksum functions                    ]
//[-------------------------------------------------------]
void ChecksumMD5::Update(const uint8 nInput[], uint32 nInputLen)
{
	// Compute number of bytes mod 64
	uint32 nIndex = static_cast<uint32>((m_nCount[0] >> 3) & 0x3F);

	// Update number of bits
	if ((m_nCount[0] += nInputLen << 3 ) < (nInputLen << 3))
		m_nCount[1]++;
	m_nCount[1] += (nInputLen >> 29);

	// Transform as many times as possible
	uint32 i = 0;
	uint32 nPartLen = 64 - nIndex;
	if (nInputLen >= nPartLen) {
		MemoryManager::Copy(&m_nBuffer[nIndex], nInput, nPartLen);
		Transform(m_nBuffer);
		for (i=nPartLen; i+63<nInputLen; i+=64)
			Transform(&nInput[i]);
		nIndex = 0;
	} else i = 0;

	// Buffer remaining input
	MemoryManager::Copy(&m_nBuffer[nIndex], &nInput[i], nInputLen - i);
}

String ChecksumMD5::Final()
{
	// Null data (except for first uint8) used to finalize the checksum calculation
	static uint8 PADDING[64] = {
		0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	// Save number of bits
	uint8 nBits[8];
	DWordToByte(nBits, m_nCount, 8);

	// Pad out to 56 mod 64
	uint32 nIndex = static_cast<uint32>((m_nCount[0] >> 3) & 0x3f);
	uint32 nPadLen = (nIndex < 56) ? (56 - nIndex) : (120 - nIndex);
	Update(PADDING, nPadLen);

	// Append length (before padding)
	Update(nBits, 8);

	// Store final state in 'nMD5'
	const uint32 nMD5Size = 16;
	uint8 nMD5[nMD5Size];
	DWordToByte(nMD5, m_nMD5, nMD5Size);

	// Convert the hexadecimal checksum to a string
	String sMD5;
	for (uint32 i=0; i<nMD5Size; i++)
		sMD5 += String::Format("%02x", nMD5[i]);
	return sMD5;
}

void ChecksumMD5::Reset()
{
	// Zero members
	MemoryManager::Set(m_nBuffer, 0, 64);
	m_nCount[0] = m_nCount[1] = 0;

	// Load magic state initialization constants
	m_nMD5[0] = 0x67452301;
	m_nMD5[1] = 0xefcdab89;
	m_nMD5[2] = 0x98badcfe;
	m_nMD5[3] = 0x10325476;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
