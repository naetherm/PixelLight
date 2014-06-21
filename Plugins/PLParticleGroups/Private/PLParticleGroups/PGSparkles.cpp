/*********************************************************\
 *  File: PGSparkles.cpp                                 *
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
#include <PLCore/System/System.h>
#include <PLScene/Scene/SceneContext.h>
#include "PLParticleGroups/PGSparkles.h"


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
pl_implement_class(PGSparkles)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
PGSparkles::PGSparkles() :
	EnergyPerSec(this),
	EnergyMin(this),
	EnergyRange(this),
	Brightness(this),
	MediumSpeed(this),
	MediumSize(this),
	DownVsAwayRatio(this),
	BuildPerSec(this),
	Material(this),
	Particles(this),
	EventHandlerUpdate(&PGSparkles::OnUpdate, this),
	m_bUpdate(false)
{
	// Overwritten SNParticleGroup variables
	m_sMaterial  = "Data/Textures/PGSparkles.dds";
	m_nParticles = 300;
}

/**
*  @brief
*    Destructor
*/
PGSparkles::~PGSparkles()
{
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneNode functions          ]
//[-------------------------------------------------------]
void PGSparkles::OnActivate(bool bActivate)
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

void PGSparkles::OnAddedToVisibilityTree(VisNode &cVisNode)
{
	m_bUpdate = true;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Initializes a particle
*/
void PGSparkles::InitParticle(Particle &cParticle) const
{
	cParticle.vPos = GetTransform().GetPosition();

	cParticle.vVelocity.x = Math::GetRandNegFloat();
	cParticle.vVelocity.y = Math::GetRandNegFloat();
	cParticle.vVelocity.z = Math::GetRandNegFloat();

	if (System::GetInstance()->GetMicroseconds() % DownVsAwayRatio) {
		float fVelLength = MediumSpeed/cParticle.vVelocity.GetLength();
		cParticle.vVelocity.x *= fVelLength;
		cParticle.vVelocity.z *= fVelLength;
		fVelLength += static_cast<float>(0.2*fVelLength);
		cParticle.vVelocity.y *= fVelLength;
	} else {
		float fVelLength = 1/cParticle.vVelocity.GetLength();
		cParticle.vVelocity.x *= fVelLength*MediumSpeed/5;
		cParticle.vVelocity.y *= fVelLength*MediumSpeed/5;
		cParticle.vVelocity.z *= (fVelLength*MediumSpeed);
		if (cParticle.vVelocity.z > 1)
			cParticle.vVelocity.z = -cParticle.vVelocity.z;
	}
	cParticle.fEnergy = Math::GetRandNegFloat()*EnergyRange + EnergyMin;

	cParticle.vColor[0] = static_cast<float>(Brightness)/255.0f;
	cParticle.vColor[1] = static_cast<float>(Brightness)/255.0f;
	cParticle.vColor[2] = static_cast<float>(Brightness)/255.0f;
	cParticle.vColor[3] = cParticle.fEnergy/255.0f;

	cParticle.fSize    = static_cast<float>(MediumSize + Math::GetRandNegFloat()*MediumSize*0.1)/10;
	cParticle.fCustom1 = cParticle.fSize*1.9f;

	cParticle.bDistorted	 = true;
	cParticle.vDistortion.x = 0.0f;
	cParticle.vDistortion.y = 0.0f;
	cParticle.vDistortion.z = 0.0f;
}

/**
*  @brief
*    Called when the scene node needs to be updated
*/
void PGSparkles::OnUpdate()
{
	// If this scene node wasn't drawn at the last frame, we can skip some update stuff
	if ((GetFlags() & ForceUpdate) || m_bUpdate) {
		m_bUpdate = true;

		// If there are free particles, create new particles
		if (!(System::GetInstance()->GetMicroseconds() % BuildPerSec)) {
			while (Math::GetRand() % 5) {
				Particle *pParticle = AddParticle();
				if (pParticle)
					InitParticle(*pParticle);
				else
					break;
			}
		}

		{ // Update particles
			float fTimeDiff = Timing::GetInstance()->GetTimeDifference();
			Iterator<Particle> cIterator = GetParticleIterator();
			while (cIterator.HasNext()) {
				Particle &cParticle = cIterator.Next();

				// Update velocity
				cParticle.vVelocity += GetGravity()*fTimeDiff*(200-cParticle.fEnergy)/10;
				cParticle.vVelocity -= cParticle.vVelocity*fTimeDiff;

				// Self induction to make the sparkles a bit more interesting ;-)
				if (!(System::GetInstance()->GetMicroseconds() % 500) && !(Math::GetRand() % 5)) {
					cParticle.fEnergy += 100;
					cParticle.fSize   += 0.2f;
				}

				// Update position
				Vector3 vMove = cParticle.vVelocity*fTimeDiff*cParticle.fEnergy/100;
				cParticle.vPos += vMove/10;

				// Update distortion
				cParticle.fCustom1 += fTimeDiff;
				float dLength = cParticle.fCustom1/vMove.GetLength()*5;
				cParticle.vDistortion = vMove*dLength;

				// Update energy, size and lifetime
				cParticle.fEnergy -= fTimeDiff*EnergyPerSec;
				cParticle.fSize   -= fTimeDiff;
				if (cParticle.fEnergy <= 0 || cParticle.fSize <= 0)
					InitParticle(cParticle);
				else
					cParticle.vColor[3] = cParticle.fEnergy/255;
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
