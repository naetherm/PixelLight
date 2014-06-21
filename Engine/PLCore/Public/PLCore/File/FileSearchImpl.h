/*********************************************************\
 *  File: FileSearchImpl.h                               *
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


#ifndef __PLCORE_FILESEARCH_IMPL_H__
#define __PLCORE_FILESEARCH_IMPL_H__
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
class FileSearch;
class FileAccess;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract base class for platform specific file search implementations
*
*  @note
*    - Implementation of the bridge design pattern, this class is the implementor of the 'FileSearch' abstraction
*/
class FileSearchImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class FileSearch;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pAccess
		*    Additional file access information (can be a null pointer)
		*/
		PLCORE_API FileSearchImpl(const FileAccess *pAccess);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~FileSearchImpl();


	//[-------------------------------------------------------]
	//[ Protected virtual FileSearchImpl functions            ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Returns whether the file searcher has a next file
		*
		*  @return
		*    'true' if the file searcher has a next file, else 'false'
		*/
		virtual bool HasNextFile() = 0;

		/**
		*  @brief
		*    Returns the next filename
		*
		*  @return
		*    The next filename, empty on error
		*
		*  @note
		*    - Only the filename, no path shall be returned
		*    - No '/' at the end of the file name shall be added for directories
		*/
		virtual String GetNextFile() = 0;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		const FileAccess *m_pAccess;	/**< File access information, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_FILESEARCH_IMPL_H__
