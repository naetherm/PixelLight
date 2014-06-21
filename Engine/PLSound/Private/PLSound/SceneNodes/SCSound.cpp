/*********************************************************\
 *  File: SCSound.cpp                                    *
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
#include <PLCore/Base/Class.h>
#include <PLMath/Matrix3x3.h>
#include <PLScene/Scene/SceneContext.h>
#include "PLSound/SoundManager.h"
#include "PLSound/SceneNodes/SCSound.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLScene;
namespace PLSound {


// [TODO]
//	if (m_pSoundManager) m_pSoundManager->UnloadUnused();


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SCSound)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
float SCSound::GetVolume() const
{
	return m_fVolume;
}

void SCSound::SetVolume(float fValue)
{
	if (m_fVolume != fValue) {
		m_fVolume = fValue;
		if (m_pSoundManager)
			m_pSoundManager->SetVolume(m_fVolume);
	}
}

float SCSound::GetPitch() const
{
	return m_fPitch;
}

void SCSound::SetPitch(float fValue)
{
	if (m_fPitch != fValue) {
		m_fPitch = fValue;
		if (m_pSoundManager)
			m_pSoundManager->SetPitch(m_fPitch);
	}
}

String SCSound::GetListener() const
{
	return m_sListener;
}

void SCSound::SetListener(const String &sValue)
{
	if (m_sListener != sValue) {
		m_sListener = sValue;
		m_cListenerNodeHandler.SetElement(GetByName(m_sListener));
		OnUpdate();
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SCSound::SCSound() :
	SoundAPI(this),
	Volume(this),
	Pitch(this),
	Listener(this),
	SlotOnUpdate(this),
	m_fVolume(1.0f),
	m_fPitch(1.0f),
	m_pSoundManager(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
SCSound::~SCSound()
{
	// De-initialize the scene container
	DeInit();

	// Destroy the PL sound manager
	if (m_pSoundManager)
		delete m_pSoundManager;
}

/**
*  @brief
*    Returns the PL sound manager
*/
SoundManager *SCSound::GetSoundManager() const
{
	return m_pSoundManager;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node needs to be updated
*/
void SCSound::OnUpdate()
{
	// Update the PL sound manager
	if (m_pSoundManager) {
		// Get the scene node used as listener
		SceneNode *pListener = m_cListenerNodeHandler.GetElement();
		if (!pListener) {
			pListener = GetByName(m_sListener);
			m_cListenerNodeHandler.SetElement(pListener);
		}
		if (pListener && pListener->GetContainer()) {
			// Set the position and rotation of the listener. If the sound takes
			// place within another container, we need to transform...
			Matrix3x4 matTransform;
			pListener->GetContainer()->GetTransformMatrixTo(*this, matTransform);
			Vector3    vPosition = matTransform*pListener->GetTransform().GetPosition();
			Quaternion qRotation = pListener->GetTransform().GetRotation()*matTransform;

			// Set listener position
			m_pSoundManager->SetListenerAttribute(SoundManager::ListenerPosition, vPosition);

			// Get/set listener direction vectors
			m_pSoundManager->SetListenerAttribute(SoundManager::ListenerForward, qRotation.GetZAxis());
			m_pSoundManager->SetListenerAttribute(SoundManager::ListenerUpward,  qRotation.GetYAxis());
		}

		// Update sound manger
		m_pSoundManager->Update();
	}
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneNode functions        ]
//[-------------------------------------------------------]
void SCSound::InitFunction()
{
	// Call base implementation
	SceneContainer::InitFunction();

	// Is the sound manager already created?
	if (!m_pSoundManager) {
		// Create the PL sound manager
		const Class *pClass = ClassManager::GetInstance()->GetClass(SoundAPI.Get());
		if (pClass && pClass->IsDerivedFrom("PLSound::SoundManager")) {
			Object *pObject = pClass->Create();
			if (pObject) {
				m_pSoundManager = static_cast<SoundManager*>(pObject);
				if (m_pSoundManager->Init()) {
					m_pSoundManager->SetVolume(m_fVolume);
					m_pSoundManager->SetPitch (m_fPitch);
					OnUpdate();
				}
			}
		}
	}
}

void SCSound::OnActivate(bool bActivate)
{
	// Call the base implementation
	SceneContainer::OnActivate(bActivate);

	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate) {
			pSceneContext->EventUpdate.Connect(SlotOnUpdate);

			// Make a first update to ensure everything is up-to-date when we're going active (synchronization and logic update)
			OnUpdate();
		} else {
			pSceneContext->EventUpdate.Disconnect(SlotOnUpdate);
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSound
