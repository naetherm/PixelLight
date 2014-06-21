/*********************************************************\
 *  File: Quaternion.h                                   *
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


#ifndef __PLMATH_QUATERNION_H__
#define __PLMATH_QUATERNION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
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
*    Quaternion
*
*  @note
*    - Orientation quaternions are unit quaternions
*/
class Quaternion {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Component
		*/
		enum Component {
			W = 0,	/**< W component */
			X = 1,	/**< X component */
			Y = 2,	/**< Y component */
			Z = 3	/**< Z component */
		};


	//[-------------------------------------------------------]
	//[ Public static data                                    ]
	//[-------------------------------------------------------]
	public:
		static PLMATH_API const Quaternion Zero;		/**<  0.0, 0.0, 0.0, 0.0 */
		static PLMATH_API const Quaternion Identity;	/**<  1.0, 0.0, 0.0, 0.0 */


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Some direct quaternion element accesses
		*/
		union {
			/*
			*  @brief
			*    0 = scalar (w), >0 = vector
			*/
			float fQ[4];

			/*
			*  @brief
			*    w = scalar, x&y&z = vector
			*/
			struct {
				float w, x, y, z;
			};
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Constructors                                          ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Default constructor setting an identity quaternion
		*/
		inline Quaternion();

		inline Quaternion(float fW, float fX, float fY, float fZ);
		inline Quaternion(const float fQ[]);
		inline Quaternion(const Quaternion &qQ);
		inline Quaternion(const Vector3 &vAxis, float fAngle);
		PLMATH_API Quaternion(const Matrix3x3 &mRot);
		PLMATH_API Quaternion(const Matrix3x4 &mRot);
		PLMATH_API Quaternion(const Matrix4x4 &mRot);

		//[-------------------------------------------------------]
		//[ Destructor                                            ]
		//[-------------------------------------------------------]
		inline ~Quaternion();

		//[-------------------------------------------------------]
		//[ Operators                                             ]
		//[-------------------------------------------------------]
		inline Quaternion  operator +(const Quaternion &qQ) const;
		inline Quaternion &operator +=(const Quaternion &qQ);
		inline Quaternion  operator -() const;
		inline Quaternion  operator -(const Quaternion &qQ) const;
		inline Quaternion &operator -=(const Quaternion &qQ);
		inline Quaternion  operator *(float f) const;
		inline Quaternion &operator *=(float f);
		inline Quaternion  operator *(const Quaternion &qQ) const;
		inline Quaternion &operator *=(const Quaternion &qQ);
		inline Vector3     operator *(const Vector3 &vV) const;
		inline Vector4     operator *(const Vector4 &vV) const;
		inline Quaternion  operator /(float f) const;
		inline Quaternion &operator /=(float f);
		inline Quaternion &operator =(const Quaternion &qQ);
		inline bool        operator ==(const Quaternion &qQ) const;
		inline bool        operator !=(const Quaternion &qQ) const;
		inline             operator float *();
		inline             operator const float *() const;

		//[-------------------------------------------------------]
		//[ Get and set functions                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Sets the component of the quaternion
		*
		*  @param[in] fW
		*    W component
		*  @param[in] fX
		*    X component
		*  @param[in] fY
		*    Y component
		*  @param[in] fZ
		*    Z component
		*
		*  @return
		*    This quaternion
		*/
		inline Quaternion &SetWXYZ(float fW, float fX, float fY, float fZ);

		/**
		*  @brief
		*    Sets the component of the quaternion
		*
		*  @param[in] fValues
		*    Array of at least 4 floats
		*
		*  @return
		*    This quaternion
		*/
		inline Quaternion &SetWXYZ(const float fValues[]);

		/**
		*  @brief
		*    Returns a selected axis and angle from the rotation quaternion
		*
		*  @param[out] fX
		*    Will receive the x component of the selected axis
		*  @param[out] fY
		*    Will receive the y component of the selected axis
		*  @param[out] fZ
		*    Will receive the z component of the selected axis
		*  @param[out] fAngle
		*    Will receive the rotation angle around the selected axis (in radian, between [0, Math::Pi])
		*
		*  @note
		*    - The quaternion must be normalized
		*/
		PLMATH_API void ToAxisAngle(float &fX, float &fY, float &fZ, float &fAngle) const;

		/**
		*  @brief
		*    Sets a rotation quaternion by using a selected axis and angle
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
		*  @return
		*    This quaternion
		*
		*  @note
		*    - The given selected axis must be normalized!
		*/
		PLMATH_API Quaternion &FromAxisAngle(float fX, float fY, float fZ, float fAngle);

		/**
		*  @brief
		*    Returns the x (left) axis
		*
		*  @return
		*    The x (left) axis, already normalized for rotation quaternions
		*/
		PLMATH_API Vector3 GetXAxis() const;

		/**
		*  @brief
		*    Returns the y (up) axis
		*
		*  @return
		*    The y (up) axis, already normalized for rotation quaternions
		*/
		PLMATH_API Vector3 GetYAxis() const;

		/**
		*  @brief
		*    Returns the z (forward) axis
		*
		*  @return
		*    The z (forward) axis, already normalized for rotation quaternions
		*/
		PLMATH_API Vector3 GetZAxis() const;

		/**
		*  @brief
		*    Returns the rotation quaternion as 3x3 matrix
		*
		*  @param[out] mRot
		*    Will receive the rotation matrix
		*/
		PLMATH_API void ToRotationMatrix(Matrix3x3 &mRot) const;

		/**
		*  @brief
		*    Sets a rotation quaternion by using a 3x3 rotation matrix
		*
		*  @param[in] mRot
		*    Rotation matrix
		*
		*  @return
		*    This quaternion
		*/
		PLMATH_API Quaternion &FromRotationMatrix(const Matrix3x3 &mRot);

		/**
		*  @brief
		*    Returns the rotation quaternion as 3x4 matrix
		*
		*  @param[out] mRot
		*    Will receive the rotation matrix
		*/
		PLMATH_API void ToRotationMatrix(Matrix3x4 &mRot) const;

		/**
		*  @brief
		*    Sets a rotation quaternion by using a 3x4 rotation matrix
		*
		*  @param[in] mRot
		*    Rotation matrix
		*
		*  @return
		*    This quaternion
		*/
		PLMATH_API Quaternion &FromRotationMatrix(const Matrix3x4 &mRot);

		/**
		*  @brief
		*    Returns the rotation quaternion as 4x4 matrix
		*
		*  @param[out] mRot
		*    Will receive the rotation matrix
		*/
		PLMATH_API void ToRotationMatrix(Matrix4x4 &mRot) const;

		/**
		*  @brief
		*    Sets a rotation quaternion by using a 4x4 rotation matrix
		*
		*  @param[in] mRot
		*    Rotation matrix
		*
		*  @return
		*    This quaternion
		*/
		PLMATH_API Quaternion &FromRotationMatrix(const Matrix4x4 &mRot);

		//[-------------------------------------------------------]
		//[ Misc                                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Compares two quaternions using an epsilon environment
		*
		*  @param[in] qQ
		*    Quaternion to compare with
		*  @param[in] fEpsilon
		*    Epsilon environment
		*
		*  @return
		*     'true' if the both quaternions are equal, else 'false'
		*/
		inline bool Compare(const Quaternion &qQ, float fEpsilon = Math::Epsilon) const;

		/**
		*  @brief
		*    Returns the length (also called magnitude) of the quaternion
		*
		*  @return
		*     The length (also called magnitude) of the quaternion
		*/
		inline float GetLength() const;

		/**
		*  @brief
		*    Returns the squared length (norm) of the quaternion
		*
		*  @return
		*     The squared length (norm) of the quaternion
		*/
		inline float GetSquaredLength() const;

		/**
		*  @brief
		*    Returns the dot product of this quaternion and another one
		*
		*  @param[in] qQ
		*    Second quaternion
		*
		*  @return
		*    The dot product of the two quaternions
		*/
		inline float DotProduct(const Quaternion &qQ) const;

		/**
		*  @brief
		*    Normalizes the quaternion
		*
		*  @return
		*    This quaternion
		*/
		inline Quaternion &Normalize();

		/**
		*  @brief
		*    Returns a normalized quaternion
		*
		*  @return
		*    Normalized quaternion
		*/
		inline Quaternion GetNormalized() const;

		/**
		*  @brief
		*    Conjugates the quaternion
		*/
		inline void Conjugate();

		/**
		*  @brief
		*    Returns the conjugated of the quaternion
		*
		*  @return
		*    The conjugated of the quaternion
		*/
		inline Quaternion GetConjugated() const;

		/**
		*  @brief
		*    Inverts the quaternion
		*/
		inline void Invert();

		/**
		*  @brief
		*    Returns the inverted quaternion
		*
		*  @return
		*    Inverted quaternion
		*/
		inline Quaternion GetInverted() const;

		/**
		*  @brief
		*    Inverts the normalized quaternion
		*
		*  @remarks
		*    This function can only be used if the quaternion is normalized!
		*    (rotation quaternions are normally always normalized)
		*    If the quaternion is not normalized, use Invert() instead.
		*/
		inline void UnitInvert();

		/**
		*  @brief
		*    Returns the inverted normalized quaternion
		*
		*  @return
		*    Inverted normalized quaternion
		*
		*  @see
		*    - UnitInvert()
		*/
		inline Quaternion GetUnitInverted() const;

		/**
		*  @brief
		*    Calculates the exponential of the quaternion
		*
		*  @return
		*    This quaternion
		*/
		inline Quaternion &Exp();

		/**
		*  @brief
		*    Returns the exponential of the quaternion
		*
		*  @return
		*    The exponential of the quaternion
		*/
		inline Quaternion GetExp() const;

		/*
		*  @brief
		*    Calculates the logarithm of a unit quaternion
		*
		*  @return
		*    This quaternion
		*/
		inline Quaternion &Log();

		/*
		*  @brief
		*    Returns the logarithm of a unit quaternion
		*
		*  @return
		*    The logarithm of a unit quaternion
		*/
		inline Quaternion GetLog() const;

		/**
		*  @brief
		*    Calculates the power of the quaternion
		*
		*  @param[in] fPower
		*    Power to calculate
		*
		*  @return
		*    This quaternion
		*/
		inline Quaternion &Power(float fPower);

		/**
		*  @brief
		*    Returns the power of the quaternion
		*
		*  @param[in] fPower
		*    Power to calculate
		*
		*  @return
		*    The power of the quaternion
		*/
		inline Quaternion GetPower(float fPower) const;

		/**
		*  @brief
		*    Computes spherical linear interpolation between qQ1 and qQ2 with time 0-1
		*
		*  @param[in] qQ1
		*    Start quaternion (time: 0)
		*  @param[in] qQ2
		*    End quaternion (time: 1)
		*  @param[in] fTime
		*    Time from 0-1
		*/
		PLMATH_API void Slerp(const Quaternion &qQ1, const Quaternion &qQ2, float fTime);

		/**
		*  @brief
		*    To string
		*
		*  @return
		*    String with the data
		*/
		PLMATH_API PLCore::String ToString() const;

		/**
		*  @brief
		*    From string
		*
		*  @param[in] sString
		*    String with the data
		*/
		PLMATH_API bool FromString(const PLCore::String &sString);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLMath/Quaternion.inl"


#endif // __PLMATH_QUATERNION_H__
