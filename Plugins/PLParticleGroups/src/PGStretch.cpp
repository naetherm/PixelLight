/*********************************************************\
 *  File: PGStretch.cpp                                  *
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
#include "PLParticleGroups/PGStretch.h"


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
pl_class_metadata(PGStretch, "PLParticleGroups", PLParticleGroups::SNParticleGroup, "Stretch particle group")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Attributes
	pl_attribute_metadata(Size,			float,				2.0f,							ReadWrite,	"Size",						"Min='0.0001'")
	pl_attribute_metadata(Length,		float,				10.0f,							ReadWrite,	"Length",					"")
	pl_attribute_metadata(Color,		PLGraphics::Color4,	PLGraphics::Color4::White,		ReadWrite,	"Particle color",			"")
		// Overwritten SNParticleGroup attributes
	pl_attribute_metadata(Material,		PLCore::String,		"Data/Textures/PGStretch.dds",	ReadWrite,	"Particle group material",	"Type='Material Effect Image TextureAni'")
	pl_attribute_metadata(Particles,	PLCore::uint32,		1,								ReadWrite,	"Number of particles",		"Min=1")
pl_class_metadata_end(PGStretch)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
PGStretch::PGStretch() :
	Size(this),
	Length(this),
	Color(this),
	Material(this),
	Particles(this),
	EventHandlerUpdate(&PGStretch::OnUpdate, this),
	m_bUpdate(false)
{
	// Overwritten SNParticleGroup variables
	m_sMaterial  = "Data/Textures/PGStretch.dds";
	m_nParticles = 1;
}

/**
*  @brief
*    Destructor
*/
PGStretch::~PGStretch()
{
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneNode functions          ]
//[-------------------------------------------------------]
void PGStretch::OnActivate(bool bActivate)
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

void PGStretch::OnAddedToVisibilityTree(VisNode &cVisNode)
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
void PGStretch::OnUpdate()
{
	// If this scene node wasn't drawn at the last frame, we can skip some update stuff
	if ((GetFlags() & ForceUpdate) || m_bUpdate) {
		m_bUpdate = false;

		// If there are free particles, create new particles
		Particle *pParticle = AddParticle();
		while (pParticle) {
			pParticle->fSize	   = Size;
			pParticle->vColor	   = Color.Get();
			pParticle->vPos		   = GetTransform().GetPosition();
			pParticle->bDistorted  = true;
			pParticle->fEnergy	   = 1;
			pParticle->vVelocity.x = Math::GetRandNegFloat();
			pParticle->vVelocity.y = Math::GetRandNegFloat();
			pParticle->vVelocity.z = Math::GetRandNegFloat();

			// Next particle, please
			pParticle = AddParticle();
		}

		{ // Update particles
			float fTimeDiff = Timing::GetInstance()->GetTimeDifference();
			Iterator<Particle> cIterator = GetParticleIterator();
			while (cIterator.HasNext()) {
				Particle &cParticle = cIterator.Next();

				cParticle.vPos = GetTransform().GetPosition();
				cParticle.vDistortion.SetXYZ(GetTransform().GetMatrix().GetZAxis().Normalize()*Length);

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

		// We have to recalculate the current axis align bounding box in 'scene node space'
		DirtyAABoundingBox();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLParticleGroups
