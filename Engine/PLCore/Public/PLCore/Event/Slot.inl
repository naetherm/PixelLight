/*********************************************************\
 *  File: Slot.inl                                       *
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
*    Constructor
*/
template <typename... TArgs>
template <class T>
Slot<TArgs...>::Slot(T *pObj, Invokable<void, T*, TArgs...> *pFunc) :
	m_pObject(pObj),
	m_pFunction(pFunc)
{
	// Initialize the invoker
	m_pInvoker = &Slot::Invoker<T>;
}

/**
*  @brief
*    Signal the slot
*/
template <typename... TArgs>
void Slot<TArgs...>::Signal(TArgs... args) const
{
	(*m_pInvoker)(m_pObject, m_pFunction, args...);
}

/**
*  @brief
*    Check equality
*/
template <typename... TArgs>
bool Slot<TArgs...>::operator==(const Slot &cOther) const
{
	// [TODO] Here we could compare signatures instead of function pointers. This would
	// be a lot more flexible but it would also require the signature to include the function
	// name
	return (m_pObject == cOther.m_pObject && m_pFunction == cOther.m_pFunction);
}


//[-------------------------------------------------------]
//[ Public virtual Invokable functions                    ]
//[-------------------------------------------------------]
/**
*  @brief
*    Invoke the delegate
*/
template <typename... TArgs>
void Slot<TArgs...>::Invoke(void*, TArgs... args) const
{
	// [TODO] Should we maybe use the provided object?
	(*m_pInvoker)(m_pObject, m_pFunction, args...);
}

/**
*  @brief
*    Retrieve the signature
*/
template <typename... TArgs>
FunctionSignature Slot<TArgs...>::GetSignature() const
{
	return FunctionSignature::FromTemplate<void, void*, TArgs...>();
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Slot invocation helper
*/
template <typename... TArgs>
template <class T>
void Slot<TArgs...>::Invoker(void *pObj, FunctionBase *pFunc, TArgs... args)
{
	// Cast untyped objects onto their typed (real) counterparts
	T *pInst = reinterpret_cast<T*>(pObj);
	Invokable<void, T*, TArgs...> *pFunction = static_cast<Invokable<void, T*, TArgs...>*>(pFunc);

	// Invoke the function
	pFunction->Invoke(pInst, args...);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
