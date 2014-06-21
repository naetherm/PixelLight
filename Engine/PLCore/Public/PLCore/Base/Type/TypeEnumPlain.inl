/*********************************************************\
 *  File: TypeEnumPlain.inl                              *
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


#ifndef __PLCORE_TYPE_ENUMPLAIN_INL__
#define __PLCORE_TYPE_ENUMPLAIN_INL__
#pragma once


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Type wrapper for enum types
*/
template <typename ENUM>
class Type< EnumTypePlain<ENUM> > {


	//[-------------------------------------------------------]
	//[ Public static type information                        ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef ENUM _Type;			/**< Real type */
		typedef ENUM _StorageType;	/**< Storage type, for this type identical to the real type */

		// Type ID
		static const int TypeID = Type<int>::TypeID;

		// Get type name
		static String GetTypeName()
		{
			return Type<int>::GetTypeName();
		}

		// Convert var to type
		static ENUM ConvertFromVar(const DynVar &cValue)
		{
			return static_cast<ENUM>(Type<int>::ConvertFromVar(cValue));
		}

		// Convert type to bool
		static bool ConvertToBool(ENUM Value)
		{
			return Type<int>::ConvertToBool(static_cast<int>(Value));
		}

		// Convert bool to type
		static ENUM ConvertFromBool(bool bValue)
		{
			return static_cast<ENUM>(Type<int>::ConvertFromBool(bValue));
		}

		// Convert type to int
		static int ConvertToInt(ENUM Value)
		{
			return Type<int>::ConvertToInt(static_cast<int>(Value));
		}

		// Convert int to type
		static ENUM ConvertFromInt(int nValue)
		{
			return static_cast<ENUM>(Type<int>::ConvertFromInt(nValue));
		}

		// Convert type to int8
		static int8 ConvertToInt8(ENUM Value)
		{
			return Type<int>::ConvertToInt8(static_cast<int>(Value));
		}

		// Convert int8 to type
		static ENUM ConvertFromInt8(int8 nValue)
		{
			return static_cast<ENUM>(Type<int>::ConvertFromInt8(nValue));
		}

		// Convert type to int16
		static int16 ConvertToInt16(ENUM Value)
		{
			return Type<int>::ConvertToInt16(static_cast<int>(Value));
		}

		// Convert int16 to type
		static ENUM ConvertFromInt16(int16 nValue)
		{
			return static_cast<ENUM>(Type<int>::ConvertFromInt16(nValue));
		}

		// Convert type to int32
		static int32 ConvertToInt32(ENUM Value)
		{
			return Type<int>::ConvertToInt32(static_cast<int>(Value));
		}

		// Convert int32 to type
		static ENUM ConvertFromInt32(int32 nValue)
		{
			return static_cast<ENUM>(Type<int>::ConvertFromInt32(nValue));
		}

		// Convert type to int64
		static int64 ConvertToInt64(ENUM Value)
		{
			return Type<int>::ConvertToInt64(static_cast<int>(Value));
		}

		// Convert int64 to type
		static ENUM ConvertFromInt64(int64 nValue)
		{
			return static_cast<ENUM>(Type<int>::ConvertFromInt64(nValue));
		}

		// Convert type to uint8
		static uint8 ConvertToUInt8(ENUM Value)
		{
			return Type<int>::ConvertToUInt8(static_cast<int>(Value));
		}

		// Convert uint8 to type
		static ENUM ConvertFromUInt8(uint8 nValue)
		{
			return static_cast<ENUM>(Type<int>::ConvertFromUInt8(nValue));
		}

		// Convert type to uint16
		static uint16 ConvertToUInt16(ENUM Value)
		{
			return Type<int>::ConvertToUInt16(static_cast<int>(Value));
		}

		// Convert uint16 to type
		static ENUM ConvertFromUInt16(uint16 nValue)
		{
			return static_cast<ENUM>(Type<int>::ConvertFromUInt16(nValue));
		}

		// Convert type to uint32
		static uint32 ConvertToUInt32(ENUM Value)
		{
			return Type<int>::ConvertToUInt32(static_cast<int>(Value));
		}

		// Convert uint32 to type
		static ENUM ConvertFromUInt32(uint32 nValue)
		{
			return static_cast<ENUM>(Type<int>::ConvertFromUInt32(nValue));
		}

		// Convert type to uint64
		static uint64 ConvertToUInt64(ENUM Value)
		{
			return Type<int>::ConvertToUInt64(static_cast<int>(Value));
		}

		// Convert uint64 to type
		static ENUM ConvertFromUInt64(uint64 nValue)
		{
			return static_cast<ENUM>(Type<int>::ConvertFromUInt64(nValue));
		}

		// Convert type to uint_ptr
		static uint_ptr ConvertToUIntPtr(ENUM Value)
		{
			return Type<int>::ConvertToUIntPtr(static_cast<int>(Value));
		}

		// Convert uint_ptr to type
		static ENUM ConvertFromUIntPtr(uint_ptr nValue)
		{
			return static_cast<ENUM>(Type<int>::ConvertFromUIntPtr(nValue));
		}

		// Convert type to float
		static float ConvertToFloat(ENUM Value)
		{
			return Type<int>::ConvertToFloat(static_cast<int>(Value));
		}

		// Convert float to type
		static ENUM ConvertFromFloat(float fValue)
		{
			return static_cast<ENUM>(Type<int>::ConvertFromFloat(fValue));
		}

		// Convert type to double
		static double ConvertToDouble(ENUM Value)
		{
			return Type<int>::ConvertToDouble(static_cast<int>(Value));
		}

		// Convert double to type
		static ENUM ConvertFromDouble(double dValue)
		{
			return static_cast<ENUM>(Type<int>::ConvertFromDouble(dValue));
		}

		// Convert type to string
		static String ConvertToString(ENUM Value)
		{
			return Type<int>::ConvertToString(static_cast<int>(Value));
		}

		// Convert string to type
		static ENUM ConvertFromString(const String &sString)
		{
			return static_cast<ENUM>(Type<int>::ConvertFromString(sString));
		}

		// Convert real to storage type
		static _StorageType ConvertRealToStorage(_Type nValue)
		{
			return nValue;
		}

		// Convert storage to real type
		static _Type ConvertStorageToReal(_StorageType nValue)
		{
			return nValue;
		}

};


#endif // __PLCORE_TYPE_ENUMPLAIN_INL__
