/*********************************************************\
 *  File: SNMLightRandomAnimation.cpp                    *
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
#include "PLScene/Scene/SNLight.h"
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneNodeModifiers/SNMLightRandomAnimation.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMLightRandomAnimation)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMLightRandomAnimation::SNMLightRandomAnimation(SceneNode &cSceneNode) : SceneNodeModifier(cSceneNode),
	Speed(this),
	Radius(this),
	FixColor(this),
	Color(this),
	Flags(this),
	SlotOnUpdate(this),
	m_fTimer(0.0f)
{
}

/**
*  @brief
*    Destructor
*/
SNMLightRandomAnimation::~SNMLightRandomAnimation()
{
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNodeModifier functions         ]
//[-------------------------------------------------------]
void SNMLightRandomAnimation::OnActivate(bool bActivate)
{
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
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node modifier needs to be updated
*/
void SNMLightRandomAnimation::OnUpdate()
{
	// Update timer
	m_fTimer += Timing::GetInstance()->GetTimeDifference()*Speed;

	// Set current scene node scale
	SNLight &cLight = static_cast<SNLight&>(GetSceneNode());

	// Animate color
		  Color3 cColor  = cLight.Color.Get();
	const Color3 cColorT = Color.Get()*((Math::Cos(m_fTimer)+1)/2)*Radius;
	if (GetFlags() & Multiply) {
		// Red
		cColor.r = (GetFlags() & NR) ? FixColor.Get().r : FixColor.Get().r*cColorT.r;
		// Green
		cColor.g = (GetFlags() & NG) ? FixColor.Get().g : FixColor.Get().g*cColorT.g;
		// Blue
		cColor.b = (GetFlags() & NB) ? FixColor.Get().b : FixColor.Get().b*cColorT.b;
	} else {
		// Red
		cColor.r = (GetFlags() & NR) ? FixColor.Get().r : FixColor.Get().r+cColorT.r;
		// Green
		cColor.g = (GetFlags() & NG) ? FixColor.Get().g : FixColor.Get().g+cColorT.g;
		// Blue
		cColor.b = (GetFlags() & NB) ? FixColor.Get().b : FixColor.Get().b+cColorT.b;
	}

	// Clamp the color values between 0.0 and 1.0
	cColor.Saturate();

	// Finally, set the new color of the light
	cLight.Color.Set(cColor);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
