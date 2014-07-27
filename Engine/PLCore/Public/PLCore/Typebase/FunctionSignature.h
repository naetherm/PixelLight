/*********************************************************\
 *  File: FunctionSignature.h                            *
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
#include <PLCore/Container/Array.h>

#ifndef __PLCORE_FUNCTIONSIGNATURE_H__
#define __PLCORE_FUNCTIONSIGNATURE_H__
#pragma once


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRefl
{
	class TypeInfo;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Signature of a function
*/
class FunctionSignature {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default ctor will create a signature of function void(*)()
		*/
		PLCORE_API FunctionSignature();

		/**
		*  @brief
		*    Value constructor
		*
		*  @param[in] pReturnType
		*    TypeInfo of the function return type
		*  @param[in] lstArgTypes
		*    Array containing TypeInfo structures for function arguments
		*/
		PLCORE_API FunctionSignature(const PLRefl::TypeInfo *pReturnType, const Array<const PLRefl::TypeInfo*> &lstArgTypes);

		/**
		*  @brief
		*    Typed constructor
		*/
		template <typename TRet = void, typename... TArgs>
		static FunctionSignature FromTemplate();

		/**
		*  @brief
		*    Get return type of the function
		*/
		inline const PLRefl::TypeInfo *GetReturnType() const;

		/**
		*  @brief
		*    Get argument types of the function
		*/
		inline const Array<const PLRefl::TypeInfo*> &GetArgumentTypes() const;

		/**
		*  @brief
		*    Check if two signatures describe the same function
		*/
		inline bool operator==(const FunctionSignature &cOther) const;

	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const PLRefl::TypeInfo			*m_pReturnType;		/**< Function return type */
		Array<const PLRefl::TypeInfo*>	m_lstArgTypes;		/**< Argument types */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "FunctionSignature.inl"


#endif // __PLCORE_FUNCTIONSIGNATURE_H__
