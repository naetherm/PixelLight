/*********************************************************\
 *  File: FileSearchAndroid.cpp                          *
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
#include <stdio.h>	// For e.g. "off_t"
#include <stdint.h>	// For e.g. "size_t"
#include <android/asset_manager.h>
#include "PLCore/System/SystemAndroid.h"
#include "PLCore/File/FileSearchAndroid.h"


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
FileSearchAndroid::FileSearchAndroid(const String &sPath, const FileAccess *pAccess) : FileSearchImpl(pAccess),
	m_pAAssetDir(nullptr),
	m_sPath(sPath),
	m_pszNextFilename(nullptr)
{
	// Get the Android asset manager instance
	AAssetManager *pAAssetManager = SystemAndroid::GetAssetManager();
	if (pAAssetManager) {
		// Start search
		m_pAAssetDir = AAssetManager_openDir(pAAssetManager, (m_sPath.GetFormat() == String::ASCII) ? m_sPath.GetASCII() : m_sPath.GetUTF8());
		if (m_pAAssetDir) {
			// Something has been found
			m_pszNextFilename = AAssetDir_getNextFileName(m_pAAssetDir);
		}
	}
}

/**
*  @brief
*    Destructor
*/
FileSearchAndroid::~FileSearchAndroid()
{
	// Close the search
	if (m_pAAssetDir)
		AAssetDir_close(m_pAAssetDir);
}


//[-------------------------------------------------------]
//[ Private virtual FileSearchImpl functions              ]
//[-------------------------------------------------------]
bool FileSearchAndroid::HasNextFile()
{
	return (m_pszNextFilename != nullptr);
}

String FileSearchAndroid::GetNextFile()
{	
	if (m_pszNextFilename) {
		m_sFilename	= m_pszNextFilename;
		m_pszNextFilename = AAssetDir_getNextFileName(m_pAAssetDir);
		return m_sFilename;
	} else {
		return "";
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
