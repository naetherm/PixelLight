/*********************************************************\
 *  File: Map.h                                          *
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


#ifndef __PLCORE_CONTAINER_MAP_H__
#define __PLCORE_CONTAINER_MAP_H__
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
*    Abstract map (associative array) base template class
*
*  @verbatim
*    Usage example:
*    HashMap<String, MyClass*> mapHashMap;  // Hash table which manages MyClass pointers
*    MyClass *pT1=new MyClass(), *pT2=new MyClass(), *pT3=new MyClass(), *pT;  // Test instances
*    mapHashMap.Add("Lenny", pT1);  // Give pointer to T1 the name 'Lenny'
*    mapHashMap.Add("Barny", pT2);  // Give pointer to T2 the name 'Barny'
*    mapHashMap.Set("Homer", pT3);  // Give pointer to T3 the name 'Homer' by using the set function
*    pT = mapHashMap.Get("Barny");   // Give us the pointer to 'Barny'
*    pT = mapHashMap.Get("Homer");   // Give us the pointer to 'Homer'
*    pT = mapHashMap.Get("Lenny");   // Give us the pointer to 'Lenny'
*  @endverbatim
*
*  @note
*    - The standard container iterator iterates over all values (NOT the keys!) within the map, use
*      the special key iterator for iterating over all keys
*    - Within a map, you can't expect that the iterator returns the elements always in exact the same order
*    - Usually map iterators are not that performance by concept, so, try to avoid to use map iterators
*/
template <class KeyType, class ValueType>
class Map : public Iterable<ValueType> {


	//[-------------------------------------------------------]
	//[ Public static variables                               ]
	//[-------------------------------------------------------]
	public:
		static KeyType NullKey;	/**< 'Null' key object, do NEVER EVER manipulate this object! */


	//[-------------------------------------------------------]
	//[ Public virtual Map functions                          ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Clears the map
		*/
		virtual void Clear() = 0;

		/**
		*  @brief
		*    Checks whether the map is complete empty
		*
		*  @return
		*    'true' if the map is empty, else 'false'
		*/
		virtual bool IsEmpty() const = 0;

		/**
		*  @brief
		*    Returns the number of elements/entries
		*
		*  @return
		*    Number of container elements/entries
		*/
		virtual uint32 GetNumOfElements() const = 0;

		/**
		*  @brief
		*    Adds/puts a new element to the map
		*
		*  @param[in] Key
		*    The key of the new element
		*  @param[in] Value
		*    The value of the new element
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - You have to check by yourself whether there's already such a key within the map
		*
		*  @see
		*    - Set()
		*/
		virtual bool Add(const KeyType &Key, const ValueType &Value) = 0;

		/**
		*  @brief
		*    Replaces the value of a map element
		*
		*  @param[in] Key
		*    The key of the element
		*  @param[in] NewValue
		*    The new value of the element
		*
		*  @return
		*    'true' if all went fine, else 'false' (key not found?)
		*
		*  @see
		*    - Set()
		*/
		virtual bool Replace(const KeyType &Key, const ValueType &NewValue) = 0;

		/**
		*  @brief
		*    Sets (adds or replaces) the value of a map element
		*
		*  @param[in] Key
		*    The key of the element
		*  @param[in] Value
		*    The set value of the element
		*
		*  @return
		*    'false' if a new element was added, 'true' if the value was replaced
		*
		*  @remarks
		*    This function is a combination of the add and replace function and 'just' assigns
		*    a value to a given key without taking care of whether or not there's already an
		*    element with the given key inside the map.
		*
		*  @see
		*    - Add()
		*    - Replace()
		*/
		virtual bool Set(const KeyType &Key, const ValueType &Value) = 0;

		/**
		*  @brief
		*    Removes an element from the map
		*
		*  @param[in] Key
		*    Key of the element which should be removed
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool Remove(const KeyType &Key) = 0;

		/**
		*  @brief
		*    Removes all elements with a certain value from the map
		*
		*  @param[in] Value
		*    Value to look for
		*
		*  @return
		*    Number of removed elements
		*
		*  @note
		*    - For performance reasons, try to use the Remove(<Key>) function
		*      whenever possible!
		*/
		virtual uint32 RemoveValue(const ValueType &Value) = 0;

		/**
		*  @brief
		*    Returns the value of a given key
		*
		*  @param[in] Key
		*    Key to use
		*
		*  @return
		*    Reference of the value of the given key, reference to the 'Null'-object on error
		*/
		virtual const ValueType &Get(const KeyType &Key) const = 0;

		/**
		*  @brief
		*    Returns the value of a given key
		*
		*  @param[in] Key
		*    Key to use
		*
		*  @return
		*    Reference of the value of the given key, reference to the 'Null'-object on error
		*/
		virtual ValueType &Get(const KeyType &Key) = 0;

		/**
		*  @brief
		*    Returns a key iterator operating on the derived data structure
		*
		*  @param[in] nIndex
		*    Start index, if >= 'total number of elements' the index is set to the last valid index
		*
		*  @return
		*    Key iterator operating on the derived
		*/
		virtual Iterator<KeyType> GetKeyIterator(uint32 nIndex = 0) const = 0;

		/**
		*  @brief
		*    Returns a constant key iterator operating on the derived data structure
		*
		*  @param[in] nIndex
		*    Start index, if >= 'total number of elements' the index is set to the last valid index
		*
		*  @return
		*    Constant key iterator operating on the derived
		*/
		virtual ConstIterator<KeyType> GetConstKeyIterator(uint32 nIndex = 0) const = 0;

		/**
		*  @brief
		*    Returns a key iterator operating on the derived data structure and starting at the end
		*
		*  @return
		*    Key iterator operating on the derived,
		*
		*  @remarks
		*    Use this function to get a key iterator if you want to iterate in reversed order starting
		*    at the end last element.
		*/
		virtual Iterator<KeyType> GetEndKeyIterator() const = 0;

		/**
		*  @brief
		*    Returns a constant key iterator operating on the derived data structure and starting at the end
		*
		*  @return
		*    Constant key iterator operating on the derived,
		*
		*  @remarks
		*    Use this function to get a constant key iterator if you want to iterate in reversed order
		*    starting at the end last element.
		*/
		virtual ConstIterator<KeyType> GetConstEndKeyIterator() const = 0;


	//[-------------------------------------------------------]
	//[ Private virtual Map functions                         ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Makes this map to a copy of another map
		*
		*  @param[in] cMap
		*    Map to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		virtual Map<KeyType, ValueType> &operator =(const Map<KeyType, ValueType> &cMap) = 0;


};


//[-------------------------------------------------------]
//[ Public static variables                               ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType> KeyType Map<KeyType, ValueType>::NullKey;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_CONTAINER_MAP_H__
