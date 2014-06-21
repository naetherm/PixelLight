/*********************************************************\
 *  File: Functor.h                                      *
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


#ifndef __PLCORE_FUNCTOR_H__
#define __PLCORE_FUNCTOR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Base/Func/Func.h"
#include "PLCore/Base/Func/FuncFunPtr.h"
#include "PLCore/Base/Func/FuncMemPtr.h"
#include "PLCore/Base/Func/FuncGenFunPtr.h"
#include "PLCore/Base/Func/FuncGenMemPtr.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    This class wraps a function and only makes it's signature (<Return> (<Parameters>))
*    visible to the world. Other components can 'just use' the function without the need to know whether
*    it's a static/global/member function. A functor is a specialized case of a function object (functoid)
*    in that it points to another function it wraps (which can again be a function, method or functiod itself).
*
*  @verbatim
*    Usage examples:
*
*    // A simple function returning a 'float' and taking 3 formal parameters:
*    // -> 'cFunction' is 'a function' returning a 'float' and taking a 'int' and 'float' parameter
*    //    (we do not know more about it and also don't need to know more, we just 'use' it :)
*    // -> 'nValue' and 'fValue' are any values given to 'cFunction' that makes 'something' and
*    //    is returning 'something'
*    float MyArithmetic(Functor<float, int, float> &cFunction, int nValue, float fValue)
*    {
*        return cFunction(nValue, fValue);
*    }
*
*    // A global function (same usage for static member methods/function)
*    float MyAdditionFunction(int i, float f)
*    {
*        return i + f;
*    }
*    // -> Usage example: float fResult = MyArithmetic(Functor<float, int, float>(MyAdditionFunction), 1, 1.5f);
*
*    // An usual class with an usual method
*    class MyClass {
*        public:
*            float Addition(int i, float f)
*            {
*                return i + f;
*            }
*    };
*    // -> float fResult = MyArithmetic(Functor<float, int, float>(&MyClass::Addition, MyClass()), 1, 1.5f);
*  @endverbatim
*/
template <typename R, typename T0 = NullType, typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType, typename T6 = NullType, typename T7 = NullType, typename T8 = NullType, typename T9 = NullType, typename T10 = NullType, typename T11 = NullType, typename T12 = NullType, typename T13 = NullType, typename T14 = NullType, typename T15 = NullType>
class Functor : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> {


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


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @remarks
		*    Constructor for an empty functor
		*/
		Functor() :
			m_pFunc(nullptr)
		{
		}

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pFunc
		*    Pointer to a function object
		*
		*  @remarks
		*    Wrap a generic function object (functoid)
		*/
		Functor(DynFunc *pFunc) :
			m_pFunc(pFunc)
		{
		}

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pFunc
		*    Pointer to a static function
		*
		*  @remarks
		*    Wrap a static function pointer
		*/
		Functor(const typename Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>(pFunc))
		{
		}

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pMemFunc
		*    Pointer to a member function of a class
		*  @param[in] pObject
		*    Pointer to an instance of that class
		*
		*  @remarks
		*    Wrap a member function pointer
		*/
		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>(pMemFunc, pObject))
		{
		}

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pFunc
		*    Pointer to a generic static function
		*
		*  @remarks
		*    Wrap a generic static function pointer
		*/
		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>(pFunc))
		{
		}

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pMemFunc
		*    Pointer to a generic member function of a class
		*  @param[in] pObject
		*    Pointer to an instance of that class
		*
		*  @remarks
		*    Wrap a generic member function pointer
		*/
		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>(pMemFunc, pObject))
		{
		}

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cFunctor
		*    Source functor
		*/
		Functor(const Functor &cFunctor) :
			m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cFunctor
		*    Source functor
		*/
		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}


	//[-------------------------------------------------------]
	//[ Public virtual DynFunc functions                      ]
	//[-------------------------------------------------------]
	public:
		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14, _T15 t15) override
		{
			if (m_pFunc)
				return (*static_cast<Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>* >(m_pFunc))(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15);
			else
				return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */


};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 16 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class Functor<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> {
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

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14, _T15 t15) override
		{
			if (m_pFunc)
				(*static_cast<Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>* >(m_pFunc))(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15);
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 15 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class Functor<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> {
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

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14) override
		{
			if (m_pFunc)
				return (*static_cast<Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>* >(m_pFunc))(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14);
			else
				return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 15 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class Functor<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> {
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

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14) override
		{
			if (m_pFunc)
				return (*static_cast<Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>* >(m_pFunc))(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14);
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 14 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class Functor<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> {
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

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13) override
		{
			if (m_pFunc)
				return (*static_cast<Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>* >(m_pFunc))(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13);
			else
				return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 14 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class Functor<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> {
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

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13) override
		{
			if (m_pFunc)
				(*static_cast<Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>* >(m_pFunc))(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13);
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 13 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class Functor<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> {
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

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12) override
		{
			if (m_pFunc)
				return (*static_cast<Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>* >(m_pFunc))(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12);
			else
				return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};


/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 13 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class Functor<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> {
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

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12) override
		{
			if (m_pFunc)
				(*static_cast<Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>* >(m_pFunc))(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12);
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 12 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class Functor<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> {
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

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11) override
		{
			if (m_pFunc)
				return (*static_cast<Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>* >(m_pFunc))(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11);
			else
				return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};


/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 12 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class Functor<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> {
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

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11) override
		{
			if (m_pFunc)
				(*static_cast<Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>* >(m_pFunc))(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11);
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 11 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class Functor<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> {
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

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10) override
		{
			if (m_pFunc)
				return (*static_cast<Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>* >(m_pFunc))(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);
			else
				return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};


/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 11 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class Functor<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> {
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

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10) override
		{
			if (m_pFunc)
				(*static_cast<Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>* >(m_pFunc))(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 10 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class Functor<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> {
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

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9) override
		{
			if (m_pFunc)
				return (*static_cast<Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>* >(m_pFunc))(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9);
			else
				return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 10 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class Functor<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> {
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

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9) override
		{
			if (m_pFunc)
				(*static_cast<Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>* >(m_pFunc))(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9);
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 9 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class Functor<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> {
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

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<R, T0, T1, T2, T3, T4, T5, T6, T7, T8>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7, T8>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8) override
		{
			if (m_pFunc)
				return (*static_cast<Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8>* >(m_pFunc))(t0, t1, t2, t3, t4, t5, t6, t7, t8);
			else
				return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 9 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class Functor<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> {
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

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7, T8>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8) override
		{
			if (m_pFunc)
				(*static_cast<Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>* >(m_pFunc))(t0, t1, t2, t3, t4, t5, t6, t7, t8);
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 8 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class Functor<R, T0, T1, T2, T3, T4, T5, T6, T7> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7> {
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

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<R, T0, T1, T2, T3, T4, T5, T6, T7>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<R, T0, T1, T2, T3, T4, T5, T6, T7>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<R, T0, T1, T2, T3, T4, T5, T6, T7>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6, T7>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7) override
		{
			if (m_pFunc)
				return (*static_cast<Func<R, T0, T1, T2, T3, T4, T5, T6, T7>* >(m_pFunc))(t0, t1, t2, t3, t4, t5, t6, t7);
			else
				return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 8 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class Functor<void, T0, T1, T2, T3, T4, T5, T6, T7> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<void, T0, T1, T2, T3, T4, T5, T6, T7>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6, T7>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6, T7>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7) override
		{
			if (m_pFunc)
				(*static_cast<Func<void, T0, T1, T2, T3, T4, T5, T6, T7>* >(m_pFunc))(t0, t1, t2, t3, t4, t5, t6, t7);
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 7 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class Functor<R, T0, T1, T2, T3, T4, T5, T6> : public Func<R, T0, T1, T2, T3, T4, T5, T6> {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<R, T0, T1, T2, T3, T4, T5, T6>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<R, T0, T1, T2, T3, T4, T5, T6>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, R, T0, T1, T2, T3, T4, T5, T6>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<R, T0, T1, T2, T3, T4, T5, T6>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5, T6>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6) override
		{
			if (m_pFunc)
				return (*static_cast<Func<R, T0, T1, T2, T3, T4, T5, T6>* >(m_pFunc))(t0, t1, t2, t3, t4, t5, t6);
			else
				return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 7 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class Functor<void, T0, T1, T2, T3, T4, T5, T6> : public Func<void, T0, T1, T2, T3, T4, T5, T6> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<void, T0, T1, T2, T3, T4, T5, T6>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<void, T0, T1, T2, T3, T4, T5, T6>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, T0, T1, T2, T3, T4, T5, T6>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5, T6>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5, T6>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6) override
		{
			if (m_pFunc)
				(*static_cast<Func<void, T0, T1, T2, T3, T4, T5, T6>* >(m_pFunc))(t0, t1, t2, t3, t4, t5, t6);
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 6 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class Functor<R, T0, T1, T2, T3, T4, T5> : public Func<R, T0, T1, T2, T3, T4, T5> {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<R, T0, T1, T2, T3, T4, T5>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<R, T0, T1, T2, T3, T4, T5>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, R, T0, T1, T2, T3, T4, T5>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<R, T0, T1, T2, T3, T4, T5>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, R, T0, T1, T2, T3, T4, T5>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5) override
		{
			if (m_pFunc)
				return (*static_cast<Func<R, T0, T1, T2, T3, T4, T5>* >(m_pFunc))(t0, t1, t2, t3, t4, t5);
			else
				return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 6 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class Functor<void, T0, T1, T2, T3, T4, T5> : public Func<void, T0, T1, T2, T3, T4, T5> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<void, T0, T1, T2, T3, T4, T5>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<void, T0, T1, T2, T3, T4, T5>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, T0, T1, T2, T3, T4, T5>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<void, T0, T1, T2, T3, T4, T5>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, void, T0, T1, T2, T3, T4, T5>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5) override
		{
			if (m_pFunc)
				(*static_cast<Func<void, T0, T1, T2, T3, T4, T5>* >(m_pFunc))(t0, t1, t2, t3, t4, t5);
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 5 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4>
class Functor<R, T0, T1, T2, T3, T4> : public Func<R, T0, T1, T2, T3, T4> {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<R, T0, T1, T2, T3, T4>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<R, T0, T1, T2, T3, T4>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, R, T0, T1, T2, T3, T4>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, R, T0, T1, T2, T3, T4>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<R, T0, T1, T2, T3, T4>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, R, T0, T1, T2, T3, T4>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4) override
		{
			if (m_pFunc)
				return (*static_cast<Func<R, T0, T1, T2, T3, T4>* >(m_pFunc))(t0, t1, t2, t3, t4);
			else
				return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 5 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4>
class Functor<void, T0, T1, T2, T3, T4> : public Func<void, T0, T1, T2, T3, T4> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<void, T0, T1, T2, T3, T4>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<void, T0, T1, T2, T3, T4>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, T0, T1, T2, T3, T4>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, void, T0, T1, T2, T3, T4>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<void, T0, T1, T2, T3, T4>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, void, T0, T1, T2, T3, T4>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4) override
		{
			if (m_pFunc)
				(*static_cast<Func<void, T0, T1, T2, T3, T4>* >(m_pFunc))(t0, t1, t2, t3, t4);
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 4 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3>
class Functor<R, T0, T1, T2, T3> : public Func<R, T0, T1, T2, T3> {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<R, T0, T1, T2, T3>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<R, T0, T1, T2, T3>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, R, T0, T1, T2, T3>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, R, T0, T1, T2, T3>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<R, T0, T1, T2, T3>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, R, T0, T1, T2, T3>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3) override
		{
			if (m_pFunc)
				return (*static_cast<Func<R, T0, T1, T2, T3>* >(m_pFunc))(t0, t1, t2, t3);
			else
				return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 4 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3>
class Functor<void, T0, T1, T2, T3> : public Func<void, T0, T1, T2, T3> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<void, T0, T1, T2, T3>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<void, T0, T1, T2, T3>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, T0, T1, T2, T3>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, void, T0, T1, T2, T3>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<void, T0, T1, T2, T3>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, void, T0, T1, T2, T3>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3) override
		{
			if (m_pFunc)
				(*static_cast<Func<void, T0, T1, T2, T3>* >(m_pFunc))(t0, t1, t2, t3);
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 3 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2>
class Functor<R, T0, T1, T2> : public Func<R, T0, T1, T2> {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<R, T0, T1, T2>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<R, T0, T1, T2>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, R, T0, T1, T2>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, R, T0, T1, T2>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<R, T0, T1, T2>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, R, T0, T1, T2>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2) override
		{
			if (m_pFunc)
				return (*static_cast<Func<R, T0, T1, T2>* >(m_pFunc))(t0, t1, t2);
			else
				return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 3 parameters without a return value
*/
template <typename T0, typename T1, typename T2>
class Functor<void, T0, T1, T2> : public Func<void, T0, T1, T2> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<void, T0, T1, T2>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<void, T0, T1, T2>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, T0, T1, T2>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, void, T0, T1, T2>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<void, T0, T1, T2>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, void, T0, T1, T2>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2) override
		{
			if (m_pFunc)
				(*static_cast<Func<void, T0, T1, T2>* >(m_pFunc))(t0, t1, t2);
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 2 parameters and a return value
*/
template <typename R, typename T0, typename T1>
class Functor<R, T0, T1> : public Func<R, T0, T1> {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<R, T0, T1>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<R, T0, T1>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, R, T0, T1>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, R, T0, T1>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<R, T0, T1>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, R, T0, T1>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual _R operator ()(_T0 t0, _T1 t1) override
		{
			if (m_pFunc)
				return (*static_cast<Func<R, T0, T1>* >(m_pFunc))(t0, t1);
			else
				return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 2 parameters without a return value
*/
template <typename T0, typename T1>
class Functor<void, T0, T1> : public Func<void, T0, T1> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<void, T0, T1>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<void, T0, T1>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, T0, T1>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, void, T0, T1>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<void, T0, T1>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, void, T0, T1>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual void operator ()(_T0 t0, _T1 t1) override
		{
			if (m_pFunc)
				(*static_cast<Func<void, T0, T1>* >(m_pFunc))(t0, t1);
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 1 parameters and a return value
*/
template <typename R, typename T0>
class Functor<R, T0> : public Func<R, T0> {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<R, T0>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<R, T0>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, R, T0>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, R, T0>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<R, T0>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, R, T0>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual _R operator ()(_T0 t0) override
		{
			if (m_pFunc)
				return (*static_cast<Func<R, T0>* >(m_pFunc))(t0);
			else
				return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 1 parameters without a return value
*/
template <typename T0>
class Functor<void, T0> : public Func<void, T0> {
	public:
		typedef typename Type<T0>::_Type _T0;

	public:
		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<void, T0>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<void, T0>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, T0>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, void, T0>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<void, T0>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, void, T0>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual void operator ()(_T0 t0) override
		{
			if (m_pFunc)
				(*static_cast<Func<void, T0>* >(m_pFunc))(t0);
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 0 parameters and a return value
*/
template <typename R>
class Functor<R> : public Func<R> {
	public:
		typedef typename Type<R>  ::_Type _R;

		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const typename Signature<R>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<R>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, R>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, R>(pMemFunc, pObject))
		{
		}

		Functor(const typename Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<R>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, R>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual _R operator ()() override
		{
			if (m_pFunc)
				return (*static_cast<Func<R>* >(m_pFunc))();
			else
				return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};

/**
*  @brief
*    Generic functor class (delegate, a form of type-safe function pointer, 'callback')
*
*  @remarks
*    Implementation for 0 parameters without a return value
*/
template <>
class Functor<void> : public Func<void> {
	public:
		Functor() : m_pFunc(nullptr)
		{
		}

		Functor(DynFunc *pFunc) : m_pFunc(pFunc)
		{
		}

		Functor(const Signature<void>::FuncType &pFunc) :
			m_pFunc(new FuncFunPtr<void>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncMemPtr<CLASS, void>(pMemFunc, pObject))
		{
		}

		Functor(const Signature<void, PLCore::DynParams&, void*>::FuncType &pFunc) :
			m_pFunc(new FuncGenFunPtr<void>(pFunc))
		{
		}

		template <class CLASS>
		Functor(const typename MethodSignature<CLASS, void, PLCore::DynParams&, void*>::MemFuncType &pMemFunc, CLASS *pObject) :
			m_pFunc(new FuncGenMemPtr<CLASS, void>(pMemFunc, pObject))
		{
		}

		Functor(const Functor &cFunctor) : m_pFunc(nullptr)
		{
			// Check wrapped function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}
		}

		virtual ~Functor()
		{
			// Destroy wrapped function object
			if (m_pFunc)
				delete m_pFunc;
		}

		Functor &operator =(const Functor &cFunctor)
		{
			// Check if the functor already wraps a function object
			if (m_pFunc) {
				// Destroy old implementation
				delete m_pFunc;
				m_pFunc = nullptr;
			}

			// Check function object
			if (cFunctor.m_pFunc) {
				// Clone and copy wrapped function object
				m_pFunc = cFunctor.m_pFunc->Clone();
			}

			// Return this object
			return *this;
		}

		virtual void operator ()() override
		{
			if (m_pFunc)
				(*static_cast<Func<void>* >(m_pFunc))();
		}

		virtual DynFunc *Clone() const override
		{
			return new Functor(m_pFunc ? m_pFunc->Clone() : nullptr);
		}

	private:
		DynFunc *m_pFunc;	/**< Pointer to wrapped function object, can be a null pointer */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_FUNCTOR_H__
