/*********************************************************\
 *  File: SNMPositionLinearAnimation.cpp                 *
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
#include "PLScene/Scene/SceneNodeModifiers/SNMPositionLinearAnimation.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMPositionLinearAnimation)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPositionLinearAnimation::SNMPositionLinearAnimation(SceneNode &cSceneNode) : SNMTransform(cSceneNode),
	AutoVector(this),
	Vector(this),
	Speed(this),
	EventHandlerUpdate(&SNMPositionLinearAnimation::OnUpdate, this)
{
}

/**
*  @brief
*    Destructor
*/
SNMPositionLinearAnimation::~SNMPositionLinearAnimation()
{
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNodeModifier functions         ]
//[-------------------------------------------------------]
void SNMPositionLinearAnimation::OnActivate(bool bActivate)
{
	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate)
			pSceneContext->EventUpdate.Connect(EventHandlerUpdate);
		else
			pSceneContext->EventUpdate.Disconnect(EventHandlerUpdate);
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node modifier needs to be updated
*/
void SNMPositionLinearAnimation::OnUpdate()
{
	// Get the scene node
	SceneNode &cSceneNode = GetSceneNode();

	// Get movement vector
	Vector3 vVector;
	switch (AutoVector) {
		case None:
			vVector = Vector.Get();
			break;

		case XAxis:
			vVector = cSceneNode.GetTransform().GetRotation().GetXAxis();
			break;

		case YAxis:
			vVector = cSceneNode.GetTransform().GetRotation().GetYAxis();
			break;

		case ZAxis:
			vVector = cSceneNode.GetTransform().GetRotation().GetZAxis();
			break;
	}

	// Apply vector, speed and time difference
	const Vector3 vPosInc = vVector*Speed*Timing::GetInstance()->GetTimeDifference();

	// 'Move' to the new position
	if (!vPosInc.IsNull())
		cSceneNode.MoveTo(cSceneNode.GetTransform().GetPosition()-vPosInc);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
