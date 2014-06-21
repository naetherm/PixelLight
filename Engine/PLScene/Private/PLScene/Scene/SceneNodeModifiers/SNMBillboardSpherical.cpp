/*********************************************************\
 *  File: SNMBillboardSpherical.cpp                      *
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
#include <PLMath/Matrix4x4.h>
#include "PLScene/Visibility/VisNode.h"
#include "PLScene/Scene/SceneNodeModifiers/SNMBillboardSpherical.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMBillboardSpherical)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMBillboardSpherical::SNMBillboardSpherical(SceneNode &cSceneNode) : SNMBillboard(cSceneNode),
	SlotOnAddedToVisibilityTree(this)
{
}

/**
*  @brief
*    Destructor
*/
SNMBillboardSpherical::~SNMBillboardSpherical()
{
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNodeModifier functions         ]
//[-------------------------------------------------------]
void SNMBillboardSpherical::OnActivate(bool bActivate)
{
	// Connect/disconnect event handler
	if (bActivate) {
		// Connect event handler
		GetSceneNode().SignalAddedToVisibilityTree.Connect(SlotOnAddedToVisibilityTree);
	} else {
		// Disconnect event handler
		GetSceneNode().SignalAddedToVisibilityTree.Disconnect(SlotOnAddedToVisibilityTree);
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Builds a transform matrix
*/
void SNMBillboardSpherical::BuildTransformMatrix(const Matrix4x4 &mView, const Matrix4x4 &mWorld, Matrix3x4 &mTransform)
{
	// Get position
	Vector3 vPosition;
	vPosition *= mWorld;

	// Get scale
	const Vector3 &vScale = GetSceneNode().GetTransform().GetScale();
	
	// Construct the new world transform matrix, (WITHOUT position!) the rotation part is just the transposed rotation part of the given view matrix
	mTransform.xx = vScale.x*mView.xx; mTransform.xy = vScale.y*mView.yx; mTransform.xz = vScale.z*mView.zx;
	mTransform.yx = vScale.x*mView.xy; mTransform.yy = vScale.y*mView.yy; mTransform.yz = vScale.z*mView.zy;
	mTransform.zx = vScale.x*mView.xz; mTransform.zy = vScale.y*mView.yz; mTransform.zz = vScale.z*mView.zz;

	/*
	// [TODO] Currently, this doesn't look correct - do we really need/want a possible rotation offset?
	// Apply rotation (can be seen as offset in here)
	if (GetSceneNode().GetTransform().GetRotation() != Quaternion::Identity) {
		Matrix3x4 mRotation;
		GetSceneNode().GetTransform().GetRotation().ToRotationMatrix(mRotation);
		mTransform = mRotation*mTransform;
	}
	*/

	// Finally, set the position
	mTransform.xw = vPosition.x;
	mTransform.yw = vPosition.y;
	mTransform.zw = vPosition.z;
}

/**
*  @brief
*    Called when the owner scene node was added to a visibility tree
*/
void SNMBillboardSpherical::OnAddedToVisibilityTree(VisNode &cVisNode)
{
	// Get the new world transform matrix
	Matrix3x4 mTransform;
	BuildTransformMatrix(cVisNode.GetViewMatrix(), cVisNode.GetWorldMatrix(), mTransform);

	// Set the new world transform matrix of the visibility node
	cVisNode.SetWorldMatrix(mTransform);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
