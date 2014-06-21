/*********************************************************\
 *  File: FileWindows.h                                  *
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


#ifndef __PLCORE_FILE_WINDOWS_H__
#define __PLCORE_FILE_WINDOWS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/File/StdIo.h"
#include "PLCore/File/FileImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Windows implementation of 'FileImpl'
*
*  @note
*    - Implementation of the state design pattern, this class is a concrete state of the 'FileImpl'-state of the 'FileObject'-context
*/
class FileWindows : public FileImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class FileObject;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cUrl
		*    URL of the file or directory
		*  @param[in] pAccess
		*    Additional file access information (can be a null pointer)
		*/
		FileWindows(const Url &cUrl, const FileAccess *pAccess);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FileWindows();


	//[-------------------------------------------------------]
	//[ Private virtual FileImpl functions                    ]
	//[-------------------------------------------------------]
	private:
		virtual bool Exists() const override;
		virtual bool IsFile() const override;
		virtual bool IsDirectory() const override;
		virtual bool CopyTo(const String &sDest, bool bOverwrite) const override;
		virtual bool MoveTo(const String &sDest) override;
		virtual bool Rename(const String &sName) override;
		virtual bool CreateNewFile(bool bAlways) override;
		virtual bool CreateNewDirectory() override;
		virtual bool Delete() override;
		virtual bool DeleteDirectory() override;
		virtual void Close() override;
		virtual bool Open(uint32 nAccess, String::EFormat nStringFormat = String::ASCII) override;
		virtual bool IsOpen() const override;
		virtual bool IsReadable() const override;
		virtual bool IsWritable() const override;
		virtual String::EFormat GetStringFormat() const override;
		virtual bool IsEof() const override;
		virtual int GetC() override;
		virtual bool PutC(int nChar) override;
		virtual String GetS() override;
		virtual int PutS(const String &sString) override;
		virtual uint32 Read(void *pBuffer, uint32 nSize, uint32 nCount) override;
		virtual uint32 Write(const void *pBuffer, uint32 nSize, uint32 nCount) override;
		virtual bool Flush() override;
		virtual bool Seek(int32 nOffset, uint32 nLocation) override;
		virtual int32 Tell() const override;
		virtual uint32 GetSize() const override;
		virtual FileSearchImpl *CreateSearch() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		String			 m_sFilename;		/**< File name (in Windows notation) */
		uint32			 m_nAccess;			/**< File access modes (see EAccess) */
		String::EFormat  m_nStringFormat;	/**< String encoding format to use when dealing with string functions */
		FILE			*m_pFile;			/**< Pointer to input stream, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_FILE_WINDOWS_H__
