/*********************************************************\
 *  File: Vector3.h                                      *
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


#ifndef __PLMATH_VECTOR3_H__
#define __PLMATH_VECTOR3_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/String/String.h>
#include "PLMath/PLMath.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Vector2;
class Vector4;
class Matrix3x3;
class Matrix3x4;
class Matrix4x4;
class Rectangle;
class Quaternion;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    3D vector
*/
class Vector3 {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Vector component
		*/
		enum Component {
			X = 0,	/**< X component */
			Y = 1,	/**< Y component */
			Z = 2	/**< Z component */
		};


	//[-------------------------------------------------------]
	//[ Public static data                                    ]
	//[-------------------------------------------------------]
	public:
		static PLMATH_API const Vector3 Zero;			/**<  0.0,  0.0,  0.0 */
		static PLMATH_API const Vector3 One;			/**<  1.0,  1.0,  1.0 */
		static PLMATH_API const Vector3 NegativeOne;	/**< -1.0, -1.0, -1.0 */
		static PLMATH_API const Vector3 UnitX;			/**<  1.0,  0.0,  0.0 */
		static PLMATH_API const Vector3 UnitY;			/**<  0.0,  1.0,  0.0 */
		static PLMATH_API const Vector3 UnitZ;			/**<  0.0,  0.0,  1.0 */
		static PLMATH_API const Vector3 NegativeUnitX;	/**< -1.0,  0.0,  0.0 */
		static PLMATH_API const Vector3 NegativeUnitY;	/**<  0.0, -1.0,  0.0 */
		static PLMATH_API const Vector3 NegativeUnitZ;	/**<  0.0,  0.0, -1.0 */


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Some direct vertex element accesses
		*/
		union {
			/**
			*  @brief
			*    Vertex element array access
			*/
			float fV[3];

			/**
			*  @brief
			*    Known vertex element names when dealing with positions or directions
			*/
			struct {
				float x, y, z;
			};

			/**
			*  @brief
			*    Known vertex element names when dealing with texture coordinates
			*/
			struct {
				float u, v, w;
			};

			/**
			*  @brief
			*    Known vertex element names when dealing with rotations
			*
			*  @note
			*    - X rotation axis: Pitch (also called 'bank') change is moving the nose down and the tail up (or vice-versa)
			*    - Y rotation axis: Yaw (also called 'heading') change is turning to the left or right
			*    - Z rotation axis: Roll (also called 'attitude') change is moving one wingtip up and the other down
			*/
			struct {
				float fPitch, fYaw, fRoll;
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
		*    Default constructor setting all components to 0
		*/
		inline Vector3();

		inline Vector3(float fX, float fY, float fZ = 0.0f);
		inline Vector3(const float fV[]);
		PLMATH_API Vector3(const Vector2 &vV, float fZ = 0.0f);
		inline Vector3(const Vector3 &vV);
		PLMATH_API Vector3(const Vector4 &vV);
		inline Vector3(const PLCore::String &sString);

		//[-------------------------------------------------------]
		//[ Destructor                                            ]
		//[-------------------------------------------------------]
		inline ~Vector3();

		//[-------------------------------------------------------]
		//[ Assignment operators                                  ]
		//[-------------------------------------------------------]
		inline     Vector3 &operator =(const float fV[]);
		PLMATH_API Vector3 &operator =(const Vector2 &vV);
		inline     Vector3 &operator =(const Vector3 &vV);
		PLMATH_API Vector3 &operator =(const Vector4 &vV);

		//[-------------------------------------------------------]
		//[ Comparison                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Compares two vectors
		*
		*  @param[in] vV
		*    Vector to compare with
		*
		*  @return
		*    'true' if all components are equal, else 'false'
		*/
		inline bool operator ==(const Vector3 &vV) const;

		/**
		*  @brief
		*    Compares two vectors
		*
		*  @param[in] vV
		*    Vector to compare with
		*
		*  @return
		*    'false' if all components are equal, else 'true'
		*/
		inline bool operator !=(const Vector3 &vV) const;

		/**
		*  @brief
		*    Compares two vectors lexicographically
		*
		*  @param[in] vV
		*    Vector to compare with
		*
		*  @return
		*    'true' if ALL components of this vector are less, else 'false'
		*
		*  @note
		*    - A lexicographical order for 3-dimensional vectors is used (see http://en.wikipedia.org/wiki/Ordered_vector_space)
		*/
		inline bool operator <(const Vector3 &vV) const;

		/**
		*  @brief
		*    Compares two vectors lexicographically
		*
		*  @param[in] vV
		*    Vector to compare with
		*
		*  @return
		*    'true' if ALL components of this vector are greater, else 'false'
		*
		*  @see
		*    - "operator <"
		*/
		inline bool operator >(const Vector3 &vV) const;

		/**
		*  @brief
		*    Compares two vectors lexicographically
		*
		*  @param[in] vV
		*    Vector to compare with
		*
		*  @return
		*    'true' if ALL components of this vector are less or equal, else 'false'
		*
		*  @see
		*    - "operator <"
		*/
		inline bool operator <=(const Vector3 &vV) const;

		/**
		*  @brief
		*    Compares two vectors lexicographically
		*
		*  @param[in] vV
		*    Vector to compare with
		*
		*  @return
		*    'true' if ALL components of this vector are greater or equal, else 'false'
		*
		*  @see
		*    - "operator <"
		*/
		inline bool operator >=(const Vector3 &vV) const;

		//[-------------------------------------------------------]
		//[ Vector                                                ]
		//[-------------------------------------------------------]
		inline Vector3  operator +(const Vector3 &vV) const;
		inline Vector3  operator +(float fN) const;
		inline Vector3 &operator +=(const Vector3 &vV);
		inline Vector3 &operator +=(float fN);
		inline Vector3  operator -() const;
		inline Vector3  operator -(const Vector3 &vV) const;
		inline Vector3  operator -(float fN) const;
		inline Vector3 &operator -=(const Vector3 &vV);
		inline Vector3 &operator -=(float fN);

		/**
		*  @brief
		*    Per component multiplication
		*
		*  @param[in] vV
		*    Vector to multiplicate with
		*
		*  @return
		*    The resulting vector
		*/
		inline Vector3 operator *(const Vector3 &vV) const;
		inline Vector3 operator *(float fS) const;

		/**
		*  @brief
		*    Per component multiplication
		*
		*  @param[in] vV
		*    Vector to multiplicate with
		*
		*  @return
		*    This vector which is now the resulting vector
		*/
		inline Vector3 &operator *=(const Vector3 &vV);
		inline Vector3 &operator *=(float fS);

		/**
		*  @brief
		*    Rotates the vector
		*
		*  @param[in] mRot
		*    Matrix which rotates the vector
		*
		*  @return
		*    This vector which is now the resulting vector
		*/
		PLMATH_API Vector3 &operator *=(const Matrix3x3 &mRot);

		/**
		*  @brief
		*    Transforms the vector
		*
		*  @param[in] mTrans
		*    Matrix which transforms the vector
		*
		*  @return
		*    This vector which is now the resulting vector
		*/
		PLMATH_API Vector3 &operator *=(const Matrix3x4 &mTrans);

		/**
		*  @brief
		*    Transforms the vector
		*
		*  @param[in] mTrans
		*    Matrix which transforms the vector
		*
		*  @return
		*    This vector which is now the resulting vector
		*/
		PLMATH_API Vector3 &operator *=(const Matrix4x4 &mTrans);

		/**
		*  @brief
		*    Rotates the vector
		*
		*  @param[in] qQ
		*    Quaternion which rotates the vector
		*
		*  @return
		*    This vector which is now the resulting vector
		*/
		PLMATH_API Vector3 &operator *=(const Quaternion &qQ);

		/**
		*  @brief
		*    Per component division
		*
		*  @param[in] vV
		*    Vector to divide through
		*
		*  @return
		*    The resulting vector
		*/
		inline Vector3 operator /(const Vector3 &vV) const;
		inline Vector3 operator /(float fS) const;

		/**
		*  @brief
		*    Per component division
		*
		*  @param[in] vV
		*    Vector to divide through
		*
		*  @return
		*    This vector which is now the resulting vector
		*/
		inline Vector3 &operator /=(const Vector3 &vV);
		inline Vector3 &operator /=(float fS);

		//[-------------------------------------------------------]
		//[ Get and set                                           ]
		//[-------------------------------------------------------]
		inline operator float *();
		inline operator const float *() const;
		inline float &operator [](int nIndex);
		inline const float &operator [](int nIndex) const;
		inline void  GetXYZ(float &fX, float &fY, float &fZ) const;
		inline float GetX() const;
		inline float GetY() const;
		inline float GetZ() const;
		inline void  SetXYZ(float fX = 0.0f, float fY = 0.0f, float fZ = 0.0f);
		inline void  SetXYZ(const float fV[]);
		inline void  SetX(float fX = 0.0f);
		inline void  SetY(float fY = 0.0f);
		inline void  SetZ(float fZ = 0.0f);
		inline void  IncXYZ(float fX = 0.0f, float fY = 0.0f, float fZ = 0.0f);
		inline void  IncXYZ(const float fV[]);
		inline void  IncX(float fX);
		inline void  IncY(float fY);
		inline void  IncZ(float fZ);

		//[-------------------------------------------------------]
		//[ Misc                                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns whether the vector is null or not
		*
		*  @return
		*    'true' if the vector is null, else 'false'
		*/
		inline bool IsNull() const;

		/**
		*  @brief
		*    Returns whether the vector is packed (within range of 0-1) or not
		*
		*  @return
		*    'true' if the vector is packed, else 'false'
		*/
		inline bool IsPacked() const;

		/**
		*  @brief
		*    Packs/clamps the vector into a range of 0-1
		*
		*  @remarks
		*    First, the vector is normalized - now each component is between -1 and 1.
		*    This normalized vector is scaled by 0.5 and 0.5 is added.
		*/
		inline void PackTo01();

		/**
		*  @brief
		*    Returns a vector which is packed/clamped into the range of 0-1
		*
		*  @return
		*    The packed vector
		*
		*  @see
		*    - PackTo01()
		*/
		inline Vector3 GetPackedTo01() const;

		/**
		*  @brief
		*    Unpacks the packed vector into a range of -1 to 1
		*
		*  @remarks
		*    The vector is scaled by 2 and 1 is subtracted.
		*
		*  @note
		*    - There's no internal check whether the vector is packed or not, you can do this
		*      by yourself using the IsPacked() function
		*/
		inline void UnpackFrom01();

		/**
		*  @brief
		*    Returns a unpacked vector of the range of -1 to 1
		*
		*  @return
		*    The unpacked vector
		*
		*  @see
		*    - UnpackFrom01()
		*/
		inline Vector3 GetUnpackedFrom01() const;

		/**
		*  @brief
		*    Returns the smallest component
		*
		*  @return
		*    The smallest component
		*
		*  @remarks
		*    If x is 1, y is 2 and z is 3, this function will return the x component.
		*/
		inline Component GetSmallestComponent() const;

		/**
		*  @brief
		*    Returns the value of the smallest component
		*
		*  @return
		*    The value of the smallest component
		*
		*  @see
		*    - GetSmallestComponent() above
		*/
		inline float GetSmallestValue() const;

		/**
		*  @brief
		*    Returns the greatest component
		*
		*  @return
		*    The greatest component
		*
		*  @remarks
		*    If x is 1, y is 2 and z is 3, this function will return the z component.
		*/
		inline Component GetGreatestComponent() const;

		/**
		*  @brief
		*    Returns the value of the greatest component
		*
		*  @return
		*    The value of the greatest component
		*
		*  @see
		*    - GetGreatestComponent() above
		*/
		inline float GetGreatestValue() const;

		/**
		*  @brief
		*    Inverts the vector
		*
		*  @remarks
		*    v'.x = -v.x\n
		*    v'.y = -v.y\n
		*    v'.z = -v.z
		*/
		inline void Invert();

		/**
		*  @brief
		*    Returns the inverted vector
		*
		*  @return
		*    Inverted vector
		*
		*  @see
		*    - Invert()
		*/
		inline Vector3 GetInverted() const;

		/**
		*  @brief
		*    Returns the length of the vector (also called magnitude)
		*
		*  @return
		*    Vector length
		*
		*  @remarks
		*    l = sqrt(x*x + y*y + z*z)
		*/
		inline float GetLength() const;

		/**
		*  @brief
		*    Returns the squared length of the vector (also called norm)
		*
		*  @return
		*    Squared vector length
		*
		*  @remarks
		*    l = x*x + y*y + z*z
		*
		*  @note
		*    - For better performance, use this function instead of GetLength() whenever
		*      possible. You can often work with squared lengths instead of the 'real' ones.
		*/
		inline float GetSquaredLength() const;

		/**
		*  @brief
		*    Sets the vector to the given length
		*
		*  @param[in] fLength
		*    Length to set
		*
		*  @remarks
		*    v' = v*l/sqrt(x*x + y*y + z*z)
		*/
		inline void SetLength(float fLength = 1.0f);

		/**
		*  @brief
		*    Normalizes the vector
		*
		*  @return
		*    This instance
		*
		*  @remarks
		*    v' = v*1/sqrt(x*x + y*y + z*z)
		*
		*  @note
		*    - This will set the vector to a length of 1 (same as SetLength(1.0f) :)
		*    - A normalized vector is called 'unit vector'
		*/
		inline Vector3 &Normalize();

		/**
		*  @brief
		*    Returns the normalized vector
		*
		*  @return
		*    Normalized vector
		*
		*  @see
		*    - Normalize()
		*/
		inline Vector3 GetNormalized() const;

		/**
		*  @brief
		*    Returns the distance to another vector
		*
		*  @param[in] vV
		*    The other vector
		*
		*  @return
		*    Distance to the other vector
		*
		*  @remarks
		*    dx = v2.x-v1.x\n
		*    dy = v2.y-v1.y\n
		*    dz = v2.z-v1.z\n
		*    d  = sqrt(dx*dx + dy*dy + dz*dz)
		*/
		inline float GetDistance(const Vector3 &vV) const;

		/**
		*  @brief
		*    Returns the squared distance to another vector
		*
		*  @param[in] vV
		*    The other vector
		*
		*  @return
		*    Distance to the other vector
		*
		*  @remarks
		*    dx = v2.x-v1.x\n
		*    dy = v2.y-v1.y\n
		*    dz = v2.z-v1.z\n
		*    d  = dx*dx + dy*dy + dz*dz
		*
		*  @note
		*    - For better performance, use this function instead of GetDistance() whenever
		*      possible. You can often work with squared distances instead of the 'real' ones.
		*/
		inline float GetSquaredDistance(const Vector3 &vV) const;

		/**
		*  @brief
		*    Returns the dot product of two vectors
		*
		*  @param[in] vV
		*    Second vector
		*
		*  @return
		*    Dot product of the vectors
		*
		*  @remarks
		*    d = this->x*vV.x + this->y*vV.y + this->z*vV.z
		*
		*  @note
		*    - The dot product is also known as 'scalar product' or 'inner product'
		*    - The dot product of a vector with itself is equal to it's squared length, so it's possible to
		*      use GetSquaredLength() instead of v.DotProduct(v)
		*    - The dot product is commutative
		*    - If the two vectors are perpendicular, their dot product equals zero
		*    - If the angle between the two vectors is acute (< 90 degrees) the dot product will be positive,
		*      if the angle is obtuse (> 90 degrees) the dot product will be negative
		*    - Geometric definition: a.DotProduct(b) = a.GetLength() * b.GetLength() * cos(r)
		*
		*  @see
		*    - GetAngle()
		*/
		inline float DotProduct(const Vector3 &vV) const;

		/**
		*  @brief
		*    Project vector a onto another vector b
		*
		*  @param[in] vA
		*    Vector to project
		*  @param[in] vB
		*    Vector to project onto
		*
		*  @remarks
		*    @code
		*            ^.
		*           / .
		*          /  .
		*       a /   .
		*        /    .                                  a.DotProduct(b)
		*       /     .                  projb(a) = b * -----------------
		*      /      .                                  b.DotProduct(b)
		*     /       .
		*     --->____.
		*      b      ^= projb(a)
		*    @endcode
		*
		*  @return
		*    The resulting projection vector
		*/
		inline Vector3 ProjectVector(const Vector3 &vA, const Vector3 &vB) const;

		/**
		*  @brief
		*    Calculates the angle between two vectors
		*
		*  @param[in] vV
		*    Second vector
		*
		*  @return
		*    The angle between the two vectors (in radians)
		*
		*  @remarks
		*    @code
		*                    v1.DotProduct(v2)
		*     cos A = --------------------------------
		*              v1.GetLength() * v2.GetLength()
		*    @endcode
		*
		*  @note
		*    - If the two vectors are normalized, you can also use acos(DotProduct()) which is
		*      in this case more performant
		*/
		inline float GetAngle(const Vector3 &vV) const;

		/**
		*  @brief
		*    Returns the cross product of this vector with another
		*
		*  @param[in] vV
		*    Vector to calculate the cross product with
		*
		*  @remarks
		*    @code
		*      c               (a = this, b = vV)
		*      ^     a
		*      |    ^                      | ax |   | bx |   | ay*bz - az*by |
		*      |   /           c = a x b = | ay | x | by | = | az*bx - ax*bz |
		*      |  /                        | az |   | bz |   | ax*by - ay*bx |
		*      | /
		*      |/--------> b
		*    @endcode
		*
		*  @return
		*    The calculated vector which is perpendicular to the given two vectors
		*
		*  @remarks
		*    The length of the cross product is the lengths of the individual vectors, multiplied together
		*    with the sine of the angle between them. This means you can use the cross product to tell when
		*    two vectors are parallel, because if they are parallel their cross product will be zero.
		*
		*  @note
		*    - The cross product is also known as the 'vector product' or 'outer product'
		*    - Unlike the dot product, the cross product is only defined for 3-dimensional
		*      vectors (and not for 2-dimensional, 4-dimensional and so on)
		*    - The cross product is anti commutative (a x b = -(b x a) ... means that the order is important)
		*/
		inline Vector3 CrossProduct(const Vector3 &vV) const;

		/**
		*  @brief
		*    Calculates the cross product of two vectors
		*
		*  @param[in] vV1
		*    First of the two vectors to calculate the cross product from
		*  @param[in] vV2
		*    Second of the two vectors to calculate the cross product from
		*
		*  @return
		*    Reference to the vector itself which is now perpendicular (and not normalized)
		*    to the two vectors
		*
		*  @see
		*    - CrossProduct() above
		*/
		inline Vector3 &CrossProduct(const Vector3 &vV1, const Vector3 &vV2);

		/**
		*  @brief
		*    Calculates the right/up vectors of the vector
		*
		*  @param[out] vRight
		*    The resulting right vectors which are perpendicular to the source vector
		*  @param[out] vUp
		*    The resulting up vectors which are perpendicular to the source vector
		*/
		PLMATH_API void GetRightUp(Vector3 &vRight, Vector3 &vUp) const;
		PLMATH_API void GetRightUp(float fRight[], float fUp[]) const;

		/**
		*  @brief
		*    Calculates a face/plane normal
		*
		*  @param[in] vV1
		*    First of the three vectors which describes the face/plane
		*  @param[in] vV2
		*    Second of the three vectors which describes the face/plane
		*  @param[in] vV3
		*    Third of the three vectors which describes the face/plane
		*
		*  @remarks
		*    GetNormalized(CrossProduct(vV1-vV2, vV1-vV3))
		*/
		inline void GetFaceNormal(const Vector3 &vV1, const Vector3 &vV2, const Vector3 &vV3);

		/**
		*  @brief
		*    Rotates two vectors around it's axis
		*
		*  @param[in] vV
		*    Other vector to rotate
		*  @param[in] fAngle
		*    Angle the two vectors should be rotated
		*
		*  @return
		*    Reference to this vector
		*/
		PLMATH_API Vector3 &RotateAxis(Vector3 &vV, float fAngle);

		/**
		*  @brief
		*    Returns a rotation quaternion to the destination vector
		*
		*  @param[in] vDest
		*    Destination vector
		*
		*  @return
		*     Rotation quaternion
		*/
		PLMATH_API Quaternion GetRotationTo(const Vector3 &vDest) const;

		/**
		*  @brief
		*    Calculates a normalized projection vector
		*
		*  @param[in] vX
		*    The first vector to calculate the projection from
		*  @param[in] vN
		*    The second vector to calculate the projection from
		*
		*  @return
		*    Reference to the vector itself which is now the resulting projection
		*/
		PLMATH_API Vector3 &GetProjection(const Vector3 &vX, const Vector3 &vN);

		/**
		*  @brief
		*    Project the vector on to the plane created by two direction vectors
		*
		*  @param[in] vV1
		*    First of the two direction vectors creating the plane
		*  @param[in] vV2
		*    Second of the two direction vectors creating the plane
		*
		*  @return
		*    Reference to the vector itself which is now the resulting projection vector
		*
		*  @note
		*    - vV1 and vV2 MUST be perpendicular to each other
		*/
		PLMATH_API Vector3 &ProjectPlane(const Vector3 &vV1, const Vector3 &vV2);

		/**
		*  @brief
		*    Calculates a reflection vector
		*
		*  @param[in] vIncidentNormal
		*    Incident normal
		*  @param[in] vNormal
		*    Plane normal which reflects the incident vector (must be normalized)
		*
		*  @return
		*    Reference to the vector itself which is now the resulting reflection vector
		*
		*  @remarks
		*    If I is the incident vector and N a normalized normal vector, then
		*    the reflection vector R is computed as: R = I - 2*N*(N*I) (N*I results in a scalar...)
		*    @code
		*                Plane normal N
		*                     |
		*    Incident ray I   |   Reflected ray R
		*                  \  |  /
		*                   \ | /
		*    ________________\|/________________ Plane
		*    @endcode
		*
		*  @note
		*    - The reflection vector has the same length as the incident vector
		*/
		PLMATH_API Vector3 &Reflect(const Vector3 &vIncidentNormal, const Vector3 &vNormal);

		/**
		*  @brief
		*    Calculates a refraction vector
		*
		*  @param[in] vIncidentNormal
		*    Incident normal
		*  @param[in] vNormal
		*    Plane normal which refracts the incident vector (must be normalized)
		*  @param[in] fEtaRatio
		*    Index of refraction ratio
		*
		*  @return
		*    Reference to the vector itself which is now the resulting refraction vector
		*
		*  @remarks
		*    Calculates a refraction vector using Snell's Law.
		*    @code
		*                Plane normal N
		*                     |
		*    Incident ray I   |
		*                  \  |
		*                   \ |
		*    ________________\|_________________ Plane
		*                     \
		*                        \
		*                           \
		*                            Refracted ray R
		*    @endcode
		*    \n
		*    Here are some indices of refraction:
		*    Vacuum = 1.0, Air = 1.003, Water = 1.3333, Glass = 1.5, Plastic = 1.5,
		*    Diamond = 2.417
		*
		*  @note
		*    - The refraction vector has the same length as the incident vector
		*/
		PLMATH_API Vector3 &Refract(const Vector3 &vIncidentNormal, const Vector3 &vNormal, float fEtaRatio);

		/**
		*  @brief
		*    Finds the closest point on a line to this point
		*
		*  @param[in] vV1
		*    Line start position
		*  @param[in] vV2
		*    Line end position
		*
		*  @return
		*    The closest point on line
		*/
		PLMATH_API Vector3 ClosestPointOnLine(const Vector3 &vV1, const Vector3 &vV2) const;

		/**
		*  @brief
		*    Check if a point is in a triangle
		*
		*  @param[in] vV1
		*    First triangle point
		*  @param[in] vV2
		*    Second triangle point
		*  @param[in] vV3
		*    Third triangle point
		*
		*  @return
		*    'true' if the point is in the triangle, else 'false'
		*/
		PLMATH_API bool IsPointInTriangle(const Vector3 &vV1, const Vector3 &vV2, const Vector3 &vV3) const;

		/**
		*  @brief
		*    Find the closest point on a triangle to this point
		*
		*  @param[in] vV1
		*    First triangle point
		*  @param[in] vV2
		*    Second triangle point
		*  @param[in] vV3
		*    Third triangle point
		*
		*  @return
		*    The closest point on triangle
		*/
		PLMATH_API Vector3 ClosestPointOnTriangle(const Vector3 &vV1, const Vector3 &vV2, const Vector3 &vV3) const;

		/**
		*  @brief
		*    Find the closest point on a triangle to this point
		*
		*  @param[in]  vV1
		*    First triangle point
		*  @param[in]  vV2
		*    Second triangle point
		*  @param[in]  vV3
		*    Third triangle point
		*  @param[in]  fRadius
		*    Point test radius
		*  @param[out] vClosest
		*    Will receive the closest point
		*
		*  @return
		*    'true' if closest point is valid, else 'false'
		*/
		PLMATH_API bool ClosestPointOnTriangle(const Vector3 &vV1, const Vector3 &vV2,
											   const Vector3 &vV3, float fRadius, Vector3 &vClosest) const;

		/**
		*  @brief
		*    Returns the 2D screen coordinate corresponding to this 3D coordinate
		*
		*  @param[in] mWorldViewProjection
		*    World view projection matrix to use
		*  @param[in] cViewportRectangle
		*    Viewport rectangle to use
		*  @param[in] fDepthRangeMin
		*    Depth range minimum
		*  @param[in] fDepthRangeMax
		*    Depth range maximum
		*  @param[in] pfHomogeneousDivide
		*    If not a null pointer, this receives the homogeneous divide
		*
		*  @return
		*    The corresponding 2D screen coordinate (x, y, z depth)
		*/
		PLMATH_API Vector3 To2DCoordinate(const Matrix4x4 &mWorldViewProjection, const Rectangle &cViewportRectangle,
										  float fDepthRangeMin = 0.0f, float fDepthRangeMax = 1.0f, float *pfHomogeneousDivide = nullptr) const;

		/**
		*  @brief
		*    Returns the 3D coordinate corresponding to this 2D screen coordinate
		*
		*  @param[in] mClipSpaceToObjectSpace
		*    The clip space to object space matrix to  use
		*  @param[in] cViewportRectangle
		*    Viewport rectangle to use
		*
		*  @return
		*    The corresponding 3D coordinate
		*
		*  @remarks
		*    The depth value stored within the z component of this vector should be between 0.0-1.0.
		*    Try to avoid using 0.0 & 1.0, instead add a small value like 0.0001 - else
		*    wrong results may be returned, especially if an infinite projection matrix is used.
		*/
		PLMATH_API Vector3 To3DCoordinate(const Matrix4x4 &mClipSpaceToObjectSpace, const Rectangle &cViewportRectangle) const;

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
#include "PLMath/Vector3.inl"
#include "PLMath/TypeVector3.inl"


#endif // __PLMATH_VECTOR3_H__
