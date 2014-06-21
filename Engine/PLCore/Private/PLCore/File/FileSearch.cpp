/*********************************************************\
 *  File: FileSearch.cpp                                 *
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
#include "PLCore/File/Directory.h"
#include "PLCore/File/FileImpl.h"
#include "PLCore/File/FileSearchImpl.h"
#include "PLCore/File/SearchFilterWildcard.h"
#include "PLCore/File/FileSearch.h"


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
FileSearch::FileSearch(const Directory &cDirectory, SearchFilter *pFilter) :
	m_pDirectory(&cDirectory),
	m_pFileSearchImpl(nullptr),
	m_pFilterWildcard(nullptr),
	m_pFilter(pFilter)
{
	// Valid file implementation?
	if (m_pDirectory->m_pFileImpl) {
		// Create a new file search implementation instance for the given directory
		m_pFileSearchImpl = m_pDirectory->m_pFileImpl->CreateSearch();

		// Find first file
		FindNextFile();
	}
}

/**
*  @brief
*    Constructor
*/
FileSearch::FileSearch(const Directory &cDirectory, const String &sFilter) :
	m_pDirectory(&cDirectory),
	m_pFileSearchImpl(nullptr),
	m_pFilterWildcard(new SearchFilterWildcard(sFilter)),
	m_pFilter(m_pFilterWildcard)
{
	// Valid file implementation?
	if (m_pDirectory->m_pFileImpl) {
		// Create a new file search implementation instance for the given directory
		m_pFileSearchImpl = m_pDirectory->m_pFileImpl->CreateSearch();

		// Find first file
		FindNextFile();
	}
}

/**
*  @brief
*    Copy constructor
*/
FileSearch::FileSearch(const FileSearch &cFileSearch) :
	m_pDirectory(cFileSearch.m_pDirectory),
	m_pFileSearchImpl(nullptr),
	m_pFilterWildcard(nullptr),
	m_pFilter(nullptr)
{
	// Copy wildcard filter
	if (cFileSearch.m_pFilterWildcard) {
		m_pFilterWildcard = new SearchFilterWildcard(cFileSearch.m_pFilterWildcard->GetFilter());
		m_pFilter		  = m_pFilterWildcard;
	}

	// Valid file implementation?
	if (m_pDirectory->m_pFileImpl) {
		// Create a new file search implementation instance for the given directory
		m_pFileSearchImpl = m_pDirectory->m_pFileImpl->CreateSearch();

		// Find first file
		FindNextFile();
	}
}

/**
*  @brief
*    Destructor
*/
FileSearch::~FileSearch()
{
	// Delete wildcard filter
	if (m_pFilterWildcard)
		delete m_pFilterWildcard;

	// Delete file searcher implementation
	if (m_pFileSearchImpl)
		delete m_pFileSearchImpl;
}

/**
*  @brief
*    Copy operator
*/
FileSearch &FileSearch::operator =(const FileSearch &cFileSearch)
{
	// Set directory
	m_pDirectory = cFileSearch.m_pDirectory;

	// Delete wildcard filter
	if (m_pFilterWildcard) {
		delete m_pFilterWildcard;
		m_pFilterWildcard = nullptr;
	}

	// Delete file searcher implementation
	if (m_pFileSearchImpl) {
		delete m_pFileSearchImpl;
		m_pFileSearchImpl = nullptr;
	}

	// Reset the filter and the next filename
	m_pFilter       = nullptr;
	m_sNextFilename = "";

	// Copy wildcard filter
	if (cFileSearch.m_pFilterWildcard) {
		m_pFilterWildcard = new SearchFilterWildcard(cFileSearch.m_pFilterWildcard->GetFilter());
		m_pFilter		  = m_pFilterWildcard;
	}

	// Valid file implementation?
	if (m_pDirectory->m_pFileImpl) {
		// Create a new file search implementation instance for the given directory
		m_pFileSearchImpl = m_pDirectory->m_pFileImpl->CreateSearch();

		// Find first file
		FindNextFile();
	}

	// Return this instance
	return *this;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Finds the next file (according to the filter)
*/
bool FileSearch::FindNextFile()
{
	// Valid file search implementation?
	if (m_pFileSearchImpl) {
		// Find next file that matches the filter
		while (m_pFileSearchImpl->HasNextFile()) {
			m_sNextFilename = m_pFileSearchImpl->GetNextFile();

			// Check filter (if a filter is used)
			if (!m_pFilter || m_pFilter->CheckFile(m_sNextFilename)) {
				// Next file found
				return true;
			}
		}
	}

	// No more files
	m_sNextFilename = "";
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
