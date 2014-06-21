/*********************************************************\
 *  File: ChecksumCRC32.h                                *
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


#ifndef __PLCORE_CHECKSUMCRC32_H__
#define __PLCORE_CHECKSUMCRC32_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Container/Array.h"
#include "PLCore/Tools/Checksum.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    CRC32 (cyclic redundancy check) checksum
*
*  @remarks
*    This implementation is using the official polynomial (0x04C11DB7) used by CRC32 in
*    PKZip, WinZip and Ethernet. Often times the polynomial shown reversed as 0xEDB88320.
*
*  @note
*    - CRC32 produces a 32-bit/4-byte hash
*/
class ChecksumCRC32 : public Checksum {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API ChecksumCRC32();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~ChecksumCRC32();

		/**
		*  @brief
		*    Returns the current CRC32 checksum
		*
		*  @return
		*    Current CRC32 checksum
		*/
		PLCORE_API uint32 GetChecksum() const;

		/**
		*  @brief
		*    Returns the checksum of a given buffer
		*
		*  @param[in] pnBuffer
		*    Buffer to create the checksum from (MUST be valid!)
		*  @param[in] nNumOfBytes
		*    Number of bytes of the given buffer (MUST be valid!)
		*
		*  @return
		*    The checksum of the given buffer
		*/
		PLCORE_API uint32 GetChecksum(const uint8 *pnBuffer, uint32 nNumOfBytes);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Initializes the CRC32 table
		*/
		void Init();

		/**
		*  @brief
		*    Reflection part of the the CRC32 table initialization
		*
		*  @param[in] nReflection
		*    Reflection value
		*  @param[in] nCharacter
		*    Character to reflect
		*
		*  @return
		*    The reflected character
		*
		*  @remarks
		*    Reflection is a requirement for the official CRC-32 standard.
		*/
		uint32 Reflect(uint32 nReflection, char nCharacter) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		static Array<uint32> m_lstCRC32Table;	/**< Static CRC32 table */
		uint32				 m_nCRC32;			/**< Current CRC32 checksum */


	//[-------------------------------------------------------]
	//[ Private virtual Checksum functions                    ]
	//[-------------------------------------------------------]
	private:
		virtual void Update(const uint8 nInput[], uint32 nInputLen) override;
		virtual String Final() override;
		virtual void Reset() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_CHECKSUMCRC32_H__

