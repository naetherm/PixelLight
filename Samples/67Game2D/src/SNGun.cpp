/*********************************************************\
 *  File: SNGun.cpp                                      *
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
#include <PLInput/Input/InputManager.h>
#include <PLScene/Scene/SceneContext.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLScene/Scene/SceneQueries/SQAABoundingBox.h>
#include <PLSound/Source.h>
#include "GunController.h"
#include "SNGun.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLInput;
using namespace PLSound;
using namespace PLScene;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SNGun, "", PLSound::SNSound, "Gun scene node")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Attributes
	pl_attribute_metadata(InputSemantic,	PLCore::String,						"Gun",									ReadWrite,	"Semantic of this input controller (e.g. \"Camera\")",													"")
		// Overwritten PLSound::SNSound attributes
	pl_attribute_metadata(Sound,			PLCore::String,						"Data/Sounds/Shot.ogg",					ReadWrite,	"Filename of the sound which should be played (full path, supported file formats are API dependent)",	"Ext='mp3 ogg wav mid midi it mod s3m xm'")
		// Overwritten PLScene::SceneNode attributes
	pl_attribute_metadata(Flags,			pl_flag_type_def3(SNGun, EFlags),	SNGun::NoLoop|SNGun::NoStartPlayback,	ReadWrite,	"Flags",																								"")
	// Slots
	pl_slot_0_metadata(OnUpdate,														"Called when the scene node needs to be updated",																			"")
	pl_slot_2_metadata(OnSceneNode,	PLScene::SceneQuery&,	PLScene::SceneNode&,	"Called when a scene node was found, query found the scene node as first parameter, found scene node as second parameter",	"")
pl_class_metadata_end(SNGun)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNGun::SNGun() :
	InputSemantic(this),
	Sound(this),
	Flags(this),
	SlotOnUpdate(this),
	SlotOnSceneNode(this),
	m_pController(new GunController()),
	m_nFrame(0),
	m_fFrame(0.0f)
{
	// Overwrite the default setting of the flags
	SetFlags(GetFlags()|NoLoop|NoStartPlayback|No3D);

	// Overwritten PLSound::SNSound variables
	m_sSound = "Data/Sounds/Shot.ogg";

	// Set the bounding box
	SetAABoundingBox(AABoundingBox(-18.0f, -7.0f, -1.0f, 18.0f, 7.0f, 1.0f));
}

/**
*  @brief
*    Destructor
*/
SNGun::~SNGun()
{
	// Destroy the input controller
	delete m_pController;
}

/**
*  @brief
*    Returns the current frame
*/
char SNGun::GetFrame() const
{
	return m_nFrame;
}


//[-------------------------------------------------------]
//[ Public virtual PLScene::SceneNode functions           ]
//[-------------------------------------------------------]
Controller *SNGun::GetInputController() const
{
	return m_pController;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a scene node was found
*/
void SNGun::OnSceneNode(SceneQuery &cQuery, SceneNode &cSceneNode)
{
	// Is this gun still active?
	if (IsActive()) {
		// Is this a bomb?
		if (cSceneNode.IsInstanceOf("SNBomb")) {
			// Is the bomb still alive?
			if (!cSceneNode.GetAttribute("Killed")->GetBool()) {
				// Jap, kill the bomb right now
				cSceneNode.SetAttribute("Killed", "1");

				// Destroy this gun
				Delete();

				// Done, do NOT continue the query!
				cQuery.Stop();
			}
		}
	}
}

/**
*  @brief
*    Called when the scene node needs to be updated
*/
void SNGun::OnUpdate()
{
	// Get the current time difference between the last frame and the current frame
	const float fTimeDiff = Timing::GetInstance()->GetTimeDifference();

	// Get X value
	float fDelta = m_pController->X.GetValue();

	// Do we need to take the current time difference into account?
	if (!m_pController->X.IsValueRelative())
		fDelta *= fTimeDiff;

	// Left button pressed?
	if (m_pController->Left.IsPressed())
		fDelta = -6.0f*fTimeDiff;

	// Right button pressed?
	if (m_pController->Right.IsPressed())
		fDelta = 6.0f*fTimeDiff;

	// Move gun
	m_fFrame += fDelta * 3.0f;
	if (m_fFrame > 12.0f)
		m_fFrame = 12.0f;
	if (m_fFrame < 0.0f)
		m_fFrame = 0.0f;
	m_nFrame = static_cast<char>(m_fFrame);

	// Create a projectile?
	if (m_pController->Fire.IsHit()) {
		// Get projectile start position in bitmap space
		Vector2 vPos;
		switch (m_nFrame) {
			case 0:  vPos.SetXY(0,  8); break;
			case 1:  vPos.SetXY(0,  6); break;
			case 2:  vPos.SetXY(0,  3); break;
			case 3:  vPos.SetXY(6,  0); break;
			case 4:  vPos.SetXY(11, 0); break;
			case 5:  vPos.SetXY(16, 0); break;
			case 6:  vPos.SetXY(18, 0); break;
			case 7:  vPos.SetXY(22, 0); break;
			case 8:  vPos.SetXY(25, 0); break;
			case 9:  vPos.SetXY(31, 0); break;
			case 10: vPos.SetXY(37, 3); break;
			case 11: vPos.SetXY(37, 6); break;
			case 12: vPos.SetXY(37, 8); break;
		}

		// Get real projective position
		vPos.x += GetTransform().GetPosition().x-18;
		vPos.y += GetTransform().GetPosition().y-6;

		// Get projective direction
		Vector2 vDir = Vector2(vPos.x-GetTransform().GetPosition().x, vPos.y-GetTransform().GetPosition().y-2).Normalize();

		// Create the projectile scene node
		GetContainer()->Create("SNProjectile", "", String::Format("Position=\"%g %g\" Direction=\"%g %g\"",
							   vPos.x, vPos.y, vDir.x, vDir.y));

		// Play sound
		Source *pSoundSource = GetSoundSource();
		if (pSoundSource)
			pSoundSource->Play(true);
	}

	// Collision detection
	// Create a new scene query we can use to check which scene nodes are within a given axis aligned bounding box
	SQAABoundingBox *pSceneQuery = static_cast<SQAABoundingBox*>(GetContainer()->CreateQuery("PLScene::SQAABoundingBox"));
	if (pSceneQuery) {
		// Connect event handler
		pSceneQuery->SignalSceneNode.Connect(SlotOnSceneNode);

		// Setup axis aligned bounding box
		pSceneQuery->GetAABoundingBox() = GetContainerAABoundingBox();

		// Let's perform the query...
		pSceneQuery->PerformQuery();

		// ... and clean up if we are done, we don't use 'GetContainer()' in here because this node
		// can be destroyed inside our query
		pSceneQuery->GetSceneContainer().DestroyQuery(*pSceneQuery);
	}
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneNode functions        ]
//[-------------------------------------------------------]
void SNGun::InitFunction()
{
	// Call base implementation
	SNSound::InitFunction();

	// Emit the input controller found event of the scene context to tell everyone about our input controller
	InputManager::GetInstance()->EventInputControllerFound(m_pController, InputSemantic);
}

void SNGun::OnActivate(bool bActivate)
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
