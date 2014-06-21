/*********************************************************\
 *  File: TypeString.inl                                 *
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


#ifndef __PLCORE_TYPE_STRING_INL__
#define __PLCORE_TYPE_STRING_INL__
#pragma once


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Type wrapper for 'String'
*/
template <>
class Type<String> {


	//[-------------------------------------------------------]
	//[ Public static type information                        ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef String _Type;			/**< Real type */
		typedef String _StorageType;	/**< Storage type, for this type identical to the real type */

		// Type ID
		static const int TypeID = TypeString;

		// Get type name
		static String GetTypeName()
		{
			return "string";
		}

		// Convert var to string
		static String ConvertFromVar(const DynVar &cValue)
		{
			return cValue.GetString();
		}

		// Convert string to bool
		static bool ConvertToBool(const String &sString)
		{
			return sString.GetBool();
		}

		// Convert bool to string
		static String ConvertFromBool(bool bValue)
		{
			return (String() + bValue);
		}

		// Convert string to int
		static int ConvertToInt(const String &sString)
		{
			return sString.GetInt();
		}

		// Convert int to string
		static String ConvertFromInt(int nValue)
		{
			return (String() + nValue);
		}

		// Convert string to int8
		static int8 ConvertToInt8(const String &sString)
		{
			return static_cast<int8>(sString.GetInt());
		}

		// Convert int8 to string
		static String ConvertFromInt8(int8 nValue)
		{
			return (String() + static_cast<int>(nValue));
		}

		// Convert string to int16
		static int16 ConvertToInt16(const String &sString)
		{
			return static_cast<int16>(sString.GetInt());
		}

		// Convert int16 to string
		static String ConvertFromInt16(int16 nValue)
		{
			return (String() + static_cast<int>(nValue));
		}

		// Convert string to int32
		static int32 ConvertToInt32(const String &sString)
		{
			return static_cast<int32>(sString.GetInt());
		}

		// Convert int32 to string
		static String ConvertFromInt32(int32 nValue)
		{
			return (String() + static_cast<int>(nValue));
		}

		// Convert string to int64
		static int64 ConvertToInt64(const String &sString)
		{
			return static_cast<int64>(sString.GetInt());
		}

		// Convert int64 to string
		static String ConvertFromInt64(int64 nValue)
		{
			return (String() + static_cast<int>(nValue));
		}

		// Convert string to uint8
		static uint8 ConvertToUInt8(const String &sString)
		{
			return static_cast<uint8>(sString.GetInt());
		}

		// Convert uint8 to string
		static String ConvertFromUInt8(uint8 nValue)
		{
			return (String() + static_cast<int>(nValue));
		}

		// Convert string to uint16
		static uint16 ConvertToUInt16(const String &sString)
		{
			return static_cast<uint16>(sString.GetInt());
		}

		// Convert uint16 to string
		static String ConvertFromUInt16(uint16 nValue)
		{
			return (String() + static_cast<int>(nValue));
		}

		// Convert string to uint32
		static uint32 ConvertToUInt32(const String &sString)
		{
			return static_cast<uint32>(sString.GetInt());
		}

		// Convert uint32 to string
		static String ConvertFromUInt32(uint32 nValue)
		{
			return (String() + static_cast<int>(nValue));
		}

		// Convert string to uint64
		static uint64 ConvertToUInt64(const String &sString)
		{
			return static_cast<uint64>(sString.GetInt());
		}

		// Convert uint64 to string
		static String ConvertFromUInt64(uint64 nValue)
		{
			return (String() + static_cast<int>(nValue));
		}

		// Convert string to uint_ptr
		static uint_ptr ConvertToUIntPtr(const String &sString)
		{
			return static_cast<uint_ptr>(sString.GetInt());
		}

		// Convert uint_ptr to string
		static String ConvertFromUIntPtr(uint_ptr nValue)
		{
			return (String() + static_cast<int>(nValue));
		}

		// Convert string to float
		static float ConvertToFloat(const String &sString)
		{
			return sString.GetFloat();
		}

		// Convert float to string
		static String ConvertFromFloat(float fValue)
		{
			return (String() + fValue);
		}

		// Convert string to double
		static double ConvertToDouble(const String &sString)
		{
			return sString.GetDouble();
		}

		// Convert double to string
		static String ConvertFromDouble(double dValue)
		{
			return (String() + dValue);
		}

		// Convert string to string
		static String ConvertToString(const String &sString)
		{
			return sString;
		}

		// Convert string to string
		static String ConvertFromString(const String &sString)
		{
			return sString;
		}

		// Convert real to storage type
		static String ConvertRealToStorage(String sValue)
		{
			return sValue;
		}

		// Convert storage to real type
		static String ConvertStorageToReal(String sValue)
		{
			return sValue;
		}

};


#endif // __PLCORE_TYPE_STRING_INL__
