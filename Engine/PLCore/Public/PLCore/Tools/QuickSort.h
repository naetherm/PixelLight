/*********************************************************\
 *  File: QuickSort.h                                    *
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


#ifndef __PLCORE_QUICKSORT_H__
#define __PLCORE_QUICKSORT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/PLCore.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Quick sort class
*
*  @remarks
*  @verbatim
*    Usage example:
*    // Derive your own class from QuickSort to implement the compare function:
*    class QuickSortTest : public QuickSort {
*      private:
*        virtual char Compare(void *pElement1, void *pElement2) const {
*          if (*((int*)pElement1)  < *((int*)pElement2)) return -1;
*          if (*((int*)pElement1) == *((int*)pElement2)) return  0;
*          else                                          return  1;
*        };
*     };
*     // Sort example using the derived class above:
*     QuickSortTest cQuickSort;
*     int nTest[10] = {3, 6, 2, 6, 8, 4, 36, 22, 64, 99};
*     cQuickSort.Sort(nTest, 10, sizeof(int));
*  @endverbatim
*/
class QuickSort {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API QuickSort();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~QuickSort();

		/**
		*  @brief
		*    Sorts a set of data using the QuickSort algorithm
		*
		*  @param[in, out] pData
		*    Data to sort (MUST be valid!)
		*  @param[in]      nNumOfElements
		*    Number of data elements to sort (MUST be valid!)
		*  @param[in]      nElementSize
		*    Size of an element in bytes (MUST be valid!)
		*
		*  @return
		*    'true' if all went fine, else 'false' (invalid data etc.)
		*/
		PLCORE_API bool Sort(void *pData, uint32 nNumOfElements, uint32 nElementSize);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		QuickSort(const QuickSort &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		QuickSort &operator =(const QuickSort &cSource);

		/**
		*  @brief
		*    Sorts a set of data recursive using the QuickSort algorithm
		*
		*  @param[in] pLeftStart
		*    Left start element (always valid!)
		*  @param[in] pRightStart
		*    Right start element (always valid!)
		*/
		void SortRec(uint8 *pLeftStart, uint8 *pRightStart);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint8  *m_pData;		/**< Data to sort, can be a null pointer */
		uint32  m_nElementSize;	/**< Size of an element in bytes */
		uint8  *m_pTemp;		/**< Temp buffer for swapping, can be a null pointer */


	//[-------------------------------------------------------]
	//[ Virtual private QuickSort functions                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Custom element compare function
		*
		*  @param[in] pElement1
		*    First element
		*  @param[in] pElement2
		*    Second element
		*
		*  @return
		*    < 0 pElement1 less than pElement2\n
		*    = 0 pElement1 equivalent to pElement2\n
		*    > 0 pElement1 greater than pElement2
		*/
		virtual char Compare(void *pElement1, void *pElement2) const = 0;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_QUICKSORT_H__
