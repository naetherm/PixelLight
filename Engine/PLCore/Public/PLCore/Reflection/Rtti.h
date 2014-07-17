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


#ifndef __PLCORE_REFL_RTTI_H__
#define __PLCORE_REFL_RTTI_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "Class.h"
#include <PLCore/Typebase/TypeTraits.h>
#include <PLCore/Typebase/Function.h>

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRefl {


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
#define __pl_declare_type(CLSS, COPYABLE) \
	template<> struct PLRefl::StaticTypeId<CLSS> { \
		static const char *Get() \
		{ \
			return #CLSS; \
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
#define pl_declare_type(CLSS) \
	__pl_declare_type(CLSS, true)

/**
*  @brief
*    Declare a non-copyable C++ type within the reflection system
*
*  @param[in] CLSS
*    The class type to declare
*/
#define pl_declare_type_no_copy(CLSS) \
	__pl_declare_type(CLSS, false)

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
		virtual const char *_ClassId() const { return PLRefl::GetStaticTypeId(this); } \
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
*    The type to define reflection for
*/
#define pl_begin_class(CLSS) \
	struct CLSS##_Register { \
		CLSS##_Register() \
		{ \
			CLSS::_RegisterReflection(); \
		} \
	}; \
	static CLSS##_Register __##CLSS##_Register; \
	\
	void CLSS::_RegisterReflection() \
	{ \
		typedef CLSS _Clss; \
		PLRefl::Class::Declare<CLSS>(#CLSS) \

/**
*  @brief
*    End the definition of a reflected type
*/
#define pl_end_class() \
	;}

/**
*  @brief
*    Attach current class' member function to the reflection
*/
#define pl_class_method(NAME) \
	.Method(#NAME, new PLCore::Function<decltype(&_Clss::NAME)>(&_Clss::NAME))

/**
*  @brief
*    Attach a property to the current class
*/
#define pl_class_property(NAME, GETTER, SETTER) \
	.Property(#NAME, new PLCore::Function<decltype(&_Clss::SETTER)>(&_Clss::SETTER), new PLCore::Function<decltype(&_Clss::GETTER)>(&_Clss::GETTER))

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRefl


#endif // __PLCORE_REFL_RTTI_H__
