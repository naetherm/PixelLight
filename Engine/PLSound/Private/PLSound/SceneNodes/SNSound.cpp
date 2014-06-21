/*********************************************************\
 *  File: SNSound.cpp                                    *
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
#include <PLSound/Source.h>
#include <PLSound/SoundManager.h>
#include <PLSound/ResourceHandler.h>
#include "PLSound/SceneNodes/SCSound.h"
#include "PLSound/SceneNodes/SNSound.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLScene;
namespace PLSound {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNSound)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SNSound::GetSound() const
{
	return m_sSound;
}

void SNSound::SetSound(const String &sValue)
{
	if (m_sSound != sValue) {
		m_sSound = sValue;

		// Load/reload the sound
		Load();
	}
}

float SNSound::GetVolume() const
{
	return m_fVolume;
}

void SNSound::SetVolume(float fValue)
{
	if (m_fVolume != fValue) {
		m_fVolume = fValue;
		Source *pSS = static_cast<Source*>(m_pSoundSourceHandler->GetResource());
		if (pSS)
			pSS->SetVolume(m_fVolume);
	}
}

float SNSound::GetPitch() const
{
	return m_fPitch;
}

void SNSound::SetPitch(float fValue)
{
	if (m_fPitch != fValue) {
		m_fPitch = fValue;
		Source *pSS = static_cast<Source*>(m_pSoundSourceHandler->GetResource());
		if (pSS)
			pSS->SetPitch(m_fPitch);
	}
}

float SNSound::GetReferenceDistance() const
{
	return m_fReferenceDistance;
}

void SNSound::SetReferenceDistance(float fValue)
{
	if (m_fReferenceDistance != fValue) {
		m_fReferenceDistance = fValue;
		Source *pSS = static_cast<Source*>(m_pSoundSourceHandler->GetResource());
		if (pSS)
			pSS->SetReferenceDistance(m_fReferenceDistance);
	}
}

float SNSound::GetMaxDistance() const
{
	return m_fMaxDistance;
}

void SNSound::SetMaxDistance(float fValue)
{
	if (m_fMaxDistance != fValue) {
		m_fMaxDistance = fValue;
		Source *pSS = static_cast<Source*>(m_pSoundSourceHandler->GetResource());
		if (pSS)
			pSS->SetMaxDistance(m_fMaxDistance);
	}
}

float SNSound::GetRolloffFactor() const
{
	return m_fRolloffFactor;
}

void SNSound::SetRolloffFactor(float fValue)
{
	if (m_fRolloffFactor != fValue) {
		m_fRolloffFactor = fValue;
		Source *pSS = static_cast<Source*>(m_pSoundSourceHandler->GetResource());
		if (pSS)
			pSS->SetRolloffFactor(m_fRolloffFactor);
	}
}

void SNSound::SetFlags(uint32 nValue)
{
	if (GetFlags() != nValue) {
		// Call base implementation
		SceneNode::SetFlags(nValue);

		// Load/reload the sound
		Load();
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNSound::SNSound() :
	Sound(this),
	Volume(this),
	Pitch(this),
	ReferenceDistance(this),
	MaxDistance(this),
	RolloffFactor(this),
	Flags(this),
	SlotOnPosition(this),
	m_fVolume(1.0f),
	m_fPitch(1.0f),
	m_fReferenceDistance(1.0f),
	m_fMaxDistance(10000.0f),
	m_fRolloffFactor(1.0f),
	m_pSoundSourceHandler(new PLSound::ResourceHandler())
{
	// Connect event handler
	GetTransform().EventPosition.Connect(SlotOnPosition);
}

/**
*  @brief
*    Destructor
*/
SNSound::~SNSound()
{
	// Destroy used sound source
	Source *pSS = static_cast<Source*>(m_pSoundSourceHandler->GetResource());
	if (pSS)
		delete pSS;
	delete m_pSoundSourceHandler;
}

/**
*  @brief
*    Returns the sound source
*/
Source *SNSound::GetSoundSource() const
{
	return static_cast<Source*>(m_pSoundSourceHandler->GetResource());
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the sound container this sound is in
*/
SCSound *SNSound::GetSoundContainer() const
{
	// Get the PL sound container this scene node is in
	SceneContainer *pContainer = GetContainer();
	while (pContainer && !pContainer->IsInstanceOf("PLSound::SCSound"))
		pContainer = pContainer->GetContainer();

	// Done
	return (pContainer && pContainer->IsInstanceOf("PLSound::SCSound")) ? static_cast<SCSound*>(pContainer) : nullptr;
}

/**
*  @brief
*    Loads/reloads the sound
*/
void SNSound::Load()
{
	// Destroy currently used sound source
	Source *pSS = static_cast<Source*>(m_pSoundSourceHandler->GetResource());
	if (pSS)
		delete pSS;

	// Get the PL sound container this scene node is in
	SCSound *pSoundContainer = GetSoundContainer();
	if (pSoundContainer) {
		SoundManager *pSoundManager = pSoundContainer->GetSoundManager();
		if (pSoundManager) {
			Source *pSoundSource = pSoundManager->CreateSoundSource(pSoundManager->CreateSoundBuffer(m_sSound, (GetFlags() & Stream) != 0));
			m_pSoundSourceHandler->SetResource(pSoundSource);
			OnPosition();
			pSoundSource->SetVolume(m_fVolume);
			pSoundSource->Set2D((GetFlags() & No3D) != 0);
			pSoundSource->SetLooping(!(GetFlags() & NoLoop));
			pSoundSource->SetPitch(m_fPitch);
			pSoundSource->SetReferenceDistance(m_fReferenceDistance);
			pSoundSource->SetMaxDistance(m_fMaxDistance);
			pSoundSource->SetRolloffFactor(m_fRolloffFactor);
			if (!(GetFlags() & NoStartPlayback))
				pSoundSource->Play();
		}
	}
}

/**
*  @brief
*    Called when the scene node position changed
*/
void SNSound::OnPosition()
{
	// Update sound source position
	if (GetContainer()) {
		Source *pSS = static_cast<Source*>(m_pSoundSourceHandler->GetResource());
		if (pSS) {
			// Get the PL sound container this scene node is in
			SCSound *pSoundContainer = GetSoundContainer();
			if (pSoundContainer) {
				Matrix3x4 matTransform;
				GetContainer()->GetTransformMatrixTo(*pSoundContainer, matTransform);
				pSS->SetAttribute(Source::Position, matTransform*GetTransform().GetPosition());
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneNode functions        ]
//[-------------------------------------------------------]
void SNSound::InitFunction()
{
	// Call base implementation
	SceneNode::InitFunction();

	// Load/reload the sound
	Load();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSound
