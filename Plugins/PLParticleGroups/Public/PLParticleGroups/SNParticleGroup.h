/*********************************************************\
 *  File: SNParticleGroup.h                              *
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


#ifndef ___PLPG_PARTICLEGROUP_H__
#define ___PLPG_PARTICLEGROUP_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/FastPool.h>
#include <PLGraphics/Color/Color4.h>
#include <PLRenderer/Material/MaterialHandler.h>
#include <PLMesh/Mesh.h>
#include <PLScene/Scene/SceneNode.h>
#include "PLParticleGroups/PLParticleGroups.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class IndexBuffer;
	class VertexBuffer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLParticleGroups {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract particle group scene node
*
*  @remarks
*    If 'Material' is not a material or an effect, the effect of the default value of
*    'Material' is used together with the given texture.
*
*  @note
*    - The 'UpdateAABoundingBox()' implementation for dirty axis align bounding box in 'scene node space'
*      gets the current bounding box by checking each particle. Try to avoid calling the 'dirty' function
*      within your particle group implementation each frame. (performance!)
*/
class SNParticleGroup : public PLScene::SceneNode {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node flags (SceneNode flags extension)
		*/
		enum EFlags {
			ForceUpdate             = 1<<10,	/**< Do always update the particle group, even if it's currently not visible on the screen. */
			PointSprites            = 1<<11,	/**< Use point sprites. Setup the point sprites within the material. */
			UseGlobalColor          = 1<<12,	/**< Use global particle color. Only possible if the 'PointSprites'-flag is set! */
			SceneNodeSpaceParticles = 1<<13		/**< Particles are within the local scene node space */
		};
		pl_enum(EFlags)
			pl_enum_base(SceneNode::EFlags)
			pl_enum_value(ForceUpdate,				"Do always update the particle group, even if it's currently not visible on the screen.")
			pl_enum_value(PointSprites,				"Use point sprites. Setup the point sprites within the material.")
			pl_enum_value(UseGlobalColor,			"Use global particle color. Only possible if the 'PointSprites'-flag is set!")
			pl_enum_value(SceneNodeSpaceParticles,	"Particles are within the local scene node space")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLPG_RTTI_EXPORT, SNParticleGroup, "PLParticleGroups", PLScene::SceneNode, "Abstract particle group scene node")
		// Properties
		pl_properties
			pl_property("Icon",	"Data/Textures/IconParticles.dds")
		pl_properties_end
		// Attributes
		pl_attribute(Material,					PLCore::String,			"Data/Effects/DefaultParticle.plfx",	ReadWrite,	GetSet,			"Particle group material",							"Type='Material Effect Image TextureAni'")
		pl_attribute(GlobalColor,				PLGraphics::Color4,		PLGraphics::Color4::White,				ReadWrite,	DirectValue,	"Global particle color",							"")
		pl_attribute(Particles,					PLCore::uint32,			10,										ReadWrite,	GetSet,			"Number of particles",								"Min=1")
		pl_attribute(TextureAnimationColumns,	PLCore::uint32,			1,										ReadWrite,	GetSet,			"Number of animation frame columns in the texture",	"")
		pl_attribute(TextureAnimationRows,		PLCore::uint32,			1,										ReadWrite,	GetSet,			"Number of animation frame rows in the texture",	"")
			// Overwritten PLScene::SceneNode attributes
		pl_attribute(Flags,						pl_flag_type(EFlags),	0,										ReadWrite,	GetSet,			"Flags",											"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLPG_API PLCore::String GetMaterial() const;
		PLPG_API void SetMaterial(const PLCore::String &sValue);
		PLPG_API PLCore::uint32 GetParticles() const;
		PLPG_API void SetParticles(const PLCore::uint32 &nValue);
		PLPG_API PLCore::uint32 GetTextureAnimationColumns() const;
		PLPG_API void SetTextureAnimationColumns(const PLCore::uint32 &nValue);
		PLPG_API PLCore::uint32 GetTextureAnimationRows() const;
		PLPG_API void SetTextureAnimationRows(const PLCore::uint32 &nValue);
		PLPG_API virtual void SetFlags(PLCore::uint32 nValue) override;	// From PLScene::SceneNode


	//[-------------------------------------------------------]
	//[ Public classes                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    A single particle of the particle group
		*/
		class Particle : public PLCore::FastPoolElement<Particle> {


			//[-------------------------------------------------------]
			//[ Public data                                           ]
			//[-------------------------------------------------------]
			public:
				// General
				bool				bActive;		/**< Is the particle active? If 'false' the particle is not rendered.
														 Use this only in special situations, use Add()/Remove() if possible!  */
				PLMath::Vector3		vPos;			/**< Position */
				PLMath::Vector3		vFixPos;		/**< Fixed position */
				PLMath::Vector3		vVelocity;		/**< Velocity */
				PLGraphics::Color4	vColor;			/**< Color (RGBA) */
				float				fEnergy;		/**< Energy */
				float				fSize;			/**< Size */
				float				fRot;			/**< Rotation (more performance cost if not 0.0f) */

				// Texture animation related
				PLCore::uint32 nAnimationStep;	/**< Texture animation step */
				float		   fAnimationTimer;	/**< Animation timer */

				// Custom variables
				int   nCustom1;	/**< Custom variable 1 */
				int   nCustom2;	/**< Custom variable 2 */
				float fCustom1;	/**< Custom variable 3 */
				float fCustom2;	/**< Custom variable 4 */

				// Advanced features (more performance cost)
				// Own orientation
				PLMath::Quaternion *pRot;	/**< Rotation quaternion for individual particle rotation (no billboarding!)
												 If you want to use it you have to use pRot = new PLMath::Quaternion() when
												 deactivating it delete pRot (Particle group will delete it on destruction
												 automatically) a null pointer if not used. */

				// Distortion
				bool			bDistorted;		/**< Distorted */
				PLMath::Vector3	vDistortion;	/**< Distortion */


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
			public:
				PLPG_API Particle();
				PLPG_API virtual ~Particle();
				PLPG_API bool operator ==(const Particle &cOther) const;


		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Initializes the particle group
		*
		*  @param[in] nMaxNumOfParticles
		*    Maximum possible number of particles
		*  @param[in] sMaterial
		*    Name of the particle material/texture, if empty
		*    the material defined in 'Material' is used
		*  @param[in] pData
		*    Additional data, can be a null pointer
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - Overwrite this function to be able to define your own particle group initialization
		*      which isn't required that often
		*    - Normally, this function calls InitParticles() which further calls
		*      the scene nodes InitFunction() in which you can initialize the individual particles
		*/
		PLPG_API virtual bool InitParticleGroup(PLCore::uint32 nMaxNumOfParticles, const PLCore::String &sMaterial = "", const void *pData = nullptr);

		/**
		*  @brief
		*    Returns the maximum possible number of particles
		*
		*  @return
		*    The maximum possible number of particles
		*/
		PLPG_API PLCore::uint32 GetMaxNumOfParticles() const;

		/**
		*  @brief
		*    Returns the current number of particles
		*
		*  @return
		*    The current number of particles
		*/
		PLPG_API PLCore::uint32 GetNumOfParticles() const;

		/**
		*  @brief
		*    Adds a new particle
		*
		*  @return
		*    Pointer to the new particle, a null pointer on error
		*    (particle limit of this group reached?)
		*
		*  @note
		*    - If GetCreateParticles() is 'false', this function will always return a null pointer
		*/
		PLPG_API Particle *AddParticle();

		/**
		*  @brief
		*    Removes a particle
		*
		*  @param[in] cParticle
		*    Particle to remove
		*/
		PLPG_API void RemoveParticle(Particle &cParticle);

		/**
		*  @brief
		*    Returns a particle iterator
		*
		*  @return
		*    Particle iterator (always valid!)
		*
		*  @note
		*    - You have to delete this iterator by yourself if you no longer need it!
		*/
		PLPG_API PLCore::Iterator<Particle> GetParticleIterator() const;

		/**
		*  @brief
		*    Returns whether the particle group can create new particles or not
		*
		*  @return
		*    'true' the particle group can create new particles, else 'false'
		*/
		PLPG_API bool GetCreateParticles() const;

		/**
		*  @brief
		*    Sets whether the particle group can create new particles or not
		*
		*  @param[in] bCreate
		*    Can the particle group create new particles?
		*
		*  @see
		*    - GetCreateParticles()
		*/
		PLPG_API void SetCreateParticles(bool bCreate = true);

		/**
		*  @brief
		*    Configures the texture animation
		*
		*  @param[in] nColumns
		*    Number of animation frame columns in the texture
		*  @param[in] nRows
		*    Number of animation frame rows in the texture
		*/
		PLPG_API void SetupTextureAnimation(PLCore::uint32 nColumns = 1, PLCore::uint32 nRows = 1);

		/**
		*  @brief
		*    Returns the number of texture animation steps
		*
		*  @return
		*    The number of texture animation steps
		*/
		PLPG_API PLCore::uint32 GetTextureAnimationSteps() const;

		/**
		*  @brief
		*    Returns whether the particle group should be removed automatically after all particles are inactive or not
		*
		*  @return
		*    'true' if the particle group should be removed automatically, else 'false'
		*/
		PLPG_API bool GetRemoveAutomatically() const;

		/**
		*  @brief
		*    Should the particle group be removed automatically after all particles are inactive?
		*
		*  @param[in] bRemoveAutomatically
		*    Should the particle group be removed automatically?
		*/
		PLPG_API void RemoveAutomatically(bool bRemoveAutomatically = false);

		/**
		*  @brief
		*    Returns the handler of the particle group material
		*
		*  @return
		*    Handler of the particle group material
		*/
		PLPG_API const PLRenderer::MaterialHandler &GetMaterialHandler();


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PLPG_API SNParticleGroup();

		/**
		*  @brief
		*    Destructor
		*/
		PLPG_API virtual ~SNParticleGroup();

		/**
		*  @brief
		*    Initializes the particles
		*
		*  @param[in] nMaxNumOfParticles
		*    Maximum possible number of particles
		*  @param[in] sMaterial
		*    Name of the particle material, if empty the particle
		*    group default material is used
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLPG_API bool InitParticles(PLCore::uint32 nMaxNumOfParticles, const PLCore::String &sMaterial = "");


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Create the particle shapes
		*/
		void CreateShapes();

		/**
		*  @brief
		*    Draws the particle shapes
		*
		*  @param[in] cRenderer
		*    The used renderer
		*/
		void DrawShapes(PLRenderer::Renderer &cRenderer);


	//[-------------------------------------------------------]
	//[ Private classes                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Internal particle texture coordinate
		*/
		class ParticleTexCoord {
			public:
			PLMath::Vector2 vTexCoord[4];
			ParticleTexCoord &operator =(const ParticleTexCoord &cSource)
			{
				vTexCoord[0] = cSource.vTexCoord[0];
				vTexCoord[1] = cSource.vTexCoord[1];
				vTexCoord[2] = cSource.vTexCoord[2];
				vTexCoord[3] = cSource.vTexCoord[3];
				return *this;
			}
			bool operator ==(const ParticleTexCoord &cT) const
			{
				for (int i=0; i<4; i++) {
					if (vTexCoord[i] != cT.vTexCoord[i])
						return false;
				}
				return true;
			}
		};


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::String					 m_sMaterial;					/**< Particle group material */
		PLRenderer::MaterialHandler		 m_cMaterial;					/**< Used material */
		PLCore::uint32					 m_nParticles;					/**< Number of particles */
		PLCore::uint32					 m_nTextureAnimationColumns;	/**< Number of animation frame columns in the texture */
		PLCore::uint32					 m_nTextureAnimationRows;		/**< Number of animation frame rows in the texture */
		PLRenderer::IndexBuffer			*m_pIndexBuffer;				/**< Index buffer, can be a null pointer */
		PLRenderer::VertexBuffer		*m_pVertexBuffer;				/**< Vertex buffer, can be a null pointer */
		PLCore::uint32					 m_nUsedIndices;				/**< Number of used indices */
		PLCore::uint32					 m_nUsedVertices;				/**< Number of used vertices */
		bool							 m_bRemoveAutomatically;		/**< Should the particle group be removed automatically? */
		PLCore::uint32					 m_nMaxNumOfParticles;			/**< Maximum number of allowed particles */
		PLCore::FastPool<Particle>		 m_lstParticles;				/**< Particle list */
		bool							 m_bCreateParticles;			/**< Create new particles? */
		PLCore::Array<ParticleTexCoord>	 m_lstTexCoord;					/**< A list of all texture coordinates */


	//[-------------------------------------------------------]
	//[ Public virtual SceneNode functions                    ]
	//[-------------------------------------------------------]
	public:
		PLPG_API virtual void DrawSolid(PLRenderer::Renderer &cRenderer, const PLScene::VisNode *pVisNode = nullptr);
		PLPG_API virtual void DrawTransparent(PLRenderer::Renderer &cRenderer, const PLScene::VisNode *pVisNode = nullptr);


	//[-------------------------------------------------------]
	//[ Protected virtual SceneNode functions                  ]
	//[-------------------------------------------------------]
	protected:
		PLPG_API virtual void InitFunction();
		PLPG_API virtual void DeInitFunction();
		PLPG_API virtual void UpdateAABoundingBox();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLParticleGroups


#endif // ___PLPG_PARTICLEGROUP_H__
