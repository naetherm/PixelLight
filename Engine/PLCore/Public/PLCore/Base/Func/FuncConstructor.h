/*********************************************************\
 *  File: FuncConstructor.h                              *
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


#ifndef __PLCORE_FUNC_CONSTRUCTOR_H__
#define __PLCORE_FUNC_CONSTRUCTOR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Base/Func/Func.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for up to 16 parameters
*/
template <typename CLASS, typename T0 = NullType, typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType, typename T6 = NullType, typename T7 = NullType, typename T8 = NullType, typename T9 = NullType, typename T10 = NullType, typename T11 = NullType, typename T12 = NullType, typename T13 = NullType, typename T14 = NullType, typename T15 = NullType>
class FuncConstructor : public Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> {
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

		FuncConstructor() {
		}

		virtual ~FuncConstructor() {
		}

		Object *operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14, _T15 t15) {
			return new CLASS(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15);
		}

		virtual DynFunc *Clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 15 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class FuncConstructor<CLASS, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> {
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

		FuncConstructor() {
		}

		virtual ~FuncConstructor() {
		}

		Object *operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14) {
			return new CLASS(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14);
		}

		virtual DynFunc *Clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 14 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class FuncConstructor<CLASS, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> {
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

		FuncConstructor() {
		}

		virtual ~FuncConstructor() {
		}

		Object *operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13) {
			return new CLASS(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13);
		}

		virtual DynFunc *Clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 13 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class FuncConstructor<CLASS, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> {
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

		FuncConstructor() {
		}

		virtual ~FuncConstructor() {
		}

		Object *operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12) {
			return new CLASS(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12);
		}

		virtual DynFunc *Clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 12 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class FuncConstructor<CLASS, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> {
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

		FuncConstructor() {
		}

		virtual ~FuncConstructor() {
		}

		Object *operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11) {
			return new CLASS(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11);
		}

		virtual DynFunc *Clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 11 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class FuncConstructor<CLASS, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> {
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

		FuncConstructor() {
		}

		virtual ~FuncConstructor() {
		}

		Object *operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10) {
			return new CLASS(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);
		}

		virtual DynFunc *Clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 10 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class FuncConstructor<CLASS, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> {
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

		FuncConstructor() {
		}

		virtual ~FuncConstructor() {
		}

		Object *operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9) {
			return new CLASS(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9);
		}

		virtual DynFunc *Clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 9 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class FuncConstructor<CLASS, T0, T1, T2, T3, T4, T5, T6, T7, T8> : public Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7, T8> {
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

		FuncConstructor() {
		}

		virtual ~FuncConstructor() {
		}

		Object *operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8) {
			return new CLASS(t0, t1, t2, t3, t4, t5, t6, t7, t8);
		}

		virtual DynFunc *Clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 8 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class FuncConstructor<CLASS, T0, T1, T2, T3, T4, T5, T6, T7> : public Func<Object*, T0, T1, T2, T3, T4, T5, T6, T7> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;

		FuncConstructor() {
		}

		virtual ~FuncConstructor() {
		}

		Object *operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7) {
			return new CLASS(t0, t1, t2, t3, t4, t5, t6, t7);
		}

		virtual DynFunc *Clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 7 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class FuncConstructor<CLASS, T0, T1, T2, T3, T4, T5, T6> : public Func<Object*, T0, T1, T2, T3, T4, T5, T6> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;

		FuncConstructor() {
		}

		virtual ~FuncConstructor() {
		}

		Object *operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6) {
			return new CLASS(t0, t1, t2, t3, t4, t5, t6);
		}

		virtual DynFunc *Clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 6 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class FuncConstructor<CLASS, T0, T1, T2, T3, T4, T5> : public Func<Object*, T0, T1, T2, T3, T4, T5> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;

		FuncConstructor() {
		}

		virtual ~FuncConstructor() {
		}

		Object *operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5) {
			return new CLASS(t0, t1, t2, t3, t4, t5);
		}

		virtual DynFunc *Clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 5 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3, typename T4>
class FuncConstructor<CLASS, T0, T1, T2, T3, T4> : public Func<Object*, T0, T1, T2, T3, T4> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;

		FuncConstructor() {
		}

		virtual ~FuncConstructor() {
		}

		Object *operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4) {
			return new CLASS(t0, t1, t2, t3, t4);
		}

		virtual DynFunc *Clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 4 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2, typename T3>
class FuncConstructor<CLASS, T0, T1, T2, T3> : public Func<Object*, T0, T1, T2, T3> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;

		FuncConstructor() {
		}

		virtual ~FuncConstructor() {
		}

		Object *operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3) {
			return new CLASS(t0, t1, t2, t3);
		}

		virtual DynFunc *Clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 3 parameters
*/
template <typename CLASS, typename T0, typename T1, typename T2>
class FuncConstructor<CLASS, T0, T1, T2> : public Func<Object*, T0, T1, T2> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;

		FuncConstructor() {
		}

		virtual ~FuncConstructor() {
		}

		Object *operator ()(_T0 t0, _T1 t1, _T2 t2) {
			return new CLASS(t0, t1, t2);
		}

		virtual DynFunc *Clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 2 parameters
*/
template <typename CLASS, typename T0, typename T1>
class FuncConstructor<CLASS, T0, T1> : public Func<Object*, T0, T1> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;

		FuncConstructor() {
		}

		virtual ~FuncConstructor() {
		}

		Object *operator ()(_T0 t0, _T1 t1) {
			return new CLASS(t0, t1);
		}

		virtual DynFunc *Clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 1 parameters
*/
template <typename CLASS, typename T0>
class FuncConstructor<CLASS, T0> : public Func<Object*, T0> {
	public:
		typedef typename Type<T0> ::_Type _T0;

		FuncConstructor() {
		}

		virtual ~FuncConstructor() {
		}

		Object *operator ()(_T0 t0) {
			return new CLASS(t0);
		}

		virtual DynFunc *Clone() const override {
			return new FuncConstructor();
		}
};

/**
*  @brief
*    Functoid that creates an object
*
*  @remarks
*    Implementation for 0 parameters
*/
template <typename CLASS>
class FuncConstructor<CLASS> : public Func<Object*> {
	public:
		FuncConstructor() {
		}

		virtual ~FuncConstructor() {
		}

		Object *operator ()() {
			return new CLASS();
		}

		virtual DynFunc *Clone() const override {
			return new FuncConstructor();
		}
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_FUNC_CONSTRUCTOR_H__
