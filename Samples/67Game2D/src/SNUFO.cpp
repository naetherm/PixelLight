/*********************************************************\
 *  File: SNUFO.cpp                                      *
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
#include <PLScene/Scene/SceneContainer.h>
#include <PLSound/Source.h>
#include "SNUFO.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLScene;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SNUFO, "", PLSound::SNSound, "UFO scene node")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Attributes
	pl_attribute_metadata(Sound,	PLCore::String,	"Data/Sounds/UFO.ogg",	ReadWrite,	"Filename of the sound which should be played (full path, supported file formats are API dependent)",	"Ext='mp3 ogg wav mid midi it mod s3m xm'")
	// Slots
	pl_slot_0_metadata(OnUpdate,	"Called when the scene node needs to be updated",	"")
pl_class_metadata_end(SNUFO)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNUFO::SNUFO() :
	Sound(this),
	SlotOnUpdate(this),
	m_fTimer(0.0f),
	m_fBombTimer(0.0f)
{
	// Overwritten PLSound::SNSound variables
	m_sSound = "Data/Sounds/UFO.ogg";

	// Overwrite the default setting of the flags
	SetFlags(GetFlags()|No3D);

	// Set the bounding box
	SetAABoundingBox(AABoundingBox(-25.0f, -10.0f, -1.0f, 25.0f, 10.0f, 1.0f));
}

/**
*  @brief
*    Destructor
*/
SNUFO::~SNUFO()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node needs to be updated
*/
void SNUFO::OnUpdate()
{
	// Get time difference
	const float fTimeDiff = Timing::GetInstance()->GetTimeDifference();

	// Update timer
	m_fTimer += fTimeDiff/2;

	// Update position
	GetTransform().SetPosition(Vector3(160 + Math::Sin(m_fTimer)*130, 20.0f + Math::Cos(m_fTimer*10)*5, 0.0f));

	// Update the bomb timer
	m_fBombTimer += fTimeDiff*2;
	if (m_fBombTimer >= 1.0f) {
		m_fBombTimer = 0.0f;
		// Drop a bomb?
		if (!(Math::GetRand() % 3))
			GetContainer()->Create("SNBomb", "", String::Format("Position=\"%g %g\"", GetTransform().GetPosition().x+18, GetTransform().GetPosition().y+10));
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneNode functions          ]
//[-------------------------------------------------------]
void SNUFO::OnActivate(bool bActivate)
{
	// Call the base implementation
	SNSound::OnActivate(bActivate);

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
