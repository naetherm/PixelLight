/*********************************************************\
 *  File: Container.h                                    *
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


#ifndef __PLCORE_CONTAINER_CONTAINER_H__
#define __PLCORE_CONTAINER_CONTAINER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Container/Iterable.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract container class
*
*  @note
*    - A container can contain duplicates
*/
template <class ValueType>
class Container : public Iterable<ValueType> {


	//[-------------------------------------------------------]
	//[ Public virtual Container functions                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Checks whether the container is complete empty
		*
		*  @return
		*    'true' if the container is empty, else 'false'
		*/
		virtual bool IsEmpty() const = 0;

		/**
		*  @brief
		*    Returns the number of elements
		*
		*  @return
		*    Number of container elements
		*/
		virtual uint32 GetNumOfElements() const = 0;

		/**
		*  @brief
		*    Returns the size of a single container element (in bytes)
		*
		*  @return
		*    Size of a single container element (in bytes)
		*/
		virtual uint32 GetElementSize() const = 0;

		/**
		*  @brief
		*    Returns the total size of all container elements (in bytes)
		*
		*  @return
		*    Total size of all container elements (in bytes)
		*
		*  @remarks
		*    You can NOT assume that GetElementSize()*GetNumOfElements() = GetSize() is
		*    always true. For instance the Bitset implementation will store the bits tightly packed
		*    and therefore in this case GetElementSize()*GetNumOfElements() != GetSize().
		*    GetSize() will only give you a hind how many bytes are currently required to store
		*    all elements WITHOUT further implementation dependent things like pointers from one
		*    to the next element.
		*/
		virtual uint32 GetSize() const = 0;

		/**
		*  @brief
		*    Clears the whole container
		*/
		virtual void Clear() = 0;

		/**
		*  @brief
		*    Returns whether the given element is within the container or not
		*
		*  @param[in] Element
		*    Element to check
		*
		*  @return
		*    'true' if the element is within the container, else 'false'
		*/
		virtual bool IsElement(const ValueType &Element) const = 0;

		/**
		*  @brief
		*    Returns the index of an element (first appearance)
		*
		*  @param[in] Element
		*    Element to get the index from
		*
		*  @return
		*    Index of the given element, < 0 if it's not in the container
		*/
		virtual int GetIndex(const ValueType &Element) const = 0;

		/**
		*  @brief
		*    Returns an element
		*
		*  @param[in] nIndex
		*    Index of the element to return
		*
		*  @return
		*    Reference to the element at the given index, reference to the 'Null'-object on error
		*/
		virtual ValueType &Get(uint32 nIndex) const = 0;

		/**
		*  @brief
		*    Returns an element
		*
		*  @param[in] nIndex
		*    Index of the element to return
		*
		*  @return
		*    Reference to the element at the given index, reference to the 'Null'-object on error
		*/
		virtual ValueType &operator [](uint32 nIndex) const = 0;

		/**
		*  @brief
		*    Replaces an element (first appearance) through another element
		*
		*  @param[in] Element1
		*    Element to replace
		*  @param[in] Element2
		*    The element which should replace the old one
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe 'Element1' is not within the container?)
		*/
		virtual bool Replace(const ValueType &Element1, const ValueType &Element2) = 0;

		/**
		*  @brief
		*    Replaces an element at the given index through another element
		*
		*  @param[in] nIndex
		*    Index of the element to replace
		*  @param[in] Element
		*    The element which should replace the old one
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe invalid index?)
		*/
		virtual bool ReplaceAtIndex(uint32 nIndex, const ValueType &Element) = 0;

		/**
		*  @brief
		*    Appends an element to the container
		*
		*  @return
		*    Reference to the new element, reference to the 'Null'-object on error
		*/
		virtual ValueType &Add() = 0;

		/**
		*  @brief
		*    Appends an element to the container
		*
		*  @param[in] Element
		*    Element to add
		*
		*  @return
		*    Reference to the new element, reference to the 'Null'-object on error
		*/
		virtual ValueType &Add(const ValueType &Element) = 0;

		/**
		*  @brief
		*    Adds elements from a given C-array
		*
		*  @param[in] pElements
		*    Pointer to C-array with the elements to add, if a null pointer, nothing is done
		*  @param[in] nCount
		*    Number of elements to add, the given C-array MUST have at least nCount elements!
		*
		*  @return
		*    Number of added elements -> if the array is full and resizing is not allowed, this
		*    number may differ from the given nCount
		*/
		virtual uint32 Add(const ValueType *pElements, uint32 nCount) = 0;

		/**
		*  @brief
		*    Appends an element to the container
		*
		*  @param[in] Element
		*    Element to add
		*
		*  @return
		*    This container
		*/
		virtual Container<ValueType> &operator +=(const ValueType &Element) = 0;

		/**
		*  @brief
		*    Appends an container to this container
		*
		*  @param[in] lstContainer
		*    Container to add
		*  @param[in] nStart
		*    Index the add operation should start within lstContainer
		*  @param[in] nCount
		*    Number of elements to copy, if 0 or exceeding the capacity of lstContainer, copy all elements of lstContainer behind nStart
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool Add(const Container<ValueType> &lstContainer, uint32 nStart = 0, uint32 nCount = 0) = 0;

		/**
		*  @brief
		*    Appends an container to this container
		*
		*  @param[in] lstContainer
		*    Container to add
		*
		*  @return
		*    This container
		*/
		virtual Container<ValueType> &operator +=(const Container<ValueType> &lstContainer) = 0;

		/**
		*  @brief
		*    Appends an element to the container at the given index
		*
		*  @param[in] nIndex
		*    Index were to add the new element, if < 0 add at end, must be <= 'GetNumOfElements()'
		*
		*  @return
		*    Reference to the new element, reference to the 'Null'-object on error (maybe invalid index?)
		*
		*  @note
		*    - If the given index is equal to 'GetNumOfElements()' the element is appended at the end of the container
		*    - The container is only enlarged by one element, this means that if 'GetNumOfElements()' is currently 0
		*      and you give an index of 5 the container is NOT automatically resized to 6 elements!
		*/
		virtual ValueType &AddAtIndex(int nIndex) = 0;

		/**
		*  @brief
		*    Appends an element to the container at the given index
		*
		*  @param[in] Element
		*    Element to add
		*  @param[in] nIndex
		*    Index were to add the new element, if < 0 add at end, must be <= 'GetNumOfElements()'
		*
		*  @return
		*    Reference to the new element, reference to the 'Null'-object on error (maybe invalid index?)
		*
		*  @see
		*    - AddAtIndex() above
		*/
		virtual ValueType &AddAtIndex(const ValueType &Element, int nIndex) = 0;

		/**
		*  @brief
		*    Removes an element (first appearance) from the container
		*
		*  @param[in] Element
		*    Element to remove
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe 'Element' is not within the container?)
		*/
		virtual bool Remove(const ValueType &Element) = 0;

		/**
		*  @brief
		*    Removes the element at the given index from the container
		*
		*  @param[in] nElement
		*    Index of the element to remove
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe invalid index?)
		*/
		virtual bool RemoveAtIndex(uint32 nElement) = 0;

		/**
		*  @brief
		*    Removes an element from the container
		*
		*  @param[in] Element
		*    Element to remove
		*
		*  @return
		*    This container
		*/
		virtual Container<ValueType> &operator -=(const ValueType &Element) = 0;

		/**
		*  @brief
		*    Removes the elements of an container from this container
		*
		*  @param[in] lstContainer
		*    Container to remove the elements from this container
		*  @param[in] nStart
		*    Index the remove operation should start within lstContainer
		*  @param[in] nCount
		*    Number of elements to remove, if 0 or exceeding the capacity of lstContainer, remove all elements of lstContainer behind nStart
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool Remove(const Container<ValueType> &lstContainer, uint32 nStart = 0, uint32 nCount = 0) = 0;

		/**
		*  @brief
		*    Removes the elements of an container from this container
		*
		*  @param[in] lstContainer
		*    Container to remove the elements from this container
		*
		*  @return
		*    This container
		*/
		virtual Container<ValueType> &operator -=(const Container<ValueType> &lstContainer) = 0;

		/**
		*  @brief
		*    Makes this container to a copy of another container
		*
		*  @param[in] lstContainer
		*    Container to copy from
		*  @param[in] nStart
		*    Index the copy operation should start within lstContainer
		*  @param[in] nCount
		*    Number of elements to copy, if 0 or exceeding the capacity of lstContainer, copy all elements of lstContainer behind nStart
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool Copy(const Container<ValueType> &lstContainer, uint32 nStart = 0, uint32 nCount = 0) = 0;

		/**
		*  @brief
		*    Makes this container to a copy of another container
		*
		*  @param[in] lstContainer
		*    Container to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		virtual Container<ValueType> &operator =(const Container<ValueType> &lstContainer) = 0;

		/**
		*  @brief
		*    Compares two containers
		*
		*  @param[in] lstContainer
		*    Container to compare with
		*  @param[in] nStart
		*    Index the compare operation should start in both containers
		*  @param[in] nCount
		*    Number of elements to compare, if 0 or exceeding the capacity of lstContainer, compare all elements of lstContainer behind nStart
		*
		*  @return
		*    'true' if both containers are equal, else 'false'
		*/
		virtual bool Compare(const Container<ValueType> &lstContainer, uint32 nStart = 0, uint32 nCount = 0) const = 0;

		/**
		*  @brief
		*    Compares two containers
		*
		*  @param[in] lstContainer
		*    Container to compare with
		*
		*  @return
		*    'true' if both containers are equal, else 'false'
		*/
		virtual bool operator ==(const Container<ValueType> &lstContainer) const = 0;

		/**
		*  @brief
		*    Compares two containers
		*
		*  @param[in] lstContainer
		*    Container to compare with
		*
		*  @return
		*    'true' if both containers are not equal, else 'false'
		*/
		virtual bool operator !=(const Container<ValueType> &lstContainer) const = 0;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_CONTAINER_CONTAINER_H__
