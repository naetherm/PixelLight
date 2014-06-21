/*********************************************************\
 *  File: Wav.cpp                                        *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Array.h>
#include <PLCore/File/File.h>
#include "PLSoundOpenAL/StreamWav.h" // For StreamWav::Header
#include "PLSoundOpenAL/Wav.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLSoundOpenAL {


//[-------------------------------------------------------]
//[ Global functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    This function loads a '.wav' file into a memory buffer and returns
*    the format and frequency
*/
bool LoadWav(File *pFile, Array<uint8> &lstBuffer, ALenum &nFormat, ALsizei &nFrequency)
{
	if (pFile) {
		// Read wav header
		StreamWav::Header sWavHeader;
		pFile->Read(&sWavHeader, sizeof(StreamWav::Header), 1);

		// Check if the wav-file has no compressed data
		if (sWavHeader.Format == 1) {
			// Check the number of channels... and bits per sample
			if (sWavHeader.Channels == 1)
				nFormat = (sWavHeader.BitsPerSample == 16) ? AL_FORMAT_MONO16   : AL_FORMAT_MONO8;
			else
				nFormat = (sWavHeader.BitsPerSample == 16) ? AL_FORMAT_STEREO16 : AL_FORMAT_STEREO8;

			// The frequency of the sampling rate
			nFrequency = sWavHeader.SamplesPerSec;

			// Keep reading until all is read
			static const uint32 BufferSize = 8192;	// 8 KB buffers
			uint8 nArray[BufferSize];				// Local fixed size array
			pFile->Seek(sizeof(StreamWav::Header));
			long nSize   = BufferSize;
			long nToRead = sWavHeader.dataSize;
			while (nToRead > 0) {
				if (pFile->IsEof()) {
					// Error!
					return false;
				}
				if (nToRead<BufferSize)
					nSize = nToRead;
				long nBytes = pFile->Read(nArray, 1, nSize);
				if (nBytes <= 0 && !pFile->IsEof()) {
					// Error!
					return false;
				}
				lstBuffer.Add(nArray, nBytes);
				nToRead -= nBytes;
			}

			// Close the file
			delete pFile;

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    This function loads a '.wav' file into a memory buffer and returns
*    the format and frequency
*/
bool LoadWav(const uint8 nData[], uint32 nSize, Array<uint8> &lstBuffer, ALenum &nFormat, ALsizei &nFrequency)
{
	// Check parameter
	if (nSize) {
		// Set stream size
		uint32 nSizeToRead = nSize - sizeof(StreamWav::Header);

		// Read wav header
		StreamWav::Header sWavHeader;
		MemoryManager::Copy(&sWavHeader, nData, sizeof(StreamWav::Header));

		// Check if the wav-file has no compressed data
		if (sWavHeader.Format == 1) {
			// Check the number of channels... and bits per sample
			if (sWavHeader.Channels == 1)
				nFormat = (sWavHeader.BitsPerSample == 16) ? AL_FORMAT_MONO16   : AL_FORMAT_MONO8;
			else
				nFormat = (sWavHeader.BitsPerSample == 16) ? AL_FORMAT_STEREO16 : AL_FORMAT_STEREO8;

			// The frequency of the sampling rate
			nFrequency = sWavHeader.SamplesPerSec;

			// Copy the data to the buffer
			const uint8 *pnTemp = nData;
			lstBuffer.Add(pnTemp + sizeof(StreamWav::Header), nSizeToRead);

			// Done
			return true;
		}
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSoundOpenAL
