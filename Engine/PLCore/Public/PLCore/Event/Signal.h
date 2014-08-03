/*********************************************************\
 *  File: Signal.h                                       *
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


#ifndef __PLCORE_SIGNAL_H__
#define __PLCORE_SIGNAL_H__
#pragma once

//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Typebase/Invokable.h"
#include "PLCore/Container/List.h"
#include "Slot.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Event source (signal) class
*
*  @remarks
*    This implements a multi-cast delegate pattern
*/
template <typename... TArgs>
class Signal : public Invokable<void, TArgs...> {


	//[-------------------------------------------------------]
	//[ Public types                                          ]
	//[-------------------------------------------------------]
	public:
		typedef Slot<TArgs...> SlotType;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Emit the signal with the specified arguments
		*/
		void Emit(TArgs... args) const;

		/**
		*  @brief
		*    Connect this signal (event) to the specified slot (event handler)
		*/
		void Connect(const SlotType &cSlot);

		/**
		*  @brief
		*    Alternative way to connect a slot to this signal
		*/
		void operator+=(const SlotType &cSlot);

		/**
		*  @brief
		*    Disconnect the given slot from this signal
		*/
		void Disconnect(const SlotType &cSlot);

		/**
		*  @brief
		*    Alternative way to disconnect a slot from this signal
		*/
		void operator-=(const SlotType &cSlot);

		/**
		*  @brief
		*    Disconnect all slots from this signal
		*/
		void DisconnectAll();


	//[-------------------------------------------------------]
	//[ Public virtual Invokable functions                    ]
	//[-------------------------------------------------------]
	public:
		virtual void Invoke(TArgs... args) const override;
		virtual FunctionSignature GetSignature() const override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		List<SlotType>	m_lstSlots;		/**< List of all slots currently connected to this signal */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "Signal.inl"


#endif // __PLCORE_SIGNAL_H__
