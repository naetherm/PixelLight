/*********************************************************\
 *  File: PGSmoke.cpp                                    *
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
#include "PLParticleGroups/PGSmoke.h"


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
pl_implement_class(PGSmoke)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
PGSmoke::PGSmoke() :
	Size(this),
	SizeTimeScale(this),
	Energie(this),
	Color(this),
	PositionScale(this),
	Material(this),
	Particles(this),
	TextureAnimationColumns(this),
	TextureAnimationRows(this),
	EventHandlerUpdate(&PGSmoke::OnUpdate, this),
	m_bUpdate(false),
	m_fParticleTime(0.0f),
	m_bCreateNewParticles(true)
{
	// Overwritten SNParticleGroup variables
	m_sMaterial				   = "Data/Textures/PGSmoke.dds";
	m_nParticles			   = 20;
	m_nTextureAnimationColumns = 4;
	m_nTextureAnimationRows    = 4;

	// Init data
	m_SData.fSize         = 0.0f;
	m_SData.fEnergy       = 0.0f;
	m_SData.fParticleTime = 0.0f;
}

/**
*  @brief
*    Destructor
*/
PGSmoke::~PGSmoke()
{
}

/**
*  @brief
*    Returns the default settings of the particle group
*/
void PGSmoke::GetDefaultSettings(PGSmoke::InitData &cData) const
{
	// Set default settings
	cData.fSize         = Size;
	cData.fEnergy       = Energie;
	cData.fParticleTime = 0.01f;
}

/**
*  @brief
*    Returns whether new particles should be created or not
*/
bool PGSmoke::GetCreateNewParticles() const
{
	return m_bCreateNewParticles;
}

/**
*  @brief
*    Sets whether new particles should be created or not
*/
void PGSmoke::CreateNewParticles(bool bCreate)
{
	m_bCreateNewParticles = bCreate;
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneNode functions          ]
//[-------------------------------------------------------]
void PGSmoke::OnActivate(bool bActivate)
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

void PGSmoke::OnAddedToVisibilityTree(VisNode &cVisNode)
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
void PGSmoke::InitParticle(Particle &cParticle) const
{
	float vColor = static_cast<float>(200 + Math::GetRand()%56)/255;
	cParticle.vColor[0]     = vColor*Color.Get().r;
	cParticle.vColor[1]     = vColor*Color.Get().g;
	cParticle.vColor[2]     = vColor*Color.Get().b;
	cParticle.vColor[3]     = 1.0f;
	cParticle.vPos.x        = GetTransform().GetPosition().x + Math::GetRandNegFloat()*PositionScale;
	cParticle.vPos.y        = GetTransform().GetPosition().y + Math::GetRandNegFloat()*PositionScale;
	cParticle.vPos.z        = GetTransform().GetPosition().z + Math::GetRandNegFloat()*PositionScale;
	cParticle.fSize         = m_SData.fSize*2;
	cParticle.fRot          = Math::GetRandNegFloat()*180;
	cParticle.fEnergy       = 255*m_SData.fEnergy;
	cParticle.vVelocity.x   = 5.0f  + 6.0f  * Math::GetRandFloat();	// Lifetime
	cParticle.vVelocity.y   = 1.0f  + 4.0f  * Math::GetRandFloat();	// Frequency
	cParticle.vVelocity.z   = 20.0f + 30.0f * Math::GetRandFloat();	// Intensity
	cParticle.vDistortion.x = 0.0f;
	cParticle.vDistortion.y = 0.0f;
	cParticle.vDistortion.z = 0.0f;
	cParticle.fCustom1      = Math::GetRandNegFloat()*0.5f;
}

/**
*  @brief
*    Called when the scene node needs to be updated
*/
void PGSmoke::OnUpdate()
{
	// If this scene node wasn't drawn at the last frame, we can skip some update stuff
	if ((GetFlags() & ForceUpdate) || m_bUpdate) {
		m_bUpdate = false;

		// Check new particle generation
		float fTimeDiff = Timing::GetInstance()->GetTimeDifference();
		if (!GetRemoveAutomatically() && m_bCreateNewParticles) {
			m_fParticleTime -= fTimeDiff;
			if (m_fParticleTime < 0.0f) {
				// Create a new particle
				m_fParticleTime = m_SData.fParticleTime;
				Particle *pParticle = AddParticle();
				if (pParticle)
					InitParticle(*pParticle);
			}
		}

		uint32 nActive = 0;
		{ // Update particles
			float fFriction = static_cast<float>(Math::Pow(0.3f, fTimeDiff));
			Iterator<Particle> cIterator = GetParticleIterator();
			while (cIterator.HasNext()) {
				Particle &cParticle = cIterator.Next();

				nActive++;

				cParticle.fSize			+= 0.8f*fTimeDiff*SizeTimeScale;
				cParticle.vVelocity.x	-= fTimeDiff;								// Lifetime
				cParticle.vVelocity.z	-= fTimeDiff*20.0f*Math::GetRandFloat();	// Intensity
				cParticle.vPos.x		+= cParticle.vDistortion.x*fTimeDiff;		// Velocity
				cParticle.vPos.y		+= cParticle.vDistortion.y*fTimeDiff;
				cParticle.vPos.z		+= cParticle.vDistortion.z*fTimeDiff;
				cParticle.vDistortion.x	*= fFriction;
				cParticle.vDistortion.y	*= fFriction;
				cParticle.vDistortion.z	*= fFriction;
				cParticle.fRot			+= fTimeDiff*cParticle.fCustom1;

				float vColor = (cParticle.vVelocity.z + 30.0f*Math::Sin(cParticle.vVelocity.x*cParticle.vVelocity.y))/255;
				if (vColor < 0.0f)  {
					vColor = 0.0f;
					if (cParticle.vVelocity.x <= 0.0f) {
						RemoveParticle(cParticle);
						continue;
					}
				}
				cParticle.vColor[3] = vColor*Color.Get().a;

				// Texture animation
				cParticle.fAnimationTimer += fTimeDiff;
				if (cParticle.fAnimationTimer > 0.08f) {
					cParticle.fAnimationTimer = 0.0f;
					cParticle.nAnimationStep++;
					if (cParticle.nAnimationStep >= GetTextureAnimationSteps())
						cParticle.nAnimationStep = 0;
				}
			}
		}

		// Remove particle group?
		if (GetRemoveAutomatically() && !nActive)
			Delete();
		else {
			// We have to recalculate the current axis align bounding box in 'scene node space'
			DirtyAABoundingBox();
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual SNParticleGroup functions              ]
//[-------------------------------------------------------]
bool PGSmoke::InitParticleGroup(uint32 nMaxNumOfParticles, const String &sMaterial, const void *pData)
{
	if (pData) {
		const InitData &cData = *static_cast<const InitData*>(pData);
		m_SData.fSize		  = cData.fSize;
		m_SData.fEnergy		  = cData.fEnergy;
		m_SData.fParticleTime = cData.fParticleTime;
	} else {
		GetDefaultSettings(m_SData);
	}

	// Initialize the particles
	if (!InitParticles(nMaxNumOfParticles, sMaterial))
		return false; // Error!

	// Init data
	m_fParticleTime = m_SData.fParticleTime;

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLParticleGroups
