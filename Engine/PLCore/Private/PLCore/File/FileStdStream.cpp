/*********************************************************\
 *  File: FileStdStream.cpp                              *
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
#if defined(WIN32)
	#include <io.h>
	#include <Fcntl.h>
#elif defined(LINUX)
	#include "PLCore/PLCoreLinuxWrapper.h"
#endif
#include <stdio.h>
#include <string.h>
#include "PLCore/File/File.h"
#include "PLCore/File/FileStdStream.h"


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
FileStdStream::FileStdStream(FILE *pFile, uint32 nAccess) : FileImpl(Url(), nullptr),
	m_bStream(true),
	m_pFile(pFile),
	m_hFile(0),
	m_nAccess(nAccess),
	m_nStringFormat(String::ASCII)
{
}

/**
*  @brief
*    Constructor
*/
FileStdStream::FileStdStream(handle hFile) : FileImpl(Url(), nullptr),
	m_bStream(false),
	m_pFile(nullptr),
	m_hFile(hFile),
	m_nAccess(0),
	m_nStringFormat(String::ASCII)
{
}

/**
*  @brief
*    Destructor
*/
FileStdStream::~FileStdStream()
{
	Close();
}


//[-------------------------------------------------------]
//[ Private virtual FileImpl functions                    ]
//[-------------------------------------------------------]
bool FileStdStream::Exists() const
{
	return true;
}

bool FileStdStream::IsFile() const
{
	return true;
}

bool FileStdStream::IsDirectory() const
{
	return false;
}

bool FileStdStream::CopyTo(const String &sDest, bool bOverwrite) const
{
	// Not implemented - error!
	return false;
}

bool FileStdStream::MoveTo(const String &sDest)
{
	// Not implemented - error!
	return false;
}

bool FileStdStream::Rename(const String &sName)
{
	// Not implemented - error!
	return false;
}

bool FileStdStream::CreateNewFile(bool bAlways)
{
	// Not implemented - error!
	return false;
}

bool FileStdStream::CreateNewDirectory()
{
	// Not implemented - error!
	return false;
}

bool FileStdStream::Delete()
{
	// Not implemented - error!
	return false;
}

bool FileStdStream::DeleteDirectory()
{
	// Not implemented - error!
	return false;
}

void FileStdStream::Close()
{
	// Close stream handle
	if (m_pFile) {
		fclose(m_pFile);
		m_pFile = nullptr;
	}
}

bool FileStdStream::Open(uint32 nAccess, String::EFormat nStringFormat)
{
	// If a standard stream has been used, we cannot open/reopen it
	if (m_bStream)
		return false;

	// Close file
	Close();

	// Set file mode
	char szMode[4];
	if ((nAccess & File::FileWrite) && !(nAccess & File::FileCreate) && (nAccess & File::FileAppend)) {
		// Append at the end of the file
		if (nAccess & File::FileRead)
			strcpy(szMode, "a+");	// Append, read and write
		else
			strcpy(szMode, "a");	// Append, write only
	} else if ((nAccess & File::FileWrite) && (nAccess & File::FileCreate) && !(nAccess & File::FileAppend)) {
		// Create and open writable
		if (nAccess & File::FileRead)
			strcpy(szMode, "w+");	// Create, read and write
		else
			strcpy(szMode, "w");	// Create, write only
	} else if ((nAccess & File::FileWrite) && !(nAccess & File::FileCreate) && !(nAccess & File::FileAppend)) {
		// Open writable
		if (nAccess & File::FileRead)
			strcpy(szMode, "r+");	// Open, read and write

		// We need to check whether the file already exist, if so, we can go on...
		else {
			if (Exists())
				strcpy(szMode, "w");	// Open, write only
			else
				return false;			// Invalid
		}
	} else if (!(nAccess & File::FileWrite) && !(nAccess & File::FileCreate) && !(nAccess & File::FileAppend)) {
		// Open not writable
		if (nAccess & File::FileRead)
			strcpy(szMode, "r");	// Open, read only
		else
			return false;			// Invalid
	} else {
		// Invalid combination
		return false;
	}

	// Set text or binary mode
	strcat(szMode, (nAccess & File::FileText) ? "t" : "b");

	// Save access modes and string encoding format
	m_nAccess       = nAccess;
	m_nStringFormat = nStringFormat;

	// Get OS file handle and open file
	#if defined(WIN32)
		const int nFile = _open_osfhandle(m_hFile, 0);
		if (nFile > -1) {
			// ASCII or Unicode?
			if (nStringFormat == String::ASCII) {
				// ASCII
				m_pFile = _fdopen(nFile, szMode);
			} else {
				// Unicode
				const wchar_t szModeUnicode[4] = {szMode[0], szMode[1], szMode[2], szMode[3]};
				m_pFile = _wfdopen(nFile, szModeUnicode);
			}
		}
	#elif defined(LINUX)
		// [TODO] Support for "nStringFormat"-parameter?
		if (m_hFile > -1)
			m_pFile = _fdopen(m_hFile, szMode);
	#endif

	// Done
	return (m_pFile != nullptr);
}

bool FileStdStream::IsOpen() const
{
	// Check whether the file is opened
	return (m_pFile != nullptr);
}

bool FileStdStream::IsReadable() const
{
	// Check whether the file is readable
	return (m_pFile && (m_nAccess & File::FileRead));
}

bool FileStdStream::IsWritable() const
{
	// Check whether the file is writable
	return (m_pFile && (m_nAccess & File::FileWrite));
}

String::EFormat FileStdStream::GetStringFormat() const
{
	// Return the saved string encoding format
	return m_nStringFormat;
}

bool FileStdStream::IsEof() const
{
	// Check end of file
	return (!m_pFile || feof(m_pFile));
}

int FileStdStream::GetC()
{
	#ifdef WIN32
		// Read character
		if (IsReadable())
			return ((m_nStringFormat == String::ASCII) ? getc(m_pFile) : getwc(m_pFile));

		// Error!
		return -1;
	#else
		// Read character
		return m_pFile ? getc(m_pFile) : -1;
	#endif
}

bool FileStdStream::PutC(int nChar)
{
	#ifdef WIN32
		// Write character
		if (IsWritable())
			return ((m_nStringFormat == String::ASCII) ? putc(nChar, m_pFile) : putwc(static_cast<wchar_t>(nChar), m_pFile)) != 0;

		// Error!
		return false;
	#else
		// Write character
		return (m_pFile && IsWritable() && putc(nChar, m_pFile) != EOF);
	#endif
}

String FileStdStream::GetS()
{
	// Read string
	if (m_pFile) {
		#ifdef WIN32
			// ASCII or Unicode?
			if (m_nStringFormat == String::ASCII) {
				char szTemp[2048] = "";
				if (fgets(szTemp, 2048, m_pFile))
					return szTemp;
			} else {
				wchar_t szTemp[2048] = L"";
				if (fgetws(szTemp, 2048, m_pFile))
					return szTemp;
			}
		#else
			char szTemp[2048] = "";
			if (fgets(szTemp, 2048, m_pFile))
				return szTemp;
		#endif
	}

	// Error!
	return "";
}

int FileStdStream::PutS(const String &sString)
{
	// Write string
	if (m_pFile && IsWritable()) {
		#ifdef WIN32
			// ASCII or Unicode?
			if (m_nStringFormat == String::ASCII) {
				const int nSize = fputs(sString.GetASCII(), m_pFile);
				if (nSize >= 0)
					return sString.GetLength();
			} else {
				const int nSize = fputws(sString.GetUnicode(), m_pFile);
				if (nSize >= 0)
					return sString.GetLength();
			}
		#else
			const int nSize = fputs((sString.GetFormat() == String::ASCII) ? sString.GetASCII() : sString.GetUTF8(), m_pFile);
			if (nSize >= 0)
				return sString.GetLength();
		#endif
	}

	// Error!
	return -1;
}

uint32 FileStdStream::Read(void *pBuffer, uint32 nSize, uint32 nCount)
{
	// Read buffer
	return m_pFile ? static_cast<uint32>(fread(pBuffer, nSize, nCount, m_pFile)) : 0;
}

uint32 FileStdStream::Write(const void *pBuffer, uint32 nSize, uint32 nCount)
{
	// Write buffer
	return m_pFile && IsWritable() ? static_cast<uint32>(fwrite(pBuffer, nSize, nCount, m_pFile)) : 0;
}

bool FileStdStream::Flush()
{
	// Flush buffer
	return (m_pFile && !fflush(m_pFile));
}

bool FileStdStream::Seek(int32 nOffset, uint32 nLocation)
{
	// Check file pointer
	if (m_pFile) {
		// Convert seek position to Windows value
		int nSeek;
		switch (nLocation) {
			case File::SeekCurrent:
				nSeek = SEEK_CUR;
				break;

			case File::SeekEnd:
				nSeek = SEEK_END;
				break;

			case File::SeekSet:
				nSeek = SEEK_SET;
				break;

			default:
				return false; // Error!
		}

		// Seek file
		return !fseek(m_pFile, nOffset, nSeek);
	} else {
		// Error!
		return false;
	}
}

int32 FileStdStream::Tell() const
{
	// Get file position
	return m_pFile ? ftell(m_pFile) : -1;
}

uint32 FileStdStream::GetSize() const
{
	// Check file pointer
	if (m_pFile) {
		// Get file size
		const int32 nPos = Tell();
		fseek(m_pFile, 0, SEEK_END);
		const int32 nSize = ftell(m_pFile);
		fseek(m_pFile, nPos, SEEK_SET);
		return nSize;
	} else {
		// Error!
		return 0;
	}
}

FileSearchImpl *FileStdStream::CreateSearch()
{
	// Not implemented
	return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
