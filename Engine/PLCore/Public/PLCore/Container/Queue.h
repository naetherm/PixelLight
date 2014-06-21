/*********************************************************\
 *  File: Queue.h                                        *
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


#ifndef __PLCORE_CONTAINER_QUEUE_H__
#define __PLCORE_CONTAINER_QUEUE_H__
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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
template <class ValueType> class QueueIterator;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Queue (FIFO -> First In First Out) template
*
*  @remarks
*  @verbatim
*    Simple extendable queue using a linked list.
*    The queue will hold copies of the type used in the queue,
*    so note that your classes should supply a copy (=) operator!
*
*    Usage example:
*    Queue<MyClass> cQueue;  // Create queue
*    MyClass T, T1, T2, T3;  // Test class instances
*    cQueue.Push(T1);        // Push a copy of T1 onto the queue
*    cQueue.Push(T2);        // Push a copy of T2 onto the queue
*    cQueue.Push(T3);        // Push a copy of T3 onto the queue
*    cQueue.Pop(&T);         // Pop last element (T1)
*    cQueue.Pop(&T);         // Pop last element (T2)
*    cQueue.Pop(&T);         // Pop last element (T3)
*  @endverbatim
*/
template <class ValueType>
class Queue : public Iterable<ValueType> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class QueueIterator<ValueType>;


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
		Queue();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source queue to copy from
		*/
		Queue(const Queue<ValueType> &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		~Queue();

		/**
		*  @brief
		*    Push an element onto the queue (enqueue)
		*
		*  @param[in] Element
		*    New queue element
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool Push(const ValueType &Element);

		/**
		*  @brief
		*    Pops the bottom element from the queue (dequeue)
		*
		*  @param[out] pElement
		*    If not a null pointer, this will receive the popped element
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - On error, pElement is NOT touched, so ensure that you take this case into account!
		*/
		bool Pop(ValueType *pElement = nullptr);

		/**
		*  @brief
		*    Returns the top element without removing it
		*
		*  @return
		*    Top queue element (newest element)
		*/
		ValueType Top() const;

		/**
		*  @brief
		*    Returns the bottom element without removing it
		*
		*  @return
		*    Bottom queue element (oldest element)
		*/
		ValueType Bottom() const;

		/**
		*  @brief
		*    Returns the number of elements on the queue
		*
		*  @return
		*    Number of queue elements
		*/
		uint32 GetNumOfElements() const;

		/**
		*  @brief
		*    Copies the data from another queue
		*
		*  @param[in] cSource
		*    Queue to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Queue<ValueType> &operator =(const Queue<ValueType> &cSource);

		/**
		*  @brief
		*    Clears the whole queue
		*/
		void Clear();


	//[-------------------------------------------------------]
	//[ Public virtual Iterable functions                     ]
	//[-------------------------------------------------------]
	public:
		virtual Iterator<ValueType> GetIterator(uint32 nIndex = 0) const override;
		virtual ConstIterator<ValueType> GetConstIterator(uint32 nIndex = 0) const override;
		virtual Iterator<ValueType> GetEndIterator() const override;
		virtual ConstIterator<ValueType> GetConstEndIterator() const override;


	//[-------------------------------------------------------]
	//[ Private structures                                    ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Internal queue element
		*/
		struct QueueElement {
			QueueElement *pNextElement;	/**< Pointer to the next element on the queue, can be a null pointer */
			ValueType	  Data;			/**< The stored data */
		};


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static ValueType temp;	/** Temp object */


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32		  m_nNumOfElements;	/**< Number of elements on the queue */
		QueueElement *m_pTop;			/**< Pointer to the top element (newest element), can be a null pointer */
		QueueElement *m_pBottom;		/**< Pointer to the bottom element (oldest element), can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Container/Queue.inl"


#endif // __PLCORE_CONTAINER_QUEUE_H__
