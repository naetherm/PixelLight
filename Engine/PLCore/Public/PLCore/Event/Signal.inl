/*********************************************************\
 *  File: Signal.inl                                     *
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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Emit the signal
*/
template <typename... TArgs>
void Signal<TArgs...>::Emit(TArgs... args) const
{
	Invoke(args...);
}

/**
*  @brief
*    Connect a slot to the signal
*/
template <typename... TArgs>
void Signal<TArgs...>::Connect(const SlotType &cSlot)
{
	// [TODO] Check for duplicity?
	m_lstSlots.Add(cSlot);
}

/**
*  @brief
*    Connect a slot to the signal
*/
template <typename... TArgs>
void Signal<TArgs...>::operator+=(const SlotType &cSlot)
{
	Connect(cSlot);
}

/**
*  @brief
*    Disconnect a slot from the signal
*/
template <typename... TArgs>
void Signal<TArgs...>::Disconnect(const SlotType &cSlot)
{
	// [TODO] Check for multiple instances?
	m_lstSlots.Remove(cSlot);
}

/**
*  @brief
*    Disconnect a slot from the signal
*/
template <typename... TArgs>
void Signal<TArgs...>::operator-=(const SlotType &cSlot)
{
	Disconnect(cSlot);
}

/**
*  @brief
*    Disconnect all slots from the signal
*/
template <typename... TArgs>
void Signal<TArgs...>::DisconnectAll()
{
	m_lstSlots.Clear();
}


//[-------------------------------------------------------]
//[ Public virtual Invokable functions                    ]
//[-------------------------------------------------------]
/**
*  @brief
*    Emit the signal
*/
template <typename... TArgs>
void Signal<TArgs...>::Invoke(TArgs... args) const
{
	auto slots = m_lstSlots.GetConstIterator();
	while (slots.HasNext()) {
		// Signal the slot
		const SlotType &slot = slots.Next();
		slot.Signal(args...);
	}
}

/**
*  @brief
*    Retrieve the signature
*/
template <typename... TArgs>
FunctionSignature Signal<TArgs...>::GetSignature() const
{
	return FunctionSignature::FromTemplate<void, TArgs...>();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
