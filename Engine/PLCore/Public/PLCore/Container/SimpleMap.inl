/*********************************************************\
 *  File: SimpleMap.inl                                  *
 *      Simple map template implementation
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
#include "PLCore/Container/SimpleMapIterator.h"
#include "PLCore/Container/SimpleMapKeyIterator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public static variables                               ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer> ValueType SimpleMap<KeyType, ValueType, Comparer>::Null;


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
template <class KeyType, class ValueType, class Comparer>
SimpleMap<KeyType, ValueType, Comparer>::SimpleMap()
{
}

/**
*  @brief
*    Destructor
*/
template <class KeyType, class ValueType, class Comparer>
SimpleMap<KeyType, ValueType, Comparer>::~SimpleMap()
{
}

/**
*  @brief
*    Copy operator
*/
template <class KeyType, class ValueType, class Comparer>
Map<KeyType, ValueType> &SimpleMap<KeyType, ValueType, Comparer>::operator =(const SimpleMap<KeyType, ValueType, Comparer> &lstSource)
{
	// 'FastPool' will do the hard work for us :D
	m_lstElements = lstSource.m_lstElements;

	// Return this instance
	return *this;
}


//[-------------------------------------------------------]
//[ Public virtual Iterable functions                     ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer>
Iterator<ValueType> SimpleMap<KeyType, ValueType, Comparer>::GetIterator(uint32 nIndex) const
{
	Iterator<ValueType> cIterator(*(new SimpleMapIterator<KeyType, ValueType, Comparer>(*this, nIndex)));
	return cIterator;
}

template <class KeyType, class ValueType, class Comparer>
ConstIterator<ValueType> SimpleMap<KeyType, ValueType, Comparer>::GetConstIterator(uint32 nIndex) const
{
	ConstIterator<ValueType> cIterator(*(new SimpleMapIterator<KeyType, ValueType, Comparer>(*this, nIndex)));
	return cIterator;
}

template <class KeyType, class ValueType, class Comparer>
Iterator<ValueType> SimpleMap<KeyType, ValueType, Comparer>::GetEndIterator() const
{
	Iterator<ValueType> cIterator(*(new SimpleMapIterator<KeyType, ValueType, Comparer>(*this)));
	return cIterator;
}

template <class KeyType, class ValueType, class Comparer>
ConstIterator<ValueType> SimpleMap<KeyType, ValueType, Comparer>::GetConstEndIterator() const
{
	ConstIterator<ValueType> cIterator(*(new SimpleMapIterator<KeyType, ValueType, Comparer>(*this)));
	return cIterator;
}


//[-------------------------------------------------------]
//[ Public virtual Map functions                          ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer>
void SimpleMap<KeyType, ValueType, Comparer>::Clear()
{
	// Clear the list of elements
	m_lstElements.Clear();
}

template <class KeyType, class ValueType, class Comparer>
bool SimpleMap<KeyType, ValueType, Comparer>::IsEmpty() const
{
	return m_lstElements.IsEmpty();
}

template <class KeyType, class ValueType, class Comparer>
uint32 SimpleMap<KeyType, ValueType, Comparer>::GetNumOfElements() const
{
	return m_lstElements.GetNumOfElements();
}

template <class KeyType, class ValueType, class Comparer>
bool SimpleMap<KeyType, ValueType, Comparer>::Add(const KeyType &Key, const ValueType &Value)
{
	// Add the element
	Element &cElement = m_lstElements.Add();
	cElement.Key   = Key;
	cElement.Value = Value;

	// Done
	return true;
}

template <class KeyType, class ValueType, class Comparer>
bool SimpleMap<KeyType, ValueType, Comparer>::Replace(const KeyType &Key, const ValueType &NewValue)
{
	// Search for that key
	Iterator<Element> cIterator = m_lstElements.GetIterator();
	while (cIterator.HasNext()) {
		Element &cElement = cIterator.Next();
		if (Comparer::AreEqual(cElement.Key, Key)) {
			// Set the new value
			cElement.Value = NewValue;

			// Done
			return true;
		}
	}

	// Error, there's no element with this key!
	return false;
}

template <class KeyType, class ValueType, class Comparer>
bool SimpleMap<KeyType, ValueType, Comparer>::Set(const KeyType &Key, const ValueType &Value)
{
	// Search for that key
	Iterator<Element> cIterator = m_lstElements.GetIterator();
	while (cIterator.HasNext()) {
		Element &cElement = cIterator.Next();
		if (Comparer::AreEqual(cElement.Key, Key)) {
			// Set the new value
			cElement.Value = Value;

			// Value was replaced
			return true;
		}
	}

	// Add the element
	Element &cElement = m_lstElements.Add();
	cElement.Key   = Key;
	cElement.Value = Value;

	// New element was added
	return false;
}

template <class KeyType, class ValueType, class Comparer>
bool SimpleMap<KeyType, ValueType, Comparer>::Remove(const KeyType &Key)
{
	// Search for that key
	Iterator<Element> cIterator = m_lstElements.GetIterator();
	while (cIterator.HasNext()) {
		Element &cElement = cIterator.Next();
		if (Comparer::AreEqual(cElement.Key, Key)) {
			// Remove that element from the list
			m_lstElements.RemoveElement(cElement);

			// Done
			return true;
		}
	}

	// Error, there's no element with this key!
	return false;
}

template <class KeyType, class ValueType, class Comparer>
uint32 SimpleMap<KeyType, ValueType, Comparer>::RemoveValue(const ValueType &Value)
{
	// List of elements to remove
	List<Element*> lstToRemove;

	// Loop through all elements
	Iterator<Element> cIterator = m_lstElements.GetIterator();
	while (cIterator.HasNext()) {
		Element &cElement = cIterator.Next();
		if (cElement.Value == Value) {
			// Add this element to the list of elements to remove - we can't remove the
			// element here because this may cause problems with the iterator...
			lstToRemove.Add(&cElement);
		}
	}

	// Remove the elements
	Iterator<Element*> cRemoveIterator = lstToRemove.GetIterator();
	while (cRemoveIterator.HasNext()) {
		// Remove that element from the list
		m_lstElements.RemoveElement(*cRemoveIterator.Next());
	}

	// Return the number of removed elements
	return lstToRemove.GetNumOfElements();
}

template <class KeyType, class ValueType, class Comparer>
const ValueType &SimpleMap<KeyType, ValueType, Comparer>::Get(const KeyType &Key) const
{
	// Search for that key
	Iterator<Element> cIterator = m_lstElements.GetIterator();
	while (cIterator.HasNext()) {
		Element &cElement = cIterator.Next();
		if (Comparer::AreEqual(cElement.Key, Key)) {
			// Return the value of this element
			return cElement.Value;
		}
	}

	// Error!
	return SimpleMap<KeyType, ValueType, Comparer>::Null;
}

template <class KeyType, class ValueType, class Comparer>
ValueType &SimpleMap<KeyType, ValueType, Comparer>::Get(const KeyType &Key)
{
	// Search for that key
	Iterator<Element> cIterator = m_lstElements.GetIterator();
	while (cIterator.HasNext()) {
		Element &cElement = cIterator.Next();
		if (Comparer::AreEqual(cElement.Key, Key)) {
			// Return the value of this element
			return cElement.Value;
		}
	}

	// Error!
	return SimpleMap<KeyType, ValueType, Comparer>::Null;
}

template <class KeyType, class ValueType, class Comparer>
Iterator<KeyType> SimpleMap<KeyType, ValueType, Comparer>::GetKeyIterator(uint32 nIndex) const
{
	Iterator<KeyType> cIterator(*(new SimpleMapKeyIterator<KeyType, ValueType, Comparer>(*this, nIndex)));
	return cIterator;
}

template <class KeyType, class ValueType, class Comparer>
ConstIterator<KeyType> SimpleMap<KeyType, ValueType, Comparer>::GetConstKeyIterator(uint32 nIndex) const
{
	ConstIterator<KeyType> cIterator(*(new SimpleMapKeyIterator<KeyType, ValueType, Comparer>(*this, nIndex)));
	return cIterator;
}

template <class KeyType, class ValueType, class Comparer>
Iterator<KeyType> SimpleMap<KeyType, ValueType, Comparer>::GetEndKeyIterator() const
{
	Iterator<KeyType> cIterator(*(new SimpleMapKeyIterator<KeyType, ValueType, Comparer>(*this)));
	return cIterator;
}

template <class KeyType, class ValueType, class Comparer>
ConstIterator<KeyType> SimpleMap<KeyType, ValueType, Comparer>::GetConstEndKeyIterator() const
{
	ConstIterator<KeyType> cIterator(*(new SimpleMapKeyIterator<KeyType, ValueType, Comparer>(*this)));
	return cIterator;
}


//[-------------------------------------------------------]
//[ Private virtual Map functions                         ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer>
Map<KeyType, ValueType> &SimpleMap<KeyType, ValueType, Comparer>::operator =(const Map<KeyType, ValueType> &cMap)
{
	// Not implemented

	// Return this instance
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
