/*********************************************************\
 *  File: List.inl                                       *
 *      List template implementation
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
#include "PLCore/Container/ListIterator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public static variables                               ]
//[-------------------------------------------------------]
template <class ValueType> ValueType List<ValueType>::Null;


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
template <class ValueType>
List<ValueType>::List() :
	m_nNumOfElements(0),
	m_pFirstElement(nullptr),
	m_pLastElement(nullptr)
{
}

/**
*  @brief
*    Copy constructor
*/
template <class ValueType>
List<ValueType>::List(const List<ValueType> &lstSource, uint32 nStart, uint32 nCount) :
	m_nNumOfElements(0),
	m_pFirstElement(nullptr),
	m_pLastElement(nullptr)
{
	// Copy
	Copy(static_cast<const Container<ValueType>&>(lstSource), nStart, nCount);
}

/**
*  @brief
*    Copy constructor
*/
template <class ValueType>
List<ValueType>::List(const Container<ValueType> &lstSource, uint32 nStart, uint32 nCount) :
	m_nNumOfElements(0),
	m_pFirstElement(nullptr),
	m_pLastElement(nullptr)
{
	// Copy
	Copy(lstSource, nStart, nCount);
}

/**
*  @brief
*    Destructor
*/
template <class ValueType>
List<ValueType>::~List()
{
	Clear();
}

/**
*  @brief
*    Copy operator
*/
template <class ValueType>
Container<ValueType> &List<ValueType>::operator =(const List<ValueType> &lstSource)
{
	return *this = static_cast<const Container<ValueType>&>(lstSource);
}


//[-------------------------------------------------------]
//[ Public virtual Iterable functions                     ]
//[-------------------------------------------------------]
template <class ValueType>
Iterator<ValueType> List<ValueType>::GetIterator(uint32 nIndex) const
{
	Iterator<ValueType> cIterator(*(new ListIterator<ValueType>(*this, nIndex)));
	return cIterator;
}

template <class ValueType>
ConstIterator<ValueType> List<ValueType>::GetConstIterator(uint32 nIndex) const
{
	ConstIterator<ValueType> cIterator(*(new ListIterator<ValueType>(*this, nIndex)));
	return cIterator;
}

template <class ValueType>
Iterator<ValueType> List<ValueType>::GetEndIterator() const
{
	Iterator<ValueType> cIterator(*(new ListIterator<ValueType>(*this)));
	return cIterator;
}

template <class ValueType>
ConstIterator<ValueType> List<ValueType>::GetConstEndIterator() const
{
	ConstIterator<ValueType> cIterator(*(new ListIterator<ValueType>(*this)));
	return cIterator;
}


//[-------------------------------------------------------]
//[ Public virtual Container functions                    ]
//[-------------------------------------------------------]
template <class ValueType>
bool List<ValueType>::IsEmpty() const
{
	return (m_nNumOfElements == 0);
}

template <class ValueType>
uint32 List<ValueType>::GetNumOfElements() const
{
	return m_nNumOfElements;
}

template <class ValueType>
uint32 List<ValueType>::GetElementSize() const
{
	return sizeof(ValueType);
}

template <class ValueType>
uint32 List<ValueType>::GetSize() const
{
	return m_nNumOfElements*sizeof(ValueType);
}

template <class ValueType>
void List<ValueType>::Clear()
{
	// Clear all list elements
	ListElement *pElement = m_pFirstElement;
	while (pElement) {
		ListElement *pNextElement = pElement->pNextElement;
		delete pElement;
		pElement = pNextElement;
	}

	// Init data
	m_nNumOfElements = 0;
	m_pFirstElement  = nullptr;
	m_pLastElement   = nullptr;
}

template <class ValueType>
bool List<ValueType>::IsElement(const ValueType &Element) const
{
	// Find the index of the given element
	ListElement *pElement = m_pFirstElement;
	while (pElement) {
		if (pElement->Data == Element)
			return true;
		pElement = pElement->pNextElement;
	}

	// The given element is not in the list!
	return false;
}

template <class ValueType>
int List<ValueType>::GetIndex(const ValueType &Element) const
{
	// Find the index of the given element
	ListElement *pElement  = m_pFirstElement;
	uint32		 nCurIndex = 0;
	while (pElement) {
		if (pElement->Data == Element)
			return nCurIndex;
		pElement = pElement->pNextElement;
		nCurIndex++;
	}

	// The given element is not in the list!
	return -1;
}

template <class ValueType>
ValueType &List<ValueType>::Get(uint32 nIndex) const
{
	// Which search direction?
	if (nIndex < m_nNumOfElements/2) {
		// Start with the first element
		ListElement *pElement  = m_pFirstElement;
		uint32		 nCurIndex = 0;
		while (pElement) {
			if (nCurIndex == nIndex)
				return pElement->Data;
			pElement = pElement->pNextElement;
			nCurIndex++;
		}
	} else {
		// Start with the last element
		ListElement *pElement  = m_pLastElement;
		uint32		 nCurIndex = m_nNumOfElements-1;
		while (pElement) {
			if (nCurIndex == nIndex)
				return pElement->Data;
			pElement = pElement->pPreviousElement;
			nCurIndex--;
		}
	}

	// There's no such index within the list :(
	return List<ValueType>::Null;
}

template <class ValueType>
ValueType &List<ValueType>::operator [](uint32 nIndex) const
{
	return Get(nIndex);
}

template <class ValueType>
bool List<ValueType>::Replace(const ValueType &Element1, const ValueType &Element2)
{
	// Replace the element through the new one
	ListElement *pElement  = m_pFirstElement;
	uint32		 nCurIndex = 0;
	while (pElement) {
		// Is this the requested element?
		if (pElement->Data == Element1) {
			pElement->Data = Element2;

			// All went fine
			return true;
		}

		// Next, please
		pElement = pElement->pNextElement;
		nCurIndex++;
	}

	// Error, 'Element1' isn't in the list!
	return false;
}

template <class ValueType>
bool List<ValueType>::ReplaceAtIndex(uint32 nIndex, const ValueType &Element)
{
	// Check index
	if (nIndex >= m_nNumOfElements)
		return false; // Error!

	// Which search direction?
	if (nIndex < m_nNumOfElements/2) {
		// Start with the first element
		ListElement *pElement  = m_pFirstElement;
		uint32		 nCurIndex = 0;
		while (pElement) {
			// Index reached?
			if (nCurIndex == nIndex) {
				pElement->Data = Element;

				// All went fine
				return true;
			}

			// Next, please
			pElement = pElement->pNextElement;
			nCurIndex++;
		}
	} else {
		// Start with the last element
		ListElement *pElement  = m_pLastElement;
		uint32		 nCurIndex = m_nNumOfElements-1;
		while (pElement) {
			// Index reached?
			if (nCurIndex == nIndex) {
				pElement->Data = Element;

				// All went fine
				return true;
			}

			// Previous, please
			pElement = pElement->pPreviousElement;
			nCurIndex--;
		}
	}

	// Error?!! (should be impossible to be right here :)
	return false;
}

template <class ValueType>
ValueType &List<ValueType>::Add()
{
	// Create the new list element
	ListElement *pNewElement = new ListElement;
	pNewElement->pNextElement	  = nullptr;
	pNewElement->pPreviousElement = m_pLastElement;

	// Add the new list element to the list
	if (m_pFirstElement) {
		m_pLastElement->pNextElement = pNewElement;
		m_pLastElement				 = pNewElement;
	} else {
		m_pFirstElement = pNewElement;
		m_pLastElement  = pNewElement;
	}
	m_nNumOfElements++;

	// Return the new element
	return pNewElement->Data;
}

template <class ValueType>
ValueType &List<ValueType>::Add(const ValueType &Element)
{
	// Create the new list element
	ListElement *pNewElement = new ListElement;
	pNewElement->Data			  = Element;
	pNewElement->pNextElement	  = nullptr;
	pNewElement->pPreviousElement = m_pLastElement;

	// Add the new list element to the list
	if (m_pFirstElement) {
		m_pLastElement->pNextElement = pNewElement;
		m_pLastElement				 = pNewElement;
	} else {
		m_pFirstElement = pNewElement;
		m_pLastElement  = pNewElement;
	}
	m_nNumOfElements++;

	// Done
	return pNewElement->Data;
}

template <class ValueType>
uint32 List<ValueType>::Add(const ValueType *pElements, uint32 nCount)
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
Container<ValueType> &List<ValueType>::operator +=(const ValueType &Element)
{
	Add(Element);

	// Return this
	return *this;
}

template <class ValueType>
bool List<ValueType>::Add(const Container<ValueType> &lstContainer, uint32 nStart, uint32 nCount)
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
		if (&Add(lstContainer[i]) == &List<ValueType>::Null)
			return false; // Error! (maybe array is full)
	}

	// Done
	return true;
}

template <class ValueType>
Container<ValueType> &List<ValueType>::operator +=(const Container<ValueType> &lstContainer)
{
	Add(lstContainer);

	// Return this
	return *this;
}

template <class ValueType>
ValueType &List<ValueType>::AddAtIndex(int nIndex)
{
	// Add the new element at the end?
	if (nIndex < 0 || static_cast<uint32>(nIndex) == m_nNumOfElements)
		return Add();

	// Valid index?
	if (static_cast<uint32>(nIndex) > m_nNumOfElements)
		return List<ValueType>::Null; // There's no such index within the list :(

	// Which search direction?
	if (static_cast<uint32>(nIndex) < m_nNumOfElements/2) {
		// Start with the first element
		ListElement *pElement  = m_pFirstElement;
		uint32		 nCurIndex = 0;
		while (pElement) {
			// Index reached?
			if (nCurIndex == static_cast<uint32>(nIndex)) {
				// Create the new list element
				ListElement *pNewElement = new ListElement;
				pNewElement->pNextElement	  = pElement;
				pNewElement->pPreviousElement = pElement->pPreviousElement;

				// Update connectivity
				if (pElement->pPreviousElement)
					pElement->pPreviousElement->pNextElement = pNewElement;
				pElement->pPreviousElement = pNewElement;
				if (m_pFirstElement == pElement)
					m_pFirstElement = pNewElement;
				m_nNumOfElements++;

				// Return the new element
				return pNewElement->Data;
			}

			// Next, please
			pElement = pElement->pNextElement;
			nCurIndex++;
		}
	} else {
		// Start with the last element
		ListElement *pElement  = m_pLastElement;
		uint32		 nCurIndex = m_nNumOfElements-1;
		while (pElement) {
			// Index reached?
			if (nCurIndex == static_cast<uint32>(nIndex)) {
				// Create the new list element
				ListElement *pNewElement = new ListElement;
				pNewElement->pNextElement	  = pElement;
				pNewElement->pPreviousElement = pElement->pPreviousElement;

				// Update connectivity
				if (pElement->pPreviousElement)
					pElement->pPreviousElement->pNextElement = pNewElement;
				pElement->pPreviousElement = pNewElement;
				if (m_pFirstElement == pElement)
					m_pFirstElement = pNewElement;
				m_nNumOfElements++;

				// Return the new element
				return pNewElement->Data;
			}

			// Previous, please
			pElement = pElement->pPreviousElement;
			nCurIndex--;
		}
	}

	// Error! (?!)
	return List<ValueType>::Null;
}

template <class ValueType>
ValueType &List<ValueType>::AddAtIndex(const ValueType &Element, int nIndex)
{
	// Add the new element at the end?
	if (nIndex < 0 || static_cast<uint32>(nIndex) == m_nNumOfElements)
		return Add(Element);

	// Valid index?
	if (static_cast<uint32>(nIndex) > m_nNumOfElements)
		return List<ValueType>::Null; // Error, there's no such index within the list :(

	// Which search direction?
	if (static_cast<uint32>(nIndex) < m_nNumOfElements/2) {
		// Start with the first element
		ListElement *pElement  = m_pFirstElement;
		uint32		 nCurIndex = 0;
		while (pElement) {
			// Index reached?
			if (nCurIndex == static_cast<uint32>(nIndex)) {
				// Create the new list element
				ListElement *pNewElement = new ListElement;
				pNewElement->Data			  = Element;
				pNewElement->pNextElement	  = pElement;
				pNewElement->pPreviousElement = pElement->pPreviousElement;

				// Update connectivity
				if (pElement->pPreviousElement)
					pElement->pPreviousElement->pNextElement = pNewElement;
				pElement->pPreviousElement = pNewElement;
				if (m_pFirstElement == pElement)
					m_pFirstElement = pNewElement;
				m_nNumOfElements++;

				// Return the new element
				return pNewElement->Data;
			}

			// Next, please
			pElement = pElement->pNextElement;
			nCurIndex++;
		}
	} else {
		// Start with the last element
		ListElement *pElement  = m_pLastElement;
		uint32		 nCurIndex = m_nNumOfElements-1;
		while (pElement) {
			// Index reached?
			if (nCurIndex == static_cast<uint32>(nIndex)) {
				// Create the new list element
				ListElement *pNewElement = new ListElement;
				pNewElement->Data			  = Element;
				pNewElement->pNextElement	  = pElement;
				pNewElement->pPreviousElement = pElement->pPreviousElement;

				// Update connectivity
				if (pElement->pPreviousElement)
					pElement->pPreviousElement->pNextElement = pNewElement;
				pElement->pPreviousElement = pNewElement;
				if (m_pFirstElement == pElement)
					m_pFirstElement = pNewElement;
				m_nNumOfElements++;

				// Return the new element
				return pNewElement->Data;
			}

			// Previous, please
			pElement = pElement->pPreviousElement;
			nCurIndex--;
		}
	}

	// Error! (?!)
	return List<ValueType>::Null;
}

template <class ValueType>
bool List<ValueType>::Remove(const ValueType &Element)
{
	// Find the element to remove in the list
	ListElement *pElement = m_pFirstElement;
	while (pElement) {
		// Is this the requested element?
		if (pElement->Data == Element) {
			// Update connectivity
			if (pElement->pPreviousElement)
				pElement->pPreviousElement->pNextElement = pElement->pNextElement;
			if (pElement->pNextElement)
				pElement->pNextElement->pPreviousElement = pElement->pPreviousElement;
			if (pElement == m_pFirstElement)
				m_pFirstElement = pElement->pNextElement;
			if (pElement == m_pLastElement)
				m_pLastElement = pElement->pPreviousElement;

			// Delete this list element
			delete pElement;
			m_nNumOfElements--;

			// All went fine
			return true;
		}

		// Next, please
		pElement = pElement->pNextElement;
	}

	// Error, there's no such element within the list :(
	return false;
}

template <class ValueType>
bool List<ValueType>::RemoveAtIndex(uint32 nIndex)
{
	// Check index
	if (nIndex >= m_nNumOfElements)
		return false; // Error!

	// Which search direction?
	if (nIndex < m_nNumOfElements/2) {
		// Start with the first element
		ListElement *pElement  = m_pFirstElement;
		uint32		 nCurIndex = 0;
		while (pElement) {
			// Index reached?
			if (nCurIndex == nIndex) {
				// Update connectivity
				if (pElement->pPreviousElement)
					pElement->pPreviousElement->pNextElement = pElement->pNextElement;
				if (pElement->pNextElement)
					pElement->pNextElement->pPreviousElement = pElement->pPreviousElement;
				if (pElement == m_pFirstElement)
					m_pFirstElement = pElement->pNextElement;
				if (pElement == m_pLastElement)
					m_pLastElement = pElement->pPreviousElement;

				// Delete this list element
				delete pElement;
				m_nNumOfElements--;

				// All went fine
				return true;
			}

			// Next, please
			pElement = pElement->pNextElement;
			nCurIndex++;
		}
	} else {
		// Start with the last element
		ListElement *pElement  = m_pLastElement;
		uint32		 nCurIndex = m_nNumOfElements-1;
		while (pElement) {
			// Index reached?
			if (nCurIndex == nIndex) {
				// Update connectivity
				if (pElement->pPreviousElement)
					pElement->pPreviousElement->pNextElement = pElement->pNextElement;
				if (pElement->pNextElement)
					pElement->pNextElement->pPreviousElement = pElement->pPreviousElement;
				if (pElement == m_pFirstElement)
					m_pFirstElement = pElement->pNextElement;
				if (pElement == m_pLastElement)
					m_pLastElement = pElement->pPreviousElement;

				// Delete this list element
				delete pElement;
				m_nNumOfElements--;

				// All went fine
				return true;
			}

			// Previous, please
			pElement = pElement->pPreviousElement;
			nCurIndex--;
		}
	}

	// Error, there's no such index within the list :(
	return false;
}

template <class ValueType>
Container<ValueType> &List<ValueType>::operator -=(const ValueType &Element)
{
	Remove(Element);

	// Return this
	return *this;
}

template <class ValueType>
bool List<ValueType>::Remove(const Container<ValueType> &lstContainer, uint32 nStart, uint32 nCount)
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
Container<ValueType> &List<ValueType>::operator -=(const Container<ValueType> &lstContainer)
{
	Remove(lstContainer);

	// Return this
	return *this;
}

template <class ValueType>
bool List<ValueType>::Copy(const Container<ValueType> &lstContainer, uint32 nStart, uint32 nCount)
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
		// Clear the old list
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
Container<ValueType> &List<ValueType>::operator =(const Container<ValueType> &lstContainer)
{
	Copy(lstContainer);

	// Return this
	return *this;
}

template <class ValueType>
bool List<ValueType>::Compare(const Container<ValueType> &lstContainer, uint32 nStart, uint32 nCount) const
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
		ListElement *pElement;
		if (nStart < m_nNumOfElements/2) {
			// Start with the first element
			pElement = m_pFirstElement;
			uint32 nCurIndex = 0;
			while (pElement && nCurIndex != nStart) {
				pElement = pElement->pNextElement;
				nCurIndex++;
			}
		} else {
			// Start with the last element
			pElement = m_pLastElement;
			uint32 nCurIndex = m_nNumOfElements-1;
			while (pElement && nCurIndex != nStart) {
				pElement = pElement->pPreviousElement;
				nCurIndex--;
			}
		}

		// Compare
		for (uint32 i=nStart; i<nStart+nCount; i++) {
			if (!pElement)
				return false; // Not equal! (? :)
			if (pElement->Data == lstContainer[i])
				pElement = pElement->pNextElement;
			else
				return false; // The two containers are not equal!
		}
	}

	// The two containers are equal!
	return true;
}

template <class ValueType>
bool List<ValueType>::operator ==(const Container<ValueType> &lstContainer) const
{
	return Compare(lstContainer, 0, 0);
}

template <class ValueType>
bool List<ValueType>::operator !=(const Container<ValueType> &lstContainer) const
{
	return !Compare(lstContainer, 0, 0);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
