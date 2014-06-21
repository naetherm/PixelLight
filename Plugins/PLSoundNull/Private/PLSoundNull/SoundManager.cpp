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
#include <PLCore/Log/Log.h>
#include "PLSoundNull/Buffer.h"
#include "PLSoundNull/Source.h"
#include "PLSoundNull/SoundManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLSoundNull {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SoundManager)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SoundManager::SoundManager() :
	m_fVolume(1.0f),
	m_fPitch(1.0f),
	m_fDopplerFactor(1.0f)
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


//[-------------------------------------------------------]
//[ Public virtual SoundManager functions                 ]
//[-------------------------------------------------------]
String SoundManager::GetDescription() const
{
	static const String sString = "Null sound";
	return sString;
}

bool SoundManager::GetFormatList(List<Format> &lstList) const
{
	// All went fine - but this backend doesn't support any sound file formats :)
	return true;
}

float SoundManager::GetVolume() const
{
	return m_fVolume;
}

void SoundManager::SetVolume(float fVolume)
{
	m_fVolume = fVolume;
}

float SoundManager::GetPitch() const
{
	return m_fPitch;
}

void SoundManager::SetPitch(float fPitch)
{
	m_fPitch = fPitch;
}

float SoundManager::GetDopplerFactor() const
{
	return m_fDopplerFactor;
}

void SoundManager::SetDopplerFactor(float fFactor)
{
	m_fDopplerFactor = fFactor;
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
	// Create the Null sound source
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
//[ Public virtual PLSound::SoundManager functions        ]
//[-------------------------------------------------------]
bool SoundManager::Init()
{
	// Done
	return true;
}

bool SoundManager::DeInit()
{
	// Call base implementation
	PLSound::SoundManager::DeInit();

	// Done
	return true;
}

bool SoundManager::Update()
{
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
} // PLSoundNull
