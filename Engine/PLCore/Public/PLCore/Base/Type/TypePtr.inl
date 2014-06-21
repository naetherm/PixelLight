/*********************************************************\
 *  File: TypePtr.inl                                    *
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


#ifndef __PLCORE_TYPE_PTR_INL__
#define __PLCORE_TYPE_PTR_INL__
#pragma once


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Type wrapper for pointers 'T*'
*/
template <typename T>
class Type<T*> {


	//[-------------------------------------------------------]
	//[ Public static type information                        ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef T* _Type;			/**< Real type */
		typedef T* _StorageType;	/**< Storage type, for this type identical to the real type */

		// Type ID
		static const int TypeID = TypePtr;

		// Get type name
		static String GetTypeName()
		{
			return "void*";
		}

		// Convert var to pointer
		static T *ConvertFromVar(const DynVar &cValue)
		{
			return reinterpret_cast<T*>(cValue.GetUIntPtr());
		}

		// Convert pointer to bool
		static bool ConvertToBool(T *pValue)
		{
			return static_cast<bool>(pValue != nullptr);
		}

		// Convert bool to pointer
		static T *ConvertFromBool(bool bValue)
		{
			return nullptr;
		}

		// Convert pointer to int
		static int ConvertToInt(T *pValue)
		{
			return static_cast<int>(reinterpret_cast<uint_ptr>(pValue));
		}

		// Convert int to pointer
		static T *ConvertFromInt(int nValue)
		{
			return reinterpret_cast<T*>(nValue);
		}

		// Convert pointer to int8
		static int8 ConvertToInt8(T *pValue)
		{
			return static_cast<int8>(reinterpret_cast<uint_ptr>(pValue));
		}

		// Convert int8 to pointer
		static T *ConvertFromInt8(int8 nValue)
		{
			return reinterpret_cast<T*>(nValue);
		}

		// Convert pointer to int16
		static int16 ConvertToInt16(T *pValue)
		{
			return static_cast<int16>(reinterpret_cast<uint_ptr>(pValue));
		}

		// Convert int16 to pointer
		static T *ConvertFromInt16(int16 nValue)
		{
			return reinterpret_cast<T*>(nValue);
		}

		// Convert pointer to int32
		static int32 ConvertToInt32(T *pValue)
		{
			return static_cast<int32>(reinterpret_cast<uint_ptr>(pValue));
		}

		// Convert int32 to pointer
		static T *ConvertFromInt32(int32 nValue)
		{
			return reinterpret_cast<T*>(nValue);
		}

		// Convert pointer to int64
		static int64 ConvertToInt64(T *pValue)
		{
			return static_cast<int64>(reinterpret_cast<uint_ptr>(pValue));
		}

		// Convert int64 to pointer
		static T *ConvertFromInt64(int64 nValue)
		{
			return reinterpret_cast<T*>(nValue);
		}

		// Convert pointer to uint8
		static uint8 ConvertToUInt8(T *pValue)
		{
			return static_cast<uint8>(reinterpret_cast<uint_ptr>(pValue));
		}

		// Convert uint8 to pointer
		static T *ConvertFromUInt8(uint8 nValue)
		{
			return reinterpret_cast<T*>(nValue);
		}

		// Convert pointer to uint16
		static uint16 ConvertToUInt16(T *pValue)
		{
			return static_cast<uint16>(reinterpret_cast<uint_ptr>(pValue));
		}

		// Convert uint16 to pointer
		static T *ConvertFromUInt16(uint16 nValue)
		{
			return reinterpret_cast<T*>(nValue);
		}

		// Convert pointer to uint32
		static uint32 ConvertToUInt32(T *pValue)
		{
			return static_cast<uint32>(reinterpret_cast<uint_ptr>(pValue));
		}

		// Convert uint32 to pointer
		static T *ConvertFromUInt32(uint32 nValue)
		{
			return reinterpret_cast<T*>(nValue);
		}

		// Convert pointer to uint64
		static uint64 ConvertToUInt64(T *pValue)
		{
			return static_cast<uint64>(reinterpret_cast<uint_ptr>(pValue));
		}

		// Convert uint64 to pointer
		static T *ConvertFromUInt64(uint64 nValue)
		{
			return reinterpret_cast<T*>(nValue);
		}

		// Convert pointer to uint_ptr
		static uint_ptr ConvertToUIntPtr(T *pValue)
		{
			return reinterpret_cast<uint_ptr>(pValue);
		}

		// Convert uint_ptr to pointer
		static T *ConvertFromUIntPtr(uint_ptr nValue)
		{
			return reinterpret_cast<T*>(nValue);
		}

		// Convert pointer to float
		static float ConvertToFloat(T *pValue)
		{
			// No conversion from pointer types in non-integral types!
			return 0.0f;
		}

		// Convert float to pointer
		static T *ConvertFromFloat(float fValue)
		{
			// No conversion from pointer types in non-integral types!
			return nullptr;
		}

		// Convert pointer to double
		static double ConvertToDouble(T *pValue)
		{
			// No conversion from pointer types in non-integral types!
			return 0.0;
		}

		// Convert double to pointer
		static T *ConvertFromDouble(double dValue)
		{
			// No conversion from pointer types in non-integral types!
			return nullptr;
		}

		// Convert pointer to string
		static String ConvertToString(T *pValue)
		{
			return String() + reinterpret_cast<uint_ptr>(pValue);
		}

		// Convert string to pointer
		static T *ConvertFromString(const String &sString)
		{
			return reinterpret_cast<T*>(sString.GetUIntPtr());
		}

		// Convert real to storage type
		static T *ConvertRealToStorage(T *pValue)
		{
			return pValue;
		}

		// Convert storage to real type
		static T *ConvertStorageToReal(T *pValue)
		{
			return pValue;
		}

};


#endif // __PLCORE_TYPE_PTR_INL__
