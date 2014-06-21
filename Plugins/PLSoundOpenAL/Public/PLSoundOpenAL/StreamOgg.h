/*********************************************************\
 *  File: StreamOgg.h                                    *
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


#ifndef __PLSOUNDOPENAL_STREAMOGG_H__
#define __PLSOUNDOPENAL_STREAMOGG_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <vorbis/vorbisfile.h>
#include "PLSoundOpenAL/Stream.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLSoundOpenAL {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Ogg stream
*
*  @remarks
*    Ogg Vorbis (or just OGG) is an open-source audio compression format similar to MP3.
*    Actually, the file .ogg file format can contain other things, but let's just assume
*    it is audio data in here. One of the biggest advantages it has over MP3 is that it
*    is patent-free. This means you do not need to pay a license fee in order to encode
*    or decode OGG files.
*/
class StreamOgg : public Stream {


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
		StreamOgg(ALuint nSource, const Buffer &cBuffer);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~StreamOgg();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Opens the stream
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool OpenStream();

		/**
		*  @brief
		*    Fill a buffer
		*
		*  @param[in] nBuffer
		*    Buffer to fill
		*
		*  @return
		*    'true' if all went fine, else 'false' (end of stream reached?)
		*/
		bool FillBuffer(ALuint nBuffer);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		OggVorbis_File  m_cOggStream;	/**< Ogg stream */
		vorbis_info	   *m_pVorbisInfo;	/**< Vorbis information, can be a null pointer */
		ALenum		    m_nFormat;		/**< Format */
		ALuint		    m_nBuffers[2];	/**< Back and front buffer */
		PLCore::uint32  m_nSwapSize;	/**< Swap buffer size in bytes */
		PLCore::uint8  *m_pnSwap;		/**< Buffer used within FillBuffer(), can be a null pointer */


	//[-------------------------------------------------------]
	//[ Public virtual Stream functions                       ]
	//[-------------------------------------------------------]
	public:
		virtual bool Init() override;
		virtual bool IsInitialized() const override;
		virtual void DeInit() override;
		virtual bool Update() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSoundOpenAL


#endif // __PLSOUNDOPENAL_STREAMOGG_H__
