/*********************************************************\
 *  File: PGFountain.h                                   *
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


#ifndef __PLPG_PGFOUNTAIN_H__
#define __PLPG_PGFOUNTAIN_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLParticleGroups/SNParticleGroup.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLParticleGroups {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Fountain particle group
*
*  @note
*    - PLScene::Particle::vVelocity = Constant speed
*      PLScene::Particle::fCustom1  = Acc factor
*      PLScene::Particle::fCustom2  = Time
*/
class PGFountain : public SNParticleGroup {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLPG_RTTI_EXPORT, PGFountain, "PLParticleGroups", PLParticleGroups::SNParticleGroup, "Fountain particle group")
		// Attributes
		pl_attribute(Steps,					PLCore::uint32,		8,								ReadWrite,	GetSet,	"Steps",					"")
		pl_attribute(RaysPerStep,			PLCore::uint32,		6,								ReadWrite,	GetSet,	"Rays per step",			"")
		pl_attribute(DropsPerRay,			PLCore::uint32,		50,								ReadWrite,	GetSet,	"Drops per ray",			"")
		pl_attribute(AngleOfDeepestStep,	float,				80.0f,							ReadWrite,	GetSet,	"Angle of deepest step",	"")
		pl_attribute(AngleOfHighestStep,	float,				85.0f,							ReadWrite,	GetSet,	"Angle of highest step",	"")
		pl_attribute(RandomAngleAddition,	float,				20.0f,							ReadWrite,	GetSet,	"Random angle addition",	"")
		pl_attribute(AccFactor,				float,				0.11f,							ReadWrite,	GetSet,	"Acc factor",				"")
			// Overwritten SNParticleGroup attributes
		pl_attribute(Material,				PLCore::String,		"Data/Textures/PGFountain.dds",	ReadWrite,	GetSet,	"Particle group material",	"Type='Material Effect Image TextureAni'")
		pl_attribute(Particles,				PLCore::uint32,		600,							ReadWrite,	GetSet,	"Number of particles",		"Min=1")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLPG_API PLCore::uint32 GetSteps() const;
		PLPG_API void SetSteps(PLCore::uint32 nValue);
		PLPG_API PLCore::uint32 GetRaysPerStep() const;
		PLPG_API void SetRaysPerStep(PLCore::uint32 nValue);
		PLPG_API PLCore::uint32 GetDropsPerRay() const;
		PLPG_API void SetDropsPerRay(PLCore::uint32 nValue);
		PLPG_API float GetAngleOfDeepestStep() const;
		PLPG_API void SetAngleOfDeepestStep(float fValue);
		PLPG_API float GetAngleOfHighestStep() const;
		PLPG_API void SetAngleOfHighestStep(float fValue);
		PLPG_API float GetRandomAngleAddition() const;
		PLPG_API void SetRandomAngleAddition(float fValue);
		PLPG_API float GetAccFactor() const;
		PLPG_API void SetAccFactor(float fValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLPG_API PGFountain();

		/**
		*  @brief
		*    Destructor
		*/
		PLPG_API virtual ~PGFountain();


	//[-------------------------------------------------------]
	//[ Private virtual PLScene::SceneNode functions          ]
	//[-------------------------------------------------------]
	private:
		virtual void InitFunction() override;
		virtual void OnActivate(bool bActivate) override;
		virtual void OnAddedToVisibilityTree(PLScene::VisNode &cVisNode) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
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
		// Exported variables
		PLCore::uint32 m_nSteps;
		PLCore::uint32 m_nRaysPerStep;
		PLCore::uint32 m_nDropsPerRay;
		float		   m_fAngleOfDeepestStep;
		float		   m_fAngleOfHighestStep;
		float		   m_fRandomAngleAddition;
		float		   m_fAccFactor;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLParticleGroups


#endif // __PLPG_PGFOUNTAIN_H__
