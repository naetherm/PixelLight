/*********************************************************\
 *  File: BinaryHeapIterator.inl                         *
 *      Binary heap iterator template implementation
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Container/BinaryHeap.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
template <class KeyType, class ValueType, class Comparer>
BinaryHeapIterator<KeyType, ValueType, Comparer>::BinaryHeapIterator(const BinaryHeap<KeyType, ValueType, Comparer> &cHeapOwner, uint32 nIndex) :
	m_pHeapOwner(&cHeapOwner),
	m_nNextID((nIndex > m_pHeapOwner->GetNumOfElements()-1) ? m_pHeapOwner->GetNumOfElements()-1 : nIndex)
{
}

/**
*  @brief
*    Constructor
*/
template <class KeyType, class ValueType, class Comparer>
BinaryHeapIterator<KeyType, ValueType, Comparer>::BinaryHeapIterator(const BinaryHeap<KeyType, ValueType, Comparer> &cHeapOwner) :
	m_pHeapOwner(&cHeapOwner),
	m_nNextID(cHeapOwner.GetNumOfElements())
{
}

/**
*  @brief
*    Copy constructor
*/
template <class KeyType, class ValueType, class Comparer>
BinaryHeapIterator<KeyType, ValueType, Comparer>::BinaryHeapIterator(const BinaryHeapIterator<KeyType, ValueType, Comparer> &cSource) :
	m_pHeapOwner(cSource.m_pHeapOwner),
	m_nNextID(cSource.m_nNextID)
{
}

/**
*  @brief
*    Destructor
*/
template <class KeyType, class ValueType, class Comparer>
BinaryHeapIterator<KeyType, ValueType, Comparer>::~BinaryHeapIterator()
{
}


//[-------------------------------------------------------]
//[ Private virtual IteratorImpl functions                ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer>
IteratorImpl<ValueType> *BinaryHeapIterator<KeyType, ValueType, Comparer>::Clone() const
{
	return new BinaryHeapIterator<KeyType, ValueType, Comparer>(*this);
}

template <class KeyType, class ValueType, class Comparer>
bool BinaryHeapIterator<KeyType, ValueType, Comparer>::HasNext() const
{
	return (m_nNextID < m_pHeapOwner->GetNumOfElements());
}

template <class KeyType, class ValueType, class Comparer>
ValueType &BinaryHeapIterator<KeyType, ValueType, Comparer>::Next()
{
	return (m_nNextID < m_pHeapOwner->GetNumOfElements()) ? m_pHeapOwner->m_pData[m_nNextID++].Value : BinaryHeap<KeyType, ValueType, Comparer>::Null;
}

template <class KeyType, class ValueType, class Comparer>
bool BinaryHeapIterator<KeyType, ValueType, Comparer>::HasPrevious() const
{
	return (m_nNextID != 0);
}

template <class KeyType, class ValueType, class Comparer>
ValueType &BinaryHeapIterator<KeyType, ValueType, Comparer>::Previous()
{
	return m_nNextID ? m_pHeapOwner->m_pData[--m_nNextID].Value : BinaryHeap<KeyType, ValueType, Comparer>::Null;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
