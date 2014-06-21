/*********************************************************\
 *  File: SNMPhysicsBodyMesh.h                           *
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


#ifndef __PLPHYSICS_SCENENODEMODIFIERS_BODYMESH_H__
#define __PLPHYSICS_SCENENODEMODIFIERS_BODYMESH_H__
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
*    Physics mesh body scene node modifier
*
*  @remarks
*    Tree collision is the preferred method for collision with polygonal meshes
*    of arbitrary complexity. The mesh must be made of flat non-intersecting polygons, but they
*    do not explicitly need to be triangles. Tree collision can be serialized by the application
*    to/from an arbitrary storage device.\n
*    If concave polygons are added to the tree collision, the variable 'Optimize' must be set to 1.
*    With the optimize variable set to 1, we will optimize the collision mesh by removing non
*    essential edges from adjacent flat polygons. We will not change the topology of the mesh but
*    significantly reduces the number of polygons in the mesh. The reduction factor of the number of
*    polygons in the mesh depends upon the irregularity of the mesh topology. A reduction factor of
*    1.5 to 2 is common. If the variable optimize is set to zero, will leave the mesh geometry unaltered.
*
*  @note
*    - When a tree collision is assigned to a body the mass of the body is ignored in all dynamics
*      calculations. This makes the body behave as a static body.
*/
class SNMPhysicsBodyMesh : public SNMPhysicsBody {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLPHYSICS_RTTI_EXPORT, SNMPhysicsBodyMesh, "PLPhysics", PLPhysics::SNMPhysicsBody, "Physics mesh body scene node modifier")
		// Attributes
		pl_attribute(Mesh,		PLCore::String,	"",		ReadWrite,	GetSet,	"Collision mesh, if empty, the variable 'Mesh' (if available) of the scene node is used instead",								"Type='Mesh'")
		pl_attribute(Optimize,	bool,			false,	ReadWrite,	GetSet,	"Flag that indicates whether it should optimize this mesh. Set to 1 to optimize (may take some time!) the mesh, otherwise 0.",	"")
		// Constructors
		pl_constructor_1(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLPHYSICS_API PLCore::String GetMesh() const;
		PLPHYSICS_API void SetMesh(const PLCore::String &sValue);
		PLPHYSICS_API bool GetOptimize() const;
		PLPHYSICS_API void SetOptimize(bool bValue);


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
		PLPHYSICS_API SNMPhysicsBodyMesh(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~SNMPhysicsBodyMesh();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String m_sMesh;		/**< Collision mesh, if empty, the variable 'Mesh' (if available) of the scene node is used instead */
		bool		   m_bOptimize;	/**< Flag that indicates whether it should optimize this mesh. Set to 1 to optimize (may take some time!) the mesh, otherwise 0. */


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


#endif // __PLPHYSICS_SCENENODEMODIFIERS_BODYMESH_H__
