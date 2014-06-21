/*********************************************************\
 *  File: Bitset.h                                       *
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


#ifndef __PLCORE_CONTAINER_BITSET_H__
#define __PLCORE_CONTAINER_BITSET_H__
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
*    Bit set class
*
*  @remarks
*    The bit set class which is useful when dealing with many binary states because
*    many bits will be put together in variables to save memory. The bit set is
*    resized automatically if required.
*/
class PLCORE_API Bitset : public Container<bool> {


	//[-------------------------------------------------------]
	//[ Public static variables                               ]
	//[-------------------------------------------------------]
	public:
		static bool Null;	/**< 'Null'-object, do NEVER EVER manipulate this object! */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nMaxNumOfElements
		*    Maximum number of elements within the bit set
		*  @param[in] bAdded
		*    Are all elements added? (GetNumOfElements() = GetMaxNumOfElements())
		*  @param[in] bInit
		*    Initialize new elements by setting them to null?
		*/
		Bitset(uint32 nMaxNumOfElements = 0, bool bAdded = true, bool bInit = false);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] lstSource
		*    Bitset to copy from
		*  @param[in] nStart
		*    Index the copy operation should start
		*  @param[in] nCount
		*    Number of elements to copy, if 0 copy all elements of lstSource behind nStart
		*/
		Bitset(const Bitset &lstSource, uint32 nStart = 0, uint32 nCount = 0);

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
		Bitset(const Container<bool> &lstSource, uint32 nStart = 0, uint32 nCount = 0);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Bitset();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] lstSource
		*    Bitset to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		inline Container<bool> &operator =(const Bitset &lstSource);

		/**
		*  @brief
		*    Returns the maximum number of elements in the bit set
		*
		*  @return
		*    Maximum number of element in the bit set
		*/
		inline uint32 GetMaxNumOfElements() const;

		/**
		*  @brief
		*    Sets the maximum number of elements in the bit set
		*
		*  @param[in] nMaxNumOfElements
		*    New maximum number of elements (0 = clear bit set)
		*  @param[in] bAdded
		*    Are all elements added? (GetNumOfElements() = GetMaxNumOfElements())
		*  @param[in] bInit
		*    Initialize new elements by setting them to null?
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool Resize(uint32 nMaxNumOfElements, bool bAdded = true, bool bInit = false);

		/**
		*  @brief
		*    Returns the number of elements automatically added if the bit set
		*    size is to small
		*
		*  @return
		*    Number of elements automatically added if the bit set size is to small
		*/
		inline uint32 GetResizeCount() const;

		/**
		*  @brief
		*    Sets the number of elements automatically added if the bit set
		*    size is to small
		*
		*  @param[in] nCount
		*    Number of elements automatically added if the bit set size is to small
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - If nCount is 0, the array size isn't changed automatically
		*/
		inline bool SetResizeCount(uint32 nCount = 64);

		/**
		*  @brief
		*    Clears a bit to 0
		*
		*  @param[in] nIndex
		*    Index of the bit to clear
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		inline bool Clear(uint32 nIndex);

		/**
		*  @brief
		*    Clears all bits to 0
		*/
		void ClearAll();

		/**
		*  @brief
		*    Sets the desired bit
		*
		*  @param[in] nIndex
		*    Index of the bit to set
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		inline bool Set(uint32 nIndex);

		/**
		*  @brief
		*    Sets all bits to 1
		*/
		void SetAll();

		/**
		*  @brief
		*    Inverts the desired bit
		*
		*  @param[in] nIndex
		*    Index of the bit to invert
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		inline bool Invert(uint32 nIndex);

		/**
		*  @brief
		*    Inverts all bits
		*/
		inline void InvertAll();

		/**
		*  @brief
		*    Returns if the desired bit slot is a 1 or a 0
		*
		*  @param[in] nIndex
		*    Index of the bit to check
		*
		*  @return
		*    'true' if the bit is set, else 'false'
		*/
		inline bool IsSet(uint32 nIndex) const;

		/**
		*  @brief
		*    Returns the number of set bits (value = 'true')
		*
		*  @return
		*    The number of set bits (value = 'true')
		*/
		uint32 GetNumOfSetBits() const;

		/**
		*  @brief
		*    Returns the internal number of integers (32 bit) used to store the bits
		*
		*  @return
		*    The internal number of integers used to store the bits
		*/
		inline uint32 GetNumOfIntegers() const;

		/**
		*  @brief
		*    Returns the internal integers (32 bit) used to store the bits
		*
		*  @return
		*    The internal integers used to store the bits, can be a null pointer, DON'T delete the memory!
		*/
		inline const uint32 *GetIntegers() const;

		/**
		*  @brief
		*    Resets the bitset
		*
		*  @remarks
		*    While the Clear() function destroys also the data, this function will only
		*    reset the current number of elements within the array to 0.
		*/
		inline void Reset();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32  m_nMaxNumOfElements;	/**< Maximum number of elements */
		uint32  m_nNumOfElements;		/**< Current number of elements */
		uint32  m_nNumOfIntegers;		/**< Number of integers */
		uint32 *m_pnIntegers;			/**< Bits data, can be a null pointer */
		uint32  m_nResizeCount;			/**< Automatic resize count */
		// Static data
		static bool m_bBit;	/**< Last returned bit */


	//[-------------------------------------------------------]
	//[ Public virtual Iterable functions                     ]
	//[-------------------------------------------------------]
	public:
		virtual Iterator<bool> GetIterator(uint32 nIndex = 0) const override;
		virtual ConstIterator<bool> GetConstIterator(uint32 nIndex = 0) const override;
		virtual Iterator<bool> GetEndIterator() const override;
		virtual ConstIterator<bool> GetConstEndIterator() const override;


	//[-------------------------------------------------------]
	//[ Public virtual Container functions                    ]
	//[-------------------------------------------------------]
	public:
		inline virtual bool IsEmpty() const;
		inline virtual uint32 GetNumOfElements() const;
		inline virtual uint32 GetElementSize() const;
		inline virtual uint32 GetSize() const;
		virtual void Clear() override;
		inline virtual bool IsElement(const bool &Element) const;
		inline virtual int GetIndex(const bool &Element) const;
		inline virtual bool &Get(uint32 nIndex) const;
		inline virtual bool &operator [](uint32 nIndex) const;
		inline virtual bool Replace(const bool &Element1, const bool &Element2);
		inline virtual bool ReplaceAtIndex(uint32 nIndex, const bool &Element);
		virtual bool &Add() override;
		virtual bool &Add(const bool &Element) override;
		virtual uint32 Add(const bool *pElements, uint32 nCount) override;
		inline virtual Container<bool> &operator +=(const bool &Element);
		virtual bool Add(const Container<bool> &lstContainer, uint32 nStart = 0, uint32 nCount = 0) override;
		inline virtual Container<bool> &operator +=(const Container<bool> &lstContainer);
		virtual bool &AddAtIndex(int nIndex) override;
		virtual bool &AddAtIndex(const bool &Element, int nIndex) override;
		inline virtual bool Remove(const bool &Element);
		virtual bool RemoveAtIndex(uint32 nElement) override;
		inline virtual Container<bool> &operator -=(const bool &Element);
		virtual bool Remove(const Container<bool> &lstContainer, uint32 nStart = 0, uint32 nCount = 0) override;
		inline virtual Container<bool> &operator -=(const Container<bool> &lstContainer);
		virtual bool Copy(const Container<bool> &lstContainer, uint32 nStart = 0, uint32 nCount = 0) override;
		inline virtual Container<bool> &operator =(const Container<bool> &lstContainer);
		virtual bool Compare(const Container<bool> &lstContainer, uint32 nStart = 0, uint32 nCount = 0) const override;
		inline virtual bool operator ==(const Container<bool> &lstContainer) const;
		inline virtual bool operator !=(const Container<bool> &lstContainer) const;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Container/Bitset.inl"


#endif // __PLCORE_CONTAINER_BITSET_H__
