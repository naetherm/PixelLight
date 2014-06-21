/*********************************************************\
 *  File: FileZip.cpp                                    *
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
#include "PLCore/File/FileAccess.h"
#include "PLCore/File/+Zip/FileSearchZip.h"
#include "PLCore/File/+Zip/FileZip.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FileZip::FileZip(const Url &cUrl, const String &sZipFile, const String &sPathInZip, const FileAccess *pAccess) : FileImpl(cUrl, pAccess),
	m_sZipFile(sZipFile),
	m_sPathInZip(sPathInZip),
	m_nAccess(0)
{
	// Get options from access-information
	String sPassword;
	bool bCaseSensitive = true;
	bool bMemBuf = true;
	if (m_pAccess && m_pAccess->HasEntry(sZipFile)) {
		const FileAccess::Entry &cZipAccess = m_pAccess->GetEntry(sZipFile);
		sPassword = cZipAccess.GetPassword();
		bCaseSensitive = cZipAccess.GetCaseSensitive();
		if (cZipAccess.GetFlagsMask() & File::FileMemBuf)
			bMemBuf = ((cZipAccess.GetFlagsValue() & File::FileMemBuf) != 0);
	}

	// Open ZIP-file
	m_cZipFile.Open(sZipFile, sPassword, bCaseSensitive, bMemBuf, pAccess);

	// Locate entry
	if (sPathInZip.GetLength())
		m_cZipFile.LocateFile(sPathInZip);
}

/**
*  @brief
*    Destructor
*/
FileZip::~FileZip()
{
	// Close file
	Close();
}


//[-------------------------------------------------------]
//[ Private virtual FileImpl functions                    ]
//[-------------------------------------------------------]
bool FileZip::Exists() const
{
	// Return true, if the ZIP-file exists and m_sPathInZip either points to the root ("") or to a valid entry
	return (m_cZipFile.IsOpen() && (m_sPathInZip.GetLength() == 0 || m_cZipFile.GetCurrentFile().m_bValid));
}

bool FileZip::IsFile() const
{
	// Check current entry
	if (m_cZipFile.IsOpen()) {
		if (m_sPathInZip.GetLength() == 0) {
			// Root directory
			return false;
		} else if (m_cZipFile.GetCurrentFile().m_bValid) {
			// Valid entry, check "/" at the end
			return (m_cZipFile.GetCurrentFile().m_sFilename.GetSubstring(m_cZipFile.GetCurrentFile().m_sFilename.GetLength()-1) != '/');
		}
	}

	// Error
	return false;
}

bool FileZip::IsDirectory() const
{
	// Check current entry
	if (m_cZipFile.IsOpen()) {
		if (m_sPathInZip.GetLength() == 0) {
			// Root directory
			return true;
		} else if (m_cZipFile.GetCurrentFile().m_bValid) {
			// Valid entry, check "/" at the end
			return (m_cZipFile.GetCurrentFile().m_sFilename.GetSubstring(m_cZipFile.GetCurrentFile().m_sFilename.GetLength()-1) == '/');
		}
	}

	// Error
	return false;
}

bool FileZip::CopyTo(const String &sDest, bool bOverwrite) const
{
	// Not implemented
	return false;
}

bool FileZip::MoveTo(const String &sDest)
{
	// Not implemented
	return false;
}

bool FileZip::Rename(const String &sName)
{
	// Not implemented
	return false;
}

bool FileZip::CreateNewFile(bool bAlways)
{
	// Not implemented
	return false;
}

bool FileZip::CreateNewDirectory()
{
	// Not implemented
	return false;
}

bool FileZip::Delete()
{
	// Not implemented
	return false;
}

bool FileZip::DeleteDirectory()
{
	// Not implemented
	return false;
}

void FileZip::Close()
{
	// Close ZIP-file
	m_cZipFile.CloseFile();
}

bool FileZip::Open(uint32 nAccess, String::EFormat nStringFormat)
{
	// Check selected file
	if (IsFile()) {
		// Check flags: FileRead must be set, while FileWrite, FileAppend and FileCreate are not allowed!
		if ((nAccess & File::FileRead) && !(nAccess & File::FileWrite) && !(nAccess & File::FileAppend) && !(nAccess & File::FileCreate)) {
			// Save access modes
			m_nAccess = nAccess;

			// Ignore the "nStringFormat"-parameter, it's not supported

			// Close file first
			Close();

			// Open file
			return m_cZipFile.OpenFile();
		}
	}

	// Error!
	return false;
}

bool FileZip::IsOpen() const
{
	// Check whether the file is open
	return m_cZipFile.IsFileOpen();
}

bool FileZip::IsReadable() const
{
	// Check whether the file is readable
	return (m_cZipFile.IsFileOpen() && (m_nAccess & File::FileRead));
}

bool FileZip::IsWritable() const
{
	// Not implemented
	return false;
}

String::EFormat FileZip::GetStringFormat() const
{
	// Default is ASCII
	return String::ASCII;
}

bool FileZip::IsEof() const
{
	// Check end of file
	return (!m_cZipFile.IsFileOpen() || m_cZipFile.IsEof());
}

int FileZip::GetC()
{
	// Check stream
	if (m_cZipFile.IsFileOpen()) {
		// Read character
		uint8 c;
		if (m_cZipFile.Read(&c, 1, 1) == 1) {
			// Text mode: \r\n -> \n
			if ((m_nAccess & File::FileText) && c == 13) {
				if (m_cZipFile.Read(&c, 1, 1) != 1)
					return -1;
			}
			return static_cast<int>(c);
		}
	}

	// Return error
	return -1;
}

bool FileZip::PutC(int nChar)
{
	// Not implemented
	return false;
}

String FileZip::GetS()
{
	// Check parameters
	if (m_cZipFile.IsFileOpen()) {
		// Read string
		char szString[2048];
		char *pszPos = szString;
		int   nChar = GetC();
		while (nChar >= 0 && nChar != 10) {
			// Concatenate char to string
			*pszPos = static_cast<char>(nChar);
			pszPos++;

			// Read next character
			nChar = GetC();
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

int FileZip::PutS(const String &sString)
{
	// Not implemented
	return -1;
}

uint32 FileZip::Read(void *pBuffer, uint32 nSize, uint32 nCount)
{
	// Read buffer
	return (m_cZipFile.IsFileOpen() ? m_cZipFile.Read(pBuffer, nSize, nCount) : 0);
}

uint32 FileZip::Write(const void *pBuffer, uint32 nSize, uint32 nCount)
{
	// Not implemented
	return 0;
}

bool FileZip::Flush()
{
	// Not implemented
	return false;
}

bool FileZip::Seek(int32 nOffset, uint32 nLocation)
{
	// Seek
	return m_cZipFile.IsFileOpen() ? m_cZipFile.Seek(nOffset, static_cast<File::ESeek>(nLocation)) : false;
}

int32 FileZip::Tell() const
{
	// Get file position
	return m_cZipFile.IsFileOpen() ? m_cZipFile.Tell() : -1;
}

uint32 FileZip::GetSize() const
{
	// Return size
	return m_cZipFile.IsFileOpen() ? m_cZipFile.GetCurrentFile().m_nUncompressedSize : 0;
}

FileSearchImpl *FileZip::CreateSearch()
{
	// Create a file searcher
	return new FileSearchZip(m_sZipFile, m_sPathInZip + '/', m_pAccess);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
