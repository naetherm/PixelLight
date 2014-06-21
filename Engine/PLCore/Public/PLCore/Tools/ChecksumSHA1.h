/*********************************************************\
 *  File: ChecksumSHA1.h                                 *
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


#ifndef __PLCORE_CHECKSUMSHA1_H__
#define __PLCORE_CHECKSUMSHA1_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
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
*    SHA-1 (Secure Hash Algorithm) checksum
*
*  @remarks
*    The Secure Hash Algorithm SHA-1 is a cryptographically secure one-way hash algorithm.
*    It was designed by the NIST (National Institute of Standards and Technology), along
*    with the NSA (National Security Agency). SHA-1 is based on the Message Digest MD4
*    algorithm design principles by Ronald L. Rivest of MIT.
*
*    This implementation is basing on the 100% free public domain implementation of the
*    SHA-1 algorithm by Dominik Reichl (http://www.dominik-reichl.de/)
*
*  @note
*    - SHA-1 produces a 160-bit/20-byte hash
*/
class ChecksumSHA1 : public Checksum {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API ChecksumSHA1();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~ChecksumSHA1();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    SHA-1 transformation
		*
		*  @param[in, out] nState
		*    State
		*  @param[in]      nBuffer
		*    Buffer
		*/
		void Transform(uint32 nState[], const uint8 nBuffer[]) const;


	//[-------------------------------------------------------]
	//[ Private structures                                    ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Workspace block
		*/
		typedef union {
			uint8  c[64];
			uint32 l[16];
		} WorkspaceBlock;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32			m_nState[5];
		uint32			m_nCount[2];
		uint8			m_nBuffer[64];
		uint8			m_nWorkspace[64];
		WorkspaceBlock *m_pBlock;			/**< SHA1 pointer to the byte array above (always valid!) */


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


#endif // __PLCORE_CHECKSUMSHA1_H__

