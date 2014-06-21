/*********************************************************\
 *  File: Queue.inl                                      *
 *      Queue template implementation
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
#include "PLCore/Container/QueueIterator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public static variables                               ]
//[-------------------------------------------------------]
template <class ValueType> ValueType Queue<ValueType>::Null;


//[-------------------------------------------------------]
//[ Private static data                                   ]
//[-------------------------------------------------------]
template <class ValueType> ValueType Queue<ValueType>::temp;


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
template <class ValueType>
Queue<ValueType>::Queue() :
	m_nNumOfElements(0),
	m_pTop(nullptr),
	m_pBottom(nullptr)
{
}

/**
*  @brief
*    Copy constructor
*/
template <class ValueType>
Queue<ValueType>::Queue(const Queue<ValueType> &cSource) :
	m_nNumOfElements(0),
	m_pTop(nullptr),
	m_pBottom(nullptr)
{
	// Copy queue
	*this = cSource;
}

/**
*  @brief
*    Destructor
*/
template <class ValueType>
Queue<ValueType>::~Queue()
{
	// Cleanup
	Clear();
}

/**
*  @brief
*    Push an element onto the queue
*/
template <class ValueType>
bool Queue<ValueType>::Push(const ValueType &Element)
{
	// Create new element
	QueueElement *pElement = new QueueElement;
	pElement->Data = Element;

	// Is this the first element?
	if (m_pTop) {
		// Push onto queue
		m_pTop->pNextElement = pElement;
		m_pTop				 = pElement;
		m_pTop->pNextElement = nullptr;
	} else {
		// This is the first element
		m_pBottom			 = pElement;
		m_pTop				 = pElement;
		m_pTop->pNextElement = nullptr;
	}
	m_nNumOfElements++;

	// Done
	return true;
}

/**
*  @brief
*    Pops the bottom element from the queue
*/
template <class ValueType>
bool Queue<ValueType>::Pop(ValueType *pElement)
{
	// Is there a bottom element?
	if (!m_pBottom)
		return false; // Error!

	// Get top element
	if (pElement)
		*pElement = m_pBottom->Data;

	// Pop element
	QueueElement *pBottomElement = m_pBottom;
	m_pBottom = m_pBottom->pNextElement;
	if (!m_pBottom)
		m_pTop = nullptr;
	delete pBottomElement;
	m_nNumOfElements--;

	// Done
	return true;
}

/**
*  @brief
*    Get the top element without removing it
*/
template <class ValueType>
ValueType Queue<ValueType>::Top() const
{
	// On error return temp object...
	return m_pTop ? m_pTop->Data : temp;
}

/**
*  @brief
*    Get the bottom element without removing it
*/
template <class ValueType>
ValueType Queue<ValueType>::Bottom() const
{
	// On error return temp object...
	return m_pBottom ? m_pBottom->Data : temp;
}

/**
*  @brief
*    Returns the number of element on the queue
*/
template <class ValueType>
uint32 Queue<ValueType>::GetNumOfElements() const
{
	return m_nNumOfElements;
}

/**
*  @brief
*    Copies the data from another queue
*/
template <class ValueType>
Queue<ValueType> &Queue<ValueType>::operator =(const Queue<ValueType> &cSource)
{
	// Clear queue
	Clear();

	// Push all elements from the other queue
	QueueElement *pElement = cSource.m_pBottom;
	while (pElement) {
		Push(pElement->Data);
		pElement = pElement->pNextElement;
	}

	// Return this instance
	return *this;
}

/**
*  @brief
*    Clears the whole queue
*/
template <class ValueType>
void Queue<ValueType>::Clear()
{
	// Remove all elements
	while (m_pBottom)
		Pop();
}


//[-------------------------------------------------------]
//[ Public Iterable functions                             ]
//[-------------------------------------------------------]
template <class ValueType>
Iterator<ValueType> Queue<ValueType>::GetIterator(uint32 nIndex) const
{
	Iterator<ValueType> cIterator(*(new QueueIterator<ValueType>(*this, nIndex)));
	return cIterator;
}

template <class ValueType>
ConstIterator<ValueType> Queue<ValueType>::GetConstIterator(uint32 nIndex) const
{
	ConstIterator<ValueType> cIterator(*(new QueueIterator<ValueType>(*this, nIndex)));
	return cIterator;
}

template <class ValueType>
Iterator<ValueType> Queue<ValueType>::GetEndIterator() const
{
	Iterator<ValueType> cIterator(*(new QueueIterator<ValueType>(*this)));
	return cIterator;
}

template <class ValueType>
ConstIterator<ValueType> Queue<ValueType>::GetConstEndIterator() const
{
	ConstIterator<ValueType> cIterator(*(new QueueIterator<ValueType>(*this)));
	return cIterator;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
