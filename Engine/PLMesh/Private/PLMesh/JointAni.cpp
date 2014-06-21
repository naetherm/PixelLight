/*********************************************************\
 *  File: JointAni.cpp                                   *
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
#include "PLMesh/JointHandler.h"
#include "PLMesh/JointAni.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLMesh {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
JointAni::JointAni()
{
}

/**
*  @brief
*    Destructor
*/
JointAni::~JointAni()
{
}

/**
*  @brief
*    Returns the animation joints
*/
Array<JointAni::AniJoint> &JointAni::GetJoints()
{
	return m_lstAniJoints;
}

/**
*  @brief
*    Applies the joint states from the given joint animation frame
*    to the given joint states
*/
bool JointAni::ApplyJointStates(Array<JointHandler> &lstJointHandlers, uint32 nFrame, float fWeight) const
{
	// Check weight of this states
	if (fWeight) {
		// Are there any joints?
		if (!lstJointHandlers.GetNumOfElements())
			return false; // Error!

		// Get the frame keys
		FrameKeys &cFrameKeys = m_lstFrameKeys[nFrame];
		if (&cFrameKeys == &Array<FrameKeys>::Null)
			return false; // Error!

		// Get frame
		Array<float> &lstFrameKeys = cFrameKeys.lstFrameKeys;

		// Loop through all animation joints
		Quaternion qDelta, qDeltaFinal;
		Vector3 vDelta;
		long nKey = 0;
		for (uint32 i=0; i<lstJointHandlers.GetNumOfElements(); i++) {
			JointHandler &cJH       = lstJointHandlers[i];
			AniJoint     &cAniJoint = m_lstAniJoints[i];

			// Reset deltas
			vDelta.SetXYZ(0.0f, 0.0f, 0.0f);
			qDelta.SetWXYZ(1.0f, 0.0f, 0.0f, 0.0f);

			// Get joint state
			char nAnimatedComponents = cAniJoint.nAnimatedComponents;

			// Translation
			if (nAnimatedComponents & AX)
				vDelta.x = lstFrameKeys[nKey++];
			if (nAnimatedComponents & AY)
				vDelta.y = lstFrameKeys[nKey++];
			if (nAnimatedComponents & AZ)
				vDelta.z = lstFrameKeys[nKey++];

			// Rotation
			if (nAnimatedComponents & AYaw ||
				nAnimatedComponents & APitch ||
				nAnimatedComponents & ARoll ||
				nAnimatedComponents & AW) {
				if (nAnimatedComponents & AYaw)
					qDelta.x = lstFrameKeys[nKey++];
				if (nAnimatedComponents & APitch)
					qDelta.y = lstFrameKeys[nKey++];
				if (nAnimatedComponents & ARoll)
					qDelta.z = lstFrameKeys[nKey++];
				if (nAnimatedComponents & AW)
					qDelta.w = lstFrameKeys[nKey++];
				else {
					// Compute last component (w) of the quaternion
					float fSqw = 1.0f - qDelta.x*qDelta.x -
										qDelta.y*qDelta.y -
										qDelta.z*qDelta.z;
					qDelta.w = (fSqw > 0.0f) ? -Math::Sqrt(fSqw) : 0.0f;
				}
			}

			// Add deltas
			qDeltaFinal.Slerp(qDelta, Quaternion::Identity, fWeight);
			cJH.SetTranslation(cJH.GetTranslation()+(vDelta*fWeight));
			cJH.SetRotation(cJH.GetRotation()*qDeltaFinal);
		}
	}

	// Done
	return true;
}

/**
*  @brief
*    Applies the blended joint states from the given joint animation frames
*    to the given joint states
*/
bool JointAni::ApplyJointStates(Array<JointHandler> &lstJointHandlers, uint32 nFrame1,
								uint32 nFrame2, float fTime, float fWeight) const
{
	// Check weight of this states
	if (fWeight) {
		// Are there any joints?
		if (!lstJointHandlers.GetNumOfElements())
			return false; // Error!

		// Get the frame keys
		FrameKeys &cFrame1Keys = m_lstFrameKeys[nFrame1];
		if (&cFrame1Keys == &Array<FrameKeys>::Null)
			return false; // Error!
		FrameKeys &cFrame2Keys = m_lstFrameKeys[nFrame2];
		if (&cFrame2Keys == &Array<FrameKeys>::Null)
			return false; // Error!

		// Get frames
		Array<float> &lstFrame1Keys = cFrame1Keys.lstFrameKeys;
		Array<float> &lstFrame2Keys = cFrame2Keys.lstFrameKeys;

		// Loop through all animation joints
		Quaternion qDelta, qDelta1, qDelta2, qDeltaFinal;
		Vector3 vDelta;
		long nKey = 0;
		for (uint32 i=0; i<lstJointHandlers.GetNumOfElements(); i++) {
			JointHandler   &cJH       = lstJointHandlers[i];
			const AniJoint &cAniJoint = m_lstAniJoints[i];

			// Reset deltas
			vDelta.SetXYZ(0.0f, 0.0f, 0.0f);
			qDelta.SetWXYZ(1.0f, 0.0f, 0.0f, 0.0f);
			qDelta1.SetWXYZ(1.0f, 0.0f, 0.0f, 0.0f);
			qDelta2.SetWXYZ(1.0f, 0.0f, 0.0f, 0.0f);

			// Get joint state
			char nAnimatedComponents = cAniJoint.nAnimatedComponents;

			// Translation
			if (nAnimatedComponents & AX) {
				vDelta.x = lstFrame1Keys[nKey]+(lstFrame2Keys[nKey]-lstFrame1Keys[nKey])*fTime;
				nKey++;
			}
			if (nAnimatedComponents & AY) {
				vDelta.y = lstFrame1Keys[nKey]+(lstFrame2Keys[nKey]-lstFrame1Keys[nKey])*fTime;
				nKey++;
			}
			if (nAnimatedComponents & AZ) {
				vDelta.z = lstFrame1Keys[nKey]+(lstFrame2Keys[nKey]-lstFrame1Keys[nKey])*fTime;
				nKey++;
			}

			// Rotation
			if (nAnimatedComponents & AYaw ||
				nAnimatedComponents & APitch ||
				nAnimatedComponents & ARoll ||
				nAnimatedComponents & AW) {
				if (nAnimatedComponents & AYaw) {
					qDelta1.x = lstFrame1Keys[nKey];
					qDelta2.x = lstFrame2Keys[nKey];
					nKey++;
				}
				if (nAnimatedComponents & APitch) {
					qDelta1.y = lstFrame1Keys[nKey];
					qDelta2.y = lstFrame2Keys[nKey];
					nKey++;
				}
				if (nAnimatedComponents & ARoll) {
					qDelta1.z = lstFrame1Keys[nKey];
					qDelta2.z = lstFrame2Keys[nKey];
					nKey++;
				}
				if (nAnimatedComponents & AW) {
					qDelta1.w = lstFrame1Keys[nKey];
					qDelta2.w = lstFrame2Keys[nKey];
					nKey++;
				} else {
					// Compute last component (w) of the quaternions
					float fSqw = 1.0f - qDelta1.x*qDelta1.x - qDelta1.y*qDelta1.y - qDelta1.z*qDelta1.z;
					qDelta1.w = (fSqw <= 0.0f) ? 0.0f : -Math::Sqrt(fSqw);
					fSqw = 1.0f - qDelta2.x*qDelta2.x - qDelta2.y*qDelta2.y - qDelta2.z*qDelta2.z;
					qDelta2.w = (fSqw <= 0.0f) ? 0.0f : -Math::Sqrt(fSqw);
				}

				// Interpolate quaternions
				qDelta.Slerp(qDelta1, qDelta2, fTime);
			}

			// Add deltas
			qDeltaFinal.Slerp(Quaternion::Identity, qDelta, fWeight);
			cJH.SetTranslation((vDelta*fWeight)+cJH.GetTranslation());
			cJH.SetRotation(qDeltaFinal*cJH.GetRotation());
		}
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Public virtual AnimationInfo functions                ]
//[-------------------------------------------------------]
JointAni &JointAni::operator =(const JointAni &cSource)
{
	// Call base function
	*static_cast<AnimationBase*>(this) = *static_cast<const AnimationBase*>(&cSource);

	// Copy data
	m_lstAniJoints = cSource.m_lstAniJoints;

	// Return pointer
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh
