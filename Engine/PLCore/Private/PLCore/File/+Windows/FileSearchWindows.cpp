/*********************************************************\
 *  File: FileSearchWindows.cpp                          *
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
#include "PLCore/File/+Windows/FileSearchWindows.h"


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
FileSearchWindows::FileSearchWindows(const String &sPath, const FileAccess *pAccess) : FileSearchImpl(pAccess),
	m_sPath(sPath),
	m_sFind(sPath + "\\*.*")
{
	// Start search
	m_hFind = FindFirstFile(m_sFind, &m_sFindData);
	if (m_hFind != INVALID_HANDLE_VALUE) {
		// Something has been found
		m_bHasNext = true;
	} else {
		// Error
		m_bHasNext = false;
		m_hFind    = nullptr;
	}
}

/**
*  @brief
*    Destructor
*/
FileSearchWindows::~FileSearchWindows()
{
	// Close the search
	if (m_hFind)
		FindClose(m_hFind);
}


//[-------------------------------------------------------]
//[ Private virtual FileSearchImpl functions              ]
//[-------------------------------------------------------]
bool FileSearchWindows::HasNextFile()
{
	return m_bHasNext;
}

String FileSearchWindows::GetNextFile()
{
	if (m_bHasNext) {
		m_sFilename = m_sFindData.cFileName;
		m_bHasNext  = (FindNextFileW(m_hFind, &m_sFindData) > 0);
		return m_sFilename;
	} else {
		return "";
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
