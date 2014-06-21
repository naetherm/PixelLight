/*********************************************************\
 *  File: FileObject.cpp                                 *
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
#ifndef DISABLE_ZIP_SUPPORT	// Although not recommended, it's possible to life without ZIP support for minimal builds
	#include "PLCore/File/+Zip/FileZip.h"
#endif
#include "PLCore/File/FileHttp.h"
#if defined(WIN32)
	#include "PLCore/File/+Windows/FileWindows.h"
#elif defined(ANDROID)
	#include "PLCore/File/+Linux/+Android/FileAndroid.h"
#elif defined(LINUX)
	#include "PLCore/File/+Linux/FileLinux.h"
#endif
#include "PLCore/File/FileStdStream.h"
#include "PLCore/File/FileObject.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Assign a new URL
*/
void FileObject::Assign(const Url &cUrl, const FileAccess *pAccess)
{
	// Get cleaned up URL (when trying to simplify this, ensure that it's still running on all supported platforms, including Android)
	Url cFinalUrl;
	if (cUrl.IsDirectory() && cUrl.GetPath().GetLength()) {
		const String sUrl = cUrl.GetUrl();
		cFinalUrl = sUrl.GetSubstring(0, sUrl.GetLength() - 1);
	} else {
		cFinalUrl = cUrl;
	}
	cFinalUrl.Collapse();

	// Delete existing file implementation
	if (m_pFileImpl) {
		delete m_pFileImpl;
		m_pFileImpl = nullptr;
	}

	// Create file implementation

	// ZIP
	const String sUrlLower = cFinalUrl.GetUrl().ToLower();
	const int nPos = sUrlLower.LastIndexOf(".zip/");
	if (nPos > -1) {
		// Although not recommended, it's possible to life without ZIP support for minimal builds
		#ifndef DISABLE_ZIP_SUPPORT
			const String sZipFile   = cFinalUrl.GetUrl().GetSubstring(0, nPos+4);
			const String sPathInZip = cFinalUrl.GetUrl().GetSubstring(nPos+5);
			m_pFileImpl = new FileZip(cFinalUrl, sZipFile, sPathInZip, pAccess);
		#endif

	// HTTP
	} else if (cFinalUrl.GetProtocol() == "http://") {
		m_pFileImpl = new FileHttp(cUrl, pAccess);

	// System file
	} else {
		#if defined(WIN32)
			m_pFileImpl = new FileWindows(cFinalUrl, pAccess);
		#elif defined(ANDROID)
			m_pFileImpl = new FileAndroid(cFinalUrl, pAccess);
		#elif defined(LINUX)
			m_pFileImpl = new FileLinux(cFinalUrl, pAccess);
		#else
			#error "Unsupported platform"
		#endif
	}
}

/**
*  @brief
*    Assign a new file
*/
void FileObject::Assign(FILE *pFile, uint32 nAccess)
{
	// Delete existing file implementation
	if (m_pFileImpl) {
		delete m_pFileImpl;
		m_pFileImpl = nullptr;
	}

	// Create file implementation
	m_pFileImpl = new FileStdStream(pFile, nAccess);
}

/**
*  @brief
*    Assign a new file
*/
void FileObject::Assign(handle hFile)
{
	// Delete existing file implementation
	if (m_pFileImpl) {
		delete m_pFileImpl;
		m_pFileImpl = nullptr;
	}

	// Create file implementation
	m_pFileImpl = new FileStdStream(hFile);
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Generic implementation of Copy (if system-implementation does not work)
*/
bool FileObject::GenericCopy(const String &sDestination, bool bOverwrite) const
{
	// Check source file
	File cInput(GetUrl());
	if (cInput.Exists()) {
		// Check destination file
		File cOutput(sDestination);
		if (!cOutput.Exists() || bOverwrite) {
			// Try to copy
			bool bSuccess = false;

			// Open the source file
			if (cInput.Open(File::FileRead)) {
				// Open the destination file, create it in case it doesn't exist
				if (cOutput.Open(File::FileWrite | File::FileCreate)) {
					// Copy the content from the source file into the destination file
					char szBuffer[4096];
					while (!cInput.IsEof()) {
						const uint32 nSize = cInput.Read(szBuffer, 1, 4096);
						cOutput.Write(szBuffer, 1, nSize);
					}

					// Close destination file
					cOutput.Close();

					// The file copy step was done successfully
					bSuccess = true;
				}

				// Close source file
				cInput.Close();
			}

			// Return result
			return bSuccess;
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Generic implementation of Move (if system-implementation does not work)
*/
bool FileObject::GenericMove(const String &sDestination)
{
	// The "GenericCopy()"-method is intentionally not reused in order to don't create an input file object, twice

	// Check source file
	File cInput(GetUrl());
	if (cInput.Exists()) {
		// Check destination file
		File cOutput(sDestination);
		if (!cOutput.Exists()) {
			// Try to copy
			bool bSuccess = false;

			// Open the source file
			if (cInput.Open(File::FileRead)) {
				// Open the destination file, create it in case it doesn't exist
				if (cOutput.Open(File::FileWrite | File::FileCreate)) {
					// Copy the content from the source file into the destination file
					char szBuffer[4096];
					while (!cInput.IsEof()) {
						const uint32 nSize = cInput.Read(szBuffer, 1, 4096);
						cOutput.Write(szBuffer, 1, nSize);
					}

					// Close destination file
					cOutput.Close();

					// The file copy step was done successfully
					bSuccess = true;
				}

				// Close source file
				cInput.Close();
			}

			// Delete source file and return result
			return bSuccess ? cInput.Delete() : false;
		}
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
FileObject::FileObject(const FileObject &cSource) :
	m_pFileImpl(nullptr)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
FileObject &FileObject::operator =(const FileObject &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
