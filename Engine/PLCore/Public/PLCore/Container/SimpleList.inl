/*********************************************************\
 *  File: SimpleList.inl                                 *
 *      Simple list template implementation
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
#include "PLCore/Container/SimpleListIterator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public static variables                               ]
//[-------------------------------------------------------]
template <class ValueType> ValueType SimpleList<ValueType>::Null;


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
template <class ValueType>
SimpleList<ValueType>::SimpleList() :
	pFirstElement(nullptr)
{
}

/**
*  @brief
*    Copy constructor
*/
template <class ValueType>
SimpleList<ValueType>::SimpleList(const SimpleList<ValueType> &lstSource, uint32 nStart, uint32 nCount) :
	pFirstElement(nullptr)
{
	// Copy
	Copy(lstSource, nStart, nCount);
}

/**
*  @brief
*    Copy constructor
*/
template <class ValueType>
SimpleList<ValueType>::SimpleList(const Container<ValueType> &lstSource, uint32 nStart, uint32 nCount) :
	pFirstElement(nullptr)
{
	// Copy
	Copy(lstSource, nStart, nCount);
}

/**
*  @brief
*    Destructor
*/
template <class ValueType>
SimpleList<ValueType>::~SimpleList()
{
	Clear();
}

/**
*  @brief
*    Makes this container to a copy of another container
*/
template <class ValueType>
bool SimpleList<ValueType>::Copy(const SimpleList<ValueType> &lstContainer, uint32 nStart, uint32 nCount)
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

/**
*  @brief
*    Copy operator
*/
template <class ValueType>
SimpleList<ValueType> &SimpleList<ValueType>::operator =(const SimpleList<ValueType> &lstSource)
{
	// Clear the old list
	Clear();

	// Start with the first element
	const ListElement *pElement  = lstSource.pFirstElement;
	while (pElement) {
		// Add
		Add(pElement->Data);

		// Next, please
		pElement = pElement->pNextElement;
	}

	// Done
	return *this;
}


//[-------------------------------------------------------]
//[ Public Iterable functions                             ]
//[-------------------------------------------------------]
template <class ValueType>
Iterator<ValueType> SimpleList<ValueType>::GetIterator(uint32 nIndex) const
{
	Iterator<ValueType> cIterator(*(new SimpleListIterator<ValueType>(*this, nIndex)));
	return cIterator;
}

template <class ValueType>
ConstIterator<ValueType> SimpleList<ValueType>::GetConstIterator(uint32 nIndex) const
{
	ConstIterator<ValueType> cIterator(*(new SimpleListIterator<ValueType>(*this, nIndex)));
	return cIterator;
}

template <class ValueType>
Iterator<ValueType> SimpleList<ValueType>::GetEndIterator() const
{
	Iterator<ValueType> cIterator(*(new SimpleListIterator<ValueType>(*this)));
	return cIterator;
}

template <class ValueType>
ConstIterator<ValueType> SimpleList<ValueType>::GetConstEndIterator() const
{
	ConstIterator<ValueType> cIterator(*(new SimpleListIterator<ValueType>(*this)));
	return cIterator;
}


//[-------------------------------------------------------]
//[ Public Container functions                            ]
//[-------------------------------------------------------]
template <class ValueType>
bool SimpleList<ValueType>::IsEmpty() const
{
	return (pFirstElement == nullptr);
}

template <class ValueType>
uint32 SimpleList<ValueType>::GetNumOfElements() const
{
	// Start with the first element
	ListElement *pElement  = pFirstElement;
	uint32		 nCurIndex = 0;
	while (pElement) {
		// Next, please
		pElement = pElement->pNextElement;
		nCurIndex++;
	}

	// Return the found number of elements
	return nCurIndex;
}

template <class ValueType>
uint32 SimpleList<ValueType>::GetElementSize() const
{
	return sizeof(ValueType);
}

template <class ValueType>
uint32 SimpleList<ValueType>::GetSize() const
{
	return GetNumOfElements()*sizeof(ValueType);
}

template <class ValueType>
void SimpleList<ValueType>::Clear()
{
	// Clear all list elements
	ListElement *pElement = pFirstElement;
	while (pElement) {
		ListElement *pNextElement = pElement->pNextElement;
		delete pElement;
		pElement = pNextElement;
	}

	// Init data
	pFirstElement = nullptr;
}

template <class ValueType>
bool SimpleList<ValueType>::IsElement(const ValueType &Element) const
{
	// Find the index of the given element
	ListElement *pElement = pFirstElement;
	while (pElement) {
		if (pElement->Data == Element)
			return true;
		pElement = pElement->pNextElement;
	}

	// The given element is not in the list!
	return false;
}

template <class ValueType>
int SimpleList<ValueType>::GetIndex(const ValueType &Element) const
{
	// Find the index of the given element
	ListElement *pElement  = pFirstElement;
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
ValueType &SimpleList<ValueType>::Get(uint32 nIndex) const
{
	// Start with the first element
	ListElement *pElement  = pFirstElement;
	uint32		 nCurIndex = 0;
	while (pElement) {
		if (nCurIndex == nIndex)
			return pElement->Data;
		pElement = pElement->pNextElement;
		nCurIndex++;
	}

	// There's no such index within the list :(
	return SimpleList<ValueType>::Null;
}

template <class ValueType>
ValueType &SimpleList<ValueType>::operator [](uint32 nIndex) const
{
	return Get(nIndex);
}

template <class ValueType>
bool SimpleList<ValueType>::Replace(const ValueType &Element1, const ValueType &Element2)
{
	// Replace the element through the new one
	ListElement *pElement  = pFirstElement;
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
bool SimpleList<ValueType>::ReplaceAtIndex(uint32 nIndex, const ValueType &Element)
{
	// Start with the first element
	ListElement *pElement  = pFirstElement;
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

	// Error, index not found
	return false;
}

template <class ValueType>
ValueType &SimpleList<ValueType>::Add()
{
	// Create the new list element
	ListElement *pNewElement = new ListElement;
	pNewElement->pNextElement = nullptr;

	// Add the new list element to the list
	if (pFirstElement) {
		// Start with the first element
		ListElement *pElement		  = pFirstElement->pNextElement;
		ListElement *pPreviousElement = pFirstElement;
		while (pElement) {
			// Next, please
			pPreviousElement = pElement;
			pElement		 = pElement->pNextElement;
		}

		// Connect elements, note that 'pPreviousElement' IS in here valid in every situation!
		pPreviousElement->pNextElement = pNewElement;
	} else {
		pFirstElement = pNewElement;
	}

	// Return the new element
	return pNewElement->Data;
}

template <class ValueType>
ValueType &SimpleList<ValueType>::Add(const ValueType &Element)
{
	// Create the new list element
	ListElement *pNewElement = new ListElement;
	pNewElement->Data		  = Element;
	pNewElement->pNextElement = nullptr;

	// Add the new list element to the list
	if (pFirstElement) {
		// Start with the first element
		ListElement *pElement		  = pFirstElement->pNextElement;
		ListElement *pPreviousElement = pFirstElement;
		while (pElement) {
			// Next, please
			pPreviousElement = pElement;
			pElement		 = pElement->pNextElement;
		}

		// Connect elements, note that 'pPreviousElement' IS in here valid in every situation!
		pPreviousElement->pNextElement = pNewElement;
	} else {
		pFirstElement = pNewElement;
	}

	// Done
	return pNewElement->Data;
}

template <class ValueType>
uint32 SimpleList<ValueType>::Add(const ValueType *pElements, uint32 nCount)
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
SimpleList<ValueType> &SimpleList<ValueType>::operator +=(const ValueType &Element)
{
	Add(Element);

	// Return this
	return *this;
}

template <class ValueType>
bool SimpleList<ValueType>::Add(const Container<ValueType> &lstContainer, uint32 nStart, uint32 nCount)
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
		if (&Add(lstContainer[i]) == &SimpleList<ValueType>::Null)
			return false; // Error! (maybe array is full)
	}

	// Done
	return true;
}

template <class ValueType>
SimpleList<ValueType> &SimpleList<ValueType>::operator +=(const Container<ValueType> &lstContainer)
{
	Add(lstContainer);

	// Return this
	return *this;
}

template <class ValueType>
ValueType &SimpleList<ValueType>::AddAtIndex(int nIndex)
{
	// Add the new element at the end?
	if (nIndex < 0)
		return Add();

	// Start with the first element
	ListElement *pElement		  = pFirstElement;
	ListElement *pPreviousElement = nullptr;
	uint32		 nCurIndex		  = 0;
	while (pElement) {
		// Index reached?
		if (nCurIndex == static_cast<uint32>(nIndex)) {
			// Create the new list element
			ListElement *pNewElement = new ListElement;
			pNewElement->pNextElement = pElement;

			// Update connectivity
			if (pPreviousElement)
				pPreviousElement->pNextElement = pNewElement;
			if (pFirstElement == pElement)
				pFirstElement = pNewElement;

			// Return the new element
			return pNewElement->Data;
		}

		// Next, please
		pPreviousElement = pElement;
		pElement		 = pElement->pNextElement;
		nCurIndex++;
	}

	// Add the new element at the end?
	if (static_cast<uint32>(nIndex) == nCurIndex)
		return Add();

	// There's no such index within the list :(
	return SimpleList<ValueType>::Null;
}

template <class ValueType>
ValueType &SimpleList<ValueType>::AddAtIndex(const ValueType &Element, int nIndex)
{
	// Add the new element at the end?
	if (nIndex < 0)
		return Add(Element);

	// Start with the first element
	ListElement *pElement		  = pFirstElement;
	ListElement *pPreviousElement = nullptr;
	uint32		 nCurIndex		  = 0;
	while (pElement) {
		// Index reached?
		if (nCurIndex == static_cast<uint32>(nIndex)) {
			// Create the new list element
			ListElement *pNewElement = new ListElement;
			pNewElement->Data		  = Element;
			pNewElement->pNextElement = pElement;

			// Update connectivity
			if (pPreviousElement)
				pPreviousElement->pNextElement = pNewElement;
			if (pFirstElement == pElement)
				pFirstElement = pNewElement;

			// Return the new element
			return pNewElement->Data;
		}

		// Next, please
		pPreviousElement = pElement;
		pElement		 = pElement->pNextElement;
		nCurIndex++;
	}

	// Add the new element at the end?
	if (static_cast<uint32>(nIndex) == nCurIndex)
		return Add(Element);

	// Error, there's no such index within the list :(
	return SimpleList<ValueType>::Null;
}

template <class ValueType>
bool SimpleList<ValueType>::Remove(const ValueType &Element)
{
	// Find the element to remove in the list
	ListElement *pElement		  = pFirstElement;
	ListElement *pPreviousElement = nullptr;
	while (pElement) {
		// Is this the requested element?
		if (pElement->Data == Element) {
			// Update connectivity
			if (pPreviousElement)
				pPreviousElement->pNextElement = pElement->pNextElement;
			if (pElement == pFirstElement)
				pFirstElement = pElement->pNextElement;

			// Delete this list element
			delete pElement;

			// All went fine
			return true;
		}

		// Next, please
		pPreviousElement = pElement;
		pElement		 = pElement->pNextElement;
	}

	// Error, there's no such element within the list :(
	return false;
}

template <class ValueType>
bool SimpleList<ValueType>::RemoveAtIndex(uint32 nIndex)
{
	// Start with the first element
	ListElement *pElement		  = pFirstElement;
	ListElement *pPreviousElement = nullptr;
	uint32		 nCurIndex		  = 0;
	while (pElement) {
		// Index reached?
		if (nCurIndex == nIndex) {
			// Update connectivity
			if (pPreviousElement)
				pPreviousElement->pNextElement = pElement->pNextElement;
			if (pElement == pFirstElement)
				pFirstElement = pElement->pNextElement;

			// Delete this list element
			delete pElement;

			// All went fine
			return true;
		}

		// Next, please
		pPreviousElement = pElement;
		pElement		 = pElement->pNextElement;
		nCurIndex++;
	}

	// Error, there's no such index within the list :(
	return false;
}

template <class ValueType>
SimpleList<ValueType> &SimpleList<ValueType>::operator -=(const ValueType &Element)
{
	Remove(Element);

	// Return this
	return *this;
}

template <class ValueType>
bool SimpleList<ValueType>::Remove(const Container<ValueType> &lstContainer, uint32 nStart, uint32 nCount)
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
SimpleList<ValueType> &SimpleList<ValueType>::operator -=(const Container<ValueType> &lstContainer)
{
	Remove(lstContainer);

	// Return this
	return *this;
}

template <class ValueType>
bool SimpleList<ValueType>::Copy(const Container<ValueType> &lstContainer, uint32 nStart, uint32 nCount)
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
SimpleList<ValueType> &SimpleList<ValueType>::operator =(const Container<ValueType> &lstContainer)
{
	Copy(lstContainer);

	// Return this
	return *this;
}

template <class ValueType>
bool SimpleList<ValueType>::Compare(const Container<ValueType> &lstContainer, uint32 nStart, uint32 nCount) const
{
	// Get the number of elements within this container
	const uint32 nNumOfElements = GetNumOfElements();

	// Check parameters
	if (nStart >= lstContainer.GetNumOfElements() || nStart >= nNumOfElements) {
		// Empty containers?
		if (pFirstElement || lstContainer.GetNumOfElements()) {
			// Error, invalid start index! Not equal!
			return false;
		}
	} else {
		// Get the number of elements to compare
		if (!nCount)
			nCount = lstContainer.GetNumOfElements()-nStart;
		if (nStart+nCount > lstContainer.GetNumOfElements())
			nCount = lstContainer.GetNumOfElements()-nStart;
		if (nStart+nCount > nNumOfElements)
			return false; // Not equal!

		// Start with the first element
		ListElement *pElement = pFirstElement;
		uint32 nCurIndex = 0;
		while (pElement && nCurIndex != nStart) {
			pElement = pElement->pNextElement;
			nCurIndex++;
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
bool SimpleList<ValueType>::operator ==(const Container<ValueType> &lstContainer) const
{
	return Compare(lstContainer, 0, 0);
}

template <class ValueType>
bool SimpleList<ValueType>::operator !=(const Container<ValueType> &lstContainer) const
{
	return !Compare(lstContainer, 0, 0);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
