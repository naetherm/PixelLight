/*********************************************************\
 *  File: FileSearchZip.cpp                              *
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
FileSearchZip::FileSearchZip(const String &sZipFile, const String &sPathInZip, const FileAccess *pAccess) : FileSearchImpl(pAccess),
	m_sZipFile(sZipFile),
	m_sPathInZip(sPathInZip)
{
	// Get options from access-information
	String sPassword;
	bool bCaseSensitive = true;
	bool bMemBuf        = true;
	if (m_pAccess && m_pAccess->HasEntry(sZipFile)) {
		const FileAccess::Entry &cZipAccess = m_pAccess->GetEntry(sZipFile);
		sPassword      = cZipAccess.GetPassword();
		bCaseSensitive = cZipAccess.GetCaseSensitive();
		if (cZipAccess.GetFlagsMask() & File::FileMemBuf)
			bMemBuf = ((cZipAccess.GetFlagsValue() & File::FileMemBuf) != 0);
	}

	// Open ZIP-file
	m_cZipFile.Open(sZipFile, sPassword, bCaseSensitive, bMemBuf, m_pAccess);

	// Jump to first entry in ZIP-file
	m_cZipFile.LocateFirstFile();

	// Find the next file that matches the path-in-zip
	FindNextFile();
}

/**
*  @brief
*    Destructor
*/
FileSearchZip::~FileSearchZip()
{
}


//[-------------------------------------------------------]
//[ Private virtual FileSearchImpl functions              ]
//[-------------------------------------------------------]
bool FileSearchZip::HasNextFile()
{
	// Return if the current file is valid
	return m_cZipFile.GetCurrentFile().m_bValid;
}

String FileSearchZip::GetNextFile()
{
	// Check if the current file is valid
	if (m_cZipFile.GetCurrentFile().m_bValid) {
		// Get name of file without the path prefix
		String sName = (m_sPathInZip.Compare('/') ?
							m_cZipFile.GetCurrentFile().m_sFilename :
							m_cZipFile.GetCurrentFile().m_sFilename.GetSubstring(m_sPathInZip.GetLength()));

		// Go to next file
		m_cZipFile.LocateNextFile();

		// Find the next file that matches the path-in-zip
		FindNextFile();

		// Return filename
		return sName;
	}

	// No more valid files found
	return "";
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Find next file in the selected directory
*/
bool FileSearchZip::FindNextFile()
{
	while (m_cZipFile.GetCurrentFile().m_bValid) {
		// Is this file in the right directory?
		if (CheckFile(m_cZipFile.GetCurrentFile().m_sFilename)) {
			// Yep, file found
			return true;
		}

		// Go to next file
		m_cZipFile.LocateNextFile();
	}

	// No valid file found
	return false;
}

bool FileSearchZip::CheckFile(const String &sFilename)
{
	// Check if m_sPathInZip is a prefix of the filename or "/"
	if (m_sPathInZip.Compare('/') ||
		(m_sPathInZip.GetLength() < sFilename.GetLength() &&
		(m_cZipFile.IsCaseSensitive() ? sFilename.Compare(m_sPathInZip, 0, m_sPathInZip.GetLength()) :
										sFilename.CompareNoCase(m_sPathInZip, 0, m_sPathInZip.GetLength()))))
	{
		// After that prefix, '/' must only appear at the end of the string or not at all
		const int nStart = m_sPathInZip.Compare('/') ? 0 : m_sPathInZip.GetLength();
		const int nSlash = sFilename.IndexOf('/', nStart);
		return (nSlash < 0 || nSlash == static_cast<int>(sFilename.GetLength())-1);
	}

	// Not in the current directory
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
