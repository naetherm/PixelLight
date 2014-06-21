/*********************************************************\
 *  File: Quaternion.cpp                                 *
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
#include <PLCore/String/RegEx.h>
#include "PLMath/Matrix3x3.h"
#include "PLMath/Matrix3x4.h"
#include "PLMath/Matrix4x4.h"
#include "PLMath/Quaternion.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLMath {


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
const Quaternion Quaternion::Zero    (0.0f, 0.0f, 0.0f, 0.0f);
const Quaternion Quaternion::Identity(1.0f, 0.0f, 0.0f, 0.0f);


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Constructors                                          ]
//[-------------------------------------------------------]
Quaternion::Quaternion(const Matrix3x3 &mRot)
{
	FromRotationMatrix(mRot);
}

Quaternion::Quaternion(const Matrix3x4 &mRot)
{
	FromRotationMatrix(mRot);
}

Quaternion::Quaternion(const Matrix4x4 &mRot)
{
	FromRotationMatrix(mRot);
}


//[-------------------------------------------------------]
//[ Get and set functions                                 ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns a selected axis and angle from the rotation quaternion
*/
void Quaternion::ToAxisAngle(float &fX, float &fY, float &fZ, float &fAngle) const
{
	// Avoid division through zero...
	float fSqrLength = x*x + y*y + z*z;
	if (fSqrLength) {
		fSqrLength = Math::Sqrt(fSqrLength);
		if (fSqrLength) {
			float fInvLength = 1.0f/fSqrLength;
			fX     = x*fInvLength;
			fY     = y*fInvLength;
			fZ     = z*fInvLength;
			fAngle = 2.0f*Math::ACos(w);

			// Done
			return;
		}
	}

	// Fallback...
	fX     = 0.0;
	fY     = 0.0;
	fZ     = 1.0;
	fAngle = 0.0f;
}

/**
*  @brief
*    Sets a rotation quaternion by using a selected axis and angle
*/
Quaternion &Quaternion::FromAxisAngle(float fX, float fY, float fZ, float fAngle)
{
	// Check whether the angle is 0, in that case we do not need to calculate sin/cos stuff...
	if (Math::Abs(fAngle) < Math::Epsilon) {
		w = 1.0f;
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	} else {
		float fSin = Math::Sin(fAngle*0.5f);
		w = Math::Cos(fAngle*0.5f);
		x = fX*fSin;
		y = fY*fSin;
		z = fZ*fSin;
	}
 
	return *this;
}

/**
*  @brief
*    Returns the x (left) axis
*/
Vector3 Quaternion::GetXAxis() const
{
	float fTy  = 2.0f*y;
	float fTz  = 2.0f*z;
	float fTwy = fTy*w;
	float fTwz = fTz*w;
	float fTxy = fTy*x;
	float fTxz = fTz*x;
	float fTyy = fTy*y;
	float fTzz = fTz*z;
	return Vector3(1.0f-(fTyy+fTzz), fTxy+fTwz, fTxz-fTwy);
}

/**
*  @brief
*    Returns the y (up) axis
*/
Vector3 Quaternion::GetYAxis() const
{
	float fTx  = 2.0f*x;
	float fTy  = 2.0f*y;
	float fTz  = 2.0f*z;
	float fTwx = fTx*w;
	float fTwz = fTz*w;
	float fTxx = fTx*x;
	float fTxy = fTy*x;
	float fTyz = fTz*y;
	float fTzz = fTz*z;
	return Vector3(fTxy-fTwz, 1.0f-(fTxx+fTzz), fTyz+fTwx);
}

/**
*  @brief
*    Returns the z (forward) axis
*/
Vector3 Quaternion::GetZAxis() const
{
	float fTx  = 2.0f*x;
	float fTy  = 2.0f*y;
	float fTz  = 2.0f*z;
	float fTwx = fTx*w;
	float fTwy = fTy*w;
	float fTxx = fTx*x;
	float fTxz = fTz*x;
	float fTyy = fTy*y;
	float fTyz = fTz*y;
	return Vector3(fTxz+fTwy, fTyz-fTwx, 1.0f-(fTxx+fTyy));
}

/**
*  @brief
*    Returns the rotation quaternion as 3x3 matrix
*/
void Quaternion::ToRotationMatrix(Matrix3x3 &mRot) const
{
	float fTx  = 2.0f*x;
	float fTy  = 2.0f*y;
	float fTz  = 2.0f*z;
	float fTwx = fTx*w;
	float fTwy = fTy*w;
	float fTwz = fTz*w;
	float fTxx = fTx*x;
	float fTxy = fTy*x;
	float fTxz = fTz*x;
	float fTyy = fTy*y;
	float fTyz = fTz*y;
	float fTzz = fTz*z;
	mRot.xx = 1.0f-(fTyy+fTzz); mRot.xy =       fTxy-fTwz;  mRot.xz =       fTxz+fTwy;
	mRot.yx =       fTxy+fTwz;  mRot.yy = 1.0f-(fTxx+fTzz); mRot.yz =       fTyz-fTwx;
	mRot.zx =       fTxz-fTwy;  mRot.zy =       fTyz+fTwx;  mRot.zz = 1.0f-(fTxx+fTyy);
}

/**
*  @brief
*    Sets a rotation quaternion by using a 3x3 rotation matrix
*/
Quaternion &Quaternion::FromRotationMatrix(const Matrix3x3 &mRot)
{
	// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
	// article "Quaternion Calculus and Fast Animation"
	float fTrace = mRot.xx+mRot.yy+mRot.zz;
	if (fTrace > 0.0) {
		// |w| > 1/2, may as well choose w > 1/2
		float fRoot = Math::Sqrt(fTrace + 1.0f);  // 2w
		w     = 0.5f*fRoot;
		fRoot = 0.5f/fRoot;  // 1/(4w)
		x     = (mRot.zy-mRot.yz)*fRoot;
		y     = (mRot.xz-mRot.zx)*fRoot;
		z     = (mRot.yx-mRot.xy)*fRoot;
	} else {
		// |w| <= 1/2
		static size_t nNext[3] = { 1, 2, 0 };
		size_t i = 0;
		if (mRot.yy > mRot.xx)
			i = 1;
		if (mRot.zz > mRot.fM33[i][i])
			i = 2;
		size_t j = nNext[i];
		size_t k = nNext[j];

		float fRoot = Math::Sqrt(mRot.fM33[i][i]-mRot.fM33[j][j]-mRot.fM33[k][k] + 1.0f);
		float *apkQuat[3] = { &x, &y, &z };
		*apkQuat[i] = 0.5f*fRoot;
		fRoot = 0.5f/fRoot;
		w = (mRot.fM33[j][k]-mRot.fM33[k][j])*fRoot;
		*apkQuat[j] = (mRot.fM33[i][j]+mRot.fM33[j][i])*fRoot;
		*apkQuat[k] = (mRot.fM33[i][k]+mRot.fM33[k][i])*fRoot;
	}

	return *this;
}

/**
*  @brief
*    Returns the rotation quaternion as 3x4 matrix
*/
void Quaternion::ToRotationMatrix(Matrix3x4 &mRot) const
{
	float fTx  = x+x;
	float fTy  = y+y;
	float fTz  = z+z;
	float fTwx = fTx*w;
	float fTwy = fTy*w;
	float fTwz = fTz*w;
	float fTxx = fTx*x;
	float fTxy = fTy*x;
	float fTxz = fTz*x;
	float fTyy = fTy*y;
	float fTyz = fTz*y;
	float fTzz = fTz*z;
	mRot.xx = 1.0f-(fTyy+fTzz); mRot.xy =       fTxy-fTwz;  mRot.xz =       fTxz+fTwy;   mRot.xw = 0.0f;
	mRot.yx =       fTxy+fTwz;  mRot.yy = 1.0f-(fTxx+fTzz); mRot.yz =       fTyz-fTwx;   mRot.yw = 0.0f;
	mRot.zx =       fTxz-fTwy;  mRot.zy =       fTyz+fTwx;  mRot.zz = 1.0f-(fTxx+fTyy);  mRot.zw = 0.0f;
}

/**
*  @brief
*    Sets a rotation quaternion by using a 3x4 rotation matrix
*/
Quaternion &Quaternion::FromRotationMatrix(const Matrix3x4 &mRot)
{
	// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
	// article "Quaternion Calculus and Fast Animation"
	float fTrace = mRot.xx+mRot.yy+mRot.zz;
	if (fTrace > 0.0) {
		// |w| > 1/2, may as well choose w > 1/2
		float fRoot = Math::Sqrt(fTrace + 1.0f);  // 2w
		w     = 0.5f*fRoot;
		fRoot = 0.5f/fRoot;  // 1/(4w)
		x     = (mRot.zy-mRot.yz)*fRoot;
		y     = (mRot.xz-mRot.zx)*fRoot;
		z     = (mRot.yx-mRot.xy)*fRoot;
	} else {
		// |w| <= 1/2
		static size_t nNext[3] = { 1, 2, 0 };
		size_t i = 0;
		if (mRot.yy > mRot.xx)
			i = 1;
		if (mRot.zz > mRot.fM43[i][i])
			i = 2;
		size_t j = nNext[i];
		size_t k = nNext[j];

		float fRoot = Math::Sqrt(mRot.fM43[i][i]-mRot.fM43[j][j]-mRot.fM43[k][k] + 1.0f);
		float *apkQuat[3] = { &x, &y, &z };
		*apkQuat[i] = 0.5f*fRoot;
		fRoot = 0.5f/fRoot;
		w = (mRot.fM43[j][k]-mRot.fM43[k][j])*fRoot;
		*apkQuat[j] = (mRot.fM43[i][j]+mRot.fM43[j][i])*fRoot;
		*apkQuat[k] = (mRot.fM43[i][k]+mRot.fM43[k][i])*fRoot;
	}

	return *this;
}

/**
*  @brief
*    Returns the rotation quaternion as 4x4 matrix
*/
void Quaternion::ToRotationMatrix(Matrix4x4 &mRot) const
{
	float fTx  = 2.0f*x;
	float fTy  = 2.0f*y;
	float fTz  = 2.0f*z;
	float fTwx = fTx*w;
	float fTwy = fTy*w;
	float fTwz = fTz*w;
	float fTxx = fTx*x;
	float fTxy = fTy*x;
	float fTxz = fTz*x;
	float fTyy = fTy*y;
	float fTyz = fTz*y;
	float fTzz = fTz*z;
	mRot.xx = 1.0f-(fTyy+fTzz); mRot.xy =       fTxy-fTwz;  mRot.xz =       fTxz+fTwy;  mRot.xw = 0.0f;
	mRot.yx =       fTxy+fTwz;  mRot.yy = 1.0f-(fTxx+fTzz); mRot.yz =       fTyz-fTwx;  mRot.yw = 0.0f;
	mRot.zx =       fTxz-fTwy;  mRot.zy =       fTyz+fTwx;  mRot.zz = 1.0f-(fTxx+fTyy); mRot.zw = 0.0f;
	mRot.wx =            0.0f;  mRot.wy =            0.0f;  mRot.wz =            0.0f;  mRot.ww = 1.0f;
}

/**
*  @brief
*    Sets a rotation quaternion by using a 4x4 rotation matrix
*/
Quaternion &Quaternion::FromRotationMatrix(const Matrix4x4 &mRot)
{
	// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
	// article "Quaternion Calculus and Fast Animation"
	float fTrace = mRot.xx+mRot.yy+mRot.zz;
	if (fTrace > 0.0) {
		// |w| > 1/2, may as well choose w > 1/2
		float fRoot = Math::Sqrt(fTrace + 1.0f);  // 2w
		w     = 0.5f*fRoot;
		fRoot = 0.5f/fRoot;  // 1/(4w)
		x     = (mRot.zy-mRot.yz)*fRoot;
		y     = (mRot.xz-mRot.zx)*fRoot;
		z     = (mRot.yx-mRot.xy)*fRoot;
	} else {
		// |w| <= 1/2
		static size_t nNext[3] = { 1, 2, 0 };
		size_t i = 0;
		if (mRot.yy > mRot.xx)
			i = 1;
		if (mRot.zz > mRot.fM44[i][i])
			i = 2;
		size_t j = nNext[i];
		size_t k = nNext[j];

		float fRoot = Math::Sqrt(mRot.fM44[i][i]-mRot.fM44[j][j]-mRot.fM44[k][k] + 1.0f);
		float *apkQuat[3] = { &x, &y, &z };
		*apkQuat[i] = 0.5f*fRoot;
		fRoot = 0.5f/fRoot;
		w = (mRot.fM44[j][k]-mRot.fM44[k][j])*fRoot;
		*apkQuat[j] = (mRot.fM44[i][j]+mRot.fM44[j][i])*fRoot;
		*apkQuat[k] = (mRot.fM44[i][k]+mRot.fM44[k][i])*fRoot;
	}

	return *this;
}


//[-------------------------------------------------------]
//[ Misc                                                  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Computes spherical linear interpolation between qQ1 and qQ2 with time 0-1
*/
void Quaternion::Slerp(const Quaternion &qQ1, const Quaternion &qQ2, float fTime)
{
	// From
	float fFrom[4] = { qQ1.fQ[W], qQ1.fQ[X], qQ1.fQ[Y], qQ1.fQ[Z] };

	// To
	float fTo[4] = { qQ2.fQ[W], qQ2.fQ[X], qQ2.fQ[Y], qQ2.fQ[Z] };

	// Calc cosine
	float fCosom = fFrom[W]*fTo[W] + fFrom[X]*fTo[X] + fFrom[Y]*fTo[Y] + fFrom[Z]*fTo[Z];

	// Adjust signs (if necessary)
	float fTo1[4];
	if (fCosom < 0.0f) {
		fCosom  = -fCosom;
		fTo1[W] = -fTo[W];
		fTo1[X] = -fTo[X];
		fTo1[Y] = -fTo[Y];
		fTo1[Z] = -fTo[Z];
	} else {
		fTo1[W] = fTo[W];
		fTo1[X] = fTo[X];
		fTo1[Y] = fTo[Y];
		fTo1[Z] = fTo[Z];
	}

	// Calculate coefficients
	float fScale0, fScale1;
	if ((1.0f-fCosom) > Math::Epsilon) {
		// Standard case (slerp)
		float fOmega = Math::ACos(fCosom);
		float fSinom = Math::Sin(fOmega);
		fScale0 = Math::Sin((1.0f - fTime)*fOmega)/fSinom;
		fScale1 = Math::Sin(fTime*fOmega)/fSinom;
	} else {
		// "fFrom" and "fTo" quaternions are very close
		//  ... so we can do a linear interpolation:
		fScale0 = 1.0f - fTime;
		fScale1 = fTime;
	}

	// Calculate final values
	w = fScale0*fFrom[W] + fScale1*fTo1[W];
	x = fScale0*fFrom[X] + fScale1*fTo1[X];
	y = fScale0*fFrom[Y] + fScale1*fTo1[Y];
	z = fScale0*fFrom[Z] + fScale1*fTo1[Z];
}

/**
*  @brief
*    To string
*/
String Quaternion::ToString() const
{
	return String::Format("%g %g %g %g", w, x, y, z);
}

/**
*  @brief
*    From string
*/
bool Quaternion::FromString(const String &sString)
{
	if (sString.GetLength()) {
		// Parse components
		uint32 nParsePos  = 0;
		uint32 nComponent = 0;
		static RegEx cRegEx("\\s*(\\S+)");
		while (nComponent < 4 && cRegEx.Match(sString, nParsePos)) {
			fQ[nComponent++] = cRegEx.GetResult(0).GetFloat();
			nParsePos = cRegEx.GetPosition();
		}

		// Set unused components to 0
		while (nComponent < 4)
			fQ[nComponent++] = 0.0f;

		// Done
		return true;
	} else {
		// Error!
		w = x = y = z = 0.0f;
		return false;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
