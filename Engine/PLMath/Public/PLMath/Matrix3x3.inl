/*********************************************************\
 *  File: Matrix3x3.inl                                  *
 *      Matrix3x3 inline implementation
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
//[ Constructor                                           ]
//[-------------------------------------------------------]
inline Matrix3x3::Matrix3x3() :
	// Set identity - and respect the correct initialization order!
	xx(1.0f), yx(0.0f), zx(0.0f),
	xy(0.0f), yy(1.0f), zy(0.0f),
	xz(0.0f), yz(0.0f), zz(1.0f)
{
}

inline Matrix3x3::Matrix3x3(const float fS[])
{
	PLCore::MemoryManager::Copy(this->fM, fS, sizeof(float)*9);
}

inline Matrix3x3::Matrix3x3(const Matrix3x3 &mM)
{
	PLCore::MemoryManager::Copy(this->fM, mM.fM, sizeof(float)*9);
}

inline Matrix3x3::Matrix3x3(float fXX, float fXY, float fXZ,
							float fYX, float fYY, float fYZ,
							float fZX, float fZY, float fZZ) :
	// Copy over the values - and respect the correct initialization order!
	xx(fXX), yx(fYX), zx(fZX),
	xy(fXY), yy(fYY), zy(fZY),
	xz(fXZ), yz(fYZ), zz(fZZ)
{
}


//[-------------------------------------------------------]
//[ Destructor                                            ]
//[-------------------------------------------------------]
inline Matrix3x3::~Matrix3x3()
{
}


//[-------------------------------------------------------]
//[ Operators                                             ]
//[-------------------------------------------------------]
inline Matrix3x3 &Matrix3x3::operator =(const float fS[])
{
	PLCore::MemoryManager::Copy(this->fM, fS, sizeof(float)*9);
	return *this;
}

inline Matrix3x3 &Matrix3x3::operator =(const Matrix3x3 &mM)
{
	PLCore::MemoryManager::Copy(this->fM, mM.fM, sizeof(float)*9);
	return *this;
}

inline Matrix3x3 Matrix3x3::operator +(const Matrix3x3 &mM) const
{
	return Matrix3x3(xx+mM.xx, xy+mM.xy, xz+mM.xz,
					 yx+mM.yx, yy+mM.yy, yz+mM.yz,
					 zx+mM.zx, zy+mM.zy, zz+mM.zz);
}

inline void Matrix3x3::operator +=(const Matrix3x3 &mM)
{
	xx += mM.xx; xy += mM.xy; xz += mM.xz;
	yx += mM.yx; yy += mM.yy; yz += mM.yz;
	zx += mM.zx; zy += mM.zy; zz += mM.zz;
}

inline Matrix3x3 Matrix3x3::operator -() const
{
	return GetInverted();
}

inline Matrix3x3 Matrix3x3::operator -(const Matrix3x3 &mM) const
{
	return Matrix3x3(xx-mM.xx, xy-mM.xy, xz-mM.xz,
					 yx-mM.yx, yy-mM.yy, yz-mM.yz,
					 zx-mM.zx, zy-mM.zy, zz-mM.zz);
}

inline void Matrix3x3::operator -=(const Matrix3x3 &mM)
{
	xx -= mM.xx; xy -= mM.xy; xz -= mM.xz;
	yx -= mM.yx; yy -= mM.yy; yz -= mM.yz;
	zx -= mM.zx; zy -= mM.zy; zz -= mM.zz;
}

inline Matrix3x3 Matrix3x3::operator *(float fS) const
{
	return Matrix3x3(xx*fS, xy*fS, xz*fS,
					 yx*fS, yy*fS, yz*fS,
					 zx*fS, zy*fS, zz*fS);
}

inline void Matrix3x3::operator *=(float fS)
{
	xx *= fS; xy *= fS; xz *= fS;
	yx *= fS; yy *= fS; yz *= fS;
	zx *= fS; zy *= fS; zz *= fS;
}

inline Vector2 Matrix3x3::operator *(const Vector2 &vV) const
{
	const float x = vV.x, y = vV.y;
	return Vector2(xx*x + xy*y,
				   yx*x + yy*y);
}

inline Vector3 Matrix3x3::operator *(const Vector3 &vV) const
{
	const float x = vV.x, y = vV.y, z = vV.z;
	return Vector3(xx*x + xy*y + xz*z,
				   yx*x + yy*y + yz*z,
				   zx*x + zy*y + zz*z);
}

inline void Matrix3x3::operator *=(const Matrix3x3 &mM)
{
	*this = *this * mM;
}

inline Matrix3x3 Matrix3x3::operator /(float fS) const
{
	return Matrix3x3(xx/fS, xy/fS, xz/fS,
					 yx/fS, yy/fS, yz/fS,
					 zx/fS, zy/fS, zz/fS);
}

inline void Matrix3x3::operator /=(float fS)
{
	xx /= fS; xy /= fS; xz /= fS;
	yx /= fS; yy /= fS; yz /= fS;
	zx /= fS; zy /= fS; zz /= fS;
}

inline float Matrix3x3::operator [](int nIndex) const
{
	return fM[nIndex];
}

inline float &Matrix3x3::operator [](int nIndex)
{
	return fM[nIndex];
}

inline float Matrix3x3::operator ()(PLCore::uint32 nRow, PLCore::uint32 nColumn) const
{
	return fM[nRow+3*nColumn];
}

inline float &Matrix3x3::operator ()(PLCore::uint32 nRow, PLCore::uint32 nColumn)
{
	return fM[nRow+3*nColumn];
}

inline Matrix3x3::operator float *()
{
	return this->fM;
}

inline Matrix3x3::operator const float *() const
{
	return this->fM;
}


//[-------------------------------------------------------]
//[ Matrix operations                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns whether or not this matrix is the zero matrix using an epsilon environment
*/
inline bool Matrix3x3::IsZero() const
{
	return (Math::AreEqual(xx, 0.0f) && Math::AreEqual(xy, 0.0f) && Math::AreEqual(xz, 0.0f) &&
			Math::AreEqual(yx, 0.0f) && Math::AreEqual(yy, 0.0f) && Math::AreEqual(yz, 0.0f) &&
			Math::AreEqual(zx, 0.0f) && Math::AreEqual(zy, 0.0f) && Math::AreEqual(zz, 0.0f));
}

/**
*  @brief
*    Returns whether or not this matrix is truly the zero matrix
*/
inline bool Matrix3x3::IsTrueZero() const
{
	return (PLCore::MemoryManager::Compare(this->fM, Zero.fM, sizeof(float)*9) == 0);
}

/**
*  @brief
*    Sets a zero matrix
*/
inline void Matrix3x3::SetZero()
{
	PLCore::MemoryManager::Set(this->fM, 0, sizeof(float)*9);
}

/**
*  @brief
*    Returns whether or not this matrix is the identity matrix using an epsilon environment
*/
inline bool Matrix3x3::IsIdentity() const
{
	return (Math::AreEqual(xx, 1.0f) && Math::AreEqual(xy, 0.0f) && Math::AreEqual(xz, 0.0f) &&
			Math::AreEqual(yx, 0.0f) && Math::AreEqual(yy, 1.0f) && Math::AreEqual(yz, 0.0f) &&
			Math::AreEqual(zx, 0.0f) && Math::AreEqual(zy, 0.0f) && Math::AreEqual(zz, 1.0f));
}

/**
*  @brief
*    Returns whether or not this matrix is truly the identity matrix
*/
inline bool Matrix3x3::IsTrueIdentity() const
{
	return (PLCore::MemoryManager::Compare(this->fM, Identity.fM, sizeof(float)*9) == 0);
}

/**
*  @brief
*    Sets an identity matrix
*/
inline void Matrix3x3::SetIdentity()
{
	PLCore::MemoryManager::Copy(this->fM, Identity.fM, sizeof(float)*9);
}

/**
*  @brief
*    Sets the elements of the matrix
*/
inline void Matrix3x3::Set(float fXX, float fXY, float fXZ,
						   float fYX, float fYY, float fYZ,
						   float fZX, float fZY, float fZZ)
{
	xx = fXX; xy = fXY; xz = fXZ;
	yx = fYX; yy = fYY; yz = fYZ;
	zx = fZX; zy = fZY; zz = fZZ;
}

/**
*  @brief
*    Returns a requested row
*/
inline Vector3 Matrix3x3::GetRow(PLCore::uint8 nRow) const
{
	switch (nRow) {
		case 0:  return Vector3(xx, xy, xz);
		case 1:  return Vector3(yx, yy, yz);
		case 2:  return Vector3(zx, zy, zz);
		default: return Vector3::Zero; // Error!
	}
}

/**
*  @brief
*    Sets a row
*/
inline void Matrix3x3::SetRow(PLCore::uint8 nRow, const Vector3 &vRow)
{
	switch (nRow) {
		case 0: xx = vRow.x; xy = vRow.y; xz = vRow.z; break;
		case 1: yx = vRow.x; yy = vRow.y; yz = vRow.z; break;
		case 2: zx = vRow.x; zy = vRow.y; zz = vRow.z; break;
	}
}

/**
*  @brief
*    Returns a requested column
*/
inline Vector3 Matrix3x3::GetColumn(PLCore::uint8 nColumn) const
{
	switch (nColumn) {
		case 0:  return Vector3(xx, yx, zx);
		case 1:  return Vector3(xy, yy, zy);
		case 2:  return Vector3(xz, yz, zz);
		default: return Vector3::Zero; // Error!
	}
}

/**
*  @brief
*    Sets a column
*/
inline void Matrix3x3::SetColumn(PLCore::uint8 nColumn, const Vector3 &vColumn)
{
	switch (nColumn) {
		case 0: xx = vColumn.x; yx = vColumn.y; zx = vColumn.z; break;
		case 1: xy = vColumn.x; yy = vColumn.y; zy = vColumn.z; break;
		case 2: xz = vColumn.x; yz = vColumn.y; zz = vColumn.z; break;
	}
}

/**
*  @brief
*    Returns true if the matrix is symmetric
*/
inline bool Matrix3x3::IsSymmetric() const
{
	return (Math::AreEqual(xy, yx) && Math::AreEqual(xz, zx) && Math::AreEqual(yz, zy));
}

/**
*  @brief
*    Returns true if this matrix is orthogonal
*/
inline bool Matrix3x3::IsOrthogonal() const
{
	return (GetTransposed() == GetInverted());
}

/**
*  @brief
*    Returns true if this matrix is a rotation matrix
*/
inline bool Matrix3x3::IsRotationMatrix() const
{
	return (GetTransposed() == GetInverted() && Math::AreEqual(GetDeterminant(), 1.0f));
}

/**
*  @brief
*    Returns the trace of the matrix
*/
inline float Matrix3x3::GetTrace() const
{
	return xx+yy+zz;
}

/**
*  @brief
*    Returns the determinant of the matrix
*/
inline float Matrix3x3::GetDeterminant() const
{
	// 'Classic' (Rule of Sarrus)
//	return xx*yy*zz + xy*yz*zx + xz*yx*zy - (zx*yy*xz + zy*yz*xx + zz*yx*xy);
	// Faster :)
	return xx*(yy*zz - yz*zy) - yx*(xy*zz - xz*zy) + zx*(xy*yz - xz*yy);
}

/**
*  @brief
*    Transpose this matrix
*/
inline void Matrix3x3::Transpose()
{
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
}

/**
*  @brief
*    Returns the transposed matrix
*/
inline Matrix3x3 Matrix3x3::GetTransposed() const
{
	return Matrix3x3(xx, yx, zx,
					 xy, yy, zy,
					 xz, yz, zz);
}


//[-------------------------------------------------------]
//[ Scale                                                 ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets a scale matrix
*/
inline void Matrix3x3::SetScaleMatrix(float fX, float fY, float fZ)
{
	xx =   fX; xy = 0.0f; xz = 0.0f;
	yx = 0.0f; yy =   fY; yz = 0.0f;
	zx = 0.0f; zy = 0.0f; zz =   fZ;
}

inline void Matrix3x3::SetScaleMatrix(const Vector3 &vV)
{
	xx = vV.x; xy = 0.0f; xz = 0.0f;
	yx = 0.0f; yy = vV.y; yz = 0.0f;
	zx = 0.0f; zy = 0.0f; zz = vV.z;
}

inline Vector3 Matrix3x3::GetScale() const
{
	Vector3 vScale;
	GetScale(vScale.x, vScale.y, vScale.z);
	return vScale;
}

inline void Matrix3x3::GetScale(float fV[]) const
{
	GetScale(fV[0], fV[1], fV[2]);
}


//[-------------------------------------------------------]
//[ Rotation                                              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the x (left) axis
*/
inline Vector3 Matrix3x3::GetXAxis() const
{
	return Vector3(xx, yx, zx);
}

/**
*  @brief
*    Returns the y (up) axis
*/
inline Vector3 Matrix3x3::GetYAxis() const
{
	return Vector3(xy, yy, zy);
}

/**
*  @brief
*    Returns the z (forward) axis
*/
inline Vector3 Matrix3x3::GetZAxis() const
{
	return Vector3(xz, yz, zz);
}


//[-------------------------------------------------------]
//[ Misc                                                  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets a shearing matrix
*/
inline Matrix3x3 &Matrix3x3::SetShearing(float fShearXY, float fShearXZ, float fShearYX, float fShearYZ,
										float fShearZX, float fShearZY)
{
	xx = 1.0f;     xy = fShearYX; xz = fShearZX;
	yx = fShearXY; yy = 1.0f;     yz = fShearZY;
	zx = fShearXZ; zy = fShearYZ; zz = 1.0f;
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
