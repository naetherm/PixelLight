/*********************************************************\
 *  File: MeshCreatorBezierPatch.cpp                     *
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


// - [TODO] Add normal support


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
#include "PLMesh/Creator/MeshCreatorBezierPatch.h"


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
pl_implement_class(MeshCreatorBezierPatch)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
MeshCreatorBezierPatch::MeshCreatorBezierPatch() :
	Offset(this),
	Divisions(this)
{
	// Control points row 0
	m_vV[0][0].SetXYZ(0.0f,  0.0f, 0.0f);
	m_vV[1][0].SetXYZ(0.25f, 1.0f, 0.0f);
	m_vV[2][0].SetXYZ(0.5f,  0.0f, 0.0f);
	m_vV[3][0].SetXYZ(1.0f,  0.5f, 0.0f);
	// Control points row 1
	m_vV[0][1].SetXYZ(0.0f,  0.3f, 0.25f);
	m_vV[1][1].SetXYZ(0.25f, 0.0f, 0.25f);
	m_vV[2][1].SetXYZ(0.5f,  0.1f, 0.25f);
	m_vV[3][1].SetXYZ(1.0f,  0.0f, 0.25f);
	// Control points row 2
	m_vV[0][2].SetXYZ(0.0f,  0.1f, 0.5f);
	m_vV[1][2].SetXYZ(0.25f, 0.2f, 0.5f);
	m_vV[2][2].SetXYZ(0.5f,  0.3f, 0.5f);
	m_vV[3][2].SetXYZ(1.0f,  0.4f, 0.5f);
	// Control points row 3
	m_vV[0][3].SetXYZ(0.0f,  0.0f, 1.0f);
	m_vV[1][3].SetXYZ(0.25f, 0.0f, 1.0f);
	m_vV[2][3].SetXYZ(0.5f,  1.0f, 1.0f);
	m_vV[3][3].SetXYZ(1.0f,  0.7f, 1.0f);
}

/**
*  @brief
*    Destructor
*/
MeshCreatorBezierPatch::~MeshCreatorBezierPatch()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Calculates 3rd degree polynomial based on array of 4 points
*    and a single variable (u) which is generally between 0 and 1
*/
Vector3 MeshCreatorBezierPatch::Bernstein(float fU, const Vector3 *pvV) const
{
	Vector3 a = pvV[0]*(Math::Pow(fU, 3.0f));
	Vector3 b = pvV[1]*(3*Math::Pow(fU, 2.0f)*(1-fU));
	Vector3 c = pvV[2]*(3*fU*Math::Pow((1-fU), 2.0f));
	Vector3 d = pvV[3]*(Math::Pow((1-fU), 3.0f));

	return a+b+c+d;
}


//[-------------------------------------------------------]
//[ Private virtual MeshCreator functions                 ]
//[-------------------------------------------------------]
Mesh *MeshCreatorBezierPatch::Create(Mesh &cMesh, uint32 nLODLevel, bool bStatic) const
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
		pVertexBuffer->Allocate((Divisions+1)*(Divisions+1)*2, bStatic ? Usage::Static : Usage::Dynamic);

		// Get LOD level
		MeshLODLevel *pLODLevel = cMesh.GetLODLevel(nLODLevel);
		if (pLODLevel && pLODLevel->GetIndexBuffer()) {
			// Allocate index buffer
			IndexBuffer *pIndexBuffer = pLODLevel->GetIndexBuffer();
			pIndexBuffer->SetElementTypeByMaximumIndex(pVertexBuffer->GetNumOfElements()-1);
			pIndexBuffer->Allocate(pVertexBuffer->GetNumOfElements(), bStatic ? Usage::Static : Usage::Dynamic);

			// Array of points to mark the first line of polygons
			Vector3 *pvLast = new Vector3[Divisions+1];

			// The first derived curve (along x axis)
			Vector3 vTemp[4] = {m_vV[0][3], m_vV[1][3], m_vV[2][3], m_vV[3][3]};

			// Create the first line of points
			for (uint32 v=0; v<=Divisions; v++) {
				// Percent along y axis
				float fPY = static_cast<float>(v)/static_cast<float>(Divisions);

				// Use the 4 points from the derives curve to calculate the points along that curve
				pvLast[v] = Bernstein(fPY, vTemp);
			}

			// Create and setup the geometries
			Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
			if (pVertexBuffer->Lock(Lock::WriteOnly)) {
				if (pIndexBuffer->Lock(Lock::WriteOnly)) {
					// Get offset
					const Vector3 &vOffset = Offset.Get();

					uint32 nIndex  = 0;
					uint32 nVertex = 0;
					for (uint32 u=1; u<=Divisions; u++) {
						// Percent along y axis
						float fPY = static_cast<float>(u)/static_cast<float>(Divisions);

						// Percent along old y axis
						float fPYOld = static_cast<float>(u-1.0f)/static_cast<float>(Divisions);

						// Calculate new bezier points
						vTemp[0] = Bernstein(fPY, m_vV[0]);
						vTemp[1] = Bernstein(fPY, m_vV[1]);
						vTemp[2] = Bernstein(fPY, m_vV[2]);
						vTemp[3] = Bernstein(fPY, m_vV[3]);

						// Create and setup new geometry
						Geometry &cGeometry = lstGeometries.Add();
						cGeometry.SetPrimitiveType(Primitive::TriangleStrip);
						cGeometry.SetStartIndex(nVertex);
						cGeometry.SetIndexSize((Divisions+1)*2);
						for (uint32 v=0; v<=Divisions; v++) {
							// Percent along the x axis
							float fPX = static_cast<float>(v)/static_cast<float>(Divisions);

							// V0
							float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
							pfVertex[Vector3::X] = vOffset.x + pvLast[v].x;
							pfVertex[Vector3::Y] = vOffset.y + pvLast[v].y;
							pfVertex[Vector3::Z] = vOffset.z + pvLast[v].z;
							if (TexCoords) {
								pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::TexCoord));
								pfVertex[Vector2::X] = fPX;
								pfVertex[Vector2::Y] = fPYOld;
							}
							pIndexBuffer->SetData(nIndex++, nVertex);
							nVertex++;

							// V1
							pvLast[v] = Bernstein(fPX, vTemp);	// Generate new point
							pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
							pfVertex[Vector3::X] = vOffset.x + pvLast[v].x;
							pfVertex[Vector3::Y] = vOffset.y + pvLast[v].y;
							pfVertex[Vector3::Z] = vOffset.z + pvLast[v].z;
							if (TexCoords) {
								pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::TexCoord));
								pfVertex[Vector2::X] = fPX;
								pfVertex[Vector2::Y] = fPY;
							}
							pIndexBuffer->SetData(nIndex++, nVertex);
							nVertex++;
						}
					}

					// Unlock the index buffer
					pIndexBuffer->Unlock();
				}

				// Unlock the vertex buffer
				pVertexBuffer->Unlock();

				// Free the old vertices array
				delete [] pvLast;
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
