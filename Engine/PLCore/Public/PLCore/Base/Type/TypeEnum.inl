/*********************************************************\
 *  File: TypeEnum.inl                                   *
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


#ifndef __PLCORE_TYPE_ENUM_INL__
#define __PLCORE_TYPE_ENUM_INL__
#pragma once


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Type wrapper for enum types
*/
template <typename ENUM>
class Type< EnumType<ENUM> > {


	//[-------------------------------------------------------]
	//[ Public static type information                        ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef typename ENUM::_BaseType _BaseType;
		typedef typename ENUM::_Type	 _Type;			/**< Real type */
		typedef typename ENUM::_Type	 _StorageType;	/**< Storage type, for this type identical to the real type */

		// Type ID
		static const int TypeID = Type<_BaseType>::TypeID;

		// Get type name
		static String GetTypeName()
		{
			return Type<_BaseType>::GetTypeName();
		}

		// Convert var to type
		static _Type ConvertFromVar(const DynVar &cValue)
		{
			if (cValue.GetTypeID() == TypeString)
				return ConvertFromString(cValue.GetString());
			else
				return static_cast<_Type>(Type<_BaseType>::ConvertFromVar(cValue));
		}

		// Convert type to bool
		static bool ConvertToBool(_Type Value)
		{
			return Type<_BaseType>::ConvertToBool(static_cast<_BaseType>(Value));
		}

		// Convert bool to type
		static _Type ConvertFromBool(bool bValue)
		{
			return static_cast<_Type>(Type<_BaseType>::ConvertFromBool(bValue));
		}

		// Convert type to int
		static int ConvertToInt(_Type Value)
		{
			return Type<_BaseType>::ConvertToInt(static_cast<_BaseType>(Value));
		}

		// Convert int to type
		static _Type ConvertFromInt(int nValue)
		{
			return static_cast<_Type>(Type<_BaseType>::ConvertFromInt(nValue));
		}

		// Convert type to int8
		static int8 ConvertToInt8(_Type Value)
		{
			return Type<_BaseType>::ConvertToInt8(static_cast<_BaseType>(Value));
		}

		// Convert int8 to type
		static _Type ConvertFromInt8(int8 nValue)
		{
			return static_cast<_Type>(Type<_BaseType>::ConvertFromInt8(nValue));
		}

		// Convert type to int16
		static int16 ConvertToInt16(_Type Value)
		{
			return Type<_BaseType>::ConvertToInt16(static_cast<_BaseType>(Value));
		}

		// Convert int16 to type
		static _Type ConvertFromInt16(int16 nValue)
		{
			return static_cast<_Type>(Type<_BaseType>::ConvertFromInt16(nValue));
		}

		// Convert type to int32
		static int32 ConvertToInt32(_Type Value)
		{
			return Type<_BaseType>::ConvertToInt32(static_cast<_BaseType>(Value));
		}

		// Convert int32 to type
		static _Type ConvertFromInt32(int32 nValue)
		{
			return static_cast<_Type>(Type<_BaseType>::ConvertFromInt32(nValue));
		}

		// Convert type to int64
		static int64 ConvertToInt64(_Type Value)
		{
			return Type<_BaseType>::ConvertToInt64(static_cast<_BaseType>(Value));
		}

		// Convert int64 to type
		static _Type ConvertFromInt64(int64 nValue)
		{
			return static_cast<_Type>(Type<_BaseType>::ConvertFromInt64(nValue));
		}

		// Convert type to uint8
		static uint8 ConvertToUInt8(_Type Value)
		{
			return Type<_BaseType>::ConvertToUInt8(static_cast<_BaseType>(Value));
		}

		// Convert uint8 to type
		static _Type ConvertFromUInt8(uint8 nValue)
		{
			return static_cast<_Type>(Type<_BaseType>::ConvertFromUInt8(nValue));
		}

		// Convert type to uint16
		static uint16 ConvertToUInt16(_Type Value)
		{
			return Type<_BaseType>::ConvertToUInt16(static_cast<_BaseType>(Value));
		}

		// Convert uint16 to type
		static _Type ConvertFromUInt16(uint16 nValue)
		{
			return static_cast<_Type>(Type<_BaseType>::ConvertFromUInt16(nValue));
		}

		// Convert type to uint32
		static uint32 ConvertToUInt32(_Type Value)
		{
			return Type<_BaseType>::ConvertToUInt32(static_cast<_BaseType>(Value));
		}

		// Convert uint32 to type
		static _Type ConvertFromUInt32(uint32 nValue)
		{
			return static_cast<_Type>(Type<_BaseType>::ConvertFromUInt32(nValue));
		}

		// Convert type to uint64
		static uint64 ConvertToUInt64(_Type Value)
		{
			return Type<_BaseType>::ConvertToUInt64(static_cast<_BaseType>(Value));
		}

		// Convert uint64 to type
		static _Type ConvertFromUInt64(uint64 nValue)
		{
			return static_cast<_Type>(Type<_BaseType>::ConvertFromUInt64(nValue));
		}

		// Convert type to uint_ptr
		static uint_ptr ConvertToUIntPtr(_Type Value)
		{
			return Type<_BaseType>::ConvertToUIntPtr(static_cast<_BaseType>(Value));
		}

		// Convert uint_ptr to type
		static _Type ConvertFromUIntPtr(uint_ptr nValue)
		{
			return static_cast<_Type>(Type<_BaseType>::ConvertFromUIntPtr(nValue));
		}

		// Convert type to float
		static float ConvertToFloat(_Type Value)
		{
			return Type<_BaseType>::ConvertToFloat(static_cast<_BaseType>(Value));
		}

		// Convert float to type
		static _Type ConvertFromFloat(float fValue)
		{
			return static_cast<_Type>(Type<_BaseType>::ConvertFromFloat(fValue));
		}

		// Convert type to double
		static double ConvertToDouble(_Type Value)
		{
			return Type<_BaseType>::ConvertToDouble(static_cast<_BaseType>(Value));
		}

		// Convert double to type
		static _Type ConvertFromDouble(double dValue)
		{
			return static_cast<_Type>(Type<_BaseType>::ConvertFromDouble(dValue));
		}

		// Convert type to string
		static String ConvertToString(_Type Value)
		{
			String sValue, sDesc;
			if (ENUM::GetEnumValue(-1, Value, sValue, sDesc))
				return sValue;
			else
				return Type<_BaseType>::ConvertToString(static_cast<_BaseType>(Value));
		}

		// Convert string to type
		static _Type ConvertFromString(const String &sString)
		{
			String sValue = sString;
			String sDesc;
			_Type Value;
			if (ENUM::GetEnumValue(-1, Value, sValue, sDesc))
				return Value;
			else
				return static_cast<_Type>(Type<_BaseType>::ConvertFromString(sString));
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


#endif // __PLCORE_TYPE_ENUM_INL__
