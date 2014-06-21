/*********************************************************\
 *  File: Functions.h                                    *
 *      Different useful container functions
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


#ifndef __PLCORE_CONTAINER_FUNCTIONS_H__
#define __PLCORE_CONTAINER_FUNCTIONS_H__
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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class String;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Different standard hash functions
*/
class HashFunction {


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		static PLCORE_API uint32 Hash(const int32 &nKey);
		static PLCORE_API uint32 Hash(const uint32 &nKey);
		static PLCORE_API uint32 Hash(const int64 &nKey);
		static PLCORE_API uint32 Hash(const uint64 &nKey);
		static PLCORE_API uint32 Hash(const char *pszKey);
		static PLCORE_API uint32 Hash(const String &sKey);


};

/**
*  @brief
*    Standard compare function
*/
class CompareFunction {


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		// int32
		static PLCORE_API bool AreEqual(const int32 &nKey1, const int32 &nKey2);
		static PLCORE_API bool IsGreater(const int32 &nKey1, const int32 &nKey2);

		// uint32
		static PLCORE_API bool AreEqual(const uint32 &nKey1, const uint32 &nKey2);
		static PLCORE_API bool IsGreater(const uint32 &nKey1, const uint32 &nKey2);

		// int64
		static PLCORE_API bool AreEqual(const int64 &nKey1, const int64 &nKey2);
		static PLCORE_API bool IsGreater(const int64 &nKey1, const int64 &nKey2);

		// uint64
		static PLCORE_API bool AreEqual(const uint64 &nKey1, const uint64 &nKey2);
		static PLCORE_API bool IsGreater(const uint64 &nKey1, const uint64 &nKey2);

		// float
		static PLCORE_API bool AreEqual(const float &fKey1, const float &fKey2);
		static PLCORE_API bool IsGreater(const float &fKey1, const float &fKey2);

		// const char *
		static PLCORE_API bool AreEqual(const char *pszKey1, const char *pszKey2);
		static PLCORE_API bool IsGreater(const char *pszKey1, const char *pszKey2);

		// String
		static PLCORE_API bool AreEqual(const String &sKey1, const String &sKey2);
		static PLCORE_API bool IsGreater(const String &sKey1, const String &sKey2);


};

/**
*  @brief
*    Standard grow functions
*
*  @remarks
*    The default grow function makes sure that there are at least 10% slots, if not, a new
*    size basing on prime numbers is returned.
*/
class GrowFunction {


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns a new size
		*
		*  @param[in] nCurrentSize
		*    Current size
		*  @param[in] nFreeSlots
		*    Number of free slots
		*
		*  @return
		*    New size, if <= current size then no grow is required
		*/
		static PLCORE_API uint32 GetNewSize(uint32 nCurrentSize, uint32 nFreeSlots);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_CONTAINER_FUNCTIONS_H__
