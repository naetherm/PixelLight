/*********************************************************\
 *  File: PGLeaf.h                                       *
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


#ifndef __PLPG_PGLEAF_H__
#define __PLPG_PGLEAF_H__
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
*    Leaf particle group
*/
class PGLeaf : public SNParticleGroup {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLPG_RTTI_EXPORT, PGLeaf, "PLParticleGroups", PLParticleGroups::SNParticleGroup, "Leaf particle group")
		// Attributes
		pl_attribute(Radius,			float,				5.0f,								ReadWrite,	DirectValue,	"Leaf area radius",			"Min='0.0001'")
		pl_attribute(Height,			float,				15.0f,								ReadWrite,	DirectValue,	"Maximum leaf height",		"")
		pl_attribute(FloorHeight,		float,				0.0f,								ReadWrite,	DirectValue,	"Floor height",				"")
		pl_attribute(Wind,				PLMath::Vector3,	PLMath::Vector3(0.2f, 0.5f, -1.5f),	ReadWrite,	DirectValue,	"Wind direction",			"")
		pl_attribute(LeafSize,			float,				0.8f,								ReadWrite,	DirectValue,	"Leaf size",				"Min='0.0001'")
		pl_attribute(LeafSizeVariation,	float,				0.5f,								ReadWrite,	DirectValue,	"Leaf size variation",		"Min='0.0'")
			// Overwritten SNParticleGroup attributes
		pl_attribute(Material,			PLCore::String,		"Data/Effects/PGLeaf.plfx",			ReadWrite,	GetSet,			"Particle group material",	"Type='Material Effect Image TextureAni'")
		pl_attribute(Particles,			PLCore::uint32,		100,								ReadWrite,	GetSet,			"Number of particles",		"Min=1")
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
		PLPG_API PGLeaf();

		/**
		*  @brief
		*    Destructor
		*/
		PLPG_API virtual ~PGLeaf();


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


#endif // __PLPG_PGLEAF_H__
