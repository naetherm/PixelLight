/*********************************************************\
 *  File: PLCoreWindows.h                                *
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


#ifndef __PLCORE_WINDOWS_H__
#define __PLCORE_WINDOWS_H__
#pragma once


//[-------------------------------------------------------]
//[ Macros                                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Backups the current warning state for all warnings
*/
#ifdef __MINGW32__
	#define PL_WARNING_PUSH
#else
	#define PL_WARNING_PUSH __pragma(warning(push))
#endif

/**
*  @brief
*    Restores the last warning state pushed onto the stack
*/
#ifdef __MINGW32__
	#define PL_WARNING_POP
#else
	#define PL_WARNING_POP __pragma(warning(pop))
#endif

/**
*  @brief
*    Disables a warning
*
*  @param[in] WarningID
*    ID of the warning to disable
*/
#ifdef __MINGW32__
	#define PL_WARNING_DISABLE(WarningID)
#else
	#define PL_WARNING_DISABLE(WarningID) __pragma(warning(disable: WarningID))
#endif


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
#ifdef __MINGW32__
	#include <_mingw.h>	// For "__int8", "__int16" and so so definitions
	// Within "_mingw.h", "__int32" is defined as "long" while PixelLight assumes "int", so, we change the definition
	#define __int32 int
	#include <stddef.h>	// For "size_t"
#else
	// Within Microsoft Visual Studio, 'wchar_t' can be defined as native type, in this case we don't need a special include
	#if !defined(_MSC_VER) || !defined(_WCHAR_T_DEFINED)
		#include <wctype.h>
	#endif
#endif


//[-------------------------------------------------------]
//[ C++11 definitions                                     ]
//[-------------------------------------------------------]
// Microsoft Visual Studio 2010: Have a look at http://blogs.msdn.com/b/vcblog/archive/2010/04/06/c-0x-core-language-features-in-vc10-the-table.aspx see which C++11 features are supported

// "nullptr"-definition
#if defined(__INTEL_COMPILER) || !defined(_MSC_VER) || _MSC_VER < 1600	// The Intel C++ compiler has no support for nullptr, 1600 = Microsoft Visual Studio 2010
	/**
	*  @brief
	*    nullptr definition for compilers don't supporting this C++11 feature
	*
	*  @note
	*    - The implementation comes from the "A name for the null pointer: nullptr"-document (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2431.pdf)
	*/
	const class {
		public:
			template<class T> operator T*() const  { return 0; }
			template<class C, class T> operator T C::*() const  { return 0; }
		private:
			void operator&() const;
	} nullptr = {};
#endif


//[-------------------------------------------------------]
//[ Disable warnings                                      ]
//[-------------------------------------------------------]
PL_WARNING_DISABLE(4100) // "'identifier' : unreferenced formal parameter"
PL_WARNING_DISABLE(4201) // "nonstandard extension used : nameless struct/union"
PL_WARNING_DISABLE(4231) // "nonstandard extension used : 'identifier' before template explicit instantiation" -> It's part of C++11 (see e.g. http://www2.research.att.com/~bs/C++0xFAQ.html#extern-templates)
PL_WARNING_DISABLE(4481) // "nonstandard extension used: override specifier 'override'" -> It's part of C++11 (see e.g. http://www2.research.att.com/~bs/C++0xFAQ.html#override)


//[-------------------------------------------------------]
//[ Project independent generic export                    ]
//[-------------------------------------------------------]
// To export classes, methods and variables
#define PL_GENERIC_API_EXPORT	__declspec(dllexport)

// To export RTTI elements
#define PL_GENERIC_RTTI_EXPORT	1


//[-------------------------------------------------------]
//[ Project independent generic import                    ]
//[-------------------------------------------------------]
// To import classes, methods and variables
#define PL_GENERIC_API_IMPORT	__declspec(dllimport)

// To import RTTI elements
#define PL_GENERIC_RTTI_IMPORT	0


//[-------------------------------------------------------]
//[ Import/Export                                         ]
//[-------------------------------------------------------]
// To export/import class templates
#define PLCORE_TMPL

// To export plugin functions
#ifdef PLCORE_STATIC
	#define PLCORE_PLUGIN_API
#else
	#define PLCORE_PLUGIN_API __declspec(dllexport)
#endif


//[-------------------------------------------------------]
//[ Data types                                            ]
//[-------------------------------------------------------]
namespace PLCore {


	// Standard types
	typedef __int8					int8;
	typedef unsigned __int8			uint8;

	typedef __int16					int16;
	typedef unsigned __int16		uint16;

	typedef __int32					int32;
	typedef unsigned __int32		uint32;

	typedef __int64					int64;
	typedef unsigned __int64		uint64;

	#if WIN64
		typedef unsigned __int64	uint_ptr;
		typedef unsigned __int64	handle;
	#else
		typedef unsigned __int32	uint_ptr;
		typedef unsigned __int32	handle;
	#endif


}

/**
*  @brief
*    Invalid handle value (for data type PLCore::handle)
*/
#ifdef WIN64
	static const PLCore::handle NULL_HANDLE    = 0x0000000000000000;
	static const PLCore::handle INVALID_HANDLE = 0xffffffffffffffff;
#else
	static const PLCore::handle NULL_HANDLE    = 0x00000000;
	static const PLCore::handle INVALID_HANDLE = 0xffffffff;
#endif


//[-------------------------------------------------------]
//[ Disable warnings                                      ]
//[-------------------------------------------------------]
PL_WARNING_DISABLE(4355) // 'this' : used in base member initializer list"


#endif // __PLCORE_WINDOWS_H__
