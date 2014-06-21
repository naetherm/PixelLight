/*********************************************************\
 *  File: Vector2i.h                                     *
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


#ifndef __PLMATH_VECTOR2I_H__
#define __PLMATH_VECTOR2I_H__
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
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    2D integer vector
*/
class Vector2i {


	//[-------------------------------------------------------]
	//[ Public static data                                    ]
	//[-------------------------------------------------------]
	public:
		static PLMATH_API const Vector2i Zero;			/**<  0,  0 */
		static PLMATH_API const Vector2i One;			/**<  1,  1 */
		static PLMATH_API const Vector2i NegativeOne;	/**< -1, -1 */
		static PLMATH_API const Vector2i UnitX;			/**<  1,  0 */
		static PLMATH_API const Vector2i UnitY;			/**<  0,  1 */
		static PLMATH_API const Vector2i NegativeUnitX;	/**< -1,  0 */
		static PLMATH_API const Vector2i NegativeUnitY;	/**<  0, -1 */


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Some direct vector element accesses
		*/
		union {
			/**
			*  @brief
			*    Vector element array access
			*/
			int nV[2];

			/**
			*  @brief
			*    Known vector element names when dealing with positions or directions
			*/
			struct {
				int x, y;
			};

			/**
			*  @brief
			*    Known vector element names when dealing with sizes
			*/
			struct {
				int width, height;
			};
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor setting all components to 0
		*/
		inline Vector2i();

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nX
		*    X component
		*  @param[in] nY
		*    Y component
		*/
		inline Vector2i(int nX, int nY);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sString
		*    String to parse
		*/
		inline Vector2i(const PLCore::String &sString);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~Vector2i();

		//[-------------------------------------------------------]
		//[ Get and set                                           ]
		//[-------------------------------------------------------]
		inline operator int *();
		inline operator const int *() const;
		inline int &operator [](int nIndex);
		inline const int &operator [](int nIndex) const;

		/**
		*  @brief
		*    Set vector
		*
		*  @param[in] nX
		*    X component
		*  @param[in] nY
		*    Y component
		*/
		inline void Set(int nX, int nY);

		/**
		*  @brief
		*    Set X component
		*
		*  @param[in] nX
		*    X component
		*/
		inline void SetX(int nX);

		/**
		*  @brief
		*    Set Y component
		*
		*  @param[in] nY
		*    Y component
		*/
		inline void SetY(int nY);

		/**
		*  @brief
		*    Assignment operator
		*
		*  @param[in] vV
		*    Source vector to copy
		*
		*  @return
		*    This vector
		*/
		inline Vector2i &operator =(const Vector2i &vV);

		/**
		*  @brief
		*    Compare operator
		*
		*  @param[in] vV
		*    Vector to compare with
		*
		*  @return
		*    'true' if equal, else 'false'
		*/
		inline bool operator ==(const Vector2i &vV) const;

		/**
		*  @brief
		*    Compare operator
		*
		*  @param[in] vV
		*    Vector to compare with
		*
		*  @return
		*    'true' if not equal, else 'false'
		*/
		inline bool operator !=(const Vector2i &vV) const;

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
		*    - A lexicographical order for 2-dimensional vectors is used (see http://en.wikipedia.org/wiki/Ordered_vector_space)
		*/
		inline bool operator <(const Vector2i &vV) const;

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
		inline bool operator >(const Vector2i &vV) const;

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
		inline bool operator <=(const Vector2i &vV) const;

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
		inline bool operator >=(const Vector2i &vV) const;

		/**
		*  @brief
		*    Addition operator
		*
		*  @param[in] vV
		*    Vector to add
		*
		*  @return
		*    The resulting vector
		*/
		inline Vector2i operator +(const Vector2i &vV) const;

		/**
		*  @brief
		*    Addition operator
		*
		*  @param[in] vV
		*    Vector to add
		*
		*  @return
		*    This vector
		*/
		inline Vector2i &operator +=(const Vector2i &vV);

		/**
		*  @brief
		*    Subtraction operator
		*
		*  @param[in] vV
		*    Vector to subtract
		*
		*  @return
		*    The resulting vector
		*/
		inline Vector2i operator -(const Vector2i &vV) const;

		/**
		*  @brief
		*    Subtraction operator
		*
		*  @param[in] vV
		*    Vector to subtract
		*
		*  @return
		*    This vector
		*/
		inline Vector2i &operator -=(const Vector2i &vV);

		/**
		*  @brief
		*    Multiplication operator
		*
		*  @param[in] fFactor
		*    Floating point factor
		*
		*  @return
		*    The resulting vector
		*/
		inline Vector2i operator *(float fFactor) const;

		/**
		*  @brief
		*    Multiplication operator
		*
		*  @param[in] fFactor
		*    Floating point factor
		*
		*  @return
		*    This vector
		*/
		inline Vector2i &operator *=(float fFactor);

		/**
		*  @brief
		*    Division operator
		*
		*  @param[in] fFactor
		*    Floating point divisor
		*
		*  @return
		*    The resulting vector
		*/
		inline Vector2i operator /(float fFactor) const;

		/**
		*  @brief
		*    Division operator
		*
		*  @param[in] fFactor
		*    Floating point divisor
		*
		*  @return
		*    This vector
		*/
		inline Vector2i &operator /=(float fFactor);

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
#include "PLMath/Vector2i.inl"
#include "PLMath/TypeVector2i.inl"


#endif // __PLMATH_VECTOR2I_H__
