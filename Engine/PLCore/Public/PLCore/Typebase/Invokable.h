/*********************************************************\
 *  File: PLCore.h                                       *
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


#ifndef __PLCORE_INVOKABLE_H__
#define __PLCORE_INVOKABLE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "FunctionBase.h"
#include "Tuple.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Represents a generic function object that can be invoked
*/
template <typename TRet, typename... TArgs>
class Invokable : public FunctionBase {

	//[-------------------------------------------------------]
	//[ Public types                                          ]
	//[-------------------------------------------------------]
	public:
		typedef Tuple<TArgs...> TupleType;

	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Invoke the target function object
		*/
		virtual TRet Invoke(TArgs... args) const = 0;

		/**
		*  @brief
		*    Invoke the target function object
		*/
		TRet operator()(TArgs... args) const
		{
			return Invoke(args...);
		}

		/**
		*  @brief
		*    Invoke the target function object using packed arguments
		*/
		TRet TupleInvoke(const TupleType &cTuple) const
		{
			return TupleInvokeHelper(cTuple, MakeIndexSequence<sizeof...(TArgs)>());
		}

		/**
		*  @brief
		*    Invoke the target function object using packed arguments
		*/
		TRet operator()(const TupleType &cTuple) const
		{
			return TupleInvoke(cTuple);
		}

	//[-------------------------------------------------------]
	//[ Private helpers                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Helper to invoke function using dynamic arguments and handle return value
		*/
		template <class TTuple, typename TRet>
		struct DynInvokeHelper {

			static void Call(DynamicObject &cRet, const Invokable *pToInvoke, const TTuple &tuple)
			{
				cRet.Set<TRet>(pToInvoke->TupleInvoke(tuple));
			}
		};

		template <class TTuple>
		struct DynInvokeHelper<TTuple, void> {
		
			static void Call(DynamicObject &cRet, const Invokable *pToInvoke, const TTuple &tuple)
			{
				pToInvoke->TupleInvoke(tuple);
			}
		};

	//[-------------------------------------------------------]
	//[ Public virtual FunctionBase functions                 ]
	//[-------------------------------------------------------]
	public:
		virtual DynamicObject DynInvoke(const Iterable<DynamicObject> *pParam) const override
		{
			// Initialize a tuple of the correct type from the dynamic argument list
			TupleType t;
			if (pParam)
				TupleFromDynamicObject(pParam, t);

			// Invoke the function using the tuple
			DynamicObject ret;
			DynInvokeHelper<TupleType, TRet>::Call(ret, this, t);
			return ret;
		}

	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Tuple invoke helper
		*/
		template <int... I>
		TRet TupleInvokeHelper(const TupleType &cTuple, IndexSequence<I...>) const
		{
			return Invoke(TupleGet<I>(cTuple)...);
		}
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_INVOKABLE_H__
