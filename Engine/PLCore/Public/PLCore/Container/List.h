/*********************************************************\
 *  File: List.h                                         *
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


#ifndef __PLCORE_CONTAINER_LIST_H__
#define __PLCORE_CONTAINER_LIST_H__
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
template <class ValueType> class ListIterator;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Linked list class
*
*  @remarks
*    Within the double linked list each node knows it's previous and next element. If you
*    want to receive element x, internally the process begins with the first element and
*    goes to the next until element x was found. As you see the following example isn't
*    that performant:\n
*  @verbatim
*    for (uint32 i=0; i<lstList.GetNumOfElements(); i++) {
*      ListElement &cElement = lstList[i];
*    }
*  @endverbatim
*    Instead use the provided iterator whenever possible:\
*  @verbatim
*    Iterator<ListElement*> cIterator = m_lstList.GetIterator();
*    while (cIterator.HasNext()) {
*        ListElement *pElement = cIterator.Next();
*    }
*  @endverbatim
*/
template <class ValueType>
class List : public Container<ValueType> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class ListIterator<ValueType>;


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
		List();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] lstSource
		*    List to copy from
		*  @param[in] nStart
		*    Index the copy operation should start
		*  @param[in] nCount
		*    Number of elements to copy, if 0 copy all elements of lstSource behind nStart
		*/
		List(const List<ValueType> &lstSource, uint32 nStart = 0, uint32 nCount = 0);

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
		List(const Container<ValueType> &lstSource, uint32 nStart = 0, uint32 nCount = 0);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~List();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] lstSource
		*    List to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Container<ValueType> &operator =(const List<ValueType> &lstSource);


	//[-------------------------------------------------------]
	//[ Private structures                                    ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Internal linked list element
		*/
		struct ListElement {
			ListElement *pNextElement;		/**< Pointer to the next element in the list, can be a null pointer */
			ListElement *pPreviousElement;	/**< Pointer to the previous element in the list, can be a null pointer */
			ValueType	 Data;				/**< The stored data */
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32		 m_nNumOfElements;	/**< Current number of elements */
		ListElement *m_pFirstElement;	/**< Pointer to first list element, can be a null pointer */
		ListElement *m_pLastElement;	/**< Pointer to last list element, can be a null pointer */


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
#include "PLCore/Container/List.inl"


#endif // __PLCORE_CONTAINER_LIST_H__
