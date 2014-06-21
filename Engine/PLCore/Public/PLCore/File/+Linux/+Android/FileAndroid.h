/*********************************************************\
 *  File: FileAndroid.h                                  *
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


#ifndef __PLCORE_FILE_ANDROID_H__
#define __PLCORE_FILE_ANDROID_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/File/FileImpl.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
struct AAsset;
typedef struct AAsset AAsset;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Android implementation of FileImpl
*
*  @note
*    - Implementation of the state design pattern, this class is a concrete state of the 'FileImpl'-state of the 'FileObject'-context
*    - This implementation is using the Android asset manager received from "SystemAndroid::GetAssetManager()"
*/
class FileAndroid : public FileImpl {


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
		FileAndroid(const Url &cUrl, const FileAccess *pAccess);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FileAndroid();


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
		String	 m_sFilename;	/**< File name (in Linux notation) */
		uint32	 m_nAccess;		/**< File access modes (see EAccess) */
		AAsset	*m_pAAsset;		/**< Pointer to the Android asset, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_FILE_ANDROID_H__
