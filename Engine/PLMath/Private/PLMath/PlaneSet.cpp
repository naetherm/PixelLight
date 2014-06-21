/*********************************************************\
 *  File: PlaneSet.cpp                                   *
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
#include "PLMath/Sphere.h"
#include "PLMath/Intersect.h"
#include "PLMath/Matrix3x4.h"
#include "PLMath/Matrix4x4.h"
#include "PLMath/PlaneSet.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLMath {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
PlaneSet::PlaneSet()
{
}

/**
*  @brief
*    Copy constructor
*/
PlaneSet::PlaneSet(const PlaneSet &cSource) :
	m_lstPlane(cSource.m_lstPlane)
{
}

/**
*  @brief
*    Destructor
*/
PlaneSet::~PlaneSet()
{
	// Clear plane list
	Clear();
}

/**
*  @brief
*    Creates and adds a new plane to the plane set
*/
Plane *PlaneSet::Create()
{
	Plane &cPlane = m_lstPlane.Add();
	return (&cPlane != &Array<Plane>::Null) ? &cPlane : nullptr;
}

/**
*  @brief
*    Creates 6 planes which enclose a box area
*/
void PlaneSet::CreateBox(const Vector3 &vMin, const Vector3 &vMax)
{
	// Check whether there are enough planes
	if (m_lstPlane.GetNumOfElements() < 6)
		m_lstPlane.Resize(6);

	// Check whether there are not to much planes
	while (m_lstPlane.GetNumOfElements() > 6)
		RemovePlane(m_lstPlane.GetNumOfElements()-1);

	// Setup the planes
	uint32 nNumOfPlanes = m_lstPlane.GetNumOfElements();
	if (nNumOfPlanes > 0) {
		// X
		m_lstPlane[0].ComputeND(Vector3(vMin.x, 0.0f, 0.0f), Vector3::UnitX);
		if (nNumOfPlanes > 1) {
			m_lstPlane[1].ComputeND(Vector3(vMax.x, 0.0f, 0.0f), Vector3::NegativeUnitX);

			// Y
			if (nNumOfPlanes > 2) {
				m_lstPlane[2].ComputeND(Vector3(0.0f, vMin.y, 0.0f), Vector3::UnitY);
				if (nNumOfPlanes > 3) {
					m_lstPlane[3].ComputeND(Vector3(0.0f, vMax.y, 0.0f), Vector3::NegativeUnitY);

					// Z
					if (nNumOfPlanes > 4) {
						m_lstPlane[4].ComputeND(Vector3(0.0f, 0.0f, vMin.z), Vector3::UnitZ);
						if (nNumOfPlanes > 5) {
							m_lstPlane[5].ComputeND(Vector3(0.0f, 0.0f, vMax.z), Vector3::NegativeUnitZ);
						}
					}
				}
			}
		}
	}
}

/**
*  @brief
*    Creates view planes using a projection and view matrix
*/
void PlaneSet::CreateViewPlanes(const Matrix4x4 &mViewProjection, bool bInfProj)
{
	// Check whether there are enough planes
	uint32 nPlanes = bInfProj ? 5 : 6;
	if (m_lstPlane.GetNumOfElements() < nPlanes)
		m_lstPlane.Resize(nPlanes);

	// Check whether there are not to much planes
	while (m_lstPlane.GetNumOfElements() > nPlanes)
		RemovePlane(m_lstPlane.GetNumOfElements()-1);

	// Setup new frustum
	const float *pfClip = mViewProjection;
	for (uint32 i=0; i<m_lstPlane.GetNumOfElements(); i++) {
		Plane &cPlane = m_lstPlane[i];
		switch (i) {
			case VPNear: // Extract the NEAR clipping plane
				cPlane.fN[0] = pfClip[ 3] + pfClip[ 2];
				cPlane.fN[1] = pfClip[ 7] + pfClip[ 6];
				cPlane.fN[2] = pfClip[11] + pfClip[10];
				cPlane.fD    = pfClip[15] + pfClip[14];
				break;

			case VPRight: // Extract the RIGHT clipping plane
				cPlane.fN[0] = pfClip[ 3] - pfClip[ 0];
				cPlane.fN[1] = pfClip[ 7] - pfClip[ 4];
				cPlane.fN[2] = pfClip[11] - pfClip[ 8];
				cPlane.fD    = pfClip[15] - pfClip[12];
				break;
			
			case VPLeft: // Extract the LEFT clipping plane
				cPlane.fN[0] = pfClip[ 3] + pfClip[ 0];
				cPlane.fN[1] = pfClip[ 7] + pfClip[ 4];
				cPlane.fN[2] = pfClip[11] + pfClip[ 8];
				cPlane.fD    = pfClip[15] + pfClip[12];
				break;

			case VPBottom: // Extract the BOTTOM clipping plane
				cPlane.fN[0] = pfClip[ 3] + pfClip[ 1];
				cPlane.fN[1] = pfClip[ 7] + pfClip[ 5];
				cPlane.fN[2] = pfClip[11] + pfClip[ 9];
				cPlane.fD    = pfClip[15] + pfClip[13];
				break;

			case VPTop: // Extract the TOP clipping plane
				cPlane.fN[0] = pfClip[ 3] - pfClip[ 1];
				cPlane.fN[1] = pfClip[ 7] - pfClip[ 5];
				cPlane.fN[2] = pfClip[11] - pfClip[ 9];
				cPlane.fD    = pfClip[15] - pfClip[13];
				break;

			case VPFar: // Extract the FAR clipping plane
				cPlane.fN[0] = pfClip[ 3] - pfClip[ 2];
				cPlane.fN[1] = pfClip[ 7] - pfClip[ 6];
				cPlane.fN[2] = pfClip[11] - pfClip[10];
				cPlane.fD    = pfClip[15] - pfClip[14];
				break;
		}

		// Avoid division through zero...
		// Normalize it
		float fU = cPlane.fN[0]*cPlane.fN[0] + cPlane.fN[1]*cPlane.fN[1] + cPlane.fN[2]*cPlane.fN[2];
		if (fU) {
			fU = Math::Sqrt(fU);
			if (fU) {
				// Scale
				cPlane.fN[0] /= fU;
				cPlane.fN[1] /= fU;
				cPlane.fN[2] /= fU;
				cPlane.fD    /= fU;
			}
		}
	}
}

/**
*  @brief
*    Creates view planes using vertices and a view position
*/
bool PlaneSet::CreateViewPlanes(const Array<Vector3> &lstVertices, const Vector3 &vViewPosition)
{
	// Are there enough vertices provided?
	const uint32 nNumOfVertices = lstVertices.GetNumOfElements();
	if (nNumOfVertices >= 3) {
		// Check whether there are enough planes
		if (GetNumOfPlanes() < nNumOfVertices+1)
			GetList().Resize(nNumOfVertices+1);

		// Check whether there are not to much planes
		while (GetNumOfPlanes() > nNumOfVertices+1)
			RemovePlane(GetNumOfPlanes()-1);

		// Near plane
		m_lstPlane[0].ComputeND(lstVertices[0], lstVertices[1], lstVertices[2]);

		// Side planes
		for (uint32 i=0; i<nNumOfVertices; i++)
			m_lstPlane[i+1].ComputeND(vViewPosition, lstVertices[i], lstVertices[(i+1) % nNumOfVertices]);

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}

/**
*  @brief
*    Creates selection planes
*/
void PlaneSet::CreateSelectionPlanes(const Vector2 &vStartPos, const Vector2 &vEndPos,
									 int nX, int nY, int nWidth, int nHeight,
									 const Matrix4x4 &mProjection, const Matrix3x4 &mView,
									 bool bInfProj)
{
	// Calculate restricted projection matrix
	Matrix4x4 mRestrictedProjection;
	mRestrictedProjection.RestrictedProjection(vStartPos, vEndPos, nX, nY, nWidth, nHeight);
	mRestrictedProjection *= mProjection;

	// Concatenate (multiply) the view matrix and the projection matrix
	mRestrictedProjection *= mView;

	// Create selection planes
	CreateViewPlanes(mRestrictedProjection, bInfProj);
}

/**
*  @brief
*    Creates selection planes
*/
void PlaneSet::CreateSelectionPlanes(const Vector2 &vStartPos, const Vector2 &vEndPos,
									 int nX, int nY, int nWidth, int nHeight,
									 const Matrix4x4 &mProjection, const Matrix4x4 &mView,
									 bool bInfProj)
{
	// Calculate restricted projection matrix
	Matrix4x4 mRestrictedProjection;
	mRestrictedProjection.RestrictedProjection(vStartPos, vEndPos, nX, nY, nWidth, nHeight);
	mRestrictedProjection *= mProjection;

	// Concatenate (multiply) the view matrix and the projection matrix
	mRestrictedProjection *= mView;

	// Create selection planes
	CreateViewPlanes(mRestrictedProjection, bInfProj);
}

/**
*  @brief
*    Returns the number of planes
*/
uint32 PlaneSet::GetNumOfPlanes() const
{
	return m_lstPlane.GetNumOfElements();
}

/**
*  @brief
*    Returns a plane
*/
const Plane *PlaneSet::operator [](int nID) const
{
	const Plane &cPlane = m_lstPlane[nID];
	return (&cPlane != &Array<Plane>::Null) ? &cPlane : nullptr;
}

/**
*  @brief
*    Returns a plane
*/
Plane *PlaneSet::operator [](int nID)
{
	Plane &cPlane = m_lstPlane[nID];
	return (&cPlane != &Array<Plane>::Null) ? &cPlane : nullptr;
}

/**
*  @brief
*    Returns the plane list
*/
Array<Plane> &PlaneSet::GetList()
{
	return m_lstPlane;
}

/**
*  @brief
*    Returns the plane list
*/
const Array<Plane> &PlaneSet::GetList() const
{
	return m_lstPlane;
}

/**
*  @brief
*    Removes a plane
*/
bool PlaneSet::RemovePlane(uint32 nID)
{
	const Plane &cPlane = m_lstPlane[nID];
	if (&cPlane != &Array<Plane>::Null) {
		m_lstPlane.RemoveAtIndex(nID);

		// Done
		return true;
	} else {
		return false; // Error!
	}
}

/**
*  @brief
*    Clears the plane set
*/
void PlaneSet::Clear()
{
	m_lstPlane.Clear();
}

/**
*  @brief
*    Copy operator
*/
PlaneSet &PlaneSet::operator =(const PlaneSet &cSource)
{
	// Copy data
	m_lstPlane = cSource.m_lstPlane;

	// Return this
	return *this;
}

/**
*  @brief
*    Plane set transformation
*/
PlaneSet &PlaneSet::operator *=(const Matrix3x3 &mRot)
{
	// Transform all planes by the given matrix
	for (uint32 i=0; i<m_lstPlane.GetNumOfElements(); i++)
		m_lstPlane[i] *= mRot;

	// Return this
	return *this;
}

/**
*  @brief
*    Plane set transformation
*/
PlaneSet &PlaneSet::operator *=(const Matrix3x4 &mTrans)
{
	// Transform all planes by the given matrix
	for (uint32 i=0; i<m_lstPlane.GetNumOfElements(); i++)
		m_lstPlane[i] *= mTrans;

	// Return this
	return *this;
}

/**
*  @brief
*    Plane set transformation
*/
PlaneSet &PlaneSet::operator *=(const Matrix4x4 &mTrans)
{
	// Transform all planes by the given matrix
	for (uint32 i=0; i<m_lstPlane.GetNumOfElements(); i++)
		m_lstPlane[i] *= mTrans;

	// Return this
	return *this;
}

/**
*  @brief
*    Checks whether the plane set is convex or not
*/
bool PlaneSet::IsConvex() const
{
	for (uint32 nP1=0; nP1<m_lstPlane.GetNumOfElements(); nP1++) {
		uint32 nP2 = 0;
		for (; nP2<m_lstPlane.GetNumOfElements(); nP2++) {
			if (nP1 != nP2) {
				// Check plane side
				const float *pfN1 = m_lstPlane[nP1].fN;
				const float *pfN2 = m_lstPlane[nP2].fN;
				if (pfN1[0]*pfN2[0] + pfN1[1]*pfN2[1] + pfN1[2]*pfN2[2] < 0.0f)
					break; // This plane has a 'neighbour'
			}
		}
		if (nP2 >= m_lstPlane.GetNumOfElements())
			return false; // The plane set isn't convex
	}

	// The plane set is convex
	return true;
}

/**
*  @brief
*    Calculates the sphere surrounding the enclosed area
*/
void PlaneSet::CalculateSphere(Sphere &cSphere) const
{
	// Init sphere
	cSphere.SetPos();
	cSphere.SetRadius();

	// Find all plane intersection points
	Vector3 vD;
	Array<Vector3> lstPoints;
	for (uint32 nP1=0; nP1<m_lstPlane.GetNumOfElements(); nP1++) {
		const Plane &cP1 = m_lstPlane[nP1];
		for (uint32 nP2=0; nP2<m_lstPlane.GetNumOfElements(); nP2++) {
			const Plane cP2 = m_lstPlane[nP2];
			if (nP2 != nP1) {
				for (uint32 nP3=0; nP3<m_lstPlane.GetNumOfElements(); nP3++) {
					const Plane &cP3 = m_lstPlane[nP3];
					if (nP3 != nP1 && nP3 != nP2) {
						Vector3 vRes;
						if (Intersect::PlanePlanePlane(cP1, cP2, cP3, vRes)) {
							lstPoints.Add(vRes);
							vD += vRes;
						}
					}
				}
			}
		}
	}

	// Get sphere position and radius
	if (lstPoints.GetNumOfElements()) {
		vD /= static_cast<float>(lstPoints.GetNumOfElements());
		cSphere.SetPos(vD);
		float fMaxLength = 0.0f;
		for (uint32 i=0; i<lstPoints.GetNumOfElements(); i++) {
			const Vector3 &vP = lstPoints[i];
			const float fLength = (vP-vD).GetLength();
			if (fLength > fMaxLength)
				fMaxLength = fLength;
		}
		cSphere.SetRadius(fMaxLength);
	}
}

/**
*  @brief
*    Calculates the box surrounding the enclosed area
*/
void PlaneSet::CalculateBox(Vector3 &vMin, Vector3 &vMax) const
{
	// Find all plane intersection points
	bool bFirst = true;
	vMin = vMax = Vector3::Zero;
	for (uint32 nP1=0; nP1<m_lstPlane.GetNumOfElements(); nP1++) {
		const Plane &cP1 = m_lstPlane[nP1];
		for (uint32 nP2=0; nP2<m_lstPlane.GetNumOfElements(); nP2++) {
			const Plane &cP2 = m_lstPlane[nP2];
			if (nP2 != nP1) {
				for (uint32 nP3=0; nP3<m_lstPlane.GetNumOfElements(); nP3++) {
					const Plane &cP3 = m_lstPlane[nP3];
					if (nP3 != nP1 && nP3 != nP2) {
						Vector3 vRes;
						if (Intersect::PlanePlanePlane(cP1, cP2, cP3, vRes)) {
							if (bFirst) {
								vMin = vMax = vRes;
								bFirst = false;
							} else {
								for (int i=0; i<3; i++) {
									if (vMin[i] > vRes[i])
										vMin[i] = vRes[i];
									if (vMax[i] < vRes[i])
										vMax[i] = vRes[i];
								}
							}
						}
					}
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
