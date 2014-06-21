/*********************************************************\
 *  File: Buffer.h                                       *
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


#ifndef __PLSOUNDOPENAL_BUFFER_H__
#define __PLSOUNDOPENAL_BUFFER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLSound/Buffer.h>
#include "PLSoundOpenAL/PLSoundOpenAL.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class File;
}
namespace PLSoundOpenAL {
	class Stream;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLSoundOpenAL {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    OpenAL sound buffer resource
*/
class Buffer : public PLSound::Buffer {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SoundManager;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Buffer();

		/**
		*  @brief
		*    Creates a stream
		*
		*  @param[in] nSource
		*    Sound source using this stream
		*
		*  @return
		*    The created stream, a null pointer on error (maybe buffer isn't streamed)
		*/
		Stream *CreateStream(ALuint nSource);

		/**
		*  @brief
		*    Returns the OpenAL buffer
		*
		*  @return
		*    The OpenAL buffer
		*/
		ALuint GetOpenALBuffer() const;

		/**
		*  @brief
		*    Returns the buffer filename
		*
		*  @return
		*    The buffer filename
		*
		*  @note
		*    - Only valid if this buffer is a stream
		*/
		PLCore::String GetFilename() const;

		/**
		*  @brief
		*    Opens the file the buffer is using
		*
		*  @return
		*    The file the buffer is using, a null pointer on error
		*
		*  @note
		*    - The returned file is already opened for reading, don't forget
		*      to destroy the file object if you no longer need it
		*/
		PLCore::File *OpenFile() const;

		/**
		*  @brief
		*    Returns the buffer data
		*
		*  @return
		*    The buffer data
		*
		*  @note
		*    - If the data is a null pointer, the stream was loaded using a filename
		*
		*  @see
		*    - GetFilename()
		*/
		const PLCore::uint8 *GetData() const;

		/**
		*  @brief
		*    Returns the buffer data size (in bytes)
		*
		*  @return
		*    The buffer data size (in bytes)
		*
		*  @note
		*    - If the data is a null pointer, the stream was loaded using a filename
		*
		*  @see
		*    - GetData()
		*/
		PLCore::uint32 GetDataSize() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cSoundManager
		*    Owner sound manager
		*  @param[in] sName
		*    Resource name to set
		*  @param[in] bStream
		*    Stream the file?
		*/
		Buffer(PLSound::SoundManager &cSoundManager, const PLCore::String &sName = "", bool bStream = false);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool				 m_bStream;		/**< Is the buffer a stream? */
		ALuint				 m_nBuffer;		/**< Sound buffer */
		// For streaming
		PLCore::String		 m_sFilename;	/**< Filename */
		const PLCore::uint8 *m_pnData;		/**< Data, can be a null pointer */
		PLCore::uint32		 m_nDataSize;	/**< Data size (in bytes) */


	//[-------------------------------------------------------]
	//[ Public virtual PLSound::Buffer functions              ]
	//[-------------------------------------------------------]
	public:
		virtual bool LoadBuffer(const PLCore::String &sFilename, bool bStream) override;
		virtual bool LoadBuffer(const PLCore::uint8 nData[], PLCore::uint32 nSize, bool bStream = false) override;
		virtual bool IsLoaded() const override;
		virtual bool IsStreamed() const override;


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::Loadable functions             ]
	//[-------------------------------------------------------]
	public:
		virtual bool LoadByFilename(const PLCore::String &sFilename, const PLCore::String &sParams = "", const PLCore::String &sMethod = "") override;
		virtual bool Unload() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSoundOpenAL


#endif // __PLSOUNDOPENAL_BUFFER_H__
