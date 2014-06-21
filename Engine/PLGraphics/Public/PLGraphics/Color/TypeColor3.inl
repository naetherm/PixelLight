/*********************************************************\
 *  File: TypeColor3.inl                                *
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


#ifndef __PLGRAPHICS_TYPE_COLOR3_INL__
#define __PLGRAPHICS_TYPE_COLOR3_INL__
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
*    Type wrapper for 'PLGraphics::Color3'
*/
template <>
class Type<PLGraphics::Color3> {


	//[-------------------------------------------------------]
	//[ Public static type information                        ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef PLGraphics::Color3 _Type;			/**< Real type */
		typedef PLGraphics::Color3 _StorageType;	/**< Storage type, for this type identical to the real type */

		// Type ID
		static const int TypeID = 2001;

		// Get type name
		static String GetTypeName()
		{
			return "color3";
		}

		// Convert dynamic variable to PLGraphics::Color3
		static PLGraphics::Color3 ConvertFromVar(const DynVar &cDynVar)
		{
			return (cDynVar.GetTypeID() == TypeID) ? static_cast<const Var<PLGraphics::Color3>&>(cDynVar).Get() : cDynVar.GetString();
		}

		// Convert PLGraphics::Color3 to bool
		static bool ConvertToBool(const PLGraphics::Color3 &cColor)
		{
			return cColor.IsValid();
		}

		// Convert bool to PLGraphics::Color3
		static PLGraphics::Color3 ConvertFromBool(bool bValue)
		{
			const float fValue = bValue ? 1.0f : 0.0f;
			return PLGraphics::Color3(fValue, fValue, fValue);
		}

		// Convert PLGraphics::Color3 to int
		static int ConvertToInt(const PLGraphics::Color3 &cColor)
		{
			return cColor.IsValid();
		}

		// Convert int to PLGraphics::Color3
		static PLGraphics::Color3 ConvertFromInt(int nValue)
		{
			return PLGraphics::Color3(static_cast<float>(nValue), static_cast<float>(nValue), static_cast<float>(nValue));
		}

		// Convert PLGraphics::Color3 to int8
		static int8 ConvertToInt8(const PLGraphics::Color3 &cColor)
		{
			return static_cast<int8>(ConvertToInt(cColor));
		}

		// Convert int8 to PLGraphics::Color3
		static PLGraphics::Color3 ConvertFromInt8(int8 nValue)
		{
			return PLGraphics::Color3(static_cast<float>(nValue), static_cast<float>(nValue), static_cast<float>(nValue));
		}

		// Convert PLGraphics::Color3 to int16
		static int16 ConvertToInt16(const PLGraphics::Color3 &cColor)
		{
			return static_cast<int16>(ConvertToInt(cColor));
		}

		// Convert int16 to PLGraphics::Color3
		static PLGraphics::Color3 ConvertFromInt16(int16 nValue)
		{
			return PLGraphics::Color3(static_cast<float>(nValue), static_cast<float>(nValue), static_cast<float>(nValue));
		}

		// Convert PLGraphics::Color3 to int32
		static int32 ConvertToInt32(const PLGraphics::Color3 &cColor)
		{
			return static_cast<int32>(ConvertToInt(cColor));
		}

		// Convert int32 to PLGraphics::Color3
		static PLGraphics::Color3 ConvertFromInt32(int32 nValue)
		{
			return PLGraphics::Color3(static_cast<float>(nValue), static_cast<float>(nValue), static_cast<float>(nValue));
		}

		// Convert PLGraphics::Color3 to int64
		static int64 ConvertToInt64(const PLGraphics::Color3 &cColor)
		{
			return static_cast<int64>(ConvertToInt(cColor));
		}

		// Convert int64 to PLGraphics::Color3
		static PLGraphics::Color3 ConvertFromInt64(int64 nValue)
		{
			return PLGraphics::Color3(static_cast<float>(nValue), static_cast<float>(nValue), static_cast<float>(nValue));
		}

		// Convert PLGraphics::Color3 to uint8
		static uint8 ConvertToUInt8(const PLGraphics::Color3 &cColor)
		{
			return static_cast<uint8>(ConvertToInt(cColor));
		}

		// Convert uint8 to PLGraphics::Color3
		static PLGraphics::Color3 ConvertFromUInt8(uint8 nValue)
		{
			return PLGraphics::Color3(static_cast<float>(nValue), static_cast<float>(nValue), static_cast<float>(nValue));
		}

		// Convert PLGraphics::Color3 to uint16
		static uint16 ConvertToUInt16(const PLGraphics::Color3 &cColor)
		{
			return static_cast<uint16>(ConvertToInt(cColor));
		}

		// Convert uint16 to PLGraphics::Color3
		static PLGraphics::Color3 ConvertFromUInt16(uint16 nValue)
		{
			return PLGraphics::Color3(static_cast<float>(nValue), static_cast<float>(nValue), static_cast<float>(nValue));
		}

		// Convert PLGraphics::Color3 to uint32
		static uint32 ConvertToUInt32(const PLGraphics::Color3 &cColor)
		{
			return static_cast<uint32>(ConvertToInt(cColor));
		}

		// Convert uint32 to PLGraphics::Color3
		static PLGraphics::Color3 ConvertFromUInt32(uint32 nValue)
		{
			return PLGraphics::Color3(static_cast<float>(nValue), static_cast<float>(nValue), static_cast<float>(nValue));
		}

		// Convert PLGraphics::Color3 to uint64
		static uint64 ConvertToUInt64(const PLGraphics::Color3 &cColor)
		{
			return static_cast<uint64>(ConvertToInt(cColor));
		}

		// Convert uint64 to PLGraphics::Color3
		static PLGraphics::Color3 ConvertFromUInt64(uint64 nValue)
		{
			return PLGraphics::Color3(static_cast<float>(nValue), static_cast<float>(nValue), static_cast<float>(nValue));
		}

		// Convert PLGraphics::Color3 to uint_ptr
		static uint_ptr ConvertToUIntPtr(const PLGraphics::Color3 &cColor)
		{
			return static_cast<uint_ptr>(ConvertToInt(cColor));
		}

		// Convert uint_ptr to PLGraphics::Color3
		static PLGraphics::Color3 ConvertFromUIntPtr(uint_ptr nValue)
		{
			return PLGraphics::Color3(static_cast<float>(nValue), static_cast<float>(nValue), static_cast<float>(nValue));
		}

		// Convert PLGraphics::Color3 to float
		static float ConvertToFloat(const PLGraphics::Color3 &cColor)
		{
			return static_cast<float>(ConvertToInt(cColor));
		}

		// Convert float to PLGraphics::Color3
		static PLGraphics::Color3 ConvertFromFloat(float fValue)
		{
			return PLGraphics::Color3(fValue, fValue, fValue);
		}

		// Convert PLGraphics::Color3 to double
		static double ConvertToDouble(const PLGraphics::Color3 &cColor)
		{
			return static_cast<double>(ConvertToInt(cColor));
		}

		// Convert double to PLGraphics::Color3
		static PLGraphics::Color3 ConvertFromDouble(double dValue)
		{
			return PLGraphics::Color3(static_cast<float>(dValue), static_cast<float>(dValue), static_cast<float>(dValue));
		}

		// Convert PLGraphics::Color3 to string
		static String ConvertToString(const PLGraphics::Color3 &cColor)
		{
			return cColor.ToString();
		}

		// Convert string to PLGraphics::Color3
		static PLGraphics::Color3 ConvertFromString(const String &sString)
		{
			PLGraphics::Color3 cColor;
			cColor.FromString(sString);
			return cColor;
		}

		// Convert real to storage type (reference is enough in here)
		static PLGraphics::Color3 &ConvertRealToStorage(PLGraphics::Color3 &cValue)
		{
			return cValue;
		}

		// Convert storage to real type (reference is enough in here)
		static PLGraphics::Color3 &ConvertStorageToReal(PLGraphics::Color3 &cValue)
		{
			return cValue;
		}


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLGRAPHICS_TYPE_COLOR3_INL__
