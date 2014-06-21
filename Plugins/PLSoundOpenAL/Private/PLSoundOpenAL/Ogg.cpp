/*********************************************************\
 *  File: Ogg.cpp                                        *
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
#include <vorbis/vorbisfile.h>
#include <PLCore/File/File.h>
#include "PLSoundOpenAL/SoundManager.h"
#include "PLSoundOpenAL/Ogg.h"


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
*    This function loads a '.ogg' file into a memory buffer and returns
*    the format and frequency
*/
bool LoadOGG(File *pFile, Array<uint8> &lstBuffer, ALenum &nFormat, ALsizei &nFrequency)
{
	bool bResult = false; // Error by default
	if (pFile) {
		// Try opening the given file
		OggVorbis_File oggFile;
		ov_callbacks ovc;
		ovc.read_func  = &SoundManager::read_func;
		ovc.seek_func  = &SoundManager::seek_func;
		ovc.close_func = &SoundManager::close_func;
		ovc.tell_func  = &SoundManager::tell_func;
		if (ov_open_callbacks(pFile, &oggFile, nullptr, 0, ovc) == 0) {
			// Get some information about the OGG file
			vorbis_info *pInfo = ov_info(&oggFile, -1);

			// Check the number of channels... always use 16-bit samples
			nFormat = (pInfo->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

			// The frequency of the sampling rate
			nFrequency = pInfo->rate;

			// Set to no error
			bResult = true;

			// Keep reading until all is read
			lstBuffer.Resize(pFile->GetSize()*100, false);
			static const uint32 BufferSize = 8192;	// 8 KB buffers
			uint8 nArray[BufferSize];				// Local fixed size array
			int nEndian = 0;						// 0 for Little-Endian, 1 for Big-Endian
			int nBitStream;
			long nBytes;
			do {
				// Read up to a buffer's worth of decoded sound data
				nBytes = ov_read(&oggFile, reinterpret_cast<char*>(nArray), BufferSize, nEndian, 2, 1, &nBitStream);
				if (nBytes < 0) {
					// Error!
					bResult = false;
				} else {
					// Append to end of buffer
					lstBuffer.Add(nArray, nBytes);
				}
			} while (nBytes>0);

			// Cleanup (OpenAL takes over the file reference)
			ov_clear(&oggFile);
		}
	}

	// Done
	return bResult;
}

/**
*  @brief
*    This function loads a '.ogg' file into a memory buffer and returns
*    the format and frequency
*/
bool LoadOGG(const uint8 nData[], uint32 nSize, Array<uint8> &lstBuffer, ALenum &nFormat, ALsizei &nFrequency)
{
	bool bResult = false; // Error by default

	// Set data pointers
	SoundManager::MemData sMemData;
	sMemData.pnData  = nData;
	sMemData.pnDataE = sMemData.pnData + nSize;

	// Try opening the given file
	OggVorbis_File oggFile;
	ov_callbacks ovc;
	ovc.read_func  = &SoundManager::read_func_mem;
	ovc.seek_func  = &SoundManager::seek_func_mem;
	ovc.close_func = &SoundManager::close_func_mem;
	ovc.tell_func  = &SoundManager::tell_func_mem;
	if (!ov_open_callbacks(&sMemData, &oggFile, nullptr, 0, ovc)) {
		// Get some information about the OGG file
		vorbis_info *pInfo = ov_info(&oggFile, -1);

		// Check the number of channels... always use 16-bit samples
		nFormat = (pInfo->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

		// The frequency of the sampling rate
		nFrequency = pInfo->rate;

		// Set to no error
		bResult = true;

		// Keep reading until all is read
		static const uint32 BufferSize = 8192;	// 8 KB buffers
		uint8 nArray[BufferSize];				// Local fixed size array
		int nEndian = 0;						// 0 for Little-Endian, 1 for Big-Endian
		int nBitStream;
		long nBytes;
		do {
			// Read up to a buffer's worth of decoded sound data
			nBytes = ov_read(&oggFile, reinterpret_cast<char*>(nArray), BufferSize, nEndian, 2, 1, &nBitStream);
			if (nBytes < 0) {
				// Error!
				bResult = false;
			} else {
				// Append to end of buffer
				lstBuffer.Add(nArray, nBytes);
			}
		} while (nBytes>0);

		// Cleanup
		ov_clear(&oggFile);
	}

	// Done
	return bResult;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSoundOpenAL
