/*********************************************************\
 *  File: BinominalHeap.h                                *
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


#ifndef __PLCORE_CONTAINER_BINOMINALHEAP_H__
#define __PLCORE_CONTAINER_BINOMINALHEAP_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Container/Heap.h"
#include "PLCore/Container/Functions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer> class BinominalHeapIterator;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Binominal heap
*/
template <class KeyType, class ValueType, class Comparer = CompareFunction >
class BinominalHeap : public Heap<KeyType, ValueType> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class BinominalHeapIterator<KeyType, ValueType, Comparer>;


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
		BinominalHeap();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BinominalHeap();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		BinominalHeap(const BinominalHeap<KeyType, ValueType, Comparer> &cSource);

		/**
		*  @brief
		*    Makes this heap to a copy of another heap
		*
		*  @param[in] cHeap
		*    'BinominalHeap' to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Heap<KeyType, ValueType> &operator =(const BinominalHeap<KeyType, ValueType, Comparer> &cHeap);


	//[-------------------------------------------------------]
	//[ Private classes                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Internal binominal tree
		*/
		class Tree {


			//[-------------------------------------------------------]
			//[ Friends                                               ]
			//[-------------------------------------------------------]
			friend class BinominalHeap<KeyType, ValueType, Comparer>;
			friend class BinominalHeapIterator<KeyType, ValueType, Comparer>;


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] Key
				*    Key
				*  @param[in] Value
				*    Value
				*/
				Tree(KeyType Key, ValueType Value);

				/**
				*  @brief
				*    Destructor
				*/
				~Tree();

				/**
				*  @brief
				*    Union
				*
				*  @param[in] cTree
				*    Tree to union with this tree
				*
				*  @return
				*    The (new) root node
				*/
				Tree &Union(Tree &cTree);


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				uint32	   m_nDegree;		/**< Degree of this tree node */
				Tree	  *m_pNextSibling;	/**< Next sibling tree, can be a null pointer */
				Tree	  *m_pChild;		/**< Child tree, can be a null pointer */
				KeyType	   m_Key;			/**< Key of this tree node */
				ValueType  m_Value;			/**< Value of this tree node */


		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32	m_nNumOfElements;	/**< Current number of elements */
		Tree   *m_pRoot;			/**< Root binominal tree, can be a null pointer */


	//[-------------------------------------------------------]
	//[ Public virtual Iterable functions                     ]
	//[-------------------------------------------------------]
	public:
		virtual Iterator<ValueType> GetIterator(uint32 nIndex = 0) const override;
		virtual ConstIterator<ValueType> GetConstIterator(uint32 nIndex = 0) const override;
		virtual Iterator<ValueType> GetEndIterator() const override;
		virtual ConstIterator<ValueType> GetConstEndIterator() const override;


	//[-------------------------------------------------------]
	//[ Public virtual Heap functions                         ]
	//[-------------------------------------------------------]
	public:
		virtual void Clear() override;
		virtual bool IsEmpty() const override;
		virtual uint32 GetNumOfElements() const override;
		virtual bool Add(const KeyType &Key, const ValueType &Value) override;
		virtual bool GetTop(ValueType *pValue = nullptr, KeyType *pKey = nullptr) const override;
		virtual bool ExtractTop(ValueType *pValue = nullptr, KeyType *pKey = nullptr) override;


	//[-------------------------------------------------------]
	//[ Private virtual Heap functions                        ]
	//[-------------------------------------------------------]
	private:
		virtual Heap<KeyType, ValueType> &operator =(const Heap<KeyType, ValueType> &cHeap) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Container/BinominalHeap.inl"


#endif // __PLCORE_CONTAINER_BINOMINALHEAP_H__
