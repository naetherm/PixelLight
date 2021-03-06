/*********************************************************\
 *  File: SNBomb.cpp                                     *
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
#include <PLCore/Tools/Timing.h>
#include <PLScene/Scene/SceneContext.h>
#include <PLSound/Source.h>
#include "SNBomb.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLSound;
using namespace PLScene;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SNBomb, "", PLSound::SNSound, "Bomb scene node")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Attributes
	pl_attribute_metadata(Killed,	bool,								false,							ReadWrite,	"Killed?",																								"")
		// Overwritten PLSound::SNSound attributes
	pl_attribute_metadata(Sound,	PLCore::String,						"Data/Sounds/Explosion.ogg",	ReadWrite,	"Filename of the sound which should be played (full path, supported file formats are API dependent)",	"Ext='mp3 ogg wav mid midi it mod s3m xm'")
		// Overwritten PLScene::SceneNode attributes
	pl_attribute_metadata(Flags,	pl_flag_type_def3(SNBomb, EFlags),	SNBomb::NoCulling,				ReadWrite,	"Flags",																								"")
	// Slots
	pl_slot_0_metadata(OnUpdate,	"Called when the scene node needs to be updated",	"")
pl_class_metadata_end(SNBomb)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNBomb::SNBomb() :
	Killed(this),
	Sound(this),
	Flags(this),
	SlotOnUpdate(this),
	m_fTimer(0.0f),
	m_nFrame(0),
	m_fFrame(0.0f),
	m_bAnimationDirection(1)
{
	// Overwrite the default setting of the flags
	SetFlags(GetFlags()|NoLoop|NoStartPlayback|No3D);

	// Overwritten PLSound::SNSound variables
	m_sSound = "Data/Sounds/Explosion.ogg";

	// Set the bounding box
	SetAABoundingBox(AABoundingBox(-8.0f, -8.0f, -1.0f, 8.0f, 8.0f, 1.0f));
}

/**
*  @brief
*    Destructor
*/
SNBomb::~SNBomb()
{
}

/**
*  @brief
*    Returns the current frame
*/
char SNBomb::GetFrame() const
{
	return m_nFrame;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node needs to be updated
*/
void SNBomb::OnUpdate()
{
	// Get time difference
	const float fTimeDiff = Timing::GetInstance()->GetTimeDifference();

	// Update timer
	m_fTimer += fTimeDiff/10;

	// Update position and check whether the bomb hits the ground
	bool bFalling = false;
	if (GetTransform().GetPosition().y < 180.0f) {
		bFalling = true;
		GetTransform().SetPosition(GetTransform().GetPosition() + Vector3(0.0f, 20.0f, 0.0f)*fTimeDiff);
	} else {
		// Kill the bomb
		Killed = true;
	}

	// Is the bomb killed?
	if (Killed) {
		// Animate bomb death
		if (m_nFrame < 7) {
			// Start killed animation
			m_nFrame = 7;
			m_fFrame = 0.0f;

			// Play sound
			Source *pSoundSource = GetSoundSource();
			if (pSoundSource)
				pSoundSource->Play();
		} else {
			m_fFrame += fTimeDiff*10;
			while (m_fFrame >= 1.0f) {
				m_fFrame -= 1.0f;
				m_nFrame++;
				if (m_nFrame >= 11) {
					// Destroy the bomb scene node if the sound is finished
					Source *pSoundSource = GetSoundSource();
					if (!pSoundSource || pSoundSource && !pSoundSource->IsPlaying())
						Delete();
				}
			}
		}
	} else {
		// Animate the bomb while falling
		if (bFalling) {
			m_fFrame += fTimeDiff*10;
			while (m_fFrame >= 1.0f) {
				m_fFrame -= 1.0f;
				if (m_bAnimationDirection) {
					m_nFrame++;
					if (m_nFrame >= 6)
						m_bAnimationDirection = 0;
				} else {
					m_nFrame--;
					if (m_nFrame <= 0)
						m_bAnimationDirection = 1;
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneNode functions          ]
//[-------------------------------------------------------]
void SNBomb::OnActivate(bool bActivate)
{
	// Call the base implementation
	SNSound::OnActivate(bActivate);

	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate)
			pSceneContext->EventUpdate.Connect(SlotOnUpdate);
		else
			pSceneContext->EventUpdate.Disconnect(SlotOnUpdate);
	}
}
