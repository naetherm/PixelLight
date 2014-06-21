/*********************************************************\
 *  File: PGSpring.cpp                                   *
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
#include "PLParticleGroups/PGSpring.h"


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
pl_implement_class(PGSpring)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
PGSpring::PGSpring() :
	Size(this),
	RotVelocity(this),
	VelocityScale(this),
	Material(this),
	Particles(this),
	EventHandlerUpdate(&PGSpring::OnUpdate, this),
	m_bUpdate(false)
{
	// Overwritten SNParticleGroup variables
	m_sMaterial  = "Data/Textures/PGSpring.dds";
	m_nParticles = 20;
}

/**
*  @brief
*    Destructor
*/
PGSpring::~PGSpring()
{
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneNode functions          ]
//[-------------------------------------------------------]
void PGSpring::OnActivate(bool bActivate)
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

void PGSpring::OnAddedToVisibilityTree(VisNode &cVisNode)
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
void PGSpring::OnUpdate()
{
	// If this scene node wasn't drawn at the last frame, we can skip some update stuff
	if ((GetFlags() & ForceUpdate) || m_bUpdate) {
		m_bUpdate = false;

		// If there are free particles, create new particles
		Particle *pParticle = AddParticle();
		while (pParticle) {
			pParticle->fSize	   = Math::GetRandFloat()*10*Size;
			pParticle->vColor.a    = Math::GetRandFloat();
			pParticle->vPos		   = GetTransform().GetPosition();
			pParticle->fEnergy	   = Math::GetRandFloat()*2;
			pParticle->vVelocity.x = Math::GetRandNegFloat()*VelocityScale;
			pParticle->vVelocity.y = Math::GetRandNegFloat()*VelocityScale;
			pParticle->vVelocity.z = Math::GetRandNegFloat()*VelocityScale;
			pParticle->vColor.r    = pParticle->vColor.g = pParticle->vColor.b = pParticle->fEnergy;

			// Next particle, please
			pParticle = AddParticle();
		}

		{ // Update particles
			float fTimeDiff = Timing::GetInstance()->GetTimeDifference();
			Iterator<Particle> cIterator = GetParticleIterator();
			while (cIterator.HasNext()) {
				Particle &cParticle = cIterator.Next();

				cParticle.fEnergy -= fTimeDiff;
				if (cParticle.fEnergy < 0.0f) {
					RemoveParticle(cParticle);
				} else {
					cParticle.fSize		+= fTimeDiff*4*Size;
					cParticle.vColor.r   = cParticle.vColor.g = cParticle.vColor.b = cParticle.fEnergy;
					cParticle.vVelocity += GetGravity()*fTimeDiff/10*VelocityScale;
					cParticle.vPos		+= cParticle.vVelocity*fTimeDiff;
					cParticle.fRot		+= fTimeDiff*5*RotVelocity;

					// Texture animation
					cParticle.fAnimationTimer += fTimeDiff;
					if (cParticle.fAnimationTimer > 0.2f) {
						cParticle.fAnimationTimer = 0.0f;
						cParticle.nAnimationStep++;
						if (cParticle.nAnimationStep >= GetTextureAnimationSteps())
							cParticle.nAnimationStep = 0;
					}
				}
			}
		}

		// We have to recalculate the current axis align bounding box in 'scene node space'
		DirtyAABoundingBox();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLParticleGroups
