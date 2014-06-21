/*********************************************************\
 *  File: FileObject.inl                                 *
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
#include "PLCore/File/FileImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
inline FileObject::FileObject() :
	m_pFileImpl(nullptr)
{
}

/**
*  @brief
*    Constructor
*/
inline FileObject::FileObject(const String &sUrl, const FileAccess *pAccess) :
	m_pFileImpl(nullptr)
{
	Assign(sUrl, pAccess);
}

/**
*  @brief
*    Constructor
*/
inline FileObject::FileObject(const Url &cUrl, const FileAccess *pAccess) :
	m_pFileImpl(nullptr)
{
	Assign(cUrl, pAccess);
}

/**
*  @brief
*    Constructor for a standard OS stream
*/
inline FileObject::FileObject(FILE *pFile, uint32 nAccess) :
	m_pFileImpl(nullptr)
{
	Assign(pFile, nAccess);
}

/**
*  @brief
*    Constructor for a OS file handle
*/
inline FileObject::FileObject(handle hFile) :
	m_pFileImpl(nullptr)
{
	Assign(hFile);
}

/**
*  @brief
*    Destructor
*/
inline FileObject::~FileObject()
{
	// Delete file implementation
	if (m_pFileImpl)
		delete m_pFileImpl;
}

/**
*  @brief
*    Assign a new URL
*/
inline void FileObject::Assign(const String &sUrl, const FileAccess *pAccess)
{
	Assign(Url(sUrl), pAccess);
}

/**
*  @brief
*    Get URL
*/
inline const Url &FileObject::GetUrl() const
{
	return m_pFileImpl ? m_pFileImpl->m_cUrl : m_cNullUrl;
}

/**
*  @brief
*    Check if the file or directory exists
*/
inline bool FileObject::Exists() const
{
	return m_pFileImpl ? m_pFileImpl->Exists() : false;
}

/**
*  @brief
*    Returns if the object is a file
*/
inline bool FileObject::IsFile() const
{
	return m_pFileImpl ? m_pFileImpl->IsFile() : false;
}

/**
*  @brief
*    Returns if the object is a directory
*/
inline bool FileObject::IsDirectory() const
{
	return m_pFileImpl ? m_pFileImpl->IsDirectory() : false;
}

/**
*  @brief
*    Copy the file or directory to a new location
*/
inline bool FileObject::Copy(const String &sDestination, bool bOverwrite) const
{
	if (m_pFileImpl) {
		// Try system-implementation
		if (m_pFileImpl->CopyTo(sDestination, bOverwrite))
			return true; // Done

		// Fallback: Use generic function (e.g. if the files exist in different file-systems)
		return GenericCopy(sDestination, bOverwrite);
	} else {
		return false;
	}
}

/**
*  @brief
*    Move the file or directory to a new location
*/
inline bool FileObject::Move(const String &sDestination)
{
	if (m_pFileImpl) {
		// Try system-implementation
		if (m_pFileImpl->MoveTo(sDestination))
			return true; // Done

		// Fallback: Use generic function (e.g. if the files exist in different file-systems)
		return GenericMove(sDestination);
	} else {
		return false;
	}
}

/**
*  @brief
*    Rename the file or directory
*/
inline bool FileObject::Rename(const String &sName)
{
	return m_pFileImpl ? m_pFileImpl->Rename(sName) : false;
}

/**
*  @brief
*    Close the file or directory
*/
inline void FileObject::Close()
{
	if (m_pFileImpl)
		m_pFileImpl->Close();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
