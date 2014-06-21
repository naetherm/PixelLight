/*********************************************************\
 *  File: PGImage.h                                      *
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


#ifndef __PLPG_PGIMAGE_H__
#define __PLPG_PGIMAGE_H__
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
*    Image particle group
*/
class PGImage : public SNParticleGroup {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLPG_RTTI_EXPORT, PGImage, "PLParticleGroups", PLParticleGroups::SNParticleGroup, "Image particle group")
		// Attributes
		pl_attribute(ImageFilename,		PLCore::String,			"Data/Textures/PGImage_Test.dds",	ReadWrite,	DirectValue,	"Image to create the particles from",	"Type='Image'")
		pl_attribute(RedColorKey,		PLCore::uint8,			0,									ReadWrite,	DirectValue,	"Color key red component",				"")
		pl_attribute(GreenColorKey,		PLCore::uint8,			0,									ReadWrite,	DirectValue,	"Color key green component",			"")
		pl_attribute(BlueColorKey,		PLCore::uint8,			0,									ReadWrite,	DirectValue,	"Color key blue component",				"")
		pl_attribute(ColorKeyTolerance,	char,					0,									ReadWrite,	DirectValue,	"Color key tolerance",					"")
		pl_attribute(ImageScale,		float,					0.1f,								ReadWrite,	DirectValue,	"Image scale",							"")
			// Overwritten SNParticleGroup attributes
		pl_attribute(Material,			PLCore::String,			"Data/Effects/PGImage.plfx",		ReadWrite,	GetSet,			"Particle group material",				"Type='Material Effect Image TextureAni'")
		pl_attribute(Particles,			PLCore::uint32,			1,									ReadWrite,	GetSet,			"Number of particles",					"Min=1")
			// Overwritten PLScene::SceneNode attributes
		pl_attribute(Flags,				pl_flag_type(EFlags),	PointSprites,						ReadWrite,	GetSet,			"Flags",								"")
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
		PLPG_API PGImage();

		/**
		*  @brief
		*    Destructor
		*/
		PLPG_API virtual ~PGImage();


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


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLParticleGroups


#endif // __PLPG_PGIMAGE_H__
