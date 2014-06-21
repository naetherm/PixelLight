/*********************************************************\
 *  File: HashMapIterator.inl                            *
 *      Hash map iterator template implementation
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
#include "PLCore/Container/HashMap.h"


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
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
HashMapIterator<KeyType, ValueType, Hasher, Comparer, Grower>::HashMapIterator(const HashMap<KeyType, ValueType, Hasher, Comparer, Grower> &mapOwner, uint32 nIndex) :
	m_pmapOwner(&mapOwner),
	m_pNextSlot(nullptr),
	m_pPreviousSlot(nullptr)
{
	// Is there at least one element within the hash map? (we do not need to check whether the slots are already created :)
	if (m_pmapOwner->GetNumOfElements()) {
		// Find start slot
		if (nIndex <= m_pmapOwner->GetNumOfElements()/2) {
			// Find the first slot
			for (m_nNextSlots=0; m_nNextSlots<m_pmapOwner->GetNumOfSlots(); m_nNextSlots++) {
				// Are there any slots within this list?
				if (m_pmapOwner->m_plstSlots[m_nNextSlots].m_pFirstSlot) {
					// Ok, we found a slots list which has any slots within it
					m_pNextSlot = m_pmapOwner->m_plstSlots[m_nNextSlots].m_pFirstSlot;
					break;
				}
			}

			// Find the correct start slot
			m_nPreviousSlots = m_nNextSlots;
			m_pPreviousSlot  = nullptr;
			uint32 nCurrentIndex = 0;
			while (HasNext() && nCurrentIndex < nIndex) {
				m_nPreviousSlots = m_nNextSlots;
				m_pPreviousSlot  = m_pNextSlot;
				Next();
				nCurrentIndex++;
			}
		} else {
			// Find the last slot
			m_nPreviousSlots = m_pmapOwner->GetNumOfSlots()-1;
			int nPreviousSlots = m_nPreviousSlots;
			for (; nPreviousSlots>=0; nPreviousSlots--) {
				// Are there any slots within this list?
				if (m_pmapOwner->m_plstSlots[nPreviousSlots].m_pLastSlot) {
					// Ok, we found a slots list which has any slots within it
					m_pPreviousSlot = m_pmapOwner->m_plstSlots[nPreviousSlots].m_pLastSlot;
					break;
				}
			}
			m_nPreviousSlots = (nPreviousSlots < 0) ? 0 : nPreviousSlots;

			// Find the correct start slot
			m_nNextSlots = m_nPreviousSlots;
			m_pNextSlot  = nullptr;
			uint32 nCurrentIndex = m_pmapOwner->GetNumOfElements()-1;
			while (HasPrevious() && nCurrentIndex > nIndex) {
				m_nNextSlots = m_nPreviousSlots;
				m_pNextSlot  = m_pPreviousSlot;
				Previous();
				nCurrentIndex--;
			}
		}
	}
}

/**
*  @brief
*    Constructor
*/
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
HashMapIterator<KeyType, ValueType, Hasher, Comparer, Grower>::HashMapIterator(const HashMap<KeyType, ValueType, Hasher, Comparer, Grower> &mapOwner) :
	m_pmapOwner(&mapOwner),
	m_pNextSlot(nullptr),
	m_pPreviousSlot(nullptr)
{
	// Is there at least one element within the hash map? (we do not need to check whether the slots are already created :)
	if (m_pmapOwner->GetNumOfElements()) {
		// Find the last slot
		m_nPreviousSlots = m_pmapOwner->GetNumOfSlots()-1;
		int nPreviousSlots = m_nPreviousSlots;
		for (; nPreviousSlots>=0; nPreviousSlots--) {
			// Are there any slots within this list?
			if (m_pmapOwner->m_plstSlots[nPreviousSlots].m_pLastSlot) {
				// Ok, we found a slots list which has any slots within it
				m_pPreviousSlot = m_pmapOwner->m_plstSlots[nPreviousSlots].m_pLastSlot;
				break;
			}
		}
		m_nPreviousSlots = (nPreviousSlots < 0) ? 0 : nPreviousSlots;

		// Find the correct start slot
		m_nNextSlots = m_nPreviousSlots;
		m_pNextSlot  = nullptr;
	}
}

/**
*  @brief
*    Copy constructor
*/
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
HashMapIterator<KeyType, ValueType, Hasher, Comparer, Grower>::HashMapIterator(const HashMapIterator<KeyType, ValueType, Hasher, Comparer, Grower> &cSource) :
	m_pmapOwner(cSource.m_pmapOwner),
	m_nNextSlots(cSource.m_nNextSlots),
	m_pNextSlot(cSource.m_pNextSlot),
	m_nPreviousSlots(cSource.m_nPreviousSlots),
	m_pPreviousSlot(cSource.m_pPreviousSlot)
{
}

/**
*  @brief
*    Destructor
*/
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
HashMapIterator<KeyType, ValueType, Hasher, Comparer, Grower>::~HashMapIterator()
{
}


//[-------------------------------------------------------]
//[ Private virtual IteratorImpl functions                ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
IteratorImpl<ValueType> *HashMapIterator<KeyType, ValueType, Hasher, Comparer, Grower>::Clone() const
{
	return new HashMapIterator<KeyType, ValueType, Hasher, Comparer, Grower>(*this);
}

template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
bool HashMapIterator<KeyType, ValueType, Hasher, Comparer, Grower>::HasNext() const
{
	return (m_pNextSlot != nullptr);
}

template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
ValueType &HashMapIterator<KeyType, ValueType, Hasher, Comparer, Grower>::Next()
{
	// Is there's a next slot?
	if (!m_pNextSlot)
		return HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::Null; // Error!

	// Get the next slot
	m_nPreviousSlots = m_nNextSlots;
	m_pPreviousSlot  = m_pNextSlot;
	m_pNextSlot      = m_pNextSlot->pNextSlot;

	// Is there a next slot?
	if (!m_pNextSlot && m_nNextSlots < m_pmapOwner->GetNumOfSlots()-1) {
		// Ok, now it becomes a bit tricky... look for the next slots list which has any slots within it
		m_nNextSlots++;
		for (; m_nNextSlots<m_pmapOwner->GetNumOfSlots(); m_nNextSlots++) {
			// Are there any slots within this list?
			if (m_pmapOwner->m_plstSlots[m_nNextSlots].m_pFirstSlot) {
				// Ok, we found a slots list which has any slots within it
				m_pNextSlot = m_pmapOwner->m_plstSlots[m_nNextSlots].m_pFirstSlot;
				break;
			}
		}
	}

	// Return the value of the 'current' slot
	return m_pPreviousSlot->Value;
}

template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
bool HashMapIterator<KeyType, ValueType, Hasher, Comparer, Grower>::HasPrevious() const
{
	return (m_pPreviousSlot != nullptr);
}

template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
ValueType &HashMapIterator<KeyType, ValueType, Hasher, Comparer, Grower>::Previous()
{
	// Is there's a previous slot?
	if (!m_pPreviousSlot)
		return HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::Null; // Error!

	// Get the previous slot
	m_nNextSlots    = m_nPreviousSlots;
	m_pNextSlot     = m_pPreviousSlot;
	m_pPreviousSlot = m_pPreviousSlot->pPreviousSlot;

	// Is there a previous slot?
	if (!m_pPreviousSlot && m_nPreviousSlots > 0) {
		// Ok, now it becomes a bit tricky... look for the previous slots list which has any slots within it
		int nPreviousSlots;
		m_nPreviousSlots--;
		for (nPreviousSlots=m_nPreviousSlots; nPreviousSlots>=0; nPreviousSlots--) {
			// Are there any slots within this list?
			if (m_pmapOwner->m_plstSlots[nPreviousSlots].m_pLastSlot) {
				// Ok, we found a slots list which has any slots within it
				m_pPreviousSlot = m_pmapOwner->m_plstSlots[nPreviousSlots].m_pLastSlot;
				break;
			}
		}
		m_nPreviousSlots = (nPreviousSlots < 0) ? 0 : nPreviousSlots;
	}

	// Return the value of the 'current' slot
	return m_pNextSlot->Value;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
