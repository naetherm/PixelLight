/*********************************************************\
 *  File: TypeInt8.inl                                   *
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


#ifndef __PLCORE_TYPE_INT8_INL__
#define __PLCORE_TYPE_INT8_INL__
#pragma once


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Type wrapper for 'int8'
*/
template <>
class Type<int8> {


	//[-------------------------------------------------------]
	//[ Public static type information                        ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef int8 _Type;			/**< Real type */
		typedef int8 _StorageType;	/**< Storage type, for this type identical to the real type */

		// Type ID
		static const uint32 TypeID = TypeInt8;

		// Get type name
		static String GetTypeName()
		{
			return "int8";
		}

		// Convert var to int8
		static int8 ConvertFromVar(const DynVar &cValue)
		{
			return cValue.GetInt8();
		}

		// Convert int8 to bool
		static bool ConvertToBool(int8 nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to int8
		static int8 ConvertFromBool(bool bValue)
		{
			return static_cast<int8>(bValue);
		}

		// Convert int8 to int
		static int ConvertToInt(int8 nValue)
		{
			return static_cast<int>(nValue);
		}

		// Convert int to int8
		static int8 ConvertFromInt(int nValue)
		{
			return static_cast<int8>(nValue);
		}

		// Convert int8 to int8
		static int8 ConvertToInt8(int8 nValue)
		{
			return static_cast<int8>(nValue);
		}

		// Convert int8 to int8
		static int8 ConvertFromInt8(int8 nValue)
		{
			return static_cast<int8>(nValue);
		}

		// Convert int8 to int16
		static int16 ConvertToInt16(int8 nValue)
		{
			return static_cast<int16>(nValue);
		}

		// Convert int16 to int8
		static int8 ConvertFromInt16(int16 nValue)
		{
			return static_cast<int8>(nValue);
		}

		// Convert int8 to int32
		static int32 ConvertToInt32(int8 nValue)
		{
			return static_cast<int32>(nValue);
		}

		// Convert int32 to int8
		static int8 ConvertFromInt32(int32 nValue)
		{
			return static_cast<int8>(nValue);
		}

		// Convert int8 to int64
		static int64 ConvertToInt64(int8 nValue)
		{
			return static_cast<int64>(nValue);
		}

		// Convert int64 to int8
		static int8 ConvertFromInt64(int64 nValue)
		{
			return static_cast<int8>(nValue);
		}

		// Convert int8 to uint8
		static uint8 ConvertToUInt8(int8 nValue)
		{
			return static_cast<uint8>(nValue);
		}

		// Convert uint8 to int8
		static int8 ConvertFromUInt8(uint8 nValue)
		{
			return static_cast<int8>(nValue);
		}

		// Convert int8 to uint16
		static uint16 ConvertToUInt16(int8 nValue)
		{
			return static_cast<uint16>(nValue);
		}

		// Convert uint16 to int8
		static int8 ConvertFromUInt16(uint16 nValue)
		{
			return static_cast<int8>(nValue);
		}

		// Convert int8 to uint32
		static uint32 ConvertToUInt32(int8 nValue)
		{
			return static_cast<uint32>(nValue);
		}

		// Convert uint32 to int8
		static int8 ConvertFromUInt32(uint32 nValue)
		{
			return static_cast<int8>(nValue);
		}

		// Convert int8 to uint64
		static uint64 ConvertToUInt64(int8 nValue)
		{
			return static_cast<uint64>(nValue);
		}

		// Convert uint64 to int8
		static int8 ConvertFromUInt64(uint64 nValue)
		{
			return static_cast<int8>(nValue);
		}

		// Convert int8 to uint_ptr
		static uint_ptr ConvertToUIntPtr(int8 nValue)
		{
			return static_cast<uint_ptr>(nValue);
		}

		// Convert uint_ptr to int8
		static int8 ConvertFromUIntPtr(uint_ptr nValue)
		{
			return static_cast<int8>(nValue);
		}

		// Convert int8 to float
		static float ConvertToFloat(int8 nValue)
		{
			return static_cast<float>(nValue);
		}

		// Convert float to int8
		static int8 ConvertFromFloat(float fValue)
		{
			return static_cast<int8>(fValue);
		}

		// Convert int8 to double
		static double ConvertToDouble(int8 nValue)
		{
			return static_cast<double>(nValue);
		}

		// Convert double to int8
		static int8 ConvertFromDouble(double dValue)
		{
			return static_cast<int8>(dValue);
		}

		// Convert int8 to string
		static String ConvertToString(int8 nValue)
		{
			return String() + static_cast<int>(nValue);
		}

		// Convert string to int8
		static int8 ConvertFromString(const String &sString)
		{
			return static_cast<int8>(sString.GetInt());
		}

		// Convert real to storage type
		static int8 ConvertRealToStorage(int8 nValue)
		{
			return nValue;
		}

		// Convert storage to real type
		static int8 ConvertStorageToReal(int8 nValue)
		{
			return nValue;
		}

};


#endif // __PLCORE_TYPE_INT8_INL__
