/*********************************************************\
 *  File: SimpleMapKeyIterator.inl                       *
 *      Simple map iterator template implementation
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
#include "PLCore/Container/SimpleMap.h"


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
template <class KeyType, class ValueType, class Comparer>
SimpleMapKeyIterator<KeyType, ValueType, Comparer>::SimpleMapKeyIterator(const SimpleMap<KeyType, ValueType, Comparer> &mapOwner, uint32 nIndex) :
	m_pmapOwner(&mapOwner),
	m_pIterator(new Iterator< typename SimpleMap<KeyType, ValueType, Comparer>::Element >(m_pmapOwner->m_lstElements.GetIterator(nIndex)))
{
}

/**
*  @brief
*    Constructor
*/
template <class KeyType, class ValueType, class Comparer>
SimpleMapKeyIterator<KeyType, ValueType, Comparer>::SimpleMapKeyIterator(const SimpleMap<KeyType, ValueType, Comparer> &mapOwner) :
	m_pmapOwner(&mapOwner),
	m_pIterator(new Iterator< typename SimpleMap<KeyType, ValueType, Comparer>::Element >(m_pmapOwner->m_lstElements.GetEndIterator()))
{
}

/**
*  @brief
*    Copy constructor
*/
template <class KeyType, class ValueType, class Comparer>
SimpleMapKeyIterator<KeyType, ValueType, Comparer>::SimpleMapKeyIterator(const SimpleMapKeyIterator<KeyType, ValueType, Comparer> &cSource) :
	m_pmapOwner(cSource.m_pmapOwner),
	m_pIterator(new Iterator< typename SimpleMap<KeyType, ValueType, Comparer>::Element >(*cSource.m_pIterator))
{
}

/**
*  @brief
*    Destructor
*/
template <class KeyType, class ValueType, class Comparer>
SimpleMapKeyIterator<KeyType, ValueType, Comparer>::~SimpleMapKeyIterator()
{
	// Destroy the internal iterator
	delete m_pIterator;
}


//[-------------------------------------------------------]
//[ Private virtual IteratorImpl functions                ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer>
IteratorImpl<KeyType> *SimpleMapKeyIterator<KeyType, ValueType, Comparer>::Clone() const
{
	return new SimpleMapKeyIterator<KeyType, ValueType, Comparer>(*this);
}

template <class KeyType, class ValueType, class Comparer>
bool SimpleMapKeyIterator<KeyType, ValueType, Comparer>::HasNext() const
{
	return m_pIterator->HasNext();
}

template <class KeyType, class ValueType, class Comparer>
KeyType &SimpleMapKeyIterator<KeyType, ValueType, Comparer>::Next()
{
	typename SimpleMap<KeyType, ValueType, Comparer>::Element &cNext = m_pIterator->Next();
	return (&cNext != &FastPool<typename SimpleMap<KeyType, ValueType, Comparer>::Element>::Null) ? cNext.Key : SimpleMap<KeyType, ValueType, Comparer>::NullKey;
}

template <class KeyType, class ValueType, class Comparer>
bool SimpleMapKeyIterator<KeyType, ValueType, Comparer>::HasPrevious() const
{
	return m_pIterator->HasPrevious();
}

template <class KeyType, class ValueType, class Comparer>
KeyType &SimpleMapKeyIterator<KeyType, ValueType, Comparer>::Previous()
{
	typename SimpleMap<KeyType, ValueType, Comparer>::Element &cPrevious = m_pIterator->Previous();
	return (&cPrevious != &FastPool<typename SimpleMap<KeyType, ValueType, Comparer>::Element>::Null) ? cPrevious.Key  : SimpleMap<KeyType, ValueType, Comparer>::NullKey;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
