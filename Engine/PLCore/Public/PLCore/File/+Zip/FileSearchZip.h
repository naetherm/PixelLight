/*********************************************************\
 *  File: FileSearchZip.h                                *
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


#ifndef __PLCORE_FILESEARCH_ZIP_H__
#define __PLCORE_FILESEARCH_ZIP_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/File/+Zip/ZipHandle.h"
#include "PLCore/File/FileSearchImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    ZIP implementation of 'FileSearchImpl'
*/
class FileSearchZip : public FileSearchImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class FileSearch;
	friend class FileZip;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sZipFile
		*    Path to the ZIP-file
		*  @param[in] sPathInZip
		*    Path inside the ZIP-file
		*  @param[in] pAccess
		*    Additional file access information (can be a null pointer)
		*
		*  @note
		*    - sPathInZip *must* end with '/'!
		*/
		FileSearchZip(const String &sZipFile, const String &sPathInZip, const FileAccess *pAccess);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FileSearchZip();


	//[-------------------------------------------------------]
	//[ Private virtual FileSearchImpl functions              ]
	//[-------------------------------------------------------]
	private:
		virtual bool HasNextFile() override;
		virtual String GetNextFile() override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Find next file in the selected directory
		*/
		bool FindNextFile();
		bool CheckFile(const String &sFilename);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		String	  m_sZipFile;	/**< File name of ZIP-file */
		String	  m_sPathInZip;	/**< Path inside the ZIP-file */
		ZipHandle m_cZipFile;	/**< ZIP file handle */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_FILESEARCH_ZIP_H__
