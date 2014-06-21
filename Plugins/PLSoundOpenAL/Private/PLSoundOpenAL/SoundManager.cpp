/*********************************************************\
 *  File: SoundManager.cpp                               *
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
#include <stdio.h>
#include <PLCore/File/File.h>
#include <PLCore/Log/Log.h>
#include <PLCore/Tools/Localization.h>
#include "PLSoundOpenAL/Buffer.h"
#include "PLSoundOpenAL/Source.h"
#include "PLSoundOpenAL/SoundManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLSoundOpenAL {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SoundManager)


//[-------------------------------------------------------]
//[ Custom static OpenAL file callbacks                   ]
//[-------------------------------------------------------]
// From PLCore::File
size_t SoundManager::read_func(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	return datasource ? static_cast<size_t>(static_cast<File*>(datasource)->Read(ptr, 1, static_cast<long>(size*nmemb))) : 0;
}

int SoundManager::seek_func(void *datasource, ogg_int64_t offset, int whence)
{
	if (datasource) {
		switch (whence) {
			case SEEK_SET:
				return !static_cast<File*>(datasource)->Seek(static_cast<uint32>(offset), File::SeekSet);

			case SEEK_CUR:
				return !static_cast<File*>(datasource)->Seek(static_cast<uint32>(offset), File::SeekCurrent);

			case SEEK_END:
				return !static_cast<File*>(datasource)->Seek(static_cast<uint32>(offset), File::SeekEnd);
		}
	}

	// Error!
	return -1;
}

int SoundManager::close_func(void *datasource)
{
	if (datasource) {
		File *pFile = static_cast<File*>(datasource);
		pFile->Close();
		delete pFile;
	}

	return 0;
}

long SoundManager::tell_func(void *datasource)
{
	return datasource ? static_cast<File*>(datasource)->Tell() : -1;
}

// From memory
size_t SoundManager::read_func_mem(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	if (datasource && static_cast<MemData*>(datasource)->pnData+size*nmemb < static_cast<MemData*>(datasource)->pnDataE) {
		MemoryManager::Copy(ptr, static_cast<MemData*>(datasource)->pnData, static_cast<uint32>(size*nmemb));
		static_cast<MemData*>(datasource)->pnData += size*nmemb;
		return size*nmemb;
	} else {
		// Error!
		return 0;
	}
}

int SoundManager::seek_func_mem(void *datasource, ogg_int64_t offset, int whence)
{
	return -1;
}

int SoundManager::close_func_mem(void *datasource)
{
	// We do not need to close/destroy 'datasource'!
	return 0;
}

long SoundManager::tell_func_mem(void *datasource)
{
	return 0;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SoundManager::SoundManager() :
	DeviceName(this),
	m_pDevice(nullptr),
	m_pContext(nullptr),
	m_nEAXLevel(0),
	m_fPitch(1.0f)
{
	PL_LOG(Info, "Create sound manager: " + GetDescription())

	// Initialize listener attributes
	m_vListenerAttributes[ListenerForward].SetXYZ(0.0f, 0.0f, 1.0f);
	m_vListenerAttributes[ListenerUpward]. SetXYZ(0.0f, 1.0f, 0.0f);
}

/**
*  @brief
*    Destructor
*/
SoundManager::~SoundManager()
{
	PL_LOG(Info, "Destroy sound manager: " + GetDescription())

	// De-initialize
	DeInit();
}

/**
*  @brief
*    Adds an active sound source to the sound manager
*/
bool SoundManager::AddActiveSource(PLSound::Source &cSource)
{
	return (!m_lstActiveSources.IsElement(&cSource) && m_lstActiveSources.Add(&cSource) != nullptr);
}

/**
*  @brief
*    Removes an active sound source from the sound manager
*/
bool SoundManager::RemoveActiveSource(PLSound::Source &cSource)
{
	return m_lstActiveSources.Remove(&cSource);
}


//[-------------------------------------------------------]
//[ Public virtual PLSound::SoundManager functions        ]
//[-------------------------------------------------------]
String SoundManager::GetDescription() const
{
	static const String sString = "OpenAL sound";
	return sString;
}

bool SoundManager::GetFormatList(List<Format> &lstList) const
{
	// List of supported file formats:
	static const char *szSupportedFormats[] = {
		// Extension	Description
		"WAV",			"Microsoft Wave files",
		"OGG",			"Ogg Vorbis format",
		"\0"			// Notes the end of the list
	};

	// Add all supported file formats to the given list
	for (uint32 i=0; *szSupportedFormats[i] != '\0'; i+=2) {
		{ // Lower case
			Format &cFormat = lstList.Add();
			if (&cFormat != &List<Format>::Null) {
				cFormat.sFormat = szSupportedFormats[i];
				cFormat.sFormat.ToLower();
				cFormat.sDescription = szSupportedFormats[i+1];
			}
		}

		{ // Upper case
			Format &cFormat = lstList.Add();
			if (&cFormat != &List<Format>::Null) {
				cFormat.sFormat = szSupportedFormats[i];
				cFormat.sFormat.ToUpper();
				cFormat.sDescription = szSupportedFormats[i+1];
			}
		}
	}

	// Done
	return true;
}

float SoundManager::GetVolume() const
{
	ALfloat fValue;
	alGetListenerf(AL_GAIN, &fValue);
	return fValue;
}

void SoundManager::SetVolume(float fVolume)
{
	alListenerf(AL_GAIN, fVolume);
}

float SoundManager::GetPitch() const
{
	return m_fPitch;
}

void SoundManager::SetPitch(float fPitch)
{
	// Check and set pitch -> no change = nothing to do :)
	if (m_fPitch != fPitch) {
		m_fPitch = fPitch;

		// Update the pitch of all active sources
		for (uint32 i=0; i<m_lstActiveSources.GetNumOfElements(); i++) {
			PLSound::Source *pSource = m_lstActiveSources[i];
			pSource->SetPitch(pSource->GetPitch());
		}
	}
}

float SoundManager::GetDopplerFactor() const
{
	float fValue;
	alGetFloatv(AL_DOPPLER_FACTOR, &fValue);
	return fValue;
}

void SoundManager::SetDopplerFactor(float fFactor)
{
	alDopplerFactor(fFactor);
}


//[-------------------------------------------------------]
//[ Create sound buffer/source                            ]
//[-------------------------------------------------------]
PLSound::Buffer *SoundManager::CreateSoundBuffer(const String &sFilename, bool bStream)
{
	// Is there already a sound buffer with this name?
	PLSound::Buffer *pSB = GetByName(sFilename);
	return pSB ? pSB : new Buffer(*this, sFilename, bStream);
}

PLSound::Source *SoundManager::CreateSoundSource(PLSound::Buffer *pSoundBuffer)
{
	// Create the OpenAL sound source
	PLSound::Source *pSS = new Source(*this);
	if (pSoundBuffer)
		pSS->Load(pSoundBuffer);

	// Return the created sound source
	return pSS;
}


//[-------------------------------------------------------]
//[ Listener                                              ]
//[-------------------------------------------------------]
Vector3 SoundManager::GetListenerAttribute(EListener nAttribute) const
{
	return m_vListenerAttributes[nAttribute];
}

bool SoundManager::SetListenerAttribute(EListener nAttribute, const Vector3 &vV)
{
	// Set attribute
	m_vListenerAttributes[nAttribute] = vV;

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::ResourceManager functions      ]
//[-------------------------------------------------------]
bool SoundManager::Init()
{
	// Create device
	if (DeviceName.GetString().GetLength()) {
		// Have a name from enumeration process above, so use it...
		m_pDevice = alcOpenDevice(static_cast<const ALchar*>(DeviceName.GetString().GetASCII()));
	} else {
		// This is supposed to select the "preferred device"
		m_pDevice = alcOpenDevice(nullptr);
	}
	if (m_pDevice) {
		// Create context
		m_pContext = alcCreateContext(m_pDevice, nullptr);
		if (m_pContext) {
			// Set active context
			alcGetError(m_pDevice);
			alcMakeContextCurrent(m_pContext);
			if (alcGetError(m_pDevice) == ALC_NO_ERROR) {
				// Clear error code
				alGetError();
				alcGetError(m_pDevice);

				// Global settings
				alListenerf(AL_GAIN, 1.0);
				alDopplerFactor(1.0);	// Don't exaggerate doppler shift
				alDopplerVelocity(340); // Using meters/second

				// Determine EAX support level
				m_nEAXLevel = 0;
			//#define USE_EAX
				#ifdef USE_EAX
					if (alIsExtensionPresent(static_cast<ALubyte*>("EAX2.0")) == AL_TRUE) {
						m_nEAXLevel = 2;
					} else {
						if (alIsExtensionPresent(static_cast<ALubyte*>("EAX")) == AL_TRUE)
							m_nEAXLevel = 1;
					}

					// Set EAX environment if EAX is available
					if (m_nEAXLevel != 0) {
						EAXSet pfPropSet = static_cast<EAXSet>(alGetProcAddress(static_cast<ALubyte*>("EAXSet")));
						if (pfPropSet != nullptr) {
							long nGlobalReverb = -10000;
							pfPropSet(&DSPROPSETID_EAX_ListenerProperties, DSPROPERTY_EAXLISTENER_ROOM,		   0, &nGlobalReverb, sizeof(unsigned long));
							unsigned long nEAXValue = EAX_ENVIRONMENT_GENERIC;
							pfPropSet(&DSPROPSETID_EAX_ListenerProperties, DSPROPERTY_EAXLISTENER_ENVIRONMENT, 0, &nEAXValue,	  sizeof(unsigned long));
						}
					}
				#endif // USE_EAX

				// Done
				return true;
			} else {
				// Log error output
				ALenum nError = alGetError();
				if (nError != AL_NO_ERROR)
					PL_LOG(Error, String("Failed to make context current! Error: ") + alGetString(nError))
				else
					PL_LOG(Error, "Failed to make context current!")
			}
		} else {
			// Log error output
			ALenum nError = alGetError();
			if (nError != AL_NO_ERROR)
				PL_LOG(Error, String("Failed to initialize OpenAL! Error: ") + alGetString(nError))
			else
				PL_LOG(Error, "Failed to initialize OpenAL!")
		}

		// Cleanup
		DeInit();
	} else {
		// Log error output
		ALenum nError = alGetError();
		if (nError != AL_NO_ERROR)
			PL_LOG(Error, String("Failed to initialize OpenAL! Error: ") + alGetString(nError))
		else
			PL_LOG(Error, "Failed to initialize OpenAL!")
	}

	// Error!
	return false;
}

bool SoundManager::DeInit()
{
	// Destroy all resources - the resources unregister automatically from the list, so, this while loop will terminate!
	while (m_lstResources.GetNumOfElements())
		delete m_lstResources[0];
	m_lstResources.Clear();

	// Call base implementation
	PLSound::SoundManager::DeInit();

	// Disable context
	alcMakeContextCurrent(nullptr);

	// Release context
	if (m_pContext) {
		alcDestroyContext(m_pContext);
		m_pContext = nullptr;
	}

	// Close device
	if (m_pDevice) {
		alcCloseDevice(m_pDevice);
		m_pDevice = nullptr;
	}

	// Done
	return true;
}

bool SoundManager::Update()
{
	// Setup listener
	alListenerfv(AL_POSITION, m_vListenerAttributes[ListenerPosition]);
	alListenerfv(AL_VELOCITY, m_vListenerAttributes[ListenerVelocity]);
	Vector3 &vAt = m_vListenerAttributes[ListenerForward];
	Vector3 &vUp = m_vListenerAttributes[ListenerUpward];
	ALfloat fListenerOri[] = {vAt.x, vAt.y, vAt.z,	// "at"
							  vUp.x, vUp.y, vUp.z};	// "up"
	alListenerfv(AL_ORIENTATION, fListenerOri);

	// Check active sources
	for (uint32 i=0; i<m_lstActiveSources.GetNumOfElements(); i++) {
		Source *pSource = static_cast<Source*>(m_lstActiveSources[i]);
		pSource->Update();
		if (!pSource->IsPlaying()) {
			// Remove the inactive sound source
			m_lstActiveSources.RemoveAtIndex(i);
			i--;
		}
	}

	// Update statistics
	m_sStatistics.nNumOfActiveSources = m_lstActiveSources.GetNumOfElements();

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::ResourceManager functions     ]
//[-------------------------------------------------------]
PLSound::Buffer *SoundManager::CreateResource(const String &sName)
{
	return CreateSoundBuffer(sName);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSoundOpenAL
