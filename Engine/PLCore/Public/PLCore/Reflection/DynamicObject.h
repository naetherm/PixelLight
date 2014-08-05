/*********************************************************\
 *  File: DynamicObject.h                                *
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


#ifndef __PLCORE_DYNAMICOBJECT_H__
#define __PLCORE_DYNAMICOBJECT_H__
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
#include <PLCore/PLCore.h>
#include <PLCore/Typebase/UntypedVariant.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {

//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class TypeInfo;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Holder for object that uses an untyped variant for storage and a TypeInfo to describe the stored object
*/
class DynamicObject {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Construct an empty object
		*/
		PLCORE_API DynamicObject();

		/**
		*  @brief
		*    Construct a dynamic object from an existing instance
		*/
		template <typename T>
		DynamicObject(T cInst);

		/**
		*  @brief
		*    Construct a dynamic object from unknown object and its type info
		*/
		PLCORE_API DynamicObject(const PLCore::UntypedVariant<> &cVariant, const TypeInfo *pType);

		/**
		*  @brief
		*    Set the dynamic object to a new value
		*/
		template <typename T>
		void Set(T cInst);

		/**
		*  @brief
		*    Get the stored untyped object
		*/
		inline const PLCore::UntypedVariant<> &GetUntyped() const;

		/**
		*  @brief
		*    Get the type info of the stored object
		*/
		inline const TypeInfo *GetTypeInfo() const;

		/**
		*  @brief
		*    Get the stored object as a particular type
		*/
		template <typename T>
		inline T GetAs();

		/**
		*  @brief
		*    Get the stored object as a particular type
		*/
		template <typename T>
		inline const T GetAs() const;

		/**
		*  @brief
		*    Comparison
		*/
		inline bool operator==(const DynamicObject &cOther) const;

	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::UntypedVariant<>	m_pStorage;		/**< The untyped object */
		const TypeInfo				*m_pTypeInfo;	/**< Type of the stored object */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "DynamicObject.inl"


#endif // __PLCORE_DYNAMICOBJECT_H__
