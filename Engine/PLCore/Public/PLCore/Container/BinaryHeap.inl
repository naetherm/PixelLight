/*********************************************************\
 *  File: BinaryHeap.inl                                 *
 *      Binary heap template implementation
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
#include "PLCore/Container/BinaryHeapIterator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public static variables                               ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer> ValueType BinaryHeap<KeyType, ValueType, Comparer>::Null;


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
template <class KeyType, class ValueType, class Comparer>
BinaryHeap<KeyType, ValueType, Comparer>::BinaryHeap(uint32 nResizeCount) :
	m_nMaxNumOfElements(0),
	m_nNumOfElements(0),
	m_nResizeCount(nResizeCount),
	m_pData(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
template <class KeyType, class ValueType, class Comparer>
BinaryHeap<KeyType, ValueType, Comparer>::~BinaryHeap()
{
	if (m_pData)
		delete [] m_pData;
}

/**
*  @brief
*    Returns the number of elements automatically added if the binary heap
*    size is to small
*/
template <class KeyType, class ValueType, class Comparer>
uint32 BinaryHeap<KeyType, ValueType, Comparer>::GetResizeCount() const
{
	return m_nResizeCount;
}

/**
*  @brief
*    Sets the number of elements automatically added if the binary heap
*    size is to small
*/
template <class KeyType, class ValueType, class Comparer>
bool BinaryHeap<KeyType, ValueType, Comparer>::SetResizeCount(uint32 nCount)
{
	m_nResizeCount = nCount;

	// Done
	return true;
}

/**
*  @brief
*    Resets the binary heap
*/
template <class KeyType, class ValueType, class Comparer>
void BinaryHeap<KeyType, ValueType, Comparer>::Reset()
{
	// Reset the elements counter
	m_nNumOfElements = 0;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
template <class KeyType, class ValueType, class Comparer>
BinaryHeap<KeyType, ValueType, Comparer>::BinaryHeap(const BinaryHeap<KeyType, ValueType, Comparer> &cSource) :
	m_nMaxNumOfElements(0),
	m_nNumOfElements(0),
	m_nResizeCount(0),
	m_pData(nullptr)
{
	// Not implemented
}

template <class KeyType, class ValueType, class Comparer>
Heap<KeyType, ValueType> &BinaryHeap<KeyType, ValueType, Comparer>::operator =(const BinaryHeap<KeyType, ValueType, Comparer> &cHeap)
{
	// Not implemented

	// Return this instance
	return *this;
}

/**
*  @brief
*    Returns the index of the parent
*/
template <class KeyType, class ValueType, class Comparer>
int BinaryHeap<KeyType, ValueType, Comparer>::GetParent(uint32 nIndex) const
{
	return static_cast<int>(nIndex-1)/2;
}

/**
*  @brief
*    Returns the index of the left child
*/
template <class KeyType, class ValueType, class Comparer>
uint32 BinaryHeap<KeyType, ValueType, Comparer>::GetLeft(uint32 nIndex) const
{
	return nIndex*2+1;
}

/**
*  @brief
*    Returns the index of the right child
*/
template <class KeyType, class ValueType, class Comparer>
uint32 BinaryHeap<KeyType, ValueType, Comparer>::GetRight(uint32 nIndex) const
{
	return nIndex*2+2;
}

/**
*  @brief
*    Shift up
*/
template <class KeyType, class ValueType, class Comparer>
uint32 BinaryHeap<KeyType, ValueType, Comparer>::UpHeap(uint32 nIndex)
{
	Element cTemp;

	// Up heap iterative
	int nParent = GetParent(nIndex);
	while (nIndex > 0 && Comparer::IsGreater(m_pData[nParent].Key, m_pData[nIndex].Key)) {
		// Exchange elements
		cTemp			 = m_pData[nIndex];
		m_pData[nIndex]  = m_pData[nParent];
		m_pData[nParent] = cTemp;

		// Update element indices
		nIndex  = nParent;
		nParent = GetParent(nIndex);
	}

	// Return the new index
	return nIndex;
}

/**
*  @brief
*    Shift down
*/
template <class KeyType, class ValueType, class Comparer>
void BinaryHeap<KeyType, ValueType, Comparer>::DownHeap(uint32 nIndex)
{
	nIndex++;
	int j = 2*nIndex;
	if (j <= static_cast<int>(m_nNumOfElements)) { // m_pData[nIndex-1] has left child m_pData[j-1]
		if (j+1 <= static_cast<int>(m_nNumOfElements)) { // m_pData[nIndex-1] has left child m_pData[j], too
			if (Comparer::IsGreater(m_pData[j-1].Key, m_pData[j].Key))
				j++; // m_pData[j] is less child of m_pData[nIndex-1]
		}
		if (Comparer::IsGreater(m_pData[nIndex-1].Key, m_pData[j-1].Key)) {
			// Exchange elements
			Element cTemp	  = m_pData[nIndex-1];
			m_pData[nIndex-1] = m_pData[j-1];
			m_pData[j-1]	  = cTemp;

			// Down heap recursive
			DownHeap(j-1);
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual Iterable functions                     ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer>
Iterator<ValueType> BinaryHeap<KeyType, ValueType, Comparer>::GetIterator(uint32 nIndex) const
{
	Iterator<ValueType> cIterator(*(new BinaryHeapIterator<KeyType, ValueType, Comparer>(*this, nIndex)));
	return cIterator;
}

template <class KeyType, class ValueType, class Comparer>
ConstIterator<ValueType> BinaryHeap<KeyType, ValueType, Comparer>::GetConstIterator(uint32 nIndex) const
{
	ConstIterator<ValueType> cIterator(*(new BinaryHeapIterator<KeyType, ValueType, Comparer>(*this, nIndex)));
	return cIterator;
}

template <class KeyType, class ValueType, class Comparer>
Iterator<ValueType> BinaryHeap<KeyType, ValueType, Comparer>::GetEndIterator() const
{
	Iterator<ValueType> cIterator(*(new BinaryHeapIterator<KeyType, ValueType, Comparer>(*this)));
	return cIterator;
}

template <class KeyType, class ValueType, class Comparer>
ConstIterator<ValueType> BinaryHeap<KeyType, ValueType, Comparer>::GetConstEndIterator() const
{
	ConstIterator<ValueType> cIterator(*(new BinaryHeapIterator<KeyType, ValueType, Comparer>(*this)));
	return cIterator;
}


//[-------------------------------------------------------]
//[ Public virtual Heap functions                         ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer>
void BinaryHeap<KeyType, ValueType, Comparer>::Clear()
{
	// Destroy the data
	if (m_pData) {
		delete [] m_pData;
		m_pData = nullptr;
	}

	// Update the number of elements counter
	m_nMaxNumOfElements = m_nNumOfElements = 0;
}

template <class KeyType, class ValueType, class Comparer>
bool BinaryHeap<KeyType, ValueType, Comparer>::IsEmpty() const
{
	return (m_nNumOfElements == 0);
}

template <class KeyType, class ValueType, class Comparer>
uint32 BinaryHeap<KeyType, ValueType, Comparer>::GetNumOfElements() const
{
	return m_nNumOfElements;
}

template <class KeyType, class ValueType, class Comparer>
bool BinaryHeap<KeyType, ValueType, Comparer>::Add(const KeyType &Key, const ValueType &Value)
{
	// Check if the element can be added
	if (m_nNumOfElements+1 >= m_nMaxNumOfElements) {
		// Enlarge heap
		m_nMaxNumOfElements += m_nResizeCount;

		// Create the new data
		Element *pNewData = new Element[m_nMaxNumOfElements];

		// If there is old data
		if (m_pData) {
			// Copy old data content
			const uint32 nNumOfElements = (m_nNumOfElements > m_nMaxNumOfElements) ? m_nMaxNumOfElements : m_nNumOfElements;
			for (uint32 i=0; i<nNumOfElements; i++)
				pNewData[i] = m_pData[i];

			// Destroy the old data
			delete [] m_pData;
		}

		// Update data pointer
		m_pData = pNewData;
	}

	// Add element at the end...
	const int nElement = m_nNumOfElements;
	m_pData[nElement].Key   = Key;
	m_pData[nElement].Value = Value;
	m_nNumOfElements++;

	// ... and trickle it up
	UpHeap(nElement);

	// Done
	return true;
}

template <class KeyType, class ValueType, class Comparer>
bool BinaryHeap<KeyType, ValueType, Comparer>::GetTop(ValueType *pValue, KeyType *pKey) const
{
	// Is there a top element?
	if (!m_nNumOfElements)
		return false; // Error!

	// Get top
	if (pValue)
		*pValue = m_pData[0].Value;
	if (pKey)
		*pKey = m_pData[0].Key;

	// Done
	return true;
}

template <class KeyType, class ValueType, class Comparer>
bool BinaryHeap<KeyType, ValueType, Comparer>::ExtractTop(ValueType *pValue, KeyType *pKey)
{
	// Is there a top element?
	if (!m_nNumOfElements)
		return false; // Error!

	// Get top
	if (pValue)
		*pValue = m_pData[0].Value;
	if (pKey)
		*pKey   = m_pData[0].Key;

	// Decrease the number of elements on the heap
	m_nNumOfElements--;

	// If there are still elements...
	if (m_nNumOfElements) {
		// Take the last element and set it on the top
		m_pData[0] = m_pData[m_nNumOfElements];

		// Down heap
		DownHeap(0);
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Private virtual Heap functions                        ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer>
Heap<KeyType, ValueType> &BinaryHeap<KeyType, ValueType, Comparer>::operator =(const Heap<KeyType, ValueType> &cHeap)
{
	// Not implemented

	// Return this instance
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
