/*********************************************************\
 *  File: HashMapKeyIterator.h                           *
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


#ifndef __PLCORE_CONTAINER_HASHMAPKEYITERATOR_H__
#define __PLCORE_CONTAINER_HASHMAPKEYITERATOR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Container/IteratorImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower> class HashMap;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Hash map key iterator class
*/
template <class KeyType, class ValueType, class Hasher, class Comparer, class Grower>
class HashMapKeyIterator : public IteratorImpl<KeyType> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class HashMap<KeyType, ValueType, Hasher, Comparer, Grower>;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] mapOwner
		*    Hash map to operate on
		*  @param[in] nIndex
		*    Start index, if >= GetNumOfElements() the index is set to the last valid index
		*/
		HashMapKeyIterator(const HashMap<KeyType, ValueType, Hasher, Comparer, Grower> &mapOwner, uint32 nIndex);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] mapOwner
		*    Hash map to operate on
		*
		*  @note
		*    - The iterator will start at the last element
		*/
		HashMapKeyIterator(const HashMap<KeyType, ValueType, Hasher, Comparer, Grower> &mapOwner);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		HashMapKeyIterator(const HashMapKeyIterator<KeyType, ValueType, Hasher, Comparer, Grower> &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~HashMapKeyIterator();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const HashMap<KeyType, ValueType, Hasher, Comparer, Grower>			 *m_pmapOwner;		/**< Hash map to operate on (always valid!) */
		uint32																  m_nNextSlots;		/**< Next slots list */
		typename HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::Slot *m_pNextSlot;		/**< Next slot, can be a null pointer */
		uint32																  m_nPreviousSlots;	/**< Next slots list */
		typename HashMap<KeyType, ValueType, Hasher, Comparer, Grower>::Slot *m_pPreviousSlot;	/**< Previous slot, can be a null pointer */


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
#include "PLCore/Container/HashMapKeyIterator.inl"


#endif // __PLCORE_CONTAINER_HASHMAPKEYITERATOR_H__
