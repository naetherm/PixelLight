/*********************************************************\
 *  File: SceneRendererPass.h                            *
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


#ifndef __PLSCENE_COMPOSITING_SCENERENDERERPASS_H__
#define __PLSCENE_COMPOSITING_SCENERENDERERPASS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include <PLCore/Container/Element.h>
#include <PLCore/Container/ElementHandler.h>
#include <PLCore/Container/ElementManager.h>
#include "PLScene/PLScene.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class Renderer;
}
namespace PLScene {
	class SQCull;
	class SceneContext;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract scene renderer pass class
*
*  @remarks
*    Usually, the rendering of complex scenes can be split up into smaller, self-containing steps.
*    (one hand does not need to know what the other is doing :)
*    One step/pass will just draw the scene without any lighting or special effects. Another may
*    'draw over' the result for each light in the scene while special steps may only draw some
*    debug information like icons, wireframes and so on. By solve the rendering in such passes,
*    implementing new features, combining existing ones etc. is much easier as if your scene renderer
*    only consists of one bit piece of code - and scene renderers can get REALLY extensive!
*    In short, you can imagine 'scene renderer passes' as 'subsystems' of a scene renderer.
*
*  @note
*    - Derived classes should use a 'SRP' prefix (example: SRPBegin)
*/
class SceneRendererPass : public PLCore::Object, public PLCore::Element<SceneRendererPass> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SceneRenderer;


	//[-------------------------------------------------------]
	//[ Public definition                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene renderer pass flags
		*/
		enum EFlags {
			Inactive = 1<<0	/**< This scene renderer pass is currently NOT active */
		};
		pl_enum(EFlags)
			pl_enum_value(Inactive,	"This scene renderer pass is currently NOT active")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SceneRendererPass, "PLScene", PLCore::Object, "Abstract scene renderer pass class")
		// Attributes
		pl_attribute(Flags,	pl_flag_type(EFlags),	0,	ReadWrite,	GetSet,	"Flags",																				"")
		pl_attribute(Name,	PLCore::String,			"",	ReadWrite,	GetSet,	"Optional scene renderer pass name. If not defined, a name is chosen automatically",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual PLCore::uint32 GetFlags() const;
		PLS_API virtual void SetFlags(PLCore::uint32 nValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the scene context the scene renderer pass is in
		*
		*  @return
		*    The scene context the scene renderer pass is in, can but shouldn't be a null pointer
		*/
		PLS_API SceneContext *GetSceneContext() const;

		/**
		*  @brief
		*    Returns the used renderer
		*
		*  @return
		*    The used renderer, can but shouldn't be a null pointer
		*/
		PLS_API PLRenderer::Renderer *GetRenderer() const;

		/**
		*  @brief
		*    Returns the first found instance of a scene renderer pass instance within the scene renderer by using a class name
		*
		*  @param[in] sClassName
		*    Class name of the scene renderer pass instance
		*
		*  @return
		*    The first found instance of a scene renderer pass instance within the scene renderer, can be a null pointer
		*/
		PLS_API SceneRendererPass *GetFirstInstanceOfSceneRendererPassClass(const PLCore::String &sClassName) const;

		/**
		*  @brief
		*    Is the scene renderer pass active?
		*
		*  @return
		*    'true' if the scene renderer pass is active, else 'false'
		*/
		PLS_API bool IsActive() const;

		/**
		*  @brief
		*    Sets whether the scene renderer pass is active or not
		*
		*  @param[in] bActive
		*    Should the scene renderer pass be active?
		*
		*  @note
		*    - Sets/unsets the 'Inactive'-flag
		*/
		PLS_API void SetActive(bool bActive = true);


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		PLS_API SceneRendererPass();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SceneRendererPass();


	//[-------------------------------------------------------]
	//[ Private virtual SceneRendererPass functions           ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Draws the pass
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cCullQuery
		*    Cull query to use
		*/
		virtual void Draw(PLRenderer::Renderer &cRenderer, const SQCull &cCullQuery) = 0;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::uint32 m_nFlags;	/**< Flags */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_COMPOSITING_SCENERENDERERPASS_H__
