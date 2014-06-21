/*********************************************************\
 *  File: MeshCreatorDome.cpp                            *
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


// [TODO] Texture coordinate bug? Reverse order option?


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include "PLMesh/Geometry.h"
#include "PLMesh/MeshManager.h"
#include "PLMesh/MeshLODLevel.h"
#include "PLMesh/MeshMorphTarget.h"
#include "PLMesh/Creator/MeshCreatorDome.h"


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
pl_implement_class(MeshCreatorDome)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
MeshCreatorDome::MeshCreatorDome() :
	Radius(this),
	DTheta(this),
	DPhi(this),
	HTile(this),
	VTile(this),
	Offset(this)
{
}

/**
*  @brief
*    Destructor
*/
MeshCreatorDome::~MeshCreatorDome()
{
}


//[-------------------------------------------------------]
//[ Private virtual MeshCreator functions                 ]
//[-------------------------------------------------------]
Mesh *MeshCreatorDome::Create(Mesh &cMesh, uint32 nLODLevel, bool bStatic) const
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
		pVertexBuffer->Allocate(static_cast<uint32>((360/DTheta)*(90/DPhi)*4), bStatic ? Usage::Static : Usage::Dynamic);

		// Setup vertices & geometry
		if (pVertexBuffer->Lock(Lock::WriteOnly)) {
			// Get offset
			const Vector3 &vOffset = Offset.Get();

			// Generate the dome
			uint32 nVertex = 0;
			for (int nPhi=0; nPhi<=90-DPhi; nPhi+=static_cast<uint32>(DPhi)) {
				for (int nTheta=0; nTheta<=360-DTheta; nTheta+=static_cast<uint32>(DTheta)) {
					// Calculate the vertex at nPhi, nTheta
					float vx = static_cast<float>(Radius*Math::Sin(nPhi*Math::DegToRad)*Math::Cos(Math::DegToRad*nTheta));
					float vy = static_cast<float>(Radius*Math::Sin(nPhi*Math::DegToRad)*Math::Sin(Math::DegToRad*nTheta));
					float vz = static_cast<float>(Radius*Math::Cos(nPhi*Math::DegToRad));

					// Set vertex
					float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
					pfVertex[0] = vOffset.x + vx;
					pfVertex[1] = vOffset.y + vz;
					pfVertex[2] = vOffset.z + vy;

					// Create a vector from the origin to this vertex
					// Normalize the vector
					float mag = Math::Sqrt(vx*vx + vy*vy + vz*vz);
					vx /= mag;
					vy /= mag;
					vz /= mag;

					// Calculate the spherical texture coordinates
					if (TexCoords) {
						pfVertex    = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::TexCoord));
						pfVertex[0] = HTile * static_cast<float>(Math::ATan2(vx, vz)/(Math::Pi2)) + 0.5f;
						pfVertex[1] = VTile * static_cast<float>(Math::ASin(vy)/Math::Pi) + 0.5f;
					}
					nVertex++;

					// Calculate the vertex at nPhi+DPhi, nTheta
					vx = static_cast<float>(Radius*Math::Sin((nPhi+DPhi)*Math::DegToRad)*Math::Cos(nTheta*Math::DegToRad));
					vy = static_cast<float>(Radius*Math::Sin((nPhi+DPhi)*Math::DegToRad)*Math::Sin(nTheta*Math::DegToRad));
					vz = static_cast<float>(Radius*Math::Cos((nPhi+DPhi)*Math::DegToRad));
					
					// Set vertex
					pfVertex    = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
					pfVertex[0] = vOffset.x + vx;
					pfVertex[1] = vOffset.y + vz;
					pfVertex[2] = vOffset.z + vy;

					// Calculate the texture coordinates
					mag = Math::Sqrt(vx*vx + vy*vy + vz*vz);
					vx /= mag;
					vy /= mag;
					vz /= mag;

					// Calculate the spherical texture coordinates
					if (TexCoords) {
						pfVertex    = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::TexCoord));
						pfVertex[0] = HTile * static_cast<float>(Math::ATan2(vx, vz)/(Math::Pi2)) + 0.5f;
						pfVertex[1] = VTile * static_cast<float>(Math::ASin(vy)/Math::Pi) + 0.5f;
					}
					nVertex++;

					// Calculate the vertex at nPhi, nTheta+DTheta
					vx = static_cast<float>(Radius*Math::Sin(Math::DegToRad*nPhi)*Math::Cos(Math::DegToRad*(nTheta+DTheta)));
					vy = static_cast<float>(Radius*Math::Sin(Math::DegToRad*nPhi)*Math::Sin(Math::DegToRad*(nTheta+DTheta)));
					vz = static_cast<float>(Radius*Math::Cos(Math::DegToRad*nPhi));

					// Set vertex
					pfVertex    = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
					pfVertex[0] = vOffset.x + vx;
					pfVertex[1] = vOffset.y + vz;
					pfVertex[2] = vOffset.z + vy;

					// Calculate the texture coordinates
					mag = Math::Sqrt(vx*vx + vy*vy + vz*vz);
					vx /= mag;
					vy /= mag;
					vz /= mag;

					// Calculate the spherical texture coordinates
					if (TexCoords) {
						pfVertex    = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::TexCoord));
						pfVertex[0] = HTile * static_cast<float>(Math::ATan2(vx, vz)/(Math::Pi2)) + 0.5f;
						pfVertex[1] = VTile * static_cast<float>(Math::ASin(vy)/Math::Pi) + 0.5f;
					}
					nVertex++;

					if (nPhi > -90 && nPhi < 90) {
						// Calculate the vertex at nPhi+DPhi, nTheta+DTheta
						vx = static_cast<float>(Radius*Math::Sin((nPhi+DPhi)*Math::DegToRad)*Math::Cos(Math::DegToRad*(nTheta+DTheta)));
						vy = static_cast<float>(Radius*Math::Sin((nPhi+DPhi)*Math::DegToRad)*Math::Sin(Math::DegToRad*(nTheta+DTheta)));
						vz = static_cast<float>(Radius*Math::Cos((nPhi+DPhi)*Math::DegToRad));

						// Set vertex
						pfVertex    = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
						pfVertex[0] = vOffset.x + vx;
						pfVertex[1] = vOffset.y + vz;
						pfVertex[2] = vOffset.z + vy;

						// Calculate the texture coordinates
						mag = Math::Sqrt(vx*vx + vy*vy + vz*vz);
						vx /= mag;
						vy /= mag;
						vz /= mag;

						if (TexCoords) {
							pfVertex    = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::TexCoord));
							pfVertex[0] = HTile * static_cast<float>(Math::ATan2(vx, vz)/(Math::Pi2)) + 0.5f;
							pfVertex[1] = VTile * static_cast<float>(Math::ASin(vy)/Math::Pi) + 0.5f;
						}
						nVertex++;
					}
				}
			}

			// Unlock the vertex buffer
			pVertexBuffer->Unlock();
		}

		// Get LOD level
		MeshLODLevel *pLODLevel = cMesh.GetLODLevel(nLODLevel);
		if (pLODLevel && pLODLevel->GetIndexBuffer()) {
			// Create geometry
			uint32 nVertices = pVertexBuffer->GetNumOfElements();

			// Allocate index buffer
			IndexBuffer *pIndexBuffer = pLODLevel->GetIndexBuffer();
			pIndexBuffer->SetElementTypeByMaximumIndex(nVertices-1);
			pIndexBuffer->Allocate(nVertices, bStatic ? Usage::Static : Usage::Dynamic);

			// Create the geometry
			Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
			Geometry &cGeometry = lstGeometries.Add();

			// Setup the new geometry
			cGeometry.SetPrimitiveType(Primitive::TriangleStrip);
			cGeometry.SetStartIndex(0);
			cGeometry.SetIndexSize(nVertices);

			// Setup the indices used by this geometry
			if (pIndexBuffer->Lock(Lock::WriteOnly)) {
				// Fill the index buffer
				for (uint32 i=0; i<nVertices; i++)
					pIndexBuffer->SetData(i, i);

				// Unlock the index buffer
				pIndexBuffer->Unlock();
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
