/*********************************************************\
 *  File: Buffer.cpp                                     *
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
#include <PLCore/File/File.h>
#include <PLCore/Log/Log.h>
#include <PLCore/Tools/LoadableManager.h>
#include "PLSoundOpenAL/Ogg.h"
#include "PLSoundOpenAL/Wav.h"
#include "PLSoundOpenAL/StreamOgg.h"
#include "PLSoundOpenAL/StreamWav.h"
#include "PLSoundOpenAL/Source.h"
#include "PLSoundOpenAL/SoundManager.h"
#include "PLSoundOpenAL/Buffer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLSoundOpenAL {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
Buffer::~Buffer()
{
	// Unload sound
	Unload();
}

/**
*  @brief
*    Creates a stream
*/
Stream *Buffer::CreateStream(ALuint nSource)
{
	// Stream the buffer?
	if (m_bStream) {
		// Get filename extension
		String sExtension;
		if (m_pnData) {
			// Try to read wav header
			if (m_nDataSize >= sizeof(StreamWav::Header)) {
				StreamWav::Header sWavHeader;

				// Read wav header
				MemoryManager::Copy(&sWavHeader, m_pnData, sizeof(StreamWav::Header));

				// Is this wav file?
				sExtension = MemoryManager::Compare(reinterpret_cast<const char*>(sWavHeader.wave), "WAVE", 4) ? "ogg" : "wav";
			} else {
				sExtension = "ogg";
			}
		} else {
			sExtension = Url(m_sFilename).GetExtension();
		}

		// Create stream
		if (sExtension == "ogg")
			return new StreamOgg(nSource, *this);
		else if (sExtension == "wav")
			return new StreamWav(nSource, *this);
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Returns the OpenAL buffer
*/
ALuint Buffer::GetOpenALBuffer() const
{
	return m_nBuffer;
}

/**
*  @brief
*    Returns the buffer filename
*/
String Buffer::GetFilename() const
{
	return m_sFilename;
}

/**
*  @brief
*    Opens the file the buffer is using
*/
File *Buffer::OpenFile() const
{
	// Filename given?
	if (m_sFilename.GetLength()) {
		// Create the file object
		File *pFile = new File();

		// Try to open the file
		if (LoadableManager::GetInstance()->OpenFile(*pFile, m_sFilename, false))
			return pFile; // Return opened file object

		// Cleanup on error
		delete pFile;
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Returns the buffer data
*/
const uint8 *Buffer::GetData() const
{
	return m_pnData;
}

/**
*  @brief
*    Returns the buffer data size (in bytes)
*/
uint32 Buffer::GetDataSize() const
{
	return m_nDataSize;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Buffer::Buffer(PLSound::SoundManager &cSoundManager, const String &sName,  bool bStream) :
	PLSound::Buffer(cSoundManager, sName),
	m_bStream(false),
	m_nBuffer(0),
	m_pnData(nullptr),
	m_nDataSize(0)
{
	// Load the sound buffer
	if (sName.GetLength())
		LoadBuffer(sName, bStream);
}


//[-------------------------------------------------------]
//[ Public virtual PLSound::Buffer functions              ]
//[-------------------------------------------------------]
bool Buffer::LoadBuffer(const String &sFilename, bool bStream)
{
	// Check parameter
	if (sFilename.GetLength()) {
		// Unload old sound buffer
		Unload();

		// Load buffer
		m_bStream = bStream;
		if (m_bStream) {
			m_sFilename = sFilename;

			// Done
			return true;
		} else {
			// Create the buffer
			alGenBuffers(1, &m_nBuffer);
			ALuint nError = alGetError();
			if (nError == AL_NO_ERROR) {
				// Try to load ogg
				m_sFilename = sFilename;
				File *pFile = OpenFile();
				if (pFile) {
					ALsizei nFreq;
					ALenum  nFormat;
					Array<uint8> lstBufferData;
					if (LoadOGG(pFile, lstBufferData, nFormat, nFreq)) {
						// Fill buffer
						alBufferData(m_nBuffer, nFormat, &lstBufferData[0], lstBufferData.GetNumOfElements(), nFreq);
						nError = alGetError();
						if (nError == AL_NO_ERROR)
							return true; // Done
						else
							PL_LOG(Error, String("alBufferData: ") + alGetString(nError))
					} else {
						// Check whether this is a wav file
						const String sExtension = Url(sFilename).GetExtension();
						if (sExtension == "wav") {
							// Try to load wav
							if (LoadWav(pFile, lstBufferData, nFormat, nFreq)) {
								// Fill buffer
								alBufferData(m_nBuffer, nFormat, &lstBufferData[0], lstBufferData.GetNumOfElements(), nFreq);
								nError = alGetError();
								if (nError == AL_NO_ERROR)
									return true; // Done
								else
									PL_LOG(Error, String("alBufferData: ") + alGetString(nError))
							} else {
								delete pFile;
							}
						} else {
							delete pFile;
						}
					}
				}

				// Cleanup on error
				alDeleteBuffers(1, &m_nBuffer);
				m_nBuffer   = 0;
				m_sFilename = "";
			} else {
				PL_LOG(Error, String("alGenBuffers: ") + alGetString(nError))
			}
		}
	}

	// Error!
	return false;
}

bool Buffer::LoadBuffer(const uint8 nData[], uint32 nSize, bool bStream)
{
	// Check parameters
	if (nSize) {
		// Unload old sound buffer
		Unload();

		// Load buffer
		m_bStream = bStream;
		if (m_bStream) {
			m_pnData    = nData;
			m_nDataSize = nSize;

			// Done
			return true;
		} else {
			// Create the buffer
			alGenBuffers(1, &m_nBuffer);
			ALuint nError = alGetError();
			if (nError == AL_NO_ERROR) {
				// Try to load ogg
				ALenum  nFormat;
				ALsizei nFreq;
				Array<uint8> lstBufferData;
				if (LoadOGG(nData, nSize, lstBufferData, nFormat, nFreq)) {
					// Fill buffer
					alBufferData(m_nBuffer, nFormat, &lstBufferData[0], lstBufferData.GetNumOfElements(), nFreq);
					nError = alGetError();
					if (nError == AL_NO_ERROR)
						return true; // Done
					else
						PL_LOG(Error, String("alBufferData: ") + alGetString(nError))
				} else {
					// Try to load wav
					if (LoadWav(nData, nSize, lstBufferData, nFormat, nFreq)) {
						// Fill buffer
						alBufferData(m_nBuffer, nFormat, &lstBufferData[0], lstBufferData.GetNumOfElements(), nFreq);
						nError = alGetError();
						if (nError == AL_NO_ERROR)
							return true; // Done
						else
							PL_LOG(Error, String("alBufferData: ") + alGetString(nError))
					}
				}

				// Cleanup on error
				alDeleteBuffers(1, &m_nBuffer);
				m_nBuffer = 0;
			} else {
				PL_LOG(Error, String("alGenBuffers: ") + alGetString(nError))
			}
		}
	}

	// Error!
	return false;
}

bool Buffer::IsLoaded() const
{
	return (m_nBuffer != 0 || m_bStream);
}

bool Buffer::IsStreamed() const
{
	return m_bStream;
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Loadable functions             ]
//[-------------------------------------------------------]
bool Buffer::LoadByFilename(const String &sFilename, const String &sParams, const String &sMethod)
{
	return LoadBuffer(sFilename, false);
}

bool Buffer::Unload()
{
	// Is the buffer loaded?
	if (m_nBuffer || m_bStream) {
		// Stop playback of all sources using this buffer
		for (uint32 i=0; i<GetNumOfSources(); i++)
			GetSource(i)->Stop();

		// Unload buffer
		m_bStream   = false;
		m_sFilename = "";
		m_pnData    = nullptr;
		m_nDataSize = 0;
		if (m_nBuffer) {
			alDeleteBuffers(1, &m_nBuffer);
			m_nBuffer = 0;
		}
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSoundOpenAL
