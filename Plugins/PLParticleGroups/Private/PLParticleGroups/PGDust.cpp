/*********************************************************\
 *  File: PGDust.cpp                                     *
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
#include <PLScene/Scene/SceneContext.h>
#include "PLParticleGroups/PGDust.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLScene;
namespace PLParticleGroups {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(PGDust)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
PGDust::PGDust() :
	EventHandlerUpdate(&PGDust::OnUpdate, this),
	m_bUpdate(false)
{
}

/**
*  @brief
*    Destructor
*/
PGDust::~PGDust()
{
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneNode functions          ]
//[-------------------------------------------------------]
void PGDust::InitFunction()
{
	// Call base implementation
	PGPhysics::InitFunction();

	// If there are free particles, create new particles
	float fLength = 0.01f;
	Particle *pParticle = AddParticle();
	while (pParticle) {
		pParticle->fSize	   = fLength + static_cast<float>(Math::GetRand() % 100) / 400.0f;
		pParticle->vColor.a    = pParticle->fEnergy = 0.4f + static_cast<float>(Math::GetRand() % 100) / 500.0f;
		pParticle->vPos		   = GetTransform().GetPosition();
		pParticle->vVelocity.x = Math::GetRandNegFloat()*fLength;
		pParticle->vVelocity.y = Math::GetRandNegFloat()*fLength;
		pParticle->vVelocity.z = -fLength*2-Math::GetRandFloat()*fLength*0.5f;
		pParticle->vColor.r    = pParticle->vColor.g = pParticle->vColor.b = pParticle->fEnergy/2;

		// Next particle, please
		pParticle = AddParticle();
	}
}

void PGDust::OnActivate(bool bActivate)
{
	// Call the base implementation
	PGPhysics::OnActivate(bActivate);

	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate)
			pSceneContext->EventUpdate.Connect(EventHandlerUpdate);
		else
			pSceneContext->EventUpdate.Disconnect(EventHandlerUpdate);
	}
}

void PGDust::OnAddedToVisibilityTree(VisNode &cVisNode)
{
	m_bUpdate = true;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node needs to be updated
*/
void PGDust::OnUpdate()
{
	// If this scene node wasn't drawn at the last frame, we can skip some update stuff
	if ((GetFlags() & ForceUpdate) || m_bUpdate) {
		uint32 nActive = 0;

		m_bUpdate = false;

		{ // Update particles
			float fTimeDiff = Timing::GetInstance()->GetTimeDifference();
			Iterator<Particle> cIterator = GetParticleIterator();
			while (cIterator.HasNext()) {
				Particle &cParticle = cIterator.Next();

				cParticle.fEnergy -= fTimeDiff;
				if (cParticle.fEnergy < 0.0f) {
					RemoveParticle(cParticle);
				} else {
					nActive++;
					cParticle.fSize     += fTimeDiff*4;
					cParticle.vColor.a   = cParticle.fEnergy;
					cParticle.vVelocity += GetGravity()*fTimeDiff*2;
					cParticle.vPos      += cParticle.vVelocity*fTimeDiff;

					// Texture animation
					cParticle.fAnimationTimer += fTimeDiff;
					if (cParticle.fAnimationTimer > 0.04f) {
						cParticle.fAnimationTimer = 0.0f;
						cParticle.nAnimationStep++;
						if (cParticle.nAnimationStep >= GetTextureAnimationSteps())
							cParticle.nAnimationStep = 0;
					}
				}
			}
		}

		// Remove particle group?
		if (nActive) {
			// We have to recalculate the current axis align bounding box in 'scene node space'
			DirtyAABoundingBox();
		} else {
			Delete();
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLParticleGroups
