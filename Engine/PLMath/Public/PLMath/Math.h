/*********************************************************\
 *  File: Math.h                                         *
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


#ifndef __PLMATH_MATH_H__
#define __PLMATH_MATH_H__
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
*    Static class with some useful math constants and functions
*/
class Math {


	//[-------------------------------------------------------]
	//[ Public static data                                    ]
	//[-------------------------------------------------------]
	public:
		static PLMATH_API const double Pi;			/**< 3.14159265358979323846 */
		static PLMATH_API const double Pi2;			/**< 6.28318530717958623200 (Pi*2) */
		static PLMATH_API const double PiHalf;		/**< 1.57079632679489655800 (Pi/2) */
		static PLMATH_API const double DegToRad;	/**< 0.01745329251994329576 (Pi/180) */
		static PLMATH_API const double RadToDeg;	/**< 57.2957795130823208768 (180/Pi) */
		static PLMATH_API const double Epsilon;		/**< 1.192092896e-07F */
		static PLMATH_API const double Log2Const;	/**< 0.693147180559945309417 */
		static PLMATH_API const int    RandMax;		/**< Maximum random value (0x7fff) */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Floating point                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Checks whether the given float-precision floating-point value is not a number (NAN)
		*
		*  @return
		*    'true' if the value is not a number, else 'false'
		*/
		static inline bool IsNotANumber(float fValue);

		/**
		*  @brief
		*    Checks whether the given double-precision floating-point value is not a number (NAN)
		*
		*  @return
		*    'true' if the value is not a number, else 'false'
		*/
		static inline bool IsNotANumber(double dValue);

		/**
		*  @brief
		*    Checks whether the given float-precision floating-point value is finite
		*
		*  @return
		*    'true' if the value is finite, (–INF < fValue < +INF) else 'false' (infinite or not a number)
		*/
		static inline bool IsFinite(float fValue);

		/**
		*  @brief
		*    Checks whether the given double-precision floating-point value is finite
		*
		*  @return
		*    'true' if the value is finite, (–INF < dValue < +INF) else 'false' (infinite or not a number)
		*/
		static inline bool IsFinite(double dValue);

		//[-------------------------------------------------------]
		//[ Nearest power of two                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Check's whether the given the number is a power of 2
		*
		*  @param[in] nNumber
		*    Number to check
		*
		*  @return
		*    'true' if the number is a power of 2, else 'false'
		*/
		static inline bool IsPowerOfTwo(PLCore::uint32 nNumber);

		/**
		*  @brief
		*    Returns the nearest power of 2
		*
		*  @param[in] nNumber
		*    Number to check
		*  @param[in] bLower
		*    'true' take the lower nearest number, else 'false' takes the higher one
		*
		*  @return
		*    The nearest power of 2, if it couldn't be found 'nNumber'
		*/
		static inline PLCore::uint32 GetNearestPowerOfTwo(PLCore::uint32 nNumber, bool bLower = true);

		//[-------------------------------------------------------]
		//[ Random                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns a positive random number
		*
		*  @return
		*    A positive random number
		*/
		static inline PLCore::uint32 GetRand();

		/**
		*  @brief
		*    Returns a positive or negative random number
		*
		*  @return
		*    A positive or negative random number
		*/
		static inline int GetRandNeg();

		/**
		*  @brief
		*    Returns a positive float random number between 0..1
		*
		*  @return
		*    A positive float random number between 0..1
		*/
		static inline float GetRandFloat();

		/**
		*  @brief
		*    Returns a positive or negative float random number between -1..1
		*
		*  @return
		*    A positive or negative float random number between -1..1
		*/
		static inline float GetRandNegFloat();

		/**
		*  @brief
		*    Returns a float random number between the given minimum/maximum
		*
		*  @param[in] fMin
		*    Minimum (inclusive), must be <= maximum
		*  @param[in] fMax
		*    Maximum (inclusive), must be >= minimum
		*
		*  @return
		*    A float random number between the given minimum/maximum
		*/
		static inline float GetRandMinMaxFloat(float fMin, float fMax);

		//[-------------------------------------------------------]
		//[ Misc                                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns a whether to given numbers are equal or not
		*
		*  @param[in] f1
		*    First number
		*  @param[in] f2
		*    Second number
		*  @param[in] fEpsilon
		*    Error tolerance
		*
		*  @return
		*    'true' if the given numbers are equal, else 'false'
		*/
		static inline bool AreEqual(float f1, float f2, float fEpsilon = Math::Epsilon);

		/**
		*  @brief
		*    Returns a whether to given numbers are equal or not
		*
		*  @param[in] d1
		*    First number
		*  @param[in] d2
		*    Second number
		*  @param[in] dEpsilon
		*    Error tolerance
		*
		*  @return
		*    'true' if the given numbers are equal, else 'false'
		*/
		static inline bool AreEqual(double d1, double d2, double dEpsilon = Math::Epsilon);

		/**
		*  @brief
		*    Returns the minimum of the 2 given values
		*
		*  @param[in] a
		*    First value
		*  @param[in] b
		*    Second value
		*
		*  @return
		*    The minimum of the 2 given values
		*/
		static inline float Min(float a, float b);
		static inline int Min(int a, int b);
		static inline PLCore::uint32 Min(PLCore::uint32 a, PLCore::uint32 b);

		/**
		*  @brief
		*    Returns the maximum of the 2 given values
		*
		*  @param[in] a
		*    First value
		*  @param[in] b
		*    Second value
		*
		*  @return
		*    The maximum of the 2 given values
		*/
		static inline float Max(float a, float b);
		static inline int Max(int a, int b);
		static inline PLCore::uint32 Max(PLCore::uint32 a, PLCore::uint32 b);

		/**
		*  @brief
		*    Returns the minimum of the 4 given values
		*
		*  @param[in] a
		*    First value
		*  @param[in] b
		*    Second value
		*  @param[in] c
		*    Third value
		*  @param[in] d
		*    Fourth value
		*
		*  @return
		*    The minimum of the 4 given values
		*/
		static inline float Min(float a, float b, float c, float d);
		static inline int Min(int a, int b, int c, int d);
		static inline PLCore::uint32 Min(PLCore::uint32 a, PLCore::uint32 b, PLCore::uint32 c, PLCore::uint32 d);

		/**
		*  @brief
		*    Returns the maximum of the 4 given values
		*
		*  @param[in] a
		*    First value
		*  @param[in] b
		*    Second value
		*  @param[in] c
		*    Third value
		*  @param[in] d
		*    Fourth value
		*
		*  @return
		*    The maximum of the 4 given values
		*/
		static inline float Max(float a, float b, float c, float d);
		static inline int Max(int a, int b, int c, int d);
		static inline PLCore::uint32 Max(PLCore::uint32 a, PLCore::uint32 b, PLCore::uint32 c, PLCore::uint32 d);

		/**
		*  @brief
		*    Returns the sign of the given value
		*
		*  @param[in] x
		*    Value to check
		*
		*  @return
		*    '-1' if the given value is negative, else '1'
		*/
		static inline float Sign(float x);

		/**
		*  @brief
		*    Ensures that the given value is within the given interval [fMin, fMax] by clamping the value
		*
		*  @param[in] fValue
		*    Value to check
		*  @param[in] fMin
		*    Minimum of the interval, must be < fMax!
		*  @param[in] fMax
		*    Maximum of the interval, must be > fMin!
		*
		*  @return
		*    The value within the interval [fMin, fMax]
		*
		*  @note
		*    - In case of violating the interface specification by swapping minimum/maximum, minimum/maximum is swapped internally in order to avoid chaos
		*/
		static inline float ClampToInterval(float fValue, float fMin, float fMax);

		/**
		*  @brief
		*    Ensures that the given value is within the given interval [fMin, fMax] by wrapping the value
		*
		*  @param[in] fValue
		*    Value to check
		*  @param[in] fMin
		*    Minimum of the interval, must be < fMax!
		*  @param[in] fMax
		*    Maximum of the interval, must be > fMin!
		*
		*  @return
		*    The value within the interval [fMin, fMax]
		*
		*  @note
		*    - In case of violating the interface specification by swapping minimum/maximum the result will not be different to the one of the correct order
		*/
		static inline float WrapToInterval(float fValue, float fMin, float fMax);

		/**
		*  @brief
		*    Returns the absolute value of the given value
		*
		*  @param[in] x
		*    Value to return the absolute value from
		*
		*  @return
		*    Absolute value of the given value
		*/
		static inline int Abs(int x);

		/**
		*  @brief
		*    Returns the absolute value of the given value
		*
		*  @param[in] x
		*    Value to return the absolute value from
		*
		*  @return
		*    Absolute value of the given value
		*/
		static inline float Abs(float x);

		/**
		*  @brief
		*    Returns the absolute value of the given value
		*
		*  @param[in] x
		*    Value to return the absolute value from
		*
		*  @return
		*    Absolute value of the given value
		*/
		static inline double Abs(double x);

		/**
		*  @brief
		*    Swaps two given values
		*
		*  @param[in, out] a
		*    First value
		*  @param[in, out] b
		*    Second value
		*/
		static inline void Swap(float &a, float &b);

		/**
		*  @brief
		*    Clamps the value between 0.0 and 1.0
		*
		*  @param[in] fValue
		*    Value to clamp
		*
		*  @return
		*    Clamped value
		*/
		static inline float Saturate(float fValue);

		/**
		*  @brief
		*    Returns the floor of the given value
		*
		*  @param[in] fValue
		*    Value to return the floor from
		*
		*  @return
		*    The floor of the given value
		*
		*  @remarks
		*    This function returns a floating-point value representing the largest integer value that
		*    is less than or equal to the given value.
		*/
		static inline float Floor(float fValue);

		/**
		*  @brief
		*    Returns the ceil of the given value
		*
		*  @param[in] fValue
		*    Value to return the ceil from
		*
		*  @return
		*    The ceil of the given value
		*
		*  @remarks
		*    This function returns a floating-point value representing the smallest integer value
		*    that is greater than or equal to the given value.
		*/
		static inline float Ceil(float fValue);

		/**
		*  @brief
		*    Returns the rounded value of the given value
		*
		*  @param[in] fValue
		*    Value to return the rounded value from
		*  @param[in] nPrecision
		*    Number of digits after the comma
		*
		*  @return
		*    The rounded value of the given value
		*/
		static inline float Round(float fValue, PLCore::uint32 nPrecision = 0);

		/**
		*  @brief
		*    Returns the sine of a given angle
		*
		*  @param[in] fAngle
		*    Angle in radians
		*
		*  @return
		*    Sine of the given angle
		*
		*  @note
		*    - If the given angle is greater than or equal to 263, or less
		*      than or equal to –263, a loss of significance in the result occurs
		*    - Sine of 0 is 0
		*/
		static inline float Sin(float fAngle);

		/**
		*  @brief
		*    Returns the sine of a given angle
		*
		*  @param[in] dAngle
		*    Angle in radians
		*
		*  @return
		*    Sine of the given angle
		*
		*  @see
		*    - Sin(float) above
		*/
		static inline double Sin(double dAngle);

		/**
		*  @brief
		*    Returns the arcsine of a given value
		*
		*  @param[in] fValue
		*    Value between –1 and 1 whose arcsine is to be calculated
		*
		*  @return
		*    Arcsine of the given value
		*
		*  @remarks
		*    This function returns the arcsine of the given value in the range –p/2 to p/2 radians.
		*    If the given value is less than –1 or greater than 1, ASin returns an indefinite
		*    (same as a quiet NaN).
		*/
		static inline float ASin(float fValue);

		/**
		*  @brief
		*    Returns the arcsine of a given value
		*
		*  @param[in] dValue
		*    Value between –1 and 1 whose arcsine is to be calculated
		*
		*  @return
		*    Arcsine of the given value
		*
		*  @see
		*    - ASin(float) above
		*/
		static inline double ASin(double dValue);

		/**
		*  @brief
		*    Returns the cosine (cos) of a given angle
		*
		*  @param[in] fAngle
		*    Angle in radians
		*
		*  @return
		*    Cosine of the given angle
		*
		*  @note
		*    - If the given angle is greater than or equal to 263, or less
		*      than or equal to –263, a loss of significance in the result occurs
		*    - Cosine of 0 is 1
		*/
		static inline float Cos(float fAngle);

		/**
		*  @brief
		*    Returns the cosine (cos) of a given angle
		*
		*  @param[in] dAngle
		*    Angle in radians
		*
		*  @return
		*    Cosine of the given angle
		*
		*  @see
		*    - Cos(float) above
		*/
		static inline double Cos(double dAngle);

		/**
		*  @brief
		*    Returns the arccosine of a given value
		*
		*  @param[in] fValue
		*    Value between –1 and 1 whose arccosine is to be calculated
		*
		*  @return
		*    Arccosine of the given value
		*
		*  @remarks
		*    This function returns the arccosine of the given value in the range –p/2 to p/2 radians.
		*    If the given value is less than –1 or greater than 1, ACos returns an indefinite
		*    (same as a quiet NaN).
		*/
		static inline float ACos(float fValue);

		/**
		*  @brief
		*    Returns the arccosine of a given value
		*
		*  @param[in] dValue
		*    Value between –1 and 1 whose arccosine is to be calculated
		*
		*  @return
		*    Arccosine of the given value
		*
		*  @see
		*    - ACos(float) above
		*/
		static inline double ACos(double dValue);

		/**
		*  @brief
		*    Returns the tangent (tan) of a given angle
		*
		*  @param[in] fAngle
		*    Angle in radians
		*
		*  @return
		*    Tangent of the given angle
		*
		*  @note
		*    - If the given angle is greater than or equal to 263, or less
		*      than or equal to –263, a loss of significance in the result occurs
		*    - Tangent of 0 is 0
		*/
		static inline float Tan(float fAngle);

		/**
		*  @brief
		*    Returns the tangent (tan) of a given angle
		*
		*  @param[in] dAngle
		*    Angle in radians
		*
		*  @return
		*    Tangent of the given angle
		*
		*  @see
		*    - Tan(float) above
		*/
		static inline double Tan(double dAngle);

		/**
		*  @brief
		*    Returns the arctangent of a given value
		*
		*  @param[in] fValue
		*    Value between –1 and 1 whose arccosine is to be calculated
		*
		*  @return
		*    Arccosine of the given value
		*/
		static inline float ATan(float fValue);

		/**
		*  @brief
		*    Returns the arctangent of a given value
		*
		*  @param[in] dValue
		*    Value between –1 and 1 whose arccosine is to be calculated
		*
		*  @return
		*    Arccosine of the given value
		*/
		static inline double ATan(double dValue);

		/**
		*  @brief
		*    Returns the arctangent of of y/x
		*
		*  @param[in] fX
		*    Any number
		*  @param[in] fY
		*    Any number
		*
		*  @return
		*    Arccosine of y/x
		*
		*  @remarks
		*    Returns a value in the range –p to p radians, using the signs of both parameters
		*    to determine the quadrant of the return value
		*
		*  @note
		*    - If fX is 0, this function returns 0
		*    - If both parameters are 0, the function returns 0
		*/
		static inline float ATan2(float fX, float fY);

		/**
		*  @brief
		*    Returns the arctangent of of y/x
		*
		*  @param[in] dX
		*    Any number
		*  @param[in] dY
		*    Any number
		*
		*  @return
		*    Arccosine of y/x
		*
		*  @see
		*    - ATan2(float, float) above
		*/
		static inline double ATan2(double dX, double dY);

		/**
		*  @brief
		*    Returns the square-root of a given value
		*
		*  @param[in] fValue
		*    Nonnegative floating-point value, if negative, this function returns an indefinite
		*    (same as a quiet NaN)
		*
		*  @return
		*    The square-root of the given value, indefinite on error
		*/
		static inline float Sqrt(float fValue);

		/**
		*  @brief
		*    Returns the square-root of a given value
		*
		*  @param[in] dValue
		*    Nonnegative floating-point value, if negative, this function returns an indefinite
		*    (same as a quiet NaN)
		*
		*  @return
		*    The square-root of the given value, indefinite on error
		*/
		static inline double Sqrt(double dValue);

		/**
		*  @brief
		*    Returns the inverse square-root of a given value
		*
		*  @param[in] fValue
		*    Nonnegative floating-point value, if negative, this function returns an indefinite
		*    (same as a quiet NaN)
		*
		*  @return
		*    The inverse square-root of the given value, indefinite on error
		*
		*  @remarks
		*    This method is slightly faster then writing "1/Sqrt(<value>)", but may return invalid results.
		*    So, be careful when using this method and only use it when you're sure that the results are
		*    correct in your use case.
		*/
		static inline float FastInvSqrt(float fValue);

		/**
		*  @brief
		*    Returns x raised to the power of y
		*
		*  @param[in] x
		*    Base
		*  @param[in] y
		*    Exponent 
		*
		*  @return
		*    x raised to the power of y
		*/
		static inline float Pow(float x, float y);

		/**
		*  @brief
		*    Returns x raised to the power of y
		*
		*  @param[in] x
		*    Base
		*  @param[in] y
		*    Exponent
		*
		*  @return
		*    x raised to the power of y
		*/
		static inline double Pow(double x, double y);

		/**
		*  @brief
		*    Returns x raised to the power of y
		*
		*  @param[in] x
		*    Base
		*  @param[in] y
		*    Exponent
		*
		*  @return
		*    x raised to the power of y
		*/
		static inline double Pow(int x, int y);

		/**
		*  @brief
		*    Returns the natural logarithm of a given number
		*
		*  @param[in] x
		*    Given number
		*
		*  @return
		*    The natural logarithm of the given number
		*/
		static inline float Log(float x);

		/**
		*  @brief
		*    Returns the natural logarithm of a given number
		*
		*  @param[in] x
		*    Given number
		*
		*  @return
		*    The natural logarithm of the given number
		*/
		static inline double Log(double x);

		/**
		*  @brief
		*    Returns the base 2 logarithm for a given number, as long as it is not negative, or zero
		*
		*  @param[in] x
		*    Given number
		*
		*  @return
		*    The base 2 logarithm for the given number, as long as it is not negative, or zero
		*
		*  @remarks
		*    Log(x)/Log(2)
		*/
		static inline float Log2(float x);

		/**
		*  @brief
		*    Returns the base 2 logarithm for a given number, as long as it is not negative, or zero
		*
		*  @param[in] x
		*    Given number
		*
		*  @return
		*    The base 2 logarithm for the given number, as long as it is not negative, or zero
		*
		*  @remarks
		*    Log(x)/Log(2)
		*/
		static inline double Log2(double x);

		/**
		*  @brief
		*    Returns the e number raised to the power x
		*
		*  @param[in] x
		*    Given number
		*
		*  @return
		*    The e number raised to the power x
		*/
		static inline float Exp(float x);

		/**
		*  @brief
		*    Returns the e number raised to the power x
		*
		*  @param[in] x
		*    Given number
		*
		*  @return
		*    The e number raised to the power x
		*/
		static inline double Exp(double x);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLMath/Math.inl"


#endif // __PLMATH_MATH_H__
