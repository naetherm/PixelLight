/*********************************************************\
 *  File: Half.inl                                       *
 *      Half inline implementation
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
/**
*  @brief
*    Returns whether or not the given half value is zero
*/
inline bool Half::IsZero(PLCore::uint16 nHalf)
{
	return ((nHalf & 0x7fff) == 0);
}

/**
*  @brief
*    Returns whether or not the given half value is negative
*/
inline bool Half::IsNegative(PLCore::uint16 nHalf)
{
	return ((nHalf & 0x8000) != 0);
}

/**
*  @brief
*    Returns whether or not the given half value is not a number (NAN)
*/
inline bool Half::IsNotANumber(PLCore::uint16 nHalf)
{
	const PLCore::uint16 nExponent = ((nHalf >> 10) & 0x001f);
	const PLCore::uint16 nMantissa = (nHalf & 0x3ff);
	return (nExponent == 31 && nMantissa != 0);
}

/**
*  @brief
*    Returns whether or not the given half value is finite
*/
inline bool Half::IsFinite(PLCore::uint16 nHalf)
{
	const PLCore::uint16 nExponent = ((nHalf >> 10) & 0x001f);
	return (nExponent < 31);
}

/**
*  @brief
*    Returns whether or not the given half value is infinity
*/
inline bool Half::IsInfinity(PLCore::uint16 nHalf)
{
	const PLCore::uint16 nExponent = ((nHalf >> 10) & 0x001f);
	const PLCore::uint16 nMantissa = (nHalf & 0x3ff);
	return (nExponent == 31 && nMantissa == 0);
}

/**
*  @brief
*    Returns whether or not the given half value is normalized
*/
inline bool Half::IsNormalized(PLCore::uint16 nHalf)
{
	const PLCore::uint16 nExponent = ((nHalf >> 10) & 0x001f);
	return (nExponent > 0 && nExponent < 31);
}

/**
*  @brief
*    Returns whether or not the given half value is de-normalized
*/
inline bool Half::IsDenormalized(PLCore::uint16 nHalf)
{
	const PLCore::uint16 nExponent = ((nHalf >> 10) & 0x001f);
	const PLCore::uint16 nMantissa = (nHalf & 0x3ff);
	return (nExponent == 0 && nMantissa != 0);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
