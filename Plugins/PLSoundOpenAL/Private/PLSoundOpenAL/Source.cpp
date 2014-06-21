/*********************************************************\
 *  File: Source.cpp                                     *
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
#include <PLCore/Log/Log.h>
#include "PLSoundOpenAL/SoundManager.h"
#include "PLSoundOpenAL/Stream.h"
#include "PLSoundOpenAL/Buffer.h"
#include "PLSoundOpenAL/Source.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLSoundOpenAL {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
Source::~Source()
{
	// Unload buffer
	Unload();

	// Delete source
	if (m_nSource)
		alDeleteSources(1, &m_nSource);
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Source::Source(PLSound::SoundManager &cSoundManager) : PLSound::Source(cSoundManager),
	m_nSource(0),
	m_pStream(nullptr),
	m_b2D(false),
	m_bLooping(false),
	m_fPitch(1.0f),
	m_fReferenceDistance(1.0f),
	m_fMaxDistance(10000.0f),
	m_fRolloffFactor(1.0f)
{
	// Generate source
	alGenSources(1, &m_nSource);
	ALuint nError = alGetError();
	if (nError != AL_NO_ERROR)
		PL_LOG(Error, String("alGenSources: ") + alGetString(nError))

	// Init data
	alSourcei(m_nSource, AL_LOOPING, m_bLooping);
}

/**
*  @brief
*    Updates the source
*/
void Source::Update()
{
	if (m_pStream) {
		// If the source is looping and the playback is finished, start again
		m_pStream->Update();
	}
}


//[-------------------------------------------------------]
//[ Public virtual PLSound::Source functions              ]
//[-------------------------------------------------------]
bool Source::Load(PLSound::Buffer *pBuffer)
{
	// Call base implementation
	PLSound::Source::Load(pBuffer);
	if (pBuffer) {
		// Load the buffer
		if (pBuffer->IsStreamed()) {
			m_pStream = static_cast<Buffer*>(pBuffer)->CreateStream(m_nSource);
			if (!m_pStream)
				return false; // Error!
		} else {
			alSourcei(m_nSource, AL_BUFFER, static_cast<Buffer*>(pBuffer)->GetOpenALBuffer());
			ALuint nError = alGetError();
			if (nError != AL_NO_ERROR) {
				PL_LOG(Error, String("alSourcei: ") + alGetString(nError))
				Unload();

				// Error!
				return false;
			}
		}

		// Set 2D-state, this method also warns in case a multi channel buffer should be used for 3D spatialization (OpenAL doesn't support this)
		if (!m_b2D) {
			m_b2D = true; // Force a state update
			Set2D(false);
		}
	}

	// Done
	return true;
}

void Source::Unload()
{
	// Unload the buffer
	PLSound::Buffer *pBuffer = GetBuffer();
	if (pBuffer) {
		Stop();
		if (m_pStream) {
			delete m_pStream;
			m_pStream = nullptr;
		} else {
			alSourcei(m_nSource, AL_BUFFER, 0);
		}
	}

	// Call base implementation
	PLSound::Source::Unload();
}

bool Source::Play(bool bRestart)
{
	// Is a buffer loaded?
	if (!GetBuffer())
		return false; // Error!

	// Check if the source is already playing
	if (IsPlaying()) {
		// Restart source?
		if (bRestart) {
			if (m_pStream)
				m_pStream->Init();
			else
				alSourceRewind(m_nSource);
		}
	} else {
		// Init stream?
		if (m_pStream && !m_pStream->Init())
			return false; // Error!

		// Add source to sound manager
		static_cast<SoundManager&>(GetSoundManager()).AddActiveSource(*this);
	}

	// Setup pitch
	float fPitch = m_fPitch;
	m_fPitch = -1.0f;
	SetPitch(fPitch);

	// Play the source
	if (m_pStream) {
		m_pStream->SetLooping(m_bLooping);
		alSourcei(m_nSource, AL_LOOPING, false);
	} else {
		alSourcei(m_nSource, AL_LOOPING, m_bLooping);
	}
	alSourcePlay(m_nSource);

	// Done
	return (alGetError() == AL_NO_ERROR);
}

bool Source::IsPlaying() const
{
	// Is a buffer loaded?
	if (GetBuffer()) {
		// Get source state
		ALint nValue = AL_STOPPED;
		alGetSourcei(m_nSource, AL_SOURCE_STATE, &nValue);

		// Return state
		return (nValue == AL_PLAYING || nValue == AL_PAUSED);
	} else {
		// Not playing
		return false;
	}
}

void Source::Pause()
{
	// Is the source currently playing?
	if (IsPlaying()) {
		// Pause now
		alSourcePause(m_nSource);
	}
}

bool Source::IsPaused() const
{
	// Is a buffer loaded?
	if (GetBuffer()) {
		// Get source state
		ALint nValue = AL_STOPPED;
		alGetSourcei(m_nSource, AL_SOURCE_STATE, &nValue);

		// Return state
		return (nValue == AL_PAUSED);
	} else {
		// Not paused
		return false;
	}
}

void Source::Stop()
{
	// Is the source currently playing? (using 'IsPlaying()' to test this is NOT correct in here!)
	if (GetBuffer()) {
		// Stop playback
		alSourceStop(m_nSource);
		if (m_pStream)
			m_pStream->DeInit();

		// Remove source from sound manager
		static_cast<SoundManager&>(GetSoundManager()).RemoveActiveSource(*this);
	}
}

float Source::GetVolume() const
{
	ALfloat fValue = 1.0f;
	alGetSourcef(m_nSource, AL_GAIN, &fValue);
	return fValue;
}

void Source::SetVolume(float fVolume)
{
	alSourcef(m_nSource, AL_GAIN, fVolume);
}

bool Source::Is2D() const
{
	return m_b2D;
}

void Source::Set2D(bool b2D)
{
	// State change?
	if (m_b2D != b2D) {
		// Backup the setting...
		m_b2D = b2D;

		// Set relative to listener state, default is false (because we place 2D sounds at the center of the listener)
		alSourcei(m_nSource, AL_SOURCE_RELATIVE, m_b2D ? AL_TRUE : AL_FALSE);

		// ... and refresh the roll off factor, position and velocity!
		SetRolloffFactor(GetRolloffFactor());
		SetAttribute(Position, GetAttribute(Position));
		SetAttribute(Velocity, GetAttribute(Velocity));

		// "OpenAL Programmer's Guide - OpenAL Versions 1.0 and 1.1" (http://connect.creativelabs.com/openal/Documentation/OpenAL_Programmers_Guide.pdf)
		// states for the "alBufferData"-documentation on page 21: "Buffers containing more than one channel of data will be played without 3D spatialization."
		// -> Check for this and write a warning into the log in case a multi channel buffer should be used for 3D spatialization (so the developer has a hint what's going on)
		if (!m_b2D) {
			// Get the buffer the source is using
			const Buffer *pBuffer = static_cast<Buffer*>(GetBuffer());
			if (pBuffer) {
				// Get the used OpenAL buffer
				const ALint nOpenALBuffer = pBuffer->GetOpenALBuffer();
				if (nOpenALBuffer) {
					// Get and check the number of channels the buffer is using
					ALint nNumOfChannels = 0;
					alGetBufferi(nOpenALBuffer, AL_CHANNELS, &nNumOfChannels);
					if (nNumOfChannels > 1)
						PL_LOG(Warning, String("Sound buffer \"") + pBuffer->GetFilename() + "\" has multiple channels and should be used for 3D spatialization, this is not supported by OpenAL (3D spatialization is disabled)")
				}
			}
		}
	}
}

bool Source::IsLooping() const
{
	return m_bLooping;
}

void Source::SetLooping(bool bLooping)
{
	m_bLooping = bLooping;
	if (m_pStream) {
		m_pStream->SetLooping(m_bLooping);
		alSourcei(m_nSource, AL_LOOPING, false);
	} else {
		alSourcei(m_nSource, AL_LOOPING, m_bLooping);
	}
}

float Source::GetPitch() const
{
	return m_fPitch;
}

void Source::SetPitch(float fPitch)
{
	m_fPitch = fPitch;
	if (GetFlags() & NoMasterPitch)
		alSourcef(m_nSource, AL_PITCH, m_fPitch);
	else
		alSourcef(m_nSource, AL_PITCH, m_fPitch*GetSoundManager().GetPitch());
}

float Source::GetReferenceDistance() const
{
	return m_fReferenceDistance;
}

void Source::SetReferenceDistance(float fReferenceDistance)
{
	m_fReferenceDistance = fReferenceDistance;
	alSourcef(m_nSource, AL_REFERENCE_DISTANCE, m_fReferenceDistance);
}

float Source::GetMaxDistance() const
{
	return m_fMaxDistance;
}

void Source::SetMaxDistance(float fMaxDistance)
{
	m_fMaxDistance = fMaxDistance;
	alSourcef(m_nSource, AL_MAX_DISTANCE, m_fMaxDistance);
}

float Source::GetRolloffFactor() const
{
	return m_fRolloffFactor;
}

void Source::SetRolloffFactor(float fRolloffFactor)
{
	m_fRolloffFactor = fRolloffFactor;
	alSourcef(m_nSource, AL_ROLLOFF_FACTOR, m_b2D ? 0.0f : m_fRolloffFactor); // Do not perform attenuation for 2D sounds!
}

Vector3 Source::GetAttribute(EAttributes nAttribute) const
{
	// Check parameter
	if (nAttribute >= NumOfAttributes)
		return Vector3::Zero; // Error!

	// Get value
	switch (nAttribute) {
		case Position:
			return m_vPosition;

		case Velocity:
			return m_vVelocity;
	}

	// Error!
	return Vector3::Zero;
}

bool Source::SetAttribute(EAttributes nAttribute, const Vector3 &vV)
{
	// Check parameter
	if (nAttribute >= NumOfAttributes)
		return false; // Error!

	// Set attribute
	switch (nAttribute) {
		case Position:
			m_vPosition = vV;
			alSourcefv(m_nSource, AL_POSITION, m_b2D ? Vector3::Zero : m_vPosition);	// The position for 2D sounds is always at the listeners origin
			break;

		case Velocity:
			m_vVelocity = vV;
			alSourcefv(m_nSource, AL_VELOCITY, m_b2D ? Vector3::Zero : m_vVelocity);	// The velocity for 2D sounds is always null
			break;
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSoundOpenAL
