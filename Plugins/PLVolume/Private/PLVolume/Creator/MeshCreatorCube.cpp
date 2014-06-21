/*********************************************************\
 *  File: MeshCreatorCube.cpp                            *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Material/MaterialManager.h>
#include <PLMesh/Geometry.h>
#include <PLMesh/MeshManager.h>
#include <PLMesh/MeshLODLevel.h>
#include <PLMesh/MeshMorphTarget.h>
#include "PLVolume/Creator/MeshCreatorCube.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
using namespace PLMesh;
namespace PLVolume {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(MeshCreatorCube)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
MeshCreatorCube::MeshCreatorCube() :
	Dimension(this),
	Offset(this)
{
}

/**
*  @brief
*    Destructor
*/
MeshCreatorCube::~MeshCreatorCube()
{
}


//[-------------------------------------------------------]
//[ Private virtual MeshCreator functions                 ]
//[-------------------------------------------------------]
Mesh *MeshCreatorCube::Create(Mesh &cMesh, uint32 nLODLevel, bool bStatic) const
{
	// Call base implementation
	MeshCreator::Create(cMesh, nLODLevel, bStatic);

	// Get dimension and offset
	const Vector3 &vDimension = Dimension.Get();
	const Vector3 &vOffset    = Offset.Get();

	// Get morph target
	MeshMorphTarget *pMorphTarget = cMesh.GetMorphTarget(0);
	if (pMorphTarget && pMorphTarget->GetVertexBuffer()) {
		VertexBuffer *pVertexBuffer = pMorphTarget->GetVertexBuffer();

		/*
			    3+------+2  y
			    /|     /|   |
			   / |    / |   |
			  / 0+---/--+1  *---x
			7+------+6 /   /
			 | /    | /   z
			 |/     |/
			4+------+5
		*/
		const uint32 nNumOfVertices = 8;
		const float fTextureCoordinate[nNumOfVertices][3] = {
			{0.0f, 0.0f, 0.0f},	// 0
			{1.0f, 0.0f, 0.0f},	// 1
			{1.0f, 1.0f, 0.0f},	// 2
			{0.0f, 1.0f, 0.0f},	// 3
			{0.0f, 0.0f, 1.0f},	// 4
			{1.0f, 0.0f, 1.0f},	// 5
			{1.0f, 1.0f, 1.0f},	// 6
			{0.0f, 1.0f, 1.0f}	// 7
		};
		const uint32 nNumOfIndices = 36;	// 6 faces, 2 triangles per face, 3 indices per triangle
		const uint32 nIndices[nNumOfIndices] = {
			// Back
			2, 3, 0,	// 0
			0, 1, 2,	// 1
			// Front
			7, 6, 5,	// 0
			5, 4, 7,	// 1
			// Left
			3, 7, 4,	// 0
			4, 0, 3,	// 1
			// Right
			6, 2, 1,	// 0
			1, 5, 6,	// 1
			// Top
			3, 2, 6,	// 0
			6, 7, 3,	// 1
			// Bottom
			0, 4, 5,	// 0
			5, 1, 0		// 1
		};

		// Allocate and setup data
		pVertexBuffer->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float3);
		if (TexCoords)
			pVertexBuffer->AddVertexAttribute(VertexBuffer::TexCoord, 0, VertexBuffer::Float3);
		pVertexBuffer->Allocate(nNumOfVertices, bStatic ? Usage::Static : Usage::Dynamic);
		if (pVertexBuffer->Lock(Lock::WriteOnly)) { // Setup vertices
			if (TexCoords) {
				for (uint32 i=0; i<nNumOfVertices; i++) {
					// Position
					float *pfVertices = static_cast<float*>(pVertexBuffer->GetData(i, VertexBuffer::Position));
					pfVertices[Vector3::X] = fTextureCoordinate[i][0]*vDimension.x + vOffset.x;
					pfVertices[Vector3::Y] = fTextureCoordinate[i][1]*vDimension.y + vOffset.y;
					pfVertices[Vector3::Z] = fTextureCoordinate[i][2]*vDimension.z + vOffset.z;

					// Texture coordinate
					pfVertices = static_cast<float*>(pVertexBuffer->GetData(i, VertexBuffer::TexCoord));
					pfVertices[Vector3::X] = fTextureCoordinate[i][0];
					pfVertices[Vector3::Y] = fTextureCoordinate[i][1];
					pfVertices[Vector3::Z] = fTextureCoordinate[i][2];
				}
			} else {
				for (uint32 i=0; i<nNumOfVertices; i++) {
					// Position
					float *pfVertices = static_cast<float*>(pVertexBuffer->GetData(i, VertexBuffer::Position));
					pfVertices[Vector3::X] = fTextureCoordinate[i][0]*vDimension.x + vOffset.x;
					pfVertices[Vector3::Y] = fTextureCoordinate[i][1]*vDimension.y + vOffset.y;
					pfVertices[Vector3::Z] = fTextureCoordinate[i][2]*vDimension.z + vOffset.z;
				}
			}

			// Unlock the vertex buffer
			pVertexBuffer->Unlock();
		}

		// Get LOD level
		MeshLODLevel *pLODLevel = cMesh.GetLODLevel(nLODLevel);
		if (pLODLevel) {
			// Allocate and setup index buffer
			IndexBuffer *pIndexBuffer = pLODLevel->GetIndexBuffer();
			pIndexBuffer->SetElementTypeByMaximumIndex(pVertexBuffer->GetNumOfElements()-1);
			pIndexBuffer->Allocate(nNumOfIndices, bStatic ? Usage::Static : Usage::Dynamic);
			if (pIndexBuffer->Lock(Lock::WriteOnly)) {
				if (Order) {
					for (uint32 i=0; i<nNumOfIndices; i+=3) {
						pIndexBuffer->SetData(i + 0, nIndices[i + 0]);
						pIndexBuffer->SetData(i + 1, nIndices[i + 1]);
						pIndexBuffer->SetData(i + 2, nIndices[i + 2]);
					}
				} else {
					for (uint32 i=0; i<nNumOfIndices; i+=3) {
						pIndexBuffer->SetData(i + 0, nIndices[i + 2]);
						pIndexBuffer->SetData(i + 1, nIndices[i + 1]);
						pIndexBuffer->SetData(i + 2, nIndices[i + 0]);
					}
				}

				// Unlock the index buffer
				pIndexBuffer->Unlock();
			}

			// Create the geometry
			Geometry &cGeometry = (*pLODLevel->GetGeometries()).Add();
			cGeometry.SetPrimitiveType(Primitive::TriangleList);
			cGeometry.SetStartIndex(0);
			cGeometry.SetIndexSize(nNumOfIndices);
		}
	}

	// Return the created mesh
	return &cMesh;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume
