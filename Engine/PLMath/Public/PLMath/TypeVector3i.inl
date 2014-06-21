/*********************************************************\
 *  File: TypeVector3i.inl                               *
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


#ifndef __PLMATH_TYPE_VECTOR3I_INL__
#define __PLMATH_TYPE_VECTOR3I_INL__
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
*    Type wrapper for 'PLMath::Vector3i'
*/
template <>
class Type<PLMath::Vector3i> {


	//[-------------------------------------------------------]
	//[ Public static type information                        ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef PLMath::Vector3i _Type;			/**< Real type */
		typedef PLMath::Vector3i _StorageType;	/**< Storage type, for this type identical to the real type */

		// Type ID
		static const int TypeID = 1004;

		// Get type name
		static String GetTypeName()
		{
			return "vector3i";
		}

		// Convert dynamic variable to PLMath::Vector3i
		static PLMath::Vector3i ConvertFromVar(const DynVar &cDynVar)
		{
			return (cDynVar.GetTypeID() == TypeID) ? static_cast<const Var<PLMath::Vector3i>&>(cDynVar).Get() : cDynVar.GetString();
		}

		// Convert PLMath::Vector3i to bool
		static bool ConvertToBool(const PLMath::Vector3i &vVector)
		{
			return (vVector == PLMath::Vector3i::Zero);
		}

		// Convert bool to PLMath::Vector3i
		static PLMath::Vector3i ConvertFromBool(bool bValue)
		{
			const int nValue = bValue ? 1 : 0;
			return PLMath::Vector3i(nValue, nValue, nValue);
		}

		// Convert PLMath::Vector3i to int
		static int ConvertToInt(const PLMath::Vector3i &vVector)
		{
			return (vVector == PLMath::Vector3i::Zero);
		}

		// Convert int to PLMath::Vector3i
		static PLMath::Vector3i ConvertFromInt(int nValue)
		{
			return PLMath::Vector3i(nValue, nValue, nValue);
		}

		// Convert PLMath::Vector3i to int8
		static int8 ConvertToInt8(const PLMath::Vector3i &vVector)
		{
			return static_cast<int8>(ConvertToInt(vVector));
		}

		// Convert int8 to PLMath::Vector3i
		static PLMath::Vector3i ConvertFromInt8(int8 nValue)
		{
			return PLMath::Vector3i(nValue, nValue, nValue);
		}

		// Convert PLMath::Vector3i to int16
		static int16 ConvertToInt16(const PLMath::Vector3i &vVector)
		{
			return static_cast<int16>(ConvertToInt(vVector));
		}

		// Convert int16 to PLMath::Vector3i
		static PLMath::Vector3i ConvertFromInt16(int16 nValue)
		{
			return PLMath::Vector3i(nValue, nValue, nValue);
		}

		// Convert PLMath::Vector3i to int32
		static int32 ConvertToInt32(const PLMath::Vector3i &vVector)
		{
			return static_cast<int32>(ConvertToInt(vVector));
		}

		// Convert int32 to PLMath::Vector3i
		static PLMath::Vector3i ConvertFromInt32(int32 nValue)
		{
			return PLMath::Vector3i(nValue, nValue, nValue);
		}

		// Convert PLMath::Vector3i to int64
		static int64 ConvertToInt64(const PLMath::Vector3i &vVector)
		{
			return static_cast<int64>(ConvertToInt(vVector));
		}

		// Convert int64 to PLMath::Vector3i
		static PLMath::Vector3i ConvertFromInt64(int32 nValue)
		{
			return PLMath::Vector3i(static_cast<int>(nValue), static_cast<int>(nValue), static_cast<int>(nValue));
		}

		// Convert PLMath::Vector3i to uint8
		static uint8 ConvertToUInt8(const PLMath::Vector3i &vVector)
		{
			return static_cast<uint8>(ConvertToInt(vVector));
		}

		// Convert uint8 to PLMath::Vector3i
		static PLMath::Vector3i ConvertFromUInt8(uint8 nValue)
		{
			return PLMath::Vector3i(nValue, nValue, nValue);
		}

		// Convert PLMath::Vector3i to uint16
		static uint16 ConvertToUInt16(const PLMath::Vector3i &vVector)
		{
			return static_cast<uint16>(ConvertToInt(vVector));
		}

		// Convert uint16 to PLMath::Vector3i
		static PLMath::Vector3i ConvertFromUInt16(uint16 nValue)
		{
			return PLMath::Vector3i(nValue, nValue, nValue);
		}

		// Convert PLMath::Vector3i to uint32
		static uint32 ConvertToUInt32(const PLMath::Vector3i &vVector)
		{
			return static_cast<uint32>(ConvertToInt(vVector));
		}

		// Convert uint32 to PLMath::Vector3i
		static PLMath::Vector3i ConvertFromUInt32(uint32 nValue)
		{
			return PLMath::Vector3i(nValue, nValue, nValue);
		}

		// Convert PLMath::Vector3i to uint64
		static uint64 ConvertToUInt64(const PLMath::Vector3i &vVector)
		{
			return static_cast<uint64>(ConvertToInt(vVector));
		}

		// Convert uint64 to PLMath::Vector3i
		static PLMath::Vector3i ConvertFromUInt64(uint64 nValue)
		{
			return PLMath::Vector3i(static_cast<int>(nValue), static_cast<int>(nValue), static_cast<int>(nValue));
		}

		// Convert PLMath::Vector3i to uint_ptr
		static uint_ptr ConvertToUIntPtr(const PLMath::Vector3i &vVector)
		{
			return static_cast<uint_ptr>(ConvertToInt(vVector));
		}

		// Convert uint_ptr to PLMath::Vector3i
		static PLMath::Vector3i ConvertFromUIntPtr(uint_ptr nValue)
		{
			return PLMath::Vector3i(static_cast<int>(nValue), static_cast<int>(nValue), static_cast<int>(nValue));
		}

		// Convert PLMath::Vector3i to float
		static float ConvertToFloat(const PLMath::Vector3i &vVector)
		{
			return static_cast<float>(ConvertToInt(vVector));
		}

		// Convert float to PLMath::Vector3i
		static PLMath::Vector3i ConvertFromFloat(float fValue)
		{
			return PLMath::Vector3i(static_cast<int>(fValue), static_cast<int>(fValue), static_cast<int>(fValue));
		}

		// Convert PLMath::Vector3i to double
		static double ConvertToDouble(const PLMath::Vector3i &vVector)
		{
			return static_cast<double>(ConvertToInt(vVector));
		}

		// Convert double to PLMath::Vector3i
		static PLMath::Vector3i ConvertFromDouble(double dValue)
		{
			return PLMath::Vector3i(static_cast<int>(dValue), static_cast<int>(dValue), static_cast<int>(dValue));
		}

		// Convert PLMath::Vector3i to string
		static String ConvertToString(const PLMath::Vector3i &vVector)
		{
			return vVector.ToString();
		}

		// Convert string to PLMath::Vector3i
		static PLMath::Vector3i ConvertFromString(const String &sString)
		{
			PLMath::Vector3i vVector;
			vVector.FromString(sString);
			return vVector;
		}

		// Convert real to storage type (reference is enough in here)
		static PLMath::Vector3i &ConvertRealToStorage(PLMath::Vector3i &vValue)
		{
			return vValue;
		}

		// Convert storage to real type (reference is enough in here)
		static PLMath::Vector3i &ConvertStorageToReal(PLMath::Vector3i &vValue)
		{
			return vValue;
		}


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLMATH_TYPE_VECTOR3I_INL__
