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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Class.h"
#include "ClassTypeInfo.h"
#include "PrimitiveTypeInfo.h"
#include "TypeRegistry.h"
#include <PLCore/Typebase/TypeTraits.h>
#include <PLCore/Typebase/Function.h>
#include <PLCore/Typebase/Constructor.h>


#ifndef __PLCORE_REFL_RTTI_H__
#define __PLCORE_REFL_RTTI_H__
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Internal macros                                       ]
//[-------------------------------------------------------]
/**
*  @brief
*    Declare a reflected class
*
*  @param[in] CLSS
*    The class type to declare
*  @param[in] COPYABLE
*    Whether the class has defined copy and assignment
*/
#define __pl_declare_class(CLSS, COPYABLE) \
	template<> struct PLCore::StaticTypeInfo<CLSS> { \
		static PLCore::TypeInfo *Get() \
		{ \
			static PLCore::ClassTypeInfo info(#CLSS); \
			static bool registered = false; \
			if (!registered) \
			{ \
				registered = true; \
				PLCore::TypeRegistry::GetInstance()->RegisterClassType(#CLSS, &info); \
			} \
			return &info; \
		} \
		enum { Defined = true, Copyable = COPYABLE }; \
	};

//[-------------------------------------------------------]
//[ RTTI macros                                           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Declare a C++ type within the reflection system
*
*  @remarks
*    This must be used with the class declaration (typically in the header file) and must be
*    accompanied by the pl_rtti() macro within the class itself.
*
*    Example:
* 
*  @code
*    class MyClass {
*
*        pl_rtti();
*
*        class MyInnerClass {
*
*            pl_rtti();
*        };
*    };
*    pl_declare_type(MyClass)
*    pl_declare_type(MyClass::MyInnerClass)
*  @endcode
*
*  @param[in] CLSS
*    The class type to declare
*/
#define pl_declare_class(CLSS) \
	__pl_declare_class(CLSS, true)

/**
*  @brief
*    Declare a non-copyable C++ type within the reflection system
*
*  @param[in] CLSS
*    The class type to declare
*/
#define pl_declare_class_no_copy(CLSS) \
	__pl_declare_class(CLSS, false)

/**
*  @brief
*    Declare a reflected primitive type
*
*  @param[in] TYPE
*    Name of the type (will be used as ID as well)
*/
#define pl_declare_basic_type(TYPE) \
	template<> struct PLCore::StaticTypeInfo<TYPE> { \
		static PLCore::TypeInfo *Get() \
		{ \
			static PLCore::PrimitiveTypeInfo info(#TYPE); \
			static bool registered = false; \
			if (!registered) \
			{ \
				registered = true; \
				PLCore::TypeRegistry::GetInstance()->RegisterPrimitiveType(#TYPE, &info); \
			} \
			return &info; \
		} \
		enum { Defined = true, Copyable = true }; \
	};

/**
*  @brief
*    Declare reflection support inside a class
*
*  @remarks
*    This macro will enable the reflection functionality inside the containing class
*/
#define pl_rtti() \
	public: \
		static void _RegisterReflection(); \
		static const PLCore::ClassTypeInfo *GetStaticClassTypeInfo(); \
		virtual const PLCore::ClassTypeInfo *GetClassTypeInfo() const { return (PLCore::ClassTypeInfo*)PLCore::GetStaticTypeInfo(this); } \
	private:

/**
*  @brief
*    Declare reflection support inside a structure
*
*  @remarks
*    This macro is to be used instead of pl_rtti with types that are not supposed to have
*    polymorphic properties (i.e. no virtual methods)
*/
#define pl_sti() \
	public: \
		static void _RegisterReflection(); \
		static const PLCore::ClassTypeInfo *GetStaticClassTypeInfo(); \
	private:

/**
*  @brief
*    Begin a definition of a reflected type
*
*  @remarks
*    This is to be used in the implementation (.cpp) file and requires the type to be first
*    declared using the other macros provided (pl_rtti or pl_sti & pl_declare_type)
*
*  @param[in] CLSS
*    The type to define reflection for (without namespace!)
*  @param[in] NAMESPACe
*    The namespace the type resides in
*/
#define pl_begin_class(CLSS, NAMESPACE) \
	const PLCore::ClassTypeInfo *NAMESPACE::CLSS::GetStaticClassTypeInfo() { return (const PLCore::ClassTypeInfo*)PLCore::StaticTypeInfo<NAMESPACE::CLSS>::Get(); } \
	struct CLSS##_Register { \
		CLSS##_Register() \
		{ \
			NAMESPACE::CLSS::_RegisterReflection(); \
		} \
	}; \
	static CLSS##_Register __##CLSS##_Register; \
	\
	void NAMESPACE::CLSS::_RegisterReflection() \
	{ \
		typedef NAMESPACE::CLSS _Clss; \
		PLCore::Class::Declare<CLSS>(#CLSS) \

/**
*  @brief
*    End the definition of a reflected type
*/
#define pl_end_class() \
	;}

/**
*  @brief
*    Connect the current class to its base
*/
#define pl_base_class(NAME) \
	.Base(#NAME)

/**
*  @brief
*    Add a constructor with the given signature to the class
*/
#define pl_ctor(...) \
	.Constructor(new PLCore::Constructor<_Clss, __VA_ARGS__>())

/**
*  @brief
*    Attach current class' member function to the reflection
*/
#define pl_method(NAME) \
	.Method(#NAME, new PLCore::Function<decltype(&_Clss::NAME)>(&_Clss::NAME))

/**
*  @brief
*    Attach a property to the current class
*/
#define pl_property(NAME) \
	.Property(#NAME, new PLCore::Function<decltype(&_Clss::Set##NAME)>(&_Clss::Set##NAME), new PLCore::Function<decltype(&_Clss::Get##NAME)>(&_Clss::Get##NAME))

/**
*  @brief
*    Attach a property to the current class
*/
#define pl_property_getset(NAME, GETTER, SETTER) \
	.Property(#NAME, new PLCore::Function<decltype(&_Clss::SETTER)>(&_Clss::SETTER), new PLCore::Function<decltype(&_Clss::GETTER)>(&_Clss::GETTER))

/**
*  @brief
*    Helper to automatically declare class properties for use with the pl_property macro
*/
#define pl_declare_property(NAME, TYPE) \
	protected: \
		TYPE NAME; \
	public: \
		void Set##NAME(const TYPE &cValue) { NAME = cValue; } \
		const TYPE &Get##NAME() const { return NAME; }

/**
*  @brief
*    Set default value for property or field
*/
#define pl_default(VALUE) // [TODO]

/**
*  @brief
*    Add a tag to the last defined tag holder
*/
#define pl_tag(NAME, VALUE) \
	.Tag(NAME, VALUE)

/**
*  @brief
*    Helper for string tags (as you cannot store const char[N] in a tag)
*/
#define pl_str_tag(NAME, VALUE) \
	pl_tag(NAME, PLCore::String(VALUE))

/**
*  @brief
*    Unified tag to provide descriptions of object
*/
#define pl_desc(DESC) \
	pl_str_tag("Description", DESC)

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Core types                                            ]
//[-------------------------------------------------------]
#include "PrimitiveTypes.inl"


#endif // __PLCORE_REFL_RTTI_H__
