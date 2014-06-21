/*********************************************************\
 *  File: PGFire.cpp                                     *
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
#include <PLScene/Scene/SceneContext.h>
#include "PLParticleGroups/PGFire.h"


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
pl_implement_class(PGFire)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Public constructor
*/
PGFire::PGFire() :
	Size(this),
	Material(this),
	Particles(this),
	EventHandlerUpdate(&PGFire::OnUpdate, this),
	m_bUpdate(false)
{
	// Overwritten SNParticleGroup variables
	m_sMaterial  = "Data/Textures/PGFire.dds";
	m_nParticles = 50;
}

/**
*  @brief
*    Destructor
*/
PGFire::~PGFire()
{
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneNode functions          ]
//[-------------------------------------------------------]
void PGFire::OnActivate(bool bActivate)
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

void PGFire::OnAddedToVisibilityTree(VisNode &cVisNode)
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
void PGFire::OnUpdate()
{
	// If this scene node wasn't drawn at the last frame, we can skip some update stuff
	if ((GetFlags() & ForceUpdate) || m_bUpdate) {
		m_bUpdate = false;

		// If there are free particles, create new particles
		Particle *pParticle = AddParticle();
		while (pParticle) {
			pParticle->fSize	   = (0.02f+Math::GetRandFloat()*0.1f)*Size;
			pParticle->vPos		   = GetTransform().GetPosition();
			pParticle->fEnergy	   = Math::GetRandFloat()*0.5f;
			pParticle->vVelocity.x = Math::GetRandNegFloat()*10*Size;
			pParticle->vVelocity.y = Math::GetRandNegFloat()*10*Size;
			pParticle->vVelocity.z = Math::GetRandNegFloat()*10*Size;
			pParticle->vColor.a    = pParticle->fEnergy;
			pParticle->vColor.r    = pParticle->vColor.g = pParticle->vColor.b = 1.0f;
			pParticle->bDistorted  = false;

			if (!pParticle->pRot)
				pParticle->pRot = new Quaternion();
			Matrix3x3 mRot;
			mRot.LookAt(Vector3::Zero, pParticle->vVelocity.GetNormalized(), Vector3::UnitY);
			pParticle->pRot->FromRotationMatrix(mRot);

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
					cParticle.fSize += fTimeDiff*2*Size;
					cParticle.vColor.a = cParticle.fEnergy*5;
					if (cParticle.vColor.a > 1.0f)
						cParticle.vColor.a = 1.0f;

					cParticle.vVelocity += GetGravity()*fTimeDiff*2*Size;

					Matrix3x3 mRot;
					mRot.LookAt(Vector3::Zero, cParticle.vVelocity.GetNormalized(), Vector3::UnitY);
					cParticle.pRot->FromRotationMatrix(mRot);

					cParticle.vPos += cParticle.vVelocity*fTimeDiff/5;
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
