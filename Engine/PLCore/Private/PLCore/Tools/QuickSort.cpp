/*********************************************************\
 *  File: QuickSort.cpp                                  *
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
#include "PLCore/Core/MemoryManager.h"
#include "PLCore/Tools/QuickSort.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
QuickSort::QuickSort() :
	m_pData(nullptr),
	m_nElementSize(0),
	m_pTemp(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
QuickSort::~QuickSort()
{
}

/**
*  @brief
*    Sorts a set of data using the QuickSort algorithm
*/
bool QuickSort::Sort(void *pData, uint32 nNumOfElements, uint32 nElementSize)
{
	// Check parameters
	if (!pData || !nElementSize)
		return false; // Error!

	// Already 'sorted'? (no elements are sorted *g*)
	if (nNumOfElements) {
		// Init data
		m_pData		   = static_cast<uint8*>(pData);
		m_nElementSize = nElementSize;
		m_pTemp		   = new uint8[nElementSize];

		// Start recursive sort
		SortRec(m_pData, m_pData+nElementSize*(nNumOfElements-1));

		// Cleanup
		delete [] m_pTemp;
		m_pTemp = nullptr;
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
QuickSort::QuickSort(const QuickSort &cSource) :
	m_pData(nullptr),
	m_nElementSize(0),
	m_pTemp(nullptr)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
QuickSort &QuickSort::operator =(const QuickSort &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}

/**
*  @brief
*    Sorts a set of data recursive using the QuickSort algorithm
*/
void QuickSort::SortRec(uint8 *pLeftStart, uint8 *pRightStart)
{
	// First, find pivot element
	uint8 *pPivot = pRightStart;

	// Sort
	uint8 *pLeft  = pLeftStart;
	uint8 *pRight = pRightStart;
	while (pLeft<pRight) {
		// Find left element to swap
		while (Compare(pLeft, pPivot) < 0)
			pLeft  += m_nElementSize;

		// Find right element to swap
		while (Compare(pRight, pPivot) > 0)
			pRight -= m_nElementSize;

		// Finished?
		if (pLeft >= pRight)
			break;

		// Swap the two elements
		MemoryManager::Copy(m_pTemp, pLeft,   m_nElementSize);
		MemoryManager::Copy(pLeft,   pRight,  m_nElementSize);
		MemoryManager::Copy(pRight,  m_pTemp, m_nElementSize);

		// Go to next elements
		pLeft  += m_nElementSize;
		pRight -= m_nElementSize;
	}

	// Swap pivot element
	MemoryManager::Copy(m_pTemp, pPivot,  m_nElementSize);
	MemoryManager::Copy(pPivot,  pLeft,   m_nElementSize);
	MemoryManager::Copy(pLeft,   m_pTemp, m_nElementSize);

	// Left recursion
	if (pLeft-m_nElementSize > pLeftStart)
		SortRec(pLeftStart,	pLeft-m_nElementSize);

	// Right recursion
	if (pLeft+m_nElementSize < pRightStart)
		SortRec(pLeft+m_nElementSize, pRightStart);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
