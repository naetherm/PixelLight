/*********************************************************\
 *  File: SNMRotationMoveDirection.cpp                   *
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
#include <PLMath/Matrix3x3.h>
#include <PLMath/EulerAngles.h>
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneNodeModifiers/SNMRotationMoveDirection.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMRotationMoveDirection)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMRotationMoveDirection::SNMRotationMoveDirection(SceneNode &cSceneNode) : SNMTransform(cSceneNode),
	Offset(this),
	UpVector(this),
	EventHandlerUpdate(&SNMRotationMoveDirection::OnUpdate, this),
	m_vLastPos(GetSceneNode().GetTransform().GetPosition())
{
}

/**
*  @brief
*    Destructor
*/
SNMRotationMoveDirection::~SNMRotationMoveDirection()
{
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNodeModifier functions         ]
//[-------------------------------------------------------]
void SNMRotationMoveDirection::OnActivate(bool bActivate)
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
void SNMRotationMoveDirection::OnUpdate()
{
	// Get the scene node
	SceneNode &cSceneNode = GetSceneNode();

	// Do ONLY update the rotation if the position difference is large enough. If this
	// is not done, the node will 'ugly flacker' if the difference is quite small.
	const Vector3 vDiff = cSceneNode.GetTransform().GetPosition()-m_vLastPos;
	if (vDiff.DotProduct(vDiff) > Math::Epsilon) {
		// Set rotation
		Matrix3x3 mRot;
		mRot.LookAt(cSceneNode.GetTransform().GetPosition(), m_vLastPos, UpVector.Get());
		mRot.Transpose();
		if (Offset.Get().IsNull())
			cSceneNode.GetTransform().SetRotation(Quaternion(mRot));
		else {
			// Get a quaternion representation of the rotation offset
			Quaternion qRotOffset;
			EulerAngles::ToQuaternion(static_cast<float>(Offset.Get().x*Math::DegToRad), static_cast<float>(Offset.Get().y*Math::DegToRad), static_cast<float>(Offset.Get().z*Math::DegToRad), qRotOffset);

			// Set new rotation
			cSceneNode.GetTransform().SetRotation(Quaternion(mRot)*qRotOffset);
		}

		// We can also use Vector3::GetRotationTo(), but the results are not always good - in the worst case
		// we will get 'extreme ugly flacker'
//		if (Offset.Get().IsNull())
//			cSceneNode.SetRotation(Vector3::UnitZ.GetRotationTo(vDiff));
//		else {
			// Get a quaternion representation of the rotation offset
//			Quaternion qRotOffset;
//			EulerAngles::ToQuaternion(static_cast<float>(Offset.Get().x*Math::DegToRad), static_cast<float>(Offset.Get().y*Math::DegToRad), static_cast<float>(Offset.Get().z*Math::DegToRad), qRotOffset);

			// Set new rotation
//			cSceneNode.SetRotation(Vector3::UnitZ.GetRotationTo(vDiff)*Quaternion(mRot)*qRotOffset);
//		}

		// Backup the current position
		m_vLastPos = cSceneNode.GetTransform().GetPosition();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
