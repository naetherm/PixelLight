/*********************************************************\
 *  File: Bitset.cpp                                     *
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
#include "PLCore/Tools/Wrapper.h"
#include "PLCore/Core/MemoryManager.h"
#include "PLCore/Container/BitsetIterator.h"
#include "PLCore/Container/Bitset.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public static variables                               ]
//[-------------------------------------------------------]
bool Bitset::Null = false;


//[-------------------------------------------------------]
//[ Private static variables                              ]
//[-------------------------------------------------------]
bool Bitset::m_bBit = false;


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Bitset::Bitset(uint32 nMaxNumOfElements, bool bAdded, bool bInit) :
	m_nMaxNumOfElements(0),
	m_nNumOfElements(0),
	m_nNumOfIntegers(0),
	m_pnIntegers(nullptr),
	m_nResizeCount(64)
{
	// Resize
	Resize(nMaxNumOfElements, bAdded, bInit);
}

/**
*  @brief
*    Copy constructor
*/
Bitset::Bitset(const Bitset &lstSource, uint32 nStart, uint32 nCount) :
	m_nMaxNumOfElements(0),
	m_nNumOfElements(0),
	m_nNumOfIntegers(0),
	m_pnIntegers(nullptr),
	m_nResizeCount(lstSource.m_nResizeCount)
{
	// Copy
	Copy(static_cast<const Container<bool>&>(lstSource), nStart, nCount);
}

/**
*  @brief
*    Copy constructor
*/
Bitset::Bitset(const Container<bool> &lstContainer, uint32 nStart, uint32 nCount) :
	m_nMaxNumOfElements(0),
	m_nNumOfElements(0),
	m_nNumOfIntegers(0),
	m_pnIntegers(nullptr),
	m_nResizeCount(64)
{
	// Copy
	Copy(lstContainer, nStart, nCount);
}

/**
*  @brief
*    Destructor
*/
Bitset::~Bitset()
{
	Clear();
}

/**
*  @brief
*    Sets the number of elements in the bit set
*/
bool Bitset::Resize(uint32 nMaxNumOfElements, bool bAdded, bool bInit)
{
	// Check parameters
	if (m_nMaxNumOfElements == nMaxNumOfElements) {
		// All elements added by default?
		if (bAdded) {
			// Initialize 'new' elements
			if (bInit && nMaxNumOfElements > m_nNumOfElements) {
				for (uint32 i=m_nNumOfElements; i<nMaxNumOfElements; i++) {
					// Transparent...
					// Clear(i);

					// Performant... ;-)
					m_pnIntegers[i >> 5] &= ~(1 << (i & 31));
				}
			}
			m_nNumOfElements = m_nMaxNumOfElements;
		}
	} else {
		// Destroy the bitset?
		if (nMaxNumOfElements) { // Resize bitset
			// Allocate the bits and initialize them
			uint32 nOldSize = m_nNumOfIntegers;
			m_nMaxNumOfElements = nMaxNumOfElements;
			m_nNumOfIntegers	= static_cast<uint32>(Wrapper::Ceil(static_cast<float>(m_nMaxNumOfElements)/(sizeof(uint32)*8)));
			if (!m_nNumOfIntegers)
				m_nNumOfIntegers = 1; // We need at least ONE integer!
			if (m_nNumOfIntegers != nOldSize)
				m_pnIntegers = static_cast<uint32*>(MemoryManager::Reallocator(m_pnIntegers, m_nNumOfIntegers*sizeof(uint32)));
			if (m_nNumOfElements > m_nMaxNumOfElements)
				m_nNumOfElements = m_nMaxNumOfElements-1;
			else {
				// Initialize the new bits?
				if (bInit) {
					// Initialize all new integers at once
					if (m_nNumOfIntegers > nOldSize)
						MemoryManager::Set(&m_pnIntegers[nOldSize], 0, sizeof(uint32)*(m_nNumOfIntegers-nOldSize));

					// Initialize remaining bits
					nOldSize = nOldSize*sizeof(uint32)*8; // Get number of old maximum bits
					for (uint32 i=m_nNumOfElements; i<nOldSize; i++) {
						// Transparent...
						// Clear(i);

						// Performant... ;-)
						m_pnIntegers[i >> 5] &= ~(1 << (i & 31));
					}
				}
			}
			if (bAdded)
				m_nNumOfElements = m_nMaxNumOfElements;
		} else {
			Clear();
		}
	}

	// Done
	return true;
}

/**
*  @brief
*    Clears all bits to 0
*/
void Bitset::ClearAll()
{
	if (m_pnIntegers)
		MemoryManager::Set(m_pnIntegers, 0, sizeof(uint32)*m_nNumOfIntegers);
}

/**
*  @brief
*    Sets all bits to 1
*/
void Bitset::SetAll()
{
	if (m_pnIntegers)
		MemoryManager::Set(m_pnIntegers, 0xFFFF, sizeof(uint32)*m_nNumOfIntegers);
}

/**
*  @brief
*    Returns the number of set bits (value = 'true')
*/
uint32 Bitset::GetNumOfSetBits() const
{
	uint32 nNumOfSetBits = 0;

	// Loop through all bits
	for (uint32 i=0; i<m_nNumOfElements; i++) {
//		Transparent...
//		if (IsSet(i)) nNumOfSetBits++;
		// Performant... ;-)
		if (m_pnIntegers[i >> 5] & (1 << (i & 31)))
			nNumOfSetBits++;
	}

	// Return the number of set bits
	return nNumOfSetBits;
}


//[-------------------------------------------------------]
//[ Public virtual Iterable functions                     ]
//[-------------------------------------------------------]
Iterator<bool> Bitset::GetIterator(uint32 nIndex) const
{
	Iterator<bool> cIterator(*(new BitsetIterator(*this, nIndex)));
	return cIterator;
}

ConstIterator<bool> Bitset::GetConstIterator(uint32 nIndex) const
{
	ConstIterator<bool> cIterator(*(new BitsetIterator(*this, nIndex)));
	return cIterator;
}

Iterator<bool> Bitset::GetEndIterator() const
{
	Iterator<bool> cIterator(*(new BitsetIterator(*this)));
	return cIterator;
}

ConstIterator<bool> Bitset::GetConstEndIterator() const
{
	ConstIterator<bool> cIterator(*(new BitsetIterator(*this)));
	return cIterator;
}


//[-------------------------------------------------------]
//[ Public virtual Container functions                    ]
//[-------------------------------------------------------]
void Bitset::Clear()
{
	// Clear data
	if (m_pnIntegers) {
		delete [] m_pnIntegers;
		m_pnIntegers = nullptr;
	}

	// Init data
	m_nMaxNumOfElements = 0;
	m_nNumOfElements    = 0;
	m_nNumOfIntegers	= 0;
}

bool &Bitset::Add()
{
	// Check whether the bit set is full and we have to resize it
	if (m_nNumOfElements >= m_nMaxNumOfElements) {
		if (!m_nResizeCount || !Resize(m_nMaxNumOfElements+m_nResizeCount, false, false))
			return Bitset::Null; // Error!
	}

	// Add element
	m_nNumOfElements++;

	// Return the new element
	return Get(m_nNumOfElements-1);
}

bool &Bitset::Add(const bool &Element)
{
	// Check whether the bit set is full and we have to resize it
	if (m_nNumOfElements >= m_nMaxNumOfElements) {
		if (!m_nResizeCount || !Resize(m_nMaxNumOfElements+m_nResizeCount, false, false))
			return Bitset::Null; // Error!
	}

	// Add element
	const uint32 nOldNumOfElements = m_nNumOfElements;
	m_nNumOfElements++;
	if (Element)
		Set(nOldNumOfElements);
	else
		Clear(nOldNumOfElements);

	// Done
	return Get(nOldNumOfElements);
}

uint32 Bitset::Add(const bool *pElements, uint32 nCount)
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

	// Set the bits
	const uint32 nOldNumOfElements = m_nNumOfElements;
	m_nNumOfElements += nCount;
	const bool *pElement = pElements;
	for (uint32 i=nOldNumOfElements; i<m_nNumOfElements; i++, pElement++) {
		if (*pElement)
			Set(i);
		else
			Clear(i);
	}

	// Done
	return nCount;
}

bool Bitset::Add(const Container<bool> &lstContainer, uint32 nStart, uint32 nCount)
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
		if (&Add(lstContainer[i]) == &Bitset::Null)
			return false; // Error! (maybe bit set is full)
	}

	// Done
	return true;
}

bool &Bitset::AddAtIndex(int nIndex)
{
	// Add at end?
	if (nIndex < 0)
		return Add();

	// Check index
	if (nIndex > static_cast<int>(m_nNumOfElements))
		return Bitset::Null; // Error!

	// Check whether the bit set is full and we have to resize it
	if (m_nNumOfElements >= m_nMaxNumOfElements) {
		if (!m_nResizeCount || !Resize(m_nMaxNumOfElements+m_nResizeCount, false, false))
			return Bitset::Null; // Error!
	}

	// Add element
	m_nNumOfElements++;

	// Shift element behind index
	for (uint32 i=m_nNumOfElements-1; i>static_cast<uint32>(nIndex); i--) {
		if (IsSet(i-1))
			Set(i);
		else
			Clear(i);
	}

	// Return the new element
	return Get(nIndex);
}

bool &Bitset::AddAtIndex(const bool &Element, int nIndex)
{
	// Add at end?
	if (nIndex < 0)
		return Add(Element);

	// Check index
	if (nIndex > static_cast<int>(m_nNumOfElements))
		return Bitset::Null; // Error!

	// Check whether the bit set is full and we have to resize it
	if (m_nNumOfElements >= m_nMaxNumOfElements) {
		if (!m_nResizeCount || !Resize(m_nMaxNumOfElements+m_nResizeCount, false, false))
			return Bitset::Null; // Error!
	}

	// Add element
	m_nNumOfElements++;

	// Shift element behind index
	for (uint32 i=m_nNumOfElements-1; i>static_cast<uint32>(nIndex); i--) {
		if (IsSet(i-1))
			Set(i);
		else
			Clear(i);
	}

	// Add element
	if (Element)
		Set(nIndex);
	else
		Clear(nIndex);

	// Done
	return Get(nIndex);
}

bool Bitset::RemoveAtIndex(uint32 nIndex)
{
	// Check index
	if (nIndex >= m_nNumOfElements)
		return false; // Error!

	// Shift element behind index
	for (uint32 i=nIndex; i<m_nNumOfElements-1; i++) {
		if (IsSet(i+1))
			Set(i);
		else
			Clear(i);
	}

	// Remove element
	m_nNumOfElements--;

	// Check whether the array can be reduced
	if (m_nResizeCount) {
		const uint32 nNewNumOfElements = (m_nMaxNumOfElements > m_nResizeCount) ? m_nMaxNumOfElements-m_nResizeCount : 0;
		if (m_nNumOfElements <= nNewNumOfElements)
			Resize(nNewNumOfElements, false, false);
	}

	// Done
	return true;
}

bool Bitset::Remove(const Container<bool> &lstContainer, uint32 nStart, uint32 nCount)
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

bool Bitset::Copy(const Container<bool> &lstContainer, uint32 nStart, uint32 nCount)
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
		// Get the number of elements to copy
		if (!nCount)
			nCount = lstContainer.GetNumOfElements()-nStart;
		if (nStart+nCount > lstContainer.GetNumOfElements())
			nCount = lstContainer.GetNumOfElements()-nStart;

		// Setup bit set size
		Resize(nCount);

		// Copy
		for (uint32 i=0; i<nCount; i++) {
			if (lstContainer[i+nStart])
				Set(i);
			else
				Clear(i);
		}
		m_nNumOfElements = nCount;
	}

	// Done
	return true;
}

bool Bitset::Compare(const Container<bool> &lstContainer, uint32 nStart, uint32 nCount) const
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
			if (IsSet(i) != lstContainer[i])
				return false; // The two containers are not equal!
		}
	}

	// The two containers are equal!
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
