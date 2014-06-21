/*********************************************************\
 *  File: TypeBool.inl                                   *
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


#ifndef __PLCORE_TYPE_BOOL_INL__
#define __PLCORE_TYPE_BOOL_INL__
#pragma once


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Type wrapper for 'bool'
*/
template <>
class Type<bool> {


	//[-------------------------------------------------------]
	//[ Public static type information                        ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef bool _Type;			/**< Real type */
		typedef bool _StorageType;	/**< Storage type, for this type identical to the real type */

		// Type ID
		static const int TypeID = TypeBool;

		// Get type name
		static String GetTypeName()
		{
			return "bool";
		}

		// Convert var to bool
		static bool ConvertFromVar(const DynVar &cValue)
		{
			return cValue.GetBool();
		}

		// Convert bool to bool
		static bool ConvertToBool(bool bValue)
		{
			return static_cast<bool>(bValue);
		}

		// Convert bool to bool
		static bool ConvertFromBool(bool bValue)
		{
			return static_cast<bool>(bValue);
		}

		// Convert bool to int
		static int ConvertToInt(bool bValue)
		{
			return static_cast<int>(bValue);
		}

		// Convert int to bool
		static bool ConvertFromInt(int nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to int8
		static int8 ConvertToInt8(bool bValue)
		{
			return static_cast<int8>(bValue);
		}

		// Convert int8 to bool
		static bool ConvertFromInt8(int8 nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to int16
		static int16 ConvertToInt16(bool bValue)
		{
			return static_cast<int16>(bValue);
		}

		// Convert int16 to bool
		static bool ConvertFromInt16(int16 nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to int32
		static int32 ConvertToInt32(bool bValue)
		{
			return static_cast<int32>(bValue);
		}

		// Convert int32 to bool
		static bool ConvertFromInt32(int32 nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to int64
		static int64 ConvertToInt64(bool bValue)
		{
			return static_cast<int64>(bValue);
		}

		// Convert int64 to bool
		static bool ConvertFromInt64(int64 nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to uint8
		static uint8 ConvertToUInt8(bool bValue)
		{
			return static_cast<uint8>(bValue);
		}

		// Convert uint8 to bool
		static bool ConvertFromUInt8(uint8 nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to uint16
		static uint16 ConvertToUInt16(bool bValue)
		{
			return static_cast<uint16>(bValue);
		}

		// Convert uint16 to bool
		static bool ConvertFromUInt16(uint16 nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to uint32
		static uint32 ConvertToUInt32(bool bValue)
		{
			return static_cast<uint32>(bValue);
		}

		// Convert uint32 to bool
		static bool ConvertFromUInt32(uint32 nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to uint64
		static uint64 ConvertToUInt64(bool bValue)
		{
			return static_cast<uint64>(bValue);
		}

		// Convert uint64 to bool
		static bool ConvertFromUInt64(uint64 nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to uint_ptr
		static uint_ptr ConvertToUIntPtr(bool bValue)
		{
			return static_cast<uint_ptr>(bValue);
		}

		// Convert uint_ptr to bool
		static bool ConvertFromUIntPtr(uint_ptr nValue)
		{
			return static_cast<bool>(nValue != 0);
		}

		// Convert bool to float
		static float ConvertToFloat(bool bValue)
		{
			return static_cast<float>(bValue);
		}

		// Convert float to bool
		static bool ConvertFromFloat(float fValue)
		{
			return static_cast<bool>(fValue != 0.0f);
		}

		// Convert bool to double
		static double ConvertToDouble(bool bValue)
		{
			return static_cast<double>(bValue);
		}

		// Convert double to bool
		static bool ConvertFromDouble(double dValue)
		{
			return static_cast<bool>(dValue != 0.0);
		}

		// Convert bool to string
		static String ConvertToString(bool bValue)
		{
			return (bValue ? "true" : "false");
		}

		// Convert string to bool
		static bool ConvertFromString(const String &sString)
		{
			return sString.GetBool();
		}

		// Convert real to storage type
		static bool ConvertRealToStorage(bool bValue)
		{
			return bValue;
		}

		// Convert storage to real type
		static bool ConvertStorageToReal(bool bValue)
		{
			return bValue;
		}

};


#endif // __PLCORE_TYPE_BOOL_INL__
