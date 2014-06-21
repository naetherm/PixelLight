/*********************************************************\
 *  File: BinaryHeapIterator.h                           *
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


#ifndef __PLCORE_CONTAINER_BINARYHEAPITERATOR_H__
#define __PLCORE_CONTAINER_BINARYHEAPITERATOR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Container/IteratorImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer> class BinaryHeap;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Binary heap iterator class
*/
template <class KeyType, class ValueType, class Comparer>
class BinaryHeapIterator : public IteratorImpl<ValueType> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class BinaryHeap<KeyType, ValueType, Comparer>;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cHeapOwner
		*    Binary heap to operate on
		*  @param[in] nIndex
		*    Start index, if >= GetNumOfElements() the index is set to the last valid index
		*/
		BinaryHeapIterator(const BinaryHeap<KeyType, ValueType, Comparer> &cHeapOwner, uint32 nIndex);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cHeapOwner
		*    Binary heap to operate on
		*
		*  @note
		*    - The iterator will start at the last element
		*/
		BinaryHeapIterator(const BinaryHeap<KeyType, ValueType, Comparer> &cHeapOwner);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		BinaryHeapIterator(const BinaryHeapIterator<KeyType, ValueType, Comparer> &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BinaryHeapIterator();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const BinaryHeap<KeyType, ValueType, Comparer> *m_pHeapOwner;	/**< Binary heap to operate on (always valid!) */
		uint32			 								m_nNextID;		/**< ID of the next element */


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
#include "PLCore/Container/BinaryHeapIterator.inl"


#endif // __PLCORE_CONTAINER_BINARYHEAPITERATOR_H__
