/*********************************************************\
 *  File: Matrix3x3.h                                    *
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


#ifndef __PLMATH_MATRIX3X3_H__
#define __PLMATH_MATRIX3X3_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLMath/Vector2.h"
#include "PLMath/Vector4.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    3x3 matrix
*
*  @remarks
*    Matrices are stored in column major order like OpenGL does. (right-handed matrix)
*    So, it's possible to give OpenGL the matrix data without transposing it first.
*
*  @note
*    - Some symbols within the comments: T = transposed, I = identity
*/
class Matrix3x3 {


	//[-------------------------------------------------------]
	//[ Public static data                                    ]
	//[-------------------------------------------------------]
	public:
		static PLMATH_API const Matrix3x3 Zero;		/**< Zero matrix */
		static PLMATH_API const Matrix3x3 Identity;	/**< Identity matrix */


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Some direct matrix accesses
		*/
		union {
			/**
			*  @brief
			*    One dimensional array representation
			*
			*  @remarks
			*    The entry of the matrix in row r (0 <= r <= 2) and column c (0 <= c <= 2) is
			*    stored at index i = r+3*c (0 <= i <= 8).\n
			*    Indices:
			*    @code
			*      | 0 3 6 |
			*      | 1 4 7 |
			*      | 2 5 8 |
			*    @endcode
			*/
			float fM[9];

			/**
			*  @brief
			*    Direct element representation
			*
			*  @remarks
			*    Indices: (row/column)
			*    @code
			*      | xx xy xz |
			*      | yx yy yz |
			*      | zx zy zz |
			*    @endcode
			*    It's recommended to use this way to access the elements.
			*/
			struct {
				float xx, yx, zx;
				float xy, yy, zy;
				float xz, yz, zz;
			};

			/**
			*  @brief
			*    Two dimensional array representation
			*
			*  @remarks
			*    fM33[i][j] -> i=column, j=row\n
			*    Try to avoid this access mode. This can be a problem on a platform/console that
			*    chooses to store the data in column-major rather than row-major format.
			*/
			struct {
				float fM33[3][3];
			};
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Constructor                                           ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Default constructor setting an identity matrix
		*/
		inline Matrix3x3();

		inline Matrix3x3(const float fS[]);
		inline Matrix3x3(const Matrix3x3 &mM);
		PLMATH_API Matrix3x3(const Matrix3x4 &mM);
		PLMATH_API Matrix3x3(const Matrix4x4 &mM);
		inline Matrix3x3(float fXX, float fXY, float fXZ,
						 float fYX, float fYY, float fYZ,
						 float fZX, float fZY, float fZZ);

		//[-------------------------------------------------------]
		//[ Destructor                                            ]
		//[-------------------------------------------------------]
		inline ~Matrix3x3();

		//[-------------------------------------------------------]
		//[ Comparison                                            ]
		//[-------------------------------------------------------]
		PLMATH_API bool operator ==(const Matrix3x3 &mM) const;
		PLMATH_API bool operator !=(const Matrix3x3 &mM) const;
		// fEpsilon = epsilon environment (to take computation errors into account...)
		PLMATH_API bool CompareScale(const Matrix3x3 &mM, float fEpsilon = Math::Epsilon) const;
		PLMATH_API bool CompareRotation(const Matrix3x3 &mM, float fEpsilon = Math::Epsilon) const;

		//[-------------------------------------------------------]
		//[ Operators                                             ]
		//[-------------------------------------------------------]
		inline Matrix3x3     &operator =(const float fS[]);
		inline Matrix3x3     &operator =(const Matrix3x3 &mM);
		PLMATH_API Matrix3x3 &operator =(const Matrix3x4 &mM);
		PLMATH_API Matrix3x3 &operator =(const Matrix4x4 &mM);
		inline Matrix3x3      operator +(const Matrix3x3 &mM) const;
		inline void           operator +=(const Matrix3x3 &mM);
		inline Matrix3x3      operator -() const;
		inline Matrix3x3      operator -(const Matrix3x3 &mM) const;
		inline void           operator -=(const Matrix3x3 &mM);
		inline Matrix3x3      operator *(float fS) const;
		inline void           operator *=(float fS);
		inline Vector2        operator *(const Vector2 &vV) const;
		inline Vector3        operator *(const Vector3 &vV) const;
		PLMATH_API Vector4    operator *(const Vector4 &vV) const;
		PLMATH_API Matrix3x3  operator *(const Matrix3x3 &mM) const;
		inline void           operator *=(const Matrix3x3 &mM);
		inline Matrix3x3      operator /(float fS) const;
		inline void           operator /=(float fS);
		inline float          operator [](int nIndex) const;
		inline float         &operator [](int nIndex);
		inline float          operator ()(PLCore::uint32 nRow = 0, PLCore::uint32 nColumn = 0) const;
		inline float         &operator ()(PLCore::uint32 nRow = 0, PLCore::uint32 nColumn = 0);
		inline                operator float *();
		inline                operator const float *() const;

		//[-------------------------------------------------------]
		//[ Matrix operations                                     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns whether or not this matrix is the zero matrix using an epsilon environment
		*
		*  @return
		*    'true' if this matrix is the zero matrix, else 'false'
		*/
		inline bool IsZero() const;

		/**
		*  @brief
		*    Returns whether or not this matrix is truly the zero matrix
		*
		*  @remarks
		*    All components MUST be exactly 0. Floating point inaccuracy
		*    is not taken into account.
		*
		*  @return
		*    'true' if this matrix is truly the zero matrix, else 'false'
		*/
		inline bool IsTrueZero() const;

		/**
		*  @brief
		*    Sets a zero matrix
		*
		*  @remarks
		*    @code
		*    | 0 0 0 |
		*    | 0 0 0 |
		*    | 0 0 0 |
		*    @endcode
		*/
		inline void SetZero();

		/**
		*  @brief
		*    Returns whether or not this matrix is the identity matrix using an epsilon environment
		*
		*  @return
		*    'true' if this matrix is the identity matrix, else 'false'
		*/
		inline bool IsIdentity() const;

		/**
		*  @brief
		*    Returns whether or not this matrix is truly the identity matrix
		*
		*  @remarks
		*    All components MUST be exactly either 0 or 1. Floating point inaccuracy
		*    is not taken into account.
		*
		*  @return
		*    'true' if this matrix is truly the identity matrix, else 'false'
		*/
		inline bool IsTrueIdentity() const;

		/**
		*  @brief
		*    Sets an identity matrix
		*
		*  @remarks
		*    @code
		*    | 1 0 0 |
		*    | 0 1 0 |
		*    | 0 0 1 |
		*    @endcode
		*/
		inline void SetIdentity();

		/**
		*  @brief
		*    Sets the elements of the matrix
		*/
		inline void Set(float fXX, float fXY, float fXZ,
						float fYX, float fYY, float fYZ,
						float fZX, float fZY, float fZZ);

		/**
		*  @brief
		*    Returns a requested row
		*
		*  @param[in] nRow
		*    Index of the row to return (0-2)
		*
		*  @return
		*    The requested row (null vector on error)
		*
		*  @remarks
		*    @code
		*    | x y z | <- Row 0
		*    | 0 0 0 |
		*    | 0 0 0 |
		*    @endcode
		*/
		inline Vector3 GetRow(PLCore::uint8 nRow) const;

		/**
		*  @brief
		*    Sets a row
		*
		*  @param[in] nRow
		*    Index of the row to set (0-2)
		*  @param[in] vRow
		*    Row vector
		*
		*  @see
		*    - GetRow()
		*/
		inline void SetRow(PLCore::uint8 nRow, const Vector3 &vRow);

		/**
		*  @brief
		*    Returns a requested column
		*
		*  @param[in] nColumn
		*    Index of the column to return (0-2)
		*
		*  @return
		*    The requested column (null vector on error)
		*
		*  @remarks
		*    @code
		*    | x 0 0 |
		*    | y 0 0 |
		*    | z 0 0 |
		*      ^
		*      |
		*      Column 0
		*    @endcode
		*/
		inline Vector3 GetColumn(PLCore::uint8 nColumn) const;

		/**
		*  @brief
		*    Sets a column
		*
		*  @param[in] nColumn
		*    Index of the column to set (0-2)
		*  @param[in] vColumn
		*    Column vector
		*
		*  @see
		*    - GetColumn()
		*/
		inline void SetColumn(PLCore::uint8 nColumn, const Vector3 &vColumn);

		/**
		*  @brief
		*    Returns true if the matrix is symmetric
		*
		*  @return
		*    'true' if the matrix is symmetric, else 'false'
		*
		*  @remarks
		*    A matrix is symmetric if it is equal to it's transposed matrix.\n
		*    A = A^T  ->  a(i, j) = a(j, i)
		*/
		inline bool IsSymmetric() const;

		/**
		*  @brief
		*    Returns true if this matrix is orthogonal
		*
		*  @return
		*    'true' if the matrix is orthogonal, else 'false'
		*
		*  @remarks
		*    A matrix is orthogonal if it's transposed matrix is equal to it's inversed matrix.\n
		*    A^T = A^-1  or  A*A^T = A^T*A = I
		*
		*  @note
		*    - An orthogonal matrix is always nonsingular (invertible) and it's inverse is equal to it's transposed
		*    - The transpose and inverse of the matrix is orthogonal, too
		*    - Products of orthogonal matrices are orthogonal, too
		*    - The determinant of a orthogonal matrix is +/- 1
		*    - The row and column vectors of an orthogonal matrix form an orthonormal basis,
		*      that is, these vectors are unit-length and they are mutually perpendicular
		*/
		inline bool IsOrthogonal() const;

		/**
		*  @brief
		*    Returns true if this matrix is a rotation matrix
		*
		*  @return
		*    'true' if this matrix is a rotation matrix, else 'false'
		*
		*  @remarks
		*    A rotation matrix is orthogonal and it's determinant is 1 to rule out reflections.
		*
		*  @see
		*    - IsOrthogonal()
		*/
		inline bool IsRotationMatrix() const;

		/**
		*  @brief
		*    Returns the trace of the matrix
		*
		*  @return
		*    The trace of the matrix
		*
		*  @remarks
		*    The trace of the matrix is the sum of the main diagonal elements:\n
		*      xx+yy+zz
		*/
		inline float GetTrace() const;

		/**
		*  @brief
		*    Returns the determinant of the matrix
		*
		*  @return
		*    Determinant of the matrix
		*
		*  @remarks
		*    The determinant is calculated using the Sarrus diagram:
		*    @code
		*      | xx   xy   xz  |  xx   xy |
		*      |     \   /\   /\     /    |
		*      | yx   yy   yz  |  yx   yy | =  xx*yy*zz + xy*yz*zx + xz*yx*zy -
		*      |    /    /\   /\      \   |   (zx*yy*xz + zy*yz*xx + zz*yx*xy)
		*      | zx   zy   zz  |  zx   zy |
		*    @endcode
		*
		*  @note
		*    - If the determinant is non-zero, then an inverse matrix exists
		*    - If the determinant is 0, the matrix is called singular, else nonsingular (invertible) matrix
		*    - If the determinant is 1, the inverse matrix is equal to the transpose of the matrix
		*/
		inline float GetDeterminant() const;

		/**
		*  @brief
		*    Transpose this matrix
		*
		*  @remarks
		*    The transpose of matrix is the matrix generated when every element in
		*    the matrix is swapped with the opposite relative to the major diagonal
		*    This can be expressed as the mathematical operation:
		*    @code
		*      M'   = M
		*        ij    ji
		*
		*     | xx xy xz |                    | xx yx zx |
		*     | yx yy yz |  the transpose is  | xy yy zy |
		*     | zx zy zz |                    | xz yz zz |
		*    @endcode
		*
		*  @note
		*    - If the matrix is a rotation matrix (= isotropic matrix = determinant is 1),
		*      then the transpose is guaranteed to be the inverse of the matrix
		*/
		inline void Transpose();

		/**
		*  @brief
		*    Returns the transposed matrix
		*
		*  @return
		*    Transposed matrix
		*
		*  @see
		*    - Transpose()
		*/
		inline Matrix3x3 GetTransposed() const;

		/**
		*  @brief
		*    Inverts the matrix
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe the determinant is null?)
		*
		*  @remarks
		*    Providing that the determinant is non-zero, then the inverse is calculated
		*    using Kramer's rule as:
		*    @code
		*     -1     1     |   yy*zz-yz*zy  -(xy*zz-zy*xz)   xy*yz-yy*xz  |
		*    M   = ----- . | -(yx*zz-yz*zx)   xx*zz-zx*xz  -(xx*yz-yx*xz) |
		*          det M   |   yx*zy-zx*yy  -(xx*zy-zx*xy)   xx*yy-xy*yx  |
		*    @endcode
		*
		*  @note
		*    - If the determinant is 1, the inversed matrix is equal to the transposed one
		*/
		PLMATH_API bool Invert();

		/**
		*  @brief
		*    Returns the inverse of the matrix
		*
		*  @return
		*    Inverse of the matrix, if the determinant is null, an identity matrix is returned
		*/
		PLMATH_API Matrix3x3 GetInverted() const;

		/**
		*  @brief
		*    Rotates a vector
		*
		*  @param[in] fX
		*    X component of the vector to rotate
		*  @param[in] fY
		*    Y component of the vector to rotate
		*  @param[in] fZ
		*    Z component of the vector to rotate
		*  @param[in] bUniformScale
		*    Is this a uniform scale matrix? (all axis are scaled equally)
		*    If you know EXACTLY it's one, set this to 'true' to gain some more speed, else DON'T set to 'true'!
		*
		*  @return
		*    The rotated vector
		*
		*  @remarks
		*    This function is similar to a matrix * vector operation - except that it
		*    can also deal with none uniform scale. So, this function can for instance be
		*    used to rotate a direction vector. (matrix * direction vector)
		*
		*  @note
		*    - You can't assume that the resulting vector is normalized
		*    - Use this function to rotate for example a normal vector
		*/
		PLMATH_API Vector3 RotateVector(float fX, float fY, float fZ, bool bUniformScale = false) const;

		/**
		*  @brief
		*    Rotates a vector
		*
		*  @param[in] vV
		*    Vector to rotate
		*  @param[in] bUniformScale
		*    Is this a uniform scale matrix? (all axis are scaled equally)
		*    If you know EXACTLY it's one, set this to 'true' to gain some more speed, else DON'T set to 'true'!
		*
		*  @return
		*    The rotated vector
		*
		*  @see
		*    - RotateVector(float, float, float) above
		*/
		PLMATH_API Vector3 RotateVector(const Vector3 &vV, bool bUniformScale = false) const;

		//[-------------------------------------------------------]
		//[ Scale                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Sets a scale matrix
		*
		*  @param[in] fX
		*    X scale
		*  @param[in] fY
		*    Y scale
		*  @param[in] fZ
		*    Z scale
		*
		*  @remarks
		*    @code
		*    | x 0 0 |
		*    | 0 y 0 |
		*    | 0 0 z |
		*    @endcode
		*/
		inline void SetScaleMatrix(float fX, float fY, float fZ);
		inline void SetScaleMatrix(const Vector3 &vV);

		/**
		*  @brief
		*    Extracts the scale vector from the matrix as good as possible
		*
		*  @param[out] fX
		*    Receives the x scale
		*  @param[out] fY
		*    Receives the y scale
		*  @param[out] fZ
		*    Receives the z scale
		*
		*  @note
		*    - This function will not work correctly if one or two components are negative while
		*      another is/are not (we can't figure out WHICH axis are negative!)
		*/
		PLMATH_API void GetScale(float &fX, float &fY, float &fZ) const;
		inline Vector3 GetScale() const;
		inline void GetScale(float fV[]) const;

		//[-------------------------------------------------------]
		//[ Rotation                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Sets an x axis rotation matrix by using one given Euler angle
		*
		*  @param[in] fAngleX
		*    Rotation angle around the x axis (in radian, between [0, Math::Pi2])
		*
		*  @remarks
		*    @code
		*         |    1       0       0    |
		*    RX = |    0     cos(a) -sin(a) |
		*         |    0     sin(a)  cos(a) |
		*    @endcode
		*    where a > 0 indicates a counterclockwise rotation in the yz-plane (if you look along -x)
		*/
		PLMATH_API void FromEulerAngleX(float fAngleX);

		/**
		*  @brief
		*    Sets an y axis rotation matrix by using one given Euler angle
		*
		*  @param[in] fAngleY
		*    Rotation angle around the y axis (in radian, between [0, Math::Pi2])
		*
		*  @remarks
		*    @code
		*         |  cos(a)    0     sin(a) |
		*    RY = |    0       1       0    |
		*         | -sin(a)    0     cos(a) |
		*    @endcode
		*    where a > 0 indicates a counterclockwise rotation in the zx-plane (if you look along -y)
		*/
		PLMATH_API void FromEulerAngleY(float fAngleY);

		/**
		*  @brief
		*    Sets an z axis rotation matrix by using one given Euler angle
		*
		*  @param[in] fAngleZ
		*    Rotation angle around the z axis (in radian, between [0, Math::Pi2])
		*
		*  @remarks
		*    @code
		*         |  cos(a) -sin(a)    0    |
		*    RZ = |  sin(a)  cos(a)    0    |
		*         |    0       0       1    |
		*    @endcode
		*    where a > 0 indicates a counterclockwise rotation in the xy-plane (if you look along -z)
		*/
		PLMATH_API void FromEulerAngleZ(float fAngleZ);

		/**
		*  @brief
		*    Returns a rotation matrix as a selected axis and angle
		*
		*  @param[out] fX
		*    Will receive the x component of the selected axis
		*  @param[out] fY
		*    Will receive the y component of the selected axis
		*  @param[out] fZ
		*    Will receive the z component of the selected axis
		*  @param[out] fAngle
		*    Will receive the rotation angle around the selected axis (in radian, between [0, Math::Pi])
		*/
		PLMATH_API void ToAxisAngle(float &fX, float &fY, float &fZ, float &fAngle) const;

		/**
		*  @brief
		*    Sets a rotation matrix by using a selected axis and angle
		*
		*  @param[in] fX
		*    X component of the selected axis
		*  @param[in] fY
		*    Y component of the selected axis
		*  @param[in] fZ
		*    Z component of the selected axis
		*  @param[in] fAngle
		*    Rotation angle around the selected axis (in radian, between [0, Math::Pi])
		*
		*  @note
		*    - The given selected axis must be normalized!
		*/
		PLMATH_API void FromAxisAngle(float fX, float fY, float fZ, float fAngle);

		/**
		*  @brief
		*    Returns the x (left) axis
		*
		*  @return
		*    The x (left) axis
		*
		*  @remarks
		*    @code
		*    | x 0 0 |
		*    | y 0 0 |
		*    | z 0 0 |
		*    @endcode
		*
		*  @note
		*    - It's possible that the axis vector is not normalized because for instance
		*      the matrix was scaled
		*/
		inline Vector3 GetXAxis() const;

		/**
		*  @brief
		*    Returns the y (up) axis
		*
		*  @return
		*    The y (up) axis
		*
		*  @remarks
		*    @code
		*    | 0 x 0 |
		*    | 0 y 0 |
		*    | 0 z 0 |
		*    @endcode
		*
		*  @see
		*    - GetXAxis()
		*/
		inline Vector3 GetYAxis() const;

		/**
		*  @brief
		*    Returns the z (forward) axis
		*
		*  @return
		*    The z (forward) axis
		*
		*  @remarks
		*    @code
		*    | 0 0 x |
		*    | 0 0 y |
		*    | 0 0 z |
		*    @endcode
		*
		*  @see
		*    - GetXAxis()
		*/
		inline Vector3 GetZAxis() const;

		/**
		*  @brief
		*    Returns the three axis of a rotation matrix (not normalized)
		*
		*  @param[out] vX
		*    Will receive the x axis
		*  @param[out] vY
		*    Will receive the y axis
		*  @param[out] vZ
		*    Will receive the z axis
		*
		*  @remarks
		*    @code
		*    | vX.x vY.x vZ.x |
		*    | vX.y vY.y vZ.y |
		*    | vX.z vY.z vZ.z |
		*    @endcode
		*/
		PLMATH_API void ToAxis(Vector3 &vX, Vector3 &vY, Vector3 &vZ) const;

		/**
		*  @brief
		*    Sets a rotation matrix by using three given axis
		*
		*  @param[in] vX
		*    X axis
		*  @param[in] vY
		*    Y axis
		*  @param[in] vZ
		*    Z axis
		*
		*  @see
		*    - ToAxis()
		*/
		PLMATH_API void FromAxis(const Vector3 &vX, const Vector3 &vY, const Vector3 &vZ);

		/**
		*  @brief
		*    Builds a look-at matrix
		*
		*  @param[in] vEye
		*    Eye position
		*  @param[in] vAt
		*    Camera look-at target
		*  @param[in] vUp
		*    Current world's up, usually [0, 1, 0]
		*
		*  @return
		*    This instance
		*/
		PLMATH_API Matrix3x3 &LookAt(const Vector3 &vEye, const Vector3 &vAt, const Vector3 &vUp);

		//[-------------------------------------------------------]
		//[ Misc                                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Sets a shearing matrix
		*
		*  @param[in] fShearXY
		*    Shear X by Y
		*  @param[in] fShearXZ
		*    Shear X by Z
		*  @param[in] fShearYX
		*    Shear Y by X
		*  @param[in] fShearYZ
		*    Shear Y by Z
		*  @param[in] fShearZX
		*    Shear Z by X
		*  @param[in] fShearZY
		*    Shear Z by Y
		*
		*  @return
		*    This instance
		*
		*  @remarks
		*    A shearing matrix can be used to for instance make a 3D model appear to slant
		*    sideways. Here's a table showing how a combined shearing matrix looks like:
		*    @code
		*    | 1        fShearYX  fShearZX  |
		*    | fShearXY 1         fShearZY  |
		*    | fShearXZ fShearYZ  1         |
		*    @endcode
		*    If you only want to shear one axis it's recommended to construct the matrix by yourself.
		*/
		inline Matrix3x3 &SetShearing(float fShearXY, float fShearXZ, float fShearYX, float fShearYZ,
									  float fShearZX, float fShearZY);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLMath/Matrix3x3.inl"


#endif // __PLMATH_MATRIX3X3_H__
