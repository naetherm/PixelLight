/*********************************************************\
 *  File: EventBase.h                                    *
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


#ifndef __PLCORE_EVENTBASE_H__
#define __PLCORE_EVENTBASE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Container/SimpleList.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class EventHandlerBase;

//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Generic event base class
*/
template <typename T0 = NullType, typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType, typename T5 = NullType, typename T6 = NullType, typename T7 = NullType, typename T8 = NullType, typename T9 = NullType, typename T10 = NullType, typename T11 = NullType, typename T12 = NullType, typename T13 = NullType, typename T14 = NullType, typename T15 = NullType>
class EventBase {
	public:
		
		typedef EventHandlerBase<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> EventHandlerType;
		
		/**
		*  @brief
		*    Constructor
		*/
		EventBase() {}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~EventBase()
		{
			// Force disconnection of event handlers
			const typename SimpleList<EventHandlerType*>::ListElement *pElement = m_lstHandlers.pFirstElement;
			while (pElement) {
				pElement->Data->m_lstEvents.Remove(this);
				pElement = pElement->pNextElement;
			}
		}

		/**
		*  @brief
		*    Connect event handler to the event
		*
		*  @param[in] cHandler
		*    Event handler
		*/
		void Connect(EventHandlerType &cHandler)
		{
			// The handler must not be connected already
			if (!cHandler.m_lstEvents.IsElement(this)) {
				// Connect handler
				m_lstHandlers.Add(&cHandler);
				cHandler.m_lstEvents.Add(this);
			}
		}


		/**
		*  @brief
		*    Disconnect event handler from the event
		*
		*  @param[in] cHandler
		*    Event handler
		*/
		void Disconnect(EventHandlerType &cHandler)
		{
			// Disconnect handler
			if (m_lstHandlers.IsElement(&cHandler)) {
				m_lstHandlers.Remove(&cHandler);
				cHandler.m_lstEvents.Remove(this);
			}
		}

		/**
		*  @brief
		*    Return the number of connections
		*
		*  @return
		*    The number of connections
		*
		*  @note
		*    - Don't use this method within performance critical situations because due internal
		*      implementation, this request has a linear, not constant runtime!
		*/
		uint32 GetNumOfConnects() const
		{
			return m_lstHandlers.GetNumOfElements();
		}

	protected:
		SimpleList<EventHandlerType*> m_lstHandlers;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_EVENTBASE_H__
