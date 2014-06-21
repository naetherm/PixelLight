/*********************************************************\
 *  File: MeshOctree.h                                   *
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


#ifndef __PLMESH_MESHOCTREE_H__
#define __PLMESH_MESHOCTREE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Octree.h>
#include "PLMesh/PLMesh.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class Bitset;
	template <class AType> class Array;
}
namespace PLGraphics {
	class Color4;
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
*    Mesh octree
*/
class MeshOctree : public PLMath::Octree {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLMESH_API MeshOctree();

		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API virtual ~MeshOctree();

		/**
		*  @brief
		*    Builds the octree
		*
		*  @param[in]  cMeshLODLevel
		*    Owner mesh LOD level
		*  @param[in]  nNumOfGeometries
		*    Number of geometry indices
		*  @param[in]  nGeometries
		*    Geometry indices
		*  @param[out] plstOctreeIDList
		*    Will optionally store a list of octrees each geometry is in if not a null pointer
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLMESH_API bool Build(MeshLODLevel &cMeshLODLevel, PLCore::uint32 nNumOfGeometries, const PLCore::uint32 nGeometries[],
							  PLCore::Array<PLCore::Array<PLCore::uint32>*> *plstOctreeIDList = nullptr);

		/**
		*  @brief
		*    Draws the octree
		*
		*  @param[in] cColor
		*    Color to use
		*  @param[in] mWorldViewProjection
		*    World view projection matrix to use
		*  @param[in] fLineWidth
		*    Line width
		*/
		PLMESH_API void Draw(const PLGraphics::Color4 &cColor, const PLMath::Matrix4x4 &mWorldViewProjection, float fLineWidth = 1.0f) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MeshLODLevel   *m_pOwnerMeshLODLevel;	/**< Pointer to the owner, can be a null pointer */
		PLCore::uint32  m_nNumOfGeometries;		/**< Number of geometry indices */
		PLCore::uint32 *m_pnGeometries;			/**< Geometry indices, can be a null pointer */


	//[-------------------------------------------------------]
	//[ Private virtual PLMath::Octree functions              ]
	//[-------------------------------------------------------]
	private:
		virtual void CustomVisible(PLCore::Bitset *pBitset = nullptr) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh


#endif // __PLMESH_MESHOCTREE_H__
