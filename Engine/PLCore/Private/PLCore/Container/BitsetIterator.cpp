/*********************************************************\
 *  File: BitsetIterator.cpp                             *
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
#include "PLCore/Container/Bitset.h"
#include "PLCore/Container/BitsetIterator.h"


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
BitsetIterator::BitsetIterator(const Bitset &lstBitset, uint32 nIndex) :
	m_plstBitset(&lstBitset),
	m_nNextID((nIndex > lstBitset.GetNumOfElements()-1) ? lstBitset.GetNumOfElements()-1 : nIndex)
{
}

/**
*  @brief
*    Constructor
*/
BitsetIterator::BitsetIterator(const Bitset &lstBitset) :
	m_plstBitset(&lstBitset),
	m_nNextID(lstBitset.GetNumOfElements())
{
}

/**
*  @brief
*    Copy constructor
*/
BitsetIterator::BitsetIterator(const BitsetIterator &cSource) :
	m_plstBitset(cSource.m_plstBitset),
	m_nNextID(cSource.m_nNextID)
{
}

/**
*  @brief
*    Destructor
*/
BitsetIterator::~BitsetIterator()
{
}


//[-------------------------------------------------------]
//[ Private virtual IteratorImpl functions                ]
//[-------------------------------------------------------]
IteratorImpl<bool> *BitsetIterator::Clone() const
{
	return new BitsetIterator(*this);
}

bool BitsetIterator::HasNext() const
{
	return (m_nNextID < m_plstBitset->GetNumOfElements());
}

bool &BitsetIterator::Next()
{
	return (m_nNextID < m_plstBitset->GetNumOfElements()) ? m_plstBitset->Get(m_nNextID++) : Bitset::Null;
}

bool BitsetIterator::HasPrevious() const
{
	return (m_nNextID != 0);
}

bool &BitsetIterator::Previous()
{
	return m_nNextID ? m_plstBitset->Get(--m_nNextID) : Bitset::Null;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
