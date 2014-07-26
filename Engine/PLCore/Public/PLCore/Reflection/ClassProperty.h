/*********************************************************\
 *  File: ClassMehod.h                                   *
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


#ifndef __PLCORE_REFL_CLASSPROPERTY_H__
#define __PLCORE_REFL_CLASSPROPERTY_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
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
*    The reflection system's representation of a class property
*
*  @remarks
*    TODO: describe this in more detail
*/
class ClassProperty {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default ctor
		*/
		PLCORE_API ClassProperty();

		/**
		*  @brief
		*    Value ctor
		*
		*  @param[in] sName
		*    The name of this property
		*  @param[in] pSetter
		*    The function object representing setter for the property
		*  @param[in] pGetter
		*    The function object representing getter for the property
		*/
		PLCORE_API ClassProperty(const PLCore::String &sName, PLCore::FunctionBase *pSetter, PLCore::FunctionBase *pGetter);

		/**
		*  @brief
		*    Set the property using dynamic value
		*/
		inline void Set(const PLCore::Iterable<PLCore::FunctionParam> *pParams) const;

		/**
		*  @brief
		*    Get the property into a dynamic value
		*/
		inline PLCore::FunctionParam Get(const PLCore::Iterable<PLCore::FunctionParam> *pParams) const;

		/**
		*  @brief
		*    Directly set the property
		*/
		template <typename T, class TObject>
		inline void SetDirect(TObject *pObj, T cValue) const;

		/**
		*  @brief
		*    Directly get the property
		*/
		template <typename T, class TObject>
		inline T GetDirect(TObject *pObj) const;

		/**
		*  @brief
		*    Comparison operator
		*/
		inline bool operator==(const ClassProperty &cOther) const;

	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String			m_sName;			/**< The name of the property */
		PLCore::FunctionBase	*m_pSetter;			/**< The setter function */
		PLCore::FunctionBase	*m_pGetter;			/**< The getter function */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRefl


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "ClassProperty.inl"


#endif // __PLCORE_REFL_CLASSPROPERTY_H__
