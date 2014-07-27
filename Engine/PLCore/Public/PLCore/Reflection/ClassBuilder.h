/*********************************************************\
 *  File: Rtti.h                                         *
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


#ifndef __PLCORE_REFL_CLASSBUILDER_H__
#define __PLCORE_REFL_CLASSBUILDER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
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
*    Class declaration helper
*
*  @remarks
*    TODO: describe this in more detail
*/
template <typename T>
class ClassBuilder {

	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cClss
		*    Reference to the class type instance we'll be constructing
		*/
		ClassBuilder(Class &cClss);

		/**
		*  @brief
		*    Add a base class
		*/
		ClassBuilder<T> &Base(const PLCore::String &sName);

		/**
		*  @brief
		*    Add a constructor
		*/
		ClassBuilder<T> &Constructor(PLCore::FunctionBase *pFn);

		/**
		*  @brief
		*    Define a method inside the class
		*/
		ClassBuilder<T> &Method(const PLCore::String &sName, PLCore::FunctionBase *pFn);

		/**
		*  @brief
		*    Define a field inside the class
		*/
		ClassBuilder<T> &Field(const PLCore::String &sName, PLCore::uint32 nOffset);

		/**
		*  @brief
		*    Define a property inside the class
		*/
		ClassBuilder<T> &Property(const PLCore::String &sName, PLCore::FunctionBase *pSetter, PLCore::FunctionBase *pGetter);
		

	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Class *m_pClass;			/**< The class being built */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRefl


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "ClassBuilder.inl"


#endif // __PLCORE_REFL_CLASSBUILDER_H__
