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


#ifndef __PLCORE_FUNCTION_H__
#define __PLCORE_FUNCTION_H__
#pragma once

//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Invokable.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Fast delegate implementation suitable for storing member functions
*
*  @remarks
*    Note that this delegate class is heavily constrained as it can only
*    handle member functions at this moment. Its invocation is really fast
*    on the other hand so it is a perfect match for code that works solely 
*    with classes
*/
template <typename T, class U = void> class Function;

template <typename TRet, class T, typename... TArgs>
class Function<TRet (T::*)(TArgs...)> : public Invokable<TRet, TArgs...> {

	//[-------------------------------------------------------]
	//[ Public typedefs                                       ]
	//[-------------------------------------------------------]
	public:
		typedef Function<TRet(T::*)(TArgs...)> MyType;

	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Ctor
		*/
		Function(T *pObj, TRet(T::*TMethod)(TArgs...));

	//[-------------------------------------------------------]
	//[ Public virtual Invokable functions                    ]
	//[-------------------------------------------------------]
	public:
		virtual TRet Invoke(TArgs... args) override;
		virtual void *GetTargetObject() override;

	//[-------------------------------------------------------]
	//[ Private typedefs                                      ]
	//[-------------------------------------------------------]
	private:
		typedef TRet(T::*FnType)(TArgs...);

	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		T *m_pObject;		/**< The target of the invocation */
		FnType m_cFunc;		/**< The function to invoke */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "Function.inl"


#endif // __PLCORE_FUNCTION_H__
