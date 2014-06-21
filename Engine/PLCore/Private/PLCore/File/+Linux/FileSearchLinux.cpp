/*********************************************************\
 *  File: FileSearchLinux.cpp                            *
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
#include "PLCore/File/FileSearchLinux.h"


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
FileSearchLinux::FileSearchLinux(const String &sPath, const FileAccess *pAccess) : FileSearchImpl(pAccess),
	m_sPath(sPath)
{
	// Start search
	m_pDir = opendir((m_sPath.GetFormat() == String::ASCII) ? m_sPath.GetASCII() : m_sPath.GetUTF8());
	if (m_pDir) {
		// Something has been found
		m_pDirEntry	= readdir(m_pDir);
		m_bHasNext	= true;
	} else {
		// Error!
		m_pDirEntry	= nullptr;
		m_bHasNext	= false;
	}
}

/**
*  @brief
*    Destructor
*/
FileSearchLinux::~FileSearchLinux()
{
	// Close the search
	if (m_pDir)
		closedir(m_pDir);
}


//[-------------------------------------------------------]
//[ Private virtual FileSearchImpl functions              ]
//[-------------------------------------------------------]
bool FileSearchLinux::HasNextFile()
{
	return m_bHasNext;
}

String FileSearchLinux::GetNextFile()
{
	if (m_bHasNext) {
		m_sFilename = m_pDirEntry->d_name;
		m_pDirEntry = readdir(m_pDir);
		m_bHasNext  = (m_pDirEntry != nullptr);
		return m_sFilename;
	} else {
		return "";
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
