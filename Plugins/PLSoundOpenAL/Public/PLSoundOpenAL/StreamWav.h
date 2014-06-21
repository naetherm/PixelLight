/*********************************************************\
 *  File: StreamWav.h                                    *
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


#ifndef __PLSOUNDOPENAL_STREAMWAV_H__
#define __PLSOUNDOPENAL_STREAMWAV_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLSoundOpenAL/Stream.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class File;
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
*    Wav stream
*/
class StreamWav : public Stream {


	//[-------------------------------------------------------]
	//[ Public structures                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Header of the 'wav' file format
		*/
		typedef struct {
			ALubyte		riff[4];
			ALsizei		riffSize;
			ALubyte		wave[4];
			ALubyte		fmt[4];
			ALuint		fmtSize;
			ALushort	Format;
			ALushort	Channels;
			ALuint		SamplesPerSec;
			ALuint		BytesPerSec;
			ALushort	BlockAlign;
			ALushort	BitsPerSample;
			ALubyte		data[4];
			ALuint		dataSize;
		} Header;


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
		StreamWav(ALuint nSource, const Buffer &cBuffer);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~StreamWav();


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
		PLCore::File	    *m_pFile;		/**< Used on file streaming, can be a null pointer */
		const PLCore::uint8 *m_pnData;		/**< Used on memory streaming, can be a null pointer */
		ALuint				 m_nStreamSize;	/**< Stream size */
		ALuint				 m_nStreamPos;	/**< Current stream position */
		ALenum				 m_nFormat;		/**< Format */
		ALuint				 m_nFrequency;	/**< Frequency */
		ALuint				 m_nBuffers[2];	/**< Back and front buffer */
		PLCore::uint32		 m_nSwapSize;	/**< Swap buffer size in bytes */
		PLCore::uint8	    *m_pnSwap;		/**< Buffer used within FillBuffer(), can be a null pointer */


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


#endif // __PLSOUNDOPENAL_STREAMWAV_H__
