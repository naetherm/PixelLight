/*********************************************************\
 *  File: FileSearch.h                                   *
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


#ifndef __PLCORE_FILESEARCH_H__
#define __PLCORE_FILESEARCH_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/String.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Directory;
class FileSearchImpl;
class SearchFilter;
class SearchFilterWildcard;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Class for searching inside a directory
*
*  @note
*    - Implementation of the bridge design pattern, this class is the abstraction
*/
class FileSearch {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cDirectory
		*    Directory to search in
		*  @param[in] pFilter
		*    Search filter to use (can be a null pointer)
		*/
		PLCORE_API FileSearch(const Directory &cDirectory, SearchFilter *pFilter = nullptr);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cDirectory
		*    Directory to search in
		*  @param[in] sFilter
		*    Wildcard search filter (e.g. "*.txt")
		*/
		PLCORE_API FileSearch(const Directory &cDirectory, const String &sFilter);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cFileSearch
		*    'FileSearch' to be copied
		*/
		PLCORE_API FileSearch(const FileSearch &cFileSearch);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API ~FileSearch();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cFileSearch
		*    'FileSearch' to be copied
		*
		*  @return
		*    Reference to this instance
		*/
		PLCORE_API FileSearch &operator =(const FileSearch &cFileSearch);

		/**
		*  @brief
		*    Returns whether the file searcher has a next file
		*
		*  @return
		*    'true' if the file searcher has a next file, else 'false'
		*/
		inline bool HasNextFile();

		/**
		*  @brief
		*    Returns the next filename
		*
		*  @return
		*    The next filename, empty on error
		*/
		inline String GetNextFile();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Finds the next file (according to the filter)
		*
		*  @return
		*    'true' if a next file is available
		*/
		PLCORE_API bool FindNextFile();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const Directory		 *m_pDirectory;			/**< The owning directory (always valid!) */
		FileSearchImpl		 *m_pFileSearchImpl;	/**< File searcher implementation, can be a null pointer */
		SearchFilterWildcard *m_pFilterWildcard;	/**< Automatically created wildcard filter, can be a null pointer */
		SearchFilter		 *m_pFilter;			/**< Search filter, can be a null pointer */
		String				  m_sNextFilename;		/**< Filename of next file */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/File/FileSearch.inl"


#endif // __PLCORE_FILESEARCH_H__
