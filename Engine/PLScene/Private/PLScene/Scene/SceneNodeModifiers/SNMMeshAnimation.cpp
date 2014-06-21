/*********************************************************\
 *  File: SNMMeshAnimation.cpp                           *
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
#include <PLRenderer/Animation/Animation.h>
#include <PLMesh/MeshHandler.h>
#include <PLMesh/MeshAnimationManager.h>
#include "PLScene/Scene/SceneNodeModifiers/SNMMeshAnimation.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
using namespace PLMesh;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMMeshAnimation)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SNMMeshAnimation::GetName() const
{
	return m_sName;
}

void SNMMeshAnimation::SetName(const String &sValue)
{
	if (m_sName != sValue) {
		m_sName = sValue;
		UpdateMesh();
	}
}

float SNMMeshAnimation::GetFrame() const
{
	return m_fFrame;
}

void SNMMeshAnimation::SetFrame(float fValue)
{
	if (m_fFrame != fValue) {
		m_fFrame = fValue;
		UpdateMesh();
	}
}

float SNMMeshAnimation::GetSpeed() const
{
	return m_fSpeed;
}

void SNMMeshAnimation::SetSpeed(float fValue)
{
	if (m_fSpeed != fValue) {
		m_fSpeed = fValue;
		UpdateMesh();
	}
}

float SNMMeshAnimation::GetWeight() const
{
	return m_fWeight;
}

void SNMMeshAnimation::SetWeight(float fValue)
{
	if (m_fWeight != fValue) {
		m_fWeight = fValue;
		UpdateMesh();
	}
}

float SNMMeshAnimation::GetMinWeight() const
{
	return m_fMinWeight;
}

void SNMMeshAnimation::SetMinWeight(float fValue)
{
	if (m_fMinWeight != fValue) {
		m_fMinWeight = fValue;
		UpdateMesh();
	}
}

float SNMMeshAnimation::GetMaxWeight() const
{
	return m_fMaxWeight;
}

void SNMMeshAnimation::SetMaxWeight(float fValue)
{
	if (m_fMaxWeight != fValue) {
		m_fMaxWeight = fValue;
		UpdateMesh();
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMMeshAnimation::SNMMeshAnimation(SceneNode &cSceneNode) : SNMMesh(cSceneNode),
	Name(this),
	Frame(this),
	Speed(this),
	Weight(this),
	MinWeight(this),
	MaxWeight(this),
	m_fFrame(0.0f),
	m_fSpeed(1.0f),
	m_fWeight(1.0f),
	m_fMinWeight(0.0f),
	m_fMaxWeight(1.0f)
{
}

/**
*  @brief
*    Destructor
*/
SNMMeshAnimation::~SNMMeshAnimation()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Updates the mesh
*/
void SNMMeshAnimation::UpdateMesh()
{
	// Clamp the weight
	if (m_fWeight < m_fMinWeight)
		m_fWeight = m_fMinWeight;
	if (m_fWeight > m_fMaxWeight)
		m_fWeight = m_fMaxWeight;

	// Is there a mesh handler?
	MeshHandler *pMeshHandler = GetSceneNode().GetMeshHandler();
	if (pMeshHandler) {
		// Get/create the mesh animation manager
		MeshAnimationManager *pAniManager = pMeshHandler->GetMeshAnimationManager();
		if (!pAniManager)
			pAniManager = pMeshHandler->CreateMeshAnimationManager();
		if (pAniManager) {
			Animation *pAni = pAniManager->GetByName(m_sName);
			if (!pAni) {
				AnimationInfo *pAniInfo = pMeshHandler->GetAnimationInfo(m_sName);
				if (pAniInfo) {
					pAni = pAniManager->Create(m_sName);
					if (pAni && m_fSpeed) {
						// Start the animation - even if we may stop it in the next step...
						// the animation settings will be still there :D
						pAni->Start(*pAniInfo, true);
					}
				}
			}
			if (pAni) {
				pAni->SetCurrentFrame(m_fFrame);
				pAni->SetSpeed(pAni->GetAnimation() ? m_fSpeed*pAni->GetAnimation()->GetSpeed() : m_fSpeed);
				pAni->SetWeight(m_fWeight);

				// If speed is 0, stop the animation
				if (m_fSpeed) {
					AnimationInfo *pAniInfo = pMeshHandler->GetAnimationInfo(m_sName);
					if (pAniInfo)
						pAni->Start(*pAniInfo);
				} else {
					pAni->Stop();
				}

				// Is the animation currently running?
				if (pAni->IsRunning()) {
					// Ensure that there's a "PLScene::SNMMeshUpdate" instance within the owner scene node which takes care of the frequent mesh update
					GetSNMMeshUpdate();
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
