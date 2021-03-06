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
#include "PLCore/Base/Event/EventHandler.h"
#include "PLCore/Base/Event/EventBase.h"


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
class PLCORE_TMPL Event : public EventBase<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>{
	public:
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>		TypeHandler;
		typedef EventHandlerBase<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>		TypeHandlerBase;
		typedef EventBase<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>	EventBaseType;
		
		Event()
		{
#ifdef _DEBUG
			Event_Dummy();
#endif
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12, T13 t13, T14 t14, T15 t15) const
		{
			// Iterate through all event handlers
			const typename SimpleList<TypeHandlerBase*>::ListElement *pElement = EventBaseType::m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<TypeHandlerBase*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15);

				// Next element, please
				pElement = pNextElement;
			}
		}


	private:
		// Needed to make sure that the compiler generates code for the corresponding eventhandler class template
		// otherwise an undefined reference to the used functor constructor is generated
		// But this is only needed for gcc < 4.7.0 and in debug build (when the -O option is added to the gcc commandline then this hack isn't needed either, strange)
		// The undefined reference is created when a Event<> is declared inside a class and used by the class but no other class inside the same libarary connects to this Event.

		void Event_Dummy()
		{
			TypeHandler _dummy;
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
class PLCORE_TMPL Event<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public EventBase<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> {
	public:
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>		TypeHandler;
		typedef EventHandlerBase<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>		TypeHandlerBase;
		typedef EventBase<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>	EventBaseType;

		Event()
		{
#ifdef _DEBUG
			Event_Dummy();
#endif
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12, T13 t13, T14 t14) const
		{
			// Iterate through all event handlers
			const typename SimpleList<TypeHandlerBase*>::ListElement *pElement = EventBaseType::m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<TypeHandlerBase*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14);

				// Next element, please
				pElement = pNextElement;
			}
		}


	private:
		// Needed to make sure that the compiler generates code for the corresponding eventhandler class template
		// otherwise an undefined reference to the used functor constructor is generated
		// But this is only needed for gcc < 4.7.0 and in debug build (when the -O option is added to the gcc commandline then this hack isn't needed either, strange)
		// The undefined reference is created when a Event<> is declared inside a class and used by the class but no other class inside the same libarary connects to this Event.

		void Event_Dummy()
		{
			TypeHandler _dummy;
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
class PLCORE_TMPL Event<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public EventBase<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> {
	public:
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>	TypeHandler;
		typedef EventHandlerBase<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>		TypeHandlerBase;
		typedef EventBase<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>	EventBaseType;

		Event()
		{
#ifdef _DEBUG
			Event_Dummy();
#endif
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12, T13 t13) const
		{
			// Iterate through all event handlers
			const typename SimpleList<TypeHandlerBase*>::ListElement *pElement = EventBaseType::m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<TypeHandlerBase*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13);

				// Next element, please
				pElement = pNextElement;
			}
		}


	private:
		// Needed to make sure that the compiler generates code for the corresponding eventhandler class template
		// otherwise an undefined reference to the used functor constructor is generated
		// But this is only needed for gcc < 4.7.0 and in debug build (when the -O option is added to the gcc commandline then this hack isn't needed either, strange)
		// The undefined reference is created when a Event<> is declared inside a class and used by the class but no other class inside the same libarary connects to this Event.

		void Event_Dummy()
		{
			TypeHandler _dummy;
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
class PLCORE_TMPL Event<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public EventBase<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> {
	public:
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>		TypeHandler;
		typedef EventHandlerBase<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>		TypeHandlerBase;
		typedef EventBase<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>	EventBaseType;

		Event()
		{
#ifdef _DEBUG
			Event_Dummy();
#endif
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11, T12 t12) const
		{
			// Iterate through all event handlers
			const typename SimpleList<TypeHandlerBase*>::ListElement *pElement = EventBaseType::m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<TypeHandlerBase*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12);

				// Next element, please
				pElement = pNextElement;
			}
		}


	private:
		// Needed to make sure that the compiler generates code for the corresponding eventhandler class template
		// otherwise an undefined reference to the used functor constructor is generated
		// But this is only needed for gcc < 4.7.0 and in debug build (when the -O option is added to the gcc commandline then this hack isn't needed either, strange)
		// The undefined reference is created when a Event<> is declared inside a class and used by the class but no other class inside the same libarary connects to this Event.

		void Event_Dummy()
		{
			TypeHandler _dummy;
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
class PLCORE_TMPL Event<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public EventBase<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> {
	public:
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>		TypeHandler;
		typedef EventHandlerBase<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>		TypeHandlerBase;
		typedef EventBase<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>	EventBaseType;

		Event()
		{
#ifdef _DEBUG
			Event_Dummy();
#endif
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, T11 t11) const
		{
			// Iterate through all event handlers
			const typename SimpleList<TypeHandlerBase*>::ListElement *pElement = EventBaseType::m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<TypeHandlerBase*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11);

				// Next element, please
				pElement = pNextElement;
			}
		}


	private:
		// Needed to make sure that the compiler generates code for the corresponding eventhandler class template
		// otherwise an undefined reference to the used functor constructor is generated
		// But this is only needed for gcc < 4.7.0 and in debug build (when the -O option is added to the gcc commandline then this hack isn't needed either, strange)
		// The undefined reference is created when a Event<> is declared inside a class and used by the class but no other class inside the same libarary connects to this Event.

		void Event_Dummy()
		{
			TypeHandler _dummy;
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
class PLCORE_TMPL Event<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public EventBase<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> {
	public:
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>		TypeHandler;
		typedef EventHandlerBase<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>		TypeHandlerBase;
		typedef EventBase<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>	EventBaseType;

		Event()
		{
#ifdef _DEBUG
			Event_Dummy();
#endif
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10) const
		{
			// Iterate through all event handlers
			const typename SimpleList<TypeHandlerBase*>::ListElement *pElement = EventBaseType::m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<TypeHandlerBase*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);

				// Next element, please
				pElement = pNextElement;
			}
		}


	private:
		// Needed to make sure that the compiler generates code for the corresponding eventhandler class template
		// otherwise an undefined reference to the used functor constructor is generated
		// But this is only needed for gcc < 4.7.0 and in debug build (when the -O option is added to the gcc commandline then this hack isn't needed either, strange)
		// The undefined reference is created when a Event<> is declared inside a class and used by the class but no other class inside the same libarary connects to this Event.

		void Event_Dummy()
		{
			TypeHandler _dummy;
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
class PLCORE_TMPL Event<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public EventBase<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> {
	public:
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>	TypeHandler;
		typedef EventHandlerBase<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>		TypeHandlerBase;
		typedef EventBase<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>	EventBaseType;

		Event()
		{
#ifdef _DEBUG
			Event_Dummy();
#endif
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9) const
		{
			// Iterate through all event handlers
			const typename SimpleList<TypeHandlerBase*>::ListElement *pElement = EventBaseType::m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<TypeHandlerBase*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9);

				// Next element, please
				pElement = pNextElement;
			}
		}


	private:
		// Needed to make sure that the compiler generates code for the corresponding eventhandler class template
		// otherwise an undefined reference to the used functor constructor is generated
		// But this is only needed for gcc < 4.7.0 and in debug build (when the -O option is added to the gcc commandline then this hack isn't needed either, strange)
		// The undefined reference is created when a Event<> is declared inside a class and used by the class but no other class inside the same libarary connects to this Event.

		void Event_Dummy()
		{
			TypeHandler _dummy;
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
class PLCORE_TMPL Event<T0, T1, T2, T3, T4, T5, T6, T7, T8> : public EventBase<T0, T1, T2, T3, T4, T5, T6, T7, T8> {
	public:
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7, T8>	TypeHandler;
		typedef EventHandlerBase<T0, T1, T2, T3, T4, T5, T6, T7, T8>	TypeHandlerBase;
		typedef EventBase<T0, T1, T2, T3, T4, T5, T6, T7, T8>	EventBaseType;

		Event()
		{
#ifdef _DEBUG
			Event_Dummy();
#endif
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8) const
		{
			// Iterate through all event handlers
			const typename SimpleList<TypeHandlerBase*>::ListElement *pElement = EventBaseType::m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<TypeHandlerBase*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7, t8);

				// Next element, please
				pElement = pNextElement;
			}
		}


	private:
		// Needed to make sure that the compiler generates code for the corresponding eventhandler class template
		// otherwise an undefined reference to the used functor constructor is generated
		// But this is only needed for gcc < 4.7.0 and in debug build (when the -O option is added to the gcc commandline then this hack isn't needed either, strange)
		// The undefined reference is created when a Event<> is declared inside a class and used by the class but no other class inside the same libarary connects to this Event.

		void Event_Dummy()
		{
			TypeHandler _dummy;
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
class PLCORE_TMPL Event<T0, T1, T2, T3, T4, T5, T6, T7> : public EventBase<T0, T1, T2, T3, T4, T5, T6, T7> {
	public:
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6, T7>	TypeHandler;
		typedef EventHandlerBase<T0, T1, T2, T3, T4, T5, T6, T7>	TypeHandlerBase;
		typedef EventBase<T0, T1, T2, T3, T4, T5, T6, T7>	EventBaseType;

		Event()
		{
#ifdef _DEBUG
			Event_Dummy();
#endif
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7) const
		{
			// Iterate through all event handlers
			const typename SimpleList<TypeHandlerBase*>::ListElement *pElement = EventBaseType::m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<TypeHandlerBase*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6, t7);

				// Next element, please
				pElement = pNextElement;
			}
		}


	private:
		// Needed to make sure that the compiler generates code for the corresponding eventhandler class template
		// otherwise an undefined reference to the used functor constructor is generated
		// But this is only needed for gcc < 4.7.0 and in debug build (when the -O option is added to the gcc commandline then this hack isn't needed either, strange)
		// The undefined reference is created when a Event<> is declared inside a class and used by the class but no other class inside the same libarary connects to this Event.

		void Event_Dummy()
		{
			TypeHandler _dummy;
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
class PLCORE_TMPL Event<T0, T1, T2, T3, T4, T5, T6> : public EventBase<T0, T1, T2, T3, T4, T5, T6> {
	public:
		typedef EventHandler<T0, T1, T2, T3, T4, T5, T6>	TypeHandler;
		typedef EventHandlerBase<T0, T1, T2, T3, T4, T5, T6>	TypeHandlerBase;
		typedef EventBase<T0, T1, T2, T3, T4, T5, T6>	EventBaseType;

		Event()
		{
#ifdef _DEBUG
			Event_Dummy();
#endif
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6) const
		{
			// Iterate through all event handlers
			const typename SimpleList<TypeHandlerBase*>::ListElement *pElement = EventBaseType::m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<TypeHandlerBase*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5, t6);

				// Next element, please
				pElement = pNextElement;
			}
		}


	private:
		// Needed to make sure that the compiler generates code for the corresponding eventhandler class template
		// otherwise an undefined reference to the used functor constructor is generated
		// But this is only needed for gcc < 4.7.0 and in debug build (when the -O option is added to the gcc commandline then this hack isn't needed either, strange)
		// The undefined reference is created when a Event<> is declared inside a class and used by the class but no other class inside the same libarary connects to this Event.

		void Event_Dummy()
		{
			TypeHandler _dummy;
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
class PLCORE_TMPL Event<T0, T1, T2, T3, T4, T5> : public EventBase<T0, T1, T2, T3, T4, T5> {
	public:
		typedef EventHandler<T0, T1, T2, T3, T4, T5>	TypeHandler;
		typedef EventHandlerBase<T0, T1, T2, T3, T4, T5>	TypeHandlerBase;
		typedef EventBase<T0, T1, T2, T3, T4, T5>	EventBaseType;

		Event()
		{
#ifdef _DEBUG
			Event_Dummy();
#endif
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5) const
		{
			// Iterate through all event handlers
			const typename SimpleList<TypeHandlerBase*>::ListElement *pElement = EventBaseType::m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<TypeHandlerBase*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4, t5);

				// Next element, please
				pElement = pNextElement;
			}
		}


	private:
		// Needed to make sure that the compiler generates code for the corresponding eventhandler class template
		// otherwise an undefined reference to the used functor constructor is generated
		// But this is only needed for gcc < 4.7.0 and in debug build (when the -O option is added to the gcc commandline then this hack isn't needed either, strange)
		// The undefined reference is created when a Event<> is declared inside a class and used by the class but no other class inside the same libarary connects to this Event.

		void Event_Dummy()
		{
			TypeHandler _dummy;
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
class PLCORE_TMPL Event<T0, T1, T2, T3, T4> : public EventBase<T0, T1, T2, T3, T4> {
	public:
		typedef EventHandler<T0, T1, T2, T3, T4>	TypeHandler;
		typedef EventHandlerBase<T0, T1, T2, T3, T4>	TypeHandlerBase;
		typedef EventBase<T0, T1, T2, T3, T4>	EventBaseType;

		Event()
		{
#ifdef _DEBUG
			Event_Dummy();
#endif
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4) const
		{
			// Iterate through all event handlers
			const typename SimpleList<TypeHandlerBase*>::ListElement *pElement = EventBaseType::m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<TypeHandlerBase*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3, t4);

				// Next element, please
				pElement = pNextElement;
			}
		}


	private:
		// Needed to make sure that the compiler generates code for the corresponding eventhandler class template
		// otherwise an undefined reference to the used functor constructor is generated
		// But this is only needed for gcc < 4.7.0 and in debug build (when the -O option is added to the gcc commandline then this hack isn't needed either, strange)
		// The undefined reference is created when a Event<> is declared inside a class and used by the class but no other class inside the same libarary connects to this Event.

		void Event_Dummy()
		{
			TypeHandler _dummy;
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
class PLCORE_TMPL Event<T0, T1, T2, T3> : public EventBase<T0, T1, T2, T3> {
	public:
		typedef EventHandler<T0, T1, T2, T3>	TypeHandler;
		typedef EventHandlerBase<T0, T1, T2, T3>	TypeHandlerBase;
		typedef EventBase<T0, T1, T2, T3>	EventBaseType;

		Event()
		{
#ifdef _DEBUG
			Event_Dummy();
#endif
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2, T3 t3) const
		{
			// Iterate through all event handlers
			const typename SimpleList<TypeHandlerBase*>::ListElement *pElement = EventBaseType::m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<TypeHandlerBase*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2, t3);

				// Next element, please
				pElement = pNextElement;
			}
		}


	private:
		// Needed to make sure that the compiler generates code for the corresponding eventhandler class template
		// otherwise an undefined reference to the used functor constructor is generated
		// But this is only needed for gcc < 4.7.0 and in debug build (when the -O option is added to the gcc commandline then this hack isn't needed either, strange)
		// The undefined reference is created when a Event<> is declared inside a class and used by the class but no other class inside the same libarary connects to this Event.

		void Event_Dummy()
		{
			TypeHandler _dummy;
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
class PLCORE_TMPL Event<T0, T1, T2> : public EventBase<T0, T1, T2> {
	public:
		typedef EventHandler<T0, T1, T2>	TypeHandler;
		typedef EventHandlerBase<T0, T1, T2>	TypeHandlerBase;
		typedef EventBase<T0>	EventBaseType;

		Event()
		{
#ifdef _DEBUG
			Event_Dummy();
#endif
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(T0 t0, T1 t1, T2 t2) const
		{
			// Iterate through all event handlers
			const typename SimpleList<TypeHandlerBase*>::ListElement *pElement = EventBaseType::m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<TypeHandlerBase*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1, t2);

				// Next element, please
				pElement = pNextElement;
			}
		}


	private:
		// Needed to make sure that the compiler generates code for the corresponding eventhandler class template
		// otherwise an undefined reference to the used functor constructor is generated
		// But this is only needed for gcc < 4.7.0 and in debug build (when the -O option is added to the gcc commandline then this hack isn't needed either, strange)
		// The undefined reference is created when a Event<> is declared inside a class and used by the class but no other class inside the same libarary connects to this Event.

		void Event_Dummy()
		{
			TypeHandler _dummy;
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
class PLCORE_TMPL Event<T0, T1> : public EventBase<T0, T1> {
	public:
		typedef EventHandler<T0, T1>	TypeHandler;
		typedef EventHandlerBase<T0, T1>	TypeHandlerBase;
		typedef EventBase<T0, T1>		EventBaseType;

		Event()
		{
 #ifdef _DEBUG
			Event_Dummy();
#endif
		}

		virtual ~Event()
		{
		}

		virtual void operator ()(T0 t0, T1 t1) const
		{
			// Iterate through all event handlers
			const typename SimpleList<TypeHandlerBase*>::ListElement *pElement = EventBaseType::m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<TypeHandlerBase*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor(t0, t1);

				// Next element, please
				pElement = pNextElement;
			}
		}


	private:
		// Needed to make sure that the compiler generates code for the corresponding eventhandler class template
		// otherwise an undefined reference to the used functor constructor is generated
		// But this is only needed for gcc < 4.7.0 and in debug build (when the -O option is added to the gcc commandline then this hack isn't needed either, strange)
		// The undefined reference is created when a Event<> is declared inside a class and used by the class but no other class inside the same libarary connects to this Event.

		void Event_Dummy()
		{
			TypeHandler _dummy;
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
class PLCORE_TMPL Event<T0>: public EventBase<T0> {
	public:
		
		typedef EventHandler<T0>	TypeHandler;
		typedef EventHandlerBase<T0>	TypeHandlerBase;
		typedef EventBase<T0>	EventBaseType;
		
		Event()
		{
#ifdef _DEBUG
			Event_Dummy();
#endif
		}

		virtual ~Event()
		{
		}
		
		virtual void operator ()(T0 t0) const
		{
			// Iterate through all event handlers
			const typename SimpleList<TypeHandlerBase*>::ListElement *pElement = EventBaseType::m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const typename SimpleList<TypeHandlerBase*>::ListElement *pNextElement = pElement->pNextElement;

				TypeHandler * pHandler = static_cast<TypeHandler*>(pElement->Data);
				// Call the functor of the current event handler
				pHandler->m_cFunctor(t0);

				// Next element, please
				pElement = pNextElement;
			}
		}


	private:
		// Needed to make sure that the compiler generates code for the corresponding eventhandler class template
		// otherwise an undefined reference to the used functor constructor is generated
		// But this is only needed for gcc < 4.7.0 and in debug build (when the -O option is added to the gcc commandline then this hack isn't needed either, strange)
		// The undefined reference is created when a Event<> is declared inside a class and used by the class but no other class inside the same libarary connects to this Event.

		void Event_Dummy()
		{
			TypeHandler _dummy;
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
class PLCORE_TMPL Event<>: public EventBase<> {
	public:
		
		typedef EventHandler<>	TypeHandler;
		typedef EventHandlerBase<>	TypeHandlerBase;
		typedef EventBase<>	EventBaseType;
		
		Event()
		{
#ifdef _DEBUG
			Event_Dummy();
#endif
		}

		virtual ~Event()
		{
		}

		virtual void operator ()() const
		{
			// Iterate through all event handlers
			const SimpleList<TypeHandlerBase*>::ListElement *pElement = EventBaseType::m_lstHandlers.pFirstElement;
			while (pElement) {
				// Backup the next element because "pElement" may get invalid within the next step...
				const SimpleList<TypeHandlerBase*>::ListElement *pNextElement = pElement->pNextElement;

				// Call the functor of the current event handler
				static_cast<TypeHandler*>(pElement->Data)->m_cFunctor();

				// Next element, please
				pElement = pNextElement;
			}
		}


	private:
		// Needed to make sure that the compiler generates code for the corresponding eventhandler class template
		// otherwise an undefined reference to the used functor constructor is generated
		// But this is only needed for gcc < 4.7.0 and in a debug build (when the -O option is added to the gcc commandline then this hack isn't needed either, strange)
		// The undefined reference is created when a Event<> is declared inside a class and used by the class but no other class inside the same libarary connects to this Event.

		void Event_Dummy()
		{
			TypeHandler _dummy;
		}

};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_EVENT_H__
