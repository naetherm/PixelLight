/*********************************************************\
 *  File: Wrapper.h                                      *
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


#ifndef __PLCORE_WRAPPER_H__
#define __PLCORE_WRAPPER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/PLCore.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Static class with some few simple wrappers you can use to avoid for instance nasty standard
*    header includes were they are undesired
*/
class Wrapper {


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the number of characters of a c-string excluding the terminating zero
		*
		*  @param[in] szString
		*    C-string to check
		*
		*  @return
		*    The number of characters of the given c-string excluding the terminating zero
		*
		*  @note
		*    - 'strlen'-wrapper
		*/
		static PLCORE_API uint32 GetStringLength(const char szString[]);

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
		static PLCORE_API float Ceil(float fValue);

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
		static PLCORE_API float Log(float x);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_WRAPPER_H__
