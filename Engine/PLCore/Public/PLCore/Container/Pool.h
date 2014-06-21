/*********************************************************\
 *  File: Pool.h                                         *
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


#ifndef __PLCORE_CONTAINER_POOL_H__
#define __PLCORE_CONTAINER_POOL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Container/Container.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
template <class ValueType> class PoolIterator;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Pool class
*
*  @remarks
*    This pool container is very similar to the list container. Within the double linked
*    list each node knows it's previous and next element. If you want to receive element x,
*    internally the process begins with the first element and goes to the next until
*    element x was found. As you see the following example isn't that performant:\n
*  @verbatim
*    for (uint32 i=0; i<lstList.GetNumOfElements(); i++) {
*        PoolElement &cElement = lstList[i];
*    }
*  @endverbatim
*    Instead use the provided iterator whenever possible:\
*  @verbatim
*    Iterator<PoolElement*> cIterator = m_lstPool.GetIterator();
*    while (cIterator.HasNext()) {
*        PoolElement *pElement = cIterator.Next();
*    }
*  @endverbatim
*   But unlike the list container, this pool container has a second internal list holding
*   currently unused elements. If a new element is added, a free element is chosen - only
*   if there is no free element a new one is created. If a element is removed, it's pushed
*   back to the list of free elements. So, this container needs more memory, but if elements
*   are added and removed frequently, this container is more performant as the standard list.\n
*   If your pool has to manage A LOT OF frequently added and removed elements, you should use
*   FastPool - it's remove function is faster because the pool element to remove must not
*   be searched first. But if you use this faster pool, you are forced to derive your elements
*   from the class FastPoolElement.
*/
template <class ValueType>
class Pool : public Container<ValueType> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class PoolIterator<ValueType>;


	//[-------------------------------------------------------]
	//[ Public static variables                               ]
	//[-------------------------------------------------------]
	public:
		static ValueType Null;	/**< 'Null'-object, do NEVER EVER manipulate this object! */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		Pool();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] lstSource
		*    Pool to copy from
		*  @param[in] nStart
		*    Index the copy operation should start
		*  @param[in] nCount
		*    Number of elements to copy, if 0 copy all elements of lstSource behind nStart
		*/
		Pool(const Pool<ValueType> &lstSource, uint32 nStart = 0, uint32 nCount = 0);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] lstSource
		*    Container to copy from
		*  @param[in] nStart
		*    Index the copy operation should start
		*  @param[in] nCount
		*    Number of elements to copy, if 0 copy all elements of lstSource behind nStart
		*/
		Pool(const Container<ValueType> &lstSource, uint32 nStart = 0, uint32 nCount = 0);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Pool();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] lstSource
		*    Pool to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Container<ValueType> &operator =(const Pool<ValueType> &lstSource);

		/**
		*  @brief
		*    Returns the number of currently free pool elements
		*
		*  @return
		*    Number of currently free pool elements
		*/
		uint32 GetNumOfFreeElements() const;

		/**
		*  @brief
		*    Returns the total size of all free container elements (in bytes)
		*
		*  @return
		*    Total size of all free container elements (in bytes)
		*/
		uint32 GetFreeSize() const;

		/**
		*  @brief
		*    Marks all elements as free
		*
		*  @note
		*    - Quite fast
		*/
		void FreeElements();

		/**
		*  @brief
		*    Removes all currently free elements
		*/
		void RemoveAllFreeElements();


	//[-------------------------------------------------------]
	//[ Private structures                                    ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Internal linked pool element
		*/
		struct PoolElement {
			PoolElement *pNextElement;		/**< Pointer to the next element in the pool, can be a null pointer */
			PoolElement *pPreviousElement;	/**< Pointer to the previous element in the pool, can be a null pointer */
			ValueType	 Data;				/**< The stored data */
		};


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Adds a free element, if there's no free element a new one is created
		*
		*  @return
		*    Added element
		*/
		PoolElement &AddElement();

		/**
		*  @brief
		*    Removes an element (added to the list of free elements)
		*
		*  @param[in] cElement
		*    Pool element to remove
		*/
		void RemoveElement(PoolElement &cElement);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Standard list data
		uint32		 m_nNumOfElements;		/**< Current number of elements */
		PoolElement *m_pFirstElement;		/**< Pointer to first pool element, can be a null pointer */
		PoolElement *m_pLastElement;		/**< Pointer to last pool element, can be a null pointer */
		// Additional pool data
		uint32		 m_nNumOfFreeElements;	/**< Current number of free elements */
		PoolElement *m_pFirstFreeElement;	/**< Pointer to first free pool element, can be a null pointer */


	//[-------------------------------------------------------]
	//[ Public virtual Iterable functions                     ]
	//[-------------------------------------------------------]
	public:
		virtual Iterator<ValueType> GetIterator(uint32 nIndex = 0) const override;
		virtual ConstIterator<ValueType> GetConstIterator(uint32 nIndex = 0) const override;
		virtual Iterator<ValueType> GetEndIterator() const override;
		virtual ConstIterator<ValueType> GetConstEndIterator() const override;


	//[-------------------------------------------------------]
	//[ Public virtual Container functions                    ]
	//[-------------------------------------------------------]
	public:
		virtual bool IsEmpty() const override;
		virtual uint32 GetNumOfElements() const override;
		virtual uint32 GetElementSize() const override;
		virtual uint32 GetSize() const override;
		virtual void Clear() override;
		virtual bool IsElement(const ValueType &Element) const override;
		virtual int GetIndex(const ValueType &Element) const override;
		virtual ValueType &Get(uint32 nIndex) const override;
		virtual ValueType &operator [](uint32 nIndex) const override;
		virtual bool Replace(const ValueType &Element1, const ValueType &Element2) override;
		virtual bool ReplaceAtIndex(uint32 nIndex, const ValueType &Element) override;
		virtual ValueType &Add() override;
		virtual ValueType &Add(const ValueType &Element) override;
		virtual uint32 Add(const ValueType *pElements, uint32 nCount) override;
		virtual Container<ValueType> &operator +=(const ValueType &Element) override;
		virtual bool Add(const Container<ValueType> &lstContainer, uint32 nStart = 0, uint32 nCount = 0) override;
		virtual Container<ValueType> &operator +=(const Container<ValueType> &lstContainer) override;
		virtual ValueType &AddAtIndex(int nIndex) override;
		virtual ValueType &AddAtIndex(const ValueType &Element, int nIndex) override;
		virtual bool Remove(const ValueType &Element) override;
		virtual bool RemoveAtIndex(uint32 nElement) override;
		virtual Container<ValueType> &operator -=(const ValueType &Element) override;
		virtual bool Remove(const Container<ValueType> &lstContainer, uint32 nStart = 0, uint32 nCount = 0) override;
		virtual Container<ValueType> &operator -=(const Container<ValueType> &lstContainer) override;
		virtual bool Copy(const Container<ValueType> &lstContainer, uint32 nStart = 0, uint32 nCount = 0) override;
		virtual Container<ValueType> &operator =(const Container<ValueType> &lstContainer) override;
		virtual bool Compare(const Container<ValueType> &lstContainer, uint32 nStart = 0, uint32 nCount = 0) const override;
		virtual bool operator ==(const Container<ValueType> &lstContainer) const override;
		virtual bool operator !=(const Container<ValueType> &lstContainer) const override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Container/Pool.inl"


#endif // __PLCORE_CONTAINER_POOL_H__
