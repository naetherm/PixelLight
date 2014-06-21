/*********************************************************\
 *  File: TypeInfoEnum.inl                               *
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


#ifndef __PLCORE_TYPEINFO_ENUM_INL__
#define __PLCORE_TYPEINFO_ENUM_INL__
#pragma once


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Type wrapper for enum types
*/
template <typename ENUM>
class TypeInfo< EnumType<ENUM> > : public DynTypeInfo {


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Type
		typedef typename ENUM::_Type _Type;


	//[-------------------------------------------------------]
	//[ Public static data                                    ]
	//[-------------------------------------------------------]
	public:
		static TypeInfo Instance;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		TypeInfo()
		{
			// Fill enum values
			for (int i=0; i<EnumType<ENUM>::GetNumOfEnumValues(); i++)
				m_lstEnumValues.Add(EnumType<ENUM>::GetEnumName(i));
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TypeInfo()
		{
		}


	//[-------------------------------------------------------]
	//[ Public virtual DynTypeInfo functions                  ]
	//[-------------------------------------------------------]
	public:
		virtual int GetTypeID() const override
		{
			return Type<_Type>::TypeID;
		}

		virtual String GetTypeName() const override
		{
			return Type<_Type>::GetTypeName();
		}

		virtual bool IsEnumType() const override
		{
			return true;
		}

		virtual bool IsFlagType() const override
		{
			return false;
		}

		virtual String GetEnumValue(const String &sEnum) const override
		{
			String sName = sEnum;
			String sDesc;
			_Type nValue = DefaultValue<_Type>::Default();
			ENUM::GetEnumValue(-1, nValue, sName, sDesc);
			return Type<_Type>::ConvertToString(nValue);
		}

		virtual String GetEnumDescription(const String &sEnum) const override
		{
			String sName = sEnum;
			String sDesc;
			_Type nValue = DefaultValue<_Type>::Default();
			ENUM::GetEnumValue(-1, nValue, sName, sDesc);
			return sDesc;
		}


};


//[-------------------------------------------------------]
//[ Static variables                                      ]
//[-------------------------------------------------------]
template <typename ENUM> TypeInfo< EnumType<ENUM> > TypeInfo< EnumType<ENUM> >::Instance;


#endif // __PLCORE_TYPEINFO_ENUM_INL__
