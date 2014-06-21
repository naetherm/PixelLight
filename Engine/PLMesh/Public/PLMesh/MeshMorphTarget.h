/*********************************************************\
 *  File: MeshMorphTarget.h                              *
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


#ifndef __PLMESH_MESH_MORPHTARGET_H__
#define __PLMESH_MESH_MORPHTARGET_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Array.h>
#include <PLMath/Plane.h>
#include "PLMesh/PLMesh.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class VertexBuffer;
}
namespace PLMesh {
	class Mesh;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMesh {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    A morph target alters the vertex data for each frame
*/
class MeshMorphTarget {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Mesh;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Main functions                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pMesh
		*    Pointer to the owner mesh, can be a null pointer
		*/
		PLMESH_API MeshMorphTarget(Mesh *pMesh = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API ~MeshMorphTarget();

		/**
		*  @brief
		*    Returns the mesh the morph target belongs to
		*
		*  @return
		*    Pointer to the owner mesh, can be a null pointer
		*/
		PLMESH_API Mesh *GetMesh() const;

		/**
		*  @brief
		*    Sets the mesh the morph target belongs to
		*
		*  @param[in] pMesh
		*    Pointer to the owner mesh, can be a null pointer
		*/
		PLMESH_API void SetMesh(Mesh *pMesh = nullptr);

		/**
		*  @brief
		*    Returns the name of this morph target
		*
		*  @return
		*    Name of this morph target
		*/
		PLMESH_API PLCore::String GetName() const;

		/**
		*  @brief
		*    Sets the name of this morph target
		*
		*  @param[in] sName
		*    Name of this morph target
		*
		*  @note
		*    - The name muse be unique, if the owner mesh has already a morph target
		*      with this name, the given name is changed automatically
		*/
		PLMESH_API void SetName(const PLCore::String &sName);

		/**
		*  @brief
		*    Returns whether this morph target is relative to the basis morph target or not
		*
		*  @return
		*    'true' if it's relative, else 'false'
		*/
		PLMESH_API bool IsRelative() const;

		/**
		*  @brief
		*    Sets whether this morph target is relative to the basis morph target or not
		*
		*  @param[in] bRelative
		*    'true' if it's relative, else 'false'
		*/
		PLMESH_API void SetRelative(bool bRelative = false);

		/**
		*  @brief
		*    Returns the vertex ID's
		*
		*  @return
		*    Vertex ID's
		*
		*  @note
		*    - Only used if the morph target is relative to the base morph target
		*    - If there are vertex ID's they will be used for the vertex buffer to skip
		*      null deltas
		*/
		PLMESH_API PLCore::Array<PLCore::uint32> &GetVertexIDs();

		/**
		*  @brief
		*    Returns the vertex buffer
		*
		*  @return
		*    Vertex buffer, can be a null pointer
		*/
		PLMESH_API PLRenderer::VertexBuffer *GetVertexBuffer() const;

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    This class instance
		*/
		PLMESH_API MeshMorphTarget &operator =(const MeshMorphTarget &cSource);

		//[-------------------------------------------------------]
		//[ Tool functions                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Builds the current triangle planes
		*
		*  @note
		*    - The triangle planes must be updated after manipulating
		*      geometries or vertices!
		*    - If the triangle list is available (MeshLODLevel::GetTriangleList())
		*      it will be used for faster triangle plane calculation, else the slower
		*      GetTriangle() is used instead.
		*      If there are vertex ID's, we CAN'T build triangle planes because only a few vertices are
		*      influenced by this morph target!
		*/
		PLMESH_API void BuildTrianglePlaneList();

		/**
		*  @brief
		*    Gets the triangle plane list
		*
		*  @return
		*    Triangle plane list
		*
		*  @see
		*    - BuildTrianglePlaneList()
		*/
		PLMESH_API PLCore::Array<PLMath::Plane> &GetTrianglePlaneList();

		/**
		*  @brief
		*    Calculate the vertex normals of the morph target
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - This function will add normals to the vertex buffer if there are no one
		*      allocated yet
		*    - If there are vertex ID's, we CAN'T calculate normals because only a few vertices are
		*      influenced by this morph target!
		*/
		PLMESH_API bool CalculateNormals();

		/**
		*  @brief
		*    Calculates all tangent space vectors of the morph target
		*
		*  @param[in] bTangent
		*    Create tangent vectors?
		*  @param[in] bBinormal
		*    Create binormal vectors?
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe the mesh has no texture coordinates...)
		*
		*  @note
		*    - This function will add tangents and binormals to the vertex buffer if
		*      there are no one allocated yet
		*    - If there are no normals available they will be computed (see CalculateNormals())
		*    - To save memory one can only hold tangent vectors and compute the binormal for
		*      instance within a vertex shader using a cross product of the normal and
		*      tangent vector
		*    - If there are vertex ID's, we CAN'T calculate normals because only a few vertices are
		*      influenced by this morph target!
		*/
		PLMESH_API bool CalculateTangentSpaceVectors(bool bTangent = true, bool bBinormal = true);

		/**
		*  @brief
		*    Calculates the morph target bounding box
		*
		*  @param[out] vMinPos
		*    Will receive the minimum bounding box position
		*  @param[out] vMaxPos
		*    Will receive the maximum bounding box position
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLMESH_API bool CalculateBoundingBox(PLMath::Vector3 &vMinPos, PLMath::Vector3 &vMaxPos) const;

		/**
		*  @brief
		*    Calculates the morph target bounding sphere
		*
		*  @param[out] vPos
		*    Will receive the bounding sphere position
		*  @param[out] fRadius
		*    Will receive the bounding sphere radius
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLMESH_API bool CalculateBoundingSphere(PLMath::Vector3 &vPos, float &fRadius) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Internal data
		PLCore::String  m_sName;							/**< Name of the morph target */
		bool		    m_bRelative;						/**< Is this morph target relative to the basis morph target? */
		Mesh		   *m_pMesh;							/**< Owner mesh, can be a null pointer */

		// Only used if relative
		PLCore::Array<PLCore::uint32> m_lstVertexIDs;		/**< Vertex ID's */

		// Vertex data
		PLRenderer::VertexBuffer *m_pVertexBuffer;			/**< A vertex buffer, can be a null pointer */

		// Precalculated data
		PLCore::Array<PLMath::Plane> m_lstTrianglePlanes;	/**< List of triangle planes */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh


#endif // __PLMESH_MESH_MORPHTARGET_H__
