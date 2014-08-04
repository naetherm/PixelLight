/*********************************************************\
 *  File: PLCore.h                                       *
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


#ifndef __PLCORE_TUPLE_H__
#define __PLCORE_TUPLE_H__
#pragma once

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
#include <PLCore/Typebase/UntypedVariant.h>
#include <PLCore/Container/Iterable.h>

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {

//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
template <int I, typename T>
struct TupleElement;

//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Tuple value holder
*/
template <int I, typename TValue>
class TupleLeaf {

	//[-------------------------------------------------------]
	//[ Public types                                          ]
	//[-------------------------------------------------------]
	public:
		typedef TypeStorage<TValue> Storage;
		typedef typename Storage::StorageType StorageType;

	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		TupleLeaf() : m_cValue() {}

		/**
		*  @brief
		*    Copy constructor
		*/
		TupleLeaf(const TupleLeaf &cLeaf) : m_cValue(cLeaf.Get()) {}

		/**
		*  @brief
		*    Value constructor
		*
		*  @param[in] cVal
		*    The value to initialize the leaf with
		*/
		template <typename T>
		explicit TupleLeaf(T &&cVal) : m_cValue(static_cast<T&&>(cVal)) {}

		/**
		*  @brief
		*    Create the leaf from another one on the same position
		*/
		template <typename T>
		explicit TupleLeaf(const TupleLeaf<I, T> &cLeaf) : m_cValue(cLeaf.Get()) {}

		/**
		*  @brief
		*    The assignment operator
		*/
		template <typename T>
		TupleLeaf &operator=(T &&cVal)
		{
			m_cValue = static_cast<T&&>(cVal);
			return *this;
		}

		/**
		*  @brief
		*    Get this leaf's data
		*/
		StorageType &Get()
		{
			return m_cValue;
		}

		/**
		*  @brief
		*    Get this leaf's data
		*/
		const StorageType &Get() const
		{
			return m_cValue;
		}

	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		StorageType m_cValue;		/**< The value held by this leaf */
};

/**
*  @brief
*    Helper to hold tuple's indices
*/
template <int... Indices>
struct TupleIndices {

};

/**
*  @brief
*    Helper to generate the tuple indices
*/
template <int Start, typename TupleIndices, int End>
struct MakeTupleIndicesImpl;

template <int Start, int... Indices, int End>
struct MakeTupleIndicesImpl<Start, TupleIndices<Indices...>, End> {

	typedef typename MakeTupleIndicesImpl<Start + 1, TupleIndices<Indices..., Start>, End>::_Type _Type;
};

template <int End, int... Indices>
struct MakeTupleIndicesImpl<End, TupleIndices<Indices...>, End> {
	
	typedef TupleIndices<Indices...> _Type;
};

template <int End, int Start = 0>
struct MakeTupleIndices {

	typedef typename MakeTupleIndicesImpl<Start, TupleIndices<>, End>::_Type _Type;
};

/**
*  @brief
*    Tuple core implementation
*
*  @remarks
*    This implements a 'flat' tuple using multiple inheritance
*/
template <typename TIndices, typename... Types>
struct TupleImpl;

template <int... Indices, typename... Types>
struct TupleImpl<TupleIndices<Indices...>, Types...> : public TupleLeaf<Indices, Types>... {

};

/**
*  @brief
*    Holder of a generic sequence of typed values
*/
template <typename... Types>
class Tuple {

	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		Tuple() {}

		/**
		*  @brief
		*    Value constructor
		*/
//		Tuple(Types... values) { /* [TODO] */ }

	//[-------------------------------------------------------]
	//[ Private types                                         ]
	//[-------------------------------------------------------]
	private:
		typedef TupleImpl<typename MakeTupleIndices<sizeof...(Types)>::_Type, Types...> _Impl;

	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	public:
		_Impl m_cImpl;
};

/**
*  @brief
*    Tuple element type helper
*/
template <int I, typename Head, typename... Tail>
struct TupleElement<I, Tuple<Head, Tail...>> : TupleElement<I-1, Tuple<Tail...>> {
};

template <typename Head, typename... Tail>
struct TupleElement<0, Tuple<Head, Tail...>> {

	typedef Head Type;
	typedef typename TypeStorage<Type>::StorageType StorageType;
};

/**
*  @brief
*    Get the value on the specified index from the tuple
*/
template <int I, typename... T>
const typename TupleElement<I, Tuple<T...>>::Type TupleGet(const Tuple<T...> &cTuple)
{
	typedef typename TupleElement<I, Tuple<T...>>::Type Type;
	return TypeStorage<Type>::Restore(static_cast<const TupleLeaf<I, Type>&>(cTuple.m_cImpl).Get());
}

/**
*  @brief
*    Get the value on the specified index from the tuple
*/
template <int I, typename... T>
typename TupleElement<I, Tuple<T...>>::Type TupleGet(Tuple<T...> &cTuple)
{
	typedef typename TupleElement<I, Tuple<T...>>::Type Type;
	return TypeStorage<Type>::Restore(static_cast<TupleLeaf<I, Type>&>(cTuple.m_cImpl).Get());
}

/**
*  @brief
*    Store the value on the specified index from the tuple
*/
template <int I, typename... T>
typename TupleElement<I, Tuple<T...>>::StorageType &TupleStore(Tuple<T...> &cTuple)
{
	typedef typename TupleElement<I, Tuple<T...>>::Type Type;
	return static_cast<TupleLeaf<I, Type>&>(cTuple.m_cImpl).Get();
}

/**
*  @brief
*    Helper to unroll tuple's indices
*/
template <int... I>
struct IndexSequence {
};

template <int N, int... I>
struct MakeIndexSequence : public MakeIndexSequence<N - 1, N - 1, I...> {
};

template <int... I>
struct MakeIndexSequence<0, I...> : public IndexSequence<I...>{
};

/**
*  @brief
*    Construct tuple from untyped variant
*/
template <int BUFFER_SIZE, typename... T>
void TupleFromUntypedVariant(const Iterable<UntypedVariant<BUFFER_SIZE>> *pDyn, Tuple<T...> &cTuple)
{
	ConstIterator<UntypedVariant<BUFFER_SIZE>> iter = pDyn->GetConstIterator();
	TupleFromUntypedVariantImpl<sizeof...(T), BUFFER_SIZE, T...>::Make(iter, cTuple);
}

template <int N, int BUFFER_SIZE, typename... T>
struct TupleFromUntypedVariantImpl {
	
	static void Make(ConstIterator<UntypedVariant<BUFFER_SIZE>> &cIterator, Tuple<T...> &cTuple)
	{
		TupleFromUntypedVariantImpl<N - 1, BUFFER_SIZE, T...>::Make(cIterator, cTuple);

		typedef typename TupleElement<N - 1, Tuple<T...>>::Type OriginalType;
		typedef typename TupleElement<N - 1, Tuple<T...>>::StorageType StorageType;

		if (cIterator.HasNext())
		{
			OriginalType v = cIterator.Next().Get<OriginalType>();
			StorageType s = TypeStorage<OriginalType>::Store(v);
			TupleStore<N - 1>(cTuple) = s;
		}
	}
};

template <int BUFFER_SIZE, typename... T>
struct TupleFromUntypedVariantImpl<0, BUFFER_SIZE, T...> {
	
	static void Make(ConstIterator<UntypedVariant<BUFFER_SIZE>> &cIterator, Tuple<T...> &cTuple)
	{
		// End of line
	}
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_TUPLE_H__
