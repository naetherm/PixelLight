/*********************************************************\
 *  File: MeshCreatorSphere.cpp                          *
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
#include "PLMesh/Creator/MeshCreatorSphere.h"


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
pl_implement_class(MeshCreatorSphere)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
MeshCreatorSphere::MeshCreatorSphere() :
	Radius(this),
	Detail(this),
	Offset(this)
{
}

/**
*  @brief
*    Destructor
*/
MeshCreatorSphere::~MeshCreatorSphere()
{
}


//[-------------------------------------------------------]
//[ Private virtual MeshCreator functions                 ]
//[-------------------------------------------------------]
Mesh *MeshCreatorSphere::Create(Mesh &cMesh, uint32 nLODLevel, bool bStatic) const
{
	// Call base implementation
	MeshCreator::Create(cMesh, nLODLevel, bStatic);

	// Check parameter
	float  fRadius = Radius;
	uint32 nDetail = Detail;
	if (fRadius < 0)
		fRadius = -fRadius;
	if (nDetail < 4 || fRadius <= 0)
		return &cMesh; // Error!

	// Get morph target
	MeshMorphTarget *pMorphTarget = cMesh.GetMorphTarget(0);
	if (pMorphTarget && pMorphTarget->GetVertexBuffer()) {
		// If this is not done, the sphere may be 'opened'
		if (Math::Floor(nDetail/2.0f) != Math::Ceil(nDetail/2.0f))
			nDetail++;

		// Allocate vertex buffer
		VertexBuffer *pVertexBuffer = pMorphTarget->GetVertexBuffer();
		pVertexBuffer->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float3);
		if (TexCoords)
			pVertexBuffer->AddVertexAttribute(VertexBuffer::TexCoord, 0, VertexBuffer::Float2);
		if (Normals)
			pVertexBuffer->AddVertexAttribute(VertexBuffer::Normal,   0, VertexBuffer::Float3);
		pVertexBuffer->Allocate(nDetail*(nDetail+1), bStatic ? Usage::Static : Usage::Dynamic);

		// Get LOD level
		MeshLODLevel *pLODLevel = cMesh.GetLODLevel(nLODLevel);
		if (pLODLevel && pLODLevel->GetIndexBuffer()) {
			// Allocate index buffer
			IndexBuffer *pIndexBuffer = pLODLevel->GetIndexBuffer();
			pIndexBuffer->SetElementTypeByMaximumIndex(pVertexBuffer->GetNumOfElements()-1);
			pIndexBuffer->Allocate(pVertexBuffer->GetNumOfElements(), bStatic ? Usage::Static : Usage::Dynamic);

			// Setup vertices & geometry
			if (pVertexBuffer->Lock(Lock::WriteOnly)) {
				if (pIndexBuffer->Lock(Lock::WriteOnly)) {
					// Get offset
					const Vector3 &vOffset = Offset.Get();

					Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
					uint32 nIndex  = 0;
					uint32 nVertex = 0;
					Vector3 e, p;
					for (uint32 j=0; j<nDetail/2; j++) {
						// Create new geometry
						Geometry &cGeometry = lstGeometries.Add();

						// Setup the new geometry
						cGeometry.SetPrimitiveType(Primitive::TriangleStrip);
						cGeometry.SetStartIndex(nVertex);
						cGeometry.SetIndexSize((nDetail+1)*2);

						double dTheta1 = j*Math::Pi2/nDetail - Math::PiHalf;
						double dTheta2 = (j+1)*Math::Pi2/nDetail - Math::PiHalf;
						for (uint32 i=0; i<=nDetail; i++) {
							double dTheta3 = i*Math::Pi2/nDetail;

							float *pfVertex;
							if (Order) {
								e.x = static_cast<float>(Math::Cos(dTheta2)*Math::Cos(dTheta3));
								e.y = static_cast<float>(Math::Sin(dTheta2));
								e.z = static_cast<float>(Math::Cos(dTheta2)*Math::Sin(dTheta3));
								p.x = fRadius*e.x;
								p.y = fRadius*e.y;
								p.z = fRadius*e.z;
								if (TexCoords) {
									pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::TexCoord));
									pfVertex[Vector2::X] = static_cast<float>(i)/nDetail;
									pfVertex[Vector2::Y] = static_cast<float>(2*(j+1))/nDetail;
								}
								if (Normals) {
									pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Normal));
									pfVertex[Vector3::X] = e.x;
									pfVertex[Vector3::Y] = e.y;
									pfVertex[Vector3::Z] = e.z;
								}
								pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
								pfVertex[Vector3::X] = vOffset.x + p.x;
								pfVertex[Vector3::Y] = vOffset.y + p.y;
								pfVertex[Vector3::Z] = vOffset.z + p.z;
								pIndexBuffer->SetData(nIndex++, nVertex);
								nVertex++;

								e.x = static_cast<float>(Math::Cos(dTheta1)*Math::Cos(dTheta3));
								e.y = static_cast<float>(Math::Sin(dTheta1));
								e.z = static_cast<float>(Math::Cos(dTheta1)*Math::Sin(dTheta3));
								p.x = fRadius*e.x;
								p.y = fRadius*e.y;
								p.z = fRadius*e.z;
								if (TexCoords) {
									pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::TexCoord));
									pfVertex[Vector2::X] = static_cast<float>(i)/nDetail;
									pfVertex[Vector2::Y] = static_cast<float>(2*j)/nDetail;
								}
								if (Normals) {
									pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Normal));
									pfVertex[Vector3::X] = e.x;
									pfVertex[Vector3::Y] = e.y;
									pfVertex[Vector3::Z] = e.z;
								}
								pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
								pfVertex[Vector3::X] = vOffset.x + p.x;
								pfVertex[Vector3::Y] = vOffset.y + p.y;
								pfVertex[Vector3::Z] = vOffset.z + p.z;
								pIndexBuffer->SetData(nIndex++, nVertex);
								nVertex++;
							} else {
								e.x = static_cast<float>(Math::Cos(dTheta1)*Math::Cos(dTheta3));
								e.y = static_cast<float>(Math::Sin(dTheta1));
								e.z = static_cast<float>(Math::Cos(dTheta1)*Math::Sin(dTheta3));
								p.x = fRadius*e.x;
								p.y = fRadius*e.y;
								p.z = fRadius*e.z;
								if (TexCoords) {
									pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::TexCoord));
									pfVertex[Vector2::X] = static_cast<float>(i)/nDetail;
									pfVertex[Vector2::Y] = static_cast<float>(2*j)/nDetail;
								}
								if (Normals) {
									pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Normal));
									pfVertex[Vector3::X] = e.x;
									pfVertex[Vector3::Y] = e.y;
									pfVertex[Vector3::Z] = e.z;
								}
								pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
								pfVertex[Vector3::X] = vOffset.x + p.x;
								pfVertex[Vector3::Y] = vOffset.y + p.y;
								pfVertex[Vector3::Z] = vOffset.z + p.z;
								pIndexBuffer->SetData(nIndex++, nVertex);
								nVertex++;

								e.x = static_cast<float>(Math::Cos(dTheta2)*Math::Cos(dTheta3));
								e.y = static_cast<float>(Math::Sin(dTheta2));
								e.z = static_cast<float>(Math::Cos(dTheta2)*Math::Sin(dTheta3));
								p.x = fRadius*e.x;
								p.y = fRadius*e.y;
								p.z = fRadius*e.z;
								if (TexCoords) {
									pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::TexCoord));
									pfVertex[Vector2::X] = static_cast<float>(i)/nDetail;
									pfVertex[Vector2::Y] = static_cast<float>(2*(j+1))/nDetail;
								}
								if (Normals) {
									pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Normal));
									pfVertex[Vector3::X] = e.x;
									pfVertex[Vector3::Y] = e.y;
									pfVertex[Vector3::Z] = e.z;
								}
								pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
								pfVertex[Vector3::X] = vOffset.x + p.x;
								pfVertex[Vector3::Y] = vOffset.y + p.y;
								pfVertex[Vector3::Z] = vOffset.z + p.z;
								pIndexBuffer->SetData(nIndex++, nVertex);
								nVertex++;
							}
						}
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
