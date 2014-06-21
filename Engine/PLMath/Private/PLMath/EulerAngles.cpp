/*********************************************************\
 *  File: EulerAngles.cpp                                *
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
#include "PLMath/Quaternion.h"
#include "PLMath/EulerAngles.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
static const float FloatEpsilon = 1.192092896e-07f; // Smallest such that 1.0f+FloatEpsilon != 1.0
 // EulGetOrd unpacks all useful information about order simultaneously
#define EulSafe "\000\001\002\000"
#define EulNext "\001\002\000\001"
#define EulGetOrd(ord,i,j,k,n,s,f) {unsigned o=ord;f=o&1;o>>=1;s=o&1;o>>=1;\
    n=o&1;o>>=1;i=EulSafe[o&3];j=EulNext[i+n];k=EulNext[i+1-n];}
// Original code was
// #define EulGetOrd(ord,i,j,k,h,n,s,f) {unsigned o=ord;f=o&1;o>>=1;s=o&1;o>>=1;\
//    n=o&1;o>>=1;i=EulSafe[o&3];j=EulNext[i+n];k=EulNext[i+1-n];h=s?k:i;}
// but we don't need "h", so it was removed to spare the compiler the work *g*


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets a rotation quaternion by using three given Euler angles
*/
void EulerAngles::ToQuaternion(float fAngleX, float fAngleY, float fAngleZ, Quaternion &qRotation, EOrder nOrder)
{
	int i, j, k, n, s, f;
	EulGetOrd(nOrder, i, j, k, n, s, f);
	if (f == 1) {
		const float t = fAngleX;
		fAngleX = fAngleZ;
		fAngleZ = t;
	}
	if (n == 1)
		fAngleY = -fAngleY;

	const double ti = fAngleX*0.5;
	const double tj = fAngleY*0.5;
	const double th = fAngleZ*0.5;
	const double ci = Math::Cos(ti);
	const double cj = Math::Cos(tj);
	const double ch = Math::Cos(th);
	const double si = Math::Sin(ti);
	const double sj = Math::Sin(tj);
	const double sh = Math::Sin(th);
	const double cc = ci*ch;
	const double cs = ci*sh;
	const double sc = si*ch;
	const double ss = si*sh;

	double a[3] = { 0.0, 0.0, 0.0 };
	if (s == 1) {
		a[i] = cj*(cs + sc);	// Could speed up with trig identities
		a[j] = sj*(cc + ss);
		a[k] = sj*(cs - sc);
		qRotation.w = static_cast<float>(cj*(cc - ss));
	} else {
		a[i] = cj*sc - sj*cs;
		a[j] = cj*ss + sj*cc;
		a[k] = cj*cs - sj*sc;
		qRotation.w = static_cast<float>(cj*cc + sj*ss);
	}
	if (n == 1)
		a[j] = -a[j];

	qRotation.x = static_cast<float>(a[0]);
	qRotation.y = static_cast<float>(a[1]);
	qRotation.z = static_cast<float>(a[2]);
}

/**
*  @brief
*    Returns the Euler angles from a rotation quaternion
*/
void EulerAngles::FromQuaternion(const Quaternion &q, float &fAngleX, float &fAngleY, float &fAngleZ, EOrder nOrder)
{
	// Construct 3x3 matrix
	enum EComponent {X, Y, Z};
	float M[3][3]; // Right-handed, for column vectors
	{
		const double Nq = q.x*q.x+q.y*q.y+q.z*q.z+q.w*q.w;
		const double s  = (Nq > 0.0) ? (2.0 / Nq) : 0.0;
		const double xs = q.x*s,  ys = q.y*s,  zs = q.z*s;
		const double wx = q.w*xs, wy = q.w*ys, wz = q.w*zs;
		const double xx = q.x*xs, xy = q.x*ys, xz = q.x*zs;
		const double yy = q.y*ys, yz = q.y*zs, zz = q.z*zs;
		M[X][X] = static_cast<float>(1.0-(yy+zz)); M[X][Y] = static_cast<float>(xy-wz);       M[X][Z] = static_cast<float>(xz+wy);
		M[Y][X] = static_cast<float>(xy+wz);       M[Y][Y] = static_cast<float>(1.0-(xx+zz)); M[Y][Z] = static_cast<float>(yz-wx);
		M[Z][X] = static_cast<float>(xz-wy);       M[Z][Y] = static_cast<float>(yz+wx);       M[Z][Z] = static_cast<float>(1.0-(xx+yy));
	}

	// Convert 3x3 matrix to Euler angles (in radians)
	int i, j, k, n, s, f;
	EulGetOrd(nOrder, i, j, k, n, s, f);
	if (s == 1) {
		const double sy = Math::Sqrt(M[i][j]*M[i][j] + M[i][k]*M[i][k]);
		if (sy > 16*FloatEpsilon) {
			fAngleX = static_cast<float>(Math::ATan2(M[i][j], M[i][k]));
			fAngleY = static_cast<float>(Math::ATan2(sy, static_cast<double>(M[i][i])));
			fAngleZ = static_cast<float>(Math::ATan2(M[j][i], -M[k][i]));
		} else {
			fAngleX = static_cast<float>(Math::ATan2(-M[j][k], M[j][j]));
			fAngleY = static_cast<float>(Math::ATan2(sy, static_cast<double>(M[i][i])));
			fAngleZ = 0.0f;
		}
	} else {
		const double cy = Math::Sqrt(M[i][i]*M[i][i] + M[j][i]*M[j][i]);
		if (cy > 16*FloatEpsilon) {
			fAngleX = static_cast<float>(Math::ATan2(M[k][j], M[k][k]));
			fAngleY = static_cast<float>(Math::ATan2(static_cast<double>(-M[k][i]), cy));
			fAngleZ = static_cast<float>(Math::ATan2(M[j][i], M[i][i]));
		} else {
			fAngleX = static_cast<float>(Math::ATan2(-M[j][k], M[j][j]));
			fAngleY = static_cast<float>(Math::ATan2(static_cast<double>(-M[k][i]), cy));
			fAngleZ = 0.0f;
		}
	}
	if (n == 1) {
		fAngleX = -fAngleX;
		fAngleY = -fAngleY;
		fAngleZ = -fAngleZ;
	}
	if (f == 1) {
		const float t = fAngleX;
		fAngleX = fAngleZ;
		fAngleZ = t;
	}
}

/**
*  @brief
*    Sets a rotation matrix by using three given Euler angles
*/
void EulerAngles::ToMatrix(float fAngleX, float fAngleY, float fAngleZ, Matrix3x3 &mRot, EOrder nOrder)
{
	int i, j, k, n, s, f;
	EulGetOrd(nOrder, i, j, k, n, s, f);
	if (f == 1) {
		const float t = fAngleX;
		fAngleX = fAngleZ;
		fAngleZ = t;
	}
	if (n == 1) {
		fAngleX = -fAngleX;
		fAngleY = -fAngleY;
		fAngleZ = -fAngleZ;
	}

	const double ti = fAngleX;
	const double tj = fAngleY;
	const double th = fAngleZ;
	const double ci = Math::Cos(ti);
	const double cj = Math::Cos(tj);
	const double ch = Math::Cos(th);
	const double si = Math::Sin(ti);
	const double sj = Math::Sin(tj);
	const double sh = Math::Sin(th);
	const double cc = ci*ch;
	const double cs = ci*sh;
	const double sc = si*ch;
	const double ss = si*sh;

	if (s == 1) {
		mRot.fM33[i][i] = static_cast<float>(cj);     mRot.fM33[j][i] = static_cast<float>(sj*si);     mRot.fM33[k][i] = static_cast<float>(sj*ci);
		mRot.fM33[i][j] = static_cast<float>(sj*sh);  mRot.fM33[j][j] = static_cast<float>(-cj*ss+cc); mRot.fM33[k][j] = static_cast<float>(-cj*cs-sc);
		mRot.fM33[i][k] = static_cast<float>(-sj*ch); mRot.fM33[j][k] = static_cast<float>(cj*sc+cs);  mRot.fM33[k][k] = static_cast<float>(cj*cc-ss);
	} else {
		mRot.fM33[i][i] = static_cast<float>(cj*ch);  mRot.fM33[j][i] = static_cast<float>(sj*sc-cs);  mRot.fM33[k][i] = static_cast<float>(sj*cc+ss);
		mRot.fM33[i][j] = static_cast<float>(cj*sh);  mRot.fM33[j][j] = static_cast<float>(sj*ss+cc);  mRot.fM33[k][j] = static_cast<float>(sj*cs-sc);
		mRot.fM33[i][k] = static_cast<float>(-sj);    mRot.fM33[j][k] = static_cast<float>(cj*si);     mRot.fM33[k][k] = static_cast<float>(cj*ci);
	}
}

void EulerAngles::ToMatrix(float fAngleX, float fAngleY, float fAngleZ, Matrix3x4 &mRot, EOrder nOrder)
{
	int i, j, k, n, s, f;
	EulGetOrd(nOrder, i, j, k, n, s, f);
	if (f == 1) {
		const float t = fAngleX;
		fAngleX = fAngleZ;
		fAngleZ = t;
	}
	if (n == 1) {
		fAngleX = -fAngleX;
		fAngleY = -fAngleY;
		fAngleZ = -fAngleZ;
	}

	const double ti = fAngleX;
	const double tj = fAngleY;
	const double th = fAngleZ;
	const double ci = Math::Cos(ti);
	const double cj = Math::Cos(tj);
	const double ch = Math::Cos(th);
	const double si = Math::Sin(ti);
	const double sj = Math::Sin(tj);
	const double sh = Math::Sin(th);
	const double cc = ci*ch;
	const double cs = ci*sh;
	const double sc = si*ch;
	const double ss = si*sh;

	if (s == 1) {
		mRot.fM43[i][i] = static_cast<float>(cj);     mRot.fM43[j][i] = static_cast<float>(sj*si);     mRot.fM43[k][i] = static_cast<float>(sj*ci);     mRot.xw = 0.0f;
		mRot.fM43[i][j] = static_cast<float>(sj*sh);  mRot.fM43[j][j] = static_cast<float>(-cj*ss+cc); mRot.fM43[k][j] = static_cast<float>(-cj*cs-sc); mRot.yw = 0.0f;
		mRot.fM43[i][k] = static_cast<float>(-sj*ch); mRot.fM43[j][k] = static_cast<float>(cj*sc+cs);  mRot.fM43[k][k] = static_cast<float>(cj*cc-ss);  mRot.zw = 0.0f;
	} else {
		mRot.fM43[i][i] = static_cast<float>(cj*ch);  mRot.fM43[j][i] = static_cast<float>(sj*sc-cs);  mRot.fM43[k][i] = static_cast<float>(sj*cc+ss);  mRot.xw = 0.0f;
		mRot.fM43[i][j] = static_cast<float>(cj*sh);  mRot.fM43[j][j] = static_cast<float>(sj*ss+cc);  mRot.fM43[k][j] = static_cast<float>(sj*cs-sc);  mRot.yw = 0.0f;
		mRot.fM43[i][k] = static_cast<float>(-sj);    mRot.fM43[j][k] = static_cast<float>(cj*si);     mRot.fM43[k][k] = static_cast<float>(cj*ci);     mRot.zw = 0.0f;
	}
}

void EulerAngles::ToMatrix(float fAngleX, float fAngleY, float fAngleZ, Matrix4x4 &mRot, EOrder nOrder)
{
	int i, j, k, n, s, f;
	EulGetOrd(nOrder, i, j, k, n, s, f);
	if (f == 1) {
		const float t = fAngleX;
		fAngleX = fAngleZ;
		fAngleZ = t;
	}
	if (n == 1) {
		fAngleX = -fAngleX;
		fAngleY = -fAngleY;
		fAngleZ = -fAngleZ;
	}

	const double ti = fAngleX;
	const double tj = fAngleY;
	const double th = fAngleZ;
	const double ci = Math::Cos(ti);
	const double cj = Math::Cos(tj);
	const double ch = Math::Cos(th);
	const double si = Math::Sin(ti);
	const double sj = Math::Sin(tj);
	const double sh = Math::Sin(th);
	const double cc = ci*ch;
	const double cs = ci*sh;
	const double sc = si*ch;
	const double ss = si*sh;

	if (s == 1) {
		mRot.fM44[i][i] = static_cast<float>(cj);     mRot.fM44[j][i] = static_cast<float>(sj*si);     mRot.fM44[k][i] = static_cast<float>(sj*ci);     mRot.xw = 0.0f;
		mRot.fM44[i][j] = static_cast<float>(sj*sh);  mRot.fM44[j][j] = static_cast<float>(-cj*ss+cc); mRot.fM44[k][j] = static_cast<float>(-cj*cs-sc); mRot.yw = 0.0f;
		mRot.fM44[i][k] = static_cast<float>(-sj*ch); mRot.fM44[j][k] = static_cast<float>(cj*sc+cs);  mRot.fM44[k][k] = static_cast<float>(cj*cc-ss);  mRot.zw = 0.0f;
		mRot.wx         = 0.0f;          			  mRot.wy         = 0.0f;            			   mRot.wz         = 0.0f;           				mRot.ww = 1.0f;
	} else {
		mRot.fM44[i][i] = static_cast<float>(cj*ch);  mRot.fM44[j][i] = static_cast<float>(sj*sc-cs);  mRot.fM44[k][i] = static_cast<float>(sj*cc+ss);  mRot.xw = 0.0f;
		mRot.fM44[i][j] = static_cast<float>(cj*sh);  mRot.fM44[j][j] = static_cast<float>(sj*ss+cc);  mRot.fM44[k][j] = static_cast<float>(sj*cs-sc);  mRot.yw = 0.0f;
		mRot.fM44[i][k] = static_cast<float>(-sj);    mRot.fM44[j][k] = static_cast<float>(cj*si);     mRot.fM44[k][k] = static_cast<float>(cj*ci);     mRot.zw = 0.0f;
		mRot.wx         = 0.0f;       				  mRot.wy         = 0.0f;             			   mRot.wz         = 0.0f; 							mRot.ww = 1.0f;
	}
}

/**
*  @brief
*    Returns the Euler angles from a rotation matrix
*/
void EulerAngles::FromMatrix(const Matrix3x3 &mRot, float &fAngleX, float &fAngleY, float &fAngleZ, EOrder nOrder)
{
	int i, j, k, n, s, f;
	EulGetOrd(nOrder, i, j, k, n, s, f);

	if (s == 1) {
		const double sy = Math::Sqrt(mRot.fM33[j][i]*mRot.fM33[j][i] + mRot.fM33[k][i]*mRot.fM33[k][i]);
		if (sy > 16*FloatEpsilon) {
			fAngleX = static_cast<float>(Math::ATan2(mRot.fM33[j][i], mRot.fM33[k][i]));
			fAngleY = static_cast<float>(Math::ATan2(sy, static_cast<double>(mRot.fM33[i][i])));
			fAngleZ = static_cast<float>(Math::ATan2(mRot.fM33[i][j], -mRot.fM33[i][k]));
		} else {
			fAngleX = static_cast<float>(Math::ATan2(-mRot.fM33[k][j], mRot.fM33[j][j]));
			fAngleY = static_cast<float>(Math::ATan2(sy, static_cast<double>(mRot.fM33[i][i])));
			fAngleZ = 0.0f;
		}
	} else {
		const double cy = Math::Sqrt(mRot.fM33[i][i]*mRot.fM33[i][i] + mRot.fM33[i][j]*mRot.fM33[i][j]);
		if (cy > 16*FloatEpsilon) {
			fAngleX = static_cast<float>(Math::ATan2(mRot.fM33[j][k], mRot.fM33[k][k]));
			fAngleY = static_cast<float>(Math::ATan2(static_cast<double>(-mRot.fM33[i][k]), cy));
			fAngleZ = static_cast<float>(Math::ATan2(mRot.fM33[i][j], mRot.fM33[i][i]));
		} else {
			fAngleX = static_cast<float>(Math::ATan2(-mRot.fM33[k][j], mRot.fM33[j][j]));
			fAngleY = static_cast<float>(Math::ATan2(static_cast<double>(-mRot.fM33[i][k]), cy));
			fAngleZ = 0.0f;
		}
	}

	if (n == 1) {
		fAngleX = -fAngleX;
		fAngleY = -fAngleY;
		fAngleZ = -fAngleZ;
	}
	if (f == 1) {
		const float t = fAngleX;
		fAngleX = fAngleZ;
		fAngleZ = t;
	}
}

void EulerAngles::FromMatrix(const Matrix3x4 &mRot, float &fAngleX, float &fAngleY, float &fAngleZ, EOrder nOrder)
{
	int i, j, k, n, s, f;
	EulGetOrd(nOrder, i, j, k, n, s, f);

	if (s == 1) {
		const double sy = Math::Sqrt(mRot.fM43[j][i]*mRot.fM43[j][i] + mRot.fM43[k][i]*mRot.fM43[k][i]);
		if (sy > 16*FloatEpsilon) {
			fAngleX = static_cast<float>(Math::ATan2(mRot.fM43[j][i], mRot.fM43[k][i]));
			fAngleY = static_cast<float>(Math::ATan2(sy, static_cast<double>(mRot.fM43[i][i])));
			fAngleZ = static_cast<float>(Math::ATan2(mRot.fM43[i][j], -mRot.fM43[i][k]));
		} else {
			fAngleX = static_cast<float>(Math::ATan2(-mRot.fM43[k][j], mRot.fM43[j][j]));
			fAngleY = static_cast<float>(Math::ATan2(sy, static_cast<double>(mRot.fM43[i][i])));
			fAngleZ = 0.0f;
		}
	} else {
		const double cy = Math::Sqrt(mRot.fM43[i][i]*mRot.fM43[i][i] + mRot.fM43[i][j]*mRot.fM43[i][j]);
		if (cy > 16*FloatEpsilon) {
			fAngleX = static_cast<float>(Math::ATan2(mRot.fM43[j][k], mRot.fM43[k][k]));
			fAngleY = static_cast<float>(Math::ATan2(static_cast<double>(-mRot.fM43[i][k]), cy));
			fAngleZ = static_cast<float>(Math::ATan2(mRot.fM43[i][j], mRot.fM43[i][i]));
		} else {
			fAngleX = static_cast<float>(Math::ATan2(-mRot.fM43[k][j], mRot.fM43[j][j]));
			fAngleY = static_cast<float>(Math::ATan2(static_cast<double>(-mRot.fM43[i][k]), cy));
			fAngleZ = 0.0f;
		}
	}

	if (n == 1) {
		fAngleX = -fAngleX;
		fAngleY = -fAngleY;
		fAngleZ = -fAngleZ;
	}
	if (f == 1) {
		const float t = fAngleX;
		fAngleX = fAngleZ;
		fAngleZ = t;
	}
}

void EulerAngles::FromMatrix(const Matrix4x4 &mRot, float &fAngleX, float &fAngleY, float &fAngleZ, EOrder nOrder)
{
	int i, j, k, n, s, f;
	EulGetOrd(nOrder, i, j, k, n, s, f);

	if (s == 1) {
		const double sy = Math::Sqrt(mRot.fM44[j][i]*mRot.fM44[j][i] + mRot.fM44[k][i]*mRot.fM44[k][i]);
		if (sy > 16*FloatEpsilon) {
			fAngleX = static_cast<float>(Math::ATan2(mRot.fM44[j][i], mRot.fM44[k][i]));
			fAngleY = static_cast<float>(Math::ATan2(sy, static_cast<double>(mRot.fM44[i][i])));
			fAngleZ = static_cast<float>(Math::ATan2(mRot.fM44[i][j], -mRot.fM44[i][k]));
		} else {
			fAngleX = static_cast<float>(Math::ATan2(-mRot.fM44[k][j], mRot.fM44[j][j]));
			fAngleY = static_cast<float>(Math::ATan2(sy, static_cast<double>(mRot.fM44[i][i])));
			fAngleZ = 0.0f;
		}
	} else {
		const double cy = Math::Sqrt(mRot.fM44[i][i]*mRot.fM44[i][i] + mRot.fM44[i][j]*mRot.fM44[i][j]);
		if (cy > 16*FloatEpsilon) {
			fAngleX = static_cast<float>(Math::ATan2(mRot.fM44[j][k], mRot.fM44[k][k]));
			fAngleY = static_cast<float>(Math::ATan2(static_cast<double>(-mRot.fM44[i][k]), cy));
			fAngleZ = static_cast<float>(Math::ATan2(mRot.fM44[i][j], mRot.fM44[i][i]));
		} else {
			fAngleX = static_cast<float>(Math::ATan2(-mRot.fM44[k][j], mRot.fM44[j][j]));
			fAngleY = static_cast<float>(Math::ATan2(static_cast<double>(-mRot.fM44[i][k]), cy));
			fAngleZ = 0.0f;
		}
	}

	if (n == 1) {
		fAngleX = -fAngleX;
		fAngleY = -fAngleY;
		fAngleZ = -fAngleZ;
	}
	if (f == 1) {
		const float t = fAngleX;
		fAngleX = fAngleZ;
		fAngleZ = t;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
