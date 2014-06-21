/*********************************************************\
 *  File: Directory.h                                    *
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


#ifndef __PLCORE_DIRECTORY_H__
#define __PLCORE_DIRECTORY_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/File/FileObject.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class FileSearch;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Class for accessing a directory within the file system
*/
class Directory : public FileObject {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class FileSearch;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline Directory();

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sUrl
		*    URL of a directory
		*  @param[in] pAccess
		*    Additional file access information (can be a null pointer)
		*/
		inline Directory(const String &sUrl, const FileAccess *pAccess = nullptr);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cUrl
		*    URL of a directory
		*  @param[in] pAccess
		*    Additional file access information (can be a null pointer)
		*/
		inline Directory(const Url &cUrl, const FileAccess *pAccess = nullptr);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - The directory is closed automatically
		*/
		inline virtual ~Directory();

		/**
		*  @brief
		*    Create directory
		*
		*  @return
		*    'true', if all went fine, else 'false'
		*/
		inline bool Create();

		/**
		*  @brief
		*    Create a directory recursively
		*
		*  @return
		*    'true', if all went fine, else 'false'
		*/
		PLCORE_API bool CreateRecursive();

		/**
		*  @brief
		*    Delete directory
		*
		*  @return
		*    'true', if all went fine, else 'false'
		*/
		inline bool Delete();

		/**
		*  @brief
		*    Returns a file searcher for listing the content of a directory
		*
		*  @param[in] sFilter
		*    Filter to be applied (e.g. "*.txt")
		*
		*  @return
		*    Pointer to a new file searcher, a null pointer on error (has to be deleted be the caller)
		*/
		PLCORE_API FileSearch *Search(const String &sFilter) const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Directory(const Directory &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Directory &operator =(const Directory &cSource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/File/Directory.inl"


#endif // __PLCORE_DIRECTORY_H__
