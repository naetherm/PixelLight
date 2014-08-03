/*********************************************************\
 *  File: Slot.h                                         *
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


#ifndef __PLCORE_SLOT_H__
#define __PLCORE_SLOT_H__
#pragma once

//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Typebase/Invokable.h"
#include "PLCore/Reflection/Rtti.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Event handler (slot) class
*/
template <typename... TArgs>
class Slot : public Invokable<void, void*, TArgs...> {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor, for use just in containters and such
		*/
		Slot() {}

		/**
		*  @brief
		*    Slot must be constructed from object pointer and function
		*/
		template <class T>
		Slot(T *pObj, Invokable<void, T*, TArgs...> *pFunc);

		/**
		*  @brief
		*    Call the slot function on the stored object
		*/
		void Signal(TArgs... args) const;

		/**
		*  @brief
		*    Check equality
		*/
		bool operator==(const Slot &cOther) const;


	//[-------------------------------------------------------]
	//[ Public virtual Invokable functions                    ]
	//[-------------------------------------------------------]
	public:
		virtual void Invoke(void *pObj, TArgs... args) const override;
		virtual FunctionSignature GetSignature() const override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Dummy typed invoker
		*/
		template <class T>
		static void Invoker(void *pObj, FunctionBase *pFunc, TArgs... args);


	//[-------------------------------------------------------]
	//[ Private types                                         ]
	//[-------------------------------------------------------]
	private:
		typedef void(*InvokerStub)(void*, FunctionBase*, TArgs...);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		void			*m_pObject;			/**< Untyped owner of this slot */
		FunctionBase	*m_pFunction;		/**< Untyped slot function */
		InvokerStub		m_pInvoker;			/**< Helper for typed invocation */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "Slot.inl"


#endif // __PLCORE_SLOT_H__
