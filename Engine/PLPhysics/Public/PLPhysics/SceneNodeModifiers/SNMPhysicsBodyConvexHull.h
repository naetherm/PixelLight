/*********************************************************\
 *  File: SNMPhysicsBodyConvexHull.h                     *
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


#ifndef __PLPHYSICS_SCENENODEMODIFIERS_BODYCONVEXHULL_H__
#define __PLPHYSICS_SCENENODEMODIFIERS_BODYCONVEXHULL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLPhysics/SceneNodeModifiers/SNMPhysicsBody.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysics {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Physics convex hull body scene node modifier
*
*  @remarks
*    Convex hulls are the solution to collision primitive that can not be easily represented
*    by and implicit solid. The implicit solid primitives (spheres, cubes, cylinders, capsules,
*    cones, etc.), have constant time complexity for contact calculation and are also extremely
*    efficient on memory usage, therefore the application get perfect smooth behavior. However
*    for cases where the shape is too difficult or a polygonal representation is desired convex
*    hulls are the ultimate solution. For example it is a mistake to model a 10000 point sphere
*    as a convex hull, when the perfect sphere is available.\n
*    There is not upper limit as to how many vertices the application can pass to make
*    a hull shape, however for performance and memory usage concern it is the application
*    responsibility to keep the max vertex at the possible minimum. The minimum number of vertices
*    should be equal or larger than 4 and it is the application responsibility that the points are
*    part of a solid geometry. Unpredictable results will occur if all points happen to be collinear
*    or coplanar.
*/
class SNMPhysicsBodyConvexHull : public SNMPhysicsBody {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLPHYSICS_RTTI_EXPORT, SNMPhysicsBodyConvexHull, "PLPhysics", PLPhysics::SNMPhysicsBody, "Physics convex hull body scene node modifier")
		// Attributes
		pl_attribute(Mesh,	PLCore::String,	"",	ReadWrite,	GetSet,	"Collision mesh, if empty, the variable 'Mesh' (if available) of the scene node is used instead",	"Type='Mesh'")
		// Constructors
		pl_constructor_1(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLPHYSICS_API PLCore::String GetMesh() const;
		PLPHYSICS_API void SetMesh(const PLCore::String &sValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cSceneNode
		*    Owner scene node
		*/
		PLPHYSICS_API SNMPhysicsBodyConvexHull(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~SNMPhysicsBodyConvexHull();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String m_sMesh;	/**< Collision mesh, if empty, the variable 'Mesh' (if available) of the scene node is used instead */


	//[-------------------------------------------------------]
	//[ Private virtual SNMPhysicsBody functions              ]
	//[-------------------------------------------------------]
	private:
		virtual void CreatePhysicsBody() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLPHYSICS_SCENENODEMODIFIERS_BODYCONVEXHULL_H__
