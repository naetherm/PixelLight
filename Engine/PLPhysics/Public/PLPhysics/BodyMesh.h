/*********************************************************\
 *  File: BodyMesh.h                                     *
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


#ifndef __PLPHYSICS_BODYMESH_H__
#define __PLPHYSICS_BODYMESH_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector3.h>
#include "PLPhysics/Body.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysics {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract PL physics mesh body
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
*    - When a mesh is assigned to a body the mass of the body is ignored in all dynamics
*      calculations. This makes the body behave as a static body.
*    - The "TwoSided" material parameter should be supported
*/
class BodyMesh : public Body {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~BodyMesh();

		/**
		*  @brief
		*    Returns the collision mesh
		*
		*  @return
		*    The collision mesh
		*/
		PLPHYSICS_API PLCore::String GetMesh() const;

		/**
		*  @brief
		*    Returns the mesh scale
		*
		*  @return
		*    The mesh scale
		*/
		PLPHYSICS_API const PLMath::Vector3 &GetMeshScale() const;

		/**
		*  @brief
		*    Returns whether the physics API is allowed to optimize the mesh or not (if supported)
		*
		*  @return
		*    'true' if the physics API is allowed to optimize the mesh, else 'false'
		*/
		PLPHYSICS_API bool GetOptimize() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cWorld
		*    World this body is in
		*  @param[in] cBodyImpl
		*    Reference to the physics API specific body implementation
		*  @param[in] sMesh
		*    Collision mesh
		*  @param[in] vMeshScale
		*    Mesh scale
		*  @param[in] bOptimize
		*    Allow the physics API to optimize the mesh? (if supported)
		*/
		PLPHYSICS_API BodyMesh(World &cWorld, BodyImpl &cBodyImpl, const PLCore::String &sMesh, const PLMath::Vector3 &vMeshScale, bool bOptimize);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::String  m_sMesh;		/**< Collision mesh */
		PLMath::Vector3	m_vMeshScale;	/**< Mesh scale */
		bool			m_bOptimize;	/**< Allow the physics API to optimize the mesh? (if supported) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLPHYSICS_BODYMESH_H__
