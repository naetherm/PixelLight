/*********************************************************\
 *  File: BinominalHeapIterator.h                        *
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


#ifndef __PLCORE_CONTAINER_BINOMINALHEAPITERATOR_H__
#define __PLCORE_CONTAINER_BINOMINALHEAPITERATOR_H__
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
template <class KeyType, class ValueType, class Comparer> class BinominalHeap;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Binominal heap iterator class
*/
template <class KeyType, class ValueType, class Comparer>
class BinominalHeapIterator : public IteratorImpl<ValueType> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class BinominalHeap<KeyType, ValueType, Comparer>;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cHeapOwner
		*    Binominal heap to operate on
		*  @param[in] nIndex
		*    Start index, if >= GetNumOfElements() the index is set to the last valid index
		*/
		BinominalHeapIterator(const BinominalHeap<KeyType, ValueType, Comparer> &cHeapOwner, uint32 nIndex);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cHeapOwner
		*    Binominal heap to operate on
		*
		*  @note
		*    - The iterator will start at the last element
		*/
		BinominalHeapIterator(const BinominalHeap<KeyType, ValueType, Comparer> &cHeapOwner);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		BinominalHeapIterator(const BinominalHeapIterator<KeyType, ValueType, Comparer> &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BinominalHeapIterator();

		/**
		*  @brief
		*    Returns the previous sibling of the given tree
		*
		*  @param[in] pTree
		*    Tree to return the previous sibling from, can be a null pointer
		*
		*  @return
		*    Previous sibling tree of the given one, a null pointer of there's no previous sibling tree
		*
		*  @note
		*    - Because the binominal heap provides no information about the previous sibling tree, we
		*      have to find out the previous sibling tree by yourself (not that performant :)
		*/
		typename BinominalHeap<KeyType, ValueType, Comparer>::Tree *GetPreviousSibling(typename BinominalHeap<KeyType, ValueType, Comparer>::Tree *pTree) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const BinominalHeap<KeyType, ValueType, Comparer>					*m_pHeapOwner;		/**< Binominal heap to operate on (always valid!) */
		Stack<typename BinominalHeap<KeyType, ValueType, Comparer>::Tree*>   m_lstParent;		/**< Parent stack */
		typename BinominalHeap<KeyType, ValueType, Comparer>::Tree			*m_pNextTree;		/**< Next tree, can be a null pointer */
		typename BinominalHeap<KeyType, ValueType, Comparer>::Tree			*m_pPreviousTree;	/**< Previous tree, can be a null pointer */


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
#include "PLCore/Container/BinominalHeapIterator.inl"


#endif // __PLCORE_CONTAINER_BINOMINALHEAPITERATOR_H__
