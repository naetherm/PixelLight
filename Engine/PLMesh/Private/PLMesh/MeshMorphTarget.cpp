/*********************************************************\
 *  File: MeshMorphTarget.cpp                            *
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
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include "PLMesh/Mesh.h"
#include "PLMesh/Geometry.h"
#include "PLMesh/MeshLODLevel.h"
#include "PLMesh/MeshMorphTarget.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
namespace PLMesh {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
MeshMorphTarget::MeshMorphTarget(Mesh *pMesh) :
	m_sName("Morph target"),
	m_bRelative(false),
	m_pMesh(pMesh),
	m_pVertexBuffer(pMesh && pMesh->GetRenderer() ? pMesh->GetRenderer()->CreateVertexBuffer() : nullptr)
{
}

/**
*  @brief
*    Destructor
*/
MeshMorphTarget::~MeshMorphTarget()
{
	if (m_pVertexBuffer)
		delete m_pVertexBuffer;
}

/**
*  @brief
*    Returns the mesh the morph target belongs to
*/
Mesh *MeshMorphTarget::GetMesh() const
{
	return m_pMesh;
}

/**
*  @brief
*    Sets the mesh the morph target belongs to
*/
void MeshMorphTarget::SetMesh(Mesh *pMesh)
{
	m_pMesh = pMesh;
}

/**
*  @brief
*    Returns the name of this morph target
*/
String MeshMorphTarget::GetName() const
{
	return m_sName;
}

/**
*  @brief
*    Sets the name of this morph target
*/
void MeshMorphTarget::SetName(const String &sName)
{
	// Check parameter
	if (m_pMesh) {
		if (sName.GetLength()) {
			// Remove old name
			m_pMesh->m_mapMorphTargets.Remove(m_sName);

			// Get unused name
			uint32 i = 1;
			m_sName = sName;
			while (m_pMesh->m_mapMorphTargets.Get(m_sName)) {
				// This name is already used, find another one!
				m_sName = sName + '_' + static_cast<int>(i);
				i++;
			}

			// Use this name
			m_pMesh->m_mapMorphTargets.Add(m_sName, this);
		}
	} else {
		m_sName = sName;
	}
}

/**
*  @brief
*    Returns whether this morph target is relative to the basis morph target or not
*/
bool MeshMorphTarget::IsRelative() const
{
	return m_bRelative;
}

/**
*  @brief
*    Sets whether this morph target is relative to the basis morph target or not
*/
void MeshMorphTarget::SetRelative(bool bRelative)
{
	m_bRelative = bRelative;
}

/**
*  @brief
*    Returns the vertex ID's
*/
Array<uint32> &MeshMorphTarget::GetVertexIDs()
{
	return m_lstVertexIDs;
}

/**
*  @brief
*    Returns the vertex buffer
*/
VertexBuffer *MeshMorphTarget::GetVertexBuffer() const
{
	return m_pVertexBuffer;
}

/**
*  @brief
*    Copy operator
*/
MeshMorphTarget &MeshMorphTarget::operator =(const MeshMorphTarget &cSource)
{
	// Copy data
	m_sName		   = cSource.m_sName;
	m_bRelative    = cSource.m_bRelative;
	m_lstVertexIDs = cSource.m_lstVertexIDs;

	// Copy vertex buffer
	if (m_pVertexBuffer) {
		if (cSource.m_pVertexBuffer) {
			// Copy vertex buffer data
			*m_pVertexBuffer = *cSource.m_pVertexBuffer;
		} else {
			// Destroy vertex buffer
			delete m_pVertexBuffer;
			m_pVertexBuffer = nullptr;
		}
	} else {
		if (cSource.m_pVertexBuffer && m_pMesh) {
			// Create vertex buffer
			m_pVertexBuffer = m_pMesh->GetRenderer()->CreateVertexBuffer();

			// Copy vertex buffer data
			*m_pVertexBuffer = *cSource.m_pVertexBuffer;
		}
	}

	// Done
	return *this;
}


//[-------------------------------------------------------]
//[ Tool functions                                        ]
//[-------------------------------------------------------]
/**
*  @brief
*    Builds the current triangle planes
*/
void MeshMorphTarget::BuildTrianglePlaneList()
{
	// If there are vertex ID's, we CAN'T build triangle planes because only a few vertices are
	// influenced by this morph target!
	if (m_pMesh && m_pVertexBuffer && !m_lstVertexIDs.GetNumOfElements()) {
		// Get data and lock vertex buffer if required
		MeshLODLevel *pLODLevel = m_pMesh->GetLODLevel(0);
		if (pLODLevel) {
			const Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
			VertexBuffer *pVertexBuffer = m_pVertexBuffer;
			if (lstGeometries.GetNumOfElements() && pVertexBuffer->GetNumOfElements() && pVertexBuffer->Lock(Lock::ReadOnly)) {
				// Calculate triangle planes
				const Array<MeshTriangle> &lstTriangles = pLODLevel->GetTriangleList();
				if (lstTriangles.GetNumOfElements()) { // Use the triangle list for faster calculation
					Vector3 vV0, vV1, vV2;
					uint32 nTriangle = 0;

					// Loop through all triangles
					m_lstTrianglePlanes.Resize(pLODLevel->GetNumOfTriangles());
					for (uint32 i=0; i<lstTriangles.GetNumOfElements(); i++) {
						const MeshTriangle &cTriangle = lstTriangles[i];
						// V0
						const float *pfVertex = static_cast<const float*>(pVertexBuffer->GetData(cTriangle.nVertex[0], VertexBuffer::Position));
						vV0.x = pfVertex[0];
						vV0.y = pfVertex[1];
						vV0.z = pfVertex[2];
						// V1
						pfVertex = static_cast<const float*>(pVertexBuffer->GetData(cTriangle.nVertex[1], VertexBuffer::Position));
						vV1.x = pfVertex[0];
						vV1.y = pfVertex[1];
						vV1.z = pfVertex[2];
						// V2
						pfVertex = static_cast<const float*>(pVertexBuffer->GetData(cTriangle.nVertex[2], VertexBuffer::Position));
						vV2.x = pfVertex[0];
						vV2.y = pfVertex[1];
						vV2.z = pfVertex[2];

						// Calculate triangle plane
						m_lstTrianglePlanes[nTriangle++].ComputeND(vV0, vV1, vV2);
					}
				} else { // Use GetTriangle() which is slower
					// Lock index buffer
					IndexBuffer *pIndexBuffer = pLODLevel->GetIndexBuffer();
					if (pIndexBuffer && pIndexBuffer->Lock(Lock::ReadOnly)) {
						Vector3 vV0, vV1, vV2;
						uint32 nTriangle = 0;

						// Build triangle plane list
						m_lstTrianglePlanes.Resize(pLODLevel->GetNumOfTriangles());
						for (uint32 nGeo=0; nGeo<lstGeometries.GetNumOfElements(); nGeo++) {
							const Geometry &cGeometry = lstGeometries[nGeo];
							for (uint32 nTri=0; nTri<cGeometry.GetNumOfTriangles(); nTri++) {
								// Get triangle data
								uint32 nVertex0, nVertex1, nVertex2;
								pLODLevel->GetTriangle(nGeo, nTri, nVertex0, nVertex1, nVertex2);
								// V0
								const float *pfVertex = static_cast<const float*>(pVertexBuffer->GetData(nVertex0, VertexBuffer::Position));
								vV0.x = pfVertex[0];
								vV0.y = pfVertex[1];
								vV0.z = pfVertex[2];
								// V1
								pfVertex = static_cast<const float*>(pVertexBuffer->GetData(nVertex1, VertexBuffer::Position));
								vV1.x = pfVertex[0];
								vV1.y = pfVertex[1];
								vV1.z = pfVertex[2];
								// V2
								pfVertex = static_cast<const float*>(pVertexBuffer->GetData(nVertex2, VertexBuffer::Position));
								vV2.x = pfVertex[0];
								vV2.y = pfVertex[1];
								vV2.z = pfVertex[2];

								// Calculate triangle plane
								m_lstTrianglePlanes[nTriangle++].ComputeND(vV0, vV1, vV2);
							}
						}

						// Unlock the index buffer
						pIndexBuffer->Unlock();
					}
				}

				// Unlock the vertex buffer
				pVertexBuffer->Unlock();
			}
		}
	}
}

/**
*  @brief
*    Gets the triangle plane list
*/
Array<Plane> &MeshMorphTarget::GetTrianglePlaneList()
{
	return m_lstTrianglePlanes;
}

/**
*  @brief
*    Calculate the vertex normals of the morph target
*/
bool MeshMorphTarget::CalculateNormals()
{
	// If there are vertex ID's, we CAN'T calculate normals because only a few vertices are
	// influenced by this morph target!
	if (!m_pMesh || !m_pVertexBuffer || m_lstVertexIDs.GetNumOfElements())
		return false; // Error!

	// Calculate normals
	const MeshLODLevel *pLODLevel = m_pMesh->GetLODLevel(0);
	if (pLODLevel) {
		const Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
		VertexBuffer *pVertexBuffer = m_pVertexBuffer;
		pVertexBuffer->AddVertexAttribute(VertexBuffer::Normal, 0, VertexBuffer::Float3);
		if (lstGeometries.GetNumOfElements() && pVertexBuffer->GetNumOfElements() &&
			pVertexBuffer->Lock(Lock::ReadWrite)) {
			// Init normals
			for (uint32 i=0; i<pVertexBuffer->GetNumOfElements(); i++) {
				float *pNormal = static_cast<float*>(pVertexBuffer->GetData(i, VertexBuffer::Normal));
				pNormal[0] = 0.0f;
				pNormal[1] = 0.0f;
				pNormal[2] = 0.0f;
			}

			// Process every geometry object
			Vector3 v1, v2, vNormal;
			for (uint32 i=0; i<lstGeometries.GetNumOfElements(); i++) {
				// Get geometry object
				const Geometry &cGeometry = lstGeometries[i];

				// Process every triangle
				for (uint32 j=0; j<cGeometry.GetNumOfTriangles(); j++) {
					// Get triangle
					uint32 nVertex1, nVertex2, nVertex3;
					pLODLevel->GetTriangle(i, j, nVertex1, nVertex2, nVertex3);

					// Calculate face normal
					v1 = Vector3(static_cast<float*>(pVertexBuffer->GetData(nVertex2, VertexBuffer::Position))) - Vector3(static_cast<float*>(pVertexBuffer->GetData(nVertex1, VertexBuffer::Position)));
					v2 = Vector3(static_cast<float*>(pVertexBuffer->GetData(nVertex3, VertexBuffer::Position))) - Vector3(static_cast<float*>(pVertexBuffer->GetData(nVertex1, VertexBuffer::Position)));
					vNormal = v1.CrossProduct(v2);

					// Add to vertex normals
					float *pNormal = static_cast<float*>(pVertexBuffer->GetData(nVertex1, VertexBuffer::Normal));
					pNormal[0] += vNormal.x;
					pNormal[1] += vNormal.y;
					pNormal[2] += vNormal.z;
					pNormal     = static_cast<float*>(pVertexBuffer->GetData(nVertex2, VertexBuffer::Normal));
					pNormal[0] += vNormal.x;
					pNormal[1] += vNormal.y;
					pNormal[2] += vNormal.z;
					pNormal     = static_cast<float*>(pVertexBuffer->GetData(nVertex3, VertexBuffer::Normal));
					pNormal[0] += vNormal.x;
					pNormal[1] += vNormal.y;
					pNormal[2] += vNormal.z;
				}
			}

			// [TODO]
			// This is a quite complicated way to calculate the normal vectors.
			// Due to the fact that vertex information are already duplicated
			// in the .mesh file if the same vertex is used with different
			// texture coordinates, this second loop is needed to produce correct
			// normal vectors for each vertex.
			// As this is quite slow, we should try to optimize it!

			// Normalize normal vectors
			for (uint32 i=0; i<pVertexBuffer->GetNumOfElements(); i++) {
				// Add normals of identical vertices
		/*		for (uint32 j=0; j<pVertexBuffer->GetNumOfElements(); j++) {
					if (i != 0) {
						float *pVertex1 = (float*)pVertexBuffer->GetData(i, VertexBuffer::Normal);
						float *pVertex2 = (float*)pVertexBuffer->GetData(j, VertexBuffer::Normal);
						if (pVertex1[0] == pVertex2[0] &&
							pVertex1[1] == pVertex2[1] &&
							pVertex1[2] == pVertex2[2]) {
							pVertex1[0] += pVertex2[0];
							pVertex1[1] += pVertex2[1];
							pVertex1[2] += pVertex2[2];
							if (j > i) {
								pVertex2[0] = 0.0f;
								pVertex2[1] = 0.0f;
								pVertex2[2] = 0.0f;
							}
						}
					}
				
				}*/

				// Normalize
				float *pNormal = static_cast<float*>(pVertexBuffer->GetData(i, VertexBuffer::Normal));
				float  fX      = pNormal[0];
				float  fY      = pNormal[1];
				float  fZ      = pNormal[2];
				float  fLength = Math::Sqrt(fX*fX + fY*fY + fZ*fZ);
				if (!fLength)
					fLength = 1.0f;
				pNormal[0] /= fLength;
				pNormal[1] /= fLength;
				pNormal[2] /= fLength;
			}

			// Unlock the vertex buffer
			pVertexBuffer->Unlock();
		}
	}

	// Done
	return true;
}

/**
*  @brief
*    Calculates all tangent space vectors of the morph target
*/
bool MeshMorphTarget::CalculateTangentSpaceVectors(bool bTangent, bool bBinormal)
{
	// If there are vertex ID's, we CAN'T calculate normals because only a few vertices are
	// influenced by this morph target!
	if (!m_pMesh || !m_pVertexBuffer || m_lstVertexIDs.GetNumOfElements())
		return false; // Error!

	// Get LOD level
	const MeshLODLevel *pLODLevel = m_pMesh->GetLODLevel(0);
	if (pLODLevel) {
		// Get pointers
		const Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
		VertexBuffer *pVertexBuffer = m_pVertexBuffer;

		// Check if there are texture coordinates
		pVertexBuffer->Lock(Lock::ReadWrite);
		if (!pVertexBuffer->GetData(0, VertexBuffer::TexCoord)) {
			// Unlock the vertex buffer
			pVertexBuffer->Unlock();

			// Error!
			return false;
		}

		// Check if there are normals available
		if (!pVertexBuffer->GetData(0, VertexBuffer::Normal))
			CalculateNormals();

		// Unlock the vertex buffer
		pVertexBuffer->Unlock();

		// Calculate tangent space vectors, algorithm from Eric Lengyel: http://www.terathon.com/code/tangent.html
		if (!bTangent && !bBinormal)
			return true; // Done - no tangent space vectors...
		if (bTangent)
			pVertexBuffer->AddVertexAttribute(VertexBuffer::Tangent,  0, VertexBuffer::Float3);	// Tangent
		if (bBinormal)
			pVertexBuffer->AddVertexAttribute(VertexBuffer::Binormal, 0, VertexBuffer::Float3);	// Binormal
		if (lstGeometries.GetNumOfElements() && pVertexBuffer->GetNumOfElements() &&
			pVertexBuffer->Lock(Lock::ReadWrite)) {
			// Tangent temp memory
			uint32 nNumOfVertices = pVertexBuffer->GetNumOfElements();
			Vector3 *pTan1 = new Vector3[nNumOfVertices];
			Vector3 *pTan2 = new Vector3[nNumOfVertices];

			// Process every geometry object
			Vector3 vE0, vE1, vNormal, vTangent, vBinormal, vN;
			for (uint32 i=0; i<lstGeometries.GetNumOfElements(); i++) {
				// Get geometry object
				const Geometry &cGeometry = lstGeometries[i];

				// Process every triangle
				for (uint32 j=0; j<cGeometry.GetNumOfTriangles(); j++) {
					// Get triangle
					uint32 nVertex1, nVertex2, nVertex3;
					pLODLevel->GetTriangle(i, j, nVertex1, nVertex2, nVertex3);

					// Get vertex data
					// 1
					const float *pfPos1		 = static_cast<const float*>(pVertexBuffer->GetData(nVertex1, VertexBuffer::Position));
					const float *pfTexCoord1 = static_cast<const float*>(pVertexBuffer->GetData(nVertex1, VertexBuffer::TexCoord));
					// 2
					const float *pfPos2		 = static_cast<const float*>(pVertexBuffer->GetData(nVertex2, VertexBuffer::Position));
					const float *pfTexCoord2 = static_cast<const float*>(pVertexBuffer->GetData(nVertex2, VertexBuffer::TexCoord));
					// 3
					const float *pfPos3		 = static_cast<const float*>(pVertexBuffer->GetData(nVertex3, VertexBuffer::Position));
					const float *pfTexCoord3 = static_cast<const float*>(pVertexBuffer->GetData(nVertex3, VertexBuffer::TexCoord));

					// Position vectors
					float x1 = pfPos2[Vector3::X] - pfPos1[Vector3::X];
					float x2 = pfPos3[Vector3::X] - pfPos1[Vector3::X];
					float y1 = pfPos2[Vector3::Y] - pfPos1[Vector3::Y];
					float y2 = pfPos3[Vector3::Y] - pfPos1[Vector3::Y];
					float z1 = pfPos2[Vector3::Z] - pfPos1[Vector3::Z];
					float z2 = pfPos3[Vector3::Z] - pfPos1[Vector3::Z];

					// Texture coordinate vectors
					float s1 = pfTexCoord2[Vector2::X] - pfTexCoord1[Vector2::X];
					float s2 = pfTexCoord3[Vector2::X] - pfTexCoord1[Vector2::X];
					float t1 = pfTexCoord2[Vector2::Y] - pfTexCoord1[Vector2::Y];
					float t2 = pfTexCoord3[Vector2::Y] - pfTexCoord1[Vector2::Y];

					// Compute tangents
					float r = 1.0f / (s1 * t2 - s2 * t1);
					Vector3 sdir((t2*x1 - t1*x2)*r, (t2*y1 - t1*y2)*r, (t2*z1 - t1*z2)*r);
					Vector3 tdir((s1*x2 - s2*x1)*r, (s1*y2 - s2*y1)*r, (s1*z2 - s2*z1)*r);

					// Tangent 1
					pTan1[nVertex1] += sdir;
					pTan1[nVertex2] += sdir;
					pTan1[nVertex3] += sdir;

					// Tangent 2
					pTan2[nVertex1] += tdir;
					pTan2[nVertex2] += tdir;
					pTan2[nVertex3] += tdir;
				}
			}

			// Calculate the final tangents
			for (uint32 i=0; i<nNumOfVertices; i++) {
				const float *pfNormal = static_cast<const float*>(pVertexBuffer->GetData(i, VertexBuffer::Normal));
				vN.x = pfNormal[Vector3::X];
				vN.y = pfNormal[Vector3::Y];
				vN.z = pfNormal[Vector3::Z];
				Vector3 &vT = pTan1[i];

				// Gram-Schmidt orthogonalize
				vTangent = (vT - vN*vN.DotProduct(vT)).Normalize();

				// Save tangent
				if (bTangent) {
					float *pfTangent = static_cast<float*>(pVertexBuffer->GetData(i, VertexBuffer::Tangent));
					pfTangent[Vector3::X] = vTangent.x;
					pfTangent[Vector3::Y] = vTangent.y;
					pfTangent[Vector3::Z] = vTangent.z;
				}

				// Calculate and save binormal
				if (bBinormal) {
					float *pfBinormal = static_cast<float*>(pVertexBuffer->GetData(i, VertexBuffer::Binormal));

					// Calculate handedness
					vBinormal = vN.CrossProduct(vTangent);
					if (vBinormal.DotProduct(pTan2[i]) < 0.0f)
						vBinormal.Invert();
					vBinormal.Normalize();
					pfBinormal[Vector3::X] = vBinormal.x;
					pfBinormal[Vector3::Y] = vBinormal.y;
					pfBinormal[Vector3::Z] = vBinormal.z;
				}
			}

			// Free temp tangent memory
			delete [] pTan1;
			delete [] pTan2;

			// Unlock the vertex buffer
			pVertexBuffer->Unlock();
		}
	}

	// Done
	return true;
}

/**
*  @brief
*    Calculates the morph target bounding box
*/
bool MeshMorphTarget::CalculateBoundingBox(Vector3 &vMinPos, Vector3 &vMaxPos) const
{
	// Get vertex buffer bounding box
	if (m_pVertexBuffer) {
		m_pVertexBuffer->CalculateBoundingBox(vMinPos, vMaxPos);

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}

/**
*  @brief
*    Calculates the morph target bounding sphere
*/
bool MeshMorphTarget::CalculateBoundingSphere(Vector3 &vPos, float &fRadius) const
{
	// Get vertex buffer bounding sphere
	if (m_pVertexBuffer) {
		m_pVertexBuffer->CalculateBoundingSphere(vPos, fRadius);

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh
