/*********************************************************\
 *  File: BinominalHeap.inl                              *
 *      Binominal heap template implementation
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
#include "PLCore/Container/BinominalHeapIterator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public static variables                               ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer> ValueType BinominalHeap<KeyType, ValueType, Comparer>::Null;


//[-------------------------------------------------------]
//[ Tree implementation                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
template <class KeyType, class ValueType, class Comparer>
BinominalHeap<KeyType, ValueType, Comparer>::Tree::Tree(KeyType Key, ValueType Value) :
	m_nDegree(0),
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
BinominalHeap<KeyType, ValueType, Comparer>::Tree::~Tree()
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
typename BinominalHeap<KeyType, ValueType, Comparer>::Tree &BinominalHeap<KeyType, ValueType, Comparer>::Tree::Union(Tree &cTree)
{
	Tree *pThisTree;

// First: Merge the siblings of the two trees (left degree > right degree)
	// For the tree's we have to swap the data :(
	if (m_nDegree < cTree.m_nDegree) {
		// Swap
		const uint32 nDegree = m_nDegree;
		m_nDegree = cTree.m_nDegree;
		cTree.m_nDegree = nDegree;

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
	Tree *pOtherTree   = &cTree;
	Tree *pCurrentTree = this;
	while (pThisTree && pOtherTree) {
		if (pOtherTree->m_nDegree > pThisTree->m_nDegree) {
			pCurrentTree->m_pNextSibling = pOtherTree;
			pOtherTree = pOtherTree->m_pNextSibling;
		} else {
			pCurrentTree->m_pNextSibling = pThisTree;
			pThisTree = pThisTree->m_pNextSibling;
		}
		pCurrentTree = pCurrentTree->m_pNextSibling;
	}
	while (pThisTree) {
		pCurrentTree->m_pNextSibling = pThisTree;
		pThisTree = pThisTree->m_pNextSibling;
		pCurrentTree = pCurrentTree->m_pNextSibling;
	}
	while (pOtherTree) {
		pCurrentTree->m_pNextSibling = pOtherTree;
		pOtherTree = pOtherTree->m_pNextSibling;
		pCurrentTree = pCurrentTree->m_pNextSibling;
	}

// Second: Combine trees of same degree
	Tree *pRoot = this;
	pThisTree  = this;
	pOtherTree = pThisTree->m_pNextSibling;
	while (pOtherTree) {
		// Same degree?
		if (pThisTree->m_nDegree == pOtherTree->m_nDegree) {
			// Compare keys, the tree with the 'greater' key gets a child of the other one
			if (Comparer::IsGreater(pThisTree->m_Key, pOtherTree->m_Key)) {
				// 'pThisTree' will become a child of 'pOtherTree'
				pThisTree->m_pNextSibling = nullptr;
				if (pThisTree == pRoot)
					pRoot = pOtherTree;
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
				// 'pOtherTree' will become a child of 'pThisTree'
				pThisTree->m_pNextSibling = pOtherTree->m_pNextSibling;
				if (pOtherTree == pRoot)
					pRoot = pThisTree;
				pOtherTree->m_pNextSibling = nullptr;
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

	// Return root node
	return *pRoot;
}


//[-------------------------------------------------------]
//[ BinominalHeap implementation                          ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
template <class KeyType, class ValueType, class Comparer>
BinominalHeap<KeyType, ValueType, Comparer>::BinominalHeap() :
	m_nNumOfElements(0),
	m_pRoot(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
template <class KeyType, class ValueType, class Comparer>
BinominalHeap<KeyType, ValueType, Comparer>::~BinominalHeap()
{
	Clear();
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
template <class KeyType, class ValueType, class Comparer>
BinominalHeap<KeyType, ValueType, Comparer>::BinominalHeap(const BinominalHeap<KeyType, ValueType, Comparer> &cSource) :
	m_nNumOfElements(0),
	m_pRoot(nullptr)
{
	// Not implemented
}

template <class KeyType, class ValueType, class Comparer>
Heap<KeyType, ValueType> &BinominalHeap<KeyType, ValueType, Comparer>::operator =(const BinominalHeap<KeyType, ValueType, Comparer> &cHeap)
{
	// Not implemented

	// Return this instance
	return *this;
}


//[-------------------------------------------------------]
//[ Public virtual Iterable functions                     ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer>
Iterator<ValueType> BinominalHeap<KeyType, ValueType, Comparer>::GetIterator(uint32 nIndex) const
{
	Iterator<ValueType> cIterator(*(new BinominalHeapIterator<KeyType, ValueType, Comparer>(*this, nIndex)));
	return cIterator;
}

template <class KeyType, class ValueType, class Comparer>
ConstIterator<ValueType> BinominalHeap<KeyType, ValueType, Comparer>::GetConstIterator(uint32 nIndex) const
{
	ConstIterator<ValueType> cIterator(*(new BinominalHeapIterator<KeyType, ValueType, Comparer>(*this, nIndex)));
	return cIterator;
}

template <class KeyType, class ValueType, class Comparer>
Iterator<ValueType> BinominalHeap<KeyType, ValueType, Comparer>::GetEndIterator() const
{
	Iterator<ValueType> cIterator(*(new BinominalHeapIterator<KeyType, ValueType, Comparer>(*this)));
	return cIterator;
}

template <class KeyType, class ValueType, class Comparer>
ConstIterator<ValueType> BinominalHeap<KeyType, ValueType, Comparer>::GetConstEndIterator() const
{
	ConstIterator<ValueType> cIterator(*(new BinominalHeapIterator<KeyType, ValueType, Comparer>(*this)));
	return cIterator;
}


//[-------------------------------------------------------]
//[ Public virtual Heap functions                         ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer>
void BinominalHeap<KeyType, ValueType, Comparer>::Clear()
{
	// Destroy the root tree
	if (m_pRoot) {
		delete m_pRoot;
		m_pRoot			 = nullptr;
		m_nNumOfElements = 0;
	}
}

template <class KeyType, class ValueType, class Comparer>
bool BinominalHeap<KeyType, ValueType, Comparer>::IsEmpty() const
{
	return (m_nNumOfElements == 0);
}

template <class KeyType, class ValueType, class Comparer>
uint32 BinominalHeap<KeyType, ValueType, Comparer>::GetNumOfElements() const
{
	return m_nNumOfElements;
}

template <class KeyType, class ValueType, class Comparer>
bool BinominalHeap<KeyType, ValueType, Comparer>::Add(const KeyType &Key, const ValueType &Value)
{
	// Create a new binominal tree
	Tree *pTree = new Tree(Key, Value);

	// Union the new tree with the root tree
	m_pRoot = m_pRoot ? &m_pRoot->Union(*pTree) : pTree;

	// Increase the number of elements
	m_nNumOfElements++;

	// Done
	return true;
}

template <class KeyType, class ValueType, class Comparer>
bool BinominalHeap<KeyType, ValueType, Comparer>::GetTop(ValueType *pValue, KeyType *pKey) const
{
	// Are there any elements?
	if (!m_nNumOfElements)
		return false; // Error!

	// Either the root tree or one of it's siblings must be the top
	Tree *pTop = m_pRoot, *pNext = pTop->m_pNextSibling;
	while (pNext) {
		// Do we have found a new top tree?
		if (Comparer::IsGreater(pTop->m_Key, pNext->m_Key))
			pTop = pNext;

		// Next sibling, please
		pNext = pNext->m_pNextSibling;
	}

	// Return value and key of the top tree
	if (pValue)
		*pValue = pTop->m_Value;
	if (pKey)
		*pKey   = pTop->m_Key;

	// Done
	return true;
}

template <class KeyType, class ValueType, class Comparer>
bool BinominalHeap<KeyType, ValueType, Comparer>::ExtractTop(ValueType *pValue, KeyType *pKey)
{
	// Are there any elements?
	if (!m_nNumOfElements)
		return false; // Error!

// First: Find the top tree
	// Either the root tree or one of it's siblings must be the top
	Tree *pTop = m_pRoot, *pPreviousTop = nullptr, *pPrevious = m_pRoot, *pNext = pTop->m_pNextSibling;
	while (pNext) {
		// Do we have found a new top tree?
		if (Comparer::IsGreater(pTop->m_Key, pNext->m_Key)) {
			pPreviousTop = pPrevious;
			pTop         = pNext;
		}

		// Next sibling, please
		pPrevious = pNext;
		pNext     = pNext->m_pNextSibling;
	}

// Second: Return value and key of the top tree
	if (pValue)
		*pValue = pTop->m_Value;
	if (pKey)
		*pKey   = pTop->m_Key;

// Third: Remove the top tree from the sibling list
	// Update sibling connectivity
	if (pPreviousTop)
		pPreviousTop->m_pNextSibling = pTop->m_pNextSibling;
	else
		m_pRoot = pTop->m_pNextSibling;

// Fourth: Reverse the sibling sequence of the child
	pPrevious = pTop->m_pChild;
	if (pPrevious) {
		pNext = pPrevious->m_pNextSibling;
		pPrevious->m_pNextSibling = nullptr;
		while (pNext) {
			// Backup sibling
			Tree *pTemp = pNext->m_pNextSibling;

			// Update sibling connectivity
			pNext->m_pNextSibling = pPrevious;

			// Next sibling, please
			pPrevious = pNext;
			pNext     = pTemp;
		}

		// Fifth: Union the tree with the head tree
		// ... 'pPrevious' now holds our 'new' tree...
		m_pRoot = m_pRoot ? &m_pRoot->Union(*pPrevious) : pPrevious;
	}

	// Destroy the node of the top tree...
	pTop->m_pNextSibling = nullptr; // Do NOT delete the sibling
	pTop->m_pChild		 = nullptr; // Do NOT delete the child
	delete pTop;

// ... and finally, decrease the number of elements
	m_nNumOfElements--;

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Private virtual Heap functions                        ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer>
Heap<KeyType, ValueType> &BinominalHeap<KeyType, ValueType, Comparer>::operator =(const Heap<KeyType, ValueType> &cHeap)
{
	// Not implemented

	// Return this instance
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
