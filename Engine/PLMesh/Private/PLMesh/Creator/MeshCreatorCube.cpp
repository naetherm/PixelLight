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
#include "PLMesh/Geometry.h"
#include "PLMesh/MeshManager.h"
#include "PLMesh/MeshLODLevel.h"
#include "PLMesh/MeshMorphTarget.h"
#include "PLMesh/Creator/MeshCreatorCube.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
namespace PLMesh {


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
	MultiMaterials(this),
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
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets the four normal vectors of a cube side
*/
void MeshCreatorCube::SetNormals(VertexBuffer &cVertexBuffer, uint32 nSide, float fX, float fY, float fZ) const
{
	// Set the normal vector of each corner vertex
	for (uint32 i=0; i<4; i++) {
		float *pfVertices = static_cast<float*>(cVertexBuffer.GetData(i + nSide*4, VertexBuffer::Normal));
		pfVertices[Vector3::X] = fX;
		pfVertices[Vector3::Y] = fY;
		pfVertices[Vector3::Z] = fZ;
	}
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
		if (TexCoords || Normals) {
			// Allocate data
			pVertexBuffer->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float3);
			if (TexCoords)
				pVertexBuffer->AddVertexAttribute(VertexBuffer::TexCoord, 0, VertexBuffer::Float2);
			if (Normals)
				pVertexBuffer->AddVertexAttribute(VertexBuffer::Normal, 0, VertexBuffer::Float3);
			pVertexBuffer->Allocate(24, bStatic ? Usage::Static : Usage::Dynamic);
			if (pVertexBuffer->Lock(Lock::WriteOnly)) { // Setup vertices
			// x-positive (0)
				// 0
				float *pfVertices = static_cast<float*>(pVertexBuffer->GetData(0, VertexBuffer::Position));
				pfVertices[Vector3::X] =  vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] =  vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] = -vDimension.z + vOffset.z;

				// 1
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(1, VertexBuffer::Position));
				pfVertices[Vector3::X] = vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] = vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] = vDimension.z + vOffset.z;

				// 2
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(2, VertexBuffer::Position));
				pfVertices[Vector3::X] =  vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] = -vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] =  vDimension.z + vOffset.z;

				// 3
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(3, VertexBuffer::Position));
				pfVertices[Vector3::X] =  vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] = -vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] = -vDimension.z + vOffset.z;

			// x-negative (1)
				// 4
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(4, VertexBuffer::Position));
				pfVertices[Vector3::X] = -vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] =  vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] =  vDimension.z + vOffset.z;

				// 5
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(5, VertexBuffer::Position));
				pfVertices[Vector3::X] = -vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] =  vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] = -vDimension.z + vOffset.z;

				// 6
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(6, VertexBuffer::Position));
				pfVertices[Vector3::X] = -vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] = -vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] = -vDimension.z + vOffset.z;

				// 7
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(7, VertexBuffer::Position));
				pfVertices[Vector3::X] = -vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] = -vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] =  vDimension.z + vOffset.z;

			// y-positive (2)
				// 8
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(8, VertexBuffer::Position));
				pfVertices[Vector3::X] =  vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] =  vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] = -vDimension.z + vOffset.z;

				// 9
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(9, VertexBuffer::Position));
				pfVertices[Vector3::X] = -vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] =  vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] = -vDimension.z + vOffset.z;

				// 10
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(10, VertexBuffer::Position));
				pfVertices[Vector3::X] = -vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] =  vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] =  vDimension.z + vOffset.z;

				// 11
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(11, VertexBuffer::Position));
				pfVertices[Vector3::X] = vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] = vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] = vDimension.z + vOffset.z;

			// y-negative (3)
				// 12
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(12, VertexBuffer::Position));
				pfVertices[Vector3::X] =  vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] = -vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] =  vDimension.z + vOffset.z;

				// 13
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(13, VertexBuffer::Position));
				pfVertices[Vector3::X] = -vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] = -vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] =  vDimension.z + vOffset.z;

				// 14
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(14, VertexBuffer::Position));
				pfVertices[Vector3::X] = -vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] = -vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] = -vDimension.z + vOffset.z;

				// 15
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(15, VertexBuffer::Position));
				pfVertices[Vector3::X] =  vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] = -vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] = -vDimension.z + vOffset.z;

			// z-positive (4)
				// 16
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(16, VertexBuffer::Position));
				pfVertices[Vector3::X] =  vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] =  vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] =  vDimension.z + vOffset.z;

				// 17
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(17, VertexBuffer::Position));
				pfVertices[Vector3::X] = -vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] =  vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] =  vDimension.z + vOffset.z;

				// 18
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(18, VertexBuffer::Position));
				pfVertices[Vector3::X] = -vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] = -vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] =  vDimension.z + vOffset.z;

				// 19
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(19, VertexBuffer::Position));
				pfVertices[Vector3::X] =  vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] = -vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] =  vDimension.z + vOffset.z;

			// z-negative (5)
				// 20
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(20, VertexBuffer::Position));
				pfVertices[Vector3::X] = -vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] =  vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] = -vDimension.z + vOffset.z;

				// 21
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(21, VertexBuffer::Position));
				pfVertices[Vector3::X] =  vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] =  vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] = -vDimension.z + vOffset.z;

				// 22
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(22, VertexBuffer::Position));
				pfVertices[Vector3::X] =  vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] = -vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] = -vDimension.z + vOffset.z;

				// 23
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(23, VertexBuffer::Position));
				pfVertices[Vector3::X] = -vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] = -vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] = -vDimension.z + vOffset.z;

				// Setup texture coordinates
				if (TexCoords) {
					for (int nSide=0; nSide<6; nSide++) {
						// 0
						pfVertices = static_cast<float*>(pVertexBuffer->GetData(0+nSide*4, VertexBuffer::TexCoord));
						pfVertices[Vector2::X] = 1.0f;
						pfVertices[Vector2::Y] = 0.0f;

						// 1
						pfVertices = static_cast<float*>(pVertexBuffer->GetData(1+nSide*4, VertexBuffer::TexCoord));
						pfVertices[Vector2::X] = 0.0f;
						pfVertices[Vector2::Y] = 0.0f;

						// 2
						pfVertices = static_cast<float*>(pVertexBuffer->GetData(2+nSide*4, VertexBuffer::TexCoord));
						pfVertices[Vector2::X] = 0.0f;
						pfVertices[Vector2::Y] = 1.0f;

						// 3
						pfVertices = static_cast<float*>(pVertexBuffer->GetData(3+nSide*4, VertexBuffer::TexCoord));
						pfVertices[Vector2::X] = 1.0f;
						pfVertices[Vector2::Y] = 1.0f;
					}
				}

				// Setup normal vectors
				if (Normals) {
					// x-positive (0)
					SetNormals(*pVertexBuffer, 0, 1.0f, 0.0f, 0.0f);

					// x-negative (1)
					SetNormals(*pVertexBuffer, 1, -1.0f, 0.0f, 0.0f);

					// y-positive (2)
					SetNormals(*pVertexBuffer, 2, 0.0f, 1.0f, 0.0f);

					// y-negative (3)
					SetNormals(*pVertexBuffer, 3, 0.0f, -1.0f, 0.0f);

					// z-positive (4)
					SetNormals(*pVertexBuffer, 4, 0.0f, 0.0f, 1.0f);

					// z-negative (5)
					SetNormals(*pVertexBuffer, 5, 0.0f, 0.0f, -1.0f);
				}

				// Unlock the vertex buffer
				pVertexBuffer->Unlock();
			}

			// Get LOD level
			MeshLODLevel *pLODLevel = cMesh.GetLODLevel(nLODLevel);
			if (pLODLevel && pLODLevel->GetIndexBuffer()) {
				// Allocate and setup index buffer
				IndexBuffer *pIndexBuffer = pLODLevel->GetIndexBuffer();
				pIndexBuffer->SetElementTypeByMaximumIndex(pVertexBuffer->GetNumOfElements()-1);
				pIndexBuffer->Allocate(24, bStatic ? Usage::Static : Usage::Dynamic);
				if (pIndexBuffer->Lock(Lock::WriteOnly)) {
					for (int nSide=0; nSide<6; nSide++) {
						if (Order) {
							pIndexBuffer->SetData(nSide*4+0, nSide*4+3-2);
							pIndexBuffer->SetData(nSide*4+1, nSide*4+3-3);
							pIndexBuffer->SetData(nSide*4+2, nSide*4+3-1);
							pIndexBuffer->SetData(nSide*4+3, nSide*4+3-0);
						} else {
							pIndexBuffer->SetData(nSide*4+0, nSide*4+3-1);
							pIndexBuffer->SetData(nSide*4+1, nSide*4+3-0);
							pIndexBuffer->SetData(nSide*4+2, nSide*4+3-2);
							pIndexBuffer->SetData(nSide*4+3, nSide*4+3-3);
						}
					}

					// Unlock the index buffer
					pIndexBuffer->Unlock();
				}

				// Create the geometries
				Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
				for (int i=0; i<6; i++) {
					Geometry &cGeometry = lstGeometries.Add();
					cGeometry.SetPrimitiveType(Primitive::TriangleStrip);
					cGeometry.SetStartIndex(i*4);
					cGeometry.SetIndexSize(4);
					if (MultiMaterials && i) {
						cMesh.AddMaterial(cMesh.GetRenderer()->GetRendererContext().GetMaterialManager().Create());
						cGeometry.SetMaterial(i);
					}
				}
			}
		} else { // No texture coordinates
			// Allocate and setup data
			pVertexBuffer->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float3);
			pVertexBuffer->Allocate(8, bStatic ? Usage::Static : Usage::Dynamic);
			if (pVertexBuffer->Lock(Lock::WriteOnly)) { // Setup vertices
				// 0
				float *pfVertices = static_cast<float*>(pVertexBuffer->GetData(0, VertexBuffer::Position));
				pfVertices[Vector3::X] = -vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] =  vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] = -vDimension.z + vOffset.z;

				// 1
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(1, VertexBuffer::Position));
				pfVertices[Vector3::X] =  vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] =  vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] = -vDimension.z + vOffset.z;

				// 2
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(2, VertexBuffer::Position));
				pfVertices[Vector3::X] =  vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] =  vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] =  vDimension.z + vOffset.z;

				// 3
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(3, VertexBuffer::Position));
				pfVertices[Vector3::X] = -vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] =  vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] =  vDimension.z + vOffset.z;

				// 4
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(4, VertexBuffer::Position));
				pfVertices[Vector3::X] = -vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] = -vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] = -vDimension.z + vOffset.z;

				// 5
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(5, VertexBuffer::Position));
				pfVertices[Vector3::X] =  vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] = -vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] = -vDimension.z + vOffset.z;

				// 6
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(6, VertexBuffer::Position));
				pfVertices[Vector3::X] =  vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] = -vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] =  vDimension.z + vOffset.z;

				// 7
				pfVertices = static_cast<float*>(pVertexBuffer->GetData(7, VertexBuffer::Position));
				pfVertices[Vector3::X] = -vDimension.x + vOffset.x;
				pfVertices[Vector3::Y] = -vDimension.y + vOffset.y;
				pfVertices[Vector3::Z] =  vDimension.z + vOffset.z;

				// Unlock the vertex buffer
				pVertexBuffer->Unlock();
			}

			// Get LOD level
			MeshLODLevel *pLODLevel = cMesh.GetLODLevel(nLODLevel);
			if (pLODLevel) {
				// Allocate and setup index buffer
				IndexBuffer *pIndexBuffer = pLODLevel->GetIndexBuffer();
				pIndexBuffer->SetElementTypeByMaximumIndex(pVertexBuffer->GetNumOfElements()-1);
				pIndexBuffer->Allocate(24, bStatic ? Usage::Static : Usage::Dynamic);
				if (pIndexBuffer->Lock(Lock::WriteOnly)) {
					if (Order) {
						// Left
						pIndexBuffer->SetData(0,  7);
						pIndexBuffer->SetData(1,  4);
						pIndexBuffer->SetData(2,  3);
						pIndexBuffer->SetData(3,  0);
						// Front
						pIndexBuffer->SetData(4,  1);
						pIndexBuffer->SetData(5,  0);
						pIndexBuffer->SetData(6,  5);
						pIndexBuffer->SetData(7,  4);
						// Right
						pIndexBuffer->SetData(8,  5);
						pIndexBuffer->SetData(9,  6);
						pIndexBuffer->SetData(10, 1);
						pIndexBuffer->SetData(11, 2);
						// Back
						pIndexBuffer->SetData(12, 6);
						pIndexBuffer->SetData(13, 7);
						pIndexBuffer->SetData(14, 2);
						pIndexBuffer->SetData(15, 3);
						// Top
						pIndexBuffer->SetData(16, 2);
						pIndexBuffer->SetData(17, 3);
						pIndexBuffer->SetData(18, 1);
						pIndexBuffer->SetData(19, 0);
						// Bottom
						pIndexBuffer->SetData(20, 5);
						pIndexBuffer->SetData(21, 4);
						pIndexBuffer->SetData(22, 6);
						pIndexBuffer->SetData(23, 7);
					} else {
						// Left
						pIndexBuffer->SetData(0,  3);
						pIndexBuffer->SetData(1,  0);
						pIndexBuffer->SetData(2,  7);
						pIndexBuffer->SetData(3,  4);
						// Front
						pIndexBuffer->SetData(4,  5);
						pIndexBuffer->SetData(5,  4);
						pIndexBuffer->SetData(6,  1);
						pIndexBuffer->SetData(7,  0);
						// Right
						pIndexBuffer->SetData(8,  1);
						pIndexBuffer->SetData(9,  2);
						pIndexBuffer->SetData(10, 5);
						pIndexBuffer->SetData(11, 6);
						// Back 
						pIndexBuffer->SetData(12, 2);
						pIndexBuffer->SetData(13, 3);
						pIndexBuffer->SetData(14, 6);
						pIndexBuffer->SetData(15, 7);
						// Top
						pIndexBuffer->SetData(16, 1);
						pIndexBuffer->SetData(17, 0);
						pIndexBuffer->SetData(18, 2);
						pIndexBuffer->SetData(19, 3);
						// Bottom
						pIndexBuffer->SetData(20, 6);
						pIndexBuffer->SetData(21, 7);
						pIndexBuffer->SetData(22, 5);
						pIndexBuffer->SetData(23, 4);
					}

					// Unlock the index buffer
					pIndexBuffer->Unlock();
				}

				// Create the geometries
				Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
				for (int i=0; i<6; i++) {
					Geometry &cGeometry = lstGeometries.Add();
					cGeometry.SetPrimitiveType(Primitive::TriangleStrip);
					cGeometry.SetStartIndex(i*4);
					cGeometry.SetIndexSize(4);
				}
			}
		}
	}

	// Return the created mesh
	return &cMesh;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh
