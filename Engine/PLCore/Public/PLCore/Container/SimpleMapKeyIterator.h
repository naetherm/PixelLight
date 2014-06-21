/*********************************************************\
 *  File: SimpleMapKeyIterator.h                         *
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


#ifndef __PLCORE_CONTAINER_SIMPLEMAPKEYITERATOR_H__
#define __PLCORE_CONTAINER_SIMPLEMAPKEYITERATOR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Container/Iterator.h"
#include "PLCore/Container/IteratorImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer> class SimpleMap;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Simple map key iterator class
*/
template <class KeyType, class ValueType, class Comparer>
class SimpleMapKeyIterator : public IteratorImpl<KeyType> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SimpleMap<KeyType, ValueType, Comparer>;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] mapOwner
		*    Map to operate on
		*  @param[in] nIndex
		*    Start index, if >= GetNumOfElements() the index is set to the last valid index
		*/
		SimpleMapKeyIterator(const SimpleMap<KeyType, ValueType, Comparer> &mapOwner, uint32 nIndex);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] mapOwner
		*    Map to operate on
		*
		*  @note
		*    - The iterator will start at the last element
		*/
		SimpleMapKeyIterator(const SimpleMap<KeyType, ValueType, Comparer> &mapOwner);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		SimpleMapKeyIterator(const SimpleMapKeyIterator<KeyType, ValueType, Comparer> &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SimpleMapKeyIterator();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const SimpleMap<KeyType, ValueType, Comparer>						 *m_pmapOwner;	/**< Simple map to operate on (always valid!) */
		Iterator<typename SimpleMap<KeyType, ValueType, Comparer>::Element > *m_pIterator;	/**< Internal iterator (always valid!) */


	//[-------------------------------------------------------]
	//[ Private virtual IteratorImpl functions                ]
	//[-------------------------------------------------------]
	private:
		virtual IteratorImpl<KeyType> *Clone() const override;
		virtual bool HasNext() const override;
		virtual KeyType &Next() override;
		virtual bool HasPrevious() const override;
		virtual KeyType &Previous() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Container/SimpleMapKeyIterator.inl"


#endif // __PLCORE_CONTAINER_SIMPLEMAPKEYITERATOR_H__
