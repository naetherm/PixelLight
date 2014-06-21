/*********************************************************\
 *  File: MeshCreatorFlashlight.cpp                      *
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
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include "PLMesh/Geometry.h"
#include "PLMesh/MeshManager.h"
#include "PLMesh/MeshLODLevel.h"
#include "PLMesh/MeshMorphTarget.h"
#include "PLMesh/Creator/MeshCreatorFlashlight.h"


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
pl_implement_class(MeshCreatorFlashlight)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
MeshCreatorFlashlight::MeshCreatorFlashlight() :
	Detail(this),
	Offset(this)
{
}

/**
*  @brief
*    Destructor
*/
MeshCreatorFlashlight::~MeshCreatorFlashlight()
{
}


//[-------------------------------------------------------]
//[ Private virtual MeshCreator functions                 ]
//[-------------------------------------------------------]
Mesh *MeshCreatorFlashlight::Create(Mesh &cMesh, uint32 nLODLevel, bool bStatic) const
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
			pVertexBuffer->AddVertexAttribute(VertexBuffer::TexCoord, 0, VertexBuffer::Float4);
		pVertexBuffer->Allocate(Detail+(Detail+2)*4, bStatic ? Usage::Static : Usage::Dynamic);

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
					Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
					uint32 nIndex = 0;

					// Get offset
					const Vector3 &vOffset = Offset.Get();

					{ // Part 1
						Geometry &cGeometry = lstGeometries.Add();
						cGeometry.SetPrimitiveType(Primitive::TriangleFan);
						cGeometry.SetStartIndex(0);
						for (uint32 i=0; i<Detail; i++) {
							float ca = static_cast<float>(Math::Cos(i*Math::Pi2/Detail));
							float sa = static_cast<float>(Math::Sin(i*Math::Pi2/Detail));
							float *pfVertex;
							if (TexCoords) {
								pfVertex = static_cast<float*>(pVertexBuffer->GetData(nIndex, VertexBuffer::TexCoord));
								pfVertex[Vector4::X] = 0.5f*(ca + 1);
								pfVertex[Vector4::Y] = 0.5f*(sa + 1);
								pfVertex[Vector4::Z] = 1.0f;
								pfVertex[Vector4::W] = 1.0f;
							}
							pfVertex = static_cast<float*>(pVertexBuffer->GetData(nIndex, VertexBuffer::Position));
							pfVertex[Vector3::X] = vOffset.x + 0.21f*ca;
							pfVertex[Vector3::Y] = vOffset.y - 0.21f*sa;
							pfVertex[Vector3::Z] = vOffset.z - 0.75f;
							pIndexBuffer->SetData(nIndex, nIndex);
							nIndex++;
						}
						cGeometry.SetIndexSize(nIndex);
					}

					uint32 nFirstIndex = nIndex;
					{ // Part 2
						Geometry &cGeometry = lstGeometries.Add();
						cGeometry.SetPrimitiveType(Primitive::TriangleStrip);
						cGeometry.SetStartIndex(nIndex);
						for (uint32 i=0; i<=Detail; i++) {
							float ca = static_cast<float>(Math::Cos(i*Math::Pi2/Detail));
							float sa = static_cast<float>(Math::Sin(i*Math::Pi2/Detail));

							// 0
							float *pfVertex;
							if (TexCoords) {
								pfVertex = static_cast<float*>(pVertexBuffer->GetData(nIndex, VertexBuffer::TexCoord));
								pfVertex[Vector4::X] = 0.0f;
								pfVertex[Vector4::Y] = static_cast<float>(2*i)/Detail;
								pfVertex[Vector4::Z] = 1.0f;
								pfVertex[Vector4::W] = 1.0f;
							}
							pfVertex = static_cast<float*>(pVertexBuffer->GetData(nIndex, VertexBuffer::Position));
							pfVertex[Vector3::X] = vOffset.x + 0.21f*ca;
							pfVertex[Vector3::Y] = vOffset.y + 0.21f*sa;
							pfVertex[Vector3::Z] = vOffset.z;
							pIndexBuffer->SetData(nIndex, nIndex);
							nIndex++;

							// 1
							if (TexCoords) {
								pfVertex = static_cast<float*>(pVertexBuffer->GetData(nIndex, VertexBuffer::TexCoord));
								pfVertex[Vector4::X] = 3.0f;
								pfVertex[Vector4::Y] = static_cast<float>(2*i)/Detail;
								pfVertex[Vector4::Z] = 1.0f;
								pfVertex[Vector4::W] = 1.0f;
							}
							pfVertex = static_cast<float*>(pVertexBuffer->GetData(nIndex, VertexBuffer::Position));
							pfVertex[Vector3::X] = vOffset.x + 0.21f*ca;
							pfVertex[Vector3::Y] = vOffset.y + 0.21f*sa;
							pfVertex[Vector3::Z] = vOffset.z - 0.75f;
							pIndexBuffer->SetData(nIndex, nIndex);
							nIndex++;
						}
						cGeometry.SetIndexSize(nIndex-nFirstIndex);
					}

					{ // Part 3
						// Use projective texturing to get the texture right on these trapeze shaped quads
						Geometry &cGeometry = lstGeometries.Add();
						cGeometry.SetPrimitiveType(Primitive::TriangleStrip);
						cGeometry.SetStartIndex(nIndex);
						nFirstIndex = nIndex;
						for (uint32 i=0; i<=Detail; i++) {
							float ca = static_cast<float>(Math::Cos(i*Math::Pi2/Detail));
							float sa = static_cast<float>(Math::Sin(i*Math::Pi2/Detail));

							// 0
							float *pfVertex;
							if (TexCoords) {
								pfVertex = static_cast<float*>(pVertexBuffer->GetData(nIndex, VertexBuffer::TexCoord));
								pfVertex[Vector4::X] = 0.0f;
								pfVertex[Vector4::Y] = 35*static_cast<float>(2*i)/Detail;
								pfVertex[Vector4::Z] = 0.0f;
								pfVertex[Vector4::W] = 35.0f;
							}
							pfVertex = static_cast<float*>(pVertexBuffer->GetData(nIndex, VertexBuffer::Position));
							pfVertex[Vector3::X] = vOffset.x + 0.35f*ca;
							pfVertex[Vector3::Y] = vOffset.y + 0.35f*sa;
							pfVertex[Vector3::Z] = vOffset.z + 0.25f;
							pIndexBuffer->SetData(nIndex, nIndex);
							nIndex++;

							// 1
							if (TexCoords) {
								pfVertex = static_cast<float*>(pVertexBuffer->GetData(nIndex, VertexBuffer::TexCoord));
								pfVertex[Vector4::X] = 21.0f;
								pfVertex[Vector4::Y] = 21*static_cast<float>(2*i)/Detail;
								pfVertex[Vector4::Z] = 0.0f;
								pfVertex[Vector4::W] = 21.0f;
							}
							pfVertex = static_cast<float*>(pVertexBuffer->GetData(nIndex, VertexBuffer::Position));
							pfVertex[Vector3::X] = vOffset.x + 0.21f*ca;
							pfVertex[Vector3::Y] = vOffset.y + 0.21f*sa;
							pfVertex[Vector3::Z] = vOffset.z;
							pIndexBuffer->SetData(nIndex, nIndex);
							nIndex++;
						}
						cGeometry.SetIndexSize(nIndex-nFirstIndex);
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
