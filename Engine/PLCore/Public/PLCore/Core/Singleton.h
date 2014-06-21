/*********************************************************\
 *  File: Singleton.h                                    *
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


#ifndef __PLCORE_SINGLETON_H__
#define __PLCORE_SINGLETON_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/PLCore.h"


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_PUSH
PL_WARNING_DISABLE(4251) // "'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Singleton template
*
*  @remarks
*    If you want to export a class based on this template by creating e.g. a dynamic library, you have to explicitly
*    instantiate that template class, so that the compiler can import it instead of creating an own class base
*    on the class template. If you don't do this, a call to ::GetInstance() will result in different objects
*    being created for each module, e.g. the library will then get another Singleton-instance than an application using
*    that library. This is of course not desired.
*    To instantiate and export a class, use the export-macros which are defined in each PL-library.
*    Because it seems that according to the ISO/IEC standard 14882 par. 17.4.3 (http://www.kuzbass.ru:8086/docs/isocpp/template.html)
*    instantiation of a template (which is defined in an namespace) inside another namespace is forbidden, use the
*    export macros like this:
*      namespace PLCore {
*        class System;
*      }
*      extern template class Singleton<System>;
*
*    Please note that the example above may not be compatible with each legacy compiler like GCC 4.2.1 used on Mac OS X 10.6.
*    -> The C++11 feature "extern template" (C++11, see e.g. http://www2.research.att.com/~bs/C++0xFAQ.html#extern-templates) can only be used on modern compilers like GCC 4.6
*    -> In PixelLight itself, we can't break legacy compiler support, especially when only the singletons are responsible for the break
*    -> As workaround, singleton implementations adding "GetInstance()" and "HasInstance()" within their interface
*
*  @note
*    - As the class same indicates, this is an implementation of the singleton design pattern
*    - Do only use singletons, if they *really* make sense - not just because it's comfortable to have a global instance, this may lead to a shaky design...
*/
template <class AType> class PLCORE_TMPL Singleton {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the class instance
		*
		*  @return
		*    Pointer to the instance, assumed to be never a null pointer
		*
		*  @note
		*    - Returns a pointer to the singleton instance.
		*      The instance will be created if it has not been created before.
		*    - The returned pointer can be a null pointer if the instance has already
		*      been destroyed, that is on deinitialization of the program.
		*      So be careful when using a singleton during deinitialization
		*      of the program, or better - don't do it :-)
		*/
		static AType *GetInstance();

		/**
		*  @brief
		*    Returns whether or not there's an instance
		*
		*  @return
		*    'true' if there's an instance, else 'false'
		*/
		static bool HasInstance();


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		Singleton();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Singleton();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Destruction guard, deletes a singleton on deinitialization
		*/
		class __Destruction_Guard {
			public:
				__Destruction_Guard()
				{
				}
				~__Destruction_Guard()
				{
					if (Singleton<AType>::__Instance) {
						// Don't use reinterpret_cast in here or hell breaks loose when starting programs
						delete static_cast<Singleton<AType>*>(Singleton<AType>::__Instance);
					}
				}
				inline void init() {}
		};

		// Static data
		static __Destruction_Guard __Guard;	/**< Destroys the object on shutdown */
		static AType *__Instance;			/**< Pointer to the singleton instance, can be a null pointer */
		static bool __Destroyed;			/**< Destruction flag - set only on shutdown */

		// Undefined methods to prevent usage
		Singleton(const Singleton&);
		Singleton& operator =(Singleton);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Core/Singleton.inl"


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP


#endif // __PLCORE_SINGLETON_H__
