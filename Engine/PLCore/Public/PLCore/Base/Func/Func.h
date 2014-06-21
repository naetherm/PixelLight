/*********************************************************\
 *  File: Func.h                                         *
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


#ifndef __PLCORE_FUNC_H__
#define __PLCORE_FUNC_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Base/Type/DefaultValue.h"
#include "PLCore/Base/Func/Signature.h"
#include "PLCore/Base/Func/Params.h"
#include "PLCore/Base/Func/DynFunc.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 16 parameters and a return value
*/
template <typename R, typename T0 = NullType, typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType, typename T6 = NullType, typename T7 = NullType, typename T8 = NullType, typename T9 = NullType, typename T10 = NullType, typename T11 = NullType, typename T12 = NullType, typename T13 = NullType, typename T14 = NullType, typename T15 = NullType>
class Func : public DynFunc {
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
		typedef typename Type<T15>::_Type _T15;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14, _T15 t15)
		{
			return DefaultValue<R>::Default();
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
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

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> &cP =
					static_cast<Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>&>(cParams);

				// Call function
				cP.Return = Type<R>::ConvertRealToStorage((*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
																  Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
																  Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
																  Type<T12>::ConvertStorageToReal(cP.Param12), Type<T13>::ConvertStorageToReal(cP.Param13), Type<T14>::ConvertStorageToReal(cP.Param14), Type<T15>::ConvertStorageToReal(cP.Param15)));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> &cP =
					static_cast<const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
						Type<T12>::ConvertStorageToReal(cP.Param12), Type<T13>::ConvertStorageToReal(cP.Param13), Type<T14>::ConvertStorageToReal(cP.Param14), Type<T15>::ConvertStorageToReal(cP.Param15));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::FromString(sParams);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::FromXml(cElement);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for up to 16 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> : public DynFunc {
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

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14, _T15 t15)
		{
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

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> &cP =
					static_cast<Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
						Type<T12>::ConvertStorageToReal(cP.Param12), Type<T13>::ConvertStorageToReal(cP.Param13), Type<T14>::ConvertStorageToReal(cP.Param14), Type<T15>::ConvertStorageToReal(cP.Param15));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> &cP =
					static_cast<const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
						Type<T12>::ConvertStorageToReal(cP.Param12), Type<T13>::ConvertStorageToReal(cP.Param13), Type<T14>::ConvertStorageToReal(cP.Param14), Type<T15>::ConvertStorageToReal(cP.Param15));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::FromString(sParams);
			Call(cParams);
			return "";
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::FromXml(cElement);
			Call(cParams);
			return "";
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 15 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public DynFunc {
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

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14)
		{
			return DefaultValue<R>::Default();
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
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

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> &cP =
					static_cast<Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>&>(cParams);

				// Call function
				cP.Return = Type<R>::ConvertRealToStorage((*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
																  Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
																  Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
																  Type<T12>::ConvertStorageToReal(cP.Param12), Type<T13>::ConvertStorageToReal(cP.Param13), Type<T14>::ConvertStorageToReal(cP.Param14)));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> &cP =
					static_cast<const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
						Type<T12>::ConvertStorageToReal(cP.Param12), Type<T13>::ConvertStorageToReal(cP.Param13), Type<T14>::ConvertStorageToReal(cP.Param14));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::FromString(sParams);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::FromXml(cElement);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 15 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public DynFunc {
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

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14)
		{
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

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> &cP =
					static_cast<Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
						Type<T12>::ConvertStorageToReal(cP.Param12), Type<T13>::ConvertStorageToReal(cP.Param13), Type<T14>::ConvertStorageToReal(cP.Param14));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> &cP =
					static_cast<const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
						Type<T12>::ConvertStorageToReal(cP.Param12), Type<T13>::ConvertStorageToReal(cP.Param13), Type<T14>::ConvertStorageToReal(cP.Param14));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::FromString(sParams);
			Call(cParams);
			return "";
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::FromXml(cElement);
			Call(cParams);
			return "";
		}
};


/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 14 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public DynFunc {
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

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13)
		{
			return DefaultValue<R>::Default();
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
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

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> &cP =
					static_cast<Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>&>(cParams);

				// Call function
				cP.Return = Type<R>::ConvertRealToStorage((*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
																  Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
																  Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
																  Type<T12>::ConvertStorageToReal(cP.Param12), Type<T13>::ConvertStorageToReal(cP.Param13)));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> &cP =
					static_cast<const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
						Type<T12>::ConvertStorageToReal(cP.Param12), Type<T13>::ConvertStorageToReal(cP.Param13));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::FromString(sParams);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::FromXml(cElement);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 14 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public DynFunc {
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

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13)
		{
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

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> &cP =
					static_cast<Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
						Type<T12>::ConvertStorageToReal(cP.Param12), Type<T13>::ConvertStorageToReal(cP.Param13));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> &cP =
					static_cast<const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
						Type<T12>::ConvertStorageToReal(cP.Param12), Type<T13>::ConvertStorageToReal(cP.Param13));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::FromString(sParams);
			Call(cParams);
			return "";
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::FromXml(cElement);
			Call(cParams);
			return "";
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 13 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public DynFunc {
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

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12)
		{
			return DefaultValue<R>::Default();
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
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

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> &cP =
					static_cast<Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>&>(cParams);

				// Call function
				cP.Return = Type<R>::ConvertRealToStorage((*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
																  Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
																  Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
																  Type<T12>::ConvertStorageToReal(cP.Param12)));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> &cP =
					static_cast<const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
						Type<T12>::ConvertStorageToReal(cP.Param12));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::FromString(sParams);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::FromXml(cElement);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 13 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public DynFunc {
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

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12)
		{
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

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> &cP =
					static_cast<Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
						Type<T12>::ConvertStorageToReal(cP.Param12));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> &cP =
					static_cast<const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
						Type<T12>::ConvertStorageToReal(cP.Param12));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::FromString(sParams);
			Call(cParams);
			return "";
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::FromXml(cElement);
			Call(cParams);
			return "";
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 12 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public DynFunc {
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

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11)
		{
			return DefaultValue<R>::Default();
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
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

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> &cP =
					static_cast<Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>&>(cParams);

				// Call function
				cP.Return = Type<R>::ConvertRealToStorage((*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
																  Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
																  Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11)));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> &cP =
					static_cast<const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::FromString(sParams);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::FromXml(cElement);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 12 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public DynFunc {
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

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11)
		{
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

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> &cP =
					static_cast<Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> &cP =
					static_cast<const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::FromString(sParams);
			Call(cParams);
			return "";
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::FromXml(cElement);
			Call(cParams);
			return "";
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 11 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public DynFunc {
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

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10)
		{
			return DefaultValue<R>::Default();
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
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

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> &cP =
					static_cast<Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>&>(cParams);

				// Call function
				cP.Return = Type<R>::ConvertRealToStorage((*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
																  Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
																  Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10)));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> &cP =
					static_cast<const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::FromString(sParams);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::FromXml(cElement);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 11 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public DynFunc {
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

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10)
		{
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

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> &cP =
					static_cast<Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> &cP =
					static_cast<const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::FromString(sParams);
			Call(cParams);
			return "";
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::FromXml(cElement);
			Call(cParams);
			return "";
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 10 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public DynFunc {
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

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9)
		{
			return DefaultValue<R>::Default();
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 10;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0>::TypeID;
				case 1:		return Type<T1>::TypeID;
				case 2:		return Type<T2>::TypeID;
				case 3:		return Type<T3>::TypeID;
				case 4:		return Type<T4>::TypeID;
				case 5:		return Type<T5>::TypeID;
				case 6:		return Type<T6>::TypeID;
				case 7:		return Type<T7>::TypeID;
				case 8:		return Type<T8>::TypeID;
				case 9:		return Type<T9>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> &cP =
					static_cast<Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>&>(cParams);

				// Call function
				cP.Return = Type<R>::ConvertRealToStorage((*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
																  Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
																  Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9)));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> &cP =
					static_cast<const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::FromString(sParams);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::FromXml(cElement);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 10 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public DynFunc {
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

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9)
		{
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
				case 0:		return Type<T0>::TypeID;
				case 1:		return Type<T1>::TypeID;
				case 2:		return Type<T2>::TypeID;
				case 3:		return Type<T3>::TypeID;
				case 4:		return Type<T4>::TypeID;
				case 5:		return Type<T5>::TypeID;
				case 6:		return Type<T6>::TypeID;
				case 7:		return Type<T7>::TypeID;
				case 8:		return Type<T8>::TypeID;
				case 9:		return Type<T9>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> &cP =
					static_cast<Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> &cP =
					static_cast<const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::FromString(sParams);
			Call(cParams);
			return "";
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::FromXml(cElement);
			Call(cParams);
			return "";
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 9 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> : public DynFunc {
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

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8)
		{
			return DefaultValue<R>::Default();
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 9;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0>::TypeID;
				case 1:		return Type<T1>::TypeID;
				case 2:		return Type<T2>::TypeID;
				case 3:		return Type<T3>::TypeID;
				case 4:		return Type<T4>::TypeID;
				case 5:		return Type<T5>::TypeID;
				case 6:		return Type<T6>::TypeID;
				case 7:		return Type<T7>::TypeID;
				case 8:		return Type<T8>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> &cP =
					static_cast<Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8>&>(cParams);

				// Call function
				cP.Return = Type<R>::ConvertRealToStorage((*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
																  Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
																  Type<T8> ::ConvertStorageToReal(cP.Param8)));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> &cP =
					static_cast<const Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8>::FromString(sParams);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7, T8>::FromXml(cElement);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 9 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> : public DynFunc {
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

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8)
		{
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
				case 0:		return Type<T0>::TypeID;
				case 1:		return Type<T1>::TypeID;
				case 2:		return Type<T2>::TypeID;
				case 3:		return Type<T3>::TypeID;
				case 4:		return Type<T4>::TypeID;
				case 5:		return Type<T5>::TypeID;
				case 6:		return Type<T6>::TypeID;
				case 7:		return Type<T7>::TypeID;
				case 8:		return Type<T8>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> &cP =
					static_cast<Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> &cP =
					static_cast<const Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>::FromString(sParams);
			Call(cParams);
			return "";
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>::FromXml(cElement);
			Call(cParams);
			return "";
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 8 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class Func<R, T0, T1, T2, T3, T4, T5, T6, T7> : public DynFunc {
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

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7)
		{
			return DefaultValue<R>::Default();
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6, T7>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 8;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0>::TypeID;
				case 1:		return Type<T1>::TypeID;
				case 2:		return Type<T2>::TypeID;
				case 3:		return Type<T3>::TypeID;
				case 4:		return Type<T4>::TypeID;
				case 5:		return Type<T5>::TypeID;
				case 6:		return Type<T6>::TypeID;
				case 7:		return Type<T7>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6, T7> &cP =
					static_cast<Params<R, T0, T1, T2, T3, T4, T5, T6, T7>&>(cParams);

				// Call function
				cP.Return = Type<R>::ConvertRealToStorage((*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
																  Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7)));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3, T4, T5, T6, T7> &cP =
					static_cast<const Params<R, T0, T1, T2, T3, T4, T5, T6, T7>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7>::FromString(sParams);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6, T7> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6, T7>::FromXml(cElement);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 8 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class Func<void, T0, T1, T2, T3, T4, T5, T6, T7> : public DynFunc {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7)
		{
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
				case 0:		return Type<T0>::TypeID;
				case 1:		return Type<T1>::TypeID;
				case 2:		return Type<T2>::TypeID;
				case 3:		return Type<T3>::TypeID;
				case 4:		return Type<T4>::TypeID;
				case 5:		return Type<T5>::TypeID;
				case 6:		return Type<T6>::TypeID;
				case 7:		return Type<T7>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6, T7> &cP =
					static_cast<Params<void, T0, T1, T2, T3, T4, T5, T6, T7>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3, T4, T5, T6, T7> &cP =
					static_cast<const Params<void, T0, T1, T2, T3, T4, T5, T6, T7>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7>::FromString(sParams);
			Call(cParams);
			return "";
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7>::FromXml(cElement);
			Call(cParams);
			return "";
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 7 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class Func<R, T0, T1, T2, T3, T4, T5, T6> : public DynFunc {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6)
		{
			return DefaultValue<R>::Default();
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5, T6>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 7;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0>::TypeID;
				case 1:		return Type<T1>::TypeID;
				case 2:		return Type<T2>::TypeID;
				case 3:		return Type<T3>::TypeID;
				case 4:		return Type<T4>::TypeID;
				case 5:		return Type<T5>::TypeID;
				case 6:		return Type<T6>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5, T6> &cP =
					static_cast<Params<R, T0, T1, T2, T3, T4, T5, T6>&>(cParams);

				// Call function
				cP.Return = Type<R>::ConvertRealToStorage((*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
																  Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6)));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3, T4, T5, T6> &cP =
					static_cast<const Params<R, T0, T1, T2, T3, T4, T5, T6>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6>::FromString(sParams);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3, T4, T5, T6> cParams = Params<R, T0, T1, T2, T3, T4, T5, T6>::FromXml(cElement);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 7 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class Func<void, T0, T1, T2, T3, T4, T5, T6> : public DynFunc {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6)
		{
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
				case 0:		return Type<T0>::TypeID;
				case 1:		return Type<T1>::TypeID;
				case 2:		return Type<T2>::TypeID;
				case 3:		return Type<T3>::TypeID;
				case 4:		return Type<T4>::TypeID;
				case 5:		return Type<T5>::TypeID;
				case 6:		return Type<T6>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5, T6> &cP =
					static_cast<Params<void, T0, T1, T2, T3, T4, T5, T6>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3, T4, T5, T6> &cP =
					static_cast<const Params<void, T0, T1, T2, T3, T4, T5, T6>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6>::FromString(sParams);
			Call(cParams);
			return "";
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6>::FromXml(cElement);
			Call(cParams);
			return "";
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 6 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class Func<R, T0, T1, T2, T3, T4, T5> : public DynFunc {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5)
		{
			return DefaultValue<R>::Default();
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4, T5>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 6;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0>::TypeID;
				case 1:		return Type<T1>::TypeID;
				case 2:		return Type<T2>::TypeID;
				case 3:		return Type<T3>::TypeID;
				case 4:		return Type<T4>::TypeID;
				case 5:		return Type<T5>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4, T5> &cP =
					static_cast<Params<R, T0, T1, T2, T3, T4, T5>&>(cParams);

				// Call function
				cP.Return = Type<R>::ConvertRealToStorage((*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
																  Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5)));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3, T4, T5> &cP =
					static_cast<const Params<R, T0, T1, T2, T3, T4, T5>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5> cParams = Params<R, T0, T1, T2, T3, T4, T5>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3, T4, T5> cParams = Params<R, T0, T1, T2, T3, T4, T5>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4, T5> cParams = Params<R, T0, T1, T2, T3, T4, T5>::FromString(sParams);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3, T4, T5> cParams = Params<R, T0, T1, T2, T3, T4, T5>::FromXml(cElement);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 6 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class Func<void, T0, T1, T2, T3, T4, T5> : public DynFunc {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5)
		{
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
				case 0:		return Type<T0>::TypeID;
				case 1:		return Type<T1>::TypeID;
				case 2:		return Type<T2>::TypeID;
				case 3:		return Type<T3>::TypeID;
				case 4:		return Type<T4>::TypeID;
				case 5:		return Type<T5>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4, T5> &cP =
					static_cast<Params<void, T0, T1, T2, T3, T4, T5>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3, T4, T5> &cP =
					static_cast<const Params<void, T0, T1, T2, T3, T4, T5>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5> cParams = Params<void, T0, T1, T2, T3, T4, T5>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3, T4, T5> cParams = Params<void, T0, T1, T2, T3, T4, T5>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4, T5> cParams = Params<void, T0, T1, T2, T3, T4, T5>::FromString(sParams);
			Call(cParams);
			return "";
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3, T4, T5> cParams = Params<void, T0, T1, T2, T3, T4, T5>::FromXml(cElement);
			Call(cParams);
			return "";
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 5 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4>
class Func<R, T0, T1, T2, T3, T4> : public DynFunc {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4)
		{
			return DefaultValue<R>::Default();
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3, T4>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 5;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0>::TypeID;
				case 1:		return Type<T1>::TypeID;
				case 2:		return Type<T2>::TypeID;
				case 3:		return Type<T3>::TypeID;
				case 4:		return Type<T4>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3, T4> &cP = static_cast<Params<R, T0, T1, T2, T3, T4>&>(cParams);

				// Call function
				cP.Return = Type<R>::ConvertRealToStorage((*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
																  Type<T4> ::ConvertStorageToReal(cP.Param4)));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3, T4> &cP = static_cast<const Params<R, T0, T1, T2, T3, T4>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4> cParams = Params<R, T0, T1, T2, T3, T4>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3, T4> cParams = Params<R, T0, T1, T2, T3, T4>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3, T4> cParams = Params<R, T0, T1, T2, T3, T4>::FromString(sParams);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3, T4> cParams = Params<R, T0, T1, T2, T3, T4>::FromXml(cElement);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 5 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4>
class Func<void, T0, T1, T2, T3, T4> : public DynFunc {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4)
		{
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
				case 0:		return Type<T0>::TypeID;
				case 1:		return Type<T1>::TypeID;
				case 2:		return Type<T2>::TypeID;
				case 3:		return Type<T3>::TypeID;
				case 4:		return Type<T4>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3, T4> &cP = static_cast<Params<void, T0, T1, T2, T3, T4>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3, T4> &cP = static_cast<const Params<void, T0, T1, T2, T3, T4>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4> cParams = Params<void, T0, T1, T2, T3, T4>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3, T4> cParams = Params<void, T0, T1, T2, T3, T4>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3, T4> cParams = Params<void, T0, T1, T2, T3, T4>::FromString(sParams);
			Call(cParams);
			return "";
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3, T4> cParams = Params<void, T0, T1, T2, T3, T4>::FromXml(cElement);
			Call(cParams);
			return "";
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 4 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3>
class Func<R, T0, T1, T2, T3> : public DynFunc {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3)
		{
			return DefaultValue<R>::Default();
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2, T3>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 4;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0>::TypeID;
				case 1:		return Type<T1>::TypeID;
				case 2:		return Type<T2>::TypeID;
				case 3:		return Type<T3>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<R, T0, T1, T2, T3> &cP = static_cast<Params<R, T0, T1, T2, T3>&>(cParams);

				// Call function
				cP.Return = Type<R>::ConvertRealToStorage((*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3)));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2, T3> &cP = static_cast<const Params<R, T0, T1, T2, T3>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3> cParams = Params<R, T0, T1, T2, T3>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3> cParams = Params<R, T0, T1, T2, T3>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2, T3> cParams = Params<R, T0, T1, T2, T3>::FromString(sParams);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2, T3> cParams = Params<R, T0, T1, T2, T3>::FromXml(cElement);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 4 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3>
class Func<void, T0, T1, T2, T3> : public DynFunc {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3)
		{
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
				case 0:		return Type<T0>::TypeID;
				case 1:		return Type<T1>::TypeID;
				case 2:		return Type<T2>::TypeID;
				case 3:		return Type<T3>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<void, T0, T1, T2, T3> &cP = static_cast<Params<void, T0, T1, T2, T3>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2, T3> &cP = static_cast<const Params<void, T0, T1, T2, T3>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3> cParams = Params<void, T0, T1, T2, T3>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3> cParams = Params<void, T0, T1, T2, T3>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2, T3> cParams = Params<void, T0, T1, T2, T3>::FromString(sParams);
			Call(cParams);
			return "";
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2, T3> cParams = Params<void, T0, T1, T2, T3>::FromXml(cElement);
			Call(cParams);
			return "";
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 3 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2>
class Func<R, T0, T1, T2> : public DynFunc {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2)
		{
			return DefaultValue<R>::Default();
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1, T2>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 3;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0>::TypeID;
				case 1:		return Type<T1>::TypeID;
				case 2:		return Type<T2>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<R, T0, T1, T2> &cP = static_cast<Params<R, T0, T1, T2>&>(cParams);

				// Call function
				cP.Return = Type<R>::ConvertRealToStorage((*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2)));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<R, T0, T1, T2> &cP = static_cast<const Params<R, T0, T1, T2>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<R, T0, T1, T2> cParams = Params<R, T0, T1, T2>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<R, T0, T1, T2> cParams = Params<R, T0, T1, T2>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<R, T0, T1, T2> cParams = Params<R, T0, T1, T2>::FromString(sParams);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}

		virtual String CallWithReturn(const XmlElement &cElement)
		{
			Params<R, T0, T1, T2> cParams = Params<R, T0, T1, T2>::FromXml(cElement);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 3 parameters without a return value
*/
template <typename T0, typename T1, typename T2>
class Func<void, T0, T1, T2> : public DynFunc {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2)
		{
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
				case 0:		return Type<T0>::TypeID;
				case 1:		return Type<T1>::TypeID;
				case 2:		return Type<T2>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<void, T0, T1, T2> &cP = static_cast<Params<void, T0, T1, T2>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<void, T0, T1, T2> &cP = static_cast<const Params<void, T0, T1, T2>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<void, T0, T1, T2> cParams = Params<void, T0, T1, T2>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2> cParams = Params<void, T0, T1, T2>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<void, T0, T1, T2> cParams = Params<void, T0, T1, T2>::FromString(sParams);
			Call(cParams);
			return "";
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<void, T0, T1, T2> cParams = Params<void, T0, T1, T2>::FromXml(cElement);
			Call(cParams);
			return "";
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 2 parameters and a return value
*/
template <typename R, typename T0, typename T1>
class Func<R, T0, T1> : public DynFunc {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual _R operator ()(_T0 t0, _T1 t1)
		{
			return DefaultValue<R>::Default();
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0, T1>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 2;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0>::TypeID;
				case 1:		return Type<T1>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<R, T0, T1> &cP = static_cast<Params<R, T0, T1>&>(cParams);

				// Call function
				cP.Return = Type<R>::ConvertRealToStorage((*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1)));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<R, T0, T1> &cP = static_cast<const Params<R, T0, T1>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<R, T0, T1> cParams = Params<R, T0, T1>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<R, T0, T1> cParams = Params<R, T0, T1>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<R, T0, T1> cParams = Params<R, T0, T1>::FromString(sParams);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}

		virtual String CallWithReturn(const XmlElement &cElement)
		{
			Params<R, T0, T1> cParams = Params<R, T0, T1>::FromXml(cElement);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 2 parameters without a return value
*/
template <typename T0, typename T1>
class Func<void, T0, T1> : public DynFunc {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1)
		{
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
				case 0:		return Type<T0>::TypeID;
				case 1:		return Type<T1>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<void, T0, T1> &cP = static_cast<Params<void, T0, T1>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<void, T0, T1> &cP = static_cast<const Params<void, T0, T1>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<void, T0, T1> cParams = Params<void, T0, T1>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<void, T0, T1> cParams = Params<void, T0, T1>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<void, T0, T1> cParams = Params<void, T0, T1>::FromString(sParams);
			Call(cParams);
			return "";
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<void, T0, T1> cParams = Params<void, T0, T1>::FromXml(cElement);
			Call(cParams);
			return "";
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 1 parameters and a return value
*/
template <typename R, typename T0>
class Func<R, T0> : public DynFunc {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual _R operator ()(_T0 t0)
		{
			return DefaultValue<R>::Default();
		}

		virtual String GetSignature() const override
		{
			return Signature<R, T0>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 1;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			switch (nIndex) {
				case 0:		return Type<T0>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<R, T0> &cP = static_cast<Params<R, T0>&>(cParams);

				// Call function
				cP.Return = Type<R>::ConvertRealToStorage((*this)(Type<T0> ::ConvertStorageToReal(cP.Param0)));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<R, T0> &cP = static_cast<const Params<R, T0>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<R, T0> cParams = Params<R, T0>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<R, T0> cParams = Params<R, T0>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<R, T0> cParams = Params<R, T0>::FromString(sParams);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<R, T0> cParams = Params<R, T0>::FromXml(cElement);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 1 parameters without a return value
*/
template <typename T0>
class Func<void, T0> : public DynFunc {
	public:
		typedef typename Type<T0>::_Type _T0;

	public:
		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()(_T0 t0)
		{
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
				case 0:		return Type<T0>::TypeID;
				default:	return TypeInvalid;
			}
		}

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<void, T0> &cP = static_cast<Params<void, T0>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0));
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const Params<void, T0> &cP = static_cast<const Params<void, T0>&>(cParams);

				// Call function
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0));
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<void, T0> cParams = Params<void, T0>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<void, T0> cParams = Params<void, T0>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<void, T0> cParams = Params<void, T0>::FromString(sParams);
			Call(cParams);
			return "";
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<void, T0> cParams = Params<void, T0>::FromXml(cElement);
			Call(cParams);
			return "";
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 0 parameters and a return value
*/
template <typename R>
class Func<R> : public DynFunc {
	public:
		typedef typename Type<R>  ::_Type _R;

		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual _R operator ()()
		{
			return DefaultValue<R>::Default();
		}

		virtual String GetSignature() const override
		{
			return Signature<R>::GetSignatureID();
		}

		virtual int GetReturnTypeID() const override
		{
			return Type<R>::TypeID;
		}

		virtual uint32 GetNumOfParameters() const override
		{
			return 0;
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			// There are no candidates, so the choice is pretty simple
			return TypeInvalid;
		}

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				Params<R> &cP = static_cast<Params<R>&>(cParams);

				// Call function
				cP.Return = Type<R>::ConvertRealToStorage((*this)());
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Call function
				(*this)();
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<R> cParams = Params<R>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<R> cParams = Params<R>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<R> cParams = Params<R>::FromString(sParams);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<R> cParams = Params<R>::FromXml(cElement);
			Call(cParams);
			return Type<R>::ConvertToString(Type<R>::ConvertStorageToReal(cParams.Return));
		}
};

/**
*  @brief
*    Function object (object that can be 'called' like a function / functoid)
*
*  @remarks
*    Implementation for 0 parameters without a return value
*/
template <>
class Func<void> : public DynFunc {
	public:
		Func()
		{
		}

		virtual ~Func()
		{
		}

		virtual void operator ()()
		{
		}

		virtual String GetSignature() const override
		{
			return Signature<void>::GetSignatureID();
		}

		virtual int GetParameterTypeID(uint32 nIndex) const override
		{
			// There are no candidates, so the choice is pretty simple
			return TypeInvalid;
		}

		virtual void Call(DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Call function
				(*this)();
			}
		}

		virtual void Call(const DynParams &cParams) override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Call function
				(*this)();
			}
		}

		virtual void Call(const String &sParams) override
		{
			Params<void> cParams = Params<void>::FromString(sParams);
			Call(cParams);
		}

		virtual void Call(const XmlElement &cElement) override
		{
			Params<void> cParams = Params<void>::FromXml(cElement);
			Call(cParams);
		}

		virtual String CallWithReturn(const String &sParams) override
		{
			Params<void> cParams = Params<void>::FromString(sParams);
			Call(cParams);
			return "";
		}

		virtual String CallWithReturn(const XmlElement &cElement) override
		{
			Params<void> cParams = Params<void>::FromXml(cElement);
			Call(cParams);
			return "";
		}
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_FUNC_H__
