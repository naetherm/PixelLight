/*********************************************************\
 *  File: SceneQuery.h                                   *
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


#ifndef __PLSCENE_SCENEQUERY_H__
#define __PLSCENE_SCENEQUERY_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include <PLCore/Base/Event/Event.h>
#include <PLCore/Container/Element.h>
#include <PLCore/Container/ElementHandler.h>
#include <PLCore/Container/ElementManager.h>
#include "PLScene/PLScene.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class SceneNode;
class SceneContext;
class SceneContainer;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract scene query class
*
*  @remarks
*    The scene query is operating on a scene container. For instance there
*    can be a query to perform volume checks against the scene or a query
*    rendering the scene. This process can be quite efficient if the scene
*    container provides a good scene hierarchy. Events will inform about the results.
*
*  @note
*    - Derived classes should use a 'SQ'-prefix (example: SQLine)
*/
class SceneQuery : public PLCore::Object, public PLCore::Element<SceneQuery> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SceneContainer;
	friend class PLCore::ElementManager<SceneQuery>;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*   Flags
		*/
		enum EFlags {
			Recursive = 1<<0,	/**< Take sub scene containers into account */
			StopQuery = 1<<1	/**< Stop the currently running query (removed within PerformQuery() on start automatically) */
		};


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SceneQuery, "PLScene", PLCore::Object, "Abstract scene query class")
		// Signals
		pl_signal_2(SignalSceneNode,	SceneQuery&,	SceneNode&,	"A scene node was 'touched' during the query. Event causing scene query and found scene node as parameters.",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the scene context the scene query is in
		*
		*  @return
		*    The scene context the scene query is in, can but shouldn't be a null pointer
		*/
		PLS_API SceneContext *GetSceneContext() const;

		/**
		*  @brief
		*    Returns the scene container this query operates on
		*
		*  @return
		*    The scene container this query operates on
		*/
		inline SceneContainer &GetSceneContainer() const;

		/**
		*  @brief
		*    Returns the flags
		*
		*  @return
		*    Flags (see EFlags)
		*/
		inline PLCore::uint32 GetFlags() const;

		/**
		*  @brief
		*    Sets the flags
		*
		*  @param[in] nFlags
		*    Flags (see EFlags)
		*/
		inline void SetFlags(PLCore::uint32 nFlags = Recursive);

		/**
		*  @brief
		*    Stops the currently running query
		*/
		inline void Stop();


	//[-------------------------------------------------------]
	//[ Public virtual SceneQuery functions                   ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Performs the query
		*
		*  @note
		*    - Before you start the query you have to do some settings within
		*      the derived query classes first. (for instance setting the query
		*      volume)
		*
		*  @return
		*    'true' if the query was cancelled by the user, else 'false'
		*/
		virtual bool PerformQuery() = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		PLS_API SceneQuery();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SceneQuery();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::uint32 m_nFlags;	/**< Flags (see EFlags) */


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		SceneContainer *m_pSceneContainer;	/**< Scene container this query operates on, NEVER a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "SceneQuery.inl"


#endif // __PLSCENE_SCENEQUERY_H__
