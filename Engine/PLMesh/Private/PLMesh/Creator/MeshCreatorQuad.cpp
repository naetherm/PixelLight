/*********************************************************\
 *  File: MeshCreatorQuad.cpp                            *
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
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include "PLMesh/Geometry.h"
#include "PLMesh/MeshManager.h"
#include "PLMesh/MeshLODLevel.h"
#include "PLMesh/MeshMorphTarget.h"
#include "PLMesh/Creator/MeshCreatorQuad.h"


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
pl_implement_class(MeshCreatorQuad)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
MeshCreatorQuad::MeshCreatorQuad() :
	V1(this),
	V2(this),
	V3(this),
	V4(this),
	Offset(this)
{
}

/**
*  @brief
*    Destructor
*/
MeshCreatorQuad::~MeshCreatorQuad()
{
}


//[-------------------------------------------------------]
//[ Private virtual MeshCreator functions                 ]
//[-------------------------------------------------------]
Mesh *MeshCreatorQuad::Create(Mesh &cMesh, uint32 nLODLevel, bool bStatic) const
{
	// Call base implementation
	MeshCreator::Create(cMesh, nLODLevel, bStatic);

	// Get morph target
	MeshMorphTarget *pMorphTarget = cMesh.GetMorphTarget(0);
	if (pMorphTarget && pMorphTarget->GetVertexBuffer()) {
		// Allocate vertex buffer
		VertexBuffer *pVertexBuffer = pMorphTarget->GetVertexBuffer();
		pVertexBuffer->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float3);
		if (TexCoords)
			pVertexBuffer->AddVertexAttribute(VertexBuffer::TexCoord, 0, VertexBuffer::Float2);
		if (Normals)
			pVertexBuffer->AddVertexAttribute(VertexBuffer::Normal,   0, VertexBuffer::Float3);
		pVertexBuffer->Allocate(4, bStatic ? Usage::Static : Usage::Dynamic);

		// Get LOD level
		MeshLODLevel *pLODLevel = cMesh.GetLODLevel(nLODLevel);
		if (pLODLevel && pLODLevel->GetIndexBuffer()) {
			// Allocate index buffer
			IndexBuffer *pIndexBuffer = pLODLevel->GetIndexBuffer();
			pIndexBuffer->SetElementTypeByMaximumIndex(pVertexBuffer->GetNumOfElements()-1);
			pIndexBuffer->Allocate(6, bStatic ? Usage::Static : Usage::Dynamic);

			// Setup vertices & geometry
			if (pVertexBuffer->Lock(Lock::WriteOnly)) {
				if (pIndexBuffer->Lock(Lock::WriteOnly)) {
					Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
					// Get vertices
					const Vector3 &vV1 = V1.Get();
					const Vector3 &vV2 = V2.Get();
					const Vector3 &vV3 = V3.Get();
					const Vector3 &vV4 = V4.Get();

					// Get offset
					const Vector3 &vOffset = Offset.Get();

					// Setup indices
					pIndexBuffer->SetData(0, 0);
					pIndexBuffer->SetData(1, 1);
					pIndexBuffer->SetData(2, 2);
					pIndexBuffer->SetData(3, 0);
					pIndexBuffer->SetData(4, 2);
					pIndexBuffer->SetData(5, 3);

					// Create and setup new geometry
					Geometry &cGeometry = lstGeometries.Add();
					cGeometry.SetPrimitiveType(Primitive::TriangleList);
					cGeometry.SetStartIndex(0);
					cGeometry.SetIndexSize(6);

					// Calculate the face normals
					Vector3 vN1;
					vN1.GetFaceNormal(vV1, vV2, vV3);
					Vector3 vN2;
					vN2.GetFaceNormal(vV1, vV2, vV4);

					// V1
					float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(0, VertexBuffer::Position));
					pfVertex[Vector3::X] = vOffset.x + vV1.x;
					pfVertex[Vector3::Y] = vOffset.y + vV1.y;
					pfVertex[Vector3::Z] = vOffset.z + vV1.z;
					if (TexCoords) {
						pfVertex = static_cast<float*>(pVertexBuffer->GetData(0, VertexBuffer::TexCoord));
						pfVertex[Vector2::X] = 0.0f;
						pfVertex[Vector2::Y] = 1.0f;
					}
					if (Normals) {
						pfVertex = static_cast<float*>(pVertexBuffer->GetData(0, VertexBuffer::Normal));
						pfVertex[Vector3::X] = (vN1.x+vN2.x)/2;
						pfVertex[Vector3::Y] = (vN1.y+vN2.y)/2;
						pfVertex[Vector3::Z] = (vN1.z+vN2.z)/2;
					}

					// V2
					pfVertex = static_cast<float*>(pVertexBuffer->GetData(1, VertexBuffer::Position));
					pfVertex[Vector3::X] = vOffset.x + vV2.x;
					pfVertex[Vector3::Y] = vOffset.y + vV2.y;
					pfVertex[Vector3::Z] = vOffset.z + vV2.z;
					if (TexCoords) {
						pfVertex = static_cast<float*>(pVertexBuffer->GetData(1, VertexBuffer::TexCoord));
						pfVertex[Vector2::X] = 1.0f;
						pfVertex[Vector2::Y] = 1.0f;
					}
					if (Normals) {
						pfVertex = static_cast<float*>(pVertexBuffer->GetData(1, VertexBuffer::Normal));
						pfVertex[Vector3::X] = vN1.x;
						pfVertex[Vector3::Y] = vN1.y;
						pfVertex[Vector3::Z] = vN1.z;
					}

					// V3
					pfVertex = static_cast<float*>(pVertexBuffer->GetData(2, VertexBuffer::Position));
					pfVertex[Vector3::X] = vOffset.x + vV3.x;
					pfVertex[Vector3::Y] = vOffset.y + vV3.y;
					pfVertex[Vector3::Z] = vOffset.z + vV3.z;
					if (TexCoords) {
						pfVertex = static_cast<float*>(pVertexBuffer->GetData(2, VertexBuffer::TexCoord));
						pfVertex[Vector2::X] = 1.0f;
						pfVertex[Vector2::Y] = 0.0f;
					}
					if (Normals) {
						pfVertex = static_cast<float*>(pVertexBuffer->GetData(2, VertexBuffer::Normal));
						pfVertex[Vector3::X] = (vN1.x+vN2.x)/2;
						pfVertex[Vector3::Y] = (vN1.y+vN2.y)/2;
						pfVertex[Vector3::Z] = (vN1.z+vN2.z)/2;
					}

					// V4
					pfVertex = static_cast<float*>(pVertexBuffer->GetData(3, VertexBuffer::Position));
					pfVertex[Vector3::X] = vOffset.x + vV4.x;
					pfVertex[Vector3::Y] = vOffset.y + vV4.y;
					pfVertex[Vector3::Z] = vOffset.z + vV4.z;
					if (TexCoords) {
						pfVertex = static_cast<float*>(pVertexBuffer->GetData(3, VertexBuffer::TexCoord));
						pfVertex[Vector2::X] = 0.0f;
						pfVertex[Vector2::Y] = 0.0f;
					}
					if (Normals) {
						pfVertex = static_cast<float*>(pVertexBuffer->GetData(3, VertexBuffer::Normal));
						pfVertex[Vector3::X] = vN2.x;
						pfVertex[Vector3::Y] = vN2.y;
						pfVertex[Vector3::Z] = vN2.z;
					}

					// Unlock the index buffer
					pIndexBuffer->Unlock();
				}

				// Unlock the vertex buffer
				pVertexBuffer->Unlock();
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
