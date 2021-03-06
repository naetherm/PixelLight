/*********************************************************\
 *  File: PGFume.h                                       *
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


#ifndef __PLPG_PGFUME_H__
#define __PLPG_PGFUME_H__
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
*    Fume particle group
*/
class PGFume : public SNParticleGroup {


	//[-------------------------------------------------------]
	//[ Public structures                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Init data for the particle group
		*/
		struct InitData {
			float fSize;			/**< Radius of the spheres around particle group position, where the particle are created (by default 1.0) */
			float fEnergy;			/**< Energy of the sparkles (by default 1.0) */
			float fParticleTime;	/**< Time until the next trace particle is created (by default 0.01) */
		};


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PLPG_API)
		// Attributes
		pl_attribute_directvalue(			Size,						float,				2.0f,						ReadWrite)
		pl_attribute_directvalue(			SizeTimeScale,				float,				1.0f,						ReadWrite)
		pl_attribute_directvalue(			Energie,					float,				1.0f,						ReadWrite)
		pl_attribute_directvalue(			Color,						PLGraphics::Color4,	PLGraphics::Color4::White,	ReadWrite)
		pl_attribute_directvalue(			PositionScale,				float,				1.0f,						ReadWrite)
			// Overwritten SNParticleGroup attributes
		pl_attribute_getset		(PGFume,	Material,					PLCore::String,		"Data/Effects/PGFume.plfx",	ReadWrite)
		pl_attribute_getset		(PGFume,	Particles,					PLCore::uint32,		20,							ReadWrite)
		pl_attribute_getset		(PGFume,	TextureAnimationColumns,	PLCore::uint32,		4,							ReadWrite)
		pl_attribute_getset		(PGFume,	TextureAnimationRows,		PLCore::uint32,		4,							ReadWrite)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLPG_API PGFume();

		/**
		*  @brief
		*    Destructor
		*/
		PLPG_API virtual ~PGFume();

		/**
		*  @brief
		*    Returns the default settings of the particle group
		*
		*  @param[out] cData
		*    Will receive the default settings
		*/
		PLPG_API void GetDefaultSettings(PGFume::InitData &cData) const;

		/**
		*  @brief
		*    Returns whether new particles should be created or not
		*
		*  @return
		*    'true' if new particles should be created. else 'false'
		*/
		PLPG_API bool GetCreateNewParticles() const;

		/**
		*  @brief
		*    Sets whether new particles should be created or not
		*
		*  @param[in] bCreate
		*    Create new particles?
		*/
		PLPG_API void CreateNewParticles(bool bCreate = true);


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
		bool	 m_bUpdate;				/**< Perform an update the next time? */
		InitData m_SData;				/**< Init data */
		float	 m_fParticleTime;		/**< Timer for particle generation */
		bool	 m_bCreateNewParticles;	/**< Create new particles? */


	//[-------------------------------------------------------]
	//[ Public virtual SNParticleGroup functions              ]
	//[-------------------------------------------------------]
	public:
		virtual bool InitParticleGroup(PLCore::uint32 nMaxNumOfParticles, const PLCore::String &sMaterial = "", const void *pData = nullptr) override;
	

};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLParticleGroups


#endif // __PLPG_PGFUME_H__
