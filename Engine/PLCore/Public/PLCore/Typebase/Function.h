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
template <typename T> class Function;

template <typename TRet, class T, typename... TArgs>
class Function<TRet (T::*)(TArgs...)> : public Invokable<TRet, T*, TArgs...> {

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
		Function(TRet(T::*TMethod)(TArgs...));

	//[-------------------------------------------------------]
	//[ Public virtual Invokable functions                    ]
	//[-------------------------------------------------------]
	public:
		virtual TRet Invoke(T *pInst, TArgs... args) const override;

	//[-------------------------------------------------------]
	//[ Private typedefs                                      ]
	//[-------------------------------------------------------]
	private:
		typedef TRet(T::*FnType)(TArgs...);

	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		FnType m_cFunc;		/**< Stored member function pointer */
};

template <typename TRet, class T, typename... TArgs>
class Function<TRet(T::*)(TArgs...) const> : public Invokable<TRet, const T*, TArgs...>{

	//[-------------------------------------------------------]
	//[ Public typedefs                                       ]
	//[-------------------------------------------------------]
public:
	typedef Function<TRet(T::*)(TArgs...) const> MyType;

	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
public:
	/**
	*  @brief
	*    Ctor
	*/
	Function(TRet(T::*TMethod)(TArgs...) const);

	//[-------------------------------------------------------]
	//[ Public virtual Invokable functions                    ]
	//[-------------------------------------------------------]
public:
	virtual TRet Invoke(const T *pInst, TArgs... args) const override;

	//[-------------------------------------------------------]
	//[ Private typedefs                                      ]
	//[-------------------------------------------------------]
private:
	typedef TRet(T::*FnType)(TArgs...) const;

	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
private:
	FnType m_cFunc;		/**< Stored member function pointer */
};

//struct Field;
//
//template <class T, typename TField>
//class Function<TField T::*, Field> : public Invokable<void, T*, TField**> {
//
//	//[-------------------------------------------------------]
//	//[ Public functions                                      ]
//	//[-------------------------------------------------------]
//public:
//	/**
//	*  @brief
//	*    Ctor
//	*/
//	Function(TField (T::*cField));
//
//	//[-------------------------------------------------------]
//	//[ Public virtual Invokable functions                    ]
//	//[-------------------------------------------------------]
//public:
//	virtual void Invoke(T *pInst, TField **pValue) const override;
//
//	//[-------------------------------------------------------]
//	//[ Private types                                         ]
//	//[-------------------------------------------------------]
//private:
//	typedef TField(T::*_FieldType);
//
//	//[-------------------------------------------------------]
//	//[ Private data                                          ]
//	//[-------------------------------------------------------]
//private:
//	_FieldType m_cField;		/**< Stored member field pointer */
//};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "Function.inl"


#endif // __PLCORE_FUNCTION_H__
