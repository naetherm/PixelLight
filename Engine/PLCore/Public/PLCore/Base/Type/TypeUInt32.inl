/*********************************************************\
 *  File: TypeUInt32.inl                                 *
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


#ifndef __PLCORE_TYPE_UINT32_INL__
#define __PLCORE_TYPE_UINT32_INL__
#pragma once


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Type wrapper for 'uint32'
*/
template <>
class Type<uint32> {


	//[-------------------------------------------------------]
	//[ Public static type information                        ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef uint32 _Type;			/**< Real type */
		typedef uint32 _StorageType;	/**< Storage type, for this type identical to the real type */

		// Type ID
		static const uint32 TypeID = TypeUInt32;

		// Get type name
		static String GetTypeName()
		{
			return "uint32";
		}

		// Convert var to uint32
		static uint32 ConvertFromVar(const DynVar &cValue)
		{
			return cValue.GetUInt32();
		}

		// Convert uint32 to bool
		static bool ConvertToBool(uint32 nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to uint32
		static uint32 ConvertFromBool(bool bValue)
		{
			return static_cast<uint32>(bValue);
		}

		// Convert uint32 to int
		static int ConvertToInt(uint32 nValue)
		{
			return static_cast<int>(nValue);
		}

		// Convert int to uint32
		static uint32 ConvertFromInt(int nValue)
		{
			return static_cast<uint32>(nValue);
		}

		// Convert uint32 to int8
		static int8 ConvertToInt8(uint32 nValue)
		{
			return static_cast<int8>(nValue);
		}

		// Convert int8 to uint32
		static uint32 ConvertFromInt8(int8 nValue)
		{
			return static_cast<uint32>(nValue);
		}

		// Convert uint32 to int16
		static int16 ConvertToInt16(uint32 nValue)
		{
			return static_cast<int16>(nValue);
		}

		// Convert int16 to uint32
		static uint32 ConvertFromInt16(int16 nValue)
		{
			return static_cast<uint32>(nValue);
		}

		// Convert uint32 to int32
		static int32 ConvertToInt32(uint32 nValue)
		{
			return static_cast<int32>(nValue);
		}

		// Convert int32 to uint32
		static uint32 ConvertFromInt32(int32 nValue)
		{
			return static_cast<uint32>(nValue);
		}

		// Convert uint32 to int64
		static int64 ConvertToInt64(uint32 nValue)
		{
			return static_cast<int64>(nValue);
		}

		// Convert int64 to uint32
		static uint32 ConvertFromInt64(int64 nValue)
		{
			return static_cast<uint32>(nValue);
		}

		// Convert uint32 to uint8
		static uint8 ConvertToUInt8(uint32 nValue)
		{
			return static_cast<uint8>(nValue);
		}

		// Convert uint8 to uint32
		static uint32 ConvertFromUInt8(uint8 nValue)
		{
			return static_cast<uint32>(nValue);
		}

		// Convert uint32 to uint16
		static uint16 ConvertToUInt16(uint32 nValue)
		{
			return static_cast<uint16>(nValue);
		}

		// Convert uint16 to uint32
		static uint32 ConvertFromUInt16(uint16 nValue)
		{
			return static_cast<uint32>(nValue);
		}

		// Convert uint32 to uint32
		static uint32 ConvertToUInt32(uint32 nValue)
		{
			return static_cast<uint32>(nValue);
		}

		// Convert uint32 to uint32
		static uint32 ConvertFromUInt32(uint32 nValue)
		{
			return static_cast<uint32>(nValue);
		}

		// Convert uint32 to uint64
		static uint64 ConvertToUInt64(uint32 nValue)
		{
			return static_cast<uint64>(nValue);
		}

		// Convert uint64 to uint32
		static uint32 ConvertFromUInt64(uint64 nValue)
		{
			return static_cast<uint32>(nValue);
		}

		// Convert uint32 to uint_ptr
		static uint_ptr ConvertToUIntPtr(uint32 nValue)
		{
			return static_cast<uint_ptr>(nValue);
		}

		// Convert uint_ptr to uint32
		static uint32 ConvertFromUIntPtr(uint_ptr nValue)
		{
			return static_cast<uint32>(nValue);
		}

		// Convert uint32 to float
		static float ConvertToFloat(uint32 nValue)
		{
			return static_cast<float>(nValue);
		}

		// Convert float to uint32
		static uint32 ConvertFromFloat(float fValue)
		{
			return static_cast<uint32>(fValue);
		}

		// Convert uint32 to double
		static double ConvertToDouble(uint32 nValue)
		{
			return static_cast<double>(nValue);
		}

		// Convert double to uint32
		static uint32 ConvertFromDouble(double dValue)
		{
			return static_cast<uint32>(dValue);
		}

		// Convert uint32 to string
		static String ConvertToString(uint32 nValue)
		{
			return String() + static_cast<int>(nValue);
		}

		// Convert string to uint32
		static uint32 ConvertFromString(const String &sString)
		{
			return sString.GetInt();
		}

		// Convert real to storage type
		static uint32 ConvertRealToStorage(uint32 nValue)
		{
			return nValue;
		}

		// Convert storage to real type
		static uint32 ConvertStorageToReal(uint32 nValue)
		{
			return nValue;
		}

};


#endif // __PLCORE_TYPE_UINT32_INL__
