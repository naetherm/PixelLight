/*********************************************************\
 *  File: Transform3.cpp                                 *
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
#include "PLMath/Transform3.h"


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
*    Default constructor (set's an identity transform matrix)
*/
Transform3::Transform3() :
	m_nInternalFlags(0),
	m_vScale(Vector3::One)
{
}

/**
*  @brief
*    Copy constructor (event data is not copied)
*/
Transform3::Transform3(const Transform3 &cSource) :
	m_nInternalFlags(cSource.m_nInternalFlags),
	m_vPosition(cSource.m_vPosition),
	m_qRotation(cSource.m_qRotation),
	m_vScale(cSource.m_vScale)
{
	// Do only copy "clean" matrices
	if (!(m_nInternalFlags & RecalculateTransformMatrix))
		m_mTrans = cSource.m_mTrans;
	if (!(m_nInternalFlags & RecalculateInverseTransformMatrix))
		m_mInvTrans = cSource.m_mInvTrans;
}

/**
*  @brief
*    Copy operator (event data is not copied)
*/
Transform3 &Transform3::operator =(const Transform3 &cSource)
{
	// Copy data
	m_nInternalFlags = cSource.m_nInternalFlags;
	m_vPosition		 = cSource.m_vPosition;
	m_qRotation		 = cSource.m_qRotation;
	m_vScale		 = cSource.m_vScale;

	// Do only copy "clean" matrices
	if (!(m_nInternalFlags & RecalculateTransformMatrix))
		m_mTrans = cSource.m_mTrans;
	if (!(m_nInternalFlags & RecalculateInverseTransformMatrix))
		m_mInvTrans = cSource.m_mInvTrans;

	// Emit events
	EventPosition();
	EventRotation();
	EventScale();

	// Return a reference to this instance
	return *this;
}

/**
*  @brief
*    Sets the position
*/
void Transform3::SetPosition(const Vector3 &vPosition)
{
	// Set new position
	m_vPosition = vPosition;

	// We have to recalculate the current transform matrix
	m_nInternalFlags |= RecalculateTransformMatrix;
	m_nInternalFlags |= RecalculateInverseTransformMatrix;

	// Emit event
	EventPosition();
}

/**
*  @brief
*    Sets the rotation
*/
void Transform3::SetRotation(const Quaternion &qRotation)
{
	// Set new rotation
	m_qRotation = qRotation;

	// We have to recalculate the current transform matrix
	m_nInternalFlags |= RecalculateTransformMatrix;
	m_nInternalFlags |= RecalculateInverseTransformMatrix;

	// Emit event
	EventRotation();
}

/**
*  @brief
*    Sets the scale
*/
void Transform3::SetScale(const Vector3 &vScale)
{
	// Set new scale
	m_vScale = vScale;

	// We have to recalculate the current transform matrix
	m_nInternalFlags |= RecalculateTransformMatrix;
	m_nInternalFlags |= RecalculateInverseTransformMatrix;

	// Emit event
	EventScale();
}

/**
*  @brief
*    Returns the current final transform matrix
*/
const Matrix3x4 &Transform3::GetMatrix()
{
	// Calculate transform matrix if required
	if (m_nInternalFlags & RecalculateTransformMatrix) {
		// Set position and rotation
		m_mTrans.FromQuatTrans(m_qRotation, m_vPosition);

		// Apply scale
	/* Not optimized
		Matrix3x4 mScale;
		mScale.SetScale(m_vScale);
		m_mTrans *= mScale;
	*/
		// Optimized
		m_mTrans.xx *= m_vScale.x; m_mTrans.xy *= m_vScale.y; m_mTrans.xz *= m_vScale.z;
		m_mTrans.yx *= m_vScale.x; m_mTrans.yy *= m_vScale.y; m_mTrans.yz *= m_vScale.z;
		m_mTrans.zx *= m_vScale.x; m_mTrans.zy *= m_vScale.y; m_mTrans.zz *= m_vScale.z;

		// Recalculation done
		m_nInternalFlags &= ~RecalculateTransformMatrix;
	}

	// Return the current transform matrix
	return m_mTrans;
}

/**
*  @brief
*    Sets the current final transform matrix
*/
void Transform3::SetMatrix(const Matrix3x4 &mTrans)
{
	// Set directly the new transform matrix
	m_mTrans = mTrans;

	// Extract the position from the given transform matrix
	m_mTrans.GetTranslation(m_vPosition.x, m_vPosition.y, m_vPosition.z);

	// Extract the rotation from the given transform matrix
	m_qRotation.FromRotationMatrix(m_mTrans);

	// Extract the scale from the given transform matrix
	m_mTrans.GetScale(m_vScale.x, m_vScale.y, m_vScale.z);

	// Clear recalculation-flag done
	m_nInternalFlags &= ~RecalculateTransformMatrix;

	// We have to recalculate the current inverse transform matrix
	m_nInternalFlags |= RecalculateInverseTransformMatrix;

	// Emit events
	EventPosition();
	EventRotation();
	EventScale();
}

/**
*  @brief
*    Returns the current final inverse transform matrix
*/
const Matrix3x4 &Transform3::GetInverseMatrix()
{
	// Calculate inverse transform matrix if required
	if (m_nInternalFlags & RecalculateInverseTransformMatrix) {
	/* Not optimized
		m_mInvTrans = GetMatrix();
		m_mInvTrans.Invert();
	*/

	// Optimized to avoid some temp variables
		const Matrix3x4 &mTrans = GetMatrix();

		// First, calculate the determinant of the matrix
		float fDet = mTrans.GetDeterminant();
		/*
		// [TODO] I think the assumption that det(A)=1 means that the matrix is orthogonal is wrong (although an orthogonal matrix has a determinant of 1), check it!
		// If the determinant is one, we can use a faster technique
		if (Math::AreEqual(fDet, 1.0f)) {
			// P = [R T] -> inv(P) = [R' -R'*T] (R' = transposed)
			// First transpose the upper left 3x3
			m_mInvTrans.xx = mTrans.xx; m_mInvTrans.xy = mTrans.yx; m_mInvTrans.xz = mTrans.zx;
			m_mInvTrans.yx = mTrans.xy; m_mInvTrans.yy = mTrans.yy; m_mInvTrans.yz = mTrans.zy;
			m_mInvTrans.zx = mTrans.xz; m_mInvTrans.zy = mTrans.yz; m_mInvTrans.zz = mTrans.zz;

			// Now rotate the inversed translation vector
			m_mInvTrans.xw = -(mTrans.xx*mTrans.xw + mTrans.yx*mTrans.yw + mTrans.zx*mTrans.zw);
			m_mInvTrans.yw = -(mTrans.xy*mTrans.xw + mTrans.yy*mTrans.yw + mTrans.zy*mTrans.zw);
			m_mInvTrans.zw = -(mTrans.xz*mTrans.xw + mTrans.yz*mTrans.yw + mTrans.zz*mTrans.zw);
		} else {*/
			// Check for null to avoid division by null
			if (fDet) {
				// Calculate the inverse of the matrix using Cramers rule. Same as Matrix4x4,
				// but wx=0, wy=0, wz=0, ww=1 and we ignore the last row.
				fDet = 1.0f/fDet;
				m_mInvTrans.xx = fDet*(mTrans.yy*mTrans.zz + mTrans.zy*-mTrans.yz); m_mInvTrans.xy = fDet*(mTrans.zy*mTrans.xz + mTrans.xy*-mTrans.zz); m_mInvTrans.xz = fDet*(mTrans.xy*mTrans.yz + mTrans.yy*-mTrans.xz); m_mInvTrans.xw = fDet*(mTrans.xy*(mTrans.zz*mTrans.yw - mTrans.yz*mTrans.zw) + mTrans.yy*(mTrans.xz*mTrans.zw - mTrans.zz*mTrans.xw) + mTrans.zy*(mTrans.yz*mTrans.xw - mTrans.xz*mTrans.yw));
				m_mInvTrans.yx = fDet*(mTrans.yz*mTrans.zx + mTrans.zz*-mTrans.yx); m_mInvTrans.yy = fDet*(mTrans.zz*mTrans.xx + mTrans.xz*-mTrans.zx); m_mInvTrans.yz = fDet*(mTrans.xz*mTrans.yx + mTrans.yz*-mTrans.xx); m_mInvTrans.yw = fDet*(mTrans.xz*(mTrans.zx*mTrans.yw - mTrans.yx*mTrans.zw) + mTrans.yz*(mTrans.xx*mTrans.zw - mTrans.zx*mTrans.xw) + mTrans.zz*(mTrans.yx*mTrans.xw - mTrans.xx*mTrans.yw));
				m_mInvTrans.zx = fDet*(mTrans.yx*mTrans.zy - mTrans.zx* mTrans.yy); m_mInvTrans.zy = fDet*(mTrans.zx*mTrans.xy - mTrans.xx* mTrans.zy); m_mInvTrans.zz = fDet*(mTrans.xx*mTrans.yy - mTrans.yx* mTrans.xy); m_mInvTrans.zw = fDet*(mTrans.xw*(mTrans.zx*mTrans.yy - mTrans.yx*mTrans.zy) + mTrans.yw*(mTrans.xx*mTrans.zy - mTrans.zx*mTrans.xy) + mTrans.zw*(mTrans.yx*mTrans.xy - mTrans.xx*mTrans.yy));
			} else { // Error!
				// For sure, set identity matrix
				m_mInvTrans.SetIdentity();
			}
	//	}

		// Recalculation done
		m_nInternalFlags &= ~RecalculateInverseTransformMatrix;
	}

	// Return the current inverse transform matrix
	return m_mInvTrans;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
