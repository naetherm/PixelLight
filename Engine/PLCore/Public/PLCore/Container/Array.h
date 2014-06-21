/*********************************************************\
 *  File: Array.h                                        *
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


#ifndef __PLCORE_CONTAINER_ARRAY_H__
#define __PLCORE_CONTAINER_ARRAY_H__
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
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Array class
*
*  @remarks
*    Within the array, all elements will be arranged sequentially in memory. The array is
*    resized automatically if required, in this case a new array is created internally,
*    old data is copied and the old array is destroyed. Therefore, avoid array resizing
*    if you have a lot of big elements. Using 'GetData()' you can receive a pointer to the
*    first element, now you can iterate through the array using 'GetData()+1' etc. for more
*    performance.
*
*  @note
*    - Do NOT backup pointer to array elements, if the array is resized they will become
*      invalid
*/
template <class ValueType>
class PLCORE_TMPL Array : public Container<ValueType> {


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
		*
		*  @param[in] nMaxNumOfElements
		*    Maximum number of elements within the array
		*  @param[in] bAdded
		*    Are all elements added? (GetNumOfElements() = GetMaxNumOfElements())
		*  @param[in] bInit
		*    Initialize new elements by setting them to null?
		*
		*  @see
		*    - Resize()
		*/
		Array(uint32 nMaxNumOfElements = 0, bool bAdded = true, bool bInit = false);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] lstSource
		*    Array to copy from
		*  @param[in] nStart
		*    Index the copy operation should start
		*  @param[in] nCount
		*    Number of elements to copy, if 0 copy all elements of lstSource behind nStart
		*/
		Array(const Array<ValueType> &lstSource, uint32 nStart = 0, uint32 nCount = 0);

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
		Array(const Container<ValueType> &lstSource, uint32 nStart = 0, uint32 nCount = 0);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Array();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] lstSource
		*    Array to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Container<ValueType> &operator =(const Array<ValueType> &lstSource);

		/**
		*  @brief
		*    Returns the maximum number of elements in the array
		*
		*  @return
		*    Maximum number of element in the array
		*/
		uint32 GetMaxNumOfElements() const;

		/**
		*  @brief
		*    Sets the maximum number of elements in the array
		*
		*  @param[in] nMaxNumOfElements
		*    New maximum number of elements (0 = clear array)
		*  @param[in] bAdded
		*    Are all elements added? (GetNumOfElements() = GetMaxNumOfElements())
		*  @param[in] bInit
		*    Initialize new elements by setting them to null?
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - In arrays with direct objects like Array<MyClass> which have some internal
		*      classes and especial virtual functions it's NOT recommended to set 'bInit' to true!
		*/
		bool Resize(uint32 nMaxNumOfElements, bool bAdded = true, bool bInit = false);

		/**
		*  @brief
		*    Returns the number of elements automatically added if the array
		*    size is to small
		*
		*  @return
		*    Number of elements automatically added if the array size is to small
		*/
		uint32 GetResizeCount() const;

		/**
		*  @brief
		*    Sets the number of elements automatically added if the array
		*    size is to small
		*
		*  @param[in] nCount
		*    Number of elements automatically added if the array size is to small
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - If nCount is 0, the array size isn't changed automatically
		*/
		bool SetResizeCount(uint32 nCount = 10);

		/**
		*  @brief
		*    Returns the array data
		*
		*  @return
		*    The array data, can be a null pointer
		*
		*  @note
		*    - Do NOT delete this data by yourself!
		*    - Do NOT mess up the memory by writing outside the given buffer...
		*/
		ValueType *GetData() const;

		/**
		*  @brief
		*    Resets the array
		*
		*  @remarks
		*    While the Clear() function destroys also the data, this function will only
		*    reset the current number of elements within the array to 0.
		*/
		void Reset();

		/**
		*  @brief
		*    Moves an element within the array
		*
		*  @param[in] nFromIndex
		*    The index of the element which should be moved
		*  @param[in] nToIndex
		*    The index to which the element should be moved
		*
		*  @note
		*    - This methods assumes, that both index values are within 0 and GetNumOfElements()-1
		*/
		void MoveElement(uint32 nFromIndex, uint32 nToIndex);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32     m_nMaxNumOfElements;	/**< Maximum number of elements */
		uint32     m_nNumOfElements;	/**< Current number of elements */
		ValueType *m_pData;				/**< Elements, can be a null pointer */
		uint32     m_nResizeCount;		/**< Automatic resize count */


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
#include "PLCore/Container/Array.inl"


#endif // __PLCORE_CONTAINER_ARRAY_H__
