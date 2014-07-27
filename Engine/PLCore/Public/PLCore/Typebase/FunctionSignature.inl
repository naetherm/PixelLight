/*********************************************************\
 *  File: FunctionSignature.inl                          *
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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Tuple.h"
#include <PLCore/Reflection/TypeInfo.h>

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Helper to fill an array of TypeInfo's from a variadic template argument list
*/
template <int N, typename... T>
struct TemplateToTypeInfoArray {

	static void Make(Array<const PLRefl::TypeInfo*> &lstTypes)
	{
		TemplateToTypeInfoArray<N - 1, T...>::Make(lstTypes);

		typedef typename TupleElement<N - 1, Tuple<T...>>::_Type _Type;
		lstTypes[N - 1] = PLRefl::StaticTypeInfo<_Type>::Get();
	}
};

template <typename... T>
struct TemplateToTypeInfoArray<0, T...> {

	static void Make(Array<const PLRefl::TypeInfo*> &lstTypes)
	{
		// End of line
	}
};

/**
*  @brief
*    Typed construction of function signature
*/
template <typename TRet, typename... TArgs>
FunctionSignature FunctionSignature::FromTemplate()
{
	// Return type
	PLRefl::TypeInfo* ret = PLRefl::StaticTypeInfo<TRet>::Get();

	// Arguments
	Array<const PLRefl::TypeInfo*> args;
	args.Resize(sizeof...(TArgs));
	TemplateToTypeInfoArray<sizeof...(TArgs), TArgs...>::Make(args);

	return FunctionSignature(ret, args);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
