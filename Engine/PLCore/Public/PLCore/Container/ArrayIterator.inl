/*********************************************************\
 *  File: ArrayIterator.inl                              *
 *      Array iterator template implementation
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
#include "PLCore/Container/Array.h"


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
ArrayIterator<ValueType>::ArrayIterator(const Container<ValueType> &lstContainer, uint32 nIndex) :
	m_plstContainer(&lstContainer),
	m_nNextID((nIndex > lstContainer.GetNumOfElements()-1) ? lstContainer.GetNumOfElements()-1 : nIndex)
{
}

/**
*  @brief
*    Constructor
*/
template <class ValueType>
ArrayIterator<ValueType>::ArrayIterator(const Container<ValueType> &lstContainer) :
	m_plstContainer(&lstContainer),
	m_nNextID(lstContainer.GetNumOfElements())
{
}

/**
*  @brief
*    Copy constructor
*/
template <class ValueType>
ArrayIterator<ValueType>::ArrayIterator(const ArrayIterator<ValueType> &cSource) :
	m_plstContainer(cSource.m_plstContainer),
	m_nNextID(cSource.m_nNextID)
{
}

/**
*  @brief
*    Destructor
*/
template <class ValueType>
ArrayIterator<ValueType>::~ArrayIterator()
{
}


//[-------------------------------------------------------]
//[ Private virtual IteratorImpl functions                ]
//[-------------------------------------------------------]
template <class ValueType>
IteratorImpl<ValueType> *ArrayIterator<ValueType>::Clone() const
{
	return new ArrayIterator<ValueType>(*this);
}

template <class ValueType>
bool ArrayIterator<ValueType>::HasNext() const
{
	return (m_nNextID < m_plstContainer->GetNumOfElements());
}

template <class ValueType>
ValueType &ArrayIterator<ValueType>::Next()
{
	return (m_nNextID < m_plstContainer->GetNumOfElements()) ? m_plstContainer->Get(m_nNextID++) : Array<ValueType>::Null;
}

template <class ValueType>
bool ArrayIterator<ValueType>::HasPrevious() const
{
	return (m_nNextID != 0);
}

template <class ValueType>
ValueType &ArrayIterator<ValueType>::Previous()
{
	return m_nNextID ? m_plstContainer->Get(--m_nNextID) : Array<ValueType>::Null;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
