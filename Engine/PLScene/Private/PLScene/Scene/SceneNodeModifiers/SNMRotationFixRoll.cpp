/*********************************************************\
 *  File: SNMRotationFixRoll.cpp                         *
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
#include <PLMath/Matrix3x3.h>
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneNodeModifiers/SNMRotationFixRoll.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMRotationFixRoll)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMRotationFixRoll::SNMRotationFixRoll(SceneNode &cSceneNode) : SNMTransform(cSceneNode),
	UpVector(this),
	Speed(this),
	EventHandlerUpdate(&SNMRotationFixRoll::OnUpdate, this)
{
}

/**
*  @brief
*    Destructor
*/
SNMRotationFixRoll::~SNMRotationFixRoll()
{
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNodeModifier functions         ]
//[-------------------------------------------------------]
void SNMRotationFixRoll::OnActivate(bool bActivate)
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
void SNMRotationFixRoll::OnUpdate()
{
	// Get the transform of the owner scene node
	Transform3 &cTransform = GetSceneNode().GetTransform();

	/*
	// This will 'flat' the rotation for example for an up-vector of '0 1 0' on the xz-plane
	Vector3    vLocalY = cTransform.GetRotation()*UpVector.Get();
	Quaternion qQuat   = vLocalY.GetRotationTo(UpVector.Get());
	cTransform.SetRotation(qQuat*cTransform.GetRotation());
	*/
	Matrix3x3 mRot;
	mRot.LookAt(cTransform.GetPosition(), cTransform.GetPosition() - cTransform.GetRotation().GetZAxis(), UpVector.Get());
	Quaternion qQ = mRot;
	qQ.UnitInvert();
	if (Speed > 0.0f)
		qQ.Slerp(cTransform.GetRotation(), qQ, Timing::GetInstance()->GetTimeDifference()*Speed);
	cTransform.SetRotation(qQ);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
