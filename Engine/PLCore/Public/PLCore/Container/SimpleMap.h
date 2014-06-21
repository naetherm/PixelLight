/*********************************************************\
 *  File: SimpleMap.h                                    *
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


#ifndef __PLCORE_CONTAINER_SIMPLEMAP_H__
#define __PLCORE_CONTAINER_SIMPLEMAP_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Container/Map.h"
#include "PLCore/Container/FastPool.h"
#include "PLCore/Container/Functions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer> class SimpleMapIterator;
template <class KeyType, class ValueType, class Comparer> class SimpleMapKeyIterator;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Simple map template
*
*  @remarks
*    This map implementation is quite simple, but not that performant. It's not recommended
*    to use this implementation if you have a lot of elements within the map.
*/
template <class KeyType, class ValueType, class Comparer = CompareFunction>
class SimpleMap : public Map<KeyType, ValueType> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SimpleMapIterator<KeyType, ValueType, Comparer>;
	friend class SimpleMapKeyIterator<KeyType, ValueType, Comparer>;


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
		SimpleMap();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SimpleMap();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    'SimpleMap' to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Map<KeyType, ValueType> &operator =(const SimpleMap<KeyType, ValueType, Comparer> &cSource);


	//[-------------------------------------------------------]
	//[ Private structures                                    ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Internal fast pool element
		*/
		class Element : public FastPoolElement<Element> {
			public:
				KeyType	  Key;		/**< The key*/
				ValueType Value;	/**< The value */
				bool operator ==(const Element &cOther) const
				{
					return false;
				}
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		FastPool<Element> m_lstElements;	/**< List of elements */


	//[-------------------------------------------------------]
	//[ Public virtual Iterable functions                     ]
	//[-------------------------------------------------------]
	public:
		virtual Iterator<ValueType> GetIterator(uint32 nIndex = 0) const override;
		virtual ConstIterator<ValueType> GetConstIterator(uint32 nIndex = 0) const override;
		virtual Iterator<ValueType> GetEndIterator() const override;
		virtual ConstIterator<ValueType> GetConstEndIterator() const override;


	//[-------------------------------------------------------]
	//[ Public virtual Map functions                          ]
	//[-------------------------------------------------------]
	public:
		virtual void Clear() override;
		virtual bool IsEmpty() const override;
		virtual uint32 GetNumOfElements() const override;
		virtual bool Add(const KeyType &Key, const ValueType &Value) override;
		virtual bool Replace(const KeyType &Key, const ValueType &NewValue) override;
		virtual bool Set(const KeyType &Key, const ValueType &Value) override;
		virtual bool Remove(const KeyType &Key) override;
		virtual uint32 RemoveValue(const ValueType &Value) override;
		virtual const ValueType &Get(const KeyType &Key) const override;
		virtual ValueType &Get(const KeyType &Key) override;
		virtual Iterator<KeyType> GetKeyIterator(uint32 nIndex = 0) const override;
		virtual ConstIterator<KeyType> GetConstKeyIterator(uint32 nIndex = 0) const override;
		virtual Iterator<KeyType> GetEndKeyIterator() const override;
		virtual ConstIterator<KeyType> GetConstEndKeyIterator() const override;


	//[-------------------------------------------------------]
	//[ Private virtual Map functions                         ]
	//[-------------------------------------------------------]
	private:
		virtual Map<KeyType, ValueType> &operator =(const Map<KeyType, ValueType> &cMap) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Container/SimpleMap.inl"


#endif // __PLCORE_CONTAINER_SIMPLEMAP_H__
