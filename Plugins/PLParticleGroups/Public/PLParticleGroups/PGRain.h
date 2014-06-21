/*********************************************************\
 *  File: PGRain.h                                       *
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


#ifndef __PLPG_PGRAIN_H__
#define __PLPG_PGRAIN_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLParticleGroups/PGPhysics.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLParticleGroups {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Rain particle group
*/
class PGRain : public PGPhysics {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLPG_RTTI_EXPORT, PGRain, "PLParticleGroups", PLParticleGroups::PGPhysics, "Rain particle group")
		// Attributes
		pl_attribute(EnergyPerSec,		PLCore::uint32,	30,							ReadWrite,	DirectValue,	"The energy consumption per second of one particle",					"")
		pl_attribute(EnergyMin,			PLCore::uint32,	300,						ReadWrite,	DirectValue,	"Minimum of energy",													"")
		pl_attribute(EnergyRange,		PLCore::uint32,	90,							ReadWrite,	DirectValue,	"EnergyMin + EnergyRange = particles energy",							"")
		pl_attribute(Brightness,		PLCore::uint32,	250,						ReadWrite,	DirectValue,	"Brightness",															"")
		pl_attribute(MediumSpeed,		float,			150.0f,						ReadWrite,	DirectValue,	"The speed factor of the particles",									"")
		pl_attribute(MediumSize,		PLCore::uint32,	5,							ReadWrite,	DirectValue,	"Medium size for each particles",										"")
		pl_attribute(DownVsAwayRatio,	PLCore::uint32,	150,						ReadWrite,	DirectValue,	"150 -> every 150 particle fly randomly away instead of falling down",	"")
		pl_attribute(BuildPerSec,		PLCore::uint32,	100,						ReadWrite,	DirectValue,	"Particle building speed",												"")
			// Overwritten SNParticleGroup attributes
		pl_attribute(Material,			PLCore::String,	"Data/Textures/PGRain.dds",	ReadWrite,	GetSet,			"Particle group material",												"Type='Material Effect Image TextureAni'")
		pl_attribute(Particles,			PLCore::uint32,	800,						ReadWrite,	GetSet,			"Number of particles",													"Min=1")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLPG_API PGRain();

		/**
		*  @brief
		*    Destructor
		*/
		PLPG_API virtual ~PGRain();


	//[-------------------------------------------------------]
	//[ Private virtual PLScene::SceneNode functions          ]
	//[-------------------------------------------------------]
	private:
		virtual void OnActivate(bool bActivate) override;
		virtual void OnAddedToVisibilityTree(PLScene::VisNode &cVisNode) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Initializes a particle
		*
		*  @param[in] cParticle
		*    Particle to initialize
		*/
		void InitParticle(SNParticleGroup::Particle &cParticle) const;

		/**
		*  @brief
		*    Called when the scene node needs to be updated
		*/
		void OnUpdate();


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<> EventHandlerUpdate;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool m_bUpdate;	/**< Perform an update the next time? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLParticleGroups


#endif // __PLPG_PGRAIN_H__
