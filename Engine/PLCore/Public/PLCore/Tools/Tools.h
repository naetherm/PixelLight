/*********************************************************\
 *  File: Tools.h                                        *
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


#ifndef __PLCORE_TOOLS_H__
#define __PLCORE_TOOLS_H__
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
*    Static class with some useful tool functions
*/
class Tools {


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Conversion tools                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    float to uint32
		*
		*  @param[in] f
		*    Float value to convert
		*
		*  @return
		*    The converted float value
		*/
		static inline uint32 FloatToUInt32(float f);

		/**
		*  @brief
		*    uint32 to float
		*
		*  @param[in] n
		*    uint32 value to convert
		*
		*  @return
		*    The converted uint32 value
		*/
		static inline float UInt32ToFloat(uint32 n);

		//[-------------------------------------------------------]
		//[ String tools                                          ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Base64 encoder
		*
		*  @param[in] sString
		*    Unencoded string
		*
		*  @return
		*    Base64 encoded string
		*/
		static inline String GetBase64(const String &sString);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Tools/Tools.inl"


#endif // __PLCORE_TOOLS_H__
