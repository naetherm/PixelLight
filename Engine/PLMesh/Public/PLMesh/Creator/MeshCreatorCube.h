/*********************************************************\
 *  File: MeshCreatorCube.h                              *
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


#ifndef __PLMESH_MESHCREATOR_CUBE_H__
#define __PLMESH_MESHCREATOR_CUBE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector3.h>
#include "PLMesh/Creator/MeshCreator.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class VertexBuffer;
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
*    Cube mesh creator class
*
*  @note
*    - If texture coordinates are generated each side has its own vertices
*    - Order of the materials of MultiMaterials is true:
*      x-positive (0), x-negative (1), y-positive (2),
*      y-negative (3), z-positive (4), z-negative (5)
*/
class MeshCreatorCube : public MeshCreator {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLMESH_RTTI_EXPORT, MeshCreatorCube, "PLMesh", PLMesh::MeshCreator, "Cube mesh creator class")
		// Attributes
		pl_attribute(Dimension,			PLMath::Vector3,	PLMath::Vector3(0.5f, 0.5f, 0.5f),	ReadWrite,	DirectValue,	"Cube dimension into positive/negative direction around the center",	"")
		pl_attribute(MultiMaterials,	bool,				false,								ReadWrite,	DirectValue,	"Should each side has it's own material?",								"")
		pl_attribute(Offset,			PLMath::Vector3,	PLMath::Vector3::Zero,				ReadWrite,	DirectValue,	"Offset to center",														"")
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
		PLMESH_API MeshCreatorCube();

		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API virtual ~MeshCreatorCube();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Sets the four normal vectors of a cube side
		*
		*  @param[in] cVertexBuffer
		*    Vertex buffer to use
		*  @param[in] nSide
		*    Cube side
		*  @param[in] fX
		*    X component of the normal vector
		*  @param[in] fY
		*    Y component of the normal vector
		*  @param[in] fZ
		*    Z component of the normal vector
		*/
		void SetNormals(PLRenderer::VertexBuffer &cVertexBuffer, PLCore::uint32 nSide, float fX, float fY, float fZ) const;


	//[-------------------------------------------------------]
	//[ Private virtual MeshCreator functions                 ]
	//[-------------------------------------------------------]
	private:
		virtual Mesh *Create(Mesh &cMesh, PLCore::uint32 nLODLevel = 0, bool bStatic = true) const override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh


#endif // __PLMESH_MESHCREATOR_CUBE_H__
