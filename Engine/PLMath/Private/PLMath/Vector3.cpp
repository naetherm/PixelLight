/*********************************************************\
 *  File: Vector3.cpp                                    *
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
#include "PLMath/Plane.h"
#include "PLMath/Rectangle.h"
#include "PLMath/Quaternion.h"
#include "PLMath/Matrix3x3.h"
#include "PLMath/Matrix3x4.h"
#include "PLMath/Matrix4x4.h"
#include "PLMath/Vector3.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLMath {


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
const Vector3 Vector3::Zero			( 0.0f,  0.0f,  0.0f);
const Vector3 Vector3::One			( 1.0f,  1.0f,  1.0f);
const Vector3 Vector3::NegativeOne	(-1.0f, -1.0f, -1.0f);
const Vector3 Vector3::UnitX		( 1.0f,  0.0f,  0.0f);
const Vector3 Vector3::UnitY		( 0.0f,  1.0f,  0.0f);
const Vector3 Vector3::UnitZ		( 0.0f,  0.0f,  1.0f);
const Vector3 Vector3::NegativeUnitX(-1.0f,  0.0f,  0.0f);
const Vector3 Vector3::NegativeUnitY( 0.0f, -1.0f,  0.0f);
const Vector3 Vector3::NegativeUnitZ( 0.0f,  0.0f, -1.0f);


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Constructors                                          ]
//[-------------------------------------------------------]
Vector3::Vector3(const Vector2 &vV, float fZ) :
	x(vV.x),
	y(vV.y),
	z(fZ)
{
}

Vector3::Vector3(const Vector4 &vV) :
	x(vV.x),
	y(vV.y),
	z(vV.z)
{
}


//[-------------------------------------------------------]
//[ Assignment operators                                  ]
//[-------------------------------------------------------]
Vector3 &Vector3::operator =(const Vector2 &vV)
{
	x = vV.x;
	y = vV.y;
	z = 0.0f;
	return *this;
}

Vector3 &Vector3::operator =(const Vector4 &vV)
{
	x = vV.x;
	y = vV.y;
	z = vV.z;
	return *this;
}


/**
*  @brief
*    Rotates the vector
*/
Vector3 &Vector3::operator *=(const Matrix3x3 &mRot)
{
	float xt = x, yt = y, zt = z;
	x = mRot.xx*xt + mRot.xy*yt + mRot.xz*zt;
	y = mRot.yx*xt + mRot.yy*yt + mRot.yz*zt;
	z = mRot.zx*xt + mRot.zy*yt + mRot.zz*zt;
	return *this;
}

/**
*  @brief
*    Transforms the vector
*/
Vector3 &Vector3::operator *=(const Matrix3x4 &mTrans)
{
	float xt = x, yt = y, zt = z;
	x = mTrans.xx*xt + mTrans.xy*yt + mTrans.xz*zt + mTrans.xw;
	y = mTrans.yx*xt + mTrans.yy*yt + mTrans.yz*zt + mTrans.yw;
	z = mTrans.zx*xt + mTrans.zy*yt + mTrans.zz*zt + mTrans.zw;
	return *this;
}

/**
*  @brief
*    Transforms the vector
*/
Vector3 &Vector3::operator *=(const Matrix4x4 &mTrans)
{
	float xt = x, yt = y, zt = z;
	float fInvW = 1.0f/(mTrans.wx*xt + mTrans.wy*yt + mTrans.wz*zt + mTrans.ww);
	x = (mTrans.xx*xt + mTrans.xy*yt + mTrans.xz*zt + mTrans.xw)*fInvW;
	y = (mTrans.yx*xt + mTrans.yy*yt + mTrans.yz*zt + mTrans.yw)*fInvW;
	z = (mTrans.zx*xt + mTrans.zy*yt + mTrans.zz*zt + mTrans.zw)*fInvW;
	return *this;
}

/**
*  @brief
*    Rotates the vector
*/
Vector3 &Vector3::operator *=(const Quaternion &qQ)
{
	/*
	// [TODO] At the moment, Quaternion*Vector3 and Vector3*=Quaternion will return different results
	// ... *= returns for the skinned correct results, I still work on this :)
	Quaternion qTemp(qQ.w, -qQ.x, -qQ.y, -qQ.z);
	float qx = qTemp.x;
	float qy = qTemp.y;
	float qz = qTemp.z;
	float qw = qTemp.w;
	qTemp.x =  qw*x + qy*z - qz*y;
	qTemp.y =  qw*y - qx*z + qz*x;
	qTemp.z =  qw*z + qx*y - qy*x;
	qTemp.w = -qx*x - qy*y - qz*z;
	qTemp *= qQ;
	x = qTemp.x;
	y = qTemp.y;
	z = qTemp.z;


*/
	// [TODO] Check this!
	// For skinning...
	Quaternion qRes = (qQ.GetConjugated()*Quaternion(0.0f, x, y, z))*qQ;

	// Correct
//	Quaternion qRes = qQ*Quaternion(0.0f, x, y, z)*qQ.GetConjugated();
	x = qRes.x;
	y = qRes.y;
	z = qRes.z;

	return *this;
}

/**
*  @brief
*    Calculates the right/up vectors of the vector
*/
void Vector3::GetRightUp(Vector3 &vRight, Vector3 &vUp) const
{
	Vector3 vFN(Math::Abs(x), Math::Abs(y), Math::Abs(z));
	if (vFN.x > 1.0f)
		vFN.z = 1.0f;
	if (vFN.y > 1.0f)
		vFN.y = 1.0f;
	if (vFN.z > 1.0f)
		vFN.z = 1.0f;
	int nMajor = 0;

	// Find the major axis:
	if (vFN.y > vFN[nMajor])
		nMajor = 1;
	if (vFN.z > vFN[nMajor])
		nMajor = 2;

	// Build right vector by hand: (faster)
	if (vFN.x >= 1.0f-0.00001 || vFN.y >= 1.0f-0.00001 || vFN.z >= 1.0f-0.00001) {
		if (!nMajor && x > 0.0f)
			vRight.SetXYZ(0.0f, 0.0f, -1.0f);
		else if (!nMajor)
			vRight.SetXYZ(0.0f, 0.0f,  1.0f);

		if (nMajor == 1 || (nMajor == 2 && z > 0.0f))
			vRight.SetXYZ( 1.0f, 0.0f, 0.0f);
		if (nMajor == 2 && z < 0.0f)
			vRight.SetXYZ(-1.0f, 0.0f, 0.0f);
	} else {
		switch (nMajor) {
			case 0: vRight.CrossProduct(Vector3::UnitX, *this); break;
			case 1: vRight.CrossProduct(Vector3::UnitY, *this); break;
			case 2: vRight.CrossProduct(Vector3::UnitZ, *this); break;
		}
	}
	vUp.CrossProduct(*this, vRight);
	vRight.Normalize();
	vUp.Normalize();
}

/**
*  @brief
*    Calculates the right/up vectors of the vector
*/
void Vector3::GetRightUp(float fRight[], float fUp[]) const
{
	Vector3 vFN(Math::Abs(x), Math::Abs(y), Math::Abs(z));
	if (vFN.x > 1.0f)
		vFN.z = 1.0f;
	if (vFN.y > 1.0f)
		vFN.y = 1.0f;
	if (vFN.z > 1.0f)
		vFN.z = 1.0f;
	int nMajor = 0;

	// Find the major axis:
	if (vFN.y > vFN[nMajor])
		nMajor = 1;
	if (vFN.z > vFN[nMajor])
		nMajor = 2;

	// Build right vector by hand: (faster)
	Vector3 vRight = fRight;
	Vector3 vUp    = fUp;
	if (vFN.x == 1.0f || vFN.y == 1.0f || vFN.z == 1.0f) {
		if (!nMajor && x > 0.0f)
			vRight.SetXYZ(0.0f, 0.0f, -1.0f);
		else if (!nMajor)
			vRight.SetXYZ(0.0f, 0.0f,  1.0f);

		if (nMajor == 1 || (nMajor == 2 && z > 0.0f))
			vRight.SetXYZ( 1.0f, 0.0f, 0.0f);
		if (nMajor == 2 && z < 0.0f)
			vRight.SetXYZ(-1.0f, 0.0f, 0.0f);
	} else {
		switch (nMajor) {
			case 0: vRight.CrossProduct(Vector3::UnitX, *this); break;
			case 1: vRight.CrossProduct(Vector3::UnitY, *this); break;
			case 2: vRight.CrossProduct(Vector3::UnitZ, *this); break;
		}
	}
	vUp.CrossProduct(*this, vRight);
	vRight.Normalize();
	vUp.Normalize();

	fRight[0] = vRight.x;
	fRight[1] = vRight.y;
	fRight[2] = vRight.z;
	fUp[0] = vUp.x;
	fUp[1] = vUp.y;
	fUp[2] = vUp.z;
}

/**
*  @brief
*    Rotates two vectors around it's axis
*/
Vector3 &Vector3::RotateAxis(Vector3 &vV, float fAngle)
{
	float C = Math::Cos(fAngle);
	float S = Math::Sin(fAngle);
	Vector3 vTemp = (*this)*C + vV*S;
	vV    = (*this)*-S + vV*C;
	*this = vTemp; 

	return *this;
}

/**
*  @brief
*    Returns a rotation quaternion to the destination vector
*/
Quaternion Vector3::GetRotationTo(const Vector3 &vDest) const
{
	// Based on Stan Melax's article in Game Programming Gems
	Vector3 v0 = *this;
	Vector3 v1 = vDest;
	v0.Normalize();
	v1.Normalize();
	Vector3 c = v0.CrossProduct(v1);

	// If dot == 1, vectors are the same
	float d = v0.DotProduct(v1);
	if (d >= 1.0f)
		return Quaternion::Identity;
	float s = Math::Sqrt((1+d)*2);
	if (s < 1e-6f) {
		// Generate an axis
		Vector3 vAxis = Vector3::UnitX.CrossProduct(*this);
		if (vAxis.DotProduct(vAxis) < 1e-06f*1e-06f)
			vAxis = Vector3::UnitY.CrossProduct(*this); // Pick another if colinear
		vAxis.Normalize();

		// Build and return the quaternion
		Quaternion q;
		q.FromAxisAngle(vAxis.x, vAxis.y, vAxis.z, static_cast<float>(Math::Pi));
		return q;
	} else {
		float fInvS = 1/s;
		return Quaternion(s*0.5f, c.x*fInvS, c.y*fInvS, c.z*fInvS);
	}
}

/**
*  @brief
*    Calculates a normalized projection vector
*/
Vector3 &Vector3::GetProjection(const Vector3 &vX, const Vector3 &vN)
{
	*this = vX + vN*(-vN.DotProduct(vX)/vN.DotProduct(vN));
	SetLength(1.0f);

	return *this;
}

/**
*  @brief
*    Project the vector on to the plane created by two direction vectors
*/
Vector3 &Vector3::ProjectPlane(const Vector3 &vV1, const Vector3 &vV2)
{
	Vector3 vT1 = ProjectVector(Vector3::Zero, vV1);
	Vector3 vT2 = ProjectVector(Vector3::Zero, vV2);
	x = vT1.x + vT2.x;
	y = vT1.y + vT2.y;
	z = vT1.z + vT2.z;

	return *this;
}

/**
*  @brief
*    Calculates a reflection vector
*/
Vector3 &Vector3::Reflect(const Vector3 &vIncidentNormal, const Vector3 &vNormal)
{
	// First, calculate the dot product of N*I
	float fDotProduct = vNormal.x*vIncidentNormal.x + vNormal.y*vIncidentNormal.y + vNormal.z*vIncidentNormal.z;

	// R = I - 2*N*(N*I)
	x = vIncidentNormal.x - 2*vNormal.x*fDotProduct;
	y = vIncidentNormal.y - 2*vNormal.y*fDotProduct;
	z = vIncidentNormal.z - 2*vNormal.z*fDotProduct;

	return *this;
}

/**
*  @brief
*    Calculates a refraction vector
*/
Vector3 &Vector3::Refract(const Vector3 &vIncidentNormal, const Vector3 &vNormal, float fEtaRatio)
{
	float fCosAlpha       = -vIncidentNormal.x*vNormal.x + -vIncidentNormal.y*vNormal.y + -vIncidentNormal.z*vNormal.z; // Dot product
	float fCosBetaSquared = Math::Abs(1.0f - fEtaRatio*fEtaRatio*(1.0f - fCosAlpha*fCosAlpha));

	// *this = fEtaRatio*vIncidentNormal + (fEtaRatio*fCosAlpha - Math::Sqrt(fCosBetaSquared))*vNormal;
	// Optimized:
	float fInner = fEtaRatio*fCosAlpha - Math::Sqrt(fCosBetaSquared);
	x = fEtaRatio*vIncidentNormal.x + fInner*vNormal.x;
	y = fEtaRatio*vIncidentNormal.y + fInner*vNormal.y;
	z = fEtaRatio*vIncidentNormal.z + fInner*vNormal.z;

	return *this;
}

/**
*  @brief
*    Finds the closest point on a line to this point
*/
Vector3 Vector3::ClosestPointOnLine(const Vector3 &vV1, const Vector3 &vV2) const
{
	// Determine dT (the length of the vector from 'vV1' to 'vV2')
	Vector3 vV3 = *this - vV1;
	Vector3 vV  = vV2 - vV1;

	double dD = vV.GetLength();
	vV.Normalize();
	double dT = vV.DotProduct(vV3);

	// Check to see if 'dT' is beyond the extents of the line segment
	if (dT < 0.0f)
		return vV1;
	if (dT > dD)
		return vV2;

	// Return the point between 'vV1' and 'vV2'
	// set length of vV to dT. vV is normalized so this is easy
	vV *= static_cast<float>(dT);

	// Done
	return vV1 + vV;
}

/**
*  @brief
*    Check if a point is in a triangle
*/
bool Vector3::IsPointInTriangle(const Vector3 &vV1, const Vector3 &vV2, const Vector3 &vV3) const
{
	// Implementation of the example used in a paper (http://www.peroxide.dk/papers/collision/collision.pdf, page 34)
	// by Kasper Fauerby original written by Keidy from Mr-Gamemaker within a little competition about 'the fastest one'
	// (because this function 'really' becomes quite fast a bottleneck if must be as fast as possible)

	// Not optimized
//	const Vector3 e21 = vV2 - vV1;
//	const Vector3 e31 = vV3 - vV1;
	// Optimized (no temporal vector objects on the runtime stack)
	const Vector3 e21(vV2.x - vV1.x,
					  vV2.y - vV1.y,
					  vV2.z - vV1.z);
	const Vector3 e31(vV3.x - vV1.x,
					  vV3.y - vV1.y,
					  vV3.z - vV1.z);

	const float a     = e21.DotProduct(e21);
	const float b     = e21.DotProduct(e31);
	const float c     = e31.DotProduct(e31);
	const float ac_bb = a*c - b*b;

	const Vector3 vp(x - vV1.x, y - vV1.y, z - vV1.z);

	const float d = vp.DotProduct(e21);
	const float e = vp.DotProduct(e31);

	const float x = d*c - e*b;
	const float y = e*a - d*b;
	const float z = x + y - ac_bb;

	// Check if z<0, x>=0 and y>=0
	return ((((reinterpret_cast<const uint32&>(z))& ~((reinterpret_cast<const uint32&>(x))|(reinterpret_cast<const uint32&>(y)))) & 0x80000000) != 0);
}

/**
*  @brief
*    Find the closest point on a triangle to this point
*/
Vector3 Vector3::ClosestPointOnTriangle(const Vector3 &vV1, const Vector3 &vV2, const Vector3 &vV3) const
{
	Vector3 vRab = ClosestPointOnLine(vV1, vV2);
	Vector3 vRbc = ClosestPointOnLine(vV2, vV3);
	Vector3 vRca = ClosestPointOnLine(vV3, vV1);

	double dAB  = (*this - vRab).GetLength();
	double dBC  = (*this - vRbc).GetLength();
	double dCA  = (*this - vRca).GetLength();
	double dMin = dAB;

	Vector3 vResult = vRab;

	if (dBC < dMin) {
		dMin = dBC;
		vResult = vRbc;
	}

	if (dCA < dMin)
		vResult = vRca;

	// Done
	return vResult;
}

/**
*  @brief
*    Find the closest point on a triangle to this point
*/
bool Vector3::ClosestPointOnTriangle(const Vector3 &vV1, const Vector3 &vV2, const Vector3 &vV3,
									 float fRadius, Vector3 &vClosest) const
{
	// Make the plane containing this triangle
	Plane cPlane;
	cPlane.ComputeND(vV1, vV2, vV3);
	Vector3 vN(cPlane.fN[0], cPlane.fN[1], cPlane.fN[2]);

	// Find how far away the plane is from point vPointPos along the planes normal
	float fDistToPlaneIntersection = cPlane.GetDistance(*this, vN);
	if (fDistToPlaneIntersection > fRadius)
		return false;

	// Find the nearest point on the plane to this point
	vClosest = *this - vN*fDistToPlaneIntersection;

	// Determine if that point is in the triangle
	return vClosest.IsPointInTriangle(vV1, vV2, vV3);
}

/**
*  @brief
*    Returns the 2D screen coordinate corresponding to this 3D coordinate
*/
Vector3 Vector3::To2DCoordinate(const Matrix4x4 &mWorldViewProjection, const Rectangle &cViewportRectangle, float fDepthRangeMin, float fDepthRangeMax, float *pfHomogeneousDivide) const
{
	// Basing on: http://www.flipcode.com/archives/Object_To_Screen_Space.shtml from Morgan McGuire (http://www.cs.williams.edu/~morgan/)

	// Multiplicate the vector with the final matrix
	Vector4 vFinal = mWorldViewProjection*(*this);

	// Homogeneous divide
	const float fWInv = 1.f/vFinal.w;

	// Calculate the final values
	vFinal.x = (1 + vFinal.x*fWInv)*cViewportRectangle.GetWidth() /2 + cViewportRectangle.GetX();
	vFinal.y = (1 - vFinal.y*fWInv)*cViewportRectangle.GetHeight()/2 + cViewportRectangle.GetY();
	vFinal.z = (vFinal.z*fWInv)*(fDepthRangeMax - fDepthRangeMin) + fDepthRangeMin;
	if (pfHomogeneousDivide)
		*pfHomogeneousDivide = fWInv;

	// Done
	return vFinal;
}

/**
*  @brief
*    Returns the 3D coordinate corresponding to this 2D screen coordinate
*/
Vector3 Vector3::To3DCoordinate(const Matrix4x4 &mClipSpaceToObjectSpace, const Rectangle &cViewportRectangle) const
{
	// Get viewport parameters
	uint32 nX      = static_cast<uint32>(cViewportRectangle.GetX());
	uint32 nY      = static_cast<uint32>(cViewportRectangle.GetY());
	uint32 nWidth  = static_cast<uint32>(cViewportRectangle.GetWidth());
	uint32 nHeight = static_cast<uint32>(cViewportRectangle.GetHeight());

	// Flip the y component and map x and y from window coordinates
	Vector3 vPos(static_cast<float>((x - nX) / nWidth),
				 static_cast<float>(((-y + nHeight) - nY) / nHeight),
				 z);

	// Map to range -1 to 1
	vPos.x = vPos.x*2 - 1;
	vPos.y = vPos.y*2 - 1;
	vPos.z = vPos.z*2 - 1;

	// Done
	return mClipSpaceToObjectSpace*vPos;
}

/**
*  @brief
*    To string
*/
String Vector3::ToString() const
{
	return String::Format("%g %g %g", x, y, z);
}

/**
*  @brief
*    From string
*/
bool Vector3::FromString(const String &sString)
{
	if (sString.GetLength()) {
		// Parse components
		uint32 nParsePos  = 0;
		uint32 nComponent = 0;
		static RegEx cRegEx("\\s*(\\S+)");
		while (nComponent < 3 && cRegEx.Match(sString, nParsePos)) {
			fV[nComponent++] = cRegEx.GetResult(0).GetFloat();
			nParsePos = cRegEx.GetPosition();
		}

		// Set unused components to 0
		while (nComponent < 3)
			fV[nComponent++] = 0.0f;

		// Done
		return true;
	} else {
		// Error!
		x = y = z = 0.0f;
		return false;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
