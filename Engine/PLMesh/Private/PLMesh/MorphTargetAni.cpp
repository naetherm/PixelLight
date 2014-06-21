/*********************************************************\
 *  File: MorphTargetAni.cpp                             *
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
#include "PLMesh/Mesh.h"
#include "PLMesh/MorphTargetAni.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLMesh {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
MorphTargetAni::MorphTargetAni(const String &sName, ElementManager<MorphTargetAni> *pManager) :
	AnimationBase(), Element<MorphTargetAni>(sName, pManager)
{
}

/**
*  @brief
*    Destructor
*/
MorphTargetAni::~MorphTargetAni()
{
}

/**
*  @brief
*    Returns the morph target animation morph targets
*/
Array<MorphTargetAni::AniMorphTarget> &MorphTargetAni::GetMorphTargets()
{
	return m_lstAniMorphTargets;
}

/**
*  @brief
*    Applies the morph target weight from the given morph target frame
*    to the given morph target weights
*/
bool MorphTargetAni::ApplyMorphTargetWeight(Array<float> &lstMorphTargetWeights,
											uint32 nFrame, float fWeight, const Mesh *pMesh) const
{
	// Check weight of this states
	if (fWeight) {
		// Check size of the given weights list
		if (pMesh) {
			if (lstMorphTargetWeights.GetNumOfElements() < pMesh->GetNumOfMorphTargets())
				lstMorphTargetWeights.Resize(pMesh->GetNumOfMorphTargets());
		} else {
			if (lstMorphTargetWeights.GetNumOfElements() < m_lstAniMorphTargets.GetNumOfElements())
				lstMorphTargetWeights.Resize(m_lstAniMorphTargets.GetNumOfElements());
		}

		// Are there any weights?
		if (lstMorphTargetWeights.GetNumOfElements()) {
			// Get the frame keys
			const FrameKeys &cFrameKeys = m_lstFrameKeys[nFrame];
			if (&cFrameKeys == &Array<FrameKeys>::Null)
				return false; // Error!

			// Get frame
			const Array<float> &lstFrameKeysT = cFrameKeys.lstFrameKeys;

			// Loop through all morph targets and add weights
			if (pMesh) {
				for (uint32 i=0; i<m_lstAniMorphTargets.GetNumOfElements(); i++) {
					const int nIndex = pMesh->GetMorphTargetIndex(m_lstAniMorphTargets[i].sName);
					if (nIndex >= 0 && lstMorphTargetWeights[nIndex])
						lstMorphTargetWeights[nIndex] += lstFrameKeysT[i]*fWeight;
				}
			} else {
				for (uint32 i=0; i<m_lstAniMorphTargets.GetNumOfElements(); i++)
					lstMorphTargetWeights[i] += lstFrameKeysT[i]*fWeight;
			}
		}
	}

	// Done
	return true;
}

/**
*  @brief
*    Applies the blended morph target weight from the given morph target frames
*    to the given morph target weights
*/
bool MorphTargetAni::ApplyMorphTargetWeight(Array<float> &lstMorphTargetWeights,
											uint32 nFrame1, uint32 nFrame2, float fTime,
											float fWeight, const Mesh *pMesh) const
{
	// Check weight of this states
	if (fWeight) {
		// Check size of the given weights list
		if (pMesh) {
			if (lstMorphTargetWeights.GetNumOfElements() < pMesh->GetNumOfMorphTargets())
				lstMorphTargetWeights.Resize(pMesh->GetNumOfMorphTargets());
		} else {
			if (lstMorphTargetWeights.GetNumOfElements() < m_lstAniMorphTargets.GetNumOfElements())
				lstMorphTargetWeights.Resize(m_lstAniMorphTargets.GetNumOfElements());
		}

		// Are there any weights?
		if (lstMorphTargetWeights.GetNumOfElements()) {
			// Get the frame keys
			const FrameKeys &cFrame1Keys = m_lstFrameKeys[nFrame1];
			if (&cFrame1Keys == &Array<FrameKeys>::Null)
				return false; // Error!
			const FrameKeys &cFrame2Keys = m_lstFrameKeys[nFrame2];
			if (&cFrame2Keys == &Array<FrameKeys>::Null)
				return false; // Error!

			// Get frames
			const Array<float> &lstFrame1Keys = cFrame1Keys.lstFrameKeys;
			const Array<float> &lstFrame2Keys = cFrame2Keys.lstFrameKeys;

			// Loop through all morph targets and add weights
			if (pMesh) {
				for (uint32 i=0; i<m_lstAniMorphTargets.GetNumOfElements(); i++) {
					const int nIndex = pMesh->GetMorphTargetIndex(m_lstAniMorphTargets[i].sName);
					if (nIndex >= 0 && (&lstMorphTargetWeights[nIndex] != &Array<float>::Null)) {
						const float f1 = lstFrame1Keys[i];
						const float f2 = lstFrame2Keys[i];
						lstMorphTargetWeights[nIndex] += (f1+(f2-f1)*fTime)*fWeight;
					}
				}
			} else {
				for (uint32 i=0; i<m_lstAniMorphTargets.GetNumOfElements(); i++) {
					const float f1 = lstFrame1Keys[i];
					const float f2 = lstFrame2Keys[i];
					lstMorphTargetWeights[i] += (f1+(f2-f1)*fTime)*fWeight;
				}
			}
		}
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Public virtual AnimationInfo functions                ]
//[-------------------------------------------------------]
MorphTargetAni &MorphTargetAni::operator =(const MorphTargetAni &cSource)
{
	// Call base function
	*static_cast<Element<MorphTargetAni>*>(this) = cSource;
	*static_cast<AnimationBase*>(this) = *static_cast<const AnimationBase*>(&cSource);

	// Copy data
	m_lstAniMorphTargets = cSource.m_lstAniMorphTargets;

	// Return pointer
	return *this;
}

/**
*  @brief
*    Get the animation source name
*/
String MorphTargetAni::GetSourceName() const
{
	// To implement in derived classes
	return GetName();
}


//[-------------------------------------------------------]
//[ MorphTargetAniManager                                 ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
MorphTargetAniManager::MorphTargetAniManager()
{
}

/**
*  @brief
*    Destructor
*/
MorphTargetAniManager::~MorphTargetAniManager()
{
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::ElementManager functions      ]
//[-------------------------------------------------------]
MorphTargetAni *MorphTargetAniManager::CreateElement(const String &sName)
{
	return new MorphTargetAni(sName, this);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh
