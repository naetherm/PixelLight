/*********************************************************\
 *  File: FibonacciHeap.inl                              *
 *      Fibonacci heap template implementation
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
#include "PLCore/Tools/Wrapper.h"
#include "PLCore/Core/MemoryManager.h"
#include "PLCore/Container/FibonacciHeapIterator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public static variables                               ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer> ValueType FibonacciHeap<KeyType, ValueType, Comparer>::Null;


//[-------------------------------------------------------]
//[ Tree implementation                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
template <class KeyType, class ValueType, class Comparer>
FibonacciHeap<KeyType, ValueType, Comparer>::Tree::Tree(KeyType Key, ValueType Value) :
	m_nDegree(0),
	m_pPreviousSibling(nullptr),
	m_pNextSibling(nullptr),
	m_pChild(nullptr),
	m_Key(Key),
	m_Value(Value)
{
}

/**
*  @brief
*    Destructor
*/
template <class KeyType, class ValueType, class Comparer>
FibonacciHeap<KeyType, ValueType, Comparer>::Tree::~Tree()
{
	// Destroy child
	if (m_pChild) {
		delete m_pChild;
		m_pChild = nullptr;
	}

	// Destroy sibling
	if (m_pNextSibling) {
		delete m_pNextSibling;
		m_pNextSibling = nullptr;
	}
}

/**
*  @brief
*    Union
*/
template <class KeyType, class ValueType, class Comparer>
void FibonacciHeap<KeyType, ValueType, Comparer>::Tree::Union(Tree &cTree)
{
	Tree *pThisTree, *pCurrentTree;

// First: Merge the siblings of the two trees (left degree > right degree)
	// For the tree's we have to swap the data :(
	if (m_nDegree < cTree.m_nDegree) {
		// Swap
		uint32 nDegree = m_nDegree;
		m_nDegree = cTree.m_nDegree;
		cTree.m_nDegree = nDegree;

		pThisTree = m_pPreviousSibling;
		m_pPreviousSibling = cTree.m_pPreviousSibling;
		cTree.m_pPreviousSibling = pThisTree;

		pThisTree = m_pNextSibling;
		m_pNextSibling = cTree.m_pNextSibling;
		cTree.m_pNextSibling = pThisTree;

		pThisTree = m_pChild;
		m_pChild = cTree.m_pChild;
		cTree.m_pChild = pThisTree;

		KeyType Key = m_Key;
		m_Key = cTree.m_Key;
		cTree.m_Key = Key;

		ValueType Value = m_Value;
		m_Value = cTree.m_Value;
		cTree.m_Value = Value;
	}
	// Now merge all other trees
	pThisTree = m_pNextSibling;
	Tree *pOtherTree = &cTree;
	pCurrentTree = this;
	while (pThisTree && pOtherTree) {
		if (pOtherTree->m_nDegree > pThisTree->m_nDegree) {
			pCurrentTree->m_pNextSibling = pOtherTree;
			pOtherTree = pOtherTree->m_pNextSibling;
		} else {
			pCurrentTree->m_pNextSibling = pThisTree;
			pThisTree = pThisTree->m_pNextSibling;
		}
		if (pCurrentTree->m_pNextSibling)
			pCurrentTree->m_pNextSibling->m_pPreviousSibling = pCurrentTree;
		pCurrentTree = pCurrentTree->m_pNextSibling;
	}
	while (pThisTree) {
		pCurrentTree->m_pNextSibling = pThisTree;
		pThisTree = pThisTree->m_pNextSibling;
		if (pCurrentTree->m_pNextSibling)
			pCurrentTree->m_pNextSibling->m_pPreviousSibling = pCurrentTree;
		pCurrentTree = pCurrentTree->m_pNextSibling;
	}
	while (pOtherTree) {
		pCurrentTree->m_pNextSibling = pOtherTree;
		pOtherTree = pOtherTree->m_pNextSibling;
		if (pCurrentTree->m_pNextSibling)
			pCurrentTree->m_pNextSibling->m_pPreviousSibling = pCurrentTree;
		pCurrentTree = pCurrentTree->m_pNextSibling;
	}

// Second: Combine trees of same degree
	pThisTree  = this;
	pOtherTree = pThisTree->m_pNextSibling;
	while (pOtherTree) {
		// Same degree?
		if (pThisTree->m_nDegree == pOtherTree->m_nDegree) {
			// Compare keys, the tree with the 'greater' key gets a child of the other one
			if (Comparer::IsGreater(pThisTree->m_Key, pOtherTree->m_Key)) {
				// 'pThisTree' will become a child of 'pOtherTree' - update connectivity
				if (pThisTree->m_pPreviousSibling)
					pThisTree->m_pPreviousSibling->m_pNextSibling = pThisTree->m_pNextSibling;
				if (pThisTree->m_pNextSibling)
					pThisTree->m_pNextSibling->m_pPreviousSibling = pThisTree->m_pPreviousSibling;
				pThisTree->m_pPreviousSibling = nullptr;
				pThisTree->m_pNextSibling     = nullptr;
				// Union
				if (pOtherTree->m_pChild) {
					pOtherTree->m_pChild->Union(*pThisTree);
					pOtherTree->m_nDegree = pOtherTree->m_pChild->m_nDegree+1;
				} else {
					pOtherTree->m_nDegree++;
					pOtherTree->m_pChild = pThisTree;
				}
				// Next pair, please
				pThisTree  = pOtherTree;
				pOtherTree = pOtherTree->m_pNextSibling;
			} else {
				// 'pOtherTree' will become a child of 'pThisTree' - update connectivity
				if (pOtherTree->m_pPreviousSibling)
					pOtherTree->m_pPreviousSibling->m_pNextSibling = pOtherTree->m_pNextSibling;
				if (pOtherTree->m_pNextSibling)
					pOtherTree->m_pNextSibling->m_pPreviousSibling = pOtherTree->m_pPreviousSibling;
				pOtherTree->m_pPreviousSibling = nullptr;
				pOtherTree->m_pNextSibling     = nullptr;
				// Union
				if (pThisTree->m_pChild) {
					pThisTree->m_pChild->Union(*pOtherTree);
					pThisTree->m_nDegree = pThisTree->m_pChild->m_nDegree+1;
				} else {
					pThisTree->m_nDegree++;
					pThisTree->m_pChild = pOtherTree;
				}
				// Update the pair
				pOtherTree = pThisTree->m_pNextSibling;
			}
		} else {
			// Next pair, please
			pThisTree  = pOtherTree;
			pOtherTree = pOtherTree->m_pNextSibling;
		}
	}
}


//[-------------------------------------------------------]
//[ FibonacciHeap implementation                          ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
template <class KeyType, class ValueType, class Comparer>
FibonacciHeap<KeyType, ValueType, Comparer>::FibonacciHeap() :
	m_nNumOfElements(0),
	m_pFirst(nullptr),
	m_pTop(nullptr),
	m_nNumOfTrees(0),
	m_nMaxNumOfMarks(0),
	m_ppMark(0)
{
}

/**
*  @brief
*    Destructor
*/
template <class KeyType, class ValueType, class Comparer>
FibonacciHeap<KeyType, ValueType, Comparer>::~FibonacciHeap()
{
	Clear();
}

/**
*  @brief
*    Returns whether the Fibonacci heap is normalized or not
*/
template <class KeyType, class ValueType, class Comparer>
bool FibonacciHeap<KeyType, ValueType, Comparer>::IsNormalized()
{
	// Increase marked array if required
	if (m_nMaxNumOfMarks < m_nNumOfTrees) {
		m_nMaxNumOfMarks = m_nNumOfTrees;
		delete [] m_ppMark;
		m_ppMark = new Tree*[m_nMaxNumOfMarks];
	}

	// Initialize the marked array
	MemoryManager::Set(m_ppMark, 0, sizeof(Tree**)*m_nNumOfTrees);

	// Check whether the Fibonacci heap is normalized
	Tree *pTree = m_pFirst;
	while (pTree) {
		// Is there already a tree with this degree
		if (m_ppMark[pTree->m_nDegree])
			return false; // Heap is not normalized :(

		// Set mark
		m_ppMark[pTree->m_nDegree] = pTree;

		// Next tree, please
		pTree = pTree->m_pNextSibling;
	}

	// Perfect, the heap is normalized! :)
	return true;
}

/**
*  @brief
*    Consolidate
*/
template <class KeyType, class ValueType, class Comparer>
bool FibonacciHeap<KeyType, ValueType, Comparer>::Consolidate()
{
	// Are there enough trees?
	if (m_nNumOfTrees < 2)
		return false; // Nothing to do here :)

	// Get theoretical maximum degree
	const uint32 nMaxDegree = static_cast<uint32>(Wrapper::Ceil(Wrapper::Log(static_cast<float>(m_nNumOfElements))+1));

	// Increase marked array if required
	if (m_nMaxNumOfMarks < nMaxDegree) {
		m_nMaxNumOfMarks = nMaxDegree;
		delete [] m_ppMark;
		m_ppMark = new Tree*[nMaxDegree];
	}

	// Initialize the marked array
	MemoryManager::Set(m_ppMark, 0, sizeof(Tree*)*nMaxDegree);

	// For all trees
	Tree *pThisTree = m_pFirst;
	while (pThisTree) {
		// Get the degree of this tree
		uint32 nDegree = pThisTree->m_nDegree;

		// As long as this mark is already set, union the two trees with the same degree...
		Tree *pOtherTree = m_ppMark[nDegree];
		while (pOtherTree) {
			// Compare keys, the tree with the 'greater' key gets a child of the other one
			if (Comparer::IsGreater(pThisTree->m_Key, pOtherTree->m_Key)) {
				// THIS can never, never happen because at start, there can not be a set mark! :)
//				if (pThisTree == m_pFirst) m_pFirst = m_pFirst->m_pNextSibling;
				// 'pThisTree' will become a child of 'pOtherTree' - update connectivity
				if (pThisTree->m_pPreviousSibling)
					pThisTree->m_pPreviousSibling->m_pNextSibling = pThisTree->m_pNextSibling;
				if (pThisTree->m_pNextSibling)
					pThisTree->m_pNextSibling->m_pPreviousSibling = pThisTree->m_pPreviousSibling;
				pThisTree->m_pPreviousSibling = nullptr;
				pThisTree->m_pNextSibling     = nullptr;
				// Union
				if (pOtherTree->m_pChild) {
					pOtherTree->m_pChild->Union(*pThisTree);
					pOtherTree->m_nDegree = pOtherTree->m_pChild->m_nDegree+1;
				} else {
					pOtherTree->m_nDegree++;
					pOtherTree->m_pChild = pThisTree;
				}
				// Update the 'this tree' pointer
				pThisTree = pOtherTree;
			} else {
				// Update the first tree pointer if required
				if (pOtherTree == m_pFirst)
					m_pFirst = m_pFirst->m_pNextSibling;
				// 'pOtherTree' will become a child of 'pThisTree' - update connectivity
				if (pOtherTree->m_pPreviousSibling)
					pOtherTree->m_pPreviousSibling->m_pNextSibling = pOtherTree->m_pNextSibling;
				if (pOtherTree->m_pNextSibling)
					pOtherTree->m_pNextSibling->m_pPreviousSibling = pOtherTree->m_pPreviousSibling;
				pOtherTree->m_pPreviousSibling = nullptr;
				pOtherTree->m_pNextSibling     = nullptr;
				// Union
				if (pThisTree->m_pChild) {
					pThisTree->m_pChild->Union(*pOtherTree);
					pThisTree->m_nDegree = pThisTree->m_pChild->m_nDegree+1;
				} else {
					pThisTree->m_nDegree++;
					pThisTree->m_pChild = pOtherTree;
				}
			}

			// Reset mark
			m_ppMark[nDegree] = nullptr;

			// Increase the degree of the current tree
			nDegree++;

			// Decrease the number of trees
			m_nNumOfTrees--;

			// Next
			pOtherTree = m_ppMark[nDegree];
		}

		// Set mark
		m_ppMark[nDegree] = pThisTree;

		// Next tree, please
		pThisTree = pThisTree->m_pNextSibling;
	}

	// Get the top tree
	m_pTop = m_pFirst;
	if (m_pTop) {
		Tree *pTree = m_pFirst->m_pNextSibling;
		while (pTree) {
			// Do we now have a new top 'tree'?
			if (Comparer::IsGreater(m_pTop->m_Key, pTree->m_Key))
				m_pTop = pTree;

			// Next sibling, please
			pTree = pTree->m_pNextSibling;
		}
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
template <class KeyType, class ValueType, class Comparer>
FibonacciHeap<KeyType, ValueType, Comparer>::FibonacciHeap(const FibonacciHeap<KeyType, ValueType, Comparer> &cSource) :
	m_nNumOfElements(0),
	m_pFirst(nullptr),
	m_pTop(nullptr),
	m_nNumOfTrees(0),
	m_nMaxNumOfMarks(0),
	m_ppMark(0)
{
	// Not implemented
}

template <class KeyType, class ValueType, class Comparer>
Heap<KeyType, ValueType> &FibonacciHeap<KeyType, ValueType, Comparer>::operator =(const FibonacciHeap<KeyType, ValueType, Comparer> &cHeap)
{
	// Not implemented

	// Return this instance
	return *this;
}


//[-------------------------------------------------------]
//[ Public virtual Iterable functions                     ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer>
Iterator<ValueType> FibonacciHeap<KeyType, ValueType, Comparer>::GetIterator(uint32 nIndex) const
{
	Iterator<ValueType> cIterator(*(new FibonacciHeapIterator<KeyType, ValueType, Comparer>(*this, nIndex)));
	return cIterator;
}

template <class KeyType, class ValueType, class Comparer>
ConstIterator<ValueType> FibonacciHeap<KeyType, ValueType, Comparer>::GetConstIterator(uint32 nIndex) const
{
	ConstIterator<ValueType> cIterator(*(new FibonacciHeapIterator<KeyType, ValueType, Comparer>(*this, nIndex)));
	return cIterator;
}

template <class KeyType, class ValueType, class Comparer>
Iterator<ValueType> FibonacciHeap<KeyType, ValueType, Comparer>::GetEndIterator() const
{
	Iterator<ValueType> cIterator(*(new FibonacciHeapIterator<KeyType, ValueType, Comparer>(*this)));
	return cIterator;
}

template <class KeyType, class ValueType, class Comparer>
ConstIterator<ValueType> FibonacciHeap<KeyType, ValueType, Comparer>::GetConstEndIterator() const
{
	ConstIterator<ValueType> cIterator(*(new FibonacciHeapIterator<KeyType, ValueType, Comparer>(*this)));
	return cIterator;
}


//[-------------------------------------------------------]
//[ Public virtual Heap functions                         ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer>
void FibonacciHeap<KeyType, ValueType, Comparer>::Clear()
{
	// Destroy the first tree
	if (m_pFirst) {
		delete m_pFirst;
		m_pFirst		 = nullptr;
		m_nNumOfElements = 0;
	}

	// Destroy and reset the cleanup data
	m_nNumOfTrees = 0;
	if (m_ppMark) {
		delete [] m_ppMark;
		m_ppMark		 = nullptr;
		m_nMaxNumOfMarks = 0;
	}
}

template <class KeyType, class ValueType, class Comparer>
bool FibonacciHeap<KeyType, ValueType, Comparer>::IsEmpty() const
{
	return (m_nNumOfElements == 0);
}

template <class KeyType, class ValueType, class Comparer>
uint32 FibonacciHeap<KeyType, ValueType, Comparer>::GetNumOfElements() const
{
	return m_nNumOfElements;
}

template <class KeyType, class ValueType, class Comparer>
bool FibonacciHeap<KeyType, ValueType, Comparer>::Add(const KeyType &Key, const ValueType &Value)
{
	// Create a new Fibonacci tree
	Tree *pTree = new Tree(Key, Value);

	// Add the new tree to the list of trees
	if (m_pFirst) {
		// Link
		m_pFirst->m_pPreviousSibling = pTree;
		pTree->m_pNextSibling = m_pFirst;
		m_pFirst = pTree;

		// Do we now have a new top 'tree'?
		if (Comparer::IsGreater(m_pTop->m_Key, m_pFirst->m_Key))
			m_pTop = m_pFirst;
	} else {
		m_pFirst = m_pTop = pTree;
	}

	// Increase the number of elements...
	m_nNumOfElements++;

	// ... and the number of trees
	m_nNumOfTrees++;

	// Done
	return true;
}

template <class KeyType, class ValueType, class Comparer>
bool FibonacciHeap<KeyType, ValueType, Comparer>::GetTop(ValueType *pValue, KeyType *pKey) const
{
	// Are there any elements?
	if (!m_nNumOfElements)
		return false; // Error!

	// Return value and key of the top tree - yeah, that was pretty easy! :)
	if (pValue)
		*pValue = m_pTop->m_Value;
	if (pKey)
		*pKey   = m_pTop->m_Key;

	// Done
	return true;
}

template <class KeyType, class ValueType, class Comparer>
bool FibonacciHeap<KeyType, ValueType, Comparer>::ExtractTop(ValueType *pValue, KeyType *pKey)
{
	// Are there any elements?
	if (!m_nNumOfElements)
		return false; // Error!

// First: Return value and key of the top tree - yeah, that was pretty easy! :)
	if (pValue)
		*pValue = m_pTop->m_Value;
	if (pKey)
		*pKey   = m_pTop->m_Key;

// Second: Remove the top tree from the list and update connectivity
	if (m_pTop->m_pPreviousSibling)
		m_pTop->m_pPreviousSibling->m_pNextSibling = m_pTop->m_pNextSibling;
	if (m_pTop->m_pNextSibling)
		m_pTop->m_pNextSibling->m_pPreviousSibling = m_pTop->m_pPreviousSibling;
	if (m_pTop == m_pFirst)
		m_pFirst = m_pTop->m_pNextSibling;

// Third: Add the child (if there's one) of the top tree with all it's siblings to the list of trees
	m_nNumOfTrees--; // Top tree goes
	Tree *pTree = m_pTop->m_pChild;
	while (pTree) {
		// Backup the next tree
		Tree *pNextTree = pTree->m_pNextSibling;

		// Link the tree to the list of trees
		if (m_pFirst) {
			m_pFirst->m_pPreviousSibling = pTree;
			pTree->m_pPreviousSibling = nullptr;
			pTree->m_pNextSibling     = m_pFirst;
		} else {
			pTree->m_pPreviousSibling = nullptr;
			pTree->m_pNextSibling     = nullptr;
		}
		m_pFirst = pTree;
		m_nNumOfTrees++;

		// Next sibling, please
		pTree = pNextTree;
	}

// Fourth: Destroy the node of the top tree...
	m_pTop->m_pNextSibling = nullptr; // Do NOT delete the sibling
	m_pTop->m_pChild	   = nullptr; // Do NOT delete the child
	delete m_pTop;

// ... and decrease the number of elements
	m_nNumOfElements--;

// Fifth: Consolidate (the heart of the Fibonacci Heap :)
	if (!Consolidate())
		m_pTop = m_pFirst;

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Private virtual Heap functions                        ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer>
Heap<KeyType, ValueType> &FibonacciHeap<KeyType, ValueType, Comparer>::operator =(const Heap<KeyType, ValueType> &cHeap)
{
	// Not implemented

	// Return this instance
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
