/*********************************************************\
 *  File: TypeVector2i.inl                               *
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


#ifndef __PLMATH_TYPE_VECTOR2I_INL__
#define __PLMATH_TYPE_VECTOR2I_INL__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Var/Var.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Type wrapper for 'PLMath::Vector2i'
*/
template <>
class Type<PLMath::Vector2i> {


	//[-------------------------------------------------------]
	//[ Public static type information                        ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef PLMath::Vector2i _Type;			/**< Real type */
		typedef PLMath::Vector2i _StorageType;	/**< Storage type, for this type identical to the real type */

		// Type ID
		static const int TypeID = 1003;

		// Get type name
		static String GetTypeName()
		{
			return "vector2i";
		}

		// Convert dynamic variable to PLMath::Vector2i
		static PLMath::Vector2i ConvertFromVar(const DynVar &cDynVar)
		{
			return (cDynVar.GetTypeID() == TypeID) ? static_cast<const Var<PLMath::Vector2i>&>(cDynVar).Get() : cDynVar.GetString();
		}

		// Convert PLMath::Vector2i to bool
		static bool ConvertToBool(const PLMath::Vector2i &vVector)
		{
			return (vVector == PLMath::Vector2i::Zero);
		}

		// Convert bool to PLMath::Vector2i
		static PLMath::Vector2i ConvertFromBool(bool bValue)
		{
			const int nValue = bValue ? 1 : 0;
			return PLMath::Vector2i(nValue, nValue);
		}

		// Convert PLMath::Vector2i to int
		static int ConvertToInt(const PLMath::Vector2i &vVector)
		{
			return (vVector == PLMath::Vector2i::Zero);
		}

		// Convert int to PLMath::Vector2i
		static PLMath::Vector2i ConvertFromInt(int nValue)
		{
			return PLMath::Vector2i(nValue, nValue);
		}

		// Convert PLMath::Vector2i to int8
		static int8 ConvertToInt8(const PLMath::Vector2i &vVector)
		{
			return static_cast<int8>(ConvertToInt(vVector));
		}

		// Convert int8 to PLMath::Vector2i
		static PLMath::Vector2i ConvertFromInt8(int8 nValue)
		{
			return PLMath::Vector2i(nValue, nValue);
		}

		// Convert PLMath::Vector2i to int16
		static int16 ConvertToInt16(const PLMath::Vector2i &vVector)
		{
			return static_cast<int16>(ConvertToInt(vVector));
		}

		// Convert int16 to PLMath::Vector2i
		static PLMath::Vector2i ConvertFromInt16(int16 nValue)
		{
			return PLMath::Vector2i(nValue, nValue);
		}

		// Convert PLMath::Vector2i to int32
		static int32 ConvertToInt32(const PLMath::Vector2i &vVector)
		{
			return static_cast<int32>(ConvertToInt(vVector));
		}

		// Convert int32 to PLMath::Vector2i
		static PLMath::Vector2i ConvertFromInt32(int32 nValue)
		{
			return PLMath::Vector2i(nValue, nValue);
		}

		// Convert PLMath::Vector2i to int64
		static int64 ConvertToInt64(const PLMath::Vector2i &vVector)
		{
			return static_cast<int64>(ConvertToInt(vVector));
		}

		// Convert int64 to PLMath::Vector2i
		static PLMath::Vector2i ConvertFromInt64(int64 nValue)
		{
			return PLMath::Vector2i(static_cast<int>(nValue), static_cast<int>(nValue));
		}

		// Convert PLMath::Vector2i to uint8
		static uint8 ConvertToUInt8(const PLMath::Vector2i &vVector)
		{
			return static_cast<uint8>(ConvertToInt(vVector));
		}

		// Convert uint8 to PLMath::Vector2i
		static PLMath::Vector2i ConvertFromUInt8(uint8 nValue)
		{
			return PLMath::Vector2i(nValue, nValue);
		}

		// Convert PLMath::Vector2i to uint16
		static uint16 ConvertToUInt16(const PLMath::Vector2i &vVector)
		{
			return static_cast<uint16>(ConvertToInt(vVector));
		}

		// Convert uint16 to PLMath::Vector2i
		static PLMath::Vector2i ConvertFromUInt16(uint16 nValue)
		{
			return PLMath::Vector2i(nValue, nValue);
		}

		// Convert PLMath::Vector2i to uint32
		static uint32 ConvertToUInt32(const PLMath::Vector2i &vVector)
		{
			return static_cast<uint32>(ConvertToInt(vVector));
		}

		// Convert uint32 to PLMath::Vector2i
		static PLMath::Vector2i ConvertFromUInt32(uint32 nValue)
		{
			return PLMath::Vector2i(nValue, nValue);
		}

		// Convert PLMath::Vector2i to uint64
		static uint64 ConvertToUInt64(const PLMath::Vector2i &vVector)
		{
			return static_cast<uint64>(ConvertToInt(vVector));
		}

		// Convert uint64 to PLMath::Vector2i
		static PLMath::Vector2i ConvertFromUInt64(uint64 nValue)
		{
			return PLMath::Vector2i(static_cast<int>(nValue), static_cast<int>(nValue));
		}

		// Convert PLMath::Vector2i to uint_ptr
		static uint_ptr ConvertToUIntPtr(const PLMath::Vector2i &vVector)
		{
			return static_cast<uint_ptr>(ConvertToInt(vVector));
		}

		// Convert uint_ptr to PLMath::Vector2i
		static PLMath::Vector2i ConvertFromUIntPtr(uint_ptr nValue)
		{
			return PLMath::Vector2i(static_cast<int>(nValue), static_cast<int>(nValue));
		}

		// Convert PLMath::Vector2i to float
		static float ConvertToFloat(const PLMath::Vector2i &vVector)
		{
			return static_cast<float>(ConvertToInt(vVector));
		}

		// Convert float to PLMath::Vector2i
		static PLMath::Vector2i ConvertFromFloat(float fValue)
		{
			return PLMath::Vector2i(static_cast<int>(fValue), static_cast<int>(fValue));
		}

		// Convert PLMath::Vector2i to double
		static double ConvertToDouble(const PLMath::Vector2i &vVector)
		{
			return static_cast<double>(ConvertToInt(vVector));
		}

		// Convert double to PLMath::Vector2i
		static PLMath::Vector2i ConvertFromDouble(double dValue)
		{
			return PLMath::Vector2i(static_cast<int>(dValue), static_cast<int>(dValue));
		}

		// Convert PLMath::Vector2i to string
		static String ConvertToString(const PLMath::Vector2i &vVector)
		{
			return vVector.ToString();
		}

		// Convert string to PLMath::Vector2i
		static PLMath::Vector2i ConvertFromString(const String &sString)
		{
			PLMath::Vector2i vVector;
			vVector.FromString(sString);
			return vVector;
		}

		// Convert real to storage type (reference is enough in here)
		static PLMath::Vector2i &ConvertRealToStorage(PLMath::Vector2i &vValue)
		{
			return vValue;
		}

		// Convert storage to real type (reference is enough in here)
		static PLMath::Vector2i &ConvertStorageToReal(PLMath::Vector2i &vValue)
		{
			return vValue;
		}


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLMATH_TYPE_VECTOR2I_INL__
