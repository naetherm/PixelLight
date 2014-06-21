/*********************************************************\
 *  File: Stack.h                                        *
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


#ifndef __PLCORE_CONTAINER_STACK_H__
#define __PLCORE_CONTAINER_STACK_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Container/Iterable.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
template <class ValueType> class StackIterator;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Stack template (FILO -> First In Last Out)
*
*  @remarks
*  @verbatim
*    Simple extendable stack using a linked list
*    The stack will hold copies of the type used in the stack,
*    so note that your classes should supply a copy (=) operator!
*
*    Usage example:
*    Stack<MyClass> cStack;  // Create stack
*    MyClass T, T1, T2, T3;  // Test class instances
*    cStack.Push(T1);        // Push a copy of T1 onto the stack
*    cStack.Push(T2);        // Push a copy of T2 onto the stack
*    cStack.Push(T3);        // Push a copy of T3 onto the stack
*    cStack.Pop(&T);         // Pop last element (T3)
*    cStack.Pop(&T);         // Pop last element (T2)
*    cStack.Pop(&T);         // Pop last element (T1)
*  @endverbatim
*/
template <class ValueType>
class Stack : public Iterable<ValueType> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class StackIterator<ValueType>;


	//[-------------------------------------------------------]
	//[ Public static variables                               ]
	//[-------------------------------------------------------]
	public:
		static ValueType Null;	/**< 'Null'-object, do NEVER EVER manipulate this object! */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		Stack();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source stack to copy from
		*/
		Stack(const Stack<ValueType> &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		~Stack();

		/**
		*  @brief
		*    Push an element onto the stack (enqueue)
		*
		*  @param[in] Element
		*    New stack element
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool Push(const ValueType &Element);

		/**
		*  @brief
		*    Pops the top element from the stack (dequeue)
		*
		*  @param[out] pElement
		*    If not a null pointer, this will receive the popped element
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - On error, pElement is NOT touched, so ensure that you take this case into account!
		*/
		bool Pop(ValueType *pElement = nullptr);

		/**
		*  @brief
		*    Returns the top element without removing it
		*
		*  @return
		*    Top stack element (newest element)
		*/
		ValueType Top() const;

		/**
		*  @brief
		*    Returns the bottom element without removing it
		*
		*  @return
		*    Bottom stack element (oldest element)
		*/
		ValueType Bottom() const;

		/**
		*  @brief
		*    Returns the number of elements on the stack
		*
		*  @return
		*    Number of stack elements
		*/
		uint32 GetNumOfElements() const;

		/**
		*  @brief
		*    Copies the data from another stack
		*
		*  @param[in] cSource
		*    Stack to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Stack<ValueType> &operator =(const Stack<ValueType> &cSource);

		/**
		*  @brief
		*    Clears the whole stack
		*/
		void Clear();


	//[-------------------------------------------------------]
	//[ Public virtual Iterable functions                     ]
	//[-------------------------------------------------------]
	public:
		virtual Iterator<ValueType> GetIterator(uint32 nIndex = 0) const override;
		virtual ConstIterator<ValueType> GetConstIterator(uint32 nIndex = 0) const override;
		virtual Iterator<ValueType> GetEndIterator() const override;
		virtual ConstIterator<ValueType> GetConstEndIterator() const override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Push an element to the back of the stack
		*
		*  @param[in] Element
		*    New stack element
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool PushBack(const ValueType &Element);


	//[-------------------------------------------------------]
	//[ Private structures                                    ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Internal stack element
		*/
		struct StackElement {
			StackElement *pNextElement;	/**< Pointer to the next element on the stack, can be a null pointer */
			ValueType	  Data;			/**< The stored data */
		};


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static ValueType temp;	/** Temp object */


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32		  m_nNumOfElements;	/**< Number of elements on the stack */
		StackElement *m_pTop;			/**< Pointer to the top element (newest element), can be a null pointer */
		StackElement *m_pBottom;		/**< Pointer to the bottom element (oldest element), can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Container/Stack.inl"


#endif // __PLCORE_CONTAINER_STACK_H__
