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
#include "PLSoundNull/SoundManager.h"
#include "PLSoundNull/Buffer.h"
#include "PLSoundNull/Source.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLMath;
namespace PLSoundNull {


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
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Source::Source(PLSound::SoundManager &cSoundManager) : PLSound::Source(cSoundManager),
	m_fVolume(1.0f),
	m_b2D(false),
	m_bLooping(false),
	m_fPitch(1.0f),
	m_fReferenceDistance(1.0f),
	m_fMaxDistance(10000.0f),
	m_fRolloffFactor(1.0f)
{
}


//[-------------------------------------------------------]
//[ Public virtual PLSound::Source functions              ]
//[-------------------------------------------------------]
bool Source::Load(PLSound::Buffer *pBuffer)
{
	// Call base implementation
	return PLSound::Source::Load(pBuffer);
}

bool Source::Play(bool bRestart)
{
	// Done
	return true;
}

bool Source::IsPlaying() const
{
	// Not playing
	return false;
}

void Source::Pause()
{
}

bool Source::IsPaused() const
{
	// Not paused
	return false;
}

void Source::Stop()
{
}

float Source::GetVolume() const
{
	return m_fVolume;
}

void Source::SetVolume(float fVolume)
{
	m_fVolume = fVolume;
}

bool Source::Is2D() const
{
	return m_b2D;
}

void Source::Set2D(bool b2D)
{
	m_b2D = b2D;
}

bool Source::IsLooping() const
{
	return m_bLooping;
}

void Source::SetLooping(bool bLooping)
{
	m_bLooping = bLooping;
}

float Source::GetPitch() const
{
	return m_fPitch;
}

void Source::SetPitch(float fPitch)
{
	m_fPitch = fPitch;
}

float Source::GetReferenceDistance() const
{
	return m_fReferenceDistance;
}

void Source::SetReferenceDistance(float fReferenceDistance)
{
	m_fReferenceDistance = fReferenceDistance;
}

float Source::GetMaxDistance() const
{
	return m_fMaxDistance;
}

void Source::SetMaxDistance(float fMaxDistance)
{
	m_fMaxDistance = fMaxDistance;
}

float Source::GetRolloffFactor() const
{
	return m_fRolloffFactor;
}

void Source::SetRolloffFactor(float fRolloffFactor)
{
	m_fRolloffFactor = fRolloffFactor;
}

Vector3 Source::GetAttribute(EAttributes nAttribute) const
{
	// Check parameter
	return (nAttribute < NumOfAttributes) ? m_vAttributes[nAttribute] : Vector3::Zero;
}

bool Source::SetAttribute(EAttributes nAttribute, const Vector3 &vV)
{
	// Check parameter
	if (nAttribute >= NumOfAttributes)
		return false; // Error!

	// Set attribute
	m_vAttributes[nAttribute] = vV;

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSoundNull
