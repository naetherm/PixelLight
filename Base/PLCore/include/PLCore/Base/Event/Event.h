/*********************************************************\
 *  File: Event.h                                        *
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


#ifndef __PLCORE_EVENT_H__
#define __PLCORE_EVENT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Base/Func/FuncGenFunPtr.h"
#include "PLCore/Base/Event/DynEvent.h"
#include "PLCore/Base/Event/EventHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for up to 16 parameters without a return value
*/
template <typename T0 = NullType, typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType, typename T6 = NullType, typename T7 = NullType, typename T8 = NullType, typename T9 = NullType, typename T10 = NullType, typename T11 = NullType, typename T12 = NullType, typename T13 = NullType, typename T14 = NullType, typename T15 = NullType>
class PLCORE_TMPL Event : public DynEvent {
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
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>		TypeHandler;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>		TypeParams;

		Event()
		{
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14, _T15 t15) const
		{
			// Iterate through all event handlers
			const typename SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual String GetSignature() const override
		{
			return TypeSignature::GetSignatureID();
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

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			return new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>(pFunc, pUserData));
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
						Type<T12>::ConvertStorageToReal(cP.Param12), Type<T13>::ConvertStorageToReal(cP.Param13), Type<T14>::ConvertStorageToReal(cP.Param14), Type<T15>::ConvertStorageToReal(cP.Param15));
			}
		}

		virtual void Emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
						Type<T12>::ConvertStorageToReal(cP.Param12), Type<T13>::ConvertStorageToReal(cP.Param13), Type<T14>::ConvertStorageToReal(cP.Param14), Type<T15>::ConvertStorageToReal(cP.Param15));
			}
		}

		virtual void Emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::FromString(sParams);
			Emit(cParams);
		}

		virtual void Emit(const XmlElement &cElement) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::FromXml(cElement);
			Emit(cParams);
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 15 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class PLCORE_TMPL Event<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public DynEvent {
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
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>		TypeHandler;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>		TypeParams;

		Event()
		{
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14) const
		{
			// Iterate through all event handlers
			const typename SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual String GetSignature() const override
		{
			return TypeSignature::GetSignatureID();
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

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			return new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>(pFunc, pUserData));
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
						Type<T12>::ConvertStorageToReal(cP.Param12), Type<T13>::ConvertStorageToReal(cP.Param13), Type<T14>::ConvertStorageToReal(cP.Param14));
			}
		}

		virtual void Emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
						Type<T12>::ConvertStorageToReal(cP.Param12), Type<T13>::ConvertStorageToReal(cP.Param13), Type<T14>::ConvertStorageToReal(cP.Param14));
			}
		}

		virtual void Emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::FromString(sParams);
			Emit(cParams);
		}

		virtual void Emit(const XmlElement &cElement) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::FromXml(cElement);
			Emit(cParams);
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 14 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class PLCORE_TMPL Event<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public DynEvent {
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
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>	TypeHandler;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>	TypeParams;

		Event()
		{
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13) const
		{
			// Iterate through all event handlers
			const typename SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual String GetSignature() const override
		{
			return TypeSignature::GetSignatureID();
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

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			return new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>(pFunc, pUserData));
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
						Type<T12>::ConvertStorageToReal(cP.Param12), Type<T13>::ConvertStorageToReal(cP.Param13));
			}
		}

		virtual void Emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
						Type<T12>::ConvertStorageToReal(cP.Param12), Type<T13>::ConvertStorageToReal(cP.Param13));
			}
		}

		virtual void Emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::FromString(sParams);
			Emit(cParams);
		}

		virtual void Emit(const XmlElement &cElement) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::FromXml(cElement);
			Emit(cParams);
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 13 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class PLCORE_TMPL Event<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public DynEvent {
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
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>		TypeHandler;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>		TypeParams;

		Event()
		{
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12) const
		{
			// Iterate through all event handlers
			const typename SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual String GetSignature() const override
		{
			return TypeSignature::GetSignatureID();
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

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			return new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>(pFunc, pUserData));
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
						Type<T12>::ConvertStorageToReal(cP.Param12));
			}
		}

		virtual void Emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
						Type<T12>::ConvertStorageToReal(cP.Param12));
			}
		}

		virtual void Emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::FromString(sParams);
			Emit(cParams);
		}

		virtual void Emit(const XmlElement &cElement) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::FromXml(cElement);
			Emit(cParams);
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 12 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class PLCORE_TMPL Event<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public DynEvent {
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
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>		TypeHandler;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>		TypeParams;

		Event()
		{
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11) const
		{
			// Iterate through all event handlers
			const typename SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual String GetSignature() const override
		{
			return TypeSignature::GetSignatureID();
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

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			return new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>(pFunc, pUserData));
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11));
			}
		}

		virtual void Emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11));
			}
		}

		virtual void Emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::FromString(sParams);
			Emit(cParams);
		}

		virtual void Emit(const XmlElement &cElement) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::FromXml(cElement);
			Emit(cParams);
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 11 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class PLCORE_TMPL Event<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public DynEvent {
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
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>		TypeHandler;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>		TypeParams;

		Event()
		{
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10) const
		{
			// Iterate through all event handlers
			const typename SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual String GetSignature() const override
		{
			return TypeSignature::GetSignatureID();
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

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			return new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>(pFunc, pUserData));
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10));
			}
		}

		virtual void Emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10));
			}
		}

		virtual void Emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::FromString(sParams);
			Emit(cParams);
		}

		virtual void Emit(const XmlElement &cElement) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::FromXml(cElement);
			Emit(cParams);
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 10 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class PLCORE_TMPL Event<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public DynEvent {
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
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>	TypeHandler;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>	TypeParams;

		Event()
		{
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9) const
		{
			// Iterate through all event handlers
			const typename SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual String GetSignature() const override
		{
			return TypeSignature::GetSignatureID();
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

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			return new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>(pFunc, pUserData));
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9));
			}
		}

		virtual void Emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9));
			}
		}

		virtual void Emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::FromString(sParams);
			Emit(cParams);
		}

		virtual void Emit(const XmlElement &cElement) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::FromXml(cElement);
			Emit(cParams);
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 9 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class PLCORE_TMPL Event<T0, T1, T2, T3, T4, T5, T6, T7, T8> : public DynEvent {
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
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7, T8>	TypeHandler;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>	TypeParams;

		Event()
		{
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8) const
		{
			// Iterate through all event handlers
			const typename SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7, t8);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual String GetSignature() const override
		{
			return TypeSignature::GetSignatureID();
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

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			return new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>(pFunc, pUserData));
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8));
			}
		}

		virtual void Emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8));
			}
		}

		virtual void Emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>::FromString(sParams);
			Emit(cParams);
		}

		virtual void Emit(const XmlElement &cElement) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>::FromXml(cElement);
			Emit(cParams);
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 8 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class PLCORE_TMPL Event<T0, T1, T2, T3, T4, T5, T6, T7> : public DynEvent {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7>	TypeHandler;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7>	TypeParams;

		Event()
		{
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7) const
		{
			// Iterate through all event handlers
			const typename SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual String GetSignature() const override
		{
			return TypeSignature::GetSignatureID();
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

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			return new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7>(pFunc, pUserData));
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7));
			}
		}

		virtual void Emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7));
			}
		}

		virtual void Emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7>::FromString(sParams);
			Emit(cParams);
		}

		virtual void Emit(const XmlElement &cElement) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7>::FromXml(cElement);
			Emit(cParams);
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 7 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class PLCORE_TMPL Event<T0, T1, T2, T3, T4, T5, T6> : public DynEvent {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6>	TypeHandler;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6>	TypeParams;

		Event()
		{
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6) const
		{
			// Iterate through all event handlers
			const typename SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual String GetSignature() const override
		{
			return TypeSignature::GetSignatureID();
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

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			return new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6>(pFunc, pUserData));
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6));
			}
		}

		virtual void Emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6));
			}
		}

		virtual void Emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6>::FromString(sParams);
			Emit(cParams);
		}

		virtual void Emit(const XmlElement &cElement) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6>::FromXml(cElement);
			Emit(cParams);
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 6 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class PLCORE_TMPL Event<T0, T1, T2, T3, T4, T5> : public DynEvent {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef EventHandler<T0, T1, T2, T3, T4, T5>	TypeHandler;
		typedef Signature<void, T0, T1, T2, T3, T4, T5>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3, T4, T5>	TypeParams;

		Event()
		{
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5) const
		{
			// Iterate through all event handlers
			const typename SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual String GetSignature() const override
		{
			return TypeSignature::GetSignatureID();
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

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			return new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5>(pFunc, pUserData));
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5));
			}
		}

		virtual void Emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5));
			}
		}

		virtual void Emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5> cParams = Params<void, T0, T1, T2, T3, T4, T5>::FromString(sParams);
			Emit(cParams);
		}

		virtual void Emit(const XmlElement &cElement) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5> cParams = Params<void, T0, T1, T2, T3, T4, T5>::FromXml(cElement);
			Emit(cParams);
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 5 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4>
class PLCORE_TMPL Event<T0, T1, T2, T3, T4> : public DynEvent {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef EventHandler<T0, T1, T2, T3, T4>	TypeHandler;
		typedef Signature<void, T0, T1, T2, T3, T4>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3, T4>	TypeParams;

		Event()
		{
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4) const
		{
			// Iterate through all event handlers
			const typename SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual String GetSignature() const override
		{
			return TypeSignature::GetSignatureID();
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

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			return new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4>(pFunc, pUserData));
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4));
			}
		}

		virtual void Emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4));
			}
		}

		virtual void Emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4> cParams = Params<void, T0, T1, T2, T3, T4>::FromString(sParams);
			Emit(cParams);
		}

		virtual void Emit(const XmlElement &cElement) const override
		{
			Params<void, T0, T1, T2, T3, T4> cParams = Params<void, T0, T1, T2, T3, T4>::FromXml(cElement);
			Emit(cParams);
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 4 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3>
class PLCORE_TMPL Event<T0, T1, T2, T3> : public DynEvent {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef EventHandler<T0, T1, T2, T3>	TypeHandler;
		typedef Signature<void, T0, T1, T2, T3>	TypeSignature;
		typedef Params<void, T0, T1, T2, T3>	TypeParams;

		Event()
		{
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3) const
		{
			// Iterate through all event handlers
			const typename SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual String GetSignature() const override
		{
			return TypeSignature::GetSignatureID();
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

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			return new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3>(pFunc, pUserData));
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3));
			}
		}

		virtual void Emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3));
			}
		}

		virtual void Emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3> cParams = Params<void, T0, T1, T2, T3>::FromString(sParams);
			Emit(cParams);
		}

		virtual void Emit(const XmlElement &cElement) const override
		{
			Params<void, T0, T1, T2, T3> cParams = Params<void, T0, T1, T2, T3>::FromXml(cElement);
			Emit(cParams);
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 3 parameters without a return value
*/
template <typename T0, typename T1, typename T2>
class PLCORE_TMPL Event<T0, T1, T2> : public DynEvent {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef EventHandler<T0, T1, T2>	TypeHandler;
		typedef Signature<void, T0, T1, T2>	TypeSignature;
		typedef Params<void, T0, T1, T2>	TypeParams;

		Event()
		{
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2) const
		{
			// Iterate through all event handlers
			const typename SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual String GetSignature() const override
		{
			return TypeSignature::GetSignatureID();
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

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			return new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2>(pFunc, pUserData));
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2));
			}
		}

		virtual void Emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2));
			}
		}

		virtual void Emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2> cParams = Params<void, T0, T1, T2>::FromString(sParams);
			Emit(cParams);
		}

		virtual void Emit(const XmlElement &cElement) const override
		{
			Params<void, T0, T1, T2> cParams = Params<void, T0, T1, T2>::FromXml(cElement);
			Emit(cParams);
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 2 parameters without a return value
*/
template <typename T0, typename T1>
class PLCORE_TMPL Event<T0, T1> : public DynEvent {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef EventHandler<T0, T1>		TypeHandler;
		typedef Signature<void, T0, T1>		TypeSignature;
		typedef Params<void, T0, T1>		TypeParams;

		Event()
		{
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(_T0 t0, _T1 t1) const
		{
			// Iterate through all event handlers
			const SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual String GetSignature() const override
		{
			return TypeSignature::GetSignatureID();
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

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			return new TypeHandler(new FuncGenFunPtr<void, T0, T1>(pFunc, pUserData));
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1));
			}
		}

		virtual void Emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1));
			}
		}

		virtual void Emit(const String &sParams) const override
		{
			Params<void, T0, T1> cParams = Params<void, T0, T1>::FromString(sParams);
			Emit(cParams);
		}

		virtual void Emit(const XmlElement &cElement) const override
		{
			Params<void, T0, T1> cParams = Params<void, T0, T1>::FromXml(cElement);
			Emit(cParams);
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 1 parameter without a return value
*/
template <typename T0>
class PLCORE_TMPL Event<T0> : public DynEvent {
	public:
		typedef typename Type<T0>::_Type _T0;

	public:
		typedef EventHandler<T0>	TypeHandler;
		typedef Signature<void, T0>	TypeSignature;
		typedef Params<void, T0>	TypeParams;

		Event()
		{
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(_T0 t0) const
		{
			// Iterate through all event handlers
			const SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0);

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual String GetSignature() const override
		{
			return TypeSignature::GetSignatureID();
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

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			return new TypeHandler(new FuncGenFunPtr<void, T0>(pFunc, pUserData));
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0));
			}
		}

		virtual void Emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// Emit event
				(*this)(Type<T0> ::ConvertStorageToReal(cP.Param0));
			}
		}

		virtual void Emit(const String &sParams) const override
		{
			Params<void, T0> cParams = Params<void, T0>::FromString(sParams);
			Emit(cParams);
		}

		virtual void Emit(const XmlElement &cElement) const override
		{
			Params<void, T0> cParams = Params<void, T0>::FromXml(cElement);
			Emit(cParams);
		}
};

/**
*  @brief
*    Generic event class
*
*  @remarks
*    Implementation for 0 parameters without a return value
*/
template <>
class PLCORE_TMPL Event<> : public DynEvent {
	public:
		typedef EventHandler<>	TypeHandler;
		typedef Signature<void>	TypeSignature;
		typedef Params<void>	TypeParams;

		Event()
		{
		}

		virtual ~Event()
		{
		}

		virtual void operator ()() const
		{
			// Iterate through all event handlers
			const SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const SimpleList<DynEventHandler*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor();

				// Next element, please
				pElement = pNextElement;
			}
		}

		virtual String GetSignature() const override
		{
			return TypeSignature::GetSignatureID();
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

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			return new TypeHandler(new FuncGenFunPtr<void>(pFunc, pUserData));
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Emit event
				(*this)();
			}
		}

		virtual void Emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Emit event
				(*this)();
			}
		}

		virtual void Emit(const String &sParams) const override
		{
			Params<void> cParams = Params<void>::FromString(sParams);
			Emit(cParams);
		}

		virtual void Emit(const XmlElement &cElement) const override
		{
			Params<void> cParams = Params<void>::FromXml(cElement);
			Emit(cParams);
		}
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_EVENT_H__
