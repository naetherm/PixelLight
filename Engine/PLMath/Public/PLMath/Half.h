/*********************************************************\
 *  File: Half.h                                         *
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


#ifndef __PLMATH_HALF_H__
#define __PLMATH_HALF_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
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
*    Static helper class for the half data type (16 bit floating point)
*
*  @note
*    - This class is using information from "Fast Half Float Conversions" (ftp://ftp.fox-toolkit.org/pub/fasthalffloatconversion.pdf)
*      written by Jeroen van der Zijp, November 2008 (Revised September 2010)
*    - OpenEXR IlmBase 1.0.2 (Modified BSD License)
*/
class Half {


	//[-------------------------------------------------------]
	//[ Public static data                                    ]
	//[-------------------------------------------------------]
	public:
		// Some half numbers
		static PLMATH_API const PLCore::uint16 Zero;				/**< Representation of 0.0 */
		static PLMATH_API const PLCore::uint16 One;					/**< Representation of 1.0 */

		// Important values
		static PLMATH_API const float SmallestPositive;				/**< Smallest positive half (5.96046448e-08f) */
		static PLMATH_API const float SmallestPositiveNormalized;	/**< Smallest positive normalized half (6.10351562e-05f) */
		static PLMATH_API const float LargestPositive;				/**< Largest positive half (65504.0f) */
		static PLMATH_API const float Epsilon;						/**< Smallest positive epsilon for which 1+e!=1 (0.00097656f) */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns whether or not the given half value is zero
		*
		*  @param[in] nHalf
		*    Half value to check
		*
		*  @return
		*    'true' if the given half value is zero, else 'false'
		*/
		static inline bool IsZero(PLCore::uint16 nHalf);

		/**
		*  @brief
		*    Returns whether or not the given half value is negative
		*
		*  @param[in] nHalf
		*    Half value to check
		*
		*  @return
		*    'true' if the given half value is negative, else 'false'
		*/
		static inline bool IsNegative(PLCore::uint16 nHalf);

		/**
		*  @brief
		*    Returns whether or not the given half value is not a number (NAN)
		*
		*  @param[in] nHalf
		*    Half value to check
		*
		*  @return
		*    'true' if the given half value is not a number, else 'false'
		*/
		static inline bool IsNotANumber(PLCore::uint16 nHalf);

		/**
		*  @brief
		*    Returns whether or not the given half value is finite
		*
		*  @param[in] nHalf
		*    Half value to check
		*
		*  @return
		*    'true' if the given half value is finite, else 'false'
		*/
		static inline bool IsFinite(PLCore::uint16 nHalf);

		/**
		*  @brief
		*    Returns whether or not the given half value is infinity
		*
		*  @param[in] nHalf
		*    Half value to check
		*
		*  @return
		*    'true' if the given half value is infinity, else 'false'
		*/
		static inline bool IsInfinity(PLCore::uint16 nHalf);

		/**
		*  @brief
		*    Returns whether or not the given half value is normalized
		*
		*  @param[in] nHalf
		*    Half value to check
		*
		*  @return
		*    'true' if the given half value is normalized, else 'false'
		*/
		static inline bool IsNormalized(PLCore::uint16 nHalf);

		/**
		*  @brief
		*    Returns whether or not the given half value is de-normalized
		*
		*  @param[in] nHalf
		*    Half value to check
		*
		*  @return
		*    'true' if the given half value is de-normalized, else 'false'
		*/
		static inline bool IsDenormalized(PLCore::uint16 nHalf);

		/**
		*  @brief
		*    Converts a given half value into a float value
		*
		*  @param[in] nHalf
		*    Half value to convert
		*
		*  @return
		*    The given half value as float
		*/
		static PLMATH_API float ToFloat(PLCore::uint16 nHalf);

		/**
		*  @brief
		*    Converts a given float value into a half value
		*
		*  @param[in] fFloat
		*    Float value to convert
		*
		*  @return
		*    The given float value as half
		*/
		static PLMATH_API PLCore::uint16 FromFloat(float fFloat);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLMath/Half.inl"


#endif // __PLMATH_HALF_H__
