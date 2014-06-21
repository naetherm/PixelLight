/*********************************************************\
 *  File: FileWindows.cpp                                *
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
#include <io.h>
#include <stdio.h>
#include "PLCore/+Windows/PLCoreWindowsIncludes.h"
#include "PLCore/File/File.h"
#include "PLCore/File/+Windows/FileSearchWindows.h"
#include "PLCore/File/+Windows/FileWindows.h"


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
FileWindows::FileWindows(const Url &cUrl, const FileAccess *pAccess) : FileImpl(cUrl, pAccess),
	m_sFilename(cUrl.GetWindowsPath()),
	m_nAccess(0),
	m_nStringFormat(String::ASCII),
	m_pFile(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
FileWindows::~FileWindows()
{
	// Close file
	Close();
}


//[-------------------------------------------------------]
//[ Private virtual FileImpl functions                    ]
//[-------------------------------------------------------]
bool FileWindows::Exists() const
{
	// Check file attributes
	if (m_sFilename.GetFormat() == String::ASCII)
		return (GetFileAttributesA(m_sFilename.GetASCII()) != INVALID_FILE_ATTRIBUTES);
	else
		return (GetFileAttributesW(m_sFilename.GetUnicode()) != INVALID_FILE_ATTRIBUTES);
}

bool FileWindows::IsFile() const
{
	// Check file attributes
	DWORD nAttributes;
	if (m_sFilename.GetFormat() == String::ASCII)
		nAttributes = GetFileAttributesA(m_sFilename.GetASCII());
	else
		nAttributes = GetFileAttributesW(m_sFilename.GetUnicode());

	// Is it a regular file?
	return (nAttributes != INVALID_FILE_ATTRIBUTES && !(nAttributes & FILE_ATTRIBUTE_DIRECTORY));
}

bool FileWindows::IsDirectory() const
{
	// Check file attributes
	DWORD nAttributes;
	if (m_sFilename.GetFormat() == String::ASCII)
		nAttributes = GetFileAttributesA(m_sFilename.GetASCII());
	else
		nAttributes = GetFileAttributesW(m_sFilename.GetUnicode());

	// Is it a regular file?
	return (nAttributes != INVALID_FILE_ATTRIBUTES && (nAttributes & FILE_ATTRIBUTE_DIRECTORY));
}

bool FileWindows::CopyTo(const String &sDest, bool bOverwrite) const
{
	// Get destination as a windows path
	const Url cDestUrl = Url(sDest);
	if (cDestUrl.IsValid() && cDestUrl.IsValidWindowsPath()) {
		// Get absolute path of old and new filename (in Windows notation)
		const String sNewFilename = Url(cDestUrl.CutFilename() + Url(m_sFilename).GetFilename()).GetWindowsPath();

		// Note: See MoveTo, same applies here

		// Copy file (if the old absolute filename was Unicode, the new absolute one is also Unicode)
		if (sNewFilename.GetFormat() == String::ASCII)
			return (CopyFileA(m_sFilename.GetASCII(), sNewFilename.GetASCII(), !bOverwrite) != 0);
		else
			return (CopyFileW(m_sFilename.GetUnicode(), sNewFilename.GetUnicode(), !bOverwrite) != 0);
	}

	// Error
	return false;
}

bool FileWindows::MoveTo(const String &sDest)
{
	// Close file before moving it
	Close();

	// Get destination as a windows path
	const Url cDestUrl = Url(sDest);
	if (cDestUrl.IsValid() && cDestUrl.IsValidWindowsPath()) {
		// Get absolute path of old and new filename (in Windows notation)
		const String sNewFilename = Url(cDestUrl.CutFilename()).GetWindowsPath();

		// Note: This might be dangerous, because the destination URL is blindly used, so if e.g.
		// the user tries to move "C:\test.txt" to "C:\test.zip\test2.txt", this will be passed
		// over to MoveFile and we can only hope that the OS does nothing bad and returns a failure.
		// The only other option would be to check beforehand, whether the destination URL is a valid
		// local filename and return false otherwise without calling MoveFile. But this would mean
		// that an expansive check would have to be done every time this function is called, even if
		// in 90% of all cases both the URLs are on the local file system anyway.

		// Move file (if the old absolute filename was Unicode, the new absolute one is also Unicode)
		bool bSuccess;
		if (sNewFilename.GetFormat() == String::ASCII)
			bSuccess = (MoveFileA(m_sFilename.GetASCII(), sNewFilename.GetASCII()) != 0);
		else
			bSuccess = (MoveFileW(m_sFilename.GetUnicode(), sNewFilename.GetUnicode()) != 0);

		// Set new file name
		if (bSuccess) {
			m_cUrl = sNewFilename;
			return true;
		}
	}

	// Error
	return false;
}

bool FileWindows::Rename(const String &sName)
{
	// Close file before renaming it
	Close();

	// Check parameter: No path is allowed ('/' or '\')
	if (sName.IsSubstring('\\') || sName.IsSubstring('/'))
		return false; // Error!

	// Get absolute path of old and new name (in Windows notation)
	const String sNewFilename = Url(m_cUrl.CutFilename() + sName).GetWindowsPath();

	// Rename file (if the old absolute filename was Unicode, the new absolute one is also Unicode)
	bool bSuccess;
	if (sNewFilename.GetFormat() == String::ASCII)
		bSuccess = (MoveFileA(m_sFilename.GetASCII(), sNewFilename.GetASCII()) != 0);
	else
		bSuccess = (MoveFileW(m_sFilename.GetUnicode(), sNewFilename.GetUnicode()) != 0);

	// Set new file name
	if (bSuccess)
		m_cUrl = sNewFilename;

	// Done
	return bSuccess;
}

bool FileWindows::CreateNewFile(bool bAlways)
{
	// Close file if open
	Close();

	// Create file
	HANDLE hFile;
	if (m_sFilename.GetFormat() == String::ASCII)
		hFile = CreateFileA(m_sFilename.GetASCII(), 0, 0, nullptr, bAlways ? CREATE_ALWAYS : CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
	else
		hFile = CreateFileW(m_sFilename.GetUnicode(), 0, 0, nullptr, bAlways ? CREATE_ALWAYS : CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);

	// Check return value
	if (hFile != INVALID_HANDLE_VALUE) {
		// Success
		CloseHandle(hFile);
		return true;
	}

	// Error
	return false;
}

bool FileWindows::CreateNewDirectory()
{
	// Close file if open
	Close();

	// Create directory
	if (m_sFilename.GetFormat() == String::ASCII)
		return (CreateDirectoryA(m_sFilename.GetASCII(), nullptr) != 0);
	else
		return (CreateDirectoryW(m_sFilename.GetUnicode(), nullptr) != 0);
}

bool FileWindows::Delete()
{
	// Close file before deleting it
	Close();

	// Delete file
	if (m_sFilename.GetFormat() == String::ASCII)
		return (DeleteFileA(m_sFilename.GetASCII()) != 0);
	else
		return (DeleteFileW(m_sFilename.GetUnicode()) != 0);
}

bool FileWindows::DeleteDirectory()
{
	// Close if open
	Close();

	// Delete directory
	if (m_sFilename.GetFormat() == String::ASCII)
		return (RemoveDirectoryA(m_sFilename.GetASCII()) != 0);
	else
		return (RemoveDirectoryW(m_sFilename.GetUnicode()) != 0);
}

void FileWindows::Close()
{
	// Check file pointer
	if (m_pFile) {
		// Close file
		fclose(m_pFile);
		m_pFile = nullptr;
	}
}

bool FileWindows::Open(uint32 nAccess, String::EFormat nStringFormat)
{
	// Close file first
	Close();

	// Set file mode
	char szMode[4];
	if ((nAccess & File::FileWrite) && !(nAccess & File::FileCreate) && (nAccess & File::FileAppend)) {
		// Append at the end of the file
		if (nAccess & File::FileRead)
			strcpy(szMode, "a+");		// Append, read and write
		else
			strcpy(szMode, "a");		// Append, write only
	} else if ((nAccess & File::FileWrite) && (nAccess & File::FileCreate) && !(nAccess & File::FileAppend)) {
		// Create and open writable
		if (nAccess & File::FileRead)
			strcpy(szMode, "w+");		// Create, read and write
		else
			strcpy(szMode, "w");		// Create, write only
	} else if ((nAccess & File::FileWrite) && !(nAccess & File::FileCreate) && !(nAccess & File::FileAppend)) {
		// Open writable
		if (nAccess & File::FileRead)
			strcpy(szMode, "r+");		// Open, read and write

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
			strcpy(szMode, "r");		// Open, read only
		else
			return false;				// Invalid
	} else {
		// Invalid combination
		return false;
	}

	// Set text or binary mode
	strcat(szMode, (nAccess & File::FileText) ? "t" : "b");

	// Save access modes and string encoding format
	m_nAccess       = nAccess;
	m_nStringFormat = nStringFormat;

	// Open file using ASCII/unicode filename
	if (m_sFilename.GetFormat() == String::ASCII) {
		// ASCII filename

		// ASCII is the default string encoding format
		if (nStringFormat == String::ASCII)
			m_pFile = fopen(m_sFilename.GetASCII(), szMode);
		else
			m_pFile = fopen(m_sFilename.GetASCII(), szMode + ((nStringFormat == String::Unicode) ? String(", ccs=UNICODE") : String(", ccs=UTF-8")));
	} else {
		// Unicode filename
		const wchar_t szModeUnicode[4] = {szMode[0], szMode[1], szMode[2], szMode[3]};

		// ASCII is the default string encoding format
		if (nStringFormat == String::ASCII)
			m_pFile = _wfopen(m_sFilename.GetUnicode(), szModeUnicode);
		else
			m_pFile = _wfopen(m_sFilename.GetUnicode(), szModeUnicode + ((nStringFormat == String::Unicode) ? String(", ccs=UNICODE") : String(", ccs=UTF-8")));
	}

	// Done
	return (m_pFile != nullptr);
}

bool FileWindows::IsOpen() const
{
	// Check whether the file is open
	return (m_pFile != nullptr);
}

bool FileWindows::IsReadable() const
{
	// Check whether the file is readable
	return (m_pFile && (m_nAccess & File::FileRead));
}

bool FileWindows::IsWritable() const
{
	// Check whether the file is writable
	return (m_pFile && (m_nAccess & File::FileWrite));
}

String::EFormat FileWindows::GetStringFormat() const
{
	// Return the saved string encoding format
	return m_nStringFormat;
}

bool FileWindows::IsEof() const
{
	// Check end of file
	return (!m_pFile || feof(m_pFile));
}

int FileWindows::GetC()
{
	// Read character
	if (IsReadable())
		return ((m_nStringFormat == String::ASCII) ? getc(m_pFile) : getwc(m_pFile));

	// Error!
	return -1;
}

bool FileWindows::PutC(int nChar)
{
	// Write character
	if (IsWritable())
		return ((m_nStringFormat == String::ASCII) ? putc(nChar, m_pFile) : putwc(static_cast<wchar_t>(nChar), m_pFile)) != 0;

	// Error!
	return false;
}

String FileWindows::GetS()
{
	// Read string
	if (IsReadable()) {
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
	}

	// Error!
	return "";
}

int FileWindows::PutS(const String &sString)
{
	// Write string
	if (IsWritable()) {
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
	}

	// Error!
	return -1;
}

uint32 FileWindows::Read(void *pBuffer, uint32 nSize, uint32 nCount)
{
	// Read buffer
	return IsReadable() ? static_cast<uint32>(fread(pBuffer, nSize, nCount, m_pFile)) : 0;
}

uint32 FileWindows::Write(const void *pBuffer, uint32 nSize, uint32 nCount)
{
	// Write buffer
	return IsWritable() ? static_cast<uint32>(fwrite(pBuffer, nSize, nCount, m_pFile)) : 0;
}

bool FileWindows::Flush()
{
	// Flush buffer
	return (m_pFile && !fflush(m_pFile));
}

bool FileWindows::Seek(int32 nOffset, uint32 nLocation)
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

int32 FileWindows::Tell() const
{
	// Get file position
	return m_pFile ? ftell(m_pFile) : -1;
}

uint32 FileWindows::GetSize() const
{
	// Check file pointer
	if (m_pFile) {
		/* // Without using special Windows API functions the implementation of this method would be:
		// Get file size
		const int32 nPos = ftell(m_pFile);
		fseek(m_pFile, 0, SEEK_END);
		const int32 nSize = ftell(m_pFile);
		fseek(m_pFile, nPos, SEEK_SET);
		return nSize;
		*/

		// Get the file descriptor associated with our stream and then request the length of the file
		return _filelength(_fileno(m_pFile));
	} else {
		// Error!
		return 0;
	}
}

FileSearchImpl *FileWindows::CreateSearch()
{
	// Create a file searcher
	return new FileSearchWindows(m_cUrl.GetWindowsPath(), m_pAccess);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
