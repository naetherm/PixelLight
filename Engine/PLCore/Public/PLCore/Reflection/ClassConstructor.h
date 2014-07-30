/*********************************************************\
 *  File: ClassConstructor.h                             *
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


#ifndef __PLCORE_REFL_CLASSCONSTRUCTOR_H__
#define __PLCORE_REFL_CLASSCONSTRUCTOR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "DynamicObject.h"
#include "TagHolder.h"
#include <PLCore/String/String.h>
#include <PLCore/Typebase/FunctionBase.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRefl {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    The reflection system's representation of a class constructor
*
*  @remarks
*    TODO: describe this in more detail
*/
class ClassConstructor : public TagHolder {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default ctor
		*/
		PLCORE_API ClassConstructor();

		/**
		*  @brief
		*    Value ctor
		*
		*  @param[in] pFunc
		*    The function object representing this constructor
		*/
		PLCORE_API ClassConstructor(PLCore::FunctionBase *pFunc);

		/**
		*  @brief
		*    Construct a new object using this constructor
		*
		*  @param[in] pParams
		*    Dynamic params for construction
		*
		*  @return
		*    Dynamic return value
		*/
		inline DynamicObject Construct(const PLCore::Iterable<PLCore::FunctionParam> *pParams) const;

		/**
		*  @brief 
		*    Get the construstor's signature
		*/
		inline const PLCore::FunctionSignature &GetSignature() const;

		/**
		*  @brief
		*    Comparison operator
		*/
		inline bool operator==(const ClassConstructor &cOther) const;

	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::FunctionBase*		m_pFunc;			/**< The invokable function object */
		PLCore::FunctionSignature	m_cSignature;       /**< Cached m_pFunc's signature */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRefl


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "ClassConstructor.inl"


#endif // __PLCORE_REFL_CLASSCONSTRUCTOR_H__
