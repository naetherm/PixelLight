/*********************************************************\
 *  File: IteratorImpl.h                                 *
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


#ifndef __PLCORE_CONTAINER_ITERATORIMPL_H__
#define __PLCORE_CONTAINER_ITERATORIMPL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Core/RefCount.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
template <class ValueType> class Iterator;
template <class ValueType> class ConstIterator;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract iterator implementation class
*/
template <class ValueType>
class IteratorImpl : protected RefCount<ValueType> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Iterator<ValueType>;
	friend class ConstIterator<ValueType>;


	//[-------------------------------------------------------]
	//[ Protected virtual IteratorImpl functions              ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~IteratorImpl<ValueType>() {};

		/**
		*  @brief
		*    Creates a clone of the implementation
		*
		*  @return
		*    The created clone of the implementation, always valid
		*/
		virtual IteratorImpl<ValueType> *Clone() const = 0;

		/**
		*  @brief
		*    Checks whether the iterator can return a next element
		*
		*  @return
		*    'true' if the iterator can return a next element, else 'false'
		*/
		virtual bool HasNext() const = 0;

		/**
		*  @brief
		*    Returns the next element
		*
		*  @return
		*    Reference to the next element, reference to the 'Null'-object on error
		*/
		virtual ValueType &Next() = 0;

		/**
		*  @brief
		*    Checks whether the iterator can return a previous element
		*
		*  @return
		*    'true' if the iterator can return a previous element, else 'false'
		*/
		virtual bool HasPrevious() const = 0;

		/**
		*  @brief
		*    Returns the previous element
		*
		*  @return
		*    Reference to the previous element, reference to the 'Null'-object on error
		*/
		virtual ValueType &Previous() = 0;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_CONTAINER_ITERATORIMPL_H__
