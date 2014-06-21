/*********************************************************\
 *  File: ChecksumCRC32.cpp                              *
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
#include "PLCore/Tools/ChecksumCRC32.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
Array<uint32> ChecksumCRC32::m_lstCRC32Table;


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ChecksumCRC32::ChecksumCRC32() :
	m_nCRC32(0xFFFFFFFF)
{
}

/**
*  @brief
*    Destructor
*/
ChecksumCRC32::~ChecksumCRC32()
{
}

/**
*  @brief
*    Returns the current CRC32 checksum
*/
uint32 ChecksumCRC32::GetChecksum() const
{
	return m_nCRC32;
}

/**
*  @brief
*    Returns the checksum of a given buffer
*/
uint32 ChecksumCRC32::GetChecksum(const uint8 *pnBuffer, uint32 nNumOfBytes)
{
	// Initialize the CRC32 table?
	if (!m_lstCRC32Table.GetNumOfElements())
		Init();

	// Update current checksum
	m_nCRC32 = 0;
	const uint8 *pInput = pnBuffer;
	for (uint32 i=0; i<nNumOfBytes; i++)
		m_nCRC32 = (m_nCRC32 >> 8) ^ m_lstCRC32Table[(m_nCRC32 & 0xFF) ^ *pInput++];

	// Done
	return m_nCRC32;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Initializes the CRC32 table
*/
void ChecksumCRC32::Init()
{
	uint32 nPolynomial = 0x04C11DB7;

	m_lstCRC32Table.Resize(256);
	for (int i=0; i<256; i++) {
		m_lstCRC32Table[i] = Reflect(i, 8) << 24;
		for (int j=0; j<8; j++)
			m_lstCRC32Table[i] = (m_lstCRC32Table[i] << 1) ^ (m_lstCRC32Table[i] & (1 << 31) ? nPolynomial : 0);
		m_lstCRC32Table[i] = Reflect(m_lstCRC32Table[i], 32);
	}
}

/**
*  @brief
*    Reflection part of the the CRC32 table initialization
*/
uint32 ChecksumCRC32::Reflect(uint32 nReflection, char nCharacter) const
{
	uint32 nValue = 0;

	// Swap bit 0 for bit 7 bit 1 for bit 6, etc.
	for (int i=1; i<(nCharacter+1); i++) {
		if (nReflection & 1)
			nValue |= 1 << (nCharacter - i);
		nReflection >>= 1;
	}

	// Done
	return nValue;
}


//[-------------------------------------------------------]
//[ Private virtual Checksum functions                    ]
//[-------------------------------------------------------]
void ChecksumCRC32::Update(const uint8 nInput[], uint32 nInputLen)
{
	// Initialize the CRC32 table?
	if (!m_lstCRC32Table.GetNumOfElements())
		Init();

	// Update current checksum
	const uint8 *pInput = nInput;
	for (uint32 i=0; i<nInputLen; i++)
		m_nCRC32 = (m_nCRC32 >> 8) ^ m_lstCRC32Table[(m_nCRC32 & 0xFF) ^ *pInput++];
}

String ChecksumCRC32::Final()
{
	// Exclusive OR the result with the beginning value
	return String::Format("%x", m_nCRC32^0xFFFFFFFF);
}

void ChecksumCRC32::Reset()
{
	m_nCRC32=0xFFFFFFFF;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
