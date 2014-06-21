/*********************************************************\
 *  File: IOSystem.cpp                                   *
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
#include <PLCore/File/File.h>
#include <PLCore/System/System.h>
#include "PLAssimp/IOStream.h"
#include "PLAssimp/IOSystem.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLAssimp {


//[-------------------------------------------------------]
//[ Public definitions                                    ]
//[-------------------------------------------------------]
const char *IOSystem::MagicDefaultFilename       = "$$$___magic___$$$";
const int   IOSystem::MagicDefaultFilenameLength = 17;


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
IOSystem::IOSystem(File &cFile, const char *pszMagicDefaultFilename, int nMagicDefaultFilenameLength) :
	m_pFile(&cFile),
	m_pszMagicDefaultFilename(pszMagicDefaultFilename),
	m_nMagicDefaultFilenameLength(nMagicDefaultFilenameLength)
{
}

/**
*  @brief
*    Destructor
*/
IOSystem::~IOSystem()
{
}


//[-------------------------------------------------------]
//[ Public virtual Assimp::IOSystem functions             ]
//[-------------------------------------------------------]
bool IOSystem::Exists(const char *pFile) const
{
	// Is it the default file? (concept based on "Assimp::MemoryIOSystem")
	if (!strncmp(pFile, m_pszMagicDefaultFilename, m_nMagicDefaultFilenameLength)) {
		// Jap, it's the default file
		return true;
	} else {
		// Check whether or not the requested file exists
		return File(GetRealFilename(pFile)).Exists();
	}
}

char IOSystem::getOsSeparator() const
{
	static const char nSeparator = System::GetInstance()->GetSeparator();
	return nSeparator;
}

Assimp::IOStream *IOSystem::Open(const char *pFile, const char *pMode)
{
	// Is it the default file? (concept based on "Assimp::MemoryIOSystem")
	if (!strncmp(pFile, m_pszMagicDefaultFilename, m_nMagicDefaultFilenameLength)) {
		// Jap, it's the default file

		// Reset file reading
		m_pFile->Seek();

		// Wrap the file into an Assimp IO stream and return the IO stream
		return new IOStream(*m_pFile, false);
	} else {
		// Create the file object
		File *pFileObject = new File(GetRealFilename(pFile));

		// Open the file, or at least try it
		if (pFileObject->Open(File::CFileModeToAccessFlags(pMode))) {
			// Wrap the file into an Assimp IO stream and return the IO stream
			return new IOStream(*pFileObject, true);
		}

		// Error! Destroy the file object and get us out of here..
		delete pFileObject;
		return nullptr;
	}
}

void IOSystem::Close(Assimp::IOStream *pFile)
{
	// Is there a valid Assimp IO stream?
	if (pFile) {
		// Destroy the file object (this also automatically closes the file) - but only if it's not the default file!
		File *pFileObject = reinterpret_cast<IOStream*>(pFile)->m_pFile;
		if (m_pFile != pFileObject)
			delete pFileObject;

		// Destroy the Assimp IO stream
		delete pFile;
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
IOSystem::IOSystem() :
	m_pFile(nullptr),
	m_pszMagicDefaultFilename(nullptr),
	m_nMagicDefaultFilenameLength(0)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy constructor
*/
IOSystem::IOSystem(const IOSystem &cSource) :
	m_pFile(nullptr),
	m_pszMagicDefaultFilename(nullptr),
	m_nMagicDefaultFilenameLength(0)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
IOSystem &IOSystem::operator =(const IOSystem &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}

/**
*  @brief
*    Returns a real filename
*/
String IOSystem::GetRealFilename(const char *pszAssimpFile) const
{
	// Is the filename using the basename of the default?
	if (!strncmp(pszAssimpFile, MagicDefaultFilename, MagicDefaultFilenameLength)) {
		// Ok... we need to construct the real filename
		String sFilename = String::FromUTF8(pszAssimpFile);	// Get given filename
		sFilename.Delete(0, MagicDefaultFilenameLength);	// Cut of the magic thing
		sFilename.Insert(m_pFile->GetUrl().GetTitle(), 0);	// Insert the title of the complete filename

		// Return the real filename
		return sFilename;
	} else {
		// Return the given filename
		return String::FromUTF8(pszAssimpFile);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLAssimp
