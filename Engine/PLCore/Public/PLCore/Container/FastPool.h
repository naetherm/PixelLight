/*********************************************************\
 *  File: FastPool.h                                     *
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


#ifndef __PLCORE_CONTAINER_FASTPOOL_H__
#define __PLCORE_CONTAINER_FASTPOOL_H__
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
template<class ValueType> class FastPool;
template<class ValueType> class FastPoolIterator;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Linked fast pool element you have to derive your elements from
*/
template <class ValueType>
class FastPoolElement {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class FastPool<ValueType>;
	friend class FastPoolIterator<ValueType>;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		FastPoolElement();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FastPoolElement();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] lstSource
		*    Fast pool element to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		virtual FastPoolElement<ValueType> &operator =(const FastPoolElement<ValueType> &lstSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ValueType *m_pNextElement;		/**< Pointer to the next element in the pool, can be a null pointer */
		ValueType *m_pPreviousElement;	/**< Pointer to the previous element in the pool, can be a null pointer */


};

/**
*  @brief
*    Fast pool class
*
*  @note
*    - Your elements MUST be derived from FastPoolElement
*    - Use RemoveElement() to remove an element in a quite fast way
*    - Do NOT use your element within more than one fast pool at once, if this is required,
*      use the pool class instead
*
*  @see
*    - Pool class
*/
template <class ValueType>
class FastPool : public Container<ValueType> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class FastPoolIterator<ValueType>;


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
		FastPool();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] lstSource
		*    Fast pool to copy from
		*  @param[in] nStart
		*    Index the copy operation should start
		*  @param[in] nCount
		*    Number of elements to copy, if 0 copy all elements of lstSource behind nStart
		*/
		FastPool(const FastPool<ValueType> &lstSource, uint32 nStart = 0, uint32 nCount = 0);

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
		FastPool(const Container<ValueType> &lstSource, uint32 nStart = 0, uint32 nCount = 0);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FastPool();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] lstSource
		*    Fast pool to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Container<ValueType> &operator =(const FastPool<ValueType> &lstSource);

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

		/**
		*  @brief
		*    Removes an element (added to the list of free elements)
		*
		*  @param[in] cElement
		*    Element to remove, MUST be an element from THIS pool!
		*
		*  @note
		*    - If you have the reference to your element, use this function to remove
		*      it because it's quite faster than the default Remove() function
		*/
		void RemoveElement(ValueType &cElement);

		/**
		*  @brief
		*    Returns the index of an element
		*
		*  @param[in] cElement
		*    Element to get the index from
		*
		*  @return
		*    Index of the given element, < 0 if it's not in the container
		*/
		int GetElementIndex(ValueType &cElement) const;


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
		ValueType &AddElement();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Standard list data
		uint32     m_nNumOfElements;		/**< Current number of elements */
		ValueType *m_pFirstElement;			/**< Pointer to first pool element, can be a null pointer */
		ValueType *m_pLastElement;			/**< Pointer to last pool element, can be a null pointer */
		// Additional pool data
		uint32     m_nNumOfFreeElements;	/**< Current number of free elements */
		ValueType *m_pFirstFreeElement;		/**< Pointer to first free pool element, can be a null pointer */


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
#include "PLCore/Container/FastPool.inl"


#endif // __PLCORE_CONTAINER_FASTPOOL_H__
