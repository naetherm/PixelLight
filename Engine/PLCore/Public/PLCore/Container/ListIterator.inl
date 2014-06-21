/*********************************************************\
 *  File: ListIterator.inl                               *
 *      List iterator template implementation
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
#include "PLCore/Container/List.h"


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
template <class ValueType>
ListIterator<ValueType>::ListIterator(const List<ValueType> &lstList, uint32 nIndex) :
	m_plstList(&lstList)
{
	// Start at first element?
	if (!nIndex) {
		m_pNextElement     = m_plstList->m_pFirstElement;
		m_pPreviousElement = nullptr;

	// Start at last element?
	} else if (nIndex >= m_plstList->m_nNumOfElements-1) {
		m_pNextElement     = nullptr;
		m_pPreviousElement = m_plstList->m_pLastElement;

	// Ok, we start somewhere between the start/end element... which search direction?
	} else {
		if (nIndex < m_plstList->m_nNumOfElements/2) {
			// Start with the first element
			m_pPreviousElement = m_plstList->m_pFirstElement;
			m_pNextElement     = m_pPreviousElement->pNextElement;
			uint32 nCurrent = 1;
			while (m_pNextElement && nCurrent != nIndex) {
				m_pPreviousElement = m_pNextElement;
				m_pNextElement     = m_pNextElement->pNextElement;
				nCurrent++;
			}
		} else {
			// Start with the last element
			m_pNextElement     = m_plstList->m_pLastElement;
			m_pPreviousElement = m_pNextElement->pPreviousElement;
			uint32 nCurrent = m_plstList->m_nNumOfElements-2;
			while (m_pPreviousElement && nCurrent != nIndex) {
				m_pNextElement     = m_pPreviousElement;
				m_pPreviousElement = m_pPreviousElement->pPreviousElement;
				nCurrent--;
			}
		}
	}
}

/**
*  @brief
*    Constructor
*/
template <class ValueType>
ListIterator<ValueType>::ListIterator(const List<ValueType> &lstList) :
	m_plstList(&lstList),
	m_pNextElement(nullptr),
	m_pPreviousElement(lstList.m_pLastElement)
{
}

/**
*  @brief
*    Copy constructor
*/
template <class ValueType>
ListIterator<ValueType>::ListIterator(const ListIterator<ValueType> &cSource) :
	m_plstList(cSource.m_plstList),
	m_pNextElement(cSource.m_pNextElement),
	m_pPreviousElement(cSource.m_pPreviousElement)
{
}

/**
*  @brief
*    Destructor
*/
template <class ValueType>
ListIterator<ValueType>::~ListIterator()
{
}


//[-------------------------------------------------------]
//[ Private virtual IteratorImpl functions                ]
//[-------------------------------------------------------]
template <class ValueType>
IteratorImpl<ValueType> *ListIterator<ValueType>::Clone() const
{
	return new ListIterator<ValueType>(*this);
}

template <class ValueType>
bool ListIterator<ValueType>::HasNext() const
{
	return (m_pNextElement != nullptr);
}

template <class ValueType>
ValueType &ListIterator<ValueType>::Next()
{
	if (!m_pNextElement)
		return List<ValueType>::Null; // There's no next element
	m_pPreviousElement = m_pNextElement;
	m_pNextElement     = m_pNextElement->pNextElement;
	return m_pPreviousElement->Data;
}

template <class ValueType>
bool ListIterator<ValueType>::HasPrevious() const
{
	return (m_pPreviousElement != nullptr);
}

template <class ValueType>
ValueType &ListIterator<ValueType>::Previous()
{
	if (!m_pPreviousElement)
		return List<ValueType>::Null; // There's no previous element
	m_pNextElement     = m_pPreviousElement;
	m_pPreviousElement = m_pPreviousElement->pPreviousElement;
	return m_pNextElement->Data;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
