/*********************************************************\
 *  File: FastPool.inl                                   *
 *      Fast pool template implementation
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
#include "PLCore/Container/FastPoolIterator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public static variables                               ]
//[-------------------------------------------------------]
template <class ValueType> ValueType FastPool<ValueType>::Null;


//[-------------------------------------------------------]
//[ FastPoolElement implementation                        ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
template <class ValueType>
FastPoolElement<ValueType>::FastPoolElement()
{
	// Nothing to do here
}

/**
*  @brief
*    Destructor
*/
template <class ValueType>
FastPoolElement<ValueType>::~FastPoolElement()
{
	// Nothing to do here
}

/**
*  @brief
*    Copy operator
*/
template <class ValueType>
FastPoolElement<ValueType> &FastPoolElement<ValueType>::operator =(const FastPoolElement<ValueType> &lstSource)
{
	// Nothing to do here
	return *this;
}


//[-------------------------------------------------------]
//[ FastPool implementation                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
template <class ValueType>
FastPool<ValueType>::FastPool() :
	m_nNumOfElements(0),
	m_pFirstElement(nullptr),
	m_pLastElement(nullptr),
	m_nNumOfFreeElements(0),
	m_pFirstFreeElement(nullptr)
{
}

/**
*  @brief
*    Copy constructor
*/
template <class ValueType>
FastPool<ValueType>::FastPool(const FastPool<ValueType> &lstSource, uint32 nStart, uint32 nCount) :
	m_nNumOfElements(0),
	m_pFirstElement(nullptr),
	m_pLastElement(nullptr),
	m_nNumOfFreeElements(0),
	m_pFirstFreeElement(nullptr)
{
	// Copy
	Copy(static_cast<const Container<ValueType>&>(lstSource), nStart, nCount);
}

/**
*  @brief
*    Copy constructor
*/
template <class ValueType>
FastPool<ValueType>::FastPool(const Container<ValueType> &lstSource, uint32 nStart, uint32 nCount) :
	m_nNumOfElements(0),
	m_pFirstElement(nullptr),
	m_pLastElement(nullptr),
	m_nNumOfFreeElements(0),
	m_pFirstFreeElement(nullptr)
{
	// Copy
	Copy(lstSource, nStart, nCount);
}

/**
*  @brief
*    Destructor
*/
template <class ValueType>
FastPool<ValueType>::~FastPool()
{
	Clear();
}

/**
*  @brief
*    Copy operator
*/
template <class ValueType>
Container<ValueType> &FastPool<ValueType>::operator =(const FastPool<ValueType> &lstSource)
{
	return *this = static_cast<const Container<ValueType>&>(lstSource);
}

/**
*  @brief
*    Returns the number of currently free pool elements
*/
template <class ValueType>
uint32 FastPool<ValueType>::GetNumOfFreeElements() const
{
	return m_nNumOfFreeElements;
}

/**
*  @brief
*    Returns the total size of all free container elements (in bytes)
*/
template <class ValueType>
uint32 FastPool<ValueType>::GetFreeSize() const
{
	return m_nNumOfFreeElements*sizeof(ValueType);
}

/**
*  @brief
*    Marks all elements as free
*/
template <class ValueType>
void FastPool<ValueType>::FreeElements()
{
	// Are there any elements to free?
	if (m_nNumOfElements) {
		// Setup the list of free elements
		m_nNumOfFreeElements += m_nNumOfElements;
		if (m_pFirstFreeElement) {
			m_pLastElement->m_pNextElement = m_pFirstFreeElement;
			m_pFirstFreeElement = m_pFirstElement;
		} else {
			m_pFirstFreeElement = m_pFirstElement;
		}

		// Clear the list of current used elements
		m_nNumOfElements = 0;
		m_pFirstElement  = nullptr;
		m_pLastElement   = nullptr;
	}
}

/**
*  @brief
*    Removes all currently free elements
*/
template <class ValueType>
void FastPool<ValueType>::RemoveAllFreeElements()
{
	// Clear all free pool elements
	ValueType *pElement = m_pFirstFreeElement;
	while (pElement) {
		ValueType *pNextElement = pElement->m_pNextElement;
		delete pElement;
		pElement = pNextElement;
	}

	// Init data
	m_nNumOfFreeElements = 0;
	m_pFirstFreeElement  = nullptr;
}

/**
*  @brief
*    Removes an element (added to the list of free elements)
*/
template <class ValueType>
void FastPool<ValueType>::RemoveElement(ValueType &cElement)
{
	// Update connectivity
	if (cElement.m_pPreviousElement)
		cElement.m_pPreviousElement->m_pNextElement = cElement.m_pNextElement;
	if (cElement.m_pNextElement)
		cElement.m_pNextElement->m_pPreviousElement = cElement.m_pPreviousElement;
	if (&cElement == m_pFirstElement)
		m_pFirstElement = cElement.m_pNextElement;
	if (&cElement == m_pLastElement)
		m_pLastElement = cElement.m_pPreviousElement;

	// Setting the previous element is not really required here :)
	// if (m_pFirstFreeElement) m_pFirstFreeElement->m_pPreviousElement = &cElement;
	cElement.m_pNextElement = m_pFirstFreeElement;
	m_pFirstFreeElement = &cElement;
	m_nNumOfFreeElements++;
	m_nNumOfElements--;
}

/**
*  @brief
*    Returns the index of an element
*/
template <class ValueType>
int FastPool<ValueType>::GetElementIndex(ValueType &cElement) const
{
	// Find the index of the given element
	ValueType *pCurrentElement = m_pFirstElement;
	uint32     nCurIndex       = 0;
	while (pCurrentElement) {
		if (pCurrentElement == &cElement)
			return nCurIndex;
		pCurrentElement = pCurrentElement->m_pNextElement;
		nCurIndex++;
	}

	// The given element is not in the pool!
	return -1;
}

/**
*  @brief
*    Adds a free element, if there's no free element a new one is created
*/
template <class ValueType>
ValueType &FastPool<ValueType>::AddElement()
{
	// Increase the number of used elements
	m_nNumOfElements++;

	// Is there at least one free element?
	if (m_pFirstFreeElement) {
		// Take the first free element
		ValueType *pNewElement = m_pFirstFreeElement;
		m_pFirstFreeElement = m_pFirstFreeElement->m_pNextElement;

		// Setting the previous element is not really required here :)
		// if (m_pFirstFreeElement) m_pFirstFreeElement->m_pPreviousElement = nullptr;

		// Decrease the number of free elements
		m_nNumOfFreeElements--;

		// Return the element
		return *pNewElement;

	// Create a new element because there's no free one
	} else {
		return *(new ValueType);
	}
}


//[-------------------------------------------------------]
//[ Public virtual Iterable functions                     ]
//[-------------------------------------------------------]
template <class ValueType>
Iterator<ValueType> FastPool<ValueType>::GetIterator(uint32 nIndex) const
{
	Iterator<ValueType> cIterator(*(new FastPoolIterator<ValueType>(*this, nIndex)));
	return cIterator;
}

template <class ValueType>
ConstIterator<ValueType> FastPool<ValueType>::GetConstIterator(uint32 nIndex) const
{
	ConstIterator<ValueType> cIterator(*(new FastPoolIterator<ValueType>(*this, nIndex)));
	return cIterator;
}

template <class ValueType>
Iterator<ValueType> FastPool<ValueType>::GetEndIterator() const
{
	Iterator<ValueType> cIterator(*(new FastPoolIterator<ValueType>(*this)));
	return cIterator;
}

template <class ValueType>
ConstIterator<ValueType> FastPool<ValueType>::GetConstEndIterator() const
{
	ConstIterator<ValueType> cIterator(*(new FastPoolIterator<ValueType>(*this)));
	return cIterator;
}


//[-------------------------------------------------------]
//[ Public virtual Container functions                    ]
//[-------------------------------------------------------]
template <class ValueType>
bool FastPool<ValueType>::IsEmpty() const
{
	return (m_nNumOfElements == 0);
}

template <class ValueType>
uint32 FastPool<ValueType>::GetNumOfElements() const
{
	return m_nNumOfElements;
}

template <class ValueType>
uint32 FastPool<ValueType>::GetElementSize() const
{
	return sizeof(ValueType);
}

template <class ValueType>
uint32 FastPool<ValueType>::GetSize() const
{
	return m_nNumOfElements*sizeof(ValueType);
}

template <class ValueType>
void FastPool<ValueType>::Clear()
{
	// Clear all pool elements
	ValueType *pElement = m_pFirstElement;
	while (pElement) {
		ValueType *pNextElement = pElement->m_pNextElement;
		delete pElement;
		pElement = pNextElement;
	}

	// Init data
	m_nNumOfElements = 0;
	m_pFirstElement  = nullptr;
	m_pLastElement   = nullptr;

	// Same for the free elements
	RemoveAllFreeElements();
}

template <class ValueType>
bool FastPool<ValueType>::IsElement(const ValueType &Element) const
{
	// Find the index of the given element
	ValueType *pElement = m_pFirstElement;
	while (pElement) {
		if (*pElement == Element)
			return true;
		pElement = pElement->m_pNextElement;
	}

	// The given element is not in the pool!
	return false;
}

template <class ValueType>
int FastPool<ValueType>::GetIndex(const ValueType &Element) const
{
	// Find the index of the given element
	ValueType *pElement  = m_pFirstElement;
	uint32     nCurIndex = 0;
	while (pElement) {
		if (*pElement == Element)
			return nCurIndex;
		pElement = pElement->m_pNextElement;
		nCurIndex++;
	}

	// The given element is not in the pool!
	return -1;
}

template <class ValueType>
ValueType &FastPool<ValueType>::Get(uint32 nIndex) const
{
	// Which search direction?
	if (nIndex < m_nNumOfElements/2) {
		// Start with the first element
		ValueType *pElement  = m_pFirstElement;
		uint32     nCurIndex = 0;
		while (pElement) {
			if (nCurIndex == nIndex)
				return *pElement;
			pElement = pElement->m_pNextElement;
			nCurIndex++;
		}
	} else {
		// Start with the last element
		ValueType *pElement  = m_pLastElement;
		uint32     nCurIndex = m_nNumOfElements-1;
		while (pElement) {
			if (nCurIndex == nIndex)
				return *pElement;
			pElement = pElement->m_pPreviousElement;
			nCurIndex--;
		}
	}

	// There's no such index within the pool :(
	return FastPool<ValueType>::Null;
}

template <class ValueType>
ValueType &FastPool<ValueType>::operator [](uint32 nIndex) const
{
	return Get(nIndex);
}

template <class ValueType>
bool FastPool<ValueType>::Replace(const ValueType &Element1, const ValueType &Element2)
{
	// Replace the element through the new one
	ValueType *pElement  = m_pFirstElement;
	uint32     nCurIndex = 0;
	while (pElement) {
		// Is this the requested element?
		if (*pElement == Element1) {
			// Backup connectivity
			ValueType *pNextElement     = pElement->m_pNextElement;
			ValueType *pPreviousElement = pElement->m_pPreviousElement;

			// Replace the data
			*pElement = Element2;

			// Reset connectivity
			pElement->m_pNextElement     = pNextElement;
			pElement->m_pPreviousElement = pPreviousElement;

			// All went fine
			return true;
		}

		// Next, please
		pElement = pElement->m_pNextElement;
		nCurIndex++;
	}

	// Error - 'Element1' isn't in the pool!
	return false;
}

template <class ValueType>
bool FastPool<ValueType>::ReplaceAtIndex(uint32 nIndex, const ValueType &Element)
{
	// Check index
	if (nIndex >= m_nNumOfElements)
		return false; // Error!

	// Which search direction?
	if (nIndex < m_nNumOfElements/2) {
		// Start with the first element
		ValueType *pElement  = m_pFirstElement;
		uint32     nCurIndex = 0;
		while (pElement) {
			// Index reached?
			if (nCurIndex == nIndex) {
				// Backup connectivity
				ValueType *pNextElement     = pElement->m_pNextElement;
				ValueType *pPreviousElement = pElement->m_pPreviousElement;

				// Replace the data
				*pElement = Element;

				// Reset connectivity
				pElement->m_pNextElement     = pNextElement;
				pElement->m_pPreviousElement = pPreviousElement;

				// All went fine
				return true;
			}

			// Next, please
			pElement = pElement->m_pNextElement;
			nCurIndex++;
		}
	} else {
		// Start with the last element
		ValueType *pElement  = m_pLastElement;
		uint32     nCurIndex = m_nNumOfElements-1;
		while (pElement) {
			// Index reached?
			if (nCurIndex == nIndex) {
				// Backup connectivity
				ValueType *pNextElement     = pElement->m_pNextElement;
				ValueType *pPreviousElement = pElement->m_pPreviousElement;

				// Replace the data
				*pElement = Element;

				// Reset connectivity
				pElement->m_pNextElement     = pNextElement;
				pElement->m_pPreviousElement = pPreviousElement;

				// All went fine
				return true;
			}

			// Previous, please
			pElement = pElement->m_pPreviousElement;
			nCurIndex--;
		}
	}

	// Error?!! (should be impossible to be right here :)
	return false;
}

template <class ValueType>
ValueType &FastPool<ValueType>::Add()
{
	// Create the new pool element
	ValueType &cNewElement = AddElement();
	cNewElement.m_pNextElement     = nullptr;
	cNewElement.m_pPreviousElement = m_pLastElement;

	// Add the new pool element to the pool
	if (m_pFirstElement) {
		m_pLastElement->m_pNextElement = &cNewElement;
		m_pLastElement				   = &cNewElement;
	} else {
		m_pFirstElement = &cNewElement;
		m_pLastElement  = &cNewElement;
	}

	// Return the new element
	return cNewElement;
}

template <class ValueType>
ValueType &FastPool<ValueType>::Add(const ValueType &Element)
{
	// Create the new pool element
	ValueType &cNewElement = AddElement();
	cNewElement					   = Element;
	cNewElement.m_pNextElement	   = nullptr;
	cNewElement.m_pPreviousElement = m_pLastElement;

	// Add the new pool element to the pool
	if (m_pFirstElement) {
		m_pLastElement->m_pNextElement = &cNewElement;
		m_pLastElement				   = &cNewElement;
	} else {
		m_pFirstElement = &cNewElement;
		m_pLastElement  = &cNewElement;
	}

	// Done
	return cNewElement;
}

template <class ValueType>
uint32 FastPool<ValueType>::Add(const ValueType *pElements, uint32 nCount)
{
	// Is someone fooling us?
	if (pElements) {
		// Add the elements
		for (uint32 i=0; i<nCount; i++)
			Add(pElements[i]);

		// Done
		return nCount;
	} else {
		// Do nothing!
		return 0;
	}
}

template <class ValueType>
Container<ValueType> &FastPool<ValueType>::operator +=(const ValueType &Element)
{
	Add(Element);

	// Return this
	return *this;
}

template <class ValueType>
bool FastPool<ValueType>::Add(const Container<ValueType> &lstContainer, uint32 nStart, uint32 nCount)
{
	// Check parameters
	if (nStart >= lstContainer.GetNumOfElements())
		return false; // Error, invalid start index!
	if (!nCount)
		nCount = lstContainer.GetNumOfElements()-nStart;
	if (nStart+nCount > lstContainer.GetNumOfElements())
		nCount = lstContainer.GetNumOfElements()-nStart;

	// Add elements
	for (uint32 i=nStart; i<nStart+nCount; i++) {
		if (&Add(lstContainer[i]) == &FastPool<ValueType>::Null)
			return false; // Error! (maybe array is full)
	}

	// Done
	return true;
}

template <class ValueType>
Container<ValueType> &FastPool<ValueType>::operator +=(const Container<ValueType> &lstContainer)
{
	Add(lstContainer);

	// Return this
	return *this;
}

template <class ValueType>
ValueType &FastPool<ValueType>::AddAtIndex(int nIndex)
{
	// Add the new element at the end?
	if (nIndex < 0 || static_cast<uint32>(nIndex) == m_nNumOfElements)
		return Add();

	// Valid index?
	if (static_cast<uint32>(nIndex) > m_nNumOfElements)
		return FastPool<ValueType>::Null; // There's no such index within the pool :(

	// Which search direction?
	if (static_cast<uint32>(nIndex) < m_nNumOfElements/2) {
		// Start with the first element
		ValueType *pElement  = m_pFirstElement;
		uint32     nCurIndex = 0;
		while (pElement) {
			// Index reached?
			if (nCurIndex == static_cast<uint32>(nIndex)) {
				// Create the new pool element
				ValueType &cNewElement = AddElement();
				cNewElement.m_pNextElement     = pElement;
				cNewElement.m_pPreviousElement = pElement->m_pPreviousElement;

				// Update connectivity
				if (pElement->m_pPreviousElement)
					pElement->m_pPreviousElement->m_pNextElement = &cNewElement;
				pElement->m_pPreviousElement = &cNewElement;
				if (m_pFirstElement == pElement)
					m_pFirstElement = &cNewElement;

				// Return the new element
				return cNewElement;
			}

			// Next, please
			pElement = pElement->m_pNextElement;
			nCurIndex++;
		}
	} else {
		// Start with the last element
		ValueType *pElement  = m_pLastElement;
		uint32     nCurIndex = m_nNumOfElements-1;
		while (pElement) {
			// Index reached?
			if (nCurIndex == static_cast<uint32>(nIndex)) {
				// Create the new pool element
				ValueType &cNewElement = AddElement();
				cNewElement.m_pNextElement     = pElement;
				cNewElement.m_pPreviousElement = pElement->m_pPreviousElement;

				// Update connectivity
				if (pElement->m_pPreviousElement)
					pElement->m_pPreviousElement->m_pNextElement = &cNewElement;
				pElement->m_pPreviousElement = &cNewElement;
				if (m_pFirstElement == pElement)
					m_pFirstElement = &cNewElement;

				// Return the new element
				return cNewElement;
			}

			// Previous, please
			pElement = pElement->m_pPreviousElement;
			nCurIndex--;
		}
	}

	// Error! (?!)
	return FastPool<ValueType>::Null;
}

template <class ValueType>
ValueType &FastPool<ValueType>::AddAtIndex(const ValueType &Element, int nIndex)
{
	// Add the new element at the end?
	if (nIndex < 0 || static_cast<uint32>(nIndex) == m_nNumOfElements)
		return Add(Element);

	// Valid index?
	if (static_cast<uint32>(nIndex) > m_nNumOfElements)
		return FastPool<ValueType>::Null; // Error, there's no such index within the pool :(

	// Which search direction?
	if (static_cast<uint32>(nIndex) < m_nNumOfElements/2) {
		// Start with the first element
		ValueType *pElement  = m_pFirstElement;
		uint32     nCurIndex = 0;
		while (pElement) {
			// Index reached?
			if (nCurIndex == static_cast<uint32>(nIndex)) {
				// Create the new pool element
				ValueType &cNewElement = AddElement();
				cNewElement					   = Element;
				cNewElement.m_pNextElement	   = pElement;
				cNewElement.m_pPreviousElement = pElement->m_pPreviousElement;

				// Update connectivity
				if (pElement->m_pPreviousElement)
					pElement->m_pPreviousElement->m_pNextElement = &cNewElement;
				pElement->m_pPreviousElement = &cNewElement;
				if (m_pFirstElement == pElement)
					m_pFirstElement = &cNewElement;

				// Return the new element
				return cNewElement;
			}

			// Next, please
			pElement = pElement->m_pNextElement;
			nCurIndex++;
		}
	} else {
		// Start with the last element
		ValueType *pElement  = m_pLastElement;
		uint32     nCurIndex = m_nNumOfElements-1;
		while (pElement) {
			// Index reached?
			if (nCurIndex == static_cast<uint32>(nIndex)) {
				// Create the new pool element
				ValueType &cNewElement = AddElement();
				cNewElement					   = Element;
				cNewElement.m_pNextElement	   = pElement;
				cNewElement.m_pPreviousElement = pElement->m_pPreviousElement;

				// Update connectivity
				if (pElement->m_pPreviousElement)
					pElement->m_pPreviousElement->m_pNextElement = &cNewElement;
				pElement->m_pPreviousElement = &cNewElement;
				if (m_pFirstElement == pElement)
					m_pFirstElement = &cNewElement;

				// Return the new element
				return cNewElement;
			}

			// Previous, please
			pElement = pElement->m_pPreviousElement;
			nCurIndex--;
		}
	}

	// Error! (?!)
	return FastPool<ValueType>::Null;
}

template <class ValueType>
bool FastPool<ValueType>::Remove(const ValueType &Element)
{
	// Find the element to remove in the pool
	ValueType *pElement = m_pFirstElement;
	while (pElement) {
		// Is this the requested element?
		if (*pElement == Element) {
			// Add this pool element to the list of free elements
			RemoveElement(*pElement);

			// All went fine
			return true;
		}

		// Next, please
		pElement = pElement->m_pNextElement;
	}

	// Error, there's no such element within the pool :(
	return false;
}

template <class ValueType>
bool FastPool<ValueType>::RemoveAtIndex(uint32 nIndex)
{
	// Check index
	if (nIndex >= m_nNumOfElements)
		return false; // Error!

	// Which search direction?
	if (nIndex < m_nNumOfElements/2) {
		// Start with the first element
		ValueType *pElement  = m_pFirstElement;
		uint32     nCurIndex = 0;
		while (pElement) {
			// Index reached?
			if (nCurIndex == nIndex) {
				// Add this pool element to the list of free elements
				RemoveElement(*pElement);

				// All went fine
				return true;
			}

			// Next, please
			pElement = pElement->m_pNextElement;
			nCurIndex++;
		}
	} else {
		// Start with the last element
		ValueType *pElement  = m_pLastElement;
		uint32     nCurIndex = m_nNumOfElements-1;
		while (pElement) {
			// Index reached?
			if (nCurIndex == nIndex) {
				// Add this pool element to the list of free elements
				RemoveElement(*pElement);

				// All went fine
				return true;
			}

			// Previous, please
			pElement = pElement->m_pPreviousElement;
			nCurIndex--;
		}
	}

	// Error, there's no such index within the pool :(
	return false;
}

template <class ValueType>
Container<ValueType> &FastPool<ValueType>::operator -=(const ValueType &Element)
{
	Remove(Element);

	// Return this
	return *this;
}

template <class ValueType>
bool FastPool<ValueType>::Remove(const Container<ValueType> &lstContainer, uint32 nStart, uint32 nCount)
{
	// Check parameters
	if (nStart >= lstContainer.GetNumOfElements())
		return false; // Error, invalid start index!
	if (!nCount)
		nCount = lstContainer.GetNumOfElements()-nStart;
	if (nStart+nCount > lstContainer.GetNumOfElements())
		nCount = lstContainer.GetNumOfElements()-nStart;

	// Remove elements
	for (uint32 i=nStart; i<nStart+nCount; i++)
		Remove(lstContainer[i]);

	// Done
	return true;
}

template <class ValueType>
Container<ValueType> &FastPool<ValueType>::operator -=(const Container<ValueType> &lstContainer)
{
	Remove(lstContainer);

	// Return this
	return *this;
}

template <class ValueType>
bool FastPool<ValueType>::Copy(const Container<ValueType> &lstContainer, uint32 nStart, uint32 nCount)
{
	// Check start index and elements to copy
	if (nStart >= lstContainer.GetNumOfElements()) {
		// Empty container?
		if (lstContainer.IsEmpty()) {
			// That's an easy situation: Just clear this container and it's a copy of the given empty container
			Clear();
		} else {
			// Error, invalid start index!
			return false;
		}
	} else {
		// Clear the old pool
		Clear();

		// Get the number of elements to copy
		if (!nCount)
			nCount = lstContainer.GetNumOfElements()-nStart;
		if (nStart+nCount > lstContainer.GetNumOfElements())
			nCount = lstContainer.GetNumOfElements()-nStart;

		// Copy
		for (uint32 i=0; i<nCount; i++)
			Add(lstContainer[i+nStart]);
	}

	// Done
	return true;
}

template <class ValueType>
Container<ValueType> &FastPool<ValueType>::operator =(const Container<ValueType> &lstContainer)
{
	Copy(lstContainer);

	// Return this
	return *this;
}

template <class ValueType>
bool FastPool<ValueType>::Compare(const Container<ValueType> &lstContainer, uint32 nStart, uint32 nCount) const
{
	// Check parameters
	if (nStart >= lstContainer.GetNumOfElements() || nStart >= m_nNumOfElements) {
		// Empty containers?
		if (m_nNumOfElements || lstContainer.GetNumOfElements()) {
			// Error, invalid start index! Not equal!
			return false;
		}
	} else {
		// Get the number of elements to compare
		if (!nCount)
			nCount = lstContainer.GetNumOfElements()-nStart;
		if (nStart+nCount > lstContainer.GetNumOfElements())
			nCount = lstContainer.GetNumOfElements()-nStart;
		if (nStart+nCount > m_nNumOfElements)
			return false; // Not equal!

		// Get the start element from which search direction?
		ValueType *pElement;
		if (nStart < m_nNumOfElements/2) {
			// Start with the first element
			pElement = m_pFirstElement;
			uint32 nCurIndex = 0;
			while (pElement && nCurIndex != nStart) {
				pElement = pElement->m_pNextElement;
				nCurIndex++;
			}
		} else {
			// Start with the last element
			pElement = m_pLastElement;
			uint32 nCurIndex = m_nNumOfElements-1;
			while (pElement && nCurIndex != nStart) {
				pElement = pElement->m_pPreviousElement;
				nCurIndex--;
			}
		}

		// Compare
		for (uint32 i=nStart; i<nStart+nCount; i++) {
			if (!pElement)
				return false; // Not equal! (? :)
			if (*pElement == lstContainer[i])
				pElement = pElement->m_pNextElement;
			else
				return false; // The two containers are not equal!
		}
	}

	// The two containers are equal!
	return true;
}

template <class ValueType>
bool FastPool<ValueType>::operator ==(const Container<ValueType> &lstContainer) const
{
	return Compare(lstContainer, 0, 0);
}

template <class ValueType>
bool FastPool<ValueType>::operator !=(const Container<ValueType> &lstContainer) const
{
	return !Compare(lstContainer, 0, 0);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
