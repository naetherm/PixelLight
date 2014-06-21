/*********************************************************\
 *  File: BodyTerrain.cpp                                *
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
#include <PLMath/Matrix4x4.h>
#include "PLPhysicsNewton/BodyImpl.h"
#include "PLPhysicsNewton/World.h"
#include "PLPhysicsNewton/BodyTerrain.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace Newton;
using namespace PLCore;
using namespace PLMath;
namespace PLPhysicsNewton {


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
#define HEIGHFIELD(zindex, xindex) m_pfTerrain[(zindex)*m_nWidth+(xindex)]


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
BodyTerrain::~BodyTerrain()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
BodyTerrain::BodyTerrain(PLPhysics::World &cWorld, uint32 nWidth, uint32 nHeight,
						 const float fTerrain[], const Vector3 &vBoxMin, const Vector3 &vBoxMax, const Vector3 &vScale) :
	PLPhysics::BodyTerrain(cWorld, static_cast<World&>(cWorld).CreateBodyImpl()),
	m_nWidth(nWidth),
	m_nHeight(nHeight),
	m_pfTerrain(fTerrain),
	m_vBoxMin(vBoxMin),
	m_vBoxMax(vBoxMax),
	m_vScale(vScale)
{
	// Deactivate the physics simulation if required
	const bool bSimulationActive = cWorld.IsSimulationActive();
	if (bSimulationActive)
		cWorld.SetSimulationActive(false);

	// Get the Newton physics world
	NewtonWorld *pNewtonWorld = static_cast<World&>(cWorld).GetNewtonWorld();

	// Create collision primitive
	NewtonCollision *pCollision = NewtonCreateUserMeshCollision(pNewtonWorld, m_vBoxMin, m_vBoxMax, this, MeshCollisionCollideCallback, UserMeshCollisionRayHitCallback, nullptr, nullptr, nullptr, 0);

	// Create the rigid body
	// [TODO] Remove this as soon as there's an up-to-date Linux version of Newton Game Dynamics available!
	#if (NEWTON_MAJOR_VERSION == 2) && (NEWTON_MINOR_VERSION >= 28)
		Newton::NewtonBody *pNewtonBody = NewtonCreateBody(pNewtonWorld, pCollision, Matrix4x4::Identity);
	#else
		Newton::NewtonBody *pNewtonBody = NewtonCreateBody(pNewtonWorld, pCollision);
	#endif
	NewtonReleaseCollision(pNewtonWorld, pCollision);

	// Initialize the Newton physics body
	static_cast<BodyImpl&>(GetBodyImpl()).InitializeNewtonBody(*this, *pNewtonBody, 0.0f);

	// Reactivate the physics simulation if required
	if (bSimulationActive)
		cWorld.SetSimulationActive(bSimulationActive);
}

/**
*  @brief
*    Calculate the intersection of a ray and a triangle
*/
dFloat BodyTerrain::RayCastTriangle(const Vector3 &p0, const Vector3 &dp, const Vector3 &origin, const Vector3 &e1, const Vector3 &e2)
{
	dFloat t;
	dFloat b0;
	dFloat b1;
	dFloat b00;
	dFloat b11;
	dFloat a00;
	dFloat a10;
	dFloat a11;
	dFloat det;
	dFloat dot;
	dFloat tol;

	// Clip line again first triangle
	Vector3 normal(e2.CrossProduct(e1));

	dot = normal.DotProduct(dp);
	if (dot <= 1.0e-6f) {
		t = ((origin - p0).DotProduct(normal)) / dot;
		if (t > 0.0f) {
			if (t < 1.0f) {
				Vector3 q = p0 + dp*t;
				a00 = e1.DotProduct(e1);
				a11 = e2.DotProduct(e2);
				a10 = e1.DotProduct(e2);
				det = a00*a11 - a10*a10;
				// det must be positive and different than zero
//				_ASSERTE(det > 0.0f);
				
				Vector3 q0p0 = q - origin;
				b0 = q0p0.DotProduct(e1);
				b1 = q0p0.DotProduct(e2);

				tol = -det*1.0e-3f;
				b00 = b0*a11 - b1*a10;
				if (b00 >= tol) {
					b11 = b1*a00 - b0*a10;
					if (b11 >= tol) {
						if ((b00 + b11) <= (det*1.001f)) {
							// Found a hit return this value
							return t;
						}
					}
				}
			}
		}
	}

	// If it come here the there no intersection
	return 1.2f;
}

/**
*  @brief
*    Calculate the intersection point of a line segment and the two triangles making a the cell of a height map terrain
*/
dFloat BodyTerrain::RayCastCell(int xIndex0, int zIndex0, const Vector3 &p0, const Vector3 &dp, Vector3 &normalOut)
{
	dFloat t;

	// Clamp x
	if (xIndex0 < 0)
		xIndex0 = 0;
	if (xIndex0 >= static_cast<int>(m_nWidth)-1)
		xIndex0 = m_nWidth-2;
	// Clamp z
	if (zIndex0 < 0)
		zIndex0 = 0;
	if (zIndex0 >= static_cast<int>(m_nHeight)-1)
		zIndex0 = m_nHeight-2;

	// Get the 3d point at the corner of the cell
	Vector3 p00((xIndex0 + 0)*m_vScale.x, HEIGHFIELD(zIndex0+0, xIndex0+0), (zIndex0 + 0)*m_vScale.z);
	Vector3 p10((xIndex0 + 1)*m_vScale.x, HEIGHFIELD(zIndex0+0, xIndex0+1), (zIndex0 + 0)*m_vScale.z);
	Vector3 p11((xIndex0 + 1)*m_vScale.x, HEIGHFIELD(zIndex0+1, xIndex0+1), (zIndex0 + 1)*m_vScale.z);

	// Clip line again first triangle
	Vector3 e0 = p10 - p00;
	Vector3 e1 = p11 - p00;

	t = RayCastTriangle(p0, dp, p00, e0, e1);
	if (t < 1.0f) {
		return t;
	}

	// Clip line against second triangle
	Vector3 p01((xIndex0 + 0)*m_vScale.x, HEIGHFIELD(zIndex0+1, xIndex0+0), (zIndex0 + 1)*m_vScale.z);
	Vector3 e2 = p01 - p00;
	return RayCastTriangle(p0, dp, p00, e1, e2);
}

/**
*  @brief
*    Calculate the bounding box surrounding a line segment
*/
void BodyTerrain::CalculateMinExtend2d(const Vector3 &p0, const Vector3 &p1, Vector3 &boxP0, Vector3 &boxP1)
{
	dFloat x0;
	dFloat x1;
	dFloat z0;
	dFloat z1;

	x0 = Math::Min(p0.x, p1.x) - 1.0e-3f;
	z0 = Math::Min(p0.z, p1.z) - 1.0e-3f;

	x1 = Math::Max(p0.x, p1.x) + 1.0e-3f;
	z1 = Math::Max(p0.z, p1.z) + 1.0e-3f;

	x0 = m_vScale.x*Math::Floor(x0*(1.0f/m_vScale.x));
	z0 = m_vScale.z*Math::Floor(z0*(1.0f/m_vScale.z));
	x1 = m_vScale.x*Math::Floor(x1*(1.0f/m_vScale.x)) + m_vScale.x;
	z1 = m_vScale.z*Math::Floor(z1*(1.0f/m_vScale.z)) + m_vScale.z;

	boxP0.x = Math::Max(x0, m_vBoxMin.x);
	boxP0.z = Math::Max(z0, m_vBoxMin.z);

	boxP1.x = Math::Min(x1, m_vBoxMax.x);
	boxP1.z = Math::Min(z1, m_vBoxMax.z);
}

/**
*  @brief
*    Calculate the bounding box surrounding a line segment
*/
void BodyTerrain::CalculateMinExtend3d(const Vector3 &p0, const Vector3 &p1, Vector3 &boxP0, Vector3 &boxP1)
{
	dFloat x0;
	dFloat x1;
	dFloat y0;
	dFloat y1;
	dFloat z0;
	dFloat z1;

	x0 = Math::Min(p0.x, p1.x) - 1.0e-3f;
	y0 = Math::Min(p0.y, p1.y) - 1.0e-3f;
	z0 = Math::Min(p0.z, p1.z) - 1.0e-3f;

	x1 = Math::Max(p0.x, p1.x) + 1.0e-3f;
	y1 = Math::Max(p0.y, p1.y) + 1.0e-3f;
	z1 = Math::Max(p0.z, p1.z) + 1.0e-3f;

	x0 = m_vScale.x*Math::Floor(x0*(1.0f/m_vScale.x));
	y0 = m_vScale.y*Math::Floor(y0*(1.0f/m_vScale.y));
	z0 = m_vScale.z*Math::Floor(z0*(1.0f/m_vScale.z));

	x1 = m_vScale.x*Math::Floor(x1*(1.0f/m_vScale.x)) + m_vScale.x;
	y1 = m_vScale.y*Math::Floor(y1*(1.0f/m_vScale.y)) + m_vScale.y;
	z1 = m_vScale.z*Math::Floor(z1*(1.0f/m_vScale.z)) + m_vScale.z;

	boxP0.x = Math::Max(x0, m_vBoxMin.x);
	boxP0.y = Math::Max(y0, m_vBoxMin.y);
	boxP0.z = Math::Max(z0, m_vBoxMin.z);

	boxP1.x = Math::Min(x1, m_vBoxMax.x);
	boxP1.y = Math::Min(y1, m_vBoxMax.y);
	boxP1.z = Math::Min(z1, m_vBoxMax.z);
}

/**
*  @brief
*    Clip a line segment against a box
*/
bool BodyTerrain::ClipRay2d(Vector3 &p0, Vector3 &p1, const Vector3 &boxP0, const Vector3 &boxP1)
{
	dFloat t;
	dFloat tmp0;
	dFloat tmp1;

	// Clip against positive x axis
	tmp0 = boxP1.x - p0.x;
	if (tmp0 > 0.0f) {
		tmp1 = boxP1.x - p1.x;
		if (tmp1 < 0.0f) {
			t = tmp0 / (p1.x - p0.x);
			p1.x = boxP1.x;
			p1.y = p0.y + (p1.y - p0.y) * t;
			p1.z = p0.z + (p1.z - p0.z) * t;
		}
	} else {
		tmp1 = boxP1.x - p1.x;
		if (tmp1 > 0.0f) {
			t = tmp0 / (p1.x - p0.x);
			p0.x = boxP1.x;
			p0.y = p0.y + (p1.y - p0.y) * t;
			p0.z = p0.z + (p1.z - p0.z) * t;
		} else {
			return false;
		}
	}

	// Clip against negative x axis
	tmp0 = boxP0.x - p0.x;
	if (tmp0 < 0.0f) {
		tmp1 = boxP0.x - p1.x;
		if (tmp1 > 0.0f) {
			t = tmp0 / (p1.x - p0.x);
			p1.x = boxP0.x;
			p1.y = p0.y + (p1.y - p0.y) * t;
			p1.z = p0.z + (p1.z - p0.z) * t;
		}
	} else {
		tmp1 = boxP0.x - p1.x;
		if (tmp1 < 0.0f) {
			t = tmp0 / (p1.x - p0.x);
			p0.x = boxP0.x;
			p0.y = p0.y + (p1.y - p0.y) * t;
			p0.z = p0.z + (p1.z - p0.z) * t;
		} else {
			return false;
		}
	}

	// Clip against positive z axis
	tmp0 = boxP1.z - p0.z;
	if (tmp0 > 0.0f) {
		tmp1 = boxP1.z - p1.z;
		if (tmp1 < 0.0f) {
			t = tmp0 / (p1.z - p0.z);
			p1.z = boxP1.z;
			p1.x = p0.x + (p1.x - p0.x) * t;
			p1.y = p0.y + (p1.y - p0.y) * t;
		}
	} else {
		tmp1 = boxP1.z - p1.z;
		if (tmp1 > 0.0f) {
			t = tmp0 / (p1.z - p0.z);
			p0.z = boxP1.z;
			p0.x = p0.x + (p1.x - p0.x) * t;
			p0.y = p0.y + (p1.y - p0.y) * t;
		} else {
			return false;
		}
	}

	// Clip against negative z axis
	tmp0 = boxP0.z - p0.z;
	if (tmp0 < 0.0f) {
		tmp1 = boxP0.z - p1.z;
		if (tmp1 > 0.0f) {
			t = tmp0 / (p1.z - p0.z);
			p1.z = boxP0.z;
			p1.x = p0.x + (p1.x - p0.x) * t;
			p1.y = p0.y + (p1.y - p0.y) * t;
		}
	} else {
		tmp1 = boxP0.z - p1.z;
		if (tmp1 < 0.0f) {
			t = tmp0 / (p1.z - p0.z);
			p0.z = boxP0.z;
			p0.x = p0.x + (p1.x - p0.x) * t;
			p0.y = p0.y + (p1.y - p0.y) * t;
		} else {
			return false;
		}
	}

	// The line or part of the line segment is contained by the cell
	return true;
}


//[-------------------------------------------------------]
//[ Private Newton callback functions                     ]
//[-------------------------------------------------------]
void BodyTerrain::MeshCollisionCollideCallback(NewtonUserMeshCollisionCollideDesc *pCollideDescData)
{
	// The user data is the pointer to the collision geometry
	BodyTerrain *map = static_cast<BodyTerrain*>(pCollideDescData->m_userData);

	int x;
	int z;
	int x0;
	int x1;
	int z0;
	int z1;
	int index;
	int faceCount;
	int vertexIndex;
	int step;

	Vector3 boxP0;
	Vector3 boxP1;

	Vector3 p0(pCollideDescData->m_boxP0[0], pCollideDescData->m_boxP0[1], pCollideDescData->m_boxP0[2]);
	Vector3 p1(pCollideDescData->m_boxP1[0], pCollideDescData->m_boxP1[1], pCollideDescData->m_boxP1[2]);

	map->CalculateMinExtend3d(p0, p1, boxP0, boxP1);

	x0 = static_cast<int>(boxP0.x*(1.0f/map->m_vScale.x));
	x1 = static_cast<int>(boxP1.x*(1.0f/map->m_vScale.x));

	z0 = static_cast<int>(boxP0.z*(1.0f/map->m_vScale.z));
	z1 = static_cast<int>(boxP1.z*(1.0f/map->m_vScale.z));

	// Clamp x
	if (x0 < 0)
		x0 = 0;
	if (x0 >= static_cast<int>(map->m_nWidth))
		x0 = map->m_nWidth-1;
	if (x1 < 0)
		x1 = 0;
	if (x1 >= static_cast<int>(map->m_nWidth))
		x1 = map->m_nWidth-1;
//	if (x0 == x1) return;
	// Clamp z
	if (z0 < 0)
		z0 = 0;
	if (z0 >= static_cast<int>(map->m_nHeight))
		z0 = map->m_nHeight-1;
	if (z1 < 0)
		z1 = 0;
	if (z1 >= static_cast<int>(map->m_nHeight))
		z1 = map->m_nHeight-1;
//	if (z0 == z1) return;

	// Initialize the callback data structure
	pCollideDescData->m_vertexStrideInBytes = sizeof(Vector3);
	pCollideDescData->m_userAttribute		= map->m_nAttribute;
	pCollideDescData->m_faceIndexCount		= map->m_nFaceIndices;
	pCollideDescData->m_faceVertexIndex		= map->m_nIndexArray;
	pCollideDescData->m_vertex				= &map->m_vCollisionVertex[0][0];

	// Avoid multiplication
	int MaxCollidingFaces_2 = MaxCollidingFaces*2;

	// Scan the vertices intersected by the box extend
	vertexIndex = 0;
	for (z = z0; z <= z1 && vertexIndex < MaxCollidingFaces_2; z ++) {
		for (x = x0; x <= x1 && vertexIndex < MaxCollidingFaces_2; x ++) {
			map->m_vCollisionVertex[vertexIndex] = Vector3(map->m_vScale.x*x, map->m_pfTerrain[z*map->m_nWidth+x], map->m_vScale.z*z);
//			map->m_vCollisionVertex[vertexIndex] = Vector3(CELL_SIZE * x, map->m_heightField[z][x], CELL_SIZE * z);
			vertexIndex ++;
//			_ASSERTE(vertexIndex < MaxCollidingFaces * 2);
		}
	}

	// Build a vertex list index list mesh from the vertices intersected by the extend
	index = 0;
	faceCount = 0;
	vertexIndex = 0;
	step = x1 - x0 + 1;
	for (z = z0; z < z1 && faceCount < MaxCollidingFaces; z ++) {
		for (x = x0; x < x1 && faceCount < MaxCollidingFaces; x ++) {
/*
			map->m_nAttribute[faceCount]   = (x << 16) + z;
			map->m_nFaceIndices[faceCount] = 3;

			map->m_nIndexArray[index + 0] = vertexIndex + step;
			map->m_nIndexArray[index + 1] = vertexIndex + 1;
			map->m_nIndexArray[index + 2] = vertexIndex;

			index += 3;
			faceCount ++;
	
			map->m_nAttribute[faceCount]   = (x << 16) + z;
			map->m_nFaceIndices[faceCount] = 3;

			map->m_nIndexArray[index + 0] = vertexIndex + step;
			map->m_nIndexArray[index + 1] = vertexIndex + step + 1;
			map->m_nIndexArray[index + 2] = vertexIndex + 1;
*/

			map->m_nAttribute[faceCount]   = (x << 16) + z;
			map->m_nFaceIndices[faceCount] = 3;

			map->m_nIndexArray[index + 0] = vertexIndex;
			map->m_nIndexArray[index + 1] = vertexIndex + step + 1;
			map->m_nIndexArray[index + 2] = vertexIndex + 1;

			index += 3;
			faceCount ++;

			map->m_nAttribute[faceCount]   = (x << 16) + z;
			map->m_nFaceIndices[faceCount] = 3;

			map->m_nIndexArray[index + 0] = vertexIndex;
			map->m_nIndexArray[index + 1] = vertexIndex + step;
			map->m_nIndexArray[index + 2] = vertexIndex + step + 1;


			index += 3;
			faceCount ++;
			vertexIndex ++;

//			_ASSERTE(faceCount < MaxCollidingFaces);
		}
		vertexIndex ++;
	}

	pCollideDescData->m_faceCount = faceCount;
}

dFloat BodyTerrain::UserMeshCollisionRayHitCallback(NewtonUserMeshCollisionRayHitDesc *pRayDescData)
{
	// If no cell was hit, return a large value
//	return 1000000.2f;
//	return 1.2f;


	dFloat t;
	dFloat tx;
	dFloat tz;
	dFloat val;
	dFloat ix0;
	dFloat iz0;
	dFloat scaleX, scaleZ;
	dFloat invScaleX, invScaleZ;
	
	dFloat stepX;
	dFloat stepZ;
	Vector3 normalOut;


	// The user data is the pointer to the collision geometry
	BodyTerrain *map = static_cast<BodyTerrain*>(pRayDescData->m_userData);

	Vector3 q0(pRayDescData->m_p0[0], pRayDescData->m_p0[1], pRayDescData->m_p0[2]);
	Vector3 q1(pRayDescData->m_p1[0], pRayDescData->m_p1[1], pRayDescData->m_p1[2]);

	// if (q1.y < 0.0f) return q0.y / (q0.y - q1.y);

	Vector3 boxP0;
	Vector3 boxP1;

	// Calculate the ray bounding box
	map->CalculateMinExtend2d(q0, q1, boxP0, boxP1);

	Vector3 dq = q1 - q0;
	float fT = 10.0f/(Math::Sqrt(dq.DotProduct(dq)) + 1.0e-6f);
	Vector3 padding(dq.x*map->m_vScale.x*fT, dq.y*map->m_vScale.y*fT, dq.z*map->m_vScale.z*fT);
	
	// Make sure the line segment crosses the original segment bbox
	Vector3 p0 = q0 - padding;
	Vector3 p1 = q1 + padding;

	// Clip the line against the bounding box
	if (map->ClipRay2d(p0, p1, boxP0, boxP1)) {
		int xInc;
		int zInc;
		int xIndex0;
		int zIndex0;
		Vector3 dp = p1 - p0;

		scaleX    = map->m_vScale.x;
		invScaleX = (1.0f / map->m_vScale.x);
		scaleZ    = map->m_vScale.z;
		invScaleZ = (1.0f / map->m_vScale.z);
		ix0		  = Math::Floor(p0.x * invScaleX);
		iz0		  = Math::Floor(p0.z * invScaleZ);

		// Implement a 3ddda line algorithm 
		if (dp.x > 0.0f) {
			xInc = 1;
			val = 1.0f / dp.x;
			stepX = scaleX * val;
			tx = (scaleX * (ix0 + 1.0f) - p0.x) * val;
		} else if (dp.x < 0.0f) {
			xInc = -1;
			val = -1.0f / dp.x;
			stepX = scaleX * val;
			tx = -(scaleX * ix0 - p0.x) * val;
		} else {
			xInc = 0;
			stepX = 0.0f;
			tx = 1.0e10f;
		}

		if (dp.z > 0.0f) {
			zInc = 1;
			val = 1.0f / dp.z;
			stepZ = scaleZ * val;
			tz = (scaleZ * (iz0 + 1.0f) - p0.z) * val;
		} else if (dp.z < 0.0f) {
			zInc = -1;
			val = -1.0f / dp.z;
			stepZ = scaleZ * val;
			tz = -(scaleZ * iz0 - p0.z) * val;
		} else {
			zInc = 0;
			stepZ = 0.0f;
			tz = 1.0e10f;
		}

		xIndex0 = static_cast<int>(ix0);
		zIndex0 = static_cast<int>(iz0);

		// For each cell touched by the line
		do {
			t = map->RayCastCell(xIndex0, zIndex0, q0, dq, normalOut);
			if (t < 1.0f) {
				// Bail out at the first intersection and copy the data into the descriptor
				normalOut = normalOut*(1.0f / Math::Sqrt(normalOut.DotProduct(normalOut)));
				pRayDescData->m_normalOut[0] = normalOut.x;
				pRayDescData->m_normalOut[1] = normalOut.y;
				pRayDescData->m_normalOut[2] = normalOut.z;
				pRayDescData->m_userIdOut = (xIndex0 << 16) + zIndex0;
				return t;
			}

			if (tx < tz) {
				xIndex0 += xInc;
				tx += stepX;
			} else {
				zIndex0 += zInc;
				tz += stepZ;
			}
		} while ((tx <= 1.0f) || (tz <= 1.0f));
	}

	// If no cell was hit, return a large value
	return 1.2f;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsNewton
