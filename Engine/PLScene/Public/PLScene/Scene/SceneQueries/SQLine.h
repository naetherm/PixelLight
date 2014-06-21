/*********************************************************\
 *  File: SQLine.h                                       *
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


#ifndef __PLSCENE_SCENEQUERY_LINE_H__
#define __PLSCENE_SCENEQUERY_LINE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Line.h>
#include <PLRenderer/Renderer/Types.h>
#include "PLScene/Scene/SceneQuery.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class SceneHierarchyNode;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Line intersection scene query class
*
*  @remarks
*    This line test scene query will enumerate all scene nodes intersecting the
*    given line.
*/
class SQLine : public SceneQuery {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SQLine, "PLScene", PLScene::SceneQuery, "Line intersection scene query class")
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
		PLS_API SQLine();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SQLine();

		/**
		*  @brief
		*    Returns the used line
		*
		*  @return
		*    The used line
		*/
		PLS_API PLMath::Line &GetLine();

		/**
		*  @brief
		*    Returns the used cull mode
		*
		*  @return
		*    The used cull mode (see "PLRenderer::Cull")
		*
		*   @note
		*     - May not always be used, e.g. usually this is useful for mesh intersection tests
		*/
		PLS_API PLRenderer::Cull::Enum GetCull() const;

		/**
		*  @brief
		*    Sets the used cull mode
		*
		*  @param[in] nCull
		*    The used cull mode (see "PLRenderer::Cull")
		*
		*  @see
		*    - "GetCull()"
		*/
		PLS_API void SetCull(PLRenderer::Cull::Enum nCull = PLRenderer::Cull::CCW);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Recursive part of PerformQuery()
		*
		*  @param[in] cHierarchyNode
		*    Current scene hierarchy node we work on
		*
		*  @return
		*    'true' to continue the query, 'false' if stop it right now
		*/
		bool PerformQueryRec(SceneHierarchyNode &cHierarchyNode);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLMath::Line		   m_cLine;	/**< Line used for the query */
		PLRenderer::Cull::Enum m_nCull;	/**< The cull mode used for the query (see "PLRenderer::Cull") */


	//[-------------------------------------------------------]
	//[ Public virtual SceneQuery functions                   ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual bool PerformQuery() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENEQUERY_LINE_H__
