/*********************************************************\
 *  File: FuncScriptPtr.h                                *
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


#ifndef __PLCORE_FUNCSCRIPTPTR_H__
#define __PLCORE_FUNCSCRIPTPTR_H__
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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Script;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Script function pointer
*
*  @remarks
*    This is a functoid that calls a function of a script
*/
template <typename R, typename T0 = NullType, typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType, typename T6 = NullType, typename T7 = NullType, typename T8 = NullType, typename T9 = NullType, typename T10 = NullType, typename T11 = NullType, typename T12 = NullType, typename T13 = NullType, typename T14 = NullType, typename T15 = NullType>
class FuncScriptPtr : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> {
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

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14, _T15 t15) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->PushArgument(t4);
				m_pScript->PushArgument(t5);
				m_pScript->PushArgument(t6);
				m_pScript->PushArgument(t7);
				m_pScript->PushArgument(t8);
				m_pScript->PushArgument(t9);
				m_pScript->PushArgument(t10);
				m_pScript->PushArgument(t11);
				m_pScript->PushArgument(t12);
				m_pScript->PushArgument(t13);
				m_pScript->PushArgument(t14);
				m_pScript->PushArgument(t15);
				if (m_pScript->EndCall()) {
					_R r = DefaultValue<R>::Default();
					return (R)m_pScript->GetReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for up to 16 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class FuncScriptPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> {
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

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14, _T15 t15) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->PushArgument(t4);
				m_pScript->PushArgument(t5);
				m_pScript->PushArgument(t6);
				m_pScript->PushArgument(t7);
				m_pScript->PushArgument(t8);
				m_pScript->PushArgument(t9);
				m_pScript->PushArgument(t10);
				m_pScript->PushArgument(t11);
				m_pScript->PushArgument(t12);
				m_pScript->PushArgument(t13);
				m_pScript->PushArgument(t14);
				m_pScript->PushArgument(t15);
				m_pScript->EndCall();
			}
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 15 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class FuncScriptPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> {
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

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->PushArgument(t4);
				m_pScript->PushArgument(t5);
				m_pScript->PushArgument(t6);
				m_pScript->PushArgument(t7);
				m_pScript->PushArgument(t8);
				m_pScript->PushArgument(t9);
				m_pScript->PushArgument(t10);
				m_pScript->PushArgument(t11);
				m_pScript->PushArgument(t12);
				m_pScript->PushArgument(t13);
				m_pScript->PushArgument(t14);
				if (m_pScript->EndCall()) {
					_R r = DefaultValue<R>::Default();
					return (R)m_pScript->GetReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 15 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class FuncScriptPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> {
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

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13, _T14 t14) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->PushArgument(t4);
				m_pScript->PushArgument(t5);
				m_pScript->PushArgument(t6);
				m_pScript->PushArgument(t7);
				m_pScript->PushArgument(t8);
				m_pScript->PushArgument(t9);
				m_pScript->PushArgument(t10);
				m_pScript->PushArgument(t11);
				m_pScript->PushArgument(t12);
				m_pScript->PushArgument(t13);
				m_pScript->PushArgument(t14);
				m_pScript->EndCall();
			}
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 14 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class FuncScriptPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> {
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

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->PushArgument(t4);
				m_pScript->PushArgument(t5);
				m_pScript->PushArgument(t6);
				m_pScript->PushArgument(t7);
				m_pScript->PushArgument(t8);
				m_pScript->PushArgument(t9);
				m_pScript->PushArgument(t10);
				m_pScript->PushArgument(t11);
				m_pScript->PushArgument(t12);
				m_pScript->PushArgument(t13);
				if (m_pScript->EndCall()) {
					_R r = DefaultValue<R>::Default();
					return (R)m_pScript->GetReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 14 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class FuncScriptPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> {
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

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12, _T13 t13) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->PushArgument(t4);
				m_pScript->PushArgument(t5);
				m_pScript->PushArgument(t6);
				m_pScript->PushArgument(t7);
				m_pScript->PushArgument(t8);
				m_pScript->PushArgument(t9);
				m_pScript->PushArgument(t10);
				m_pScript->PushArgument(t11);
				m_pScript->PushArgument(t12);
				m_pScript->PushArgument(t13);
				m_pScript->EndCall();
			}
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 13 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class FuncScriptPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> {
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

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->PushArgument(t4);
				m_pScript->PushArgument(t5);
				m_pScript->PushArgument(t6);
				m_pScript->PushArgument(t7);
				m_pScript->PushArgument(t8);
				m_pScript->PushArgument(t9);
				m_pScript->PushArgument(t10);
				m_pScript->PushArgument(t11);
				m_pScript->PushArgument(t12);
				if (m_pScript->EndCall()) {
					_R r = DefaultValue<R>::Default();
					return (R)m_pScript->GetReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 13 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class FuncScriptPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> {
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

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11, _T12 t12) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->PushArgument(t4);
				m_pScript->PushArgument(t5);
				m_pScript->PushArgument(t6);
				m_pScript->PushArgument(t7);
				m_pScript->PushArgument(t8);
				m_pScript->PushArgument(t9);
				m_pScript->PushArgument(t10);
				m_pScript->PushArgument(t11);
				m_pScript->PushArgument(t12);
				m_pScript->EndCall();
			}
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 12 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class FuncScriptPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> {
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

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->PushArgument(t4);
				m_pScript->PushArgument(t5);
				m_pScript->PushArgument(t6);
				m_pScript->PushArgument(t7);
				m_pScript->PushArgument(t8);
				m_pScript->PushArgument(t9);
				m_pScript->PushArgument(t10);
				m_pScript->PushArgument(t11);
				if (m_pScript->EndCall()) {
					_R r = DefaultValue<R>::Default();
					return (R)m_pScript->GetReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 12 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class FuncScriptPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> {
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

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10, _T11 t11) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->PushArgument(t4);
				m_pScript->PushArgument(t5);
				m_pScript->PushArgument(t6);
				m_pScript->PushArgument(t7);
				m_pScript->PushArgument(t8);
				m_pScript->PushArgument(t9);
				m_pScript->PushArgument(t10);
				m_pScript->PushArgument(t11);
				m_pScript->EndCall();
			}
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 11 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class FuncScriptPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> {
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

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->PushArgument(t4);
				m_pScript->PushArgument(t5);
				m_pScript->PushArgument(t6);
				m_pScript->PushArgument(t7);
				m_pScript->PushArgument(t8);
				m_pScript->PushArgument(t9);
				m_pScript->PushArgument(t10);
				if (m_pScript->EndCall()) {
					_R r = DefaultValue<R>::Default();
					return (R)m_pScript->GetReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 11 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class FuncScriptPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> {
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

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9, _T10 t10) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->PushArgument(t4);
				m_pScript->PushArgument(t5);
				m_pScript->PushArgument(t6);
				m_pScript->PushArgument(t7);
				m_pScript->PushArgument(t8);
				m_pScript->PushArgument(t9);
				m_pScript->PushArgument(t10);
				m_pScript->EndCall();
			}
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 10 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class FuncScriptPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> {
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

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->PushArgument(t4);
				m_pScript->PushArgument(t5);
				m_pScript->PushArgument(t6);
				m_pScript->PushArgument(t7);
				m_pScript->PushArgument(t8);
				m_pScript->PushArgument(t9);
				if (m_pScript->EndCall()) {
					_R r = DefaultValue<R>::Default();
					return (R)m_pScript->GetReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 10 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class FuncScriptPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> {
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

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8, _T9 t9) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->PushArgument(t4);
				m_pScript->PushArgument(t5);
				m_pScript->PushArgument(t6);
				m_pScript->PushArgument(t7);
				m_pScript->PushArgument(t8);
				m_pScript->PushArgument(t9);
				m_pScript->EndCall();
			}
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 9 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class FuncScriptPtr<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8> {
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

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<R, T0, T1, T2, T3, T4, T5, T6, T7, T8>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->PushArgument(t4);
				m_pScript->PushArgument(t5);
				m_pScript->PushArgument(t6);
				m_pScript->PushArgument(t7);
				m_pScript->PushArgument(t8);
				if (m_pScript->EndCall()) {
					_R r = DefaultValue<R>::Default();
					return (R)m_pScript->GetReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 9 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class FuncScriptPtr<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8> {
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

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7, _T8 t8) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<void, T0, T1, T2, T3, T4, T5, T6, T7, T8>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->PushArgument(t4);
				m_pScript->PushArgument(t5);
				m_pScript->PushArgument(t6);
				m_pScript->PushArgument(t7);
				m_pScript->PushArgument(t8);
				m_pScript->EndCall();
			}
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 8 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class FuncScriptPtr<R, T0, T1, T2, T3, T4, T5, T6, T7> : public Func<R, T0, T1, T2, T3, T4, T5, T6, T7> {
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

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<R, T0, T1, T2, T3, T4, T5, T6, T7>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->PushArgument(t4);
				m_pScript->PushArgument(t5);
				m_pScript->PushArgument(t6);
				m_pScript->PushArgument(t7);
				if (m_pScript->EndCall()) {
					_R r = DefaultValue<R>::Default();
					return (R)m_pScript->GetReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 8 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class FuncScriptPtr<void, T0, T1, T2, T3, T4, T5, T6, T7> : public Func<void, T0, T1, T2, T3, T4, T5, T6, T7> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;
		typedef typename Type<T7> ::_Type _T7;

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6, _T7 t7) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<void, T0, T1, T2, T3, T4, T5, T6, T7>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->PushArgument(t4);
				m_pScript->PushArgument(t5);
				m_pScript->PushArgument(t6);
				m_pScript->PushArgument(t7);
				m_pScript->EndCall();
			}
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 7 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class FuncScriptPtr<R, T0, T1, T2, T3, T4, T5, T6> : public Func<R, T0, T1, T2, T3, T4, T5, T6> {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<R, T0, T1, T2, T3, T4, T5, T6>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->PushArgument(t4);
				m_pScript->PushArgument(t5);
				m_pScript->PushArgument(t6);
				if (m_pScript->EndCall()) {
					_R r = DefaultValue<R>::Default();
					return (R)m_pScript->GetReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 7 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class FuncScriptPtr<void, T0, T1, T2, T3, T4, T5, T6> : public Func<void, T0, T1, T2, T3, T4, T5, T6> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;
		typedef typename Type<T6> ::_Type _T6;

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5, _T6 t6) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<void, T0, T1, T2, T3, T4, T5, T6>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->PushArgument(t4);
				m_pScript->PushArgument(t5);
				m_pScript->PushArgument(t6);
				m_pScript->EndCall();
			}
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 6 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class FuncScriptPtr<R, T0, T1, T2, T3, T4, T5> : public Func<R, T0, T1, T2, T3, T4, T5> {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<R, T0, T1, T2, T3, T4, T5>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->PushArgument(t4);
				m_pScript->PushArgument(t5);
				if (m_pScript->EndCall()) {
					_R r = DefaultValue<R>::Default();
					return (R)m_pScript->GetReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 6 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class FuncScriptPtr<void, T0, T1, T2, T3, T4, T5> : public Func<void, T0, T1, T2, T3, T4, T5> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;
		typedef typename Type<T5> ::_Type _T5;

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4, _T5 t5) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<void, T0, T1, T2, T3, T4, T5>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->PushArgument(t4);
				m_pScript->PushArgument(t5);
				m_pScript->EndCall();
			}
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 5 parameters and a return value
*/
template < typename R, typename T0, typename T1, typename T2, typename T3, typename T4>
class FuncScriptPtr<R, T0, T1, T2, T3, T4> : public Func<R, T0, T1, T2, T3, T4> {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<R, T0, T1, T2, T3, T4>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->PushArgument(t4);
				if (m_pScript->EndCall()) {
					_R r = DefaultValue<R>::Default();
					return (R)m_pScript->GetReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 5 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3, typename T4>
class FuncScriptPtr<void, T0, T1, T2, T3, T4> : public Func<void, T0, T1, T2, T3, T4> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;
		typedef typename Type<T4> ::_Type _T4;

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3, _T4 t4) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<void, T0, T1, T2, T3, T4>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->PushArgument(t4);
				m_pScript->EndCall();
			}
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 4 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2, typename T3>
class FuncScriptPtr<R, T0, T1, T2, T3> : public Func<R, T0, T1, T2, T3> {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<R, T0, T1, T2, T3>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				if (m_pScript->EndCall()) {
					_R r = DefaultValue<R>::Default();
					return (R)m_pScript->GetReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 4 parameters without a return value
*/
template <typename T0, typename T1, typename T2, typename T3>
class FuncScriptPtr<void, T0, T1, T2, T3> : public Func<void, T0, T1, T2, T3> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;
		typedef typename Type<T3> ::_Type _T3;

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2, _T3 t3) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<void, T0, T1, T2, T3>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->PushArgument(t3);
				m_pScript->EndCall();
			}
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 3 parameters and a return value
*/
template <typename R, typename T0, typename T1, typename T2>
class FuncScriptPtr<R, T0, T1, T2> : public Func<R, T0, T1, T2> {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1, _T2 t2) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<R, T0, T1, T2>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				if (m_pScript->EndCall()) {
					_R r = DefaultValue<R>::Default();
					return (R)m_pScript->GetReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 3 parameters without a return value
*/
template <typename T0, typename T1, typename T2>
class FuncScriptPtr<void, T0, T1, T2> : public Func<void, T0, T1, T2> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;
		typedef typename Type<T2> ::_Type _T2;

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1, _T2 t2) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<void, T0, T1, T2>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->PushArgument(t2);
				m_pScript->EndCall();
			}
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 2 parameters and a return value
*/
template <typename R, typename T0, typename T1>
class FuncScriptPtr<R, T0, T1> : public Func<R, T0, T1> {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual _R operator ()(_T0 t0, _T1 t1) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<R, T0, T1>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				if (m_pScript->EndCall()) {
					_R r = DefaultValue<R>::Default();
					return (R)m_pScript->GetReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 2 parameters without a return value
*/
template <typename T0, typename T1>
class FuncScriptPtr<void, T0, T1> : public Func<void, T0, T1> {
	public:
		typedef typename Type<T0> ::_Type _T0;
		typedef typename Type<T1> ::_Type _T1;

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(_T0 t0, _T1 t1) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<void, T0, T1>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->PushArgument(t1);
				m_pScript->EndCall();
			}
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 1 parameters and a return value
*/
template <typename R, typename T0>
class FuncScriptPtr<R, T0> : public Func<R, T0> {
	public:
		typedef typename Type<R>  ::_Type _R;
		typedef typename Type<T0> ::_Type _T0;

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual _R operator ()(_T0 t0) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<R, T0>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				if (m_pScript->EndCall()) {
					_R r = DefaultValue<R>::Default();
					return (R)m_pScript->GetReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 1 parameters without a return value
*/
template <typename T0>
class FuncScriptPtr<void, T0> : public Func<void, T0> {
	public:
		typedef typename Type<T0> ::_Type _T0;

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()(_T0 t0) override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<void, T0>::GetSignature(), m_sNamespace)) {
				m_pScript->PushArgument(t0);
				m_pScript->EndCall();
			}
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 0 parameters and a return value
*/
template <typename R>
class FuncScriptPtr<R> : public Func<R> {
	public:
		typedef typename Type<R>  ::_Type _R;

	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual _R operator ()() override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<R>::GetSignature(), m_sNamespace)) {
				if (m_pScript->EndCall()) {
					_R r = DefaultValue<R>::Default();
					return (R)m_pScript->GetReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return DefaultValue<R>::Default();
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};

/**
*  @brief
*    Script function pointer
*
*  @remarks
*    Implementation for 0 parameters without a return value
*/
template <>
class FuncScriptPtr<void> : public Func<void> {
	public:
		FuncScriptPtr(Script *pScript, const String &sFunction) : m_pScript(pScript), m_sFunction(sFunction) {
		}

		FuncScriptPtr(Script *pScript, const String &sFunction, const String &sNamespace) : m_pScript(pScript), m_sFunction(sFunction), m_sNamespace(sNamespace) {
		}

		virtual ~FuncScriptPtr() {
		}

		virtual void operator ()() override {
			if (m_pScript && m_pScript->BeginCall(m_sFunction, Func<void>::GetSignature(), m_sNamespace))
				m_pScript->EndCall();
		}

		virtual DynFunc *Clone() const override {
			return new FuncScriptPtr(m_pScript, m_sFunction, m_sNamespace);
		}

	private:
		Script *m_pScript;		/**< Script instance the function is in, can be a null pointer */
		String  m_sFunction;	/**< Name of the script function to use */
		String  m_sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_FUNCSCRIPTPTR_H__
