/*********************************************************\
 *  File: PickingResult.cpp                              *
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
#include <PLMath/Vector2i.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLMesh/MeshHandler.h>
#include <PLMesh/MeshLODLevel.h>
#include <PLScene/Scene/SceneNode.h>
#include "PLEngine/Picking/PickingResult.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
using namespace PLMesh;
using namespace PLScene;
namespace PLEngine {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
PickingResult::PickingResult() :
	m_pSceneNode(nullptr),
	m_nGeometry(0),
	m_nTriangle(0),
	m_pSceneContainer(nullptr),
	m_fNearestSquaredDistance(-1.0f)
{
}

/**
*  @brief
*    Destructor
*/
PickingResult::~PickingResult()
{
}

/**
*  @brief
*    Returns the picked scene node
*/
SceneNode *PickingResult::GetSceneNode() const
{
	return m_pSceneNode;
}

/**
*  @brief
*    Returns the picked geometry
*/
uint32 PickingResult::GetGeometry() const
{
	return m_nGeometry;
}

/**
*  @brief
*    Returns the picked triangle
*/
uint32 PickingResult::GetTriangle() const
{
	return m_nTriangle;
}

/**
*  @brief
*    Returns the picked point in node space
*/
const Vector3 &PickingResult::GetPoint() const
{
	return m_vPoint;
}

/**
*  @brief
*    Returns the scene container the picking line and distance is in
*/
SceneContainer *PickingResult::GetSceneContainer() const
{
	return m_pSceneContainer;
}

/**
*  @brief
*    Returns the picking line start position
*/
const Vector3 &PickingResult::GetLineStartPos() const
{
	return m_vLineStartPos;
}

/**
*  @brief
*    Returns the nearest squared distance to the line start position
*/
float PickingResult::GetNearestSquaredDistance() const
{
	return m_fNearestSquaredDistance;
}

/**
*  @brief
*    Returns the currently picked texture coordinate (can also be outside the 0..1 interval)
*/
bool PickingResult::GetTextureCoordinate(Vector2 &vTexCoord, uint32 nTexCoordChannel) const
{
	// Is anything picked?
	if (m_pSceneNode) {
		// Get the mesh handler
		MeshHandler *pMeshHandler = m_pSceneNode->GetMeshHandler();
		if (pMeshHandler) {
			// Get the mesh
			Mesh *pMesh = pMeshHandler->GetResource();
			if (pMesh) {
				// Get the first LOD level of the mesh
				MeshLODLevel *pLODLevel = pMesh->GetLODLevel(0);
				if (pLODLevel) {
					// Get the vertex indices of the picked triangle
					uint32 nVertex0, nVertex1, nVertex2;
					if (pLODLevel->GetTriangle(m_nGeometry, m_nTriangle, nVertex0, nVertex1, nVertex2)) {
						// Get and lock the vertex buffer
						VertexBuffer *pVertexBuffer = pMeshHandler->GetVertexBuffer();
						if (pVertexBuffer && pVertexBuffer->GetNumOfElements() && pVertexBuffer->Lock(Lock::ReadOnly)) {
							// Get triangle vertex position
							const Vector3 vA = static_cast<const float*>(pVertexBuffer->GetData(nVertex0, VertexBuffer::Position));
							const Vector3 vB = static_cast<const float*>(pVertexBuffer->GetData(nVertex1, VertexBuffer::Position));
							const Vector3 vC = static_cast<const float*>(pVertexBuffer->GetData(nVertex2, VertexBuffer::Position));

							// Get triangle texture coordinates
							const float *pfTexCoordA = static_cast<const float*>(pVertexBuffer->GetData(nVertex0, VertexBuffer::TexCoord, nTexCoordChannel));
							const float *pfTexCoordB = static_cast<const float*>(pVertexBuffer->GetData(nVertex1, VertexBuffer::TexCoord, nTexCoordChannel));
							const float *pfTexCoordC = static_cast<const float*>(pVertexBuffer->GetData(nVertex2, VertexBuffer::TexCoord, nTexCoordChannel));

							// Unlock the vertex buffer
							pVertexBuffer->Unlock();

							// Calculate picked texture coordinate
							if (pfTexCoordA && pfTexCoordB && pfTexCoordC) {
								const Vector2 vTexCoordA = pfTexCoordA;
								const Vector2 vTexCoordB = pfTexCoordB;
								const Vector2 vTexCoordC = pfTexCoordC;

								// We want to have the texture coordinate for 'm_vPoint'...
								// ... we have all triangle data we need to calculate our ...
								// ... 3 vertex positions, 3 texture coordinates and the point on the triangle we want to calculate
								// the texture coordinate for. We solve the problem by using "Barycentric coordinates".
								// http://www.gamedev.net/community/forums/topic.asp?topic_id=451357

								// Compute the normal of the triangle
								Vector3 vN;
								vN.CrossProduct(vB-vA, vC-vA);
								vN.Normalize();

								// Compute twice area of triangle ABC
								const float fAreaABC = vN.DotProduct(Vector3().CrossProduct(vB-vA, vC-vA));

								// Compute a
								const float fAreaPBC = vN.DotProduct(Vector3().CrossProduct(vB-m_vPoint, vC-m_vPoint));
								const float fA = fAreaPBC / fAreaABC;

								// Compute b
								const float fAreaPCA = vN.DotProduct(Vector3().CrossProduct(vC-m_vPoint, vA-m_vPoint));
								const float fB = fAreaPCA / fAreaABC;

								// Compute c
								const float fC = 1.0f - fA - fB;

								// Finally, calculate the texture coordinate
								vTexCoord = vTexCoordA*fA + vTexCoordB*fB + vTexCoordC*fC;

								// Done
								return true;
							}
						}
					}
				}
			}
		}
	}

	// Set to null on error
	vTexCoord = Vector2::Zero;

	// Error!
	return false;
}

/**
*  @brief
*    Returns the currently picked wrapped texture coordinate (always between 0..1)
*/
bool PickingResult::GetWrappedTextureCoordinate(Vector2 &vTexCoord, uint32 nTexCoordChannel) const
{
	// Get the currently picked texture coordinate
	if (GetTextureCoordinate(vTexCoord, nTexCoordChannel)) {
		// Perform wrapping to get into the 0..1 interval
		vTexCoord.x = vTexCoord.x - static_cast<int>(vTexCoord.x);
		if (vTexCoord.x < 0.0f)
			vTexCoord.x = vTexCoord.x + 1.0f;
		vTexCoord.y = vTexCoord.y - static_cast<int>(vTexCoord.y);
		if (vTexCoord.y < 0.0f)
			vTexCoord.y = vTexCoord.y + 1.0f;

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
} // PLEngine
