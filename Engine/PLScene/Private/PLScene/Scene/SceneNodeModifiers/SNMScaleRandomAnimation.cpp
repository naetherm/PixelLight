/*********************************************************\
 *  File: SNMScaleRandomAnimation.cpp                    *
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
#include "PLScene/Scene/SceneNodeModifiers/SNMScaleRandomAnimation.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMScaleRandomAnimation)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMScaleRandomAnimation::SNMScaleRandomAnimation(SceneNode &cSceneNode) : SNMTransform(cSceneNode),
	Speed(this),
	Radius(this),
	FixScale(this),
	EventHandlerUpdate(&SNMScaleRandomAnimation::OnUpdate, this),
	m_fTimer(0.0f)
{
	// Set initial fixed scale
	FixScale.Set(cSceneNode.GetTransform().GetScale());
}

/**
*  @brief
*    Destructor
*/
SNMScaleRandomAnimation::~SNMScaleRandomAnimation()
{
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNodeModifier functions         ]
//[-------------------------------------------------------]
void SNMScaleRandomAnimation::OnActivate(bool bActivate)
{
	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate) {
			pSceneContext->EventUpdate.Connect(EventHandlerUpdate);

			// Make a first update to ensure everything is up-to-date when we're going active (synchronization and logic update)
			OnUpdate();
		} else {
			pSceneContext->EventUpdate.Disconnect(EventHandlerUpdate);
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
void SNMScaleRandomAnimation::OnUpdate()
{
	// Update timer
	m_fTimer += Timing::GetInstance()->GetTimeDifference()*Speed;

	// Set current scene node scale
	GetSceneNode().GetTransform().SetScale(Vector3(FixScale.Get().x+Math::Cos(m_fTimer*2)*Radius,
										   FixScale.Get().y+Math::Sin(m_fTimer)  *Radius,
										   FixScale.Get().z+Math::Cos(m_fTimer/2)*Radius));
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
