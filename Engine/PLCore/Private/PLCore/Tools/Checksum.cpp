/*********************************************************\
 *  File: Checksum.cpp                                   *
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
#include "PLCore/File/File.h"
#include "PLCore/Tools/Checksum.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the checksum of a given buffer
*/
String Checksum::Get(const uint8 *pnBuffer, uint32 nNumOfBytes)
{
	// Clear stored checksums
	Reset();

	// Check buffer size
	if (nNumOfBytes) {
		// Get the checksum
		Update(pnBuffer, nNumOfBytes);
		String sChecksum = Final();

		// Return the checksum
		return sChecksum;
	}

	// Error!
	return "";
}

/**
*  @brief
*    Returns the checksum of a given string
*/
String Checksum::Get(const String &sString)
{
	// Clear stored checksums
	Reset();

	// Get the checksum
	if (sString.GetFormat() == String::Unicode)
		Update(reinterpret_cast<const uint8*>(sString.GetUnicode()), sString.GetNumOfBytes(String::Unicode));
	else
		Update(reinterpret_cast<const uint8*>(sString.GetASCII()),   sString.GetNumOfBytes(String::ASCII));
	String sChecksum = Final();

	// Return the checksum
	return sChecksum;
}

/**
*  @brief
*    Returns the checksum of a given file
*/
String Checksum::Get(File &cFile)
{
	// First at all, check the given file
	if (cFile.IsReadable()) {
		// Backup the current file position
		int32 nFilePosition = cFile.Tell();

		// Checksum the file in blocks of 1024 bytes
		const uint32 nBufferSize = 1024;							// Checksum the file in blocks of 1024 bytes
		uint8		 nBuffer[nBufferSize];							// Buffer for data read from the file
		uint32		 nLength = cFile.Read(nBuffer, 1, nBufferSize);	// Number of bytes read from the file

		// Clear stored checksums
		Reset();

		while (nLength > 0) {
			// Update the checksum
			Update(nBuffer, nLength);

			// Update the buffer
			nLength = cFile.Read(nBuffer, 1, nBufferSize);
		}
		String sChecksum = Final();

		// Reset the current file position
		cFile.Seek(nFilePosition);

		// Return the checksum
		return sChecksum;
	}

	// Error!
	return "";
}

/**
*  @brief
*    Returns the checksum of a given file
*/
String Checksum::GetFile(const String &sFilename)
{
	// Check parameters
	if (sFilename.GetLength()) {
		// Open the file
		File cFile(sFilename);
		if (cFile.Exists() && cFile.IsFile() && cFile.Open(File::FileRead)) {
			// Get the checksum
			const String sChecksum = Get(cFile);

			// Close the file
			cFile.Close();

			// Done
			return sChecksum;
		}
	}

	// Error!
	return "";
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
Checksum::Checksum()
{
}

/**
*  @brief
*    Destructor
*/
Checksum::~Checksum()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Checksum::Checksum(const Checksum &cSource)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
Checksum &Checksum::operator =(const Checksum &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
