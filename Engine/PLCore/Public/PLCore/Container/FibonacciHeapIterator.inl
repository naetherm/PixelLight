/*********************************************************\
 *  File: FibonacciHeapIterator.inl                      *
 *      Fibonacci heap iterator template implementation
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
#include "PLCore/Container/FibonacciHeap.h"


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
FibonacciHeapIterator<KeyType, ValueType, Comparer>::FibonacciHeapIterator(const FibonacciHeap<KeyType, ValueType, Comparer> &cHeapOwner, uint32 nIndex) :
	m_pHeapOwner(&cHeapOwner)
{
	// Which search direction?
	if (nIndex <= m_pHeapOwner->m_nNumOfElements/2) { // 0 -> (NumOfElements-1)
		// Initialize data
		m_pNextTree     = m_pHeapOwner->m_pFirst;
		m_pPreviousTree = nullptr;

		// Go to the correct start index
		uint32 nCurrentIndex = 0;
		while (nCurrentIndex < nIndex && HasNext()) {
			Next();
			nCurrentIndex++;
		}
	} else { // (NumOfElements-1) -> 0
		// Initialize data
		m_pNextTree     = nullptr;
		m_pPreviousTree = m_pHeapOwner->m_pFirst;
		do {
			// Find last sibling
			while (m_pPreviousTree->m_pNextSibling)
				m_pPreviousTree = m_pPreviousTree->m_pNextSibling;

			// Is there a child?
			if (m_pPreviousTree->m_pChild) {
				m_lstParent.Push(m_pPreviousTree);
				m_pPreviousTree = m_pPreviousTree->m_pChild;
			} else {
				break;
			}
		} while (true);

		// Go to the correct start index
		uint32 nCurrentIndex = m_pHeapOwner->m_nNumOfElements-1;
		while (nCurrentIndex > nIndex && HasPrevious()) {
			Previous();
			nCurrentIndex--;
		}
	}
}

/**
*  @brief
*    Constructor
*/
template <class KeyType, class ValueType, class Comparer>
FibonacciHeapIterator<KeyType, ValueType, Comparer>::FibonacciHeapIterator(const FibonacciHeap<KeyType, ValueType, Comparer> &cHeapOwner) :
	m_pHeapOwner(&cHeapOwner),
	m_pNextTree(nullptr),
	m_pPreviousTree(m_pHeapOwner->m_pFirst)
{
	// Initialize data
	if (m_pPreviousTree) {
		do {
			// Find last sibling
			while (m_pPreviousTree->m_pNextSibling)
				m_pPreviousTree = m_pPreviousTree->m_pNextSibling;

			// Is there a child?
			if (m_pPreviousTree->m_pChild) {
				m_lstParent.Push(m_pPreviousTree);
				m_pPreviousTree = m_pPreviousTree->m_pChild;
			} else {
				break;
			}
		} while (true);
	}
}

/**
*  @brief
*    Copy constructor
*/
template <class KeyType, class ValueType, class Comparer>
FibonacciHeapIterator<KeyType, ValueType, Comparer>::FibonacciHeapIterator(const FibonacciHeapIterator<KeyType, ValueType, Comparer> &cSource) :
	m_pHeapOwner(cSource.m_pHeapOwner),
	m_lstParent(cSource.m_lstParent),
	m_pNextTree(cSource.m_pNextTree),
	m_pPreviousTree(cSource.m_pPreviousTree)
{
}

/**
*  @brief
*    Destructor
*/
template <class KeyType, class ValueType, class Comparer>
FibonacciHeapIterator<KeyType, ValueType, Comparer>::~FibonacciHeapIterator()
{
}


//[-------------------------------------------------------]
//[ Private virtual IteratorImpl functions                ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer>
IteratorImpl<ValueType> *FibonacciHeapIterator<KeyType, ValueType, Comparer>::Clone() const
{
	return new FibonacciHeapIterator<KeyType, ValueType, Comparer>(*this);
}

template <class KeyType, class ValueType, class Comparer>
bool FibonacciHeapIterator<KeyType, ValueType, Comparer>::HasNext() const
{
	return (m_pNextTree != nullptr);
}

template <class KeyType, class ValueType, class Comparer>
ValueType &FibonacciHeapIterator<KeyType, ValueType, Comparer>::Next()
{
	// Is there a next tree?
	if (!m_pNextTree)
		return FibonacciHeap<KeyType, ValueType, Comparer>::Null;

	// Backup value
	ValueType *pValue = &m_pNextTree->m_Value;

	// Set previous tree
	m_pPreviousTree = m_pNextTree;

	// Get next tree, first check whether there's a child
	if (m_pNextTree->m_pChild) {
		m_lstParent.Push(m_pNextTree);
		m_pNextTree = m_pNextTree->m_pChild;
	} else {
		// Get next sibling
		m_pNextTree = m_pNextTree->m_pNextSibling;
		while (!m_pNextTree) {
			// Go to parent
			if (!m_lstParent.GetNumOfElements())
				break; // Ok, no parent, no next sibling - this must be the end :)
			m_lstParent.Pop(&m_pNextTree);

			// Get next sibling
			m_pNextTree = m_pNextTree->m_pNextSibling;
		}
	}

	// Return value
	return pValue ? *pValue : FibonacciHeap<KeyType, ValueType, Comparer>::Null;
}

template <class KeyType, class ValueType, class Comparer>
bool FibonacciHeapIterator<KeyType, ValueType, Comparer>::HasPrevious() const
{
	return (m_pPreviousTree != nullptr);
}

template <class KeyType, class ValueType, class Comparer>
ValueType &FibonacciHeapIterator<KeyType, ValueType, Comparer>::Previous()
{
	// Is there a previous tree?
	if (!m_pPreviousTree)
		return FibonacciHeap<KeyType, ValueType, Comparer>::Null;

	// Backup value
	ValueType *pValue = &m_pPreviousTree->m_Value;

	// Set next tree
	m_pNextTree = m_pPreviousTree;

	// Get previous sibling
	m_pPreviousTree = m_pPreviousTree->m_pPreviousSibling;
	if (!m_pPreviousTree) {
		// Go to parent...
		if (!m_lstParent.GetNumOfElements()) {
			// Ok, no parent, no previous sibling - this must be the beginning :)
			m_pPreviousTree = nullptr;

			// Return the value
			return pValue ? *pValue : FibonacciHeap<KeyType, ValueType, Comparer>::Null;
		}
		m_lstParent.Pop(&m_pPreviousTree);

		// Return the value
		return pValue ? *pValue : FibonacciHeap<KeyType, ValueType, Comparer>::Null;
	}

	// Check whether there's a child
	while (m_pPreviousTree && m_pPreviousTree->m_pChild) {
		// Push current tree onto the stack
		m_lstParent.Push(m_pPreviousTree);

		// Change into child...
		m_pPreviousTree = m_pPreviousTree->m_pChild;

		// ...and take the last tree
		while (m_pPreviousTree->m_pNextSibling)
			m_pPreviousTree = m_pPreviousTree->m_pNextSibling;
	}

	// Return the value
	return pValue ? *pValue : FibonacciHeap<KeyType, ValueType, Comparer>::Null;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
