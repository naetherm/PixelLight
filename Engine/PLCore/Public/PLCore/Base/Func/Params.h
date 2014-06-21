/*********************************************************\
 *  File: Params.h                                       *
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


#ifndef __PLCORE_PARAMS_H__
#define __PLCORE_PARAMS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Base/Type/Type.h"
#include "PLCore/Base/Type/DefaultValue.h"
#include "PLCore/Base/Tools/TypeTraits.h"
#include "PLCore/Base/Tools/ParamsParser.h"
#include "PLCore/Base/Tools/ParamsParserXml.h"
#include "PLCore/Base/Func/Signature.h"
#include "PLCore/Base/Func/DynParams.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for up to 16 parameters and a return value
*/
template <typename R, typename T0 = NullType, typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType, typename T6 = NullType, typename T7 = NullType, typename T8 = NullType, typename T9 = NullType, typename T10 = NullType, typename T11 = NullType, typename T12 = NullType, typename T13 = NullType, typename T14 = NullType, typename T15 = NullType>
class Params : public DynParams {


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Storage types
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef typename Type<T12>::_Type _T12;
		typedef typename Type<T13>::_Type _T13;
		typedef typename Type<T14>::_Type _T14;
		typedef typename Type<T15>::_Type _T15;
		typedef typename Type<R>  ::_StorageType _SR;
		typedef typename Type<T0> ::_StorageType _ST0;
		typedef typename Type<T1> ::_StorageType _ST1;
		typedef typename Type<T2> ::_StorageType _ST2;
		typedef typename Type<T3> ::_StorageType _ST3;
		typedef typename Type<T4> ::_StorageType _ST4;
		typedef typename Type<T5> ::_StorageType _ST5;
		typedef typename Type<T6> ::_StorageType _ST6;
		typedef typename Type<T7> ::_StorageType _ST7;
		typedef typename Type<T8> ::_StorageType _ST8;
		typedef typename Type<T9> ::_StorageType _ST9;
		typedef typename Type<T10>::_StorageType _ST10;
		typedef typename Type<T11>::_StorageType _ST11;
		typedef typename Type<T12>::_StorageType _ST12;
		typedef typename Type<T13>::_StorageType _ST13;
		typedef typename Type<T14>::_StorageType _ST14;
		typedef typename Type<T15>::_StorageType _ST15;


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		_SR   Return;
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;
		_ST4  Param4;
		_ST5  Param5;
		_ST6  Param6;
		_ST7  Param7;
		_ST8  Param8;
		_ST9  Param9;
		_ST10 Param10;
		_ST11 Param11;
		_ST12 Param12;
		_ST13 Param13;
		_ST14 Param14;
		_ST15 Param15;


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()					? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()					? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()					? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()					? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()					? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()					? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			_T10 t10 = (cParser.Next()					? Type<T10>::ConvertFromString(cParser.GetValue()) : DefaultValue<T10>::Default());
			_T11 t11 = (cParser.Next()					? Type<T11>::ConvertFromString(cParser.GetValue()) : DefaultValue<T11>::Default());
			_T12 t12 = (cParser.Next()					? Type<T12>::ConvertFromString(cParser.GetValue()) : DefaultValue<T12>::Default());
			_T13 t13 = (cParser.Next()					? Type<T13>::ConvertFromString(cParser.GetValue()) : DefaultValue<T13>::Default());
			_T14 t14 = (cParser.Next()					? Type<T14>::ConvertFromString(cParser.GetValue()) : DefaultValue<T14>::Default());
			_T15 t15 = (cParser.Next()					? Type<T15>::ConvertFromString(cParser.GetValue()) : DefaultValue<T15>::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()				? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()				? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()				? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()				? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()				? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()				? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			_T10 t10 = (cParser.Next()				? Type<T10>::ConvertFromString(cParser.GetValue()) : DefaultValue<T10>::Default());
			_T11 t11 = (cParser.Next()				? Type<T11>::ConvertFromString(cParser.GetValue()) : DefaultValue<T11>::Default());
			_T12 t12 = (cParser.Next()				? Type<T12>::ConvertFromString(cParser.GetValue()) : DefaultValue<T12>::Default());
			_T13 t13 = (cParser.Next()				? Type<T13>::ConvertFromString(cParser.GetValue()) : DefaultValue<T13>::Default());
			_T14 t14 = (cParser.Next()				? Type<T14>::ConvertFromString(cParser.GetValue()) : DefaultValue<T14>::Default());
			_T15 t15 = (cParser.Next()				? Type<T15>::ConvertFromString(cParser.GetValue()) : DefaultValue<T15>::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15);
		}


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		Params(const Params &cParams) :
			Return(DefaultValue<_SR>::Default()),
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3),
			Param4(cParams.Param4),
			Param5(cParams.Param5),
			Param6(cParams.Param6),
			Param7(cParams.Param7),
			Param8(cParams.Param8),
			Param9(cParams.Param9),
			Param10(cParams.Param10),
			Param11(cParams.Param11),
			Param12(cParams.Param12),
			Param13(cParams.Param13),
			Param14(cParams.Param14),
			Param15(cParams.Param15)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14, _T15 t15) :
			Return(DefaultValue<_SR>::Default()),
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3)),
			Param4(Type<T4>  ::ConvertRealToStorage(t4)),
			Param5(Type<T5>  ::ConvertRealToStorage(t5)),
			Param6(Type<T6>  ::ConvertRealToStorage(t6)),
			Param7(Type<T7>  ::ConvertRealToStorage(t7)),
			Param8(Type<T8>  ::ConvertRealToStorage(t8)),
			Param9(Type<T9>  ::ConvertRealToStorage(t9)),
			Param10(Type<T10>::ConvertRealToStorage(t10)),
			Param11(Type<T11>::ConvertRealToStorage(t11)),
			Param12(Type<T12>::ConvertRealToStorage(t12)),
			Param13(Type<T13>::ConvertRealToStorage(t13)),
			Param14(Type<T14>::ConvertRealToStorage(t14)),
			Param15(Type<T15>::ConvertRealToStorage(t15))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Return  = cParams.Return;
			Param0  = cParams.Param0;
			Param1  = cParams.Param1;
			Param2  = cParams.Param2;
			Param3  = cParams.Param3;
			Param4  = cParams.Param4;
			Param5  = cParams.Param5;
			Param6  = cParams.Param6;
			Param7  = cParams.Param7;
			Param8  = cParams.Param8;
			Param9  = cParams.Param9;
			Param10 = cParams.Param10;
			Param11 = cParams.Param11;
			Param12 = cParams.Param12;
			Param13 = cParams.Param13;
			Param14 = cParams.Param14;
			Param15 = cParams.Param15;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual void *GetPointerToReturnValue() override
		{
			return (void*)&Return;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 16;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				case 10:	return Type<T10>::TypeID;
				case 11:	return Type<T11>::TypeID;
				case 12:	return Type<T12>::TypeID;
				case 13:	return Type<T13>::TypeID;
				case 14:	return Type<T14>::TypeID;
				case 15:	return Type<T15>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				case 4:		return (void*)&Param4;
				case 5:		return (void*)&Param5;
				case 6:		return (void*)&Param6;
				case 7:		return (void*)&Param7;
				case 8:		return (void*)&Param8;
				case 9:		return (void*)&Param9;
				case 10:	return (void*)&Param10;
				case 11:	return (void*)&Param11;
				case 12:	return (void*)&Param12;
				case 13:	return (void*)&Param13;
				case 14:	return (void*)&Param14;
				case 15:	return (void*)&Param15;
				default:	return nullptr;
			}
		}

};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for up to 16 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> : public DynParams {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef typename Type<T12>::_Type _T12;
		typedef typename Type<T13>::_Type _T13;
		typedef typename Type<T14>::_Type _T14;
		typedef typename Type<T15>::_Type _T15;
		typedef typename Type<T0> ::_StorageType _ST0;
		typedef typename Type<T1> ::_StorageType _ST1;
		typedef typename Type<T2> ::_StorageType _ST2;
		typedef typename Type<T3> ::_StorageType _ST3;
		typedef typename Type<T4> ::_StorageType _ST4;
		typedef typename Type<T5> ::_StorageType _ST5;
		typedef typename Type<T6> ::_StorageType _ST6;
		typedef typename Type<T7> ::_StorageType _ST7;
		typedef typename Type<T8> ::_StorageType _ST8;
		typedef typename Type<T9> ::_StorageType _ST9;
		typedef typename Type<T10>::_StorageType _ST10;
		typedef typename Type<T11>::_StorageType _ST11;
		typedef typename Type<T12>::_StorageType _ST12;
		typedef typename Type<T13>::_StorageType _ST13;
		typedef typename Type<T14>::_StorageType _ST14;
		typedef typename Type<T15>::_StorageType _ST15;

	public:
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;
		_ST4  Param4;
		_ST5  Param5;
		_ST6  Param6;
		_ST7  Param7;
		_ST8  Param8;
		_ST9  Param9;
		_ST10 Param10;
		_ST11 Param11;
		_ST12 Param12;
		_ST13 Param13;
		_ST14 Param14;
		_ST15 Param15;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()					? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()					? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()					? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()					? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()					? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()					? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			_T10 t10 = (cParser.Next()					? Type<T10>::ConvertFromString(cParser.GetValue()) : DefaultValue<T10>::Default());
			_T11 t11 = (cParser.Next()					? Type<T11>::ConvertFromString(cParser.GetValue()) : DefaultValue<T11>::Default());
			_T12 t12 = (cParser.Next()					? Type<T12>::ConvertFromString(cParser.GetValue()) : DefaultValue<T12>::Default());
			_T13 t13 = (cParser.Next()					? Type<T13>::ConvertFromString(cParser.GetValue()) : DefaultValue<T13>::Default());
			_T14 t14 = (cParser.Next()					? Type<T14>::ConvertFromString(cParser.GetValue()) : DefaultValue<T14>::Default());
			_T15 t15 = (cParser.Next()					? Type<T15>::ConvertFromString(cParser.GetValue()) : DefaultValue<T15>::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()				? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()				? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()				? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()				? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()				? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()				? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			_T10 t10 = (cParser.Next()				? Type<T10>::ConvertFromString(cParser.GetValue()) : DefaultValue<T10>::Default());
			_T11 t11 = (cParser.Next()				? Type<T11>::ConvertFromString(cParser.GetValue()) : DefaultValue<T11>::Default());
			_T12 t12 = (cParser.Next()				? Type<T12>::ConvertFromString(cParser.GetValue()) : DefaultValue<T12>::Default());
			_T13 t13 = (cParser.Next()				? Type<T13>::ConvertFromString(cParser.GetValue()) : DefaultValue<T13>::Default());
			_T14 t14 = (cParser.Next()				? Type<T14>::ConvertFromString(cParser.GetValue()) : DefaultValue<T14>::Default());
			_T15 t15 = (cParser.Next()				? Type<T15>::ConvertFromString(cParser.GetValue()) : DefaultValue<T15>::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15);
		}

	public:
		Params(const Params &cParams) :
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3),
			Param4(cParams.Param4),
			Param5(cParams.Param5),
			Param6(cParams.Param6),
			Param7(cParams.Param7),
			Param8(cParams.Param8),
			Param9(cParams.Param9),
			Param10(cParams.Param10),
			Param11(cParams.Param11),
			Param12(cParams.Param12),
			Param13(cParams.Param13),
			Param14(cParams.Param14),
			Param15(cParams.Param15)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14, _T15 t15) :
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3)),
			Param4(Type<T4>  ::ConvertRealToStorage(t4)),
			Param5(Type<T5>  ::ConvertRealToStorage(t5)),
			Param6(Type<T6>  ::ConvertRealToStorage(t6)),
			Param7(Type<T7>  ::ConvertRealToStorage(t7)),
			Param8(Type<T8>  ::ConvertRealToStorage(t8)),
			Param9(Type<T9>  ::ConvertRealToStorage(t9)),
			Param10(Type<T10>::ConvertRealToStorage(t10)),
			Param11(Type<T11>::ConvertRealToStorage(t11)),
			Param12(Type<T12>::ConvertRealToStorage(t12)),
			Param13(Type<T13>::ConvertRealToStorage(t13)),
			Param14(Type<T14>::ConvertRealToStorage(t14)),
			Param15(Type<T15>::ConvertRealToStorage(t15))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Param0  = cParams.Param0;
			Param1  = cParams.Param1;
			Param2  = cParams.Param2;
			Param3  = cParams.Param3;
			Param4  = cParams.Param4;
			Param5  = cParams.Param5;
			Param6  = cParams.Param6;
			Param7  = cParams.Param7;
			Param8  = cParams.Param8;
			Param9  = cParams.Param9;
			Param10 = cParams.Param10;
			Param11 = cParams.Param11;
			Param12 = cParams.Param12;
			Param13 = cParams.Param13;
			Param14 = cParams.Param14;
			Param15 = cParams.Param15;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::GetSignatureID();
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 16;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				case 10:	return Type<T10>::TypeID;
				case 11:	return Type<T11>::TypeID;
				case 12:	return Type<T12>::TypeID;
				case 13:	return Type<T13>::TypeID;
				case 14:	return Type<T14>::TypeID;
				case 15:	return Type<T15>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				case 4:		return (void*)&Param4;
				case 5:		return (void*)&Param5;
				case 6:		return (void*)&Param6;
				case 7:		return (void*)&Param7;
				case 8:		return (void*)&Param8;
				case 9:		return (void*)&Param9;
				case 10:	return (void*)&Param10;
				case 11:	return (void*)&Param11;
				case 12:	return (void*)&Param12;
				case 13:	return (void*)&Param13;
				case 14:	return (void*)&Param14;
				case 15:	return (void*)&Param15;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 15 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public DynParams {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef typename Type<T12>::_Type _T12;
		typedef typename Type<T13>::_Type _T13;
		typedef typename Type<T14>::_Type _T14;
		typedef typename Type<R>  ::_StorageType _SR;
		typedef typename Type<T0> ::_StorageType _ST0;
		typedef typename Type<T1> ::_StorageType _ST1;
		typedef typename Type<T2> ::_StorageType _ST2;
		typedef typename Type<T3> ::_StorageType _ST3;
		typedef typename Type<T4> ::_StorageType _ST4;
		typedef typename Type<T5> ::_StorageType _ST5;
		typedef typename Type<T6> ::_StorageType _ST6;
		typedef typename Type<T7> ::_StorageType _ST7;
		typedef typename Type<T8> ::_StorageType _ST8;
		typedef typename Type<T9> ::_StorageType _ST9;
		typedef typename Type<T10>::_StorageType _ST10;
		typedef typename Type<T11>::_StorageType _ST11;
		typedef typename Type<T12>::_StorageType _ST12;
		typedef typename Type<T13>::_StorageType _ST13;
		typedef typename Type<T14>::_StorageType _ST14;

	public:
		_SR   Return;
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;
		_ST4  Param4;
		_ST5  Param5;
		_ST6  Param6;
		_ST7  Param7;
		_ST8  Param8;
		_ST9  Param9;
		_ST10 Param10;
		_ST11 Param11;
		_ST12 Param12;
		_ST13 Param13;
		_ST14 Param14;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()					? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()					? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()					? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()					? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()					? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()					? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			_T10 t10 = (cParser.Next()					? Type<T10>::ConvertFromString(cParser.GetValue()) : DefaultValue<T10>::Default());
			_T11 t11 = (cParser.Next()					? Type<T11>::ConvertFromString(cParser.GetValue()) : DefaultValue<T11>::Default());
			_T12 t12 = (cParser.Next()					? Type<T12>::ConvertFromString(cParser.GetValue()) : DefaultValue<T12>::Default());
			_T13 t13 = (cParser.Next()					? Type<T13>::ConvertFromString(cParser.GetValue()) : DefaultValue<T13>::Default());
			_T14 t14 = (cParser.Next()					? Type<T14>::ConvertFromString(cParser.GetValue()) : DefaultValue<T14>::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()				? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()				? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()				? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()				? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()				? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()				? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			_T10 t10 = (cParser.Next()				? Type<T10>::ConvertFromString(cParser.GetValue()) : DefaultValue<T10>::Default());
			_T11 t11 = (cParser.Next()				? Type<T11>::ConvertFromString(cParser.GetValue()) : DefaultValue<T11>::Default());
			_T12 t12 = (cParser.Next()				? Type<T12>::ConvertFromString(cParser.GetValue()) : DefaultValue<T12>::Default());
			_T13 t13 = (cParser.Next()				? Type<T13>::ConvertFromString(cParser.GetValue()) : DefaultValue<T13>::Default());
			_T14 t14 = (cParser.Next()				? Type<T14>::ConvertFromString(cParser.GetValue()) : DefaultValue<T14>::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14);
		}

	public:
		Params(const Params &cParams) :
			Return(DefaultValue<_SR>::Default()),
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3),
			Param4(cParams.Param4),
			Param5(cParams.Param5),
			Param6(cParams.Param6),
			Param7(cParams.Param7),
			Param8(cParams.Param8),
			Param9(cParams.Param9),
			Param10(cParams.Param10),
			Param11(cParams.Param11),
			Param12(cParams.Param12),
			Param13(cParams.Param13),
			Param14(cParams.Param14)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14) :
			Return(DefaultValue<_SR>::Default()),
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3)),
			Param4(Type<T4>  ::ConvertRealToStorage(t4)),
			Param5(Type<T5>  ::ConvertRealToStorage(t5)),
			Param6(Type<T6>  ::ConvertRealToStorage(t6)),
			Param7(Type<T7>  ::ConvertRealToStorage(t7)),
			Param8(Type<T8>  ::ConvertRealToStorage(t8)),
			Param9(Type<T9>  ::ConvertRealToStorage(t9)),
			Param10(Type<T10>::ConvertRealToStorage(t10)),
			Param11(Type<T11>::ConvertRealToStorage(t11)),
			Param12(Type<T12>::ConvertRealToStorage(t12)),
			Param13(Type<T13>::ConvertRealToStorage(t13)),
			Param14(Type<T14>::ConvertRealToStorage(t14))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Return  = cParams.Return;
			Param0  = cParams.Param0;
			Param1  = cParams.Param1;
			Param2  = cParams.Param2;
			Param3  = cParams.Param3;
			Param4  = cParams.Param4;
			Param5  = cParams.Param5;
			Param6  = cParams.Param6;
			Param7  = cParams.Param7;
			Param8  = cParams.Param8;
			Param9  = cParams.Param9;
			Param10 = cParams.Param10;
			Param11 = cParams.Param11;
			Param12 = cParams.Param12;
			Param13 = cParams.Param13;
			Param14 = cParams.Param14;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual void *GetPointerToReturnValue() override
		{
			return (void*)&Return;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 15;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				case 10:	return Type<T10>::TypeID;
				case 11:	return Type<T11>::TypeID;
				case 12:	return Type<T12>::TypeID;
				case 13:	return Type<T13>::TypeID;
				case 14:	return Type<T14>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				case 4:		return (void*)&Param4;
				case 5:		return (void*)&Param5;
				case 6:		return (void*)&Param6;
				case 7:		return (void*)&Param7;
				case 8:		return (void*)&Param8;
				case 9:		return (void*)&Param9;
				case 10:	return (void*)&Param10;
				case 11:	return (void*)&Param11;
				case 12:	return (void*)&Param12;
				case 13:	return (void*)&Param13;
				case 14:	return (void*)&Param14;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 15 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public DynParams {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef typename Type<T12>::_Type _T12;
		typedef typename Type<T13>::_Type _T13;
		typedef typename Type<T14>::_Type _T14;
		typedef typename Type<T0> ::_StorageType _ST0;
		typedef typename Type<T1> ::_StorageType _ST1;
		typedef typename Type<T2> ::_StorageType _ST2;
		typedef typename Type<T3> ::_StorageType _ST3;
		typedef typename Type<T4> ::_StorageType _ST4;
		typedef typename Type<T5> ::_StorageType _ST5;
		typedef typename Type<T6> ::_StorageType _ST6;
		typedef typename Type<T7> ::_StorageType _ST7;
		typedef typename Type<T8> ::_StorageType _ST8;
		typedef typename Type<T9> ::_StorageType _ST9;
		typedef typename Type<T10>::_StorageType _ST10;
		typedef typename Type<T11>::_StorageType _ST11;
		typedef typename Type<T12>::_StorageType _ST12;
		typedef typename Type<T13>::_StorageType _ST13;
		typedef typename Type<T14>::_StorageType _ST14;

	public:
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;
		_ST4  Param4;
		_ST5  Param5;
		_ST6  Param6;
		_ST7  Param7;
		_ST8  Param8;
		_ST9  Param9;
		_ST10 Param10;
		_ST11 Param11;
		_ST12 Param12;
		_ST13 Param13;
		_ST14 Param14;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()					? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()					? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()					? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()					? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()					? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()					? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			_T10 t10 = (cParser.Next()					? Type<T10>::ConvertFromString(cParser.GetValue()) : DefaultValue<T10>::Default());
			_T11 t11 = (cParser.Next()					? Type<T11>::ConvertFromString(cParser.GetValue()) : DefaultValue<T11>::Default());
			_T12 t12 = (cParser.Next()					? Type<T12>::ConvertFromString(cParser.GetValue()) : DefaultValue<T12>::Default());
			_T13 t13 = (cParser.Next()					? Type<T13>::ConvertFromString(cParser.GetValue()) : DefaultValue<T13>::Default());
			_T14 t14 = (cParser.Next()					? Type<T14>::ConvertFromString(cParser.GetValue()) : DefaultValue<T14>::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()				? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()				? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()				? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()				? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()				? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()				? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			_T10 t10 = (cParser.Next()				? Type<T10>::ConvertFromString(cParser.GetValue()) : DefaultValue<T10>::Default());
			_T11 t11 = (cParser.Next()				? Type<T11>::ConvertFromString(cParser.GetValue()) : DefaultValue<T11>::Default());
			_T12 t12 = (cParser.Next()				? Type<T12>::ConvertFromString(cParser.GetValue()) : DefaultValue<T12>::Default());
			_T13 t13 = (cParser.Next()				? Type<T13>::ConvertFromString(cParser.GetValue()) : DefaultValue<T13>::Default());
			_T14 t14 = (cParser.Next()				? Type<T14>::ConvertFromString(cParser.GetValue()) : DefaultValue<T14>::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14);
		}

	public:
		Params(const Params &cParams) :
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3),
			Param4(cParams.Param4),
			Param5(cParams.Param5),
			Param6(cParams.Param6),
			Param7(cParams.Param7),
			Param8(cParams.Param8),
			Param9(cParams.Param9),
			Param10(cParams.Param10),
			Param11(cParams.Param11),
			Param12(cParams.Param12),
			Param13(cParams.Param13),
			Param14(cParams.Param14)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14) :
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3)),
			Param4(Type<T4>  ::ConvertRealToStorage(t4)),
			Param5(Type<T5>  ::ConvertRealToStorage(t5)),
			Param6(Type<T6>  ::ConvertRealToStorage(t6)),
			Param7(Type<T7>  ::ConvertRealToStorage(t7)),
			Param8(Type<T8>  ::ConvertRealToStorage(t8)),
			Param9(Type<T9>  ::ConvertRealToStorage(t9)),
			Param10(Type<T10>::ConvertRealToStorage(t10)),
			Param11(Type<T11>::ConvertRealToStorage(t11)),
			Param12(Type<T12>::ConvertRealToStorage(t12)),
			Param13(Type<T13>::ConvertRealToStorage(t13)),
			Param14(Type<T14>::ConvertRealToStorage(t14))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Param0  = cParams.Param0;
			Param1  = cParams.Param1;
			Param2  = cParams.Param2;
			Param3  = cParams.Param3;
			Param4  = cParams.Param4;
			Param5  = cParams.Param5;
			Param6  = cParams.Param6;
			Param7  = cParams.Param7;
			Param8  = cParams.Param8;
			Param9  = cParams.Param9;
			Param10 = cParams.Param10;
			Param11 = cParams.Param11;
			Param12 = cParams.Param12;
			Param13 = cParams.Param13;
			Param14 = cParams.Param14;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::GetSignatureID();
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 15;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				case 10:	return Type<T10>::TypeID;
				case 11:	return Type<T11>::TypeID;
				case 12:	return Type<T12>::TypeID;
				case 13:	return Type<T13>::TypeID;
				case 14:	return Type<T14>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				case 4:		return (void*)&Param4;
				case 5:		return (void*)&Param5;
				case 6:		return (void*)&Param6;
				case 7:		return (void*)&Param7;
				case 8:		return (void*)&Param8;
				case 9:		return (void*)&Param9;
				case 10:	return (void*)&Param10;
				case 11:	return (void*)&Param11;
				case 12:	return (void*)&Param12;
				case 13:	return (void*)&Param13;
				case 14:	return (void*)&Param14;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 14 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public DynParams {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef typename Type<T12>::_Type _T12;
		typedef typename Type<T13>::_Type _T13;
		typedef typename Type<R>  ::_StorageType _SR;
		typedef typename Type<T0> ::_StorageType _ST0;
		typedef typename Type<T1> ::_StorageType _ST1;
		typedef typename Type<T2> ::_StorageType _ST2;
		typedef typename Type<T3> ::_StorageType _ST3;
		typedef typename Type<T4> ::_StorageType _ST4;
		typedef typename Type<T5> ::_StorageType _ST5;
		typedef typename Type<T6> ::_StorageType _ST6;
		typedef typename Type<T7> ::_StorageType _ST7;
		typedef typename Type<T8> ::_StorageType _ST8;
		typedef typename Type<T9> ::_StorageType _ST9;
		typedef typename Type<T10>::_StorageType _ST10;
		typedef typename Type<T11>::_StorageType _ST11;
		typedef typename Type<T12>::_StorageType _ST12;
		typedef typename Type<T13>::_StorageType _ST13;

	public:
		_SR   Return;
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;
		_ST4  Param4;
		_ST5  Param5;
		_ST6  Param6;
		_ST7  Param7;
		_ST8  Param8;
		_ST9  Param9;
		_ST10 Param10;
		_ST11 Param11;
		_ST12 Param12;
		_ST13 Param13;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()					? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()					? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()					? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()					? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()					? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()					? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			_T10 t10 = (cParser.Next()					? Type<T10>::ConvertFromString(cParser.GetValue()) : DefaultValue<T10>::Default());
			_T11 t11 = (cParser.Next()					? Type<T11>::ConvertFromString(cParser.GetValue()) : DefaultValue<T11>::Default());
			_T12 t12 = (cParser.Next()					? Type<T12>::ConvertFromString(cParser.GetValue()) : DefaultValue<T12>::Default());
			_T13 t13 = (cParser.Next()					? Type<T13>::ConvertFromString(cParser.GetValue()) : DefaultValue<T13>::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()				? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()				? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()				? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()				? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()				? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()				? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			_T10 t10 = (cParser.Next()				? Type<T10>::ConvertFromString(cParser.GetValue()) : DefaultValue<T10>::Default());
			_T11 t11 = (cParser.Next()				? Type<T11>::ConvertFromString(cParser.GetValue()) : DefaultValue<T11>::Default());
			_T12 t12 = (cParser.Next()				? Type<T12>::ConvertFromString(cParser.GetValue()) : DefaultValue<T12>::Default());
			_T13 t13 = (cParser.Next()				? Type<T13>::ConvertFromString(cParser.GetValue()) : DefaultValue<T13>::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13);
		}

	public:
		Params(const Params &cParams) :
			Return(DefaultValue<_SR>::Default()),
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3),
			Param4(cParams.Param4),
			Param5(cParams.Param5),
			Param6(cParams.Param6),
			Param7(cParams.Param7),
			Param8(cParams.Param8),
			Param9(cParams.Param9),
			Param10(cParams.Param10),
			Param11(cParams.Param11),
			Param12(cParams.Param12),
			Param13(cParams.Param13)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13) :
			Return(DefaultValue<_SR>::Default()),
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3)),
			Param4(Type<T4>  ::ConvertRealToStorage(t4)),
			Param5(Type<T5>  ::ConvertRealToStorage(t5)),
			Param6(Type<T6>  ::ConvertRealToStorage(t6)),
			Param7(Type<T7>  ::ConvertRealToStorage(t7)),
			Param8(Type<T8>  ::ConvertRealToStorage(t8)),
			Param9(Type<T9>  ::ConvertRealToStorage(t9)),
			Param10(Type<T10>::ConvertRealToStorage(t10)),
			Param11(Type<T11>::ConvertRealToStorage(t11)),
			Param12(Type<T12>::ConvertRealToStorage(t12)),
			Param13(Type<T13>::ConvertRealToStorage(t13))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Return  = cParams.Return;
			Param0  = cParams.Param0;
			Param1  = cParams.Param1;
			Param2  = cParams.Param2;
			Param3  = cParams.Param3;
			Param4  = cParams.Param4;
			Param5  = cParams.Param5;
			Param6  = cParams.Param6;
			Param7  = cParams.Param7;
			Param8  = cParams.Param8;
			Param9  = cParams.Param9;
			Param10 = cParams.Param10;
			Param11 = cParams.Param11;
			Param12 = cParams.Param12;
			Param13 = cParams.Param13;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual void *GetPointerToReturnValue() override
		{
			return (void*)&Return;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 14;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				case 10:	return Type<T10>::TypeID;
				case 11:	return Type<T11>::TypeID;
				case 12:	return Type<T12>::TypeID;
				case 13:	return Type<T13>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				case 4:		return (void*)&Param4;
				case 5:		return (void*)&Param5;
				case 6:		return (void*)&Param6;
				case 7:		return (void*)&Param7;
				case 8:		return (void*)&Param8;
				case 9:		return (void*)&Param9;
				case 10:	return (void*)&Param10;
				case 11:	return (void*)&Param11;
				case 12:	return (void*)&Param12;
				case 13:	return (void*)&Param13;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 14 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public DynParams {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef typename Type<T12>::_Type _T12;
		typedef typename Type<T13>::_Type _T13;
		typedef typename Type<T0> ::_StorageType _ST0;
		typedef typename Type<T1> ::_StorageType _ST1;
		typedef typename Type<T2> ::_StorageType _ST2;
		typedef typename Type<T3> ::_StorageType _ST3;
		typedef typename Type<T4> ::_StorageType _ST4;
		typedef typename Type<T5> ::_StorageType _ST5;
		typedef typename Type<T6> ::_StorageType _ST6;
		typedef typename Type<T7> ::_StorageType _ST7;
		typedef typename Type<T8> ::_StorageType _ST8;
		typedef typename Type<T9> ::_StorageType _ST9;
		typedef typename Type<T10>::_StorageType _ST10;
		typedef typename Type<T11>::_StorageType _ST11;
		typedef typename Type<T12>::_StorageType _ST12;
		typedef typename Type<T13>::_StorageType _ST13;

	public:
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;
		_ST4  Param4;
		_ST5  Param5;
		_ST6  Param6;
		_ST7  Param7;
		_ST8  Param8;
		_ST9  Param9;
		_ST10 Param10;
		_ST11 Param11;
		_ST12 Param12;
		_ST13 Param13;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()					? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()					? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()					? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()					? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()					? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()					? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			_T10 t10 = (cParser.Next()					? Type<T10>::ConvertFromString(cParser.GetValue()) : DefaultValue<T10>::Default());
			_T11 t11 = (cParser.Next()					? Type<T11>::ConvertFromString(cParser.GetValue()) : DefaultValue<T11>::Default());
			_T12 t12 = (cParser.Next()					? Type<T12>::ConvertFromString(cParser.GetValue()) : DefaultValue<T12>::Default());
			_T13 t13 = (cParser.Next()					? Type<T13>::ConvertFromString(cParser.GetValue()) : DefaultValue<T13>::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()				? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()				? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()				? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()				? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()				? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()				? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			_T10 t10 = (cParser.Next()				? Type<T10>::ConvertFromString(cParser.GetValue()) : DefaultValue<T10>::Default());
			_T11 t11 = (cParser.Next()				? Type<T11>::ConvertFromString(cParser.GetValue()) : DefaultValue<T11>::Default());
			_T12 t12 = (cParser.Next()				? Type<T12>::ConvertFromString(cParser.GetValue()) : DefaultValue<T12>::Default());
			_T13 t13 = (cParser.Next()				? Type<T13>::ConvertFromString(cParser.GetValue()) : DefaultValue<T13>::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13);
		}

	public:
		Params(const Params &cParams) :
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3),
			Param4(cParams.Param4),
			Param5(cParams.Param5),
			Param6(cParams.Param6),
			Param7(cParams.Param7),
			Param8(cParams.Param8),
			Param9(cParams.Param9),
			Param10(cParams.Param10),
			Param11(cParams.Param11),
			Param12(cParams.Param12),
			Param13(cParams.Param13)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13) :
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3)),
			Param4(Type<T4>  ::ConvertRealToStorage(t4)),
			Param5(Type<T5>  ::ConvertRealToStorage(t5)),
			Param6(Type<T6>  ::ConvertRealToStorage(t6)),
			Param7(Type<T7>  ::ConvertRealToStorage(t7)),
			Param8(Type<T8>  ::ConvertRealToStorage(t8)),
			Param9(Type<T9>  ::ConvertRealToStorage(t9)),
			Param10(Type<T10>::ConvertRealToStorage(t10)),
			Param11(Type<T11>::ConvertRealToStorage(t11)),
			Param12(Type<T12>::ConvertRealToStorage(t12)),
			Param13(Type<T13>::ConvertRealToStorage(t13))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Param0  = cParams.Param0;
			Param1  = cParams.Param1;
			Param2  = cParams.Param2;
			Param3  = cParams.Param3;
			Param4  = cParams.Param4;
			Param5  = cParams.Param5;
			Param6  = cParams.Param6;
			Param7  = cParams.Param7;
			Param8  = cParams.Param8;
			Param9  = cParams.Param9;
			Param10 = cParams.Param10;
			Param11 = cParams.Param11;
			Param12 = cParams.Param12;
			Param13 = cParams.Param13;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::GetSignatureID();
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 14;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				case 10:	return Type<T10>::TypeID;
				case 11:	return Type<T11>::TypeID;
				case 12:	return Type<T12>::TypeID;
				case 13:	return Type<T13>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				case 4:		return (void*)&Param4;
				case 5:		return (void*)&Param5;
				case 6:		return (void*)&Param6;
				case 7:		return (void*)&Param7;
				case 8:		return (void*)&Param8;
				case 9:		return (void*)&Param9;
				case 10:	return (void*)&Param10;
				case 11:	return (void*)&Param11;
				case 12:	return (void*)&Param12;
				case 13:	return (void*)&Param13;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 13 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public DynParams {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef typename Type<T12>::_Type _T12;
		typedef typename Type<R>  ::_StorageType _SR;
		typedef typename Type<T0> ::_StorageType _ST0;
		typedef typename Type<T1> ::_StorageType _ST1;
		typedef typename Type<T2> ::_StorageType _ST2;
		typedef typename Type<T3> ::_StorageType _ST3;
		typedef typename Type<T4> ::_StorageType _ST4;
		typedef typename Type<T5> ::_StorageType _ST5;
		typedef typename Type<T6> ::_StorageType _ST6;
		typedef typename Type<T7> ::_StorageType _ST7;
		typedef typename Type<T8> ::_StorageType _ST8;
		typedef typename Type<T9> ::_StorageType _ST9;
		typedef typename Type<T10>::_StorageType _ST10;
		typedef typename Type<T11>::_StorageType _ST11;
		typedef typename Type<T12>::_StorageType _ST12;

	public:
		_SR   Return;
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;
		_ST4  Param4;
		_ST5  Param5;
		_ST6  Param6;
		_ST7  Param7;
		_ST8  Param8;
		_ST9  Param9;
		_ST10 Param10;
		_ST11 Param11;
		_ST12 Param12;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()					? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()					? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()					? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()					? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()					? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()					? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			_T10 t10 = (cParser.Next()					? Type<T10>::ConvertFromString(cParser.GetValue()) : DefaultValue<T10>::Default());
			_T11 t11 = (cParser.Next()					? Type<T11>::ConvertFromString(cParser.GetValue()) : DefaultValue<T11>::Default());
			_T12 t12 = (cParser.Next()					? Type<T12>::ConvertFromString(cParser.GetValue()) : DefaultValue<T12>::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()				? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()				? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()				? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()				? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()				? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()				? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			_T10 t10 = (cParser.Next()				? Type<T10>::ConvertFromString(cParser.GetValue()) : DefaultValue<T10>::Default());
			_T11 t11 = (cParser.Next()				? Type<T11>::ConvertFromString(cParser.GetValue()) : DefaultValue<T11>::Default());
			_T12 t12 = (cParser.Next()				? Type<T12>::ConvertFromString(cParser.GetValue()) : DefaultValue<T12>::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12);
		}

	public:
		Params(const Params &cParams) :
			Return(DefaultValue<_SR>::Default()),
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3),
			Param4(cParams.Param4),
			Param5(cParams.Param5),
			Param6(cParams.Param6),
			Param7(cParams.Param7),
			Param8(cParams.Param8),
			Param9(cParams.Param9),
			Param10(cParams.Param10),
			Param11(cParams.Param11),
			Param12(cParams.Param12)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12) :
			Return(DefaultValue<_SR>::Default()),
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3)),
			Param4(Type<T4>  ::ConvertRealToStorage(t4)),
			Param5(Type<T5>  ::ConvertRealToStorage(t5)),
			Param6(Type<T6>  ::ConvertRealToStorage(t6)),
			Param7(Type<T7>  ::ConvertRealToStorage(t7)),
			Param8(Type<T8>  ::ConvertRealToStorage(t8)),
			Param9(Type<T9>  ::ConvertRealToStorage(t9)),
			Param10(Type<T10>::ConvertRealToStorage(t10)),
			Param11(Type<T11>::ConvertRealToStorage(t11)),
			Param12(Type<T12>::ConvertRealToStorage(t12))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Return  = cParams.Return;
			Param0  = cParams.Param0;
			Param1  = cParams.Param1;
			Param2  = cParams.Param2;
			Param3  = cParams.Param3;
			Param4  = cParams.Param4;
			Param5  = cParams.Param5;
			Param6  = cParams.Param6;
			Param7  = cParams.Param7;
			Param8  = cParams.Param8;
			Param9  = cParams.Param9;
			Param10 = cParams.Param10;
			Param11 = cParams.Param11;
			Param12 = cParams.Param12;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual void *GetPointerToReturnValue() override
		{
			return (void*)&Return;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 13;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				case 10:	return Type<T10>::TypeID;
				case 11:	return Type<T11>::TypeID;
				case 12:	return Type<T12>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				case 4:		return (void*)&Param4;
				case 5:		return (void*)&Param5;
				case 6:		return (void*)&Param6;
				case 7:		return (void*)&Param7;
				case 8:		return (void*)&Param8;
				case 9:		return (void*)&Param9;
				case 10:	return (void*)&Param10;
				case 11:	return (void*)&Param11;
				case 12:	return (void*)&Param12;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 13 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public DynParams {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef typename Type<T12>::_Type _T12;
		typedef typename Type<T0> ::_StorageType _ST0;
		typedef typename Type<T1> ::_StorageType _ST1;
		typedef typename Type<T2> ::_StorageType _ST2;
		typedef typename Type<T3> ::_StorageType _ST3;
		typedef typename Type<T4> ::_StorageType _ST4;
		typedef typename Type<T5> ::_StorageType _ST5;
		typedef typename Type<T6> ::_StorageType _ST6;
		typedef typename Type<T7> ::_StorageType _ST7;
		typedef typename Type<T8> ::_StorageType _ST8;
		typedef typename Type<T9> ::_StorageType _ST9;
		typedef typename Type<T10>::_StorageType _ST10;
		typedef typename Type<T11>::_StorageType _ST11;
		typedef typename Type<T12>::_StorageType _ST12;

	public:
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;
		_ST4  Param4;
		_ST5  Param5;
		_ST6  Param6;
		_ST7  Param7;
		_ST8  Param8;
		_ST9  Param9;
		_ST10 Param10;
		_ST11 Param11;
		_ST12 Param12;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()					? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()					? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()					? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()					? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()					? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()					? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			_T10 t10 = (cParser.Next()					? Type<T10>::ConvertFromString(cParser.GetValue()) : DefaultValue<T10>::Default());
			_T11 t11 = (cParser.Next()					? Type<T11>::ConvertFromString(cParser.GetValue()) : DefaultValue<T11>::Default());
			_T12 t12 = (cParser.Next()					? Type<T12>::ConvertFromString(cParser.GetValue()) : DefaultValue<T12>::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()				? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()				? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()				? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()				? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()				? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()				? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			_T10 t10 = (cParser.Next()				? Type<T10>::ConvertFromString(cParser.GetValue()) : DefaultValue<T10>::Default());
			_T11 t11 = (cParser.Next()				? Type<T11>::ConvertFromString(cParser.GetValue()) : DefaultValue<T11>::Default());
			_T12 t12 = (cParser.Next()				? Type<T12>::ConvertFromString(cParser.GetValue()) : DefaultValue<T12>::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12);
		}

	public:
		Params(const Params &cParams) :
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3),
			Param4(cParams.Param4),
			Param5(cParams.Param5),
			Param6(cParams.Param6),
			Param7(cParams.Param7),
			Param8(cParams.Param8),
			Param9(cParams.Param9),
			Param10(cParams.Param10),
			Param11(cParams.Param11),
			Param12(cParams.Param12)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12) :
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3)),
			Param4(Type<T4>  ::ConvertRealToStorage(t4)),
			Param5(Type<T5>  ::ConvertRealToStorage(t5)),
			Param6(Type<T6>  ::ConvertRealToStorage(t6)),
			Param7(Type<T7>  ::ConvertRealToStorage(t7)),
			Param8(Type<T8>  ::ConvertRealToStorage(t8)),
			Param9(Type<T9>  ::ConvertRealToStorage(t9)),
			Param10(Type<T10>::ConvertRealToStorage(t10)),
			Param11(Type<T11>::ConvertRealToStorage(t11)),
			Param12(Type<T12>::ConvertRealToStorage(t12))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Param0  = cParams.Param0;
			Param1  = cParams.Param1;
			Param2  = cParams.Param2;
			Param3  = cParams.Param3;
			Param4  = cParams.Param4;
			Param5  = cParams.Param5;
			Param6  = cParams.Param6;
			Param7  = cParams.Param7;
			Param8  = cParams.Param8;
			Param9  = cParams.Param9;
			Param10 = cParams.Param10;
			Param11 = cParams.Param11;
			Param12 = cParams.Param12;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::GetSignatureID();
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 13;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				case 10:	return Type<T10>::TypeID;
				case 11:	return Type<T11>::TypeID;
				case 12:	return Type<T12>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				case 4:		return (void*)&Param4;
				case 5:		return (void*)&Param5;
				case 6:		return (void*)&Param6;
				case 7:		return (void*)&Param7;
				case 8:		return (void*)&Param8;
				case 9:		return (void*)&Param9;
				case 10:	return (void*)&Param10;
				case 11:	return (void*)&Param11;
				case 12:	return (void*)&Param12;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 12 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public DynParams {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef typename Type<R>  ::_StorageType _SR;
		typedef typename Type<T0> ::_StorageType _ST0;
		typedef typename Type<T1> ::_StorageType _ST1;
		typedef typename Type<T2> ::_StorageType _ST2;
		typedef typename Type<T3> ::_StorageType _ST3;
		typedef typename Type<T4> ::_StorageType _ST4;
		typedef typename Type<T5> ::_StorageType _ST5;
		typedef typename Type<T6> ::_StorageType _ST6;
		typedef typename Type<T7> ::_StorageType _ST7;
		typedef typename Type<T8> ::_StorageType _ST8;
		typedef typename Type<T9> ::_StorageType _ST9;
		typedef typename Type<T10>::_StorageType _ST10;
		typedef typename Type<T11>::_StorageType _ST11;

	public:
		_SR   Return;
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;
		_ST4  Param4;
		_ST5  Param5;
		_ST6  Param6;
		_ST7  Param7;
		_ST8  Param8;
		_ST9  Param9;
		_ST10 Param10;
		_ST11 Param11;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()					? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()					? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()					? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()					? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()					? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()					? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			_T10 t10 = (cParser.Next()					? Type<T10>::ConvertFromString(cParser.GetValue()) : DefaultValue<T10>::Default());
			_T11 t11 = (cParser.Next()					? Type<T11>::ConvertFromString(cParser.GetValue()) : DefaultValue<T11>::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()				? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()				? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()				? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()				? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()				? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()				? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			_T10 t10 = (cParser.Next()				? Type<T10>::ConvertFromString(cParser.GetValue()) : DefaultValue<T10>::Default());
			_T11 t11 = (cParser.Next()				? Type<T11>::ConvertFromString(cParser.GetValue()) : DefaultValue<T11>::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11);
		}

	public:
		Params(const Params &cParams) :
			Return(DefaultValue<_SR>::Default()),
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3),
			Param4(cParams.Param4),
			Param5(cParams.Param5),
			Param6(cParams.Param6),
			Param7(cParams.Param7),
			Param8(cParams.Param8),
			Param9(cParams.Param9),
			Param10(cParams.Param10),
			Param11(cParams.Param11)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11) :
			Return(DefaultValue<_SR>::Default()),
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3)),
			Param4(Type<T4>  ::ConvertRealToStorage(t4)),
			Param5(Type<T5>  ::ConvertRealToStorage(t5)),
			Param6(Type<T6>  ::ConvertRealToStorage(t6)),
			Param7(Type<T7>  ::ConvertRealToStorage(t7)),
			Param8(Type<T8>  ::ConvertRealToStorage(t8)),
			Param9(Type<T9>  ::ConvertRealToStorage(t9)),
			Param10(Type<T10>::ConvertRealToStorage(t10)),
			Param11(Type<T11>::ConvertRealToStorage(t11))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Return  = cParams.Return;
			Param0  = cParams.Param0;
			Param1  = cParams.Param1;
			Param2  = cParams.Param2;
			Param3  = cParams.Param3;
			Param4  = cParams.Param4;
			Param5  = cParams.Param5;
			Param6  = cParams.Param6;
			Param7  = cParams.Param7;
			Param8  = cParams.Param8;
			Param9  = cParams.Param9;
			Param10 = cParams.Param10;
			Param11 = cParams.Param11;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual void *GetPointerToReturnValue() override
		{
			return (void*)&Return;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 12;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				case 10:	return Type<T10>::TypeID;
				case 11:	return Type<T11>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				case 4:		return (void*)&Param4;
				case 5:		return (void*)&Param5;
				case 6:		return (void*)&Param6;
				case 7:		return (void*)&Param7;
				case 8:		return (void*)&Param8;
				case 9:		return (void*)&Param9;
				case 10:	return (void*)&Param10;
				case 11:	return (void*)&Param11;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 12 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public DynParams {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T11>::_Type _T11;
		typedef typename Type<T0> ::_StorageType _ST0;
		typedef typename Type<T1> ::_StorageType _ST1;
		typedef typename Type<T2> ::_StorageType _ST2;
		typedef typename Type<T3> ::_StorageType _ST3;
		typedef typename Type<T4> ::_StorageType _ST4;
		typedef typename Type<T5> ::_StorageType _ST5;
		typedef typename Type<T6> ::_StorageType _ST6;
		typedef typename Type<T7> ::_StorageType _ST7;
		typedef typename Type<T8> ::_StorageType _ST8;
		typedef typename Type<T9> ::_StorageType _ST9;
		typedef typename Type<T10>::_StorageType _ST10;
		typedef typename Type<T11>::_StorageType _ST11;

	public:
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;
		_ST4  Param4;
		_ST5  Param5;
		_ST6  Param6;
		_ST7  Param7;
		_ST8  Param8;
		_ST9  Param9;
		_ST10 Param10;
		_ST11 Param11;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()					? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()					? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()					? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()					? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()					? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()					? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			_T10 t10 = (cParser.Next()					? Type<T10>::ConvertFromString(cParser.GetValue()) : DefaultValue<T10>::Default());
			_T11 t11 = (cParser.Next()					? Type<T11>::ConvertFromString(cParser.GetValue()) : DefaultValue<T11>::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()				? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()				? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()				? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()				? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()				? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()				? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			_T10 t10 = (cParser.Next()				? Type<T10>::ConvertFromString(cParser.GetValue()) : DefaultValue<T10>::Default());
			_T11 t11 = (cParser.Next()				? Type<T11>::ConvertFromString(cParser.GetValue()) : DefaultValue<T11>::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11);
		}

	public:
		Params(const Params &cParams) :
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3),
			Param4(cParams.Param4),
			Param5(cParams.Param5),
			Param6(cParams.Param6),
			Param7(cParams.Param7),
			Param8(cParams.Param8),
			Param9(cParams.Param9),
			Param10(cParams.Param10),
			Param11(cParams.Param11)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11) :
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3)),
			Param4(Type<T4>  ::ConvertRealToStorage(t4)),
			Param5(Type<T5>  ::ConvertRealToStorage(t5)),
			Param6(Type<T6>  ::ConvertRealToStorage(t6)),
			Param7(Type<T7>  ::ConvertRealToStorage(t7)),
			Param8(Type<T8>  ::ConvertRealToStorage(t8)),
			Param9(Type<T9>  ::ConvertRealToStorage(t9)),
			Param10(Type<T10>::ConvertRealToStorage(t10)),
			Param11(Type<T11>::ConvertRealToStorage(t11))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Param0  = cParams.Param0;
			Param1  = cParams.Param1;
			Param2  = cParams.Param2;
			Param3  = cParams.Param3;
			Param4  = cParams.Param4;
			Param5  = cParams.Param5;
			Param6  = cParams.Param6;
			Param7  = cParams.Param7;
			Param8  = cParams.Param8;
			Param9  = cParams.Param9;
			Param10 = cParams.Param10;
			Param11 = cParams.Param11;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::GetSignatureID();
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 12;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				case 10:	return Type<T10>::TypeID;
				case 11:	return Type<T11>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				case 4:		return (void*)&Param4;
				case 5:		return (void*)&Param5;
				case 6:		return (void*)&Param6;
				case 7:		return (void*)&Param7;
				case 8:		return (void*)&Param8;
				case 9:		return (void*)&Param9;
				case 10:	return (void*)&Param10;
				case 11:	return (void*)&Param11;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 11 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public DynParams {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<R>  ::_StorageType _SR;
		typedef typename Type<T0> ::_StorageType _ST0;
		typedef typename Type<T1> ::_StorageType _ST1;
		typedef typename Type<T2> ::_StorageType _ST2;
		typedef typename Type<T3> ::_StorageType _ST3;
		typedef typename Type<T4> ::_StorageType _ST4;
		typedef typename Type<T5> ::_StorageType _ST5;
		typedef typename Type<T6> ::_StorageType _ST6;
		typedef typename Type<T7> ::_StorageType _ST7;
		typedef typename Type<T8> ::_StorageType _ST8;
		typedef typename Type<T9> ::_StorageType _ST9;
		typedef typename Type<T10>::_StorageType _ST10;

	public:
		_SR   Return;
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;
		_ST4  Param4;
		_ST5  Param5;
		_ST6  Param6;
		_ST7  Param7;
		_ST8  Param8;
		_ST9  Param9;
		_ST10 Param10;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()					? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()					? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()					? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()					? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()					? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()					? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			_T10 t10 = (cParser.Next()					? Type<T10>::ConvertFromString(cParser.GetValue()) : DefaultValue<T10>::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()				? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()				? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()				? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()				? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()				? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()				? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			_T10 t10 = (cParser.Next()				? Type<T10>::ConvertFromString(cParser.GetValue()) : DefaultValue<T10>::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);
		}

	public:
		Params(const Params &cParams) :
			Return(DefaultValue<_SR>::Default()),
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3),
			Param4(cParams.Param4),
			Param5(cParams.Param5),
			Param6(cParams.Param6),
			Param7(cParams.Param7),
			Param8(cParams.Param8),
			Param9(cParams.Param9),
			Param10(cParams.Param10)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10) :
			Return(DefaultValue<_SR>::Default()),
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3)),
			Param4(Type<T4>  ::ConvertRealToStorage(t4)),
			Param5(Type<T5>  ::ConvertRealToStorage(t5)),
			Param6(Type<T6>  ::ConvertRealToStorage(t6)),
			Param7(Type<T7>  ::ConvertRealToStorage(t7)),
			Param8(Type<T8>  ::ConvertRealToStorage(t8)),
			Param9(Type<T9>  ::ConvertRealToStorage(t9)),
			Param10(Type<T10>::ConvertRealToStorage(t10))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Return  = cParams.Return;
			Param0  = cParams.Param0;
			Param1  = cParams.Param1;
			Param2  = cParams.Param2;
			Param3  = cParams.Param3;
			Param4  = cParams.Param4;
			Param5  = cParams.Param5;
			Param6  = cParams.Param6;
			Param7  = cParams.Param7;
			Param8  = cParams.Param8;
			Param9  = cParams.Param9;
			Param10 = cParams.Param10;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual void *GetPointerToReturnValue() override
		{
			return (void*)&Return;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 11;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				case 10:	return Type<T10>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				case 4:		return (void*)&Param4;
				case 5:		return (void*)&Param5;
				case 6:		return (void*)&Param6;
				case 7:		return (void*)&Param7;
				case 8:		return (void*)&Param8;
				case 9:		return (void*)&Param9;
				case 10:	return (void*)&Param10;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 11 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public DynParams {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T10>::_Type _T10;
		typedef typename Type<T0> ::_StorageType _ST0;
		typedef typename Type<T1> ::_StorageType _ST1;
		typedef typename Type<T2> ::_StorageType _ST2;
		typedef typename Type<T3> ::_StorageType _ST3;
		typedef typename Type<T4> ::_StorageType _ST4;
		typedef typename Type<T5> ::_StorageType _ST5;
		typedef typename Type<T6> ::_StorageType _ST6;
		typedef typename Type<T7> ::_StorageType _ST7;
		typedef typename Type<T8> ::_StorageType _ST8;
		typedef typename Type<T9> ::_StorageType _ST9;
		typedef typename Type<T10>::_StorageType _ST10;

	public:
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;
		_ST4  Param4;
		_ST5  Param5;
		_ST6  Param6;
		_ST7  Param7;
		_ST8  Param8;
		_ST9  Param9;
		_ST10 Param10;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()					? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()					? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()					? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()					? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()					? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()					? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			_T10 t10 = (cParser.Next()					? Type<T10>::ConvertFromString(cParser.GetValue()) : DefaultValue<T10>::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()				? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()				? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()				? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()				? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()				? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()				? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			_T10 t10 = (cParser.Next()				? Type<T10>::ConvertFromString(cParser.GetValue()) : DefaultValue<T10>::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);
		}

	public:
		Params(const Params &cParams) :
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3),
			Param4(cParams.Param4),
			Param5(cParams.Param5),
			Param6(cParams.Param6),
			Param7(cParams.Param7),
			Param8(cParams.Param8),
			Param9(cParams.Param9),
			Param10(cParams.Param10)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10) :
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3)),
			Param4(Type<T4>  ::ConvertRealToStorage(t4)),
			Param5(Type<T5>  ::ConvertRealToStorage(t5)),
			Param6(Type<T6>  ::ConvertRealToStorage(t6)),
			Param7(Type<T7>  ::ConvertRealToStorage(t7)),
			Param8(Type<T8>  ::ConvertRealToStorage(t8)),
			Param9(Type<T9>  ::ConvertRealToStorage(t9)),
			Param10(Type<T10>::ConvertRealToStorage(t10))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Param0  = cParams.Param0;
			Param1  = cParams.Param1;
			Param2  = cParams.Param2;
			Param3  = cParams.Param3;
			Param4  = cParams.Param4;
			Param5  = cParams.Param5;
			Param6  = cParams.Param6;
			Param7  = cParams.Param7;
			Param8  = cParams.Param8;
			Param9  = cParams.Param9;
			Param10 = cParams.Param10;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::GetSignatureID();
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 11;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				case 10:	return Type<T10>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				case 4:		return (void*)&Param4;
				case 5:		return (void*)&Param5;
				case 6:		return (void*)&Param6;
				case 7:		return (void*)&Param7;
				case 8:		return (void*)&Param8;
				case 9:		return (void*)&Param9;
				case 10:	return (void*)&Param10;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 10 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public DynParams {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<R> ::_StorageType _SR;
		typedef typename Type<T0>::_StorageType _ST0;
		typedef typename Type<T1>::_StorageType _ST1;
		typedef typename Type<T2>::_StorageType _ST2;
		typedef typename Type<T3>::_StorageType _ST3;
		typedef typename Type<T4>::_StorageType _ST4;
		typedef typename Type<T5>::_StorageType _ST5;
		typedef typename Type<T6>::_StorageType _ST6;
		typedef typename Type<T7>::_StorageType _ST7;
		typedef typename Type<T8>::_StorageType _ST8;
		typedef typename Type<T9>::_StorageType _ST9;

	public:
		_SR   Return;
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;
		_ST4  Param4;
		_ST5  Param5;
		_ST6  Param6;
		_ST7  Param7;
		_ST8  Param8;
		_ST9  Param9;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()					? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()					? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()					? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()					? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()					? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()					? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()				? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()				? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()				? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()				? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()				? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()				? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9);
		}

	public:
		Params(const Params &cParams) :
			Return(DefaultValue<_SR>::Default()),
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3),
			Param4(cParams.Param4),
			Param5(cParams.Param5),
			Param6(cParams.Param6),
			Param7(cParams.Param7),
			Param8(cParams.Param8),
			Param9(cParams.Param9)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9) :
			Return(DefaultValue<_SR>::Default()),
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3)),
			Param4(Type<T4>  ::ConvertRealToStorage(t4)),
			Param5(Type<T5>  ::ConvertRealToStorage(t5)),
			Param6(Type<T6>  ::ConvertRealToStorage(t6)),
			Param7(Type<T7>  ::ConvertRealToStorage(t7)),
			Param8(Type<T8>  ::ConvertRealToStorage(t8)),
			Param9(Type<T9>  ::ConvertRealToStorage(t9))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Return = cParams.Return;
			Param0 = cParams.Param0;
			Param1 = cParams.Param1;
			Param2 = cParams.Param2;
			Param3 = cParams.Param3;
			Param4 = cParams.Param4;
			Param5 = cParams.Param5;
			Param6 = cParams.Param6;
			Param7 = cParams.Param7;
			Param8 = cParams.Param8;
			Param9 = cParams.Param9;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual void *GetPointerToReturnValue() override
		{
			return (void*)&Return;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 10;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				case 4:		return (void*)&Param4;
				case 5:		return (void*)&Param5;
				case 6:		return (void*)&Param6;
				case 7:		return (void*)&Param7;
				case 8:		return (void*)&Param8;
				case 9:		return (void*)&Param9;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 10 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public DynParams {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T9> ::_Type _T9;
		typedef typename Type<T0>::_StorageType _ST0;
		typedef typename Type<T1>::_StorageType _ST1;
		typedef typename Type<T2>::_StorageType _ST2;
		typedef typename Type<T3>::_StorageType _ST3;
		typedef typename Type<T4>::_StorageType _ST4;
		typedef typename Type<T5>::_StorageType _ST5;
		typedef typename Type<T6>::_StorageType _ST6;
		typedef typename Type<T7>::_StorageType _ST7;
		typedef typename Type<T8>::_StorageType _ST8;
		typedef typename Type<T9>::_StorageType _ST9;

	public:
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;
		_ST4  Param4;
		_ST5  Param5;
		_ST6  Param6;
		_ST7  Param7;
		_ST8  Param8;
		_ST9  Param9;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()					? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()					? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()					? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()					? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()					? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()					? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()				? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()				? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()				? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()				? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()				? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			_T9  t9  = (cParser.Next()				? Type<T9> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T9> ::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9);
		}

	public:
		Params(const Params &cParams) :
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3),
			Param4(cParams.Param4),
			Param5(cParams.Param5),
			Param6(cParams.Param6),
			Param7(cParams.Param7),
			Param8(cParams.Param8),
			Param9(cParams.Param9)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9) :
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3)),
			Param4(Type<T4>  ::ConvertRealToStorage(t4)),
			Param5(Type<T5>  ::ConvertRealToStorage(t5)),
			Param6(Type<T6>  ::ConvertRealToStorage(t6)),
			Param7(Type<T7>  ::ConvertRealToStorage(t7)),
			Param8(Type<T8>  ::ConvertRealToStorage(t8)),
			Param9(Type<T9>  ::ConvertRealToStorage(t9))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Param0 = cParams.Param0;
			Param1 = cParams.Param1;
			Param2 = cParams.Param2;
			Param3 = cParams.Param3;
			Param4 = cParams.Param4;
			Param5 = cParams.Param5;
			Param6 = cParams.Param6;
			Param7 = cParams.Param7;
			Param8 = cParams.Param8;
			Param9 = cParams.Param9;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::GetSignatureID();
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 10;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				case 9:		return Type<T9> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				case 4:		return (void*)&Param4;
				case 5:		return (void*)&Param5;
				case 6:		return (void*)&Param6;
				case 7:		return (void*)&Param7;
				case 8:		return (void*)&Param8;
				case 9:		return (void*)&Param9;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 9 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> : public DynParams {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<R> ::_StorageType _SR;
		typedef typename Type<T0>::_StorageType _ST0;
		typedef typename Type<T1>::_StorageType _ST1;
		typedef typename Type<T2>::_StorageType _ST2;
		typedef typename Type<T3>::_StorageType _ST3;
		typedef typename Type<T4>::_StorageType _ST4;
		typedef typename Type<T5>::_StorageType _ST5;
		typedef typename Type<T6>::_StorageType _ST6;
		typedef typename Type<T7>::_StorageType _ST7;
		typedef typename Type<T8>::_StorageType _ST8;

	public:
		_SR   Return;
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;
		_ST4  Param4;
		_ST5  Param5;
		_ST6  Param6;
		_ST7  Param7;
		_ST8  Param8;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()					? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()					? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()					? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()					? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()					? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()				? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()				? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()				? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()				? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()				? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8);
		}

	public:
		Params(const Params &cParams) :
			Return(DefaultValue<_SR>::Default()),
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3),
			Param4(cParams.Param4),
			Param5(cParams.Param5),
			Param6(cParams.Param6),
			Param7(cParams.Param7),
			Param8(cParams.Param8)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8) :
			Return(DefaultValue<_SR>::Default()),
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3)),
			Param4(Type<T4>  ::ConvertRealToStorage(t4)),
			Param5(Type<T5>  ::ConvertRealToStorage(t5)),
			Param6(Type<T6>  ::ConvertRealToStorage(t6)),
			Param7(Type<T7>  ::ConvertRealToStorage(t7)),
			Param8(Type<T8>  ::ConvertRealToStorage(t8))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Return = cParams.Return;
			Param0 = cParams.Param0;
			Param1 = cParams.Param1;
			Param2 = cParams.Param2;
			Param3 = cParams.Param3;
			Param4 = cParams.Param4;
			Param5 = cParams.Param5;
			Param6 = cParams.Param6;
			Param7 = cParams.Param7;
			Param8 = cParams.Param8;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual void *GetPointerToReturnValue() override
		{
			return (void*)&Return;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 9;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				case 4:		return (void*)&Param4;
				case 5:		return (void*)&Param5;
				case 6:		return (void*)&Param6;
				case 7:		return (void*)&Param7;
				case 8:		return (void*)&Param8;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 9 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> : public DynParams {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T8> ::_Type _T8;
		typedef typename Type<T0>::_StorageType _ST0;
		typedef typename Type<T1>::_StorageType _ST1;
		typedef typename Type<T2>::_StorageType _ST2;
		typedef typename Type<T3>::_StorageType _ST3;
		typedef typename Type<T4>::_StorageType _ST4;
		typedef typename Type<T5>::_StorageType _ST5;
		typedef typename Type<T6>::_StorageType _ST6;
		typedef typename Type<T7>::_StorageType _ST7;
		typedef typename Type<T8>::_StorageType _ST8;

	public:
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;
		_ST4  Param4;
		_ST5  Param5;
		_ST6  Param6;
		_ST7  Param7;
		_ST8  Param8;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()					? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()					? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()					? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()					? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()					? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()				? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()				? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()				? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()				? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			_T8  t8  = (cParser.Next()				? Type<T8> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T8> ::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7, t8);
		}

	public:
		Params(const Params &cParams) :
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3),
			Param4(cParams.Param4),
			Param5(cParams.Param5),
			Param6(cParams.Param6),
			Param7(cParams.Param7),
			Param8(cParams.Param8)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8) :
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3)),
			Param4(Type<T4>  ::ConvertRealToStorage(t4)),
			Param5(Type<T5>  ::ConvertRealToStorage(t5)),
			Param6(Type<T6>  ::ConvertRealToStorage(t6)),
			Param7(Type<T7>  ::ConvertRealToStorage(t7)),
			Param8(Type<T8>  ::ConvertRealToStorage(t8))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Param0 = cParams.Param0;
			Param1 = cParams.Param1;
			Param2 = cParams.Param2;
			Param3 = cParams.Param3;
			Param4 = cParams.Param4;
			Param5 = cParams.Param5;
			Param6 = cParams.Param6;
			Param7 = cParams.Param7;
			Param8 = cParams.Param8;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>::GetSignatureID();
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 9;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				case 8:		return Type<T8> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				case 4:		return (void*)&Param4;
				case 5:		return (void*)&Param5;
				case 6:		return (void*)&Param6;
				case 7:		return (void*)&Param7;
				case 8:		return (void*)&Param8;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 8 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class Params<R, T0, T1, T2, T3, T4, T5, T6, T7> : public DynParams {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<R> ::_StorageType _SR;
		typedef typename Type<T0>::_StorageType _ST0;
		typedef typename Type<T1>::_StorageType _ST1;
		typedef typename Type<T2>::_StorageType _ST2;
		typedef typename Type<T3>::_StorageType _ST3;
		typedef typename Type<T4>::_StorageType _ST4;
		typedef typename Type<T5>::_StorageType _ST5;
		typedef typename Type<T6>::_StorageType _ST6;
		typedef typename Type<T7>::_StorageType _ST7;

	public:
		_SR   Return;
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;
		_ST4  Param4;
		_ST5  Param5;
		_ST6  Param6;
		_ST7  Param7;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()					? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()					? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()					? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()					? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()				? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()				? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()				? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()				? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7);
		}

	public:
		Params(const Params &cParams) :
			Return(DefaultValue<_SR>::Default()),
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3),
			Param4(cParams.Param4),
			Param5(cParams.Param5),
			Param6(cParams.Param6),
			Param7(cParams.Param7)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7) :
			Return(DefaultValue<_SR>::Default()),
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3)),
			Param4(Type<T4>  ::ConvertRealToStorage(t4)),
			Param5(Type<T5>  ::ConvertRealToStorage(t5)),
			Param6(Type<T6>  ::ConvertRealToStorage(t6)),
			Param7(Type<T7>  ::ConvertRealToStorage(t7))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Return = cParams.Return;
			Param0 = cParams.Param0;
			Param1 = cParams.Param1;
			Param2 = cParams.Param2;
			Param3 = cParams.Param3;
			Param4 = cParams.Param4;
			Param5 = cParams.Param5;
			Param6 = cParams.Param6;
			Param7 = cParams.Param7;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual void *GetPointerToReturnValue() override
		{
			return (void*)&Return;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 8;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				case 4:		return (void*)&Param4;
				case 5:		return (void*)&Param5;
				case 6:		return (void*)&Param6;
				case 7:		return (void*)&Param7;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 8 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class Params<void, T0, T1, T2, T3, T4, T5, T6, T7> : public DynParams {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef typename Type<T0>::_StorageType _ST0;
		typedef typename Type<T1>::_StorageType _ST1;
		typedef typename Type<T2>::_StorageType _ST2;
		typedef typename Type<T3>::_StorageType _ST3;
		typedef typename Type<T4>::_StorageType _ST4;
		typedef typename Type<T5>::_StorageType _ST5;
		typedef typename Type<T6>::_StorageType _ST6;
		typedef typename Type<T7>::_StorageType _ST7;

	public:
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;
		_ST4  Param4;
		_ST5  Param5;
		_ST6  Param6;
		_ST7  Param7;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()					? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()					? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()					? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()					? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()				? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()				? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()				? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			_T7  t7  = (cParser.Next()				? Type<T7> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T7> ::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6, t7);
		}

	public:
		Params(const Params &cParams) :
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3),
			Param4(cParams.Param4),
			Param5(cParams.Param5),
			Param6(cParams.Param6),
			Param7(cParams.Param7)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7) :
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3)),
			Param4(Type<T4>  ::ConvertRealToStorage(t4)),
			Param5(Type<T5>  ::ConvertRealToStorage(t5)),
			Param6(Type<T6>  ::ConvertRealToStorage(t6)),
			Param7(Type<T7>  ::ConvertRealToStorage(t7))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Param0 = cParams.Param0;
			Param1 = cParams.Param1;
			Param2 = cParams.Param2;
			Param3 = cParams.Param3;
			Param4 = cParams.Param4;
			Param5 = cParams.Param5;
			Param6 = cParams.Param6;
			Param7 = cParams.Param7;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<void, T0, T1, T2, T3, T4, T5, T6, T7>::GetSignatureID();
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 8;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				case 7:		return Type<T7> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				case 4:		return (void*)&Param4;
				case 5:		return (void*)&Param5;
				case 6:		return (void*)&Param6;
				case 7:		return (void*)&Param7;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 7 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class Params<R, T0, T1, T2, T3, T4, T5, T6> : public DynParams {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<R> ::_StorageType _SR;
		typedef typename Type<T0>::_StorageType _ST0;
		typedef typename Type<T1>::_StorageType _ST1;
		typedef typename Type<T2>::_StorageType _ST2;
		typedef typename Type<T3>::_StorageType _ST3;
		typedef typename Type<T4>::_StorageType _ST4;
		typedef typename Type<T5>::_StorageType _ST5;
		typedef typename Type<T6>::_StorageType _ST6;

	public:
		_SR   Return;
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;
		_ST4  Param4;
		_ST5  Param5;
		_ST6  Param6;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()					? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()					? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()					? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()				? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()				? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()				? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6);
		}

	public:
		Params(const Params &cParams) :
			Return(DefaultValue<_SR>::Default()),
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3),
			Param4(cParams.Param4),
			Param5(cParams.Param5),
			Param6(cParams.Param6)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6) :
			Return(DefaultValue<_SR>::Default()),
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3)),
			Param4(Type<T4>  ::ConvertRealToStorage(t4)),
			Param5(Type<T5>  ::ConvertRealToStorage(t5)),
			Param6(Type<T6>  ::ConvertRealToStorage(t6))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Return = cParams.Return;
			Param0 = cParams.Param0;
			Param1 = cParams.Param1;
			Param2 = cParams.Param2;
			Param3 = cParams.Param3;
			Param4 = cParams.Param4;
			Param5 = cParams.Param5;
			Param6 = cParams.Param6;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual void *GetPointerToReturnValue() override
		{
			return (void*)&Return;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 7;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				case 4:		return (void*)&Param4;
				case 5:		return (void*)&Param5;
				case 6:		return (void*)&Param6;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 7 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class Params<void, T0, T1, T2, T3, T4, T5, T6> : public DynParams {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T0>::_StorageType _ST0;
		typedef typename Type<T1>::_StorageType _ST1;
		typedef typename Type<T2>::_StorageType _ST2;
		typedef typename Type<T3>::_StorageType _ST3;
		typedef typename Type<T4>::_StorageType _ST4;
		typedef typename Type<T5>::_StorageType _ST5;
		typedef typename Type<T6>::_StorageType _ST6;

	public:
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;
		_ST4  Param4;
		_ST5  Param5;
		_ST6  Param6;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()					? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()					? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()					? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()				? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()				? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			_T6  t6  = (cParser.Next()				? Type<T6> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T6> ::Default());
			return Params(t0, t1, t2, t3, t4, t5, t6);
		}

	public:
		Params(const Params &cParams) :
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3),
			Param4(cParams.Param4),
			Param5(cParams.Param5),
			Param6(cParams.Param6)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6) :
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3)),
			Param4(Type<T4>  ::ConvertRealToStorage(t4)),
			Param5(Type<T5>  ::ConvertRealToStorage(t5)),
			Param6(Type<T6>  ::ConvertRealToStorage(t6))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Param0 = cParams.Param0;
			Param1 = cParams.Param1;
			Param2 = cParams.Param2;
			Param3 = cParams.Param3;
			Param4 = cParams.Param4;
			Param5 = cParams.Param5;
			Param6 = cParams.Param6;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<void, T0, T1, T2, T3, T4, T5, T6>::GetSignatureID();
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 7;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				case 6:		return Type<T6> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				case 4:		return (void*)&Param4;
				case 5:		return (void*)&Param5;
				case 6:		return (void*)&Param6;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 6 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class Params<R, T0, T1, T2, T3, T4, T5> : public DynParams {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<R> ::_StorageType _SR;
		typedef typename Type<T0>::_StorageType _ST0;
		typedef typename Type<T1>::_StorageType _ST1;
		typedef typename Type<T2>::_StorageType _ST2;
		typedef typename Type<T3>::_StorageType _ST3;
		typedef typename Type<T4>::_StorageType _ST4;
		typedef typename Type<T5>::_StorageType _ST5;

	public:
		_SR   Return;
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;
		_ST4  Param4;
		_ST5  Param5;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()					? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()					? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			return Params(t0, t1, t2, t3, t4, t5);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()				? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()				? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			return Params(t0, t1, t2, t3, t4, t5);
		}

	public:
		Params(const Params &cParams) :
			Return(DefaultValue<_SR>::Default()),
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3),
			Param4(cParams.Param4),
			Param5(cParams.Param5)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5) :
			Return(DefaultValue<_SR>::Default()),
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3)),
			Param4(Type<T4>  ::ConvertRealToStorage(t4)),
			Param5(Type<T5>  ::ConvertRealToStorage(t5))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Return = cParams.Return;
			Param0 = cParams.Param0;
			Param1 = cParams.Param1;
			Param2 = cParams.Param2;
			Param3 = cParams.Param3;
			Param4 = cParams.Param4;
			Param5 = cParams.Param5;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual void *GetPointerToReturnValue() override
		{
			return (void*)&Return;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 6;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				case 4:		return (void*)&Param4;
				case 5:		return (void*)&Param5;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 6 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class Params<void, T0, T1, T2, T3, T4, T5> : public DynParams {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T0>::_StorageType _ST0;
		typedef typename Type<T1>::_StorageType _ST1;
		typedef typename Type<T2>::_StorageType _ST2;
		typedef typename Type<T3>::_StorageType _ST3;
		typedef typename Type<T4>::_StorageType _ST4;
		typedef typename Type<T5>::_StorageType _ST5;

	public:
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;
		_ST4  Param4;
		_ST5  Param5;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()					? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()					? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			return Params(t0, t1, t2, t3, t4, t5);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()				? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			_T5  t5  = (cParser.Next()				? Type<T5> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T5> ::Default());
			return Params(t0, t1, t2, t3, t4, t5);
		}

	public:
		Params(const Params &cParams) :
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3),
			Param4(cParams.Param4),
			Param5(cParams.Param5)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5) :
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3)),
			Param4(Type<T4>  ::ConvertRealToStorage(t4)),
			Param5(Type<T5>  ::ConvertRealToStorage(t5))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Param0 = cParams.Param0;
			Param1 = cParams.Param1;
			Param2 = cParams.Param2;
			Param3 = cParams.Param3;
			Param4 = cParams.Param4;
			Param5 = cParams.Param5;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<void, T0, T1, T2, T3, T4, T5>::GetSignatureID();
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 6;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				case 5:		return Type<T5> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				case 4:		return (void*)&Param4;
				case 5:		return (void*)&Param5;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 5 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4>
class Params<R, T0, T1, T2, T3, T4> : public DynParams {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<R> ::_StorageType _SR;
		typedef typename Type<T0>::_StorageType _ST0;
		typedef typename Type<T1>::_StorageType _ST1;
		typedef typename Type<T2>::_StorageType _ST2;
		typedef typename Type<T3>::_StorageType _ST3;
		typedef typename Type<T4>::_StorageType _ST4;

	public:
		_SR   Return;
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;
		_ST4  Param4;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()					? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			return Params(t0, t1, t2, t3, t4);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()				? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			return Params(t0, t1, t2, t3, t4);
		}

	public:
		Params(const Params &cParams) :
			Return(DefaultValue<_SR>::Default()),
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3),
			Param4(cParams.Param4)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4) :
			Return(DefaultValue<_SR>::Default()),
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3)),
			Param4(Type<T4>  ::ConvertRealToStorage(t4))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Return = cParams.Return;
			Param0 = cParams.Param0;
			Param1 = cParams.Param1;
			Param2 = cParams.Param2;
			Param3 = cParams.Param3;
			Param4 = cParams.Param4;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual void *GetPointerToReturnValue() override
		{
			return (void*)&Return;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 5;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				case 4:		return (void*)&Param4;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 5 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4>
class Params<void, T0, T1, T2, T3, T4> : public DynParams {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T0>::_StorageType _ST0;
		typedef typename Type<T1>::_StorageType _ST1;
		typedef typename Type<T2>::_StorageType _ST2;
		typedef typename Type<T3>::_StorageType _ST3;
		typedef typename Type<T4>::_StorageType _ST4;

	public:
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;
		_ST4  Param4;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()					? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			return Params(t0, t1, t2, t3, t4);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			_T4  t4  = (cParser.Next()				? Type<T4> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T4> ::Default());
			return Params(t0, t1, t2, t3, t4);
		}

	public:
		Params(const Params &cParams) :
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3),
			Param4(cParams.Param4)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4) :
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3)),
			Param4(Type<T4>  ::ConvertRealToStorage(t4))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Param0 = cParams.Param0;
			Param1 = cParams.Param1;
			Param2 = cParams.Param2;
			Param3 = cParams.Param3;
			Param4 = cParams.Param4;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<void, T0, T1, T2, T3, T4>::GetSignatureID();
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 5;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				case 4:		return Type<T4> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				case 4:		return (void*)&Param4;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 4 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3>
class Params<R, T0, T1, T2, T3> : public DynParams {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<R> ::_StorageType _SR;
		typedef typename Type<T0>::_StorageType _ST0;
		typedef typename Type<T1>::_StorageType _ST1;
		typedef typename Type<T2>::_StorageType _ST2;
		typedef typename Type<T3>::_StorageType _ST3;

	public:
		_SR   Return;
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			return Params(t0, t1, t2, t3);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			return Params(t0, t1, t2, t3);
		}

	public:
		Params(const Params &cParams) :
			Return(DefaultValue<_SR>::Default()),
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3) :
			Return(DefaultValue<_SR>::Default()),
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Return = cParams.Return;
			Param0 = cParams.Param0;
			Param1 = cParams.Param1;
			Param2 = cParams.Param2;
			Param3 = cParams.Param3;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual void *GetPointerToReturnValue() override
		{
			return (void*)&Return;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 4;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 4 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3>
class Params<void, T0, T1, T2, T3> : public DynParams {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T0>::_StorageType _ST0;
		typedef typename Type<T1>::_StorageType _ST1;
		typedef typename Type<T2>::_StorageType _ST2;
		typedef typename Type<T3>::_StorageType _ST3;

	public:
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;
		_ST3  Param3;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()					? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			return Params(t0, t1, t2, t3);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			_T3  t3  = (cParser.Next()				? Type<T3> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T3> ::Default());
			return Params(t0, t1, t2, t3);
		}

	public:
		Params(const Params &cParams) :
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2),
			Param3(cParams.Param3)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2, _T3 t3) :
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2)),
			Param3(Type<T3>  ::ConvertRealToStorage(t3))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Param0 = cParams.Param0;
			Param1 = cParams.Param1;
			Param2 = cParams.Param2;
			Param3 = cParams.Param3;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<void, T0, T1, T2, T3>::GetSignatureID();
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 4;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				case 3:		return Type<T3> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				case 3:		return (void*)&Param3;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 3 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2>
class Params<R, T0, T1, T2> : public DynParams {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<R> ::_StorageType _SR;
		typedef typename Type<T0>::_StorageType _ST0;
		typedef typename Type<T1>::_StorageType _ST1;
		typedef typename Type<T2>::_StorageType _ST2;

	public:
		_SR   Return;
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			return Params(t0, t1, t2);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			return Params(t0, t1, t2);
		}

	public:
		Params(const Params &cParams) :
			Return(DefaultValue<_SR>::Default()),
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2) :
			Return(DefaultValue<_SR>::Default()),
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Return = cParams.Return;
			Param0 = cParams.Param0;
			Param1 = cParams.Param1;
			Param2 = cParams.Param2;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual void *GetPointerToReturnValue() override
		{
			return (void*)&Return;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 3;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 3 parameters without a return value
*/
template <typename T0, typename T1, typename T2>
class Params<void, T0, T1, T2> : public DynParams {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T0>::_StorageType _ST0;
		typedef typename Type<T1>::_StorageType _ST1;
		typedef typename Type<T2>::_StorageType _ST2;

	public:
		_ST0  Param0;
		_ST1  Param1;
		_ST2  Param2;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()					? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			return Params(t0, t1, t2);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			_T2  t2  = (cParser.Next()				? Type<T2> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T2> ::Default());
			return Params(t0, t1, t2);
		}

	public:
		Params(const Params &cParams) :
			Param0(cParams.Param0),
			Param1(cParams.Param1),
			Param2(cParams.Param2)
		{
		}

		Params(_T0 t0, _T1 t1, _T2 t2) :
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1)),
			Param2(Type<T2>  ::ConvertRealToStorage(t2))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Param0 = cParams.Param0;
			Param1 = cParams.Param1;
			Param2 = cParams.Param2;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<void, T0, T1, T2>::GetSignatureID();
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 3;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				case 2:		return Type<T2> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				case 2:		return (void*)&Param2;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 2 parameters and a return value
*/
template <typename R, typename T0, typename T1>
class Params<R, T0, T1> : public DynParams {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<R> ::_StorageType _SR;
		typedef typename Type<T0>::_StorageType _ST0;
		typedef typename Type<T1>::_StorageType _ST1;

	public:
		_SR   Return;
		_ST0  Param0;
		_ST1  Param1;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			return Params(t0, t1);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			return Params(t0, t1);
		}

	public:
		Params(const Params &cParams) :
			Return(DefaultValue<_SR>::Default()),
			Param0(cParams.Param0),
			Param1(cParams.Param1)
		{
		}

		Params(_T0 t0, _T1 t1) :
			Return(DefaultValue<_SR>::Default()),
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Return = cParams.Return;
			Param0 = cParams.Param0;
			Param1 = cParams.Param1;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual void *GetPointerToReturnValue() override
		{
			return (void*)&Return;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 2;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 2 parameters without a return value
*/
template <typename T0, typename T1>
class Params<void, T0, T1> : public DynParams {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T0>::_StorageType _ST0;
		typedef typename Type<T1>::_StorageType _ST1;

	public:
		_ST0  Param0;
		_ST1  Param1;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()					? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			return Params(t0, t1);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			_T1  t1  = (cParser.Next()				? Type<T1> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T1> ::Default());
			return Params(t0, t1);
		}

	public:
		Params(const Params &cParams) :
			Param0(cParams.Param0),
			Param1(cParams.Param1)
		{
		}

		Params(_T0 t0, _T1 t1) :
			Param0(Type<T0>  ::ConvertRealToStorage(t0)),
			Param1(Type<T1>  ::ConvertRealToStorage(t1))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Param0 = cParams.Param0;
			Param1 = cParams.Param1;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<void, T0, T1>::GetSignatureID();
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 2;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				case 1:		return Type<T1> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				case 1:		return (void*)&Param1;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 1 parameters and a return value
*/
template <typename R, typename T0>
class Params<R, T0> : public DynParams {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<R> ::_StorageType _SR;
		typedef typename Type<T0>::_StorageType _ST0;

	public:
		_SR   Return;
		_ST0  Param0;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			return Params(t0);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			return Params(t0);
		}

	public:
		Params(const Params &cParams) :
			Return(DefaultValue<_SR>::Default()),
			Param0(cParams.Param0)
		{
		}

		Params(_T0 t0) :
			Return(DefaultValue<_SR>::Default()),
			Param0(Type<T0>  ::ConvertRealToStorage(t0))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Return = cParams.Return;
			Param0 = cParams.Param0;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual void *GetPointerToReturnValue() override
		{
			return (void*)&Return;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 1;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 1 parameters without a return value
*/
template <typename T0>
class Params<void, T0> : public DynParams {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T0>::_StorageType _ST0;

	public:
		_ST0  Param0;

	public:
		static Params FromString(const String &sParams) {
			ParamsParser cParser;
			_T0  t0  = (cParser.ParseString(sParams)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			return Params(t0);
		}

		static Params FromXml(const XmlElement &cElement) {
			ParamsParserXml cParser;
			_T0  t0  = (cParser.ParseXml(cElement)	? Type<T0> ::ConvertFromString(cParser.GetValue()) : DefaultValue<T0> ::Default());
			return Params(t0);
		}

	public:
		Params(const Params &cParams) :
			Param0(cParams.Param0)
		{
		}

		Params(_T0 t0) :
			Param0(Type<T0>  ::ConvertRealToStorage(t0))
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Param0 = cParams.Param0;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<void, T0>::GetSignatureID();
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 1;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0> ::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void *GetPointerToParameterValue(uint32 nIndex) override
		{
			switch (nIndex) {
				case 0:		return (void*)&Param0;
				default:	return nullptr;
			}
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 0 parameters and a return value
*/
template <typename R>
class Params<R> : public DynParams {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<R>::_StorageType _SR;

	public:
		_SR	Return;

	public:
		static Params FromString(const String &sParams) {
			return Params();
		}

		static Params FromXml(const XmlElement &cElement) {
			return Params();
		}

	public:
		Params(const Params &cParams) :
			Return(DefaultValue<_SR>::Default())
		{
		}

		Params() :
			Return(DefaultValue<_SR>::Default())
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			Return = cParams.Return;
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<R>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual void *GetPointerToReturnValue() override
		{
			return (void*)&Return;
		}
};

/**
*  @brief
*    Parameters
*
*  @remarks
*    Implementation for 0 parameters without a return value
*/
template <>
class Params<void> : public DynParams {
	public:
		static Params FromString(const String &sParams) {
			return Params();
		}

		static Params FromXml(const XmlElement &cElement) {
			return Params();
		}

	public:
		Params(const Params &cParams)
		{
		}

		Params()
		{
		}

		virtual ~Params()
		{
		}

		Params &operator =(const Params &cParams)
		{
			return *this;
		}

		virtual String GetSignature() const override
		{
			return Signature<void>::GetSignatureID();
		}
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_PARAMS_H__
