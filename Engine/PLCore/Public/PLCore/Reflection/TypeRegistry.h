/*********************************************************\
 *  File: TypeRegistry.h                                 *
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


#ifndef __PLCORE_REFL_TYPEREGISTRY_H__
#define __PLCORE_REFL_TYPEREGISTRY_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Core/Singleton.h>
#include <PLCore/Container/HashMap.h>
#include <PLCore/String/String.h>

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class ClassTypeInfo;
class PrimitiveTypeInfo;

//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    The central reflected type registry
*
*  @remarks
*    TODO: describe this in more detail
*/
class TypeRegistry : public PLCore::Singleton<TypeRegistry> {


	//[-------------------------------------------------------]
	//[ Public static PLCore::Singleton functions             ]
	//[-------------------------------------------------------]
	// This solution enhances the compatibility with legacy compilers like GCC 4.2.1 used on Mac OS X 10.6
	// -> The C++11 feature "extern template" (C++11, see e.g. http://www2.research.att.com/~bs/C++0xFAQ.html#extern-templates) can only be used on modern compilers like GCC 4.6
	// -> We can't break legacy compiler support, especially when only the singletons are responsible for the break
	// -> See PLCore::Singleton for more details about singletons
	public:
	static PLCORE_API TypeRegistry *GetInstance();
	static PLCORE_API bool HasInstance();


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Register a new class into the registry
		*
		*  @param[in] sName
		*    Name of the type to register
		*  @param[in] pTypeInfo
		*    The type info for the class
		*/
		PLCORE_API void RegisterClassType(const PLCore::String &sName, ClassTypeInfo *pTypeInfo);

		/**
		*  @brief
		*    Register a new primitive into the registry
		*
		*  @param[in] sName
		*    Name of the type to register
		*  @param[in] pTypeInfo
		*    The type info for the type
		*/
		PLCORE_API void RegisterPrimitiveType(const PLCore::String &sName, PrimitiveTypeInfo *pTypeInfo);

		/**
		*  @brief
		*    Find a class type by name
		*
		*  @param[in] sName
		*    Name of the class type to find
		*
		*  @return
		*    Pointer to the type info or nullptr of it was not found
		*/
		PLCORE_API ClassTypeInfo *GetClassType(const PLCore::String &sName);

		/**
		*  @brief
		*    Find a primitive type by name
		*
		*  @param[in] sName
		*    Name of the primitive type to find
		*
		*  @return
		*    Pointer to the type info or nullptr of it was not found
		*/
		PLCORE_API PrimitiveTypeInfo *GetPrimitiveType(const PLCore::String &sName);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		typedef PLCore::HashMap<PLCore::String, ClassTypeInfo*> _ClassTypeMap;
		_ClassTypeMap m_mapClassTypes;				/**< All known class types by name */

		typedef PLCore::HashMap<PLCore::String, PrimitiveTypeInfo*> _PrimitiveTypeMap;
		_PrimitiveTypeMap m_mapPrimitiveTypes;		/**< All known class types by name */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "TypeRegistry.inl"


#endif // __PLCORE_REFL_TYPEREGISTRY_H__
