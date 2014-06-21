/*********************************************************\
 *  File: SNLoadScreenBase.h                             *
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


#ifndef __PLENGINE_COMPOSITING_LOADSCREENBASE_H__
#define __PLENGINE_COMPOSITING_LOADSCREENBASE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLScene/Scene/SceneNode.h>
#include "PLEngine/PLEngine.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLEngine {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract load screen base scene node
*
*  @remarks
*    This scene node listens to the 'load progress'-event of its parent and visualizes it. If
*    the progress is NOT between 0.0-1.0 the load screen will hide itself.
*/
class SNLoadScreenBase : public PLScene::SceneNode {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PL_RTTI_EXPORT, SNLoadScreenBase, "PLEngine", PLScene::SceneNode, "Abstract load screen base scene node")
		// Attributes
			// Overwritten SceneNode attributes
		pl_attribute(Flags,	pl_flag_type(EFlags),	NoCulling,	ReadWrite,	GetSet,	"Flags",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PL_API SNLoadScreenBase();

		/**
		*  @brief
		*    Destructor
		*/
		PL_API virtual ~SNLoadScreenBase();

		/**
		*  @brief
		*    Returns the current load progress
		*
		*  @return
		*    The current load progress between 0.0-1.0
		*/
		PL_API float GetLoadProgress() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when the scene node container changed
		*/
		void OnContainer();

		/**
		*  @brief
		*    Called on load progress
		*
		*  @param[in] fLoadProgress
		*    Load progress (0.0-1.0)
		*/
		void OnLoadProgress(float fLoadProgress);


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<>		EventHandlerContainer;
		PLCore::EventHandler<float>	EventHandlerLoadProgress;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLScene::SceneContainer *m_pContainer;		/**< Current parent container, can be a null pointer */
		float					 m_fLoadProgress;	/**< Current load progress (0.0-1.0) */


	//[-------------------------------------------------------]
	//[ Public virtual PLScene::SceneNode functions           ]
	//[-------------------------------------------------------]
	public:
		PL_API virtual void DrawPost(PLRenderer::Renderer &cRenderer, const PLScene::VisNode *pVisNode = nullptr) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_COMPOSITING_LOADSCREENBASE_H__
