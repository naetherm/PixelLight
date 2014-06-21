/*********************************************************\
 *  File: File.cpp                                       *
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
#include "PLCore/Core/MemoryManager.h"
#include "PLCore/File/File.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
File File::StandardInput (stdin,  File::FileRead);
File File::StandardOutput(stdout, File::FileWrite);
File File::StandardError (stderr, File::FileWrite);


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Translates a given C file mode into access flags
*/
uint32 File::CFileModeToAccessFlags(const String &sMode)
{
	uint32 nAccess = 0;

	// Read, write and file must exist
	if (sMode.IsSubstring("r+")) {
		nAccess |= File::FileRead;		// File can be read
		nAccess |= File::FileWrite;		// File can be written

	// Read, write and create new empty file (content of previous file is destroyed)
	} else if (sMode.IsSubstring("w+")) {
		nAccess |= File::FileRead;		// File can be read
		nAccess |= File::FileWrite;		// File can be written
		nAccess |= File::FileCreate;	// File will be created

	// Read and append
	} else if (sMode.IsSubstring("a+")) {
		nAccess |= File::FileRead;		// File can be read
		nAccess |= File::FileAppend;	// File will be appended

	// Read and file must exist
	} else if (sMode.IsSubstring("r")) {
		nAccess |= File::FileRead;		// File can be read

	// Write and create new empty file (content of previous file is destroyed)
	} else if (sMode.IsSubstring("w")) {
		nAccess |= File::FileWrite;		// File can be written
		nAccess |= File::FileCreate;	// File will be created

	// Append
	} else if (sMode.IsSubstring("a")) {
		nAccess |= File::FileAppend;	// File will be appended
	}

	// Text mode
	if (sMode.IsSubstring("t"))
		nAccess |= File::FileText;		// File will be opened in text mode

	// Check for "b" not required, this is default if text mode is not set

	// Done, return the result
	return nAccess;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
File::File(uint8 *pnData, uint32 nNumOfBytes, bool bCopy, const String &sUrl) :
	m_pMemBuf(nullptr),
	m_nMemBufSize(0),
	m_nMemBufPos(0),
	m_nMemBufAccess(FileRead)
{
	// File data given?
	if (pnData && nNumOfBytes) {
		// Copy the file data?
		if (bCopy) {
			m_pMemBuf = new uint8[nNumOfBytes];
			MemoryManager::Copy(m_pMemBuf, pnData, nNumOfBytes);
		} else {
			m_pMemBuf        = pnData;
			m_nMemBufAccess |= FileMemBufShared;
		}

		// Set the buffer size in bytes
		m_nMemBufSize = nNumOfBytes;
	}

	// Set URL
	m_cNullUrl = sUrl;
}

/**
*  @brief
*    Open the file for reading and/or writing
*/
bool File::Open(uint32 nAccess, String::EFormat nStringFormat)
{
	// Memory buffered file?
	if (m_pMemBuf)
		MemBufClose();

	// Open file
	if (nAccess & File::FileMemBuf)
		return MemBufOpen(nAccess, nStringFormat);
	else if (m_pFileImpl)
		return m_pFileImpl->Open(nAccess, nStringFormat);
	else
		return false; // Error!
}

/**
*  @brief
*    Returns the complete content of the file as string
*/
String File::GetContentAsString()
{
	// Get the file size
	const uint32 nFileSize = GetSize();
	if (nFileSize) {
		// Check string encoding format
		switch (GetStringFormat()) {
			case String::ASCII:
			{
				// Load in the data -> We also take care of the terminating zero (\0)
				char *pData = new char[nFileSize + 1];
				Read(pData, nFileSize, 1);
				pData[nFileSize] = '\0';

				// The string class takes over the control of the memory
				return String(pData, false, nFileSize);
			}

			case String::Unicode:
			{
				// It's not recommended to use Unicode by because internally wchar_t is used and this data type has not
				// the same size on every platform (use ASCII or UTF8 instead)... but this decision is up to the user...

				// Load in the data -> We also take care of the terminating zero (\0)
				const uint32 nNumOfCharacters = nFileSize/sizeof(wchar_t);
				wchar_t *pData = new wchar_t[nNumOfCharacters + 1];
				Read(pData, nNumOfCharacters*sizeof(wchar_t), 1);
				pData[nNumOfCharacters] = L'\0';

				// The string class takes over the control of the memory
				return String(pData, false, nFileSize);
			}

			case String::UTF8:
			{
				// Load in the data
				char *pData = new char[nFileSize];
				Read(pData, nFileSize, 1);

				// Sets the character string as UTF8
				const String sString = String::FromUTF8(pData, -1, nFileSize);

				// Cleanup
				delete [] pData;

				// Done
				return sString;
			}
		}
	}

	// Error!
	return "";
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
File::File(const File &cSource)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
File &File::operator =(const File &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}

/**
*  @brief
*    Open the file (for memory buffered files)
*/
bool File::MemBufOpen(uint32 nAccess, String::EFormat nStringFormat)
{
	// [TODO] Support for "nStringFormat"-parameter

	// Check flags
	if ((nAccess & FileRead) && !(nAccess & FileWrite) && !(nAccess & FileAppend) && !(nAccess & FileCreate)) {
		// Just to be sure ...
		if (m_pMemBuf)
			MemBufClose();

		// Open file
		if (m_pFileImpl->Open(nAccess & ~static_cast<uint32>(FileMemBuf))) {
			// Get file size
			bool bSuccess = false;
			m_nMemBufSize = m_pFileImpl->GetSize();
			if (m_nMemBufSize > 0) {
				// Read file into memory buffer
				m_pMemBuf = new uint8[m_nMemBufSize];
				bSuccess = (m_pFileImpl->Read(m_pMemBuf, 1, m_nMemBufSize) == m_nMemBufSize);
			}

			// Close file
			m_pFileImpl->Close();

			// Check success
			if (bSuccess) {
				// Success!
				m_nMemBufPos    = 0;
				m_nMemBufAccess = nAccess;
				return true;
			} else {
				// Error!
				MemBufClose();
			}
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Close the file (for memory buffered files)
*/
void File::MemBufClose()
{
	// Destroy memory buffer
	if (m_pMemBuf) {
		if (!(m_nMemBufAccess & FileMemBufShared))
			delete [] m_pMemBuf;
		m_pMemBuf		= nullptr;
		m_nMemBufSize	= 0;
		m_nMemBufPos	= 0;
		m_nMemBufAccess	= 0;
	}
}

/**
*  @brief
*    Reads a character (for memory buffered files)
*/
int File::MemBufGetC()
{
	// Check memory buffer
	if (m_pMemBuf) {
		// Get next character
		if (m_nMemBufPos < m_nMemBufSize) {
			uint8 c = m_pMemBuf[m_nMemBufPos];
			m_nMemBufPos++;

			// Text mode: \r\n -> \n
			if ((m_nMemBufAccess & File::FileText) && c == 13) {
				if (m_nMemBufPos < m_nMemBufSize) {
					c = m_pMemBuf[m_nMemBufPos];
					m_nMemBufPos++;
				} else {
					return -1;
				}
			}
			return static_cast<int>(c);
		}
	}

	// Return error
	return -1;
}

/**
*  @brief
*    Reads a string (for memory buffered files)
*/
String File::MemBufGetS()
{
	// Check memory buffer
	if (m_pMemBuf) {
		// Read string
		char szString[2048];
		char *pszPos = szString;
		int   nChar = MemBufGetC();
		while (nChar >= 0 && nChar != 10) {
			// Concatenate char to string
			*pszPos = static_cast<char>(nChar);
			pszPos++;

			// Read next character
			nChar = MemBufGetC();
		}

		// Terminate string
		if (nChar == 10) {
			*pszPos = '\n';
			pszPos++;
		}
		*pszPos = 0;

		// Return string
		if (pszPos > szString || nChar != -1)
			return szString;
	}

	// Error!
	return "";
}

/**
*  @brief
*    Reads the given number of bytes (for memory buffered files)
*/
uint32 File::MemBufRead(void *pBuffer, uint32 nSize, uint32 nCount)
{
	// Check parameters
	if (m_pMemBuf && pBuffer && nSize && nCount) {
		const uint32 nRead = ((nSize*nCount < m_nMemBufSize-m_nMemBufPos) ? nSize*nCount : m_nMemBufSize-m_nMemBufPos);
		if (nRead > 0) {
			MemoryManager::Copy(pBuffer, &m_pMemBuf[m_nMemBufPos], nRead);
			m_nMemBufPos += nRead;
			return nRead / nSize;
		}
	}

	// Error!
	return 0;
}

/**
*  @brief
*    Sets the starting position (for memory buffered files)
*/
bool File::MemBufSeek(int32 nOffset, ESeek nLocation)
{
	// Check memory buffer
	if (m_pMemBuf) {
		// Calculate offset
		int nOfs;
		if (nLocation == File::SeekCurrent)
			nOfs = m_nMemBufPos + nOffset;
		else if (nLocation == File::SeekEnd)
			nOfs = m_nMemBufSize + nOffset;
		else if (nLocation == File::SeekSet)
			nOfs = nOffset;
		else
			return false; // Error!

		// Set new position
		if (nOfs >= 0 && static_cast<uint32>(nOfs) < m_nMemBufSize) {
			m_nMemBufPos = nOfs;

			// Done
			return true;
		}
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
