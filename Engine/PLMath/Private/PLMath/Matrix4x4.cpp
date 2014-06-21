/*********************************************************\
 *  File: Matrix4x4.cpp                                  *
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
#include "PLMath/Plane.h"
#include "PLMath/Quaternion.h"
#include "PLMath/Matrix3x3.h"
#include "PLMath/Matrix3x4.h"
#include "PLMath/Matrix4x4.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
const Matrix4x4 Matrix4x4::Zero    (0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
const Matrix4x4 Matrix4x4::Identity(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Constructor                                           ]
//[-------------------------------------------------------]
Matrix4x4::Matrix4x4(const Matrix3x3 &mM) :
	// Copy over the values - and respect the correct initialization order!
	xx(mM.xx), yx(mM.yx), zx(mM.zx), wx(0.0f),
	xy(mM.xy), yy(mM.yy), zy(mM.zy), wy(0.0f),
	xz(mM.xz), yz(mM.yz), zz(mM.zz), wz(0.0f),
	xw(0.0f),  yw(0.0f),  zw(0.0f),  ww(1.0f)
{
}

Matrix4x4::Matrix4x4(const Matrix3x4 &mM) :
	// Copy over the values - and respect the correct initialization order!
	xx(mM.xx), yx(mM.yx), zx(mM.zx), wx(0.0f),
	xy(mM.xy), yy(mM.yy), zy(mM.zy), wy(0.0f),
	xz(mM.xz), yz(mM.yz), zz(mM.zz), wz(0.0f),
	xw(mM.xw), yw(mM.yw), zw(mM.zw), ww(1.0f)
{
}


//[-------------------------------------------------------]
//[ Comparison                                            ]
//[-------------------------------------------------------]
bool Matrix4x4::operator ==(const Matrix3x4 &mM) const
{
	return (xx == mM.xx && xy == mM.xy && xz == mM.xz && xw == mM.xw &&
			yx == mM.yx && yy == mM.yy && yz == mM.yz && yw == mM.yw &&
			zx == mM.zx && zy == mM.zy && zz == mM.zz && zw == mM.zw &&
			wx ==  0.0f && wy ==  0.0f && wz ==  0.0f && ww ==  1.0f);
}

bool Matrix4x4::operator ==(const Matrix4x4 &mM) const
{
	return (xx == mM.xx && xy == mM.xy && xz == mM.xz && xw == mM.xw &&
			yx == mM.yx && yy == mM.yy && yz == mM.yz && yw == mM.yw &&
			zx == mM.zx && zy == mM.zy && zz == mM.zz && zw == mM.zw &&
			wx == mM.wx && wy == mM.wy && wz == mM.wz && ww == mM.ww);
}

bool Matrix4x4::operator !=(const Matrix3x4 &mM) const
{
	return (xx != mM.xx || xy != mM.xy || xz != mM.xz || xw != mM.xw ||
			yx != mM.yx || yy != mM.yy || yz != mM.yz || yw != mM.yw ||
			zx != mM.zx || zy != mM.zy || zz != mM.zz || zw != mM.zw ||
			wx !=  0.0f || wy !=  0.0f || wz !=  0.0f || ww !=  1.0f);
}

bool Matrix4x4::operator !=(const Matrix4x4 &mM) const
{
	return (xx != mM.xx || xy != mM.xy || xz != mM.xz || xw != mM.xw ||
			yx != mM.yx || yy != mM.yy || yz != mM.yz || yw != mM.yw ||
			zx != mM.zx || zy != mM.zy || zz != mM.zz || zw != mM.zw ||
			wx != mM.wx || wy != mM.wy || wz != mM.wz || ww != mM.ww);
}

bool Matrix4x4::CompareScale(const Matrix4x4 &mM, float fEpsilon) const
{
	if (fEpsilon)
		return (Math::AreEqual(Math::Abs(xx-mM.xx), fEpsilon) && Math::AreEqual(Math::Abs(yy-mM.yy), fEpsilon) && Math::AreEqual(Math::Abs(zz-mM.zz), fEpsilon));
	else
		return (xx == mM.xx && yy == mM.yy && zz == mM.zz);
}

bool Matrix4x4::CompareTranslation(const Matrix4x4 &mM, float fEpsilon) const
{
	if (fEpsilon)
		return (Math::AreEqual(Math::Abs(xw-mM.xw), fEpsilon) && Math::AreEqual(Math::Abs(yw-mM.yw), fEpsilon) && Math::AreEqual(Math::Abs(zw-mM.zw), fEpsilon));
	else
		return (xw == mM.xw && yw == mM.yw && zw == mM.zw);
}

bool Matrix4x4::CompareRotation(const Matrix4x4 &mM, float fEpsilon) const
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
Matrix4x4 &Matrix4x4::operator =(const Matrix3x3 &mM)
{
	xx = mM.xx; xy = mM.xy; xz = mM.xz; xw = 0.0f;
	yx = mM.yx; yy = mM.yy; yz = mM.yz; yw = 0.0f;
	zx = mM.zx; zy = mM.zy; zz = mM.zz; zw = 0.0f;
	wx = 0.0f;  wy = 0.0f;  wz = 0.0f;  ww = 1.0f;
	return *this;
}

Matrix4x4 &Matrix4x4::operator =(const Matrix3x4 &mM)
{
	xx = mM.xx; xy = mM.xy; xz = mM.xz; xw = mM.xw;
	yx = mM.yx; yy = mM.yy; yz = mM.yz; yw = mM.yw;
	zx = mM.zx; zy = mM.zy; zz = mM.zz; zw = mM.zw;
	wx = 0.0f;  wy = 0.0f;  wz = 0.0f;  ww = 1.0f;
	return *this;
}

Matrix4x4 Matrix4x4::operator *(const Matrix3x4 &mM) const
{
	// Have a look at the function documentation for more information about what is done in here
/*  Not optimized
	return Matrix4x4(xx*mM.xx + xy*mM.yx + xz*mM.zx + xw*0.0f, xx*mM.xy + xy*mM.yy + xz*mM.zy + xw*0.0f, xx*mM.xz + xy*mM.yz + xz*mM.zz + xw*0.0f, xx*mM.xw + xy*mM.yw + xz*mM.zw + xw*1.0f,
					 yx*mM.xx + yy*mM.yx + yz*mM.zx + yw*0.0f, yx*mM.xy + yy*mM.yy + yz*mM.zy + yw*0.0f, yx*mM.xz + yy*mM.yz + yz*mM.zz + yw*0.0f, yx*mM.xw + yy*mM.yw + yz*mM.zw + yw*1.0f,
					 zx*mM.xx + zy*mM.yx + zz*mM.zx + zw*0.0f, zx*mM.xy + zy*mM.yy + zz*mM.zy + zw*0.0f, zx*mM.xz + zy*mM.yz + zz*mM.zz + zw*0.0f, zx*mM.xw + zy*mM.yw + zz*mM.zw + zw*1.0f,
					 wx*mM.xx + wy*mM.yx + wz*mM.zx + ww*0.0f, wx*mM.xy + wy*mM.yy + wz*mM.zy + ww*0.0f, wx*mM.xz + wy*mM.yz + wz*mM.zz + ww*0.0f, wx*mM.xw + wy*mM.yw + wz*mM.zw + ww*1.0f);
*/
	// Optimized
	return Matrix4x4(xx*mM.xx + xy*mM.yx + xz*mM.zx, xx*mM.xy + xy*mM.yy + xz*mM.zy, xx*mM.xz + xy*mM.yz + xz*mM.zz, xx*mM.xw + xy*mM.yw + xz*mM.zw + xw,
					 yx*mM.xx + yy*mM.yx + yz*mM.zx, yx*mM.xy + yy*mM.yy + yz*mM.zy, yx*mM.xz + yy*mM.yz + yz*mM.zz, yx*mM.xw + yy*mM.yw + yz*mM.zw + yw,
					 zx*mM.xx + zy*mM.yx + zz*mM.zx, zx*mM.xy + zy*mM.yy + zz*mM.zy, zx*mM.xz + zy*mM.yz + zz*mM.zz, zx*mM.xw + zy*mM.yw + zz*mM.zw + zw,
					 wx*mM.xx + wy*mM.yx + wz*mM.zx, wx*mM.xy + wy*mM.yy + wz*mM.zy, wx*mM.xz + wy*mM.yz + wz*mM.zz, wx*mM.xw + wy*mM.yw + wz*mM.zw + ww);
}

Matrix4x4 Matrix4x4::operator *(const Matrix4x4 &mM) const
{
	return Matrix4x4(xx*mM.xx + xy*mM.yx + xz*mM.zx + xw*mM.wx, xx*mM.xy + xy*mM.yy + xz*mM.zy + xw*mM.wy, xx*mM.xz + xy*mM.yz + xz*mM.zz + xw*mM.wz, xx*mM.xw + xy*mM.yw + xz*mM.zw + xw*mM.ww,
					 yx*mM.xx + yy*mM.yx + yz*mM.zx + yw*mM.wx, yx*mM.xy + yy*mM.yy + yz*mM.zy + yw*mM.wy, yx*mM.xz + yy*mM.yz + yz*mM.zz + yw*mM.wz, yx*mM.xw + yy*mM.yw + yz*mM.zw + yw*mM.ww,
					 zx*mM.xx + zy*mM.yx + zz*mM.zx + zw*mM.wx, zx*mM.xy + zy*mM.yy + zz*mM.zy + zw*mM.wy, zx*mM.xz + zy*mM.yz + zz*mM.zz + zw*mM.wz, zx*mM.xw + zy*mM.yw + zz*mM.zw + zw*mM.ww,
					 wx*mM.xx + wy*mM.yx + wz*mM.zx + ww*mM.wx, wx*mM.xy + wy*mM.yy + wz*mM.zy + ww*mM.wy, wx*mM.xz + wy*mM.yz + wz*mM.zz + ww*mM.wz, wx*mM.xw + wy*mM.yw + wz*mM.zw + ww*mM.ww);
}


//[-------------------------------------------------------]
//[ Matrix operations                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets the elements of the matrix
*/
void Matrix4x4::Set(float fXX, float fXY, float fXZ, float fXW,
					float fYX, float fYY, float fYZ, float fYW,
					float fZX, float fZY, float fZZ, float fZW,
					float fWX, float fWY, float fWZ, float fWW)
{
	xx = fXX; xy = fXY; xz = fXZ; xw = fXW;
	yx = fYX; yy = fYY; yz = fYZ; yw = fYW;
	zx = fZX; zy = fZY; zz = fZZ; zw = fZW;
	wx = fWX; wy = fWY; wz = fWZ; ww = fWW;
}

/**
*  @brief
*    Returns the determinant of the matrix
*/
float Matrix4x4::GetDeterminant() const
{
	return (xx*yy - yx*xy)*(zz*ww - wz*zw) - (xx*zy - zx*xy)*(yz*ww - wz*yw) +
		   (xx*wy - wx*xy)*(yz*zw - zz*yw) + (yx*zy - zx*yy)*(xz*ww - wz*xw) -
		   (yx*wy - wx*yy)*(xz*zw - zz*xw) + (zx*wy - wx*zy)*(xz*yw - yz*xw);
}

/**
*  @brief
*    Transpose this matrix
*/
void Matrix4x4::Transpose()
{
	// xy <-> yx
	float fT = xy;
	xy = yx;
	yx = fT;

	// xz <-> zx
	fT = xz;
	xz = zx;
	zx = fT;

	// xw <-> wx
	fT = xw;
	xw = wx;
	wx = fT;

	// yz <-> zy
	fT = yz;
	yz = zy;
	zy = fT;

	// yw <-> wy
	fT = yw;
	yw = wy;
	wy = fT;

	// zw <-> wz
	fT = zw;
	zw = wz;
	wz = fT;
}

/**
*  @brief
*    Returns the transposed matrix
*/
Matrix4x4 Matrix4x4::GetTransposed() const
{
	return Matrix4x4(xx, yx, zx, wx,
					 xy, yy, zy, wy,
					 xz, yz, zz, wz,
					 xw, yw, zw, ww);
}

/**
*  @brief
*    Inverts the matrix
*/
bool Matrix4x4::Invert()
{
	// First, calculate the determinant of the matrix
	float fDet = GetDeterminant();

	// [TODO] I think the assumption that det(A)=1 means that the matrix is orthogonal is wrong (although an orthogonal matrix has a determinant of 1), check it!
/*	// If the determinant is one, we can use a faster technique
	if (Math::AreEqual(fDet, 1.0f)) {
		// P = [R T] -> inv(P) = [R' -R'*T] (R' = transposed)
		// First transpose the upper left 3x3
		// xy <-> yx
		float fT = xy;
		xy = yx;
		yx = fT;

		// xz <-> zx
		fT = xz;
		xz = zx;
		zx = fT;

		// yz <-> zy
		fT = yz;
		yz = zy;
		zy = fT;

		// Now rotate the inversed translation vector
			  fT  = xx*xw + xy*yw + xz*zw;
		float fT2 = yx*xw + yy*yw + yz*zw;
		float fT3 = zx*xw + zy*yw + zz*zw;
		xw = -fT;
		yw = -fT2;
		zw = -fT3;
	} else {*/
		// Check for null to avoid division by null
		if (fDet) {
			// Calculate the inverse of the matrix using Cramers rule
			fDet = 1.0f/fDet;
			Set(fDet*(yy*(zz*ww - wz*zw) + zy*(wz*yw - yz*ww) + wy*(yz*zw - zz*yw)), fDet*(zy*(xz*ww - wz*xw) + wy*(zz*xw - xz*zw) + xy*(wz*zw - zz*ww)), fDet*(wy*(xz*yw - yz*xw) + xy*(yz*ww - wz*yw) + yy*(wz*xw - xz*ww)), fDet*(xy*(zz*yw - yz*zw) + yy*(xz*zw - zz*xw) + zy*(yz*xw - xz*yw)),
				fDet*(yz*(zx*ww - wx*zw) + zz*(wx*yw - yx*ww) + wz*(yx*zw - zx*yw)), fDet*(zz*(xx*ww - wx*xw) + wz*(zx*xw - xx*zw) + xz*(wx*zw - zx*ww)), fDet*(wz*(xx*yw - yx*xw) + xz*(yx*ww - wx*yw) + yz*(wx*xw - xx*ww)), fDet*(xz*(zx*yw - yx*zw) + yz*(xx*zw - zx*xw) + zz*(yx*xw - xx*yw)),
				fDet*(yw*(zx*wy - wx*zy) + zw*(wx*yy - yx*wy) + ww*(yx*zy - zx*yy)), fDet*(zw*(xx*wy - wx*xy) + ww*(zx*xy - xx*zy) + xw*(wx*zy - zx*wy)), fDet*(ww*(xx*yy - yx*xy) + xw*(yx*wy - wx*yy) + yw*(wx*xy - xx*wy)), fDet*(xw*(zx*yy - yx*zy) + yw*(xx*zy - zx*xy) + zw*(yx*xy - xx*yy)),
				fDet*(yx*(wy*zz - zy*wz) + zx*(yy*wz - wy*yz) + wx*(zy*yz - yy*zz)), fDet*(zx*(wy*xz - xy*wz) + wx*(xy*zz - zy*xz) + xx*(zy*wz - wy*zz)), fDet*(wx*(yy*xz - xy*yz) + xx*(wy*yz - yy*wz) + yx*(xy*wz - wy*xz)), fDet*(xx*(yy*zz - zy*yz) + yx*(zy*xz - xy*zz) + zx*(xy*yz - yy*xz)));
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
Matrix4x4 Matrix4x4::GetInverted() const
{
	// First, calculate the determinant of the matrix
	float fDet = GetDeterminant();

	// [TODO] I think the assumption that det(A)=1 means that the matrix is orthogonal is wrong (although an orthogonal matrix has a determinant of 1), check it!
/*	// If the determinant is one, we can use a faster technique
	if (Math::AreEqual(fDet, 1.0f)) {
		// P = [R T] -> inv(P) = [R' -R'*T] (R' = transposed)
		// Transpose the upper left 3x3 and rotate the inversed translation vector
		return Matrix4x4(xx,   yx,   zx,   -(xx*xw + yx*yw + zx*zw),
						 xy,   yy,   zy,   -(xy*xw + yy*yw + zy*zw),
						 xz,   yz,   zz,   -(xz*xw + yz*yw + zz*zw),
						 0.0f, 0.0f, 0.0f, 1.0f);
	} else {*/
		// Check for null to avoid division by null
		if (fDet) {
			// Calculate the inverse of the matrix using Cramers rule
			fDet = 1.0f/fDet;
			return Matrix4x4(fDet*(yy*(zz*ww - wz*zw) + zy*(wz*yw - yz*ww) + wy*(yz*zw - zz*yw)), fDet*(zy*(xz*ww - wz*xw) + wy*(zz*xw - xz*zw) + xy*(wz*zw - zz*ww)), fDet*(wy*(xz*yw - yz*xw) + xy*(yz*ww - wz*yw) + yy*(wz*xw - xz*ww)), fDet*(xy*(zz*yw - yz*zw) + yy*(xz*zw - zz*xw) + zy*(yz*xw - xz*yw)),
							 fDet*(yz*(zx*ww - wx*zw) + zz*(wx*yw - yx*ww) + wz*(yx*zw - zx*yw)), fDet*(zz*(xx*ww - wx*xw) + wz*(zx*xw - xx*zw) + xz*(wx*zw - zx*ww)), fDet*(wz*(xx*yw - yx*xw) + xz*(yx*ww - wx*yw) + yz*(wx*xw - xx*ww)), fDet*(xz*(zx*yw - yx*zw) + yz*(xx*zw - zx*xw) + zz*(yx*xw - xx*yw)),
							 fDet*(yw*(zx*wy - wx*zy) + zw*(wx*yy - yx*wy) + ww*(yx*zy - zx*yy)), fDet*(zw*(xx*wy - wx*xy) + ww*(zx*xy - xx*zy) + xw*(wx*zy - zx*wy)), fDet*(ww*(xx*yy - yx*xy) + xw*(yx*wy - wx*yy) + yw*(wx*xy - xx*wy)), fDet*(xw*(zx*yy - yx*zy) + yw*(xx*zy - zx*xy) + zw*(yx*xy - xx*yy)),
							 fDet*(yx*(wy*zz - zy*wz) + zx*(yy*wz - wy*yz) + wx*(zy*yz - yy*zz)), fDet*(zx*(wy*xz - xy*wz) + wx*(xy*zz - zy*xz) + xx*(zy*wz - wy*zz)), fDet*(wx*(yy*xz - xy*yz) + xx*(wy*yz - yy*wz) + yx*(xy*wz - wy*xz)), fDet*(xx*(yy*zz - zy*yz) + yx*(zy*xz - xy*zz) + zx*(xy*yz - yy*xz)));
		} else return Matrix4x4::Identity; // Error!
//	}
}

/**
*  @brief
*    Rotates a vector
*/
Vector3 Matrix4x4::RotateVector(float fX, float fY, float fZ, bool bUniformScale) const
{
	if (bUniformScale) {
		const float fInvW = 1.0f/(wx*fX + wy*fY + wz*fZ + ww);
		return Vector3((xx*fX + xy*fY + xz*fZ)*fInvW,
					   (yx*fX + yy*fY + yz*fZ)*fInvW,
					   (zx*fX + zy*fY + zz*fZ)*fInvW);
	} else {
		// We REALLY need to use the invert transpose of this matrix to avoid troubles with none uniform scale...
		Matrix3x4 mIT = *this;
		mIT.Invert();
		mIT.Transpose();
		return mIT.RotateVector(fX, fY, fZ, true);
	}
}

/**
*  @brief
*    Rotates a vector
*/
Vector3 Matrix4x4::RotateVector(const Vector3 &vV, bool bUniformScale) const
{
	if (bUniformScale) {
		const float x = vV.x, y = vV.y, z = vV.z;
		const float fInvW = 1.0f/(wx*x + wy*y + wz*z + ww);
		return Vector3((xx*x + xy*y + xz*z)*fInvW,
					   (yx*x + yy*y + yz*z)*fInvW,
					   (zx*x + zy*y + zz*z)*fInvW);
	} else {
		// We REALLY need to use the invert transpose of this matrix to avoid troubles with none uniform scale...
		Matrix3x4 mIT = *this;
		mIT.Invert();
		mIT.Transpose();
		return mIT.RotateVector(vV, true);
	}
}


//[-------------------------------------------------------]
//[ Scale                                                 ]
//[-------------------------------------------------------]
void Matrix4x4::GetScale(float &fX, float &fY, float &fZ) const
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
*    Sets an x axis rotation matrix by using one given Euler angle
*/
void Matrix4x4::FromEulerAngleX(float fAngleX)
{
	float fSin = Math::Sin(fAngleX);
	float fCos = Math::Cos(fAngleX);
	xx = 1.0f; xy = 0.0f; xz =  0.0f; xw = 0.0f;
	yx = 0.0f; yy = fCos; yz = -fSin; yw = 0.0f;
	zx = 0.0f; zy = fSin; zz =  fCos; zw = 0.0f;
	wx = 0.0f; wy = 0.0f; wz =  0.0f; ww = 1.0f;
}

/**
*  @brief
*    Sets an y axis rotation matrix by using one given Euler angle
*/
void Matrix4x4::FromEulerAngleY(float fAngleY)
{
	float fSin = Math::Sin(fAngleY);
	float fCos = Math::Cos(fAngleY);
	xx =  fCos; xy = 0.0f; xz = fSin; xw = 0.0f;
	yx =  0.0f; yy = 1.0f; yz = 0.0f; yw = 0.0f;
	zx = -fSin; zy = 0.0f; zz = fCos; zw = 0.0f;
	wx =  0.0f; wy = 0.0f; wz = 0.0f; ww = 1.0f;
}

/**
*  @brief
*    Sets an z axis rotation matrix by using one given Euler angle
*/
void Matrix4x4::FromEulerAngleZ(float fAngleZ)
{
	float fSin = Math::Sin(fAngleZ);
	float fCos = Math::Cos(fAngleZ);
	xx = fCos; xy = -fSin; xz = 0.0f; xw = 0.0f;
	yx = fSin; yy =  fCos; yz = 0.0f; yw = 0.0f;
	zx = 0.0f; zy =  0.0f; zz = 1.0f; zw = 0.0f;
	wx = 0.0f; wy =  0.0f; wz = 0.0f; ww = 1.0f;
}

/**
*  @brief
*    Returns a rotation matrix as a selected axis and angle
*/
void Matrix4x4::ToAxisAngle(float &fX, float &fY, float &fZ, float &fAngle) const
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
*    Returns the three axis of a rotation matrix (not normalized)
*/
void Matrix4x4::ToAxis(Vector3 &vX, Vector3 &vY, Vector3 &vZ) const
{
	vX.x = xx; vY.x = xy; vZ.x = xz;
	vX.y = yx; vY.y = yy; vZ.y = yz;
	vX.z = zx; vY.z = zy; vZ.z = zz;
}

/**
*  @brief
*    Sets a rotation matrix by using a selected axis and angle
*/
void Matrix4x4::FromAxisAngle(float fX, float fY, float fZ, float fAngle)
{
	float fRSin = Math::Sin(fAngle);
	float fRCos = Math::Cos(fAngle);
	xx =     fRCos + fX*fX*(1-fRCos); xy = -fZ*fRSin + fX*fY*(1-fRCos); xz =  fY*fRSin + fX*fZ*(1-fRCos); xw = 0.0f;
	yx =  fZ*fRSin + fY*fX*(1-fRCos); yy =     fRCos + fY*fY*(1-fRCos); yz = -fX*fRSin + fY*fZ*(1-fRCos); yw = 0.0f;
	zx = -fY*fRSin + fZ*fX*(1-fRCos); zy =  fX*fRSin + fZ*fY*(1-fRCos); zz =     fRCos + fZ*fZ*(1-fRCos); zw = 0.0f;
	wx = 0.0f;                        wy = 0.0f;                        wz = 0.0f;                        ww = 1.0f;
}

/**
*  @brief
*    Sets a rotation matrix by using three given axis
*/
void Matrix4x4::FromAxis(const Vector3 &vX, const Vector3 &vY, const Vector3 &vZ)
{
	xx = vX.x; xy = vY.x; xz = vZ.x; xw = 0.0f;
	yx = vX.y; yy = vY.y; yz = vZ.y; yw = 0.0f;
	zx = vX.z; zy = vY.z; zz = vZ.z; zw = 0.0f;
	wx = 0.0f; wy = 0.0f; wz = 0.0f; ww = 1.0f;
}

/**
*  @brief
*    Builds a look-at matrix
*/
Matrix4x4 &Matrix4x4::LookAt(const Vector3 &vEye, const Vector3 &vAt, const Vector3 &vUp)
{
	Vector3 vZAxis = (vEye - vAt).GetNormalized();
	Vector3 vXAxis = vUp.CrossProduct(vZAxis).GetNormalized();
	Vector3 vYAxis = vZAxis.CrossProduct(vXAxis);

	// Setup matrix
	xx = vXAxis.x; xy = vXAxis.y; xz = vXAxis.z; xw = -vXAxis.DotProduct(vEye);
	yx = vYAxis.x; yy = vYAxis.y; yz = vYAxis.z; yw = -vYAxis.DotProduct(vEye);
	zx = vZAxis.x; zy = vZAxis.y; zz = vZAxis.z; zw = -vZAxis.DotProduct(vEye);
	wx = 0.0f;     wy = 0.0f;     wz = 0.0f;     ww = 1.0f;

	// Return this matrix
	return *this;
}

/**
*  @brief
*    Builds a view matrix
*/
Matrix4x4 &Matrix4x4::View(const Quaternion &qRotation, const Vector3 &vPosition)
{
	// Calculate view matrix:
	Matrix3x3 mRot;
	qRotation.ToRotationMatrix(mRot);
	mRot.Transpose();
	Vector3 vPos = -(mRot*vPosition);

	// Now create the view matrix
	xx = mRot.xx; xy = mRot.xy; xz = mRot.xz; xw = vPos.x;
	yx = mRot.yx; yy = mRot.yy; yz = mRot.yz; yw = vPos.y;
	zx = mRot.zx; zy = mRot.zy; zz = mRot.zz; zw = vPos.z;
	wx = 0.0f;    wy = 0.0f;    wz = 0.0f;    ww = 1.0f;

	// Return this matrix
	return *this;
}


//[-------------------------------------------------------]
//[ Misc                                                  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets a matrix using a given quaternion and translation
*/
Matrix4x4 &Matrix4x4::FromQuatTrans(const Quaternion &qRotation, const Vector3 &vTranslation)
{
	// Get rotation matrix
	qRotation.ToRotationMatrix(*this);

	// Set translation
	xw = vTranslation.x;
	yw = vTranslation.y;
	zw = vTranslation.z;

	// Return this matrix
	return *this;
}

/**
*  @brief
*    Sets a matrix that reflects the coordinate system about a plane
*/
Matrix4x4 &Matrix4x4::SetReflection(const Plane &cPlane)
{
	xx = -2*cPlane.a*cPlane.a+1; xy = -2*cPlane.a*cPlane.b;   xz = -2*cPlane.a*cPlane.c;   xw = -2*cPlane.a*cPlane.d;
	yx = -2*cPlane.b*cPlane.a;   yy = -2*cPlane.b*cPlane.b+1; yz = -2*cPlane.b*cPlane.c;   yw = -2*cPlane.b*cPlane.d;
	zx = -2*cPlane.c*cPlane.a;   zy = -2*cPlane.c*cPlane.b;   zz = -2*cPlane.c*cPlane.c+1; zw = -2*cPlane.c*cPlane.d;
	wx = 0;                      wy = 0;                      wz = 0;                      ww = 1;

	// Return this matrix
	return *this;
}

/**
*  @brief
*    Sets a shadow projection matrix
*/
Matrix4x4 &Matrix4x4::SetShadowProjection(const Vector4 &vLight, const Plane &cPlane)
{
	float fDot = cPlane.a*vLight.x + cPlane.b*vLight.y + cPlane.c*vLight.z + cPlane.d*vLight.w;

	xx = fDot-vLight.x*cPlane.a; xy =     -vLight.x*cPlane.b; xz =     -vLight.x*cPlane.c; xw =     -vLight.x*cPlane.d;
	yx =     -vLight.y*cPlane.a; yy = fDot-vLight.y*cPlane.b; yz =     -vLight.y*cPlane.c; yw =     -vLight.y*cPlane.d;
	zx =     -vLight.z*cPlane.a; zy =     -vLight.z*cPlane.b; zz = fDot-vLight.z*cPlane.c; zw =     -vLight.z*cPlane.d;
	wx =     -vLight.w*cPlane.a; wy =     -vLight.w*cPlane.b; wz =     -vLight.w*cPlane.c; ww = fDot-vLight.w*cPlane.d;

	// Return this matrix
	return *this;
}

/**
*  @brief
*    Creates a projection matrix that restricts drawing to a small region of the given viewport
*/
void Matrix4x4::RestrictedProjection(const Vector2 &vStartPos, const Vector2 &vEndPos,
								 	 int nX, int nY, int nWidth, int nHeight)
{
	// Get left-top and right-bottom corners
	Vector2 v2DLeftTop, v2DRightBottom;
	if (vStartPos.x < vEndPos.x) {
		v2DLeftTop.x     = vStartPos.x;
		v2DRightBottom.x = vEndPos.x;
	} else {
		v2DLeftTop.x     = vEndPos.x;
		v2DRightBottom.x = vStartPos.x;
	}
	if (vStartPos.y < vEndPos.y) {
		v2DLeftTop.y     = vStartPos.y;
		v2DRightBottom.y = vEndPos.y;
	} else {
		v2DLeftTop.y     = vEndPos.y;
		v2DRightBottom.y = vStartPos.y;
	}

	// Get center and dimension
	float fX      = (v2DLeftTop.x+v2DRightBottom.x)/2;
	float fY      = (v2DLeftTop.y+v2DRightBottom.y)/2;
	float fWidth  = (v2DRightBottom.x-v2DLeftTop.x);
	float fHeight = (v2DRightBottom.y-v2DLeftTop.y);
	if (!fWidth)  fWidth  = 1.0f;
	if (!fHeight) fHeight = 1.0f;

	// Calculate new projection matrix
	float fSX = nWidth/fWidth;
	float fSY = nHeight/fHeight;
	float fTX = (nWidth  + 2.0f*(nX - fX))/fWidth;
	float fTY = (nHeight + 2.0f*(nY - (nHeight-fY)))/fHeight;
	xx = fSX;  xy = 0.0f; xz = 0.0f; xw = fTX;
	yx = 0.0f; yy = fSY;  yz = 0.0f; yw = fTY;
	zx = 0.0f; zy = 0.0f; zz = 1.0f; zw = 0.0f;
	wx = 0.0f; wy = 0.0f; wz = 0.0f; ww = 1.0f;
}

/**
*  @brief
*    Builds a perspective projection matrix
*/
Matrix4x4 &Matrix4x4::Perspective(float fWidth, float fHeight, float fZNear, float fZFar)
{
	// Setup the matrix
	xx = 2*fZNear/fWidth; xy = 0.0f;             xz = 0.0f;                 xw = 0.0f;
	yx = 0.0f;            yy = 2*fZNear/fHeight; yz = 0.0f;                 yw = 0.0f;
	zx = 0.0f;            zy = 0.0f;             zz = fZFar/(fZNear-fZFar); zw = fZNear*fZFar/(fZNear-fZFar);
	wx = 0.0f;            wy = 0.0f;             wz = -1.0f;                ww = 0.0f;

	// Return this matrix
	return *this;
}

/**
*  @brief
*    Builds a perspective projection matrix based on a field of view
*/
Matrix4x4 &Matrix4x4::PerspectiveFov(float fFov, float fAspect, float fZNear, float fZFar)
{
	float e = 1.0f/Math::Tan(fFov*0.5f); // Focal length

	// Setup the matrix
	xx = e/fAspect; xy = 0.0f; xz = 0.0f;                          xw = 0.0f;
	yx = 0.0f;      yy = e;    yz = 0.0f;                          yw = 0.0f;
	zx = 0.0f;      zy = 0.0f; zz = (fZFar+fZNear)/(fZNear-fZFar); zw = (2*fZNear*fZFar)/(fZNear-fZFar);
	wx = 0.0f;      wy = 0.0f; wz = -1.0f;                         ww = 0.0f;

	// Return this matrix
	return *this;
}

/**
*  @brief
*    Builds an infinite perspective projection matrix
*/
Matrix4x4 &Matrix4x4::PerspectiveInfinite(float fWidth, float fHeight, float fZNear)
{
	// Setup the matrix
	xx = 2*fZNear/fWidth; xy = 0.0f;             xz = 0.0f;  xw = 0.0f;
	yx = 0.0f;            yy = 2*fZNear/fHeight; yz = 0.0f;  yw = 0.0f;
	zx = 0.0f;            zy = 0.0f;             zz = -1.0f; zw = -2.0f*fZNear;
	wx = 0.0f;            wy = 0.0f;             wz = -1.0f; ww = 0.0f;

	// Return this matrix
	return *this;
}

/**
*  @brief
*    Builds an infinite perspective projection matrix based on a field of view
*/
Matrix4x4 &Matrix4x4::PerspectiveFovInfinite(float fFov, float fAspect, float fZNear)
{
	float e = 1.0f/Math::Tan(fFov*0.5f); // Focal length

	// Setup the matrix
	xx = e/fAspect; xy = 0.0f; xz = 0.0f;  xw = 0.0f;
	yx = 0.0f;      yy = e;    yz = 0.0f;  yw = 0.0f;
	zx = 0.0f;      zy = 0.0f; zz = -1.0f; zw = -2.0f*fZNear;
	wx = 0.0f;      wy = 0.0f; wz = -1.0f; ww = 0.0f;

	// Return this matrix
	return *this;
}

/**
*  @brief
*    Builds a customized, perspective projection matrix
*/
Matrix4x4 &Matrix4x4::PerspectiveOffCenter(float fL, float fR, float fB, float fT, float fZNear, float fZFar)
{
	// Setup the matrix
	xx = 2*fZNear/(fR-fL); xy = 0.0f;             xz = (fL+fR)/(fR-fL);                  xw = 0.0f;
	yx = 0.0f;             yy = 2*fZNear/(fT-fB); yz = (fT+fB)/(fT-fB);                  yw = 0.0f;
	zx = 0.0f;             zy = 0.0f;             zz = -((fZFar+fZNear)/(fZFar-fZNear)); zw = -((2*fZFar*fZNear)/(fZFar-fZNear));
	wx = 0.0f;             wy = 0.0f;             wz = -1.0f;                            ww = 0.0f;

	// Return this matrix
	return *this;
}

/**
*  @brief
*    Builds a orthogonal projection matrix
*/
Matrix4x4 &Matrix4x4::Ortho(float fWidth, float fHeight, float fZNear, float fZFar)
{
	// Setup the matrix
	xx = 2/fWidth; xy = 0.0f;      xz = 0.0f;                xw = 0.0f;
	yx = 0.0f;     yy = 2/fHeight; yz = 0.0f;                yw = 0.0f;
	zx = 0.0f;     zy = 0.0f;      zz = 1.0f/(fZNear-fZFar); zw = fZNear/(fZNear-fZFar);
	wx = 0.0f;     wy = 0.0f;      wz = 0.0f;                ww = 1.0f;

	// Return this matrix
	return *this;
}

/**
*  @brief
*    Builds a customized, orthogonal projection matrix
*/
Matrix4x4 &Matrix4x4::OrthoOffCenter(float fL, float fR, float fT, float fB, float fZNear, float fZFar)
{
	// Setup the matrix
	xx = 2/(fR-fL); xy = 0.0f;      xz = 0.0f;                xw = (fL+fR)/(fL-fR);
	yx = 0.0f;      yy = 2/(fT-fB); yz = 0.0f;                yw = (fT+fB)/(fB-fT);
	zx = 0.0f;      zy = 0.0f;      zz = 1.0f/(fZNear-fZFar); zw = fZNear/(fZNear-fZFar);
	wx = 0.0f;      wy = 0.0f;      wz = 0.0f;                ww = 1.0f;

	// Return this matrix
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
