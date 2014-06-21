/*********************************************************\
 *  File: SRPVolume.h                                    *
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


#ifndef __PLVOLUME_SRPVOLUME_H__
#define __PLVOLUME_SRPVOLUME_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLScene/Compositing/SceneRendererPass.h>
#include "PLVolume/PLVolume.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {
	class VisNode;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolume {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract scene renderer pass for volume scene nodes
*/
class SRPVolume : public PLScene::SceneRendererPass {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene renderer pass flags (PLScene::SceneRendererPass flags extension)
		*/
		enum EFlags {
			DrawOrderFrontToBack = 1<<1,	/**< Draw order is front to back (e.g. useful when rendering solid objects) */
		};
		pl_enum(EFlags)
			pl_enum_base(PLScene::SceneRendererPass::EFlags)
			pl_enum_value(DrawOrderFrontToBack, "Draw order is front to back (e.g. useful when rendering solid objects)")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLVOLUME_RTTI_EXPORT, SRPVolume, "PLVolume", PLScene::SceneRendererPass, "Abstract scene renderer pass for volume scene nodes")
		// Attributes
			// Overwritten PLScene::SceneRendererPass attributes
		pl_attribute(Flags,	pl_flag_type(EFlags),	0,	ReadWrite,	GetSet,	"Flags",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLVOLUME_API virtual ~SRPVolume();

		/**
		*  @brief
		*    Returns a list of all visible scene node instances intersecting with the given scene node
		*
		*  @param[in]  cCullQuery
		*    Cull query to use
		*  @param[in]  cSceneNode
		*    Scene node to intersect with
		*  @param[in]  cClass
		*    RTTI class the intersecting scene nodes must be derived from
		*  @param[out] lstIntersecting
		*    Receives the list of other visible scene nodes intersecting with the given scene node, list is not cleared before new elements are added
		*/
		PLVOLUME_API void GetIntersectingInstancesOf(const PLScene::SQCull &cCullQuery, PLScene::SceneNode &cSceneNode, const PLCore::Class &cClass, PLCore::Array<const PLScene::VisNode*> &lstIntersecting) const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PLVOLUME_API SRPVolume();

		/**
		*  @brief
		*    Searches for the first (= nearest) visible clip plane scene node
		*
		*  @param[in] cCullQuery
		*    Cull query to use
		*
		*  @return
		*    Visibility node referencing the first visible clip plane scene node, a null pointer if no such scene node was found
		*/
		PLVOLUME_API const PLScene::VisNode *GetFirstClipPlane(const PLScene::SQCull &cCullQuery) const;


	//[-------------------------------------------------------]
	//[ Protected virtual PLScene::SceneRendererPass functions ]
	//[-------------------------------------------------------]
	protected:
		PLVOLUME_API virtual void Draw(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery) override;


	//[-------------------------------------------------------]
	//[ Protected virtual SRPVolume functions                 ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Draws a volume scene node
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cCullQuery
		*    Cull query to use
		*  @param[in] cVisNode
		*    Visibility node to use
		*/
		virtual void DrawVolumeSceneNode(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode) = 0;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Draws recursive
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cCullQuery
		*    Cull query to use
		*/
		void DrawRec(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery);

		/**
		*  @brief
		*    Returns a list of all visible scene node instances intersecting with the given scene node, recursive part
		*
		*  @param[in]  cCullQuery
		*    Cull query to use
		*  @param[in]  cSceneNode
		*    Scene node to intersect with
		*  @param[in]  cClass
		*    RTTI class the intersecting scene nodes must be derived from
		*  @param[out] lstIntersecting
		*    Receives the list of other visible scene nodes intersecting with the given scene node, list is not cleared before new elements are added
		*/
		void GetIntersectingInstancesOfRec(const PLScene::SQCull &cCullQuery, PLScene::SceneNode &cSceneNode, const PLCore::Class &cClass, PLCore::Array<const PLScene::VisNode*> &lstIntersecting) const;

		/**
		*  @brief
		*    Searches for the first (= nearest) visible clip plane scene node, recursive part
		*
		*  @param[in] cCullQuery
		*    Cull query to use
		*
		*  @return
		*    Visibility node referencing the first visible clip plane scene node, a null pointer if no such scene node was found
		*/
		const PLScene::VisNode *GetFirstClipPlaneRec(const PLScene::SQCull &cCullQuery) const;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume


#endif // __PLVOLUME_SRPVOLUME_H__
