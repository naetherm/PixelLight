/*********************************************************\
 *  File: TypeFlag.inl                                   *
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


#ifndef __PLCORE_TYPE_FLAG_INL__
#define __PLCORE_TYPE_FLAG_INL__
#pragma once


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Type wrapper for flag types
*/
template <typename ENUM>
class Type< FlagType<ENUM> > {


	//[-------------------------------------------------------]
	//[ Public static type information                        ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef typename ENUM::_BaseType _BaseType;
		typedef typename ENUM::_BaseType _Type;			/**< Real type */
		typedef typename ENUM::_BaseType _StorageType;	/**< Storage type, for this type identical to the real type */
		typedef typename ENUM::_Type	 _EnumType;

		// Type ID
		static const int TypeID = Type<_BaseType>::TypeID;

		// Get type name
		static String GetTypeName()
		{
			return Type<_BaseType>::GetTypeName();
		}

		// Convert var to type
		static _BaseType ConvertFromVar(const DynVar &cValue)
		{
			if (cValue.GetTypeID() == TypeString)
				return ConvertFromString(cValue.GetString());
			else
				return Type<_BaseType>::ConvertFromVar(cValue);
		}

		// Convert type to bool
		static bool ConvertToBool(_BaseType Value)
		{
			return Type<_BaseType>::ConvertToBool(Value);
		}

		// Convert bool to type
		static _BaseType ConvertFromBool(bool bValue)
		{
			return Type<_BaseType>::ConvertFromBool(bValue);
		}

		// Convert type to int
		static int ConvertToInt(_BaseType Value)
		{
			return Type<_BaseType>::ConvertToInt(Value);
		}

		// Convert int to type
		static _BaseType ConvertFromInt(int nValue)
		{
			return Type<_BaseType>::ConvertFromInt(nValue);
		}

		// Convert type to int8
		static int8 ConvertToInt8(_BaseType Value)
		{
			return Type<_BaseType>::ConvertToInt8(Value);
		}

		// Convert int8 to type
		static _BaseType ConvertFromInt8(int8 nValue)
		{
			return Type<_BaseType>::ConvertFromInt8(nValue);
		}

		// Convert type to int16
		static int16 ConvertToInt16(_BaseType Value)
		{
			return Type<_BaseType>::ConvertToInt16(Value);
		}

		// Convert int16 to type
		static _BaseType ConvertFromInt16(int16 nValue)
		{
			return Type<_BaseType>::ConvertFromInt16(nValue);
		}

		// Convert type to int32
		static int32 ConvertToInt32(_BaseType Value)
		{
			return Type<_BaseType>::ConvertToInt32(Value);
		}

		// Convert int32 to type
		static _BaseType ConvertFromInt32(int32 nValue)
		{
			return Type<_BaseType>::ConvertFromInt32(nValue);
		}

		// Convert type to int64
		static int64 ConvertToInt64(_BaseType Value)
		{
			return Type<_BaseType>::ConvertToInt64(Value);
		}

		// Convert int64 to type
		static _BaseType ConvertFromInt64(int64 nValue)
		{
			return Type<_BaseType>::ConvertFromInt64(nValue);
		}

		// Convert type to uint8
		static uint8 ConvertToUInt8(_BaseType Value)
		{
			return Type<_BaseType>::ConvertToUInt8(Value);
		}

		// Convert uint8 to type
		static _BaseType ConvertFromUInt8(uint8 nValue)
		{
			return Type<_BaseType>::ConvertFromUInt8(nValue);
		}

		// Convert type to uint16
		static uint16 ConvertToUInt16(_BaseType Value)
		{
			return Type<_BaseType>::ConvertToUInt16(Value);
		}

		// Convert uint16 to type
		static _BaseType ConvertFromUInt16(uint16 nValue)
		{
			return Type<_BaseType>::ConvertFromUInt16(nValue);
		}

		// Convert type to uint32
		static uint32 ConvertToUInt32(_BaseType Value)
		{
			return Type<_BaseType>::ConvertToUInt32(Value);
		}

		// Convert uint32 to type
		static _BaseType ConvertFromUInt32(uint32 nValue)
		{
			return Type<_BaseType>::ConvertFromUInt32(nValue);
		}

		// Convert type to uint64
		static uint64 ConvertToUInt64(_BaseType Value)
		{
			return Type<_BaseType>::ConvertToUInt64(Value);
		}

		// Convert uint64 to type
		static _BaseType ConvertFromUInt64(int64 nValue)
		{
			return Type<_BaseType>::ConvertFromUInt64(nValue);
		}

		// Convert type to uint_ptr
		static uint_ptr ConvertToUIntPtr(_BaseType Value)
		{
			return Type<_BaseType>::ConvertToUIntPtr(Value);
		}

		// Convert uint_ptr to type
		static _BaseType ConvertFromUIntPtr(uint_ptr nValue)
		{
			return Type<_BaseType>::ConvertFromUIntPtr(nValue);
		}

		// Convert type to float
		static float ConvertToFloat(_BaseType Value)
		{
			return Type<_BaseType>::ConvertToFloat(Value);
		}

		// Convert float to type
		static _BaseType ConvertFromFloat(float fValue)
		{
			return Type<_BaseType>::ConvertFromFloat(fValue);
		}

		// Convert type to double
		static double ConvertToDouble(_BaseType Value)
		{
			return Type<_BaseType>::ConvertToDouble(Value);
		}

		// Convert double to type
		static _BaseType ConvertFromDouble(double dValue)
		{
			return Type<_BaseType>::ConvertFromDouble(dValue);
		}

		// Convert type to string
		static String ConvertToString(_BaseType Value)
		{
			// Init return value
			String sFlags;
			_BaseType nValue = Value;

			// Loop through enum values
			String sName, sDesc;
			_EnumType nFlag;
			for (int i=0; ENUM::GetEnumValue(i, nFlag, sName, sDesc); i++) {
				// Check if flag is set
				if (nValue & nFlag) {
					if (sFlags.GetLength() > 0)
						sFlags += "|";
					sFlags += sName;
					nValue = (nValue & ~nFlag);
				}
			}

			// Add rest
			if (nValue != 0) {
				if (sFlags.GetLength() > 0)
					sFlags += "|";
				sFlags += nValue;
			}

			// Return flags
			if (sFlags.GetLength() == 0)
				sFlags = "0";
			return sFlags;
		}

		// Convert string to type
		static _BaseType ConvertFromString(const String &sString)
		{
			// Init return value
			_BaseType nFlags = 0;

			// Tokenizer flags string
			if (sString.GetLength()) {
				Tokenizer cTokenizer;
				cTokenizer.Start(sString);
				cTokenizer.SetDelimiters(" \t\r\n&,|");
				while (cTokenizer.GetNextToken().GetLength()) {
					// Get next token
					String sToken = cTokenizer.GetToken();
					String sDesc;

					// Try to convert token to number, else look up name in enum
					_EnumType nValue = static_cast<_EnumType>(sToken.GetInt());
					if (static_cast<_BaseType>(nValue) == 0)
						ENUM::GetEnumValue(-1, nValue, sToken, sDesc);
					nFlags |= nValue;
				}
				cTokenizer.Stop();
			}

			// Return flags value
			return nFlags;
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


#endif // __PLCORE_TYPE_FLAG_INL__
