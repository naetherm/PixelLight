/*********************************************************\
 *  File: TypeRef.inl                                    *
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


#ifndef __PLCORE_TYPE_REF_INL__
#define __PLCORE_TYPE_REF_INL__
#pragma once


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Type wrapper for references 'T&'
*/
template <typename T>
class Type<T&> {


	//[-------------------------------------------------------]
	//[ Public static type information                        ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef T& _Type;			/**< Real type */
		typedef T* _StorageType;	/**< Storage type... We really can't use "&" in here because references can't copy each other, they're copying the referenced value instead! */

		// Type ID
		static const int TypeID = TypeRef;

		// Get type name
		static String GetTypeName()
		{
			return "void&";
		}

		// Convert var to pointer
		static T &ConvertFromVar(const DynVar &cValue)
		{
			return *static_cast<T*>(cValue.GetUIntPtr());
		}

		// Convert pointer to bool
		static bool ConvertToBool(T &cValue)
		{
			return static_cast<bool>((&cValue) != nullptr);
		}

		// Convert bool to pointer
		static T &ConvertFromBool(bool bValue)
		{
			return *static_cast<T*>(nullptr);
		}

		// Convert pointer to int
		static int ConvertToInt(T &cValue)
		{
			return static_cast<int>(static_cast<uint_ptr>(&cValue));
		}

		// Convert int to pointer
		static T &ConvertFromInt(int nValue)
		{
			return *static_cast<T*>(nValue);
		}

		// Convert pointer to int8
		static int8 ConvertToInt8(T &cValue)
		{
			return static_cast<int8>(static_cast<uint_ptr>(&cValue));
		}

		// Convert int8 to pointer
		static T &ConvertFromInt8(int8 nValue)
		{
			return *static_cast<T*>(nValue);
		}

		// Convert pointer to int16
		static int16 ConvertToInt16(T &cValue)
		{
			return static_cast<int16>(static_cast<uint_ptr>(&cValue));
		}

		// Convert int16 to pointer
		static T &ConvertFromInt16(int16 nValue)
		{
			return *static_cast<T*>(nValue);
		}

		// Convert pointer to int32
		static int32 ConvertToInt32(T &cValue)
		{
			return static_cast<int32>(static_cast<uint_ptr>(&cValue));
		}

		// Convert int32 to pointer
		static T &ConvertFromInt32(int32 nValue)
		{
			return *static_cast<T*>(nValue);
		}

		// Convert pointer to int64
		static int64 ConvertToInt64(T &cValue)
		{
			return static_cast<int64>(static_cast<uint_ptr>(&cValue));
		}

		// Convert int64 to pointer
		static T &ConvertFromInt64(int64 nValue)
		{
			return *static_cast<T*>(nValue);
		}

		// Convert pointer to uint8
		static uint8 ConvertToUInt8(T &cValue)
		{
			return static_cast<uint8>(static_cast<uint_ptr>(&cValue));
		}

		// Convert uint8 to pointer
		static T &ConvertFromUInt8(uint8 nValue)
		{
			return *static_cast<T*>(nValue);
		}

		// Convert pointer to uint16
		static uint16 ConvertToUInt16(T &cValue)
		{
			return static_cast<uint16>(static_cast<uint_ptr>(&cValue));
		}

		// Convert uint16 to pointer
		static T &ConvertFromUInt16(uint16 nValue)
		{
			return *static_cast<T*>(nValue);
		}

		// Convert pointer to uint32
		static uint32 ConvertToUInt32(T &cValue)
		{
			return static_cast<uint32>(static_cast<uint_ptr>(&cValue));
		}

		// Convert uint32 to pointer
		static T &ConvertFromUInt32(uint32 nValue)
		{
			return *static_cast<T*>(nValue);
		}

		// Convert pointer to uint64
		static uint64 ConvertToUInt64(T &cValue)
		{
			return static_cast<uint64>(static_cast<uint_ptr>(&cValue));
		}

		// Convert uint64 to pointer
		static T &ConvertFromUInt64(uint64 nValue)
		{
			return *static_cast<T*>(nValue);
		}

		// Convert pointer to uint_ptr
		static uint_ptr ConvertToUIntPtr(T &cValue)
		{
			return static_cast<uint_ptr>(&cValue);
		}

		// Convert uint_ptr to pointer
		static T &ConvertFromUIntPtr(uint_ptr nValue)
		{
			return *static_cast<T*>(nValue);
		}

		// Convert pointer to float
		static float ConvertToFloat(T &cValue)
		{
			// No conversion from pointer types in non-integral types!
			return 0.0f;
		}

		// Convert float to pointer
		static T &ConvertFromFloat(float fValue)
		{
			// No conversion from pointer types in non-integral types!
			return *static_cast<T*>(nullptr);
		}

		// Convert pointer to double
		static double ConvertToDouble(T &cValue)
		{
			// No conversion from pointer types in non-integral types!
			return 0.0;
		}

		// Convert double to pointer
		static T &ConvertFromDouble(double dValue)
		{
			// No conversion from pointer types in non-integral types!
			return *static_cast<T*>(nullptr);
		}

		// Convert pointer to string
		static String ConvertToString(T &cValue)
		{
			return String() + reinterpret_cast<uint_ptr>(reinterpret_cast<T*>(&cValue));
		}

		// Convert string to pointer
		static T &ConvertFromString(const String &sString)
		{
			return *reinterpret_cast<T*>(sString.GetUIntPtr());
		}

		// Convert real to storage type
		static T *ConvertRealToStorage(T &cValue)
		{
			return &cValue;
		}

		// Convert storage to real type
		static T &ConvertStorageToReal(T *pValue)
		{
			return *pValue;
		}

};


#endif // __PLCORE_TYPE_REF_INL__
