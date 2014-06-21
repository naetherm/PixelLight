/*********************************************************\
 *  File: Bitset.inl                                     *
 *      Bitset template implementation
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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy operator
*/
inline Container<bool> &Bitset::operator =(const Bitset &lstSource)
{
	return *this = static_cast<const Container<bool>&>(lstSource);
}

/**
*  @brief
*    Returns the maximum number of elements in the bit set
*/
inline uint32 Bitset::GetMaxNumOfElements() const
{
	return m_nMaxNumOfElements;
}

/**
*  @brief
*    Returns the number of elements automatically added if the bit set
*    size is to small
*/
inline uint32 Bitset::GetResizeCount() const
{
	return m_nResizeCount;
}

/**
*  @brief
*    Sets the number of elements automatically added if the bit set
*    size is to small
*/
inline bool Bitset::SetResizeCount(uint32 nCount)
{
	// Set new resize count
	m_nResizeCount = nCount;

	// Done
	return true;
}

/**
*  @brief
*    Clears a bit to 0
*/
inline bool Bitset::Clear(uint32 nIndex)
{
	// Check index
	if (nIndex >= m_nNumOfElements)
		return false; // Error!

	// Clear bit at the given index
	m_pnIntegers[nIndex >> 5] &= ~(1 << (nIndex & 31));

	// Done
	return true;
}

/**
*  @brief
*    Sets the desired bit
*/
inline bool Bitset::Set(uint32 nIndex)
{
	// Check index
	if (nIndex >= m_nNumOfElements)
		return false; // Error!

	// Set the bit at the given index
	m_pnIntegers[nIndex >> 5] |= (1 << (nIndex & 31));

	// Done
	return true;
}

/**
*  @brief
*    Inverts the desired bit
*/
inline bool Bitset::Invert(uint32 nIndex)
{
	// Check index
	if (nIndex >= m_nNumOfElements)
		return false; // Error!

	// Invert
//	Transparent...
//	if (IsSet(nIndex)) Clear(nIndex);
//	else			   Set(nIndex);
	// Performant... ;-)
	if (m_pnIntegers[nIndex >> 5] & (1 << (nIndex & 31)))
		m_pnIntegers[nIndex >> 5] &= ~(1 << (nIndex & 31));
	else
		m_pnIntegers[nIndex >> 5] |=  (1 << (nIndex & 31));

	// Done
	return true;
}

/**
*  @brief
*    Inverts all bits
*/
inline void Bitset::InvertAll()
{
	// Invert all bits
	for (uint32 i=0; i<m_nNumOfElements; i++) {
//		Transparent...
//		if (IsSet(i)) Clear(i);
//		else		  Set(i);
		// Performant... ;-)
		if (m_pnIntegers[i >> 5] & (1 << (i & 31)))
			m_pnIntegers[i >> 5] &= ~(1 << (i & 31));
		else
			m_pnIntegers[i >> 5] |=  (1 << (i & 31));
	}
}

/**
*  @brief
*    Returns if the desired bit slot is a 1 or a 0
*/
inline bool Bitset::IsSet(uint32 nIndex) const
{
	return ((nIndex < m_nNumOfElements) && (m_pnIntegers[nIndex >> 5] & (1 << (nIndex & 31))));
}

/**
*  @brief
*    Returns the internal number of integers (32 bit) used to store the bits
*/
inline uint32 Bitset::GetNumOfIntegers() const
{
	return m_nNumOfIntegers;
}

/**
*  @brief
*    Returns the internal integers (32 bit) used to store the bits
*/
inline const uint32 *Bitset::GetIntegers() const
{
	return m_pnIntegers;
}

/**
*  @brief
*    Resets the bitset
*/
void Bitset::Reset()
{
	m_nNumOfElements = 0;
}


//[-------------------------------------------------------]
//[ Public virtual Container functions                    ]
//[-------------------------------------------------------]
inline bool Bitset::IsEmpty() const
{
	return (m_nNumOfElements == 0);
}

inline uint32 Bitset::GetNumOfElements() const
{
	return m_nNumOfElements;
}

inline uint32 Bitset::GetElementSize() const
{
	return sizeof(bool);
}

inline uint32 Bitset::GetSize() const
{
	return m_nNumOfIntegers*sizeof(uint32);
}

inline bool Bitset::IsElement(const bool &Element) const
{
	// Find element index
	for (uint32 i=0; i<m_nNumOfElements; i++) {
		if (IsSet(i) == Element)
			return true;
	}

	// The given element is not in the container!
	return false;
}

inline int Bitset::GetIndex(const bool &Element) const
{
	// Find element index
	for (uint32 i=0; i<m_nNumOfElements; i++) {
		if (IsSet(i) == Element)
			return i;
	}

	// The given element is not in the container!
	return -1;
}

inline bool &Bitset::Get(uint32 nIndex) const
{
	// Is the index valid?
	if (nIndex >= m_nNumOfElements)
		return Bitset::Null; // Error!
	m_bBit = IsSet(nIndex);
	return m_bBit;
}

inline bool &Bitset::operator [](uint32 nIndex) const
{
	// Is the index valid?
	if (nIndex >= m_nNumOfElements)
		return Bitset::Null; // Error!
	m_bBit = IsSet(nIndex);
	return m_bBit;
}

inline bool Bitset::Replace(const bool &Element1, const bool &Element2)
{
	// Get the index of the given element
	int nIndex = GetIndex(Element1);
	if (nIndex >= 0) {
		// Replace the element through the new one
		if (Element2)
			Set(nIndex);
		else
			Clear(nIndex);

		// Done
		return true;
	} else {
		// Error, the given element is NOT within the container!
		return false;
	}
}

inline bool Bitset::ReplaceAtIndex(uint32 nIndex, const bool &Element)
{
	// Check index
	if (nIndex >= m_nNumOfElements)
		return false; // Error!

	// Replace the element through the new one
	if (Element)
		Set(nIndex);
	else
		Clear(nIndex);

	// Done
	return true;
}

inline Container<bool> &Bitset::operator +=(const bool &Element)
{
	Add(Element);

	// Return this
	return *this;
}

inline Container<bool> &Bitset::operator +=(const Container<bool> &lstContainer)
{
	Add(lstContainer);

	// Return this
	return *this;
}

inline bool Bitset::Remove(const bool &Element)
{
	// Get the index of the given element
	const int nIndex = GetIndex(Element);
	return (nIndex >= 0 && RemoveAtIndex(nIndex));
}

inline Container<bool> &Bitset::operator -=(const bool &Element)
{
	Remove(Element);

	// Return this
	return *this;
}

inline Container<bool> &Bitset::operator -=(const Container<bool> &lstContainer)
{
	Remove(lstContainer);

	// Return this
	return *this;
}

inline Container<bool> &Bitset::operator =(const Container<bool> &lstContainer)
{
	Copy(lstContainer);

	// Return this
	return *this;
}

inline bool Bitset::operator ==(const Container<bool> &lstContainer) const
{
	return Compare(lstContainer, 0, 0);
}

inline bool Bitset::operator !=(const Container<bool> &lstContainer) const
{
	return !Compare(lstContainer, 0, 0);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
