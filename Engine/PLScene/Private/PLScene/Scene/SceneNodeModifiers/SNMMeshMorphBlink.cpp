/*********************************************************\
 *  File: SNMMeshMorphBlink.cpp                          *
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
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneNodeModifiers/SNMMeshMorphBlink.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMMeshMorphBlink)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMMeshMorphBlink::SNMMeshMorphBlink(SceneNode &cSceneNode) : SNMMeshMorph(cSceneNode),
	Time(this),
	TimeBase(this),
	TimeRandom(this),
	Direction(this),
	Speed(this),
	SlotOnUpdate(this)
{
}

/**
*  @brief
*    Destructor
*/
SNMMeshMorphBlink::~SNMMeshMorphBlink()
{
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNodeModifier functions         ]
//[-------------------------------------------------------]
void SNMMeshMorphBlink::OnActivate(bool bActivate)
{
	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate) {
			pSceneContext->EventUpdate.Connect(SlotOnUpdate);

			// Make a first update to ensure everything is up-to-date when we're going active (synchronization, no logic update)
			UpdateMorphTarget();
		} else {
			pSceneContext->EventUpdate.Disconnect(SlotOnUpdate);
		}
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node modifier needs to be updated
*/
void SNMMeshMorphBlink::OnUpdate()
{
	// Get time difference
	const float fTimeDiff = Timing::GetInstance()->GetTimeDifference();

	// Update animation start
	if (!Direction) {
		Time = Time - fTimeDiff;
		if (Time < 0.0f) {
			Time      = TimeBase + Math::GetRandFloat()*TimeRandom;
			Direction = true;
		}
	}

	// Update animation playback
	const float fPreviousWeight = m_fWeight;
	if (Direction) {
		m_fWeight += fTimeDiff*Speed;
		if (m_fWeight >= m_fMaxWeight) {
			m_fWeight = m_fMaxWeight;
			Direction = false;
		}
	} else {
		m_fWeight -= fTimeDiff*Speed;
		if (m_fWeight < m_fMinWeight)
			m_fWeight = m_fMinWeight;
	}

	// Update the morph target if required
	if (fPreviousWeight != m_fWeight)
		UpdateMorphTarget();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
