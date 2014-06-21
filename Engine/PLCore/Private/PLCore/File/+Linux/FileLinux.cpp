/*********************************************************\
 *  File: FileLinux.cpp                                  *
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "PLCore/PLCoreLinuxIncludes.h"
#include "PLCore/File/File.h"
#include "PLCore/File/FileSearchLinux.h"
#include "PLCore/File/FileLinux.h"


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
FileLinux::FileLinux(const Url &cUrl, const FileAccess *pAccess) : FileImpl(cUrl, pAccess),
	m_sFilename(cUrl.GetUnixPath()),
	m_nAccess(0),
	m_pFile(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
FileLinux::~FileLinux()
{
	// Close file
	Close();
}


//[-------------------------------------------------------]
//[ Private virtual FileImpl functions                    ]
//[-------------------------------------------------------]
bool FileLinux::Exists() const
{
	// Get file status
	struct stat sStat;
	return (stat((m_sFilename.GetFormat() == String::ASCII) ? m_sFilename.GetASCII() : m_sFilename.GetUTF8(), &sStat) == 0);
}

bool FileLinux::IsFile() const
{
	// Get file status
	struct stat sStat;
	if (stat((m_sFilename.GetFormat() == String::ASCII) ? m_sFilename.GetASCII() : m_sFilename.GetUTF8(), &sStat) == 0)
		return ((sStat.st_mode & S_IFREG) != 0);

	// Error!
	return false;
}

bool FileLinux::IsDirectory() const
{
	// Get file status
	struct stat sStat;
	if (stat((m_sFilename.GetFormat() == String::ASCII) ? m_sFilename.GetASCII() : m_sFilename.GetUTF8(), &sStat) == 0)
		return ((sStat.st_mode & S_IFDIR) != 0);

	// Error!
	return false;
}

bool FileLinux::CopyTo(const String &sDest, bool bOverwrite) const
{
	// Get destination as a unix path
	const Url cDestUrl(sDest);
	if (cDestUrl.IsValid() && cDestUrl.IsValidUnixPath()) {
		// Get absolute path of old and new filename (in Windows notation)
		const String sNewFilename = Url(cDestUrl.CutFilename() + Url(m_sFilename).GetFilename()).GetUnixPath();

		// Note: This might be dangerous, because the destination URL is blindly used, so if e.g.
		// the user tries to move "C:\test.txt" to "C:\test.zip\test2.txt", this will be passed
		// over to MoveFile and we can only hope that the OS does nothing bad and returns a failure.
		// The only other option would be to check beforehand, whether the destination URL is a valid
		// local filename and return false otherwise without calling MoveFile. But this would mean
		// that an expansive check would have to be done every time this function is called, even if
		// in 90% of all cases both the URLs are on the local file system anyway.

		// Copy file (-d same as --no-dereference --preserve=links)
		// [TODO] Linux copy file: There must be another way to copy a file using c functions...
		const String sTarget = "cp -d \"" + m_sFilename + "\" \"" + sNewFilename + '\"';
		return (system((sTarget.GetFormat() == String::ASCII) ? sTarget.GetASCII() : sTarget.GetUTF8()) >= 0);
	}

	// Error!
	return false;
}

bool FileLinux::MoveTo(const String &sDest)
{
	// Close file before moving it
	Close();

	// Get destination as a unix path
	const Url cDestUrl(sDest);
	if (cDestUrl.IsValid() && cDestUrl.IsValidUnixPath()) {
		// Get absolute path of old and new filename (in Unix notation)
		const String sNewFilename = cDestUrl.GetUnixPath();

		// Note: This might be dangerous, because the destination URL is blindly used, so if e.g.
		// the user tries to move "C:\test.txt" to "C:\test.zip\test2.txt", this will be passed
		// over to MoveFile and we can only hope that the OS does nothing bad and returns a failure.
		// The only other option would be to check beforehand, whether the destination URL is a valid
		// local filename and return false otherwise without calling MoveFile. But this would mean
		// that an expansive check would have to be done every time this function is called, even if
		// in 90% of all cases both the URLs are on the local file system anyway.

		// Move file
		if (rename((m_sFilename .GetFormat() == String::ASCII) ? m_sFilename .GetASCII() : m_sFilename .GetUTF8(),
				   (sNewFilename.GetFormat() == String::ASCII) ? sNewFilename.GetASCII() : sNewFilename.GetUTF8()) == 0) {
			// Set new file name
			m_cUrl = sNewFilename;

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

bool FileLinux::Rename(const String &sName)
{
	// Close file before renaming it
	Close();

	// Check parameter: No path is allowed ('/' or '\')
	if (sName.IsSubstring('\\') || sName.IsSubstring('/'))
		return false; // Error!

	// Get absolute path of old and new name (in Unix notation)
	const String sNewFilename = Url(m_cUrl.CutFilename() + sName).GetUnixPath();

	// Rename file
	if (rename((m_sFilename .GetFormat() == String::ASCII) ? m_sFilename .GetASCII() : m_sFilename .GetUTF8(),
			   (sNewFilename.GetFormat() == String::ASCII) ? sNewFilename.GetASCII() : sNewFilename.GetUTF8()) == 0) {
		// Set new file name
		m_cUrl = sNewFilename;

		// Done
		return true;
	}

	// Error!
	return false;
}

bool FileLinux::CreateNewFile(bool bAlways)
{
	// Close file if open
	Close();

	// Create file
	const int nFile = open((m_sFilename.GetFormat() == String::ASCII) ? m_sFilename.GetASCII() : m_sFilename.GetUTF8(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (nFile != -1) {
		// Success
		close(nFile);
		return true;
	}

	// Error
	return false;
}

bool FileLinux::CreateNewDirectory()
{
	// Close file if open
	Close();

	// Create directory
	return (mkdir((m_sFilename.GetFormat() == String::ASCII) ? m_sFilename.GetASCII() : m_sFilename.GetUTF8(), 0711) != -1);
}

bool FileLinux::Delete()
{
	// Close file before deleting it
	Close();

	// Delete file
	return (unlink((m_sFilename.GetFormat() == String::ASCII) ? m_sFilename.GetASCII() : m_sFilename.GetUTF8()) == 0);
}

bool FileLinux::DeleteDirectory()
{
	// Close if open
	Close();

	// Delete directory
	return (rmdir((m_sFilename.GetFormat() == String::ASCII) ? m_sFilename.GetASCII() : m_sFilename.GetUTF8()) == 0);
}

void FileLinux::Close()
{
	// Check file pointer
	if (m_pFile) {
		// Close file
		fclose(m_pFile);
		m_pFile = nullptr;
	}
}

bool FileLinux::Open(uint32 nAccess, String::EFormat nStringFormat)
{
	// Close file first
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

	// Save access modes
	m_nAccess = nAccess;

	// [TODO] Support of the "nStringFormat"-parameter?

	// Open file
	m_pFile = fopen((m_sFilename.GetFormat() == String::ASCII) ? m_sFilename.GetASCII() : m_sFilename.GetUTF8(), szMode);

	// Done
	return (m_pFile != nullptr);
}

bool FileLinux::IsOpen() const
{
	// Check whether the file is open
	return (m_pFile != nullptr);
}

bool FileLinux::IsReadable() const
{
	// Check whether the file is readable
	return (m_pFile && (m_nAccess & File::FileRead));
}

bool FileLinux::IsWritable() const
{
	// Check whether the file is writable
	return (m_pFile && (m_nAccess & File::FileWrite));
}

String::EFormat FileLinux::GetStringFormat() const
{
	// Default is ASCII
	return String::ASCII;
}

bool FileLinux::IsEof() const
{
	// Check end of file
	return (!m_pFile || feof(m_pFile));
}

int FileLinux::GetC()
{
	// Read character
	return IsReadable() ? getc(m_pFile) : -1;
}

bool FileLinux::PutC(int nChar)
{
	// Write character
	return (IsWritable() && putc(nChar, m_pFile) != EOF);
}

String FileLinux::GetS()
{
	// Read string
	if (IsReadable()) {
		char szTemp[2048] = "";
		if (fgets(szTemp, 2048, m_pFile))
			return szTemp;
	}

	// Error!
	return "";
}

int FileLinux::PutS(const String &sString)
{
	// Write string
	if (IsWritable()) {
		const int nSize = fputs(sString, m_pFile);
		if (nSize >= 0)
			return sString.GetLength();
	}

	// Error!
	return -1;
}

uint32 FileLinux::Read(void *pBuffer, uint32 nSize, uint32 nCount)
{
	// Read buffer
	return IsReadable() ? static_cast<uint32>(fread(pBuffer, nSize, nCount, m_pFile)) : 0;
}

uint32 FileLinux::Write(const void *pBuffer, uint32 nSize, uint32 nCount)
{
	// Write buffer
	return IsWritable() ? static_cast<uint32>(fwrite(pBuffer, nSize, nCount, m_pFile)) : 0;
}

bool FileLinux::Flush()
{
	// Flush buffer
	return (m_pFile && !fflush(m_pFile));
}

bool FileLinux::Seek(int32 nOffset, uint32 nLocation)
{
	// Check file pointer
	if (m_pFile) {
		// Convert seek position to system value
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

int32 FileLinux::Tell() const
{
	// Get file position
	return m_pFile ? ftell(m_pFile) : -1;
}

uint32 FileLinux::GetSize() const
{
	// Check file pointer
	if (m_pFile) {
		// Get file size
		const int32 nPos = ftell(m_pFile);
		fseek(m_pFile, 0, SEEK_END);
		const int32 nSize = ftell(m_pFile);
		fseek(m_pFile, nPos, SEEK_SET);
		return nSize;
	} else {
		// Error!
		return 0;
	}
}

FileSearchImpl *FileLinux::CreateSearch()
{
	// Create a file searcher
	return new FileSearchLinux(m_cUrl.GetUnixPath(), m_pAccess);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
