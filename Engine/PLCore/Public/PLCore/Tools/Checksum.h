/*********************************************************\
 *  File: Checksum.h                                     *
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


#ifndef __PLCORE_CHECKSUM_H__
#define __PLCORE_CHECKSUM_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/String.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class File;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract checksum ("digital fingerprint") base class
*
*  @remarks
*    Normally a checksum is used to detect errors after transmission or storage. Before transmission
*    or storage of data, a checksum of the data is calculated and send/saved. Afterwards the received
*    data is verified by using the checksum to confirm that no changes occurred on transit.
*
*  @note
*    - Each time a checksum is calculated, an instance of the class is created and destroyed at the end
*/
class Checksum {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
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
		*    The checksum of the given buffer, empty on error (maybe empty buffer?)
		*/
		PLCORE_API String Get(const uint8 *pnBuffer, uint32 nNumOfBytes);

		/**
		*  @brief
		*    Returns the checksum of a given string
		*
		*  @param[in] sString
		*    String to create the checksum from, the original internal format is used -
		*    if it's a Unicode string, Unicode will be used to get the checksum
		*
		*  @return
		*    The checksum of the given string, empty on error (maybe unknown class?)
		*
		*  @note
		*     - sString: "My text" != L"My text" (ASCII != Unicode even if the text itself is the same!)
		*/
		PLCORE_API String Get(const String &sString);

		/**
		*  @brief
		*    Returns the checksum of a given file
		*
		*  @param[in] cFile
		*    File to create the checksum from (MUST be opened and readable!)
		*
		*  @return
		*    The checksum of the given file, empty string on error
		*    (maybe unknown class or the given file is invalid?)
		*
		*  @note
		*    - The current file offset is not changed
		*/
		PLCORE_API String Get(File &cFile);

		/**
		*  @brief
		*    Returns the checksum of a given file
		*
		*  @param[in] sFilename
		*    Name of the file to create the checksum from
		*
		*  @return
		*    The checksum of the given file, empty on error (maybe unknown class or invalid file?)
		*/
		PLCORE_API String GetFile(const String &sFilename);


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCORE_API Checksum();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~Checksum();


	//[-------------------------------------------------------]
	//[ Protected virtual Checksum functions                  ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Implementation of main checksum algorithm
		*
		*  @param[in] nInput
		*    Input block
		*  @param[in] nInputLen
		*    Length of input block in bytes
		*
		*  @remarks
		*    Computes the partial checksum for 'nInputLen' bytes of data in 'Input'.
		*/
		virtual void Update(const uint8 nInput[], uint32 nInputLen) = 0;

		/**
		*  @brief
		*    Implementation of main checksum algorithm; ends the checksum calculation.
		*
		*  @return
		*    The final hexadecimal checksum result
		*    (without the 0x prefix and with lower case hex characters)
		*
		*  @remarks
		*    Performs the final checksum calculation. 'Update' does most of the work,
		*    this function just finishes the calculation.
		*/
		virtual String Final() = 0;

		/**
		*  @brief
		*    Resets stored checksum
		*/
		virtual void Reset() = 0;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Checksum(const Checksum &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Checksum &operator =(const Checksum &cSource);
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_CHECKSUM_H__
