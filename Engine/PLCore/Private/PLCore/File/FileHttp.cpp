/*********************************************************\
 *  File: FileHttp.cpp                                   *
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
#include "PLCore/File/FileSearchHttp.h"
#include "PLCore/File/FileHttp.h"


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
FileHttp::FileHttp(const Url &cUrl, const FileAccess *pAccess) : FileImpl(cUrl, pAccess),
	m_nAccess(0)
{
}

/**
*  @brief
*    Destructor
*/
FileHttp::~FileHttp()
{
	// Close file
	Close();
}


//[-------------------------------------------------------]
//[ Private virtual FileImpl functions                    ]
//[-------------------------------------------------------]
bool FileHttp::Exists() const
{
	// Check HTTP-connection
	return m_cHttp.IsOpen();
}

bool FileHttp::IsFile() const
{
	// Check HTTP-connection
	return m_cHttp.IsOpen();
}

bool FileHttp::IsDirectory() const
{
	// Not implemented
	return false;
}

bool FileHttp::CopyTo(const String &sDest, bool bOverwrite) const
{
	// Not implemented
	return false;
}

bool FileHttp::MoveTo(const String &sDest)
{
	// Not implemented
	return false;
}

bool FileHttp::Rename(const String &sName)
{
	// Not implemented
	return false;
}

bool FileHttp::CreateNewFile(bool bAlways)
{
	// Not implemented
	return false;
}

bool FileHttp::CreateNewDirectory()
{
	// Not implemented
	return false;
}

bool FileHttp::Delete()
{
	// Not implemented
	return false;
}

bool FileHttp::DeleteDirectory()
{
	// Not implemented
	return false;
}

void FileHttp::Close()
{
	// Close HTTP-connection
	m_cHttp.Close();
}

bool FileHttp::Open(uint32 nAccess, String::EFormat nStringFormat)
{
	// Check flags: FileRead must be set, while FileWrite, FileAppend and FileCreate are not allowed!
	if ((nAccess & File::FileRead) && !(nAccess & File::FileWrite) && !(nAccess & File::FileAppend) && !(nAccess & File::FileCreate)) {
		// Save access modes
		m_nAccess = nAccess;

		// Ignore the "nStringFormat"-parameter, it's not supported

		// Close first
		if (m_cHttp.IsOpen())
			Close();

		// Open HTTP handle
		String sUsername, sPassword;
		const String sDomain = m_cUrl.GetProtocol() + m_cUrl.GetFirstPath();
		if (m_pAccess && m_pAccess->HasEntry(sDomain)) {
			const FileAccess::Entry &cHttpAccess = m_pAccess->GetEntry(sDomain);
			sUsername = cHttpAccess.GetUsername();
			sPassword = cHttpAccess.GetPassword();
		}
		return m_cHttp.Open(m_cUrl.GetUrl(), sUsername, sPassword);
	}

	// Error!
	return false;
}

bool FileHttp::IsOpen() const
{
	// Check whether the file is open
	return m_cHttp.IsOpen();
}

bool FileHttp::IsReadable() const
{
	// Check whether the file is readable
	return (m_cHttp.IsOpen() && (m_nAccess & File::FileRead));
}

bool FileHttp::IsWritable() const
{
	// Not implemented
	return false;
}

String::EFormat FileHttp::GetStringFormat() const
{
	// Default is ASCII
	return String::ASCII;
}

bool FileHttp::IsEof() const
{
	// Check end of file
	return (!m_cHttp.IsOpen() || m_cHttp.IsEof());
}

int FileHttp::GetC()
{
	// Check stream
	if (m_cHttp.IsOpen()) {
		// Read character
		uint8 c;
		if (m_cHttp.Read(&c, 1, 1) == 1) {
			// Text mode: \r\n -> \n
			if ((m_nAccess & File::FileText) && c == 13) {
				if (m_cHttp.Read(&c, 1, 1) != 1)
					return -1; // Error!
			}
			return static_cast<int>(c);
		}
	}

	// Return error
	return -1;
}

bool FileHttp::PutC(int nChar)
{
	// Not implemented
	return false;
}

String FileHttp::GetS()
{
	// Check parameters
	if (m_cHttp.IsOpen()) {
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

int FileHttp::PutS(const String &sString)
{
	// Not implemented
	return -1;
}

uint32 FileHttp::Read(void *pBuffer, uint32 nSize, uint32 nCount)
{
	// Read buffer
	return m_cHttp.IsOpen() ? m_cHttp.Read(pBuffer, nSize, nCount) : 0;
}

uint32 FileHttp::Write(const void *pBuffer, uint32 nSize, uint32 nCount)
{
	// Not implemented
	return 0;
}

bool FileHttp::Flush()
{
	// Not implemented
	return false;
}

bool FileHttp::Seek(int32 nOffset, uint32 nLocation)
{
	// Seek
	return m_cHttp.IsOpen() ? m_cHttp.Seek(nOffset, static_cast<File::ESeek>(nLocation)) : false;
}

int32 FileHttp::Tell() const
{
	// Get file position
	return m_cHttp.IsOpen() ? m_cHttp.Tell() : -1;
}

uint32 FileHttp::GetSize() const
{
	// Return size
	return m_cHttp.IsOpen() ? m_cHttp.GetContentLength() : 0;
}

FileSearchImpl *FileHttp::CreateSearch()
{
	// Create a file searcher
	return new FileSearchHttp(m_pAccess);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
