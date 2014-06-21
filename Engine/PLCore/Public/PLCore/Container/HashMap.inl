/*********************************************************\
 *  File: HashMap.inl                                    *
 *      Hash map template implementation
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
#include "PLCore/Container/HashMapIterator.h"
#include "PLCore/Container/HashMapKeyIterator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public static variables                               ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower> ValueType HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::Null;


//[-------------------------------------------------------]
//[ SlotsList implementation                              ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::SlotsList::SlotsList() :
	m_pFirstSlot(nullptr),
	m_pLastSlot(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::SlotsList::~SlotsList()
{
	Clear();
}

/**
*  @brief
*    Copy operator
*/
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
void HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::SlotsList::operator =(const SlotsList &cSource)
{
	// Clear the old data
	if (m_pFirstSlot)
		Clear();

	// Add new data
	const Slot *pSlot = cSource.m_pFirstSlot;
	while (pSlot) {
		Add(pSlot->Key, pSlot->Value);
		pSlot = pSlot->pNextSlot;
	}
}

/**
*  @brief
*    Clears the hash slots
*/
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
void HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::SlotsList::Clear()
{
	Slot *pSlot = m_pFirstSlot;
	while (pSlot) {
		Slot *pSlotNext = pSlot->pNextSlot;
		delete pSlot;
		pSlot = pSlotNext;
	}
	m_pFirstSlot = m_pLastSlot = nullptr;
}

/**
*  @brief
*    Adds a new element to the hash map slot list
*/
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
void HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::SlotsList::Add(const KeyType &Key, const ValueType &Value)
{
	// Add the hash slot
	Slot *pSlot = new Slot;
	pSlot->Key   = Key;
	pSlot->Value = Value;

	// Add hash slot to the list
	if (m_pFirstSlot) {
		pSlot->pNextSlot       = nullptr;
		pSlot->pPreviousSlot   = m_pLastSlot;
		m_pLastSlot->pNextSlot = pSlot;
	} else {
		pSlot->pNextSlot     = nullptr;
		pSlot->pPreviousSlot = nullptr;
		m_pFirstSlot         = pSlot;
	}
	m_pLastSlot = pSlot;
}

/**
*  @brief
*    Replaces the value of a map element
*/
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
bool HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::SlotsList::Replace(const KeyType &Key, const ValueType &NewValue)
{
	// Find the element
	Slot *pSlot = m_pFirstSlot;
	while (pSlot) {
		// Compare keys
		if (Comparer::AreEqual(pSlot->Key, Key)) {
			// Set the new key
			pSlot->Value = NewValue;

			// Done
			return true;
		}

		// Get next internal slot
		pSlot = pSlot->pNextSlot;
	}

	// Error, there's no element with this key!
	return false;
}

/**
*  @brief
*    Replaces the value of a map element
*/
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
bool HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::SlotsList::Set(const KeyType &Key, const ValueType &Value)
{
	// Find the element
	Slot *pSlot = m_pFirstSlot;
	while (pSlot) {
		// Compare keys
		if (Comparer::AreEqual(pSlot->Key, Key)) {
			// Set the new key
			pSlot->Value = Value;

			// Value was replaced
			return true;
		}

		// Get next internal slot
		pSlot = pSlot->pNextSlot;
	}

	// Add the hash slot
	pSlot = new Slot;
	pSlot->Key   = Key;
	pSlot->Value = Value;

	// Add hash slot to the list
	if (m_pFirstSlot) {
		pSlot->pNextSlot       = nullptr;
		pSlot->pPreviousSlot   = m_pLastSlot;
		m_pLastSlot->pNextSlot = pSlot;
	} else {
		pSlot->pNextSlot     = nullptr;
		pSlot->pPreviousSlot = nullptr;
		m_pFirstSlot         = pSlot;
	}
	m_pLastSlot = pSlot;

	// New element was added
	return false;
}

/**
*  @brief
*    Removes an element from the hash map slot list
*/
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
bool HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::SlotsList::Remove(const KeyType &Key)
{
	// Find the element
	Slot *pSlot = m_pFirstSlot, *pSlotPrev = nullptr;
	while (pSlot) {
		// Compare keys
		if (Comparer::AreEqual(pSlot->Key, Key)) {
			// Is this the first internal slot?
			if (pSlot == m_pFirstSlot) {
				// Is this also the last internal slot?
				if (pSlot == m_pLastSlot) {
					m_pFirstSlot = m_pLastSlot = nullptr;
				} else {
					m_pFirstSlot = pSlot->pNextSlot;
					m_pFirstSlot->pPreviousSlot = nullptr;
				}

			// Else...
			} else {
				// Update connectivity
				pSlotPrev->pNextSlot = pSlot->pNextSlot;

				// Is this also the last internal slot?
				if (pSlot == m_pLastSlot)
					m_pLastSlot = pSlotPrev;
				else
					pSlot->pNextSlot->pPreviousSlot = pSlotPrev;
			}

			// Destroy the internal slot
			delete pSlot;

			// Done
			return true;
		}

		// Get next internal slot
		pSlotPrev = pSlot;
		pSlot     = pSlot->pNextSlot;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Removes all elements with a certain value from the map
*/
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
uint32 HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::SlotsList::RemoveValue(const ValueType &Value)
{
	// Find the element
	uint32 nRemoved = 0;
	Slot *pSlot = m_pFirstSlot, *pSlotPrev = nullptr;
	while (pSlot) {
		// Compare values
		if (pSlot->Value == Value) {
			// Is this the first internal slot?
			if (pSlot == m_pFirstSlot) {
				// Is this also the last internal slot?
				if (pSlot == m_pLastSlot) {
					m_pFirstSlot = m_pLastSlot = nullptr;
				} else {
					m_pFirstSlot = pSlot->pNextSlot;
					m_pFirstSlot->pPreviousSlot = nullptr;
				}

			// Else...
			} else {
				// Update connectivity
				pSlotPrev->pNextSlot = pSlot->pNextSlot;

				// Is this also the last internal slot?
				if (pSlot == m_pLastSlot)
					m_pLastSlot = pSlotPrev;
				else
					pSlot->pNextSlot->pPreviousSlot = pSlotPrev;
			}

			// Destroy the internal slot
			Slot *pSlotT = pSlot;
			pSlot = pSlot->pNextSlot;
			delete pSlotT;

			// Update the number of removed elements and continue
			nRemoved++;

		// Get next internal slot
		} else {
			pSlotPrev = pSlot;
			pSlot     = pSlot->pNextSlot;
		}
	}

	// Return the number of removed elements
	return nRemoved;
}

/**
*  @brief
*    Returns the value of a given key
*/
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
const ValueType &HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::SlotsList::Get(const KeyType &Key) const
{
	// Find the element
	const Slot *pSlot = m_pFirstSlot;
	while (pSlot) {
		// Compare keys
		if (Comparer::AreEqual(pSlot->Key, Key))
			return pSlot->Value;

		// Get next internal slot
		pSlot = pSlot->pNextSlot;
	}

	// Error!
	return HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::Null;
}

/**
*  @brief
*    Returns the value of a given key
*/
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
ValueType &HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::SlotsList::Get(const KeyType &Key)
{
	// Find the element
	Slot *pSlot = m_pFirstSlot;
	while (pSlot) {
		// Compare keys
		if (Comparer::AreEqual(pSlot->Key, Key))
			return pSlot->Value;

		// Get next internal slot
		pSlot = pSlot->pNextSlot;
	}

	// Error!
	return HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::Null;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::SlotsList::SlotsList(const SlotsList &cSource) :
	m_pFirstSlot(nullptr),
	m_pLastSlot(nullptr)
{
	// Not implemented
}




//[-------------------------------------------------------]
//[ HashMap implementation                                ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::HashMap(uint32 nNumOfSlots) :
	m_nNumOfSlots((nNumOfSlots > 0) ? nNumOfSlots : 1),
	m_plstSlots(nullptr),
	m_nNumOfElements(0)
{
}

/**
*  @brief
*    Copy constructor
*/
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::HashMap(const HashMap<KeyType, ValueType, Hasher, Comparer, Grower> &cSource) :
	m_nNumOfSlots(cSource.m_nNumOfSlots),
	m_plstSlots(cSource.m_nNumOfElements ? new SlotsList[m_nNumOfSlots] : nullptr),
	m_nNumOfElements(cSource.m_nNumOfElements)
{
	// Copy slots
	if (m_plstSlots) {
		for (uint32 i=0; i<m_nNumOfSlots; i++)
			m_plstSlots[i] = cSource.m_plstSlots[i];
	}
}

/**
*  @brief
*    Destructor
*/
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::~HashMap()
{
	// Destroy the slots
	if (m_plstSlots)
		delete [] m_plstSlots;
}

/**
*  @brief
*    Copy operator
*/
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
Map<KeyType, ValueType> &HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::operator =(const HashMap<KeyType, ValueType, Hasher, Comparer, Grower> &cSource)
{
	// Clear the old data
	Clear();

	// Copy data
	m_nNumOfSlots    = cSource.m_nNumOfSlots;
	m_plstSlots      = cSource.m_nNumOfElements ? new SlotsList[m_nNumOfSlots] : nullptr;
	m_nNumOfElements = cSource.m_nNumOfElements;

	// Copy slots
	if (m_plstSlots) {
		for (uint32 i=0; i<m_nNumOfSlots; i++)
			m_plstSlots[i] = cSource.m_plstSlots[i];
	}

	// Return this instance
	return *this;
}

/**
*  @brief
*    Returns the current number of slots
*/
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
uint32 HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::GetNumOfSlots() const
{
	return m_nNumOfSlots;
}

/**
*  @brief
*    Returns some statistics
*/
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
void HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::GetStatistics(uint32 &nNumOfCollisions, uint32 &nNumOfFreeSlotsLists) const
{
	// Init statistics
	nNumOfCollisions     = 0;
	nNumOfFreeSlotsLists = 0;

	// Slots already created?
	if (m_plstSlots) {
		// Get statistics
		for (uint32 i=0; i<m_nNumOfSlots; i++) {
			const SlotsList *pList = &m_plstSlots[i];
			if (pList->m_pFirstSlot) {
				const Slot *pSlot = pList->m_pFirstSlot;
				while (pSlot) {
					// Get next internal slot
					pSlot = pSlot->NextSlot;
					if (pSlot)
						nNumOfCollisions++;
				}
			} else {
				nNumOfFreeSlotsLists++;
			}
		}
	} else {
		nNumOfFreeSlotsLists = m_nNumOfSlots;
	}
}


//[-------------------------------------------------------]
//[ Public virtual Iterable functions                     ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
Iterator<ValueType> HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::GetIterator(uint32 nIndex) const
{
	Iterator<ValueType> cIterator(*(new HashMapIterator<KeyType, ValueType, Hasher, Comparer, Grower>(*this, nIndex)));
	return cIterator;
}

template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
ConstIterator<ValueType> HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::GetConstIterator(uint32 nIndex) const
{
	ConstIterator<ValueType> cIterator(*(new HashMapIterator<KeyType, ValueType, Hasher, Comparer, Grower>(*this, nIndex)));
	return cIterator;
}

template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
Iterator<ValueType> HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::GetEndIterator() const
{
	Iterator<ValueType> cIterator(*(new HashMapIterator<KeyType, ValueType, Hasher, Comparer, Grower>(*this)));
	return cIterator;
}

template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
ConstIterator<ValueType> HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::GetConstEndIterator() const
{
	ConstIterator<ValueType> cIterator(*(new HashMapIterator<KeyType, ValueType, Hasher, Comparer, Grower>(*this)));
	return cIterator;
}


//[-------------------------------------------------------]
//[ Public virtual Map functions                          ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
void HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::Clear()
{
	// Slots already created?
	if (m_plstSlots) {
		// Clear all slots
		for (uint32 i=0; i<m_nNumOfSlots; i++)
			m_plstSlots[i].Clear();

		// Update the number of elements counter
		m_nNumOfElements = 0;
	}
}

template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
bool HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::IsEmpty() const
{
	return (m_nNumOfElements == 0);
}

template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
uint32 HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::GetNumOfElements() const
{
	return m_nNumOfElements;
}

template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
bool HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::Add(const KeyType &Key, const ValueType &Value)
{
	// Slots already created?
	if (m_plstSlots) {
		// Do we have to resize the hash map now?
		uint32 nFreeSlots     = (m_nNumOfSlots > m_nNumOfElements) ? m_nNumOfSlots-m_nNumOfElements : 0;
		uint32 nNewNumOfSlots = Grower::GetNewSize(m_nNumOfSlots, nFreeSlots);
		if (nNewNumOfSlots > m_nNumOfSlots) {
			// Create the new hash map
			SlotsList *plstSlots = new SlotsList[nNewNumOfSlots];

			// Add the elements from the old hash map into the new one
			SlotsList *pSlotsList = m_plstSlots;
			for (uint32 nSlotsList=0; nSlotsList<m_nNumOfSlots; nSlotsList++, pSlotsList++) {
				Slot *pSlot = pSlotsList->m_pFirstSlot;
				while (pSlot) {
					// Hash the key
					uint32 nIndex = Hasher::Hash(pSlot->Key) % nNewNumOfSlots;

					// Add the element to the hash map
					plstSlots[nIndex].Add(pSlot->Key, pSlot->Value);

					// Get the next slot
					pSlot = pSlot->pNextSlot;
				}
			}

			// Delete the old hash map and set the new one
			delete [] m_plstSlots;
			m_plstSlots   = plstSlots;
			m_nNumOfSlots = nNewNumOfSlots;
		}
	} else {
		m_plstSlots = new SlotsList[m_nNumOfSlots];
	}

	// Hash the key
	const uint32 nIndex = Hasher::Hash(Key) % m_nNumOfSlots;

	// Add the element to the hash map
	m_plstSlots[nIndex].Add(Key, Value);

	// Update the number of elements counter
	m_nNumOfElements++;

	// Done
	return true;
}

template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
bool HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::Replace(const KeyType &Key, const ValueType &NewValue)
{
	// Slots already created?
	if (m_plstSlots) {
		// Hash the key
		const uint32 nIndex = Hasher::Hash(Key) % m_nNumOfSlots;

		// Replace the value (or at least try it ;-)
		return m_plstSlots[nIndex].Replace(Key, NewValue);
	} else {
		// Error!
		return false;
	}
}

template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
bool HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::Set(const KeyType &Key, const ValueType &Value)
{
	// Slots already created?
	if (m_plstSlots) {
		// Hash the key
		const uint32 nIndex = Hasher::Hash(Key) % m_nNumOfSlots;

		// Set the value
		if (m_plstSlots[nIndex].Set(Key, Value))
			return true; // Value was replaced
		else {
			// Update the number of elements counter
			m_nNumOfElements++;

			// New element was added
			return false;
		}
	} else {
		// Just call the add function
		Add(Key, Value);

		// New element was added
		return false;
	}
}

template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
bool HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::Remove(const KeyType &Key)
{
	// Slots already created?
	if (m_plstSlots) {
		// Hash the key
		const uint32 nIndex = Hasher::Hash(Key) % m_nNumOfSlots;

		// Remove the element from the hash map (or at least try it ;-)
		if (m_plstSlots[nIndex].Remove(Key)) {
			// Update the number of elements counter
			m_nNumOfElements--;

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
uint32 HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::RemoveValue(const ValueType &Value)
{
	// Slots already created?
	if (m_plstSlots) {
		// Loop through all slots
		uint32 nRemoved = 0;
		for (uint32 i=0; i<m_nNumOfSlots; i++)
			nRemoved += m_plstSlots[i].RemoveValue(Value);

		// Update the number of elements counter
		m_nNumOfElements -= nRemoved;

		// Return the number of removed elements
		return nRemoved;
	} else {
		return 0;
	}
}

template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
const ValueType &HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::Get(const KeyType &Key) const
{
	// Slots already created?
	if (m_plstSlots) {
		// Hash the key
		const uint32 nIndex = Hasher::Hash(Key) % m_nNumOfSlots;

		// Return the object (or at least try it ;-)
		return m_plstSlots[nIndex].Get(Key);
	} else {
		return HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::Null;
	}
}

template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
ValueType &HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::Get(const KeyType &Key)
{
	// Slots already created?
	if (m_plstSlots) {
		// Hash the key
		const uint32 nIndex = Hasher::Hash(Key) % m_nNumOfSlots;

		// Return the object (or at least try it ;-)
		return m_plstSlots[nIndex].Get(Key);
	} else {
		return HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::Null;
	}
}

template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
Iterator<KeyType> HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::GetKeyIterator(uint32 nIndex) const
{
	Iterator<KeyType> cIterator(*(new HashMapKeyIterator<KeyType, ValueType, Hasher, Comparer, Grower>(*this, nIndex)));
	return cIterator;
}

template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
ConstIterator<KeyType> HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::GetConstKeyIterator(uint32 nIndex) const
{
	ConstIterator<KeyType> cIterator(*(new HashMapKeyIterator<KeyType, ValueType, Hasher, Comparer, Grower>(*this, nIndex)));
	return cIterator;
}

template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
Iterator<KeyType> HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::GetEndKeyIterator() const
{
	Iterator<KeyType> cIterator(*(new HashMapKeyIterator<KeyType, ValueType, Hasher, Comparer, Grower>(*this)));
	return cIterator;
}

template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
ConstIterator<KeyType> HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::GetConstEndKeyIterator() const
{
	ConstIterator<KeyType> cIterator(*(new HashMapKeyIterator<KeyType, ValueType, Hasher, Comparer, Grower>(*this)));
	return cIterator;
}


//[-------------------------------------------------------]
//[ Private virtual Map functions                         ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
Map<KeyType, ValueType> &HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::operator =(const Map<KeyType, ValueType> &cMap)
{
	// Not implemented

	// Return this instance
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
