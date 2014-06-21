/*********************************************************\
 *  File: PGMagic2.cpp                                   *
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
#include "PLParticleGroups/PGMagic2.h"


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
pl_implement_class(PGMagic2)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
PGMagic2::PGMagic2() :
	SmallParticles(this),
	TexAni(this),
	Size(this),
	Material(this),
	Particles(this),
	TextureAnimationColumns(this),
	TextureAnimationRows(this),
	EventHandlerUpdate(&PGMagic2::OnUpdate, this),
	m_bUpdate(false)
{
	// Overwritten SNParticleGroup variables
	m_sMaterial				   = "Data/Textures/PGMagic2.dds";
	m_nParticles			   = 100;
	m_nTextureAnimationColumns = 2;
	m_nTextureAnimationRows    = 1;
}

/**
*  @brief
*    Destructor
*/
PGMagic2::~PGMagic2()
{
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneNode functions          ]
//[-------------------------------------------------------]
void PGMagic2::InitFunction()
{
	// Call base implementation
	SNParticleGroup::InitFunction();

	// If there are free particles, create new particles
	Particle *pParticle = AddParticle();
	while (pParticle) {
		pParticle->fSize	 = 2.0f*Size;
		pParticle->vColor.r  = 0.8f+Math::GetRandFloat()*0.2f;
		pParticle->vColor.g  = 0.8f+Math::GetRandFloat()*0.2f;
		pParticle->vColor.b  = 0.8f+Math::GetRandFloat()*0.2f;
		pParticle->vColor.a  = Math::Max(0.8f, Math::GetRandFloat());
		pParticle->fEnergy   = 1.0f;
		pParticle->fCustom2	 = 0.5f+Math::GetRandFloat()*2.0f;
		if (Math::GetRandFloat() > 0.5f)
			pParticle->fCustom2 = -pParticle->fCustom2;
		if (!SmallParticles || (Math::GetRand() % 10) == 5) {
			pParticle->nAnimationStep = 0;
			pParticle->nCustom1 = 1;
			pParticle->fSize = (8.0f+(static_cast<float>(Math::GetRand() % 800)/200))*Size;
			pParticle->vPos = pParticle->vFixPos = pParticle->vDistortion = GetTransform().GetPosition();
		} else {
			pParticle->nAnimationStep = 1;
			pParticle->nCustom1 = 0;
			pParticle->fSize = (1.0f+(static_cast<float>(Math::GetRand() % 1000)/500))*Size;
			for (int i=0; i<3; i++) {
				if ((Math::GetRand() % 2))
					pParticle->vPos[i] = pParticle->vFixPos[i] = pParticle->vDistortion[i] = static_cast<float>(Math::GetRand() % 200)/100+GetTransform().GetPosition()[i];
				else
					pParticle->vPos[i] = pParticle->vFixPos[i] = pParticle->vDistortion[i] = -static_cast<float>(Math::GetRand() % 200)/100+GetTransform().GetPosition()[i];
			}
		}
		pParticle->fCustom1 = pParticle->fSize;
		pParticle->fCustom2 *= 2;

		// Next particle, please
		pParticle = AddParticle();
	}
}

void PGMagic2::OnActivate(bool bActivate)
{
	// Call the base implementation
	SNParticleGroup::OnActivate(bActivate);

	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate)
			pSceneContext->EventUpdate.Connect(EventHandlerUpdate);
		else
			pSceneContext->EventUpdate.Disconnect(EventHandlerUpdate);
	}
}

void PGMagic2::OnAddedToVisibilityTree(VisNode &cVisNode)
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
void PGMagic2::OnUpdate()
{
	// If this scene node wasn't drawn at the last frame, we can skip some update stuff
	if ((GetFlags() & ForceUpdate) || m_bUpdate) {
		m_bUpdate = false;

		{ // Update particles
			float fTimeDiff = Timing::GetInstance()->GetTimeDifference();
			Iterator<Particle> cIterator = GetParticleIterator();
			while (cIterator.HasNext()) {
				Particle &cParticle = cIterator.Next();

				cParticle.fSize  = cParticle.fCustom1*Math::Sin(cParticle.vVelocity.GetLength()/10);
				cParticle.fRot  += fTimeDiff*cParticle.fCustom2;

				// Texture animation
				if (TexAni) {
					cParticle.fAnimationTimer += fTimeDiff;
					if (cParticle.fAnimationTimer > 0.2f) {
						cParticle.fAnimationTimer = 0.0f;
						cParticle.nAnimationStep++;
						if (cParticle.nAnimationStep >= GetTextureAnimationSteps())
							cParticle.nAnimationStep = 0;
					}
				}

				for (int i=0; i<3; i++) {
					if (cParticle.nCustom1) {
						if (cParticle.vPos[i] > cParticle.vDistortion[i])
							cParticle.vVelocity[i] -= fTimeDiff*15;
						else {
							if (cParticle.vPos[i] < cParticle.vDistortion[i])
								cParticle.vVelocity[i] += fTimeDiff*15;
						}
						if (cParticle.vVelocity[i] > 100.0f)
							cParticle.vVelocity[i] = 100.0f;
						if (cParticle.vVelocity[i] < -100.0f)
							cParticle.vVelocity[i] = -100.0f;
					} else {
						if (cParticle.vPos[i] > cParticle.vDistortion[i])
							cParticle.vVelocity[i] -= fTimeDiff*20;
						else {
							if (cParticle.vPos[i] < cParticle.vDistortion[i])
								cParticle.vVelocity[i] += fTimeDiff*20;
						}
						if (cParticle.vVelocity[i] > 200.0f)
							cParticle.vVelocity[i] = 200.0f;
						if (cParticle.vVelocity[i] < -200.0f)
							cParticle.vVelocity[i] = -200.0f;
					}

					cParticle.vPos[i] += cParticle.vVelocity[i]*fTimeDiff;

					if (cParticle.nCustom1) {
						if ((cParticle.vPos[i] <= cParticle.vDistortion[i]+0.1f &&
							 cParticle.vPos[i] >= cParticle.vDistortion[i]-0.1f) ||
							 cParticle.vPos[i] >= cParticle.vFixPos[i]+2.0f ||
							 cParticle.vPos[i] <= cParticle.vFixPos[i]-2.0) {
							if (cParticle.vDistortion[i] != cParticle.vFixPos[i])
								cParticle.vVelocity[i] *= 0.4f;
							if (cParticle.vPos[i] > cParticle.vFixPos[i]+1.0f ||
								cParticle.vPos[i] < cParticle.vFixPos[i]-1.0f)
								cParticle.vDistortion[i] = cParticle.vFixPos[i];
							else {
								if (Math::GetRand() % 2)
									cParticle.vDistortion[i] -= 1.0f+(Math::GetRand() % 200)/100+GetTransform().GetPosition()[i];
								else
									cParticle.vDistortion[i] += 1.0f+(Math::GetRand() % 200)/100+GetTransform().GetPosition()[i];
							}
						}
					} else {
						if ((cParticle.vPos[i] <= cParticle.vDistortion[i]+0.1f &&
							 cParticle.vPos[i] >= cParticle.vDistortion[i]-0.1f) ||
							 cParticle.vPos[i] >= cParticle.vFixPos[i]+4.0f ||
							 cParticle.vPos[i] <= cParticle.vFixPos[i]-4.0f) {
							if (cParticle.vDistortion[i] != cParticle.vFixPos[i])
								cParticle.vVelocity[i] *= 0.8f;
							if (cParticle.vPos[i] > cParticle.vFixPos[i]+1.0f ||
								cParticle.vPos[i] < cParticle.vFixPos[i]-1.0f)
								cParticle.vDistortion[i] = cParticle.vFixPos[i];
							else {
								if (Math::GetRand() % 2)
									cParticle.vDistortion[i] -= 1.0f+(Math::GetRand() % 200)/100+GetTransform().GetPosition()[i];
								else
									cParticle.vDistortion[i] += 1.0f+(Math::GetRand() % 200)/100+GetTransform().GetPosition()[i];
							}
						}
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
