/*********************************************************\
 *  File: Matrix3x3.cpp                                  *
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
#include "PLMath/Matrix3x4.h"
#include "PLMath/Matrix4x4.h"
#include "PLMath/Matrix3x3.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
const Matrix3x3 Matrix3x3::Zero    (0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
const Matrix3x3 Matrix3x3::Identity(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Constructor                                           ]
//[-------------------------------------------------------]
Matrix3x3::Matrix3x3(const Matrix3x4 &mM) :
	// Copy over the values - and respect the correct initialization order!
	xx(mM.xx), yx(mM.yx), zx(mM.zx),
	xy(mM.xy), yy(mM.yy), zy(mM.zy),
	xz(mM.xz), yz(mM.yz), zz(mM.zz)	
{
}

Matrix3x3::Matrix3x3(const Matrix4x4 &mM) :
	// Copy over the values - and respect the correct initialization order!
	xx(mM.xx), yx(mM.yx), zx(mM.zx),
	xy(mM.xy), yy(mM.yy), zy(mM.zy),
	xz(mM.xz), yz(mM.yz), zz(mM.zz)
{
}


//[-------------------------------------------------------]
//[ Comparison                                            ]
//[-------------------------------------------------------]
bool Matrix3x3::operator ==(const Matrix3x3 &mM) const
{
	return (xx == mM.xx && xy == mM.xy && xz == mM.xz &&
			yx == mM.yx && yy == mM.yy && yz == mM.yz &&
			zx == mM.zx && zy == mM.zy && zz == mM.zz);
}

bool Matrix3x3::operator !=(const Matrix3x3 &mM) const
{
	return (xx != mM.xx || xy != mM.xy || xz != mM.xz ||
			yx != mM.yx || yy != mM.yy || yz != mM.yz ||
			zx != mM.zx || zy != mM.zy || zz != mM.zz);
}

bool Matrix3x3::CompareScale(const Matrix3x3 &mM, float fEpsilon) const
{
	if (fEpsilon)
		return (Math::AreEqual(Math::Abs(xx-mM.xx), fEpsilon) && Math::AreEqual(Math::Abs(yy-mM.yy), fEpsilon) && Math::AreEqual(Math::Abs(zz-mM.zz), fEpsilon));
	else
		return (xx == mM.xx && yy == mM.yy && zz == mM.zz);
}

bool Matrix3x3::CompareRotation(const Matrix3x3 &mM, float fEpsilon) const
{
	if (fEpsilon) {
		return (Math::AreEqual(Math::Abs(xx-mM.xx), fEpsilon) && Math::AreEqual(Math::Abs(xy-mM.xy), fEpsilon) && Math::AreEqual(Math::Abs(xz-mM.xz), fEpsilon) &&
				Math::AreEqual(Math::Abs(yx-mM.yx), fEpsilon) && Math::AreEqual(Math::Abs(yy-mM.yy), fEpsilon) && Math::AreEqual(Math::Abs(yz-mM.yz), fEpsilon) &&
				Math::AreEqual(Math::Abs(zx-mM.zx), fEpsilon) && Math::AreEqual(Math::Abs(zy-mM.zy), fEpsilon) && Math::AreEqual(Math::Abs(zz-mM.zz), fEpsilon));
	} else {
		return (xx == mM.xx && yx == mM.yx && zx == mM.zx &&
				xy == mM.xy && yy == mM.yy && zy == mM.zy &&
				xz == mM.xz && yz == mM.yz && zz == mM.zz);
	}
}


//[-------------------------------------------------------]
//[ Operators                                             ]
//[-------------------------------------------------------]
Matrix3x3 &Matrix3x3::operator =(const Matrix3x4 &mM)
{
	// Copy data
	xx = mM.xx; xy = mM.xy; xz = mM.xz;
	yx = mM.yx; yy = mM.yy; yz = mM.yz;
	zx = mM.zx; zy = mM.zy; zz = mM.zz;

	// Return this
	return *this;
}

Matrix3x3 &Matrix3x3::operator =(const Matrix4x4 &mM)
{
	// Copy data
	xx = mM.xx; xy = mM.xy; xz = mM.xz;
	yx = mM.yx; yy = mM.yy; yz = mM.yz;
	zx = mM.zx; zy = mM.zy; zz = mM.zz;

	// Return this
	return *this;
}

Vector4 Matrix3x3::operator *(const Vector4 &vV) const
{
	float x = vV.x, y = vV.y, z = vV.z;
	return Vector4(xx*x + xy*y + xz*z,
				   yx*x + yy*y + yz*z,
				   zx*x + zy*y + zz*z,
				   vV.w);
}

Matrix3x3 Matrix3x3::operator *(const Matrix3x3 &mM) const
{
	return Matrix3x3(xx*mM.xx + xy*mM.yx + xz*mM.zx, xx*mM.xy + xy*mM.yy + xz*mM.zy, xx*mM.xz + xy*mM.yz + xz*mM.zz,
					 yx*mM.xx + yy*mM.yx + yz*mM.zx, yx*mM.xy + yy*mM.yy + yz*mM.zy, yx*mM.xz + yy*mM.yz + yz*mM.zz,
					 zx*mM.xx + zy*mM.yx + zz*mM.zx, zx*mM.xy + zy*mM.yy + zz*mM.zy, zx*mM.xz + zy*mM.yz + zz*mM.zz);
}


//[-------------------------------------------------------]
//[ Matrix operations                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Inverts the matrix
*/
bool Matrix3x3::Invert()
{
	// First, calculate the determinant of the matrix
	float fDet = GetDeterminant();

	// [TODO] I think the assumption that det(A)=1 means that the matrix is orthogonal is wrong (although an orthogonal matrix has a determinant of 1), check it!
/*	// If the determinant is one, we can use a faster technique
	if (Math::AreEqual(fDet, 1.0f)) {
		// We just have to transpose the matrix :)
		Transpose();
	} else {
	*/
		// Check for null to avoid division by null
		if (fDet) {
			// Calculate the inverse of the matrix using Cramers rule. Same as matrix3x4,
			// but we ignore the translation.
			fDet = 1.0f/fDet;
			Set(fDet*(yy*zz + zy*-yz), fDet*(zy*xz + xy*-zz), fDet*(xy*yz + yy*-xz),
				fDet*(yz*zx + zz*-yx), fDet*(zz*xx + xz*-zx), fDet*(xz*yx + yz*-xx),
				fDet*(yx*zy - zx* yy), fDet*(zx*xy - xx* zy), fDet*(xx*yy - yx* xy));
		} else {
			// For sure, set identity matrix
			SetIdentity();

			// Error!
			return false;
		}
//	}

	// Done
	return true;
}

/**
*  @brief
*    Returns the inverse of the matrix
*/
Matrix3x3 Matrix3x3::GetInverted() const
{
	// First, calculate the determinant of the matrix
	float fDet = GetDeterminant();

	// [TODO] I think the assumption that det(A)=1 means that the matrix is orthogonal is wrong (although an orthogonal matrix has a determinant of 1), check it!
/*	// If the determinant is one, we can use a faster technique
	if (Math::AreEqual(fDet, 1.0f)) {
		// We just have to transpose the matrix :)
		return Matrix3x3(xx, yx, zx,
						 xy, yy, zy,
						 xz, yz, zz);
	} else {*/
		// Check for null to avoid division by null
		if (fDet) {
			// Calculate the inverse of the matrix using Cramers rule. Same as matrix3x4,
			// but we ignore the translation.
			fDet = 1.0f/fDet;
			return Matrix3x3(fDet*(yy*zz + zy*-yz), fDet*(zy*xz + xy*-zz), fDet*(xy*yz + yy*-xz),
							 fDet*(yz*zx + zz*-yx), fDet*(zz*xx + xz*-zx), fDet*(xz*yx + yz*-xx),
							 fDet*(yx*zy - zx* yy), fDet*(zx*xy - xx* zy), fDet*(xx*yy - yx* xy));
		} else return Matrix3x3::Identity; // Error!
//	}
}

/**
*  @brief
*    Rotates a vector
*/
Vector3 Matrix3x3::RotateVector(float fX, float fY, float fZ, bool bUniformScale) const
{
	if (bUniformScale) {
		return Vector3(xx*fX + xy*fY + xz*fZ,
					   yx*fX + yy*fY + yz*fZ,
					   zx*fX + zy*fY + zz*fZ);
	} else {
		// We REALLY need to use the invert transpose of this matrix to avoid troubles with none uniform scale...
		Matrix3x3 mIT = *this;
		mIT.Invert();
		mIT.Transpose();
		return mIT.RotateVector(fX, fY, fZ, true);
	}
}

/**
*  @brief
*    Rotates a vector
*/
Vector3 Matrix3x3::RotateVector(const Vector3 &vV, bool bUniformScale) const
{
	if (bUniformScale) {
		const float x = vV.x, y = vV.y, z = vV.z;
		return Vector3(xx*x + xy*y + xz*z,
					   yx*x + yy*y + yz*z,
					   zx*x + zy*y + zz*z);
	} else {
		// We REALLY need to use the invert transpose of this matrix to avoid troubles with none uniform scale...
		Matrix3x3 mIT = *this;
		mIT.Invert();
		mIT.Transpose();
		return mIT.RotateVector(vV, true);
	}
}


//[-------------------------------------------------------]
//[ Scale                                                 ]
//[-------------------------------------------------------]
void Matrix3x3::GetScale(float &fX, float &fY, float &fZ) const
{
/*
	// Not optimized version

	// Because a rotation matrix is orthogonal, we can extract the scale through the
	// length of the axis vectors
	fX = GetXAxis().GetLength();
	fY = GetYAxis().GetLength();
	fZ = GetZAxis().GetLength();

	// Negative scale is a bit tricky because we can't find out which axis is negative, so we
	// just flip all if the determinant is negative and hope this will do the job.
	if (GetDeterminant() < 0.0f) {
		fX = -fX;
		fY = -fY;
		fZ = -fZ;
	}
*/

	// Optimized version
	fX = Math::Sqrt(xx*xx + yx*yx + zx*zx);
	fY = Math::Sqrt(xy*xy + yy*yy + zy*zy);
	fZ = Math::Sqrt(xz*xz + yz*yz + zz*zz);
	if (GetDeterminant() < 0.0f) {
		fX = -fX;
		fY = -fY;
		fZ = -fZ;
	}
}


//[-------------------------------------------------------]
//[ Rotation                                              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets an x axis rotation matrix
*/
void Matrix3x3::FromEulerAngleX(float fAngleX)
{
	float fSin = Math::Sin(fAngleX);
	float fCos = Math::Cos(fAngleX);
	xx = 1.0f; xy = 0.0f; xz =  0.0f;
	yx = 0.0f; yy = fCos; yz = -fSin;
	zx = 0.0f; zy = fSin; zz =  fCos;
}

/**
*  @brief
*    Sets an y axis rotation matrix by using one given Euler angle
*/
void Matrix3x3::FromEulerAngleY(float fAngleY)
{
	float fSin = Math::Sin(fAngleY);
	float fCos = Math::Cos(fAngleY);
	xx =  fCos; xy = 0.0f; xz = fSin;
	yx =  0.0f; yy = 1.0f; yz = 0.0f;
	zx = -fSin; zy = 0.0f; zz = fCos;
}

/**
*  @brief
*    Sets an z axis rotation matrix by using one given Euler angle
*/
void Matrix3x3::FromEulerAngleZ(float fAngleZ)
{
	float fSin = Math::Sin(fAngleZ);
	float fCos = Math::Cos(fAngleZ);
	xx = fCos; xy = -fSin; xz = 0.0f;
	yx = fSin; yy =  fCos; yz = 0.0f;
	zx = 0.0f; zy =  0.0f; zz = 1.0f;
}

/**
*  @brief
*    Returns a rotation matrix as a selected axis and angle
*/
void Matrix3x3::ToAxisAngle(float &fX, float &fY, float &fZ, float &fAngle) const
{
	float fTrace = xx + yy + zz;
	float fCos   = 0.5f*(fTrace-1.0f);
	fAngle = Math::ACos(fCos);  // In [0, Math::Pi]

	if (fAngle > 0.0f) {
		if (fAngle < Math::Pi) {
			fX = zy-yz;
			fY = xz-zx;
			fZ = yx-xy;

			// Normalize the axis
			// Avoid division through zero...
			float fU = fX*fX + fY*fY + fZ*fZ;
			if (fU) {
				fU = Math::Sqrt(fU);
				if (fU) {
					// Scale
					float fScale = 1.0f/fU;
					fX *= fScale;
					fY *= fScale;
					fZ *= fScale;
				}
			}
		} else {
			// Angle is Math::Pi
			if (xx >= yy) {
				// 00 >= 11
				if (xx >= zz) {
					// 00 is maximum diagonal term
					fX = 0.5f*Math::Sqrt(xx - yy - zz + 1.0f);
					float fHalfInverse = 0.5f/fX;
					fY = fHalfInverse*xy;
					fZ = fHalfInverse*xz;
				} else {
					// 22 is maximum diagonal term
					fZ = 0.5f*Math::Sqrt(zz - xx - yy + 1.0f);
					float fHalfInverse = 0.5f/fZ;
					fX = fHalfInverse*xz;
					fY = fHalfInverse*yz;
				}
			} else {
				// 11 > 00
				if (yy >= zz) {
					// 11 is maximum diagonal term
					fY = 0.5f*Math::Sqrt(yy - xx - zz + 1.0f);
					float fHalfInverse  = 0.5f/fY;
					fX = fHalfInverse*xy;
					fZ = fHalfInverse*xz;
				} else {
					// 22 is maximum diagonal term
					fZ = 0.5f*Math::Sqrt(zz - xx - yy + 1.0f);
					float fHalfInverse = 0.5f/fZ;
					fX = fHalfInverse*xz;
					fY = fHalfInverse*yz;
				}
			}
		}
	} else {
		// Angle is 0 and the matrix is the identity. So, we can choose any axis...
		fX = 0.0f;
		fY = 0.0f;
		fZ = 1.0f;
	}
}

/**
*  @brief
*    Sets a rotation matrix by using a selected axis and angle
*/
void Matrix3x3::FromAxisAngle(float fX, float fY, float fZ, float fAngle)
{
	float fRSin = Math::Sin(fAngle);
	float fRCos = Math::Cos(fAngle);
	xx =     fRCos + fX*fX*(1-fRCos); xy = -fZ*fRSin + fX*fY*(1-fRCos); xz =  fY*fRSin + fX*fZ*(1-fRCos);
	yx =  fZ*fRSin + fY*fX*(1-fRCos); yy =     fRCos + fY*fY*(1-fRCos); yz = -fX*fRSin + fY*fZ*(1-fRCos);
	zx = -fY*fRSin + fZ*fX*(1-fRCos); zy =  fX*fRSin + fZ*fY*(1-fRCos); zz =     fRCos + fZ*fZ*(1-fRCos);
}

/**
*  @brief
*    Returns the three axis of a rotation matrix (not normalized)
*/
void Matrix3x3::ToAxis(Vector3 &vX, Vector3 &vY, Vector3 &vZ) const
{
	vX.x = xx; vY.x = xy; vZ.x = xz;
	vX.y = yx; vY.y = yy; vZ.y = yz;
	vX.z = zx; vY.z = zy; vZ.z = zz;
}

/**
*  @brief
*    Sets a rotation matrix by using three given axis
*/
void Matrix3x3::FromAxis(const Vector3 &vX, const Vector3 &vY, const Vector3 &vZ)
{
	xx = vX.x; xy = vY.x; xz = vZ.x;
	yx = vX.y; yy = vY.y; yz = vZ.y;
	zx = vX.z; zy = vY.z; zz = vZ.z;
}

/**
*  @brief
*    Builds a look-at matrix
*/
Matrix3x3 &Matrix3x3::LookAt(const Vector3 &vEye, const Vector3 &vAt, const Vector3 &vUp)
{
	Vector3 vZAxis = (vEye - vAt).GetNormalized();
	Vector3 vXAxis = vUp.CrossProduct(vZAxis).GetNormalized();
	Vector3 vYAxis = vZAxis.CrossProduct(vXAxis);

	// Setup matrix
	xx = vXAxis.x; xy = vXAxis.y; xz = vXAxis.z;
	yx = vYAxis.x; yy = vYAxis.y; yz = vYAxis.z;
	zx = vZAxis.x; zy = vZAxis.y; zz = vZAxis.z;

	// Return this matrix
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
