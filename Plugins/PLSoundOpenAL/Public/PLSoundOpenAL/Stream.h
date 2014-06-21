/*********************************************************\
 *  File: Stream.h                                       *
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


#ifndef __PLSOUNDOPENAL_STREAM_H__
#define __PLSOUNDOPENAL_STREAM_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLSoundOpenAL/PLSoundOpenAL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLSoundOpenAL {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Buffer;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract sound stream
*/
class Stream {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nSource
		*    Sound source using this stream
		*  @param[in] cBuffer
		*    Sound buffer this stream is using
		*/
		Stream(ALuint nSource, const Buffer &cBuffer);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Stream();

		/**
		*  @brief
		*    Returns the sound source using this stream
		*
		*  @return
		*    The sound source using this stream
		*/
		ALuint GetSource() const;

		/**
		*  @brief
		*    Returns the sound buffer this stream is using
		*
		*  @return
		*    The sound buffer this stream is using
		*/
		const Buffer &GetBuffer() const;

		/**
		*  @brief
		*    Returns whether the stream is looping or not
		*
		*  @return
		*    'true' if stream is looping, else 'false'
		*/
		bool IsLooping() const;

		/**
		*  @brief
		*    Sets whether the stream is looping or not
		*
		*  @param[in] bLooping
		*    If 'true' the stream is looping, else 'false'
		*/
		void SetLooping(bool bLooping = false);


	//[-------------------------------------------------------]
	//[ Public virtual Stream functions                       ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Initializes the stream for playback
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool Init() = 0;

		/**
		*  @brief
		*    Returns whether or not the stream is currently initialized (and therefore running)
		*
		*  @return
		*    'true' if the stream is currently initialized, else 'false'
		*/
		virtual bool IsInitialized() const = 0;

		/**
		*  @brief
		*    De-initializes the stream (playback stopped)
		*/
		virtual void DeInit() = 0;

		/**
		*  @brief
		*    Updates the stream
		*
		*  @return
		*    'true' if the stream is still active, else 'false' (maybe finished)
		*/
		virtual bool Update() = 0;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ALuint		  m_nSource;	/**< Sound source using this stream */
		const Buffer *m_pBuffer;	/**< Sound buffer this stream is using (always valid!) */
		bool		  m_bLooping;	/**< Is the stream looping? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSoundOpenAL


#endif // __PLSOUNDOPENAL_STREAM_H__
