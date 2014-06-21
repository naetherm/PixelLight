/*********************************************************\
 *  File: SNMMeshMorphCameraNear.cpp                     *
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
#include "PLScene/Scene/SNCamera.h"
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneNodes/SNMesh.h"
#include "PLScene/Scene/SceneNodeModifiers/SNMMeshMorphCameraNear.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMMeshMorphCameraNear)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMMeshMorphCameraNear::SNMMeshMorphCameraNear(SceneNode &cSceneNode) : SNMMeshMorph(cSceneNode),
	Joint(this),
	Distance(this),
	InSpeed(this),
	OutSpeed(this),
	SlotOnUpdate(this)
{
}

/**
*  @brief
*    Destructor
*/
SNMMeshMorphCameraNear::~SNMMeshMorphCameraNear()
{
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNodeModifier functions         ]
//[-------------------------------------------------------]
void SNMMeshMorphCameraNear::OnActivate(bool bActivate)
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
void SNMMeshMorphCameraNear::OnUpdate()
{
	// Get camera
	const SNCamera *pCamera = SNCamera::GetCamera();
	if (pCamera) {
		// Get joint position in world space
		Vector3 vPosition;
		if (!Joint.Get().GetLength() || !static_cast<SNMesh&>(GetSceneNode()).GetSkeletonJointWorldPosition(Joint.Get(), vPosition))
			vPosition = GetSceneNode().GetTransform().GetPosition();

		// Update the morph target weight
		const float fDistance       = pCamera->GetTransform().GetPosition().GetDistance(vPosition);
		const float fPreviousWeight = m_fWeight;
		const float fTimeDiff       = Timing::GetInstance()->GetTimeDifference();
		if (fDistance < Distance) {
			m_fWeight += fTimeDiff*InSpeed;
			if (m_fWeight > m_fMaxWeight)
				m_fWeight = m_fMaxWeight;
		} else {
			m_fWeight -= fTimeDiff*OutSpeed;
			if (m_fWeight < m_fMinWeight)
				m_fWeight = m_fMinWeight;
		}

		// Update the morph target if required
		if (fPreviousWeight != m_fWeight)
			UpdateMorphTarget();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
