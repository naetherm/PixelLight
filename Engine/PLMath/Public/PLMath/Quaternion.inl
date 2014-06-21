/*********************************************************\
 *  File: Quaternion.inl                                 *
 *      Quaternion inline implementation
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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Constructors                                          ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor setting an identity quaternion
*/
inline Quaternion::Quaternion() :
	w(1.0f),
	x(0.0f),
	y(0.0f),
	z(0.0f)
{
}

inline Quaternion::Quaternion(float fW, float fX, float fY, float fZ) :
	w(fW),
	x(fX),
	y(fY),
	z(fZ)
{
}

inline Quaternion::Quaternion(const float fV[]) :
	w(fV[0]),
	x(fV[1]),
	y(fV[2]),
	z(fV[3])
{
}

inline Quaternion::Quaternion(const Quaternion &qQ) :
	w(qQ.w),
	x(qQ.x),
	y(qQ.y),
	z(qQ.z)
{
}

inline Quaternion::Quaternion(const Vector3 &vAxis, float fAngle)
{
	FromAxisAngle(vAxis.x, vAxis.y, vAxis.z, fAngle);
}


//[-------------------------------------------------------]
//[ Destructor                                            ]
//[-------------------------------------------------------]
inline Quaternion::~Quaternion()
{
}


//[-------------------------------------------------------]
//[ Operators                                             ]
//[-------------------------------------------------------]
inline Quaternion Quaternion::operator +(const Quaternion &qQ) const
{
	return Quaternion(w+qQ.w, x+qQ.x, y+qQ.y, z+qQ.z);
}

inline Quaternion &Quaternion::operator +=(const Quaternion &qQ)
{
	w += qQ.w;
	x += qQ.x;
	y += qQ.y;
	z += qQ.z;
	return *this;
}

inline Quaternion Quaternion::operator -() const
{
	const float fNorm = w*w + x*x + y*y + z*z;
	if (fNorm > 0.0f) {
		const float fInvNorm = 1.0f/fNorm;
		return Quaternion(w*fInvNorm, -x*fInvNorm, -y*fInvNorm, -z*fInvNorm);
	} else {
		return Zero; // Error!
	}
}

inline Quaternion Quaternion::operator -(const Quaternion &qQ) const
{
	return Quaternion(w-qQ.w, x-qQ.x, y-qQ.y, z-qQ.z);
}

inline Quaternion &Quaternion::operator -=(const Quaternion &qQ)
{
	w -= qQ.w;
	x -= qQ.x;
	y -= qQ.y;
	z -= qQ.z;
	return *this;
}

inline Quaternion Quaternion::operator *(float f) const
{
	return Quaternion(w*f, x*f, y*f, z*f);
}

inline Quaternion &Quaternion::operator *=(float f)
{
	w *= f;
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

inline Quaternion Quaternion::operator *(const Quaternion &qQ) const
{
	return Quaternion(w*qQ.w - x*qQ.x - y*qQ.y - z*qQ.z,
					  w*qQ.x + x*qQ.w + y*qQ.z - z*qQ.y,
					  w*qQ.y + y*qQ.w + z*qQ.x - x*qQ.z,
					  w*qQ.z + z*qQ.w + x*qQ.y - y*qQ.x);
}

inline Quaternion &Quaternion::operator *=(const Quaternion &qQ)
{
	const float qw = w, qx = x, qy = y, qz = z;
	w = qw*qQ.w - qx*qQ.x - qy*qQ.y - qz*qQ.z;
	x = qw*qQ.x + qx*qQ.w + qy*qQ.z - qz*qQ.y;
	y = qw*qQ.y + qy*qQ.w + qz*qQ.x - qx*qQ.z;
	z = qw*qQ.z + qz*qQ.w + qx*qQ.y - qy*qQ.x;
	return *this;
}

inline Vector3 Quaternion::operator *(const Vector3 &vV) const
{
// Classic
//	const Quaternion qRes = (*this)*Quaternion(0.0f, vV.x, vV.y, vV.z)*GetConjugated();
//	return Vector3(qRes.x, qRes.y, qRes.z);

// Optimized
	const float x2 = x*x;
	const float y2 = y*y;
	const float z2 = z*z;
	const float w2 = w*w;
	const float xa = x*vV.x;
	const float yb = y*vV.y;
	const float zc = z*vV.z;
	return Vector3(vV.x*( x2 - y2 - z2 + w2) + 2*(w*(y*vV.z - z*vV.y) + x*(yb + zc)),
				   vV.y*(-x2 + y2 - z2 + w2) + 2*(w*(z*vV.x - x*vV.z) + y*(xa + zc)),
				   vV.z*(-x2 - y2 + z2 + w2) + 2*(w*(x*vV.y - y*vV.x) + z*(xa + yb)));
}

inline Vector4 Quaternion::operator *(const Vector4 &vV) const
{
// Classic
//	const Quaternion qRes = (*this)*Quaternion(0.0f, vV.x, vV.y, vV.z)*GetConjugated();
//	return Vector4(qRes.x, qRes.y, qRes.z, 1.0f);

// Optimized
	const float x2 = x*x;
	const float y2 = y*y;
	const float z2 = z*z;
	const float w2 = w*w;
	const float xa = x*vV.x;
	const float yb = y*vV.y;
	const float zc = z*vV.z;
	return Vector4(vV.x*( x2 - y2 - z2 + w2) + 2*(w*(y*vV.z - z*vV.y) + x*(yb + zc)),
				   vV.y*(-x2 + y2 - z2 + w2) + 2*(w*(z*vV.y - x*vV.z) + y*(xa + zc)),
				   vV.z*(-x2 - y2 + z2 + w2) + 2*(w*(x*vV.y - y*vV.x) + z*(xa + yb)),
				   1.0f);
}

inline Quaternion Quaternion::operator /(float f) const
{
	return Quaternion(w/f, x/f, y/f, z/f);
}

inline Quaternion &Quaternion::operator /=(float f)
{
	w /= f;
	x /= f;
	y /= f;
	z /= f;
	return *this;
}

inline Quaternion &Quaternion::operator =(const Quaternion &qQ)
{
	w = qQ.w;
	x = qQ.x;
	y = qQ.y;
	z = qQ.z;
	return *this;
}

inline bool Quaternion::operator ==(const Quaternion &qQ) const
{
	return (w == qQ.w && x == qQ.x && y == qQ.y && z == qQ.z);
}

inline bool Quaternion::operator !=(const Quaternion &qQ) const
{
	return (w != qQ.w || x != qQ.x || y != qQ.y || z != qQ.z);
}

inline Quaternion::operator float *()
{
	return fQ;
}

inline Quaternion::operator const float *() const
{
	return fQ;
}


//[-------------------------------------------------------]
//[ Get and set functions                                 ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets the component of the quaternion
*/
inline Quaternion &Quaternion::SetWXYZ(float fW, float fX, float fY, float fZ)
{
	w = fW;
	x = fX;
	y = fY;
	z = fZ;
	return *this;
}

/**
*  @brief
*    Sets the component of the quaternion
*/
inline Quaternion &Quaternion::SetWXYZ(const float fValues[])
{
	w = fValues[0];
	x = fValues[1];
	y = fValues[2];
	z = fValues[3];
	return *this;
}


//[-------------------------------------------------------]
//[ Misc                                                  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Compares two quaternions using an epsilon environment
*/
inline bool Quaternion::Compare(const Quaternion &qQ, float fEpsilon) const
{
	return (Math::AreEqual(Math::Abs(w-qQ.w), fEpsilon) &&
			Math::AreEqual(Math::Abs(x-qQ.x), fEpsilon) &&
			Math::AreEqual(Math::Abs(y-qQ.y), fEpsilon) &&
			Math::AreEqual(Math::Abs(z-qQ.z), fEpsilon));
}

/**
*  @brief
*    Returns the length (also called magnitude) of the quaternion
*/
inline float Quaternion::GetLength() const
{
	return Math::Sqrt(w*w + x*x + y*y + z*z);
}

/**
*  @brief
*    Returns the squared length (norm) of the quaternion
*/
inline float Quaternion::GetSquaredLength() const
{
	return w*w + x*x + y*y + z*z;
}

/**
*  @brief
*    Returns the dot product of this quaternion and another one
*/
inline float Quaternion::DotProduct(const Quaternion &qQ) const
{
	return w*qQ.w + x*qQ.x + y*qQ.y + z*qQ.z;
}

/**
*  @brief
*    Normalizes the vector
*/
inline Quaternion &Quaternion::Normalize()
{
	// Avoid division through zero...
	float fU = w*w + x*x + y*y + z*z;
	if (fU) {
		fU = Math::Sqrt(fU);
		if (fU) {
			// Scale
			const float fFactor = 1.0f/fU;
			w *= fFactor;
			x *= fFactor;
			y *= fFactor;
			z *= fFactor;
		}
	}

	return *this;
}

/**
*  @brief
*    Returns a normalized quaternion
*/
inline Quaternion Quaternion::GetNormalized() const
{
	// Avoid division through zero...
	float fU = w*w + x*x + y*y + z*z;
	if (fU) {
		fU = Math::Sqrt(fU);
		if (fU) {
			// Scale
			const float fFactor = 1.0f/fU;
			return Quaternion(w*fFactor, x*fFactor, y*fFactor, z*fFactor);
		}
	}

	// Error!
	return Zero;
}

/**
*  @brief
*    Conjugates the quaternion
*/
inline void Quaternion::Conjugate()
{
	x = -x;
	y = -y;
	z = -z;
}

/**
*  @brief
*    Returns the conjugated of the quaternion
*/
inline Quaternion Quaternion::GetConjugated() const
{
	return Quaternion(w, -x, -y, -z);
}

/**
*  @brief
*    Inverts the quaternion
*/
inline void Quaternion::Invert()
{
	const float fNorm = w*w + x*x + y*y + z*z;
	if (fNorm > 0.0f) {
		const float fInvNorm = 1.0f/fNorm;
		w =  w*fInvNorm;
		x = -x*fInvNorm;
		y = -y*fInvNorm;
		z = -z*fInvNorm;
	} else { // Error!
		x = y = z = w = 0.0f;
	}
}

/**
*  @brief
*    Returns the inverted quaternion
*/
inline Quaternion Quaternion::GetInverted() const
{
	const float fNorm = w*w + x*x + y*y + z*z;
	if (fNorm > 0.0f) {
		const float fInvNorm = 1.0f/fNorm;
		return Quaternion(w*fInvNorm, -x*fInvNorm, -y*fInvNorm, -z*fInvNorm);
	} else {
		return Zero; // Error!
	}
}

/**
*  @brief
*    Inverts the normalized quaternion
*/
inline void Quaternion::UnitInvert()
{
	// w is not touched :)
	// w = w;
	x = -x;
	y = -y;
	z = -z;
}

/**
*  @brief
*    Returns the inverted normalized quaternion
*/
inline Quaternion Quaternion::GetUnitInverted() const
{
	return Quaternion(w, -x, -y, -z);
}

/**
*  @brief
*    Calculates the exponential of the quaternion
*/
inline Quaternion &Quaternion::Exp()
{
	// Avoid division through zero...
	float fOm = x*x + y*y + z*z;
	if (fOm) {
		fOm = Math::Sqrt(fOm);
		if (fOm) {
			const float fSinom = Math::Sin(fOm)/fOm;
			w = Math::Cos(fOm);
			x *= fSinom;
			y *= fSinom;
			z *= fSinom;
		}
	}

	return *this;
}

/**
*  @brief
*    Returns the exponential of the quaternion
*/
inline Quaternion Quaternion::GetExp() const
{
	// Avoid division through zero...
	float fOm = x*x + y*y + z*z;
	if (fOm) {
		fOm = Math::Sqrt(fOm);
		if (fOm) {
			const float fSinom = Math::Sin(fOm)/fOm;
			return Quaternion(Math::Cos(fOm), x*fSinom, y*fSinom, z*fSinom);
		}
	}

	// Fallback...
	return Quaternion(Math::Cos(fOm), x, y, z);
}

/*
*  @brief
*    Calculates the logarithm of a unit quaternion
*/
inline Quaternion &Quaternion::Log()
{
	const float fTheta    = Math::ACos(w);
	const float fSinTheta = Math::Sin(fTheta);
	if (Math::Abs(fSinTheta) > 0.0f) {
		w = 0.0f;
		x = x/fSinTheta*fTheta;
		y = y/fSinTheta*fTheta;
		z = z/fSinTheta*fTheta;
	}

	return *this;
}

/*
*  @brief
*    Calculates the logarithm of a unit quaternion
*/
inline Quaternion Quaternion::GetLog() const
{
	const float fTheta    = Math::ACos(w);
	const float fSinTheta = Math::Sin(fTheta);
	if (Math::Abs(fSinTheta) > 0.0f)
		return Quaternion(0.0f, x/fSinTheta*fTheta, y/fSinTheta*fTheta, z/fSinTheta*fTheta);
	else
		return Quaternion(0.0f, x,                  y,                  z);
}

/**
*  @brief
*    Calculates the power of the quaternion
*/
inline Quaternion &Quaternion::Power(float fPower)
{
	Log();
	w *= fPower;
	x *= fPower;
	x *= fPower;
	y *= fPower;
	Exp();

	return *this;
}

/**
*  @brief
*    Returns the power of the quaternion
*/
inline Quaternion Quaternion::GetPower(float fPower) const
{
	return (GetLog()*fPower).GetExp();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
