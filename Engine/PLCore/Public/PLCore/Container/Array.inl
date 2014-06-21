/*********************************************************\
 *  File: Array.inl                                      *
 *      Array template implementation
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
#include "PLCore/Core/MemoryManager.h"
#include "PLCore/Container/ArrayIterator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public static variables                               ]
//[-------------------------------------------------------]
template <class ValueType> ValueType Array<ValueType>::Null;


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
template <class ValueType>
Array<ValueType>::Array(uint32 nMaxNumOfElements, bool bAdded, bool bInit) :
	m_nMaxNumOfElements(0),
	m_nNumOfElements(0),
	m_pData(nullptr),
	m_nResizeCount(10)
{
	// Resize
	Resize(nMaxNumOfElements, bAdded, bInit);
}

/**
*  @brief
*    Copy constructor
*/
template <class ValueType>
Array<ValueType>::Array(const Array<ValueType> &lstSource, uint32 nStart, uint32 nCount) :
	m_nMaxNumOfElements(0),
	m_nNumOfElements(0),
	m_pData(nullptr),
	m_nResizeCount(lstSource.m_nResizeCount)
{
	// Copy
	Copy(static_cast<const Container<ValueType>&>(lstSource), nStart, nCount);
}

/**
*  @brief
*    Copy constructor
*/
template <class ValueType>
Array<ValueType>::Array(const Container<ValueType> &lstSource, uint32 nStart, uint32 nCount) :
	m_nMaxNumOfElements(0),
	m_nNumOfElements(0),
	m_pData(nullptr),
	m_nResizeCount(10)
{
	// Copy
	Copy(lstSource, nStart, nCount);
}

/**
*  @brief
*    Destructor
*/
template <class ValueType>
Array<ValueType>::~Array()
{
	Clear();
}

/**
*  @brief
*    Copy operator
*/
template <class ValueType>
Container<ValueType> &Array<ValueType>::operator =(const Array<ValueType> &lstSource)
{
	return *this = static_cast<const Container<ValueType>&>(lstSource);
}

/**
*  @brief
*    Returns the maximum number of elements in the array
*/
template <class ValueType>
uint32 Array<ValueType>::GetMaxNumOfElements() const
{
	return m_nMaxNumOfElements;
}

/**
*  @brief
*    Sets the number of elements in the array
*/
template <class ValueType>
bool Array<ValueType>::Resize(uint32 nMaxNumOfElements, bool bAdded, bool bInit)
{
	// Check parameter
	if (m_nMaxNumOfElements == nMaxNumOfElements) {
		// All elements added by default?
		if (bAdded) {
			// Initialize 'new' elements
			if (bInit && nMaxNumOfElements > m_nNumOfElements) {
				for (uint32 i=m_nNumOfElements; i<nMaxNumOfElements; i++)
					MemoryManager::Set(&m_pData[i], 0, sizeof(ValueType));
			}
			m_nNumOfElements = m_nMaxNumOfElements;
		}
	} else {
		// Clear the array completely or just resize it?
		if (nMaxNumOfElements) {
			// Create the new data
			ValueType *pNewData = new ValueType[nMaxNumOfElements];

			// Use the new maximum number of elements to clamp the number of current elements
			if (m_nNumOfElements > nMaxNumOfElements)
				m_nNumOfElements = nMaxNumOfElements;

			// If there is old data, rescue what's possible
			if (m_pData) {
				// Copy old data content
				for (uint32 i=0; i<m_nNumOfElements; i++)
					pNewData[i] = m_pData[i];

				// Destroy the old data
				delete [] m_pData;
			}

			// Update data pointer
			m_pData = pNewData;

			// Initialize new elements
			if (bInit && nMaxNumOfElements > m_nMaxNumOfElements) {
				for (uint32 i=m_nMaxNumOfElements; i<nMaxNumOfElements; i++)
					MemoryManager::Set(&m_pData[i], 0, sizeof(ValueType));
			}

			// Update element counters
			m_nMaxNumOfElements = nMaxNumOfElements;
			if (bAdded)
				m_nNumOfElements = m_nMaxNumOfElements;
		} else {
			// The resized array has to be completely empty
			Clear();
		}
	}

	// Done
	return true;
}

/**
*  @brief
*    Returns the number of elements automatically added if the array
*    size is to small
*/
template <class ValueType>
uint32 Array<ValueType>::GetResizeCount() const
{
	return m_nResizeCount;
}

/**
*  @brief
*    Sets the number of elements automatically added if the array
*    size is to small
*/
template <class ValueType>
bool Array<ValueType>::SetResizeCount(uint32 nCount)
{
	// Set new resize count
	m_nResizeCount = nCount;

	// Done
	return true;
}

/**
*  @brief
*    Returns the array data
*/
template <class ValueType>
ValueType *Array<ValueType>::GetData() const
{
	return m_pData;
}

/**
*  @brief
*    Resets the array
*/
template <class ValueType>
void Array<ValueType>::Reset()
{
	m_nNumOfElements = 0;
}

/**
*  @brief
*    Moves an element within the array
*/
template <class ValueType>
void Array<ValueType>::MoveElement(uint32 nFromIndex, uint32 nToIndex)
{
	// Anything to do in here?
	if (nFromIndex != nToIndex) {
		// Get the item we want to move
		ValueType cItemToMove = Get(nFromIndex);

		// Evaluate the movement direction
		if (nToIndex > nFromIndex) {
			// Move all items after "nFromIndex" one down until "nToIndex" is reached
			for (uint32 i=nFromIndex; i<nToIndex; i++)
				m_pData[i] = m_pData[i+1];
		} else {
			// Move all items before "nFromIndex" one up until "nToIndex" is reached
			for (uint32 i=nFromIndex; i>nToIndex; i--)
				m_pData[i] = m_pData[i-1];
		}

		// Give the moved element a new home
		m_pData[nToIndex] = cItemToMove;
	}
}


//[-------------------------------------------------------]
//[ Public virtual Iterable functions                     ]
//[-------------------------------------------------------]
template <class ValueType>
Iterator<ValueType> Array<ValueType>::GetIterator(uint32 nIndex) const
{
	Iterator<ValueType> cIterator(*(new ArrayIterator<ValueType>(*this, nIndex)));
	return cIterator;
}

template <class ValueType>
ConstIterator<ValueType> Array<ValueType>::GetConstIterator(uint32 nIndex) const
{
	ConstIterator<ValueType> cIterator(*(new ArrayIterator<ValueType>(*this, nIndex)));
	return cIterator;
}

template <class ValueType>
Iterator<ValueType> Array<ValueType>::GetEndIterator() const
{
	Iterator<ValueType> cIterator(*(new ArrayIterator<ValueType>(*this)));
	return cIterator;
}

template <class ValueType>
ConstIterator<ValueType> Array<ValueType>::GetConstEndIterator() const
{
	ConstIterator<ValueType> cIterator(*(new ArrayIterator<ValueType>(*this)));
	return cIterator;
}


//[-------------------------------------------------------]
//[ Public virtual Container functions                    ]
//[-------------------------------------------------------]
template <class ValueType>
bool Array<ValueType>::IsEmpty() const
{
	return (m_nNumOfElements == 0);
}

template <class ValueType>
uint32 Array<ValueType>::GetNumOfElements() const
{
	return m_nNumOfElements;
}

template <class ValueType>
uint32 Array<ValueType>::GetElementSize() const
{
	return sizeof(ValueType);
}

template <class ValueType>
uint32 Array<ValueType>::GetSize() const
{
	return m_nNumOfElements*sizeof(ValueType);
}

template <class ValueType>
void Array<ValueType>::Clear()
{
	if (m_pData) {
		delete [] m_pData;
		m_pData = nullptr;
	}
	m_nMaxNumOfElements = 0;
	m_nNumOfElements	= 0;
}

template <class ValueType>
bool Array<ValueType>::IsElement(const ValueType &Element) const
{
	// Find element index
	for (uint32 i=0; i<m_nNumOfElements; i++) {
		if (m_pData[i] == Element)
			return true;
	}

	// The given element is not in the container!
	return false;
}

template <class ValueType>
int Array<ValueType>::GetIndex(const ValueType &Element) const
{
	// Find element index
	for (uint32 i=0; i<m_nNumOfElements; i++) {
		if (m_pData[i] == Element)
			return i;
	}

	// The given element is not in the container!
	return -1;
}

template <class ValueType>
ValueType &Array<ValueType>::Get(uint32 nIndex) const
{
	return (nIndex < m_nNumOfElements) ? m_pData[nIndex] : Array<ValueType>::Null;
}

template <class ValueType>
ValueType &Array<ValueType>::operator [](uint32 nIndex) const
{
	return (nIndex < m_nNumOfElements) ? m_pData[nIndex] : Array<ValueType>::Null;
}

template <class ValueType>
bool Array<ValueType>::Replace(const ValueType &Element1, const ValueType &Element2)
{
	// Get the index of the given element
	const int nIndex = GetIndex(Element1);
	if (nIndex >= 0) {
		// Replace the element through the new one
		m_pData[nIndex] = Element2;

		// Done
		return true;
	} else {
		// Error, the given element is NOT within the container!
		return false;
	}
}

template <class ValueType>
bool Array<ValueType>::ReplaceAtIndex(uint32 nIndex, const ValueType &Element)
{
	// Check index
	if (nIndex >= m_nNumOfElements)
		return false; // Error!

	// Replace the element through the new one
	m_pData[nIndex] = Element;

	// Done
	return true;
}

template <class ValueType>
ValueType &Array<ValueType>::Add()
{
	// Check whether the array is full and we have to resize it
	if (m_nNumOfElements >= m_nMaxNumOfElements) {
		if (!m_nResizeCount || !Resize(m_nMaxNumOfElements+m_nResizeCount, false, false))
			return Array<ValueType>::Null; // Error!
	}

	// Add element
	m_nNumOfElements++;

	// Return the new element
	return m_pData[m_nNumOfElements-1];
}

template <class ValueType>
ValueType &Array<ValueType>::Add(const ValueType &Element)
{
	// Check whether the array is full and we have to resize it
	if (m_nNumOfElements >= m_nMaxNumOfElements) {
		if (!m_nResizeCount || !Resize(m_nMaxNumOfElements+m_nResizeCount, false, false))
			return Array<ValueType>::Null; // Error!
	}

	// Add element
	m_pData[m_nNumOfElements] = Element;
	m_nNumOfElements++;

	// Done
	return m_pData[m_nNumOfElements-1];
}

template <class ValueType>
uint32 Array<ValueType>::Add(const ValueType *pElements, uint32 nCount)
{
	// Is someone fooling us?
	if (!pElements || !nCount)
		return 0;

	// Check whether the array is full and we have to resize it
	if (m_nNumOfElements+nCount > m_nMaxNumOfElements) {
		if (m_nResizeCount) {
			// Add m_nResizeCount AND nCount
			if (!Resize(m_nMaxNumOfElements+m_nResizeCount+nCount, false, false))
				return 0; // Error!
		} else {
			nCount -= (m_nNumOfElements+nCount)-m_nMaxNumOfElements;
		}
	}

	// Add the elements
	for (uint32 i=0; i<nCount; i++)
		m_pData[m_nNumOfElements+i] = pElements[i];
	m_nNumOfElements += nCount;

	// Done
	return nCount;
}

template <class ValueType>
Container<ValueType> &Array<ValueType>::operator +=(const ValueType &Element)
{
	Add(Element);

	// Return this
	return *this;
}

template <class ValueType>
bool Array<ValueType>::Add(const Container<ValueType> &lstContainer, uint32 nStart, uint32 nCount)
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
		if (&Add(lstContainer[i]) == &Array<ValueType>::Null)
			return false; // Error! (maybe array is full)
	}

	// Done
	return true;
}

template <class ValueType>
Container<ValueType> &Array<ValueType>::operator +=(const Container<ValueType> &lstContainer)
{
	Add(lstContainer);

	// Return this
	return *this;
}

template <class ValueType>
ValueType &Array<ValueType>::AddAtIndex(int nIndex)
{
	// Add at end?
	if (nIndex < 0)
		return Add();

	// Check index
	if (static_cast<uint32>(nIndex) > m_nNumOfElements)
		return Array<ValueType>::Null; // Error!

	// Check whether the array is full and we have to resize it
	if (m_nNumOfElements >= m_nMaxNumOfElements) {
		if (!m_nResizeCount || !Resize(m_nMaxNumOfElements+m_nResizeCount, false, false))
			return Array<ValueType>::Null; // Error!
	}

	// Shift element behind index
	for (uint32 i=m_nNumOfElements; i>static_cast<uint32>(nIndex); i--)
		m_pData[i] = m_pData[i-1];

	// Add element
	m_nNumOfElements++;

	// Return the new element
	return m_pData[nIndex];
}

template <class ValueType>
ValueType &Array<ValueType>::AddAtIndex(const ValueType &Element, int nIndex)
{
	// Add at end?
	if (nIndex < 0)
		return Add(Element);

	// Check index
	if (static_cast<uint32>(nIndex) > m_nNumOfElements)
		return Array<ValueType>::Null; // Error!

	// Check whether the array is full and we have to resize it
	if (m_nNumOfElements >= m_nMaxNumOfElements) {
		if (!m_nResizeCount || !Resize(m_nMaxNumOfElements+m_nResizeCount, false, false))
			return Array<ValueType>::Null; // Error!
	}

	// Shift element behind index
	for (uint32 i=m_nNumOfElements; i>static_cast<uint32>(nIndex); i--)
		m_pData[i] = m_pData[i-1];

	// Add element
	m_pData[nIndex] = Element;
	m_nNumOfElements++;

	// Done
	return m_pData[nIndex];
}

template <class ValueType>
bool Array<ValueType>::Remove(const ValueType &Element)
{
	// Get the index of the given element
	const int nIndex = GetIndex(Element);
	return (nIndex >= 0 && RemoveAtIndex(nIndex));
}

template <class ValueType>
bool Array<ValueType>::RemoveAtIndex(uint32 nIndex)
{
	// Check index
	if (nIndex >= m_nNumOfElements)
		return false; // Error!

	// Shift element behind index
	for (uint32 i=nIndex; i<m_nNumOfElements-1; i++)
		m_pData[i] = m_pData[i+1];

	// Remove element
	m_nNumOfElements--;

	// Check whether the array can be reduced
	if (m_nResizeCount) {
		uint32 nNewNumOfElements = (m_nMaxNumOfElements > m_nResizeCount) ? m_nMaxNumOfElements-m_nResizeCount : 0;
		if (m_nNumOfElements <= nNewNumOfElements)
			Resize(nNewNumOfElements, false, false);
	}

	// Done
	return true;
}

template <class ValueType>
Container<ValueType> &Array<ValueType>::operator -=(const ValueType &Element)
{
	Remove(Element);

	// Return this
	return *this;
}

template <class ValueType>
bool Array<ValueType>::Remove(const Container<ValueType> &lstContainer, uint32 nStart, uint32 nCount)
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
Container<ValueType> &Array<ValueType>::operator -=(const Container<ValueType> &lstContainer)
{
	Remove(lstContainer);

	// Return this
	return *this;
}

template <class ValueType>
bool Array<ValueType>::Copy(const Container<ValueType> &lstContainer, uint32 nStart, uint32 nCount)
{
	// Check parameters
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
		// Get the number of elements to copy
		if (!nCount)
			nCount = lstContainer.GetNumOfElements()-nStart;
		if (nStart+nCount > lstContainer.GetNumOfElements())
			nCount = lstContainer.GetNumOfElements()-nStart;

		// Setup array size
		Resize(nCount);

		// Copy
		for (uint32 i=0; i<nCount; i++)
			m_pData[i] = lstContainer[i+nStart];
		m_nNumOfElements = nCount;
	}

	// Done
	return true;
}

template <class ValueType>
Container<ValueType> &Array<ValueType>::operator =(const Container<ValueType> &lstContainer)
{
	Copy(lstContainer);

	// Return this
	return *this;
}

template <class ValueType>
bool Array<ValueType>::Compare(const Container<ValueType> &lstContainer, uint32 nStart, uint32 nCount) const
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

		// Compare
		for (uint32 i=nStart; i<nStart+nCount; i++) {
			if (!(m_pData[i] == lstContainer[i]))
				return false; // The two containers are not equal!
		}
	}

	// The two containers are equal!
	return true;
}

template <class ValueType>
bool Array<ValueType>::operator ==(const Container<ValueType> &lstContainer) const
{
	return Compare(lstContainer, 0, 0);
}

template <class ValueType>
bool Array<ValueType>::operator !=(const Container<ValueType> &lstContainer) const
{
	return !Compare(lstContainer, 0, 0);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
