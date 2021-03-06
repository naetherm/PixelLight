/*********************************************************\
 *  File: SNSystem.h                                     *
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


#ifndef __SPARK_PL_SNSPARK_H__
#define __SPARK_PL_SNSPARK_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLScene/Scene/SceneNode.h>
#include "SPARK_PL/SPARK_PL.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace SPK {
	class System;
}
namespace SPARK_PL {
	class SPK_PLRenderer;
	class SPK_PLQuadRenderer;
	class SPK_PLLineRenderer;
	class SPK_PLPointRenderer;
	class SPK_PLLineTrailRenderer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace SPARK_PL {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract SPARK particle system scene node base class
*/
class SNSystem : public PLScene::SceneNode {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node flags (PLScene::SceneNode flags extension)
		*/
		enum EFlags {
			UpdateInvisible = 1<<10,	/**< Do also update the particle system if it's currently not visible */
			NoAutomaticAABB = 1<<11,	/**< Do not automatically update the axis aligned bounding box */
			NoShaders       = 1<<12		/**< Do not use shaders, use fixed functions instead (not recommended) */
		};
		pl_flag(EFlags)
			pl_enum_base(SceneNode::EFlags)
			pl_enum_value(UpdateInvisible,	"Do also update the particle system if it's currently not visible")
			pl_enum_value(NoAutomaticAABB,	"Do not automatically update the axis aligned bounding box")
			pl_enum_value(NoShaders,		"Do not use shaders, use fixed functions instead (not recommended)")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(SPARK_PL_API)
		// Attributes
		pl_attribute_directvalue(			ShaderLanguage,	PLCore::String,	"",	ReadWrite)
			// Overwritten PLScene::SceneNode attributes
		pl_attribute_getset		(SNSystem,	Flags,			PLCore::uint32,	0,	ReadWrite)
		// Slots
		pl_slot_0_def(SNSystem,	OnUpdate)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		SPARK_PL_API SNSystem();

		/**
		*  @brief
		*    Destructor
		*/
		SPARK_PL_API virtual ~SNSystem();


	//[-------------------------------------------------------]
	//[ Public virtual PLScene::SceneNode functions           ]
	//[-------------------------------------------------------]
	public:
		SPARK_PL_API virtual void DrawTransparent(PLRenderer::Renderer &cRenderer, const PLScene::VisNode *pVisNode = nullptr) override;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Creates and registers a new SPK_PLPointRenderer
		*
		*  @param[in] cRenderer
		*    PixelLight renderer to use
		*  @param[in] fSize
		*    The size of the points
		*
		*  @return
		*    A new registered SPK_PLPointRenderer
		*/
		SPARK_PL_API SPK_PLPointRenderer *CreateSPK_PLPointRenderer(PLRenderer::Renderer &cRenderer, float fSize = 1.0f);

		/**
		*  @brief
		*    Creates and registers a new SPK_PLQuadRenderer
		*
		*  @param[in] cRenderer
		*    PixelLight renderer to use
		*  @param[in] fScaleX
		*    The scale of the width of the quad
		*  @param[in] fScaleY
		*    The scale of the height of the quad
		*
		*  @return
		*    A new registered SPK_PLQuadRenderer
		*/
		SPARK_PL_API SPK_PLQuadRenderer *CreateSPK_PLQuadRenderer(PLRenderer::Renderer &cRenderer, float fScaleX = 1.0f, float fScaleY = 1.0f);

		/**
		*  @brief
		*    Creates and registers a new SPK_PLLineRenderer
		*
		*  @param[in] cRenderer
		*    PixelLight renderer to use
		*  @param[in] fLength
		*    The length multiplier of this SPK_PLLineRenderer
		*  @param[in] fWidth
		*    The width of this SPK_PLLineRenderer in pixels
		*
		*  @return
		*    A new registered SPK_PLLineRenderer
		*/
		SPARK_PL_API SPK_PLLineRenderer *CreateSPK_PLLineRenderer(PLRenderer::Renderer &cRenderer, float fLength = 1.0f, float fWidth = 1.0f);

		/**
		*  @brief
		*    Creates and registers a new SPK_PLLineTrailRenderer
		*
		*  @param[in] cRenderer
		*    PixelLight renderer to use
		*
		*  @return
		*    A new registered SPK_PLLineTrailRenderer
		*/
		SPARK_PL_API SPK_PLLineTrailRenderer *CreateSPK_PLLineTrailRenderer(PLRenderer::Renderer &cRenderer);


	//[-------------------------------------------------------]
	//[ Protected virtual SNSystem functions                  ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called just before the particle system is updated
		*
		*  @note
		*    - The default implementation is empty
		*/
		SPARK_PL_API virtual void Update();


	//[-------------------------------------------------------]
	//[ Private virtual PLScene::SceneNode functions          ]
	//[-------------------------------------------------------]
	private:
		SPARK_PL_API virtual void OnActivate(bool bActivate) override;
		SPARK_PL_API virtual void UpdateAABoundingBox() override;
		SPARK_PL_API virtual void OnAddedToVisibilityTree(PLScene::VisNode &cVisNode) override;


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
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		SPK::System					   *m_pParticleSystem;		/**< SPARK particle system, can be a null pointer */
		PLCore::Array<SPK_PLRenderer*>  m_lstSPK_PLRenderer;	/**< Used SPK_PLRenderer instances */


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool m_bUpdate;	/**< Perform an update the next time? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // SPARK_PL


#endif // __SPARK_PL_SNSPARK_H__
