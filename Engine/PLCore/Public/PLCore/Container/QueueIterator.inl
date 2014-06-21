/*********************************************************\
 *  File: QueueIterator.inl                              *
 *      Queue iterator template implementation
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
#include "PLCore/Container/Queue.h"


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
QueueIterator<ValueType>::QueueIterator(const Queue<ValueType> &lstQueue, uint32 nIndex) :
	m_plstQueue(&lstQueue)
{
	// Start at first element?
	if (!nIndex) {
		m_pNextElement     = m_plstQueue->m_pBottom;
		m_pPreviousElement = nullptr;

	// Ok, we start somewhere between the start/end element
	} else {
		// Start with the first element
		m_pPreviousElement = m_plstQueue->m_pBottom;
		m_pNextElement     = m_pPreviousElement->pNextElement;
		uint32 nCurrent = 1;
		while (m_pNextElement && nCurrent != nIndex) {
			m_pPreviousElement = m_pNextElement;
			m_pNextElement     = m_pNextElement->pNextElement;
			nCurrent++;
		}
	}
}

/**
*  @brief
*    Constructor
*/
template <class ValueType>
QueueIterator<ValueType>::QueueIterator(const Queue<ValueType> &lstQueue) :
	m_plstQueue(&lstQueue),
	m_pNextElement(nullptr),
	m_pPreviousElement(nullptr)
{
	// Start with the first element
	typename Queue<ValueType>::QueueElement *pElement = lstQueue.m_pBottom;
	while (pElement) {
		// Next, please
		m_pPreviousElement = pElement;
		pElement		   = pElement->pNextElement;
	}
}

/**
*  @brief
*    Copy constructor
*/
template <class ValueType>
QueueIterator<ValueType>::QueueIterator(const QueueIterator<ValueType> &cSource) :
	m_plstQueue(cSource.m_plstQueue),
	m_pNextElement(cSource.m_pNextElement),
	m_pPreviousElement(cSource.m_pPreviousElement)
{
}

/**
*  @brief
*    Destructor
*/
template <class ValueType>
QueueIterator<ValueType>::~QueueIterator()
{
}


//[-------------------------------------------------------]
//[ Private virtual IteratorImpl functions                ]
//[-------------------------------------------------------]
template <class ValueType>
IteratorImpl<ValueType> *QueueIterator<ValueType>::Clone() const
{
	return new QueueIterator<ValueType>(*this);
}

template <class ValueType>
bool QueueIterator<ValueType>::HasNext() const
{
	return (m_pNextElement != nullptr);
}

template <class ValueType>
ValueType &QueueIterator<ValueType>::Next()
{
	if (!m_pNextElement)
		return Queue<ValueType>::Null; // There's no next element
	m_pPreviousElement = m_pNextElement;
	m_pNextElement     = m_pNextElement->pNextElement;
	return m_pPreviousElement->Data;
}

template <class ValueType>
bool QueueIterator<ValueType>::HasPrevious() const
{
	return (m_pPreviousElement != nullptr);
}

template <class ValueType>
ValueType &QueueIterator<ValueType>::Previous()
{
	if (!m_pPreviousElement)
		return Queue<ValueType>::Null; // There's no previous element

	// Set next element
	m_pNextElement = m_pPreviousElement;

	// Start with the first element
	typename Queue<ValueType>::QueueElement *pElement = m_plstQueue->m_pBottom;
	m_pPreviousElement = nullptr;
	while (pElement && m_pNextElement != pElement) {
		// Next, please
		m_pPreviousElement = pElement;
		pElement		   = pElement->pNextElement;
	}

	// Return data
	return m_pNextElement->Data;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
