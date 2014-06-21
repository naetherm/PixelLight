/*********************************************************\
 *  File: SQSphere.h                                     *
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


#ifndef __PLSCENE_SCENEQUERY_SPHERE_H__
#define __PLSCENE_SCENEQUERY_SPHERE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Sphere.h>
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
*    Sphere intersection scene query class
*
*  @remarks
*    This sphere test scene query will enumerate all scene nodes intersecting the
*    given sphere. The intersection sphere will ALWAYS stay an sphere, even if it's transformed
*    into a not uniform scaled child scene container! Therefore, if there are such scaled
*    child scene containers, a new sphere in this container space is calculated automatically.
*    As result, even scene nodes which are not inside the 'original' intersection volume may be
*    within this transformed volume.
*/
class SQSphere : public SceneQuery {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SQSphere, "PLScene", PLScene::SceneQuery, "Sphere intersection scene query class")
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
		PLS_API SQSphere();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SQSphere();

		/**
		*  @brief
		*    Returns the used sphere
		*
		*  @return
		*    The used sphere
		*/
		PLS_API PLMath::Sphere &GetSphere();


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
		PLMath::Sphere m_cSphere;	/**< Sphere used for the query */


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


#endif // __PLSCENE_SCENEQUERY_SPHERE_H__
