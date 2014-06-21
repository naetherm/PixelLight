/*********************************************************\
 *  File: Iterable.h                                     *
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


#ifndef __PLCORE_CONTAINER_ITERABLE_H__
#define __PLCORE_CONTAINER_ITERABLE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/PLCore.h"
#include "PLCore/Container/Iterator.h"
#include "PLCore/Container/ConstIterator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
template <class ValueType> class Iterator;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract iterable (aggregate) class
*
*  @see
*    - Iterator for further details and an usage example
*/
template <class ValueType>
class Iterable {


	//[-------------------------------------------------------]
	//[ Public virtual Iterable functions                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Iterable<ValueType>() {};

		/**
		*  @brief
		*    Returns an iterator operating on the derived data structure
		*
		*  @param[in] nIndex
		*    Start index, if >= 'total number of elements' the index is set to the last valid index
		*
		*  @return
		*    Iterator operating on the derived
		*/
		virtual Iterator<ValueType> GetIterator(uint32 nIndex = 0) const = 0;

		/**
		*  @brief
		*    Returns a constant iterator operating on the derived data structure
		*
		*  @param[in] nIndex
		*    Start index, if >= 'total number of elements' the index is set to the last valid index
		*
		*  @return
		*    Constant iterator operating on the derived
		*/
		virtual ConstIterator<ValueType> GetConstIterator(uint32 nIndex = 0) const = 0;

		/**
		*  @brief
		*    Returns an iterator operating on the derived data structure and starting at the end
		*
		*  @return
		*    Iterator operating on the derived,
		*
		*  @remarks
		*    Use this function to get an iterator if you want to iterate in reversed order starting
		*    at the end last element.
		*/
		virtual Iterator<ValueType> GetEndIterator() const = 0;

		/**
		*  @brief
		*    Returns a constant iterator operating on the derived data structure and starting at the end
		*
		*  @return
		*    Constant iterator operating on the derived,
		*
		*  @remarks
		*    Use this function to get a constant iterator if you want to iterate in reversed order
		*    starting at the end last element.
		*/
		virtual ConstIterator<ValueType> GetConstEndIterator() const = 0;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_CONTAINER_ITERABLE_H__
