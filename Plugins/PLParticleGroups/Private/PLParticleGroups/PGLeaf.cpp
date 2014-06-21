/*********************************************************\
 *  File: PGLeaf.cpp                                     *
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
#include <PLMath/EulerAngles.h>
#include <PLScene/Scene/SceneContext.h>
#include "PLParticleGroups/PGLeaf.h"


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
pl_implement_class(PGLeaf)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
PGLeaf::PGLeaf() :
	Radius(this),
	Height(this),
	FloorHeight(this),
	Wind(this),
	LeafSize(this),
	LeafSizeVariation(this),
	Material(this),
	Particles(this),
	EventHandlerUpdate(&PGLeaf::OnUpdate, this),
	m_bUpdate(false)
{
	// Overwritten SNParticleGroup variables
	m_sMaterial  = "Data/Effects/PGLeaf.plfx";
	m_nParticles = 100;
}

/**
*  @brief
*    Destructor
*/
PGLeaf::~PGLeaf()
{
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneNode functions          ]
//[-------------------------------------------------------]
void PGLeaf::OnActivate(bool bActivate)
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

void PGLeaf::OnAddedToVisibilityTree(VisNode &cVisNode)
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
void PGLeaf::InitParticle(Particle &cParticle) const
{
	// Initialize all particles to a random but valued state
	cParticle.vPos.x = (Math::GetRandNegFloat()*Radius) + GetTransform().GetPosition().x;
	cParticle.vPos.z = (Math::GetRandNegFloat()*Radius) + GetTransform().GetPosition().z;
	cParticle.vPos.y = (Math::GetRandFloat()*Height) + GetTransform().GetPosition().y;
	Vector3 vN(Math::GetRandNegFloat(), Math::GetRandNegFloat(), Math::GetRandNegFloat());
	vN.SetLength(1.0f);
	if (!cParticle.pRot)
		cParticle.pRot = new Quaternion();
	EulerAngles::ToQuaternion(static_cast<float>(vN.x*Math::RadToDeg), static_cast<float>(vN.y*Math::RadToDeg), static_cast<float>(vN.z*Math::RadToDeg), *cParticle.pRot);
	cParticle.bDistorted  = true;
	cParticle.fCustom1    = cParticle.fSize = LeafSize + LeafSizeVariation*Math::GetRandNegFloat();
	cParticle.vVelocity.x = Math::GetRandNegFloat();
	cParticle.vVelocity.y = Math::GetRandNegFloat();
	cParticle.vVelocity.z = Math::GetRandNegFloat();
	cParticle.nCustom1    = ((static_cast<int>(Math::GetRandNegFloat()*255.0f*2.0f))<<8)|0xff;
	Vector3 vD = Vector3::UnitY.CrossProduct(vN);
	vD.SetLength(cParticle.fSize);
	cParticle.vDistortion.x = vD.x;
	cParticle.vDistortion.y = vD.y;
	cParticle.vDistortion.z = vD.z;
	cParticle.vFixPos.x = cParticle.vColor[0] = 0.8f;
	cParticle.vFixPos.y = cParticle.vColor[1] = 0.5f+Math::GetRandFloat()*0.5f;
	cParticle.vFixPos.z = cParticle.vColor[2] = 0.5f+Math::GetRandFloat()*0.5f;
	cParticle.vColor[3] = 1.0f;
}

/**
*  @brief
*    Called when the scene node needs to be updated
*/
void PGLeaf::OnUpdate()
{
	// If this scene node wasn't drawn at the last frame, we can skip some update stuff
	if ((GetFlags() & ForceUpdate) || m_bUpdate) {
		m_bUpdate = false;

		// If there are free particles, create new particles
		Particle *pParticle = AddParticle();
		while (pParticle) {
			InitParticle(*pParticle);

			// Next particle, please
			pParticle = AddParticle();
		}

		{ // Update particles
			float fTimeDiff = Timing::GetInstance()->GetTimeDifference();
			Iterator<Particle> cIterator = GetParticleIterator();
			while (cIterator.HasNext()) {
				Particle &cParticle = cIterator.Next();

				// Update forces
				// One: gravity
				cParticle.vVelocity[1] -= fTimeDiff*(cParticle.vPos[1]-FloorHeight)/5;

				// Two wind (dot wind vector normal)
				Vector3 vRot;
				EulerAngles::FromQuaternion(*cParticle.pRot, vRot.x, vRot.y, vRot.z);
				vRot.x *= static_cast<float>(Math::RadToDeg);
				vRot.y *= static_cast<float>(Math::RadToDeg);
				vRot.z *= static_cast<float>(Math::RadToDeg);
				cParticle.vVelocity += Wind.Get()*fTimeDiff*Math::Abs(vRot.DotProduct(Wind.Get()));

				// Four collision (just check whether leaf would end up on wrong side)
				if (cParticle.vPos.y + cParticle.vVelocity.y*fTimeDiff < FloorHeight) {
					// Random bouncy and some friction
					cParticle.vVelocity.y *= -1.0;
					cParticle.vVelocity.z *= Math::GetRandNegFloat();
					cParticle.vVelocity.x *= Math::GetRandNegFloat();
					cParticle.nCustom1     = ((static_cast<int>(Math::GetRandNegFloat()*255.0f*8.0f))<<8) | (cParticle.nCustom1&0xff);
				}

				// Five check whether particle has left area and can die... if it is outside, let it die
				float  fDistx		 = GetTransform().GetPosition().x - cParticle.vPos.x;
				float  fDisty		 = GetTransform().GetPosition().z - cParticle.vPos.z;
				float  fDistToCenter = fDistx*fDistx + fDisty*fDisty;
				uint32 nStart		 = cParticle.nCustom1 & 0xff;
				if (nStart < 0xff) { // It is being spawned
					nStart += static_cast<uint32>(fTimeDiff*512.0f + 0.5f);
					if (nStart > 0xff)
						nStart = 0xff; // Clamp
					cParticle.nCustom1 = (cParticle.nCustom1&0xffffff00)|nStart; // Write to lower 8 bits
				}

				if (fDistToCenter > Radius*Radius) {
					// Start dying... set alpha according to how far it is away
					fDistToCenter = 255 - (Math::Sqrt(fDistToCenter) - Radius)*3.0f;
					cParticle.fSize = static_cast<float>(static_cast<int>(fDistToCenter*(cParticle.nCustom1 & 0xff))>>8)/255*cParticle.fCustom1;
					if (cParticle.fSize < 0.2f)
						InitParticle(cParticle);
				} else {
					cParticle.fSize = static_cast<float>(cParticle.nCustom1 &0xff)/255*cParticle.fCustom1;
				}

				// Update position
				cParticle.vPos += cParticle.vVelocity*fTimeDiff*cParticle.fSize/2;

				// Update spin
				Vector3 vAxis = cParticle.vVelocity;
				vAxis.SetLength(1);

				// Transform normal and distortion
				vAxis *= ((static_cast<float>(cParticle.nCustom1>>8))/255.0f)*fTimeDiff/10;
				Quaternion qRotInc;
				EulerAngles::ToQuaternion(static_cast<float>(vAxis.x*Math::DegToRad), static_cast<float>(vAxis.y*Math::DegToRad), static_cast<float>(vAxis.z*Math::DegToRad), qRotInc);
				*cParticle.pRot *= qRotInc;

				Vector3 d = cParticle.vDistortion;
				d.SetLength(cParticle.fSize);
				cParticle.vDistortion = (*cParticle.pRot)*d;

				// Update color
				EulerAngles::FromQuaternion(*cParticle.pRot, vRot.x, vRot.y, vRot.z);
				vRot.x *= static_cast<float>(Math::RadToDeg);
				vRot.y *= static_cast<float>(Math::RadToDeg);
				vRot.z *= static_cast<float>(Math::RadToDeg);
				vAxis = vRot.Normalize();
				fDistx  = vAxis.x+vAxis.y*2+vAxis.z*3;
				if (fDistx > 1.0f)
					fDistx = 1.0f;
				if (fDistx < 0.5f)
					fDistx = 0.5f;
				cParticle.vColor[0] = cParticle.vFixPos.x*fDistx;
				cParticle.vColor[1] = cParticle.vFixPos.y*fDistx;
				cParticle.vColor[2] = cParticle.vFixPos.z*fDistx;
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
