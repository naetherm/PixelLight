/*********************************************************\
 *  File: EnumType.h                                     *
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


#ifndef __PLCORE_ENUMTYPE_H__
#define __PLCORE_ENUMTYPE_H__
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
*    Enum type wrapper (for plain enum types without RTTI interface)
*/
template <typename ENUM>
class EnumTypePlain {
};


/**
*  @brief
*    Enum type wrapper
*/
template <typename ENUM>
class EnumType {


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		typedef typename ENUM::_BaseType _BaseType;
		typedef typename ENUM::_Type	 _Type;			/**< Real type */
		typedef typename ENUM::_Type	 _StorageType;	/**< Storage type, for this type identical to the real type */


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		static int GetNumOfEnumValues() {
			String sEnum, sDesc;
			_Type nValue;
			int nIndex = 0;
			for (nIndex=0; ENUM::GetEnumValue(nIndex, nValue, sEnum, sDesc); nIndex++)
				;	// Nothing to do in here
			return nIndex;
		}

		static String GetEnumName(int nIndex) {
			String sEnum, sDesc;
			_Type nValue;
			ENUM::GetEnumValue(nIndex, nValue, sEnum, sDesc);
			return sEnum;
		}

		static _Type GetEnumValue(const String &sName) {
			String sEnum = sName;
			String sDesc;
			_Type nValue;
			ENUM::GetEnumValue(-1, nValue, sEnum, sDesc);
			return nValue;
		}

		static String GetEnumDescription(const String &sName) {
			String sEnum = sName;
			String sDesc;
			_Type nValue;
			ENUM::GetEnumValue(-1, nValue, sEnum, sDesc);
			return sDesc;
		}


};


/**
*  @brief
*    Flag type wrapper
*/
template <typename ENUM>
class FlagType {


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		typedef typename PLCore::uint32 	_BaseType;
		typedef typename ENUM::_Type       _Type;			/**< Real type */
		typedef typename ENUM::_Type       _StorageType;	/**< Storage type, for this type identical to the real type */


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		static int GetNumOfEnumValues() {
			String sEnum, sDesc;
			_Type nValue;
			int nIndex = 0;
			for (nIndex=0; ENUM::GetEnumValue(nIndex, nValue, sEnum, sDesc); nIndex++)
				;	// Nothing to do in here
			return nIndex;
		}

		static String GetEnumName(int nIndex) {
			String sEnum, sDesc;
			_Type nValue;
			ENUM::GetEnumValue(nIndex, nValue, sEnum, sDesc);
			return sEnum;
		}

		static _Type GetEnumValue(const String &sName) {
			String sEnum = sName;
			String sDesc;
			_Type nValue;
			ENUM::GetEnumValue(-1, nValue, sEnum, sDesc);
			return nValue;
		}

		static String GetEnumDescription(const String &sName) {
			String sEnum = sName;
			String sDesc;
			_Type nValue;
			ENUM::GetEnumValue(-1, nValue, sEnum, sDesc);
			return sDesc;
		}


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_ENUMTYPE_H__
