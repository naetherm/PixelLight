/*********************************************************\
 *  File: FibonacciHeapIterator.h                        *
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


#ifndef __PLCORE_CONTAINER_FIBONACCIHEAPITERATOR_H__
#define __PLCORE_CONTAINER_FIBONACCIHEAPITERATOR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Container/Stack.h"
#include "PLCore/Container/IteratorImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer> class FibonacciHeap;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Fibonacci heap iterator class
*/
template <class KeyType, class ValueType, class Comparer>
class FibonacciHeapIterator : public IteratorImpl<ValueType> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class FibonacciHeap<KeyType, ValueType, Comparer>;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cHeapOwner
		*    Fibonacci heap to operate on
		*  @param[in] nIndex
		*    Start index, if >= GetNumOfElements() the index is set to the last valid index
		*/
		FibonacciHeapIterator(const FibonacciHeap<KeyType, ValueType, Comparer> &cHeapOwner, uint32 nIndex);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cHeapOwner
		*    Fibonacci heap to operate on
		*
		*  @note
		*    - The iterator will start at the last element
		*/
		FibonacciHeapIterator(const FibonacciHeap<KeyType, ValueType, Comparer> &cHeapOwner);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		FibonacciHeapIterator(const FibonacciHeapIterator<KeyType, ValueType, Comparer> &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FibonacciHeapIterator();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const FibonacciHeap<KeyType, ValueType, Comparer>				   *m_pHeapOwner;		/**< Fibonacci heap to operate on (always valid!) */
		Stack<typename FibonacciHeap<KeyType, ValueType, Comparer>::Tree*>  m_lstParent;		/**< Parent stack */
		typename FibonacciHeap<KeyType, ValueType, Comparer>::Tree		   *m_pNextTree;		/**< Next tree, can be a null pointer */
		typename FibonacciHeap<KeyType, ValueType, Comparer>::Tree		   *m_pPreviousTree;	/**< Previous tree, can be a null pointer */


	//[-------------------------------------------------------]
	//[ Private virtual IteratorImpl functions                ]
	//[-------------------------------------------------------]
	private:
		virtual IteratorImpl<ValueType> *Clone() const override;
		virtual bool HasNext() const override;
		virtual ValueType &Next() override;
		virtual bool HasPrevious() const override;
		virtual ValueType &Previous() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Container/FibonacciHeapIterator.inl"


#endif // __PLCORE_CONTAINER_FIBONACCIHEAPITERATOR_H__
