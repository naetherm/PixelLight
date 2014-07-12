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
		virtual TRet Invoke(TArgs... args) = 0;

		/**
		*  @brief
		*    Invoke the target function object
		*/
		TRet operator()(TArgs... args)
		{
			return Invoke(args...);
		}

		/**
		*  @brief
		*    Invoke the target function object using packed arguments
		*/
		TRet TupleInvoke(const TupleType &cTuple)
		{
			return TupleInvokeHelper(cTuple, MakeIndexSequence<sizeof...(TArgs)>());
		}

		/**
		*  @brief
		*    Invoke the target function object using packed arguments
		*/
		TRet operator()(const TupleType &cTuple)
		{
			return TupleInvoke(cTuple);
		}

		/**
		*  @brief
		*    Return a pointer to the object associated with the invocation target
		*
		*  @remarks
		*    The pointer returned should point to the object the invocation depends on. This
		*    might be the instance in case of member functions, for example. This can return null
		*    if there is no object associated with the invocation.
		*/
		virtual void *GetTargetObject() = 0;

	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Tuple invoke helper
		*/
		template <int... I>
		TRet TupleInvokeHelper(const TupleType &cTuple, IndexSequence<I...>)
		{
			return Invoke(TupleGet<I>(cTuple)...);
		}
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_INVOKABLE_H__
