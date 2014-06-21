/*********************************************************\
 *  File: ParseTools.h                                   *
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


#ifndef __PLCORE_PARSETOOLS_H__
#define __PLCORE_PARSETOOLS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/String.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Static class with some useful string parse tool functions
*/
class ParseTools {


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Parse string containing a hex value to an integer (for example "FF" => "255")
		*
		*  @param[in] sString
		*    String containing a hex value
		*
		*  @return
		*    Integer representation of the given hex value
		*/
		static PLCORE_API uint32 ParseHexValue(const String &sString);

		/**
		*  @brief
		*    Parse an array of integer values
		*
		*  @param[in]  sString
		*    String containing integer numbers
		*  @param[out] pnValues
		*    Pointer to C-array which will receive the integer values, if a null pointer nothing happens
		*  @param[in]  nMaxNumOfElements
		*    The maximum number of elements 'nValues' can hold
		*
		*  @return
		*    The number of read values
		*
		*  @note
		*    - Example string with three values: "0 5 34"
		*/
		static PLCORE_API int ParseIntegerArray(const String &sString, int *pnValues, uint32 nMaxNumOfElements);

		/**
		*  @brief
		*    Parse an array of float values
		*
		*  @param[in]  sString
		*    String containing floating point numbers
		*  @param[out] pfValues
		*    Pointer to C-array which will receive the float values, if a null pointer nothing happens
		*  @param[in]  nMaxNumOfElements
		*    The maximum number of elements 'fValues' can hold
		*
		*  @return
		*    The number of read values
		*
		*  @note
		*    - Example string with three values: "0 0.0 34.245"
		*/
		static PLCORE_API int ParseFloatArray(const String &sString, float *pfValues, uint32 nMaxNumOfElements);

		/**
		*  @brief
		*    Parse an array of double values
		*
		*  @param[in]  sString
		*    String containing double precision floating point numbers
		*  @param[out] pdValues
		*    Pointer to C-array which will receive the double values, if a null pointer nothing happens
		*  @param[in]  nMaxNumOfElements
		*    The maximum number of elements 'dValues' can hold
		*
		*  @return
		*    The number of read values
		*
		*  @note
		*    - Example string with three values: "0 0.0 34.245"
		*/
		static PLCORE_API int ParseDoubleArray(const String &sString, double *pdValues, uint32 nMaxNumOfElements);

		//[-------------------------------------------------------]
		//[ Flags                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the flags from a string
		*
		*  @param[in] sFlags
		*    String containing flags
		*
		*  @return
		*    The flags from the string
		*
		*  @remarks
		*    Flags can be stored in strings too. (easy to edit :)\n
		*    Example: "1|2|4|8"
		*/
		static PLCORE_API uint32 GetFlagsFromString(const String &sFlags);

		/**
		*  @brief
		*    Creates a string from flags
		*
		*  @param[in] nFlags
		*    Flags value
		*
		*  @return
		*    String containing the flags in textual form
		*
		*  @see
		*    - GetFlagsFromString()
		*/
		static PLCORE_API String GetStringFromFlags(uint32 nFlags);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_PARSETOOLS_H__
