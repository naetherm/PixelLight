/*********************************************************\
 *  File: EventDynEvent.h                                *
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


#ifndef __PLCORE_EVENTDYNEVENT_H__
#define __PLCORE_EVENTDYNEVENT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Base/Func/FuncGenFunPtr.h"
#include "PLCore/Base/Event/DynEvent.h"
#include "PLCore/Base/Event/EventHandlerDynEventHandler.h"
#include "PLCore/Base/Event/Event.h"
#include "PLCore/Base/Event/EventDesc.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
// [TODO] move this to DynEvent when conversion is done
class PLCORE_TMPL EventWrapperBase : public DynEvent {
	public:
		EventWrapperBase(const EventDesc *pDesc)
			: m_pDesc(pDesc)
		{
			
		}
		
		virtual ~EventWrapperBase()
		{
			
		}
		
		virtual const EventDesc *GetDesc() const
		{
			return m_pDesc;
		}

	private:
		const EventDesc *m_pDesc;


};
/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for up to 16 parameters without a return value
*/
template <typename T0 = NullType, typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType, typename T6 = NullType, typename T7 = NullType, typename T8 = NullType, typename T9 = NullType, typename T10 = NullType, typename T11 = NullType, typename T12 = NullType, typename T13 = NullType, typename T14 = NullType, typename T15 = NullType>
class PLCORE_TMPL EventWrapper : public EventWrapperBase {
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
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14, _T15>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>		TypeParams; // Needed for DynEvent::Emit

		EventWrapper(Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14, _T15> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
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

		virtual void Connect(DynEventHandler &cHandler)
		{
			if (cHandler.GetSignature() != GetSignature())
				return;
			
			m_pEvent->Connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void Disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->Disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 GetNumOfConnects() const { return m_pEvent->GetNumOfConnects(); }

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
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
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
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

	private:
		Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14, _T15> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 15 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class PLCORE_TMPL EventWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public EventWrapperBase {
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
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>		TypeParams; // Needed for DynEvent::Emit

		EventWrapper(Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
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

		virtual void Connect(DynEventHandler &cHandler)
		{
			if (cHandler.GetSignature() != GetSignature())
				return;
			
			m_pEvent->Connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void Disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->Disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 GetNumOfConnects() const { return m_pEvent->GetNumOfConnects(); }

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
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
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
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

	private:
		Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13, _T14> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 14 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class PLCORE_TMPL EventWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public EventWrapperBase {
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
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>		TypeParams; // Needed for DynEvent::Emit

		EventWrapper(Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
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

		virtual void Connect(DynEventHandler &cHandler)
		{
			if (cHandler.GetSignature() != GetSignature())
				return;
			
			m_pEvent->Connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void Disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->Disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 GetNumOfConnects() const { return m_pEvent->GetNumOfConnects(); }

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
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
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
						Type<T4> ::ConvertStorageToReal(cP.Param4),  Type<T5> ::ConvertStorageToReal(cP.Param5),  Type<T6> ::ConvertStorageToReal(cP.Param6),  Type<T7> ::ConvertStorageToReal(cP.Param7),
						Type<T8> ::ConvertStorageToReal(cP.Param8),  Type<T9> ::ConvertStorageToReal(cP.Param9),  Type<T10>::ConvertStorageToReal(cP.Param10), Type<T11>::ConvertStorageToReal(cP.Param11),
						Type<T12>::ConvertStorageToReal(cP.Param12), Type<T13>::ConvertStorageToReal(cP.Param13));
			}
		}

		virtual void Emit(const String &sParams) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13 >::FromString(sParams);
			Emit(cParams);
		}

		virtual void Emit(const XmlElement &cElement) const override
		{
			Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> cParams = Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::FromXml(cElement);
			Emit(cParams);
		}

	private:
		Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12, _T13> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 13 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class PLCORE_TMPL EventWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public EventWrapperBase {
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
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>		TypeParams; // Needed for DynEvent::Emit

		EventWrapper(Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
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

		virtual void Connect(DynEventHandler &cHandler)
		{
			if (cHandler.GetSignature() != GetSignature())
				return;
			
			m_pEvent->Connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void Disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->Disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 GetNumOfConnects() const { return m_pEvent->GetNumOfConnects(); }

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
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
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
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

	private:
		Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11, _T12> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 12 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class PLCORE_TMPL EventWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public EventWrapperBase {
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
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>		TypeParams; // Needed for DynEvent::Emit

		EventWrapper(Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
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

		virtual void Connect(DynEventHandler &cHandler)
		{
			if (cHandler.GetSignature() != GetSignature())
				return;
			
			m_pEvent->Connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void Disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->Disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 GetNumOfConnects() const { return m_pEvent->GetNumOfConnects(); }

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
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
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
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

	private:
		Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10, _T11> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 11 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class PLCORE_TMPL EventWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public EventWrapperBase {
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
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>		TypeParams; // Needed for DynEvent::Emit

		EventWrapper(Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
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

		virtual void Connect(DynEventHandler &cHandler)
		{
			if (cHandler.GetSignature() != GetSignature())
				return;
			
			m_pEvent->Connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void Disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->Disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 GetNumOfConnects() const { return m_pEvent->GetNumOfConnects(); }

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
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
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
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

	private:
		Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9, _T10> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 10 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class PLCORE_TMPL EventWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public EventWrapperBase {
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
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>		TypeParams; // Needed for DynEvent::Emit

		EventWrapper(Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
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

		virtual void Connect(DynEventHandler &cHandler)
		{
			if (cHandler.GetSignature() != GetSignature())
				return;
			
			m_pEvent->Connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void Disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->Disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 GetNumOfConnects() const { return m_pEvent->GetNumOfConnects(); }

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
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
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
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

	private:
		Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8, _T9> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 9 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class PLCORE_TMPL EventWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8> : public EventWrapperBase {
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
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3, T4, T5, T6, T7, T8>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>		TypeParams; // Needed for DynEvent::Emit

		EventWrapper(Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
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

		virtual void Connect(DynEventHandler &cHandler)
		{
			if (cHandler.GetSignature() != GetSignature())
				return;
			
			m_pEvent->Connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void Disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->Disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 GetNumOfConnects() const { return m_pEvent->GetNumOfConnects(); }

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
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
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
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

	private:
		Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7, _T8> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 8 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class PLCORE_TMPL EventWrapper<T0, T1, T2, T3, T4, T5, T6, T7> : public EventWrapperBase {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3, T4, T5, T6, T7>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6, T7>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6, T7>		TypeParams; // Needed for DynEvent::Emit

		EventWrapper(Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
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

		virtual void Connect(DynEventHandler &cHandler)
		{
			if (cHandler.GetSignature() != GetSignature())
				return;
			
			m_pEvent->Connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void Disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->Disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 GetNumOfConnects() const { return m_pEvent->GetNumOfConnects(); }

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
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
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
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

	private:
		Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6, _T7> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 7 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class PLCORE_TMPL EventWrapper<T0, T1, T2, T3, T4, T5, T6> : public EventWrapperBase {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4, _T5, _T6>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3, T4, T5, T6>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3, T4, T5, T6>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3, T4, T5, T6>		TypeParams; // Needed for DynEvent::Emit

		EventWrapper(Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
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

		virtual void Connect(DynEventHandler &cHandler)
		{
			if (cHandler.GetSignature() != GetSignature())
				return;
			
			m_pEvent->Connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void Disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->Disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 GetNumOfConnects() const { return m_pEvent->GetNumOfConnects(); }

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
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
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
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

	private:
		Event<_T0, _T1, _T2, _T3, _T4, _T5, _T6> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 6 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class PLCORE_TMPL EventWrapper<T0, T1, T2, T3, T4, T5> : public EventWrapperBase {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4, _T5>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3, T4, T5>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3, T4, T5>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3, T4, T5>		TypeParams; // Needed for DynEvent::Emit

		EventWrapper(Event<_T0, _T1, _T2, _T3, _T4, _T5> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
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

		virtual void Connect(DynEventHandler &cHandler)
		{
			if (cHandler.GetSignature() != GetSignature())
				return;
			
			m_pEvent->Connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void Disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->Disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 GetNumOfConnects() const { return m_pEvent->GetNumOfConnects(); }

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
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
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
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

	private:
		Event<_T0, _T1, _T2, _T3, _T4, _T5> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 5 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4>
class PLCORE_TMPL EventWrapper<T0, T1, T2, T3, T4> : public EventWrapperBase {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef EventHandler<_T0, _T1, _T2, _T3, _T4>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3, T4>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3, T4>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3, T4>		TypeParams; // Needed for DynEvent::Emit

		EventWrapper(Event<_T0, _T1, _T2, _T3, _T4> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
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

		virtual void Connect(DynEventHandler &cHandler)
		{
			if (cHandler.GetSignature() != GetSignature())
				return;
			
			m_pEvent->Connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void Disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->Disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 GetNumOfConnects() const { return m_pEvent->GetNumOfConnects(); }

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3, T4>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
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
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3),
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

	private:
		Event<_T0, _T1, _T2, _T3, _T4> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 4 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3>
class PLCORE_TMPL EventWrapper<T0, T1, T2, T3> : public EventWrapperBase {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef EventHandler<_T0, _T1, _T2, _T3>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2, T3>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2, T3>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2, T3>		TypeParams; // Needed for DynEvent::Emit

		EventWrapper(Event<_T0, _T1, _T2, _T3> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
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

		virtual void Connect(DynEventHandler &cHandler)
		{
			if (cHandler.GetSignature() != GetSignature())
				return;
			
			m_pEvent->Connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void Disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->Disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 GetNumOfConnects() const { return m_pEvent->GetNumOfConnects(); }

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2, T3>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3));
			}
		}

		virtual void Emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// Emit event
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2),  Type<T3> ::ConvertStorageToReal(cP.Param3));
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

	private:
		Event<_T0, _T1, _T2, _T3> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 3 parameters without a return value
*/
template <typename T0, typename T1, typename T2>
class PLCORE_TMPL EventWrapper<T0, T1, T2> : public EventWrapperBase {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef EventHandler<_T0, _T1, _T2>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1, T2>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1, T2>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1, T2>		TypeParams; // Needed for DynEvent::Emit

		EventWrapper(Event<_T0, _T1, _T2> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
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

		virtual void Connect(DynEventHandler &cHandler)
		{
			if (cHandler.GetSignature() != GetSignature())
				return;
			
			m_pEvent->Connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void Disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->Disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 GetNumOfConnects() const { return m_pEvent->GetNumOfConnects(); }

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1, T2>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2));
			}
		}

		virtual void Emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// Emit event
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1),  Type<T2> ::ConvertStorageToReal(cP.Param2));
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

	private:
		Event<_T0, _T1, _T2> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 2 parameters without a return value
*/
template <typename T0, typename T1>
class PLCORE_TMPL EventWrapper<T0, T1> : public EventWrapperBase {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef EventHandler<_T0, _T1>		TypeHandler; // Needed for  DynEvent::CreateGenericEventHandler
		typedef EventHandlerWrapper<T0, T1>	TypeHandlerWrapper;
		typedef Signature<void, T0, T1>	TypeSignature; // Needed for DynEvent::GetSignature (From DynSignature)
		typedef Params<void, T0, T1>		TypeParams; // Needed for DynEvent::Emit

		EventWrapper(Event<_T0, _T1> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc) ,m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
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

		virtual void Connect(DynEventHandler &cHandler)
		{
			if (cHandler.GetSignature() != GetSignature())
				return;
			
			m_pEvent->Connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void Disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->Disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 GetNumOfConnects() const { return m_pEvent->GetNumOfConnects(); }

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0, T1>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1));
			}
		}

		virtual void Emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// Emit event
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0),  Type<T1> ::ConvertStorageToReal(cP.Param1));
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

	private:
		Event<_T0, _T1> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 1 parameters without a return value
*/
template <typename T0>
class PLCORE_TMPL EventWrapper<T0> : public EventWrapperBase {
	public:
		typedef typename Type<T0>::_Type _T0;
	public:
		typedef EventHandler<_T0>	TypeHandler;
		typedef EventHandlerWrapper<T0>	TypeHandlerWrapper;
		typedef Signature<void, T0>	TypeSignature;
		typedef Params<void, T0>	TypeParams;

		EventWrapper(Event<_T0> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc), m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
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

		virtual void Connect(DynEventHandler &cHandler)
		{
			if (cHandler.GetSignature() != GetSignature())
				return;
			
			m_pEvent->Connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void Disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->Disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 GetNumOfConnects() const { return m_pEvent->GetNumOfConnects(); }

		virtual DynEventHandler *CreateGenericEventHandler(const FUNC &pFunc, void *pUserData = nullptr) const override
		{
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void, T0>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				TypeParams &cP = static_cast<TypeParams&>(cParams);

				// Emit event
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0));
			}
		}

		virtual void Emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Get typed params
				const TypeParams &cP = static_cast<const TypeParams&>(cParams);

				// Emit event
				(*m_pEvent)(Type<T0> ::ConvertStorageToReal(cP.Param0));
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

	private:
		Event<_T0> *m_pEvent;
};

/**
*  @brief
*    DynEvent wrapper of a Generic event class
*
*  @remarks
*    Implementation for 0 parameters without a return value
*/
template <>
class PLCORE_TMPL EventWrapper<> : public EventWrapperBase {
	public:
		typedef EventHandler<>	TypeHandler;
		typedef EventHandlerWrapper<>	TypeHandlerWrapper;
		typedef Signature<void>	TypeSignature;
		typedef Params<void>	TypeParams;

		EventWrapper(Event<> *pEvent, const EventDesc *pDesc)
			: EventWrapperBase(pDesc), m_pEvent(pEvent)
		{
		}

		virtual ~EventWrapper()
		{
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
			// We create dynamically an new event handler.
			// so the event handler wrapper takes ownership over the event handler (is responsible for the cleanup of the handler)
			TypeHandler* pHandler = new TypeHandler(new FuncGenFunPtr<void>(pFunc, pUserData));
			return new TypeHandlerWrapper(pHandler, nullptr, true);
		}

		virtual void Connect(DynEventHandler &cHandler)
		{
			if (cHandler.GetSignature() != GetSignature())
				return;
			
			m_pEvent->Connect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual void Disconnect(DynEventHandler &cHandler)
		{
			m_pEvent->Disconnect(*static_cast<TypeHandlerWrapper&>(cHandler).m_pHandler);
		}

		virtual uint32 GetNumOfConnects() const { return m_pEvent->GetNumOfConnects(); }


		virtual void Emit(DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Emit event
				(*m_pEvent)();
			}
		}

		virtual void Emit(const DynParams &cParams) const override
		{
			// Check signature
			if (cParams.GetSignature() == GetSignature()) {
				// Emit event
				(*m_pEvent)();
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

	private:
		Event<> *m_pEvent;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_EVENTDYNEVENT_H__
