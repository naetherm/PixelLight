/*********************************************************\
 *  File: MeshCreatorCappedCylinder.cpp                  *
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


// [TODO] Add texture coordinate support


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include "PLMesh/Geometry.h"
#include "PLMesh/MeshManager.h"
#include "PLMesh/MeshLODLevel.h"
#include "PLMesh/MeshMorphTarget.h"
#include "PLMesh/Creator/MeshCreatorCappedCylinder.h"


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
pl_implement_class(MeshCreatorCappedCylinder)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
MeshCreatorCappedCylinder::MeshCreatorCappedCylinder() :
	Length(this),
	Radius(this),
	Quality(this),
	Offset(this)
{
}

/**
*  @brief
*    Destructor
*/
MeshCreatorCappedCylinder::~MeshCreatorCappedCylinder()
{
}


//[-------------------------------------------------------]
//[ Private virtual MeshCreator functions                 ]
//[-------------------------------------------------------]
Mesh *MeshCreatorCappedCylinder::Create(Mesh &cMesh, uint32 nLODLevel, bool bStatic) const
{
	// Call base implementation
	MeshCreator::Create(cMesh, nLODLevel, bStatic);

	// Get morph target
	MeshMorphTarget *pMorphTarget = cMesh.GetMorphTarget(0);
	if (pMorphTarget && pMorphTarget->GetVertexBuffer()) {
		// Number of sides to the cylinder (divisible by 4):
		const uint32 n = Quality*4;

		// Allocate vertex buffer
		VertexBuffer *pVertexBuffer = pMorphTarget->GetVertexBuffer();
		pVertexBuffer->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float3);
		if (Normals)
			pVertexBuffer->AddVertexAttribute(VertexBuffer::Normal, 0, VertexBuffer::Float3);
		pVertexBuffer->Allocate(2*(n+1)+(n/4)*2*(n+1)*2, bStatic ? Usage::Static : Usage::Dynamic);

		// Get LOD level
		MeshLODLevel *pLODLevel = cMesh.GetLODLevel(nLODLevel);
		if (pLODLevel && pLODLevel->GetIndexBuffer()) {
			// Allocate index buffer
			IndexBuffer *pIndexBuffer = pLODLevel->GetIndexBuffer();
			pIndexBuffer->SetElementTypeByMaximumIndex(pVertexBuffer->GetNumOfElements()-1);
			pIndexBuffer->Allocate(pVertexBuffer->GetNumOfElements(), bStatic ? Usage::Static : Usage::Dynamic);

			// Setup vertices, indices & geometry
			if (pVertexBuffer->Lock(Lock::WriteOnly)) {
				if (pIndexBuffer->Lock(Lock::WriteOnly)) {
					Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
					float r  = Radius;
					float l  = Length*0.5f;
					float a  = static_cast<float>(Math::Pi2)/static_cast<float>(n);
					float sa = Math::Sin(a);
					float ca = Math::Cos(a);

					// Setup the index buffer
					for (uint32 i=0; i<pIndexBuffer->GetNumOfElements(); i++)
						pIndexBuffer->SetData(i, i);

					// Normal vector = (ny, 0, nz)
					float ny = 1.0f;
					float nz = 0.0f;

					// Get offset
					const Vector3 &vOffset = Offset.Get();

					// Draw cylinder body
					uint32 nStartIndex = 0;
					uint32 nIndex      = 0;
					for (uint32 i=0; i<=n; i++) {
						float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(nIndex, VertexBuffer::Position));
						pfVertex[Vector3::X] = vOffset.x + ny*r;
						pfVertex[Vector3::Y] = vOffset.y - l;
						pfVertex[Vector3::Z] = vOffset.z + nz*r;
						if (Normals) {
							pfVertex = static_cast<float*>(pVertexBuffer->GetData(nIndex, VertexBuffer::Normal));
							pfVertex[Vector3::X] = ny;
							pfVertex[Vector3::Y] = 0.0f;
							pfVertex[Vector3::Z] = nz;
						}
						nIndex++;
						pfVertex = static_cast<float*>(pVertexBuffer->GetData(nIndex, VertexBuffer::Position));
						pfVertex[Vector3::X] = vOffset.x + ny*r;
						pfVertex[Vector3::Y] = vOffset.y + l;
						pfVertex[Vector3::Z] = vOffset.z + nz*r;
						if (Normals) {
							pfVertex = static_cast<float*>(pVertexBuffer->GetData(nIndex, VertexBuffer::Normal));
							pfVertex[Vector3::X] = ny;
							pfVertex[Vector3::Y] = 0.0f;
							pfVertex[Vector3::Z] = nz;
						}
						nIndex++;

						// Rotate ny, nz
						float tmp = ca*ny - sa*nz;
						nz = sa*ny + ca*nz;
						ny = tmp;
					}

					{ // Create and setup the geometry
						Geometry &cGeometry = lstGeometries.Add();
						cGeometry.SetPrimitiveType(Primitive::TriangleStrip);
						cGeometry.SetStartIndex(nStartIndex);
						cGeometry.SetIndexSize(nIndex-nStartIndex);
						nStartIndex = nIndex;
					}

					// Draw first cylinder cap
					float start_nx = 0.0f;
					float start_ny = 1.0f;
					for (uint32 j=0; j<(n/4); j++) {
						// Get start_n2 = rotated start_n
						float start_nx2 =  ca*start_nx + sa*start_ny;
						float start_ny2 = -sa*start_nx + ca*start_ny;

						// Get n=start_n and n2=start_n2
						float nx = start_nx;
						ny = start_ny;
						nz = 0.0f;
						float nx2 = start_nx2;
						float ny2 = start_ny2;
						float nz2 = 0.0f;
						nIndex = nStartIndex;
						for (uint32 i=0; i<=n; i++) {
							float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(nIndex, VertexBuffer::Position));
							pfVertex[Vector3::X] = vOffset.x + ny*r;
							pfVertex[Vector3::Y] = vOffset.y + nx*r + l;
							pfVertex[Vector3::Z] = vOffset.z + nz*r;
							if (Normals) {
								pfVertex = static_cast<float*>(pVertexBuffer->GetData(nIndex, VertexBuffer::Normal));
								pfVertex[Vector3::X] = ny;
								pfVertex[Vector3::Y] = nx;
								pfVertex[Vector3::Z] = nz;
							}
							nIndex++;
							pfVertex = static_cast<float*>(pVertexBuffer->GetData(nIndex, VertexBuffer::Position));
							pfVertex[Vector3::X] = vOffset.x + ny2*r;
							pfVertex[Vector3::Y] = vOffset.y + nx2*r + l;
							pfVertex[Vector3::Z] = vOffset.z + nz2*r;
							if (Normals) {
								pfVertex = static_cast<float*>(pVertexBuffer->GetData(nIndex, VertexBuffer::Normal));
								pfVertex[Vector3::X] = ny2;
								pfVertex[Vector3::Y] = nx2;
								pfVertex[Vector3::Z] = nz2;
							}
							nIndex++;

							// Rotate n, n2
							float tmp = ca*ny - sa*nz;
							nz  = sa*ny + ca*nz;
							ny  = tmp;
							tmp = ca*ny2 - sa*nz2;
							nz2 = sa*ny2 + ca*nz2;
							ny2 = tmp;
						}
						start_nx = start_nx2;
						start_ny = start_ny2;

						// Create and setup the geometry
						Geometry &cGeometry = lstGeometries.Add();
						cGeometry.SetPrimitiveType(Primitive::TriangleStrip);
						cGeometry.SetStartIndex(nStartIndex);
						cGeometry.SetIndexSize(nIndex-nStartIndex);
						nStartIndex = nIndex;
					}

					// Draw second cylinder cap
					start_nx = 0;
					start_ny = 1;
					for (uint32 j=0; j<(n/4); j++) {
						// Get start_n2 = rotated start_n
						float start_nx2 = ca*start_nx - sa*start_ny;
						float start_ny2 = sa*start_nx + ca*start_ny;

						// Get n=start_n and n2=start_n2
						float nx = start_nx;
						ny = start_ny;
						nz = 0.0f;
						float nx2 = start_nx2;
						float ny2 = start_ny2;
						float nz2 = 0.0f;
						nIndex = nStartIndex;
						for (uint32 i=0; i<=n; i++) {
							float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(nIndex, VertexBuffer::Position));
							pfVertex[Vector3::X] = vOffset.x + ny2*r;
							pfVertex[Vector3::Y] = vOffset.y + nx2*r - l;
							pfVertex[Vector3::Z] = vOffset.z + nz2*r;
							if (Normals) {
								pfVertex = static_cast<float*>(pVertexBuffer->GetData(nIndex, VertexBuffer::Normal));
								pfVertex[Vector3::X] = ny2;
								pfVertex[Vector3::Y] = nx2;
								pfVertex[Vector3::Z] = nz2;
							}
							nIndex++;
							pfVertex = static_cast<float*>(pVertexBuffer->GetData(nIndex, VertexBuffer::Position));
							pfVertex[Vector3::X] = vOffset.x + ny*r;
							pfVertex[Vector3::Y] = vOffset.y + nx*r - l;
							pfVertex[Vector3::Z] = vOffset.z + nz*r;
							if (Normals) {
								pfVertex = static_cast<float*>(pVertexBuffer->GetData(nIndex, VertexBuffer::Normal));
								pfVertex[Vector3::X] = ny;
								pfVertex[Vector3::Y] = nx;
								pfVertex[Vector3::Z] = nz;
							}
							nIndex++;

							// Rotate n, n2
							float tmp = ca*ny - sa*nz;
							nz  = sa*ny + ca*nz;
							ny  = tmp;
							tmp = ca*ny2 - sa*nz2;
							nz2 = sa*ny2 + ca*nz2;
							ny2 = tmp;
						}
						start_nx = start_nx2;
						start_ny = start_ny2;

						// Create and setup the geometry
						Geometry &cGeometry = lstGeometries.Add();
						cGeometry.SetPrimitiveType(Primitive::TriangleStrip);
						cGeometry.SetStartIndex(nStartIndex);
						cGeometry.SetIndexSize(nIndex-nStartIndex);
						nStartIndex = nIndex;
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
