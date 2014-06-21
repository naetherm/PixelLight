/*********************************************************\
 *  File: Plane.cpp                                      *
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
#include "PLMath/Matrix3x3.h"
#include "PLMath/Matrix3x4.h"
#include "PLMath/Matrix4x4.h"
#include "PLMath/Plane.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Transformation                                        ]
//[-------------------------------------------------------]
Plane Plane::operator *(const Matrix3x3 &mRot) const
{
	/* Not optimized
	Plane cPlane;

	// Transform p (-D*N) using the 3x3 matrix to p'
	Vector3 vP = GetPointOnPlane();
	vP = mRot*vP;

	// Transform n by the 3x3 matrix to n' and (re-)normalize transformed normal
	Vector3 vN = mRot*Vector3(fN);
	vN.Normalize();

	// Make a new plane through p' with normal n'
	cPlane.ComputeND(vP, vN);
	*/

	// Optimized
	Plane cPlane;

	{ // Transform normal
		// We REALLY need to use the invert transpose of this matrix to avoid troubles with none uniform scale...
		Matrix3x3 mRotIT = mRot;
		mRotIT.Invert();
		mRotIT.Transpose();
		cPlane.a = mRotIT.xx*a + mRotIT.xy*b + mRotIT.xz*c;
		cPlane.b = mRotIT.yx*a + mRotIT.yy*b + mRotIT.yz*c;
		cPlane.c = mRotIT.zx*a + mRotIT.zy*b + mRotIT.zz*c;
	}

	// (Re-)normalize transformed normal (MUST be done because the matrix can contain scale!)
	cPlane.Normalize();

	// Transform point on plane
	float fX  = a*-fD;
	float fY  = b*-fD;
	float fZ  = c*-fD;
	float fX2 = mRot.xx*fX + mRot.xy*fY + mRot.xz*fZ;
	float fY2 = mRot.yx*fX + mRot.yy*fY + mRot.yz*fZ;
	float fZ2 = mRot.zx*fX + mRot.zy*fY + mRot.zz*fZ;
	cPlane.fD = -(fX2*cPlane.a + fY2*cPlane.b + fZ2*cPlane.c);

	// Return the transformed plane
	return cPlane;
}

Plane Plane::operator *(const Matrix3x4 &mTrans) const
{
	/* Not optimized
	// Transform p (-D*N) using the 4x4 matrix to p'
	Vector3 vP = GetPointOnPlane();
	vP = mTrans*vP;

	// Transform n by the upper left 3x3 submatrix to n' and (re-)normalize transformed normal
	Vector3 vN = mTrans.RotateVector(Vector3(fN));
	vN.Normalize();

	// Make a new plane through p' with normal n'
	cPlane.ComputeND(vP, vN);

	// Return the transformed plane
	return cPlane;
	*/

	// Optimized
	Plane cPlane;

	{ // Transform normal
		// We REALLY need to use the invert transpose of this matrix to avoid troubles with none uniform scale...
		Matrix3x4 mTransIT = mTrans;
		mTransIT.Invert();
		mTransIT.Transpose();
		cPlane.a = mTransIT.xx*a + mTransIT.xy*b + mTransIT.xz*c;
		cPlane.b = mTransIT.yx*a + mTransIT.yy*b + mTransIT.yz*c;
		cPlane.c = mTransIT.zx*a + mTransIT.zy*b + mTransIT.zz*c;
	}

	// (Re-)normalize transformed normal (MUST be done because the matrix can contain scale!)
	cPlane.Normalize();

	// Transform point on plane
	float fX  = a*-fD;
	float fY  = b*-fD;
	float fZ  = c*-fD;
	float fX2 = mTrans.xx*fX + mTrans.xy*fY + mTrans.xz*fZ + mTrans.xw;
	float fY2 = mTrans.yx*fX + mTrans.yy*fY + mTrans.yz*fZ + mTrans.yw;
	float fZ2 = mTrans.zx*fX + mTrans.zy*fY + mTrans.zz*fZ + mTrans.zw;
	cPlane.fD = -(fX2*cPlane.a + fY2*cPlane.b + fZ2*cPlane.c);

	// Return the transformed plane
	return cPlane;
}

Plane Plane::operator *(const Matrix4x4 &mTrans) const
{
	/* Not optimized
	// Transform p (-D*N) using the 4x4 matrix to p'
	Vector3 vP = GetPointOnPlane();
	vP = mTrans*vP;

	// Transform n by the upper left 3x3 submatrix to n' and (re-)normalize transformed normal
	Vector3 vN = mTrans.RotateVector(Vector3(fN));
	vN.Normalize();

	// Make a new plane through p' with normal n'
	cPlane.ComputeND(vP, vN);

	// Return the transformed plane
	return cPlane;
	*/

	// Optimized
	Plane cPlane;

	{ // Transform normal
		// We REALLY need to use the invert transpose of this matrix to avoid troubles with none uniform scale...
		Matrix3x4 mTransIT = mTrans;
		mTransIT.Invert();
		mTransIT.Transpose();
		cPlane.a = mTransIT.xx*a + mTransIT.xy*b + mTransIT.xz*c;
		cPlane.b = mTransIT.yx*a + mTransIT.yy*b + mTransIT.yz*c;
		cPlane.c = mTransIT.zx*a + mTransIT.zy*b + mTransIT.zz*c;
	}

	// (Re-)normalize transformed normal (MUST be done because the matrix can contain scale!)
	cPlane.Normalize();

	// Transform point on plane
	float fX  = a*-fD;
	float fY  = b*-fD;
	float fZ  = c*-fD;
	float fX2 = mTrans.xx*fX + mTrans.xy*fY + mTrans.xz*fZ + mTrans.xw;
	float fY2 = mTrans.yx*fX + mTrans.yy*fY + mTrans.yz*fZ + mTrans.yw;
	float fZ2 = mTrans.zx*fX + mTrans.zy*fY + mTrans.zz*fZ + mTrans.zw;
	cPlane.fD = -(fX2*cPlane.a + fY2*cPlane.b + fZ2*cPlane.c);

	// Return the transformed plane
	return cPlane;
}

Plane &Plane::operator *=(const Matrix3x3 &mRot)
{
	/* Not optimized
	*this = *this*mRot;
	*/

	// Optimized
	// Transform point on plane
	float fX  = a*-fD;
	float fY  = b*-fD;
	float fZ  = c*-fD;
	float fX2 = mRot.xx*fX + mRot.xy*fY + mRot.xz*fZ;
	float fY2 = mRot.yx*fX + mRot.yy*fY + mRot.yz*fZ;
	float fZ2 = mRot.zx*fX + mRot.zy*fY + mRot.zz*fZ;

	{ // Transform normal
		// We REALLY need to use the invert transpose of this matrix to avoid troubles with none uniform scale...
		Matrix3x4 mRotIT = mRot;
		mRotIT.Invert();
		mRotIT.Transpose();
		fX = mRotIT.xx*a + mRotIT.xy*b + mRotIT.xz*c;
		fY = mRotIT.yx*a + mRotIT.yy*b + mRotIT.yz*c;
		fZ = mRotIT.zx*a + mRotIT.zy*b + mRotIT.zz*c;
	}

	// (Re-)normalize transformed normal (MUST be done because the matrix can contain scale!)
	// Avoid division through zero...
	float fU = fX*fX + fY*fY + fZ*fZ;
	if (fU) {
		fU = Math::Sqrt(fU);
		if (fU) {
			// Scale
			fU = 1.0f/fU;
			fX *= fU;
			fY *= fU;
			fZ *= fU;
		}
	}

	// Finally, calculate the plane components
	fD = -(fX2*fX + fY2*fY + fZ2*fZ);
	a = fX;
	b = fY;
	c = fZ;

	// Return this transformed plane
	return *this;
}

Plane &Plane::operator *=(const Matrix3x4 &mTrans)
{
	/* Not optimized
	*this = *this*mTrans;
	*/

	// Optimized
	// Transform point on plane
	float fX  = a*-fD;
	float fY  = b*-fD;
	float fZ  = c*-fD;
	float fX2 = mTrans.xx*fX + mTrans.xy*fY + mTrans.xz*fZ + mTrans.xw;
	float fY2 = mTrans.yx*fX + mTrans.yy*fY + mTrans.yz*fZ + mTrans.yw;
	float fZ2 = mTrans.zx*fX + mTrans.zy*fY + mTrans.zz*fZ + mTrans.zw;

	{ // Transform normal
		// We REALLY need to use the invert transpose of this matrix to avoid troubles with none uniform scale...
		Matrix3x4 mTransIT = mTrans;
		mTransIT.Invert();
		mTransIT.Transpose();
		fX = mTransIT.xx*a + mTransIT.xy*b + mTransIT.xz*c;
		fY = mTransIT.yx*a + mTransIT.yy*b + mTransIT.yz*c;
		fZ = mTransIT.zx*a + mTransIT.zy*b + mTransIT.zz*c;
	}

	// (Re-)normalize transformed normal (MUST be done because the matrix can contain scale!)
	// Avoid division through zero...
	float fU = fX*fX + fY*fY + fZ*fZ;
	if (fU) {
		fU = Math::Sqrt(fU);
		if (fU) {
			// Scale
			fU = 1.0f/fU;
			fX *= fU;
			fY *= fU;
			fZ *= fU;
		}
	}

	// Finally, calculate the plane components
	fD = -(fX2*fX + fY2*fY + fZ2*fZ);
	a = fX;
	b = fY;
	c = fZ;

	// Return this transformed plane
	return *this;
}

Plane &Plane::operator *=(const Matrix4x4 &mTrans)
{
	/* Not optimized
	*this = *this*mTrans;
	*/

	// Optimized
	// Transform point on plane
	float fX  = a*-fD;
	float fY  = b*-fD;
	float fZ  = c*-fD;
	float fX2 = mTrans.xx*fX + mTrans.xy*fY + mTrans.xz*fZ + mTrans.xw;
	float fY2 = mTrans.yx*fX + mTrans.yy*fY + mTrans.yz*fZ + mTrans.yw;
	float fZ2 = mTrans.zx*fX + mTrans.zy*fY + mTrans.zz*fZ + mTrans.zw;

	{ // Transform normal
		// We REALLY need to use the invert transpose of this matrix to avoid troubles with none uniform scale...
		Matrix3x4 mTransIT = mTrans;
		mTransIT.Invert();
		mTransIT.Transpose();
		fX = mTransIT.xx*a + mTransIT.xy*b + mTransIT.xz*c;
		fY = mTransIT.yx*a + mTransIT.yy*b + mTransIT.yz*c;
		fZ = mTransIT.zx*a + mTransIT.zy*b + mTransIT.zz*c;
	}

	// (Re-)normalize transformed normal (MUST be done because the matrix can contain scale!)
	// Avoid division through zero...
	float fU = fX*fX + fY*fY + fZ*fZ;
	if (fU) {
		fU = Math::Sqrt(fU);
		if (fU) {
			// Scale
			fU = 1.0f/fU;
			fX *= fU;
			fY *= fU;
			fZ *= fU;
		}
	}

	// Finally, calculate the plane components
	fD = -(fX2*fX + fY2*fY + fZ2*fZ);
	a = fX;
	b = fY;
	c = fZ;

	// Return this transformed plane
	return *this;
}

/**
*  @brief
*    Calculates the plane
*/
Plane &Plane::ComputeND(const Vector3 &vV1, const Vector3 &vV2, const Vector3 &vV3)
{
	// Vector 1
	float fX1 = vV2.x-vV1.x;
	float fY1 = vV2.y-vV1.y;
	float fZ1 = vV2.z-vV1.z;

	// Vector 2
	float fX2 = vV2.x-vV3.x;
	float fY2 = vV2.y-vV3.y;
	float fZ2 = vV2.z-vV3.z;

	// Normal (cross product)
	a = fY1*fZ2 - fZ1*fY2;
	b = fZ1*fX2 - fX1*fZ2;
	c = fX1*fY2 - fY1*fX2;

	// Normalize
	Normalize();

	// Get d (negative scalar product)
	d = -(vV1.x*a + vV1.y*b + vV1.z*c);

	return *this;
}

/**
*  @brief
*    Computes the tangent plane of an ellipsoid
*/
Plane &Plane::ComputeTangentPlaneOfEllipsoid(const Vector3 &vPointPos, const Vector3 &vEllipsoidPos,
											 const Vector3 &vEllipsoidRadius)
{
	const Vector3 vP = vPointPos - vEllipsoidPos;

	const double dA2 = vEllipsoidRadius.x*vEllipsoidPos.x;
	const double dB2 = vEllipsoidRadius.y*vEllipsoidPos.y;

	Vector3 vN(static_cast<float>(vP.x/dA2),
			   static_cast<float>(vP.y/dB2),
			   static_cast<float>(vP.y/dB2));
	vN.Normalize();

	return ComputeND(vPointPos, vN);
}

/**
*  @brief
*    Calculates the interpolated plane from two other planes
*/
Plane Plane::Lerp(const Plane &cP2, float fFactor)
{
	const float fInvFactor = 1.0f-fFactor;
	Plane cRes(a*fInvFactor + cP2.a*fFactor,
			   b*fInvFactor + cP2.b*fFactor,
			   c*fInvFactor + cP2.c*fFactor,
			   d*fInvFactor + cP2.d*fFactor);
	cRes.Normalize();
	return cRes;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
